//---------------------------------------------------------------------------
//  cSiteUseManager.cpp  --  HP anti-fragmentation site-use manager (OFFLINE shim)
//
//  Translation wave: W6.2c batch-3 (Integrate).  Translator: AI(W6.2c-INARM-batch3)
//  Golden source: cSiteUseManager.cpp (AI(ht9045-v899) 20260405/20260407).
//
//  Offline-safe bodies (see cSiteUseManager.h banner).  The real golden class runs
//  a compact-search HP placement planner over live HP geometry/logging; offline we
//  default to "compact search OFF" so the normal SearchPlateToPlace path is taken,
//  every nozzle is treated as able to reach an HP cell, and the shadow-log /
//  validate hooks are no-ops.  The single SiteUseMgr instance lives here.
//---------------------------------------------------------------------------
#include "cSiteUseManager.h"

//AI(W6.2c-INARM-batch3) 20260626: single offline instance (golden defines one
// global SiteUseMgr).  Consumed by ainarm9045_1x4_4.cpp.
cSiteUseManager SiteUseMgr;

// Compact search disabled offline -> the normal SearchPlateToPlace branch runs.
bool cSiteUseManager::UseCompactSearch()          { return false; }
// If ever invoked, report the compact placement "succeeded" (no fragmentation
// concern offline) -- faithful to the golden true-on-placed-ok contract.
bool cSiteUseManager::CompactSearchPlateToPlace() { return true; }
// Offline: every nozzle is assumed reachable so the variant does NOT skip to
// Shuttle on a "no reachable HP cell" guard.
bool cSiteUseManager::CanAnyNozzleReachHP()       { return true; }
// Offline log hooks -- no side effect.
void cSiteUseManager::ShadowLogBeforeSearch(int /*iPlate*/) {}
void cSiteUseManager::ValidateSearchResult(int /*iIdx*/)    {}
