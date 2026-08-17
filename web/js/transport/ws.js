/* ==========================================================================
   ws.js -- the real transport: a WebSocket to the HT9045 bridge.
   ==========================================================================
   NOT YET WIRED TO ANYTHING. No backend exists at the time of writing; this
   file fixes the wire contract so the C++ side has something concrete to
   implement, and so switching over is a one-line change in index.js.

   Wire contract (JSON text frames)
   --------------------------------
   server -> browser
     { "type": "snapshot", "data": { "<tag>": <value>, ... } }   full state, on connect
     { "type": "patch",    "data": { "<tag>": <value>, ... } }   deltas, as they happen
     { "type": "ack",      "id": <n>, "ok": true|false, "error": "..." }
     { "type": "alarm",    "code": "WAR0152", "text": "...", "at": "<iso8601>" }

   browser -> server
     { "type": "cmd", "id": <n>, "cmd": "<name>", "tag": "<tag>", "value": <any> }
     { "type": "ping", "id": <n> }

   Rules
     - `tag` strings are exactly the ones in js/model/tagmap.js.
     - The server is authoritative. The browser NEVER assumes a command
       succeeded; it waits for the resulting patch. That is what keeps two
       operators on two browsers from disagreeing about machine state.
     - Values are JSON primitives. null means "unknown / not installed".
   ========================================================================== */

const RECONNECT_MS = [500, 1000, 2000, 4000, 8000, 15000];

export function createWebSocketTransport({ url } = {}) {
  const endpoint = url || defaultEndpoint();
  let sock = null;
  let onPatch = () => {};
  let onStatus = () => {};
  let attempt = 0;
  let nextId = 1;
  let closedByUs = false;

  function defaultEndpoint() {
    const proto = location.protocol === "https:" ? "wss:" : "ws:";
    return `${proto}//${location.host}/ht9045`;
  }

  function open() {
    closedByUs = false;
    onStatus({ state: "connecting", endpoint });
    try {
      sock = new WebSocket(endpoint);
    } catch (err) {
      scheduleReconnect(err);
      return;
    }

    sock.addEventListener("open", () => {
      attempt = 0;
      onStatus({ state: "online", endpoint });
    });

    sock.addEventListener("message", ev => {
      let msg;
      try { msg = JSON.parse(ev.data); }
      catch { console.warn("[ws] non-JSON frame dropped"); return; }

      switch (msg.type) {
        case "snapshot":
        case "patch":
          if (msg.data && typeof msg.data === "object") onPatch(msg.data);
          break;
        case "ack":
          if (msg.ok === false) console.warn("[ws] command rejected:", msg);
          break;
        case "alarm":
          onStatus({ state: "alarm", alarm: msg });
          break;
        default:
          console.info("[ws] unhandled frame type:", msg.type);
      }
    });

    sock.addEventListener("close", () => {
      if (!closedByUs) scheduleReconnect(new Error("socket closed"));
    });

    sock.addEventListener("error", () => { /* close handler does the work */ });
  }

  function scheduleReconnect(err) {
    onStatus({ state: "offline", endpoint, error: String(err && err.message || err) });
    const delay = RECONNECT_MS[Math.min(attempt, RECONNECT_MS.length - 1)];
    attempt += 1;
    setTimeout(() => { if (!closedByUs) open(); }, delay);
  }

  return {
    name: "ws",
    endpoint,

    connect(handler, statusHandler) {
      onPatch = handler;
      if (statusHandler) onStatus = statusHandler;
      open();
      return Promise.resolve();
    },

    disconnect() {
      closedByUs = true;
      if (sock) sock.close();
      sock = null;
    },

    send({ cmd, tag, value }) {
      if (!sock || sock.readyState !== WebSocket.OPEN) {
        console.warn("[ws] dropped command, socket not open:", cmd);
        return;
      }
      sock.send(JSON.stringify({ type: "cmd", id: nextId++, cmd, tag, value }));
    },
  };
}
