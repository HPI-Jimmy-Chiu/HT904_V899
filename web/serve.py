#!/usr/bin/env python3
"""Dev static server for the HT9045 web HMI.

ES modules will not load over file://, so the page needs a real origin.
This is stdlib-only on purpose -- no npm, no virtualenv, nothing to install.

    python serve.py            # http://127.0.0.1:8045/
    python serve.py 9000       # different port
    python serve.py --no-open  # don't launch a browser

Not a production server. When the handler serves this UI itself, it will do
so from its own embedded HTTP server; this script only exists for editing.
"""

import functools
import http.server
import os
import socketserver
import sys
import threading
import webbrowser

ROOT = os.path.dirname(os.path.abspath(__file__))
DEFAULT_PORT = 8045


class Handler(http.server.SimpleHTTPRequestHandler):
    """Static files, correct MIME for .mjs/.js, and no caching while editing."""

    extensions_map = {
        **http.server.SimpleHTTPRequestHandler.extensions_map,
        ".js": "text/javascript",
        ".mjs": "text/javascript",
        ".css": "text/css",
        ".json": "application/json",
        ".svg": "image/svg+xml",
    }

    def end_headers(self):
        self.send_header("Cache-Control", "no-store, must-revalidate")
        self.send_header("Pragma", "no-cache")
        super().end_headers()

    def log_message(self, fmt, *args):
        # One quiet line per request; the default logs to stderr very noisily.
        sys.stdout.write("  %s\n" % (fmt % args))


class Server(socketserver.ThreadingTCPServer):
    allow_reuse_address = True
    daemon_threads = True


def main(argv):
    port = DEFAULT_PORT
    auto_open = True
    for arg in argv[1:]:
        if arg in ("--no-open", "-n"):
            auto_open = False
        elif arg.isdigit():
            port = int(arg)
        else:
            print(__doc__)
            return 2

    handler = functools.partial(Handler, directory=ROOT)
    try:
        httpd = Server(("127.0.0.1", port), handler)
    except OSError as exc:
        print("cannot bind 127.0.0.1:%d -- %s" % (port, exc))
        return 1

    url = "http://127.0.0.1:%d/" % port
    print("HT9045 web HMI")
    print("  serving %s" % ROOT)
    print("  %s        (mock data)" % url)
    print("  %s?src=ws (live handler)" % url)
    print("  Ctrl-C to stop")

    if auto_open:
        threading.Timer(0.4, webbrowser.open, args=(url,)).start()

    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print("\nstopped")
    finally:
        httpd.server_close()
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
