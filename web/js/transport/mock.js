/* ==========================================================================
   mock.js -- an in-browser stand-in for the handler.
   ==========================================================================
   Purpose: let the whole UI be developed, reviewed and demoed with no
   backend at all. It publishes the same patch shape the real transport will
   ({tag: value}) and accepts the same commands, so swapping to ws.js changes
   nothing above this layer.

   It deliberately animates only what genuinely ticks on an idle machine --
   the clock, the power-saving countdown, the index time. It does NOT invent
   production data (counts, yields, lot ids); showing fabricated numbers on a
   handler screen is worse than showing none.
   ========================================================================== */

import { initialState } from "../model/state.js";

export function createMockTransport() {
  const state = initialState();
  let onPatch = () => {};
  let timer = null;

  // Seed the countdown from the value shown in the design (00:59:11).
  let powerSavingLeft = 59 * 60 + 11;

  function publish(patch) {
    Object.assign(state, patch);
    onPatch(patch);
  }

  function tick() {
    const now = new Date();
    const p = n => String(n).padStart(2, "0");
    const patch = {
      "clock.text":
        `${now.getFullYear()} / ${p(now.getMonth() + 1)} / ${p(now.getDate())}` +
        `   ${p(now.getHours())}:${p(now.getMinutes())}`,
    };

    if (powerSavingLeft > 0) {
      powerSavingLeft -= 1;
      const s = powerSavingLeft;
      patch["status.powerSaving"] =
        `Power Saving: ${p(Math.floor(s / 3600))}:${p(Math.floor(s / 60) % 60)}:${p(s % 60)}`;
    }

    publish(patch);
  }

  return {
    name: "mock",

    connect(handler) {
      onPatch = handler;
      onPatch({ ...state });          // initial full snapshot
      timer = setInterval(tick, 1000);
      return Promise.resolve();
    },

    disconnect() {
      if (timer) clearInterval(timer);
      timer = null;
    },

    /**
     * Commands are echoed optimistically so the UI is testable, but only for
     * things that are purely local UI state. Anything that would move the
     * machine is logged and NOT faked -- a mock must never make an operator
     * believe an axis moved.
     */
    send({ cmd, tag, value }) {
      switch (cmd) {
        case "temp.setSV":     publish({ "temp.sv": value }); break;
        case "temp.setSoak":   publish({ "temp.soak": value }); break;
        case "recipe.change":  publish({ "recipe.current": value }); break;
        case "user.change":    publish({ "user.level": value }); break;
        case "startmode.change": publish({ "startmode.value": value }); break;
        case "light.toggle":   publish({ "light.off": !state["light.off"] }); break;
        case "fan.toggle":     publish({ "fan.off": !state["fan.off"] }); break;
        default:
          console.info("[mock] command not simulated:", cmd, { tag, value });
      }
    },

    snapshot() { return { ...state }; },
  };
}
