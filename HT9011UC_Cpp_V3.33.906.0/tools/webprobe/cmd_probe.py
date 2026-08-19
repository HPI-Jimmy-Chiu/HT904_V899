# =============================================================================
#  tools/webprobe/cmd_probe.py -- FW-W1 command-channel e2e probe.
#
#  AI(W906-FW-W1) 20260819: new file. Exercises the browser->ws->CommandQueue->
#  tick-drain->CompleteCommand->ack round trip that wb_serve --allow-cmd wires
#  (docs/WEBBRIDGE_WRITEPATH_DESIGN.md section 6, wave FW-W1).
#
#  Modes:
#    default            : against `wb_serve --allow-cmd` --
#                           sys.ping        -> ack ok:true
#                           no.such.cmd     -> ack ok:false ("unknown cmd...")
#    --expect-readonly  : against plain read-only wb_serve --
#                           sys.ping        -> ack ok:false ("read-only")
#
#  Exit 0 = every expectation met; nonzero = which stage failed (see prints).
#  Client frames are MASKED per RFC6455 (server drops unmasked client frames).
# =============================================================================
import argparse, base64, hashlib, json, os, socket, struct, time

WS_GUID = '258EAFA5-E914-47DA-95CA-C5AB0DC85B11'


def ws_handshake(host, port, path, deadline):
    key = base64.b64encode(os.urandom(16)).decode()
    expect = base64.b64encode(hashlib.sha1((key + WS_GUID).encode()).digest()).decode()
    sock = socket.create_connection((host, port), timeout=5.0)
    sock.sendall(('GET %s HTTP/1.1\r\nHost: %s:%d\r\nUpgrade: websocket\r\n'
                  'Connection: Upgrade\r\nSec-WebSocket-Key: %s\r\n'
                  'Sec-WebSocket-Version: 13\r\n\r\n'
                  % (path, host, port, key)).encode())
    sock.settimeout(0.5)
    buf = b''
    while b'\r\n\r\n' not in buf:
        if time.monotonic() > deadline:
            raise RuntimeError('handshake: no response before deadline')
        try:
            c = sock.recv(4096)
        except socket.timeout:
            continue
        if not c:
            raise RuntimeError('handshake: connection closed')
        buf += c
    head, rest = buf.split(b'\r\n\r\n', 1)
    if expect.encode() not in head:
        raise RuntimeError('handshake: Sec-WebSocket-Accept mismatch')
    return sock, rest


def send_text(sock, payload):
    data = payload.encode('utf-8')
    mask = os.urandom(4)
    header = bytearray([0x81])                      # FIN + text
    n = len(data)
    if n < 126:
        header.append(0x80 | n)                     # MASK bit + len
    elif n < 65536:
        header.append(0x80 | 126)
        header += struct.pack('>H', n)
    else:
        header.append(0x80 | 127)
        header += struct.pack('>Q', n)
    header += mask
    masked = bytes(b ^ mask[i % 4] for i, b in enumerate(data))
    sock.sendall(bytes(header) + masked)


def read_frames(sock, leftover, deadline):
    """Generator of (opcode, payload) server frames (server frames unmasked)."""
    buf = leftover
    while True:
        while True:
            if len(buf) >= 2:
                b0, b1 = buf[0], buf[1]
                ln = b1 & 0x7F
                off = 2
                if ln == 126:
                    if len(buf) < 4: pass
                    else:
                        ln = struct.unpack('>H', buf[2:4])[0]; off = 4
                elif ln == 127:
                    if len(buf) < 10: pass
                    else:
                        ln = struct.unpack('>Q', buf[2:10])[0]; off = 10
                if ln < 126 or off > 2:
                    if len(buf) >= off + ln:
                        payload = buf[off:off + ln]
                        buf = buf[off + ln:]
                        yield (b0 & 0x0F, payload)
                        continue
            if time.monotonic() > deadline:
                return
            try:
                c = sock.recv(65536)
            except socket.timeout:
                continue
            if not c:
                return
            buf += c


def wait_ack(sock, leftover, want_id, deadline):
    for op, payload in read_frames(sock, leftover, deadline):
        if op != 1:
            continue
        try:
            j = json.loads(payload.decode('utf-8', 'replace'))
        except ValueError:
            continue
        if j.get('type') == 'ack' and j.get('id') == want_id:
            return j
    return None


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--host', default='127.0.0.1')
    ap.add_argument('--port', type=int, default=8045)
    ap.add_argument('--path', default='/ht9045')
    ap.add_argument('--seconds', type=float, default=10.0)
    ap.add_argument('--expect-readonly', action='store_true')
    args = ap.parse_args()

    deadline = time.monotonic() + args.seconds
    try:
        sock, leftover = ws_handshake(args.host, args.port, args.path, deadline)
    except (OSError, RuntimeError) as e:
        print('HANDSHAKE FAIL: %s' % e)
        return 2
    print('ws handshake ok')

    # --- 1. sys.ping ---------------------------------------------------------
    send_text(sock, json.dumps({'type': 'cmd', 'id': 101, 'cmd': 'sys.ping'}))
    ack = wait_ack(sock, leftover, 101, deadline)
    leftover = b''
    if ack is None:
        print('CMD FAIL: no ack for sys.ping before deadline')
        return 3
    print('ack(sys.ping): %s' % json.dumps(ack))

    if args.expect_readonly:
        if ack.get('ok') is False and 'read-only' in str(ack.get('error', '')):
            print('READONLY PROBE PASS: cmd correctly refused')
            return 0
        print('READONLY PROBE FAIL: expected ok:false with read-only error')
        return 4

    if ack.get('ok') is not True:
        print('CMD FAIL: sys.ping expected ok:true')
        return 4

    # --- 2. unknown command must ack ok:false --------------------------------
    send_text(sock, json.dumps({'type': 'cmd', 'id': 102, 'cmd': 'no.such.cmd'}))
    ack2 = wait_ack(sock, leftover, 102, deadline)
    if ack2 is None:
        print('CMD FAIL: no ack for no.such.cmd before deadline')
        return 3
    print('ack(no.such.cmd): %s' % json.dumps(ack2))
    if ack2.get('ok') is not False:
        print('CMD FAIL: unknown cmd expected ok:false')
        return 4

    print('CMD PROBE PASS: round trip proven (ping acked true, unknown acked false)')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
