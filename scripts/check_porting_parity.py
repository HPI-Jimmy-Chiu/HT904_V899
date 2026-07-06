# -*- coding: cp950 -*-
"""
check_porting_parity.py — 跨版本搬移對等性檢查

用途：
    驗證從來源版本搬移到目標版本的程式碼，是否在「行為錨點 / 資料錨點 /
    常數錨點」三個維度上保持等價。專門抓「條件對、資料對，但呼叫方式
    不一樣」這類架構性錯誤（例如 V874 ADAMTCP_WriteReg(reg=1, count=16)
    被誤搬成 V899 (reg=1, count=8) + (reg=33, count=8)）。

用法：
    python scripts/check_porting_parity.py <case.yaml>
    python scripts/check_porting_parity.py --case multi_ep_v874_to_v899

YAML 案例檔位置：scripts/parity_cases/<case>.yaml

輸出：
    - 標準輸出：紅/黃/綠三色報告
    - 退出碼：0=全綠, 1=有黃, 2=有紅
"""
from __future__ import annotations
import argparse
import re
import sys
from pathlib import Path
from typing import Dict, List, Tuple

import yaml

WORKSPACE = Path(__file__).resolve().parent.parent
CASES_DIR = Path(__file__).resolve().parent / "parity_cases"
ENCODING = "cp950"
SOURCE_EXTS = {".cpp", ".h", ".hpp", ".c"}

# ---------------------------------------------------------------------------
# YAML 極簡解析（避免依賴 PyYAML）
# ---------------------------------------------------------------------------

def parse_simple_yaml(text: str) -> dict:
    """支援 key: value / 巢狀 / list (- ) 的最簡 YAML，夠這個工具用。"""
    root: dict = {}
    stack: List[Tuple[int, object]] = [(-1, root)]
    for raw in text.splitlines():
        line = raw.split("#", 1)[0].rstrip()
        if not line.strip():
            continue
        indent = len(line) - len(line.lstrip())
        content = line.strip()
        # 退到正確層
        while stack and stack[-1][0] >= indent:
            stack.pop()
        parent = stack[-1][1]
        if content.startswith("- "):
            value = content[2:].strip()
            if not isinstance(parent, list):
                # 上一層是 dict 的最後一個 key，要把它變 list
                # 這裡用約定：父 dict 最後加入的 key 是 list 容器
                raise ValueError(f"YAML 結構錯誤（list 缺父 key）: {raw!r}")
            if ":" in value:
                k, v = value.split(":", 1)
                item = {k.strip(): v.strip().strip('"')} if v.strip() else {k.strip(): {}}
                parent.append(item)
                stack.append((indent, item))
            else:
                parent.append(value.strip('"'))
        elif ":" in content:
            k, v = content.split(":", 1)
            k = k.strip()
            v = v.strip().strip('"')
            if not isinstance(parent, dict):
                raise ValueError(f"YAML 結構錯誤（key 在 list 內）: {raw!r}")
            if v == "":
                # 子節點：先當 dict，遇到 - 再轉 list
                child: object = {}
                parent[k] = child
                stack.append((indent, child))
            elif v == "[]":
                parent[k] = []
                stack.append((indent, parent[k]))
            else:
                parent[k] = v
        else:
            raise ValueError(f"YAML 結構無法解析: {raw!r}")
    return root


def coerce_list_node(parent: dict, key: str) -> list:
    """確保 parent[key] 是 list（解析時可能初始為 dict {}）。"""
    cur = parent.get(key)
    if isinstance(cur, list):
        return cur
    if isinstance(cur, dict) and not cur:
        parent[key] = []
        return parent[key]
    if cur is None:
        parent[key] = []
        return parent[key]
    raise TypeError(f"{key} 應為 list，但得到 {type(cur).__name__}")


# ---------------------------------------------------------------------------
# 版本資料夾解析（與 version_diff.py 同邏輯）
# ---------------------------------------------------------------------------

def resolve_version_dir(arg: str) -> Path:
    p = WORKSPACE / arg
    if p.is_dir():
        return p
    m = re.fullmatch(r"V(\d{3,4})", arg, re.IGNORECASE)
    if m:
        ver = m.group(1)
        candidates = sorted(WORKSPACE.glob(f"HT9011UC_Code_V*.{ver}.*"))
        if not candidates:
            candidates = sorted(WORKSPACE.glob(f"HT9046LS_Code_V*.{ver}.*"))
        if len(candidates) == 1:
            return candidates[0]
        if len(candidates) > 1:
            sys.stderr.write(f"[WARN] {arg} 對應多個資料夾，採用最後一個\n")
            return candidates[-1]
    sys.exit(f"[ERROR] 找不到版本資料夾: {arg}")


# ---------------------------------------------------------------------------
# 行為錨點：API 呼叫掃描
# ---------------------------------------------------------------------------
# 抓 API_NAME(...) 的所有呼叫，回傳「正規化後的引數簽名」清單。
# 我們不需要解析完整 C++，只需把括號內的引數抽出做粗略比對。

def find_api_calls(root: Path, api_name: str) -> List[Tuple[Path, int, str, List[str]]]:
    """回傳 [(檔案, 行號, 完整呼叫文字, 引數列表)]"""
    pattern = re.compile(rf"\b{re.escape(api_name)}\s*\(")
    results: List[Tuple[Path, int, str, List[str]]] = []
    for p in root.rglob("*"):
        if not p.is_file() or p.suffix.lower() not in SOURCE_EXTS:
            continue
        parts_lower = [s.lower() for s in p.parts]
        if any(seg in parts_lower for seg in (".svn", "backup", "obj")):
            continue
        try:
            text = p.read_text(encoding=ENCODING, errors="replace")
        except OSError:
            continue
        lines = text.splitlines()
        for i, line in enumerate(lines, 1):
            stripped = line.lstrip()
            if stripped.startswith("//"):
                continue
            if not pattern.search(line):
                continue
            # 抽出第一個 (...) 內容（不處理巢狀，但對單行呼叫夠用）
            call_text, args = extract_args(line, api_name)
            if call_text is None:
                continue
            results.append((p, i, call_text, args))
    return results


def extract_args(line: str, api_name: str) -> Tuple[str | None, List[str]]:
    """從一行抓 api_name(...) 的引數，括號平衡。"""
    idx = re.search(rf"\b{re.escape(api_name)}\s*\(", line)
    if not idx:
        return None, []
    start = idx.end() - 1  # 指向 (
    depth = 0
    end = -1
    for j in range(start, len(line)):
        ch = line[j]
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
            if depth == 0:
                end = j
                break
    if end == -1:
        return None, []
    inner = line[start + 1: end]
    # 用括號深度切 ,
    args = split_top_commas(inner)
    return f"{api_name}({inner})", [a.strip() for a in args]


def split_top_commas(s: str) -> List[str]:
    parts: List[str] = []
    depth = 0
    cur = []
    for ch in s:
        if ch in "([{":
            depth += 1
        elif ch in ")]}":
            depth -= 1
        if ch == "," and depth == 0:
            parts.append("".join(cur))
            cur = []
        else:
            cur.append(ch)
    if cur:
        parts.append("".join(cur))
    return parts


def normalize_call(args: List[str], pick: List[int] | None) -> str:
    """挑選關鍵引數做正規化簽名。pick=None 代表全部。"""
    if pick is None:
        sel = args
    else:
        sel = [args[i] for i in pick if 0 <= i < len(args)]
    return ", ".join(sel)


# ---------------------------------------------------------------------------
# 全域變數錨點
# ---------------------------------------------------------------------------

def find_var_decl(root: Path, var_name: str) -> List[Tuple[Path, int, str]]:
    """搜全域變數宣告（粗略：含變數名且帶 = 或 [ 的行）"""
    pattern = re.compile(rf"\b{re.escape(var_name)}\s*(\[[^\]]*\])?\s*(=|;)")
    results: List[Tuple[Path, int, str]] = []
    for p in root.rglob("*"):
        if not p.is_file() or p.suffix.lower() not in SOURCE_EXTS:
            continue
        parts_lower = [s.lower() for s in p.parts]
        if any(seg in parts_lower for seg in (".svn", "backup", "obj")):
            continue
        try:
            text = p.read_text(encoding=ENCODING, errors="replace")
        except OSError:
            continue
        for i, line in enumerate(text.splitlines(), 1):
            if line.lstrip().startswith("//"):
                continue
            if pattern.search(line):
                results.append((p, i, line.strip()))
    return results


# ---------------------------------------------------------------------------
# 常數字串錨點（IP / INI key）
# ---------------------------------------------------------------------------

def find_string_literal(root: Path, literal: str) -> List[Tuple[Path, int]]:
    pattern = re.compile(re.escape(literal))
    results: List[Tuple[Path, int]] = []
    for p in root.rglob("*"):
        if not p.is_file() or p.suffix.lower() not in SOURCE_EXTS:
            continue
        parts_lower = [s.lower() for s in p.parts]
        if any(seg in parts_lower for seg in (".svn", "backup", "obj")):
            continue
        try:
            text = p.read_text(encoding=ENCODING, errors="replace")
        except OSError:
            continue
        for i, line in enumerate(text.splitlines(), 1):
            if pattern.search(line):
                results.append((p, i))
    return results


# ---------------------------------------------------------------------------
# 報告
# ---------------------------------------------------------------------------

class Report:
    def __init__(self) -> None:
        self.red: List[str] = []
        self.yellow: List[str] = []
        self.green: List[str] = []

    def add(self, level: str, msg: str) -> None:
        bucket = {"RED": self.red, "YELLOW": self.yellow, "GREEN": self.green}[level]
        bucket.append(msg)

    def print(self) -> int:
        for tag, items in (("RED", self.red), ("YELLOW", self.yellow), ("GREEN", self.green)):
            if not items:
                continue
            print(f"\n=== {tag} ({len(items)}) ===")
            for m in items:
                print(f"  [{tag}] {m}")
        print(f"\nSummary: RED={len(self.red)}  YELLOW={len(self.yellow)}  GREEN={len(self.green)}")
        if self.red:
            return 2
        if self.yellow:
            return 1
        return 0


# ---------------------------------------------------------------------------
# 主流程
# ---------------------------------------------------------------------------

def check_behavior(case: dict, src: Path, dst: Path, rep: Report) -> None:
    items = case.get("behavior_anchors") or []
    if not isinstance(items, list):
        return
    for entry in items:
        if not isinstance(entry, dict):
            continue
        api = entry.get("api")
        guard = entry.get("guard", "").lower()  # 例如 "INSTALL_DOUBLE_EP==3"
        pick_raw = entry.get("pick", "")
        pick = [int(x) for x in pick_raw.split(",")] if pick_raw else None

        if not api:
            continue

        src_calls = find_api_calls(src, api)
        dst_calls = find_api_calls(dst, api)

        # 過濾 guard：簡化策略 = 引數正規化字串裡含關鍵字（粗略）
        # 改進：未來可改用「往上看 N 行有沒有 guard 條件」
        def keep(args: List[str]) -> bool:
            if not guard:
                return True
            joined = " ".join(args).lower()
            # guard 只比對引數中是否提及關鍵字（如 Address[2]）
            return guard in joined

        src_sigs = sorted({normalize_call(a, pick) for _, _, _, a in src_calls if keep(a)})
        dst_sigs = sorted({normalize_call(a, pick) for _, _, _, a in dst_calls if keep(a)})

        only_src = [s for s in src_sigs if s not in dst_sigs]
        only_dst = [s for s in dst_sigs if s not in src_sigs]

        header = f"behavior {api} (guard={guard or '*'}, pick={pick_raw or 'all'})"
        if not only_src and not only_dst:
            rep.add("GREEN", f"{header}: 兩版簽名一致 ({len(src_sigs)} 種)")
            continue
        if only_src and not only_dst:
            rep.add("RED", f"{header}: 來源有但目標沒搬到 -> {only_src}")
        elif only_dst and not only_src:
            rep.add("YELLOW", f"{header}: 目標多出簽名 (可能是新功能或誤搬) -> {only_dst}")
        else:
            rep.add("RED",
                    f"{header}: 雙向不一致\n"
                    f"      only_src={only_src}\n"
                    f"      only_dst={only_dst}")


def check_data(case: dict, src: Path, dst: Path, rep: Report) -> None:
    items = case.get("data_anchors") or []
    if not isinstance(items, list):
        return
    for entry in items:
        if not isinstance(entry, dict):
            continue
        var = entry.get("var")
        if not var:
            continue
        s_decl = find_var_decl(src, var)
        d_decl = find_var_decl(dst, var)
        if not s_decl:
            rep.add("YELLOW", f"data {var}: 來源無宣告 (可能為函式區域變數，跳過)")
            continue
        if not d_decl:
            rep.add("RED", f"data {var}: 目標版完全沒有對應宣告")
            continue
        # 取每個 decl 的「正規化字串」（去空白）
        s_norm = sorted({re.sub(r"\s+", " ", t) for _, _, t in s_decl})
        d_norm = sorted({re.sub(r"\s+", " ", t) for _, _, t in d_decl})
        if s_norm == d_norm:
            rep.add("GREEN", f"data {var}: 宣告完全一致")
        else:
            rep.add("YELLOW",
                    f"data {var}: 宣告差異\n"
                    f"      src={s_norm}\n"
                    f"      dst={d_norm}")


def check_constants(case: dict, src: Path, dst: Path, rep: Report) -> None:
    items = case.get("constant_anchors") or []
    if not isinstance(items, list):
        return
    for entry in items:
        if not isinstance(entry, dict):
            continue
        literal = entry.get("literal")
        if literal is None:
            continue
        s_hits = find_string_literal(src, literal)
        d_hits = find_string_literal(dst, literal)
        if not s_hits and not d_hits:
            rep.add("YELLOW", f"const {literal!r}: 兩版都找不到")
        elif s_hits and not d_hits:
            rep.add("RED", f"const {literal!r}: 來源 {len(s_hits)} 處，目標 0 處 (漏搬)")
        elif d_hits and not s_hits:
            rep.add("YELLOW", f"const {literal!r}: 目標 {len(d_hits)} 處，來源 0 處 (新增)")
        else:
            rep.add("GREEN", f"const {literal!r}: 來源 {len(s_hits)} / 目標 {len(d_hits)} 都存在")


def load_case(case_arg: str) -> dict:
    p = Path(case_arg)
    if not p.exists():
        p2 = CASES_DIR / f"{case_arg}.yaml"
        if p2.exists():
            p = p2
        else:
            sys.exit(f"[ERROR] 找不到案例檔: {case_arg} (also tried {p2})")
    raw = p.read_bytes()
    for enc in ("utf-8", "cp950"):
        try:
            text = raw.decode(enc)
            break
        except UnicodeDecodeError:
            continue
    else:
        sys.exit(f"[ERROR] 無法解碼案例檔: {p}")
    return yaml.safe_load(text) or {}


def main() -> int:
    ap = argparse.ArgumentParser(description="跨版本搬移對等性檢查")
    ap.add_argument("case", nargs="?", help="案例檔路徑或 parity_cases/<name>.yaml 的 name")
    ap.add_argument("--case", dest="case_kw", help="同上，--case <name> 寫法")
    args = ap.parse_args()

    case_arg = args.case or args.case_kw
    if not case_arg:
        ap.error("必須提供案例檔")

    case = load_case(case_arg)
    src_dir = resolve_version_dir(case.get("source_version", ""))
    dst_dir = resolve_version_dir(case.get("target_version", ""))

    print(f"Source: {src_dir.name}")
    print(f"Target: {dst_dir.name}")
    print(f"Case  : {case.get('name', '(unnamed)')}")

    rep = Report()
    check_behavior(case, src_dir, dst_dir, rep)
    check_data(case, src_dir, dst_dir, rep)
    check_constants(case, src_dir, dst_dir, rep)
    return rep.print()


if __name__ == "__main__":
    sys.exit(main())
