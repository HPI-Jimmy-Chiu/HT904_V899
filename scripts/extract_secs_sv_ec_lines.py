# -*- coding: utf-8 -*-
"""Read-only: extract HT9045 V899 SECS SV/EC registrations WITH line numbers.

Adds a Line / SourceRef column to the SV and EC reference tables so each
SVID/ECID can be traced back to file#line. Does NOT modify any machine
control source; only reads the two SECSGEM registration files.

Usage:
    python scripts/extract_secs_sv_ec_lines.py
"""
import csv
import os
import re

CODE_FOLDER = "HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422"
SRC_ENCODING = "cp950"  # BIG5 source files
REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT_DIR = os.path.join(REPO_ROOT, "scripts", "_secs_id_export")


def find_calls_with_line(text, func_name):
    """Yield (line_number_1based, raw_arg_string) for every func_name(...) call."""
    out = []
    needle = func_name + "("
    idx = 0
    while True:
        start = text.find(needle, idx)
        if start == -1:
            break
        line_no = text.count("\n", 0, start) + 1
        i = start + len(needle)
        depth = 1
        in_str = False
        buf = []
        while i < len(text) and depth > 0:
            ch = text[i]
            if in_str:
                if ch == '\\':
                    buf.append(ch); i += 1
                    if i < len(text):
                        buf.append(text[i]); i += 1
                    continue
                if ch == '"':
                    in_str = False
                buf.append(ch)
            else:
                if ch == '"':
                    in_str = True; buf.append(ch)
                elif ch == '(':
                    depth += 1; buf.append(ch)
                elif ch == ')':
                    depth -= 1
                    if depth == 0:
                        break
                    buf.append(ch)
                else:
                    buf.append(ch)
            i += 1
        out.append((line_no, "".join(buf)))
        idx = i + 1
    return out


def split_args(s):
    args, depth, in_str, buf, i = [], 0, False, [], 0
    while i < len(s):
        ch = s[i]
        if in_str:
            if ch == '\\':
                buf.append(ch); i += 1
                if i < len(s):
                    buf.append(s[i])
                i += 1; continue
            if ch == '"':
                in_str = False
            buf.append(ch)
        else:
            if ch == '"':
                in_str = True; buf.append(ch)
            elif ch in "([{":
                depth += 1; buf.append(ch)
            elif ch in ")]}":
                depth -= 1; buf.append(ch)
            elif ch == ',' and depth == 0:
                args.append("".join(buf).strip()); buf = []
            else:
                buf.append(ch)
        i += 1
    if "".join(buf).strip():
        args.append("".join(buf).strip())
    return args


def unquote(t):
    t = t.strip()
    if len(t) >= 2 and t[0] == '"' and t[-1] == '"':
        return t[1:-1]
    return t


def clean_type(t):
    return t.strip().replace("HType.", "")


def to_int(idtok):
    m = re.match(r"^\s*(-?\d+)", idtok)
    return int(m.group(1)) if m else None


def read_src(rel):
    path = os.path.join(REPO_ROOT, CODE_FOLDER, "SECSGEM", rel)
    with open(path, "r", encoding=SRC_ENCODING, errors="replace") as fh:
        return fh.read()


def parse_sv():
    text = read_src("uHGemHT9045_SV.cpp")
    rows = []
    for ln, raw in find_calls_with_line(text, "SetSVDataPointer"):
        a = split_args(raw)
        if len(a) < 6:
            continue
        rows.append({
            "SVID": to_int(a[0]),
            "Type": clean_type(a[1]),
            "Name": unquote(a[2]),
            "Unit": unquote(a[3]),
            "Pointer": a[4].strip(),
            "Comment": unquote(a[5]),
            "Line": ln,
            "SourceRef": "SECSGEM/uHGemHT9045_SV.cpp#L%d" % ln,
        })
    return rows


def parse_ec():
    text = read_src("uHGemHT9045_EC.cpp")
    rows = []
    for ln, raw in find_calls_with_line(text, "SetECDataPointer"):
        a = split_args(raw)
        if len(a) < 9:
            continue
        rows.append({
            "ECID": to_int(a[0]),
            "Type": clean_type(a[1]),
            "Name": unquote(a[2]),
            "Unit": unquote(a[3]),
            "Pointer": a[4].strip(),
            "Max": unquote(a[5]),
            "Min": unquote(a[6]),
            "Default": unquote(a[7]),
            "Comment": unquote(a[8]),
            "Line": ln,
            "SourceRef": "SECSGEM/uHGemHT9045_EC.cpp#L%d" % ln,
        })
    return rows


def write_csv(rows, fields, name):
    os.makedirs(OUT_DIR, exist_ok=True)
    p = os.path.join(OUT_DIR, name)
    with open(p, "w", encoding="utf-8-sig", newline="") as fh:
        w = csv.DictWriter(fh, fieldnames=fields)
        w.writeheader()
        w.writerows(rows)
    return p


def group_summary(rows, key):
    """Bucket IDs into human ranges and report count + min/max line."""
    buckets = {}
    for r in rows:
        v = r[key]
        if v is None:
            continue
        b = (v // 100) * 100
        buckets.setdefault(b, []).append(r)
    print("\n=== %s group summary (bucket of 100) ===" % key)
    print("bucket_start, count, id_min, id_max, line_min, line_max")
    for b in sorted(buckets):
        g = buckets[b]
        ids = [r[key] for r in g]
        lns = [r["Line"] for r in g]
        print("%6d, %4d, %6d, %6d, L%d, L%d" % (
            b, len(g), min(ids), max(ids), min(lns), max(lns)))


def main():
    sv = parse_sv()
    ec = parse_ec()
    sv.sort(key=lambda r: (r["SVID"] if r["SVID"] is not None else 1 << 30, r["Line"]))
    ec.sort(key=lambda r: (r["ECID"] if r["ECID"] is not None else 1 << 30, r["Line"]))

    sv_path = write_csv(sv, ["SVID", "Type", "Name", "Unit", "Pointer", "Comment", "Line", "SourceRef"], "sv_table_with_lines.csv")
    ec_path = write_csv(ec, ["ECID", "Type", "Name", "Unit", "Pointer", "Max", "Min", "Default", "Comment", "Line", "SourceRef"], "ec_table_with_lines.csv")

    print("SV count = %d -> %s" % (len(sv), sv_path))
    print("EC count = %d -> %s" % (len(ec), ec_path))

    # duplicate id detection
    def dups(rows, key):
        seen, d = {}, {}
        for r in rows:
            v = r[key]
            seen.setdefault(v, []).append(r["Line"])
        for v, lns in seen.items():
            if v is not None and len(lns) > 1:
                d[v] = lns
        return d

    sv_d = dups(sv, "SVID")
    ec_d = dups(ec, "ECID")
    print("\nSVID duplicates: %d -> %s" % (len(sv_d), sorted(sv_d.keys())))
    print("ECID duplicates: %d -> %s" % (len(ec_d), sorted(ec_d.keys())))

    group_summary(sv, "SVID")
    group_summary(ec, "ECID")


if __name__ == "__main__":
    raise SystemExit(main())
