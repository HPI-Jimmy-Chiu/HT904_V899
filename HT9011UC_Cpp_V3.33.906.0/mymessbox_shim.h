//---------------------------------------------------------------------------
//  mymessbox_shim.h -- TMyMessageBoxShim, extracted verbatim from
//  acatchtray_shims.h (which now includes this file back, so every existing
//  consumer keeps the single class definition -- no ODR split).
//
//  AI(W906-FW-G24) 20260824: WHY THIS FILE EXISTS -- SECSGEM/uHGemHT9045.cpp
//  needs MyMessageBox for gates G24/G26/G45, but it cannot include the full
//  acatchtray_shims.h: that header's TColor consts collide with
//  uHGemEquipment.h's (clYellow redefinition) and its NewRecordProcess
//  declaration re-states default arguments cMyDB.h already gave -- both hard
//  errors in one TU (probe-verified 20260824, not assumed).  Narrowing the
//  seam to just this class sidesteps both conflicts without touching either
//  header's existing surface.  Golden source: mymessbox.h (TMyMessageBox*);
//  offline the dialog is never visible, so Close() is a no-op.
//  Real bodies + instantiation: acatchtray_shims.cpp:95-98 (unchanged).
//---------------------------------------------------------------------------
#ifndef mymessbox_shimH
#define mymessbox_shimH

class TMyMessageBoxShim
{
public:
    bool Visible;       // golden -- dialog visible?  (offline false)
    bool fShow;         // golden -- show flag        (offline false)
    void Close();       // golden -- offline no-op
    TMyMessageBoxShim();
};
extern TMyMessageBoxShim *MyMessageBox;         // golden mymessbox.h

#endif
