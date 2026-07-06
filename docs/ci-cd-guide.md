# HT9045 CI/CD 導入指南

## 架構總覽

```
Phase 1: 自動編譯驗證    ← 已建立 workflow
Phase 2: 靜態分析        ← 已建立 Python 腳本
Phase 3: 邏輯單元測試    ← 未來擴充
Phase 4: 自動部署打包    ← 未來擴充
```

## 檔案清單

| 檔案 | 用途 |
|------|------|
| `.github/workflows/build-v899.yml` | GitHub Actions workflow（Phase 1 + 2） |
| `scripts/ci/check_code_quality.py` | Big5 編碼 / AI 註解 / 除法風險檢查 |
| `scripts/ci/check_readonly_files.py` | 禁止修改檔案守衛 |
| `scripts/build_bcb.bat`? | BCB6 命令列編譯腳本（已存在於 skills） |

? 位於 `.agents/skills/bcb_build/scripts/build_bcb.bat`，需複製到 `scripts/` 或 workflow 中直接引用。

## 前置需求

### Self-Hosted Runner 設定

GitHub Actions 需要一台 **Windows self-hosted runner**，條件：

1. **已安裝 BCB6**（預設路徑 `D:\ProgramFiles\Borland\CBuilder6`）
2. **Python 3.8+**（用於 Phase 2 靜態分析）
3. **安裝 GitHub Actions Runner**：
   ```powershell
   # 1. 到 GitHub repo → Settings → Actions → Runners → New self-hosted runner
   # 2. 依照指示下載並設定 runner
   # 3. 設定標籤：windows, bcb6
   ./config.cmd --url https://github.com/<org>/<repo> --token <TOKEN> --labels windows,bcb6
   ./run.cmd
   ```

4. **（選用）設定為 Windows 服務**：
   ```powershell
   ./svc.cmd install
   ./svc.cmd start
   ```

### 環境變數

| 變數 | 預設值 | 說明 |
|------|--------|------|
| `BCB_ROOT` | `D:\ProgramFiles\Borland\CBuilder6` | BCB6 安裝路徑 |
| `BCB6` | 同 `BCB_ROOT` | VS Code task 使用的變數名 |

可在 GitHub repo → Settings → Variables → Actions 中設定 `BCB_ROOT`。

## 本地測試

### 測試 Phase 1（編譯）
```powershell
# 直接使用現有 VS Code task
# 或手動：
scripts\build_bcb.bat "HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422" "HT9045.bpr"
```

### 測試 Phase 2（靜態分析）
```powershell
python scripts/ci/check_code_quality.py "HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422"
```

## 未來擴充

### Phase 3: 邏輯單元測試
- 抽離純計算邏輯（Contact Force、Bin 分類、SECS 訊息解析）
- 用 Python ctypes 呼叫編譯出的 DLL 做黑箱測試
- 或用 BCB6 原生 console app 做簡易測試

### Phase 4: 自動部署
- 成功編譯後自動打包 EXE + DLL + 設定檔
- 上傳至 GitHub Release 或內部檔案伺服器
- 自動更新 `Ver.txt` 版本號
