# -*- coding: utf-8 -*-
"""Replace INSTALL_DOUBLE_EP==<digit> with named constants (byte-level, Big5-safe)."""
from pathlib import Path
import re

ROOT = Path(r"d:\HT9045\HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422")
MAP = {
    b"0": b"DOUBLE_EP_NONE",
    b"1": b"DOUBLE_EP_NORMAL",
    b"2": b"DOUBLE_EP_INDIVIAL",
    b"3": b"DOUBLE_EP_MULTI",
}
PAT = re.compile(rb"INSTALL_DOUBLE_EP\s*==\s*([0-3])\b")

def repl(m):
    return b"INSTALL_DOUBLE_EP==" + MAP[m.group(1)]

total = 0
files_changed = 0
for p in ROOT.rglob("*"):
    if p.suffix.lower() not in (".cpp", ".h", ".hpp"):
        continue
    if ".svn" in p.parts:
        continue
    data = p.read_bytes()
    new, n = PAT.subn(repl, data)
    if n:
        p.write_bytes(new)
        files_changed += 1
        total += n
        print(f"{p.relative_to(ROOT)}: {n}")
print(f"---\nfiles={files_changed} replacements={total}")
