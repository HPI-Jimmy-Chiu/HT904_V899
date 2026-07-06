# -*- coding: utf-8 -*-
import glob, os

targets = {
    "auto_clean_machine": b"\xa6\xdb\xb0\xca\xb2\x4d\xbe\xf7",
    "auto_clean_machine_func": b"\xa6\xdb\xb0\xca\xb2\x4d\xbe\xf7\xa5\x5c\xaf\xe0",
}

exts = (".dfm", ".cpp", ".h", ".hpp")
base = r"d:\HT9045"
patterns = [
    os.path.join(base, "HT9011UC_Code_*", "**", "*.*"),
    os.path.join(base, "HT9046LS_Code_*", "**", "*.*"),
]

for patt in patterns:
    for f in glob.glob(patt, recursive=True):
        if not f.lower().endswith(exts):
            continue
        try:
            data = open(f, "rb").read()
        except:
            continue
        for name, t in targets.items():
            if t in data:
                lines = data.split(b"\n")
                for i, line in enumerate(lines):
                    if t in line:
                        decoded = line.decode("big5", "replace").strip()[:150]
                        print(f"[{name}] {f}:{i+1}: {decoded}")

print("--- DONE ---")
