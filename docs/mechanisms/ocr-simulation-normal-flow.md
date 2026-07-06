# OCR Simulation Normal Flow

Date: 2026-05-22
Target version: `HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422`

## Purpose

This note records the current OCR Simulation normal flow used from the OCR Form. It is intended as the baseline for motion-flow verification before adding or reviewing diagnostic logs.

## Entry Point

1. Open OCR Form.
2. The form dynamically creates the `OCR Simulation` tab.
3. Press `Run OCR Sim`.
4. The timer calls `DoOCRFlow1()` repeatedly until the OCR tray scan finishes, errors, or the user stops the simulation.

## Preconditions

1. Auto Run is stopped.
2. Home All is complete.
3. Tray OCR is enabled.
4. `INSTALL_OCR_YMot = eocrYMotInstal`.
5. Lot is not started.
6. Compare OCR Data is disabled.

## Normal Motion Flow

1. Backup current OCR and tray data.
2. Copy the full Tray Form area into `MMOCR` as simulation source data.
3. Call `InitOCRFlow(false)`.
4. Start the OCR simulation timer.
5. `DoOCRFlow1()` searches the next `HAS_IC` in `MMOCR`.
6. If InArm is not at XY wait position, move InArm XY to wait.
7. Check TrayArm X safe-to-move condition.
8. Calculate OCR target position:
   - TrayArm X = `Prod.iXTrayOCR + LoadForm->XStart + Col * Prod.LoadForm.iXPitch + OffsetX`
   - LoaderY = `Prod.iMLoaderYOCRPos + LoadForm->YStart + Row * Prod.LoadForm.iYPitch + OffsetY`
9. Move TrayArm X to OCR camera X position.
10. Move Loader MotorY to the target row OCR position.
11. Delete old OCR image.
12. Move OCR light cylinder if `OCRLightNoDown` is disabled.
13. Trigger OCR capture:
    - `SwOCRTigger.On()`
    - Wait 0.1 seconds
    - `SwOCRTigger.Off()`
14. Wait for OCR result:
    - Normal mode: Cognex result socket sets `asBarCode` and `bOCRReadOK`.
    - Bypass AOI mode: simulation fabricates a local `TxxAAxx` barcode after trigger off.
15. Parse result:
    - `T...`: pass, store barcode to `MMTrayY.Tray.cDeviceInf[col][row]`, mark `MMOCR` as `HAS_OCR_OK`.
    - `F...`: inspection error path.
    - `NoIC`: no-device path.
    - `?` or timeout: retry path.
16. Return to search the next `HAS_IC`.
17. After all positions finish, turn OCR light off.
18. Move TrayArm X to `Prod.iXTrayColor`.
19. Return OCR simulation result and stop/restore simulation state.

## Z Note

The `INSTALL_OCR_YMot = eocrYMotInstal` simulation path does not drive OCR Z by itself. Z should be placed manually in a safe position before running OCR Simulation. The simulation mainly exercises TrayArm X, LoaderY, OCR light cylinder, trigger output, and OCR result handling.

## Simulation Log Rule

The diagnostic log added for this verification is simulation-only. Each loop start overwrites the current loop log file, then appends only that loop's motion and OCR events. After a loop finishes, the file contains that loop only; the next loop starts by clearing it again.

Default runtime log file:

`D:\HT9045_Log\OCRSimulation\YYYY_MM_DD\CurrentLoop.txt`