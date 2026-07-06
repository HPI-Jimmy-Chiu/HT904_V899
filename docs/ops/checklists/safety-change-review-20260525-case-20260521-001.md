# Safety Change Review - CASE-20260521-001

| Item | Content |
|------|---------|
| Date | 2026-05-25 |
| Case | CASE-20260521-001 |
| Change | V899 First Tray Check On Unloader PASS Bin sync after workfile switch |
| Classification | Runtime gate / alarm enablement path. No motion, IO, cylinder, motor, or interlock order change. |

## Before Change

- [x] Classified as runtime gate / alarm enablement change.
- [x] Writable boundary checked: only V899 source folder was modified.
- [x] No system/config/CFG runtime files modified, so no config backup required.
- [x] Rollback path defined: restore the previous direct `cbFirstTrayCheckOnUnloader->Checked=true` assignment.
- [x] Impacted flow listed: `TfMain::cbSetupFileNameChange`, `TfLotInfo::SetFirstTrayCheckOnUnloader`, Auto tray P62 first-tray-check gate.

## During Change

- [x] Minimal change only: one call site in `main.cpp`.
- [x] No unrelated formatting or refactor.
- [x] Verification plan defined before build: //AI check, IDE errors, BCB6 build, field workfile-switch regression.
- [x] Assumption recorded: workfile switch should keep auto-enabling the main First Tray Check option, but must also sync Auto1/2/3 PASS positions.

## After Change

- [x] Daily Worklog updated: `docs/ops/daily/2026-05-25.md`.
- [x] ADR not required: no architecture or safety policy change.
- [x] Hook / Script / Policy / Instruction update not required.
- [x] No boundary drift: only V899 `main.cpp` plus documentation/case records changed.

## Reviewer Self-Check

| Level | Finding |
|-------|---------|
| Critical | None. No motion/IO/interlock order changed. |
| Major | Release package and customer validation are still pending, so case remains `in-progress`. |
| Minor | Field should confirm actual machine/version because intake screenshot title may differ from text. |

## Verification

- [x] VS Code file diagnostics: no errors in `main.cpp`.
- [x] `check_ai_annotation.py`: 447 valid annotations, 0 format errors.
- [x] BCB6 build: `bpr2mak HT9045.bpr` + `make -f HT9045.mak`, `LASTEXITCODE=0`.

## Conclusion

- [x] Can enter next verification step.
- [ ] Needs higher-level review.
- [ ] Must rollback first.

Field regression to run: set Auto1/Auto2 PASS Bin, enable P62 First Tray Check, switch workfile, confirm main checkbox and Auto1/Auto2 Position are all checked. If Auto3 is PASS Bin, it should sync too.
