# -*- coding: utf-8 -*-
"""
link_hangup_to_spec.py — 建立 hangup 案例 ↔ SPEC ↔ ADR 的反向鏈結表

用途：
    Hangup 紀錄、SPEC、ADR 三套文件互不知道彼此存在，根因分析時要在多處翻找。
    本工具掃描三個來源，產出一份交叉參照表：
      1. /memories/repo/hangup-*.md（透過讀檔模擬，因 memory 在外部）
         → 讀 docs/hangup-notes/ 或 hangup/ 資料夾名稱
      2. docs/spec/features/*.spec.md  → 抓 metadata 中的 hangup / case 欄位
      3. docs/adr/ADR-*.md              → 抓 「關聯」段中的 hangup 引用

用法：
    python scripts/link_hangup_to_spec.py
    python scripts/link_hangup_to_spec.py --out docs/spec/_hangup_crossref.md

輸出包含：
    - 每個 hangup 對應到哪些 SPEC / ADR
    - 孤兒 hangup（無對應 SPEC）→ 提醒應補 SPEC
    - 孤兒 SPEC（無對應 hangup 但實作清單寫了 bug 修復）→ 提醒檢查
"""
from __future__ import annotations
import argparse
import re
import sys
from pathlib import Path
from typing import Dict, List, Set

WORKSPACE = Path(__file__).resolve().parent.parent
SPEC_DIR = WORKSPACE / "docs" / "spec"
ADR_DIR = WORKSPACE / "docs" / "adr"
HANGUP_DIR = WORKSPACE / "hangup"

# memory hangup 檔在 /memories/repo/hangup-*.md，但本機 script 無法讀
# 作為替代，從 docs/hangup-notes/ 也掃；若不存在則只用 hangup/ 資料夾名
DOC_HANGUP_DIR = WORKSPACE / "docs" / "hangup-notes"

# 從文字裡抓 hangup 識別字（日期 / case-id）
HANGUP_DATE_RE = re.compile(r"hangup[-/]?(\d{8})", re.IGNORECASE)
CASE_ID_RE = re.compile(r"CASE-(\d{8})-(\d{3})")
ADR_REF_RE = re.compile(r"ADR-(\d{4})")
SPEC_REF_RE = re.compile(r"F(\d{3,4})-(\d{3})")


def list_hangup_folders() -> List[str]:
    """掃 hangup/ 資料夾下的子目錄名稱（每筆代表一個案例）。"""
    if not HANGUP_DIR.is_dir():
        return []
    return sorted(p.name for p in HANGUP_DIR.iterdir() if p.is_dir())


def list_hangup_notes() -> List[Path]:
    """掃 docs/hangup-notes/ 下的 .md（若使用者有從 memory 同步出來）。"""
    if not DOC_HANGUP_DIR.is_dir():
        return []
    return sorted(DOC_HANGUP_DIR.glob("*.md"))


def list_specs() -> List[Path]:
    feat = SPEC_DIR / "features"
    if not feat.is_dir():
        return []
    return sorted(feat.glob("*.spec.md"))


def list_adrs() -> List[Path]:
    if not ADR_DIR.is_dir():
        return []
    return sorted(ADR_DIR.glob("ADR-*.md"))


def scan_refs(path: Path) -> Dict[str, Set[str]]:
    """從文件抓出所引用的 hangup / case / spec / adr。"""
    refs = {"hangup_dates": set(), "case_ids": set(), "specs": set(), "adrs": set()}
    try:
        text = path.read_text(encoding="utf-8", errors="replace")
    except OSError:
        try:
            text = path.read_text(encoding="cp950", errors="replace")
        except OSError:
            return refs
    for m in HANGUP_DATE_RE.finditer(text):
        refs["hangup_dates"].add(m.group(1))
    for m in CASE_ID_RE.finditer(text):
        refs["case_ids"].add(f"CASE-{m.group(1)}-{m.group(2)}")
    for m in SPEC_REF_RE.finditer(text):
        refs["specs"].add(f"F{m.group(1)}-{m.group(2)}")
    for m in ADR_REF_RE.finditer(text):
        refs["adrs"].add(f"ADR-{m.group(1)}")
    return refs


def build_report() -> List[str]:
    out: List[str] = []
    out.append("# Hangup ↔ SPEC ↔ ADR 交叉參照表")
    out.append("")
    out.append("> 自動產生，請勿手改。重新執行 `scripts/link_hangup_to_spec.py` 更新。")
    out.append("")

    hangup_folders = list_hangup_folders()
    hangup_notes = list_hangup_notes()
    specs = list_specs()
    adrs = list_adrs()

    out.append("## 摘要")
    out.append("")
    out.append(f"- Hangup 資料夾數：{len(hangup_folders)}")
    out.append(f"- Hangup 筆記文件數（docs/hangup-notes/）：{len(hangup_notes)}")
    out.append(f"- 功能 SPEC 數：{len(specs)}")
    out.append(f"- ADR 數：{len(adrs)}")
    out.append("")

    # ---------- SPEC -> 引用 ----------
    spec_refs: Dict[str, Dict[str, Set[str]]] = {}
    for sp in specs:
        spec_id_match = re.match(r"(F\d+-\d+)", sp.stem)
        spec_id = spec_id_match.group(1) if spec_id_match else sp.stem
        spec_refs[spec_id] = scan_refs(sp)

    # ---------- ADR -> 引用 ----------
    adr_refs: Dict[str, Dict[str, Set[str]]] = {}
    for ad in adrs:
        adr_id_match = re.match(r"(ADR-\d{4})", ad.stem)
        adr_id = adr_id_match.group(1) if adr_id_match else ad.stem
        adr_refs[adr_id] = scan_refs(ad)

    # ---------- Hangup notes -> 引用 ----------
    note_refs: Dict[str, Dict[str, Set[str]]] = {}
    for nt in hangup_notes:
        note_refs[nt.stem] = scan_refs(nt)

    # ---------- 反向：每個 hangup 被誰引用 ----------
    out.append("## SPEC 引用清單")
    out.append("")
    out.append("| Feature SPEC | 引用 Hangup | 引用 Case | 引用 ADR |")
    out.append("|--------------|-------------|-----------|----------|")
    for sid, r in sorted(spec_refs.items()):
        h = ", ".join(sorted(r["hangup_dates"])) or "—"
        c = ", ".join(sorted(r["case_ids"])) or "—"
        a = ", ".join(sorted(r["adrs"])) or "—"
        out.append(f"| `{sid}` | {h} | {c} | {a} |")
    out.append("")

    out.append("## ADR 引用清單")
    out.append("")
    out.append("| ADR | 引用 Hangup | 引用 Case | 引用 SPEC |")
    out.append("|-----|-------------|-----------|-----------|")
    for aid, r in sorted(adr_refs.items()):
        h = ", ".join(sorted(r["hangup_dates"])) or "—"
        c = ", ".join(sorted(r["case_ids"])) or "—"
        s = ", ".join(sorted(r["specs"])) or "—"
        out.append(f"| `{aid}` | {h} | {c} | {s} |")
    out.append("")

    # ---------- Hangup 資料夾覆蓋率 ----------
    out.append("## Hangup 資料夾 SPEC/ADR 覆蓋率")
    out.append("")
    referenced_dates: Set[str] = set()
    for r in list(spec_refs.values()) + list(adr_refs.values()):
        referenced_dates |= r["hangup_dates"]

    out.append("| Hangup 資料夾 | 對應 SPEC | 對應 ADR | 狀態 |")
    out.append("|--------------|-----------|----------|------|")
    for folder in hangup_folders:
        # 從資料夾名抓日期片段 YYYY-MM-DD HH_MM_SS → 取 YYYYMMDD
        date_match = re.match(r"(\d{4})-(\d{2})-(\d{2})", folder)
        if date_match:
            ymd = f"{date_match.group(1)}{date_match.group(2)}{date_match.group(3)}"
        else:
            ymd = ""
        related_specs = [sid for sid, r in spec_refs.items() if ymd in r["hangup_dates"]]
        related_adrs = [aid for aid, r in adr_refs.items() if ymd in r["hangup_dates"]]
        status = "[OK]" if (related_specs or related_adrs) else "[警告] 無對應 SPEC/ADR"
        out.append(f"| `{folder}` | {', '.join(related_specs) or '—'} | {', '.join(related_adrs) or '—'} | {status} |")
    out.append("")

    out.append("## 建議")
    out.append("")
    out.append("1. 標記 `[警告]` 的 hangup 資料夾應建立對應 SPEC（`docs/spec/features/`）或 ADR")
    out.append("2. 若 hangup 已結案但決策非本案 SPEC 範圍 → 建立 ADR")
    out.append("3. 若 hangup 為一次性偶發 → 在 SPEC 的「Known Issues」加一行即可")
    return out


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--out", help="輸出 markdown 檔（預設 stdout）")
    args = parser.parse_args()

    lines = build_report()
    text = "\n".join(lines)

    if args.out:
        out_path = Path(args.out)
        if not out_path.is_absolute():
            out_path = WORKSPACE / out_path
        out_path.parent.mkdir(parents=True, exist_ok=True)
        out_path.write_text(text, encoding="utf-8")
        sys.stderr.write(f"[INFO] 已寫入 {out_path}\n")
    else:
        sys.stdout.write(text)
        sys.stdout.write("\n")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
