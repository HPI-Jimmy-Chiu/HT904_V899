---
description: "Use when: 處理 webapi 與前端相關問題、回答 wave 提問、設計或修改 API/UI/前後端整合。每次提問都自動套用 3D check（三個檢查維度）。"
applyTo: "**"
---

# WebAPI / Frontend 3D Check 規則

> 來源：使用者偏好的「多角度思考規則」三維度標準。
> 套用時機：**每次詢問或修改 webapi / 前端相關內容時自動執行**，不需使用者每次提醒。

## 3D Check 三個檢查維度（強制）

回答或修改前，必須在心中（或回覆中）逐一確認以下三個維度，三者缺一不可，且必須是**真正不同維度**，不可同維度疊加三次。

| 維度 | 定義 | webapi / 前端範例 |
|------|------|-------------------|
| **條件錨點** | 判斷式、旗標、enum、狀態、權限、`==N` `!=N`、route guard、feature flag | `if (user.role === 'admin')`、`status === 200`、`if (!token)` |
| **資料錨點** | 實際資料形狀：request/response payload、欄位名稱、型別、陣列結構、storage key、URL query | `payload.items[].id`、`Content-Type`、`localStorage['user']` |
| **行為錨點** | 實際呼叫的 API、副作用、UI 渲染結果、產出的 HTTP 封包、事件觸發 | `fetch(url, {method:'POST'})`、`router.push()`、`element.dispatchEvent` |

## 執行步驟（每次自動套用）

1. **接到問題 → 先標記三維度**：在內部分析時，明確指出本題的「條件 / 資料 / 行為」分別是什麼。
2. **修改前 → 三維度搜尋**：grep / 讀檔時，三個方向各做一次（不要重複同方向三次）。
3. **回覆中 → 至少涵蓋三維度**：給答案、改 code、或設計 API 時，三維度都要提到或驗證過。
4. **若 1 個維度都還沒查清楚 → 不下結論**。

## 反例（禁止）

- ? 連續三次搜同一個變數名稱不同檔案 → 仍是「資料錨點」單維度。
- ? 只看 condition、不看實際 fetch 的 payload → 缺行為錨點。
- ? 只看 UI render、不看 API 回傳結構 → 缺資料錨點。

## 例外（可降為 1 維度）

僅以下小型操作可不必三維度：
- UI 文字 / 樣式微調
- 加 log、加註解、變數重命名
- 純文件修正

其他所有 webapi / 前端問題，**預設三維度起跳**。

## 與 WCAG 的關係

前端修改同時必須符合使用者既有規則：語義 HTML、aria 標籤、focus 可見、色彩對比 4.5:1+、鍵盤可操作。WCAG 屬於「行為錨點」中的 UX/可及性子項，不抵銷 3D check。
