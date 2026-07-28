# -*- coding: utf-8 -*-
"""
res_parse.py -- W7-B1b: pure-Python Win32 .RES container + DLGTEMPLATEEX /
DLGITEMTEMPLATEEX binary reader, for gate G6.

AI(W906-W7-B1b) 20260728: new file. Implements the "G6 是不需 MFC 的最強
oracle" gate from docs/W7_UI_ARCHITECTURE_PLAN.md S6-W7-B1b: after rc.exe
compiles a generated .rc into a .res, this module decodes the *compiled
binary* back into plain Python structures so check_g6.py can diff it
against the IR/emitter's own expectations -- catching bugs that only show
up post-compilation (duplicate IDs, mis-escaped captions, dropped/garbled
templates), which is exactly the class of bug the prototype's
DUP_ID_IN_RES(4294967295) incident (plan S6-B1b) was.

FORMAT NOTES (reverse-engineered against a real rc.exe here today, not
copied from a spec sheet without checking -- see the smoke-test probes
under the session scratchpad for the empirical verification of each
field's byte offset/width, including the sz_Or_Ord variable-length
encoding and the fact rc.exe folds literal class-name strings like
"Static"/"Edit"/"Button" into PREDEFINED_CLASS_ORDINALS atoms rather than
keeping them as strings):

RES container: a flat sequence of resource entries. Each entry is
  DWORD DataSize; DWORD HeaderSize;
  sz_Or_Ord Type; sz_Or_Ord Name;               (variable length)
  <pad to DWORD boundary>
  DWORD DataVersion; WORD MemoryFlags; WORD LanguageId;
  DWORD Version; DWORD Characteristics;
  <Data: DataSize bytes>
  <pad to DWORD boundary>
`HeaderSize` already accounts for the Type/Name fields *and* their
trailing pad, so `data_start = entry_start + HeaderSize` is exact --
verified against a real compiled .res (see scratch smoke test): the very
first entry in every .res is the standard zero/zero/DataSize=0 lead-in,
which this parser must skip like any other entry (it naturally does, as
DataSize=0 just yields an empty data blob).

sz_Or_Ord (used for Type/Name/menu/windowClass/title fields): read a WORD.
  0x0000            -> None (absent)
  0xFFFF, then WORD  -> ('ORD', value) -- a predefined atom/ordinal
  otherwise          -> that WORD is the *first* UTF-16LE code unit of a
                        null-terminated string; keep reading WORDs until a
                        0x0000 terminator -> ('STR', text)

DLGTEMPLATEEX (data of an RT_DIALOG=5 resource emitted by a DIALOGEX
statement -- rc.exe always emits the *extended* (signature 0xFFFF)
format for a `DIALOGEX` keyword, verified empirically):
  WORD dlgVer(=1); WORD signature(=0xFFFF); DWORD helpID;
  DWORD exStyle; DWORD style; WORD cDlgItems;
  SHORT x,y,cx,cy; sz_Or_Ord menu; sz_Or_Ord windowClass; sz_Or_Ord title;
  IF style & DS_SETFONT(0x40):
      WORD pointsize; WORD weight; BYTE italic; BYTE charset;
      sz typeface (null-terminated string, no ordinal form);
  <pad to DWORD boundary>
  then cDlgItems x DLGITEMTEMPLATEEX:
      DWORD helpID; DWORD exStyle; DWORD style;
      SHORT x,y,cx,cy; DWORD id;          (NOTE: id is DWORD here, not WORD)
      sz_Or_Ord windowClass; sz_Or_Ord title;
      WORD extraCount; BYTE extraData[extraCount];
      <pad to DWORD boundary>
"""
from __future__ import print_function

import struct


def _align4(x):
    return (x + 3) & ~3


def read_sz_or_ord(buf, off):
    (w,) = struct.unpack_from('<H', buf, off)
    off += 2
    if w == 0x0000:
        return None, off
    if w == 0xFFFF:
        (ordv,) = struct.unpack_from('<H', buf, off)
        off += 2
        return ('ORD', ordv), off
    chars = [w]
    while True:
        (c,) = struct.unpack_from('<H', buf, off)
        off += 2
        if c == 0:
            break
        chars.append(c)
    return ('STR', u''.join(_unichr(c) for c in chars)), off


try:
    _unichr = unichr  # type: ignore  # python2
except NameError:
    _unichr = chr  # python3


class ResEntry(object):
    __slots__ = ('type', 'name', 'lang', 'data')

    def __init__(self, type_, name, lang, data):
        self.type = type_
        self.name = name
        self.lang = lang
        self.data = data


def parse_res_file(path):
    """Return a list of ResEntry for every resource in a compiled .res
    (including the standard leading zero-size header entry -- callers
    filter for what they need, e.g. type == ('ORD', 5) for RT_DIALOG)."""
    data = open(path, 'rb').read()
    return parse_res_bytes(data)


RT_DIALOG = 5


def parse_res_bytes(data):
    n = len(data)
    off = 0
    entries = []
    while off + 8 <= n:
        data_size, header_size = struct.unpack_from('<II', data, off)
        entry_start = off
        p = off + 8
        type_, p = read_sz_or_ord(data, p)
        name, p = read_sz_or_ord(data, p)
        data_start = entry_start + header_size
        if data_start < p or data_start + data_size > n:
            raise ValueError(
                'malformed .res entry at offset %d: header_size=%d would '
                'place data at %d (parsed fields ended at %d), file len=%d'
                % (entry_start, header_size, data_start, p, n))
        # The 16-byte {DataVersion(4), MemoryFlags(2), LanguageId(2),
        # Version(4), Characteristics(4)} block sits immediately before
        # data_start; LanguageId is therefore at data_start-10.
        lang = struct.unpack_from('<H', data, data_start - 10)[0] if data_start >= 10 else 0
        res_data = data[data_start:data_start + data_size]
        entries.append(ResEntry(type_, name, lang, res_data))
        off = _align4(data_start + data_size)
    return entries


class DlgItem(object):
    __slots__ = ('help_id', 'ex_style', 'style', 'x', 'y', 'cx', 'cy',
                 'id', 'window_class', 'title')

    def __init__(self, **kw):
        for k, v in kw.items():
            setattr(self, k, v)


class DlgTemplate(object):
    __slots__ = ('dlg_ver', 'signature', 'help_id', 'ex_style', 'style',
                 'x', 'y', 'cx', 'cy', 'menu', 'window_class', 'title',
                 'font_pointsize', 'font_weight', 'font_italic',
                 'font_charset', 'font_typeface', 'items')

    def __init__(self, **kw):
        for k, v in kw.items():
            setattr(self, k, v)


DS_SETFONT = 0x40


def parse_dlgtemplateex(buf):
    """Decode one RT_DIALOG resource's data blob (produced by a DIALOGEX
    statement) into a DlgTemplate. Raises ValueError if the signature
    isn't the extended (0xFFFF) form -- that would mean rc.exe fell back
    to classic DLGTEMPLATE, which this pipeline never expects to hit
    since every generated dialog uses the DIALOGEX keyword."""
    p = 0
    dlg_ver, signature = struct.unpack_from('<HH', buf, p)
    p += 4
    if signature != 0xFFFF:
        raise ValueError('unexpected DLGTEMPLATE signature %#x (expected extended 0xFFFF)' % signature)
    help_id, ex_style, style = struct.unpack_from('<III', buf, p)
    p += 12
    (c_items,) = struct.unpack_from('<H', buf, p)
    p += 2
    x, y, cx, cy = struct.unpack_from('<hhhh', buf, p)
    p += 8
    menu, p = read_sz_or_ord(buf, p)
    window_class, p = read_sz_or_ord(buf, p)
    title, p = read_sz_or_ord(buf, p)
    font_pointsize = font_weight = font_italic = font_charset = font_typeface = None
    if style & DS_SETFONT:
        font_pointsize, font_weight = struct.unpack_from('<HH', buf, p)
        p += 4
        font_italic, font_charset = struct.unpack_from('<BB', buf, p)
        p += 2
        font_typeface, p = read_sz_or_ord(buf, p)
    p = _align4(p)

    items = []
    for _ in range(c_items):
        help_id_i, ex_style_i, style_i = struct.unpack_from('<III', buf, p)
        p += 12
        xi, yi, cxi, cyi = struct.unpack_from('<hhhh', buf, p)
        p += 8
        (id_,) = struct.unpack_from('<I', buf, p)
        p += 4
        wclass, p = read_sz_or_ord(buf, p)
        wtitle, p = read_sz_or_ord(buf, p)
        (extra_count,) = struct.unpack_from('<H', buf, p)
        p += 2
        p += extra_count
        p = _align4(p)
        items.append(DlgItem(help_id=help_id_i, ex_style=ex_style_i, style=style_i,
                              x=xi, y=yi, cx=cxi, cy=cyi, id=id_,
                              window_class=wclass, title=wtitle))

    return DlgTemplate(dlg_ver=dlg_ver, signature=signature, help_id=help_id,
                        ex_style=ex_style, style=style, x=x, y=y, cx=cx, cy=cy,
                        menu=menu, window_class=window_class, title=title,
                        font_pointsize=font_pointsize, font_weight=font_weight,
                        font_italic=font_italic, font_charset=font_charset,
                        font_typeface=font_typeface, items=items)


def dialogs_in_res(path):
    """Convenience: parse a .res file and return {name_str_or_ord:
    DlgTemplate} for every RT_DIALOG entry."""
    out = {}
    for e in parse_res_file(path):
        if e.type == ('ORD', RT_DIALOG):
            tmpl = parse_dlgtemplateex(e.data)
            key = e.name[1] if e.name and e.name[0] == 'STR' else e.name
            out[key] = tmpl
    return out
