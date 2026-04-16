# -*- coding: utf-8 -*-
"""
import_from_doc.py - 從既有 .doc Release Note 匯入為 YAML 格式
用法: python import_from_doc.py <input.doc> <output.yaml> [--customer NAME]
"""
import sys
import os
import re
import yaml

def extract_text_from_doc(doc_path):
    """使用 Word COM 自動化讀取 .doc 文字"""
    import win32com.client
    word = win32com.client.Dispatch("Word.Application")
    word.Visible = False
    try:
        doc = word.Documents.Open(os.path.abspath(doc_path))
        paragraphs = []
        for i in range(1, doc.Paragraphs.Count + 1):
            p = doc.Paragraphs(i)
            text = p.Range.Text.strip()
            style = p.Style.NameLocal if p.Style else ""
            paragraphs.append({"text": text, "style": style})

        # 取得 Handler Type
        handler_type = ""
        for p in paragraphs:
            if "Handler Type" in p["text"]:
                m = re.search(r"Handler Type[：:]\s*(.+)", p["text"])
                if m:
                    handler_type = m.group(1).strip()
                break

        # 取得 Footer
        footer_rev = ""
        for sec_idx in range(1, doc.Sections.Count + 1):
            sec = doc.Sections(sec_idx)
            ftr = sec.Footers(1).Range.Text.strip()
            if ftr:
                footer_rev = ftr
                break

        doc.Close(False)
        return paragraphs, handler_type, footer_rev
    finally:
        word.Quit()


def parse_entries(paragraphs):
    """解析段落列表，萃取 Rev / Date / Engineer / Changes"""
    entries = []
    current = None
    rev_pattern = re.compile(
        r"Rev[.．]?[：:]\s*(\S+)\s+.*?Date\s*[：:]\s*([\d/]+)\s+.*?Engineer[：:]\s*(\S+)"
    )

    for p in paragraphs:
        text = p["text"]
        if not text:
            continue

        m = rev_pattern.search(text)
        if m:
            if current:
                entries.append(current)
            current = {
                "rev": m.group(1),
                "date": m.group(2),
                "engineer": m.group(3),
                "changes": [],
            }
        elif current is not None:
            # 略過 header 區域的段落
            if any(kw in text for kw in ["鴻 勁", "HON.TECH", "Software Release", "Handler Type"]):
                continue
            # 清理控制字元
            clean = text.replace("\x01", "").replace("\x07", "").replace("\x15", "").strip()
            if clean:
                current["changes"].append(clean)

    if current:
        entries.append(current)

    return entries


def write_yaml(output_path, customer, handler_type, footer_rev, entries):
    """輸出 YAML 檔案"""
    data = {
        "customer": customer,
        "handler_type": handler_type,
        "footer_rev": footer_rev,
        "entries": [],
    }
    for e in entries:
        data["entries"].append({
            "rev": e["rev"],
            "date": e["date"],
            "engineer": e["engineer"],
            "changes": e["changes"],
        })

    # 使用 allow_unicode 以保留中文
    with open(output_path, "w", encoding="utf-8") as f:
        yaml.dump(data, f, allow_unicode=True, default_flow_style=False, sort_keys=False)

    print(f"[OK] 匯出 {len(entries)} 筆紀錄 -> {output_path}")


def main():
    if len(sys.argv) < 3:
        print("用法: python import_from_doc.py <input.doc> <output.yaml> [--customer NAME]")
        sys.exit(1)

    doc_path = sys.argv[1]
    yaml_path = sys.argv[2]
    customer = "Unknown"
    for i, arg in enumerate(sys.argv):
        if arg == "--customer" and i + 1 < len(sys.argv):
            customer = sys.argv[i + 1]

    if not os.path.exists(doc_path):
        print(f"[ERROR] 找不到檔案: {doc_path}")
        sys.exit(1)

    print(f"[INFO] 讀取 {doc_path} ...")
    paragraphs, handler_type, footer_rev = extract_text_from_doc(doc_path)
    print(f"[INFO] 共 {len(paragraphs)} 個段落, Handler Type: {handler_type}")

    entries = parse_entries(paragraphs)
    print(f"[INFO] 解析出 {len(entries)} 筆 Release 紀錄")

    write_yaml(yaml_path, customer, handler_type, footer_rev, entries)


if __name__ == "__main__":
    main()