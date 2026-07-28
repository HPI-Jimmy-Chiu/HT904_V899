// ===========================================================================
//  vclcompat/render/BtnPanelRender.cpp
//  See BtnPanelRender.h for scope/provenance -- there is no golden Draw/
//  Paint function to cite line numbers against (butPa1.cpp/BtnPanelLane.cpp
//  have none, confirmed by grep); this is a flat fill of core.Color only.
//
//  AI(W906-W7-C2) 20260728: added this file's convention tag (LOW-3 audit
//  -- no functional change).
// ===========================================================================
#include "vclcompat/render/BtnPanelRender.h"

namespace vclcompat {
namespace render {

void RenderBtnPanel(HDC hdc, const RECT& rc, const BtnPanelCoreBase& core)
{
    COLORREF fill = ResolveColor(core.Color);
    HBRUSH   br   = ::CreateSolidBrush(fill);
    RECT     r    = rc;
    ::SetBkColor(hdc, fill);
    ::FillRect(hdc, &r, br);
    ::DeleteObject(br);
}

} // namespace render
} // namespace vclcompat
