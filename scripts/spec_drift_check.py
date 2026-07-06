# -*- coding: cp950 -*-
"""
spec_drift_check.py — 偵測 SPEC 與實際程式碼的偏差

用途：
    SPEC 是「契約」，但程式碼會持續演進。本工具檢查每個 SPEC 提到的：
      - 函式名 → 是否仍存在於目標版本程式碼
      - 檔案路徑 → 是否仍存在
      - 變數 / 結構名 → 是否仍存在於 .h 檔
    任何不一致都會列為 drift，提醒 SPEC 該更新。

用法：
    python scripts/spec_drift_check.py
    python scripts/spec_drift_check.py --version V899
    python scripts/spec_drift_check.py --spec docs/spec/modules/inarm.spec.md
    python scripts/spec_drift_check.py --out docs/spec/_drift_report.md

預設掃 docs/spec/modules/ + docs/spec/features/，比對 V899 程式碼。
"""
from __future__ import annotations
import argparse
import re
import sys
from pathlib import Path
from typing import Dict, List, Set, Tuple

WORKSPACE = Path(__file__).resolve().parent.parent
SPEC_DIR = WORKSPACE / "docs" / "spec"
DEFAULT_VERSION = "HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422"
SOURCE_EXTS = {".cpp", ".h", ".hpp", ".c"}
ENCODING = "cp950"

sys.path.insert(0, str(WORKSPACE / "scripts"))
try:
    from version_diff import resolve_version_dir, extract_funcs  # type: ignore
except ImportError:
    sys.exit("[ERROR] 找不到 scripts/version_diff.py")

# 從 SPEC 抓「程式碼引用」
# 1. 函式名：` `Func()` ` 或 `Func()` 在 markdown table
FUNC_REF_RE = re.compile(r"`([A-Za-z_][\w]*)\(\)?`")
# 2. 檔案路徑：` `ARMS/foo.cpp` `
FILE_REF_RE = re.compile(r"`([\w\-/]+\.(?:cpp|h|hpp|c))`")
# 3. 變數 / 結構名（簡化：抓 `XxxData[]` `iXxxYyy` 這類）
VAR_REF_RE = re.compile(r"`([A-Za-z_][\w]+(?:\[\])?)`")


def collect_all_funcs(version_dir: Path) -> Set[str]:
    """收集整個版本程式碼的所有函式名。"""
    funcs: Set[str] = set()
    for p in version_dir.rglob("*"):
        if not p.is_file() or p.suffix.lower() not in SOURCE_EXTS:
            continue
        parts_lower = [s.lower() for s in p.parts]
        if any(seg in parts_lower for seg in (".svn", "backup", "obj")):
            continue
        funcs |= extract_funcs(p)
    return funcs


def collect_all_files(version_dir: Path) -> Set[str]:
    """收集所有原始檔的相對路徑（POSIX 格式）。"""
    files: Set[str] = set()
    for p in version_dir.rglob("*"):
        if not p.is_file() or p.suffix.lower() not in SOURCE_EXTS:
            continue
        parts_lower = [s.lower() for s in p.parts]
        if any(seg in parts_lower for seg in (".svn", "backup", "obj")):
            continue
        files.add(p.relative_to(version_dir).as_posix())
    return files


def collect_global_symbols(version_dir: Path) -> Set[str]:
    """從 .h 檔粗略抓全域變數/結構/typedef 名稱（用於變數 drift 檢查）。"""
    symbols: Set[str] = set()
    name_re = re.compile(r"\b([A-Z][\w]{3,})\b")
    for p in version_dir.rglob("*.h"):
        parts_lower = [s.lower() for s in p.parts]
        if any(seg in parts_lower for seg in (".svn", "backup", "obj")):
            continue
        try:
            text = p.read_text(encoding=ENCODING, errors="replace")
        except OSError:
            continue
        for m in name_re.finditer(text):
            symbols.add(m.group(1))
    return symbols


def parse_spec_refs(spec_path: Path) -> Tuple[Set[str], Set[str], Set[str]]:
    """從 SPEC 抓所引用的 (函式, 檔案, 變數)。"""
    text = spec_path.read_text(encoding="utf-8", errors="replace")
    funcs = set(m.group(1) for m in FUNC_REF_RE.finditer(text))
    files = set(m.group(1) for m in FILE_REF_RE.finditer(text))
    # 變數：所有 backtick 包住的字串扣掉檔案與函式
    all_back = set(m.group(1) for m in re.finditer(r"`([^`]+)`", text))
    vars_ = set()
    for s in all_back:
        if "(" in s or s.endswith(".cpp") or s.endswith(".h") or "/" in s or " " in s:
            continue
        # 基本過濾
        if re.match(r"^[A-Za-z_][\w]+(?:\[\])?$", s):
            vars_.add(s.replace("[]", ""))
    return funcs, files, vars_


def check_drift(spec_path: Path, all_funcs: Set[str], all_files: Set[str], all_symbols: Set[str]) -> Dict[str, List[str]]:
    """回傳 {drift 類型: [列出問題]}。"""
    funcs, files, vars_ = parse_spec_refs(spec_path)
    drift = {"missing_funcs": [], "missing_files": [], "missing_vars": []}

    # 過濾常見無意義詞
    ignore_funcs = {"true", "false", "NULL", "void", "int", "bool", "for", "if", "while"}

    for f in sorted(funcs):
        if f in ignore_funcs:
            continue
        if f not in all_funcs:
            drift["missing_funcs"].append(f)

    for f in sorted(files):
        # 寬鬆比對：只要尾段檔名相符就算存在
        fname = f.split("/")[-1].lower()
        if not any(af.endswith(f) or af.lower().endswith(fname) for af in all_files):
            drift["missing_files"].append(f)

    # 變數採寬鬆檢查（避免太多誤報）
    for v in sorted(vars_):
        if v in ignore_funcs:
            continue
        if len(v) < 4:
            continue
        if v not in all_symbols and v not in all_funcs:
            drift["missing_vars"].append(v)

    return drift


def build_report(version_dir: Path, spec_files: List[Path]) -> List[str]:
    out: List[str] = []
    out.append("# SPEC Drift 偵測報告")
    out.append("")
    out.append(f"- 比對版本：`{version_dir.name}`")
    out.append(f"- SPEC 數：{len(spec_files)}")
    out.append("")
    out.append("> 收集程式碼符號中... (這需要幾秒)")
    out.append("")

    sys.stderr.write("[INFO] 收集函式...\n")
    all_funcs = collect_all_funcs(version_dir)
    sys.stderr.write(f"[INFO] 收到 {len(all_funcs)} 個函式\n")
    sys.stderr.write("[INFO] 收集檔案...\n")
    all_files = collect_all_files(version_dir)
    sys.stderr.write(f"[INFO] 收到 {len(all_files)} 個原始檔\n")
    sys.stderr.write("[INFO] 收集全域符號...\n")
    all_symbols = collect_global_symbols(version_dir)
    sys.stderr.write(f"[INFO] 收到 {len(all_symbols)} 個全域符號\n")

    total_drift = 0
    for sp in spec_files:
        drift = check_drift(sp, all_funcs, all_files, all_symbols)
        n = sum(len(v) for v in drift.values())
        total_drift += n
        rel = sp.relative_to(WORKSPACE).as_posix()
        out.append(f"## `{rel}`")
        out.append("")
        if n == 0:
            out.append("[OK] 無 drift")
            out.append("")
            continue
        if drift["missing_funcs"]:
            out.append(f"### 失蹤函式 ({len(drift['missing_funcs'])})")
            out.append("")
            for f in drift["missing_funcs"]:
                out.append(f"- `{f}()` — SPEC 引用但版本內找不到此函式定義")
            out.append("")
        if drift["missing_files"]:
            out.append(f"### 失蹤檔案 ({len(drift['missing_files'])})")
            out.append("")
            for f in drift["missing_files"]:
                out.append(f"- `{f}` — SPEC 引用但版本內找不到此檔案")
            out.append("")
        if drift["missing_vars"]:
            out.append(f"### 可能失蹤的變數/結構 ({len(drift['missing_vars'])})")
            out.append("")
            out.append("> 注意：變數檢查為寬鬆比對，可能有誤報。")
            for v in drift["missing_vars"][:20]:
                out.append(f"- `{v}`")
            if len(drift["missing_vars"]) > 20:
                out.append(f"- ... 另 {len(drift['missing_vars']) - 20} 個")
            out.append("")

    out.append("## 摘要")
    out.append("")
    out.append(f"- 總 drift 數：{total_drift}")
    if total_drift == 0:
        out.append("- 狀態：[OK] 所有 SPEC 與目標版本同步")
    else:
        out.append("- 狀態：[警告] 有 SPEC 過時，請更新")
    return out


def main() -> int:
    parser = argparse.ArgumentParser(description="HT9045 SPEC drift 偵測")
    parser.add_argument("--version", default="V899", help="目標版本（預設 V899）")
    parser.add_argument("--spec", help="只檢查指定 SPEC 檔案路徑")
    parser.add_argument("--out", help="輸出 markdown 檔（預設 stdout）")
    args = parser.parse_args()

    version_dir = resolve_version_dir(args.version)

    if args.spec:
        sp = Path(args.spec)
        if not sp.is_absolute():
            sp = WORKSPACE / args.spec
        spec_files = [sp]
    else:
        spec_files = sorted((SPEC_DIR / "modules").glob("*.spec.md")) + \
                     sorted((SPEC_DIR / "features").glob("*.spec.md"))

    lines = build_report(version_dir, spec_files)
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
