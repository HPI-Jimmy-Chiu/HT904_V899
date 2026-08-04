// =============================================================================
//  ui/resource.h -- Gate A skeleton resource IDs (W7-U0)
//  AI(W906-GateA-0) 20260804: NEW FILE. Only the disposable placeholder dialog
//  lives here. The real per-form IDs are generated (tools/dfm2rc/rc_out/
//  <Form>_ids.h) and must NEVER be merged into this file -- D11's two-namespace
//  join stays with the generator.
// =============================================================================
#ifndef GateA_resource_H
#define GateA_resource_H

#define IDD_GATEA_PLACEHOLDER   9045
#define IDC_GATEA_BANNER        9046

// rc.exe (the authoritative gate, W7 plan SS9-R2) does not know IDC_STATIC --
// that macro lives in afxres.h/winres.h, not windows.h. Define it here rather
// than pull afxres.h into the .rc so the file stays SDK-only.
#ifndef IDC_STATIC
#define IDC_STATIC              (-1)
#endif

#endif // GateA_resource_H
