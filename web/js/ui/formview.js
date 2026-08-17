/* ==========================================================================
   formview.js -- generic READ-ONLY renderer for fw-weblayout/1 documents
   (web/forms/<stem>.layout.json, emitted by tools/dfm2rc/emit_web.py from
   the .dfm IR).

   AI(W906-FW2) 20260817: FW campaign pilot. Design rules:

     * READ-ONLY. Events in the document are never wired -- that is the
       campaign's hard boundary (write path is a separate, safety-critical
       design round). Buttons render inert.
     * DEFENSIVE. An unknown widget class renders as a visible placeholder
       and is reported in the returned summary -- never silently dropped and
       never an exception. A layout document must not be able to blank the
       page.
     * FAITHFUL GEOMETRY. dfm Left/Top are PARENT-relative; the renderer
       nests elements by parent path and positions with absolute offsets, so
       the browser reproduces the VCL containment tree rather than a flat
       repaint of design-time screen coordinates.

   Usage:
     import { renderForm } from "./js/ui/formview.js";
     const summary = await renderForm(containerEl, "cTestCategory");
   ========================================================================== */

const VCL_COLORS = {
  clBtnFace: "#d4d0c8", clWindow: "#ffffff", clWindowText: "#000000",
  clBlack: "#000000", clWhite: "#ffffff", clRed: "#c94f3d",
  clYellow: "#e0c341", clLime: "#4caf50", clGreen: "#2e7d32",
  clBlue: "#3d6dc9", clSilver: "#c0c0c0", clGray: "#808080",
  clMaroon: "#800000", clNavy: "#000080", clInfoBk: "#fffbe6",
};

function vclColor(v, fallback) {
  if (typeof v === "number") {
    // BGR integer form (dfm stores TColor as $00BBGGRR)
    const b = (v >> 16) & 0xff, g = (v >> 8) & 0xff, r = v & 0xff;
    return `rgb(${r},${g},${b})`;
  }
  return VCL_COLORS[v] || fallback;
}

function el(tag, cls, parent) {
  const e = document.createElement(tag);
  if (cls) e.className = cls;
  if (parent) parent.appendChild(e);
  return e;
}

function place(e, w) {
  e.style.position = "absolute";
  e.style.left = (w.rect.x ?? 0) + "px";
  e.style.top = (w.rect.y ?? 0) + "px";
  if (w.rect.w != null) e.style.width = w.rect.w + "px";
  if (w.rect.h != null) e.style.height = w.rect.h + "px";
}

/* One renderer per VCL class family. Each returns the created element. */
const RENDERERS = {
  TLabel: (w, p) => { const e = el("span", "fv-label", p); e.textContent = w.props.Caption ?? ""; return e; },
  TStaticText: (w, p) => RENDERERS.TLabel(w, p),
  TPanel: (w, p) => { const e = el("div", "fv-panel", p); if (w.props.Caption) e.dataset.caption = w.props.Caption; return e; },
  TGroupBox: (w, p) => { const e = el("fieldset", "fv-group", p); el("legend", "", e).textContent = w.props.Caption ?? ""; return e; },
  TEdit: (w, p) => { const e = el("div", "fv-edit", p); e.textContent = w.props.Text ?? ""; return e; },
  TMaskEdit: (w, p) => RENDERERS.TEdit(w, p),
  TMemo: (w, p) => { const e = el("div", "fv-edit fv-memo", p); return e; },
  TButton: (w, p) => { const e = el("div", "fv-btn", p); e.textContent = w.props.Caption ?? ""; return e; },
  TBitBtn: (w, p) => RENDERERS.TButton(w, p),
  TSpeedButton: (w, p) => RENDERERS.TButton(w, p),
  TCheckBox: (w, p) => {
    const e = el("label", "fv-check", p);
    const dot = el("span", "fv-check__box" + (String(w.props.Checked) === "True" ? " is-on" : ""), e);
    dot.setAttribute("aria-hidden", "true");
    e.appendChild(document.createTextNode(w.props.Caption ?? ""));
    return e;
  },
  TRadioButton: (w, p) => RENDERERS.TCheckBox(w, p),
  TComboBox: (w, p) => {
    const e = el("div", "fv-combo", p);
    const items = w.props["Items.Strings"] || w.props.Items;
    e.textContent = w.props.Text ?? (Array.isArray(items) ? items[0] ?? "" : "");
    return e;
  },
  TStringGrid: (w, p) => {
    const e = el("div", "fv-grid", p);
    const cols = w.props.ColCount ?? 5, rows = w.props.RowCount ?? 5;
    const dcw = w.props.DefaultColWidth ?? 64, drh = w.props.DefaultRowHeight ?? 24;
    const cw = Array.isArray(w.props.ColWidths) ? w.props.ColWidths : [];
    const t = el("table", "fv-grid__t", e);
    for (let r = 0; r < rows; r++) {
      const tr = el("tr", "", t);
      for (let c = 0; c < cols; c++) {
        const td = el("td", "", tr);
        td.style.width = (cw[c] ?? dcw) + "px";
        td.style.height = drh + "px";
      }
    }
    return e;
  },
  TImage: (w, p) => el("div", "fv-image", p),
  TShape: (w, p) => {
    const e = el("div", "fv-shape", p);
    e.style.background = vclColor(w.props["Brush.Color"], "#888");
    return e;
  },
  TALed: (w, p) => el("div", "fv-led", p),
};

export async function renderForm(container, stem, base = "forms/") {
  const summary = { stem, widgets: 0, unknown: new Map(), errors: [] };
  let doc;
  try {
    const rsp = await fetch(base + stem + ".layout.json");
    if (!rsp.ok) throw new Error("HTTP " + rsp.status);
    doc = await rsp.json();
  } catch (err) {
    summary.errors.push(String(err));
    el("div", "fv-error", container).textContent =
      `formview: cannot load ${stem}.layout.json (${err})`;
    return summary;
  }

  container.textContent = "";
  const canvas = el("div", "fv-canvas", container);
  canvas.style.position = "relative";
  if (doc.canvas?.w) canvas.style.width = doc.canvas.w + "px";
  if (doc.canvas?.h) canvas.style.height = doc.canvas.h + "px";

  const byPath = new Map();
  for (const w of doc.widgets) {
    if (w.kind === "ROOT") {
      byPath.set(w.path, canvas);
      canvas.style.background = vclColor(w.props?.Color, "#d4d0c8");
      continue;
    }
    const parent = byPath.get(w.parent) ?? canvas;
    let e;
    try {
      const r = RENDERERS[w.cls];
      if (r) {
        e = r(w, parent);
      } else {
        e = el("div", "fv-unknown", parent);
        e.textContent = w.cls;
        summary.unknown.set(w.cls, (summary.unknown.get(w.cls) ?? 0) + 1);
      }
      place(e, w);
      e.dataset.dfm = w.path;               // future tag-binding join key
      if (w.props?.Visible === "False") e.style.visibility = "hidden";
      if (w.props?.["Font.Color"]) e.style.color = vclColor(w.props["Font.Color"], "");
      byPath.set(w.path, e);
      summary.widgets++;
    } catch (err) {
      // one bad widget must not take the page down
      summary.errors.push(w.path + ": " + String(err));
    }
  }
  return summary;
}
