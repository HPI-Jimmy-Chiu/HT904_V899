# -*- coding: utf-8 -*-
"""
generate_doc.py  -  YAML -> .docx Release Note (HON.PREC format)
Usage: python generate_doc.py <input.yaml> [--output path/output.docx]

Uses python-docx for precise formatting control.
Optionally converts to .doc via Word COM if --doc flag is given.
"""
import sys
import os
import yaml
from docx import Document
from docx.shared import Pt, Cm, RGBColor
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.enum.section import WD_ORIENT
from docx.oxml.ns import qn, nsdecls
from docx.oxml import parse_xml

# Style constants
DARK_BLUE_RGB = RGBColor(0x38, 0x48, 0x91)   # #384891
RED_RGB       = RGBColor(0xFF, 0x00, 0x00)
WHITE_RGB     = RGBColor(0xFF, 0xFF, 0xFF)
BLACK_RGB     = RGBColor(0x00, 0x00, 0x00)

DARK_BLUE_HEX = "384891"


def load_yaml(path):
    with open(path, "r", encoding="utf-8") as f:
        return yaml.safe_load(f)


def set_shading(paragraph, color_hex):
    """Set paragraph background shading color."""
    shading = parse_xml(
        '<w:shd {} w:fill="{}" w:val="clear"/>'.format(nsdecls('w'), color_hex)
    )
    paragraph._element.get_or_add_pPr().append(shading)


def add_run(paragraph, text, font_name, font_size, bold=False, color=BLACK_RGB):
    """Add a formatted run to a paragraph."""
    run = paragraph.add_run(text)
    run.font.name = font_name
    run.font.size = Pt(font_size)
    run.font.bold = bold
    run.font.color.rgb = color
    # Set East Asian font
    rpr = run._element.get_or_add_rPr()
    ea_font = parse_xml(
        '<w:rFonts {} w:eastAsia="{}"/>'.format(nsdecls('w'), font_name)
    )
    rpr.append(ea_font)
    return run


def create_docx(data, output_path):
    doc = Document()

    # --- Page Setup: A4 Landscape ---
    section = doc.sections[0]
    section.orientation = WD_ORIENT.LANDSCAPE
    section.page_width  = Cm(29.7)
    section.page_height = Cm(21.0)
    section.top_margin    = Cm(2.0)
    section.bottom_margin = Cm(1.5)
    section.left_margin   = Cm(1.5)
    section.right_margin  = Cm(1.5)

    # --- Header: Company Name (Chinese) ---
    p = doc.add_paragraph()
    p.alignment = WD_ALIGN_PARAGRAPH.CENTER
    add_run(p, u"\u9d3b \u52c1 \u79d1 \u6280 / \u9d3b \u52c1 \u7cbe \u5bc6 (\u80a1)\u516c \u53f8",
            u"\u83ef\u5eb7\u65b0\u7279\u9ed1\u9ad4", 36, color=DARK_BLUE_RGB)

    # --- Header: Company Name (English) ---
    p = doc.add_paragraph()
    p.alignment = WD_ALIGN_PARAGRAPH.LEFT
    add_run(p, "HON.TECH / HON.PREC, INC.", "Arial Black", 24, color=DARK_BLUE_RGB)

    # --- Blank lines ---
    doc.add_paragraph()
    doc.add_paragraph()

    # --- Title ---
    p = doc.add_paragraph()
    p.alignment = WD_ALIGN_PARAGRAPH.LEFT
    add_run(p, "        Software Release Note        ",
            u"\u5fae\u8edf\u6b63\u9ed1\u9ad4", 40)

    # --- Blank lines ---
    doc.add_paragraph()
    doc.add_paragraph()

    # --- Handler Type ---
    handler = data.get("handler_type", "HT-9045/46 Series")
    p = doc.add_paragraph()
    p.alignment = WD_ALIGN_PARAGRAPH.CENTER
    add_run(p, u"Handler Type\uff1a" + handler,
            u"\u5fae\u8edf\u6b63\u9ed1\u9ad4", 28)

    # --- Blank lines ---
    doc.add_paragraph()
    doc.add_paragraph()

    # --- Release Entries ---
    for entry in data.get("entries", []):
        rev  = entry.get("rev", "")
        date = entry.get("date", "")
        eng  = entry.get("engineer", "")
        changes = entry.get("changes", [])

        # Rev line: blue background, white bold text
        rev_text = u"Rev.\uff1a%s  \u25ba Date : %s  \u25ba Engineer\uff1a %s" % (
            rev, date, eng)
        p = doc.add_paragraph()
        p.alignment = WD_ALIGN_PARAGRAPH.LEFT
        set_shading(p, DARK_BLUE_HEX)
        add_run(p, rev_text, u"\u5fae\u8edf\u6b63\u9ed1\u9ad4", 14,
                bold=True, color=WHITE_RGB)

        # Change lines: red text
        for chg in changes:
            p = doc.add_paragraph()
            p.alignment = WD_ALIGN_PARAGRAPH.LEFT
            add_run(p, chg, u"\u5fae\u8edf\u6b63\u9ed1\u9ad4", 14, color=RED_RGB)

        # Separator blank line
        doc.add_paragraph()

    # --- Footer ---
    footer_rev = data.get("footer_rev", "Rev.4")
    footer = section.footer
    footer.is_linked_to_previous = False
    fp = footer.paragraphs[0]
    fp.alignment = WD_ALIGN_PARAGRAPH.RIGHT
    fp.add_run(footer_rev)

    # --- Save ---
    abs_path = os.path.abspath(output_path)
    doc.save(abs_path)
    print("[OK] Release Note: %s" % abs_path)
    return abs_path


def convert_to_doc(docx_path):
    """Optional: convert .docx to .doc using Word COM."""
    try:
        import win32com.client
        word = win32com.client.Dispatch("Word.Application")
        word.Visible = False
        doc_path = docx_path.replace(".docx", ".doc")
        wdoc = word.Documents.Open(os.path.abspath(docx_path))
        wdoc.SaveAs(os.path.abspath(doc_path), 0)  # wdFormatDocument=0
        wdoc.Close(False)
        word.Quit()
        print("[OK] Converted to .doc: %s" % doc_path)
        return doc_path
    except ImportError:
        print("[WARN] pywin32 not available, skipping .doc conversion")
        return None


def main():
    if len(sys.argv) < 2:
        print("Usage: python generate_doc.py <input.yaml> [--output path] [--doc]")
        sys.exit(1)

    yaml_path = sys.argv[1]
    output_path = None
    want_doc = "--doc" in sys.argv

    for i, arg in enumerate(sys.argv):
        if arg == "--output" and i + 1 < len(sys.argv):
            output_path = sys.argv[i + 1]

    data = load_yaml(yaml_path)

    if output_path is None:
        customer = data.get("customer", "output")
        out_dir = os.path.join(
            os.path.dirname(os.path.dirname(os.path.abspath(yaml_path))),
            "output")
        os.makedirs(out_dir, exist_ok=True)
        output_path = os.path.join(out_dir, "%s_Software_Release_Note.docx" % customer)

    docx_path = create_docx(data, output_path)

    if want_doc:
        convert_to_doc(docx_path)


if __name__ == "__main__":
    main()
# -*- coding: utf-8 -*-
"""
generate_doc.py  -  YAML -> .doc Release Note (HON.PREC format)
Usage: python generate_doc.py <input.yaml> [--output path/output.doc]

Strategy: build all text with Content.Text, then format by paragraph index.
Accounts for Word's extra trailing paragraph by matching text content.
"""
import sys, os, yaml

# Word enums
wdAlignLeft   = 0
wdAlignCenter = 1
wdAlignRight  = 2
wdLandscape   = 1
wdFormatDoc   = 0
wdNoShading   = -16777216

# Style constants
DARK_BLUE = 9527094
WHITE     = 16777215
RED_BGR   = 255

# Font names
F_JHENG   = u"\u5fae\u8edf\u6b63\u9ed1\u9ad4"
F_KAIU    = u"\u6a19\u6977\u9ad4"
F_COMPANY = u"\u83ef\u5eb7\u65b0\u7279\u9ed1\u9ad4"

BLANK_SPEC = ("", F_KAIU, 20, False, 0, wdAlignLeft, None)


def load_yaml(path):
    with open(path, "r", encoding="utf-8") as f:
        return yaml.safe_load(f)


def build_specs(data):
    specs = []
    company = u"\u9d3b \u52c1 \u79d1 \u6280 / \u9d3b \u52c1 \u7cbe \u5bc6 (\u80a1)\u516c \u53f8"
    specs.append((company, F_COMPANY, 36, False, DARK_BLUE, wdAlignCenter, None))
    specs.append(("HON.TECH / HON.PREC, INC.", "Arial Black", 24, False, DARK_BLUE, wdAlignLeft, None))
    specs.append(BLANK_SPEC)
    specs.append(BLANK_SPEC)
    specs.append(("        Software Release Note        ", F_JHENG, 40, False, 0, wdAlignLeft, None))
    specs.append(BLANK_SPEC)
    specs.append(BLANK_SPEC)
    handler = data.get("handler_type", "HT-9045/46 Series")
    specs.append((u"Handler Type\uff1a" + handler, F_JHENG, 28, False, 0, wdAlignCenter, None))
    specs.append(BLANK_SPEC)
    specs.append(BLANK_SPEC)

    for entry in data.get("entries", []):
        rev  = entry.get("rev", "")
        date = entry.get("date", "")
        eng  = entry.get("engineer", "")
        rev_line = u"Rev.\uff1a%s  \u25ba Date : %s  \u25ba Engineer\uff1a %s" % (rev, date, eng)
        specs.append((rev_line, F_JHENG, 14, True, WHITE, wdAlignLeft, DARK_BLUE))
        for chg in entry.get("changes", []):
            specs.append((chg, F_JHENG, 14, False, RED_BGR, wdAlignLeft, None))
        specs.append(BLANK_SPEC)

    return specs


def fmt_para(p, font, size, bold, color, align, shading):
    rng = p.Range
    rng.Font.Name  = font
    rng.Font.Size  = size
    rng.Font.Bold  = -1 if bold else 0
    rng.Font.Color = color
    p.Alignment    = align
    rng.Shading.BackgroundPatternColor = shading if shading is not None else wdNoShading


def create_doc(data, output_path):
    import win32com.client
    word = win32com.client.Dispatch("Word.Application")
    word.Visible = False

    try:
        doc = word.Documents.Add()
        ps = doc.PageSetup
        ps.Orientation  = wdLandscape
        ps.TopMargin    = 56.7
        ps.BottomMargin = 42.55
        ps.LeftMargin   = 42.55
        ps.RightMargin  = 42.55

        specs = build_specs(data)

        # Pass 1: set all text at once (fast)
        lines = [s[0] for s in specs]
        doc.Content.Text = "\r".join(lines)

        # Determine offset: find which Word paragraph holds specs[0] text
        para_count = doc.Paragraphs.Count
        offset = 0
        for trial in range(min(3, para_count)):
            p_text = doc.Paragraphs(trial + 1).Range.Text.replace("\r", "").replace("\n", "").strip()
            if p_text == specs[0][0].strip():
                offset = trial
                break

        # Pass 2: format each paragraph
        for i, spec in enumerate(specs):
            pidx = i + 1 + offset
            if pidx > para_count:
                break
            _, font, size, bold, color, align, shading = spec
            fmt_para(doc.Paragraphs(pidx), font, size, bold, color, align, shading)

        # Footer
        footer_rev = data.get("footer_rev", "Rev.4")
        ftr = doc.Sections(1).Footers(1).Range
        ftr.Text = footer_rev
        ftr.ParagraphFormat.Alignment = wdAlignRight

        abs_path = os.path.abspath(output_path)
        doc.SaveAs(abs_path, wdFormatDoc)
        doc.Close(False)
        print("[OK] Release Note: %s (%d records, offset=%d)" % (abs_path, len(specs), offset))

    finally:
        word.Quit()


def main():
    if len(sys.argv) < 2:
        print("Usage: python generate_doc.py <input.yaml> [--output path/output.doc]")
        sys.exit(1)

    yaml_path = sys.argv[1]
    output_path = None
    for i, arg in enumerate(sys.argv):
        if arg == "--output" and i + 1 < len(sys.argv):
            output_path = sys.argv[i + 1]

    data = load_yaml(yaml_path)

    if output_path is None:
        customer = data.get("customer", "output")
        out_dir = os.path.join(
            os.path.dirname(os.path.dirname(os.path.abspath(yaml_path))),
            "output")
        os.makedirs(out_dir, exist_ok=True)
        output_path = os.path.join(out_dir, "%s_Software_Release_Note.doc" % customer)

    create_doc(data, output_path)


if __name__ == "__main__":
    main()
# -*- coding: utf-8 -*-
"""
generate_doc.py  -  YAML -> .doc Release Note (HON.PREC format)
Usage: python generate_doc.py <input.yaml> [--output path/output.doc]

Two-pass approach:
  Pass 1 - insert all text (plain) separated by paragraph marks
  Pass 2 - walk paragraphs by index and apply formatting
"""
import sys, os, yaml

# ---- Word enums ----
wdAlignLeft    = 0
wdAlignCenter  = 1
wdAlignRight   = 2
wdLandscape    = 1
wdFormatDoc    = 0
wdNoShading    = -16777216

# ---- Style constants ----
DARK_BLUE = 9527094
WHITE     = 16777215
RED_BGR   = 255

# Font names (Unicode literals to avoid encoding issues)
F_JHENG   = u"\u5fae\u8edf\u6b63\u9ed1\u9ad4"        # Microsoft JhengHei
F_KAIU    = u"\u6a19\u6977\u9ad4"                      # DFKai-SB
F_COMPANY = u"\u83ef\u5eb7\u65b0\u7279\u9ed1\u9ad4"   # DFPHeiStd-W9

# Paragraph spec: (text, font, size, bold, color, align, shading)
#   shading=None means no background
BLANK = ("", F_KAIU, 20, False, 0, wdAlignLeft, None)


def load_yaml(path):
    with open(path, "r", encoding="utf-8") as f:
        return yaml.safe_load(f)


def build_para_specs(data):
    """Build a list of paragraph specifications from YAML data."""
    specs = []

    # Header
    company = u"\u9d3b \u52c1 \u79d1 \u6280 / \u9d3b \u52c1 \u7cbe \u5bc6 (\u80a1)\u516c \u53f8"
    specs.append((company, F_COMPANY, 36, False, DARK_BLUE, wdAlignCenter, None))
    specs.append(("HON.TECH / HON.PREC, INC.", "Arial Black", 24, False, DARK_BLUE, wdAlignLeft, None))
    specs.append(BLANK)
    specs.append(BLANK)
    specs.append(("        Software Release Note        ", F_JHENG, 40, False, 0, wdAlignLeft, None))
    specs.append(BLANK)
    specs.append(BLANK)

    handler = data.get("handler_type", "HT-9045/46 Series")
    specs.append((u"Handler Type\uff1a" + handler, F_JHENG, 28, False, 0, wdAlignCenter, None))
    specs.append(BLANK)
    specs.append(BLANK)

    # Entries
    for entry in data.get("entries", []):
        rev  = entry.get("rev", "")
        date = entry.get("date", "")
        eng  = entry.get("engineer", "")

        rev_line = u"Rev.\uff1a%s  \u25ba Date : %s  \u25ba Engineer\uff1a %s" % (rev, date, eng)
        specs.append((rev_line, F_JHENG, 14, True, WHITE, wdAlignLeft, DARK_BLUE))

        for chg in entry.get("changes", []):
            specs.append((chg, F_JHENG, 14, False, RED_BGR, wdAlignLeft, None))

        specs.append(BLANK)

    return specs


def create_doc(data, output_path):
    import win32com.client
    word = win32com.client.Dispatch("Word.Application")
    word.Visible = False

    try:
        doc = word.Documents.Add()

        # Page setup
        ps = doc.PageSetup
        ps.Orientation = wdLandscape
        ps.TopMargin   = 56.7
        ps.BottomMargin = 42.55
        ps.LeftMargin  = 42.55
        ps.RightMargin = 42.55

        specs = build_para_specs(data)

        # ---- Pass 1: insert plain text ----
        # Build one big string with \r as paragraph separator
        lines = [s[0] for s in specs]
        full_text = "\r".join(lines)
        doc.Content.Text = full_text

        # ---- Pass 2: format each paragraph ----
        para_count = doc.Paragraphs.Count
        n = min(len(specs), para_count)
        for i in range(n):
            text, font, size, bold, color, align, shading = specs[i]
            p = doc.Paragraphs(i + 1)   # 1-based
            rng = p.Range
            rng.Font.Name  = font
            rng.Font.Size  = size
            rng.Font.Bold  = -1 if bold else 0
            rng.Font.Color = color
            p.Alignment    = align
            if shading is not None:
                rng.Shading.BackgroundPatternColor = shading
            else:
                rng.Shading.BackgroundPatternColor = wdNoShading

        # Footer
        footer_rev = data.get("footer_rev", "Rev.4")
        sec = doc.Sections(1)
        ftr = sec.Footers(1).Range
        ftr.Text = footer_rev
        ftr.ParagraphFormat.Alignment = wdAlignRight

        # Save
        abs_path = os.path.abspath(output_path)
        doc.SaveAs(abs_path, wdFormatDoc)
        doc.Close(False)
        print("[OK] Release Note: %s (%d entries)" % (abs_path, len(specs)))

    finally:
        word.Quit()


def main():
    if len(sys.argv) < 2:
        print("Usage: python generate_doc.py <input.yaml> [--output path/output.doc]")
        sys.exit(1)

    yaml_path = sys.argv[1]
    output_path = None
    for i, arg in enumerate(sys.argv):
        if arg == "--output" and i + 1 < len(sys.argv):
            output_path = sys.argv[i + 1]

    data = load_yaml(yaml_path)

    if output_path is None:
        customer = data.get("customer", "output")
        out_dir = os.path.join(
            os.path.dirname(os.path.dirname(os.path.abspath(yaml_path))),
            "output")
        os.makedirs(out_dir, exist_ok=True)
        output_path = os.path.join(out_dir, "%s_Software_Release_Note.doc" % customer)

    create_doc(data, output_path)


if __name__ == "__main__":
    main()
# -*- coding: utf-8 -*-
"""
generate_doc.py - Generate .doc Release Note from YAML (HON.PREC format)
Usage: python generate_doc.py <input.yaml> [--output path/output.doc]

Spec (reverse-engineered from HT-9046_Software Release Note.doc):
  Paper : A4 Landscape (842 x 595 pt)
  Margin: T=56.7, B=42.55, L=42.55, R=42.55 pt
  Rev   : Microsoft JhengHei 14pt Bold White on #384891
  Change: Microsoft JhengHei 14pt Red (BGR=255)
"""
import sys
import os
import yaml

wdAlignLeft   = 0
wdAlignCenter = 1
wdAlignRight  = 2
wdLandscape   = 1
wdFormatDoc   = 0
wdNoShading   = -16777216
wdStory       = 6

DARK_BLUE = 9527094
WHITE     = 16777215
RED       = 255

FONT_TITLE    = u"\u5fae\u8edf\u6b63\u9ed1\u9ad4"   # Microsoft JhengHei
FONT_BODY     = u"\u6a19\u6977\u9ad4"                 # DFKai-SB
FONT_COMPANY  = u"\u83ef\u5eb7\u65b0\u7279\u9ed1\u9ad4"  # DFPHeiStd-W9


def load_yaml(path):
    with open(path, "r", encoding="utf-8") as f:
        return yaml.safe_load(f)


def _type_para(sel, text, font_name, font_size,
               bold=False, color=0, align=wdAlignLeft, bg=None):
    """Type a new paragraph at the current Selection and apply formatting."""
    sel.TypeParagraph()
    # Reset shading BEFORE typing to avoid inheritance
    sel.Range.Shading.BackgroundPatternColor = wdNoShading
    sel.Font.Name = font_name
    sel.Font.Size = font_size
    sel.Font.Bold = -1 if bold else 0
    sel.Font.Color = color
    sel.ParagraphFormat.Alignment = align
    if text:
        sel.TypeText(text)
    # Apply shading AFTER text is in place
    if bg is not None:
        sel.Paragraphs(1).Range.Shading.BackgroundPatternColor = bg


def create_doc(data, output_path):
    import win32com.client
    word = win32com.client.Dispatch("Word.Application")
    word.Visible = False

    try:
        doc = word.Documents.Add()

        # Page setup
        ps = doc.PageSetup
        ps.Orientation = wdLandscape
        ps.TopMargin = 56.7
        ps.BottomMargin = 42.55
        ps.LeftMargin = 42.55
        ps.RightMargin = 42.55

        sel = word.Selection

        # -- First paragraph (company name) --
        sel.Font.Name = FONT_COMPANY
        sel.Font.Size = 36
        sel.Font.Bold = 0
        sel.Font.Color = DARK_BLUE
        sel.ParagraphFormat.Alignment = wdAlignCenter
        sel.TypeText(u"\u9d3b \u52c1 \u79d1 \u6280 / \u9d3b \u52c1 \u7cbe \u5bc6 (\u80a1)\u516c \u53f8")

        _type_para(sel, "HON.TECH / HON.PREC, INC.",
                   "Arial Black", 24, color=DARK_BLUE, align=wdAlignLeft)
        _type_para(sel, "", FONT_BODY, 20)
        _type_para(sel, "", FONT_BODY, 20)
        _type_para(sel, "        Software Release Note        ",
                   FONT_TITLE, 40, align=wdAlignLeft)
        _type_para(sel, "", FONT_BODY, 20)
        _type_para(sel, "", FONT_BODY, 20)

        handler = data.get("handler_type", "HT-9045/46 Series")
        _type_para(sel, u"Handler Type\uff1a" + handler,
                   FONT_TITLE, 28, align=wdAlignCenter)
        _type_para(sel, "", FONT_BODY, 20)
        _type_para(sel, "", FONT_BODY, 20)

        # -- Entries --
        for entry in data.get("entries", []):
            rev = entry.get("rev", "")
            date = entry.get("date", "")
            eng = entry.get("engineer", "")
            changes = entry.get("changes", [])

            rev_line = u"Rev.\uff1a%s  \u25ba Date : %s  \u25ba Engineer\uff1a %s" % (rev, date, eng)
            _type_para(sel, rev_line,
                       FONT_TITLE, 14, bold=True, color=WHITE, bg=DARK_BLUE)

            for chg in changes:
                _type_para(sel, chg, FONT_TITLE, 14, color=RED)

            _type_para(sel, "", FONT_BODY, 20)

        # Footer
        footer_rev = data.get("footer_rev", "Rev.4")
        sec = doc.Sections(1)
        ftr_range = sec.Footers(1).Range
        ftr_range.Text = footer_rev
        ftr_range.ParagraphFormat.Alignment = wdAlignRight

        # Save
        abs_path = os.path.abspath(output_path)
        doc.SaveAs(abs_path, wdFormatDoc)
        doc.Close(False)
        print("[OK] Release Note: %s" % abs_path)

    finally:
        word.Quit()


def main():
    if len(sys.argv) < 2:
        print("Usage: python generate_doc.py <input.yaml> [--output path/output.doc]")
        sys.exit(1)

    yaml_path = sys.argv[1]
    output_path = None
    for i, arg in enumerate(sys.argv):
        if arg == "--output" and i + 1 < len(sys.argv):
            output_path = sys.argv[i + 1]

    data = load_yaml(yaml_path)

    if output_path is None:
        customer = data.get("customer", "output")
        out_dir = os.path.join(
            os.path.dirname(os.path.dirname(os.path.abspath(yaml_path))),
            "output")
        os.makedirs(out_dir, exist_ok=True)
        output_path = os.path.join(out_dir, "%s_Software_Release_Note.doc" % customer)

    create_doc(data, output_path)


if __name__ == "__main__":
    main()
# -*- coding: utf-8 -*-

"""

generate_doc.py - �q YAML ���� .doc Release Note�]�ƻs�E�l��l�榡�^

�Ϊk: python generate_doc.py <input.yaml> [--output path/output.doc]



�榡�W��]�q��l HT-9046_Software Release Note.doc �ϦV�u�{�^�G

  - �ȱi: A4 ��V (842 x 595 pt)

  - ��Z: T=56.7, B=42.55, L=42.55, R=42.55 pt

  - ���q�W: �رd�s�S���� 36pt, �~��, �`�Ŧ� #384891

  - �^��W: Arial Black 24pt, �����, �`�Ŧ�

  - ���D: �L�n������ 40pt, �����

  - Handler Type: �L�n������ 28pt, �~��

  - Rev ��: �L�n������ 14pt, ����, �զr, ���� #384891 (wdColor=9527094)

  - �ܧ��: �L�n������ 14pt, ���r (wdColorRed=16711680��BGR=255)

  - ����: �k���

"""

import sys

import os

import yaml



# Word constants

wdAlignParagraphLeft = 0

wdAlignParagraphCenter = 1

wdAlignParagraphRight = 2

wdOrientLandscape = 1

wdFormatDocument = 0  # .doc format





def load_yaml(path):

    with open(path, "r", encoding="utf-8") as f:

        return yaml.safe_load(f)





def create_release_note_doc(data, output_path):

    """�� Word COM �۰ʤƲ��� .doc"""

    import win32com.client



    word = win32com.client.Dispatch("Word.Application")

    word.Visible = False



    try:

        doc = word.Documents.Add()



        # --- Page Setup ---

        ps = doc.PageSetup

        ps.Orientation = wdOrientLandscape

        ps.TopMargin = 56.7

        ps.BottomMargin = 42.55

        ps.LeftMargin = 42.55

        ps.RightMargin = 42.55



        rng = doc.Range()

        DARK_BLUE = 9527094     # RGB(134,72,145) -> BGR encoded = #384891 area

        RED = 255               # Word BGR: pure red



        # --- Header: ���q�W ---

        rng.InsertAfter("�E �l �� �� / �E �l �� �K (��)�� �q\r")

        p = doc.Paragraphs(doc.Paragraphs.Count)

        p.Alignment = wdAlignParagraphCenter

        fmt_range(p.Range, "�رd�s�S����", 36, bold=False, color=DARK_BLUE)



        # --- Header: �^��W ---

        rng = doc.Range(doc.Content.End - 1, doc.Content.End - 1)

        rng.InsertAfter("HON.TECH / HON.PREC, INC.\r")

        p = doc.Paragraphs(doc.Paragraphs.Count)

        p.Alignment = wdAlignParagraphLeft

        fmt_range(p.Range, "Arial Black", 24, bold=False, color=DARK_BLUE)



        # --- �Ŧ� x2 ---

        insert_empty_line(doc, "�з���", 20)

        insert_empty_line(doc, "�з���", 20)



        # --- ���D ---

        rng = doc.Range(doc.Content.End - 1, doc.Content.End - 1)

        rng.InsertAfter("        Software Release Note        \r")

        p = doc.Paragraphs(doc.Paragraphs.Count)

        p.Alignment = wdAlignParagraphLeft

        fmt_range(p.Range, "�L�n������", 40, bold=False, color=0)



        # --- �Ŧ� x2 ---

        insert_empty_line(doc, "�з���", 20)

        insert_empty_line(doc, "�з���", 20)



        # --- Handler Type ---

        handler_type = data.get("handler_type", "HT-9045/46 Series")

        rng = doc.Range(doc.Content.End - 1, doc.Content.End - 1)

        rng.InsertAfter("Handler Type�G%s\r" % handler_type)

        p = doc.Paragraphs(doc.Paragraphs.Count)

        p.Alignment = wdAlignParagraphCenter

        fmt_range(p.Range, "�L�n������", 28, bold=False, color=0)



        # --- �Ŧ� x2 ---

        insert_empty_line(doc, "�з���", 20)

        insert_empty_line(doc, "�з���", 20)



        # --- Release Entries ---

        for entry in data.get("entries", []):

            rev = entry.get("rev", "")

            date = entry.get("date", "")

            engineer = entry.get("engineer", "")

            changes = entry.get("changes", [])



            # Rev �� (�ũ��զr����)

            rev_text = "Rev.�G%s  \u25ba Date : %s  \u25ba Engineer�G %s" % (rev, date, engineer)

            rng = doc.Range(doc.Content.End - 1, doc.Content.End - 1)

            rng.InsertAfter(rev_text + "\r")

            p = doc.Paragraphs(doc.Paragraphs.Count)

            p.Alignment = wdAlignParagraphLeft

            fmt_range(p.Range, "�L�n������", 14, bold=True, color=16777215)

            p.Range.Shading.BackgroundPatternColor = DARK_BLUE



            # �ܧ�y�z�� (���r)

            for change in changes:

                rng = doc.Range(doc.Content.End - 1, doc.Content.End - 1)

                rng.InsertAfter(change + "\r")

                p = doc.Paragraphs(doc.Paragraphs.Count)

                p.Alignment = wdAlignParagraphLeft

                fmt_range(p.Range, "�L�n������", 14, bold=False, color=RED)



            # �Ŧ���j

            insert_empty_line(doc, "�з���", 20)



        # --- Footer ---

        footer_rev = data.get("footer_rev", "Rev.4")

        sec = doc.Sections(1)

        ftr = sec.Footers(1)  # wdHeaderFooterPrimary

        ftr.Range.Text = footer_rev

        ftr.Range.ParagraphFormat.Alignment = wdAlignParagraphRight



        # --- Save ---

        abs_path = os.path.abspath(output_path)

        doc.SaveAs(abs_path, wdFormatDocument)

        doc.Close(False)

        print("[OK] ���� Release Note: %s" % abs_path)



    finally:

        word.Quit()





def fmt_range(rng, font_name, font_size, bold=False, color=0):

    """�]�w Range ���r���榡"""

    rng.Font.Name = font_name

    rng.Font.Size = font_size

    rng.Font.Bold = -1 if bold else 0

    rng.Font.Color = color





def insert_empty_line(doc, font_name="�з���", font_size=20):

    """���J�Ŧ�"""

    rng = doc.Range(doc.Content.End - 1, doc.Content.End - 1)

    rng.InsertAfter("\r")

    p = doc.Paragraphs(doc.Paragraphs.Count)

    fmt_range(p.Range, font_name, font_size)





def main():

    if len(sys.argv) < 2:

        print("�Ϊk: python generate_doc.py <input.yaml> [--output path/output.doc]")

        sys.exit(1)



    yaml_path = sys.argv[1]

    output_path = None

    for i, arg in enumerate(sys.argv):

        if arg == "--output" and i + 1 < len(sys.argv):

            output_path = sys.argv[i + 1]



    data = load_yaml(yaml_path)



    if output_path is None:

        customer = data.get("customer", "output")

        output_dir = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(yaml_path))), "output")

        os.makedirs(output_dir, exist_ok=True)

        output_path = os.path.join(output_dir, "%s_Software_Release_Note.doc" % customer)



    create_release_note_doc(data, output_path)





if __name__ == "__main__":

    main()

