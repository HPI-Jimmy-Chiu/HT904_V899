// ===========================================================================
//  Public/HTKeyProShim.h
//  Portable replacement for the single KeyPro DLL entry actually used by the
//  HT9011UC V906 source.  Additive shim; mirrors the BCB6 Public/ path.
//
//  Translator: AI(W?) 20260626
//
//  PROVENANCE (verified against golden ref
//  D:\HT9045\HT9011UC_Code_V3.33.906.0_20260618):
//    * Original declaration: Public/HTKeyPro.h:27
//        extern "C" __declspec(dllimport)
//            int __stdcall KEYPRO_GET_LEVEL( unsigned int iIndex );
//    * Included only at HS_Function.cpp:37 (#include "HTKeyPro.h").
//    * SOLE call site: HS_Function.cpp:4933, inside TFormHS::CheckKeyPro():
//          iLevel = KEYPRO_GET_LEVEL(3);
//          if (iLevel != 1) { ShowMyMessage("...please insert KeyPro..."); ... }
//      i.e. level == 1 means "valid dongle"; argument 3 is the SECS/GEM
//      feature index.  Guarded by CosFunction.bSECSGEM_UseKeyPro &&
//      IniConfig.bEnable_SECS_GEM.
//
//  WHY A SHIM (not the original header):
//    * HTKeyPro.h also declares std::string-returning exports
//      (KEYPRO_GET_ERROR_CODE / GET_DATE_INFO / GET_ID / WRITE_ID_INOF).
//      Those are NEVER called in 906 (grep clean) and would drag in the
//      Borland std::string ABI, which MinGW cannot match -- so they are
//      deliberately NOT ported here.
//    * KeyProDLL.lib is a 32-bit OMF Borland import lib, unusable by MinGW;
//      we therefore do NOT static-link it.  Instead we resolve KEYPRO_GET_LEVEL
//      at run time via LoadLibrary/GetProcAddress (see HTKeyProShim.cpp).
//
//  W7 NOTE: the CheckKeyPro call site (a VCL TFormHS method) is translated in a
//  LATER batch.  When it lands, the call changes:
//        KEYPRO_GET_LEVEL(3)        ->  KeyPro_GetLevel(3)
//        #include "HTKeyPro.h"      ->  #include "HTKeyProShim.h"
//
//  This is a plain C/C++ header: no VCL dependency, no <string> in the API.
// ===========================================================================
#ifndef HTKEYPROSHIM_H
#define HTKEYPROSHIM_H

// ---------------------------------------------------------------------------
//  KeyPro_GetLevel
//  Faithful portable stand-in for KEYPRO_GET_LEVEL(unsigned int iIndex).
//
//  iIndex : feature number (the live call passes 3 = SECS/GEM feature index).
//  return : the feature's licensed level.  As in the original DLL, a return
//           value of 1 means "valid dongle / feature enabled".
//
//  Resolution order (HTKeyProShim.cpp):
//    1. LoadLibraryA("KeyProDLL.dll")
//    2. GetProcAddress(h, "KEYPRO_GET_LEVEL")  (Borland exports it undecorated)
//       then fall back to "_KEYPRO_GET_LEVEL@4" (MSVC __stdcall decoration).
//    3. Call through a typed __stdcall function pointer.
//  The handle and resolved pointer are cached (resolved once).
//
//  OFFLINE / STUB behavior: if the DLL is absent or the proc cannot be
//  resolved (no dongle hardware), KeyPro_GetLevel returns 1 (== valid dongle)
//  so CheckKeyPro passes and SECS/GEM stays enabled in the offline port.
//
//  Compile-time override: define KEYPRO_STUB to force the offline stub path
//  unconditionally (never touches the DLL); useful for hardware-free builds.
// ---------------------------------------------------------------------------
int KeyPro_GetLevel(unsigned int iIndex);

#endif // HTKEYPROSHIM_H
