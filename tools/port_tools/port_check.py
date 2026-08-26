# -*- coding: utf-8 -*-
"""MG 戰役：波次 gate——驗證 V910 修改檔的編碼／EOL／對齊／來源忠實度。

用法：
  python tools/port_tools/port_check.py <v910-relpath> [...]
  （不給參數 = 自動抓 git status 裡 V910 樹全部 M 檔）

對每個檔比對 git HEAD 版 vs 磁碟版（行級 diff），檢查：
  1. FAIL  cp950 解碼錯誤數增加（引入亂碼）
  2. FAIL  未變更行的位元組不完全一致（不可能發生於行級 diff，防禦性）
  3. REPORT EOL 分布變化（CRLF/LF 計數，新增行應跟隨主流 EOL；混用=FAIL）
  4. 新增行分類：
     - SPLICED  在 V899 同路徑檔找到位元組級相同行（modulo EOL）→ 對齊保證成立
     - COMMENT  含 mg899to910（本戰役撰寫的註解）
     - AUTHORED 其他（客戶碼 gate 行等）→ 列出供主腦複驗；縮排風格與鄰行不符=WARN
  5. REPORT 被移除/被取代的原 V910 行全數列出（主腦複驗依據）
結束碼：任一 FAIL → 1；否則 0（AUTHORED/WARN 需主腦人工看過）。
"""
import os, sys, subprocess, difflib

ROOT = r"D:\HT9045"
V899 = os.path.join(ROOT, "HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422")
V910REL = "HT9011UC_Code_V3.33.910.0_20260716_Jimmy"
V910 = os.path.join(ROOT, V910REL)


def decode_errors(data):
    n = 0
    for line in data.split(b"\n"):
        try:
            line.decode("cp950")
        except UnicodeDecodeError:
            n += 1
    return n


def eol_hist(lines):
    crlf = sum(1 for l in lines if l.endswith(b"\r\n"))
    nl = sum(1 for l in lines if l.endswith(b"\n"))
    return crlf, nl - crlf  # (CRLF, bare-LF)


def core(l):
    return l.rstrip(b"\r\n")


def indent_of(l):
    c = core(l)
    return c[:len(c) - len(c.lstrip(b" \t"))]


def check(rel):
    rel_u = rel.replace("\\", "/")
    disk_path = os.path.join(V910, rel)
    bak_path = disk_path + ".mgbak"
    if os.path.exists(bak_path):
        with open(bak_path, "rb") as f:
            old = f.read()
    else:
        head = subprocess.run(["git", "show", "HEAD:%s/%s" % (V910REL, rel_u)],
                              cwd=ROOT, capture_output=True)
        if head.returncode != 0:
            print("[%s] SKIP: 不在 HEAD（新檔？）— 需主腦人工審" % rel)
            return True
        old = head.stdout
        # autocrlf 環境：repo blob 是 LF，磁碟多為 CRLF——重建 checkout 形式再比
        if b"\r\n" not in old:
            with open(disk_path, "rb") as f:
                probe = f.read(65536)
            if probe.count(b"\r\n") > probe.count(b"\n") - probe.count(b"\r\n"):
                old = old.replace(b"\n", b"\r\n")
    with open(disk_path, "rb") as f:
        new = f.read()
    if old == new:
        print("[%s] no change" % rel)
        return True

    ok = True
    de_old, de_new = decode_errors(old), decode_errors(new)
    if de_new > de_old:
        print("[%s] FAIL: cp950 解碼錯誤 %d -> %d（引入亂碼）" % (rel, de_old, de_new))
        ok = False

    ol, nl = old.splitlines(keepends=True), new.splitlines(keepends=True)
    o_eol, n_eol = eol_hist(ol), eol_hist(nl)
    dominant = b"\r\n" if o_eol[0] >= o_eol[1] else b"\n"

    v899_path = os.path.join(V899, rel)
    v899_cores = set()
    if os.path.exists(v899_path):
        with open(v899_path, "rb") as f:
            v899_cores = set(core(l) for l in f.read().splitlines(keepends=True))

    sm = difflib.SequenceMatcher(a=[core(l) for l in ol], b=[core(l) for l in nl], autojunk=False)
    added, removed = [], []
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag in ("insert", "replace"):
            added += [(j + 1, nl[j]) for j in range(j1, j2)]
        if tag in ("delete", "replace"):
            removed += [(i + 1, ol[i]) for i in range(i1, i2)]
        if tag == "equal":
            for k in range(i2 - i1):
                if ol[i1 + k] != nl[j1 + k]:
                    print("[%s] FAIL: 未變更行 %d 的 EOL/位元組被改動" % (rel, i1 + k + 1))
                    ok = False

    spliced = comment = 0
    authored = []
    for lineno, l in added:
        c = core(l)
        if l.endswith(b"\n") and ((dominant == b"\r\n") != l.endswith(b"\r\n")):
            print("[%s] FAIL: 新增行 %d 的 EOL 與主流(%s)不符" %
                  (rel, lineno, "CRLF" if dominant == b"\r\n" else "LF"))
            ok = False
        if not c.strip():
            spliced += 1
            continue
        if b"mg899to910" in c:
            comment += 1
        elif c in v899_cores:
            spliced += 1
        else:
            near = indent_of(nl[lineno - 2]) if lineno >= 2 else b""
            style = "" if (indent_of(l)[:1] or near[:1]) in (near[:1], b"") else " ⚠縮排風格與上一行不符"
            authored.append((lineno, c, style))

    print("[%s] added=%d (spliced=%d, comment=%d, authored=%d) removed=%d | EOL %s->%s"
          % (rel, len(added), spliced, comment, len(authored), len(removed), o_eol, n_eol))
    for lineno, c, style in authored:
        print("    AUTHORED %5d: %s%s" % (lineno, c.decode("cp950", errors="backslashreplace")[:100], style))
    for lineno, l in removed:
        print("    REMOVED  %5d: %s" % (lineno, core(l).decode("cp950", errors="backslashreplace")[:100]))
    return ok


def main():
    rels = sys.argv[1:]
    if not rels:
        st = subprocess.run(["git", "status", "--porcelain", "--", V910REL],
                            cwd=ROOT, capture_output=True).stdout.decode("utf-8", errors="replace")
        rels = [line[3:].strip().strip('"')[len(V910REL) + 1:]
                for line in st.splitlines() if line[:2].strip() == "M"]
    if not rels:
        print("nothing to check"); return
    all_ok = True
    for rel in rels:
        all_ok = check(rel.replace("/", "\\")) and all_ok
    print("== port_check %s ==" % ("PASS" if all_ok else "FAIL"))
    sys.exit(0 if all_ok else 1)


if __name__ == "__main__":
    main()
