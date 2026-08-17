/* ==========================================================================
   transport/index.js -- picks a data source.
   ==========================================================================
   Default is the mock, so opening the page anywhere just works. Force the
   real one with a query string:

     index.html?src=ws                       -> ws://<same host>/ht9045
     index.html?src=ws&ws=ws://10.0.0.5:8080/ht9045

   Nothing above this file knows which one it got.
   ========================================================================== */

import { createMockTransport } from "./mock.js";
import { createWebSocketTransport } from "./ws.js";

export function createTransport() {
  const q = new URLSearchParams(location.search);
  const src = (q.get("src") || "mock").toLowerCase();

  if (src === "ws" || src === "live") {
    return createWebSocketTransport({ url: q.get("ws") || undefined });
  }
  return createMockTransport();
}
