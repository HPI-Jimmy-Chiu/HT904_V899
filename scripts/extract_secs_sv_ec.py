# -*- coding: utf-8 -*-
"""Extract HT9045 SECS/GEM SV and EC registrations into CSV reference tables.

Parses SetSVDataPointer(...) / SetECDataPointer(...) calls from the V899
SECSGEM source files and emits two CSV lookup tables (utf-8-sig so Excel
opens Chinese comments correctly).

SV signature:
    SetSVDataPointer(SVID, HType.TYPE, "Name", "Unit", &Ptr, "Comment");
EC signature:
    SetECDataPointer(ECID, HType.TYPE, "Name", "Unit", Ptr, "Max", "Min", "Default", "Comment");

Usage:
    python scripts/extract_secs_sv_ec.py
    python scripts/extract_secs_sv_ec.py --code-folder HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422
"""
import argparse
import csv
import os
import re
import sys

DEFAULT_CODE_FOLDER = "HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422"
SRC_ENCODING = "cp950"  # BIG5 source files


def read_source(path):
    with open(path, "r", encoding=SRC_ENCODING, errors="replace") as fh:
        return fh.read()


def find_calls(text, func_name):
    """Yield the raw argument string of every func_name(...) call.

    Handles multi-line calls by tracking parenthesis depth, and ignores
    parentheses/quotes that appear inside string literals.
    """
    calls = []
    needle = func_name + "("
    idx = 0
    while True:
        start = text.find(needle, idx)
        if start == -1:
            break
        i = start + len(needle)
        depth = 1
        in_str = False
        buf = []
        while i < len(text) and depth > 0:
            ch = text[i]
            if in_str:
                if ch == '\\':
                    buf.append(ch)
                    i += 1
                    if i < len(text):
                        buf.append(text[i])
                        i += 1
                    continue
                if ch == '"':
                    in_str = False
                buf.append(ch)
            else:
                if ch == '"':
                    in_str = True
                    buf.append(ch)
                elif ch == '(':
                    depth += 1
                    buf.append(ch)
                elif ch == ')':
                    depth -= 1
                    if depth == 0:
                        break
                    buf.append(ch)
                else:
                    buf.append(ch)
            i += 1
        calls.append("".join(buf))
        idx = i + 1
    return calls


def split_args(arg_str):
    """Split a call's argument string on top-level commas."""
    args = []
    depth = 0
    in_str = False
    buf = []
    i = 0
    while i < len(arg_str):
        ch = arg_str[i]
        if in_str:
            if ch == '\\':
                buf.append(ch)
                i += 1
                if i < len(arg_str):
                    buf.append(arg_str[i])
                i += 1
                continue
            if ch == '"':
                in_str = False
            buf.append(ch)
        else:
            if ch == '"':
                in_str = True
                buf.append(ch)
            elif ch in "([{":
                depth += 1
                buf.append(ch)
            elif ch in ")]}":
                depth -= 1
                buf.append(ch)
            elif ch == ',' and depth == 0:
                args.append("".join(buf).strip())
                buf = []
            else:
                buf.append(ch)
        i += 1
    if "".join(buf).strip():
        args.append("".join(buf).strip())
    return args


def unquote(token):
    token = token.strip()
    if len(token) >= 2 and token[0] == '"' and token[-1] == '"':
        return token[1:-1]
    return token


def clean_type(token):
    return token.strip().replace("HType.", "")


def parse_sv(code_dir):
    path = os.path.join(code_dir, "SECSGEM", "uHGemHT9045_SV.cpp")
    text = read_source(path)
    rows = []
    for raw in find_calls(text, "SetSVDataPointer"):
        a = split_args(raw)
        if len(a) < 6:
            continue
        rows.append({
            "SVID": a[0].strip(),
            "Type": clean_type(a[1]),
            "Name": unquote(a[2]),
            "Unit": unquote(a[3]),
            "Pointer": a[4].strip(),
            "Comment": unquote(a[5]),
        })
    return rows


def parse_ec(code_dir):
    path = os.path.join(code_dir, "SECSGEM", "uHGemHT9045_EC.cpp")
    text = read_source(path)
    rows = []
    for raw in find_calls(text, "SetECDataPointer"):
        a = split_args(raw)
        if len(a) < 9:
            continue
        rows.append({
            "ECID": a[0].strip(),
            "Type": clean_type(a[1]),
            "Name": unquote(a[2]),
            "Unit": unquote(a[3]),
            "Pointer": a[4].strip(),
            "Max": unquote(a[5]),
            "Min": unquote(a[6]),
            "Default": unquote(a[7]),
            "Comment": unquote(a[8]),
        })
    return rows


def write_csv(rows, fieldnames, out_path):
    os.makedirs(os.path.dirname(out_path), exist_ok=True)
    with open(out_path, "w", encoding="utf-8-sig", newline="") as fh:
        writer = csv.DictWriter(fh, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(rows)


def main():
    repo_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    parser = argparse.ArgumentParser(description="Extract SECS SV/EC reference tables.")
    parser.add_argument("--code-folder", default=DEFAULT_CODE_FOLDER,
                        help="Version source folder name under repo root.")
    parser.add_argument("--out-dir", default=None,
                        help="Output dir (default: skill references folder).")
    args = parser.parse_args()

    code_dir = os.path.join(repo_root, args.code_folder)
    if not os.path.isdir(code_dir):
        print("ERROR: code folder not found: %s" % code_dir)
        return 1

    out_dir = args.out_dir or os.path.join(
        repo_root, ".agents", "skills", "ht9045-secsgem", "references")

    sv_rows = parse_sv(code_dir)
    ec_rows = parse_ec(code_dir)

    sv_out = os.path.join(out_dir, "sv_table.csv")
    ec_out = os.path.join(out_dir, "ec_table.csv")
    write_csv(sv_rows, ["SVID", "Type", "Name", "Unit", "Pointer", "Comment"], sv_out)
    write_csv(ec_rows, ["ECID", "Type", "Name", "Unit", "Pointer", "Max", "Min", "Default", "Comment"], ec_out)

    print("SV entries: %d -> %s" % (len(sv_rows), sv_out))
    print("EC entries: %d -> %s" % (len(ec_rows), ec_out))
    return 0


if __name__ == "__main__":
    sys.exit(main())
