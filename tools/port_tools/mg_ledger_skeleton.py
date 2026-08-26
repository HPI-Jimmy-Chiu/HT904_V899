# -*- coding: utf-8 -*-
"""MG 戰役：從 mg_ai_matrix_missing.csv 產 LEDGER 骨架（日期叢集 → 波次候選）。"""
import csv, io, os, re
from collections import defaultdict

ROOT = r"D:\HT9045"
SRC = os.path.join(ROOT, "docs", "mg_ai_matrix_missing.csv")
OUT = os.path.join(ROOT, "docs", "MG_PORT_LEDGER.md")
CASE_RE = re.compile(r"CASE-[A-Z_a-z]*-?\d{8}-\d{3}|SPEC-[A-Za-z0-9-]+|ADR-\d+")

rows = list(csv.DictReader(io.open(SRC, encoding="utf-8-sig")))
by_date = defaultdict(list)
for r in rows:
    by_date[r["date"]].append(r)

with io.open(OUT, "w", encoding="utf-8", newline="\n") as f:
    f.write("# MG_PORT_LEDGER — V899→V910 搬移總帳\n\n")
    f.write("- 資料源：docs/mg_ai_matrix_missing.csv（337 條 MISSING）＋ git log bb69c60..b515ed5 ＋ weekly 案件盤點\n")
    f.write("- 狀態值：pending / in-wave / done(commit) / skipped-C / queued-decision / excluded-E\n")
    f.write("- 分類：A 乾淨搬｜B 客戶碼隔離｜C V910已有｜D 安全關鍵(忠實+錨點乾淨→可執行,否則進最終決策清單)｜E 純分析\n\n")
    f.write("| 叢集(日期) | 註解數 | 檔數 | comment 內 CASE/SPEC 線索 | 主要檔案 | 分類初判 | 狀態 |\n")
    f.write("|---|---|---|---|---|---|---|\n")
    for date in sorted(by_date):
        rs = by_date[date]
        files = defaultdict(int)
        cases = set()
        for r in rs:
            files[r["file"]] += 1
            for m in CASE_RE.findall(r["comment"]):
                cases.add(m)
        top = ", ".join("%s(%d)" % (k, v) for k, v in sorted(files.items(), key=lambda x: -x[1])[:4])
        more = "" if len(files) <= 4 else " +%d檔" % (len(files) - 4)
        f.write("| %s | %d | %d | %s | %s%s | pending | pending |\n" %
                (date, len(rs), len(files), "; ".join(sorted(cases)) or "—", top, more))
    f.write("\n## 叢集明細（每叢集樣本註解）\n\n")
    for date in sorted(by_date):
        rs = by_date[date]
        f.write("### %s（%d 條）\n\n" % (date, len(rs)))
        seen = set()
        for r in rs:
            key = re.sub(r"\d+", "N", r["comment"][:70])
            if key in seen:
                continue
            seen.add(key)
            f.write("- `%s:%s` %s\n" % (r["file"], r["line"], r["comment"][:110]))
            if len(seen) >= 6:
                break
        f.write("\n")
print("clusters=%d rows=%d -> %s" % (len(by_date), len(rows), OUT))
