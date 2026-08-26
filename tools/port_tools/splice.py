# -*- coding: utf-8 -*-
"""MG 戰役：位元組級 splice——把 V899 的行原樣插入/取代到 V910 檔。絕不重打字。

用法：
  python tools/port_tools/splice.py <ops.json>

ops.json 格式（UTF-8，list of op）：
[
  {
    "v910_file": "CosFunction.cpp",          # 相對 V910 樹根
    "v899_file": "CosFunction.cpp",          # 相對 V899 樹根（略=同 v910_file）
    "v899_lines": [120, 135],                 # 1-based inclusive，抽這段位元組
    "mode": "after",                          # after | before | replace
    "anchor_line": 118,                       # V910 1-based 行號（after/before 的錨）
    "anchor_expect": "if(xxx==true)",         # 錨行內容必含此字串（cp950 比對），防呆必填
    "replace_lines": [200, 203]               # 僅 mode=replace：V910 要被取代的行段
  }, ...
]

規則：
- 同檔多個 op 由本工具**由下往上**套用（行號以套用前的 V910 檔為準，不互相位移）。
- 插入行行尾自動轉成目標檔主流 EOL；行內位元組（含縮排）原樣不動。
- 每檔第一次動它時存 <file>.mgbak（已存在則不覆蓋）。
- anchor_expect 不符 → 整檔中止不寫入（all-or-nothing）。
"""
import json, os, sys, io, shutil

ROOT = r"D:\HT9045"
V899 = os.path.join(ROOT, "HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422")
V910 = os.path.join(ROOT, "HT9011UC_Code_V3.33.910.0_20260716_Jimmy")


def split_keepends(data):
    return data.splitlines(keepends=True)


def dominant_eol(lines):
    crlf = sum(1 for l in lines if l.endswith(b"\r\n"))
    lf = sum(1 for l in lines if l.endswith(b"\n")) - crlf
    return b"\r\n" if crlf >= lf else b"\n"


def norm_eol(lines, eol):
    out = []
    for l in lines:
        core = l.rstrip(b"\r\n")
        out.append(core + eol)
    return out


def main():
    if len(sys.argv) != 2:
        print("usage: splice.py <ops.json>"); sys.exit(2)
    ops = json.load(io.open(sys.argv[1], encoding="utf-8"))
    by_file = {}
    for op in ops:
        by_file.setdefault(op["v910_file"], []).append(op)

    for rel, file_ops in by_file.items():
        dst_path = os.path.join(V910, rel)
        with open(dst_path, "rb") as f:
            dst = split_keepends(f.read())
        eol = dominant_eol(dst)
        # 由下往上，行號互不位移
        def keyline(op):
            return op.get("anchor_line") or op.get("replace_lines", [0])[0]
        file_ops.sort(key=keyline, reverse=True)

        for op in file_ops:
            src_rel = op.get("v899_file", rel)
            with open(os.path.join(V899, src_rel), "rb") as f:
                src = split_keepends(f.read())
            s, e = op["v899_lines"]
            if not (1 <= s <= e <= len(src)):
                print("FAIL %s: v899_lines %s 超界(%d)" % (rel, op["v899_lines"], len(src))); sys.exit(1)
            payload = norm_eol(src[s - 1:e], eol)

            if op["mode"] in ("after", "before"):
                a = op["anchor_line"]
                if not (1 <= a <= len(dst)):
                    print("FAIL %s: anchor_line %d 超界(%d)" % (rel, a, len(dst))); sys.exit(1)
                expect = op["anchor_expect"].encode("cp950", errors="ignore")
                if expect not in dst[a - 1]:
                    print("FAIL %s:%d 錨行不符: 期待含 %r，實際 %r"
                          % (rel, a, op["anchor_expect"], dst[a - 1][:80])); sys.exit(1)
                at = a if op["mode"] == "after" else a - 1
                dst[at:at] = payload
            elif op["mode"] == "replace":
                rs, re_ = op["replace_lines"]
                if not (1 <= rs <= re_ <= len(dst)):
                    print("FAIL %s: replace_lines %s 超界(%d)" % (rel, op["replace_lines"], len(dst))); sys.exit(1)
                expect = op["anchor_expect"].encode("cp950", errors="ignore")
                if expect not in b"".join(dst[rs - 1:re_]):
                    print("FAIL %s: replace 區段不含期待字串 %r" % (rel, op["anchor_expect"])); sys.exit(1)
                dst[rs - 1:re_] = payload
            else:
                print("FAIL: 未知 mode %r" % op["mode"]); sys.exit(1)

        bak = dst_path + ".mgbak"
        if not os.path.exists(bak):
            shutil.copy2(dst_path, bak)
        with open(dst_path, "wb") as f:
            f.write(b"".join(dst))
        print("OK %s: %d op(s) applied, eol=%s" % (rel, len(file_ops), "CRLF" if eol == b"\r\n" else "LF"))


if __name__ == "__main__":
    main()
