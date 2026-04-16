# Skill Registry — HT9045

## 使用說明

- 每新增一個 Skill，就新增一列
- 每次大改 description、references、觸發範圍或流程時，更新 Last Verified
- 如果 Skill 已過時，不立刻刪除，先標記狀態並記錄替代方案

## Skill 清單

| Skill | Domain | Lines | refs/ | Trigger Keywords | Owner | Last Updated | Last Verified | Status | Next Action |
|-------|--------|-------|-------|------------------|-------|--------------|---------------|--------|-------------|
| ht9045-inarm-flow | 機構流程 | ~250 | ✓ | InArm, DoInArm, Loader, HotPlate, Shuttle 放料, AutoClean, Precisor, Rotator | Jimmy | 2026-04-14 | 2026-04-14 | Active | §6-9 → refs/hotplate-shuttle.md |
| ht9045-inarm-suck-logic | 深度邏輯 | 290 | ✓ | InArm 吸取, Vacuum, HAS_NULL_IC, HotPlate pick/place, DataSwap, 16-site 2x4_16 | Jimmy | 2026-03-30 | 2026-04-04 | Active | — |
| ht9045-outarm-flow | 機構流程 | ~250 | ✓ | OutArm, DoOutArm, Shuttle 取料, Unloader, Auto Tray, Fix Tray, AOI, BinBox | Jimmy | 2026-04-14 | 2026-04-14 | Active | §4-10 → refs/state-machine.md |
| ht9045-index-flow | 機構流程 | ~350 | ✓ | Index, DoTestY, TestHead, Socket 下壓, FTestSuck, BTestSuck, MTestY, MTestZ, 32-site | Jimmy | 2026-04-14 | 2026-04-14 | Active | §4-5 → refs/child-state-machines.md |
| ht9045-shuttle-flow | 機構流程 | 410 | ✗ | Shuttle, Do_Auto_SHT1, Do_Auto_SHT2, Floating, Null IC, 2D Barcode | Jimmy | 2026-03-30 | 2026-04-04 | Active | — |
| ht9045-catchtray-flow | 機構流程 | 470 | ✗ | CatchTray, DoCatchTray, Loader/Empty/Color 補盤, Auto 放盤, TrayID, RFID | Jimmy | 2026-03-30 | 2026-04-04 | Active | — |
| ht9045-contact-force | 深度邏輯 | 420 | ✗ | Contact Force, SLK, Kit Diameter, Air Force, EP 壓力, Index Press, D25 | Jimmy | 2026-03-30 | 2026-04-04 | Active | — |
| ht9045-autostart-flow | 通訊 | ~180 | ✓ | AutoStart, GTK Loader, Handshake, HTSET 700/702/333, SOT, EOT | Jimmy | 2026-04-14 | 2026-04-14 | Active | 重複內容已移除，指向 refs/ |
| ht9045-lotinfo-flow | 通訊 | 455 | ✓ | LotInfo, Lot Start/End, Recipe 下載, PP_DL_REQUEST, FTP, Security_new.def | Jimmy | 2026-04-04 | 2026-04-04 | Active | frontmatter 已修復 |
| ht9045-secsgem | 通訊 | 355 | ✗ | SECS, GEM, SVID, ECID, CEID, S1F1, S2F15, AddSV, AddEC, uHGemHT9045 | Jimmy | 2026-03-30 | 2026-04-04 | Active | — |
| ht9045-v899 | 版本 | 345 | ✓ | V899, V3.33.899, HotPlate Data Swap, iArmTask, 16-site, 2x4_16, 2x8_32 | Jimmy | 2026-03-30 | 2026-04-04 | Active | — |
| bcb_build | 工具 | 141 | ✗ | BCB6, bpr2mak, make, Borland, 編譯, build, clean, rebuild | Jimmy | 2026-03-30 | 2026-04-04 | Active | — |
| cpp-pro | 工具 | 56 | ✗ | C++, RAII, smart pointer, STL, template, move semantics | Jimmy | 2026-03-30 | 2026-04-04 | Active | ⚠ 注意 BCB6 不支援 C++11 |
| data-analysis | 工具 | 175 | ✗ | State record, Task_ListWithTime.csv, DoStateRecord, cycle time, root cause | Jimmy | 2026-03-30 | 2026-04-04 | Active | — |
| search-division | 工具 | ~55 | ✓ | 除法, divide by zero, division safety, 除以零, ChangeToFloatNonPcnt | Jimmy | 2026-04-14 | 2026-04-14 | Active | 腳本全移至 refs/scripts.md |
| ht9045-autoclean-flow | 機構流程 | 320 | ✓ | AutoClean, DoAutoCleanKit, Clean Kit, Clean Pad, bRunAutoClean, IntervalContact, SmartAutoClean, DoIndexAutoClean, EnableAutoclean, CleanAir, eCKPos, WAR16102, JAM0110, JAM0312 | Jimmy | 2026-04-07 | 2026-04-07 | Active | — |
| ht9045-sorting-bintray | 機構流程 | 230 | ✗ | SortingBinTray, DoSortingBinTray, P27, bSortingBinTraywhenCleanOut, bSortingBinTrayWhenTrayFeed, DoFix3FullTray, Unloader 整盤, Clean Out 整盤, Tray Feed 整盤 | Jimmy | 2026-04-10 | 2026-04-10 | Active | — |
| skill-creator | 工具 | 395 | ✗ | Skill 建立, SKILL.md, skill factory, 技能工廠 | Jimmy | 2026-03-30 | 2026-04-04 | Active | — |
| ht9045-tray-group-mechanism | 機構原理 | 65 | ✓ | Tray Group, GoUp, GoDown, Loader, Color, Empty, C_Color_Up, C_Empty_Up, 料盤升降, CylinderUp, CylinderMiddle, asendic_Loader, asendic_Color, asendic_Empty | Jimmy | 2026-04-14 | 2026-04-14 | Active | — |

## 狀態定義

| 狀態 | 說明 |
|------|------|
| Active | 目前仍在使用，且已驗證 |
| Review | 內容可能過時，需要檢查 |
| Split | 粒度過大，準備拆分 |
| Merge | 與其他 Skill 重疊，準備合併 |
| Deprecated | 不再建議使用，但暫不刪除 |

## 每月最低維護要求

- [ ] 檢查最近是否有新模組或新流程未建立 Skill
- [ ] 檢查 description 是否仍含足夠觸發詞
- [ ] 檢查是否有 Skill 長期未驗證
- [ ] 檢查是否有 Skill 已經過胖需要拆 references/
