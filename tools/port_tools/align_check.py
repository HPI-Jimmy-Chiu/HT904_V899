# -*- coding: utf-8 -*-
"""MG 戰役排版驗收：掃描 V910 樹自基線以來所有新增行的排版一致性。

分級（重點是分辨「MG 造成的排版問題」與「V899 原樣照搬」）：
  DEFECT  本戰役自己寫出來、且偏離 V910 該檔慣例 → 要修
  FAITHFUL V899 有一模一樣的行（位元組級）→ 忠實搬運，形狀與出貨版一致，不算缺陷
  跳過      續行（前一行括號未平衡）、空行

檢查項：TAB / INDENT(非4倍數) / COLUMN(註解欄位) / TRAIL(行尾空白) / ENC(亂碼)
用法： python tools/port_tools/align_check.py [--verbose] [--faithful]
"""
import subprocess, sys, re, os
from collections import Counter

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
V910REL = "HT9011UC_Code_V3.33.910.0_20260716_Jimmy"
V899REL = "HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422"
BASE = "e06524a"
VERBOSE = "--verbose" in sys.argv
SHOW_FAITHFUL = "--faithful" in sys.argv
CMT = re.compile(r'\S[ \t]{2,}//')


def sh(a):
    return subprocess.run(a, cwd=ROOT, capture_output=True).stdout


def changed_files():
    fs = [l.strip() for l in sh(["git", "diff", "--name-only", BASE, "--", V910REL]).decode("utf-8", "replace").splitlines() if l.strip()]
    for l in sh(["git", "status", "--porcelain", "--", V910REL]).decode("utf-8", "replace").splitlines():
        p = l[3:].strip().strip('"')
        if p and p not in fs:
            fs.append(p)
    return [f for f in fs if f.lower().endswith((".cpp", ".h", ".c", ".hpp"))]


def base_bytes(rel):
    r = subprocess.run(["git", "show", "%s:%s" % (BASE, rel)], cwd=ROOT, capture_output=True)
    return r.stdout if r.returncode == 0 else b""


def ccol_b(b):
    """行尾註解的起始欄位，以位元組計（等同 BCB6 定寬編輯器的顯示欄）"""
    if b.lstrip().startswith(b"//"):
        return None                    # 整行註解不算
    idx = b.find(b"//")
    while idx > 0:
        before = b[:idx]
        if before.strip() and before.endswith(b"  "):
            return idx
        idx = b.find(b"//", idx + 2)
    return None


def ccol(s):
    return ccol_b(s.encode("cp950", "replace"))


def strip_str_cmt(s):
    """去掉字串/字元常數與行註解（手寫掃描, 不用 regex 跳脫）, 供括號平衡判斷"""
    out=[]; i=0; n=len(s); q=None
    while i<n:
        ch=s[i]
        if q is None:
            if ch=="/" and i+1<n and s[i+1]=="/":
                break
            if ch=="'" or ch==chr(34):
                q=ch; i+=1; continue
            out.append(ch); i+=1
        else:
            if ch==chr(92):
                i+=2; continue
            if ch==q:
                q=None
            i+=1
    return "".join(out)


def analyse(rel):
    disk = os.path.join(ROOT, rel.replace("/", os.sep))
    if not os.path.exists(disk):
        return [], []
    new = open(disk, "rb").read()
    old = base_bytes(rel)
    if b"\r\n" not in old and b"\r\n" in new:
        old = old.replace(b"\n", b"\r\n")
    old_set = set(old.splitlines())
    L = new.splitlines()

    # V899 同名檔的行集合（判斷是否忠實搬運）
    v899p = os.path.join(ROOT, V899REL, rel[len(V910REL) + 1:].replace("/", os.sep))
    v899_set = set()
    if os.path.exists(v899p):
        v899_set = set(x.rstrip(b"\r") for x in open(v899p, "rb").read().splitlines())

    cols = Counter()
    for b in L:
        c = ccol_b(b)
        if c is not None:
            cols[c] += 1
    dom = cols.most_common(1)[0][0] if cols else None

    # 續行偵測：累計括號深度
    depth = [0] * (len(L) + 1)
    d = 0
    for i, b in enumerate(L):
        s = strip_str_cmt(b.decode("cp950", "replace"))
        depth[i] = d                      # 進入本行之前的深度
        d += s.count("(") - s.count(")")
    defects, faithful = [], []
    for i, b in enumerate(L):
        if b in old_set:
            continue
        try:
            s = b.decode("cp950")
        except UnicodeDecodeError:
            defects.append((i + 1, "ENC", "cp950 解碼失敗（亂碼）")); continue
        if not s.strip():
            continue
        is_faithful = b.rstrip(b"\r") in v899_set
        bucket = faithful if is_faithful else defects
        ind = s[:len(s) - len(s.lstrip(" \t"))]
        if "\t" in ind:
            bucket.append((i + 1, "TAB", "縮排含 tab"))
        elif len(ind) % 4 != 0 and depth[i] == 0:      # 深度 0 才算真縮排（否則是續行對齊）
            bucket.append((i + 1, "INDENT", "縮排 %d 非 4 的倍數" % len(ind)))
        if s != s.rstrip():
            bucket.append((i + 1, "TRAIL", "行尾多餘空白 %d" % (len(s) - len(s.rstrip()))))
        c = ccol_b(b)
        if c is not None and dom is not None and c != dom:
            lo, hi = max(0, i - 25), min(len(L), i + 25)
            loc = Counter()
            for j in range(lo, hi):
                cj = ccol_b(L[j])
                if cj is not None:
                    loc[cj] += 1
            if loc.get(c, 0) < 2:
                bucket.append((i + 1, "COLUMN", "註解 col%d vs 該檔主流 col%d" % (c, dom)))
    return defects, faithful


def main():
    files = changed_files()
    print("排版驗收：V910 樹自 %s 以來變更的 %d 個原始檔" % (BASE, len(files)))
    print("=" * 76)
    tot_d = tot_f = 0
    kinds = Counter()
    for f in sorted(files):
        d, fa = analyse(f)
        tot_d += len(d); tot_f += len(fa)
        for _, k, _ in d:
            kinds[k] += 1
        if d:
            print("\n[%s]  DEFECT %d 項" % (f[len(V910REL) + 1:], len(d)))
            for ln, k, m in (d if VERBOSE else d[:10]):
                print("   %-7s 行%-6d %s" % (k, ln, m))
            if len(d) > 10 and not VERBOSE:
                print("   ... 另 %d 項" % (len(d) - 10))
        if fa and SHOW_FAITHFUL:
            print("\n[%s]  FAITHFUL %d 項（與 V899 出貨版位元組相同，形狀一致）" % (f[len(V910REL) + 1:], len(fa)))
    print("\n" + "=" * 76)
    print("FAITHFUL（V899 原樣，不算缺陷）：%d 項" % tot_f)
    if tot_d == 0:
        print("排版驗收 PASS：本戰役自行撰寫的行，全部符合各檔縮排與註解欄位慣例")
        return 0
    print("排版驗收：本戰役自撰行有 %d 項偏離 — %s" % (tot_d, dict(kinds)))
    return 1


if __name__ == "__main__":
    sys.exit(main())
