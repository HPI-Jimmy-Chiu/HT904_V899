# tools/webprobe — FW 戰役的 e2e 探針（gate 工具）

20260813 的驗證腳本活在 session scratchpad、隨 session 消失；本目錄照該日 DEVLOG
記載的功能重寫進樹（stdlib only，無第三方依賴）。

| 工具 | 驗什麼 | 綠燈條件 |
|---|---|---|
| `wire_probe.py` | publisher 原始 tag feed（tcp://127.0.0.1:8046，NDJSON） | hello(proto 1)＋snapshot 都收到（exit 0） |
| `ws_probe.py` | 瀏覽器實際走的鏈：HTTP 靜態（:8045，js 的 Content-Type 承重）＋ RFC 6455 握手（accept key 獨立重算）＋ snapshot/patch frames | 200＋101＋accept 相符＋snapshot（exit 0） |
| `system_guard.py` | 量產共用 `D:\HT9045\system` 有沒有被寫（MD5＋mtime，前後對比） | compare exit 0＝逐位元且 mtime 全等 |

## 標準 e2e 流程

```
python system_guard.py snapshot before.json
<啟動 wb_publish --pump 與 wb_gateway>
python wire_probe.py --seconds 8
python ws_probe.py --seconds 8
<停掉兩個程序>
python system_guard.py snapshot after.json
python system_guard.py compare before.json after.json
```

## 硬規則

- **絕不與 ctest 並行**：`WB_TcpLink` 綁同一族埠，兩邊都會假失敗。
- `wb_publish`／`wb_serve` 一律 `--dry`（`common.cpp:89` 的 `asGeneralPath`
  指向量產共用 `system\Gerneral.ini`，未拆家）；`--pump` 模式本就不載 config。
- `system_guard` 的 `REWRITTEN-SAME-BYTES`（內容相同但 mtime 變）**算 drift**——
  這正是 20260813 抓到 BinCount.txt 的症狀，不可視為通過。
