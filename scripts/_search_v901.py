import os, glob, sys

base = r'd:\HT9045\HT9011UC_Code_V3.33.901.0_20260408_Jimmy_20260410'

def decode_line(bline):
    return bline.decode('big5', errors='replace').rstrip()

def search_all(base_dir, extensions, term_bytes, context=0):
    results = []
    for ext in extensions:
        for f in glob.glob(os.path.join(base_dir, '**', ext), recursive=True):
            if '.svn' in f:
                continue
            try:
                data = open(f, 'rb').read()
                if term_bytes not in data:
                    continue
                lines = data.split(b'\n')
                rel = os.path.relpath(f, base_dir)
                for i, line in enumerate(lines):
                    if term_bytes in line:
                        if context == 0:
                            results.append((rel, i+1, decode_line(line)))
                        else:
                            s = max(0, i - context)
                            e = min(len(lines), i + context + 1)
                            block = []
                            for j in range(s, e):
                                marker = '>>>' if j == i else '   '
                                block.append((marker, j+1, decode_line(lines[j])))
                            results.append((rel, i+1, block))
            except:
                pass
    return results

# === 1. SPIL_FOR_QLE ===
print('=' * 70)
print('1. SPIL_FOR_QLE definitions in .cpp/.h')
print('=' * 70)
for rel, ln, text in search_all(base, ['*.cpp', '*.h'], b'SPIL_FOR_QLE'):
    print(f'  {rel}:{ln}  {text[:250]}')

# === 2a. palAutoDeviceEjection in .h ===
print()
print('=' * 70)
print('2a. palAutoDeviceEjection in .h files')
print('=' * 70)
for rel, ln, text in search_all(base, ['*.h'], b'palAutoDeviceEjection'):
    print(f'  {rel}:{ln}  {text[:250]}')

# === 2b. palAutoDeviceEjection in .dfm with context ===
print()
print('=' * 70)
print('2b. palAutoDeviceEjection in .dfm files (panel block)')
print('=' * 70)
for f in glob.glob(os.path.join(base, '**', '*.dfm'), recursive=True):
    if '.svn' in f:
        continue
    try:
        data = open(f, 'rb').read()
        if b'palAutoDeviceEjection' not in data:
            continue
        lines = data.split(b'\n')
        rel = os.path.relpath(f, base)
        for i, line in enumerate(lines):
            if b'palAutoDeviceEjection' in line:
                s = max(0, i - 2)
                e = min(len(lines), i + 100)
                print(f'\n  --- {rel} (line {i+1}, showing {s+1}-{e}) ---')
                for j in range(s, e):
                    print(f'  {j+1:5d}: {decode_line(lines[j])[:250]}')
                break
    except:
        pass

# === 3. cShowBinSelect.cpp palAutoDeviceEjection->Visible ===
print()
print('=' * 70)
print('3. cShowBinSelect.cpp - palAutoDeviceEjection->Visible context')
print('=' * 70)
for rel, ln, block in search_all(base, ['cShowBinSelect.cpp', '**\\cShowBinSelect.cpp'],
                                  b'palAutoDeviceEjection', context=15):
    if isinstance(block, list):
        print(f'\n  --- {rel} (match at line {ln}) ---')
        for marker, lnum, text in block:
            print(f'  {marker} {lnum:5d}: {text[:250]}')

# also try direct file search
for f in glob.glob(os.path.join(base, '**', 'cShowBinSelect.cpp'), recursive=True):
    if '.svn' in f:
        continue
    try:
        data = open(f, 'rb').read()
        lines = data.split(b'\n')
        rel = os.path.relpath(f, base)
        for i, line in enumerate(lines):
            if b'palAutoDeviceEjection' in line:
                s = max(0, i - 15)
                e = min(len(lines), i + 16)
                print(f'\n  --- {rel} (match at line {i+1}, showing {s+1}-{e}) ---')
                for j in range(s, e):
                    marker = '>>>' if j == i else '   '
                    print(f'  {marker} {j+1:5d}: {decode_line(lines[j])[:250]}')
    except:
        pass

# === 4. Chinese captions inside palAutoDeviceEjection panel ===
print()
print('=' * 70)
print('4. Buttons/Captions inside palAutoDeviceEjection panel (DFM)')
print('=' * 70)
# Big5 bytes for Chinese terms
tui_pan = b'\xb0\xf0\xbd\xc6'       # tui pan
qing_ji = b'\xb2\xc0\xbe\xf9'       # qing ji
zi_dong_qing_ji = b'\xa6\xdb\xb0\xf1\xb2\xc0\xbe\xf9'  # zi dong qing ji
zi_dong_tui_pan = b'\xa6\xdb\xb0\xf1\xb0\xf0\xbd\xc6'  # zi dong tui pan

search_terms = [
    ('tui_pan', tui_pan),
    ('qing_ji', qing_ji),
    ('zi_dong_qing_ji', zi_dong_qing_ji),
    ('zi_dong_tui_pan', zi_dong_tui_pan),
    ('Caption', b'Caption'),
    ('CleanMachine', b'CleanMachine'),
    ('ReturnTray', b'ReturnTray'),
    ('Button', b'Button'),
    ('SpeedButton', b'SpeedButton'),
    ('GroupBox', b'GroupBox'),
]

for f in glob.glob(os.path.join(base, '**', '*.dfm'), recursive=True):
    if '.svn' in f:
        continue
    try:
        data = open(f, 'rb').read()
        if b'palAutoDeviceEjection' not in data:
            continue
        lines = data.split(b'\n')
        rel = os.path.relpath(f, base)
        in_panel = False
        panel_indent = 0
        end_count = 0
        obj_count = 0
        for i, line in enumerate(lines):
            stripped = line.strip()
            if b'palAutoDeviceEjection' in line and b'object' in line.lower():
                in_panel = True
                panel_indent = len(line) - len(line.lstrip())
                obj_count = 1
                end_count = 0
                continue
            if in_panel:
                if stripped.lower().startswith(b'object '):
                    obj_count += 1
                if stripped == b'end':
                    end_count += 1
                    if end_count >= obj_count:
                        in_panel = False
                        break
                for label, term in search_terms:
                    if term in line:
                        decoded = decode_line(line)
                        print(f'  [{label}] {rel}:{i+1}  {decoded[:250]}')
    except:
        pass

# === 5. All palAutoDeviceEjection in .cpp ===
print()
print('=' * 70)
print('5. All palAutoDeviceEjection usages in .cpp files')
print('=' * 70)
for rel, ln, text in search_all(base, ['*.cpp'], b'palAutoDeviceEjection'):
    print(f'  {rel}:{ln}  {text[:250]}')
