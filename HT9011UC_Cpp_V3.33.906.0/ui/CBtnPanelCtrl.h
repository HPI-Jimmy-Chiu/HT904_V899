// =============================================================================
//  ui/CBtnPanelCtrl.h -- W7-C5 / GA-4-C5
//  MFC shell for the button-panel family: window classes "HT9045.BtnPanel"
//  (56 .dfm instances) and "HT9045.BtnPanelLane" (701) = 757.
//
//  AI(W906-GateA-4-C5) 20260804: NEW FILE.
//
//  BASE CLASS = CStatic, per plan S7-6 (:544): "CBtnPanelCtrl should derive
//  from CStatic + SS_NOTIFY rather than CButton -- golden's base is TPanel,
//  no focus rect, no keyboard activation, and 714 of the 757 instances'
//  .dfm OnClick/OnMouseDown only ever use the click."
//
//  The CStatic choice is load-bearing as a NEGATIVE -- it is about what is
//  NOT inherited. A CButton base would bring BS_* semantics, a focus
//  rectangle, WS_TABSTOP participation, space/Enter keyboard activation and
//  default-button behaviour, none of which a VCL TPanel has; wiring 757
//  panels to CButton would silently add all of it. CStatic brings none.
//
//  SS_NOTIFY, however, is deliberately NOT applied, and that is a correction
//  to the letter (not the intent) of S7-6. Two independent reasons:
//   1. SS_NOTIFY is a style interpreted by the SYSTEM "Static" window
//      procedure. These controls are NOT of class "Static": D12 has the .rc
//      name them "HT9045.BtnPanel"/"HT9045.BtnPanelLane", whose registered
//      procedure is DefWindowProc (see RegisterCustomClasses.h). The bit
//      would simply never be read.
//   2. Even if the class were superclassed from "Static" so the bit did
//      work, it would produce a SECOND click notification (STN_CLICKED)
//      alongside the BN_CLICKED this shell already sends -- one golden
//      OnClick delivered twice.
//  Corroboration from the generator, which is the authority per the brief:
//  tools/dfm2rc/classmap.py's `_custom()` gives every custom class an EMPTY
//  style list, so no generated .rc asks for SS_NOTIFY on these controls.
//
//  STATE + FILL come from W7-C1/C2 untouched: vclcompat::BtnPanelCoreBase
//  (SetDown -> Color/FontColor/BevelInner/BevelOuter, butPa1.cpp:81-106) and
//  vclcompat::render::RenderBtnPanel (flat fill of core.Color).
//
//  THE SHELL ADDS EXACTLY TWO THINGS, AND ONLY BECAUSE W7-C2 HANDED THEM
//  OVER IN WRITING. BtnPanelRender.h:24-29 says: "This renderer paints ONLY
//  the fill ... The natural home for a faithful bevel is the eventual W7-C5
//  MFC shell (CBtnPanelCtrl : CStatic + SS_NOTIFY, plan S7-6), which can
//  call the real Win32 DrawEdge() against BevelInner/BevelOuter as thin
//  adapter code".
//   (a) BEVEL: BevelOuter/BevelInner -> DrawEdge(BDR_*OUTER|BDR_*INNER).
//       This is stock TPanel chrome, and it is observable golden state:
//       SetDown writes bvLowered/bvLowered when down and bvRaised/bvRaised
//       (or bvNone when Style==tsFlatButtons) when up, so with no bevel the
//       down-state of 757 buttons would be conveyed by colour alone.
//   (b) CAPTION: all 757 .dfm instances carry a non-empty Caption (measured
//       over tools/dfm2rc/ir_out/*.ir.json), and BtnPanelCoreBase::FontColor
//       exists solely as the stand-in for golden's Font->Color. If nobody
//       draws text, FontColor -- written by SetTrueFontColor/
//       SetFalseFontColor on every Down toggle -- is unobservable and 757
//       buttons are blank rectangles. Alignment is TPanel's default
//       (centred both ways): NO instance sets Alignment in its .dfm, so
//       DT_CENTER|DT_VCENTER|DT_SINGLELINE is the measured default, not a
//       guess.
// =============================================================================
#ifndef HT9045_UI_CBTNPANELCTRL_H
#define HT9045_UI_CBTNPANELCTRL_H

#include <afxwin.h>
#include "vclcompat/BtnPanelCore.h"

class CBtnPanelCtrl : public CStatic
{
public:
    CBtnPanelCtrl();

    // Point the shell at the caller-owned core (BtnPanelCore for
    // "HT9045.BtnPanel", BtnPanelLaneCore for "HT9045.BtnPanelLane" -- they
    // differ only in IO-address metadata, so one shell serves both).
    // NULL restores the internal default. The shell never takes ownership.
    void AttachCore(vclcompat::BtnPanelCoreBase* pCore);
    vclcompat::BtnPanelCoreBase&       Core()       { return *m_pCore; }
    const vclcompat::BtnPanelCoreBase& Core() const { return *m_pCore; }

    void SetDown(bool v);       // forwards to the core, then repaints
    void SyncFromCore();        // call after mutating Core() directly

protected:
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()

private:
    void DrawInto(HDC hdc);

    vclcompat::BtnPanelLaneCore  m_ownCore;   // the more capable of the two
    vclcompat::BtnPanelCoreBase* m_pCore;
    BOOL m_bClickPending;
};

#endif // HT9045_UI_CBTNPANELCTRL_H
