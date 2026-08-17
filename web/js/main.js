/* ==========================================================================
   main.js -- boot.
   Assemble the shell, register every [data-tag] node, then hand the page
   over to a transport. Panels never talk to the transport directly.
   ========================================================================== */

import { h } from "./ui/dom.js";
import { register, applyState, onCommand, boundTags } from "./ui/bind.js";
import { rail, topbar } from "./panels/chrome.js";
import { leftColumn } from "./panels/left.js";
import { rightColumn } from "./panels/right.js";
import { statusRail } from "./panels/statusrail.js";
import { createTransport } from "./transport/index.js";
import { TAGMAP, coverage } from "./model/tagmap.js";

/* The size the layout is authored at; see the note on .app in base.css. */
const DESIGN_W = 1456;
const DESIGN_H = 818;
const MAX_SCALE = 2;

function applyViewportScale() {
  const fit = Math.min(window.innerWidth / DESIGN_W, window.innerHeight / DESIGN_H);
  const scale = Math.min(MAX_SCALE, Math.max(1, fit));
  document.documentElement.style.setProperty("--ui-scale", scale.toFixed(4));
}

function buildShell() {
  return h("div.app", null,
    rail({ active: "main" }),
    h("main.content", null,
      leftColumn(topbar()),
      rightColumn(),
    ),
    statusRail(),
  );
}

/**
 * Tabs and radio groups are pure presentation until the backend defines what
 * they switch between, so they are handled here rather than round-tripped.
 * Everything else goes to the transport.
 */
function handleLocalUi({ cmd, source }) {
  if (cmd && cmd.startsWith("ui.tab.")) {
    const bar = source.parentElement;
    for (const t of bar.querySelectorAll('[role="tab"]')) {
      t.setAttribute("aria-selected", t === source ? "true" : "false");
    }
    return true;
  }
  if (cmd && cmd.startsWith("ui.radio.")) {
    const grid = source.closest(".radios");
    const name = source.getAttribute("name");
    if (grid) {
      for (const lbl of grid.querySelectorAll(".radio")) {
        const input = lbl.querySelector("input");
        if (input && input.name === name) lbl.classList.toggle("radio--on", input.checked);
      }
    }
    return true;
  }
  return false;
}

function boot() {
  applyViewportScale();
  window.addEventListener("resize", applyViewportScale);

  const shell = buildShell();
  document.body.appendChild(shell);

  const count = register(shell);
  const cov = coverage();
  console.info(
    `[ht9045] ${count} bound nodes, ${boundTags().length} distinct tags; ` +
    `tagmap ${cov.resolved}/${cov.total} resolved to a real VCL control`,
  );

  const transport = createTransport();

  onCommand(payload => {
    if (handleLocalUi(payload)) return;
    transport.send(payload);
  });

  transport.connect(
    patch => applyState(patch),
    status => {
      document.documentElement.setAttribute("data-conn", status.state);
      if (status.state !== "online" && transport.name === "ws") {
        console.warn("[ht9045] link", status.state, status.error || "");
      }
    },
  );

  // Handy in the browser console while developing:
  //   __ht9045.applyState({"machine.state":"RUN"})
  window.__ht9045 = { applyState, boundTags, transport, TAGMAP, coverage };
}

if (document.readyState === "loading") {
  document.addEventListener("DOMContentLoaded", boot);
} else {
  boot();
}
