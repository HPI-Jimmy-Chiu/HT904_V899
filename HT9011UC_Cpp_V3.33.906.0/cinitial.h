// =============================================================================
//  cinitial.h  --  interface header for cinitial.cpp
//
//  Golden ref: HT9011UC_Code_V3.33.906.0_20260618/cinitial.h (declares the
//  full ~15,242-line cinitial.cpp surface).  THIS HEADER currently declares
//  ONLY the functions landed by GA-2-C1 (golden cinitial.cpp:77-3060):
//
//      GetSHCHKPos, InitialSuckerName, InitSucker, InitialSwitchName,
//      InitialSwitch, InitialSensorName, InitialSensor, InitialSafeDoor,
//      InitialHeaterDoor
//
//  (InitialSwitch() is also forward-declared in myswitch.h -- a pre-existing
//  stub for exactly this definition; repeating an identical declaration here
//  is legal C++ and keeps this header a complete, self-contained interface
//  for cinitial.cpp.)
//
//  GA-2-C2 .. GA-2-C5 (future waves) will append the remaining declarations
//  as they land the rest of cinitial.cpp (InitCylinder, InitialMotorParameter,
//  SetMotorSpeed family, ReadTechData/SetWorkParameter/ChangeRunParameter,
//  InitialMachine, Save/LoadMachineRecord, SetTechDataToProd*, ChangeSite,
//  shuttle-thread parameter setup, etc. -- see golden cinitial.h for the full
//  target list). Each future wave should extend this header in the same
//  ascending, banner-documented style cinitial.cpp itself uses.
//
//  Translator: AI(W906-GA2-C1) 20260804
//  Encoding: UTF-8, bare LF, trailing newline.
// =============================================================================
#ifndef cinitialH
#define cinitialH

//------------------------------------------------------------------------------
//  GA-2-C1  (golden cinitial.cpp:77-3060)
//------------------------------------------------------------------------------
int  GetSHCHKPos(int iSite, int iCenterBase);   // golden cinitial.h (JerryYang 20160728)
void InitialSuckerName();
void InitSucker();
void InitialSwitchName();
void InitialSwitch();
void InitialSensorName();
void InitialSensor();
void InitialSafeDoor();
void InitialHeaterDoor();

//------------------------------------------------------------------------------
//  GA-2-IMP  (golden cinitial.cpp:3392-4101)
//  Declared here because golden cinitial.h:13 declares exactly this.
//  NOTE: InitialMotorName() (golden cinitial.cpp:3109) is deliberately NOT
//  declared -- golden declares it in no header either; its only caller is
//  InitialMotorParameter, same TU, definition-first.
//------------------------------------------------------------------------------
void InitialMotorParameter();                   // golden cinitial.h:13

#endif
