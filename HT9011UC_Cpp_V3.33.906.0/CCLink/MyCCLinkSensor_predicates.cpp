// =============================================================================
//  CCLink/MyCCLinkSensor_predicates.cpp  --  2 pure shuttle-sensor-type predicates
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/CCLink/MyCCLinkSensor.cpp:2584-2598
//  Translation wave: W5 (CCLink driver pair -- tiny cross-front predicate pair)
//  Translator: AI(W5-CCLink-Translate) 20260710
//
//  See MyCCLinkSensor_predicates.h for scope notes (this is NOT the full
//  MyCCLinkSensor.cpp -- that VCL form remains deferred to W7-UI).
//
//  Only dependency: cmydef.h (extern int SHUTTLE_SENSOR_TYPE) + MachineType.h
//  (eSensorCanBus/eSensorCanBus3/eSensorEtherCAT/eSensorEtherCAT3/eSensorCCLink3
//  enum constants), both already translated (W0-TAIL, part of ht9045_globals).
//  Zero VCL/form coupling.
// =============================================================================
#include "MyCCLinkSensor_predicates.h"
#include "cmydef.h"       // SHUTTLE_SENSOR_TYPE
#include "MachineType.h"  // eSensorCanBus / eSensorCanBus3 / eSensorEtherCAT /
                          // eSensorEtherCAT3 / eSensorCCLink3

//---------------------------------------------------------------------------
bool UseCanBusOrEtherCAT()
{
    if(SHUTTLE_SENSOR_TYPE==eSensorCanBus || SHUTTLE_SENSOR_TYPE==eSensorCanBus3 || SHUTTLE_SENSOR_TYPE==eSensorEtherCAT || SHUTTLE_SENSOR_TYPE==eSensorEtherCAT3)
        return true;
    else
        return false;
}
//---------------------------------------------------------------------------
bool UseSensorMod3()
{
    if(SHUTTLE_SENSOR_TYPE==eSensorCCLink3 || SHUTTLE_SENSOR_TYPE==eSensorCanBus3 || SHUTTLE_SENSOR_TYPE==eSensorEtherCAT3)
        return true;
    else
        return false;
}
//---------------------------------------------------------------------------
