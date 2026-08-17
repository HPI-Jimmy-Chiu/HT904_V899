# ===========================================================================
#  tools/webprobe/system_guard.py
#  Proof-by-measurement that a run (ctest, e2e probe, pump) did or did not
#  write the PRODUCTION-shared D:\HT9045\system tree. This replaces "prove a
#  negative by reading 25k lines of state machine" with before/after facts --
#  the exact method that caught the BinCount.txt rewrite on 20260813.
#
#  AI(W906-FW0) 20260817: new tool (the 20260813 ad-hoc script died with its
#  session scratchpad). stdlib only.
#
#  Usage:
#    python system_guard.py snapshot <out.json> [--root D:\HT9045\system]
#    python system_guard.py compare  <before.json> <after.json>
#  Exit (compare): 0 = identical (bytes AND mtime); 1 = any drift.
#  Note: mtime drift with identical MD5 is still reported -- "content same
#  but rewritten" is exactly the BinCount symptom and must stay visible.
# ===========================================================================
import argparse
import hashlib
import json
import os
import sys


def snapshot(root):
    entries = {}
    for dp, dn, fn in os.walk(root):
        dn[:] = [d for d in dn if d.lower() != '.svn']
        for f in fn:
            p = os.path.join(dp, f)
            rel = os.path.relpath(p, root)
            try:
                st = os.stat(p)
                h = hashlib.md5()
                with open(p, 'rb') as fh:
                    for chunk in iter(lambda: fh.read(1 << 20), b''):
                        h.update(chunk)
                entries[rel] = {'size': st.st_size,
                                'mtime_ns': st.st_mtime_ns,
                                'md5': h.hexdigest()}
            except OSError as e:
                entries[rel] = {'error': str(e)}
    return entries


def main():
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest='mode', required=True)
    s1 = sub.add_parser('snapshot')
    s1.add_argument('out')
    s1.add_argument('--root', default=r'D:\HT9045\system')
    s2 = sub.add_parser('compare')
    s2.add_argument('before')
    s2.add_argument('after')
    args = ap.parse_args()

    if args.mode == 'snapshot':
        entries = snapshot(args.root)
        with open(args.out, 'w', encoding='utf-8') as fh:
            json.dump({'root': args.root, 'files': entries}, fh, indent=1, sort_keys=True)
        print('snapshot: %d files under %s -> %s' % (len(entries), args.root, args.out))
        return 0

    with open(args.before, encoding='utf-8') as fh:
        before = json.load(fh)['files']
    with open(args.after, encoding='utf-8') as fh:
        after = json.load(fh)['files']
    drift = []
    for rel in sorted(set(before) | set(after)):
        b, a = before.get(rel), after.get(rel)
        if b is None:
            drift.append(('ADDED', rel, ''))
        elif a is None:
            drift.append(('REMOVED', rel, ''))
        elif b.get('md5') != a.get('md5') or b.get('size') != a.get('size'):
            drift.append(('CONTENT', rel, 'md5 %s -> %s' % (b.get('md5'), a.get('md5'))))
        elif b.get('mtime_ns') != a.get('mtime_ns'):
            drift.append(('REWRITTEN-SAME-BYTES', rel,
                          'mtime %s -> %s' % (b.get('mtime_ns'), a.get('mtime_ns'))))
    if not drift:
        print('IDENTICAL: %d files, bytes and mtime all unchanged' % len(before))
        return 0
    print('DRIFT: %d of %d files' % (len(drift), len(set(before) | set(after))))
    for kind, rel, detail in drift:
        print('  %-20s %s  %s' % (kind, rel, detail))
    return 1


if __name__ == '__main__':
    sys.exit(main())
