# OCR Command Trigger 導入計畫

## 需求摘要

欣銓 OCR 現場若無法用 switch 觸發拍照，需要提供可選擇的拍照觸發方式：

- 預設維持 `Switch Trigger`，避免既有機台行為改變。
- 可在 OCR 設定頁切換為 `Command Trigger(SE8)`。
- Command 模式只改變拍照觸發來源，OCR 結果接收、重試、NoIC、Barcode parsing 流程不改。

## 現況架構

- OCR 結果由 `ClientSocket1` 接收，既有預設 port 為 `3000`。
- Cognex Native Mode 命令由 `ClientSocket2` 發送，port 由 `Gerneral.ini` 的 `[OCR SETTING] OCR Port` 決定，程式預設值為 `24`。
- 生產 OCR 主流程在 `DoOCRFlow1()`，目前有兩段拍照觸發：`case 31/32` 與 `case 420/430`。
- 既有 switch 觸發使用 `SW[SwOCRTigger].On()` / `Off()`。

## 導入設計

新增設定 `iOCRTriggerMode`：

| 值 | 模式 | 行為 |
| --- | --- | --- |
| 0 | Switch Trigger | 維持既有 `SwOCRTigger` On/Off |
| 1 | Command Trigger(SE8) | 只透過 `ClientSocket2` 發送 `SE8\r\n`，不操作 `SwOCRTigger` |

設定來源與預設：

- 儲存在目前 recipe 的 `AOI.Data`，section 為 `[OCR SETTING]`。
- 舊 recipe 沒有此 key 時，讀取預設 `0`，也就是 Switch Trigger。
- 若資料異常不是 `0/1`，程式回退成 `0`。

UI：

- 在 OCR Cognex Setting 頁新增 `OCR Trigger Mode` radio group。
- 預設選項為 `Switch Trigger`。
- 第二選項為 `Command Trigger(SE8)`。

程式切入點：

- `Config.h`：新增 `IniConfig.iOCRTriggerMode`。
- `OCR.dfm` / `OCR.h`：新增設定 UI 控制項。
- `OCR.cpp`：讀寫 `iOCRTriggerMode`，並提供 `StartOCRCaptureTrigger()` / `StopOCRCaptureTrigger()`。
- `OCRInsp.cpp`：兩段 OCR 觸發點改呼叫 helper，不直接操作 switch。

## 現場導入步驟

1. 更新程式後先維持 `Switch Trigger`。
2. 確認原本 OCR 讀碼流程可正常跑完，結果 socket `3000` 有回傳。
3. 確認 Native Mode command socket port：優先看 `Gerneral.ini` 的 `[OCR SETTING] OCR Port`，常見為 `24`，手冊範例為 `23`。
4. 確認 Cognex CCD 已 Online，且 job 的 capture event 支援 `SE8` 觸發。
5. 切換 OCR 設定頁的 `OCR Trigger Mode` 為 `Command Trigger(SE8)` 並儲存。
6. 單步或低速跑 OCR，觀察 `Memo1` 是否送出 `SE8`，並確認 `Memo2` / tray map 結果有正常更新。
7. 若 command 模式未觸發拍照，立即切回 `Switch Trigger`，舊流程可直接回復。

## 驗證矩陣

| 項目 | 預期結果 |
| --- | --- |
| 舊 recipe 未設定 `iOCRTriggerMode` | 自動使用 `Switch Trigger` |
| Switch 模式跑 OCR | `SwOCRTigger` 仍會 On/Off，結果流程不變 |
| Command 模式跑 OCR | 不做 switch On/Off，只透過 `ClientSocket2` 發送 `SE8` |
| Command socket 未連線 | 觸發失敗時重新連線，停在原狀態並走既有 WAR09113 retry 路徑 |
| Result socket 未連線 | 維持原本 `bOCRConnect` / WAR09113 行為 |
| OCR Simulation + Bypass AOI | 不要求實機 AOI 回傳，仍可跑 handler motion 驗證 |
| `iOCRTriggerMode` 異常值 | 自動回退 `Switch Trigger` |

## 回退策略

- 現場最快回退：OCR 設定頁切回 `Switch Trigger` 並儲存。
- Recipe 回退：刪除或改回 `AOI.Data` 的 `iOCRTriggerMode=0`。
- 程式回退：由於結果接收與解析未改，回退範圍只限 trigger mode helper 與 UI 設定。

## 風險與注意事項

- `SE8` 只負責觸發拍照，結果仍必須由 result socket 回傳。
- Command 模式依賴 Native Mode socket 可連線，現場需確認 port 與登入狀態。
- CCD job 必須支援 External / capture event 行為，否則 `SE8` 可能回 `1` 但不產生預期結果。
- 不建議直接修改全域預設為 command，避免影響既有 switch trigger 機台。