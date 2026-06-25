// =============================================================================
//  FormsFacade.h  --  non-VCL stand-in for the VCL form pointers the SMs deref
//
//  Translation wave: W6.0 (decoupling scaffold for the W6.1 Empty-tray canary)
//  Translator: AI(W6.0-SCAFFOLD) 20260626
//
//  W6 DECOUPLING STRATEGY (form-pointer cut)
//  -----------------------------------------
//  The BCB6 state machines reach UI/automation state through global VCL TForm
//  pointers (fMain / fAGV / ...).  Those forms cannot be pulled into the
//  portable build (they derive from VCL TForm).  Instead we expose ONLY the
//  members a given sub-wave's SM actually dereferences, on a non-VCL facade,
//  and grow it per sub-wave.
//
//  W6.1 SURFACE = fAGV ONLY.
//    The Empty-tray canary (asendic_Empty.cpp) has 0 fMain-> derefs (verified)
//    and exactly ONE satellite deref: fAGV->IsATK_AMR() at golden
//    asendic_Empty.cpp:827 (DoAutoEmpty case 70, the DUMMY non-AMR branch).
//    The golden TfAGV is a VCL TForm (Automation/AGV.h:15
//    `class TfAGV : public TForm`, fAGV ptr at :210) -> cannot be pulled in.
//
//  Do NOT add TfMain / main.h members here -- the canary needs none.  Keep this
//  facade strictly minimal; later sub-waves extend it as their SMs require.
// =============================================================================
#ifndef FormsFacadeH
#define FormsFacadeH

// ---------------------------------------------------------------------------
//  TfAGV -- non-VCL stub.  Mirrors ONLY the one method the canary calls.
//  Golden: bool TfAGV::IsATK_AMR();  (Automation/AGV.h:205)
//  In the offline sim there is no ATK AMR present, so IsATK_AMR() returns false
//  (the DUMMY non-AMR path -- faithful to a handler with no AMR attached).
// ---------------------------------------------------------------------------
class TfAGV
{
public:
    bool IsATK_AMR();
};

extern TfAGV *fAGV;     // golden: extern PACKAGE TfAGV *fAGV; (AGV.h:210)

#endif // FormsFacadeH
