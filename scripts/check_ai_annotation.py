# -*- coding: cp950 -*-
"""
check_ai_annotation.py — HT9045 //AI 註解格式檢查

用途：
    1. 檢查指定資料夾內所有 //AI 註解是否符合 `//AI(AgentName) YYYYMMDD: 描述` 格式。
    2. 可選：對比 git diff，找出本次修改但「未加 //AI 註解」的檔案。

用法：
    python scripts/check_ai_annotation.py [<目標資料夾>]
    python scripts/check_ai_annotation.py --diff [<base_ref>]

範例：
    python scripts/check_ai_annotation.py
    python scripts/check_ai_annotation.py HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422
    python scripts/check_ai_annotation.py --diff HEAD~1

預設目標：HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422
"""
from __future__ import annotations
import argparse
import re
import subprocess
import sys
from pathlib import Path
from typing import List, Tuple

WORKSPACE = Path(__file__).resolve().parent.parent
DEFAULT_TARGET = "HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422"
SOURCE_EXTS = {".cpp", ".h", ".hpp", ".c"}
ENCODING = "cp950"

# 合法格式：//AI(AgentName) YYYYMMDD: 描述
# AgentName 允許英數、底線、連字；日期後可有額外備註，最終接冒號
AI_VALID_RE = re.compile(
    r"//AI\(([\w\-]+)\)\s+(\d{8})\b[^:]*:\s*\S"
)
# 寬鬆抓 //AI( 開頭（避免抓到 // AI/AO 這類縮寫）
AI_LOOSE_RE = re.compile(r"//\s*AI\s*\(")


def scan_file(path: Path) -> Tuple[List[str], List[str]]:
    """回傳 (合法註解清單, 違規行清單)。"""
    valid: List[str] = []
    invalid: List[str] = []
    try:
        text = path.read_text(encoding=ENCODING, errors="replace")
    except OSError:
        return valid, invalid
    for i, line in enumerate(text.splitlines(), start=1):
        if not AI_LOOSE_RE.search(line):
            continue
        m = AI_VALID_RE.search(line)
        if m:
            valid.append(f"{path.name}:{i}  agent={m.group(1)} date={m.group(2)}")
        else:
            invalid.append(f"{path.relative_to(WORKSPACE).as_posix()}:{i}  {line.strip()}")
    return valid, invalid


def scan_folder(target: Path) -> Tuple[int, List[str]]:
    """掃整個資料夾，回傳 (合法數量, 違規清單)。"""
    valid_count = 0
    invalid_all: List[str] = []
    for p in target.rglob("*"):
        if not p.is_file() or p.suffix.lower() not in SOURCE_EXTS:
            continue
        parts_lower = [s.lower() for s in p.parts]
        if any(seg in parts_lower for seg in (".svn", "backup", "obj")):
            continue
        valid, invalid = scan_file(p)
        valid_count += len(valid)
        invalid_all.extend(invalid)
    return valid_count, invalid_all


def get_changed_files(base_ref: str) -> List[Path]:
    """用 git diff 取得本地修改檔案。"""
    try:
        out = subprocess.check_output(
            ["git", "diff", "--name-only", base_ref],
            cwd=WORKSPACE,
            text=True,
        )
    except (subprocess.CalledProcessError, FileNotFoundError) as e:
        sys.stderr.write(f"[ERROR] git diff 失敗：{e}\n")
        return []
    files = []
    for line in out.splitlines():
        p = WORKSPACE / line.strip()
        if p.suffix.lower() in SOURCE_EXTS and p.is_file():
            files.append(p)
    return files


def check_diff_mode(base_ref: str) -> int:
    """檢查 git 修改的檔案是否都有 //AI 註解。"""
    files = get_changed_files(base_ref)
    if not files:
        print(f"[INFO] {base_ref} 之後沒有 .cpp/.h 修改")
        return 0

    print(f"# //AI 註解檢查（diff 模式 vs {base_ref}）\n")
    print(f"檢查 {len(files)} 個修改檔案：\n")

    missing: List[str] = []
    invalid_all: List[str] = []
    for p in files:
        valid, invalid = scan_file(p)
        rel = p.relative_to(WORKSPACE).as_posix()
        if not valid and not invalid:
            missing.append(rel)
        invalid_all.extend(invalid)

    if missing:
        print("## ? 修改了但完全沒有 //AI 註解")
        print()
        for f in missing:
            print(f"- `{f}`")
        print()

    if invalid_all:
        print("## ??  //AI 註解格式錯誤")
        print()
        print("正確格式：`//AI(AgentName) YYYYMMDD: 描述`\n")
        for line in invalid_all:
            print(f"- {line}")
        print()

    if not missing and not invalid_all:
        print("? 所有修改檔案都有合法 //AI 註解")
        return 0
    return 1


def check_full_mode(target: Path) -> int:
    """全資料夾掃描。"""
    print(f"# //AI 註解檢查（全資料夾掃描）\n")
    print(f"目標：`{target.relative_to(WORKSPACE).as_posix()}`\n")

    valid_count, invalid_all = scan_folder(target)

    print(f"- 合法註解：{valid_count} 條")
    print(f"- 格式錯誤：{len(invalid_all)} 條\n")

    if invalid_all:
        print("## ??  格式錯誤明細\n")
        print("正確格式：`//AI(AgentName) YYYYMMDD: 描述`\n")
        for line in invalid_all[:200]:
            print(f"- {line}")
        if len(invalid_all) > 200:
            print(f"- ... 另 {len(invalid_all) - 200} 條")
        return 1
    print("? 沒有格式錯誤")
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description="HT9045 //AI 註解檢查")
    parser.add_argument("target", nargs="?", default=DEFAULT_TARGET,
                        help=f"目標資料夾（預設 {DEFAULT_TARGET}）")
    parser.add_argument("--diff", nargs="?", const="HEAD",
                        help="改用 git diff 模式，比對未提交的修改（預設 HEAD）")
    args = parser.parse_args()

    if args.diff is not None:
        return check_diff_mode(args.diff)

    target = WORKSPACE / args.target
    if not target.is_dir():
        sys.exit(f"[ERROR] 目標資料夾不存在：{target}")
    return check_full_mode(target)


if __name__ == "__main__":
    raise SystemExit(main())
