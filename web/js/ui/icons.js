/* ==========================================================================
   icons.js -- inline SVG, no icon font and no network fetch.
   Stroke icons inherit currentColor so a single CSS colour drives them.
   ========================================================================== */

import { h } from "./dom.js";

function stroke(...paths) {
  return () => h("svg", {
    viewBox: "0 0 24 24", fill: "none", stroke: "currentColor",
    "stroke-width": "1.7", "stroke-linecap": "round", "stroke-linejoin": "round",
  }, ...paths.map(d => h("path", { d })));
}

export const icons = {
  home:   stroke("M3 10.5 12 3l9 7.5", "M5.5 9.5V20h13V9.5", "M9.5 20v-6h5v6"),
  chart:  stroke("M4 20V10", "M10 20V4", "M16 20v-7", "M22 20H2"),
  gauge:  stroke("M21 14a9 9 0 1 0-18 0", "M12 14l4.5-4"),
  calc:   stroke("M6 3h12v18H6z", "M9 7h6", "M9 12h.01", "M12 12h.01",
                 "M15 12h.01", "M9 16h.01", "M12 16h.01", "M15 16h.01"),
  oval:   stroke("M12 8c5 0 9 1.8 9 4s-4 4-9 4-9-1.8-9-4 4-4 9-4z"),
  axes:   stroke("M12 3v18", "M3 12h18", "M12 3 9.5 6", "M12 3l2.5 3",
                 "M3 12l3-2.5", "M3 12l3 2.5"),
  wrench: stroke("M15.5 3.5a5 5 0 0 0-6.3 6.3L3 16v5h5l6.2-6.2a5 5 0 0 0 6.3-6.3l-3 3-2.8-2.8z"),
  gear:   stroke("M12 15.5a3.5 3.5 0 1 0 0-7 3.5 3.5 0 0 0 0 7z",
                 "M19.4 15a1.6 1.6 0 0 0 .3 1.8l.1.1a2 2 0 1 1-2.8 2.8l-.1-.1a1.6 1.6 0 0 0-2.7 1.1V21a2 2 0 1 1-4 0v-.1A1.6 1.6 0 0 0 7.9 19.4l-.1.1a2 2 0 1 1-2.8-2.8l.1-.1A1.6 1.6 0 0 0 3 14.1H3a2 2 0 1 1 0-4h.1A1.6 1.6 0 0 0 4.6 7.9l-.1-.1a2 2 0 1 1 2.8-2.8l.1.1a1.6 1.6 0 0 0 1.8.3H9.4A1.6 1.6 0 0 0 10.5 3.9V3a2 2 0 1 1 4 0v.1a1.6 1.6 0 0 0 2.7 1.1l.1-.1a2 2 0 1 1 2.8 2.8l-.1.1a1.6 1.6 0 0 0-.3 1.8v.1a1.6 1.6 0 0 0 1.4 1h.1a2 2 0 1 1 0 4h-.1a1.6 1.6 0 0 0-1.4 1z"),
  camera: stroke("M4 8h3l1.5-2h7L17 8h3v11H4z", "M12 16.5a3.5 3.5 0 1 0 0-7 3.5 3.5 0 0 0 0 7z"),
  exit:   stroke("M15 4h4v16h-4", "M11 8l-4 4 4 4", "M7 12h9"),
  refresh: stroke("M20 12a8 8 0 1 1-2.6-5.9", "M20 4v4.5h-4.5"),
  chip:   stroke("M7 7h10v10H7z", "M9.5 3v2", "M14.5 3v2", "M9.5 19v2", "M14.5 19v2",
                 "M3 9.5h2", "M3 14.5h2", "M19 9.5h2", "M19 14.5h2"),
  cpu:    stroke("M6 6h12v12H6z", "M10 10h4v4h-4z",
                 "M9.5 3v3", "M14.5 3v3", "M9.5 18v3", "M14.5 18v3",
                 "M3 9.5h3", "M3 14.5h3", "M18 9.5h3", "M18 14.5h3"),
  tester: stroke("M9 4v5.5L5 17a2 2 0 0 0 1.8 3h10.4A2 2 0 0 0 19 17l-4-7.5V4",
                 "M8 4h8", "M7.5 14h9"),
  bulb:   stroke("M9 18h6", "M10 21h4",
                 "M12 3a6 6 0 0 0-3.5 10.9V16h7v-2.1A6 6 0 0 0 12 3z"),
  fan:    stroke("M12 12a3 3 0 1 0 0-6 3 3 0 0 0 0 6z",
                 "M12 12c0 3.5 2.5 6 6 6 0-3.5-2.5-6-6-6z",
                 "M12 12c-3 1.8-3.8 5.2-2 8 3-1.8 3.8-5.2 2-8z",
                 "M12 12C9.5 9.7 6 9.6 4 11.8c2.5 2.3 6 2.4 8 .2z"),
};

/** icon("home") -> <svg> node. Unknown names render nothing rather than throwing. */
export function icon(name) {
  const f = icons[name];
  return f ? f() : null;
}
