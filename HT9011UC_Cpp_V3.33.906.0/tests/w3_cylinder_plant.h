// =============================================================================
//  w3_cylinder_plant.h  --  test-only "plant model" for the tray-lifter and
//                           Auto-stack cylinders.
//
//  AI(W906-W7-L1-Wave3) 20260802.  HEADER-ONLY, tests/ only, never linked into
//  the product.  No CMakeLists change is needed to use it: it is #included by
//  path from the test .cpp files that live beside it.
//
//  WHY IT EXISTS
//  -------------
//  Wave 3 replaced acatchtray_shims.cpp's three `{ return true; }` AutoCylinder*
//  stubs and asendic.cpp's Sim CylinderUp/Middle/Lower with golden's REAL state
//  machines.  Those machines are closed loops: they command Cylinder[].On()/Off()
//  and then wait for Cylinder[].OnStatus()/OffStatus() to report that the metal
//  arrived.  A default-constructed TMyCylinder has Enable==false and both
//  sensor-enables false, so OnStatus() and OffStatus() BOTH return false forever
//  and the loops never close -- see the analysis banner in asendic.cpp.
//
//  The correct repair for a test that used to ride on "the stub returns true" is
//  therefore NOT to weaken its assertion: it is to CONFIGURE THE FIXTURE LIKE A
//  REAL MACHINE -- enable the cylinders, give them distinct Sim IO addresses,
//  set the sensor types and the alarm times -- and then let something play the
//  part of the physical stack, moving the sensor inputs in response to the
//  commanded outputs.  That "something" is this header.
//
//  THE PLANT
//  ---------
//  Every wired cylinder gets three DISTINCT Sim IO addresses on one IP:
//        output bit  -> (ring 0, ip, port 0, bit)
//        ON  sensor  -> (ring 0, ip, port 1, bit)
//        OFF sensor  -> (ring 0, ip, port 2, bit)
//  TSimIOBackend keeps one in-memory byte per (ring, ip, port), and TLaneIO's
//  CheckPortRangeErr short-circuits to "ok" while InitialOK==false, so these
//  addresses round-trip exactly.  Keeping the sensors OFF the output address is
//  what lets the plant, not the command, decide what the sensors say.
//
//  LIFTER pair (C_*_Up, C_*_Middle) -- three physical positions:
//        top    = Up commanded ON  and Middle commanded ON
//        middle = Up commanded ON  and Middle commanded OFF
//        bottom = Up commanded OFF
//        Up.On   sensor := (position==top)      Up.Off   sensor := (position==bottom)
//        Mid.On  sensor := (position==middle)   Mid.Off  sensor := (position==bottom)
//    NOTE ON MID.OFF -- THE ASYMMETRY IS DELIBERATE.  AI(W906-W7-L1-W3fixB)
//    20260802: `bottom` is `!oA`, so the Middle cylinder's OFF sensor ignores the
//    Middle cylinder's OWN command while every other sensor in this law reads both.
//    That is a real asymmetry and it is kept, because the alternative law
//    (`!oA && !oB`) is INDISTINGUISHABLE from it under golden.  Re-derived from
//    asendic.cpp in this pass: Mid.OffStatus() is read at exactly three places --
//    CylinderUp case 100 (:363), CylinderLower case 100 (:607) and case 200 (:622).
//    CylinderUp reads it having commanded BOTH cylinders ON in the same case, and
//    CylinderLower reads it having commanded BOTH OFF (:584-585 and :597-598), so
//    every read happens with oA==oB and the two laws agree.  The one golden state
//    with oA==false && oB==true is CylinderUp case 1->50->60 (:318 turns Middle on
//    before Up is ever commanded), and no case in that window reads Mid.OffStatus()
//    at all.  Modelling `!oA && !oB` would therefore add an untestable difference,
//    so the simpler law stands -- documented rather than silently unequal.
//    Checked against the golden bodies: CylinderUp case 100 wants
//    Up.OnStatus() && !Mid.OnStatus() && !Mid.OffStatus() -> satisfied at `top`;
//    CylinderMiddle case 100 wants !Up.OnStatus() && Mid.OnStatus() -> satisfied
//    at `middle`; CylinderLower case 100 wants Mid.OffStatus() -> satisfied at
//    `bottom`.  No STATIC sensor configuration can satisfy all three (they
//    disagree on every bit), which is exactly why a plant is required rather
//    than a one-off fixture tweak.
//
//  AUTO-STACK pair -- KEYED BY PARAMETER POSITION, NOT BY CYLINDER NAME.  This
//  is deliberate and it is the whole point of the argument-order pin.  Golden's
//  AutoCylinderUp/Middle/Lower take (Part, CylinderName, CylinderNameMid) and
//  treat the two cylinders completely differently:
//        param2 (CylinderName)    is commanded UNCONDITIONALLY
//        param3 (CylinderNameMid) is Enable-guarded and is the SOLE input to
//                                 AutoCylinderMidIsOn -- the "arrived at the top"
//                                 witness.
//  Golden callers DISAGREE about which physical cylinder goes in which slot:
//  asendic_Auto.cpp / _Auto_RT.cpp / csystem.cpp / acatchtray.cpp normally pass
//  (C_Auto_Up[i], C_Auto_Selector[i]); asendic_Auto2.cpp passes the REVERSE at
//  all 12 of its sites; and eleven further sites swap at RUN TIME on
//  bARTUnloaderUseTwoCylin.  None of that may be "normalised".
//  So WireAutoStack takes the pair in CALL-SITE ORDER and models:
//        param2.On  sensor := (param2 commanded ON  and param3 commanded OFF)
//        param2.Off sensor := (param2 commanded OFF)
//        param3.On  sensor := (param3 commanded ON)
//        param3.Off sensor := (param3 commanded OFF)
//    Checked against the golden non-ART bodies: AutoCylinderUp case 100/201 want
//    param2.OnStatus()==false and param3.OnStatus()==true; AutoCylinderMiddle
//    case 100/201 want param2.OnStatus()==true and param3.OnStatus()==false;
//    AutoCylinderLower case 100/201 want param2.OffStatus()==true and both
//    OnStatus() false.
//  CONSEQUENCE, and this is the pin: a test fixture must wire the pair in the
//  SAME order its SM passes them.  Swap either the fixture or the call site
//  alone and the SM stops converging.  asendic_Auto2.cpp's tests therefore call
//  WireAutoStack(C_Auto2_Selector, C_Auto2_Up, ...) -- the reversal, recorded in
//  the fixture -- while the asendic_Auto* tests call
//  WireAutoStack(C_Auto_Up[i], C_Auto_Selector[i], ...).
//
//  AUTO-STACK, ART VARIANT (WireAutoStackART) -- when bARTUnloaderUseTwoCylin is
//  true the golden bodies take completely different arms and want the OPPOSITE
//  readings, so the ART fixture needs its own law:
//        param2.On  sensor := (param2 commanded ON  and param3 commanded ON)
//        param2.Off sensor := (param2 commanded OFF)
//        param3.On  sensor := (param3 commanded OFF and param2 commanded ON)
//        param3.Off sensor := (param3 commanded OFF and param2 commanded OFF)
//    Checked against the golden ART arms: AutoCylinderUp case 100 wants
//    param2.OnStatus() and AutoCylinderMidIsOn's two-cylinder arm (param3 neither
//    on NOR off); AutoCylinderMiddle case 100/201 want !param2.OnStatus() and
//    param3.OnStatus(); AutoCylinderLower case 100 wants param3.OffStatus() and
//    its case 201 reads Cylinder[C_Auto_Selector[Part]].OffStatus() BY NAME
//    (golden :1102, "kevin 20220527 change sensor") rather than by parameter --
//    which this law satisfies from either side of the runtime swap.
//
//  TICK CONTRACT
//  -------------
//  Call w3::Tick() ONCE at the top of every SM tick, BEFORE calling the SM.  It
//    (a) expires LifterTime[][] and AutoTime[][] -- the same "virtual time"
//        idiom the existing tests already use on their own TQPF_Timers -- but
//        deliberately NOT CylinderAlarmTime[]; and
//    (b) recomputes every wired pair's sensors from the commanded outputs.
//
//  WHY CylinderAlarmTime[] IS EXCLUDED -- AI(W906-W7-L1-W3fixB) 20260802.  This
//  used to be justified as "expiring that one would manufacture an alarm", which is
//  true but does not distinguish it from the timers Tick() DOES expire: LifterTime
//  and AutoTime are alarm timers too (AutoCylinderUp's case-100 "Lifter Up error"
//  arm is `else if(AutoTime[0][Part].Off())`).  THE REAL DISTINCTION IS DUAL-ROLE
//  vs PURE-ALARM, re-derived from asendic.cpp in this pass:
//    * LifterTime[][] / AutoTime[][] are PROGRESS timers that double as the alarm
//      deadline, and in every arm the SUCCESS branch is evaluated FIRST -- case 100
//      is `if(<sensors say arrived>) {...} else if(<timer>.Off()) {<alarm>}`, and
//      case 200 / case 201 use the very same `.Off()` as the SUCCESS condition
//      (`if(AutoTime[0][Part].Off()) { CylinderAlarmTime[0].SetSecAndOn(10); Task++; }`).
//      Expiring them therefore ACCELERATES a converging machine and only alarms a
//      machine that was going to alarm anyway.  That is what "virtual time" means
//      here.
//    * CylinderAlarmTime[] has NO success branch.  It is the first statement of
//      case 201: `if(CylinderAlarmTime[0].Off()) { ShowMyMessage(...); Task=1;
//      return false; }`.  Expiring it fires unconditionally, before any sensor is
//      consulted.  THAT is why it is excluded, and the distinction is not stylistic.
//  CONSEQUENCE, and a test depends on it: because case 100 re-arms AutoTime only on
//  the pass where bAuto2Pause/bHandlerPause is still set, and Tick() expires it
//  again on the next pass, THE EFFECTIVE CASE-100 WINDOW IS ONE TICK.  A correct
//  call site converges inside that tick; a swapped one cannot, and alarms on the
//  next.  test_w7_l1_auto2.cpp sub-test [12b] asserts exactly that difference
//  (0 dialogs vs many), so shortening or lengthening this window changes what [12b]
//  measures.
//  The sensors therefore lag the command by exactly one tick, which is what real
//  metal does.  The golden SMs tolerate that lag because the tick on which they
//  first issue the command is also the tick on which bLifterPause/bAuto2Pause
//  re-arms the alarm timer, so the "did not arrive" branch cannot fire yet.
//  That is also why WireLifter/WireAutoStack set a NON-ZERO OnAlarmTime /
//  OffAlarmTime: with golden's default of 0 the re-arm expires instantly and the
//  very first pass alarms.  A real machine never configures 0 either.
// =============================================================================
#ifndef W3_CYLINDER_PLANT_H
#define W3_CYLINDER_PLANT_H

#include <vector>

#include "../mycylin.h"         // Cylinder[] / TMyCylinder
#include "../MyLaneIo.h"        // MyLaneIO (IOBitOn / IOBitOff)
#include "../MachineType.h"     // eMotionNet, TYPE_A, MAX_UNLOAD_TRAY, MAX_AUTO_TRAY
#include "../cmydef.h"          // C_Load_Up / C_Empty_Up / C_Color_Up / C_Auto_*

// ---------------------------------------------------------------------------
//  asendic.cpp file-scope state the plant has to reach.
//
//  ROW-STRIDE WARNING -- these two declarations use the DEFINITION's strides
//  (golden asendic.cpp:28 `int iLifterTask[3][10];` and :35
//  `int iAutoTask[3][MAX_UNLOAD_TRAY];`).  Golden main.cpp:9136-9137 declares
//  BOTH as [3][7] instead, which is a genuine golden defect handed to the W7-U
//  wave; do not "align" with it here -- [3][7] would index the wrong storage.
// ---------------------------------------------------------------------------
extern TQPF_Timer LifterTime[3][3];                     // golden asendic.cpp:24
extern int        iLifterTask[3][10];                   // golden asendic.cpp:28
extern TQPF_Timer AutoTime[3][MAX_UNLOAD_TRAY];         // golden asendic.cpp:30
extern int        iAutoTask[3][MAX_UNLOAD_TRAY];        // golden asendic.cpp:35
extern TQPF_Timer CylinderAlarmTime[MAX_AUTO_TRAY];     // golden asendic.cpp:36
extern void       initLifterTask();                     // golden asendic.cpp:48
extern void       initAutoTask();                       // golden asendic.cpp:38

namespace w3 {

// ---------------------------------------------------------------------------
//  Registered pairs.  `lifter` selects which of the two sensor laws applies.
// ---------------------------------------------------------------------------
enum PlantLaw { LAW_LIFTER = 0, LAW_AUTO = 1, LAW_AUTO_ART = 2 };
struct PlantPair { int iA; int iB; PlantLaw law; };

inline std::vector<PlantPair>& Pairs()
{
    static std::vector<PlantPair> v;
    return v;
}

// ---------------------------------------------------------------------------
//  WireOne -- give one cylinder a realistic Sim IO wiring.
//    ip  : the Sim IO "IP" slot this pair owns (must be >= 1: TLaneIO rejects
//          the all-zero address, which is every un-wired cylinder's default and
//          is why un-wired cylinders are inert rather than aliased onto bit 0).
//    bit : 0 for the first cylinder of the pair, 1 for the second.
// ---------------------------------------------------------------------------
inline void WireOne(int c, int ip, int bit)
{
    TMyCylinder &C = Cylinder[c];
    C.Enable        = true;

    C.OutISABase    = eMotionNet;
    C.OutType       = TYPE_A;               // On() -> IOBitOn, Off() -> IOBitOff
    C.OutRing = 0;  C.OutIP = ip;  C.OutPort = 0;  C.OutBit = bit;

    C.OnSenEnable   = true;
    C.OnSenISABase  = eMotionNet;
    C.OnSenType     = TYPE_A;               // sensor made == bit high
    C.OnSenRing = 0; C.OnSenIP = ip; C.OnSenPort = 1; C.OnSenBit = bit;

    C.OffSenEnable  = true;
    C.OffSenISABase = eMotionNet;
    C.OffSenType    = TYPE_A;
    C.OffSenRing = 0; C.OffSenIP = ip; C.OffSenPort = 2; C.OffSenBit = bit;

    // Non-zero alarm windows -- see the TICK CONTRACT note.  50 == 5.0 s at the
    // 0.1-s granularity Set0_1SecAndOn uses; a real Cylinder .DB row is in this
    // range.  The delays stay 0 so the post-arrival dwell is one tick.
    C.OnAlarmTime   = 50;
    C.OffAlarmTime  = 50;
    C.OnDelayTime   = 0;
    C.OffDelayTime  = 0;

    C.Off();                                // start at the retracted end stop
}

inline void SetSensors(int c, bool onMade, bool offMade)
{
    TMyCylinder &C = Cylinder[c];
    if (onMade)  MyLaneIO.IOBitOn (C.OnSenRing,  C.OnSenIP,  C.OnSenPort,  C.OnSenBit,  C.OnSenISABase,  C.OnSensorName);
    else         MyLaneIO.IOBitOff(C.OnSenRing,  C.OnSenIP,  C.OnSenPort,  C.OnSenBit,  C.OnSenISABase,  C.OnSensorName);
    if (offMade) MyLaneIO.IOBitOn (C.OffSenRing, C.OffSenIP, C.OffSenPort, C.OffSenBit, C.OffSenISABase, C.OffSensorName);
    else         MyLaneIO.IOBitOff(C.OffSenRing, C.OffSenIP, C.OffSenPort, C.OffSenBit, C.OffSenISABase, C.OffSensorName);
}

// ---------------------------------------------------------------------------
//  Registration.  `ip` must be unique per pair within one test binary.
// ---------------------------------------------------------------------------
inline void WireLifter(int iUp, int iMid, int ip)
{
    WireOne(iUp, ip, 0);
    WireOne(iMid, ip, 1);
    PlantPair p; p.iA = iUp; p.iB = iMid; p.law = LAW_LIFTER;
    Pairs().push_back(p);
}

//  iCylinderName / iCylinderNameMid are golden's PARAMETER 2 and PARAMETER 3 --
//  pass them in the same order the SM under test passes them.
inline void WireAutoStack(int iCylinderName, int iCylinderNameMid, int ip)
{
    WireOne(iCylinderName,    ip, 0);
    WireOne(iCylinderNameMid, ip, 1);
    PlantPair p; p.iA = iCylinderName; p.iB = iCylinderNameMid; p.law = LAW_AUTO;
    Pairs().push_back(p);
}

inline void WireAutoStackART(int iCylinderName, int iCylinderNameMid, int ip)
{
    WireOne(iCylinderName,    ip, 0);
    WireOne(iCylinderNameMid, ip, 1);
    PlantPair p; p.iA = iCylinderName; p.iB = iCylinderNameMid; p.law = LAW_AUTO_ART;
    Pairs().push_back(p);
}

// ---------------------------------------------------------------------------
//  Unwire / Forget -- THE TEARDOWN.
//
//  AI(W906-W7-L1-W3fixB) 20260802: Forget() used to be `{ Pairs().clear(); }` and
//  was described at its call sites as un-registering a pair "so later sub-tests see
//  the default (unwired) machine again".  IT DID NOT DO THAT.  WireOne writes
//  Enable / OnSenEnable / OffSenEnable straight onto the global Cylinder[], and
//  there was no inverse anywhere in this header -- clearing the registration list
//  only stopped Tick() from MOVING the pair, leaving it sensor-enabled with its Sim
//  IO bits frozen at the last state the previous walk drove them to.  Sub-test
//  ORDER therefore became load-bearing without anything pinning it.  (Nothing was
//  hollowed out by that: the ART walk in test_w7_l1_auto.cpp [6] happens to end in
//  exactly the state the disabled-sensor substitution produces, so the later
//  sub-tests passed either way.  That is luck, not a property.)
//
//  Unwire restores the three enables to false -- the default-constructed
//  TMyCylinder state that makes OnStatus()/OffStatus() substitute rather than read
//  IO.  The IO addresses and alarm times are deliberately LEFT ALONE: they are
//  inert once Enable is false, and every WireOne call overwrites them anyway, so
//  clearing them would add motion without adding meaning.
// ---------------------------------------------------------------------------
inline void Unwire(int c)
{
    TMyCylinder &C = Cylinder[c];
    C.Enable       = false;
    C.OnSenEnable  = false;
    C.OffSenEnable = false;
}

inline void Forget()
{
    for (size_t k = 0; k < Pairs().size(); ++k)
    {
        Unwire(Pairs()[k].iA);
        Unwire(Pairs()[k].iB);
    }
    Pairs().clear();
}

// ---------------------------------------------------------------------------
//  WireAllLifters / WireAllAutoStacks -- the usual full-machine fixture.
//  IP slots 11..13 are the three tray-group lifters, 21..26 the six Auto stacks.
// ---------------------------------------------------------------------------
inline void WireAllLifters()
{
    WireLifter(C_Load_Up,  C_Load_Middle,  11);
    WireLifter(C_Empty_Up, C_Empty_Middle, 12);
    WireLifter(C_Color_Up, C_Color_Middle, 13);
}

inline void WireAllAutoStacks()
{
    for (int i = 0; i < MAX_AUTO_TRAY; ++i)
        WireAutoStack(C_Auto_Up[i], C_Auto_Selector[i], 21 + i);
}

// ---------------------------------------------------------------------------
//  ResetCursors -- golden main.cpp:9159-9160 calls these two at FormShow; a
//  fixture that runs several SM walks in one binary needs the same reset so a
//  previous walk's parked cursor does not leak into the next one.
// ---------------------------------------------------------------------------
inline void ResetCursors()
{
    initLifterTask();
    initAutoTask();
}

// ---------------------------------------------------------------------------
//  Tick -- call once at the top of every SM tick, BEFORE the SM call.
// ---------------------------------------------------------------------------
inline void Tick()
{
    // (a) virtual time for the lifter / Auto motion+alarm timers.  NOT
    //     CylinderAlarmTime: that is the case-201 10-second watchdog and
    //     expiring it would manufacture an alarm the machine never sees.
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)               LifterTime[i][j].SetMSAndOn(0);
        for (int j = 0; j < MAX_UNLOAD_TRAY; ++j) AutoTime[i][j].SetMSAndOn(0);
    }

    // (b) move the metal.
    for (size_t k = 0; k < Pairs().size(); ++k)
    {
        const PlantPair &p = Pairs()[k];
        const bool oA = Cylinder[p.iA].bCylinderOn;
        const bool oB = Cylinder[p.iB].bCylinderOn;

        if (p.law == LAW_LIFTER)
        {
            const bool top    =  oA &&  oB;
            const bool middle =  oA && !oB;
            const bool bottom = !oA;
            SetSensors(p.iA, top,    bottom);
            SetSensors(p.iB, middle, bottom);
        }
        else if (p.law == LAW_AUTO)
        {
            SetSensors(p.iA, (oA && !oB), !oA);
            SetSensors(p.iB, oB,          !oB);
        }
        else    // LAW_AUTO_ART
        {
            SetSensors(p.iA, (oA && oB),  !oA);
            SetSensors(p.iB, (!oB && oA), (!oB && !oA));
        }
    }
}

}   // namespace w3

#endif // W3_CYLINDER_PLANT_H
