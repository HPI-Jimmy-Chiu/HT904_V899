# -*- coding: cp950 -*-
"""
version_diff.py — HT9045 跨版本差異比對工具

用途：比對兩個版本資料夾，列出函式級差異，協助跨版搬功能。

用法：
    python scripts/version_diff.py <版本A> <版本B> [--module <關鍵字>] [--out <檔案>]

範例：
    python scripts/version_diff.py V897 V899
    python scripts/version_diff.py V897 V899 --module InArm
    python scripts/version_diff.py HT9011UC_Code_V3.33.897.0_20260304_Jimmy_20260312 \\
                                  HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422

版本參數可填：
  - 完整資料夾名稱（如 HT9011UC_Code_V3.33.899.0_...）
  - 簡寫 V<NNN>（如 V897、V899），自動匹配 HT9011UC_Code_V3.33.<NNN>.* 資料夾
"""
from __future__ import annotations
import argparse
import re
import sys
from pathlib import Path
from typing import Dict, List, Set, Tuple

WORKSPACE = Path(__file__).resolve().parent.parent
SOURCE_EXTS = {".cpp", ".h", ".hpp", ".c"}

# BCB6 風格函式定義 regex（簡化版）
# 抓 "void Class::Func(...)" 或 "bool DoXxx(...)" 等
FUNC_DEF_RE = re.compile(
    r"^\s*(?:[\w_*&]+\s+){1,4}"               # 回傳型別 + 修飾字
    r"(?:__fastcall\s+)?"                      # BCB 修飾
    r"(?:[\w_]+::)?"                           # Class::
    r"([A-Za-z_][\w_]*)"                       # 函式名（capture）
    r"\s*\([^;{}]*\)\s*$"                      # 參數列，結尾無 ; 或 {
)

# Big5 來源檔
ENCODING = "cp950"


def resolve_version_dir(arg: str) -> Path:
    """把 V897 / V899 簡寫對應到完整資料夾路徑。"""
    p = WORKSPACE / arg
    if p.is_dir():
        return p
    m = re.fullmatch(r"V(\d{3,4})", arg, re.IGNORECASE)
    if m:
        ver = m.group(1)
        candidates = sorted(WORKSPACE.glob(f"HT9011UC_Code_V*.{ver}.*"))
        if not candidates:
            candidates = sorted(WORKSPACE.glob(f"HT9046LS_Code_V*.{ver}.*"))
        if len(candidates) == 1:
            return candidates[0]
        if len(candidates) > 1:
            sys.stderr.write(f"[WARN] {arg} 對應多個資料夾，採用最後一個：\n")
            for c in candidates:
                sys.stderr.write(f"  {c.name}\n")
            return candidates[-1]
    sys.exit(f"[ERROR] 找不到版本資料夾：{arg}")


def collect_files(root: Path, module_filter: str | None) -> Dict[str, Path]:
    """走訪資料夾，回傳 {相對路徑: 絕對路徑}。"""
    result: Dict[str, Path] = {}
    for p in root.rglob("*"):
        if not p.is_file():
            continue
        if p.suffix.lower() not in SOURCE_EXTS:
            continue
        # 略過備份/SVN
        parts_lower = [s.lower() for s in p.parts]
        if any(seg in parts_lower for seg in (".svn", "backup", "obj")):
            continue
        rel = p.relative_to(root).as_posix()
        if module_filter and module_filter.lower() not in rel.lower():
            continue
        result[rel] = p
    return result


def extract_funcs(path: Path) -> Set[str]:
    """從原始檔抓函式名。簡化版：抓「型別 + 名稱(...)」後面接 { 的行。"""
    funcs: Set[str] = set()
    try:
        text = path.read_text(encoding=ENCODING, errors="replace")
    except OSError:
        return funcs
    lines = text.splitlines()
    for i, line in enumerate(lines):
        # 跳過註解行
        stripped = line.strip()
        if stripped.startswith("//") or stripped.startswith("*") or stripped.startswith("/*"):
            continue
        m = FUNC_DEF_RE.match(line)
        if not m:
            continue
        # 確認下一非空行是 { 或本行已含 {
        if "{" in line:
            funcs.add(m.group(1))
            continue
        for j in range(i + 1, min(i + 3, len(lines))):
            nxt = lines[j].strip()
            if not nxt:
                continue
            if nxt.startswith("{"):
                funcs.add(m.group(1))
            break
    return funcs


def file_hash(path: Path) -> int:
    try:
        return hash(path.read_bytes())
    except OSError:
        return 0


def diff_versions(dir_a: Path, dir_b: Path, module_filter: str | None) -> List[str]:
    files_a = collect_files(dir_a, module_filter)
    files_b = collect_files(dir_b, module_filter)

    set_a = set(files_a.keys())
    set_b = set(files_b.keys())

    only_a = sorted(set_a - set_b)
    only_b = sorted(set_b - set_a)
    common = sorted(set_a & set_b)

    out: List[str] = []
    out.append(f"# 版本差異報告")
    out.append("")
    out.append(f"- 來源 A：`{dir_a.name}`")
    out.append(f"- 目標 B：`{dir_b.name}`")
    if module_filter:
        out.append(f"- 模組過濾：`{module_filter}`")
    out.append("")

    out.append(f"## 摘要")
    out.append("")
    out.append(f"| 類別 | 數量 |")
    out.append(f"|------|------|")
    out.append(f"| A 獨有檔案 | {len(only_a)} |")
    out.append(f"| B 獨有檔案 | {len(only_b)} |")
    out.append(f"| 兩版皆有檔案 | {len(common)} |")
    out.append("")

    if only_a:
        out.append("## A 獨有檔案（B 已刪除）")
        out.append("")
        for f in only_a[:50]:
            out.append(f"- `{f}`")
        if len(only_a) > 50:
            out.append(f"- ... 另 {len(only_a) - 50} 個")
        out.append("")

    if only_b:
        out.append("## B 獨有檔案（A 沒有，需評估搬移）")
        out.append("")
        for f in only_b[:50]:
            out.append(f"- `{f}`")
        if len(only_b) > 50:
            out.append(f"- ... 另 {len(only_b) - 50} 個")
        out.append("")

    # 找出內容不同的共同檔
    changed: List[Tuple[str, Set[str], Set[str]]] = []
    for rel in common:
        if file_hash(files_a[rel]) == file_hash(files_b[rel]):
            continue
        funcs_a = extract_funcs(files_a[rel])
        funcs_b = extract_funcs(files_b[rel])
        new_funcs = funcs_b - funcs_a
        removed_funcs = funcs_a - funcs_b
        if new_funcs or removed_funcs or funcs_a != funcs_b:
            changed.append((rel, new_funcs, removed_funcs))

    out.append(f"## 兩版皆有但內容不同的檔案：{len(changed)}")
    out.append("")
    if changed:
        out.append("| 檔案 | B 新增函式 | B 刪除函式 |")
        out.append("|------|-----------|-----------|")
        for rel, new_funcs, removed_funcs in changed[:100]:
            new_str = ", ".join(sorted(new_funcs)[:5]) or "—"
            if len(new_funcs) > 5:
                new_str += f" (+{len(new_funcs) - 5})"
            rem_str = ", ".join(sorted(removed_funcs)[:5]) or "—"
            if len(removed_funcs) > 5:
                rem_str += f" (+{len(removed_funcs) - 5})"
            out.append(f"| `{rel}` | {new_str} | {rem_str} |")
        if len(changed) > 100:
            out.append(f"| ... 另 {len(changed) - 100} 個變更檔案 | | |")
    out.append("")

    return out


def main() -> int:
    parser = argparse.ArgumentParser(description="HT9045 版本差異比對")
    parser.add_argument("version_a", help="來源版本（V897 或完整資料夾名）")
    parser.add_argument("version_b", help="目標版本（V899 或完整資料夾名）")
    parser.add_argument("--module", help="只比對路徑含此關鍵字的檔案（如 InArm、SECSGEM）")
    parser.add_argument("--out", help="輸出 markdown 檔路徑（預設輸出到 stdout）")
    args = parser.parse_args()

    dir_a = resolve_version_dir(args.version_a)
    dir_b = resolve_version_dir(args.version_b)

    sys.stderr.write(f"[INFO] A = {dir_a.name}\n")
    sys.stderr.write(f"[INFO] B = {dir_b.name}\n")

    lines = diff_versions(dir_a, dir_b, args.module)
    text = "\n".join(lines)

    if args.out:
        out_path = Path(args.out)
        out_path.parent.mkdir(parents=True, exist_ok=True)
        out_path.write_text(text, encoding="utf-8")
        sys.stderr.write(f"[INFO] 已寫入 {out_path}\n")
    else:
        sys.stdout.write(text)
        sys.stdout.write("\n")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
