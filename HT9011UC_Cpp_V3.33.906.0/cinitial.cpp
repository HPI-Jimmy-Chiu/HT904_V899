// =============================================================================
//  cinitial.cpp  --  Handler / IO subsystem initialisation (name tables,
//  sucker-grid wiring, switch/sensor config load, safe-door + heater-door
//  interlocks)
//
//  Golden ref: HT9011UC_Code_V3.33.906.0_20260618/cinitial.cpp (15,242 lines
//  total).  THIS FILE currently covers ONLY the first slice:
//
//      GA-2-C1  (this wave)          golden :77-3060
//          GetSHCHKPos, InitialSuckerName, InitSucker, InitialSwitchName,
//          InitialSwitch, InitialSensorName, InitialSensor, InitialSafeDoor,
//          InitialHeaterDoor  (+ the golden dead-code tail -- a commented-out
//          InitialSensorLane -- preserved verbatim as a disabled comment,
//          matching golden byte-for-byte through the block boundary).
//
//      GA-2-C2 .. GA-2-C5 (future waves, NOT yet landed) append the remaining
//      ~12,180 golden lines (:3061-15242) in ascending order: CheckMotorValue
//      + the motor-database loaders, InitialMotorParameter, SetMotorSpeed
//      family, ReadTechData/SetWorkParameter/ChangeRunParameter, InitCylinder
//      call site, InitialMachine, Save/LoadMachineRecord, SetTechDataToProd*,
//      ChangeSite, shuttle-thread parameter setup, and the rest of the
//      surface cinitial.h will eventually declare.  EACH FUTURE WAVE MUST
//      extend this banner with its own slice + boundary so the accumulated
//      coverage of this file stays legible from one comment block.
//
//  Translator: AI(W906-GA2-C1) 20260804
//
//  ---------------------------------------------------------------------------
//  GATING PHILOSOPHY
//  ---------------------------------------------------------------------------
//  Per the write-boundary rules for this wave, the following are READ-ONLY
//  (shared headers, or owned by a parallel wave): aHotPlateSubstrate.h,
//  cmydef.h, cmydef.cpp, myswitch.h, mysensor.h, database.h, database.cpp,
//  Config.h, MachineType.h, cprod.h, cprod.cpp, cpublic.h, cpublic.cpp.
//  Where a golden statement in THIS slice reads or writes a member, global,
//  or free function that does not yet exist on the read-only side, the exact
//  golden text is preserved verbatim inside
//      #if 0 // TODO(GA2-C1): blocked by <symbol>@<file> -- golden cinitial.cpp:<range>
//      ... verbatim golden statement(s) ...
//      #endif
//  so the precise completion point survives for whichever future wave lands
//  the missing symbol, and every gate is re-listed in _ga2_c1_report.md.
//  Three recurring blockers account for nearly all of them:
//
//    (1) TMySucker (aHotPlateSubstrate.h) is a documented MINIMAL mirror: it
//        carries Enable / OnAlarmTime / OnDelayTime / sName / SenUsing /
//        iMotNo / iMyRow / iMyCol (+ the W7-L2 vacuum-latch fields) but NOT
//        golden's IO-wiring surface -- SuckerName / SensorName / OnPortName /
//        OffPortName / OnUsing / OffUsing / ISABase / OnISABase / OffISABase /
//        SenISABase / OnRing / OffRing / SenRing / OnIP / OffIP / SenIP /
//        OnPort / OffPort / SenPort / OnBit / OffBit / SenBit / OnType /
//        OffType / SenType / OffAlarmTime / OffDelayTime / OnEnable /
//        OffEnable.  Every golden statement writing one of THOSE fields is
//        gated (this is nearly all of InitSucker's per-nozzle IO-config body,
//        and most of InitialSuckerName's ".SuckerName=" assignments).
//    (2) TTable / DataModule1 (BDE database access) do not exist anywhere in
//        the ported tree yet -- database.h's own banner documents this as
//        still gated pending the BDE wave.  The switch.db / sensor.db /
//        sucker*.db else-branches (taken only when IO_CARD_TYPE is NEITHER
//        NewIO_MN200 nor PCI_P64C64) are gated wholesale in InitSucker /
//        InitialSwitch / InitialSensor.
//    (3) A few small free functions / globals golden calls from this slice
//        are not ported yet: CopyKitSuck (MyKitSuck.cpp -- whole-grid clone;
//        NOT the same as the already-ported CopyInitSuck), the
//        FTestSuckBackup/BTestSuckBackup/InArmSuckBackup backup grids and the
//        CheckKitSuck grid (no extern anywhere in the tree), and
//        SetIOTableByNUEC1 (EtherCAT/MyNUEC1.cpp) -- each gated at its call
//        site.
//
//  Everything else is FAITHFUL, ACTIVE translation:
//    - GetSHCHKPos: fully self-contained geometry, zero gating.
//    - InitialSwitchName / InitialSensorName: pure Name-table literal lists --
//      every SW[]/Sen[] index referenced already exists in cmydef.h.  The 9
//      Sen[] lines golden itself left commented out (SnIndexY1CHK etc.) are
//      preserved as comments, not silently re-enabled.
//    - InitialSwitch / InitialSensor: the IO_CARD_TYPE==NewIO_MN200 ||
//      PCI_P64C64 (IO_Table.csv-driven, via HSys.mapIOTable/IOTable) path is
//      fully active -- SW[]/Sen[] already carry every field it touches.
//    - InitSucker: the pSuck scaffolding list, the FTestSuck/BTestSuck
//      sName/iMyRow/iMyCol loop, the Type_HT9045/_12Site Enable=false
//      disabling, and the full InArmSuck/OutArmSuck/OutArm2Suck iMotNo
//      wiring (incl. the ep16Picker/eptUseMot branch and its golden
//      commented-out HT1032 legacy-motor alternative, preserved as a comment)
//      are active.
//    - InitialSafeDoor / InitialHeaterDoor: the safety-critical door
//      interlock pair -- fully active, zero gating, translated with the same
//      "do not modify carelessly" banners golden carries.
//
//  Encoding: UTF-8, bare LF, trailing newline.  Chinese comments decoded from
//  the golden cp950 source; no U+FFFD.
// =============================================================================
#include "MachineDefine.h"
#include "cinitial.h"

#include "MachineType.h"
#include "cmydef.h"
#include "cprod.h"              // TestIF_File, Temperature (SYSTEM_TEST_IF / SYSTEM_TEMPERATURE)
#include "cUnitConvert.h"       // iUnitMultiply100 (GetSHCHKPos)
#include "CosFunction.h"        // CosFunction.b2x4SupportCenterPitch
#include "Config.h"             // IniConfig.bP35TrayArm (InitialSafeDoor)
#include "database.h"           // HSys.mapIOTable / HSys.IOTable[] (InitialSwitch/InitialSensor active path)
#include "mysensor.h"           // Sen[] / SenBackUp[] / CopySensor
#include "myswitch.h"           // SW[] / SWBackup[] / CopySwitch (also forward-declares InitialSwitch())
#include "aHotPlateSubstrate.h" // TMyKitSuck grids (InArmSuck/OutArmSuck/FTestSuck/...), TList shim
#include "acarry_shims.h"       // OutSht3Kit (HT-9046AU sort kit)
#include "MyLaneIo.h"           // MyLaneIO.SetUseIP
#include "MyPLC/MyPLC_IO_Modbus.h" // bPLCIO[][] (InitialSensor PLC-safety branch)

//------------------------------------------------------------------------------
//  InitSucker() scaffolding globals.
//  golden mykitsuck.h declares `extern TList *pSuck;` / `extern TMySucker
//  *pTempSuck;` (consumed from several modules).  Grepping the ENTIRE ported
//  tree (golden and already-landed .cpp alike) turns up zero consumers other
//  than this function, so TU-local `static` is the minimal-scope-correct
//  choice for GA-2-C1; promote to `extern` (declared in a shared header) the
//  day a second consumer needs them.
//------------------------------------------------------------------------------
static TList     *pSuck    = NULL;
static TMySucker *pTempSuck = NULL;
//------------------------------------------------------------------------------
//  GetSHCHKPos  (golden cinitial.cpp:77-255)
//  Self-contained site-position geometry helper.  Zero gating -- every symbol
//  it touches (TestIF_File, CosFunction, the eTestMode enum constants) is
//  already active in the ported tree.
//------------------------------------------------------------------------------
int GetSHCHKPos(int iSite, int iCenterBase)
{
    int Pos=0;
    int iSiteXPitch     = iUnitMultiply100(TestIF_File.dSiteXPitch);
    int iSiteShiftXPitch= iUnitMultiply100(TestIF_File.dSiteXCenterPitch);

    if(TestIF_File.iTestMode==SingleSite ||                                     //Steven 20160919 : for 1x1偏心要跑Z向Latch
       TestIF_File.iTestMode==DualSite2x1)                                      //jou 980827 start : use 14x14 ; Pitch X=40mm Y=60mm ; site 3 is tech point
    {
        Pos=iCenterBase;
    }
    else if(CUSTOMER_CODE==CC_TSMC_TAINAN && TestIF_File.bQualSite2X2Shift)     //wei 20160226 TSMC X Shift
    {
        iSiteShiftXPitch=iUnitMultiply100(TestIF_File.dShiftXPitch);
        switch(iSite)
        {
            case 0:
                Pos=iCenterBase+iSiteXPitch/2-iSiteShiftXPitch;                 //Line 1    //wei 20160922 +  --->  -
                break;
            case 1:
                Pos=iCenterBase-iSiteXPitch/2-iSiteShiftXPitch;                 //Line 2    //wei 20160922 +  --->  -
                break;
        }
    }
    else if(TestIF_File.iTestMode==DualSite     ||                              //1x2 & 2X2 & 2x1 BusyShuttle
            TestIF_File.iTestMode==QualSite2X2  ||
            TestIF_File.iTestMode==QualSite2X2N)                                //Frank 20200520 2X2NN Mode
    {
        switch(iSite)
        {
            case 0:
                Pos=iCenterBase+iSiteXPitch/2;                                  //Line 1
                break;
            case 1:
                Pos=iCenterBase-iSiteXPitch/2;                                  //Line 2
                break;
        }
    }
    else if(TestIF_File.iTestMode==TriSite1X3 ||
            TestIF_File.iTestMode==_6Site2X3  ||                                //ChungHung 20140115 add for 2x3_6
            TestIF_File.iTestMode==_6Site2X3N)                                  //Steven 20220425 : 2X3NN Mode
    {
        switch(iSite)
        {
            case 0:
                Pos=iCenterBase+iSiteXPitch;
                break;
            case 1:
                Pos=iCenterBase;
                break;
            case 2:
                Pos=iCenterBase-iSiteXPitch;
                break;
        }
    }
    else if(TestIF_File.iTestMode==QualSite1X4  ||                              //1x4 & 2X4
            TestIF_File.iTestMode==_8Site1X4    ||                              //ChungHung 20150528 add for 海思 _8Site1x4
            TestIF_File.iTestMode==_8Site2X4    ||
            TestIF_File.iTestMode==_8Site2X4N   ||                              //Wei 20231211 : 2X4NN Mode
            TestIF_File.iTestMode==_16Site4X4)                                  //Sam 20190226 : 16Site4X4
    {
        if(CosFunction.b2x4SupportCenterPitch &&                                //Richard 20220929 :Add for UTAC
           TestIF_File.bEnableUseXCenterPitch==true)
        {
            switch(iSite)
            {
                case 0:
                    Pos=iCenterBase+iSiteShiftXPitch/2+iSiteXPitch;             //Line 1
                    break;
                case 1:
                    Pos=iCenterBase+iSiteShiftXPitch/2;                         //Line 2
                    break;
                case 2:
                    Pos=iCenterBase-iSiteShiftXPitch/2;                         //Line 3
                    break;
                case 3:
                    Pos=iCenterBase-iSiteShiftXPitch/2-iSiteXPitch;             //Line 4
                    break;
            }
        }
        else
        {
            switch(iSite)
            {
                case 0:
                    Pos=iCenterBase+iSiteXPitch*1.5;                            //Line 1
                    break;
                case 1:
                    Pos=iCenterBase+iSiteXPitch/2;                              //Line 2
                    break;
                case 2:
                    Pos=iCenterBase-iSiteXPitch/2;                              //Line 3
                    break;
                case 3:
                    Pos=iCenterBase-iSiteXPitch*1.5;                            //Line 4
                    break;
            }
        }
    }
    else if(TestIF_File.iTestMode==_10Site2X5)                                  //2X5         //wei 20190614 10 site
    {
        switch(iSite)
        {
            case 0:
                Pos=iCenterBase+iSiteXPitch*2;
                break;
            case 1:
                Pos=iCenterBase+iSiteXPitch;
                break;
            case 2:
                Pos=iCenterBase;
                break;
            case 3:
                Pos=iCenterBase-iSiteXPitch;
                break;
            case 4:
                Pos=iCenterBase-iSiteXPitch*2;
                break;
        }
    }
    else if(TestIF_File.iTestMode==_12Site2X6)                                  //2X6        //Steven 20130702 : _12Site2X6
    {
        switch(iSite)
        {
            case 0:
                Pos=iCenterBase+iSiteXPitch*2.5;                                //Line 1
                break;
            case 1:
                Pos=iCenterBase+iSiteXPitch*1.5;                                //Line 2
                break;
            case 2:
                Pos=iCenterBase+iSiteXPitch/2;                                  //Line 3
                break;
            case 3:
                Pos=iCenterBase-iSiteXPitch/2;                                  //Line 4
                break;
            case 4:
                Pos=iCenterBase-iSiteXPitch*1.5;                                //Line 5
                break;
            case 5:
                Pos=iCenterBase-iSiteXPitch*2.5;                                //Line 6
                break;
        }
    }
    else if(TestIF_File.iTestMode==_16Site2X8   ||                              //2X8
            TestIF_File.iTestMode==_32Site4X8N  ||                              //Steven 20140813 : 32Site
            TestIF_File.iTestMode==_32Site4X8M)
    {
        switch(iSite)
        {
            case 0:
                Pos=iCenterBase+iSiteXPitch*3.5;                                //Line 1
                break;
            case 1:
                Pos=iCenterBase+iSiteXPitch*2.5;                                //Line 2
                break;
            case 2:
                Pos=iCenterBase+iSiteXPitch*1.5;                                //Line 3
                break;
            case 3:
                Pos=iCenterBase+iSiteXPitch/2;                                  //Line 4
                break;
            case 4:
                Pos=iCenterBase-iSiteXPitch/2;                                  //Line 5
                break;
            case 5:
                Pos=iCenterBase-iSiteXPitch*1.5;                                //Line 6
                break;
            case 6:
                Pos=iCenterBase-iSiteXPitch*2.5;                                //Line 7
                break;
            case 7:
                Pos=iCenterBase-iSiteXPitch*3.5;                                //Line 8
                break;
        }
    }

    return Pos;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//  InitialSuckerName  (golden cinitial.cpp:257-354)
//  Assigns two name fields per nozzle: the long `.SuckerName` (e.g.
//  "FTestSuckAA") and the short `.sName` (e.g. "Aa").  `.sName` exists on the
//  ported TMySucker MINIMAL mirror; `.SuckerName` does not (see file banner,
//  blocker (1)) -- every `.SuckerName=` assignment below is gated at its
//  exact golden line range, `.sName=` assignments are active.
//------------------------------------------------------------------------------
void InitialSuckerName()
{
    AnsiString str1, str2, strOn1, strOn2, strOff1, strOff2;
    for(int i=0; i<MAX_Index_Row; i++)                                          //Eliot 2009_12_27 2->MAX_Index_Row
    {
        for(int j=0; j<NEW_MAX_Index_Col; j++)                                  //Eliot 2009_12_27 4->MAX_Index_Col
        {
            str1.sprintf("FTestSuck%c%c", 'A'+i, 'A'+j);
            str2.sprintf("BTestSuck%c%c", 'A'+i, 'A'+j);

#if 0 // TODO(GA2-C1): blocked by TMySucker::SuckerName@aHotPlateSubstrate.h (not in MINIMAL mirror)
      //             + FTestSuckBackup/BTestSuckBackup grids not declared anywhere -- golden cinitial.cpp:267-270
            FTestSuck.Suck[i][j].SuckerName         =str1;
            BTestSuck.Suck[i][j].SuckerName         =str2;
            FTestSuckBackup.Suck[i][j].SuckerName   =str1;
            BTestSuckBackup.Suck[i][j].SuckerName   =str2;
#endif
        }
    }

    for(int i=0; i<MAX_SOCKET_ROW; i++)
    {
        for(int j=0; j<MAX_SOCKET_COL; j++)
        {
            str1.sprintf("TestSocket%c%c", 'A'+i, 'A'+j);
#if 0 // TODO(GA2-C1): blocked by TMySucker::SuckerName@aHotPlateSubstrate.h -- golden cinitial.cpp:279
            TestSocket.Suck[i][j].SuckerName=str1;
#endif

            str1.sprintf("%c%c", 'A'+i, 'a'+j);
            TestSocket.Suck[i][j].sName=str1;
        }
    }

    for(int i=0; i<FLCarryKit.iMaxRow; i++)
    {
        for(int j=0; j<FLCarryKit.iMaxCol; j++)
        {
#if 0 // TODO(GA2-C1): blocked by TMySucker::SuckerName@aHotPlateSubstrate.h -- golden cinitial.cpp:290-299
            str1.sprintf("FLCarryKit%c%c", 'A'+i, 'A'+j);
            FLCarryKit.Suck[i][j].SuckerName=str1;
            str1.sprintf("FRCarryKit%c%c", 'A'+i, 'A'+j);
            FRCarryKit.Suck[i][j].SuckerName=str1;
            str1.sprintf("BLCarryKit%c%c", 'A'+i, 'A'+j);
            BLCarryKit.Suck[i][j].SuckerName=str1;
            str1.sprintf("BRCarryKit%c%c", 'A'+i, 'A'+j);
            BRCarryKit.Suck[i][j].SuckerName=str1;
            str1.sprintf("OutSht3Kit%c%c", 'A'+i, 'A'+j);                       //Steven 20240822 : For HT-9046AU
            OutSht3Kit.Suck[i][j].SuckerName=str1;
#endif

            str1.sprintf("%c%c", 'A'+i, 'a'+j);
            FLCarryKit.Suck[i][j].sName=str1;
            FRCarryKit.Suck[i][j].sName=str1;
            BLCarryKit.Suck[i][j].sName=str1;
            BRCarryKit.Suck[i][j].sName=str1;
            OutSht3Kit.Suck[i][j].sName=str1;                                   //Steven 20240822 : For HT-9046AU
        }
    }

    if(USE_PICKER_COUNT!=ep16Picker)
    {
        for(int j=0; j<4; j++)
        {
            for(int i=0; i<2; i++)
            {
#if 0 // TODO(GA2-C1): blocked by TMySucker::SuckerName@aHotPlateSubstrate.h + InArmSuckBackup/OutArmSuckBackup.SuckerName -- golden cinitial.cpp:316-319
                InArmSuck.Suck[i][j].SuckerName         ="InArmSuck"+AnsiString(char ('A'+(i+2*j)));
                OutArmSuck.Suck[i][j].SuckerName        ="OutArmSuck"+AnsiString(char ('A'+(i+2*j)));
                InArmSuckBackup.Suck[i][j].SuckerName   =InArmSuck.Suck[i][j].SuckerName;
                OutArmSuckBackup.Suck[i][j].SuckerName  =OutArmSuck.Suck[i][j].SuckerName;
#endif
            }
        }
    }
    else if(USE_PICKER_COUNT==ep16Picker)
    {
        for(int i=0; i<2; i++)                                                  //Eliot 2009_12_27 2->MAX_Index_Row
        {
            for(int j=0; j<8; j++)                                              //Eliot 2009_12_27 4->MAX_Index_Col
            {
                str1.sprintf("InArmSuck%c%c", 'A'+i, 'A'+j);
                str2.sprintf("OutArmSuck%c%c", 'A'+i, 'A'+j);

#if 0 // TODO(GA2-C1): blocked by TMySucker::SuckerName@aHotPlateSubstrate.h + InArmSuckBackup grid not declared -- golden cinitial.cpp:332-335
                InArmSuck.Suck[i][j].SuckerName         =str1;
                OutArmSuck.Suck[i][j].SuckerName        =str2;
                InArmSuckBackup.Suck[i][j].SuckerName   =str1;
                OutArmSuckBackup.Suck[i][j].SuckerName  =str2;
#endif
            }
        }
    }

    for(int i=0; i<MAX_OutArm3_Row; i++)                                        //Steven 20240822 : For HT-9046AU
    {
        for(int j=0; j<MAX_OutArm3_Col; j++)
        {
            str1.sprintf("OutArm2Suck%c%c", 'A'+i, 'A'+j);
            str2.sprintf("%c%c", 'A'+i, 'a'+j);
#if 0 // TODO(GA2-C1): blocked by TMySucker::SuckerName@aHotPlateSubstrate.h -- golden cinitial.cpp:346
            OutArm2Suck.Suck[i][j].SuckerName=str1;
#endif
            OutArm2Suck.Suck[i][j].sName=str2;
        }
    }

#if 0 // TODO(GA2-C1): blocked by TMySucker::SuckerName@aHotPlateSubstrate.h (CatchTraySuck) + CheckKitSuck grid not declared anywhere -- golden cinitial.cpp:351-353
    CatchTraySuck.Suck[0][0].SuckerName="CatchSuck";
    CheckKitSuck.Suck[0][0].SuckerName ="CheckKitSuck_1";                       //20111130  Dell
    CheckKitSuck.Suck[0][1].SuckerName ="CheckKitSuck_2";                       //20111130  Dell
#endif
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//  InitSucker  (golden cinitial.cpp:356-1000)
//  Builds the pSuck scan list, loads per-nozzle IO wiring from either
//  IO_Table.csv (via HSys.mapIOTable, card type 2/3) or a BDE .db (else
//  branch), then wires nozzle names/rows/cols and per-arm Z-motor indices.
//
//  The per-nozzle IO-wiring body (both branches) writes ~25 TMySucker fields
//  that do not exist on the ported MINIMAL mirror (see file banner, blocker
//  (1)) and the else branch additionally needs TTable/DataModule1 (blocker
//  (2)) -- gated wholesale, golden cinitial.cpp:401-774.  The scan-list
//  construction, the sName/iMyRow/iMyCol/Enable wiring, and the full
//  iMotNo Z-axis wiring (golden :823-996) need none of those and are FAITHFUL
//  active translation.
//------------------------------------------------------------------------------
void InitSucker()
{
    if(pSuck==NULL)
        pSuck=new TList;                                                        //Steven 20230701 : 吸嘴改用List方式, 避免例外
    else
        pSuck->Clear();                                                         //RogerYang 20250328 avoid reload ++

    AnsiString S;

    InitialSuckerName();                                                        // AI(W906-GA2-C1-fix) 20260804: HIGH finding H-1 -- golden :369's only call
                                                                                // site was dropped when the adjacent declaration block moved into the gated
                                                                                // IO region; without it TestSocket/FL/FR/BL/BRCarryKit/OutSht3Kit .sName
                                                                                // are never set (empirically proven by the independent review).

    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            pSuck->Add(&InArmSuck.Suck[i][j]);
            pSuck->Add(&OutArmSuck.Suck[i][j]);
        }
    }

    for(int i=0; i<MAX_Index_Row; i++)                                          //Steven 20230808 : 修正Suck數量錯誤, 影響8site以上模式
    {
        for(int j=0; j<NEW_MAX_Index_Col; j++)
        {
            pSuck->Add(&FTestSuck.Suck[i][j]);
            pSuck->Add(&BTestSuck.Suck[i][j]);
        }
    }

    for(int i=0; i<MAX_OutArm3_Row; i++)                                        //Steven 20240822 : For HT-9046AU
    {
        for(int j=0; j<MAX_OutArm3_Col; j++)
        {
            pSuck->Add(&OutArm2Suck.Suck[i][j]);
        }
    }

    pSuck->Add(&CatchTraySuck.Suck[0][0]);
#if 0 // TODO(GA2-C1): blocked by CheckKitSuck@aHotPlateSubstrate.h (grid extern not declared anywhere) -- golden cinitial.cpp:398-399
    pSuck->Add(&CheckKitSuck.Suck[0][0]);
    pSuck->Add(&CheckKitSuck.Suck[0][1]);
#endif

#if 0 // TODO(GA2-C1): blocked by TMySucker IO-wiring fields (SensorName/OnPortName/OffPortName/OnUsing/OffUsing/
      //   ISABase/OnISABase/OffISABase/SenISABase/OnRing/OffRing/SenRing/OnIP/OffIP/SenIP/OnPort/OffPort/SenPort/
      //   OnBit/OffBit/SenBit/OnType/OffType/SenType/OffAlarmTime/OffDelayTime/OnEnable/OffEnable -- see file
      //   banner blocker (1)) @aHotPlateSubstrate.h + TTable/DataModule1->SuckerTable (blocker (2)) @database.h
      //   -- golden cinitial.cpp:401-774
    int iSu=0, iSuOn=0, iSuOff=0;
    bool flag;
    bool bHasSuckSen, bHasSuckOn, bHasSuckOff;                                  //Stevem 20210625 : 修正新的CSV IO表讀檔方式
    AnsiString sDBDir;
    TTable *T;

    if(IO_CARD_TYPE==NewIO_MN200 ||
       IO_CARD_TYPE==PCI_P64C64)                                                //Steven 20231218 HT7080B
    {
        for(int i=0; i<pSuck->Count; i++)                                       //Steven 20230701 : 吸嘴改用List方式, 避免例外
        {
            bHasSuckSen=false;
            bHasSuckOn =false;
            bHasSuckOff=false;
            pTempSuck=(TMySucker*)pSuck->Items[i];
            pTempSuck->SensorName   =pTempSuck->SuckerName;
            pTempSuck->OnPortName   =pTempSuck->SensorName+AnsiString("_On");
            pTempSuck->OffPortName  =pTempSuck->SensorName+AnsiString("_Off");

            pTempSuck->Enable=false;
            if(pTempSuck->SuckerName=="")
                continue;

            HSys.mapIOTableIter=HSys.mapIOTable.find(pTempSuck->SuckerName);
            if(HSys.mapIOTableIter!=HSys.mapIOTable.end())
            {
                bHasSuckSen=true;
                iSu=atoi(HSys.mapIOTable[pTempSuck->SuckerName].c_str());
            }
            else
            {
                S.sprintf("Can not find sucker sensor %s", pTempSuck->SuckerName.c_str());
//                continue;
            }

            HSys.mapIOTableIter=HSys.mapIOTable.find(pTempSuck->OnPortName);
            if(HSys.mapIOTableIter!=HSys.mapIOTable.end())
            {
                bHasSuckOn=true;
                iSuOn=atoi(HSys.mapIOTable[pTempSuck->OnPortName].c_str());
            }
            else
            {
                S.sprintf("Can not find sucker on %s", pTempSuck->OnPortName.c_str());
//                continue;
            }

            HSys.mapIOTableIter=HSys.mapIOTable.find(pTempSuck->OffPortName);
            if(HSys.mapIOTableIter!=HSys.mapIOTable.end())
            {
                bHasSuckOff=true;
                iSuOff=atoi(HSys.mapIOTable[pTempSuck->OffPortName].c_str());
            }
            else
            {
                S.sprintf("Can not find sucker off %s", pTempSuck->OffPortName.c_str());
//                continue;
            }

            if(bHasSuckOn)
            {
                pTempSuck->OnUsing     =(HSys.IOTable[iSuOn]->iPort==-1)?AnsiString(""):AnsiString(HSys.IOTable[iSuOn]->iPort);
                pTempSuck->OnISABase   =HSys.IOTable[iSuOn]->iISABase;
                pTempSuck->OnRing      =HSys.IOTable[iSuOn]->iLane;
                pTempSuck->OnIP        =HSys.IOTable[iSuOn]->iIP;
                pTempSuck->OnPort      =HSys.IOTable[iSuOn]->iPort;
                pTempSuck->OnBit       =HSys.IOTable[iSuOn]->iBit;
                pTempSuck->OnType      =HSys.IOTable[iSuOn]->iInType;
                pTempSuck->ISABase     =pTempSuck->OnISABase;
                pTempSuck->OnEnable    =(pTempSuck->OnUsing!="");
            }

            if(bHasSuckOff)
            {
                pTempSuck->OffUsing    =(HSys.IOTable[iSuOff]->iPort==-1)?AnsiString(""):AnsiString(HSys.IOTable[iSuOff]->iPort);
                pTempSuck->OffISABase  =HSys.IOTable[iSuOff]->iISABase;
                pTempSuck->OffRing     =HSys.IOTable[iSuOff]->iLane;
                pTempSuck->OffIP       =HSys.IOTable[iSuOff]->iIP;
                pTempSuck->OffPort     =HSys.IOTable[iSuOff]->iPort;
                pTempSuck->OffBit      =HSys.IOTable[iSuOff]->iBit;
                pTempSuck->OffType     =HSys.IOTable[iSuOff]->iInType;
                pTempSuck->ISABase     =pTempSuck->OffISABase;
                pTempSuck->OffEnable   =(pTempSuck->OffUsing!="");
            }

            if(bHasSuckSen)
            {
                pTempSuck->SenUsing    =(HSys.IOTable[iSu]->iPort==-1)?AnsiString(""):AnsiString(HSys.IOTable[iSu]->iPort);
                pTempSuck->SenISABase  =HSys.IOTable[iSu]->iISABase;
                pTempSuck->SenRing     =HSys.IOTable[iSu]->iLane;
                pTempSuck->SenIP       =HSys.IOTable[iSu]->iIP;
                pTempSuck->SenPort     =HSys.IOTable[iSu]->iPort;
                pTempSuck->SenBit      =HSys.IOTable[iSu]->iBit;
                pTempSuck->SenType     =HSys.IOTable[iSu]->iInType;

                pTempSuck->OnAlarmTime =HSys.IOTable[iSu]->iOnAlarmTime*10;
                pTempSuck->OffAlarmTime=HSys.IOTable[iSu]->iOffAlarmTime*10;
                pTempSuck->OnDelayTime =HSys.IOTable[iSu]->iOnDelayTime*10;
                pTempSuck->OffDelayTime=HSys.IOTable[iSu]->iOffDelayTime*10;
                pTempSuck->ISABase     =pTempSuck->SenISABase;
            }

            #ifdef SOFT_SIMULTE
                pTempSuck->Enable       =false;
                pTempSuck->OnEnable     =false;
                pTempSuck->OffEnable    =false;
                pTempSuck->OnDelayTime =0;
                pTempSuck->OffDelayTime=0;
            #else
            if(pTempSuck->SenUsing=="")                                         //Steven 20230723 : 修正Suck Enable判斷方式
                pTempSuck->Enable=false;
            else if(bHasSuckSen && HSys.IOTable[iSu]->iEnable==1)
                pTempSuck->Enable=true;
            else
                pTempSuck->Enable=false;

            if(pTempSuck->Enable==false)
            {
                pTempSuck->OnEnable =false;
                pTempSuck->OffEnable=false;
            }
            #endif

            pTempSuck->fAlarm    =false;

            if(INDEX_SUCKER_TYPE==1)                                            //負壓
            {
                if(pTempSuck->SensorName.Pos("TestSuck")>0)
                {
                    pTempSuck->OnAlarmTime=pTempSuck->OnAlarmTime/10;           //Steven 20160224 : 因為避免機台INDEX吸不起IC
                    if(pTempSuck->OnAlarmTime>50)
                        pTempSuck->OnAlarmTime=50;
                }
            }

            #ifndef SOFT_SIMULTE
            if(pTempSuck->OnEnable)
            #endif
            {
                MyLaneIO.SetUseIP(pTempSuck->OnRing, pTempSuck->OnIP, pTempSuck->OnISABase, pTempSuck->SensorName);             //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
            }

            #ifndef SOFT_SIMULTE
            if(pTempSuck->OffEnable)
            #endif
            {
                MyLaneIO.SetUseIP(pTempSuck->OffRing, pTempSuck->OffIP, pTempSuck->OffISABase, pTempSuck->SensorName);          //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
            }

            #ifndef SOFT_SIMULTE
            if(pTempSuck->Enable)
            #endif
            {
                MyLaneIO.SetUseIP(pTempSuck->SenRing, pTempSuck->SenIP, pTempSuck->SenISABase, pTempSuck->SensorName, false);   //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
            }
        }
    }
    else
    {
        if(USE_IN_Y_IS_AUTO_PITCH==true || USE_OUT_Y_IS_AUTO_PITCH)                                              //Steven 20170424 (wei) : new XY變距  //JerryYang 20251218 : IN/OUT ARM支援不同模組
        {                                                                       //Steven 20140828 : For XY-Pitch
            sDBDir=CurrentDir+"\\system\\sucker_YPitch.db";                     //JerryYang 20150903 檢查Database檔是否存在
            if(FileExists(sDBDir))
            {
                DataModule1->SuckerTable->TableName=sDBDir;
            }
            else
            {
                 ShowMyMessage("sucker_YPitch.db does not exist");
                 return;
            }
        }
        else if(MachineTypeChoice==Type_HT9046 ||
                MachineTypeChoice==Type_HT9046_LS ||
                MachineTypeChoice==Type_HT1032)
        {
            sDBDir=CurrentDir+"\\system\\sucker_9046.db";
            if(FileExists(sDBDir))
            {
                DataModule1->SuckerTable->TableName=sDBDir;
            }
            else
            {
                ShowMyMessage("sucker_9046.db does not exist");
                return;
            }
        }
        else if(USE_46_SUCKER_DB==1)                                            //Steven 20131008 : HT9045使用46配氣
        {
            sDBDir=CurrentDir+"\\system\\sucker_9046.db";
            if(FileExists(sDBDir))
            {
                DataModule1->SuckerTable->TableName=sDBDir;
            }
            else
            {
                ShowMyMessage("sucker_9046.db does not exist");
                return;
            }
        }
        else if(MachineTypeChoice==Type_HT9045_12Site)                          //ChungHung 20130507 add HT9045 updata for 12site 517
        {
            sDBDir=CurrentDir+"\\system\\sucker.db";
            if(FileExists(sDBDir))
            {
                DataModule1->SuckerTable->TableName=sDBDir;
            }
            else
            {
                ShowMyMessage("sucker.db does not exist");
                return;
            }
        }
        else
        {
            sDBDir=CurrentDir+"\\system\\sucker.db";
            if(FileExists(sDBDir))
            {
                DataModule1->SuckerTable->TableName=sDBDir;
            }
            else
            {
                ShowMyMessage("sucker.db does not exist");
                return;
            }
        }

        T=DataModule1->SuckerTable;
        T->Open();
        for(int i=0; i<pSuck->Count; i++)
        {
            flag=false;
            T->First();
            while(!T->Eof)
            {
                S=T->FieldByName("SuckerName")->AsString;
                pTempSuck=(TMySucker*)pSuck->Items[i];
                if(pTempSuck!=NULL && S==pTempSuck->SuckerName)
                {
                    flag=true;
                    break;
                }
                T->Next();
            }

            if(flag==false)
                continue;

            pTempSuck->SensorName   =T->FieldByName("SuckerName")->AsString;
            pTempSuck->OnPortName   =pTempSuck->SensorName+AnsiString("_On");
            pTempSuck->OffPortName  =pTempSuck->SensorName+AnsiString("_Off");
            if(T->FindField("ISABase")!=NULL)
                pTempSuck->ISABase  =atoi(T->FieldByName("ISABase")->AsString.c_str());         //Nickliu 20230310 add Suck ISABase Type
            else
                pTempSuck->ISABase  =eMotionNet;

            pTempSuck->OnISABase    =pTempSuck->ISABase;
            pTempSuck->OffISABase   =pTempSuck->ISABase;
            pTempSuck->SenISABase   =pTempSuck->ISABase;

            #ifdef PCI132
            pTempSuck->OnIP         =atoi(T->FieldByName("OnIP"         )->AsString.c_str());
            if(pTempSuck->OnISABase==eISABase ||                                //Nickliu 20230310 add Suck ISABase Type
               pTempSuck->OnISABase==ePCI1735U ||
               pTempSuck->OnISABase==ePLCbase)
            {
                pTempSuck->OnUsing  =T->FieldByName("OnPort")->AsString;
                pTempSuck->OnPort   =HexStrToInt(T->FieldByName("OnPort")->AsString.c_str());
            }
            else
            {
                pTempSuck->OnUsing  =T->FieldByName("OnLane")->AsString;
                if(pTempSuck->OnUsing!="")
                    pTempSuck->OnRing   =atoi(pTempSuck->OnUsing.c_str());
                pTempSuck->OnPort   =atoi(T->FieldByName("OnPort"       )->AsString.c_str());
            }

            pTempSuck->OnBit        =atoi(T->FieldByName("OnBit"        )->AsString.c_str());
            pTempSuck->OffIP        =atoi(T->FieldByName("OffIP"        )->AsString.c_str());
            if(pTempSuck->OffISABase==eISABase ||                               //Nickliu 20230310 add Suck ISABase Type
               pTempSuck->OffISABase==ePCI1735U ||
               pTempSuck->OffISABase==ePLCbase)
            {
                pTempSuck->OffUsing =T->FieldByName("OffPort"      )->AsString;
                pTempSuck->OffPort  =HexStrToInt(T->FieldByName("OffPort")->AsString.c_str());
            }
            else
            {
                pTempSuck->OffUsing     = T->FieldByName("OffLane"      )->AsString;
                if(pTempSuck->OffUsing!="")
                    pTempSuck->OffRing  =atoi(pTempSuck->OffUsing.c_str());
                pTempSuck->OffPort  =atoi(T->FieldByName("OffPort"      )->AsString.c_str());
            }

            pTempSuck->OffBit       =atoi(T->FieldByName("OffBit"       )->AsString.c_str());

            pTempSuck->SenIP        =atoi(T->FieldByName("SenIP"        )->AsString.c_str());
            if(pTempSuck->OffISABase==eISABase ||                               //Nickliu 20230310 add Suck ISABase Type
               pTempSuck->OffISABase==ePCI1735U ||
               pTempSuck->OffISABase==ePLCbase)
            {
                pTempSuck->SenUsing =T->FieldByName("SenPort"           )->AsString;
                pTempSuck->SenPort  =HexStrToInt(T->FieldByName("SenPort")->AsString.c_str());
            }
            else
            {
                pTempSuck->SenUsing =T->FieldByName("SenLane"           )->AsString;
                if(pTempSuck->SenUsing!="")
                    pTempSuck->SenRing  =atoi(T->FieldByName("SenLane"  )->AsString.c_str());
                pTempSuck->SenPort  =atoi(T->FieldByName("SenPort"      )->AsString.c_str());
            }
            pTempSuck->SenBit       =atoi(T->FieldByName("SenBit"       )->AsString.c_str());
            #else
            pTempSuck->OnIOPort     =HexStrToInt(T->FieldByName("OnIOPort"     )->AsString.c_str());
            pTempSuck->OnIOBit      =atoi(T->FieldByName("OnIOBit"      )->AsString.c_str());
            pTempSuck->OffIOPort    =HexStrToInt(T->FieldByName("OffIOPort"    )->AsString.c_str());
            pTempSuck->OffIOBit     =atoi(T->FieldByName("OffIOBit"     )->AsString.c_str());
            pTempSuck->SensorPort   =HexStrToInt(T->FieldByName("SensorPort"   )->AsString.c_str());
            pTempSuck->SensorBit    =atoi(T->FieldByName("SensorBit"    )->AsString.c_str());
            #endif
            pTempSuck->OnType       =atoi(T->FieldByName("OnType"      )->AsString.c_str());
            pTempSuck->OffType      =atoi(T->FieldByName("OffType"     )->AsString.c_str());
            pTempSuck->SenType      =atoi(T->FieldByName("SensorType"  )->AsString.c_str());
            pTempSuck->OnAlarmTime  =atoi(T->FieldByName("OnAlarmTime" )->AsString.c_str())*10;
            pTempSuck->OffAlarmTime =atoi(T->FieldByName("OffAlarmTime")->AsString.c_str())*10;
            pTempSuck->OnDelayTime  =atoi(T->FieldByName("OnDelayTime" )->AsString.c_str())*10;
            pTempSuck->OffDelayTime =atoi(T->FieldByName("OffDelayTime")->AsString.c_str())*10;
            pTempSuck->fAlarm       =false;

            #ifdef SOFT_SIMULTE
                pTempSuck->Enable       =false;
                pTempSuck->OnEnable     =false;
                pTempSuck->OffEnable    =false;
                pTempSuck->OnDelayTime  =0;
                pTempSuck->OffDelayTime =0;
            #else
            if(pTempSuck->SenUsing=="" ||                                       //Steven 20230723 : 修正Suck Enable判斷方式
               atoi(T->FieldByName("Enable")->AsString.c_str())==0)
                pTempSuck->Enable=false;
            else
                pTempSuck->Enable=true;

            pTempSuck->OnEnable  =pTempSuck->Enable;
            pTempSuck->OffEnable =pTempSuck->Enable;
            #endif

            #ifndef SOFT_SIMULTE
            if(pTempSuck->OnEnable)
            #endif
            {
                MyLaneIO.SetUseIP(pTempSuck->OnRing, pTempSuck->OnIP, pTempSuck->OnISABase, pTempSuck->SensorName);             //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
            }

            #ifndef SOFT_SIMULTE
            if(pTempSuck->OffEnable)
            #endif
            {
                MyLaneIO.SetUseIP(pTempSuck->OffRing, pTempSuck->OffIP, pTempSuck->OffISABase, pTempSuck->SensorName);          //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
            }

            #ifndef SOFT_SIMULTE
            if(pTempSuck->Enable)
            #endif
            {
                MyLaneIO.SetUseIP(pTempSuck->SenRing, pTempSuck->SenIP, pTempSuck->SenISABase, pTempSuck->SensorName, false);   //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
            }

            //負壓
            if(INDEX_SUCKER_TYPE==1)
            {
                if(pTempSuck->SensorName.Pos("TestSuck")>0)
                {
                    pTempSuck->OnAlarmTime=pTempSuck->OnAlarmTime/10;           //Steven 20160224 : 因為避免機台INDEX吸不起IC
                    if(pTempSuck->OnAlarmTime>50)
                        pTempSuck->OnAlarmTime=50;
                }
            }
        }
        T->Close();
    }
#endif

    for(int i=0; i<2; i++)
    {
        for(int j=0; j<8; j++)
        {
            S.sprintf("%c%c", 'A'+i, 'a'+j);
            FTestSuck.Suck[i][j].sName=S;
            FTestSuck.Suck[i][j].iMyRow=i;
            FTestSuck.Suck[i][j].iMyCol=j;
            BTestSuck.Suck[i][j].sName=S;
            BTestSuck.Suck[i][j].iMyRow=i;
            BTestSuck.Suck[i][j].iMyCol=j;
        }
    }

    if(MachineTypeChoice==Type_HT9045)
    {
        for(int i=0; i<2; i++)
        {
            for(int j=4; j<8; j++)
            {
                FTestSuck.Suck[i][j].Enable    =false;
                FTestSuck.Suck[i][j].OnEnable=false;   // AI(W906-GA2-C1-fix) 20260804: M-1 -- OnEnable EXISTS (aHotPlateSubstrate.h:252); un-gated
#if 0 // TODO(GA2-C1): blocked by TMySucker::OffEnable -- genuinely absent from the MINIMAL mirror
                FTestSuck.Suck[i][j].OffEnable=false;
#endif
                BTestSuck.Suck[i][j].Enable    =false;
                BTestSuck.Suck[i][j].OnEnable=false;   // AI(W906-GA2-C1-fix) 20260804: M-1 -- OnEnable EXISTS (aHotPlateSubstrate.h:252); un-gated
#if 0 // TODO(GA2-C1): blocked by TMySucker::OffEnable -- genuinely absent from the MINIMAL mirror
                BTestSuck.Suck[i][j].OffEnable=false;
#endif
            }
        }
    }
    else if(MachineTypeChoice==Type_HT9045_12Site)
    {
        for(int i=0; i<2; i++)
        {
            for(int j=6; j<8; j++)
            {
                FTestSuck.Suck[i][j].Enable    =false;
                FTestSuck.Suck[i][j].OnEnable=false;   // AI(W906-GA2-C1-fix) 20260804: M-1 -- OnEnable EXISTS (aHotPlateSubstrate.h:252); un-gated
#if 0 // TODO(GA2-C1): blocked by TMySucker::OffEnable -- genuinely absent from the MINIMAL mirror
                FTestSuck.Suck[i][j].OffEnable=false;
#endif
                BTestSuck.Suck[i][j].Enable    =false;
                BTestSuck.Suck[i][j].OnEnable=false;   // AI(W906-GA2-C1-fix) 20260804: M-1 -- OnEnable EXISTS (aHotPlateSubstrate.h:252); un-gated
#if 0 // TODO(GA2-C1): blocked by TMySucker::OffEnable -- genuinely absent from the MINIMAL mirror
                BTestSuck.Suck[i][j].OffEnable=false;
#endif
            }
        }
    }

#if 0 // TODO(GA2-C1): blocked by CopyKitSuck@MyKitSuck.cpp (whole-grid clone helper, not ported -- distinct from the
      //   already-ported CopyInitSuck) + FTestSuckBackup/BTestSuckBackup grids not declared anywhere -- golden cinitial.cpp:821-822
    CopyKitSuck(&FTestSuck, &FTestSuckBackup);
    CopyKitSuck(&BTestSuck, &BTestSuckBackup);
#endif
    InArmSuck.Suck[0][0].iMotNo=MInArmZA;
    InArmSuck.Suck[1][0].iMotNo=MInArmZB;
    InArmSuck.Suck[0][1].iMotNo=MInArmZC;
    InArmSuck.Suck[1][1].iMotNo=MInArmZD;
    InArmSuck.Suck[0][2].iMotNo=MInArmZE;
    InArmSuck.Suck[1][2].iMotNo=MInArmZF;
    InArmSuck.Suck[0][3].iMotNo=MInArmZG;
    InArmSuck.Suck[1][3].iMotNo=MInArmZH;

    OutArmSuck.Suck[0][0].iMotNo=MOutArmZA;
    OutArmSuck.Suck[1][0].iMotNo=MOutArmZB;
    OutArmSuck.Suck[0][1].iMotNo=MOutArmZC;
    OutArmSuck.Suck[1][1].iMotNo=MOutArmZD;
    OutArmSuck.Suck[0][2].iMotNo=MOutArmZE;
    OutArmSuck.Suck[1][2].iMotNo=MOutArmZF;
    OutArmSuck.Suck[0][3].iMotNo=MOutArmZG;
    OutArmSuck.Suck[1][3].iMotNo=MOutArmZH;

    OutArm2Suck.Suck[0][0].iMotNo=MOutSortAa;                                    //RogerYang 20250416 for HT9046AU add Sort Arm Z
    OutArm2Suck.Suck[0][1].iMotNo=MOutSortAb;

//    if(USE_PICKER_COUNT==ep16Picker &&
//       InOutArmPickerUseMotor==eptUseMotCyn)                                    //Steven for HT1032
//    {
//        InArmSuck.Suck[0][0].iMotNo=C_InArmAa;
//        InArmSuck.Suck[1][0].iMotNo=C_InArmBa;
//        InArmSuck.Suck[0][1].iMotNo=C_InArmAb;
//        InArmSuck.Suck[1][1].iMotNo=C_InArmBb;
//        InArmSuck.Suck[0][2].iMotNo=C_InArmAc;
//        InArmSuck.Suck[1][2].iMotNo=C_InArmBc;
//        InArmSuck.Suck[0][3].iMotNo=C_InArmAd;
//        InArmSuck.Suck[1][3].iMotNo=C_InArmBd;
//        InArmSuck.Suck[0][4].iMotNo=C_InArmAe;
//        InArmSuck.Suck[1][4].iMotNo=C_InArmBe;
//        InArmSuck.Suck[0][5].iMotNo=C_InArmAf;
//        InArmSuck.Suck[1][5].iMotNo=C_InArmBf;
//        InArmSuck.Suck[0][6].iMotNo=C_InArmAg;
//        InArmSuck.Suck[1][6].iMotNo=C_InArmBg;
//        InArmSuck.Suck[0][7].iMotNo=C_InArmAh;
//        InArmSuck.Suck[1][7].iMotNo=C_InArmBh;
//
//        OutArmSuck.Suck[0][0].iMotNo=C_OutArmAa;
//        OutArmSuck.Suck[1][0].iMotNo=C_OutArmBa;
//        OutArmSuck.Suck[0][1].iMotNo=C_OutArmAb;
//        OutArmSuck.Suck[1][1].iMotNo=C_OutArmBb;
//        OutArmSuck.Suck[0][2].iMotNo=C_OutArmAc;
//        OutArmSuck.Suck[1][2].iMotNo=C_OutArmBc;
//        OutArmSuck.Suck[0][3].iMotNo=C_OutArmAd;
//        OutArmSuck.Suck[1][3].iMotNo=C_OutArmBd;
//        OutArmSuck.Suck[0][4].iMotNo=C_OutArmAe;
//        OutArmSuck.Suck[1][4].iMotNo=C_OutArmBe;
//        OutArmSuck.Suck[0][5].iMotNo=C_OutArmAf;
//        OutArmSuck.Suck[1][5].iMotNo=C_OutArmBf;
//        OutArmSuck.Suck[0][6].iMotNo=C_OutArmAg;
//        OutArmSuck.Suck[1][6].iMotNo=C_OutArmBg;
//        OutArmSuck.Suck[0][7].iMotNo=C_OutArmAh;
//        OutArmSuck.Suck[1][7].iMotNo=C_OutArmBh;
//
//        for(int i=0; i<2; i++)
//        {
//            for(int j=0; j<8; j++)
//            {
//                S.sprintf("%c%c", 'A'+i, 'a'+j);
//                InArmSuck.Suck[i][j].sName=S;
//                InArmSuck.Suck[i][j].iMyRow=i;
//                InArmSuck.Suck[i][j].iMyCol=j;
//                OutArmSuck.Suck[i][j].sName=S;
//                OutArmSuck.Suck[i][j].iMyRow=i;
//                OutArmSuck.Suck[i][j].iMyCol=j;
//            }
//        }
//    }
//    else
    {
        InArmSuck.Suck[0][0].iMotNo=MInArmZA;
        InArmSuck.Suck[1][0].iMotNo=MInArmZB;
        InArmSuck.Suck[0][1].iMotNo=MInArmZC;
        InArmSuck.Suck[1][1].iMotNo=MInArmZD;
        InArmSuck.Suck[0][2].iMotNo=MInArmZE;
        InArmSuck.Suck[1][2].iMotNo=MInArmZF;
        InArmSuck.Suck[0][3].iMotNo=MInArmZG;
        InArmSuck.Suck[1][3].iMotNo=MInArmZH;

        OutArmSuck.Suck[0][0].iMotNo=MOutArmZA;
        OutArmSuck.Suck[1][0].iMotNo=MOutArmZB;
        OutArmSuck.Suck[0][1].iMotNo=MOutArmZC;
        OutArmSuck.Suck[1][1].iMotNo=MOutArmZD;
        OutArmSuck.Suck[0][2].iMotNo=MOutArmZE;
        OutArmSuck.Suck[1][2].iMotNo=MOutArmZF;
        OutArmSuck.Suck[0][3].iMotNo=MOutArmZG;
        OutArmSuck.Suck[1][3].iMotNo=MOutArmZH;

        if(USE_PICKER_COUNT==ep16Picker &&
           InOutArmPickerUseMotor==eptUseMot)                                   //Ztex 2023.04.19 Add HT-1032
        {
            InArmSuck.Suck[0][0].iMotNo=MInArmZA;
            InArmSuck.Suck[0][1].iMotNo=MInArmZC;
            InArmSuck.Suck[0][2].iMotNo=MInArmZE;
            InArmSuck.Suck[0][3].iMotNo=MInArmZG;
            InArmSuck.Suck[0][4].iMotNo=MInArmZAe;
            InArmSuck.Suck[0][5].iMotNo=MInArmZAf;
            InArmSuck.Suck[0][6].iMotNo=MInArmZAg;
            InArmSuck.Suck[0][7].iMotNo=MInArmZAh;

            InArmSuck.Suck[1][0].iMotNo=MInArmZB;
            InArmSuck.Suck[1][1].iMotNo=MInArmZD;
            InArmSuck.Suck[1][2].iMotNo=MInArmZF;
            InArmSuck.Suck[1][3].iMotNo=MInArmZH;
            InArmSuck.Suck[1][4].iMotNo=MInArmZBe;
            InArmSuck.Suck[1][5].iMotNo=MInArmZBf;
            InArmSuck.Suck[1][6].iMotNo=MInArmZBg;
            InArmSuck.Suck[1][7].iMotNo=MInArmZBh;

            OutArmSuck.Suck[0][0].iMotNo=MOutArmZA;
            OutArmSuck.Suck[0][1].iMotNo=MOutArmZC;
            OutArmSuck.Suck[0][2].iMotNo=MOutArmZE;
            OutArmSuck.Suck[0][3].iMotNo=MOutArmZG;
            OutArmSuck.Suck[0][4].iMotNo=MOutArmZAe;
            OutArmSuck.Suck[0][5].iMotNo=MOutArmZAf;
            OutArmSuck.Suck[0][6].iMotNo=MOutArmZAg;
            OutArmSuck.Suck[0][7].iMotNo=MOutArmZAh;

            OutArmSuck.Suck[1][0].iMotNo=MOutArmZB;
            OutArmSuck.Suck[1][1].iMotNo=MOutArmZD;
            OutArmSuck.Suck[1][2].iMotNo=MOutArmZF;
            OutArmSuck.Suck[1][3].iMotNo=MOutArmZH;
            OutArmSuck.Suck[1][4].iMotNo=MOutArmZBe;
            OutArmSuck.Suck[1][5].iMotNo=MOutArmZBf;
            OutArmSuck.Suck[1][6].iMotNo=MOutArmZBg;
            OutArmSuck.Suck[1][7].iMotNo=MOutArmZBh;

            for(int j=0; j<8; j++)
            {
                for(int i=0; i<2; i++)
                {
                    S.sprintf("%c%c", 'A'+i, 'a'+j);                            //Ztex 2024.07.11 Add Suck Name A --> Aa
                    InArmSuck.Suck[i][j].sName=S;
                    InArmSuck.Suck[i][j].iMyRow=i;
                    InArmSuck.Suck[i][j].iMyCol=j;
                    OutArmSuck.Suck[i][j].sName=S;
                    OutArmSuck.Suck[i][j].iMyRow=i;
                    OutArmSuck.Suck[i][j].iMyCol=j;
                }
            }
        }
        else
        {
            int k=0;
            for(int j=0; j<8; j++)
            {
                for(int i=0; i<2; i++)
                {
                    S.sprintf("%c", 'A'+k);
                    InArmSuck.Suck[i][j].sName=S;
                    InArmSuck.Suck[i][j].iMyRow=i;
                    InArmSuck.Suck[i][j].iMyCol=j;
                    OutArmSuck.Suck[i][j].sName=S;
                    OutArmSuck.Suck[i][j].iMyRow=i;
                    OutArmSuck.Suck[i][j].iMyCol=j;
                    k++;
                }
            }

            k=0;
            for(int i=0; i<2; i++)
            {
                S.sprintf("%c", 'A'+k);                                         //RogerYang 20250416 for HT9046AU add Sort Arm Z
                OutArm2Suck.Suck[0][i].sName=S;
                OutArm2Suck.Suck[0][i].iMyRow=0;
                OutArm2Suck.Suck[0][i].iMyCol=i;
                k++;
            }
        }
    }

#if 0 // TODO(GA2-C1): blocked by CopyKitSuck@MyKitSuck.cpp (not ported); InArmSuckBackup grid not declared anywhere
      //   (OutArmSuckBackup DOES exist @ aHotPlateSubstrate.h, but CopyKitSuck itself is still missing either way)
      //   -- golden cinitial.cpp:998-999
    CopyKitSuck(&InArmSuck, &InArmSuckBackup);                                  //Frank HT1032 add
    CopyKitSuck(&OutArmSuck,&OutArmSuckBackup);
#endif
}
//==============================================================================
//------------------------------------------------------------------------------
//  InitialSwitchName  (golden cinitial.cpp:1002-1375)
//  Pure Name-table literal list.  Every SW[] index below already exists in
//  cmydef.h (checked exhaustively: all 331 distinct symbols referenced here
//  resolve).  Zero gating, byte-identical to golden.
//------------------------------------------------------------------------------
void InitialSwitchName()
{
    SW[SwFKPowerOff                 ].Name="SwFKPowerOff";
    SW[SwFKPowerOn                  ].Name="SwFKPowerOn";
    SW[SwFKReset                    ].Name="SwFKReset";
    SW[SwFKPause                    ].Name="SwFKPause";
    SW[SwFKHome                     ].Name="SwFKHome";
    SW[SwFKStart                    ].Name="SwFKStart";
    SW[SwFKOneCycle                 ].Name="SwFKOneCycle";
    SW[SwFKRetry                    ].Name="SwFKRetry";
    SW[SwFKSkip                     ].Name="SwFKSkip";
    SW[SwFKCleanOut                 ].Name="SwFKCleanOut";
    SW[SwFKTrayFeed                 ].Name="SwFKTrayFeed";
    SW[SwFKTrayEnd                  ].Name="SwFKTrayEnd";
    SW[SwFKAlarmReset               ].Name="SwFKAlarmReset";
    SW[SwFKCoverOpen                ].Name="SwFKCoverOpen";
    SW[SwRKPowerOff                 ].Name="SwRKPowerOff";
    SW[SwRKPowerOn                  ].Name="SwRKPowerOn";
    SW[SwRKReset                    ].Name="SwRKReset";
    SW[SwRKPause                    ].Name="SwRKPause";
    SW[SwRKHome                     ].Name="SwRKHome";
    SW[SwRKStart                    ].Name="SwRKStart";
    SW[SwRKOneCycle                 ].Name="SwRKOneCycle";
    SW[SwRKRetry                    ].Name="SwRKRetry";
    SW[SwRKSkip                     ].Name="SwRKSkip";
    SW[SwRKCleanOut                 ].Name="SwRKCleanOut";
    SW[SwRKTrayFeed                 ].Name="SwRKTrayFeed";
    SW[SwRKTrayEnd                  ].Name="SwRKTrayEnd";
    SW[SwRKAlarmReset               ].Name="SwRKAlarmReset";
    SW[SwRKCoverOpen                ].Name="SwRKCoverOpen";
    SW[SwRKManualStep               ].Name="SwRKManualStep";
    SW[SwRKManualTStart             ].Name="SwRKManualTStart";
    SW[SwTowerRed                   ].Name="SwTowerRed";
    SW[SwTowerGreen                 ].Name="SwTowerGreen";
    SW[SwTowerYellow                ].Name="SwTowerYellow";
    SW[SwFMotorBreaker              ].Name="SwFMotorBreaker";
    SW[SwBMotorBreaker              ].Name="SwBMotorBreaker";
    SW[SwMotorRelay                 ].Name="SwMotorRelay";
    SW[SwHeaterRelay                ].Name="SwHeaterRelay";
    SW[SwMusic1                     ].Name="SwMusic1";
    SW[SwMusic2                     ].Name="SwMusic2";
    SW[SwMusic3                     ].Name="SwMusic3";
    SW[SwMusic4                     ].Name="SwMusic4";
    SW[SwTestPassLed                ].Name="SwTestPassLed";
    SW[SwTestFailLed                ].Name="SwTestFailLed";
    SW[SwFrontActiveLed             ].Name="SwFrontActiveLed";
    SW[SwRearActiveLed              ].Name="SwRearActiveLed";
    SW[SwClear0                     ].Name="SwClear0";
    SW[SwClear1                     ].Name="SwClear1";
    SW[SwStart0                     ].Name="SwStart0";
    SW[SwStart1                     ].Name="SwStart1";
    SW[SwACTrayY                    ].Name="SwACTrayY";
    SW[SwACAuto1                    ].Name="SwACAuto1";
    SW[SwACAuto2                    ].Name="SwACAuto2";
    SW[SwACAuto3                    ].Name="SwACAuto3";
    SW[SwServerON                   ].Name="SwServerON";
    SW[SwManualZ1                   ].Name="SwManualZ1";
    SW[SwManualZ2                   ].Name="SwManualZ2";
    SW[SwACEmptyCW                  ].Name="SwACEmptyCW";
    SW[SwACEmptyCCW                 ].Name="SwACEmptyCCW";
    SW[SwACColorCW                  ].Name="SwACColorCW";
    SW[SwClear2                     ].Name="SwClear2";
    SW[SwClear3                     ].Name="SwClear3";
    SW[SwStart2                     ].Name="SwStart2";
    SW[SwStart3                     ].Name="SwStart3";
    SW[SwCCDAir                     ].Name="SwCCDAir";
    SW[SwACAutoCW                   ].Name="SwACAutoCW";
    SW[SwACAuto1Mode                ].Name="SwACAuto1Mode";
    SW[SwReadTorue                  ].Name="SwReadTorue";
    SW[SwBigFan                     ].Name="SwBigFan";
    SW[SwDut0                       ].Name="SwDut0";
    SW[SwDut1                       ].Name="SwDut1";
    SW[SwDut2                       ].Name="SwDut2";
    SW[SwDut3                       ].Name="SwDut3";
    SW[Sw10Bit                      ].Name="Sw10Bit";
    SW[SwAuto1SelectSlow            ].Name="SwAuto1SelectSlow";
    SW[SwCCDDestroy                 ].Name="SwCCDDestroy";
    SW[SwSafeLock                   ].Name="SwSafeLock";
    SW[SwRKSafeLock                 ].Name="SwRKSafeLock";                      //KenHsieh 20211228 : 區分實體IO與通訊面板
    SW[SwCCDLight                   ].Name="SwCCDLight";
    SW[SwHeaterFan                  ].Name="SwHeaterFan";
    SW[SwSocketClean                ].Name="SwSocketClean";
    SW[SwACColorCCW                 ].Name="SwACColorCCW";
    SW[SwShuttleFan                 ].Name="SwShuttleFan";
    SW[SwTesterDoubleContact        ].Name="SwTesterDoubleContact";
    SW[SwTesterPower                ].Name="SwTesterPower";
    SW[SwACEmpty1CW                 ].Name="SwACEmpty1CW";
    SW[SwACEmpty1CCW                ].Name="SwACEmpty1CCW";
    SW[SwDutHeaterCoolFan           ].Name="SwDutHeaterCoolFan";
    SW[SwLoaderBin                  ].Name="SwLoaderBin";
    SW[SwEmpty1Bin                  ].Name="SwEmpty1Bin";
    SW[SwEmpty2Bin                  ].Name="SwEmpty2Bin";
    SW[SwAuto1Bin                   ].Name="SwAuto1Bin";
    SW[SwAuto2Bin                   ].Name="SwAuto2Bin";
    SW[SwAuto3Bin                   ].Name="SwAuto3Bin";
    SW[SwFix1Bin                    ].Name="SwFix1Bin";
    SW[SwFix2Bin                    ].Name="SwFix2Bin";
    SW[SwFix3Bin                    ].Name="SwFix3Bin";
    SW[SwFix4Bin                    ].Name="SwFix4Bin";
    SW[SwFix5Bin                    ].Name="SwFix5Bin";
    SW[SwFix6Bin                    ].Name="SwFix6Bin";
    SW[SwZ1SuckMode0                ].Name="SwZ1SuckMode0";
    SW[SwZ1SuckMode1                ].Name="SwZ1SuckMode1";
    SW[SwZ2SuckMode0                ].Name="SwZ2SuckMode0";
    SW[SwZ2SuckMode1                ].Name="SwZ2SuckMode1";
    SW[SwShuttleCooling             ].Name="SwShuttleCooling";                  //jou 2010-06-09
    SW[SwCCDCooling                 ].Name="SwCCDCooling";                      //Steven 20110705
    SW[SwEpArm1                     ].Name="SwEpArm1";                          //Steven 20110708
    SW[SwEpArm2                     ].Name="SwEpArm2";                          //Steven 20110708
    SW[SwHeaterFanSpeed             ].Name="SwHeaterFanSpeed";                  //Steven 20110725
    SW[SwRotateCheckClear           ].Name="SwRotateCheckClear";                //ChungHung 20110922 : 轉轉蝦頭要檢查有沒有轉頭 Check Sensor
    SW[SwEP_D0                      ].Name="SwEP_D0";                           //Dell 20111111 Start : 加入Digital E/P
    SW[SwEP_D1                      ].Name="SwEP_D1";
    SW[SwEP_D2                      ].Name="SwEP_D2";
    SW[SwEP_D3                      ].Name="SwEP_D3";
    SW[SwEP_D4                      ].Name="SwEP_D4";
    SW[SwEP_D5                      ].Name="SwEP_D5";
    SW[SwEP_D6                      ].Name="SwEP_D6";
    SW[SwEP_D7                      ].Name="SwEP_D7";
    SW[SwEP_D8                      ].Name="SwEP_D8";
    SW[SwEP_D9                      ].Name="SwEP_D9";
    SW[SwCoolingFan_Blower          ].Name="SwCoolingFan_Blower";               //20111130  Dell
    SW[SwSafeDoorLock               ].Name="SwSafeDoorLock";                    //20111130  Dell
    SW[SwIndexIonFan                ].Name="SwIndexIonFan";                     //jou 2012-03-13 index離子槍出風開關控制
    SW[SwIndexChangeToque1          ].Name="SwIndexChangeToque1";               //jou 2012-06-21 Enable index I/O Change Toque
    SW[SwIndexChangeToque2          ].Name="SwIndexChangeToque2";               //jou 2012-06-21 Enable index I/O Change Toque
    SW[SwACLoaderCCW                ].Name="SwACLoaderCCW";                     //Loader退Tray
    SW[SwACAuto2CW                  ].Name="SwACAuto2CW";                       //Auto2進Tray
    SW[SwACAuto3CW                  ].Name="SwACAuto3CW";                       //Auto3進Tray
    SW[SwHeatGun                    ].Name="SwHeatGun";                         //ChungHung 20121107 add
    SW[SwCDAGun                     ].Name="SwCDAGun";                          //Steven 20181012 : 使用熱風槍吹冷風
    SW[SwVacuumPumpTogetherOn       ].Name="SwVacuumPumpTogetherOn";
    SW[SwAirConditioner             ].Name="SwAirConditioner";                  //Steven 20131011 : 冷氣機
    SW[SwHotplateCooling            ].Name="SwHotplateCooling";                 //jou 2013-11-07
    SW[SwCarRecord                  ].Name="SwCarRecord";                       //wei 2013-12-09
    SW[SwLoadCellA                  ].Name="SwLoadCellA";                       //kevin 20190306  add load Cell  read 1
    SW[SwLoadCellB                  ].Name="SwLoadCellB";                       //kevin 20190306  add load Cell  read 2

    SW[SwUnDock                     ].Name="SwUnDock";                          //Steven 20140310 : One Touch Docking
    SW[SwDockError                  ].Name="SwDockError";                       //Steven 20140310 : One Touch Docking

    SW[SwStartTest1                 ].Name="SwStartTest1";                      //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    SW[SwStartTest2                 ].Name="SwStartTest2";                      //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    SW[SwStartTest3                 ].Name="SwStartTest3";                      //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    SW[SwOCRTigger                  ].Name="SwOCRTigger";                       //wei 20150720 OCR觸發

    SW[SwTesterAirCooling           ].Name="SwTesterAirCooling";                //jou 2016-01-07 Tester Air Cooling Switch
    SW[SwFanDirection               ].Name="SwFanDirection";                    //wei 20160215大風扇方向
    SW[SwDieClean                   ].Name="SwDieClean";
    SW[SwDieCleanSuck               ].Name="SwDieCleanSuck";

    SW[SwLightOff                   ].Name="SwLightOff";
    SW[SwAirOff                     ].Name="SwAirOff";

    SW[SwCaselevatorZ               ].Name="SwCaselevatorZ";
    SW[SwTrayBracketZ               ].Name="SwTrayBracketZ";

    SW[SwE84LREQ                    ].Name="SwE84LREQ";
    SW[SwE84UREQ                    ].Name="SwE84UREQ";
    SW[SwE84VA                      ].Name="SwE84VA";
    SW[SwE84READY                   ].Name="SwE84READY";
    SW[SwE84VS0                     ].Name="SwE84VS0";
    SW[SwE84VS1                     ].Name="SwE84VS1";
    SW[SwE84HOAVBL                  ].Name="SwE84HOAVBL";
    SW[SwE84ES                      ].Name="SwE84ES";
    SW[SwE84POWER                   ].Name="SwE84POWER";

    SW[SwCassette01                 ].Name="SwCassette01";                      //wei 20180702 MR
    SW[SwCassette02                 ].Name="SwCassette02";                      //wei 20180702 MR
    SW[SwCassette03                 ].Name="SwCassette03";                      //wei 20180702 MR
    SW[SwCassette04                 ].Name="SwCassette04";                      //wei 20180702 MR
    SW[SwCassette05                 ].Name="SwCassette05";                      //wei 20180702 MR
    SW[SwCassette06                 ].Name="SwCassette06";                      //wei 20180702 MR
    SW[SwCassette07                 ].Name="SwCassette07";                      //wei 20180702 MR
    SW[SwCassette08                 ].Name="SwCassette08";                      //wei 20180702 MR

    SW[SwCassette09                 ].Name="SwCassette09";                      //wei 20180702 MR
    SW[SwCassette10                 ].Name="SwCassette10";                      //wei 20180702 MR
    SW[SwMRStart                    ].Name="SwMRStart";                         //wei 20180702 MR
    SW[SwMRPause                    ].Name="SwMRPause";                         //wei 20180702 MR
    SW[SwMRUp                       ].Name="SwMRUp";                            //wei 20180702 MR
    SW[SwMRDown                     ].Name="SwMRDown";                          //wei 20180702 MR

    SW[SwMRError                    ].Name="SwMRError";                         //wei 20180702 MR
    SW[SwMRUnLoadReady              ].Name="SwMRUnLoadReady";                   //wei 20180702 MR
    SW[SwMRPresence                 ].Name="SwMRPresence";                      //wei 20180702 MR
    SW[SwMRPlacement                ].Name="SwMRPlacement";                     //wei 20180702 MR
    SW[SwMRLoadReady                ].Name="SwMRLoadReady";                     //wei 20180702 MR
    SW[SwMRManualMode               ].Name="SwMRManualMode";                    //wei 20180702 MR

    SW[SwMRLight                    ].Name="SwMRLight";                         //wei 20180702 MR
    SW[SwBufferArmZ                 ].Name="SwBufferArmZ";                      //wei 20200302 MR

    SW[SwTrayBracket2Z              ].Name="SwTrayBracket2Z";                   //KaiChen 20200716 ：OHT
    SW[SwCaselevator2Z              ].Name="SwCaselevator2Z";
    SW[SwMultileEmptyZ              ].Name="SwMultileEmptyZ";

    SW[SwOHT_UnloadReady_1          ].Name="SwOHT_UnloadReady_1";
    SW[SwOHT_CarrierPresence_1      ].Name="SwOHT_CarrierPresence_1";
    SW[SwOHT_CarrierPlacement_1     ].Name="SwOHT_CarrierPlacement_1";
    SW[SwOHT_LoadReady_1            ].Name="SwOHT_LoadReady_1";
    SW[SwOHT_ManualMode_1           ].Name="SwOHT_ManualMode_1";
    SW[SwOHT_AutoMode_1             ].Name="SwOHT_AutoMode_1";
    SW[SwOHT_Error_1                ].Name="SwOHT_Error_1";

    SW[SwOHT_UnloadReady_2          ].Name="SwOHT_UnloadReady_2";
    SW[SwOHT_CarrierPresence_2      ].Name="SwOHT_CarrierPresence_2";
    SW[SwOHT_CarrierPlacement_2     ].Name="SwOHT_CarrierPlacement_2";
    SW[SwOHT_LoadReady_2            ].Name="SwOHT_LoadReady_2";
    SW[SwOHT_ManualMode_2           ].Name="SwOHT_ManualMode_2";
    SW[SwOHT_AutoMode_2             ].Name="SwOHT_AutoMode_2";
    SW[SwOHT_Error_2                ].Name="SwOHT_Error_2";

    SW[SwMultileEmpty_ScanTray_Open ].Name="SwMultileEmpty_ScanTray_Open";
    SW[SwMultileEmpty_ScanTray_Close].Name="SwMultileEmpty_ScanTray_Close";
    SW[SwMultileEmpty_ScanTrayID    ].Name="SwMultileEmpty_ScanTrayID";

    SW[SwE84_2_LREQ                 ].Name="SwE84_2_LREQ";
    SW[SwE84_2_UREQ                 ].Name="SwE84_2_UREQ";
    SW[SwE84_2_VA                   ].Name="SwE84_2_VA";
    SW[SwE84_2_READY                ].Name="SwE84_2_READY";
    SW[SwE84_2_VS0                  ].Name="SwE84_2_VS0";
    SW[SwE84_2_VS1                  ].Name="SwE84_2_VS1";
    SW[SwE84_2_HOAVBL               ].Name="SwE84_2_HOAVBL";
    SW[SwE84_2_ES                   ].Name="SwE84_2_ES";
    SW[SwE84_2_POWER                ].Name="SwE84_2_POWER";

    SW[SwE84_1_LREQ                 ].Name="SwE84_1_LREQ";
    SW[SwE84_1_UREQ                 ].Name="SwE84_1_UREQ";
    SW[SwE84_1_VA                   ].Name="SwE84_1_VA";
    SW[SwE84_1_READY                ].Name="SwE84_1_READY";
    SW[SwE84_1_VS0                  ].Name="SwE84_1_VS0";
    SW[SwE84_1_VS1                  ].Name="SwE84_1_VS1";
    SW[SwE84_1_HOAVBL               ].Name="SwE84_1_HOAVBL";
    SW[SwE84_1_ES                   ].Name="SwE84_1_ES";
    SW[SwE84_1_POWER                ].Name="SwE84_1_POWER";

    SW[SwSafeDoorLock_LoadPort1     ].Name="SwSafeDoorLock_LoadPort1";
    SW[SwSafeDoorLock_LoadPort2     ].Name="SwSafeDoorLock_LoadPort2";

    SW[SwLoadDoorLock               ].Name="SwLoadDoorLock";                    //Sam 20190112 LM
    SW[SwLoadRobotZ                 ].Name="SwLoadRobotZ";
    SW[SwUnloadDoorLock             ].Name="SwUnloadDoorLock";
    SW[SwUnloadRobotZ               ].Name="SwUnloadRobotZ";

    SW[SwIonRelay                   ].Name="SwIonRelay";                        //Ifor 20190114 : add Chamber 開啟時 關閉 Ion 離子槍 吹氣
    //------------------------------------
    //Alick 20161011 (Steven) : TTL支援8Site
    //------------------------------------
    SW[SwClear4                     ].Name="SwClear4";                          //Clear All
    SW[SwClear5                     ].Name="SwClear5";                          //Anti-Start Signal
    SW[SwClear6                     ].Name="SwClear6";                          //Start Enable
    SW[SwClear7                     ].Name="SwClear7";                          //Reserve
    SW[SwStart4                     ].Name="SwStart4";
    SW[SwStart5                     ].Name="SwStart5";
    SW[SwStart6                     ].Name="SwStart6";
    SW[SwStart7                     ].Name="SwStart7";
    SW[SwDut4                       ].Name="SwDut4";
    SW[SwDut5                       ].Name="SwDut5";
    SW[SwDut6                       ].Name="SwDut6";
    SW[SwDut7                       ].Name="SwDut7";
    SW[Sw10Bit2                     ].Name="Sw10Bit2";

    SW[SwShuttleVibration1          ].Name="SwShuttleVibration1";               //JerryYang 20171006 (wei) Shuttle 震動馬達
    SW[SwShuttleVibration2          ].Name="SwShuttleVibration2";
    SW[SwPurgeAir                   ].Name="SwPurgeAir";                        //kevin 20180928 (Steven) : add blower load board
    SW[SwSocketClean2               ].Name="SwSocketClean2";                    //JerryYang 20190715 Clean air arm1 arm2分開控制
    SW[SwLoaderVibration            ].Name="SwLoaderVibration";                 //JerryYang 20191001 loader震動馬達
    SW[SwTesterDryAirSwitch         ].Name="SwTesterDryAirSwitch";              //Ifor 20200115 : add Tester Dry Air Control
    SW[SwIndEpArm1                  ].Name="SwIndEpArm1";                       //Steven 20110708
    SW[SwIndEpArm2                  ].Name="SwIndEpArm2";                       //Steven 20110708
    SW[SwAutoCoolDown               ].Name="SwAutoCoolDown";                    //kevin 20201223 AutoCool down
    SW[SwIonFanClean                ].Name="SwIonFanClean";                     //Isaac 20210609 : IO觸發IonFan清針

    SW[SwTjSignal01                 ].Name="SwTjSignal01";                      //Ifor 20210622 add: ATC Switch TJ
    SW[SwTjSignal02                 ].Name="SwTjSignal02";
    SW[SwTjSignal03                 ].Name="SwTjSignal03";
    SW[SwTjSignal04                 ].Name="SwTjSignal04";
    SW[SwTjSignal05                 ].Name="SwTjSignal05";
    SW[SwTjSignal06                 ].Name="SwTjSignal06";
    SW[SwTjSignal07                 ].Name="SwTjSignal07";
    SW[SwTjSignal08                 ].Name="SwTjSignal08";

    SW[SwATCHeatGun                 ].Name="SwATCHeatGun";                      //JerryYang 20220408 : add for ATC3.5
    SW[SwLBAir                      ].Name="SwLBAir";                           //JerryYang 20220923 : LB吹氣function

    SW[SwDryAirSwitch               ].Name="SwDryAirSwitch";                    //Ztex 2023.04.13 Add HT-1032 IO
    SW[SwColdAirSwitch              ].Name="SwColdAirSwitch";                   //Ztex 2023.04.13 Add HT-1032 IO
    SW[SwTriTempSafeDoor6Lock       ].Name="SwTriTempSafeDoor6Lock";            //Ztex 2023.04.13 Add HT-1032 IO

    SW[SwEnhaustAirFanPowerOn       ].Name="SwEnhaustAirFanPowerOn";            //Ztex 2023.04.26 Add HT-1032 IO Exhaust Air
    SW[SwMagazineMotorBreaker       ].Name="SwMagazineMotorBreaker";            //JerryYang 20221215 : add Magazine
    SW[SwMagazineSafeDoorLock       ].Name="SwMagazineSafeDoorLock";
    SW[SwMagazineSafeDoor2LockOn    ].Name="SwMagazineSafeDoor2LockOn";
    SW[SwMagazineSafeDoor2LockOff   ].Name="SwMagazineSafeDoor2LockOff";

    SW[SwESDAntennaRelay1           ].Name="SwESDAntennaRelay1";
    SW[SwESDAntennaRelay2           ].Name="SwESDAntennaRelay2";
    SW[SwESDAntennaRelay3           ].Name="SwESDAntennaRelay3";
    SW[SwESDAntennaRelay4           ].Name="SwESDAntennaRelay4";
    SW[SwESDAntennaRelay5           ].Name="SwESDAntennaRelay5";
    SW[SwESDAntennaRelay6           ].Name="SwESDAntennaRelay6";
    SW[SwESDAntennaRelay7           ].Name="SwESDAntennaRelay7";
    SW[SwESDAntennaRelay8           ].Name="SwESDAntennaRelay8";

    SW[SwLoaderAirClean             ].Name="SwLoaderAirClean";

    SW[SwACAuto4                    ].Name="SwACAuto4";                         //Steven 20240822 : For HT-9046AU
    SW[SwACAuto4CW                  ].Name="SwACAuto4CW";
    SW[SwACAuto5                    ].Name="SwACAuto5";
    SW[SwACAuto5CW                  ].Name="SwACAuto5CW";
    SW[SwACAuto6                    ].Name="SwACAuto6";
    SW[SwACAuto6CW                  ].Name="SwACAuto6CW";

    SW[SwLoad2Bin                   ].Name="SwLoad2Bin";
    SW[SwLoad2AirClean              ].Name="SwLoad2AirClean";
    SW[SwACLoad2CCW                 ].Name="SwACLoad2CCW";
    SW[SwACTray2Y                   ].Name="SwACTray2Y";
    SW[SwLoad2Vibration             ].Name="SwLoad2Vibration";

    SW[SwSafeDoor1HatchwayLock      ].Name="SwSafeDoor1HatchwayLock";           //ChungHung 20230718 add for Safe plc start
    SW[SwSafeDoor2HatchwayLock      ].Name="SwSafeDoor2HatchwayLock";
    SW[SwSafeDoor3HatchwayLock      ].Name="SwSafeDoor3HatchwayLock";
    SW[SwSafeDoor4HatchwayLock      ].Name="SwSafeDoor4HatchwayLock";
    SW[SwSafeDoor5HatchwayLock      ].Name="SwSafeDoor5HatchwayLock";
    SW[SwSafeDoor6HatchwayLock      ].Name="SwSafeDoor6HatchwayLock";
    SW[SwSafeDoor7HatchwayLock      ].Name="SwSafeDoor7HatchwayLock";
    SW[SwSafeDoor8HatchwayLock      ].Name="SwSafeDoor8HatchwayLock";

    SW[SwElectricControlBoxLock1    ].Name="SwElectricControlBoxLock1";
    SW[SwElectricControlBoxLock2    ].Name="SwElectricControlBoxLock2";
    SW[SwElectricControlBoxLock3    ].Name="SwElectricControlBoxLock3";
    SW[SwElectricControlBoxLock4    ].Name="SwElectricControlBoxLock4";
    SW[SwElectricControlBoxLock5    ].Name="SwElectricControlBoxLock5";
    SW[SwElectricControlBoxLock6    ].Name="SwElectricControlBoxLock6";
    SW[SwElectricControlBoxLock7    ].Name="SwElectricControlBoxLock7";         //ChungHung 20230718 add for Safe plc end

    SW[SwInArmZBreaker              ].Name="SwInArmZBreaker";                   //add One sucker with rotate
    SW[SwOutArmZBreaker             ].Name="SwOutArmZBreaker";                  //add One sucker with rotate

    SW[SwBottomBlower               ].Name="SwBottomBlower";
    SW[SwFixedSeatTLOn              ].Name="SwFixedSeatTLOn";
    SW[SwFixedSeatTLOff             ].Name="SwFixedSeatTLOff";
    SW[SwFixedSeatTROn              ].Name="SwFixedSeatTROn";
    SW[SwFixedSeatTROff             ].Name="SwFixedSeatTROff";
    SW[SwFixedSeatBLOn              ].Name="SwFixedSeatBLOn";
    SW[SwFixedSeatBLOff             ].Name="SwFixedSeatBLOff";
    SW[SwFixedSeatBROn              ].Name="SwFixedSeatBROn";
    SW[SwFixedSeatBROff             ].Name="SwFixedSeatBROff";

    SW[SwCCDZBreaker                ].Name="SwCCDZBreaker";
    SW[SwTopBtmRotateLockOn         ].Name="SwTopBtmRotateLockOn";
    SW[SwTopBtmRotateLockOff        ].Name="SwTopBtmRotateLockOff";
    SW[SwLightStart                 ].Name="SwLightStart";
    SW[SwPRGSEL0                    ].Name="SwPRGSEL0";
    SW[SwPRGSEL1                    ].Name="SwPRGSEL1";
    SW[SwPRGSEL2                    ].Name="SwPRGSEL2";
    SW[SwPRGSEL3                    ].Name="SwPRGSEL3";
    SW[SwPRGSEL4                    ].Name="SwPRGSEL4";
    SW[SwPRGSEL5                    ].Name="SwPRGSEL5";
    SW[SwPRGSEL6                    ].Name="SwPRGSEL6";
    SW[SwLightOrg                   ].Name="SwLightOrg";
    SW[SwMultiEp                    ].Name="SwMultiEp";                         //Ifor 20250618 add:Auto Switch Multi EP

    SW[SwLoadCarRFIDZBreaker        ].Name="SwLoadCarRFIDZBreaker";             //RogerYang 20250828 add for Loader Rotate Arm

    SW[SwCassetteLDMotBreaker     ].Name="SwCassetteLDMotBreaker";              //Ifor 20251216 add:Boat Carrier
    SW[SwCassetteAuto1MotBreaker  ].Name="SwCassetteAuto1MotBreaker";
    SW[SwCassetteAuto2MotBreaker  ].Name="SwCassetteAuto2MotBreaker";

    SW[SwDryAirUseHandler           ].Name="SwDryAirUseHandler";
    SW[SwDryAirUseATCCar            ].Name="SwDryAirUseATCCar";
    SW[SwMultiEp                    ].Name="SwMultiEp";                       //Ifor 20250618 add:Auto Switch Multi EP
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//  InitialSwitch  (golden cinitial.cpp:1377-1591)
//  The IO_CARD_TYPE==NewIO_MN200||PCI_P64C64 (IO_Table.csv-driven) path is
//  FAITHFUL/active -- SW[] already carries every field it touches (Name/
//  Enable/ISABase/Using/Ring/IP/Port/Bit/Type) and HSys.mapIOTable/IOTable
//  are wired (database.h).  The switch.db (BDE) else-branch is gated: TTable/
//  DataModule1 do not exist anywhere in the ported tree yet (file banner,
//  blocker (2)).
//------------------------------------------------------------------------------
void InitialSwitch()
{
    int iSw;
    bool bHasSwitch;                                                            //Stevem 20210625 : 修正新的CSV IO表讀檔方式
    AnsiString str;

    if(IO_CARD_TYPE==NewIO_MN200 ||                                             //Steven 20231218 HT7080B
       IO_CARD_TYPE==PCI_P64C64)
    {
        InitialSwitchName();
        for(int i=0; i<MAX_SWITCH_ITEM; i++)
        {
            bHasSwitch=false;
            SW[i].Enable=false;
            if(SW[i].Name=="")
                continue;

            HSys.mapIOTableIter=HSys.mapIOTable.find(SW[i].Name);
            if(HSys.mapIOTableIter!=HSys.mapIOTable.end())
            {
                bHasSwitch=true;
                iSw=atoi(HSys.mapIOTable[SW[i].Name].c_str());
            }
            else
            {
                str.sprintf("Can not find switch %s", SW[i].Name);
//                continue;
            }

            if(bHasSwitch)
            {
                if(HSys.IOTable[iSw]->iEnable==1)
                    SW[i].Enable    =true;
                SW[i].ISABase       =HSys.IOTable[iSw]->iISABase;
                SW[i].Using         =(HSys.IOTable[iSw]->iPort==-1)?AnsiString(""):AnsiString(HSys.IOTable[iSw]->iPort);
                SW[i].Ring          =HSys.IOTable[iSw]->iLane;
                SW[i].IP            =HSys.IOTable[iSw]->iIP;
                SW[i].Port          =HSys.IOTable[iSw]->iPort;
                SW[i].Bit           =HSys.IOTable[iSw]->iBit;
                SW[i].Type          =HSys.IOTable[iSw]->iInType;

                #ifndef SOFT_SIMULTE
                if(SW[i].Enable)
                #endif
                {
                    MyLaneIO.SetUseIP(SW[i].Ring, SW[i].IP, SW[i].ISABase, SW->Name);     //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
                }
            }
            else
            {
                SW[i].Enable=false;
            }
            #ifdef SOFT_SIMULTE
            SW[i].Enable=false;
            #endif
        }
    }
#if 0 // TODO(GA2-C1): blocked by TTable/DataModule1->SwitchTable (BDE not ported, see database.h's own
      //   "WHAT IS STILL GATED" banner) -- golden cinitial.cpp:1437-1515
      // AI(W906-GA2-C1-fix) 20260804: M-2 WARNING for the un-gating wave -- this gate holds
      // relocated declarations followed by a bare `else`; flipping it on AS-IS is a parse
      // error. Re-derive declaration placement from golden when un-gating.
    TTable *T;
    bool flag;
    AnsiString S, S1, sDBDir;

    else
    {
        sDBDir=CurrentDir+"\\system\\switch.db";                                //JerryYang 20150903 檢查Database檔是否存在
        if(FileExists(sDBDir))
        {
            DataModule1->SwitchTable->TableName=sDBDir;
        }
        else
        {
            ShowMyMessage("switch.db does not exist");
            return;
        }

        T=DataModule1->SwitchTable;
        T->Open();
        InitialSwitchName();
        for(int i=0; i<MAX_SWITCH_ITEM; i++)
        {
            if(SW[i].Name=="")
                continue;
            T->First();
            flag=false;
            while(!T->Eof)
            {
                S=T->FieldByName("SwitchName")->AsString;
                if(S==SW[i].Name)
                {
                    flag=true;
                    break;
                }
                T->Next();
            }

            if(flag==false)
            {
                str.sprintf("Can not find switch name:%s", SW[i].Name);
            }
            else
            {
                SW[i].Enable            =atoi(T->FieldByName("Enable")->AsString.c_str());
                SW[i].ISABase           =atoi(T->FieldByName("ISABase")->AsString.c_str());
                if(SW[i].ISABase==eISABase ||
                   SW[i].ISABase==ePCI1735U ||                                  //Alick 20160809 add for PCI-1735U
                   SW[i].ISABase==ePLCbase)
                {
                    SW[i].Using         =T->FieldByName("Port")->AsString;
                    if(SW[i].Using=="")
                        SW[i].Enable    =false;
                    SW[i].Ring          =0;
                    SW[i].IP            =0;
                    SW[i].Port          =HexStrToInt(T->FieldByName("Port")->AsString.c_str());
                    SW[i].Bit           =atoi(T->FieldByName("Bit")->AsString.c_str());
                }
                else
                {
                    SW[i].Using         =T->FieldByName("Lane")->AsString;
                    if(SW[i].Using!="")
                        SW[i].Ring      =atoi(SW[i].Using.c_str());
                    else
                        SW[i].Enable=false;
                    SW[i].IP            =atoi(T->FieldByName("IP")->AsString.c_str());
                    SW[i].Port          =atoi(T->FieldByName("Port")->AsString.c_str());
                    SW[i].Bit           =atoi(T->FieldByName("Bit")->AsString.c_str());
                }
                SW[i].Type              =atoi(T->FieldByName("OutIOType")->AsString.c_str());
                #ifdef SOFT_SIMULTE
                SW[i].Enable=false;
                #endif

                #ifndef SOFT_SIMULTE
                if(SW[i].Enable)
                #endif
                {
                    MyLaneIO.SetUseIP(SW[i].Ring, SW[i].IP, SW[i].ISABase, SW->Name);     //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
                }
            }
        }
        T->Close();
    }
#endif
    #ifdef SOFT_SIMULTE
        SW[SwReadTorue].Enable=false;                                           // for home and get height use
        SW[SwStart0].Enable=true;
        SW[SwStart1].Enable=true;
        SW[SwStart2].Enable=true;
        SW[SwStart3].Enable=true;
        SW[SwDut0].Enable=true;
        SW[SwDut1].Enable=true;
        SW[SwDut2].Enable=true;
        SW[SwDut3].Enable=true;

        SW[SwStart4].Enable=true;                                               //Alick 20161011 (Steven) : TTL支援8Site
        SW[SwStart5].Enable=true;
        SW[SwStart6].Enable=true;
        SW[SwStart7].Enable=true;
        SW[SwDut4].Enable=true;
        SW[SwDut5].Enable=true;
        SW[SwDut6].Enable=true;
        SW[SwDut7].Enable=true;
    #endif

    if(iControlPanelMode==1)                                                    //Steven 20250317 : 使用通訊式面板的要disable
    {
        SW[SwFKPowerOff                 ].Enable=false;
        SW[SwFKPowerOn                  ].Enable=false;
        SW[SwFKReset                    ].Enable=false;
        SW[SwFKPause                    ].Enable=false;
        SW[SwFKHome                     ].Enable=false;
        SW[SwFKStart                    ].Enable=false;
        SW[SwFKOneCycle                 ].Enable=false;
        SW[SwFKRetry                    ].Enable=false;
        SW[SwFKSkip                     ].Enable=false;
        SW[SwFKCleanOut                 ].Enable=false;
        SW[SwFKTrayFeed                 ].Enable=false;
        SW[SwFKTrayEnd                  ].Enable=false;
        SW[SwFKAlarmReset               ].Enable=false;
        SW[SwFKCoverOpen                ].Enable=false;
        SW[SwRKPowerOff                 ].Enable=false;
        SW[SwRKPowerOn                  ].Enable=false;
        SW[SwRKReset                    ].Enable=false;
        SW[SwRKPause                    ].Enable=false;
        SW[SwRKHome                     ].Enable=false;
        SW[SwRKStart                    ].Enable=false;
        SW[SwRKOneCycle                 ].Enable=false;
        SW[SwRKRetry                    ].Enable=false;
        SW[SwRKSkip                     ].Enable=false;
        SW[SwRKCleanOut                 ].Enable=false;
        SW[SwRKTrayFeed                 ].Enable=false;
        SW[SwRKTrayEnd                  ].Enable=false;
        SW[SwRKAlarmReset               ].Enable=false;
        SW[SwRKCoverOpen                ].Enable=false;
        SW[SwRKManualStep               ].Enable=false;
        SW[SwRKManualTStart             ].Enable=false;
        SW[SwFrontActiveLed             ].Enable=false;
    }

    CopySwitch(&SW[SwStart0], &SWBackup[BackSwStart0]);
    CopySwitch(&SW[SwStart1], &SWBackup[BackSwStart1]);
    CopySwitch(&SW[SwStart2], &SWBackup[BackSwStart2]);
    CopySwitch(&SW[SwStart3], &SWBackup[BackSwStart3]);

    CopySwitch(&SW[SwDut0], &SWBackup[BackSwDut0]);
    CopySwitch(&SW[SwDut1], &SWBackup[BackSwDut1]);
    CopySwitch(&SW[SwDut2], &SWBackup[BackSwDut2]);
    CopySwitch(&SW[SwDut3], &SWBackup[BackSwDut3]);

    CopySwitch(&SW[SwStart4], &SWBackup[BackSwStart4]);                         //Alick 20161011 (Steven) : TTL支援8Site
    CopySwitch(&SW[SwStart5], &SWBackup[BackSwStart5]);
    CopySwitch(&SW[SwStart6], &SWBackup[BackSwStart6]);
    CopySwitch(&SW[SwStart7], &SWBackup[BackSwStart7]);

    CopySwitch(&SW[SwDut4], &SWBackup[BackSwDut4]);
    CopySwitch(&SW[SwDut5], &SWBackup[BackSwDut5]);
    CopySwitch(&SW[SwDut6], &SWBackup[BackSwDut6]);
    CopySwitch(&SW[SwDut7], &SWBackup[BackSwDut7]);
}
//==============================================================================
//------------------------------------------------------------------------------
//  InitialSensorName  (golden cinitial.cpp:1593-2469)
//  Pure Name-table literal list.  Every Sen[] index below already exists in
//  cmydef.h.  The 9 lines golden itself already left commented out
//  (SnIndexY1CHK/SnIndexZ1CHK/SnIndexZ2CHK/SnIndexY2CHK/SnCatchTrayOnDetect/
//  SnArm1OverPress1/SnArm1OverPress2/SnArm2OverPress1/SnArm2OverPress2
//  plus SnCatchTrayDown/SnMotorYAlarm/SnMotorZ1Alarm/SnChamberDryAir -- 13 in all,
//  AI(W906-GA2-C1-fix) 20260804: count corrected from 9 per independent review) are
//  preserved as comments, not silently re-enabled -- they are not gaps this
//  wave introduced.  Zero gating otherwise, byte-identical to golden.
//------------------------------------------------------------------------------
void InitialSensorName()
{
    Sen[SnFKPowerOff                ].Name="SnFKPowerOff";
    Sen[SnFKPowerOn                 ].Name="SnFKPowerOn";
    Sen[SnFKReset                   ].Name="SnFKReset";
    Sen[SnFKPause                   ].Name="SnFKPause";
    Sen[SnFKHome                    ].Name="SnFKHome";
    Sen[SnFKStart                   ].Name="SnFKStart";
    Sen[SnFKOneCycle                ].Name="SnFKOneCycle";
    Sen[SnFKRetry                   ].Name="SnFKRetry";
    Sen[SnFKSkip                    ].Name="SnFKSkip";
    Sen[SnFKCleanOut                ].Name="SnFKCleanOut";
    Sen[SnFKTrayFeed                ].Name="SnFKTrayFeed";
    Sen[SnFKTrayEnd                 ].Name="SnFKTrayEnd";
    Sen[SnFKAlarmReset              ].Name="SnFKAlarmReset";
    Sen[SnFKCoverOpen               ].Name="SnFKCoverOpen";
    Sen[SnRKPowerOff                ].Name="SnRKPowerOff";
    Sen[SnRKPowerOn                 ].Name="SnRKPowerOn";
    Sen[SnRKReset                   ].Name="SnRKReset";
    Sen[SnRKPause                   ].Name="SnRKPause";
    Sen[SnRKHome                    ].Name="SnRKHome";
    Sen[SnRKStart                   ].Name="SnRKStart";
    Sen[SnRKOneCycle                ].Name="SnRKOneCycle";
    Sen[SnRKRetry                   ].Name="SnRKRetry";
    Sen[SnRKSkip                    ].Name="SnRKSkip";
    Sen[SnRKCleanOut                ].Name="SnRKCleanOut";
    Sen[SnRKTrayFeed                ].Name="SnRKTrayFeed";
    Sen[SnRKTrayEnd                 ].Name="SnRKTrayEnd";
    Sen[SnRKAlarmReset              ].Name="SnRKAlarmReset";
    Sen[SnRKCoverOpen               ].Name="SnRKCoverOpen";
    Sen[SnRKManualStep              ].Name="SnRKManualStep";
    Sen[SnRKManualTStart            ].Name="SnRKManualTStart";
    Sen[SnLoaderTrayHasTray         ].Name="SnLoaderTrayHasTray";
    Sen[SnLoaderCarHasTray          ].Name="SnLoaderCarHasTray";
    Sen[SnInPutSHT1S1               ].Name="SnInPutSHT1S1";
    Sen[SnInPutSHT1S2               ].Name="SnInPutSHT1S2";
    Sen[SnInPutSHT1S3               ].Name="SnInPutSHT1S3";
    Sen[SnInPutSHT1S4               ].Name="SnInPutSHT1S4";
    Sen[SnInPutSHT1S5               ].Name="SnInPutSHT1S5";
    Sen[SnInPutSHT1S6               ].Name="SnInPutSHT1S6";
    Sen[SnInPutSHT1S7               ].Name="SnInPutSHT1S7";
    Sen[SnInPutSHT1S8               ].Name="SnInPutSHT1S8";
    Sen[SnInPutSHT1S9               ].Name="SnInPutSHT1S9";
    Sen[SnInPutSHT2S1               ].Name="SnInPutSHT2S1";
    Sen[SnInPutSHT2S2               ].Name="SnInPutSHT2S2";
    Sen[SnInPutSHT2S3               ].Name="SnInPutSHT2S3";
    Sen[SnInPutSHT2S4               ].Name="SnInPutSHT2S4";
    Sen[SnInPutSHT2S5               ].Name="SnInPutSHT2S5";
    Sen[SnInPutSHT2S6               ].Name="SnInPutSHT2S6";
    Sen[SnInPutSHT2S7               ].Name="SnInPutSHT2S7";
    Sen[SnInPutSHT2S8               ].Name="SnInPutSHT2S8";
    Sen[SnInPutSHT2S9               ].Name="SnInPutSHT2S9";
    Sen[SnOutPutSHT1S1              ].Name="SnOutPutSHT1S1";
    Sen[SnOutPutSHT1S2              ].Name="SnOutPutSHT1S2";
    Sen[SnOutPutSHT1S3              ].Name="SnOutPutSHT1S3";
    Sen[SnOutPutSHT1S4              ].Name="SnOutPutSHT1S4";
    Sen[SnOutPutSHT1S5              ].Name="SnOutPutSHT1S5";
    Sen[SnOutPutSHT1S6              ].Name="SnOutPutSHT1S6";
    Sen[SnOutPutSHT1S7              ].Name="SnOutPutSHT1S7";
    Sen[SnOutPutSHT2S1              ].Name="SnOutPutSHT2S1";
    Sen[SnOutPutSHT2S2              ].Name="SnOutPutSHT2S2";
    Sen[SnOutPutSHT2S3              ].Name="SnOutPutSHT2S3";
    Sen[SnOutPutSHT2S4              ].Name="SnOutPutSHT2S4";
    Sen[SnOutPutSHT2S5              ].Name="SnOutPutSHT2S5";
    Sen[SnOutPutSHT2S6              ].Name="SnOutPutSHT2S6";
    Sen[SnOutPutSHT2S7              ].Name="SnOutPutSHT2S7";
    Sen[SnOutPutSHT1ZS1             ].Name="SnOutPutSHT1ZS1";
    Sen[SnOutPutSHT1ZS2             ].Name="SnOutPutSHT1ZS2";
    Sen[SnOutPutSHT2ZS1             ].Name="SnOutPutSHT2ZS1";
    Sen[SnOutPutSHT2ZS2             ].Name="SnOutPutSHT2ZS2";
    Sen[SnAuto1TrayDetect           ].Name="SnAuto1TrayDetect";
    Sen[SnAuto2TrayDetect           ].Name="SnAuto2TrayDetect";
    Sen[SnAuto3TrayDetect           ].Name="SnAuto3TrayDetect";
    Sen[SnAuto1IsFull               ].Name="SnAuto1IsFull";
    Sen[SnAuto2IsFull               ].Name="SnAuto2IsFull";
    Sen[SnAuto3IsFull               ].Name="SnAuto3IsFull";
    Sen[SnFixedTray1Detect          ].Name="SnFixedTray1Detect";
    Sen[SnFixedTray2Detect          ].Name="SnFixedTray2Detect";
    Sen[SnFixedTray3Detect          ].Name="SnFixedTray3Detect";
    Sen[SnFixedTray4Detect          ].Name="SnFixedTray4Detect";
    Sen[SnFixedTray5Detect          ].Name="SnFixedTray5Detect";
    Sen[SnFixedTray6Detect          ].Name="SnFixedTray6Detect";
    Sen[SnSafeDoor1                 ].Name="SnSafeDoor1";
    Sen[SnSafeDoor2                 ].Name="SnSafeDoor2";
    Sen[SnSafeDoor3                 ].Name="SnSafeDoor3";
    Sen[SnSafeDoor4                 ].Name="SnSafeDoor4";
    Sen[SnSafeDoor5                 ].Name="SnSafeDoor5";
    Sen[SnSafeDoor6                 ].Name="SnSafeDoor6";
    Sen[SnSafeDoor7                 ].Name="SnSafeDoor7";
    Sen[SnSafeDoor8                 ].Name="SnSafeDoor8";
    Sen[SnSafeDoor9                 ].Name="SnSafeDoor9";
    Sen[SnSafeDoor10                ].Name="SnSafeDoor10";
    Sen[SnAirIsEnough               ].Name="SnAirIsEnough";
    Sen[SnFrontRightEMG             ].Name="SnFrontRightEMG";
    Sen[SnRearLeftEMG               ].Name="SnRearLeftEMG";
    Sen[SnFMotorDown                ].Name="SnFMotorDown";
    Sen[SnBMotorDown                ].Name="SnBMotorDown";
    Sen[SnMotorPower                ].Name="SnMotorPower";
    Sen[SnSystemPower               ].Name="SnSystemPower";
    Sen[SnEmptyTrayHasTray1         ].Name="SnEmptyTrayHasTray1";
    Sen[SnEmptyTrayIsFull1          ].Name="SnEmptyTrayIsFull1";
    Sen[SnEmptyTrayIsLock1          ].Name="SnEmptyTrayIsLock1";
    Sen[SnEmptyTrayHasTray2         ].Name="SnEmptyTrayHasTray2";
    Sen[SnEmptyTrayIsFull2          ].Name="SnEmptyTrayIsFull2";
    Sen[SnRearPadActive             ].Name="SnRearPadActive";
    Sen[SnFrontLeftEMG              ].Name="SnFrontLeftEMG";
    Sen[SnRearRightEMG              ].Name="SnRearRightEMG";
    Sen[SnEPDieForce                ].Name="SnEPDieForce";
//    Sen[SnCatchTrayDown             ].Name="SnCatchTrayDown";
    Sen[SenBit0                     ].Name="SenBit0";
    Sen[SenBit1                     ].Name="SenBit1";
    Sen[SenBit2                     ].Name="SenBit2";
    Sen[SenBit3                     ].Name="SenBit3";
    Sen[SenBit4                     ].Name="SenBit4";
    Sen[SenBit5                     ].Name="SenBit5";
    Sen[SenBit6                     ].Name="SenBit6";
    Sen[SenBit7                     ].Name="SenBit7";
    Sen[SenBit8                     ].Name="SenBit8";
    Sen[SenBit9                     ].Name="SenBit9";
    Sen[SnLoaderPreDete             ].Name="SnLoaderPreDete";
    Sen[SnAuto1_Tray_Car            ].Name="SnAuto1_Tray_Car";
    Sen[SnAuto2_Tray_Car            ].Name="SnAuto2_Tray_Car";
    Sen[SnAuto3_Tray_Car            ].Name="SnAuto3_Tray_Car";
    Sen[SnAuto1PreDete              ].Name="SnAuto1PreDete";
    Sen[SnAuto2PreDete              ].Name="SnAuto2PreDete";
    Sen[SnAuto3PreDete              ].Name="SnAuto3PreDete";
    Sen[SnLoaderSureTray            ].Name="SnLoaderSureTray";
//    Sen[SnMotorYAlarm               ].Name="SnMotorYAlarm";
//    Sen[SnMotorZ1Alarm              ].Name="SnMotorZ1Alarm";
    Sen[SnAuto1FixCyPush            ].Name="SnAuto1FixCyPush";
    Sen[SnAuto2FixCyPush            ].Name="SnAuto2FixCyPush";
    Sen[SnAuto3FixCyPush            ].Name="SnAuto3FixCyPush";
    Sen[SnLoaderFixCyPush           ].Name="SnLoaderFixCyPush";
    Sen[SenBit10                    ].Name="SenBit10";
    Sen[SenBit11                    ].Name="SenBit11";
    Sen[SenBit12                    ].Name="SenBit12";
    Sen[SenBit13                    ].Name="SenBit13";
    Sen[SenBit14                    ].Name="SenBit14";
    Sen[SenBit15                    ].Name="SenBit15";
    Sen[SenBit16                    ].Name="SenBit16";
    Sen[SenBit17                    ].Name="SenBit17";
    Sen[SenBit18                    ].Name="SenBit18";
    Sen[SenBit19                    ].Name="SenBit19";
    Sen[SenEmptyHasTray             ].Name="SenEmptyHasTray";
    Sen[SenEmptyCWDete              ].Name="SenEmptyCWDete";
    Sen[SenEmptySelectHasTray       ].Name="SenEmptySelectHasTray";
    Sen[SenEmptyCCWDete             ].Name="SenEmptyCCWDete";
    Sen[SenEmptyCarHasTray          ].Name="SenEmptyCarHasTray";
    Sen[SenColorHasTray             ].Name="SenColorHasTray";
    Sen[SenColorCWDete              ].Name="SenColorCWDete";
    Sen[SenColorSelectHasTray       ].Name="SenColorSelectHasTray";
    Sen[SenColorCarHasTray          ].Name="SenColorCarHasTray";
    Sen[SenEmptyFixCyPush           ].Name="SenEmptyFixCyPush";
    Sen[SnAuto1TrayHasTray          ].Name="SnAuto1TrayHasTray";
    Sen[SnAuto1CWPreDetect          ].Name="SnAutoCWPreDetect";
    Sen[SnHeaterDoor                ].Name="SnHeaterDoor";
    Sen[SnCatchTrayFix1On           ].Name="SnCatchTrayFix1On";
    Sen[SnCatchTrayFix2On           ].Name="SnCatchTrayFix2On";
    Sen[SnSafeLock                  ].Name="SnSafeLock";
    Sen[SnRKSafeLock                ].Name="SnRKSafeLock";                      //KenHsieh 20211228 : 區分實體IO與通訊面板
//    Sen[SnIndexY1CHK                ].Name="SnIndexY1CHK";
//    Sen[SnIndexZ1CHK                ].Name="SnIndexZ1CHK";
//    Sen[SnIndexZ2CHK                ].Name="SnIndexZ2CHK";
//    Sen[SnIndexY2CHK                ].Name="SnIndexY2CHK";

    Sen[SnFPLevelOpe                ].Name="SnFPLevelOpe";
    Sen[SnFPLevelEng                ].Name="SnFPLevelEng";                      //Steven 20190503 : 指紋辨識權限
    Sen[SnFPLevelSup                ].Name="SnFPLevelSup";
    Sen[SnFPLevelHon                ].Name="SnFPLevelHon";

    Sen[SnIndexHeaterFan            ].Name="SnIndexHeaterFan";
//    Sen[SnCatchTrayOnDetect         ].Name="SnCatchTrayOnDetect";
    Sen[SnHeaterDoor2               ].Name="SnHeaterDoor2";
    Sen[SnHeaterDoor3               ].Name="SnHeaterDoor3";
    Sen[SnHeaterDoor4               ].Name="SnHeaterDoor4";                     //wei 20200616 : For ATC3.3 MR, 要第四個加熱門
    Sen[SnAuto1TrackDetect          ].Name="SnAuto1TrackDetect";
    Sen[SnAuto2TrackDetect          ].Name="SnAuto2TrackDetect";
    Sen[SnAuto3TrackDetect          ].Name="SnAuto3TrackDetect";
    Sen[SnEmptyIsFull               ].Name="SnEmptyIsFull";
    Sen[SnColorIsFull               ].Name="SnColorIsFull";
    Sen[SenColorCCWDete             ].Name="SenColorCCWDete";
    Sen[SenColorFixCyPush           ].Name="SenColorFixCyPush";
    Sen[SenEmpty1HasTray            ].Name="SenEmpty1HasTray";
    Sen[SenEmpty1CCWDete            ].Name="SenEmpty1CCWDete";
    Sen[SenEmpty1CarHasTray         ].Name="SenEmpty1CarHasTray";
    Sen[SnAutoUpSafedetect0         ].Name="SnAutoUpSafedetect0";
    Sen[SnAutoUpSafedetect1         ].Name="SnAutoUpSafedetect1";
    Sen[SnAutoUpSafedetect2         ].Name="SnAutoUpSafedetect2";
    Sen[SnLoaderUpSafedetect        ].Name="SnLoaderUpSafedetect";
    Sen[SnEmptyUpSafedetect         ].Name="SnEmptyUpSafedetect";
    Sen[SnColorUpSafedetect         ].Name="SnColorUpSafedetect";
    Sen[SnIonFanAlarm               ].Name="SnIonFanAlarm";
    Sen[SnIonFanLevelAlarm          ].Name="SnIonFanLevelAlarm";
    Sen[SnIonBarrierAlarm           ].Name="SnIonBarrierAlarm";
    Sen[SnIonBarrierLevelAlarm      ].Name="SnIonBarrierLevelAlarm";
    Sen[SnIonBarrierConditionAlarm  ].Name="SnIonBarrierConditionAlarm";
    Sen[SnAutoColorTrayDetect0      ].Name="SnAutoColorTrayDetect0";
    Sen[SnAutoColorTrayDetect1      ].Name="SnAutoColorTrayDetect1";
    Sen[SnAutoColorTrayDetect2      ].Name="SnAutoColorTrayDetect2";
    Sen[SnAutoColorTrayDetect4      ].Name="SnAutoColorTrayDetect4";            //RogerYang 20250825 : 新增Auto4~6
    Sen[SnAutoColorTrayDetect5      ].Name="SnAutoColorTrayDetect5";
    Sen[SnAutoColorTrayDetect6      ].Name="SnAutoColorTrayDetect6";
//    Sen[SnArm1OverPress1            ].Name="SnArm1OverPress1";
//    Sen[SnArm1OverPress2            ].Name="SnArm1OverPress2";
//    Sen[SnArm2OverPress1            ].Name="SnArm2OverPress1";
//    Sen[SnArm2OverPress2            ].Name="SnArm2OverPress2";
    Sen[SnEPAlarm                   ].Name="SnEPAlarm";
    Sen[SnCheckTrayDirection        ].Name="SnCheckTrayDirection";
    Sen[SnCheckLoadDirection        ].Name="SnCheckLoadDirection";
    Sen[SnIonFan6Alarm              ].Name="SnIonFan6Alarm";
    Sen[SnIonFan7Alarm              ].Name="SnIonFan7Alarm";
    Sen[SnIonFan8Alarm              ].Name="SnIonFan8Alarm";
    Sen[SnIonFan9Alarm              ].Name ="SnIonFan9Alarm";
    Sen[SnIonFan10Alarm             ].Name="SnIonFan10Alarm";
    Sen[SnIonFan11Alarm             ].Name="SnIonFan11Alarm";
    Sen[SnIonFan12Alarm             ].Name="SnIonFan12Alarm";
    Sen[SnNegativePressureAir       ].Name="SnNegativePressureAir";
    Sen[SnNegativePressureAir2      ].Name="SnNegativePressureAir2";            //Sam 20171110 (Steven) : 新增氣壓 Sensor
    Sen[SnLoaderEdgePush            ].Name="SnLoaderEdgePush";
    Sen[SnAuto1EdgePush             ].Name="SnAuto1EdgePush";
    Sen[SnAuto2EdgePush             ].Name="SnAuto2EdgePush";
    Sen[SnAuto3EdgePush             ].Name="SnAuto3EdgePush";
    //----- by dell ccd realtime-------------
    Sen[SnRealTimeCCDStop           ].Name="SnRealTimeCCDStop";
    Sen[SnRealTimeCCDIndexArm       ].Name="SnRealTimeCCDIndexArm";
    Sen[SnRTCCDTempCtrl             ].Name="SnRTCCDTempCtrl";
    //---------------------------------------
    Sen[SnUnLoaderFloating          ].Name="SnUnLoaderFloating";                //Steven 20110705 : Tray置偏
    Sen[SnRotateCheck               ].Name="SnRotateCheck";                     //ChungHung 20110922 : 轉轉蝦頭要檢查有沒有轉頭 Check Sensor
    Sen[SnCheckConnectIndexArm_1    ].Name="SnCheckConnectIndexArm_1";          //20111130  Dell
    Sen[SnCheckConnectIndexArm_2    ].Name="SnCheckConnectIndexArm_2";          //20111130  Dell
    Sen[SnFixFloating1              ].Name="SnFixFloating1";                    //Steven 20120131 : Fix Tray置偏偵測
    Sen[SnFixFloating2              ].Name="SnFixFloating2";
    Sen[SnFixFloating3              ].Name="SnFixFloating3";                    //Sam 20240129 : 新增第三組 Fix floating Sensor
    Sen[SnATCAlarm1                 ].Name="SnATCAlarm1";                       //jou 2012-03-13 ATC Alarm 1 Sensor
    Sen[SnATCAlarm2                 ].Name="SnATCAlarm2";                       //jou 2012-03-13 ATC Alarm 2 Sensor
    Sen[SnATCAlarm3                 ].Name="SnATCAlarm3";                       //jou 2012-03-13 ATC Alarm 3 Sensor
    Sen[SnATCAlarm4                 ].Name="SnATCAlarm4";                       //jou 2012-03-13 ATC Alarm 4 Sensor
    Sen[SenInArmYPitch60            ].Name="SenInArmYPitch60";                  //ChungHung 20120505 : HT9045 WS Only
    Sen[SenOutArmYPitch60           ].Name="SenOutArmYPitch60";                 //ChungHung 20120505 : HT9045 WS Only
    Sen[SnAutoDockingOff            ].Name="SnAutoDockingOff";                  //ChungHung 20120718 add UseAutoDocking Check Sensor
    Sen[SnAutoDockingOn             ].Name="SnAutoDockingOn";                   //ChungHung 20120718 add UseAutoDocking Check Sensor
    //---------Auto2 kevin 20120725---------------------------------------
    Sen[SnAuto2TrayHasTray          ].Name="SnAuto2TrayHasTray";
    Sen[SnAuto3TrayHasTray          ].Name="SnAuto3TrayHasTray";
    Sen[SnLoaderIsFull              ].Name="SnLoaderIsFull";
    Sen[SnAuto2CWPreDetect          ].Name="SnAuto2CWPreDetect";
    Sen[SnAuto3CWPreDetect          ].Name="SnAuto3CWPreDetect";
    Sen[SenLoaderCCWDete            ].Name="SenLoaderCCWDete";

    Sen[SnTesterDocking             ].Name="SnTesterDocking";                   //jou 2012-09-13 Tester Docking
    Sen[SnTrain                     ].Name="SnTrain";
    Sen[SnFix3FullPlace             ].Name="SnFix3FullPlace";                   //Steven 20121020 : Fix3滿盤
    Sen[SnGroundMan                 ].Name="SnGroundMan";                       //wei 20150424 add SnGroundMan偵測
    Sen[SnGroundMan2                ].Name="SnGroundMan2";
    Sen[SnGroundMan3                ].Name="SnGroundMan3";
    Sen[SnGroundMan4                ].Name="SnGroundMan4";

    Sen[SnTesterDryAir              ].Name="SnTesterDryAir";                    //Ifor 20200115 add: add Tester Dry Air Control

    Sen[SnIonFanPower01             ].Name="SnIonFanPower01";                   //Steven 20130201 : Kasuga離子風扇電源偵測
    Sen[SnIonFanPower02             ].Name="SnIonFanPower02";
    Sen[SnIonFanPower03             ].Name="SnIonFanPower03";
    Sen[SnIonFanPower04             ].Name="SnIonFanPower04";
    Sen[SnIonFanPower05             ].Name="SnIonFanPower05";
    Sen[SnIonFanPower06             ].Name="SnIonFanPower06";
    Sen[SnIonFanPower07             ].Name="SnIonFanPower07";
    Sen[SnIonFanPower08             ].Name="SnIonFanPower08";
    Sen[SnIonFanPower09             ].Name="SnIonFanPower09";
    Sen[SnIonFanPower10             ].Name="SnIonFanPower10";
    Sen[SnIonFanPower11             ].Name="SnIonFanPower11";
    Sen[SnIonFanPower12             ].Name="SnIonFanPower12";
    Sen[SnFix3FullPlace             ].Name="SnFix3FullPlace";                   //Steven 20121020 : Fix3滿盤

    Sen[SnRotateRowIn1              ].Name="SnRotateRowIn1";                    //kevin 20130524
    Sen[SnRotateRowIn2              ].Name="SnRotateRowIn2";
    Sen[SnRotateRowOut1             ].Name="SnRotateRowOut1";
    Sen[SnRotateRowOut2             ].Name="SnRotateRowOut2";

    Sen[SnSocket1                   ].Name="SnSocket1";                         //kevin 20130429   socket sensor
    Sen[SnSocket2                   ].Name="SnSocket2";                         //kevin 20130429   socket sensor
    Sen[SnSocket3                   ].Name="SnSocket3";                         //kevin 20130429   socket sensor
    Sen[SnSocket4                   ].Name="SnSocket4";                         //kevin 20130429   socket sensor
    Sen[SnSocket5                   ].Name="SnSocket5";                         //kevin 20130429   socket sensor
    Sen[SnSocket6                   ].Name="SnSocket6";                         //kevin 20130429   socket sensor
    Sen[SnSocket7                   ].Name="SnSocket7";                         //kevin 20130429   socket sensor
    Sen[SnSocket8                   ].Name="SnSocket8";                         //kevin 20130429   socket sensor

    Sen[SnSocket9                   ].Name="SnSocket9";                         //Steven 20200610 : Socket sensor 改成16顆
    Sen[SnSocket10                  ].Name="SnSocket10";
    Sen[SnSocket11                  ].Name="SnSocket11";
    Sen[SnSocket12                  ].Name="SnSocket12";
    Sen[SnSocket13                  ].Name="SnSocket13";
    Sen[SnSocket14                  ].Name="SnSocket14";
    Sen[SnSocket15                  ].Name="SnSocket15";
    Sen[SnSocket16                  ].Name="SnSocket16";

    Sen[SnSocket17                  ].Name="SnSocket17";
    Sen[SnSocket18                  ].Name="SnSocket18";
    Sen[SnSocket19                  ].Name="SnSocket19";
    Sen[SnSocket20                  ].Name="SnSocket20";
    Sen[SnSocket21                  ].Name="SnSocket21";
    Sen[SnSocket22                  ].Name="SnSocket22";
    Sen[SnSocket23                  ].Name="SnSocket23";
    Sen[SnSocket24                  ].Name="SnSocket24";

    Sen[SnSocket25                  ].Name="SnSocket25";                        //JerryYang 20260205 : add第三組Socket sensor(8+16+8)
    Sen[SnSocket26                  ].Name="SnSocket26";
    Sen[SnSocket27                  ].Name="SnSocket27";
    Sen[SnSocket28                  ].Name="SnSocket28";
    Sen[SnSocket29                  ].Name="SnSocket29";
    Sen[SnSocket30                  ].Name="SnSocket30";
    Sen[SnSocket31                  ].Name="SnSocket31";
    Sen[SnSocket32                  ].Name="SnSocket32";

    Sen[SnCrossSHT1S1               ].Name="SnCrossSHT1S1";                     //2013-07-16    Dell    Shuttle cross sensor
    Sen[SnCrossSHT1S2               ].Name="SnCrossSHT1S2";                     //2013-07-16    Dell    Shuttle cross sensor
    Sen[SnCrossSHT2S1               ].Name="SnCrossSHT2S1";                     //2013-07-16    Dell    Shuttle cross sensor
    Sen[SnCrossSHT2S2               ].Name="SnCrossSHT2S2";                     //2013-07-16    Dell    Shuttle cross sensor

    Sen[SnServo                     ].Name="SnServo";                           //kevin 20140121 偵測servo on 訊號
    Sen[SnAutoTeach                 ].Name="SnAutoTeach";                       //kevin 201400512 AUTOTEACH IN/OUT ARM SENSOR

    Sen[SnEOF1                      ].Name="SnEOF1";                            //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    Sen[SnEOF2                      ].Name="SnEOF2";                            //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    Sen[SnEOF3                      ].Name="SnEOF3";                            //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    Sen[SnAOIResult1                ].Name="SnAOIResult1";                      //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    Sen[SnAOIResult2                ].Name="SnAOIResult2";                      //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    Sen[SnAOIResult3                ].Name="SnAOIResult3";                      //2014-03-04    Dell    for SPIL WLP Add 5S Inspection

    Sen[SnFix3Lock                  ].Name="SnFix3Lock";                        //Steven 20140512 : For HT-9046LA

    Sen[SenAutoDocking240KG         ].Name="SenAutoDocking240KG";               //ChungHung 20140709 add for hardware control OTD
    Sen[SenAutoDocking360KG         ].Name="SenAutoDocking360KG";               //ChungHung 20140709 add for hardware control OTD

    Sen[SnFixColorTrayDetect0       ].Name="SnFixColorTrayDetect0";             //20140903 wei colcr Tray
    Sen[SnFixColorTrayDetect1       ].Name="SnFixColorTrayDetect1";             //20140903 wei colcr Tray
    Sen[SnFixColorTrayDetect2       ].Name="SnFixColorTrayDetect2";             //20140903 wei colcr Tray

    Sen[SnFix4ColorTrayDetect       ].Name="SnFix4ColorTrayDetect";             //20140903 wei colcr Tray
    Sen[SnFix5ColorTrayDetect       ].Name="SnFix5ColorTrayDetect";             //20140903 wei colcr Tray
    Sen[SnFix6ColorTrayDetect       ].Name="SnFix6ColorTrayDetect";             //20140903 wei colcr Tray

    Sen[SnLoaderColorTrayDetect     ].Name="SnLoaderColorTrayDetect";           //20140903 wei colcr Tray
    Sen[SnEmptyColorTrayDetect      ].Name="SnEmptyColorTrayDetect";            //20140903 wei colcr Tray
    Sen[SnColorColorTrayDetect      ].Name="SnColorColorTrayDetect";            //20140903 wei colcr Tray
    Sen[SnOCRTigger                 ].Name="SnOCRTigger";                       //wei 20150720 OCR觸發
    Sen[SnLowSpeed                  ].Name="SnLowSpeed";                        //wei 20150720 OCR觸發
    Sen[SnOCRPosition               ].Name="SnOCRPosition";                     //wei 20150720 OCR觸發
    Sen[SnLoaderTrayHasTray_ART     ].Name="SnLoaderTrayHasTray_ART";           //wei 20151210
    Sen[SnSLK1UnclampButton         ].Name="SnSLK1UnclampButton";               //JerryYang 20160524
    Sen[SnSLK2UnclampButton         ].Name="SnSLK2UnclampButton";               //JerryYang 20160524
    Sen[SnSocketClampPush1          ].Name="SnSocketClampPush1";                //JerryYang 20160606
    Sen[SnSocketClampPush2          ].Name="SnSocketClampPush2";                //JerryYang 20160606
    Sen[SnSocketClampPull1          ].Name="SnSocketClampPull1";                //JerryYang 20160606
    Sen[SnSocketClampPull2          ].Name="SnSocketClampPull2";                //JerryYang 20160606
    Sen[SnSocketHasClamp1           ].Name="SnSocketHasClamp1";                 //JerryYang 20160606
    Sen[SnSocketHasClamp2           ].Name="SnSocketHasClamp2";                 //JerryYang 20160606

    Sen[SnHingeTopTray              ].Name="SnHingeTopTray";                    //wei 20170418 Hinge cassette
    Sen[SnSafeDoor11                ].Name="SnSafeDoor11";                      //wei 20180702 MR
    Sen[SnEmptyFull                 ].Name="SnEmptyFull";                       //wei 20180702 MR
    Sen[SnCassetteArmHave           ].Name="SnCassetteArmHave";                 //wei 20180702 MR
    Sen[SnBuffer1HaveCassette       ].Name="SnBuffer1HaveCassette";             //wei 20180702 MR
    Sen[SnBuffer2HaveCassette       ].Name="SnBuffer2HaveCassette";             //wei 20180702 MR
    Sen[SnBuffer3HaveCassette       ].Name="SnBuffer3HaveCassette";             //wei 20180702 MR
    Sen[SnBuffer4HaveCassette       ].Name="SnBuffer4HaveCassette";             //wei 20180702 MR
    Sen[SnBuffer5HaveCassette       ].Name="SnBuffer5HaveCassette";             //wei 20180702 MR
    Sen[SnBuffer6HaveCassette       ].Name="SnBuffer6HaveCassette";             //wei 20180702 MR
    Sen[SnBuffer7HaveCassette       ].Name="SnBuffer7HaveCassette";             //wei 20180702 MR
    Sen[SnBuffer8HaveCassette       ].Name="SnBuffer8HaveCassette";             //wei 20180702 MR
    Sen[SnBuffer9HaveCassette       ].Name="SnBuffer9HaveCassette";             //wei 20180702 MR
    Sen[SnBuffer10HaveCassette      ].Name="SnBuffer10HaveCassette";            //wei 20180702 MR
    Sen[SnLoadPortCatch             ].Name="SnLoadPortCatch";                   //wei 20180702 MR
    Sen[SnLoadPortPresent           ].Name="SnLoadPortPresent";                 //wei 20180702 MR
    Sen[SnLoadPortPlacement1        ].Name="SnLoadPortPlacement1";              //wei 20180702 MR
    Sen[SnLoadPortPlacement2        ].Name="SnLoadPortPlacement2";              //wei 20180702 MR
    Sen[SnTrayBracketHave           ].Name="SnTrayBracketHave";                 //wei 20180702 MR
    Sen[SnStackedTrayHave           ].Name="SnStackedTrayHave";                 //wei 20180702 MR
    Sen[SnBuffer6HaveTray           ].Name="SnBuffer6HaveTray";                 //wei 20180702 MR
    Sen[SnSafeDoor12                ].Name="SnSafeDoor12";                      //wei 20180702 MR
    Sen[SnSafeDoor13                ].Name="SnSafeDoor13";                      //wei 20180702 MR
    Sen[SnSafeDoor14                ].Name="SnSafeDoor14";                      //wei 20180702 MR
    Sen[SnSafeDoor15                ].Name="SnSafeDoor15";                      //wei 20180702 MR
    Sen[SnCassetteCatch             ].Name="SnCassetteCatch";                   //wei 20180702 MR

    Sen[SnE84VALID                  ].Name="SnE84VALID";                        //wei 20180702 E84
    Sen[SnE84CS0                    ].Name="SnE84CS0";                          //wei 20180702 E84
    Sen[SnE84CS1                    ].Name="SnE84CS1";                          //wei 20180702 E84
    Sen[SnE84AMAVBL                 ].Name="SnE84AMAVBL";                       //wei 20180702 E84
    Sen[SnE84TRREQ                  ].Name="SnE84TRREQ";                        //wei 20180702 E84
    Sen[SnE84BUSY                   ].Name="SnE84BUSY";                         //wei 20180702 E84
    Sen[SnE84COMPT                  ].Name="SnE84COMPT";                        //wei 20180702 E84
    Sen[SnE84CONT                   ].Name="SnE84CONT";                         //wei 20180702 E84
    Sen[SnE84GO                     ].Name="SnE84GO";                           //wei 20180702 E84

    Sen[SnCassette01                ].Name="SnCassette01";                      //wei 20180702 MR
    Sen[SnCassette02                ].Name="SnCassette02";                      //wei 20180702 MR
    Sen[SnCassette03                ].Name="SnCassette03";                      //wei 20180702 MR
    Sen[SnCassette04                ].Name="SnCassette04";                      //wei 20180702 MR
    Sen[SnCassette05                ].Name="SnCassette05";                      //wei 20180702 MR
    Sen[SnCassette06                ].Name="SnCassette06";                      //wei 20180702 MR
    Sen[SnCassette07                ].Name="SnCassette07";                      //wei 20180702 MR
    Sen[SnCassette08                ].Name="SnCassette08";                      //wei 20180702 MR

    Sen[SnCassette09                ].Name="SnCassette09";                      //wei 20180702 MR
    Sen[SnCassette10                ].Name="SnCassette10";                      //wei 20180702 MR
    Sen[SnMRStart                   ].Name="SnMRStart";                         //wei 20180702 MR
    Sen[SnMRPause                   ].Name="SnMRPause";                         //wei 20180702 MR
    Sen[SnMRUp                      ].Name="SnMRUp";                            //wei 20180702 MR
    Sen[SnMRDown                    ].Name="SnMRDown";                          //wei 20180702 MR

    Sen[SnCassetteArmCatch          ].Name="SnCassetteArmCatch";                //Sam 20190112 LM
    Sen[SnLoadPortHaveCassette1_1   ].Name="SnLoadPortHaveCassette1_1";
    Sen[SnLoadPortHaveCassette1_2   ].Name="SnLoadPortHaveCassette1_2";
    Sen[SnLoadPortHaveCassette2_1   ].Name="SnLoadPortHaveCassette2_1";
    Sen[SnLoadPortHaveCassette2_2   ].Name="SnLoadPortHaveCassette2_2";
    Sen[SnLoadPortHaveCassette3_1   ].Name="SnLoadPortHaveCassette3_1";
    Sen[SnLoadPortHaveCassette3_2   ].Name="SnLoadPortHaveCassette3_2";
    Sen[SnLoadPortHaveCassette4_1   ].Name="SnLoadPortHaveCassette4_1";
    Sen[SnLoadPortHaveCassette4_2   ].Name="SnLoadPortHaveCassette4_2";
    Sen[SnUnloadPortHaveCassette1_1 ].Name="SnUnloadPortHaveCassette1_1";
    Sen[SnUnloadPortHaveCassette1_2 ].Name="SnUnloadPortHaveCassette1_2";
    Sen[SnUnloadPortHaveCassette2_1 ].Name="SnUnloadPortHaveCassette2_1";
    Sen[SnUnloadPortHaveCassette2_2 ].Name="SnUnloadPortHaveCassette2_2";
    Sen[SnUnloadPortHaveCassette3_1 ].Name="SnUnloadPortHaveCassette3_1";
    Sen[SnUnloadPortHaveCassette3_2 ].Name="SnUnloadPortHaveCassette3_2";
    Sen[SnUnloadPortHaveCassette4_1 ].Name="SnUnloadPortHaveCassette4_1";
    Sen[SnUnloadPortHaveCassette4_2 ].Name="SnUnloadPortHaveCassette4_2";

    Sen[SnLoadRobotHaveCassette1    ].Name="SnLoadRobotHaveCassette1";
    Sen[SnLoadRobotHaveCassette2    ].Name="SnLoadRobotHaveCassette2";
    Sen[SnUnloadRobotHaveCassette1  ].Name="SnUnloadRobotHaveCassette1";
    Sen[SnUnloadRobotHaveCassette2  ].Name="SnUnloadRobotHaveCassette2";

    Sen[SnLoadRobotPosUp            ].Name="SnLoadRobotPosUp";
    Sen[SnLoadRobotPosDown          ].Name="SnLoadRobotPosDown";
    Sen[SnUnloadRobotPosUp          ].Name="SnUnloadRobotPosUp";
    Sen[SnUnloadRobotPosDown        ].Name="SnUnloadRobotPosDown";

    Sen[SnSafeDoor16                ].Name="SnSafeDoor16";
    Sen[SnSafeDoor17                ].Name="SnSafeDoor17";
    Sen[SnSafeDoor18                ].Name="SnSafeDoor18";
    Sen[SnSafeDoor19                ].Name="SnSafeDoor19";

    Sen[SnIndex1Connect1            ].Name="SnIndex1Connect1";                  //RogerYang 20161212 (Steven) 偵測Index1 SLK獨立加熱或共用加熱
    Sen[SnIndex1Connect2            ].Name="SnIndex1Connect2";                  //RogerYang 20161212 (Steven) 偵測Index1 SLK獨立加熱或共用加熱
    Sen[SnIndex2Connect1            ].Name="SnIndex2Connect1";                  //RogerYang 20161212 (Steven) 偵測Index2 SLK獨立加熱或共用加熱
    Sen[SnIndex2Connect2            ].Name="SnIndex2Connect2";                  //RogerYang 20161212 (Steven) 偵測Index2 SLK獨立加熱或共用加熱
    Sen[SnTrayCover                 ].Name="SnTrayCover";                       //Steven 20170623 (wei) : Add for catch tray with cover

    Sen[SnLoaderIsPreAlarm          ].Name="SnLoaderIsPreAlarm";                //wei 20170802 (Steven) Pre alarm sensor
    Sen[SnEmptyIsPreAlarm           ].Name="SnEmptyIsPreAlarm";                 //wei 20170802 (Steven) Pre alarm sensor
    Sen[SnColorIsPreAlarm           ].Name="SnColorIsPreAlarm";                 //wei 20170802 (Steven) Pre alarm sensor
    Sen[SnAuto1IsPreAlarm           ].Name="SnAuto1IsPreAlarm";                 //wei 20170802 (Steven) Pre alarm sensor
    Sen[SnAuto2IsPreAlarm           ].Name="SnAuto2IsPreAlarm";                 //wei 20170802 (Steven) Pre alarm sensor
    Sen[SnAuto3IsPreAlarm           ].Name="SnAuto3IsPreAlarm";                 //wei 20170802 (Steven) Pre alarm sensor

    //------------------------------------
    //Alick 20161011 (Steven) : TTL支援8Site
    //------------------------------------
    Sen[SenBit20                    ].Name="SenBit20";
    Sen[SenBit21                    ].Name="SenBit21";
    Sen[SenBit22                    ].Name="SenBit22";
    Sen[SenBit23                    ].Name="SenBit23";
    Sen[SenBit24                    ].Name="SenBit24";
    Sen[SenBit25                    ].Name="SenBit25";
    Sen[SenBit26                    ].Name="SenBit26";
    Sen[SenBit27                    ].Name="SenBit27";
    Sen[SenBit28                    ].Name="SenBit28";
    Sen[SenBit29                    ].Name="SenBit29";
    Sen[SenBit30                    ].Name="SenBit30";
    Sen[SenBit31                    ].Name="SenBit31";
    Sen[SenBit32                    ].Name="SenBit32";
    Sen[SenBit33                    ].Name="SenBit33";
    Sen[SenBit34                    ].Name="SenBit34";
    Sen[SenBit35                    ].Name="SenBit35";
    Sen[SenBit36                    ].Name="SenBit36";
    Sen[SenBit37                    ].Name="SenBit37";
    Sen[SenBit38                    ].Name="SenBit38";
    Sen[SenBit39                    ].Name="SenBit39";
    Sen[SnTrayArmSafePos            ].Name="SnTrayArmSafePos";                  //kevin 20171006 (wei) Home tray arm must on
    Sen[SnTJCurrent                 ].Name="SnTJCurrent";                       //Steven 20180124 (Wei) : Check ATC7.0 TJ Current
    Sen[SnLoaderUpPress             ].Name="SnLoaderUpPress";                   //JerryYang 20181120 (Steven) : (Steven) : 獨立控制loader壓tray
    Sen[SnPreciserDetect1           ].Name="SnPreciserDetect1";                 //Frank 20180410 (Steven) : InArm Preciser Station
    Sen[SnPreciserDetect2           ].Name="SnPreciserDetect2";
    Sen[SnLoadCell1                 ].Name="SnLoadCell1";                       //kevin 20190307  read load cell pass  or fail
    Sen[SnLoadCell2                 ].Name="SnLoadCell2";                       //kevin 20190307  read load cell pass  or fail
    Sen[SnLoadCell3                 ].Name="SnLoadCell3";                       //kevin 20190307  read load cell pass  or fail
    Sen[SnLoadCell4                 ].Name="SnLoadCell4";                       //kevin 20190307  read load cell pass  or fail
    Sen[SnLoadCell5                 ].Name="SnLoadCell5";                       //kevin 20190307  read load cell pass  or fail
    Sen[SnLoadCell6                 ].Name="SnLoadCell6";                       //kevin 20190307  read load cell pass  or fail
    Sen[SnLoadCell7                 ].Name="SnLoadCell7";                       //kevin 20190307  read load cell pass  or fail
    Sen[SnLoadCell8                 ].Name="SnLoadCell8";                       //kevin 20190307  read load cell pass  or fail
    Sen[SnHotGun1                   ].Name="SnHotGun1";                         //kevin 20190621   Hot Gun 1 流量偵測
    Sen[SnHotGun2                   ].Name="SnHotGun2";                         //kevin 20130429   Hot Gun 2流量偵測

    Sen[SnAuto1UpPress              ].Name="SnAuto1UpPress";                     //JerryYang 20190423 新增unloader壓tray
    Sen[SnAuto2UpPress              ].Name="SnAuto2UpPress";
    Sen[SnAuto3UpPress              ].Name="SnAuto3UpPress";
    Sen[SnAuto4UpPress              ].Name="SnAuto4UpPress";                    //RogerYang 20250825 : 新增unloader壓tray(4~6)
    Sen[SnAuto5UpPress              ].Name="SnAuto5UpPress";
    Sen[SnAuto6UpPress              ].Name="SnAuto6UpPress";
    Sen[SnWaterLeakageUp            ].Name="SnWaterLeakageUp";                  //wei 20190617 漏水檢測
    Sen[SnWaterLeakageDown          ].Name="SnWaterLeakageDown";                //wei 20190617 漏水檢測
    Sen[SnWaterLeakagePlate         ].Name="SnWaterLeakagePlate";               //wei 20190617 漏水檢測

    Sen[SnLoaderTrackDetect         ].Name="SnLoaderTrackDetect";               //Sam 20200316 : Loader Detect Tray
    Sen[SnAuto1Z_Select1            ].Name="SnAuto1Z_Select1";                  //JerryYang 20200615 ART分離汽缸sensor
    Sen[SnAuto2Z_Select1            ].Name="SnAuto2Z_Select1";
    Sen[SnAuto3Z_Select1            ].Name="SnAuto3Z_Select1";
    Sen[SnAuto1Z_Select2            ].Name="SnAuto1Z_Select2";
    Sen[SnAuto2Z_Select2            ].Name="SnAuto2Z_Select2";
    Sen[SnAuto3Z_Select2            ].Name="SnAuto3Z_Select2";
    Sen[SnAuto1Z_Select3            ].Name="SnAuto1Z_Select3";
    Sen[SnAuto2Z_Select3            ].Name="SnAuto2Z_Select3";
    Sen[SnAuto3Z_Select3            ].Name="SnAuto3Z_Select3";
    Sen[SnAuto1Z_Select4            ].Name="SnAuto1Z_Select4";
    Sen[SnAuto2Z_Select4            ].Name="SnAuto2Z_Select4";
    Sen[SnAuto3Z_Select4            ].Name="SnAuto3Z_Select4";

    Sen[SnInFlipper1DeviceDetect    ].Name="SnInFlipper1DeviceDetect";          //Frank 20210612 : Flipper Function
    Sen[SnInFlipper2DeviceDetect    ].Name="SnInFlipper2DeviceDetect";
    Sen[SnInFlipper3DeviceDetect    ].Name="SnInFlipper3DeviceDetect";
    Sen[SnOutFlipper1DeviceDetect   ].Name="SnOutFlipper1DeviceDetect";
    Sen[SnOutFlipper2DeviceDetect   ].Name="SnOutFlipper2DeviceDetect";
    Sen[SnOutFlipper3DeviceDetect   ].Name="SnOutFlipper3DeviceDetect";

    Sen[SnLoaderCasstteDetect       ].Name="SnLoaderCasstteDetect";
    Sen[SnEmptyCasstteDetect        ].Name="SnEmptyCasstteDetect";
    Sen[SnColorCasstteDetect        ].Name="SnColorCasstteDetect";
    Sen[SnAuto1CasstteDetect        ].Name="SnAuto1CasstteDetect";
    Sen[SnAuto2CasstteDetect        ].Name="SnAuto2CasstteDetect";
    Sen[SnAuto3CasstteDetect        ].Name="SnAuto3CasstteDetect";
    Sen[SnAuto4CasstteDetect        ].Name="SnAuto4CasstteDetect";              //RogerYang 20250825 : 新增Auto4~6
    Sen[SnAuto5CasstteDetect        ].Name="SnAuto5CasstteDetect";
    Sen[SnAuto6CasstteDetect        ].Name="SnAuto6CasstteDetect";
    Sen[SnArm1SLK                   ].Name="SnArm1SLK";
    Sen[SnArm2SLK                   ].Name="SnArm2SLK";

    Sen[SnChamberHeatDetect         ].Name="SnChamberHeatDetect";               //JerryYang 20210107 : ChamberHeatDetect

    Sen[SnMRAuto21                  ].Name="SnMRAuto21";
    Sen[SnE84LREQ                   ].Name="SnE84LREQ";
    Sen[SnE84UREQ                   ].Name="SnE84UREQ";
    Sen[SnE84VA                     ].Name="SnE84VA";
    Sen[SnE84READY                  ].Name="SnE84READY";
    Sen[SnE84VS0                    ].Name="SnE84VS0";
    Sen[SnE84VS1                    ].Name="SnE84VS1";
    Sen[SnE84HOAVBL                 ].Name="SnE84HOAVBL";
    Sen[SnE84ES                     ].Name="SnE84ES";
    Sen[SnE84POWER                  ].Name="SnE84POWER";
    Sen[SnBufferTop                 ].Name="SnBufferTop";
    Sen[SnBufferCatch               ].Name="SnBufferCatch";
    Sen[SnCassetteOpen              ].Name="SnCassetteOpen";
    Sen[SnMultileEmpty_Door         ].Name="SnMultileEmpty_Door";
    Sen[SnMultileEmpty_MagazineTop  ].Name="SnMultileEmpty_MagazineTop";
    Sen[SnMultileEmpty_MagazineLow  ].Name="SnMultileEmpty_MagazineLow";
    Sen[SnMultileEmpty_MagazineCatch].Name="SnMultileEmpty_MagazineCatch";
    Sen[SnMultileEmpty_SelectHasTray].Name="SnMultileEmpty_SelectHasTray";
    Sen[SnMultileEmpty_HasTray      ].Name="SnMultileEmpty_HasTray";
    Sen[SnMultileEmpty_CornerPushCyPush].Name="SnMultileEmpty_CornerPushCyPush";
    Sen[SnMultileEmpty_CatchHasTray ].Name="SnMultileEmpty_CatchHasTray";
    Sen[SnSafeDoor20                ].Name="SnSafeDoor20";
    Sen[SnSafeDoor21                ].Name="SnSafeDoor21";
    Sen[SnLoadPort2Present          ].Name="SnLoadPort2Present";
    Sen[SnLoadPort2Placement1       ].Name="SnLoadPort2Placement1";
    Sen[SnLoadPort2Placement2       ].Name="SnLoadPort2Placement2";
    Sen[SnTrayBracket2Have          ].Name="SnTrayBracket2Have";
    Sen[SnCassetteArmIntoLoadPort   ].Name="SnCassetteArmIntoLoadPort";
    Sen[SnCassetteArmIntoLoadPort2  ].Name="SnCassetteArmIntoLoadPort2";
    Sen[SnBuffer7HaveTray           ].Name="SnBuffer7HaveTray";
    Sen[SnLoadPortAutoManual        ].Name="SnLoadPortAutoManual";
    Sen[SnLoadPort2AutoManual       ].Name="SnLoadPort2AutoManual";
    Sen[SnOHTIntoLoadPort           ].Name="SnOHTIntoLoadPort";
    Sen[SnOHTIntoLoadPort2          ].Name="SnOHTIntoLoadPort2";
    Sen[SnCassetteClose             ].Name="SnCassetteClose";
    Sen[SnCassetteOpen2             ].Name="SnCassetteOpen2";
    Sen[SnCassetteClose2            ].Name="SnCassetteClose2";
    Sen[SnCassetteSlotMap           ].Name="SnCassetteSlotMap";
    Sen[SnCassetteFloating          ].Name="SnCassetteFloating";
    Sen[SnMultileEmpty_ScanTrayID   ].Name="SnMultileEmpty_ScanTrayID";
    Sen[SnE84_2_VALID               ].Name="SnE84_2_VALID";
    Sen[SnE84_2_CS0                 ].Name="SnE84_2_CS0";
    Sen[SnE84_2_CS1                 ].Name="SnE84_2_CS1";
    Sen[SnE84_2_AMAVBL              ].Name="SnE84_2_AMAVBL";
    Sen[SnE84_2_TRREQ               ].Name="SnE84_2_TRREQ";
    Sen[SnE84_2_BUSY                ].Name="SnE84_2_BUSY";
    Sen[SnE84_2_COMPT               ].Name="SnE84_2_COMPT";
    Sen[SnE84_2_CONT                ].Name="SnE84_2_CONT";
    Sen[SnE84_2_GO                  ].Name="SnE84_2_GO";
    Sen[SnE84_1_VALID               ].Name="SnE84_1_VALID";
    Sen[SnE84_1_CS0                 ].Name="SnE84_1_CS0";
    Sen[SnE84_1_CS1                 ].Name="SnE84_1_CS1";
    Sen[SnE84_1_AMAVBL              ].Name="SnE84_1_AMAVBL";
    Sen[SnE84_1_TRREQ               ].Name="SnE84_1_TRREQ";
    Sen[SnE84_1_BUSY                ].Name="SnE84_1_BUSY";
    Sen[SnE84_1_COMPT               ].Name="SnE84_1_COMPT";
    Sen[SnE84_1_CONT                ].Name="SnE84_1_CONT";
    Sen[SnE84_1_GO                  ].Name="SnE84_1_GO";

    Sen[SnLoaderTrayHasTray_AGV     ].Name="SnLoaderTrayHasTray_AGV";           //kevin 20220520 add AGV load
    Sen[SnEmptyTrayHasTray_AGV      ].Name="SnEmptyTrayHasTray_AGV";            //kevin 20220520 add AGV load
    Sen[SnColorTrayHasTray_AGV      ].Name="SnColorTrayHasTray_AGV";            //kevin 20220520 add AGV load
    Sen[SnMultileEmptyCatch_HasTray ].Name="SnMultileEmptyCatch_HasTray";
    Sen[SnCSTHoldDownOff            ].Name="SnCSTHoldDownOff";
    Sen[SnCSTHoldDown2Off           ].Name="SnCSTHoldDown2Off";
    Sen[SnMultileEmpty_ScanTray2DID ].Name="SnMultileEmpty_ScanTray2DID";
    Sen[SnDieDetect_Sh1             ].Name="SnDieDetect_Sh1";
    Sen[SnDieDetect_Sh2             ].Name="SnDieDetect_Sh2";
    Sen[SnBuffer1_Placement1        ].Name="SnBuffer1_Placement1";
    Sen[SnBuffer2_Placement1        ].Name="SnBuffer2_Placement1";
    Sen[SnBuffer3_Placement1        ].Name="SnBuffer3_Placement1";
    Sen[SnBuffer4_Placement1        ].Name="SnBuffer4_Placement1";
    Sen[SnBuffer5_Placement1        ].Name="SnBuffer5_Placement1";
    Sen[SnBuffer6_Placement1        ].Name="SnBuffer6_Placement1";
    Sen[SnBuffer7_Placement1        ].Name="SnBuffer7_Placement1";
    Sen[SnBuffer8_Placement1        ].Name="SnBuffer8_Placement1";
    Sen[SnBuffer9_Placement1        ].Name="SnBuffer9_Placement1";
    Sen[SnBuffer10_Placement1       ].Name="SnBuffer10_Placement1";
    Sen[SnStackedTrayYDetect_Loader ].Name="SnStackedTrayYDetect_Loader";
    Sen[SnStackedTrayYDetect_Elevator2].Name="SnStackedTrayYDetect_Elevator2";

    Sen[SnInAreaAlignmentSenX       ].Name="SnInAreaAlignmentSenX";             //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
    Sen[SnInAreaAlignmentSenY       ].Name="SnInAreaAlignmentSenY";
    Sen[SnOutAreaAlignmentSenX      ].Name="SnOutAreaAlignmentSenX";
    Sen[SnOutAreaAlignmentSenY      ].Name="SnOutAreaAlignmentSenY";
    Sen[SnInAreaAlignmentSenZ       ].Name="SnInAreaAlignmentSenZ";             //KenHsieh 20211110 ： AutoAlignment add Auto Z
    Sen[SnOutAreaAlignmentSenZ      ].Name="SnOutAreaAlignmentSenZ";            //KenHsieh 20211110 ： AutoAlignment add Auto Z

    Sen[SnSht1Left                  ].Name="SnSht1Left";                        //kevin 20220512 add SHUTTLE Left 位置偏移
    Sen[SnSht1Right                 ].Name="SnSht1Right";                       //kevin 20220512 add SHUTTLE Right 位置偏移
    Sen[SnSht2Left                  ].Name="SnSht2Left";                        //kevin 20220512 add SHUTTLE Left 位置偏移
    Sen[SnSht2Right                 ].Name="SnSht2Right";                       //kevin 20220512 add SHUTTLE Right 位置偏移

    Sen[SnAseTrayBufferLeft         ].Name="SnAseTrayBufferLeft";               //kevin 20220709 ASEKH 左邊放空TRAY 軟體不須判斷
    Sen[SnAseTrayBufferRight        ].Name="SnAseTrayBufferRight";              //kevin 20220709 ASEKH 右邊放空TRAY  軟體不須判斷

    Sen[SnATC1Ready                 ].Name="SnATC1Ready";                       //Jimmychiu 20210902 add: ATC Winway IO ready
    Sen[SnATC2Ready                 ].Name="SnATC2Ready";
    Sen[SnATC3Ready                 ].Name="SnATC3Ready";
    Sen[SnATC4Ready                 ].Name="SnATC4Ready";

    Sen[SnPlate1TempOverDetect      ].Name="SnPlate1TempOverDetect";            //Ztex 2023.04.13 Add HT-1032 IO
    Sen[SnPlate2TempOverDetect      ].Name="SnPlate2TempOverDetect";
    Sen[SnShuttle1TempOverDetect    ].Name="SnShuttle1TempOverDetect";
    Sen[SnShuttle2TempOverDetect    ].Name="SnShuttle2TempOverDetect";
    Sen[SnHead1TempOverDetect       ].Name="SnHead1TempOverDetect";
    Sen[SnHead2TempOverDetect       ].Name="SnHead2TempOverDetect";
    Sen[SnHead5TempOverDetect       ].Name="SnHead5TempOverDetect";
    Sen[SnHead6TempOverDetect       ].Name="SnHead6TempOverDetect";
    Sen[SnHumidityAnomaly1Detect    ].Name="SnHumidityAnomaly1Detect";
    Sen[SnHumidityAnomaly2Detect    ].Name="SnHumidityAnomaly2Detect";
    Sen[SnHumidityAnomaly3Detect    ].Name="SnHumidityAnomaly3Detect";
    Sen[SnDryAirIsEnough            ].Name="SnDryAirIsEnough";
    Sen[SnIonBarInAirIsEnough       ].Name="SnIonBarInAirIsEnough";
    Sen[SnIonBarOutAirIsEnough      ].Name="SnIonBarOutAirIsEnough";
    Sen[SnSafeDoor1Hatchway         ].Name="SnSafeDoor1Hatchway";
    Sen[SnSafeDoor2Hatchway         ].Name="SnSafeDoor2Hatchway";
    Sen[SnSafeDoor3Hatchway         ].Name="SnSafeDoor3Hatchway";
    Sen[SnSafeDoor4Hatchway         ].Name="SnSafeDoor4Hatchway";
    Sen[SnSafeDoor5Hatchway         ].Name="SnSafeDoor5Hatchway";
    Sen[SnSafeDoor6Hatchway         ].Name="SnSafeDoor6Hatchway";
    Sen[SnSafeDoor7Hatchway         ].Name="SnSafeDoor7Hatchway";
    Sen[SnSafeDoor8Hatchway         ].Name="SnSafeDoor8Hatchway";
    Sen[SnSafeDoor9Hatchway         ].Name="SnSafeDoor9Hatchway";
    Sen[SnSafeDoor10Hatchway        ].Name="SnSafeDoor10Hatchway";
    Sen[SnSafeDoor11Hatchway        ].Name="SnSafeDoor11Hatchway";
    Sen[SnSafeDoor6PosFixPickPlace  ].Name="SnSafeDoor6PosFixPickPlace";
    Sen[SnTriTempSafeDoor6Lock      ].Name="SnTriTempSafeDoor6Lock";
    Sen[SnTrayArmZSafePos           ].Name="SnTrayArmZSafePos";
    Sen[SnEnhaustAirFanAlarmDetect  ].Name="SnEnhaustAirFanAlarmDetect";        //Ztex 2023.04.26 Add HT-1032 IO Exhaust Air
    Sen[SnDockingAreaOpenCheck      ].Name="SnDockingAreaOpenCheck";            //Ztex 2023.05.02 Add HT-1032 IO Docking Area Open Check
    Sen[SnDewPointDetectIndexArm1   ].Name="SnDewPointDetectIndexArm1";         //Hmy 20170603  add By 三溫機 露點SENSOR Arm1偵測
    Sen[SnDewPointDetectIndexArm2   ].Name="SnDewPointDetectIndexArm2";         //Hmy 20170603  add By 三溫機 露點SENSOR Arm2偵測

    Sen[SnMagazineSafeDoor          ].Name="SnMagazineSafeDoor";                //JerryYang 20221215 : add Magazine
    Sen[SnMagazineDetect            ].Name="SnMagazineDetect";
    Sen[SnMagazineDetectTop         ].Name="SnMagazineDetectTop";
    Sen[SnMagazineTrackDetect       ].Name="SnMagazineTrackDetect";
    Sen[SnMagazineTrackDetect2      ].Name="SnMagazineTrackDetect2";            //Sam 20221116 : Magazine TrayArm 自動補 Tray
    Sen[SnMagazineTrackSelectDetect ].Name="SnMagazineTrackSelectDetect";
    Sen[SnMagazineSafeDoor2         ].Name="SnMagazineSafeDoor2";
    Sen[SnMagazineSafeDoor3         ].Name="SnMagazineSafeDoor3";
    Sen[SnMagazineHasTrayInside     ].Name="SnMagazineHasTrayInside";

    Sen[SnLoaderSeparateHasTray     ].Name="SnLoaderSeparateHasTray";
    Sen[SnEmptySeparateHasTray      ].Name="SnEmptySeparateHasTray";
    Sen[SnColorSeparateHasTray      ].Name="SnColorSeparateHasTray";
    Sen[SnAuto1SeparateHasTray      ].Name="SnAuto1SeparateHasTray";
    Sen[SnAuto2SeparateHasTray      ].Name="SnAuto2SeparateHasTray";
    Sen[SnAuto3SeparateHasTray      ].Name="SnAuto3SeparateHasTray";

    Sen[SenLoaderCCWDete_2          ].Name="SenLoaderCCWDete_2";
    Sen[SenEmptyCCWDete_2           ].Name="SenEmptyCCWDete_2";
    Sen[SenColorCCWDete_2           ].Name="SenColorCCWDete_2";
    Sen[SenAuto1CCWDete_2           ].Name="SenAuto1CCWDete_2";
    Sen[SenAuto2CCWDete_2           ].Name="SenAuto2CCWDete_2";
    Sen[SenAuto3CCWDete_2           ].Name="SenAuto3CCWDete_2";

    Sen[SnOpenDoorChangeKit1        ].Name="SnOpenDoorChangeKit1";
    Sen[SnOpenDoorChangeKit2        ].Name="SnOpenDoorChangeKit2";
    Sen[SnOpenDoorChangeKit3        ].Name="SnOpenDoorChangeKit3";

    Sen[SnIndexCylinderDetectHead1  ].Name="SnIndexCylinderDetectHead1";
    Sen[SnIndexCylinderDetectHead2  ].Name="SnIndexCylinderDetectHead2";
    Sen[SnIndexCylinderDetectHead5  ].Name="SnIndexCylinderDetectHead5";
    Sen[SnIndexCylinderDetectHead6  ].Name="SnIndexCylinderDetectHead6";

    Sen[SnIonBar1                   ].Name="SnIonBar1";
    Sen[SnIonBar2                   ].Name="SnIonBar2";
    Sen[SnIonBar3                   ].Name="SnIonBar3";
    Sen[SnIonBar4                   ].Name="SnIonBar4";
    Sen[SnIonBar5                   ].Name="SnIonBar5";
    Sen[SnIonBar6                   ].Name="SnIonBar6";
    Sen[SnIonBar7                   ].Name="SnIonBar7";
    Sen[SnIonBar8                   ].Name="SnIonBar8";
    Sen[SnIonBar9                   ].Name="SnIonBar9";

    Sen[SnTrayArmHasTray            ].Name="SnTrayArmHasTray";
    Sen[SnMCUSensor1                ].Name="SnMCUSensor1";                      //Jimmychiu 20230630 : add color sensor MU-N in Loader
    Sen[SnMCUSensor2                ].Name="SnMCUSensor2";                      //Jimmychiu 20230630 : add color sensor MU-N in Loader
    Sen[SnMCUSensor3                ].Name="SnMCUSensor3";                      //Jimmychiu 20230630 : add color sensor MU-N in Loader
    Sen[SnMCUSensor4                ].Name="SnMCUSensor4";                      //Jimmychiu 20230630 : add color sensor MU-N in Loader

    Sen[SnAuto4_Tray_Car            ].Name="SnAuto4_Tray_Car";                  //Steven 20230907 : For HT-9011UC
    Sen[SnAuto4PreDete              ].Name="SnAuto4PreDete";
    Sen[SnAuto4TrackDetect          ].Name="SnAuto4TrackDetect";
    Sen[SnAuto4FixCyPush            ].Name="SnAuto4FixCyPush";
    Sen[SnAuto4EdgePush             ].Name="SnAuto4EdgePush";
    Sen[SnAuto4UpSafedetect         ].Name="SnAuto4UpSafedetect";
    Sen[SnAuto4SeparateHasTray      ].Name="SnAuto4SeparateHasTray";
    Sen[SnAuto4TrayHasTray          ].Name="SnAuto4TrayHasTray";
    Sen[SnAuto4CWPreDetect          ].Name="SnAuto4CWPreDetect";
    Sen[SenAuto4CCWDete_2           ].Name="SenAuto4CCWDete_2";
    Sen[SnAuto4Z_Select1            ].Name="SnAuto4Z_Select1";
    Sen[SnAuto4Z_Select2            ].Name="SnAuto4Z_Select2";
    Sen[SnAuto4Z_Select3            ].Name="SnAuto4Z_Select3";
    Sen[SnAuto4Z_Select4            ].Name="SnAuto4Z_Select4";
    Sen[SnAuto4IsPreAlarm           ].Name="SnAuto4IsPreAlarm";
    Sen[SnAuto5_Tray_Car            ].Name="SnAuto5_Tray_Car";
    Sen[SnAuto5PreDete              ].Name="SnAuto5PreDete";
    Sen[SnAuto5TrackDetect          ].Name="SnAuto5TrackDetect";
    Sen[SnAuto5FixCyPush            ].Name="SnAuto5FixCyPush";
    Sen[SnAuto5EdgePush             ].Name="SnAuto5EdgePush";
    Sen[SnAuto5UpSafedetect         ].Name="SnAuto5UpSafedetect";
    Sen[SnAuto5SeparateHasTray      ].Name="SnAuto5SeparateHasTray";
    Sen[SnAuto5TrayHasTray          ].Name="SnAuto5TrayHasTray";
    Sen[SnAuto5CWPreDetect          ].Name="SnAuto5CWPreDetect";
    Sen[SenAuto5CCWDete_2           ].Name="SenAuto5CCWDete_2";
    Sen[SnAuto5Z_Select1            ].Name="SnAuto5Z_Select1";
    Sen[SnAuto5Z_Select2            ].Name="SnAuto5Z_Select2";
    Sen[SnAuto5Z_Select3            ].Name="SnAuto5Z_Select3";
    Sen[SnAuto5Z_Select4            ].Name="SnAuto5Z_Select4";
    Sen[SnAuto5IsPreAlarm           ].Name="SnAuto5IsPreAlarm";
    Sen[SnAuto6_Tray_Car            ].Name="SnAuto6_Tray_Car";
    Sen[SnAuto6PreDete              ].Name="SnAuto6PreDete";
    Sen[SnAuto6TrackDetect          ].Name="SnAuto6TrackDetect";
    Sen[SnAuto6FixCyPush            ].Name="SnAuto6FixCyPush";
    Sen[SnAuto6EdgePush             ].Name="SnAuto6EdgePush";
    Sen[SnAuto6UpSafedetect         ].Name="SnAuto6UpSafedetect";
    Sen[SnAuto6SeparateHasTray      ].Name="SnAuto6SeparateHasTray";
    Sen[SnAuto6TrayHasTray          ].Name="SnAuto6TrayHasTray";
    Sen[SnAuto6CWPreDetect          ].Name="SnAuto6CWPreDetect";
    Sen[SenAuto6CCWDete_2           ].Name="SenAuto6CCWDete_2";
    Sen[SnAuto6Z_Select1            ].Name="SnAuto6Z_Select1";
    Sen[SnAuto6Z_Select2            ].Name="SnAuto6Z_Select2";
    Sen[SnAuto6Z_Select3            ].Name="SnAuto6Z_Select3";
    Sen[SnAuto6Z_Select4            ].Name="SnAuto6Z_Select4";
    Sen[SnAuto6IsPreAlarm           ].Name="SnAuto6IsPreAlarm";

    Sen[SnAuto4IsFull               ].Name="SnAuto4IsFull";
    Sen[SnAuto5IsFull               ].Name="SnAuto5IsFull";
    Sen[SnAuto6IsFull               ].Name="SnAuto6IsFull";

    Sen[SnAuto4TrayDetect           ].Name="SnAuto4TrayDetect";
    Sen[SnAuto5TrayDetect           ].Name="SnAuto5TrayDetect";
    Sen[SnAuto6TrayDetect           ].Name="SnAuto6TrayDetect";

    Sen[SnTrayArmTrayDetect1        ].Name="SnTrayArmTrayDetect1";
    Sen[SnTrayArmTrayDetect2        ].Name="SnTrayArmTrayDetect2";
    Sen[SnTrayArmTrayDetect3        ].Name="SnTrayArmTrayDetect3";
    Sen[SnTrayArmTrayDetect4        ].Name="SnTrayArmTrayDetect4";
    Sen[SnSafeMode                  ].Name="SnSafeMode";                        //jou 20231016 : CE PLC safe mode
    Sen[SnWaterLeakageChiller       ].Name="SnWaterLeakageChiller";             //jou 20231019 : Water Leakage Chiller
    Sen[SnSmokeDetect01             ].Name="SnSmokeDetect01";                   //Sam 20240112 : 新增煙霧偵測
    Sen[SnIonBar10                  ].Name="SnIonBar10";
    Sen[SnIonBar11                  ].Name="SnIonBar11";

    Sen[SnTesterAlarm               ].Name="SnTesterAlarm";
    Sen[SnDoubleLoadDetection       ].Name="SnDoubleLoadDetection";             //Steven 20240426 : 偵測loader疊盤

    Sen[SnLoader_Detect             ].Name="SnLoader_Detect";                   //Ifor 20211005 add Tray 載盤上升下降前判斷是否有異常
    Sen[SnEmpty_Detect              ].Name="SnEmpty_Detect";
    Sen[SnColor_Detect              ].Name="SnColor_Detect";
    Sen[SnAuto1_Detect              ].Name="SnAuto1_Detect";
    Sen[SnAuto2_Detect              ].Name="SnAuto2_Detect";
    Sen[SnAuto3_Detect              ].Name="SnAuto3_Detect";
    Sen[SnAuto4_Detect              ].Name="SnAuto4_Detect";
    Sen[SnAuto5_Detect              ].Name="SnAuto5_Detect";
    Sen[SnAuto6_Detect              ].Name="SnAuto6_Detect";
    Sen[SnIonFanCar                 ].Name="SnIonFanCar";                       //Ifor 20220310 add: Bin Car Ion Fan Check
    Sen[SnRKSafeLock                ].Name="SnRKSafeLock";                      //KenHsieh 20211228 : 區分實體IO與通訊面板
    Sen[SnIonFanCarPower            ].Name="SnIonFanCarPower";                  //Ifor 20220816 add: Bin Car Ion Fan Power Check
    Sen[SnEPFlowmeter               ].Name="SnEPFlowmeter";                     //Ifor 20240326 add: EP流量計監控
    Sen[SnChamberDryAir             ].Name="SnChamberDryAir";                   //Ifor 20240919 add: Chamber Dry Air

    Sen[SnLoad2IsFull               ].Name="SnLoad2IsFull";                     //Steven 20240822 : For HT-9046AU
    Sen[SnLoad2IsPreAlarm           ].Name="SnLoad2IsPreAlarm";
    Sen[SnLoad2TrayHasTray_AGV      ].Name="SnLoad2TrayHasTray_AGV";
    Sen[SnLoad2TrayHasTray_ART      ].Name="SnLoad2TrayHasTray_ART";
    Sen[SnLoad2TrayHasTray          ].Name="SnLoad2TrayHasTray";
    Sen[SnLoad2CarHasTray           ].Name="SnLoad2CarHasTray";
    Sen[SnCheckTray2Direction       ].Name="SnCheckTray2Direction";
    Sen[SenLoad2CCWDete             ].Name="SenLoad2CCWDete";
    Sen[SnLoad2UpSafedetect         ].Name="SnLoad2UpSafedetect";
    Sen[SenLoad2CCWDete_2           ].Name="SenLoad2CCWDete_2";
    Sen[SnLoad2TrackDetect          ].Name="SnLoad2TrackDetect";
    Sen[SnLoad2FixCyPush            ].Name="SnLoad2FixCyPush";
    Sen[SnLoad2EdgePush             ].Name="SnLoad2EdgePush";
    Sen[SnLoad2SeparateHasTray      ].Name="SnLoad2SeparateHasTray";
    Sen[SnLoad2SureTray             ].Name="SnLoad2SureTray";
    Sen[SnLoad2PreDete              ].Name="SnLoad2PreDete";
    Sen[SnCheckLoad2Direction       ].Name="SnCheckLoad2Direction";
    Sen[SnLoad2CasstteDetect        ].Name="SnLoad2CasstteDetect";
    Sen[SnDoubleLoad2Detection      ].Name="SnDoubleLoad2Detection";
    Sen[SnLoad2UpPress              ].Name="SnLoad2UpPress";
    Sen[SnAuto1HasCoverTray         ].Name="SnAuto1HasCoverTray";               //JerryYang 20241021 : Unloader增加第二組Sensor檢查是否有cover tray
    Sen[SnAuto2HasCoverTray         ].Name="SnAuto2HasCoverTray";
    Sen[SnAuto3HasCoverTray         ].Name="SnAuto3HasCoverTray";
    Sen[SnAuto4HasCoverTray         ].Name="SnAuto4HasCoverTray";
    Sen[SnAuto5HasCoverTray         ].Name="SnAuto5HasCoverTray";
    Sen[SnAuto6HasCoverTray         ].Name="SnAuto6HasCoverTray";

    Sen[SnElectricControlBox        ].Name="SnElectricControlBox";              //ChungHung 20230718 add for Safe plc
    Sen[SnAllSafeDoor               ].Name="SnAllSafeDoor";                     //ChungHung 20230718 add for Safe plc
    Sen[SnAllEMG                    ].Name="SnAllEMG";                          //KenHsieh 20250212 : 新增PLC 斷線可瞬間判斷EMG及安全門
    Sen[SnLoadIonGun                ].Name="SnLoadIonGun";                      //Ifor 20230427 add:Loader Ionizer Gun
    Sen[SnLoadLightGat              ].Name="SnLoadLightGat";                    //Ifor 20230427 add:LD/ULD light gat

    Sen[SnLightZORG                 ].Name="SnLightZORG";
    Sen[SnLightZINP                 ].Name="SnLightZINP";
    Sen[SnLightZREADY               ].Name="SnLightZREADY";
    Sen[SnLightZSERVO               ].Name="SnLightZSERVO";

    Sen[SnFixedSeatTLOn             ].Name="SnFixedSeatTLOn";
    Sen[SnFixedSeatTLOff            ].Name="SnFixedSeatTLOff";
    Sen[SnFixedSeatTROn             ].Name="SnFixedSeatTROn";
    Sen[SnFixedSeatTROff            ].Name="SnFixedSeatTROff";
    Sen[SnFixedSeatBLOn             ].Name="SnFixedSeatBLOn";
    Sen[SnFixedSeatBLOff            ].Name="SnFixedSeatBLOff";
    Sen[SnFixedSeatBROn             ].Name="SnFixedSeatBROn";
    Sen[SnFixedSeatBROff            ].Name="SnFixedSeatBROff";

    Sen[SnTopBtmRotateLockOn1       ].Name="SnTopBtmRotateLockOn1";
    Sen[SnTopBtmRotateLockOn2       ].Name="SnTopBtmRotateLockOn2";

    Sen[SnTopBtmAirMaxAlarm         ].Name="SnTopBtmAirMaxAlarm";
    Sen[SnTopBtmAirMinAlarm         ].Name="SnTopBtmAirMinAlarm";
    Sen[SnTopBtmAOIR180             ].Name="SnTopBtmAOIR180";
    //Sen[SnChamberDryAir             ].Name="SnChamberDryAir";                 //RogerYang 20260407 : Mark重複宣告  //Ifor 20240919 add: Chamber Dry Air

    Sen[SnLoadCarRFIDSW             ].Name="SnLoadCarRFIDSW";                   //RogerYang 20250828 : add for Loader Rotate Arm

    Sen[SnLoaderCarrier1             ].Name="SnLoaderCarrier1";                 //Ifor 20251216 add:Boat Carrier
    Sen[SnAuto1Carrier1              ].Name="SnAuto1Carrier1";
    Sen[SnAuto2Carrier1              ].Name="SnAuto2Carrier1";
    Sen[SnLoaderCarrier2             ].Name="SnLoaderCarrier2";
    Sen[SnAuto1Carrier2              ].Name="SnAuto1Carrier2";
    Sen[SnAuto2Carrier2              ].Name="SnAuto2Carrier2";
    Sen[SnLoaderBoatActDetect        ].Name="SnLoaderBoatActDetect";
    Sen[SnAuto1BoatActDetect         ].Name="SnAuto1BoatActDetect";
    Sen[SnAuto2BoatActDetect         ].Name="SnAuto2BoatActDetect";

    Sen[SnDailyCorrelation_Open      ].Name="SnDailyCorrelation_Open";      //KaiChen 20200525 ：Daily Correlation Function
    Sen[SnDailyCorrelation_Close     ].Name="SnDailyCorrelation_Close";     //KaiChen 20200525 ：Daily Correlation Function
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//  InitialSensor  (golden cinitial.cpp:2471-2768)
//  Mirrors InitialSwitch's structure: the IO_CARD_TYPE==NewIO_MN200||
//  PCI_P64C64 path is FAITHFUL/active (Sen[] already carries every field it
//  touches, HSys.mapIOTable/IOTable are wired); the sensor*.db (BDE)
//  else-branch is gated (TTable/DataModule1 missing, blocker (2)).
//  SetIOTableByNUEC1() (EtherCAT/MyNUEC1.cpp) is not ported yet either --
//  gated at its single call site, the rest of the TTL/iControlPanelMode/
//  safe-door tail is active.
//------------------------------------------------------------------------------
void InitialSensor()
{
    int iSn;
    bool bHasSensor;                                                            //Stevem 20210625 : 修正新的CSV IO表讀檔方式
    AnsiString S;
    AnsiString str;

    if(IO_CARD_TYPE==NewIO_MN200 ||
       IO_CARD_TYPE==PCI_P64C64)                                                //Steven 20231218 HT7080B
    {
        InitialSensorName();
        for(int i=0; i<MAX_SENSOR_ITEM; i++)
        {
            bHasSensor=false;
            Sen[i].Enable=false;
            if(Sen[i].Name=="")
                continue;

            HSys.mapIOTableIter=HSys.mapIOTable.find(Sen[i].Name);
            if(HSys.mapIOTableIter!=HSys.mapIOTable.end())
            {
                bHasSensor=true;
                iSn=atoi(HSys.mapIOTable[Sen[i].Name].c_str());
            }
            else
            {
                str.sprintf("Can not find sensor %s", Sen[i].Name);
//                continue;
            }

            if(bHasSensor)
            {
                Sen[i].ISABase      =HSys.IOTable[iSn]->iISABase;
                if(HSys.IOTable[iSn]->iEnable==1)
                    Sen[i].Enable   =true;
                Sen[i].Using        =(HSys.IOTable[iSn]->iPort==-1)?AnsiString(""):AnsiString(HSys.IOTable[iSn]->iPort);

                if(Sen[i].ISABase==ePLCbase)                                    //KenHsieh 20260421 : 新增PLC判斷
                {
                    if(Sen[i].Using=="")
                        Sen[i].Enable=false;
                    Sen[i].Ring     =0;
                    Sen[i].IP       =0;
                }
                else
                {
                    Sen[i].Ring     =HSys.IOTable[iSn]->iLane;
                    Sen[i].IP       =HSys.IOTable[iSn]->iIP;
                }

                Sen[i].Port         =HSys.IOTable[iSn]->iPort;
                Sen[i].Bit          =HSys.IOTable[iSn]->iBit;
                Sen[i].Type         =HSys.IOTable[iSn]->iInType;
            }
            else
            {
                Sen[i].Enable=false;
            }
            #ifdef SOFT_SIMULTE
                #ifdef SOFT_SIMULTE_EtherCAT
                    if(Sen[i].ISABase==ePCI1203)                                //Sam 20230724 : 測試 SensorEtherCAT 用
                    {
                    }
                    else
                    {
                        Sen[i].Enable=false;
                    }
                #else
                    Sen[i].Enable=false;
                #endif
            #endif

            S=Sen[i].Name;
            if(Enable_PLCSafety_IO &&                                           //Austin 20190531.01 增加PLC_IO模組及安全相關IO.
               Sen[i].ISABase==ePLCbase)                                        // 20230821 Joseph , Add Heater door //
            {
                bPLCIO[Sen[i].Port][Sen[i].Bit]=true;
                Sen[i].Type=1;
                Sen[i].Enable=1;
            }

            #ifndef SOFT_SIMULTE
            if(Sen[i].Enable)
            #endif
            {
                MyLaneIO.SetUseIP(Sen[i].Ring, Sen[i].IP, Sen[i].ISABase, Sen[i].Name, false);    //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
            }
        }
    }
#if 0 // TODO(GA2-C1): blocked by TTable/DataModule1->SensorTable (BDE not ported, see database.h's own
      //   "WHAT IS STILL GATED" banner) -- golden cinitial.cpp:2562-2704
      // AI(W906-GA2-C1-fix) 20260804: M-2 WARNING for the un-gating wave -- this gate holds
      // relocated declarations followed by a bare `else`; flipping it on AS-IS is a parse
      // error. Re-derive declaration placement from golden when un-gating.
    bool flag;
    AnsiString S1, sDBDir;
    TTable *T;

    else
    {
        if(MachineTypeChoice==Type_HT9046)                                      //9046
        {
            sDBDir=CurrentDir+"\\system\\sensor_9046.db";                       //JerryYang 20150903 檢查Database檔是否存在
            if(FileExists(sDBDir))
            {
                DataModule1->SensorTable->TableName=sDBDir;
            }
            else
            {
                ShowMyMessage("sensor_9046.db does not exist");
                return;
            }
        }
        else if(MachineTypeChoice==Type_HT9046_LS ||
                MachineTypeChoice==Type_HT1032)
        {
            sDBDir=CurrentDir+"\\system\\Sensor_9046LS.db";
            if(FileExists(sDBDir))
            {
                DataModule1->SensorTable->TableName=sDBDir;
            }
            else
            {
                ShowMyMessage("Sensor_9046LS.db does not exist");
                return;
            }
        }
        else if(USE_46_SENSOR_DB)                                               //Steven 20131008 : HT9045使用46配電
        {
            sDBDir=CurrentDir+"\\system\\sensor_9046.db";
            if(FileExists(sDBDir))
            {
                DataModule1->SensorTable->TableName=sDBDir;
            }
            else
            {
                ShowMyMessage("sensor_9046.db does not exist");
                return;
            }
        }
        else if(MachineTypeChoice==Type_HT9045_12Site)                          //ChungHung 20130507 add HT9045 updata for 12site 517
        {
            sDBDir=CurrentDir+"\\system\\sensor.db";
            if(FileExists(sDBDir))
            {
                DataModule1->SensorTable->TableName=sDBDir;
            }
            else
            {
                ShowMyMessage("sensor.db does not exist");
                return;
            }
        }
        else
        {
            sDBDir=CurrentDir+"\\system\\sensor.db";
            if(FileExists(sDBDir))
            {
                DataModule1->SensorTable->TableName=sDBDir;
            }
            else
            {
                ShowMyMessage("sensor.db does not exist");
                return;
            }
        }
        T=DataModule1->SensorTable;
        T->Open();
        InitialSensorName();
        for(int i=0; i<MAX_SENSOR_ITEM; i++)
        {
            if(Sen[i].Name=="")
                continue;
            T->First();
            flag=false;
            while(!T->Eof)
            {
                S=T->FieldByName("SensorName")->AsString;
                if(S==Sen[i].Name)
                {
                    flag=true;
                    break;
                }
                T->Next();
            }

            if(flag==false)
            {
                str.sprintf("Can not find sensor name:%s", Sen[i].Name);
            }
            else
            {
                Sen[i].ISABase      =atoi(T->FieldByName("ISABase")->AsString.c_str());
                Sen[i].Enable       =atoi(T->FieldByName("Enable")->AsString.c_str());
                if(Sen[i].ISABase==eISABase ||                                  //Alick 20160809 modify for PIC-1735U
                   Sen[i].ISABase==ePCI1735U ||
                   Sen[i].ISABase==ePLCbase)
                {
                    Sen[i].Using    =T->FieldByName("Port")->AsString;
                    if(Sen[i].Using=="")
                        Sen[i].Enable=false;
                    Sen[i].Ring     =0;
                    Sen[i].IP       =0;
                    Sen[i].Port     =HexStrToInt(T->FieldByName("Port")->AsString.c_str());
                    Sen[i].Bit      =atoi(T->FieldByName("Bit")->AsString.c_str());
                }
                else
                {
                    Sen[i].Using    =T->FieldByName("Lane")->AsString;
                    if(Sen[i].Using!="")
                        Sen[i].Ring =atoi(Sen[i].Using.c_str());
                    else
                        Sen[i].Enable=false;
                    Sen[i].IP       =atoi(T->FieldByName("IP")->AsString.c_str());
                    Sen[i].Port     =atoi(T->FieldByName("Port")->AsString.c_str());
                    Sen[i].Bit      =atoi(T->FieldByName("Bit")->AsString.c_str());
                }
                Sen[i].Type         =atoi(T->FieldByName("InType")->AsString.c_str());

                if(Enable_PLCSafety_IO &&                                       //Austin 20190531.01 增加PLC_IO模組及安全相關IO.
                   Sen[i].ISABase==ePLCbase)                                    // 20230821 Joseph , Add Heater door //
                {
                    bPLCIO[Sen[i].Port][Sen[i].Bit]=true;
                    Sen[i].Type=1;
                    Sen[i].Enable=1;
                }

                #ifdef SOFT_SIMULTE
                    Sen[i].Enable=false;
                #endif

                #ifndef SOFT_SIMULTE
                if(Sen[i].Enable)
                #endif
                {
                    MyLaneIO.SetUseIP(Sen[i].Ring, Sen[i].IP, Sen[i].ISABase, Sen[i].Name, false);    //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
                }
            }
        }
        T->Close();
    }
#endif

    for(int i=0; i<10; i++)
    {
        Sen[SenBit0+i ].Enable=true;
        Sen[SenBit10+i].Enable=true;
    }

    for(int i=0; i<10; i++)
    {
        CopySensor(&Sen[SenBit0+i ], &SenBackUp[BackSenBit0+i]);
        CopySensor(&Sen[SenBit10+i], &SenBackUp[BackSenBit10+i]);
    }

    if(CosFunction.bTTLCanUse8Site)                                             //Alick 20161011 (Steven) : TTL支援8Site
    {
        for(int i=0; i<20; i++)
        {
            Sen[SenBit20+i].Enable=true;
            CopySensor(&Sen[SenBit20+i], &SenBackUp[BackSenBit20+i]);
        }
    }

    if(iControlPanelMode==1)                                                    //Steven 20250317 : 使用通訊式面板的要disable
    {
        Sen[SnFKPowerOff                ].Enable=false;
        Sen[SnFKPowerOn                 ].Enable=false;
        Sen[SnFKReset                   ].Enable=false;
        Sen[SnFKPause                   ].Enable=false;
        Sen[SnFKHome                    ].Enable=false;
        Sen[SnFKStart                   ].Enable=false;
        Sen[SnFKOneCycle                ].Enable=false;
        Sen[SnFKRetry                   ].Enable=false;
        Sen[SnFKSkip                    ].Enable=false;
        Sen[SnFKCleanOut                ].Enable=false;
        Sen[SnFKTrayFeed                ].Enable=false;
        Sen[SnFKTrayEnd                 ].Enable=false;
        Sen[SnFKAlarmReset              ].Enable=false;
        Sen[SnFKCoverOpen               ].Enable=false;
        Sen[SnRKPowerOff                ].Enable=false;
        Sen[SnRKPowerOn                 ].Enable=false;
        Sen[SnRKReset                   ].Enable=false;
        Sen[SnRKPause                   ].Enable=false;
        Sen[SnRKHome                    ].Enable=false;
        Sen[SnRKStart                   ].Enable=false;
        Sen[SnRKOneCycle                ].Enable=false;
        Sen[SnRKRetry                   ].Enable=false;
        Sen[SnRKSkip                    ].Enable=false;
        Sen[SnRKCleanOut                ].Enable=false;
        Sen[SnRKTrayFeed                ].Enable=false;
        Sen[SnRKTrayEnd                 ].Enable=false;
        Sen[SnRKAlarmReset              ].Enable=false;
//        Sen[SnRKCoverOpen               ].Enable=false;
        Sen[SnRKManualStep              ].Enable=false;
        Sen[SnRKManualTStart            ].Enable=false;
        Sen[SnRearPadActive             ].Enable=false;
    }

    
#if 0 // TODO(GA2-C1): blocked by SetIOTableByNUEC1@EtherCAT/MyNUEC1.cpp (not ported) -- golden cinitial.cpp:2762
    SetIOTableByNUEC1();                                                        //Sam 20230725 : EtherCAT NUEC1 IO 取代線路 IO
#endif

    //******************************************************************************
    //  注意!! Handler 安全門相關, 不可任意修改!!
    //******************************************************************************
    InitialSafeDoor();
}
//******************************************************************************
//  注意!! Handler 安全門相關, 不可任意修改!!
//  (golden cinitial.cpp:2769-2957 -- safety-critical door interlocks.  Fully
//  active, zero gating: every Sen[]/IniConfig/CUSTOMER_CODE/enum symbol this
//  function touches already exists in the ported tree.)
//******************************************************************************
void InitialSafeDoor()
{
    Sen[SnSafeDoor1].Enable=true;
    Sen[SnSafeDoor2].Enable=true;
    Sen[SnSafeDoor3].Enable=true;
    Sen[SnSafeDoor6].Enable=true;
    Sen[SnSafeDoor7].Enable=true;
    Sen[SnSafeDoor8].Enable=true;
    Sen[SnSafeDoor9].Enable=true;

    if(MachineTypeChoice!=Type_HT7080)                                          //Nickliu 20230321 add 70XX safe door type
    {
        Sen[SnSafeDoor1].Type=1;                                                //JerryYang 20160720 補上安全門不能Disable的漏洞
        Sen[SnSafeDoor2].Type=1;
        Sen[SnSafeDoor3].Type=1;
        Sen[SnSafeDoor6].Type=1;
        Sen[SnSafeDoor7].Type=1;
        Sen[SnSafeDoor8].Type=1;
        Sen[SnSafeDoor9].Type=1;
    }
    else
    {
       Sen[SnSafeDoor1].Type=0;
       Sen[SnSafeDoor2].Type=0;
       Sen[SnSafeDoor3].Type=0;
       Sen[SnSafeDoor6].Type=0;
       Sen[SnSafeDoor7].Type=0;
       Sen[SnSafeDoor8].Type=0;
       Sen[SnSafeDoor9].Type=0;
    }

    if(SAFE_DOOR_AMOUNT==0)                                                     //7扇門
    {
        Sen[SnSafeDoor4 ].Enable=false;
        Sen[SnSafeDoor5 ].Enable=false;
        Sen[SnSafeDoor10].Enable=false;
    }
    else if(SAFE_DOOR_AMOUNT==1)                                                //8扇門
    {
        Sen[SnSafeDoor4 ].Enable=false;
        Sen[SnSafeDoor5 ].Enable=false;
        Sen[SnSafeDoor10].Enable=true;
        Sen[SnSafeDoor10].Type=1;
    }
    else if(SAFE_DOOR_AMOUNT==2)                                                //9扇門
    {
        Sen[SnSafeDoor4 ].Enable=true;
        Sen[SnSafeDoor5 ].Enable=true;
        Sen[SnSafeDoor10].Enable=false;

        Sen[SnSafeDoor4].Type=1;
        Sen[SnSafeDoor5].Type=1;
    }
    else if(SAFE_DOOR_AMOUNT==3)                                                //10扇門
    {
        Sen[SnSafeDoor4 ].Enable=true;
        Sen[SnSafeDoor5 ].Enable=true;
        Sen[SnSafeDoor10].Enable=true;

        Sen[SnSafeDoor4 ].Type=1;
        Sen[SnSafeDoor5 ].Type=1;
        Sen[SnSafeDoor10].Type=1;
    }
    else if(SAFE_DOOR_AMOUNT==4)                                                //Ztex 2023.04.13 Add HT-1032 IO
    {
        Sen[SnSafeDoor4].Enable=true;
        Sen[SnSafeDoor5].Enable=false;
        Sen[SnSafeDoor9].Enable=false;
    }

    if(USE_MR_SYSTEM)                                                           //wei 20180702 MR
    {
        if(USE_MR_SYSTEM==1)
        {
            #ifdef SOFT_SIMULTE
            Sen[SnSafeDoor11].Enable=false;
            #else
            Sen[SnSafeDoor11].Enable=true;
            #endif
        }
        else
        {
            Sen[SnSafeDoor16].Enable=true;
            Sen[SnSafeDoor17].Enable=true;
            Sen[SnSafeDoor18].Enable=true;
            Sen[SnSafeDoor19].Enable=true;
        }
        Sen[SnSafeDoor9].Enable=false;

        Sen[SnSafeDoor12].Enable=true;
        Sen[SnSafeDoor13].Enable=true;
        Sen[SnSafeDoor14].Enable=true;
        Sen[SnSafeDoor15].Enable=true;

        Sen[SnSafeDoor11].Type=1;
        Sen[SnSafeDoor12].Type=1;
        Sen[SnSafeDoor13].Type=1;
        Sen[SnSafeDoor14].Type=1;
        Sen[SnSafeDoor15].Type=1;
    }
    else if(USE_OHT_SYSTEM==1)                                                  //KaiChen 20200716 ：OHT
    {
        Sen[SnSafeDoor9].Enable=false;
        Sen[SnSafeDoor10].Enable=true;

        Sen[SnSafeDoor12].Enable=true;
        Sen[SnSafeDoor13].Enable=true;
        Sen[SnSafeDoor14].Enable=true;
        Sen[SnSafeDoor15].Enable=true;
        Sen[SnSafeDoor16].Enable=true;
        Sen[SnSafeDoor17].Enable=true;

        Sen[SnSafeDoor19].Enable=true;
        Sen[SnSafeDoor20].Enable=true;
        Sen[SnSafeDoor21].Enable=true;

        #ifdef SOFT_SIMULTE
        Sen[SnSafeDoor11].Enable=false;
        Sen[SnSafeDoor18].Enable=false;
        #else
        Sen[SnSafeDoor11].Enable=true;
        Sen[SnSafeDoor18].Enable=true;
        #endif

        Sen[SnSafeDoor10].Type=1;
        Sen[SnSafeDoor11].Type=1;
        Sen[SnSafeDoor12].Type=1;
        Sen[SnSafeDoor13].Type=1;
        Sen[SnSafeDoor14].Type=1;
        Sen[SnSafeDoor15].Type=1;
        Sen[SnSafeDoor16].Type=1;
        Sen[SnSafeDoor17].Type=1;
        Sen[SnSafeDoor18].Type=1;
        Sen[SnSafeDoor19].Type=1;
        Sen[SnSafeDoor20].Type=1;
        Sen[SnSafeDoor21].Type=1;

        Sen[SnOHTIntoLoadPort].Enable=true;
        Sen[SnOHTIntoLoadPort2].Enable=true;

        Sen[SnOHTIntoLoadPort].Type=1;
        Sen[SnOHTIntoLoadPort2].Type=1;
    }

    if(AUTO3_IS_MAGAZINE==1)                                                    //JerryYang 20221215 : add Magazine安全門
    {
        Sen[SnMagazineSafeDoor].Enable=true;
        Sen[SnMagazineSafeDoor].Type=1;
    }
    #ifdef SOFT_SIMULTE
    for(int i=0; i<MAX_SAFE_DOOR_CNT; i++)
        Sen[iSafeDoor[i]].Enable=false;
    #endif

    if(MachineTypeChoice==Type_HT1032)                                          //Ztex 2023.04.13 Add HT-1032 IO
    {
        for(int i=0; i<MAX_HATCH_DOOR_CNT; i++)
        {
            #ifdef SOFT_SIMULTE
            Sen[iSafeDoorHatchway[i]].Enable=false;
            #else
            Sen[iSafeDoorHatchway[i]].Enable=true;
            #endif
        }
        Sen[iSafeDoorHatchway[4]].Enable=false;
        Sen[iSafeDoorHatchway[9]].Enable=false;
        Sen[iSafeDoorHatchway[10]].Enable=false;
    }
    else
    {
        for(int i=0; i<MAX_HATCH_DOOR_CNT; i++)
        {
            Sen[iSafeDoorHatchway[i]].Enable=false;
        }
    }

    if(MachineTypeChoice!=Type_HT7080)                                          //Nickliu 20230313 add HT70XX No Heater Door
        InitialHeaterDoor();

    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //Ifor 20230608 add:KYEC 要求[P35] Tray Arm home safe pos強制反灰由DB選擇開/關
    {
        IniConfig.bP35TrayArm=Sen[SnTrayArmSafePos].Enable;
    }
//#endif
}
//******************************************************************************
//
//  注意!! Handler 安全門相關, 不可任意修改!!
//  (golden cinitial.cpp:2958-2979.  Fully active, zero gating.)
//
//******************************************************************************
void InitialHeaterDoor()
{
    #ifndef SOFT_SIMULTE                                                        //JerryYang 20230204 : 軟體模擬不用卡安全門
    if(ATC_SYSTEM==eATCSiliconType &&
       (TestIF_File.iTestMode==DualSite ||
        TestIF_File.iTestMode==SingleSite) &&
       Temperature.bATCActiveCooling==true)
    {
        Sen[SnHeaterDoor].Enable=false;
    }
    else
    {
        Sen[SnHeaterDoor].Enable=true;
    }

    Sen[SnHeaterDoor2].Enable=true;
    #endif
}
//------------------------------------------------------------------------------
//  golden cinitial.cpp:2981-3059 -- InitialSensorLane, already commented out
//  in golden itself (dead code, not called from anywhere).  Preserved
//  verbatim as a disabled comment for byte-for-byte fidelity through the
//  GA-2-C1 block boundary (:3060); NOT re-enabled.
//------------------------------------------------------------------------------
//void InitialSensorLane()
//{
//    TTable *T;
//    int i;
//    bool flag;
//    AnsiString S, S1;
//    AnsiString sTemp;
//    char str[256];
//    DataModule1->SensorTable->TableName=CurrentDir+"\\system\\sensorLane.db";
//    T=DataModule1->SensorTable;
//    T->Open();
//    InitialSensorName();
//    for(i=0; i<MAX_SENSOR_ITEM; i++)
//    {
//        if(Sen[i].Name=="")
//            continue;
//        T->First();
//        flag=false;
//        while(!T->Eof)
//        {
//            S=T->FieldByName("SensorName")->AsString;
//            if(S==Sen[i].Name)
//            {
//                flag=true;
//                break;
//            }
//            T->Next();
//        }
//        if(flag==false)
//        {
//            sprintf(str,"Can not find sensor name:%s",Sen[i].Name.c_str());
//        }
//        else
//        {
//
//            Sen[i].Using         =T->FieldByName("Lane")->AsString;
//            if(Sen[i].Using!="")
//                Sen[i].Ring     =atoi(Sen[i].Using.c_str());
//            else
//                Sen[i].Enable=false;
//            Sen[i].IP           =atoi(T->FieldByName("IP")->AsString.c_str());
//            Sen[i].Port         =atoi(T->FieldByName("Port")->AsString.c_str());
//            Sen[i].Bit          =atoi(T->FieldByName("Bit")->AsString.c_str());
//            Sen[i].Type         =atoi(T->FieldByName("InType")->AsString.c_str());
//
//            #ifdef SOFT_SIMULTE
//                Sen[i].Enable=false;
//            #else
//                Sen[i].Enable=atoi(        T->FieldByName("Enable")->AsString.c_str());
//            #endif
//        }
//    }
//    T->Close();
//
//    //Alick 20161011 (Steven) : TTL支援8Site
//    //==>
//    for(i=0; i<10; i++)
//    {
//        Sen[SenBit0+i].Enable=true;
//        Sen[SenBit10+i].Enable=true;
//    }
//
//    for(i=0; i<10; i++)
//    {
//        CopySensor(&Sen[SenBit0 +i], &SenBackUp[BackSenBit0 +i]);
//        CopySensor(&Sen[SenBit10+i], &SenBackUp[BackSenBit10+i]);
//    }
//
//    if(CosFunction.bTTLCanUse8Site)
//    {
//        for(i=0; i<20; i++)
//        {
//            Sen[SenBit20+i].Enable=true;
//            CopySensor(&Sen[SenBit20+i], &SenBackUp[BackSenBit20+i]);
//        }
//    }
//    //<==
//    //Alick 20161011 (Steven) : TTL支援8Site
//}
//==============================================================================
//------------------------------------------------------------------------------
//  InitialMotorName  (golden cinitial.cpp:3109-3390)
//
//  The MOT[] identity/name table -- golden's sibling of InitialMotorParameter
//  (golden :3392-...), and the exact analogue of InitialSuckerName (:314 here) /
//  InitialSwitchName (:1122) / InitialSensorName (:1740).  274 flat
//  MOT[<enum>].SetAlias(<literal index>, "<alias>") calls, nothing else: no
//  control flow, no #ifdef, no golden dead code, no external state.
//
//  ZERO GATING -- byte-identical to golden.  Verified before landing
//  (AI(W906-GA2-IMN) 20260809):
//    * all 274 MOT[] index symbols (MInArmX .. MMUnloadPort4) are declared
//      `extern const int` in cmydef.h AND defined in cmydef.cpp
//      (ht9045_globals).  Values are 0..273, all distinct, all < the
//      MAX_TRAY_MOTOR (300) bound of MOT[] -- so every subscript here is in
//      range.  0 missing.
//    * every one of the 274 port cmydef.cpp values equals the literal index
//      golden passes as SetAlias's first argument, and equals golden
//      cmydef.cpp's own value -- i.e. MOT[MInArmX].SetAlias(0,..) is
//      self-consistent in the port exactly as in golden.  0 divergences.
//    * MOT[] is `extern class TTrayMotor MOT[MAX_TRAY_MOTOR]` @
//      Motor/mymotor.h:385, DEFINED at Motor/mymotor.cpp:124 (ht9045_motor);
//      TTrayMotor is declared in exactly ONE header in the tree
//      (Motor/mymotor.h:318) -- no second-layout hazard here.
//      MOT[] is already visible in this TU transitively via
//      aHotPlateSubstrate.h:68 -> "Motor/mymotor.h", so this unit adds NO
//      #include (append-only constraint honoured).
//    * TMyMotor::SetAlias(int,AnsiString) is ACTIVE at Motor/mymotor.cpp:227
//      and its body matches golden Motor/mymotor.cpp:396-401 statement for
//      statement (Alias=Name; Mot_Name=iNo; NumberAlias.sprintf("[%02d] %s",
//      iNo, Name)) -- whitespace-only diff.  So the observable effect of this
//      function is identical to golden's.
//
//  NOT declared in cinitial.h -- deliberately.  Golden's cinitial.h does NOT
//  declare InitialMotorName either; golden's ONLY caller is
//  InitialMotorParameter at golden cinitial.cpp:3407, i.e. same TU, later in
//  the file, relying on definition-before-use.  When InitialMotorParameter
//  lands in this port file it MUST be appended AFTER this definition (as in
//  golden) or it will need a forward declaration -- flagged for the
//  integration step rather than pre-empted here.
//
//  Four aliases intentionally DIFFER from their enum spelling and MUST NOT be
//  "corrected": MInShuttle1->"MInShutte1", MInShuttle2->"MInShutte2"
//  (golden :3122 carries the warning comment), MInRotateKit->"MInRotate",
//  MOutRotateKit->"MOutRotate" (:3152).  The alias is the Teaching-file key;
//  renaming it breaks teach-data lookup on a real machine.  Preserved verbatim,
//  typo and all.
//
//  Translator: AI(W906-GA2-IMN) 20260809
//------------------------------------------------------------------------------
void InitialMotorName()                                                         //Steven 20250520 : 變更Mot初始化方式
{
    MOT[MInArmX       ].SetAlias(  0, "MInArmX");
    MOT[MInArmY       ].SetAlias(  1, "MInArmY");
    MOT[MInArmPitch   ].SetAlias(  2, "MInArmPitch");
    MOT[MInArmZA      ].SetAlias(  3, "MInArmZA");
    MOT[MInArmZB      ].SetAlias(  4, "MInArmZB");
    MOT[MInArmZC      ].SetAlias(  5, "MInArmZC");
    MOT[MInArmZD      ].SetAlias(  6, "MInArmZD");
    MOT[MInArmZE      ].SetAlias(  7, "MInArmZE");
    MOT[MInArmZF      ].SetAlias(  8, "MInArmZF");
    MOT[MInArmZG      ].SetAlias(  9, "MInArmZG");
    MOT[MInArmZH      ].SetAlias( 10, "MInArmZH");
    MOT[MInShuttle1   ].SetAlias( 11, "MInShutte1");                            //不可以改成MInShuttle1, 會造成Teaching異常
    MOT[MInShuttle2   ].SetAlias( 12, "MInShutte2");
    MOT[MTestY1       ].SetAlias( 13, "MTestY1");
    MOT[MTestZ1       ].SetAlias( 14, "MTestZ1");
    MOT[MTestZ2       ].SetAlias( 15, "MTestZ2");
    MOT[MTestY2       ].SetAlias( 16, "MTestY2");
    MOT[MOutShuttle1  ].SetAlias( 17, "MOutShuttle1");
    MOT[MOutShuttle2  ].SetAlias( 18, "MOutShuttle2");
    MOT[MOutArmX      ].SetAlias( 19, "MOutArmX");
    MOT[MOutArmY      ].SetAlias( 20, "MOutArmY");
    MOT[MOutArmPitch  ].SetAlias( 21, "MOutArmPitch");
    MOT[MOutArmZA     ].SetAlias( 22, "MOutArmZA");
    MOT[MOutArmZB     ].SetAlias( 23, "MOutArmZB");
    MOT[MOutArmZC     ].SetAlias( 24, "MOutArmZC");
    MOT[MOutArmZD     ].SetAlias( 25, "MOutArmZD");
    MOT[MOutArmZE     ].SetAlias( 26, "MOutArmZE");
    MOT[MOutArmZF     ].SetAlias( 27, "MOutArmZF");
    MOT[MOutArmZG     ].SetAlias( 28, "MOutArmZG");
    MOT[MOutArmZH     ].SetAlias( 29, "MOutArmZH");
    MOT[MTrayX        ].SetAlias( 30, "MTrayX");
    MOT[MInArmPitchY  ].SetAlias( 31, "MInArmPitchY");
    MOT[MInArmPitchX2 ].SetAlias( 32, "MInArmPitchX2");
    MOT[MOutArmPitchY ].SetAlias( 33, "MOutArmPitchY");
    MOT[MOutArmPitchX2].SetAlias( 34, "MOutArmPitchX2");
    MOT[MLoaderZ      ].SetAlias( 35, "MLoaderZ");
    MOT[MEmptyZ       ].SetAlias( 36, "MEmptyZ");
    MOT[MColorZ       ].SetAlias( 37, "MColorZ");
    MOT[MAuto1Z       ].SetAlias( 38, "MAuto1Z");
    MOT[MAuto2Z       ].SetAlias( 39, "MAuto2Z");
    MOT[MAuto3Z       ].SetAlias( 40, "MAuto3Z");
    MOT[MInRotateKit  ].SetAlias( 41, "MInRotate");                             //不可以改成MInRotateKit, 會造成Teaching異常
    MOT[MOutRotateKit ].SetAlias( 42, "MOutRotate");
    MOT[MAOIKit       ].SetAlias( 43, "MAOIKit");
    MOT[MLoaderY      ].SetAlias( 44, "MLoaderY");
    MOT[MEmptyY       ].SetAlias( 45, "MEmptyY");
    MOT[MColorY       ].SetAlias( 46, "MColorY");
    MOT[MAuto1Y       ].SetAlias( 47, "MAuto1Y");
    MOT[MAuto2Y       ].SetAlias( 48, "MAuto2Y");
    MOT[MAuto3Y       ].SetAlias( 49, "MAuto3Y");
    MOT[MInArmZAe     ].SetAlias( 50, "MInArmZAe");                             //Steven 20230323 : For HT1032
    MOT[MInArmPitchX3 ].SetAlias( 51, "MInArmPitchX3");
    MOT[MInArmPitchX4 ].SetAlias( 52, "MInArmPitchX4");
    MOT[MInArmZAf     ].SetAlias( 53, "MInArmZAf");
    MOT[MOutArmPitchX3].SetAlias( 54, "MOutArmPitchX3");
    MOT[MOutArmPitchX4].SetAlias( 55, "MOutArmPitchX4");
    MOT[MTrayZ        ].SetAlias( 56, "MTrayZ");
    MOT[MOutSortAa    ].SetAlias( 57, "MOutSortAa");                            //Steven 20240822 : For HT-9046AU
    MOT[MOutSortAb    ].SetAlias( 58, "MOutSortAb");
    MOT[MInArmXScale  ].SetAlias( 59, "MInArmXScale");
    MOT[MInArmYScale  ].SetAlias( 60, "MInArmYScale");
    MOT[MOutArmXScale ].SetAlias( 61, "MOutArmXScale");
    MOT[MOutArmYScale ].SetAlias( 62, "MOutArmYScale");
    MOT[MShuttle1Pitch].SetAlias( 63, "MShuttle1Pitch");
    MOT[MShuttle2Pitch].SetAlias( 64, "MShuttle2Pitch");
    MOT[MInRotateB    ].SetAlias( 65, "MInRotateB");
    MOT[MInRotateC    ].SetAlias( 66, "MInRotateC");
    MOT[MInRotateD    ].SetAlias( 67, "MInRotateD");
    MOT[MInRotateE    ].SetAlias( 68, "MInRotateE");
    MOT[MInRotateF    ].SetAlias( 69, "MInRotateF");
    MOT[MInRotateG    ].SetAlias( 70, "MInRotateG");
    MOT[MInRotateH    ].SetAlias( 71, "MInRotateH");
    MOT[MOutRotateB   ].SetAlias( 72, "MOutRotateB");
    MOT[MOutRotateC   ].SetAlias( 73, "MOutRotateC");
    MOT[MOutRotateD   ].SetAlias( 74, "MOutRotateD");
    MOT[MOutRotateE   ].SetAlias( 75, "MOutRotateE");
    MOT[MOutRotateF   ].SetAlias( 76, "MOutRotateF");
    MOT[MOutRotateG   ].SetAlias( 77, "MOutRotateG");
    MOT[MOutRotateH   ].SetAlias( 78, "MOutRotateH");
    MOT[MLightScale   ].SetAlias( 79, "MLightScale");
    MOT[MInArmZAg     ].SetAlias( 80, "MInArmZAg");
    MOT[MInArmZAh     ].SetAlias( 81, "MInArmZAh");
    MOT[MArmAlignment ].SetAlias( 82, "MArmAlignment");                         //Steven 20240507 : 只是為了Teaching存檔方便;
    MOT[MLoadHingeR   ].SetAlias( 83, "MLoadHingeR");
    MOT[MLoadHingeZ   ].SetAlias( 84, "MLoadHingeZ");
    MOT[MPreciser     ].SetAlias( 85, "MPreciser");
    MOT[MInArmZBe     ].SetAlias( 86, "MInArmZBe");
    MOT[MInArmZBf     ].SetAlias( 87, "MInArmZBf");
    MOT[MInArmZBg     ].SetAlias( 88, "MInArmZBg");
    MOT[MInArmZBh     ].SetAlias( 89, "MInArmZBh");
    MOT[MOutArmZAe    ].SetAlias( 90, "MOutArmZAe");
    MOT[MOutArmZAf    ].SetAlias( 91, "MOutArmZAf");
    MOT[MOutArmZAg    ].SetAlias( 92, "MOutArmZAg");
    MOT[MOutArmZAh    ].SetAlias( 93, "MOutArmZAh");
    MOT[MOutArmZBe    ].SetAlias( 94, "MOutArmZBe");
    MOT[MOutArmZBf    ].SetAlias( 95, "MOutArmZBf");
    MOT[MOutArmZBg    ].SetAlias( 96, "MOutArmZBg");
    MOT[MOutArmZBh    ].SetAlias( 97, "MOutArmZBh");
    MOT[MOutSortX     ].SetAlias( 98, "MOutSortX");                             //Steven 20240822 : For HT-9046AU
    MOT[MCaselevatorZ ].SetAlias( 99, "MCaselevatorZ");
    MOT[MCasArmX      ].SetAlias(100, "MCasArmX");
    MOT[MCasArmZ      ].SetAlias(101, "MCasArmZ");
    MOT[MTrayBracketZ ].SetAlias(102, "MTrayBracketZ");
    MOT[MStackedTrayX ].SetAlias(103, "MStackedTrayX");
    MOT[MStackedTrayZ ].SetAlias(104, "MStackedTrayZ");
    MOT[MUnloadRobotZ ].SetAlias(105, "MUnloadRobotZ");
    MOT[MOutSortY     ].SetAlias(106, "MOutSortY");                             //Steven 20240822 : For HT-9046AU
    MOT[MCCDX         ].SetAlias(107, "MCCDX");
    MOT[MCCDY         ].SetAlias(108, "MCCDY");
    MOT[MCCDZ         ].SetAlias(109, "MCCDZ");
    MOT[MInFlipper1   ].SetAlias(110, "MInFlipper1");
    MOT[MInFlipper2   ].SetAlias(111, "MInFlipper2");
    MOT[MInFlipper3   ].SetAlias(112, "MInFlipper3");
    MOT[MOutFlipper1  ].SetAlias(113, "MOutFlipper1");
    MOT[MOutFlipper2  ].SetAlias(114, "MOutFlipper2");
    MOT[MOutFlipper3  ].SetAlias(115, "MOutFlipper3");
    MOT[MLdCarRotArm  ].SetAlias(116, "MLdCarRotArm");
    MOT[MLoaderY_CCW  ].SetAlias(117, "MLoaderY_CCW");
    MOT[MAuto1Y_CCW   ].SetAlias(118, "MAuto1Y_CCW");
    MOT[MAuto2Y_CCW   ].SetAlias(119, "MAuto2Y_CCW");
    MOT[MAuto3Y_CCW   ].SetAlias(120, "MAuto3Y_CCW");
    MOT[MAuto4Y_CCW   ].SetAlias(121, "MAuto4Y_CCW");
    MOT[MAuto5Y_CCW   ].SetAlias(122, "MAuto5Y_CCW");
    MOT[MAuto6Y_CCW   ].SetAlias(123, "MAuto6Y_CCW");
    MOT[M1_3R         ].SetAlias(124, "M1_3R");
    MOT[M1_4X         ].SetAlias(125, "M1_4X");
    MOT[M1_4Y         ].SetAlias(126, "M1_4Y");
    MOT[M1_4R         ].SetAlias(127, "M1_4R");
    MOT[M1_5X         ].SetAlias(128, "M1_5X");
    MOT[M1_5Y         ].SetAlias(129, "M1_5Y");
    MOT[M1_5R         ].SetAlias(130, "M1_5R");
    MOT[M1_6X         ].SetAlias(131, "M1_6X");
    MOT[M1_6Y         ].SetAlias(132, "M1_6Y");
    MOT[M1_6R         ].SetAlias(133, "M1_6R");
    MOT[M1_7X         ].SetAlias(134, "M1_7X");
    MOT[M1_7Y         ].SetAlias(135, "M1_7Y");
    MOT[M1_7R         ].SetAlias(136, "M1_7R");
    MOT[M1_8X         ].SetAlias(137, "M1_8X");
    MOT[M1_8Y         ].SetAlias(138, "M1_8Y");
    MOT[M1_8R         ].SetAlias(139, "M1_8R");
    MOT[MMagazine     ].SetAlias(140, "MMagazine");                             //JerryYang 20221215 : add Magazine
    MOT[MCatchMgzTray ].SetAlias(141, "MCatchMgzTray");
    MOT[MMagYTrayOut  ].SetAlias(142, "MMagYTrayOut");

    MOT[MFix3Full     ].SetAlias(143, "MFix3Full");
    MOT[MAuto4Z       ].SetAlias(144, "MAuto4Z");                               //Steven 20230907 : For HT-9011UC
    MOT[MAuto5Z       ].SetAlias(145, "MAuto5Z");
    MOT[MAuto6Z       ].SetAlias(146, "MAuto6Z");
    MOT[MAuto4Y       ].SetAlias(147, "MAuto4Y");
    MOT[MAuto5Y       ].SetAlias(148, "MAuto5Y");
    MOT[MAuto6Y       ].SetAlias(149, "MAuto6Y");

    MOT[MTopAOIArmX   ].SetAlias(150, "MTopAOIArmX");
    MOT[MTopAOIArmY   ].SetAlias(151, "MTopAOIArmY");
    MOT[MTopAOIArmR   ].SetAlias(152, "MTopAOIArmR");
    MOT[MTopAOICCDZ   ].SetAlias(153, "MTopAOICCDZ");
    MOT[MTopAOIElevZ1 ].SetAlias(154, "MTopAOIElevZ1");
    MOT[MTopAOIElevZ2 ].SetAlias(155, "MTopAOIElevZ2");

    MOT[MOutSortPitchX].SetAlias(156, "MOutSortPitchX");                        //Steven 20240822 : For HT-9046AU
    MOT[MOutSortSht   ].SetAlias(157, "MOutSortSht");
    MOT[MLoad2Z       ].SetAlias(158, "MLoad2Z");
    MOT[MLoad2Y       ].SetAlias(159, "MLoad2Y");

    MOT[MInSh1LtcSenZ1].SetAlias(160, "MInSh1LtcSenZ1");                        //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
    MOT[MInSh1LtcSenZ2].SetAlias(161, "MInSh1LtcSenZ2");
    MOT[MInSh2LtcSenZ1].SetAlias(162, "MInSh2LtcSenZ1");
    MOT[MInSh2LtcSenZ2].SetAlias(163, "MInSh2LtcSenZ2");
    //虛擬Tray------------------------------------------------------------------
    MOT[MManualTray1            ].SetAlias(164, "MManualTray1");
    MOT[MManualTray2            ].SetAlias(165, "MManualTray2");
    MOT[MManualTray3            ].SetAlias(166, "MManualTray3");
    MOT[MMTrayY                 ].SetAlias(167, "MMTrayY");
    MOT[MMTrayY_Car             ].SetAlias(168, "MMTrayY_Car");
    MOT[MMPlate1                ].SetAlias(169, "MMPlate1");
    MOT[MMPlate2                ].SetAlias(170, "MMPlate2");
    MOT[MMAuto1                 ].SetAlias(171, "MMAuto1");
    MOT[MMAuto2                 ].SetAlias(172, "MMAuto2");
    MOT[MMAuto3                 ].SetAlias(173, "MMAuto3");
    MOT[MMAuto1_Car             ].SetAlias(174, "MMAuto1_Car");
    MOT[MMAuto2_Car             ].SetAlias(175, "MMAuto2_Car");
    MOT[MMAuto3_Car             ].SetAlias(176, "MMAuto3_Car");
    MOT[MMEmpty                 ].SetAlias(177, "MMEmpty");
    MOT[MMColor                 ].SetAlias(178, "MMColor");
    MOT[MMEmpty_Car             ].SetAlias(179, "MMEmpty_Car");
    MOT[MMColor_Car             ].SetAlias(180, "MMColor_Car");
    MOT[MMEmpty1                ].SetAlias(181, "MMEmpty1");
    MOT[MMEmpty1_Car            ].SetAlias(182, "MMEmpty1_Car");
    MOT[MMHot1RecBuf            ].SetAlias(183, "MMHot1RecBuf");
    MOT[MMHot2RecBuf            ].SetAlias(184, "MMHot2RecBuf");
    MOT[MMAutoCleanKit          ].SetAlias(185, "MMAutoCleanKit");
    MOT[MMOCR                   ].SetAlias(186, "MMOCR");
    MOT[MMBulkboxKit            ].SetAlias(187, "MMBulkboxKit");
    MOT[MMCABuffer1             ].SetAlias(188, "MMCABuffer1");
    MOT[MMCABuffer2             ].SetAlias(189, "MMCABuffer2");
    MOT[MMCABuffer3             ].SetAlias(190, "MMCABuffer3");
    MOT[MMCABuffer4             ].SetAlias(191, "MMCABuffer4");
    MOT[MMCABuffer5             ].SetAlias(192, "MMCABuffer5");
    MOT[MMCABuffer6             ].SetAlias(193, "MMCABuffer6");
    MOT[MMCABuffer7             ].SetAlias(194, "MMCABuffer7");
    MOT[MMCABuffer8             ].SetAlias(195, "MMCABuffer8");
    MOT[MMCABuffer9             ].SetAlias(196, "MMCABuffer9");
    MOT[MMCABuffer10            ].SetAlias(197, "MMCABuffer10");
    MOT[MMLoadPort              ].SetAlias(198, "MMLoadPort");
    MOT[MMTrayLoader            ].SetAlias(199, "MMTrayLoader");
    MOT[MMTrayEmpty             ].SetAlias(200, "MMTrayEmpty");
    MOT[MMTrayConversion        ].SetAlias(201, "MMTrayConversion");
    MOT[MMTrayAuto1             ].SetAlias(202, "MMTrayAuto1");
    MOT[MMTrayAuto2             ].SetAlias(203, "MMTrayAuto2");
    MOT[MMTrayAuto3             ].SetAlias(204, "MMTrayAuto3");
    MOT[MMFixTray1              ].SetAlias(205, "MMFixTray1");
    MOT[MMFixTray2              ].SetAlias(206, "MMFixTray2");
    MOT[MMFixTray3              ].SetAlias(207, "MMFixTray3");
    MOT[MMScanAOI               ].SetAlias(208, "MMScanAOI");
    MOT[MMInArmAOATray          ].SetAlias(209, "MMInArmAOATray");
    MOT[MMOutArmAOATray         ].SetAlias(210, "MMOutArmAOATray");
    MOT[MMAOASampleTray         ].SetAlias(211, "MMAOASampleTray");
    MOT[MMAOASamplePlate        ].SetAlias(212, "MMAOASamplePlate");
    MOT[MInPlacementX           ].SetAlias(213, "MInPlacementX");
    MOT[MInPlacementY           ].SetAlias(214, "MInPlacementY");
    MOT[MMMagazineTary1         ].SetAlias(215, "MMMagazineTary1");
    MOT[MMMagazineTary2         ].SetAlias(216, "MMMagazineTary2");
    MOT[MMMagazineTary3         ].SetAlias(217, "MMMagazineTary3");
    MOT[MMMagazineTary4         ].SetAlias(218, "MMMagazineTary4");
    MOT[MMMagazineTary5         ].SetAlias(219, "MMMagazineTary5");
    MOT[MMMagazineTary6         ].SetAlias(220, "MMMagazineTary6");
    MOT[MMMagazineTary7         ].SetAlias(221, "MMMagazineTary7");
    MOT[MMMagazineTary8         ].SetAlias(222, "MMMagazineTary8");
    MOT[MMMagazineTary9         ].SetAlias(223, "MMMagazineTary9");
    MOT[MMMagazineTary10        ].SetAlias(224, "MMMagazineTary10");
    MOT[MMMagazineTary11        ].SetAlias(225, "MMMagazineTary11");
    MOT[MMMagazineTary12        ].SetAlias(226, "MMMagazineTary12");
    MOT[MMMagazineTary13        ].SetAlias(227, "MMMagazineTary13");
    MOT[MMMagazineTary14        ].SetAlias(228, "MMMagazineTary14");
    MOT[MMMagazineTaryTop       ].SetAlias(229, "MMMagazineTaryTop");
    MOT[MMMagazineBuffer        ].SetAlias(230, "MMMagazineBuffer");
    MOT[MMBackupMagazineTary1   ].SetAlias(231, "MMBackupMagazineTary1");
    MOT[MMBackupMagazineTary2   ].SetAlias(232, "MMBackupMagazineTary2");
    MOT[MMBackupMagazineTary3   ].SetAlias(233, "MMBackupMagazineTary3");
    MOT[MMBackupMagazineTary4   ].SetAlias(234, "MMBackupMagazineTary4");
    MOT[MMBackupMagazineTary5   ].SetAlias(235, "MMBackupMagazineTary5");
    MOT[MMBackupMagazineTary6   ].SetAlias(236, "MMBackupMagazineTary6");
    MOT[MMBackupMagazineTary7   ].SetAlias(237, "MMBackupMagazineTary7");
    MOT[MMBackupMagazineTary8   ].SetAlias(238, "MMBackupMagazineTary8");
    MOT[MMBackupMagazineTary9   ].SetAlias(239, "MMBackupMagazineTary9");
    MOT[MMBackupMagazineTary10  ].SetAlias(240, "MMBackupMagazineTary10");
    MOT[MMBackupMagazineTary11  ].SetAlias(241, "MMBackupMagazineTary11");
    MOT[MMBackupMagazineTary12  ].SetAlias(242, "MMBackupMagazineTary12");
    MOT[MMBackupMagazineTary13  ].SetAlias(243, "MMBackupMagazineTary13");
    MOT[MMBackupMagazineTary14  ].SetAlias(244, "MMBackupMagazineTary14");
    MOT[MMTrayZ                 ].SetAlias(245, "MMTrayZ");
    MOT[MMEmptyZ                ].SetAlias(246, "MMEmptyZ");
    MOT[MMColorZ                ].SetAlias(247, "MMColorZ");
    MOT[MMAuto1Z                ].SetAlias(248, "MMAuto1Z");
    MOT[MMAuto2Z                ].SetAlias(249, "MMAuto2Z");
    MOT[MMAuto3Z                ].SetAlias(250, "MMAuto3Z");
    MOT[MMAuto4Z                ].SetAlias(251, "MMAuto4Z");
    MOT[MMAuto5Z                ].SetAlias(252, "MMAuto5Z");
    MOT[MMAuto6Z                ].SetAlias(253, "MMAuto6Z");
    MOT[MManualTray4            ].SetAlias(254, "MManualTray4");
    MOT[MManualTray5            ].SetAlias(255, "MManualTray5");
    MOT[MManualTray6            ].SetAlias(256, "MManualTray6");
    MOT[MMAuto4                 ].SetAlias(257, "MMAuto4");
    MOT[MMAuto5                 ].SetAlias(258, "MMAuto5");
    MOT[MMAuto6                 ].SetAlias(259, "MMAuto6");
    MOT[MMAuto4_Car             ].SetAlias(260, "MMAuto4_Car");
    MOT[MMAuto5_Car             ].SetAlias(261, "MMAuto5_Car");
    MOT[MMAuto6_Car             ].SetAlias(262, "MMAuto6_Car");
    MOT[MMFixTray4              ].SetAlias(263, "MMFixTray4");
    MOT[MMFixTray5              ].SetAlias(264, "MMFixTray5");
    MOT[MMFixTray6              ].SetAlias(265, "MMFixTray6");
    MOT[MMLoadPort1             ].SetAlias(266, "MMLoadPort1");
    MOT[MMLoadPort2             ].SetAlias(267, "MMLoadPort2");
    MOT[MMLoadPort3             ].SetAlias(268, "MMLoadPort3");
    MOT[MMLoadPort4             ].SetAlias(269, "MMLoadPort4");
    MOT[MMUnloadPort1           ].SetAlias(270, "MMUnloadPort1");
    MOT[MMUnloadPort2           ].SetAlias(271, "MMUnloadPort2");
    MOT[MMUnloadPort3           ].SetAlias(272, "MMUnloadPort3");
    MOT[MMUnloadPort4           ].SetAlias(273, "MMUnloadPort4");
}
//==============================================================================
//------------------------------------------------------------------------------
//  InitialMotorParameter  (golden cinitial.cpp:3392-4101)
//
//  THE MOTOR-DRIVER ATTACH SEAM.  This is golden's ONLY home for
//  `MOT[i].Motor = new TMy<brand>Motor(iAdder)`.  Until this landed, MOT[].Motor
//  was NULL tree-wide, so all 292 golden `MOT[i].Motor->` derefs were dead and
//  Motor/myGALILmotor.cpp had no caller.
//
//  Translator: AI(W906-GA2-IMP) 20260809
//
//  ---------------------------------------------------------------------------
//  SIMULATION POSTURE -- THE CRUX, TRANSLATED AS GOLDEN WROTE IT
//  ---------------------------------------------------------------------------
//  Golden's own `#ifdef SOFT_SIMULTE` arms (:3483-3484, :3548-3549, :3565-3566,
//  :3720-3721, :3743-3744, :3900-3901, :3936-3937) STILL `new` a driver and
//  merely set `Enable=false`.  Golden therefore has NO "MOT[].Motor stays NULL"
//  state at all -- not in simulation, not on a machine with an empty
//  Mot_Table.csv row, not on a machine whose IO_CARD_TYPE matches none of the
//  three arms: the unconditional tail loop at golden :4027-4034 attaches
//  `new TMySYNTEKMotor(-1)` with `Enable=false, GearRatio=1.0` to every one of
//  the TOTAL_MOTOR(164) slots that is still NULL.  "Attached but DISABLED" is
//  golden's simulation state.  Every #ifdef SOFT_SIMULTE block below is kept
//  VERBATIM (SOFT_SIMULTE is NOT defined in this tree -- MachineType.h has it
//  commented out -- so the #else arms are what compiles); NO NULL path was
//  invented.
//
//  ---------------------------------------------------------------------------
//  WHAT IS ACTIVE
//  ---------------------------------------------------------------------------
//   * The whole `IO_CARD_TYPE==NewIO_MN200 || IO_CARD_TYPE==PCI_P64C64`
//     (Mot_Table.csv-driven) arm, golden :3409-3645 -- i.e. the real attach:
//     HSys.LoadMotData(), the mapMotTable name lookup with golden's
//     MInShuttle1/MInShuttle2 fallback, sModel=CardModel, and the five
//     `new TMy<brand>Motor` branches plus every TMOTDATA -> HTMotor field copy.
//     This mirrors InitialSwitch (:1506) / InitialSensor (:2628) in this same
//     file, which activate exactly the same IO_CARD_TYPE arm and gate the BDE
//     arm -- the in-file precedent this unit was written against.
//   * The unconditional NULL-fallback + InitMotor(0) tail loop (:4027-4054).
//   * The 18 x 30 x 70 Tray.PordRec allocation loop (:4064-4087).
//   * The MInArmPitch / MOutArmPitch SetGroup pair (:4090-4099).
//
//  ---------------------------------------------------------------------------
//  GOLDEN DEFECTS PRESERVED, NOT FIXED (per the faithful-port contract)
//  ---------------------------------------------------------------------------
//   (A) golden :3510-3517 + :3548-3560 -- NULL DEREF for CardModel=="MC88X1".
//       Golden's MC88X1 branch computes iAdder but its
//       `MOT[i].Motor = new HTMC88X1Motor(iAdder);` was COMMENTED OUT by
//       "Steven 20231218 HT7080B" (:3516), so Motor is still NULL when :3555 /
//       :3559 write `MOT[i].Motor->Enable`.  A machine whose Mot_Table.csv says
//       CardModel=MC88X1 crashes here on the first call.  Kept exactly (the
//       instantiation stays a comment; HTMC88X1Motor has no port either).
//   (B) golden :3396 + :3643 -- `int iAdder;` is never initialised, and the
//       Galil index-axis branch (:3464-3496) never assigns it, yet :3643 passes
//       it to `InitMotor(iAdder)`.  First loop iteration can pass an
//       indeterminate value; later iterations pass the PREVIOUS axis's adder.
//       Kept verbatim.
//   (C) golden :3639-3640 -- `SoftLimit[0]/[1]` are written and never read in
//       this arm (dead store; the BDE arms are the ones that use them).  Kept.
//   (D) golden :4064-4087 -- 18 x 2100 `new TMyProductionRecord()` with no
//       matching delete.  InitialMotorParameter is called again on every
//       recipe/setup change, so this leaks 37,800 records per call.  Kept.
//
//  ---------------------------------------------------------------------------
//  MID-FILE #include / #undef -- WHY, AND WHY IT IS SAFE
//  ---------------------------------------------------------------------------
//  This unit needs COMPLETE types for the five driver classes it instantiates,
//  and a sibling agent is editing this file concurrently, so the include block
//  at the top of the file is off-limits (append-only).  The includes therefore
//  sit here, at file scope, immediately below.  Two collisions had to be
//  resolved and BOTH are value-identical, so neither changes behaviour:
//
//   1. `#undef MAXRing / MAXIP / MAXPort` before Motor/myMN200motor.h.
//      MyLaneIo.h:52/55/58 (already included at the top of this file) define
//      them as MACROS aliasing IOBackend.h:50/53/56 (IO_MAXRing=4, IO_MAXIP=64,
//      IO_MAXPort=4), while Motor/myMN200motor.h:92-94 declares them as
//      `const int MAXRing=4; MAXIP=64; MAXPort=4;`.  Without the #undef the
//      macro rewrites the declaration into `const int 4=4` (3 hard errors).
//      THE VALUES ARE IDENTICAL (4/64/4 both ways, checked in both headers), so
//      the #undef swaps one spelling of the same three constants for another.
//      Everything above this point in the file was already preprocessed with
//      the macro form; nothing below it uses these names.
//   2. Motor/vendor/CMNet.h is included BEFORE Motor/myEthercatmotor.h.
//      myEthercatmotor.h:77 does `#ifndef F32 / #define F32 float`; CMNet.h ->
//      Motor/vendor/Type_def.h:9 does `typedef float F32;`.  In the other order
//      the macro rewrites the typedef into `typedef float float;`.  Both spell
//      "F32 is float", so the order is a pure compile-order fix.
//      CMNet.h is included ONLY for `#define G9004_M204 0xA7` (CMNet.h:20),
//      which golden :3546 assigns to SYN_TEK_MOTION_MODULE.  NOTE: cmydef.cpp
//      :3176-3180 still GATES golden's own initialiser
//      `SYN_TEK_MOTION_MODULE=G9004_M204` down to 0 -- that pre-existing
//      divergence is NOT touched here; this unit only makes golden :3546's
//      RUNTIME assignment real, which is what the "Device superfluous at Output
//      Shuttle" false-alarm fix (jou 2014-10-09) depends on.
//------------------------------------------------------------------------------
#undef  MAXRing                 // see note 1 above -- MyLaneIo.h macro vs myMN200motor.h const int (both 4)
#undef  MAXIP                   // both 64
#undef  MAXPort                 // both 4
#include "CMNet.h"              // G9004_M204 (CMNet.h:20) for golden :3546; also brings Type_def.h's `typedef float F32` before note 2
#include "Motor/myGALILmotor.h" // TMyGALILMotor    -- golden :3482 :3719 :3899
#include "Motor/myMN200motor.h" // TMyMN200Motor    -- golden :3508 :3922
#include "Motor/mySYNTEKmotor.h"// TMySYNTEKMotor   -- golden :3524 :3700 :3742 :4031
#include "Motor/mySMCmotor.h"   // TMySMCMotor      -- golden :3542 :3682 :3861 :3933
#include "Motor/myEthercatmotor.h" // TMyEtherCatMotor -- golden :3533   (MUST stay after CMNet.h, note 2)

//------------------------------------------------------------------------------
//  Free functions golden reaches from this unit.
//
//  InitialMotorName()  -- golden :3109, declared in NO golden header; golden's
//  only caller is golden :3407, same TU, definition-first.  Already landed in
//  THIS file at :3288 by the sibling GA-2-IMN pass, i.e. above this line, so
//  this declaration is belt-and-braces against append ordering only.
//
//  SetMotorAccelSpeed(int,int) -- golden cinitial.h:51, golden body
//  cinitial.cpp:4997-5017.  Declared here exactly as AutoClean/AutoClean.cpp
//  :160 already does, rather than by including acatchtray_shims.h.
//  *** STUB WARNING (link-closure, reported to the integrator): the ONLY body
//  in this tree is the empty no-op at acatchtray_shims.cpp:167.  Golden's body
//  clamps ADCSpeed to 1..100, honours IniConfig.bG14UseStartSoundAlarm, and
//  calls MOT[Index].SetADCRate(ADCSpeed), which is what actually programs
//  Motor->SetAcc/SetDec from GetAccDataBase()/GetDecDataBase().  With the stub
//  in place, golden :3616's `SetMotorAccelSpeed(i, 100)` and golden :3988's are
//  no-ops, so HTMotor::dAcc/dDec stay at their ctor values even though
//  dAccDataBase/dDecDataBase ARE correctly loaded by :3597-3598 below.  That is
//  a pre-existing stub, NOT a gate introduced by this unit; retiring it belongs
//  to the wave that translates golden cinitial.cpp:4997. ***
//------------------------------------------------------------------------------
void InitialMotorName();                                                        // golden cinitial.cpp:3109 (no golden header declares it)
void SetMotorAccelSpeed(int Index, int ADCSpeed);                               // golden cinitial.h:51 -- body is a NO-OP STUB at acatchtray_shims.cpp:167
//------------------------------------------------------------------------------
// AI(W906-PT-W5a-integrate) 20260809: GATE (W5a-G) -- golden's line is
//   `MOT[i].Motor=new TMyGALILMotor(iGalilPort);` and it is gated for ONE reason:
//   it is the first and only consumer of TMyGALILMotor in this tree, so it makes the
//   linker extract Motor/myGALILmotor.cpp.obj, whose 48 real `TMyMotor::Gali_*`
//   bodies then collide with the 48 ACTIVE stubs still standing at
//   Motor/mymotor.cpp:944-994 / :1043-1045.  MEASURED, not predicted: with this line
//   live the build reports exactly 48 `multiple definition` errors.
//
//   WHY THE STUBS ARE NOT SIMPLY RETIRED HERE -- I TRIED IT AND MEASURED THE RESULT.
//   Retiring all 48 makes the build link cleanly (rc=0, zero undefined), and then
//   ctest goes from 6 failures to 19: TEN new SEGFAULTs (W6_4_Tester,
//   W6_4b_FrontRearDestroy, W6_5_Shuttle, W6_6_Hub, W6_6_CSystemCycle,
//   W7_S0_MotorConvergence, W7_C1_CleanOutFinish, W7_C2_OneCycleFinish,
//   W906_DoIndexAutoClean, W5_Atester32Site).  Cause: golden's real Galil bodies
//   dereference `MOT[i].Motor` UNGUARDED -- golden can, because golden always runs
//   this function first -- while three of the retired stubs were returning
//   `(Motor==NULL)` as their offline "completed" answer, which is what those tests
//   have been standing on.  The NULL-ness and those fast paths are two halves of ONE
//   convention.
//
//   SO THE UN-GATE IS A THREE-PART, SINGLE WAVE, and all three parts must land
//   together or the suite goes red:
//     1. delete this gate (restore golden's line),
//     2. retire Motor/mymotor.cpp:944-994 + :1043-1045 -- all 48, but KEEP :963
//        `Gali_MotHome_HighSpeed`, which myGALILmotor.cpp does NOT define,
//     3. make the ten tests above attach motors before they pump, the way golden
//        does (InitHontechHardware -> InitialMotorParameter).
//   BEHAVIOUR DELTA UNTIL THEN: the four index axes (MTestY1/Z1/Z2/Y2 = 13..16) get
//   NO driver from this branch, so they fall through to golden's own tail loop and
//   end up `new TMySYNTEKMotor(-1)` with `Enable=false` -- attached but disabled,
//   which is exactly golden's `#ifdef SOFT_SIMULTE` posture for every axis.  They
//   are therefore non-NULL and safe to dereference; they simply are not Galil.
static const bool W5aG_INDEX_GALIL_BRANCH_ENABLED = false;   // GATE (W5a-G) -- flip to true ONLY with parts 2 and 3 above

void InitialMotorParameter()
{
// ---- GATE 1 --------------------------------------------------------------
// golden cinitial.cpp:3394 -- `TTable *PT;`
// GATED because: TTable / DataModule1 (BDE) exist NOWHERE in this tree.
//   database.h:204-207 keeps `class TDataModule1 ... TTable *MotorTable;` and
//   `extern PACKAGE TDataModule1 *DataModule1;` commented out as its own
//   documented "still gated pending the BDE wave".  Same blocker as
//   InitialSwitch (:1506) / InitialSensor (:2628) in this file.
// DEFAULT IS FAITHFUL because: PT is read ONLY inside the two BDE arms that
//   GATE 2 gates as one block (golden :3646-4025).  Nothing in the ACTIVE arm
//   or in the common tail touches it, so removing the declaration removes no
//   reachable behaviour.  Golden's other locals that only the BDE arms use
//   (sDBDir, flag, iLane, iIP, iPort) are deliberately LEFT DECLARED and
//   ACTIVE below, verbatim, so un-gating GATE 2 is a pure delete of two
//   preprocessor lines -- no declaration has to be re-derived from golden
//   (which is the trap the InitialSwitch gate at :1533-1540 warns about).
//   They cost only -Wunused warnings.
// BEHAVIOUR DELTA ON A REAL MACHINE: none from this gate alone; see GATE 2.
#if 0 // GATE 1: blocked by TTable/DataModule1 @database.h:204-207 (BDE unported)
    TTable *PT;
#endif
    AnsiString S, sDBDir;
    int iAdder, iMot;
    AnsiString Mot_Name;
    AnsiString sModel;
    int SoftLimit[2];
    int iIP;
    int iLane;
    int iPort;
    double dAcc, dDec;
    bool flag;
    bool bHasMotor=false;                                                       //Stevem 20210625 : 修正新的CSV IO表讀檔方式

    InitialMotorName();

    if(IO_CARD_TYPE==NewIO_MN200 ||
       IO_CARD_TYPE==PCI_P64C64)                                                //Steven 20231218 HT7080B
    {
        HSys.LoadMotData();
        for(int i=0; i<TOTAL_MOTOR; i++)
        {
            iMot=-1;
            bHasMotor=false;
            Mot_Name.sprintf("M%02d", i);
            HSys.mapMotTableIter=HSys.mapMotTable.find(Mot_Name);
            if(HSys.mapMotTableIter!=HSys.mapMotTable.end())
            {
                iMot=atoi(HSys.mapMotTable[Mot_Name].c_str());
                if(iMot<0)                                                      //Steven 20250520 : 變更Mot初始化方式
                {
                    if(i==11)
                    {
                        iMot=atoi(HSys.mapMotTable["MInShuttle1"].c_str());
                    }
                    else if(i==12)
                    {
                        iMot=atoi(HSys.mapMotTable["MInShuttle2"].c_str());
                    }

                    if(iMot<0)                                                  //Steven 20250520 : 變更Mot初始化方式
                    {
                        S.sprintf("Can not find motor %s", Mot_Name);
                    }
                    else
                    {
                        bHasMotor=true;
                    }
                }
                else
                {
                    bHasMotor=true;
                }
            }
            else
            {
                S.sprintf("Can not find motor %s", Mot_Name);
            }

            if(bHasMotor)
            {
                sModel          =HSys.MotTable[iMot]->CardModel;
//                MOT[i].Alias    =HSys.MotTable[iMot]->Alias;
            }
            else
            {
                sModel          ="";
            }
            MOT[i].CardType =sModel;                                            //RogerYang 20250411 ECAT Motor need reinitiated when reopencard
//            MOT[i].NumberAlias  =AnsiString("[")+AnsiString(Mot_Name)+AnsiString("] ")+MOT[i].Alias;

            if(W5aG_INDEX_GALIL_BRANCH_ENABLED &&                               //AI(W906-PT-W5a-integrate) 20260809 GATE (W5a-G): see the note above the flag's definition. golden's condition is just the two lines below.
               INDEX_MOTION_CARD==0 &&                                          //Steven 20210621 : for HT-502 II
               (i==MTestZ1 || i==MTestZ2 || i==MTestY1 || i==MTestY2))
            {
                int iGalilPort=0;                                               //AI(general) 20260316 (RogerYang) : Use TMyGALILMotor for index axes and map MTestY1/Z1/Z2/Y2 to X/Y/Z/W.
                if(i==MTestY1)
                    iGalilPort=0;
                else if(i==MTestZ1)
                    iGalilPort=1;
                else if(i==MTestZ2)
                    iGalilPort=2;
                else
                    iGalilPort=3;

                if(MOT[i].Motor!=NULL)
                {
                    delete MOT[i].Motor;
                    MOT[i].Motor=NULL;
                }
                MOT[i].Motor=new TMyGALILMotor(iGalilPort);
                #ifdef SOFT_SIMULTE
                    MOT[i].Motor->Enable=false;
                #else
                if(USE_INDEX_ARM_AXES==IndexArm_3_Axis &&                       //JimmyChiu 20220708 : add Index Arm Axis
                   (i==MTestY2))
                {
                    MOT[i].Motor->Enable=false;
                }
                else
                {
                    MOT[i].Motor->Enable=true;
                }
                #endif
            }
            else
            {
                if(MOT[i].Motor==NULL)
                {
                    if(sModel=="MN200")                                         //Steven 20150417 : ADD MN200軸控
                    {
                        if(bHasMotor && HSys.MotTable[iMot]->iBoardID!=-1 && HSys.MotTable[iMot]->iPort!=-1)
                            iAdder=HSys.MotTable[iMot]->iBoardID*100+HSys.MotTable[iMot]->iPort;
                        else
                            iAdder=-1;

                        MOT[i].Motor=new TMyMN200Motor(iAdder);
                    }
                    else if(sModel=="MC88X1")                                   //Jimmychiu 20220926 : ADD MC88X1
                    {
                        if(HSys.MotTable[iMot]->iBoardID==-1)
                            iAdder=HSys.MotTable[iMot]->iPort;
                        else
                            iAdder=HSys.MotTable[iMot]->iBoardID*0x10+HSys.MotTable[iMot]->iPort;
//                         MOT[i].Motor = new HTMC88X1Motor(iAdder);            //Steven 20231218 HT7080B
                    }
                    else if(sModel=="SYNTEK")
                    {
                        if(bHasMotor && HSys.MotTable[iMot]->iBoardID!=-1 && HSys.MotTable[iMot]->iPort!=-1 && HSys.MotTable[iMot]->iIP!=-1)
                            iAdder=HSys.MotTable[iMot]->iBoardID*1000+HSys.MotTable[iMot]->iIP*100+HSys.MotTable[iMot]->iPort;
                        else
                            iAdder=-1;
                        MOT[i].Motor=new TMySYNTEKMotor(iAdder);
                    }
                    else if(sModel=="PCI1203")                                  //RogerYang 20250326 整合EtherCAT Mot
                    {
                        if(bHasMotor && HSys.MotTable[iMot]->iBoardID!=-1 && HSys.MotTable[iMot]->iPort!=-1)
                            iAdder=HSys.MotTable[iMot]->iBoardID*100+HSys.MotTable[iMot]->iPort;
                        else
                            iAdder=-1;

                        MOT[i].Motor= new TMyEtherCatMotor(iAdder);
                    }
                    else
                    {
                        if(bHasMotor && HSys.MotTable[iMot]->iBoardID!=-1 && HSys.MotTable[iMot]->iPort!=-1)
                            iAdder=HSys.MotTable[iMot]->iBoardID*10+HSys.MotTable[iMot]->iPort;
                        else
                            iAdder=-1;

                        MOT[i].Motor=new TMySMCMotor(iAdder);

                        if(MOT[i].Mot_Name==MInShuttle1 ||                      //jou 2014-10-09 修正Device superfluous at Output Shuttle誤報錯誤
                           MOT[i].Mot_Name==MInShuttle2)
                            SYN_TEK_MOTION_MODULE=G9004_M204;
                    }
                    #ifdef SOFT_SIMULTE
                        MOT[i].Motor->Enable=false;
                    #else
                        if((sModel=="MC88X1" && iAdder<=0) ||                   //Steven 20240112 : Fixed for HT7080B
                           (sModel!="MC88X1" && iAdder<0)  ||
                           bHasMotor==false)
                        {
                            MOT[i].Motor->Enable=false;
                        }
                        else
                        {
                            MOT[i].Motor->Enable=HSys.MotTable[iMot]->iEnable;
                        }
                    #endif
                }
                else
                {
                    #ifdef SOFT_SIMULTE
                        MOT[i].Motor->Enable=false;
                    #else
                        if(bHasMotor)
                            MOT[i].Motor->Enable=HSys.MotTable[iMot]->iEnable;
                        else
                            MOT[i].Motor->Enable=false;
                    #endif
                }
            }

            if(bHasMotor)
            {
                MOT[i].Motor->GearRatio         = HSys.MotTable[iMot]->dGearRatio;
                MOT[i].Motor->Direction         =(HSys.MotTable[iMot]->iDirection==1)?true:false;
                MOT[i].Motor->HomeDirection     =(HSys.MotTable[iMot]->iHomeDirectior==1)?true:false;
                dAcc=HSys.MotTable[iMot]->dAcc;
                dDec=HSys.MotTable[iMot]->dDec;
                if(sModel=="MN200")                                             //Steven 20230616 : MN200的加減速單位是秒
                {
                    if(dAcc>1)
                        dAcc=HSys.MotTable[iMot]->dAcc/100.0;
                    if(dDec>1)
                        dDec=HSys.MotTable[iMot]->dDec/100.0;
                }
                else if(sModel=="MC88X1")                                       //Nickliu 20240108 add MCXX8 Set Rate
                {
                    dAcc=HSys.MotTable[iMot]->iRate;
                    dDec=HSys.MotTable[iMot]->iRate;
                    MOT[i].Motor->SetRate    (HSys.MotTable[iMot]->iRate);      //Nickliu 20230315 add MCXX8 Set Rate
                }

                MOT[i].Motor->SetAccDataBase     (dAcc);
                MOT[i].Motor->SetDecDataBase     (dDec);
                MOT[i].Motor->SetRange           (HSys.MotTable[iMot]->iRange);
                MOT[i].Motor->PHomeHighSpeed    = HSys.MotTable[iMot]->iHomeHighSpeed;                 //Steven 20231218 HT7080B
                MOT[i].Motor->PHomeLowSpeed     = HSys.MotTable[iMot]->iHomeLowSpeed;
                MOT[i].Motor->PJogHighSpeed     = HSys.MotTable[iMot]->iJogHighSpeed;
                MOT[i].Motor->PJogLowSpeed      = HSys.MotTable[iMot]->iJogLowSpeed;
                MOT[i].Motor->SetInitSpeed       (HSys.MotTable[iMot]->iInitSpeed);
                MOT[i].Motor->InitSpeed         = HSys.MotTable[iMot]->iInitSpeed;
                MOT[i].Motor->PServoAlarmOn     =(HSys.MotTable[iMot]->iServoAlarmOn==1)?true:false;
                MOT[i].Motor->MotorType         = HSys.MotTable[iMot]->i1P2P;
                MOT[i].Motor->bSensorType       = HSys.MotTable[iMot]->iSensorType;
                MOT[i].Motor->bLimitLogic       =(HSys.MotTable[iMot]->iLimitLogic==1)?true:false;
                MOT[i].Motor->bIn1Logic         =(HSys.MotTable[iMot]->iIn1Logic==1)?true:false;
                MOT[i].Motor->PSoftLimitN       = HSys.MotTable[iMot]->iSoftLimitN;
                MOT[i].Motor->PSoftLimitP       = HSys.MotTable[iMot]->iSoftLimitP;
                MOT[i].SimulateSpeed            = HSys.MotTable[iMot]->iSimulateSpeed;
            }
            MOT[i].HomeFlag                 = 0;
            SetMotorAccelSpeed(i, 100);

            if(MOT[i].Mot_Name==MInShuttle1)                                    //kevin 20110531 旋轉shuttle記錄速度
            {
                iInitSpeedSh1=MOT[i].Motor->InitSpeed;
                iPJogHighSpeedSh1=MOT[i].Motor->PJogHighSpeed;
            }

            if(MOT[i].Mot_Name==MInShuttle2)                                    //kevin 20110531 旋轉shuttle記錄速度
            {
                iInitSpeedSh2=MOT[i].Motor->InitSpeed;
                iPJogHighSpeedSh2=MOT[i].Motor->PJogHighSpeed;
            }

            if(bHasMotor && (i==MTestZ1 || i==MTestZ2))                         //Steven 20210623 : Index使用Galil
                MOT[i].IndexPickLimit=HSys.MotTable[iMot]->iPickLimit;

            if(INDEX_MOTION_CARD==0 &&                                          //Steven 20210623 : Index使用Galil
               (i==MTestY1 || i==MTestZ1 || i==MTestZ2 || i==MTestY2))
                ;                                                               //Steven 20090922 需要修改
            else
                MOT[i].SetArmMaxSpeed();

            SoftLimit[0]=MOT[i].Motor->PSoftLimitN;
            SoftLimit[1]=MOT[i].Motor->PSoftLimitP;

            if(MOT[i].Motor->Enable)
                MOT[i].Motor->InitMotor(iAdder);
        }
    }
// ---- GATE 2 --------------------------------------------------------------
// golden cinitial.cpp:3646-4025 -- the ENTIRE `else if(MOTION_CARD_TYPE==0)`
// (system/motor.db) arm AND the trailing `else` (system/motor_SMC.db /
// motor_SMC_XYPitch.db) arm, kept VERBATIM below.
// GATED because: both arms are pure BDE code -- `DataModule1->MotorTable`,
//   `PT->Open()/First()/Next()/Eof/FieldByName()`, and
//   `CheckMotorValue(TTable*, AnsiString, bool)` (golden :3061-3107, which
//   itself needs TTable + MessageDlg + asMotorDatabaseErr).  NONE of those
//   exist in this tree: `grep -rn "class TTable|DataModule1" *.h` finds only
//   database.h:204-207's commented-out shell, and CheckMotorValue has no
//   definition anywhere (see ABSENCE CLAIMS in the hand-off report).
// DEFAULT IS FAITHFUL because: the gate is placed so that the surviving code
//   is exactly `if(IO_CARD_TYPE==...){...}` with nothing dangling, and
//   un-gating is a two-line delete that restores golden's if/else-if/else
//   chain intact (the `else if` still directly follows the closing brace of
//   the active arm).  No statement was moved, reordered or reindented.
// BEHAVIOUR DELTA ON A REAL MACHINE: on a machine whose IO_CARD_TYPE is
//   NEITHER NewIO_MN200(2) NOR PCI_P64C64(3), golden reads motor.db /
//   motor_SMC.db and attaches per-axis SYNTEK/SMC/MN200/Galil drivers with
//   real Enable, GearRatio, speeds, Acc/Dec, Range, soft limits and
//   InitMotor(); this port attaches NOTHING here, so the tail loop at golden
//   :4027-4034 gives every one of the 164 axes `new TMySYNTEKMotor(-1)` with
//   Enable=false and GearRatio=1.0 -- i.e. the whole machine is
//   attached-but-disabled and cannot move, with NO alarm raised (golden's own
//   `ShowMyMessage("motor.db does not exist")` early-return at :3655-3656 is
//   inside this gate too).  Machines on the IO_Table.csv path (the active arm)
//   are unaffected.
#if 0 // GATE 2: blocked by TTable/DataModule1 @database.h:204-207 + CheckMotorValue (golden :3061) -- BDE unported
    else if(MOTION_CARD_TYPE==0)                                                //SYN-TEK Motion模組
    {
        sDBDir=AnsiString(CurrentDir)+"\\system\\motor.db";                     //JerryYang 20150903 檢查Database檔是否存在
        if(FileExists(sDBDir))
        {
            DataModule1->MotorTable->TableName=sDBDir;
        }
        else
        {
            ShowMyMessage("motor.db does not exist");
            return;
        }

        PT=DataModule1->MotorTable;
        PT->Open();

        for(int i=0; i<TOTAL_MOTOR; i++)
        {
            MOT[i].Mot_Name=i;
            PT->First();
            Mot_Name.sprintf("M%02d", i);

            flag=false;
            while(!PT->Eof)
            {
                if(CheckMotorValue(PT, "Motorname", false)==Mot_Name)
                {
                    flag=true;
                    break;
                }
                PT->Next();
            }

            if(flag==false)
            {
                if(MOT[i].Motor==NULL)
                    MOT[i].Motor=new TMySMCMotor(-1);
                 MOT[i].Motor->Enable=false;
                S.sprintf("Can not find motor %s", Mot_Name);
                continue;
            }

            if(CheckMotorValue(PT, "Motorname", false)==Mot_Name)
            {
                MOT[i].Alias=CheckMotorValue(PT, "Alias", false);
                MOT[i].NumberAlias=AnsiString("[")+Mot_Name+AnsiString("] ")+MOT[i].Alias;

                iLane   =(StrToIntDef(CheckMotorValue(PT, "Lane", true), -1));
                iIP     =(StrToIntDef(CheckMotorValue(PT, "IP"  , true), -1));
                iPort   =(StrToIntDef(CheckMotorValue(PT, "Port", true), -1));

                if(iIP==-1 || iLane==-1 || iPort==-1)
                {
                    if(MOT[i].Motor==NULL)
                        MOT[i].Motor=new TMySYNTEKMotor(-1);
                    if(INDEX_MOTION_CARD==0 &&                                  //Steven 20210623 : Index使用Galil
                       (i==MTestY1 || i==MTestZ1 || i==MTestZ2 || i==MTestY2))
                    {
                        int iGalilPort=0;                                       //AI(general) 20260316 (RogerYang) : Keep Galil axis-port mapping consistent in fallback branch.
                        if(i==MTestY1)
                            iGalilPort=0;
                        else if(i==MTestZ1)
                            iGalilPort=1;
                        else if(i==MTestZ2)
                            iGalilPort=2;
                        else
                            iGalilPort=3;

                        if(MOT[i].Motor!=NULL)
                        {
                            delete MOT[i].Motor;
                            MOT[i].Motor=NULL;
                        }
                        MOT[i].Motor=new TMyGALILMotor(iGalilPort);
                        #ifdef SOFT_SIMULTE
                            MOT[i].Motor->Enable=false;
                        #else
                            if(USE_INDEX_ARM_AXES==IndexArm_3_Axis &&           //JimmyChiu 20220708 : add Index Arm Axis
                               (i==MTestY2))
                            {
                                MOT[i].Motor->Enable=false;
                            }
                            else
                            {
                                MOT[i].Motor->Enable=true;
                            }
                        #endif
                    }
                    else
                    {
                        MOT[i].Motor->Enable=false;
                    }
                }
                else
                {
                    if(MOT[i].Motor==NULL)
                        MOT[i].Motor = new TMySYNTEKMotor((iLane*1000+iIP*10+iPort));
                    #ifdef SOFT_SIMULTE
                        MOT[i].Motor->Enable=false;
                    #else
                        MOT[i].Motor->Enable=(atoi(CheckMotorValue(PT,"Enable"   ,true).c_str())==1)?true:false;
                    #endif
                }

                if(MOT[i].Motor->Enable)
                {
                    if(CheckMotorValue(PT,"GearRatio",true)!="" &&
                       CheckMotorValue(PT,"GearRatio",true)!="0")
                    {
                        MOT[i].Motor->GearRatio    = atof(CheckMotorValue(PT,"GearRatio"      ,true).c_str());
                    }
                    else
                    {
                        MOT[i].Motor->GearRatio=1.0;
                    }

                    MOT[i].Motor->Direction         =(atoi(CheckMotorValue(PT,"Direction"      ,true).c_str())==1)?true:false;
                    MOT[i].Motor->HomeDirection     =(atoi(CheckMotorValue(PT,"HomeDirectior"  ,true).c_str())==1)?true:false;
                    MOT[i].Motor->PHomeHighSpeed    = atoi(CheckMotorValue(PT,"HomeHighSpeed"  ,true).c_str());
                    MOT[i].Motor->PHomeLowSpeed     = atoi(CheckMotorValue(PT,"HomeLowSpeed"   ,true).c_str());
                    MOT[i].Motor->PJogHighSpeed     = atoi(CheckMotorValue(PT,"JogHighSpeed"   ,true).c_str());
//                    MOT[i].Motor->PJogLowSpeed      = atoi(CheckMotorValue(PT,"JogLowSpeed"    ,true).c_str());   //Steven 20200811 : 先達版本沒有JogLowSpeed
                    MOT[i].Motor->InitSpeed         = atoi(CheckMotorValue(PT,"InitSpeed"      ,true).c_str());
                    MOT[i].Motor->PServoAlarmOn     =(atoi(CheckMotorValue(PT,"ServoAlarmOn"   ,true).c_str())==1)?true:false;
                    MOT[i].Motor->MotorType         = atoi(CheckMotorValue(PT,"1P2P"           ,true).c_str());
                    MOT[i].Motor->bSensorType       = atoi(CheckMotorValue(PT,"HomeSenLogic"   ,true).c_str());
//                        MOT[i].SimulateSpeed            = atoi(CheckMotorValue(PT,"SimulateSpeed"  )->AsString.c_str());
                    MOT[i].HomeFlag                 = 0;

                    if(MOT[i].Mot_Name==MInShuttle1)                            //kevin 20110531 旋轉shuttle記錄速度
                    {
                        iInitSpeedSh1=MOT[i].Motor->InitSpeed;
                        iPJogHighSpeedSh1=MOT[i].Motor->PJogHighSpeed;
                    }

                    if(MOT[i].Mot_Name==MInShuttle2)                            //kevin 20110531 旋轉shuttle記錄速度
                    {
                        iInitSpeedSh2=MOT[i].Motor->InitSpeed;
                        iPJogHighSpeedSh2=MOT[i].Motor->PJogHighSpeed;
                    }

                    SoftLimit[0]=atoi(CheckMotorValue(PT,"SoftLimitN",true).c_str());
                    SoftLimit[1]=atoi(CheckMotorValue(PT,"SoftLimitP",true).c_str());

                    if(INDEX_MOTION_CARD==0 &&  (i==MTestZ1 || i==MTestZ2))     //Steven 20210623 : Index使用Galil
                        MOT[i].IndexPickLimit=atoi(CheckMotorValue(PT, "PickLimit", true).c_str());

                    if(INDEX_MOTION_CARD==0 &&                                  //Steven 20210623 : Index使用Galil
                       (i==MTestY1 || i==MTestZ1 || i==MTestZ2 || i==MTestY2))
                        ;                                                       //Steven 20090922 需要修改
                    else
                        MOT[i].SetArmMaxSpeed();
                }

                MOT[i].Motor->PSoftLimitN=atoi(CheckMotorValue(PT,"SoftLimitN",true).c_str());
                MOT[i].Motor->PSoftLimitP=atoi(CheckMotorValue(PT,"SoftLimitP",true).c_str());
                SoftLimit[0]=MOT[i].Motor->PSoftLimitN;
                SoftLimit[1]=MOT[i].Motor->PSoftLimitP;

                //--------------------------------------------------------------
                //PCI Board
                MOT[i].Mot_Name=i;
                dAcc=PT->FieldByName("ADc")->AsFloat;
                dDec=PT->FieldByName("ADc")->AsFloat;
                if(sModel=="MN200")                                             //Steven 20230616 : MN200的加減速單位是秒
                {
                    if(dAcc>1)
                        dAcc=dAcc/100.0;
                    if(dDec>1)
                        dDec=dDec/100.0;
                }
                MOT[i].Motor->SetAccDataBase(dAcc);
                MOT[i].Motor->SetDecDataBase(dDec);

                if(MOT[i].Motor->Enable)
                {
                    if(INDEX_MOTION_CARD==0 &&                                  //Steven 20210623 : Index使用Galil
                       (i==MTestY1 || i==MTestZ1 || i==MTestZ2 || i==MTestY2))
                        ;
                    else
                        MOT[i].Motor->SetSoftLimit(SoftLimit[0], SoftLimit[1]);
                }
            }
        }
    }
    else
    {
        if(USE_IN_Y_IS_AUTO_PITCH==true || USE_OUT_Y_IS_AUTO_PITCH==true)                                              //Steven 20170424 (wei) : new XY變距  //JerryYang 20251218 : IN/OUT ARM支援不同模組
            DataModule1->MotorTable->TableName=CurrentDir+"\\system\\motor_SMC_XYPitch.db";
        else
            DataModule1->MotorTable->TableName=CurrentDir+"\\system\\motor_SMC.db";

        PT=DataModule1->MotorTable;
        PT->Open();

        for(int i=0; i<TOTAL_MOTOR; i++)
        {
            PT->First();
            MOT[i].Mot_Name=i;
            Mot_Name.sprintf("M%02d", i);

            flag=false;
            while(!PT->Eof)
            {
                if(CheckMotorValue(PT, "Motorname", false)==Mot_Name)
                {
                    flag=true;
                    break;
                }
                PT->Next();
            }

            if(flag==false)
            {
                if(MOT[i].Motor==NULL)
                    MOT[i].Motor = new TMySMCMotor(-1);
                 MOT[i].Motor->Enable=false;
                S.sprintf("Can not find motor %s", Mot_Name);
                continue;
            }

            S=CheckMotorValue(PT, "Motorname", false);
            if(CheckMotorValue(PT, "Motorname", false)==Mot_Name)
            {
                S=CheckMotorValue(PT, "Port", true);
                if(PT->FindField("CardModel")!=NULL)
                {
                    sModel=CheckMotorValue(PT, "CardModel", false);
                }

                MOT[i].Alias        =CheckMotorValue(PT,"Alias"          , false);
                MOT[i].NumberAlias  =AnsiString("[")+Mot_Name+AnsiString("] ")+MOT[i].Alias;

                if(sModel=="SMC" || sModel=="MN200" || sModel=="MC88X1")        //Steven 20150417 : ADD MN200軸控  //Jimmychiu 20220926 : ADD MC88X1
                {
                    if(INDEX_MOTION_CARD==0 &&                                  //Steven 20210623 : Index使用Galil
                       (i==MTestY1 || i==MTestZ1 || i==MTestZ2 || i==MTestY2))
                    {
                        int iGalilPort=0;                                       //AI(general) 20260316 (RogerYang) : Instantiate TMyGALILMotor for index Galil axes in SMC/MN200/MC88X1 branch.
                        if(i==MTestY1)
                            iGalilPort=0;
                        else if(i==MTestZ1)
                            iGalilPort=1;
                        else if(i==MTestZ2)
                            iGalilPort=2;
                        else
                            iGalilPort=3;

                        if(MOT[i].Motor!=NULL)
                        {
                            delete MOT[i].Motor;
                            MOT[i].Motor=NULL;
                        }
                        MOT[i].Motor = new TMyGALILMotor(iGalilPort);
                        #ifdef SOFT_SIMULTE
                            MOT[i].Motor->Enable=false;
                        #else
                            if(USE_INDEX_ARM_AXES==IndexArm_3_Axis &&
                               (i==MTestY2))                                    //JimmyChiu 20220708 : add Index Arm Axis
                            {
                                MOT[i].Motor->Enable=false;
                            }
                            else
                            {
                                MOT[i].Motor->Enable=true;
                            }
                        #endif
                    }
                    else
                    {
                        if(MOT[i].Motor==NULL)
                        {
                            if(sModel=="MN200")                                 //Steven 20150417 : ADD MN200軸控
                            {
                                iAdder=(atoi(CheckMotorValue(PT, "BoardID", true).c_str()))*100;
                                iAdder+=atoi(CheckMotorValue(PT, "Port", true).c_str());
                                MOT[i].Motor = new TMyMN200Motor(iAdder);
                            }
/*                            else if(sModel=="MC88X1")                         //Jimmychiu 20220926 : ADD MC88X1        //Steven 20231218 HT7080B
                            {
                                iAdder=atoi(CheckMotorValue(PT, "Port", true).c_str());
                                MOT[i].Motor = new HTMC88X1Motor(iAdder);
                            }*/
                            else
                            {
                                iAdder=(atoi(CheckMotorValue(PT, "BoardID", true).c_str()))*10;
                                iAdder+=atoi(CheckMotorValue(PT, "Port", true).c_str());
                                MOT[i].Motor = new TMySMCMotor(iAdder);
                            }
                        }
                        #ifdef SOFT_SIMULTE
                            MOT[i].Motor->Enable=false;
                        #else
                            MOT[i].Motor->Enable=(atoi(CheckMotorValue(PT, "Enable", true).c_str())==1)?true:false;
                        #endif

                        if(MOT[i].Mot_Name==MInShuttle1 ||                      //jou 2014-10-09 修正Device superfluous at Output Shuttle誤報錯誤
                           MOT[i].Mot_Name==MInShuttle2)
                        {
                            SYN_TEK_MOTION_MODULE=G9004_M204;
                        }
                    }

                    if(CheckMotorValue(PT, "GearRatio", true)!="" &&
                       CheckMotorValue(PT, "GearRatio", true)!="0")
                    {
                        MOT[i].Motor->GearRatio    = atof(CheckMotorValue(PT, "GearRatio"      ,true).c_str());
                    }
                    else
                    {
                        MOT[i].Motor->GearRatio=1.0;
                    }

                    MOT[i].Motor->Direction         =(atoi(CheckMotorValue(PT,"Direction"      ,true).c_str())==1)?true:false;
                    MOT[i].Motor->HomeDirection     =(atoi(CheckMotorValue(PT,"HomeDirectior"  ,true).c_str())==1)?true:false;
                    MOT[i].Motor->PHomeHighSpeed    = atoi(CheckMotorValue(PT,"HomeHighSpeed"  ,true).c_str());
                    MOT[i].Motor->PHomeLowSpeed     = atoi(CheckMotorValue(PT,"HomeLowSpeed"   ,true).c_str());
                    MOT[i].Motor->PJogHighSpeed     = atoi(CheckMotorValue(PT,"JogHighSpeed"   ,true).c_str());
                    MOT[i].Motor->PJogLowSpeed      = atoi(CheckMotorValue(PT,"JogLowSpeed"    ,true).c_str());
                    MOT[i].Motor->InitSpeed         = atoi(CheckMotorValue(PT,"InitSpeed"      ,true).c_str());
                    MOT[i].Motor->PServoAlarmOn     =(atoi(CheckMotorValue(PT,"ServoAlarmOn"   ,true).c_str())==1)?true:false;
                    MOT[i].Motor->MotorType         = atoi(CheckMotorValue(PT,"1P2P"           ,true).c_str()); //
                    MOT[i].Motor->bSensorType       = atoi(CheckMotorValue(PT,"SensorType"     ,true).c_str()); //
                    dAcc=atof(CheckMotorValue(PT,"Acc"            ,true).c_str());
                    dDec=atof(CheckMotorValue(PT,"Dec"            ,true).c_str());
                    if(sModel=="MN200")                                         //Steven 20230616 : MN200的加減速單位是秒
                    {
                        if(dAcc>1)
                            dAcc=dAcc/100.0;
                        if(dDec>1)
                            dDec=dDec/100.0;
                    }
                    MOT[i].Motor->SetAccDataBase     (dAcc);
                    MOT[i].Motor->SetDecDataBase     (dDec);
                    if(sModel=="MC88X1")                                        //Nickliu 20230315 add MCXX8 Set Rate
                    {
                        MOT[i].Motor->SetRate        (atoi(CheckMotorValue(PT,"Rate"           ,true).c_str()));//Nickliu 20230315 add MCXX8 Set Rate
                    }
                    MOT[i].Motor->SetRange           (atoi(CheckMotorValue(PT,"Range"          ,true).c_str()));
                    MOT[i].Motor->bLimitLogic       =(atoi(CheckMotorValue(PT,"LimitLogic"     ,true).c_str())==1)?true:false;
                    MOT[i].Motor->bIn1Logic         =(atoi(CheckMotorValue(PT,"In1Logic"       ,true).c_str())==1)?true:false;
                    MOT[i].SimulateSpeed            = atoi(CheckMotorValue(PT,"SimulateSpeed"  ,true).c_str());
                    SetMotorAccelSpeed(i, 100);
                    MOT[i].HomeFlag                 = 0;

                    if(MOT[i].Mot_Name==MInShuttle1)                            //kevin 20110531 旋轉shuttle記錄速度
                    {
                        iInitSpeedSh1=MOT[i].Motor->InitSpeed;
                        iPJogHighSpeedSh1=MOT[i].Motor->PJogHighSpeed;
                    }

                    if(MOT[i].Mot_Name==MInShuttle2)                            //kevin 20110531 旋轉shuttle記錄速度
                    {
                        iInitSpeedSh2=MOT[i].Motor->InitSpeed;
                        iPJogHighSpeedSh2=MOT[i].Motor->PJogHighSpeed;
                    }

                    SoftLimit[0]=atoi(CheckMotorValue(PT, "SoftLimitN", true).c_str());
                    SoftLimit[1]=atoi(CheckMotorValue(PT, "SoftLimitP", true).c_str());

                    if(INDEX_MOTION_CARD==0 &&  (i==MTestZ1 || i==MTestZ2))     //Steven 20210623 : Index使用Galil
                        MOT[i].IndexPickLimit=atoi(CheckMotorValue(PT, "PickLimit", true).c_str());

                    if(INDEX_MOTION_CARD==0 && (i==MTestY1 || i==MTestZ1 || i==MTestZ2 || i==MTestY2))     //Steven 20210623 : Index使用Galil
                        ;                                                       //Steven 20090922 需要修改
                    else
                        MOT[i].SetArmMaxSpeed();

                    MOT[i].Motor->PSoftLimitN=atoi(CheckMotorValue(PT,"SoftLimitN",true).c_str());
                    MOT[i].Motor->PSoftLimitP=atoi(CheckMotorValue(PT,"SoftLimitP",true).c_str());
                    SoftLimit[0]=MOT[i].Motor->PSoftLimitN;
                    SoftLimit[1]=MOT[i].Motor->PSoftLimitP;

                    if(MOT[i].Motor->Enable)
                        MOT[i].Motor->InitMotor(iAdder);
                }
            }
        }
        PT->Close();
    }
#endif

    for(int i=0; i<TOTAL_MOTOR; i++)
    {
        if(MOT[i].Motor==NULL)
        {
            MOT[i].Motor=new TMySYNTEKMotor(-1);
            MOT[i].Motor->Enable=false;
            MOT[i].Motor->GearRatio=1.0;
        }

        if(MOT[i].Motor->Enable==true)
        {
            // ---- GATE 3 --------------------------------------------------
            // golden cinitial.cpp:4038 --
            //   MOT[i].Motor->MotorIdleSafeDoorCheck=IdleCheckSafeDoor;
            // GATED because: `IdleCheckSafeDoor` is DECLARED (csystem.h:239)
            //   but has NO DEFINITION anywhere in this tree -- golden's body is
            //   golden csystem.cpp:2749 and the ported csystem.cpp contains 0
            //   occurrences of the name.  This is an ADDRESS-OF, not a call, so
            //   it is a hard undefined reference at link the moment this TU is
            //   linked -- -fsyntax-only would NOT have caught it.
            // DEFAULT IS FAITHFUL because: HTMotor::HTMotor() already sets
            //   MotorIdleSafeDoorCheck=NULL (Motor/HTMotor.cpp:51), which is
            //   also golden's pre-assignment value, so skipping the assignment
            //   leaves the field at the only value the tree can produce.
            // BEHAVIOUR DELTA ON A REAL MACHINE -- SAFETY-RELEVANT, READ THIS:
            //   HTMotor::CheckIsSafeDoorOpen() (Motor/HTMotor.cpp:111-126) falls
            //   back to `return (Enable==true)` when the callback is NULL.  So
            //   with this gate every ENABLED axis reports "safe door OPEN" and
            //   TMyMotor::JogP/JogN (mymotor.cpp:728-741) and the other
            //   CheckIsSafeDoorOpen consumers REFUSE TO MOVE.  The failure is
            //   fail-safe (motion blocked, never wrongly permitted), but it
            //   means jog/home will look dead on a real machine until the wave
            //   that translates golden csystem.cpp:2749 lands the body and this
            //   gate is flipped.  Flipping it needs `#include "csystem.h"`.
#if 0 // GATE 3: blocked by IdleCheckSafeDoor (declared csystem.h:239, body golden csystem.cpp:2749 -- 0 definitions in this tree)
            MOT[i].Motor->MotorIdleSafeDoorCheck=IdleCheckSafeDoor;             // 2015.01.15 , Joye , Safe door check
#endif
            if(INDEX_MOTION_CARD==0 &&                                          //Steven 20210623 : Index使用Galil
               (i==MTestY1 || i==MTestZ1 || i==MTestZ2 || i==MTestY2))
            {
                ;
            }
            else
            {
                MOT[i].PCIL132_StopMotor();
                //2013-03-27    Dell -----------------
                //InitMotor(0)絕對不可以跟"=new TMySYNTEKMotor((iLane*1000+iIP*10+iPort));"
                //放在同一個迴圈，會造成送出去的pulse count與command count對不起來．至於"為什麼""我在function抬頭解釋
                MOT[i].Motor->InitMotor(0);
                //2013-03-27    Dell -----------------
            }
        }
    }
//    delete PT;
    #ifdef DEBUG_AutoHomeLog
    if(fAllMotorHome==true)
    {
        NewRecordProcess("", "fAllMotorHome", "InitialMotorParameter");
    }
    #endif
    fAllMotorHome=false;

    for(int x=0; x<_MAX_COL_ITEM; x++)                                          //Steven 20221005 : Production Log減少記憶體使用量
    {
        for(int y=0; y<_MAX_ROW_ITEM; y++)
        {
            MOT[MMTrayY         ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MMPlate1        ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MMPlate2        ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MMAuto1         ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MMAuto2         ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MMAuto3         ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MMAuto4         ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MMAuto5         ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MMAuto6         ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MManualTray1    ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MManualTray2    ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MManualTray3    ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MManualTray4    ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MManualTray5    ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MManualTray6    ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MOutRotateKit   ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MInRotateKit    ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MMBulkboxKit    ].Tray.PordRec[x][y]=new TMyProductionRecord(); //kevin 20221005 add error bin box
        }
    }

    // ---- GATE 4 ----------------------------------------------------------
    // golden cinitial.cpp:4089 -- fHome->InitialHomeClass();
    // GATED because: the tree's TfHome is the deliberately minimal facade
    //   forms/fHome.h:54 (iHomeStep / fShow / fAbort only -- its own banner
    //   states it declares nothing else on purpose so nobody "retires" an
    //   untranslated uhome.cpp method).  There is no InitialHomeClass anywhere:
    //   0 hits tree-wide.  Golden's body lives in the unported uhome.cpp.
    // DEFAULT IS FAITHFUL because: golden's own comment on this line is
    //   "Steven 20240603 : 修正歸零馬達名稱消失問題" -- it re-publishes MOT[]
    //   aliases into the Home FORM's widget list.  It is a pure UI refresh; it
    //   writes nothing this function computed, and no non-UI consumer reads it.
    //   Skipping it changes no motor state.
    // BEHAVIOUR DELTA ON A REAL MACHINE: the Home screen's per-axis name
    //   labels are not refreshed after a re-init, i.e. exactly the
    //   "歸零馬達名稱消失" symptom Steven fixed on 20240603 comes back --
    //   cosmetic on the Home dialog, no motion / interlock / alarm effect.
    //   Un-gate together with uhome.cpp (also needs `#include "forms/fHome.h"`).
#if 0 // GATE 4: blocked by TfHome::InitialHomeClass (golden uhome.cpp; forms/fHome.h:54 facade has 3 members, 0 methods)
    fHome->InitialHomeClass();                                                  //Steven 20240603 : 修正歸零馬達名稱消失問題
#endif
    BYTE bDevNo_In[4]= {MOT[MInArmPitch  ].Motor->iPortID,
                        MOT[MInArmPitchX2].Motor->iPortID,
                        MOT[MInArmPitchX3].Motor->iPortID,
                        MOT[MInArmPitchX4].Motor->iPortID};
    BYTE bDevNo_Put[4]={MOT[MOutArmPitch  ].Motor->iPortID,
                        MOT[MOutArmPitchX2].Motor->iPortID,
                        MOT[MOutArmPitchX3].Motor->iPortID,
                        MOT[MOutArmPitchX4].Motor->iPortID};

    MOT[MInArmPitch].Motor->SetGroup(1, 4, bDevNo_In);
    MOT[MOutArmPitch].Motor->SetGroup(2, 4, bDevNo_Put);
}
//==============================================================================
