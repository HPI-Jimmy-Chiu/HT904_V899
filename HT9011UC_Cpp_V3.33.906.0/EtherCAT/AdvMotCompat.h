// =============================================================================
//  EtherCAT/AdvMotCompat.h -- the ONE sanctioned way to include AdvMotApi.h.
//
//  AI(W906-1203HAL-1) 20260820: new file. The vendor header's own guard
//  (EtherCAT/vendor/AdvMotApi.h:137-141) only defines its calling-convention
//  macro for MSVC:
//
//      #ifndef ADVCMNAPI
//      #if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
//      #define ADVCMNAPI __stdcall
//      #endif
//      #endif
//
//  On MinGW neither test holds, ADVCMNAPI stays undefined, and every one of
//  the header's 657 `U32 ADVCMNAPI Acm_*(...)` declarations becomes a syntax
//  error ("expected initializer before 'Acm_GetAvailableDevs'") the moment
//  HAVE_PCI1203=1 exposes them. MinGW supports __stdcall on 32-bit targets,
//  and the vendor guard honours a pre-definition -- so this shim supplies it
//  and then defers to the vendor header, which stays byte-untouched
//  (read-only per AGENTS.md).
//
//  Include THIS file instead of "AdvMotApi.h" everywhere in tree code. The
//  vendor header includes AdvMotDev.h/AdvMotDrv.h/AdvMotPropID.h/AdvMotErr.h
//  itself, so this is a drop-in replacement.
// =============================================================================
#ifndef ADVMOT_COMPAT_H
#define ADVMOT_COMPAT_H

#if defined(__GNUC__) && !defined(ADVCMNAPI)
#define ADVCMNAPI __stdcall
#endif

#include "AdvMotApi.h"

// AI(W906-1203HAL-1) 20260820: AdvMotDrv.h:855-858 #defines four GENERIC
// names as home-mode constants (AbsSwitch/LmtSwitch/RefPulse/Direct). `Direct`
// is a PROVEN poison: csystem.h:162 (ArmCanSuck4IC's parameter) and
// atester_shims.h:66 both use `Direct` as an identifier, so any TU that saw
// this vendor header first failed with "expected ',' or '...' before numeric
// constant". No tree code consumes any of the four as vendor constants
// (tree-wide grep 20260820: only this vendor header defines or uses them), so
// the proven one is #undef'd here; if translated code ever needs the vendor
// home-mode values, spell them literally (0/1/2/3) with a comment, or add a
// prefixed constant -- do NOT remove this #undef. The sibling three are left
// defined but are the same trap class if a future identifier collides.
#undef Direct

#endif  // ADVMOT_COMPAT_H
