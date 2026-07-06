# -*- coding: utf-8 -*-
"""
sync_case_spec.py - HT9045 CASE / SPEC / ADR 雙向綁定與健康檢查工具

用途：
    把 Weekly_AI 的 weekly_data.json case 與 HT9045 的 docs/spec/, docs/adr/ 雙向綁定。

    雙向同步：
      1. 從 SPEC features/*.spec.md 抓「對應 CASE-ID」 -> 寫入 weekly_data.json items 的 spec_refs
      2. 從 ADR docs/adr/ADR-*.md 抓「關聯 CASE」 -> 寫入 weekly_data.json items 的 adr_refs
      3. 從 weekly_data.json items 的 notes (case=CASE-...) 反向找 SPEC -> 在 SPEC metadata 提示

    健康檢查：
      - status=done 但無 SPEC 的 case (應補 SPEC)
      - SPEC 引用了不存在的 CASE-ID (週報沒這筆)
      - 完整證據鏈報告

用法：
    python scripts/sync_case_spec.py                    # 報告模式 (read-only)
    python scripts/sync_case_spec.py --write            # 真的寫回 weekly_data.json
    python scripts/sync_case_spec.py --out docs/spec/_case_health.md
    python scripts/sync_case_spec.py --weekly D:/path/to/weekly_data.json

預設 weekly_data.json 路徑：D:/Work-jimmychiu/document/WeeklyReport/Weekly_AI/weekly_data.json
"""
from __future__ import annotations
import argparse
import json
import re
import sys
from pathlib import Path
from typing import Dict, List, Set, Tuple

WORKSPACE = Path(__file__).resolve().parent.parent
SPEC_DIR = WORKSPACE / "docs" / "spec"
ADR_DIR = WORKSPACE / "docs" / "adr"
DEFAULT_WEEKLY = Path(r"D:/Work-jimmychiu/document/WeeklyReport/Weekly_AI/weekly_data.json")

CASE_ID_RE = re.compile(r"CASE-(\d{8})-(\d{3})")
CASE_NOTE_RE = re.compile(r"case\s*=\s*(CASE-\d{8}-\d{3})", re.IGNORECASE)
FEATURE_ID_RE = re.compile(r"^F(\d{3,4})-(\d{3})")


def load_weekly(path: Path) -> dict:
    return json.loads(path.read_text(encoding="utf-8"))


def save_weekly(path: Path, data: dict) -> None:
    # 自動 backup
    bak = path.with_suffix(path.suffix + ".bak_sync_case_spec")
    if path.exists():
        bak.write_bytes(path.read_bytes())
    path.write_text(
        json.dumps(data, ensure_ascii=False, indent=2),
        encoding="utf-8",
    )


def extract_case_from_item(item: dict) -> str | None:
    """從 weekly item 的 notes 抓 case_id。"""
    notes = item.get("notes", "") or ""
    m = CASE_NOTE_RE.search(notes)
    if m:
        return m.group(1)
    # 也支援頂層 case_id 欄位
    if item.get("case_id"):
        return item["case_id"]
    return None


def scan_specs() -> Dict[str, dict]:
    """掃 docs/spec/features/ 抓 {feature_id: {path, case_ids, title, ...}}."""
    result: Dict[str, dict] = {}
    feat_dir = SPEC_DIR / "features"
    if not feat_dir.is_dir():
        return result
    for sp in feat_dir.glob("*.spec.md"):
        m = FEATURE_ID_RE.match(sp.stem)
        if not m:
            continue
        fid = f"F{m.group(1)}-{m.group(2)}"
        text = sp.read_text(encoding="utf-8", errors="replace")
        case_ids = set(f"CASE-{a}-{b}" for a, b in CASE_ID_RE.findall(text))
        # 抓標題
        title_match = re.search(r"\|\s*標題\s*\|\s*([^|\n]+?)\s*\|", text)
        title = title_match.group(1).strip() if title_match else ""
        result[fid] = {
            "path": sp.relative_to(WORKSPACE).as_posix(),
            "case_ids": sorted(case_ids),
            "title": title,
        }
    return result


def scan_adrs() -> Dict[str, dict]:
    """掃 docs/adr/ADR-*.md 抓 {adr_id: {path, case_ids, title}}."""
    result: Dict[str, dict] = {}
    if not ADR_DIR.is_dir():
        return result
    for ad in ADR_DIR.glob("ADR-*.md"):
        m = re.match(r"(ADR-\d{4})", ad.stem)
        if not m:
            continue
        aid = m.group(1)
        text = ad.read_text(encoding="utf-8", errors="replace")
        case_ids = set(f"CASE-{a}-{b}" for a, b in CASE_ID_RE.findall(text))
        title_match = re.match(r"#\s*ADR-\d{4}[：:]?\s*(.+)", text)
        title = title_match.group(1).strip() if title_match else ""
        result[aid] = {
            "path": ad.relative_to(WORKSPACE).as_posix(),
            "case_ids": sorted(case_ids),
            "title": title,
        }
    return result


def build_case_to_specs(specs: Dict[str, dict]) -> Dict[str, List[str]]:
    """反向索引：{case_id: [feature_ids]}"""
    idx: Dict[str, List[str]] = {}
    for fid, info in specs.items():
        for cid in info["case_ids"]:
            idx.setdefault(cid, []).append(fid)
    return {k: sorted(v) for k, v in idx.items()}


def build_case_to_adrs(adrs: Dict[str, dict]) -> Dict[str, List[str]]:
    idx: Dict[str, List[str]] = {}
    for aid, info in adrs.items():
        for cid in info["case_ids"]:
            idx.setdefault(cid, []).append(aid)
    return {k: sorted(v) for k, v in idx.items()}


def find_conflicting_case_ids(weekly_data: dict) -> Dict[str, List[dict]]:
    """找出被多個不同 (customer, title) 共用的 CASE-ID。
    回傳 {case_id: [item, item, ...]} 只含衝突項。
    """
    by_cid: Dict[str, List[dict]] = {}
    for item in weekly_data.get("items", []):
        cid = extract_case_from_item(item)
        if not cid:
            continue
        by_cid.setdefault(cid, []).append(item)
    conflicts: Dict[str, List[dict]] = {}
    for cid, items in by_cid.items():
        if len(items) <= 1:
            continue
        # 不同 customer 或 title 視為衝突
        signatures = {(it.get("customer", ""), (it.get("title", "") or "")[:30])
                      for it in items}
        if len(signatures) > 1:
            conflicts[cid] = items
    return conflicts


def sync_to_weekly(weekly_data: dict, case_to_specs: Dict[str, List[str]],
                   case_to_adrs: Dict[str, List[str]],
                   conflicts: Dict[str, List[dict]] | None = None) -> Tuple[int, int]:
    """把 spec_refs / adr_refs 寫入 weekly items。
    回傳 (updated, skipped_due_to_conflict)。
    對於 CASE-ID 衝突的 item，不寫入 spec_refs/adr_refs 以避免污染。
    """
    conflicts = conflicts or {}
    updated = 0
    skipped = 0
    for item in weekly_data.get("items", []):
        cid = extract_case_from_item(item)
        if not cid:
            continue
        if cid in conflicts:
            # 清掉舊的污染綁定，但保留 case_id 供使用者檢視
            if item.get("spec_refs") or item.get("adr_refs"):
                item["spec_refs"] = []
                item["adr_refs"] = []
                updated += 1
            item["case_id"] = cid
            skipped += 1
            continue
        new_specs = case_to_specs.get(cid, [])
        new_adrs = case_to_adrs.get(cid, [])
        old_specs = item.get("spec_refs", []) or []
        old_adrs = item.get("adr_refs", []) or []
        if sorted(new_specs) != sorted(old_specs) or sorted(new_adrs) != sorted(old_adrs):
            item["spec_refs"] = new_specs
            item["adr_refs"] = new_adrs
            item["case_id"] = cid
            updated += 1
    return updated, skipped


def health_check(weekly_data: dict, specs: Dict[str, dict],
                 adrs: Dict[str, dict],
                 case_to_specs: Dict[str, List[str]]) -> List[str]:
    """產健康報告。"""
    out: List[str] = []
    out.append("# CASE / SPEC / ADR 證據鏈健康報告")
    out.append("")

    items = weekly_data.get("items", [])
    cases_in_weekly: Dict[str, dict] = {}
    for item in items:
        cid = extract_case_from_item(item)
        if cid:
            cases_in_weekly[cid] = item

    out.append("## 摘要")
    out.append("")
    out.append(f"- Weekly items 總數：{len(items)}")
    out.append(f"- 有 case_id 的 items：{len(cases_in_weekly)}")
    out.append(f"- Feature SPEC 數：{len(specs)}")
    out.append(f"- ADR 數：{len(adrs)}")
    out.append(f"- SPEC 引用的 case 數：{len(case_to_specs)}")

    # 0. CASE-ID 衝突（同一 ID 被多個不同客戶 / 標題共用）
    conflicts = find_conflicting_case_ids(weekly_data)
    out.append("")
    out.append(f"## [警告] CASE-ID 衝突 ({len(conflicts)})")
    out.append("")
    if conflicts:
        out.append("以下 CASE-ID 被多個不同 (客戶, 標題) 共用，工具已**拒絕**為其寫入 spec_refs / adr_refs 以免污染證據鏈。請使用 Weekly_AI 的 case_registry.py 重新發 ID。")
        out.append("")
        out.append("| CASE-ID | 共用 item 數 | 客戶清單 |")
        out.append("|---------|--------------|----------|")
        for cid, its in sorted(conflicts.items()):
            custs = sorted({it.get("customer", "") for it in its})
            out.append(f"| {cid} | {len(its)} | {', '.join(custs)} |")
    else:
        out.append("[OK] 無 CASE-ID 衝突。")

    # 1. status=done 但無 SPEC
    done_no_spec = [
        (cid, it) for cid, it in cases_in_weekly.items()
        if (it.get("status") == "done") and not case_to_specs.get(cid)
    ]
    out.append("")
    out.append(f"## [警告] status=done 但無 SPEC 的 case ({len(done_no_spec)})")
    out.append("")
    if done_no_spec:
        out.append("| CASE-ID | 客戶 | 標題 | 建議 |")
        out.append("|---------|------|------|------|")
        for cid, it in done_no_spec[:30]:
            out.append(
                f"| {cid} | {it.get('customer','')} | {(it.get('title','') or '')[:40]} | 建立 docs/spec/features/F899-NNN-...spec.md |"
            )
        if len(done_no_spec) > 30:
            out.append(f"| ... 另 {len(done_no_spec)-30} 筆 | | | |")
    else:
        out.append("[OK] 所有 done case 都有對應 SPEC。")

    # 2. SPEC 引用了不存在的 CASE
    out.append("")
    out.append("## SPEC 引用檢查")
    out.append("")
    out.append("| Feature SPEC | 引用 CASE-ID | 是否存在於 weekly_data |")
    out.append("|--------------|--------------|----------------------|")
    for fid, info in sorted(specs.items()):
        for cid in info["case_ids"]:
            exists = "[OK]" if cid in cases_in_weekly else "[警告] 不存在"
            out.append(f"| `{fid}` | {cid} | {exists} |")
        if not info["case_ids"]:
            out.append(f"| `{fid}` | (無) | [警告] SPEC 未填 CASE-ID |")

    # 3. 完整證據鏈（排除衝突）
    linked = [(cid, it) for cid, it in cases_in_weekly.items()
              if case_to_specs.get(cid) and cid not in conflicts]
    out.append("")
    out.append(f"## [OK] 完整證據鏈 case ({len(linked)})")
    out.append("")
    if linked:
        out.append("| CASE-ID | 客戶 | 狀態 | SPEC | ADR |")
        out.append("|---------|------|------|------|-----|")
        for cid, it in sorted(linked):
            sp = ", ".join(case_to_specs.get(cid, [])) or "-"
            ad_idx: Dict[str, List[str]] = {}
            for aid, info in adrs.items():
                if cid in info["case_ids"]:
                    ad_idx.setdefault(cid, []).append(aid)
            ad = ", ".join(ad_idx.get(cid, [])) or "-"
            out.append(
                f"| {cid} | {it.get('customer','')} | {it.get('status','')} | {sp} | {ad} |"
            )

    # 4. 客戶分布（以 (customer, case_id) 為唯一鍵，避免 dict overwrite）
    out.append("")
    out.append("## 客戶 SPEC 覆蓋率（done case，已排除衝突）")
    out.append("")
    cust_stats: Dict[str, Dict[str, int]] = {}
    seen: Set[Tuple[str, str]] = set()
    for item in items:
        if item.get("status") != "done":
            continue
        cid = extract_case_from_item(item)
        if not cid or cid in conflicts:
            continue
        cust = item.get("customer", "(unknown)")
        key = (cust, cid)
        if key in seen:
            continue
        seen.add(key)
        s = cust_stats.setdefault(cust, {"total": 0, "with_spec": 0})
        s["total"] += 1
        if case_to_specs.get(cid):
            s["with_spec"] += 1
    if cust_stats:
        out.append("| 客戶 | done case | 有 SPEC | 覆蓋率 |")
        out.append("|------|-----------|---------|--------|")
        for cust, s in sorted(cust_stats.items(), key=lambda x: -x[1]["total"]):
            ratio = (s["with_spec"] / s["total"] * 100) if s["total"] else 0
            out.append(f"| {cust} | {s['total']} | {s['with_spec']} | {ratio:.0f}% |")

    return out


def main() -> int:
    parser = argparse.ArgumentParser(description="HT9045 CASE/SPEC/ADR 雙向綁定")
    parser.add_argument("--weekly", default=str(DEFAULT_WEEKLY),
                        help="weekly_data.json 路徑")
    parser.add_argument("--write", action="store_true",
                        help="實際寫回 weekly_data.json（預設只報告）")
    parser.add_argument("--out", help="健康報告輸出 markdown 路徑")
    args = parser.parse_args()

    weekly_path = Path(args.weekly)
    if not weekly_path.is_file():
        sys.exit(f"[ERROR] 找不到 weekly_data.json：{weekly_path}")

    sys.stderr.write(f"[INFO] weekly_data: {weekly_path}\n")
    weekly_data = load_weekly(weekly_path)

    specs = scan_specs()
    adrs = scan_adrs()
    case_to_specs = build_case_to_specs(specs)
    case_to_adrs = build_case_to_adrs(adrs)

    sys.stderr.write(f"[INFO] 掃到 {len(specs)} 份 SPEC、{len(adrs)} 份 ADR\n")
    sys.stderr.write(f"[INFO] {len(case_to_specs)} 個 case 被 SPEC 引用\n")

    conflicts = find_conflicting_case_ids(weekly_data)
    if conflicts:
        sys.stderr.write(f"[WARN] 偵測到 {len(conflicts)} 個 CASE-ID 衝突，將不會為這些 item 寫入 spec_refs\n")

    if args.write:
        updated, skipped = sync_to_weekly(weekly_data, case_to_specs, case_to_adrs, conflicts)
        save_weekly(weekly_path, weekly_data)
        sys.stderr.write(f"[INFO] 已更新 weekly_data.json：{updated} 筆 items（已自動 backup），跳過 {skipped} 筆衝突\n")
    else:
        # dry-run：只計算會更新的筆數
        updated, skipped = sync_to_weekly(json.loads(weekly_path.read_text(encoding="utf-8")),
                                          case_to_specs, case_to_adrs, conflicts)
        sys.stderr.write(f"[DRY-RUN] 若加 --write 會更新 {updated} 筆 items，跳過 {skipped} 筆衝突\n")

    lines = health_check(weekly_data, specs, adrs, case_to_specs)
    text = "\n".join(lines)

    if args.out:
        out_path = Path(args.out)
        if not out_path.is_absolute():
            out_path = WORKSPACE / out_path
        out_path.parent.mkdir(parents=True, exist_ok=True)
        out_path.write_text(text, encoding="utf-8")
        sys.stderr.write(f"[INFO] 已寫入健康報告 {out_path}\n")
    else:
        sys.stdout.write(text)
        sys.stdout.write("\n")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
