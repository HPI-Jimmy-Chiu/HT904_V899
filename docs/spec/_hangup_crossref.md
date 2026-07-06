# Hangup ↔ SPEC ↔ ADR 交叉參照表

> 自動產生，請勿手改。重新執行 `scripts/link_hangup_to_spec.py` 更新。

## 摘要

- Hangup 資料夾數：3
- Hangup 筆記文件數（docs/hangup-notes/）：0
- 功能 SPEC 數：1
- ADR 數：3

## SPEC 引用清單

| Feature SPEC | 引用 Hangup | 引用 Case | 引用 ADR |
|--------------|-------------|-----------|----------|
| `F899-001` | — | — | — |

## ADR 引用清單

| ADR | 引用 Hangup | 引用 Case | 引用 SPEC |
|-----|-------------|-----------|-----------|
| `ADR-0001` | — | — | — |
| `ADR-0002` | 20260428 | CASE-20260428-001 | — |
| `ADR-0003` | — | — | — |

## Hangup 資料夾 SPEC/ADR 覆蓋率

| Hangup 資料夾 | 對應 SPEC | 對應 ADR | 狀態 |
|--------------|-----------|----------|------|
| `2026-04-29 09_47_06` | — | — | [警告] 無對應 SPEC/ADR |
| `2026-04-29 09_52_11` | — | — | [警告] 無對應 SPEC/ADR |
| `2026-04-29 10_16_26` | — | — | [警告] 無對應 SPEC/ADR |

## 建議

1. 標記 `[警告]` 的 hangup 資料夾應建立對應 SPEC（`docs/spec/features/`）或 ADR
2. 若 hangup 已結案但決策非本案 SPEC 範圍 → 建立 ADR
3. 若 hangup 為一次性偶發 → 在 SPEC 的「Known Issues」加一行即可