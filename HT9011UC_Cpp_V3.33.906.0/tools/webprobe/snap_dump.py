# =============================================================================
#  tools/webprobe/snap_dump.py -- dump every key in the first `{"type":
#  "snapshot"}` frame a wb_serve/wb_gateway WS endpoint sends on connect.
#
#  AI(W906-FW1-recon) 20260819: FW-1 tag-wiring recon (crosswalk between
#  browser-bound tag names and what wb_serve actually publishes). Reuses
#  cmd_probe.py's ws_handshake()/read_frames() verbatim (imported, not
#  reimplemented -- see tools/webprobe/README.md posture: no second frame
#  parser in this directory) instead of hand-rolling another WS client.
#
#  Usage:
#      python snap_dump.py [--host 127.0.0.1] [--port 8061] [--path /ht9045]
#                           [--seconds 15] [--out snapshot.json]
#
#  Prints: total key count, non-null count, then every key sorted with its
#  value (or "null"). With --out, also writes the raw {tag: value} dict as
#  JSON for downstream diffing.
#
#  Exit: 0 = snapshot frame captured; 2 = connect/handshake failed;
#        5 = no snapshot frame before deadline.
# =============================================================================
import argparse
import json
import os
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from cmd_probe import ws_handshake, read_frames  # noqa: E402  (reuse, not reimplement)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--host', default='127.0.0.1')
    ap.add_argument('--port', type=int, default=8061)
    ap.add_argument('--path', default='/ht9045')
    ap.add_argument('--seconds', type=float, default=15.0)
    ap.add_argument('--out', default=None, help='optional path to dump raw {tag: value} JSON')
    args = ap.parse_args()

    deadline = time.monotonic() + args.seconds
    try:
        sock, leftover = ws_handshake(args.host, args.port, args.path, deadline)
    except (OSError, RuntimeError) as e:
        print('HANDSHAKE FAIL: %s' % e)
        return 2
    print('ws handshake ok (%s:%d%s)' % (args.host, args.port, args.path))

    snap = None
    for op, payload in read_frames(sock, leftover, deadline):
        if op != 1:
            continue
        try:
            msg = json.loads(payload.decode('utf-8', 'replace'))
        except ValueError:
            continue
        if msg.get('type') == 'snapshot':
            snap = dict(msg.get('data', {}))
            break
    sock.close()

    if snap is None:
        print('SNAP FAIL: no snapshot frame before deadline (%.1fs)' % args.seconds)
        return 5

    nonnull = {k: v for k, v in snap.items() if v is not None}
    print('snapshot tags: %d total, %d non-null, %d null'
          % (len(snap), len(nonnull), len(snap) - len(nonnull)))
    print('')
    for k in sorted(snap.keys()):
        v = snap[k]
        print('%-40s %s' % (k, 'null' if v is None else json.dumps(v)))

    if args.out:
        with open(args.out, 'w', encoding='utf-8') as f:
            json.dump(snap, f, ensure_ascii=False, indent=2, sort_keys=True)
        print('\nwrote raw snapshot dict -> %s' % args.out)

    return 0


if __name__ == '__main__':
    sys.exit(main())
