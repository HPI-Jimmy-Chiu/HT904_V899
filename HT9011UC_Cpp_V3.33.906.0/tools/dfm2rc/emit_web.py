# ===========================================================================
#  tools/dfm2rc/emit_web.py -- IR -> per-form web layout JSON.
#
#  AI(W906-FW2) 20260817: new emitter for the FW campaign
#  (docs/DFM2WEB_CAMPAIGN_PLAN.md 4-FW-2). The .dfm is the layout source of
#  truth; dfm_parse.py already turned all 133 forms into IR
#  (ir_out/<form>.dfm.ir.json, cp950 decoded at parse time). This emitter is
#  deliberately a PROJECTION, not a translation: it re-shapes the IR for the
#  browser and drops nothing it cannot name.
#
#  Output: web/forms/<stem>.layout.json  (schema "fw-weblayout/1")
#    - widget order == IR node order  (z-order faithful)
#    - properties flattened {type,value} -> value, EXCEPT type==BINARY
#      (Glyph.Data / Picture.Data blobs), which are dropped by NAME into
#      "binary_dropped" so the omission is visible, never silent.
#    - events carried through verbatim but the renderer never wires them:
#      read-only is the campaign's hard boundary (plan 7).
#
#  Determinism: sorted keys, LF, single trailing newline (same gate-4
#  conventions as the sibling emitters). `--check` regenerates to a temp file
#  and byte-compares against the committed output -- the same G7 idempotency
#  idea run_b1d.py applies to the rc/layout trees.
#
#  Usage:
#    python emit_web.py <form-stem> [<form-stem> ...]     emit/overwrite
#    python emit_web.py --check <form-stem> [...]         verify, write nothing
#  Exit: 0 ok; 1 check mismatch; 2 usage/missing IR.
# ===========================================================================
import json
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
IR_DIR = os.path.join(HERE, 'ir_out')
# repo layout: <repo>/HT9011UC_Cpp_V3.33.906.0/tools/dfm2rc -> <repo>/web/forms
OUT_DIR = os.path.normpath(os.path.join(HERE, '..', '..', '..', 'web', 'forms'))


def project_node(n):
    props = {}
    dropped = []
    for k in sorted((n.get('properties') or {}).keys()):
        p = n['properties'][k]
        if isinstance(p, dict) and p.get('type') == 'BINARY':
            dropped.append(k)
            continue
        props[k] = p.get('value') if isinstance(p, dict) and 'value' in p else p
    g = n.get('geometry') or {}
    w = {
        'path':   n.get('path'),
        'name':   n.get('name'),
        'cls':    n.get('class'),
        'kind':   n.get('kind'),
        'parent': n.get('parent_path'),
        'rect':   {'x': g.get('left'), 'y': g.get('top'),
                   'w': g.get('width'), 'h': g.get('height')},
        'align':  n.get('align'),
        'props':  props,
        'events': n.get('events') or {},
    }
    if dropped:
        w['binary_dropped'] = dropped
    return w


def emit_text(stem):
    ir_path = os.path.join(IR_DIR, *(stem + '.dfm.ir.json').split('/'))
    if not os.path.isfile(ir_path):
        raise FileNotFoundError(ir_path)
    ir = json.load(open(ir_path, encoding='utf-8'))
    nodes = ir['nodes']
    root = nodes[0]
    rprops = root.get('properties') or {}

    def rprop(name, default=None):
        p = rprops.get(name)
        return p.get('value') if isinstance(p, dict) else default

    doc = {
        'schema': 'fw-weblayout/1',
        'source_dfm': ir.get('source_dfm'),
        'form': ir.get('form_name'),
        'form_class': ir.get('form_class'),
        'caption': rprop('Caption'),
        'canvas': {'w': rprop('ClientWidth'), 'h': rprop('ClientHeight')},
        'widgets': [project_node(n) for n in nodes],
    }
    # exactly one trailing newline: gate-4 convention shared with the siblings
    return json.dumps(doc, ensure_ascii=False, indent=1, sort_keys=True) + '\n'


def main(argv):
    args = [a for a in argv[1:]]
    check = False
    if args and args[0] == '--check':
        check = True
        args = args[1:]
    if not args:
        print(__doc__ or 'usage: emit_web.py [--check] <form-stem> [...]',
              file=sys.stderr)
        return 2
    bad = 0
    for stem in args:
        stem = stem.replace('\\', '/')
        try:
            text = emit_text(stem)
        except FileNotFoundError as e:
            print('MISSING IR: %s' % e)
            return 2
        # subdir forms (e.g. Monitor/xxx) keep their directory: flattening the
        # path into the filename would make the stem un-derivable, because
        # real form names contain underscores (uTemp_Set).
        out = os.path.join(OUT_DIR, *(stem + '.layout.json').split('/'))
        if check:
            if not os.path.isfile(out):
                print('CHECK FAIL %s: committed file missing (%s)' % (stem, out))
                bad += 1
                continue
            cur = open(out, encoding='utf-8', newline='').read()
            if cur != text:
                print('CHECK FAIL %s: regenerated text differs from %s' % (stem, out))
                bad += 1
            else:
                print('CHECK OK   %s (%d bytes)' % (stem, len(text)))
        else:
            os.makedirs(os.path.dirname(out), exist_ok=True)
            with open(out, 'w', encoding='utf-8', newline='\n') as fh:
                fh.write(text)
            print('emitted %s (%d widgets, %d bytes)'
                  % (out, len(json.loads(text)['widgets']), len(text)))
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
