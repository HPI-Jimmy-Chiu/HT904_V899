# ADR-0003：HT9045 程式碼禁用 C++11 以上語法

- **狀態**：Accepted
- **日期**：2026-05-02（追溯歷史決策）
- **作者**：JimmyChiu
- **影響範圍**：所有 HT9045 / HT9046 C/C++ 原始檔
- **關聯**：
  - Instructions：`.github/instructions/ht9045-v899-cpp-guardrails.instructions.md`
  - AGENTS.md：「BCB 6.0 限制」段

## 1. 背景

HT9045 主機台軟體使用 Borland C++ Builder 6.0（2002 年發布）編譯，VCL 框架。BCB6 編譯器：
- 不支援 C++11 以上語法（無 `auto`、`nullptr`、range-based for、lambda、`<chrono>`、`<thread>`、`<unordered_map>` 等）
- 不支援部分 C++03 模板特性
- STL 為老舊版本

## 2. 考慮過的方案

### 方案 A：升級到 C++ Builder XE / 10.x
- 優點：取得現代 C++ 語法
- 缺點：
  - 大量第三方 lib（CSMC、DMC32、MN200、KeyProDLL）需重新驗證
  - VCL 表單檔 .dfm 格式遷移風險
  - 客戶機台已交付，更換編譯器 = 重新做整機驗證
- 成本：極高

### 方案 B：保留 BCB6，禁用 C++11+ 語法
- 優點：相容現有 build chain、無客戶端風險
- 缺點：開發者需自我克制，不能用現代慣例
- 成本：低

### 方案 C：混用（部分模組編 C++11）
- 缺點：BCB6 無法編譯，技術不可行

## 3. 決定

採用 **方案 B**：保留 BCB6 + 禁用 C++11+ 語法。

具體規則：
- 不用 `auto`、`nullptr`（用 `NULL`）、range-based for
- 不用 `<chrono>`、`<thread>`、`<atomic>`、`<filesystem>`、`<unordered_map>`
- 不用 lambda、move semantics、smart pointers (`unique_ptr`/`shared_ptr`)
- 字串用 VCL `AnsiString`，不用 `std::string`
- 多執行緒用 `HThreadCtrl/` 自訂模組或 VCL `TThread`

## 4. 理由

- 客戶風險為最高優先（已交付機台 + KeyPro 授權綁定編譯器）
- 升級編譯器的整機驗證成本遠超 C++11 語法的便利價值
- 對 AI agent 而言可寫成明確 instructions，自動防呆

## 5. 後果

### 正面
- 維持與既有 lib / DLL 的相容性
- 客戶端零變更
- 編譯產物大小、效能可預測

### 負面 / 技術債
- 無法享用 C++11+ 安全特性（智慧指標、移動語意）
- 新工程師學習曲線（從未用過 BCB6）
- 第三方現代 C++ lib 完全不可用

### 後續觀察點
- 若 BCB6 在 Windows 11/12 出現相容性問題 → 重新評估
- 若 KeyPro 授權方式變更 → 重新評估

## 6. 修訂紀錄

| 日期 | 變更 | 作者 |
|------|------|------|
| 2026-05-02 | 追溯記錄歷史決策 | JimmyChiu |
