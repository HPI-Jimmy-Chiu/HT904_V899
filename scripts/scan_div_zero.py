# -*- coding: cp950 -*-
"""
scan_div_zero.py — HT9045 除法風險快速掃描

用途：
    輕量版除以零掃描（Phase 1）。列出所有除法位置與除數變數，並標記是否
    在前後 20 行內看到保護（如 `if (var != 0)`）。

    這是 search-division skill 的「快速版」，跑得快、用於 build 前自檢。
    若要做完整四階段（過濾常數除數 + 自動替換成 ChangeToFloatNonPcnt），
    請執行完整的 search-division skill 流程。

用法：
    python scripts/scan_div_zero.py [<目標資料夾>] [--show-protected]
    python scripts/scan_div_zero.py --top 30

預設目標：HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422
"""
from __future__ import annotations
import argparse
import re
import sys
from collections import Counter
from pathlib import Path
from typing import List, Tuple

WORKSPACE = Path(__file__).resolve().parent.parent
DEFAULT_TARGET = "HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422"
ENCODING = "cp950"

# 排除字串、行尾註解後抓除法
DIV_RE = re.compile(r"(?<=[)\]\w])\s*/\s*(?![*/=])([A-Za-z_]\w*)")

CPP_KEYWORDS = {
    "double", "float", "int", "long", "short", "char", "void", "bool",
    "unsigned", "signed", "const", "static", "class", "struct", "enum",
    "return", "if", "else", "for", "while", "do", "switch", "case",
    "break", "continue", "goto", "typedef", "sizeof", "new", "delete",
    "try", "catch", "throw", "this", "virtual", "public", "private",
    "protected", "template", "typename", "namespace", "using", "inline",
    "extern", "register", "volatile", "mutable", "explicit", "friend",
    "operator", "and", "or", "not", "xor", "auto",
}

CONSTANT_DIVISORS = {
    "2", "3", "4", "5", "8", "10", "16", "32", "60", "64",
    "100", "128", "255", "256", "360", "512", "1000", "1024",
    "3600", "4096", "10000", "65535", "100000", "1000000",
}

PROTECTION_PATTERNS = [
    r"if\s*\(\s*{var}\s*[!><=]+\s*0",
    r"\(\s*{var}\s*==\s*0\s*\)\s*\?",
    r"ChangeToFloat",
    r"ChangeToPercentage",
    r"{var}\s*=\s*[1-9]\d*\b",
]


def strip_strings_and_comments(line: str) -> str:
    line = re.sub(r'"[^"]*"', '""', line)
    line = re.sub(r"'[^']*'", "''", line)
    if "//" in line:
        line = line.split("//", 1)[0]
    return line


def is_protected(divisor: str, context: str) -> bool:
    var = re.escape(divisor)
    for pat in PROTECTION_PATTERNS:
        if re.search(pat.format(var=var), context):
            return True
    return False


def scan_file(path: Path) -> List[Tuple[int, str, str, bool]]:
    """回傳 [(行號, 除數, 程式碼, 是否受保護)]。"""
    results: List[Tuple[int, str, str, bool]] = []
    try:
        text = path.read_text(encoding=ENCODING, errors="replace")
    except OSError:
        return results
    lines = text.splitlines()
    for idx, raw in enumerate(lines):
        stripped = raw.strip()
        if not stripped or stripped.startswith(("/*", "*", "#")):
            continue
        scrubbed = strip_strings_and_comments(raw)
        for m in DIV_RE.finditer(scrubbed):
            divisor = m.group(1)
            if divisor in CPP_KEYWORDS or divisor in CONSTANT_DIVISORS:
                continue
            if "ChangeToFloat" in raw or "ChangeToPercentage" in raw:
                continue
            ctx_start = max(0, idx - 20)
            ctx_end = min(len(lines), idx + 21)
            context = "\n".join(lines[ctx_start:ctx_end])
            results.append((idx + 1, divisor, raw.strip(), is_protected(divisor, context)))
    return results


def main() -> int:
    parser = argparse.ArgumentParser(description="HT9045 除法風險快速掃描")
    parser.add_argument("target", nargs="?", default=DEFAULT_TARGET,
                        help=f"目標資料夾（預設 {DEFAULT_TARGET}）")
    parser.add_argument("--show-protected", action="store_true",
                        help="連同已保護的除法一起列出")
    parser.add_argument("--top", type=int, default=20,
                        help="顯示 TOP N 高風險除數變數（預設 20）")
    args = parser.parse_args()

    target = WORKSPACE / args.target
    if not target.is_dir():
        sys.exit(f"[ERROR] 目標資料夾不存在：{target}")

    print(f"# 除法風險掃描\n")
    print(f"目標：`{target.relative_to(WORKSPACE).as_posix()}`\n")

    risky: List[Tuple[Path, int, str, str]] = []
    protected: List[Tuple[Path, int, str, str]] = []
    divisor_counter: Counter = Counter()

    for p in target.rglob("*.cpp"):
        parts_lower = [s.lower() for s in p.parts]
        if any(seg in parts_lower for seg in (".svn", "backup", "obj")):
            continue
        for line_no, divisor, code, prot in scan_file(p):
            divisor_counter[divisor] += 1
            entry = (p, line_no, divisor, code)
            (protected if prot else risky).append(entry)

    print(f"- 總候選除法：{len(risky) + len(protected)}")
    print(f"- ?? 未保護（高風險）：{len(risky)}")
    print(f"- ? 已保護：{len(protected)}\n")

    print(f"## TOP {args.top} 高頻除數變數\n")
    print("| 除數變數 | 出現次數 |")
    print("|---------|---------|")
    for var, cnt in divisor_counter.most_common(args.top):
        print(f"| `{var}` | {cnt} |")
    print()

    print(f"## ?? 未保護除法（前 100 筆）\n")
    if risky:
        print("| 檔案 | 行 | 除數 | 程式碼 |")
        print("|------|-----|------|--------|")
        for p, line_no, divisor, code in risky[:100]:
            rel = p.relative_to(WORKSPACE).as_posix()
            safe_code = code.replace("|", "\\|")[:80]
            print(f"| `{rel}` | {line_no} | `{divisor}` | `{safe_code}` |")
        if len(risky) > 100:
            print(f"\n... 另 {len(risky) - 100} 筆未列出")
    else:
        print("? 沒有發現未保護除法")

    if args.show_protected and protected:
        print(f"\n## ? 已保護除法（前 50 筆）\n")
        print("| 檔案 | 行 | 除數 |")
        print("|------|-----|------|")
        for p, line_no, divisor, _ in protected[:50]:
            rel = p.relative_to(WORKSPACE).as_posix()
            print(f"| `{rel}` | {line_no} | `{divisor}` |")

    return 0 if not risky else 2


if __name__ == "__main__":
    raise SystemExit(main())
