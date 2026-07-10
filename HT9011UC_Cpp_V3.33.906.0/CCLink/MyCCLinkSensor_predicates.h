// =============================================================================
//  CCLink/MyCCLinkSensor_predicates.h  --  2 pure shuttle-sensor-type predicates
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/CCLink/MyCCLinkSensor.cpp:2584-2598
//  Translation wave: W5 (CCLink driver pair -- tiny cross-front predicate pair)
//  Translator: AI(W5-CCLink-Translate) 20260710
//
//  IMPORTANT: this is NOT the full MyCCLinkSensor.cpp translation. That file is
//  the golden TfCCLink VCL FORM (~2950 lines, incl. a ~672-line live-refresh
//  loop) and remains DEFERRED to W7-UI in its entirety. This file extracts
//  ONLY the 2 pure free-function predicates confirmed (by recon) to have zero
//  VCL/widget coupling -- both are single-expression reads of the global
//  SHUTTLE_SENSOR_TYPE against MachineType.h enum constants, with no side
//  effects. Named distinctly (MyCCLinkSensor_predicates, not
//  MyCCLinkSensor) so nobody later mistakes this for the deferred full file.
//
//  Both predicates are already used by other still-golden (deferred) call
//  sites across the codebase (e.g. TfCCLink's own ctor: golden
//  MyCCLinkSensor.cpp:47 `if(MachineTypeChoice==Type_HT9045 && UseSensorMod3())`)
//  -- translating them now unblocks any sibling unit that needs them without
//  waiting on the full VCL form.
// =============================================================================
#ifndef MyCCLinkSensor_predicatesH
#define MyCCLinkSensor_predicatesH

// golden MyCCLinkSensor.cpp:2584 -- true when the shuttle sensor is configured
// for CanBus OR EtherCAT (any of the 4 non-CC-Link variants).
bool UseCanBusOrEtherCAT();

// golden MyCCLinkSensor.cpp:2592 -- true when the shuttle sensor is configured
// for one of the "Mod3" (12-amp, HT9046AH-style) variants: CCLink3 / CanBus3 /
// EtherCAT3.
bool UseSensorMod3();

#endif // MyCCLinkSensor_predicatesH
