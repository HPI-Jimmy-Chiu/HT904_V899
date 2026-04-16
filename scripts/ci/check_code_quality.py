#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""HT9045 CI - Phase 2 Code Quality Checker
Checks: Big5 encoding integrity, AI annotation format, division-by-zero risk.
Usage: python check_code_quality.py <SRC_DIR>
"""

import re
import sys
from pathlib import Path

AI_COMMENT_PATTERN = re.compile(
    r'//AI\([^)]+\)\s+\d{8}[\s:]'
)


def check_big5_integrity(filepath):
    issues = []
    try:
        raw = filepath.read_bytes()
        raw.decode('big5', errors='strict')
    except UnicodeDecodeError as e:
        issues.append(
            f'  WARN: Big5 decode failed at byte offset {e.start}-{e.end}'
        )
    return issues


def check_ai_annotation(filepath, lines):
    issues = []
    for i, line in enumerate(lines, 1):
        if '//AI(' in line and not AI_COMMENT_PATTERN.search(line):
            stripped = line.strip()
            if stripped.startswith('#'):
                continue
            if '//AIO' in stripped:
                continue
            issues.append(
                f'  WARN [L{i}]: AI annotation format violation: {stripped[:80]}'
            )
    return issues


def check_division_risk(filepath, lines):
    issues = []
    in_block_comment = False
    for i, line in enumerate(lines, 1):
        stripped = line.strip()
        if '/*' in stripped:
            in_block_comment = True
        if '*/' in stripped:
            in_block_comment = False
            continue
        if in_block_comment:
            continue
        if stripped.startswith('//') or stripped.startswith('#'):
            continue
        m = re.search(r'\b([A-Za-z_]\w*)\s*/\s*([A-Za-z_]\w*)\b', stripped)
        if m:
            if any(kw in stripped for kw in ['#include', '://', '"']):
                continue
            divisor = m.group(2)
            issues.append(
                f'  INFO [L{i}]: Division risk ({m.group(1)}/{divisor}): {stripped[:80]}'
            )
    return issues[:10]


def main():
    if len(sys.argv) < 2:
        print('Usage: check_code_quality.py <SRC_DIR>')
        sys.exit(1)

    src_dir = Path(sys.argv[1])
    if not src_dir.is_dir():
        print(f'ERR: Directory not found: {src_dir}')
        sys.exit(1)

    error_count = 0
    warn_count = 0
    info_count = 0

    source_files = sorted(
        p for p in src_dir.rglob('*')
        if p.suffix.lower() in {'.cpp', '.h', '.hpp', '.c'}
        and '.svn' not in str(p)
    )

    print(f'[Code Quality] Scanning {len(source_files)} source files...')
    print(f'[Code Quality] Directory: {src_dir}')
    print()

    for fpath in source_files:
        rel = fpath.relative_to(src_dir).as_posix()
        file_issues = []

        big5_issues = check_big5_integrity(fpath)
        file_issues.extend(big5_issues)

        try:
            raw = fpath.read_bytes()
            text = raw.decode('big5', errors='replace')
            lines = text.splitlines()
        except Exception:
            file_issues.append('  ERR: Cannot read file')
            error_count += 1
            continue

        ai_issues = check_ai_annotation(fpath, lines)
        file_issues.extend(ai_issues)

        div_issues = check_division_risk(fpath, lines)
        file_issues.extend(div_issues)

        if file_issues:
            print(f'[{rel}]')
            for issue in file_issues:
                print(issue)
                trimmed = issue.strip()
                if trimmed.startswith('ERR'):
                    error_count += 1
                elif trimmed.startswith('WARN'):
                    warn_count += 1
                else:
                    info_count += 1
            print()

    print('=' * 60)
    print('[Code Quality] Scan complete')
    print(f'  Errors:   {error_count}')
    print(f'  Warnings: {warn_count}')
    print(f'  Info:     {info_count}')
    print('=' * 60)

    if error_count > 0:
        sys.exit(1)
    sys.exit(0)


if __name__ == '__main__':
    main()
