# ===========================================================================
#  tools/webprobe/ws_probe.py
#  Verifies the chain the BROWSER actually uses: HTTP static serve on :8045,
#  then a real RFC 6455 WebSocket handshake on /ht9045 and N seconds of
#  frames. Hand-rolled client on purpose (no libraries, no mocks) -- same
#  posture as the 20260813 verification: if this passes, a browser gets the
#  same bytes.
#
#  AI(W906-FW0) 20260817: rewritten into the tree; the scratchpad original
#  died with its session (function recorded in DEVLOG 20260813).
#
#  Usage:  python ws_probe.py [--host 127.0.0.1] [--port 8045] [--seconds 8]
#  Exit:   0 = static 200 + WS 101 (accept key independently recomputed and
#              matched) + snapshot frame seen
#          2 = connect failed; 3 = HTTP static failed; 4 = handshake failed;
#          5 = no snapshot frame.
#
#  WARNING: never run while ctest is running (WB_TcpLink port clash).
# ===========================================================================
import argparse
import base64
import hashlib
import json
import os
import socket
import sys
import time

WS_GUID = '258EAFA5-E914-47DA-95CA-C5AB0DC85B11'


def http_get(host, port, path):
    s = socket.create_connection((host, port), timeout=5.0)
    s.sendall(('GET %s HTTP/1.1\r\nHost: %s:%d\r\nConnection: close\r\n\r\n'
               % (path, host, port)).encode())
    data = b''
    s.settimeout(5.0)
    try:
        while True:
            c = s.recv(65536)
            if not c:
                break
            data += c
    except socket.timeout:
        pass
    s.close()
    head, _, body = data.partition(b'\r\n\r\n')
    lines = head.split(b'\r\n')
    status = lines[0].decode('latin-1') if lines else ''
    headers = {}
    for ln in lines[1:]:
        k, _, v = ln.decode('latin-1').partition(':')
        headers[k.strip().lower()] = v.strip()
    return status, headers, body


def read_exact(sock, n, deadline):
    out = b''
    while len(out) < n:
        if time.monotonic() > deadline:
            raise socket.timeout()
        try:
            c = sock.recv(n - len(out))
        except socket.timeout:
            continue
        if not c:
            raise ConnectionError('peer closed')
        out += c
    return out


def read_ws_frame(sock, deadline):
    """Returns (opcode, payload). Server->client frames MUST be unmasked
    (RFC 6455 5.1) -- a masked one is reported as a protocol violation."""
    h = read_exact(sock, 2, deadline)
    fin = h[0] & 0x80
    opcode = h[0] & 0x0F
    masked = h[1] & 0x80
    ln = h[1] & 0x7F
    if ln == 126:
        ln = int.from_bytes(read_exact(sock, 2, deadline), 'big')
    elif ln == 127:
        ln = int.from_bytes(read_exact(sock, 8, deadline), 'big')
    if masked:
        raise AssertionError('server frame is masked -- RFC 6455 violation')
    payload = read_exact(sock, ln, deadline)
    if not fin:
        raise AssertionError('fragmented server frame -- not expected on this link')
    return opcode, payload


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--host', default='127.0.0.1')
    ap.add_argument('--port', type=int, default=8045)
    ap.add_argument('--path', default='/ht9045')
    ap.add_argument('--seconds', type=float, default=8.0)
    args = ap.parse_args()

    # --- 1. static serve (the ES module loading depends on the js MIME) ---
    try:
        status, headers, body = http_get(args.host, args.port, '/')
    except OSError as e:
        print('CONNECT FAIL %s:%d -- %s' % (args.host, args.port, e))
        return 2
    ok_index = ' 200 ' in status + ' '
    print('GET /            : %s (%d bytes)' % (status, len(body)))
    status_js, headers_js, body_js = http_get(args.host, args.port, '/js/main.js')
    ctype = headers_js.get('content-type', '')
    print('GET /js/main.js  : %s (%d bytes, Content-Type: %s)'
          % (status_js, len(body_js), ctype or '<none>'))
    if not ok_index or ' 200 ' not in status_js + ' ':
        return 3
    if 'javascript' not in ctype:
        print('STATIC FAIL: js served without a javascript Content-Type -- ES modules will not load')
        return 3

    # --- 2. real WS handshake, accept key recomputed independently ---
    key = base64.b64encode(os.urandom(16)).decode()
    expect = base64.b64encode(hashlib.sha1((key + WS_GUID).encode()).digest()).decode()
    sock = socket.create_connection((args.host, args.port), timeout=5.0)
    sock.sendall(('GET %s HTTP/1.1\r\nHost: %s:%d\r\nUpgrade: websocket\r\n'
                  'Connection: Upgrade\r\nSec-WebSocket-Key: %s\r\n'
                  'Sec-WebSocket-Version: 13\r\n\r\n'
                  % (args.path, args.host, args.port, key)).encode())
    sock.settimeout(0.5)
    deadline = time.monotonic() + args.seconds
    buf = b''
    while b'\r\n\r\n' not in buf:
        if time.monotonic() > deadline:
            print('HANDSHAKE FAIL: no response before deadline')
            return 4
        try:
            c = sock.recv(4096)
        except socket.timeout:
            continue
        if not c:
            print('HANDSHAKE FAIL: peer closed during handshake')
            return 4
        buf += c
    head, _, rest = buf.partition(b'\r\n\r\n')
    lines = head.split(b'\r\n')
    print('WS %s        : %s' % (args.path, lines[0].decode('latin-1')))
    if b'101' not in lines[0]:
        return 4
    accept = ''
    for ln in lines[1:]:
        k, _, v = ln.decode('latin-1').partition(':')
        if k.strip().lower() == 'sec-websocket-accept':
            accept = v.strip()
    if accept != expect:
        print('HANDSHAKE FAIL: Sec-WebSocket-Accept %r != independently computed %r'
              % (accept, expect))
        return 4
    print('accept key       : matches independent SHA1/base64 recompute')

    # --- 3. frames ---
    frames = {}
    snapshot_tags = {}
    moving = {}
    # anything already buffered after the header is frame bytes
    pending = rest

    class _Pre:
        def __init__(self, first, s):
            self.first = first
            self.s = s
        def recv(self, n):
            if self.first:
                out, self.first = self.first[:n], self.first[n:]
                return bytes(out)
            return self.s.recv(n)

    src = _Pre(bytearray(pending), sock)
    while time.monotonic() < deadline:
        try:
            opcode, payload = read_ws_frame(src, deadline)
        except socket.timeout:
            break
        except (ConnectionError, AssertionError) as e:
            print('frame read stop: %s' % e)
            break
        if opcode != 1:
            frames['opcode%d' % opcode] = frames.get('opcode%d' % opcode, 0) + 1
            continue
        try:
            msg = json.loads(payload.decode('utf-8'))
        except ValueError:
            frames['UNPARSEABLE'] = frames.get('UNPARSEABLE', 0) + 1
            continue
        t = msg.get('type', '?')
        frames[t] = frames.get(t, 0) + 1
        if t == 'snapshot':
            snapshot_tags = dict(msg.get('data', {}))
        elif t == 'patch':
            for k in (msg.get('data') or {}):
                moving[k] = moving.get(k, 0) + 1
    sock.close()

    nonnull = {k: v for k, v in snapshot_tags.items() if v is not None}
    print('  frames        : %s' % json.dumps(frames, sort_keys=True))
    print('  snapshot tags : %d total, %d non-null' % (len(snapshot_tags), len(nonnull)))
    print('  moving tags   : %d (%s)' % (
        len(moving), ', '.join(sorted(moving)[:12]) + ('...' if len(moving) > 12 else '')))
    if 'snapshot' not in frames:
        print('WS FAIL: no snapshot frame received')
        return 5
    return 0


if __name__ == '__main__':
    sys.exit(main())
