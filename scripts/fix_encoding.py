# -*- coding: utf-8 -*-
"""
Fix V899_20260330 encoding damage.

Problem: 19 files were accidentally saved as UTF-8, corrupting BIG5 Chinese
comments (U+FFFD replacement characters).

Strategy (diff-based line alignment):
1. Read damaged file (V330) as UTF-8
2. Read reference file (V407) as BIG5
3. Use difflib.SequenceMatcher to align lines between V330 and V407
4. For each aligned pair where V330 line has U+FFFD, replace with V407 line
5. Keep V330-only additions (AI annotations, new code) as-is
6. Save result as BIG5

Author: AI(ht9045-v899) 20260407
"""

import os
import sys
import shutil
import re
import difflib

# --- Configuration ---
BASE_330 = r"d:\HT9045\HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260330"
BASE_407 = r"d:\HT9045\HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260407"
BASE_898 = r"d:\HT9045\HT9011UC_Code_V3.33.898.0_20260313_Jimmy_20260323"

FFFD = "\ufffd"

DAMAGED_FILES = [
    "CosFunction.cpp",
    "Command.cpp",
    "cinitial.cpp",
    "ainarm9045_2x8_32.cpp",
    "CosFunction.h",
    "ainarm9045_2x4_16.cpp",
    "ainarm9045_2x4_8.cpp",
    "ainarm9045_2x4_4.cpp",
    "ainarm9045_2x2_8_Hot.cpp",
    "ainarm9045_1x4_8_Hot.cpp",
    "ainarm9045_1x3_2_14.cpp",
    "ainarm9045_2x2_4.cpp",
    "ainarm9045_1x2_4_Hot.cpp",
    "ainarm9045_1x4_2.cpp",
    "ainarm9045_1x1_1.cpp",
    "ainarm9045_2x2_4_12.cpp",
    "cShowBinSelect.cpp",
    "uHGemHT9045_SV.cpp",
    "cShowBinSelect.h",
]

# Pre-compiled regex: strip all non-ASCII
RE_NON_ASCII = re.compile(r"[^\x00-\x7f]")


def find_file(base_dir, filename):
    """Find a file by name in a directory tree, skipping .svn."""
    for root, dirs, files in os.walk(base_dir):
        dirs[:] = [d for d in dirs if d != ".svn"]
        if filename in files:
            return os.path.join(root, filename)
    return None


def normalize_for_diff(line):
    """Normalize line for difflib comparison: strip non-ASCII to make
    lines with garbled vs correct Chinese match as 'equal'."""
    return RE_NON_ASCII.sub("", line)


def fix_file(damaged_path, ref_paths, dry_run=False):
    """
    Fix encoding using diff-based line alignment + multi-reference fallback.
    ref_paths: list of (path, label) tuples, tried in order.
    Returns (fixed_count, total_fffd_lines, unmatched_list, ref_label).
    """
    # Read damaged file as UTF-8
    with open(damaged_path, "r", encoding="utf-8") as f:
        damaged_lines = f.readlines()

    # Load all references
    all_refs = []
    for rpath, rlabel in ref_paths:
        with open(rpath, "r", encoding="big5", errors="replace") as f:
            rlines = f.readlines()
        all_refs.append((rlines, rlabel))

    re_code_key = re.compile(r"^(.*?)(?://|$)")

    # Build resolution map: damaged_idx -> (ref_line_text, ref_label)
    resolution = {}

    for ref_lines, rlabel in all_refs:
        # Phase 1: diff-based alignment
        norm_damaged = [normalize_for_diff(l) for l in damaged_lines]
        norm_ref = [normalize_for_diff(l) for l in ref_lines]
        sm = difflib.SequenceMatcher(None, norm_damaged, norm_ref, autojunk=False)

        for tag, i1, i2, j1, j2 in sm.get_opcodes():
            if tag == "equal":
                for di, ri in zip(range(i1, i2), range(j1, j2)):
                    if di not in resolution:
                        resolution[di] = (ref_lines[ri], rlabel)
            elif tag == "replace" and (i2 - i1) == (j2 - j1):
                for di, ri in zip(range(i1, i2), range(j1, j2)):
                    if di not in resolution:
                        resolution[di] = (ref_lines[ri], rlabel)

        # Phase 2: code-key fallback
        ref_code_map = {}
        ref_norm_map = {}
        for idx, rl in enumerate(ref_lines):
            m = re_code_key.match(rl)
            key = m.group(1).strip() if m else rl.strip()
            if key:
                ref_code_map.setdefault(key, []).append((idx, rl))
            norm_key = RE_NON_ASCII.sub("", rl).replace("\\", "").strip()
            if norm_key:
                ref_norm_map.setdefault(norm_key, []).append((idx, rl))

        for i, line in enumerate(damaged_lines):
            if i in resolution or FFFD not in line:
                continue
            m = re_code_key.match(line)
            code_key = m.group(1).strip() if m else ""
            if code_key and code_key in ref_code_map:
                best = min(ref_code_map[code_key], key=lambda x: abs(x[0] - i))
                resolution[i] = (best[1], rlabel)
            else:
                norm_key = RE_NON_ASCII.sub("", line).replace("\\", "").strip()
                if norm_key and norm_key in ref_norm_map:
                    best = min(ref_norm_map[norm_key], key=lambda x: abs(x[0] - i))
                    resolution[i] = (best[1], rlabel)

    # Apply resolutions
    fixed_count = 0
    fffd_line_count = 0
    unmatched = []
    result_lines = []
    ref_labels_used = set()

    for i, line in enumerate(damaged_lines):
        if FFFD not in line:
            result_lines.append(line)
            continue

        fffd_line_count += 1

        if i in resolution:
            ref_line, rlabel = resolution[i]
            ref_labels_used.add(rlabel)
            if line.endswith("\r\n"):
                ref_line = ref_line.rstrip("\r\n") + "\r\n"
            elif line.endswith("\n"):
                ref_line = ref_line.rstrip("\r\n") + "\n"
            result_lines.append(ref_line)
            fixed_count += 1
        else:
            result_lines.append(line)
            unmatched.append((i + 1, line.strip()[:80]))

    if not dry_run and fixed_count > 0:
        backup_path = damaged_path + ".utf8bak"
        if not os.path.exists(backup_path):
            shutil.copy2(damaged_path, backup_path)
        content = "".join(result_lines)
        with open(damaged_path, "w", encoding="big5", errors="replace") as f:
            f.write(content)

    ref_label = "+".join(sorted(ref_labels_used)) if ref_labels_used else "none"

    return fixed_count, fffd_line_count, unmatched


def main():
    dry_run = "--dry-run" in sys.argv
    if dry_run:
        print("=== DRY RUN MODE (no files will be modified) ===\n")

    total_fixed = 0
    total_fffd = 0
    total_unmatched = 0

    for fname in DAMAGED_FILES:
        damaged_path = find_file(BASE_330, fname)
        if not damaged_path:
            print(f"SKIP {fname}: not found in V330")
            continue

        # Build reference list: V407 (primary) + V898 (fallback)
        ref_paths = []
        p407 = find_file(BASE_407, fname)
        if p407:
            ref_paths.append((p407, "V407"))
        p898 = find_file(BASE_898, fname)
        if p898:
            ref_paths.append((p898, "V898"))
        if not ref_paths:
            print(f"SKIP {fname}: no reference file found")
            continue

        fixed, fffd_lines, unmatched = fix_file(damaged_path, ref_paths, dry_run)
        total_fixed += fixed
        total_fffd += fffd_lines
        total_unmatched += len(unmatched)

        status = "OK" if len(unmatched) == 0 else f"WARN({len(unmatched)} unmatched)"
        print(f"  {fname}: {fixed}/{fffd_lines} lines fixed [{status}]")

        if unmatched:
            for ln, text in unmatched[:3]:
                print(f"    L{ln}: {text}")
            if len(unmatched) > 3:
                print(f"    ... and {len(unmatched)-3} more")

    print(f"\n{'='*60}")
    print(f"Total: {total_fixed} lines fixed, {total_fffd} FFFD lines found, {total_unmatched} unmatched")
    if dry_run:
        print("(dry run - no files modified)")
    else:
        print("Backups saved as *.utf8bak")


if __name__ == "__main__":
    main()
