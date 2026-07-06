# SPEC: V899 OCR Area Simulation

## Background

Tray Map laser simulation validates Auto Run behavior by running the real scan flow over a tray area. OCR simulation must provide the same kind of confidence: it must validate the actual Auto Run OCR path, not only send repeated Cognex trigger commands.

## Goal

- Use Tray Form data as the default simulation source.
- Run the real Cognex Tray OCR flow used by Auto Run.
- Validate OCR movement, light cylinder, trigger timing, barcode readback, retry, NoIC, and OCR pass/fail handling.
- Stop immediately on OCR error, timeout, or unexpected result.
- Restore production tray data after simulation so the test does not pollute Auto Run state.

## Non-Goals

- Do not edit INI or recipe files.
- Do not consume barcode comparison files during simulation.
- Do not allow the simulation to run while Auto Run is active.
- Do not replace the existing communication-only Cognex trigger test.

## Default Source

The simulation reads the tray size from the same tray data used by Auto Run:

- `MOT[MMTrayY].Tray.XItem`
- `MOT[MMTrayY].Tray.YItem`
- Tray Form pitch and start position are then consumed by the existing OCR flow through `DoOCRFlow1()`.

The simulation initializes `MOT[MMOCR]` as a full Tray Form area with `HAS_IC`, matching the existing Tray OCR Auto Run preparation path that forces OCR to scan the loader tray area.

## Runtime Flow

1. User opens OCR Cognex Setting page.
2. User presses `Run OCR Sim`.
3. Software checks:
   - `SystemStart == false`
   - OCR is installed and enabled
   - Tray OCR customer function is enabled
   - Compare OCR Data is disabled to avoid consuming customer barcode files
4. Software backs up:
   - `MOT[MMOCR].Tray.Data`
   - `MOT[MMTrayY].Tray.cDeviceInf`
   - OCR counters and temporary OCR flags
5. Software prepares the full Tray Form OCR area.
6. A timer repeatedly calls `DoOCRFlow1()`.
7. Progress is shown as `Running loop n/N`.
8. On completion, the next loop restarts from the same backed-up Tray Form state.
9. On stop, error, form close, or finish, backed-up production tray data is restored.

## Stop Conditions

- User presses `Stop OCR Sim`.
- `SystemStart` becomes true.
- `DoOCRFlow1()` returns a non-pass result.
- `bBarCodeError` becomes true after retry exhaustion.

## Acceptance Criteria

- Simulation runs only while Auto Run is idle.
- Simulation uses Tray Form full area by default.
- Simulation uses the real `DoOCRFlow1()` path.
- OCR errors stop the simulation instead of asking for production key-in flow.
- Existing tray data is restored after simulation.
- The existing manual Cognex trigger buttons keep their original behavior.