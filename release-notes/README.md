# Release Note 管理系統

## 目錄結構

```
release-notes/
├── README.md                  ← 本說明文件
├── template/
│   └── base.doc               ← .doc 模板（由原始檔產生，不手動編輯）
├── changelogs/
│   ├── _example.yaml          ← YAML 格式範例
│   ├── greatek.yaml           ← 客戶: Greatek
│   ├── spil.yaml              ← 客戶: SPIL
│   └── ...                    ← 每個客戶一份
├── output/                    ← 產出目錄（自動產生，不需版控）
│   ├── Greatek_Software_Release_Note.doc
│   └── ...
└── scripts/
    ├── import_from_doc.py     ← 從既有 .doc 匯入 → YAML
    ├── generate_doc.py        ← 從 YAML → .doc（複製模板格式）
    └── extract_ai_comments.py ← 從原始碼 //AI 註解 → YAML 草稿
```

## 工作流程

### 日常維護（改完程式後）
1. 編輯對應客戶的 `changelogs/<customer>.yaml`，新增一筆紀錄
2. 執行 `python scripts/generate_doc.py changelogs/<customer>.yaml`
3. 產出的 .doc 檔在 `output/` 目錄

### 從 AI 註解產生草稿
```bash
python scripts/extract_ai_comments.py
```
會掃描 V899 原始碼中的 `//AI(...)` 註解，依日期分群輸出草稿。

### 匯入既有 .doc
```bash
python scripts/import_from_doc.py "path/to/existing.doc" changelogs/customer.yaml
```

## YAML 格式說明

每個 YAML 檔案結構：
```yaml
customer: "Greatek"
handler_type: "HT-9045/46 Series"
entries:
  - rev: "HT9045_3.33.899.6"
    date: "2026/04/15"
    engineer: "JimmyChiu"
    changes:
      - "加入透過Socket遠端控制(Auto Start, Clear Lot, Start Lot)"
```
