/* ==========================================================================
   operator.js -- the write-path operator console (FW-FE1).
   ==========================================================================
   AI(W906-FW-FE1) 20260819: new file. The browser face of the write-path
   waves FW-W1..W5a: login (auth.login/logout), the single-operator control
   token (control.acquire/release), the first real command family
   (counter.clear), and display-only modal frames (ShowMyMessage pass-through).

   This is an OPERATOR CONSOLE, not a golden form render: index.html stays the
   faithful DFM shell, untouched. Same wire contract as js/transport/ws.js;
   this page keeps its own socket on purpose -- it is also the manual test rig
   for the single-operator rule, so it must be able to be the "second browser".

   Server truths this page reflects, never assumes:
     - every ack comes from the server (ok / error text is logged verbatim);
     - auth.level / control.owner arrive as tags via snapshot+patch;
     - a modal frame is display-only (golden ShowMyMessage returns void) --
       the OK button only hides the overlay locally, nothing flows back.
   ========================================================================== */

"use strict";

var RECONNECT_MS = [500, 1000, 2000, 4000, 8000, 15000];

var sock = null;
var attempt = 0;
var nextId = 1;
var pending = {};          // id -> label, so acks can be logged readably
var closedByUs = false;

function $(id) { return document.getElementById(id); }

function endpoint() {
  var proto = location.protocol === "https:" ? "wss:" : "ws:";
  return proto + "//" + location.host + "/ht9045";
}

function logLine(kind, text) {
  var el = document.createElement("div");
  el.className = "log-" + kind;
  var t = new Date().toTimeString().slice(0, 8);
  el.textContent = "[" + t + "] " + text;
  var log = $("log");
  log.appendChild(el);
  while (log.childNodes.length > 200) log.removeChild(log.firstChild);
  log.scrollTop = log.scrollHeight;
}

function setConn(state) {
  $("conn").textContent = state;
  $("conn").className = "conn conn-" + state;
}

function applyTags(data) {
  if (!data || typeof data !== "object") return;
  if ("auth.level" in data) {
    var lv = data["auth.level"];
    $("level").textContent = (lv === null || lv === undefined) ? "--" : String(lv);
  }
  if ("control.owner" in data) {
    var ow = data["control.owner"];
    $("owner").textContent = (ow === null || ow === undefined || ow === "") ? "(nobody)" : String(ow);
  }
}

function showModal(msg) {
  $("modal-title").textContent = String(msg.title || "Message");
  $("modal-text").textContent = String(msg.text || "");
  $("modal-at").textContent = String(msg.at || "");
  $("modal-ok").style.display = "";
  $("modal-answers").innerHTML = "";
  $("overlay").style.display = "flex";
}

/* AI(W906-FW-W5b) 20260819: answer-carrying query (golden ShowErrorMessage).
   The offered buttons are exactly the server-decoded K mask options; each
   sends modal.answer with the query's qid. No local dismiss: golden's dialog
   stays up until an offered answer is taken. */
function showQuery(msg) {
  $("modal-title").textContent = "Alarm " + String(msg.code || "");
  $("modal-text").textContent = "Operator decision required (kcode " + String(msg.kcode) + ")";
  $("modal-at").textContent = String(msg.at || "");
  $("modal-ok").style.display = "none";
  var row = $("modal-answers");
  row.innerHTML = "";
  (msg.options || []).forEach(function (opt) {
    var b = document.createElement("button");
    b.textContent = opt;
    b.addEventListener("click", function () {
      send("modal.answer", String(msg.qid), opt, "modal.answer[" + opt + "]");
      $("overlay").style.display = "none";
    });
    row.appendChild(b);
  });
  $("overlay").style.display = "flex";
}

function send(cmd, tag, value, label) {
  if (!sock || sock.readyState !== WebSocket.OPEN) {
    logLine("err", "not connected; dropped " + cmd);
    return;
  }
  var id = nextId++;
  pending[id] = label || cmd;
  var frame = { type: "cmd", id: id, cmd: cmd };
  if (tag !== null && tag !== undefined) frame.tag = tag;
  if (value !== null && value !== undefined) frame.value = value;
  sock.send(JSON.stringify(frame));
  logLine("out", "-> " + (label || cmd));
}

function onMessage(ev) {
  var msg;
  try { msg = JSON.parse(ev.data); }
  catch (e) { logLine("err", "non-JSON frame dropped"); return; }

  switch (msg.type) {
    case "snapshot":
    case "patch":
      applyTags(msg.data);
      break;
    case "ack": {
      var label = pending[msg.id] || ("cmd#" + msg.id);
      delete pending[msg.id];
      if (msg.ok === true) logLine("ok", "<- " + label + " ok");
      else logLine("err", "<- " + label + " REFUSED: " + String(msg.error || "(no reason)"));
      break;
    }
    case "modal":
      logLine("modal", "MODAL: " + String(msg.text || ""));
      showModal(msg);
      break;
    case "query":
      logLine("modal", "QUERY " + String(msg.code || "") + " qid=" + String(msg.qid) +
                       " options=" + JSON.stringify(msg.options || []));
      showQuery(msg);
      break;
    case "alarm":
      logLine("err", "ALARM " + String(msg.code || "") + ": " + String(msg.text || ""));
      break;
    default:
      logLine("info", "unhandled frame type: " + String(msg.type));
  }
}

function open() {
  closedByUs = false;
  setConn("connecting");
  try { sock = new WebSocket(endpoint()); }
  catch (e) { scheduleReconnect(); return; }

  sock.addEventListener("open", function () { attempt = 0; setConn("online"); });
  sock.addEventListener("message", onMessage);
  sock.addEventListener("close", function () {
    setConn("offline");
    if (!closedByUs) scheduleReconnect();
  });
  sock.addEventListener("error", function () { /* close handler reconnects */ });
}

function scheduleReconnect() {
  var delay = RECONNECT_MS[Math.min(attempt, RECONNECT_MS.length - 1)];
  attempt += 1;
  setTimeout(function () { if (!closedByUs) open(); }, delay);
}

/* The seven counter families of FW-W4 -- key strings must match the wb_serve
   dispatch table (tools/wb_serve.cpp kFamilies) exactly. */
var COUNTER_FAMILIES = [
  ["alarmData",      "Alarm Data"],
  ["testerCategory", "Tester Category"],
  ["loadingCount",   "Loading Count"],
  ["contactCurr",    "Contact Count (Current)"],
  ["contactHis",     "Contact Count (History)"],
  ["sortingCount",   "Sorting Count"],
  ["timeData",       "Time Data"],
];

function boot() {
  var grid = $("counters");
  COUNTER_FAMILIES.forEach(function (fam) {
    var b = document.createElement("button");
    b.textContent = fam[1];
    b.addEventListener("click", function () {
      send("counter.clear", fam[0], null, "counter.clear[" + fam[0] + "]");
    });
    grid.appendChild(b);
  });

  $("btn-login").addEventListener("click", function () {
    send("auth.login", $("user").value, $("password").value, "auth.login(" + $("user").value + ")");
  });
  $("btn-logout").addEventListener("click", function () { send("auth.logout"); });
  $("btn-acquire").addEventListener("click", function () { send("control.acquire"); });
  $("btn-release").addEventListener("click", function () { send("control.release"); });
  $("modal-ok").addEventListener("click", function () {
    $("overlay").style.display = "none";   // local dismiss only: golden's ShowMyMessage returns void
  });

  open();
}

if (document.readyState === "loading") {
  document.addEventListener("DOMContentLoaded", boot);
} else {
  boot();
}
