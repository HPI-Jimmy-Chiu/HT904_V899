# -*- coding: utf-8 -*-
"""
extract_ai_comments.py - 從 V899 原始碼掃描 //AI(...) 註解，產生 YAML 草稿
用法: python extract_ai_comments.py [--source-dir path] [--output draft.yaml]
"""
import os
import re
import sys
import yaml
from collections import defaultdict

DEFAULT_SRC = os.path.join(
    os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))),
    "HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422"
)

AI_PATTERN = re.compile(
    r'//AI\(([^)]+)\)\s+(\d{8})\s*:\s*(.+?)(?:\s*$)',
    re.IGNORECASE
)


def scan_source_files(src_dir):
    """掃描所有 .cpp/.h 檔案，擷取 //AI(...) 註解"""
    results = []
    for root, dirs, files in os.walk(src_dir):
        # 略過 .svn
        dirs[:] = [d for d in dirs if d != '.svn']
        for fname in files:
            if not fname.lower().endswith(('.cpp', '.h', '.hpp')):
                continue
            fpath = os.path.join(root, fname)
            try:
                with open(fpath, 'r', encoding='big5', errors='replace') as f:
                    for lineno, line in enumerate(f, 1):
                        m = AI_PATTERN.search(line)
                        if m:
                            results.append({
                                'agent': m.group(1),
                                'date': m.group(2),
                                'desc': m.group(3).strip(),
                                'file': os.path.relpath(fpath, src_dir),
                                'line': lineno,
                            })
            except Exception:
                pass
    return results


def group_by_date(results):
    """依日期分群，合併相同描述"""
    by_date = defaultdict(list)
    for r in results:
        by_date[r['date']].append(r)

    grouped = {}
    for date_str in sorted(by_date.keys(), reverse=True):
        items = by_date[date_str]
        # 去重：同一描述只保留一次，但記錄涉及的檔案
        seen = {}
        for item in items:
            key = item['desc']
            if key not in seen:
                seen[key] = {
                    'desc': item['desc'],
                    'files': [],
                    'agent': item['agent'],
                }
            seen[key]['files'].append(item['file'])

        formatted_date = "%s/%s/%s" % (date_str[:4], date_str[4:6], date_str[6:8])
        grouped[formatted_date] = list(seen.values())

    return grouped


def write_draft_yaml(grouped, output_path):
    """輸出草稿 YAML"""
    entries = []
    for date_str, items in grouped.items():
        changes = []
        for item in items:
            file_list = ", ".join(sorted(set(item['files'])))
            changes.append("%s [%s]" % (item['desc'], file_list))

        entries.append({
            'rev': "HT9045_3.33.899.x",  # 需手動填入正確版號
            'date': date_str,
            'engineer': "JimmyChiu",       # 需手動確認
            'changes': changes,
        })

    data = {
        'customer': "DRAFT",
        'handler_type': "HT-9045/46 Series",
        'footer_rev': "Rev.4",
        '_note': "此為自動產生的草稿，請手動：(1) 填入正確 rev 版號 (2) 將技術描述改為客戶友好的中文描述 (3) 移除檔名標記",
        'entries': entries,
    }

    os.makedirs(os.path.dirname(os.path.abspath(output_path)), exist_ok=True)
    with open(output_path, 'w', encoding='utf-8') as f:
        yaml.dump(data, f, allow_unicode=True, default_flow_style=False, sort_keys=False)

    print("[OK] 產生草稿: %s (%d 筆)" % (output_path, len(entries)))


def main():
    src_dir = DEFAULT_SRC
    output_path = os.path.join(
        os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
        "changelogs", "_ai_draft.yaml"
    )

    for i, arg in enumerate(sys.argv):
        if arg == "--source-dir" and i + 1 < len(sys.argv):
            src_dir = sys.argv[i + 1]
        if arg == "--output" and i + 1 < len(sys.argv):
            output_path = sys.argv[i + 1]

    print("[INFO] 掃描原始碼: %s" % src_dir)
    results = scan_source_files(src_dir)
    print("[INFO] 找到 %d 筆 AI 註解" % len(results))

    grouped = group_by_date(results)
    print("[INFO] 分為 %d 個日期群組" % len(grouped))

    write_draft_yaml(grouped, output_path)


if __name__ == "__main__":
    main()