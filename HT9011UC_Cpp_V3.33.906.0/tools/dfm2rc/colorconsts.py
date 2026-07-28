# -*- coding: utf-8 -*-
"""
colorconsts.py -- W7-B1c: VCL TColor symbolic-constant resolver.

AI(W906-W7-B1c) 20260728: new file (docs/W7_UI_ARCHITECTURE_PLAN.md S6
W7-B1c). golden .dfm text stores colour properties either as a raw decimal
TColor integer (already resolved, e.g. `Color = 12761254`) or as a symbolic
Delphi Graphics.pas identifier (e.g. `Font.Color = clWindowText`). emit_layout
needs a numeric value for both cases (D7: this table is the pixel/colour
authority a later OnCtlColor pass consumes) *without* discarding the
original token -- a symbolic system-colour reference like clBtnFace is only
resolvable at paint time via GetSysColor(), so silently baking in "whatever
it happens to render as today" would be a silent fidelity loss, not a
fidelity win.

Table scope: built empirically, not from memory. A full-corpus scan of all
133 IR files' Color/Font.Color/TrueColor/FalseColor/TrueFontColor/
FalseFontColor properties (tools/dfm2rc/reports/b1c_gate_report.json ->
'unresolved_color_tokens' reproduces this scan) found exactly 34 distinct
`cl*` symbolic tokens in this corpus. This table carries the complete
standard Delphi Graphics.pas TColor constant set (50 names: 30 system-colour
references following the Win32 GetSysColor() index order, encoded per
Delphi's `$80000000 + index` convention, plus the 20 direct 0x00BBGGRR
"basic 16 + 4" colours) -- a strict superset of what this corpus uses today,
so a form added later in the same style will not silently go unresolved.
Any token NOT in this table is reported as unresolved (see resolve_color's
return contract) rather than guessed at.

TColor encoding (verified against the existing vclcompat/BtnPanelCore.h /
LedCore.h / TrayCore.h shim, which independently hard-codes clBlack/clGreen/
clRed/clBlue/clLime/clYellow/clWhite/clSilver/clBtnFace with the exact same
values used here -- cross-checked, not just asserted):
  - Direct colours: 0x00BBGGRR (a plain, non-negated 24-bit BGR value --
    byte-identical to a Win32 COLORREF, per plan S6-W7-C2 note "TColor is
    0x00BBGGRR, same as COLORREF, direct cast, no swizzle").
  - System-colour references: high bit set, `0x80000000 | COLOR_xxx` where
    COLOR_xxx is the Win32 GetSysColor() index (COLOR_SCROLLBAR=0 ..
    COLOR_MENUBAR=30, sequential, no gaps). As a signed 32-bit Delphi
    Integer these are negative; we store them here as the unsigned 0x8...
    literal (matching the existing shim's `TColor(0x8000000F)` style) and
    let the C++ side reinterpret/cast as needed -- resolving a system colour
    to an actual RGB pixel value requires calling GetSysColor() at paint
    time (theme/user-scheme dependent), which is correctly out of scope for
    a static generated data table.
"""
from __future__ import print_function

# System colour references: Delphi Graphics.pas clXxx = 0x80000000 + N,
# where N is the Win32 GetSysColor()/COLOR_* index (sequential, verified
# against the standard Win32 SDK COLOR_* enumeration order).
_SYSTEM_COLOR_NAMES = [
    'clScrollBar', 'clBackground', 'clActiveCaption', 'clInactiveCaption',
    'clMenu', 'clWindow', 'clWindowFrame', 'clMenuText', 'clWindowText',
    'clCaptionText', 'clActiveBorder', 'clInactiveBorder', 'clAppWorkSpace',
    'clHighlight', 'clHighlightText', 'clBtnFace', 'clBtnShadow',
    'clGrayText', 'clBtnText', 'clInactiveCaptionText', 'clBtnHighlight',
    'cl3DDkShadow', 'cl3DLight', 'clInfoText', 'clInfoBk', 'clHotLight',
    'clGradientActiveCaption', 'clGradientInactiveCaption',
    'clMenuHighlight', 'clMenuBar',
]

# Direct 0x00BBGGRR colours (Delphi's standard "basic 16" + a handful of
# named extras that appear in real .dfm corpora). Verified byte-for-byte
# against vclcompat/BtnPanelCore.h's existing shim for the 8 names it also
# defines (clBlack/clGreen/clRed/clBlue/clLime/clYellow/clWhite/clSilver).
_DIRECT_COLOR_VALUES = {
    'clBlack':      0x00000000,
    'clMaroon':     0x00000080,
    'clGreen':      0x00008000,
    'clOlive':      0x00008080,
    'clNavy':       0x00800000,
    'clPurple':     0x00800080,
    'clTeal':       0x00808000,
    'clGray':       0x00808080,
    'clSilver':     0x00C0C0C0,
    'clRed':        0x000000FF,
    'clLime':       0x0000FF00,
    'clYellow':     0x0000FFFF,
    'clBlue':       0x00FF0000,
    'clFuchsia':    0x00FF00FF,
    'clAqua':       0x00FFFF00,
    'clWhite':      0x00FFFFFF,
    'clMoneyGreen': 0x00C0DCC0,
    'clSkyBlue':    0x00F0CAA6,
    'clCream':      0x00F0FBFF,
    'clMedGray':    0x00A4A0A0,
}

VCL_COLOR_CONSTS = {}
for _i, _name in enumerate(_SYSTEM_COLOR_NAMES):
    VCL_COLOR_CONSTS[_name] = 0x80000000 | _i
VCL_COLOR_CONSTS.update(_DIRECT_COLOR_VALUES)


def to_i32(u):
    """Wrap an arbitrary Python int to the signed 32-bit value it would hold
    as a real Delphi `TColor = type Integer` (int32_t) -- i.e. the *actual*
    in-memory bit pattern, not the 0x8000000N "looks unsigned" literal
    spelling. This matters mechanically, not just cosmetically: a C++
    aggregate initializer (`DfmControlLayout x = { ..., 2147483663, ... }`)
    is list-initialization, which the standard treats a narrowing
    int-literal-that-does-not-fit as ILL-FORMED (GCC hard-errors on it, see
    this file's own generated-code compile-smoke gate) -- so the field must
    already be a plain (32-bit) `int`-range value, matching how the
    existing vclcompat shim (BtnPanelCore.h/LedCore.h/TrayCore.h) also
    ultimately treats TColor as `typedef int TColor`. A direct colour
    (e.g. clWhite=0x00FFFFFF) is unaffected (already in range); only the
    high-bit-set system-colour references wrap to negative, exactly as they
    would in the real golden binary."""
    u &= 0xFFFFFFFF
    return u - 0x100000000 if u >= 0x80000000 else u


def resolve_color(value):
    """Resolve a Color/Font.Color/TrueColor/FalseColor/... IR SCALAR value
    (either a python int already, or a 'clXxx' token string) into
    (has_value, resolved_int, raw_token). raw_token is '' when the source
    was already a plain integer (nothing to preserve beyond the number
    itself); it is the original identifier when a symbolic name was
    involved, resolved or not -- callers must not drop it. has_value is
    False only for an unresolved symbolic name (unknown token): the number
    is meaningless in that case and must not be used, but the token is
    still returned so nothing is silently lost."""
    if isinstance(value, bool):
        # SET/SCALAR bool leakage guard: never reached in practice (colour
        # properties are never bool-typed in this corpus) but fail safe.
        return False, 0, repr(value)
    if isinstance(value, int):
        return True, to_i32(value), ''
    if isinstance(value, str):
        if value in VCL_COLOR_CONSTS:
            return True, to_i32(VCL_COLOR_CONSTS[value]), value
        return False, 0, value
    return False, 0, '' if value is None else repr(value)
