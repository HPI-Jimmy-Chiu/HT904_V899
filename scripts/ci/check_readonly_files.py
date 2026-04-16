#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
HT9045 CI - Readonly File Guard
Checks Git diff against a list of readonly driver/definition files.

Usage: python check_readonly_files.py <SRC_DIR>
"""

import subprocess
import sys

READONLY_FILES = {
    'CCLink/Mdfunc.h',
    'EtherCAT/AdvMotApi.h', 'EtherCAT/AdvMotDev.h',
    'EtherCAT/AdvMotDrv.h', 'EtherCAT/AdvMotErr.h',
    'EtherCAT/AdvMotPropID.h',
    'Motor/CMNETError.h', 'Motor/CMNet.h',
    'Motor/CSmc.h', 'Motor/CSmcdef.h',
    'Motor/DMCCOM.H', 'Motor/dmcdrc.h', 'Motor/dmcdrco.h',
    'Motor/DMCMLIB.H', 'Motor/Mc88x1p_DLL.h',
    'Motor/mn200.h', 'Motor/PCI_L112.h',
    'Motor/PCI_L122_Err.h', 'Motor/PCI_L122.H', 'Motor/PCI_L132.H',
    'MR/DMT.h',
    'Public/ADSMOD.h', 'Public/HTKeyPro.h',
    'ADAMTCP.h', 'sqlite3.h',
}


def get_changed_files():
    try:
        result = subprocess.run(
            ['git', 'diff', '--name-only', 'HEAD~1'],
            capture_output=True, text=True, check=True,
        )
        return result.stdout.strip().splitlines()
    except subprocess.CalledProcessError:
        return []


def main():
    if len(sys.argv) < 2:
        print('Usage: check_readonly_files.py <SRC_DIR>')
        sys.exit(1)

    src_dir = sys.argv[1].replace('\\', '/')
    changed = get_changed_files()

    violations = []
    for f in changed:
        normalized = f.replace('\\', '/')
        if not normalized.startswith(src_dir):
            continue
        rel = normalized[len(src_dir):].lstrip('/')
        if rel in READONLY_FILES:
            violations.append(rel)

    if violations:
        print('ERROR: The following readonly files were modified:')
        for v in violations:
            print(f'  - {v}')
        sys.exit(1)
    else:
        print('[Readonly Check] PASSED - No readonly files modified')
        sys.exit(0)


if __name__ == '__main__':
    main()
