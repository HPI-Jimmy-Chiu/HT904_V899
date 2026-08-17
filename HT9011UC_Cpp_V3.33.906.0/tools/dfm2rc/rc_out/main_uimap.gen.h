// ===========================================================================
//  main_uimap.gen.h
//  DO NOT EDIT -- GENERATED FILE (GA-4 uimap join table: dfm_path <->
//  numeric dialog/control ids, plus the caption/tab/root-client data the
//  layout table truthfully lacks). Hand edits will be overwritten.
//  Regenerate: python tools/dfm2rc/emit_uimap.py main
//  Generator:   emit_uimap.py GA-4 v1.0
//  Sources:     rc_out/main.rcmeta.json + ir_out/main.dfm.ir.json
//  Census:      dialogs=91 controls=758 tabhosts=5
//
//  AI(W906-GA4) 20260804: generated join table for main -- see
//  docs/DESIGN_GA4_UI_ENGINES.md D-GA4-1 and ui/layout/UiFormMap.h for the
//  schema + per-field contracts (including why synthesized rows carry
//  invented '#'-suffixed source_paths). "UiFormMap.h" is a bare include:
//  ui/layout/ is on the include path (GA-4 build contract), unlike the
//  layout emitter's schema header which lives inside layout_out/ itself.
// ===========================================================================
#ifndef HT9045_GEN_UIMAP_MAIN_H
#define HT9045_GEN_UIMAP_MAIN_H

#include "UiFormMap.h"

namespace ht9045_uimap {

extern const UiMapDialog  kmainUiMapDialogs[];
extern const UiMapControl kmainUiMapControls[];
extern const UiMapTabHost kmainUiMapTabHosts[];
extern const UiMapForm    kmainUiMap;

} // namespace ht9045_uimap

#endif // HT9045_GEN_UIMAP_MAIN_H
