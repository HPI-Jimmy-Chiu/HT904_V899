# -*- coding: utf-8 -*-
"""MG 戰役：抽出某檔在 V899 期的 final-state diff hunks（位元組級，cp950 安全預覽）。

用法：
  python tools/port_tools/extract_hunks.py <relpath> [--contains 20260612] [--contains CASE-...]

- relpath 相對 V899 樹根（例 CosFunction.cpp、AutoClean\\uCleaning.cpp）
- diff 範圍固定 bb69c60..b515ed5（V899 pristine → 在製收斂）
- --contains 可重複：只印「新增行含任一關鍵位元組串」的 hunk（cp950 編碼後比對）
- 輸出每個 hunk：@@ 標頭（含 V899 新檔行號）＋逐行（-/+/空格），非 ASCII 以 cp950 解碼預覽
"""
import subprocess, sys, os

ROOT = r"D:\HT9045"
V899 = "HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422"
RANGE = "bb69c60..b515ed5"


def main():
    args = sys.argv[1:]
    contains = []
    rel = None
    i = 0
    while i < len(args):
        if args[i] == "--contains":
            contains.append(args[i + 1].encode("cp950", errors="ignore")); i += 2
        else:
            rel = args[i]; i += 1
    if not rel:
        print("usage: extract_hunks.py <relpath> [--contains STR]..."); sys.exit(2)
    path = "%s/%s" % (V899, rel.replace("\\", "/"))
    out = subprocess.run(["git", "diff", "--no-color", RANGE, "--", path],
                         cwd=ROOT, capture_output=True).stdout
    if not out.strip():
        print("(no diff for %s in %s)" % (rel, RANGE)); return
    hunks, cur = [], None
    for line in out.split(b"\n"):
        if line.startswith(b"@@"):
            cur = [line]; hunks.append(cur)
        elif cur is not None:
            cur.append(line)
    kept = 0
    for h in hunks:
        if contains:
            added = b"\n".join(l[1:] for l in h if l.startswith(b"+"))
            if not any(c in added for c in contains):
                continue
        kept += 1
        for line in h:
            sys.stdout.write(line.decode("cp950", errors="backslashreplace") + "\n")
        sys.stdout.write("\n")
    sys.stdout.write("== %d/%d hunks shown (%s %s) ==\n" % (kept, len(hunks), rel, RANGE))


if __name__ == "__main__":
    main()
