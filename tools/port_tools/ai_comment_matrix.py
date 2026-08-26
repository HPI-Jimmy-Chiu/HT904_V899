# -*- coding: utf-8 -*-
"""MG 戰役 Phase 0 盤點工具：V899 //AI 註解 → V910 存在性矩陣。

逐位元組操作（Big5/cp950 安全），絕不重寫任何原始碼檔。
輸出：
  docs/mg_ai_matrix_summary.md   — 日期×存在性統計（人讀）
  docs/mg_ai_matrix_missing.csv  — V910 缺席的註解明細（搬移工作清單，utf-8-sig）
  docs/mg_ai_matrix_all.csv      — 全量明細（含已存在者，複驗用）
"""
import os, re, csv, sys, io

ROOT = r"D:\HT9045"
SRC = os.path.join(ROOT, "HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422")
DST = os.path.join(ROOT, "HT9011UC_Code_V3.33.910.0_20260716_Jimmy")
OUTDIR = os.path.join(ROOT, "docs")
EXTS = (".cpp", ".h", ".c", ".hpp", ".dfm")
DATE_MIN = b"20260323"  # V899 樹版本日（V3.33.899.0_20260323）；之後的註解才算 V899 期工作
# 20260826 修正：原用 20260422（資料夾後綴日）會漏掉 0323–0421 的早期工作，
# weekly 盤點交叉驗證抓到（樹內有 0316–0421 註解）。0323 前的屬 898 期繼承，不在範圍。

AI_RE = re.compile(rb"//AI\(([^)]{1,40})\)\s*(\d{8})\s*[:\xa1\xb0]?\s*(.*)")


def iter_files(base):
    for dirpath, dirnames, filenames in os.walk(base):
        dirnames[:] = [d for d in dirnames if d != ".svn"]
        for fn in filenames:
            if fn.lower().endswith(EXTS):
                yield os.path.join(dirpath, fn)


def collect_entries(base):
    """回傳 [(relpath, lineno, agent, date, sig_bytes, full_line)]"""
    out = []
    for path in iter_files(base):
        rel = os.path.relpath(path, base)
        with open(path, "rb") as f:
            data = f.read()
        for i, line in enumerate(data.split(b"\n"), 1):
            m = AI_RE.search(line)
            if not m:
                continue
            agent, date, rest = m.group(1), m.group(2), m.group(3)
            # 簽章 = 從 //AI( 起到行尾（去尾端空白/CR），比對用
            sig = line[m.start():].rstrip(b" \t\r")
            out.append((rel, i, agent, date, sig))
    return out


def build_haystacks(base):
    hay = {}
    for path in iter_files(base):
        rel = os.path.relpath(path, base)
        with open(path, "rb") as f:
            hay[rel] = f.read()
    return hay


def d(b):
    return b.decode("cp950", errors="replace")


def main():
    src_entries = [e for e in collect_entries(SRC) if e[3] >= DATE_MIN]
    hay = build_haystacks(DST)
    blob = b"\n".join(hay.values())  # 全樹串接，做「任意檔存在」查詢

    rows = []
    for rel, ln, agent, date, sig in src_entries:
        in_same = rel in hay and sig in hay[rel]
        in_any = in_same or (sig in blob)
        status = "same-file" if in_same else ("other-file" if in_any else "MISSING")
        rows.append({
            "date": d(date), "agent": d(agent), "file": rel, "line": ln,
            "status": status, "comment": d(sig)[:160],
        })

    # 統計：日期 × 狀態
    stat = {}
    for r in rows:
        k = r["date"]
        stat.setdefault(k, {"same-file": 0, "other-file": 0, "MISSING": 0})
        stat[k][r["status"]] += 1

    os.makedirs(OUTDIR, exist_ok=True)
    with io.open(os.path.join(OUTDIR, "mg_ai_matrix_summary.md"), "w", encoding="utf-8", newline="\n") as f:
        f.write("# MG 盤點：V899 //AI 註解在 V910 的存在性矩陣\n\n")
        f.write("- 產生工具：tools/port_tools/ai_comment_matrix.py（位元組級簽章比對）\n")
        f.write("- 簽章 = `//AI(...)` 起至行尾的位元組串；`same-file`=同路徑檔命中、"
                "`other-file`=他檔命中（需人工確認）、`MISSING`=全樹未命中\n")
        f.write("- 注意：位元組級命中是強證據；**未命中≠功能一定缺**（公司可能改寫過），"
                "缺席項仍需波次內確認。\n\n")
        f.write("| 日期 | same-file | other-file | MISSING | 小計 |\n|---|---|---|---|---|\n")
        tot = {"same-file": 0, "other-file": 0, "MISSING": 0}
        for k in sorted(stat):
            s = stat[k]
            f.write("| %s | %d | %d | %d | %d |\n" % (k, s["same-file"], s["other-file"], s["MISSING"], sum(s.values())))
            for kk in tot: tot[kk] += s[kk]
        f.write("| **合計** | %d | %d | %d | %d |\n" % (tot["same-file"], tot["other-file"], tot["MISSING"], sum(tot.values())))
        # 缺席檔案 top
        fmiss = {}
        for r in rows:
            if r["status"] == "MISSING":
                fmiss[r["file"]] = fmiss.get(r["file"], 0) + 1
        f.write("\n## MISSING 依檔案分布（前 40）\n\n| 檔案 | 缺席註解數 |\n|---|---|\n")
        for fn, c in sorted(fmiss.items(), key=lambda x: -x[1])[:40]:
            f.write("| %s | %d |\n" % (fn, c))

    for name, filt in (("mg_ai_matrix_missing.csv", lambda r: r["status"] == "MISSING"),
                       ("mg_ai_matrix_all.csv", lambda r: True)):
        with io.open(os.path.join(OUTDIR, name), "w", encoding="utf-8-sig", newline="") as f:
            w = csv.DictWriter(f, fieldnames=["date", "agent", "file", "line", "status", "comment"])
            w.writeheader()
            for r in sorted(rows, key=lambda r: (r["date"], r["file"], r["line"])):
                if filt(r):
                    w.writerow(r)

    print("entries(V899-era)=%d  same-file=%d  other-file=%d  MISSING=%d" %
          (len(rows), tot["same-file"], tot["other-file"], tot["MISSING"]))


if __name__ == "__main__":
    main()
