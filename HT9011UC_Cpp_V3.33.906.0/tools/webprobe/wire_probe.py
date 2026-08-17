# ===========================================================================
#  tools/webprobe/wire_probe.py
#  Reads the raw publisher tag feed (tcp://127.0.0.1:8046, one JSON object per
#  line -- WebBridge/TcpTagLink.h "WIRE FORMAT") for N seconds and reports
#  what is actually on the wire. Evidence tool for FW campaign gates: the
#  browser story starts here, so this probe deliberately consumes the feed the
#  same way wb_gateway does (NDJSON over loopback TCP).
#
#  AI(W906-FW0) 20260817: rewritten into the tree. The 20260813 originals
#  lived in a session scratchpad and died with the session; DEVLOG of that day
#  records their function, this file re-implements it. stdlib only.
#
#  Usage:  python wire_probe.py [--host 127.0.0.1] [--port 8046] [--seconds 8]
#  Exit:   0 = hello(proto 1) + snapshot both seen; 2 = connect failed;
#          3 = protocol violation (no hello / wrong proto / no snapshot).
#
#  WARNING: never run while ctest is running -- WB_TcpLink binds the same
#  port family and both sides will report false failures.
# ===========================================================================
import argparse
import json
import socket
import sys
import time


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--host', default='127.0.0.1')
    ap.add_argument('--port', type=int, default=8046)
    ap.add_argument('--seconds', type=float, default=8.0)
    args = ap.parse_args()

    try:
        sock = socket.create_connection((args.host, args.port), timeout=5.0)
    except OSError as e:
        print('CONNECT FAIL %s:%d -- %s' % (args.host, args.port, e))
        return 2
    sock.settimeout(0.5)

    deadline = time.monotonic() + args.seconds
    buf = b''
    frames = {}          # type -> count
    hello = None
    snapshot_tags = {}
    changed = {}         # tag -> distinct values seen in patches
    while time.monotonic() < deadline:
        try:
            chunk = sock.recv(65536)
        except socket.timeout:
            continue
        except OSError:
            break
        if not chunk:
            break
        buf += chunk
        while b'\n' in buf:
            line, buf = buf.split(b'\n', 1)
            if not line.strip():
                continue
            try:
                msg = json.loads(line.decode('utf-8'))
            except ValueError:
                frames['UNPARSEABLE'] = frames.get('UNPARSEABLE', 0) + 1
                continue
            t = msg.get('type', '?')
            frames[t] = frames.get(t, 0) + 1
            if t == 'hello' and hello is None:
                hello = msg
            elif t == 'snapshot':
                snapshot_tags = dict(msg.get('data', {}))
            elif t == 'patch':
                for k, v in (msg.get('data') or {}).items():
                    changed.setdefault(k, set()).add(json.dumps(v))
    sock.close()

    # --- self-check first (a measurement tool must validate a known-true
    #     sample before its numbers are believed -- PT-W10 lesson) ---
    if hello is None:
        print('PROTOCOL FAIL: no hello frame in %.1fs' % args.seconds)
        print('frames seen:', frames)
        return 3
    if hello.get('proto') != 1:
        print('PROTOCOL FAIL: hello proto=%r, expected 1' % hello.get('proto'))
        return 3
    if 'snapshot' not in frames:
        print('PROTOCOL FAIL: hello ok but no snapshot in %.1fs' % args.seconds)
        return 3

    nonnull = {k: v for k, v in snapshot_tags.items() if v is not None}
    print('WIRE PROBE %s:%d over %.1fs' % (args.host, args.port, args.seconds))
    print('  frames        : %s' % json.dumps(frames, sort_keys=True))
    print('  snapshot tags : %d total, %d non-null' % (len(snapshot_tags), len(nonnull)))
    print('  moving tags   : %d (%s)' % (
        len(changed), ', '.join(sorted(changed)[:12]) + ('...' if len(changed) > 12 else '')))
    for k in sorted(nonnull)[:20]:
        print('    %-28s = %s' % (k, json.dumps(nonnull[k], ensure_ascii=False)))
    if len(nonnull) > 20:
        print('    ... and %d more non-null' % (len(nonnull) - 20))
    return 0


if __name__ == '__main__':
    sys.exit(main())
