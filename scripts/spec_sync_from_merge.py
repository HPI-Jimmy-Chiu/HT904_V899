# -*- coding: cp950 -*-
"""
spec_sync_from_merge.py — HT9045 合入他人版本時的 SPEC 缺口偵測工具

用途：
    當你要把另一位開發者的版本（如 V897 RogerYang）合入到目前版本（V899 Jimmy）時，
    本工具會：
      1. 呼叫 version_diff 邏輯找出函式級差異
      2. 對每個變動函式檢查是否有對應 //AI 註解
      3. 比對現有 docs/spec/ 是否已有相關 feature SPEC
      4. 產出「SPEC 缺口報告」草稿，標記需要補哪些 SPEC

用法：
    python scripts/spec_sync_from_merge.py --from V897 --to V899
    python scripts/spec_sync_from_merge.py --from V897 --to V899 --module InArm
    python scripts/spec_sync_from_merge.py --from V897 --to V899 --out docs/spec/_gap_report.md

依賴：scripts/version_diff.py 必須存在。
"""
from __future__ import annotations
import argparse
import re
import subprocess
import sys
from pathlib import Path
from typing import List, Set, Dict, Tuple

WORKSPACE = Path(__file__).resolve().parent.parent
SOURCE_EXTS = {".cpp", ".h", ".hpp", ".c"}
ENCODING = "cp950"
SPEC_DIR = WORKSPACE / "docs" / "spec"

# 重用 version_diff 的版本解析
sys.path.insert(0, str(WORKSPACE / "scripts"))
try:
    from version_diff import resolve_version_dir, collect_files, extract_funcs, file_hash  # type: ignore
except ImportError:
    sys.exit("[ERROR] 找不到 scripts/version_diff.py，請先確認檔案存在")

AI_VALID_RE = re.compile(r"//AI\(([\w\-]+)\)\s+(\d{8})\b[^:]*:\s*\S")


def scan_ai_annotations(path: Path) -> List[Tuple[int, str]]:
    """掃描檔案內所有 //AI 合法註解，回傳 [(行號, 描述)]。"""
    result: List[Tuple[int, str]] = []
    try:
        text = path.read_text(encoding=ENCODING, errors="replace")
    except OSError:
        return result
    for i, line in enumerate(text.splitlines(), start=1):
        if AI_VALID_RE.search(line):
            result.append((i, line.strip()))
    return result


def list_existing_spec_features() -> Set[str]:
    """列出現有 features/ 目錄下所有 SPEC 檔名（不含副檔名）。"""
    feat_dir = SPEC_DIR / "features"
    if not feat_dir.is_dir():
        return set()
    return {p.stem for p in feat_dir.glob("*.spec.md")}


def list_existing_spec_modules() -> Set[str]:
    mod_dir = SPEC_DIR / "modules"
    if not mod_dir.is_dir():
        return set()
    return {p.stem.replace(".spec", "") for p in mod_dir.glob("*.spec.md")}


def guess_module_from_path(rel_path: str) -> str:
    """從檔案路徑猜模組名（取最頂層資料夾，全小寫）。"""
    parts = rel_path.split("/")
    if len(parts) >= 2:
        return parts[0].lower()
    return "root"


def build_gap_report(
    dir_a: Path,
    dir_b: Path,
    module_filter: str | None,
    target_version: str,
) -> List[str]:
    files_a = collect_files(dir_a, module_filter)
    files_b = collect_files(dir_b, module_filter)

    common = sorted(set(files_a.keys()) & set(files_b.keys()))
    only_b = sorted(set(files_b.keys()) - set(files_a.keys()))

    existing_specs = list_existing_spec_features()
    existing_modules = list_existing_spec_modules()

    # 統計：每個模組的變動函式 / //AI 覆蓋率
    module_stats: Dict[str, Dict[str, int]] = {}
    rows: List[Tuple[str, Set[str], Set[str], int]] = []  # (rel, new_funcs, removed_funcs, ai_count)

    for rel in common:
        if file_hash(files_a[rel]) == file_hash(files_b[rel]):
            continue
        funcs_a = extract_funcs(files_a[rel])
        funcs_b = extract_funcs(files_b[rel])
        new_funcs = funcs_b - funcs_a
        removed_funcs = funcs_a - funcs_b
        if not (new_funcs or removed_funcs):
            # 內容變但函式名沒變（可能是內部修改）
            ai_anns = scan_ai_annotations(files_b[rel])
            rows.append((rel, set(), set(), len(ai_anns)))
        else:
            ai_anns = scan_ai_annotations(files_b[rel])
            rows.append((rel, new_funcs, removed_funcs, len(ai_anns)))

        mod = guess_module_from_path(rel)
        st = module_stats.setdefault(mod, {"changed_files": 0, "new_funcs": 0, "ai_count": 0})
        st["changed_files"] += 1
        st["new_funcs"] += len(new_funcs)
        st["ai_count"] += len(ai_anns)

    # only_b
    for rel in only_b:
        ai_anns = scan_ai_annotations(files_b[rel])
        funcs_b = extract_funcs(files_b[rel])
        rows.append((rel, funcs_b, set(), len(ai_anns)))
        mod = guess_module_from_path(rel)
        st = module_stats.setdefault(mod, {"changed_files": 0, "new_funcs": 0, "ai_count": 0})
        st["changed_files"] += 1
        st["new_funcs"] += len(funcs_b)
        st["ai_count"] += len(ai_anns)

    # ---------- 產出 markdown ----------
    out: List[str] = []
    out.append("# SPEC 缺口報告（合入他人版本後）")
    out.append("")
    out.append(f"- 來源 A：`{dir_a.name}`")
    out.append(f"- 目標 B：`{dir_b.name}`")
    if module_filter:
        out.append(f"- 模組過濾：`{module_filter}`")
    out.append("")
    out.append("> 此報告為**草稿**。請對每個「待補 SPEC」項目，建立對應功能 SPEC 並補完三維度錨點。")
    out.append("")

    # 摘要
    out.append("## 摘要")
    out.append("")
    out.append(f"- 變動檔案數：{len(rows)}")
    total_new_funcs = sum(len(nf) for _, nf, _, _ in rows)
    total_ai = sum(c for _, _, _, c in rows)
    out.append(f"- 新增函式總數：{total_new_funcs}")
    out.append(f"- //AI 註解總數：{total_ai}")
    out.append(f"- //AI 覆蓋率：{(total_ai / total_new_funcs * 100):.1f}%" if total_new_funcs else "- //AI 覆蓋率：N/A")
    out.append("")

    # 模組級摘要
    if module_stats:
        out.append("## 模組級摘要")
        out.append("")
        out.append("| 模組 | 變動檔 | 新函式 | //AI 數 | 模組 SPEC | 建議 |")
        out.append("|------|--------|--------|---------|-----------|------|")
        for mod, st in sorted(module_stats.items()):
            has_spec = "?" if mod in existing_modules else "? 待建"
            advice = ""
            if mod not in existing_modules and st["changed_files"] >= 3:
                advice = "建立模組 SPEC"
            elif st["new_funcs"] > 0 and st["ai_count"] == 0:
                advice = "補 //AI 註解 + 功能 SPEC"
            elif st["new_funcs"] == 0:
                advice = "更新模組 SPEC 一段"
            out.append(f"| `{mod}` | {st['changed_files']} | {st['new_funcs']} | {st['ai_count']} | {has_spec} | {advice} |")
        out.append("")

    # 待補功能 SPEC 清單
    out.append("## 待補功能 SPEC 清單")
    out.append("")
    seq = 1
    for rel, new_funcs, removed_funcs, ai_count in rows:
        if not new_funcs and not removed_funcs and ai_count == 0:
            continue  # 內部小改且無 //AI，跳過
        feature_id_hint = f"F{target_version}-{seq:03d}"
        seq += 1
        out.append(f"### {feature_id_hint}（建議檔名）")
        out.append("")
        out.append(f"- **檔案**：`{rel}`")
        if new_funcs:
            out.append(f"- **B 新增函式**：{', '.join(sorted(new_funcs))}")
        if removed_funcs:
            out.append(f"- **B 刪除函式**：{', '.join(sorted(removed_funcs))}")
        out.append(f"- **//AI 註解數**：{ai_count}")
        if ai_count == 0 and (new_funcs or removed_funcs):
            out.append("- ?? **缺 //AI 註解**：請開發者補上 `//AI(<Author>) YYYYMMDD: 描述`")
        out.append("- **建議建立**：")
        suggested_path = f"docs/spec/features/{feature_id_hint}-<簡短名>.spec.md"
        out.append(f"  - `{suggested_path}`")
        out.append("- **三維度錨點**（待補）：")
        out.append("  - 條件錨點：（請補）")
        out.append("  - 資料錨點：（請補）")
        out.append("  - 行為錨點：（請補）")
        out.append("")

    # 已存在的功能 SPEC 提醒
    if existing_specs:
        out.append("## 現有功能 SPEC（請確認是否需更新）")
        out.append("")
        for spec in sorted(existing_specs):
            out.append(f"- `features/{spec}.md`")
        out.append("")

    out.append("## 下一步")
    out.append("")
    out.append("1. 對每個「待補功能 SPEC」項目，複製 `docs/spec/TEMPLATE_feature.md` 建立新檔案")
    out.append("2. 補完三維度錨點（條件 / 資料 / 行為）")
    out.append("3. 若 `?? 缺 //AI 註解`，請聯絡原作者補註解或自行追加")
    out.append("4. 更新對應模組 SPEC 的「Known Issues」或「行為錨點」表")
    out.append("5. 在 weekly_data 對應 case 加入 SPEC 連結")
    return out


def main() -> int:
    parser = argparse.ArgumentParser(description="HT9045 合入他人版本時的 SPEC 缺口偵測")
    parser.add_argument("--from", dest="ver_from", required=True, help="來源版本（如 V897）")
    parser.add_argument("--to", dest="ver_to", required=True, help="目標版本（如 V899）")
    parser.add_argument("--module", help="只比對路徑含此關鍵字（如 InArm）")
    parser.add_argument("--out", help="輸出 markdown 檔路徑（預設 stdout）")
    args = parser.parse_args()

    dir_a = resolve_version_dir(args.ver_from)
    dir_b = resolve_version_dir(args.ver_to)

    sys.stderr.write(f"[INFO] FROM = {dir_a.name}\n")
    sys.stderr.write(f"[INFO] TO   = {dir_b.name}\n")

    # 從 ver_to 萃取版本號做為 feature ID 前綴
    m = re.search(r"V(\d{3,4})", args.ver_to)
    target_ver = m.group(1) if m else "XXX"

    lines = build_gap_report(dir_a, dir_b, args.module, target_ver)
    text = "\n".join(lines)

    if args.out:
        out_path = Path(args.out)
        if not out_path.is_absolute():
            out_path = WORKSPACE / out_path
        out_path.parent.mkdir(parents=True, exist_ok=True)
        out_path.write_text(text, encoding="utf-8")
        sys.stderr.write(f"[INFO] 已寫入 {out_path}\n")
    else:
        sys.stdout.write(text)
        sys.stdout.write("\n")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
