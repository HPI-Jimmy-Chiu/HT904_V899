# -*- coding: cp950 -*-
"""
spec_to_release_note.py — 從功能 SPEC 萃取 customer-facing release note 草稿

用途：
    每份功能 SPEC 第 8 節「Release Note 摘要」是 customer-facing 的描述。
    本工具掃描指定版本的所有功能 SPEC，把第 8 節抽出來組成 release note 草稿。

用法：
    python scripts/spec_to_release_note.py --version 899
    python scripts/spec_to_release_note.py --version 899 --out docs/release_notes/V899_draft.md

輸出格式：分客戶/分內部三版本（internal / distributor / customer）。
"""
from __future__ import annotations
import argparse
import re
import sys
from pathlib import Path
from typing import List

WORKSPACE = Path(__file__).resolve().parent.parent
SPEC_DIR = WORKSPACE / "docs" / "spec" / "features"


def extract_section_8(spec_path: Path) -> str:
    """抓第 8 節「Release Note 摘要」內容。"""
    text = spec_path.read_text(encoding="utf-8", errors="replace")
    # 找 ## 8. Release Note 摘要 → 到下一個 ## 為止
    m = re.search(r"##\s*8\.\s*Release Note 摘要\s*\n(.+?)(?=\n##\s|\Z)", text, re.DOTALL)
    if not m:
        return ""
    return m.group(1).strip()


def extract_metadata(spec_path: Path) -> dict:
    """抓 metadata 表的關鍵欄位。"""
    text = spec_path.read_text(encoding="utf-8", errors="replace")
    md = {}
    for key in ["Feature ID", "標題", "客戶 / 來源", "對應 CASE-ID", "影響模組", "安全等級"]:
        m = re.search(rf"\|\s*{re.escape(key)}\s*\|\s*([^|\n]+?)\s*\|", text)
        if m:
            md[key] = m.group(1).strip()
    return md


def build_release_note(version: str, spec_files: List[Path]) -> List[str]:
    out: List[str] = []
    out.append(f"# HT9045 V{version} Release Note 草稿")
    out.append("")
    out.append("> 自動從 `docs/spec/features/F" + version + "-*.spec.md` 第 8 節萃取。")
    out.append("> 三個版本：internal（含技術細節）/ distributor（含風險）/ customer（純功能描述）。")
    out.append("")

    valid_specs = []
    for sp in spec_files:
        meta = extract_metadata(sp)
        rn = extract_section_8(sp)
        if not rn:
            continue
        valid_specs.append((sp, meta, rn))

    if not valid_specs:
        out.append("[警告] 找不到任何 V" + version + " 功能 SPEC，或所有 SPEC 第 8 節為空。")
        return out

    out.append(f"## 摘要：本版共 {len(valid_specs)} 項變更")
    out.append("")
    out.append("| Feature ID | 標題 | 影響模組 | 安全等級 |")
    out.append("|------------|------|----------|----------|")
    for sp, meta, _ in valid_specs:
        out.append(
            f"| {meta.get('Feature ID', sp.stem)} | {meta.get('標題', '')} | "
            f"{meta.get('影響模組', '')} | {meta.get('安全等級', '')} |"
        )
    out.append("")

    # ---------- Customer 版（純功能描述）----------
    out.append("---")
    out.append("")
    out.append("## CUSTOMER 版（給客戶 / 終端使用者）")
    out.append("")
    for sp, meta, rn in valid_specs:
        out.append(rn)
        out.append("")

    # ---------- Distributor 版（含風險）----------
    out.append("---")
    out.append("")
    out.append("## DISTRIBUTOR 版（給代理商 / FAE）")
    out.append("")
    for sp, meta, rn in valid_specs:
        out.append(f"### {meta.get('Feature ID', sp.stem)} — {meta.get('標題', '')}")
        out.append("")
        out.append(rn)
        out.append("")
        # 加上風險段（從 SPEC 第 7 節）
        text = sp.read_text(encoding="utf-8", errors="replace")
        risk_match = re.search(r"##\s*7\.\s*風險[^\n]*\n(.+?)(?=\n##\s|\Z)", text, re.DOTALL)
        if risk_match:
            out.append("**風險 / 副作用**：")
            out.append("")
            out.append(risk_match.group(1).strip())
            out.append("")

    # ---------- Internal 版（含技術細節）----------
    out.append("---")
    out.append("")
    out.append("## INTERNAL 版（給內部開發 / QA）")
    out.append("")
    for sp, meta, rn in valid_specs:
        out.append(f"### {meta.get('Feature ID', sp.stem)} — {meta.get('標題', '')}")
        out.append("")
        out.append(f"- 客戶 / 來源：{meta.get('客戶 / 來源', 'N/A')}")
        out.append(f"- CASE-ID：{meta.get('對應 CASE-ID', 'N/A')}")
        out.append(f"- 影響模組：{meta.get('影響模組', 'N/A')}")
        out.append(f"- 安全等級：{meta.get('安全等級', 'N/A')}")
        out.append(f"- SPEC 來源：`{sp.relative_to(WORKSPACE).as_posix()}`")
        out.append("")
        out.append(rn)
        out.append("")

    out.append("---")
    out.append("")
    out.append("## 下一步")
    out.append("")
    out.append("1. 人工 review 三版內容差異是否合適")
    out.append("2. 若有 //AI 註解但無對應 SPEC，請補 SPEC 後重跑")
    out.append("3. 對應 case 的 weekly_data 加入此 release note 連結")
    return out


def main() -> int:
    parser = argparse.ArgumentParser(description="HT9045 從 SPEC 產 release note 草稿")
    parser.add_argument("--version", required=True, help="版本號（如 899）")
    parser.add_argument("--out", help="輸出 markdown 檔（預設 stdout）")
    args = parser.parse_args()

    pattern = f"F{args.version}-*.spec.md"
    spec_files = sorted(SPEC_DIR.glob(pattern))

    sys.stderr.write(f"[INFO] 找到 {len(spec_files)} 份 SPEC 符合 {pattern}\n")

    lines = build_release_note(args.version, spec_files)
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
