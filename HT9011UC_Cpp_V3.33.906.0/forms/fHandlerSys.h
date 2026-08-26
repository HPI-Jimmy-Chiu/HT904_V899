// =============================================================================
//  forms/fHandlerSys.h  --  non-VCL facade for golden's THandlerSystem
//  (HandlerSys.h/.cpp)
//
//  AI(W906-FW3-HandlerSys-WA) 20260819: new file, batch-5 dual-form
//  display-side wave (cStartCondition + HandlerSys). Golden ref:
//  HT9011UC_Code_V3.33.906.0_20260618/HandlerSys.h (515 lines) + .cpp
//  (1,376 lines, 44 methods; cp950 -- decoded with `python3 -c
//  "open(...,'rb').read().decode('cp950')"`, 0 U+FFFD, this wave) + .dfm.
//  No prior forms/fHandlerSys.h or same-named stand-in existed before this
//  wave (grepped 20260819: only tools/dfm2rc/* generator artifacts + a few
//  passing mentions in csystem.cpp comments, no C++ class/global).
//
//  ROLE
//  ----
//  THandlerSystem is golden's "Handler System Setup" dialog -- the machine-
//  IDENTITY / installed-hardware-options editor (which motion card, which IO
//  card, which optional modules -- OCR/2D-barcode/ATC/RFID/ESD-monitor/
//  vibration-motor/tri-temperature/... -- are physically installed on THIS
//  machine, plus the safe-door sensor enable set). Unlike cStartCondition
//  (mostly write-oriented), this form's dominant bucket IS display: its two
//  biggest methods (LoaderSystemSet, 378 lines; SaveSystemSet, 520 lines)
//  split CLEANLY into a pure-read display refresh and a pure-write persist --
//  no read/write-combo trap like cStartCondition's ReadWriteStartCondition.
//
//  METHOD CLASSIFICATION (all 44 golden methods; the mandate's 4-bucket taxonomy)
//  --------------------------------------------------------------------------
//  (a) DISPLAY/READ-ONLY -- TRANSLATED this wave (3 methods, bodies in
//      HandlerSys.cpp):
//        FormShow          golden :34-77   (44 lines -- calls LoaderSystemSet/
//                            LoaderSafeDoorSet, GATE (H1) on SortItemToMap(),
//                            reads a GPIB-bridge ini for cbHandlerModel/
//                            rgModel)
//        LoaderSystemSet   golden :79-457  (378 lines -- ~150
//                            CheckAndReadIniDataGeneral() calls filling
//                            RadioGroup->ItemIndex/ComboBox->Text/
//                            Edit->Text/CheckBox->Checked from the machine's
//                            installed-hardware config; see MAJOR RISK below)
//        LoaderSafeDoorSet golden :990-1004 (12 lines -- Sen[SnSafeDoorN].Enable
//                            -> SafeDoorN->Checked)
//
//  (b) WRITE PATH -- QUEUED, NOT TRANSLATED, NOT EVEN DECLARED this wave -- 34
//      methods: SaveSystemSet (:459-978, 520 lines, the LoaderSystemSet
//      mirror -- WriteIniDataGeneral/WriteIniData), SaveBtnClick/LoadBtnClick
//      (thin wrappers), SaveSafeDoorSet (:1006-1035, writes Sen[].Enable +
//      SAFE_DOOR_AMOUNT-conditioned overrides -- SAFETY-ADJACENT, see (c)),
//      ExitBtnClick (:1037-1043, calls HSys.ReadGeneralIni()+SaveSafeDoorSet),
//      BtnEnableAllClick (:1045-1052, zeroes all SafeDoorN checkboxes --
//      SAFETY-ADJACENT, see (c)), rgCustomerListClick (:1054-1060, stages
//      edtCustomerCode->Text from the selected list item), ExitBtnMouseDown
//      (:1062-1071, right-click reveals the hidden customer-code tab),
//      FormClose (:1073-1079, TabVisible reset + Do_Log -- pure chrome, no
//      persist of its own; bucketed here rather than as (a) since it has no
//      display VALUE to refresh, matching forms/fSpeed.h's sbtExitClick
//      "nothing to translate" precedent), btnSetATCComClick (:1081-1098,
//      hardcodes 14 COM-port text fields), GetCustomerName (:1100-1130 --
//      SEE DEFERRAL NOTE below, NOT bucketed (b), a data-fidelity deferral),
//      rgRotateKit_TypeClick (:1132-1136, a 2-line Enabled recompute
//      IDENTICAL to FormShow's own already-translated logic, but gated as an
//      edit-adjacent Click handler per convention), edtUserDefMaxContactHeightClick
//      + 20 more ed*Click/ed*MouseDown QwertyKey-opener handlers (same
//      "edit gesture" bucket as forms/fSpeed.h's ~24 MouseDown handlers):
//      edtSearchCodeChange, edtSearchFunctionChange (both GATE (H1)-
//      dependent, see (d)), rgTTLCardClick, edtUserDefineIndexZSafePosClick,
//      edtCustomerCodeClick, edMaxMpaFBClick, edMaxKpaClick, edMinMpaClick,
//      edATCSystemPortClick, edATCSystemUseHeatClick,
//      edHotGunFlow_Gun1_ChannelNoClick, edHotGunFlow_DevNoClick,
//      edtMinYPitchClick, edtMaxYPitchClick, edCognexSystemCCDClick,
//      ed24VMonitorPulseCountClick, edGroundMan_AlarmOhmClick,
//      edIONPulseCountClick, edtTriTempTotalChClick, edtOutShtMaxTempClick,
//      edtTriTemperature_MaxDegreeClick, edtTriTemperature_MinDegreeClick,
//      edtIndexMaxTempClick, edt_Total_CompressorChange, edtMinXPitchClick,
//      edtMaxXPitchClick.
//
//  (c) SAFETY-ADJACENT -- subset of (b), called out separately, NOT
//      translated this wave: SaveSafeDoorSet + BtnEnableAllClick both mutate
//      `Sen[SnSafeDoorN].Enable` -- whether a physical safe-door interlock
//      sensor is even CHECKED by the running handler. Disabling one here
//      (BtnEnableAllClick zeroes ALL of them in one click) does not itself
//      open a door, but it can silence the software-side detection that a
//      door is open. Flagged explicitly, not silently bucketed with the
//      ~150-line generic WriteIniDataGeneral cascade in SaveSystemSet.
//
//  (d) GOLDEN-DEAD or STRUCTURALLY GATED -- 4 methods, NOT declared this wave:
//        SortItemToMap (:1170-1199) / InitItemToMap (:1201-1218) -- GATE (H1)
//          below. edtSearchCodeChange (:1149-1168) / edtSearchFunctionChange
//          (:1220-1245) both transitively depend on TempComp (populated only
//          by SortItemToMap/InitItemToMap), so both are structurally gated
//          alongside them, same as forms/fSpeed.h's GATE (S4)
//          DoReplyDefaultToForm/SearchRecipeParameter both depending on the
//          gated fRPDefault.
//        pcSettingChange (:1144-1147, `edtSearchCode->Visible=(pcSetting->
//          ActivePage==tsCustomerCode);`) -- SAME missing-property class as
//          cStartCondition's GATE (SC6): vclcompat::TPageControl has no
//          pointer-valued ActivePage. Trivial 1-line body, not worth
//          declaring just to #if-0 it whole; omitted per forms/fSpeed.h's
//          "not even a stub declared" convention for structurally-blocked
//          methods.
//
//  GATE REGISTER
//  --------------------------------------------------------------------------
//  (H1) FormShow :36 `SortItemToMap();` -- SortItemToMap/InitItemToMap
//      (golden :1170-1218) take/recurse via `TWinControl *PCtrl` +
//      `PCtrl->ControlCount` + `PCtrl->Controls[iP]` -- the IDENTICAL
//      TWinControl-shaped component-tree-walk gap forms/fSpeed.h's GATE (S4)
//      already documents in exhaustive detail (vclcompat::TPanel/TGroupBox/
//      TRadioGroup all derive directly from TControl, none from any
//      TWinControl-shaped ancestor carrying ControlCount/Controls[]; a
//      DIFFERENT, unrelated TWinControl exists at handlerlog.h:104 for
//      TMyLog's own unrelated walk and is not convertible to accept a
//      TPanel*). Per "跨檔缺口 GATE 不自建 shim", the call is `#if 0`-gated
//      in FormShow and SortItemToMap/InitItemToMap themselves are NOT
//      declared here at all. DISCLOSED CONSEQUENCE: this facade's
//      `edtSearchCode`/`edtSearchFunction`/customer-code-search feature is
//      entirely inert offline (TempComp always empty) -- same posture as the
//      GetCustomerName DEFERRAL below, both centred on the same
//      rgCustomerList data this wave declined to hand-transcribe.
//
//  MAJOR RISK -- LoaderSystemSet reads the SHARED PRODUCTION Gerneral.ini
//  --------------------------------------------------------------------------
//  `CheckAndReadIniDataGeneral(Group,Name,Default)` (common.cpp:1444-1525)
//  operates directly on `INIFileGeneral` -- the SAME TIniFile the project's
//  own `common.cpp:89`/`asGeneralPath` gotcha is about
//  (`D:\HT9045\system\Gerneral.ini`, the shared production machine-identity
//  file that `LoadMachineConfig()` back-fills missing keys into). This is
//  NOT a new risk this wave introduces -- CheckAndReadIniDataGeneral is an
//  existing, already-merged, already-relied-on utility (forms/fSpeed.h's own
//  "DISCLOSED, NOT GATED" precedent covers its SISTER function
//  CheckAndReadIniData's identical lazy-seed behaviour) -- but THIS form is
//  categorically the MOST central place in the whole tree for it: golden's
//  own LoaderSystemSet calls CheckAndReadIniDataGeneral roughly 150 times in
//  one function, covering literally every installed-hardware-option flag the
//  machine has. If THandlerSystem is ever wired to a live INIFileGeneral (a
//  future integration wave's job, not this one's -- no `extern THandlerSystem
//  *HandlerSystem;` is declared here, see INTEGRATION-PENDING), calling
//  LoaderSystemSet for real means: (1) any key ABSENT from Gerneral.ini gets
//  SEEDED into it (a real write against the shared production file), and
//  (2) running this against a live production machine's Gerneral.ini without
//  `--dry` is squarely the scenario the project's standing `--dry` policy
//  for `wb_serve`/`wb_publish` exists to prevent. NOT gated here (matches
//  precedent for an already-relied-on utility), but flagged with maximum
//  visibility since this file is the single largest concentration of that
//  risk surface in the tree.
//
//  DEFERRAL -- GetCustomerName / rgCustomerList NOT translated (data-fidelity
//  risk, not a missing-dependency gate)
//  --------------------------------------------------------------------------
//  golden's ctor (:18-24) populates `slCustomerCode` by copying
//  `rgCustomerList->Items` -- a HARDCODED ~200-entry TRadioGroup Items list
//  (HandlerSys.dfm :Items.Strings, one line per customer code "NNN Name
//  Description", many with cp950-decimal-escaped (`#40251#21185` etc.)
//  Chinese/Japanese/Korean company names) that GetCustomerName's entire
//  SubString(1,3)/SubString(5,..) parsing depends on verbatim. Hand-
//  transcribing ~200 lines of decimal-escaped multi-byte text under this
//  wave's time budget is exactly the kind of high-risk, easy-to-silently-
//  corrupt transcription this tree's own "分析前先驗快照歸屬"/customer-
//  identity-mistake lessons warn against -- a single dropped or mis-decoded
//  entry would misreport a REAL customer's name back through GetCustomerName
//  to (eventually) SECS/GEM or on-screen operator-facing text. NEITHER
//  `rgCustomerList` NOR `GetCustomerName` NOR the ctor's `slCustomerCode`
//  wiring is declared/translated this wave; a future wave with a proper
//  automated (not hand-typed) dfm-string extraction pass should do this
//  list, matching the project's own "有維度的 widget 照 golden .dfm 維度
//  hydrate" discipline extended to "text CONTENT hydration", not just
//  numeric dimensions.
//
//
//  ADDENDUM -- RESOLVED (W906-FW3-HandlerSys-WB, 20260820)
//  --------------------------------------------------------------------------
//  The DEFERRAL above is RESOLVED. rgCustomerList/slCustomerCode/
//  GetCustomerName are now declared/translated (this header + HandlerSys.cpp).
//  The ~200-entry text WAS NOT hand-transcribed -- a one-time Python script
//  (session scratchpad, not checked into this tree) parsed HandlerSys.dfm's
//  `Items.Strings = (...)` block in BINARY mode, decoding quoted segments as
//  cp950 and `#NNNNN` escapes as Unicode codepoints (chr(NNNNN), verified
//  NOT cp950 byte pairs -- see the data-table banner in HandlerSys.cpp for
//  the full method). Result: 211 items, 0 tokenizer anomalies (every one of
//  211 physical lines -- including the terminator line, where the block's
//  closing ')' sits glued onto the last item's final token with no
//  separator -- fully round-tripped with zero leftover bytes), item count
//  cross-checked two independent ways (tokenizer count vs. independent
//  regex line count, both 211), and 5 entries spot-checked against both raw
//  dfm bytes and real-world semantic plausibility (own company name +
//  Thailand x2 + Germany + USA + Qualcomm's actual Chinese brand name). See
//  HandlerSys.cpp's data-table banner for the itemized spot-check list.
//
//  DEPENDENCY AUDIT (this wave, 20260819) -- confirmed present with matching
//  field/function names:
//    common.h/.cpp : CheckAndReadIniDataGeneral (4 overloads, common.h:229-232),
//                    CheckAndReadIniData, CheckRange<T>(), MyForceDirectories.
//    mysensor.h    : `extern class TMySensor Sen[MAX_SENSOR_ITEM];` with
//                    `bool Enable` member.
//    cmydef.h      : SnSafeDoor1..10/SnHeaterDoor/SnHeaterDoor2 (sensor-index
//                    enum), SAFE_DOOR_AMOUNT, and the ~150 golden hardware-
//                    config globals LoaderSystemSet reads by name (
//                    SUPPORT_2_EMPTY_EMPTY/LOAD_Z_USE_MOTOR[]/LOAD_Y_USE_MOTOR[]/
//                    LOADUNLOAD_USE_CASSETTE[]/InOutArmPickerUseMotor/
//                    NUMBER_PANEL_TYPE/ION_FAN_TYPE/SHUTTLE_SENSOR_TYPE/
//                    NUEC_TYPE/ENABLE_OUT_SHUTTLE_SENEOR/
//                    ENABLE_OUT_SHUTTLEY_LATCH/Use_AxisY_Sensor_2x3mode/
//                    Bias_Mode_Use_Y_Sensor/WEIGHT_CALIBRATION/bUseAuto2Empty/
//                    USE_AUTO_RETEST/UNLOADER_ART[]/ATC_SYSTEM/
//                    USE_IN_OUT_ARM_Y_PITCH/USE_OUT_ARM_Y_PITCH/iIndEPCnt --
//                    all confirmed present tree-wide via `grep -rl`, this
//                    wave; individual struct-field arity not re-verified one
//                    by one for every one of the ~150 -- see self-check
//                    result for the authoritative pass/fail).
//    MachineType.h : eAuto1..eAuto6, eNewATCSystem, iXPitch60/iXPitch40mm,
//                    tTemp175/155/150/130, e85KG, eht4Heater, KT4H,
//                    eocrUninstal, ebctUninstall, eATCUninstall, eATCNonMix,
//                    Fix3K_Uninstall, eCSMUN_Uninstall, eartUninstall,
//                    Type_None.
//    NOT found      : COMMSPEED_20M (golden Motor/mn200.h #define) -- NO
//                    ported header anywhere in this tree. SAME already-
//                    documented gate as database.cpp:1082's own
//                    `TODO(GA1-B6)` for this identifier (not a new finding
//                    this wave). `#if 0`-gated in LoaderSystemSet's
//                    rgMNetSpeed->ItemIndex assignment; left untouched
//                    rather than substituting a guessed literal.
//    handlerlog.h  : myLog (TMyLog), Do_Log() -- FormShow's own call
//                    GATE'd out (see below), not because it's missing but
//                    because it needs the `Sender` parameter this facade's
//                    FormShow (matching every other translated form in this
//                    tree) intentionally drops.
//  NOT found / gated: TWinControl-shaped base for TPanel/TGroupBox/
//  TRadioGroup (GATE H1), TPageControl::ActivePage (pcSettingChange, not
//  declared).
//
//  INTEGRATION-PENDING -- NO `extern THandlerSystem *HandlerSystem;` here
//  --------------------------------------------------------------------------
//  Same posture as forms/fSpeed.h/forms/fStartCondition.h: no consumer of a
//  `HandlerSystem` global exists anywhere else in this tree yet. Picking the
//  live singleton wiring -- and resolving the MAJOR RISK above by deciding
//  whether that wiring goes through a `--dry`-safe copy of INIFileGeneral --
//  is a later integration wave's call, not this one's.
// =============================================================================
// =============================================================================
//  ADDENDUM -- WAVE FW-HSYS-W22   AI(W906-FW-HSYS-W22) 20260826
//  （本段是「補充」，不是改寫。上面那份 WA/WB 的分類表有數處已經不成立，
//    下面逐條指名。上面的原文一律保留不動，這樣新舊裁決都查得到。）
// =============================================================================
//
//  DENOMINATOR（本波實測，指令可重跑）
//  --------------------------------------------------------------------------
//  golden `HT9011UC_Code_V3.33.906.0_20260618/HandlerSys.cpp`（1,376 行，
//  cp950，`open(p,'rb').read().decode('cp950')` → 0 個 U+FFFD）內
//  `THandlerSystem::` 的**定義**數，先剝掉 `/* */` 與 `//` 再用
//  `^\s*(?:(?:void|AnsiString|int|bool|double|float)\s+)?(?:__fastcall\s+)?
//   THandlerSystem::(\w+)\s*\(` 比對（20260826）：
//
//      **47 個定義 = 1 個 ctor + 46 個方法**
//
//  ⚠ 上面 WA 的檔頭反覆寫「all 44 golden methods」/「3 of 44」。**44 是低估**，
//  正確分母是 47（ctor 也在內）。WA 的 (a)3 + (b) 條列 + (c) + (d)4 三個桶
//  加起來也對不上它自己宣稱的 44——(b) 那段文字實際點名了 38 個名字卻寫
//  「34 methods」。本波不去改那些既有行（append-only），改在這裡給實測值。
//
//  分母現況（單位＝golden 方法定義，分母 47）：
//    本波之前已翻：5   ctor / FormShow / LoaderSystemSet / LoaderSafeDoorSet /
//                      GetCustomerName
//    本波新翻：   27   （明細見下）
//    本波之後累計：32 / 47
//    仍未翻：     15   （SaveSystemSet, SaveBtnClick, LoadBtnClick,
//                       SaveSafeDoorSet, ExitBtnClick, BtnEnableAllClick,
//                       rgCustomerListClick, btnSetATCComClick,
//                       rgRotateKit_TypeClick, pcSettingChange,
//                       SortItemToMap, InitItemToMap, edtSearchFunctionChange,
//                       rgTTLCardClick, FormDestroy）
//
//  本波選批依據
//  --------------------------------------------------------------------------
//  上一輪把整批 QwertyKey opener 判成「阻塞」，理由是「fQwertyKey 全樹無
//  port」。**那個理由已經過期**：`forms/fQwertyKey.h` 於 20260824 落地
//  （commit fc08e09 一族），20/20 方法 ACTIVE，`ShowQwertyKey` 是活的
//  （golden myQwertyKeyBoard.cpp:169-302，port body forms/fQwertyKey.cpp:166）。
//  同一份檔頭的 :105-122 BEHAVIOUR NOTE 已經把 offline 語意寫死了，本波直接
//  引用、不重新發明結論：
//    * `ShowModal()` / `Close()` 是**永久 no-op**（無視窗、無訊息迴圈）。
//    * 因此一次 ShowQwertyKey 呼叫的 offline 淨語意 ＝
//      「操作員打開鍵盤、原字不動立刻按 Summit」：modal 之後那段尾巴會**當場**
//      執行，帶 bCheckRange 時 `atof → CheckRange → AnsiString(double)`
//      這個來回**可能改寫目標控制項的文字**（例：`"abc"` + N_INTEGER → `"0"`）。
//      那是 golden 自己的 submit 路徑套在未編輯的值上，不是本 port 發明的。
//    * `spbChangeCase->Click()` 也是 no-op，所以 QWERTY 鍵面 caption 不會刷新。
//  另外 `ShowModal` 曾被宣稱「全樹無 port」——本波重跑 grep：
//  `forms/fPassword.h:337`、`forms/fQwertyKey.h:365`、`BarcodeReader.h:107`
//  三處都有（20260826）。該宣稱為偽。
//
//  本波交付（27 支，golden 行號逐支對過原文）
//  --------------------------------------------------------------------------
//  (1) edtSearchCodeChange           golden :1149-1168  ★ 推翻 GATE (H1) 的
//        連坐。上面 :86-91 寫它「transitively depends on TempComp」——
//        **實測為偽**：整段本體（:1151-1167）只碰 `edtSearchCode->Text`、
//        `rgCustomerList->Items`、`slCustomerCode`，**一個 TempComp 都沒有**。
//        真正碰 TempComp 的是 edtSearchFunctionChange（:1224/:1226/:1231/
//        :1233/:1239/:1240），那一支仍然 gated、本波不翻。
//  (2) ExitBtnMouseDown              golden :1062-1071  ★ 原阻塞理由三條裡兩條
//        是假的；剩下那條（TMouseButton/TShiftState 無 port）也已由
//        `vclcompat/ShiftState.h`（20260826）解除。簽章依該檔檔頭裁決
//        **保留 golden 完整原文**（新翻 handler 不再丟參數）。
//  (3) FormClose                     golden :1073-1079  ★ 原阻塞理由已被判偽。
//        本體確認為 `tsCustomerCode->TabVisible=false;` + `myLog.Do_Log(...)`。
//        **部分翻譯**：Do_Log 那一行 NOT translated，理由見下面 GATE (H22-1)。
//  (4)-(27) 24 支 QwertyKey opener（全部只有一行 `fQwertyKey->ShowQwertyKey(
//        (TEdit*)Sender, ...)`）：
//        edtUserDefMaxContactHeightClick    :1138-1142
//        edtUserDefineIndexZSafePosClick    :1257-1261
//        edtCustomerCodeClick               :1263-1266
//        edMaxMpaFBClick                    :1268-1271
//        edMaxKpaClick                      :1273-1276
//        edMinMpaClick                      :1278-1281
//        edATCSystemPortClick               :1283-1286
//        edATCSystemUseHeatClick            :1288-1291
//        edHotGunFlow_Gun1_ChannelNoClick   :1293-1297
//        edHotGunFlow_DevNoClick            :1299-1302
//        edtMinYPitchClick                  :1304-1307
//        edtMaxYPitchClick                  :1309-1312
//        edCognexSystemCCDClick             :1314-1317
//        ed24VMonitorPulseCountClick        :1319-1323
//        edGroundMan_AlarmOhmClick          :1325-1328
//        edIONPulseCountClick               :1330-1333
//        edtTriTempTotalChClick             :1335-1338
//        edtOutShtMaxTempClick              :1340-1343
//        edtTriTemperature_MaxDegreeClick   :1345-1349
//        edtTriTemperature_MinDegreeClick   :1351-1355
//        edtIndexMaxTempClick               :1357-1360
//        edt_Total_CompressorChange         :1362-1365
//        edtMinXPitchClick                  :1367-1370
//        edtMaxXPitchClick                  :1372-1375
//
//  DEVIATIONS（本波，均為 port 適配，無意圖行為變更）
//  --------------------------------------------------------------------------
//  (W22-D1) 24 支 opener 的 `(TEdit *)Sender` C-style cast **收進簽章**：
//      `void xxxClick(TEdit *Sender)`。依 forms/fQwertyKey.h (D-3) 的既有慣例
//      （「Sender IS read → cast target typed directly in the signature」，
//      forms/fSetup.cpp RadioButton1KeyDown precedent）。**24 支的目標 widget
//      在 golden HandlerSys.h 全部宣告為 `TEdit`**（逐支查過，見 golden .h
//      :155/:188/:189/:208/:217/:239/:250/:251/:257/:264/:265/:278/:279/:294/
//      :300/:317/:326/:327/:377/:378/:379/:384/:385/:387），所以這個型別是對的，
//      不是 TLabeledEdit 被 C-cast 硬扳的那一類。
//  (W22-D2) `edtSearchCodeChange` / `FormClose` 的 `TObject *Sender` 丟掉
//      （本體不讀；FormClose 只有被 gate 掉的 Do_Log 會用到它）；FormClose 的
//      `TCloseAction &Action` 也丟掉——`TCloseAction` 全樹無 port，且本體不讀，
//      與 cSecurity.cpp:510 / cCounterClear.cpp:517 / cObserver.cpp:4102 /
//      ATC/ATCInterface.cpp:434 / forms/fPassword.cpp:220 / cTemperFrom.cpp:1319
//      六支已翻 FormClose 的處置一致。
//  (W22-D3) `ExitBtnMouseDown` **不**丟參數（見上）。未讀的四個
//      （Sender/Shift/X/Y）用 `(void)x;` 標明，與 cObserver.cpp:6590 同形。
//  (W22-D4) `edtSearchCodeChange` 內 golden 寫
//      `slCustomerCode->Strings[i].UpperCase()`；vclcompat 的 `Strings[i]`
//      回 StringsProxy，只有 `operator AnsiString()`，沒有轉發 `.UpperCase()`，
//      所以先綁一個 local `AnsiString`。同一個 DEVIATION 這個檔的
//      GetCustomerName（HandlerSys.cpp:813-819）已經記過。值與順序不變。
//
//  GOLDEN NOTES（忠實照翻，記在帳上）
//  --------------------------------------------------------------------------
//  (W22-G1) 24 支 opener 的最後兩個引數，在 ported 簽章裡叫 `min, max`
//      （forms/fQwertyKey.h:370），但 golden 這個檔的呼叫點**多數寫成
//      (大, 小)**。實測（20260826）：23 支帶範圍的呼叫裡
//      **17 支是 (大, 小)**、**6 支是 (小, 大)**、1 支（edATCSystemPortClick）
//      只給 N_PORT 不帶範圍。
//      **這不是 bug，兩種寫法結果相同**，鏈路如下：
//        ShowQwertyKey 顯示端（forms/fQwertyKey.cpp:258-267）自己有
//        `if(max>min){edMax=max;edMin=min;} else {edMax=min;edMin=max;}`；
//        判定端（同檔 :287）`CheckRange(d, min, max)`，而 CheckRange 的宣告是
//        `CheckRange(Value, Maximum, Minimum)`（MachineType.h:1525），
//        且它自己有 `if(Maximum<Minimum)` 的對稱分支（:1527-1535）。
//      兩端都對調過，所以 (大,小) 與 (小,大) 夾出來的區間一樣。逐字照翻，
//      不「修正」任何一支。（forms/fQwertyKey.h (G-a) 記的是同一件事的另一半。）
//  (W22-G2) `edtUserDefineIndexZSafePosClick`（:1260）傳 `N_INTEGER` 卻給
//      `iDP=2`（小數位數）。整數模式下 iDP 只影響 ChangeDecimalPoint 的鍵面
//      啟用，不會產生小數。照翻。
//  (W22-G3) `edtOutShtMaxTempClick`/`edtTriTemperature_*`/`edtIndexMaxTempClick`
//      傳 `N_INTEGER` 卻給浮點字面值（120.0/175.0/35.0/-55.0/20.0）。
//      min/max 參數本來就是 `double`，沒有截斷。照翻。
//  (W22-G4) `edtCustomerCodeClick`（:1265）範圍是 (999, 0)，等效 0..999；
//      而 `edtCustomerCode` 的內容由 rgCustomerListClick（:1054-1060，本波未翻）
//      填三碼客戶代碼，兩者一致。照翻。
//
//  GATE REGISTER（本波新增；退出本波的部分逐條列出理由與 golden 行號）
//  --------------------------------------------------------------------------
//  (H22-1) FormClose golden **:1078** `myLog.Do_Log(Sender, asUser, asLogPath);`
//      —— NOT translated（FormClose 其餘部分已翻，所以這是**局部 gate**，
//      不是整支退出；本方法可觀察到的 offline 行為只有 :1076 那一行）。
//      三個獨立理由：
//        (a) 寫檔路徑。`TMyLog::Do_Log`（handlerlog.cpp:660-686）在
//            `loglist.size()>0` 時呼叫 `SaveEventLog()`，那是真的落檔
//            （handlerlog.h:13-16 檔頭自述「writing one line per changed
//            control to a dated ChangeLog ini file … also into the machine's
//            own EventLog」）。本波的硬規則是「有檔案寫入就不做」。
//        (b) 本樹**目前 0 個 live 呼叫點**。全樹 grep `Do_Log`（20260826）：
//            cConfiguration.cpp:6076 / cSpeed.cpp:423 / cSpeed.cpp:1326 三處
//            都是 `//` 註解掉的，cStartCondition.cpp:647 是 gate 說明，
//            〔AI(W906-FW-HSYS-W22) 20260826 主迴圈整併修正：兩個 cSpeed 行號
//              原寫 416/1319，是本波開工當下的值；兄弟波 FW-SPEED-W21 同時段
//              在 cSpeed.cpp:94-100 插入 7 行，兩處各位移 +7，已逐行對字面複驗。〕
//            本檔 HandlerSys.cpp:101-105 是 FormShow 自己那次同樣的 gate。
//            從一個連 TWinControl 都不是的 facade 開出第一條寫檔邊，
//            換不到任何行為（見 (c)），只換到風險。
//        (c) 就算接上也不會有行為。Do_Log 的第一道門是
//            `dynamic_cast<TWinControl*>(PCtrl)!=NULL`（handlerlog.cpp:675）；
//            `THandlerSystem` 這個 facade 不是 TWinControl、也不是 TObject
//            的後代，所以 golden 傳的 `Sender`（在 VCL 裡就是表單自己）
//            在本樹根本轉不出 TWinControl。
//      → 依 (W22-D2) 丟掉 `Sender` 之後，這一行**在型別上也拼不出來**，
//        與本檔 FormShow（golden :76，HandlerSys.cpp:101-105）完全同形。
//  (H22-2) GATE (H1) 的**範圍收窄**（上面 :102-117 的原文保留）。H1 本身
//      （SortItemToMap/InitItemToMap 需要 TWinControl 形狀的
//      ControlCount/Controls[] 走訪）**仍然成立、仍然 gated**，
//      `edtSearchFunctionChange`（:1220-1244，真的讀 TempComp）也仍然 gated。
//      但 H1 對 `edtSearchCodeChange` 的**連坐是錯的**，本波解除，理由見上 (1)。
//      連帶：上面 :113-117 那句「this facade's `edtSearchCode`/
//      `edtSearchFunction`/customer-code-search feature is entirely inert
//      offline (TempComp always empty)」——**對 edtSearchCode 那一半已不成立**：
//      客戶代碼搜尋走的是 slCustomerCode（ctor 已 hydrate 211 筆，
//      HandlerSys.cpp:803-808），與 TempComp 無關，現在是活的。
//      `edtSearchFunction` 那一半仍然成立。
//  (H22-3) NULL-GLOBAL 曝險（**不是 gate，是揭露**）。24 支 opener 逐字保留
//      `fQwertyKey->`，而 `fQwertyKey`/`fQwertyKey2` 兩個全域在本樹是
//      **NULL 直到某個接線波去 construct**（forms/fQwertyKey.h (G-d)、
//      forms/fQwertyKey.cpp:168-170 進門就 deref 這兩顆）。今天**沒有任何東西
//      呼叫這 24 支**（本波不接線，見下），所以到不了；golden 自己在
//      CreateForm 之前也是一樣的曝險。與 cObserver.cpp:6615/:6635、
//      cSetUp.cpp、cConfiguration.cpp 已合併的同型呼叫點同一姿態。
//  (H22-4) 本波**不接線**（"event handler 本體翻譯但不接線"）：沒有設定任何
//      OnClick/OnMouseDown 委派，vclcompat 的 stock widget 本來也沒有委派槽
//      （cSetUp.cpp GATE(G-Delegate) 的既有慣例）。未來 web write path 由
//      前端直接呼叫這些方法。
//
//  NOT DONE / 仍未翻（15 支，理由）
//  --------------------------------------------------------------------------
//    寫檔／持久化（(b)/(c) 桶，沿用 WA 的裁決，本波不動）：SaveSystemSet
//      (:459-978)、SaveBtnClick (:979-984)、LoadBtnClick (:985-989)、
//      SaveSafeDoorSet (:1006-1035)、ExitBtnClick (:1037-1043，呼叫
//      HSys.ReadGeneralIni()+SaveSafeDoorSet)、BtnEnableAllClick
//      (:1045-1052，一鍵清掉所有安全門 checkbox —— SAFETY-ADJACENT)。
//    純 UI-state、但本波刻意不擴大範圍（保持本波「複驗過的乾淨批」性質）：
//      rgCustomerListClick (:1054-1060)、btnSetATCComClick (:1081-1098)、
//      rgRotateKit_TypeClick (:1132-1136)。
//    結構性 gated（理由未變）：pcSettingChange (:1144-1147，
//      `TPageControl::ActivePage` 是指標型 property，vclcompat 只有
//      ActivePageIndex)、SortItemToMap (:1170-1199)、InitItemToMap
//      (:1201-1218)、edtSearchFunctionChange (:1220-1244) —— 全部 GATE (H1)。
//    其他：rgTTLCardClick (:1246-1255，純 UI-state，同上不擴大範圍)、
//      FormDestroy (:26-32，`delete slCustomerCode` + `TempComp.clear()` +
//      `LogSoftwareOffTime(...)`；本 facade 的 slCustomerCode 是 NSDMI
//      `new`、生命週期與 golden 的 TForm 解構不同，且 TempComp 未宣告
//      —— 屬於 lifetime 決策，不是本波的翻譯工作)。
// =============================================================================
#ifndef FORMS_FHANDLERSYS_H
#define FORMS_FHANDLERSYS_H

// AI(W906-FW-HSYS-W22) 20260826: AnsiString / TObject / TStringList at global
// scope.  This header was NOT self-contained before this wave -- compiling
// `#include "forms/fHandlerSys.h"` alone at HEAD failed with
// "'TStringList' does not name a type" (:558) and "'AnsiString' does not name
// a type" (:560), both dating from the WB wave; it only ever built because its
// single consumer, HandlerSys.cpp, includes MachineDefine.h first.  This wave
// would have added a third such name (TObject, in ExitBtnMouseDown's golden
// signature), so the include goes in instead -- same one-liner
// forms/fQwertyKey.h:224 already carries for the same reason.  Verified after
// the change: the header now compiles standalone.
#include "vclcompat/vcl_compat.h" // AnsiString/TObject/TStringList (see above)
#include "vclcompat/Controls.h"   // TEdit/TCheckBox/TRadioGroup/TComboBox/
                                  //   TGroupBox/TPageControl/TTabSheet
                                  //   (brought to global scope by that
                                  //   header's own footer)
#include "vclcompat/ShiftState.h" // AI(W906-FW-HSYS-W22) 20260826:
                                  //   TMouseButton/mbRight/TShiftState --
                                  //   ExitBtnMouseDown keeps golden's full
                                  //   signature, see that header's ruling
                                  //   and this file's W22 ADDENDUM (W22-D3)

// ===========================================================================
//  THandlerSystem -- non-VCL facade (golden HandlerSys.h,
//  THandlerSystem : public TForm)
// ===========================================================================
class THandlerSystem
{
public:
    THandlerSystem();

    // -----------------------------------------------------------------------
    //  Widget members touched by this wave's 3 translated (a) methods only
    //  (FormShow/LoaderSystemSet/LoaderSafeDoorSet) -- golden __published
    //  order preserved. `rgCustomerList`/`slCustomerCode`/`TempComp` NOT
    //  declared -- see DEFERRAL and GATE (H1) notes above. TPopupMenu/
    //  TMenuItem/TScrollBox-typed widgets don't exist in this form anyway
    //  (unlike cStartCondition); the omission here is purely "not touched by
    //  the 3 in-scope methods", same DEVIATION rationale as
    //  forms/fStartCondition.h's leaner-subset note.
    //
    //  UPDATE (W906-FW3-HandlerSys-WB, 20260820): the "`rgCustomerList`/
    //  `slCustomerCode` NOT declared" sentence above is now STALE -- both
    //  ARE declared below (see the DEFERRAL ADDENDUM further up this file).
    //  `TempComp` remains NOT declared (still GATE (H1), unaffected by this
    //  wave's data-fidelity fix).
    // -----------------------------------------------------------------------
    TGroupBox    *GroupBox1  = new TGroupBox();
    TGroupBox    *GroupBox2  = new TGroupBox();
    TPageControl *pcSetting  = new TPageControl();
    TTabSheet    *tsCustomerCode = new TTabSheet();

    TComboBox   *cbComIndex = new TComboBox();
    TComboBox   *cbComTemp = new TComboBox();
    TComboBox   *cbComTempOmron = new TComboBox();
    TComboBox   *cbComTester = new TComboBox();
    TComboBox   *cbComBinDisp = new TComboBox();
    TComboBox   *cbComRTC = new TComboBox();
    TComboBox   *cbComDyTemp = new TComboBox();
    TComboBox   *cbbATC2 = new TComboBox();
    TComboBox   *cbbATC3 = new TComboBox();
    TComboBox   *cbbATC4 = new TComboBox();
    TComboBox   *cbbATC1 = new TComboBox();
    TComboBox   *cbOCR = new TComboBox();
    TComboBox   *cbOCRwithTester = new TComboBox();
    TComboBox   *cbAirCon = new TComboBox();
    TComboBox   *cbEMAwarePort1 = new TComboBox();
    TComboBox   *cbEMAwarePort2 = new TComboBox();
    TComboBox   *cbEMAwarePort3 = new TComboBox();
    TComboBox   *cbEMAwarePort4 = new TComboBox();
    TCheckBox   *cbESDUse4COM = new TCheckBox();
    TComboBox   *cbNovx3360 = new TComboBox();
    TComboBox   *cbComLaser1 = new TComboBox();
    TComboBox   *cbComLaser2 = new TComboBox();
    TComboBox   *cbComLaserInArm = new TComboBox();
    TRadioGroup *rgColorSensor = new TRadioGroup();
    TComboBox   *cb2DReader1 = new TComboBox();
    TComboBox   *cb2DReader2 = new TComboBox();
    TComboBox   *cb2DReader3 = new TComboBox();
    TComboBox   *cb2DReader4 = new TComboBox();
    TRadioGroup *rgRotateKit = new TRadioGroup();
    TRadioGroup *rgRotateKitIn = new TRadioGroup();
    TRadioGroup *rgRotateKitOut = new TRadioGroup();
    TRadioGroup *rgRotateKit_Type = new TRadioGroup();
    TComboBox   *cbbGroundMan = new TComboBox();
    TComboBox   *cbKASUGA_Fan = new TComboBox();
    TComboBox   *cbSocketSenAmpCnt = new TComboBox();
    TComboBox   *cbRotateSenAmpCnt = new TComboBox();
    TComboBox   *cbColorSenAmpCnt = new TComboBox();
    TRadioGroup *rgSocketSen = new TRadioGroup();
    TComboBox   *cbbTrayStepMotor = new TComboBox();
    TCheckBox   *chkUseHPComCard = new TCheckBox();
    TRadioGroup *rgCanBusMethod = new TRadioGroup();
    TRadioGroup *rgTrayVibration = new TRadioGroup();
    TRadioGroup *rgNumberPanelType = new TRadioGroup();
    TRadioGroup *rgInstallFix3 = new TRadioGroup();
    TRadioGroup *rgAutoAlignment = new TRadioGroup();
    TRadioGroup *Auto2SelectCy = new TRadioGroup();
    TRadioGroup *rgLdUldUseStepMotor = new TRadioGroup();
    TRadioGroup *rgCKDFCM = new TRadioGroup();
    TRadioGroup *rgCatchTrayModel = new TRadioGroup();
    TRadioGroup *rgTrayMappingGrabImage = new TRadioGroup();
    TRadioGroup *rgLoaderKeyence = new TRadioGroup();
    TRadioGroup *rgMultileEmptyTrayIDKeyence = new TRadioGroup();
    TRadioGroup *rgEmptyKeyence = new TRadioGroup();
    TRadioGroup *rgFix3FullPlace = new TRadioGroup();
    TRadioGroup *rgRFIDSystem = new TRadioGroup();
    TRadioGroup *rgCatchTrayBlock = new TRadioGroup();
    TRadioGroup *rgTrayRobot = new TRadioGroup();
    TRadioGroup *rgMultileEmpty = new TRadioGroup();
    TEdit       *edtCustomerCode = new TEdit();
    TEdit       *edtSeriaNo = new TEdit();
    TComboBox   *cbHandlerModel = new TComboBox();
    TRadioGroup *rgModel = new TRadioGroup();
    TRadioGroup *rgMotionCard = new TRadioGroup();
    TRadioGroup *rgIOCard = new TRadioGroup();
    TRadioGroup *rgTTLCard = new TRadioGroup();
    TRadioGroup *rgUseSucker_9046_DB = new TRadioGroup();
    TRadioGroup *rgUseSensor_9046_DB = new TRadioGroup();
    TRadioGroup *rgSafeDoorLock = new TRadioGroup();
    TRadioGroup *rgMNetSpeed = new TRadioGroup();
    TRadioGroup *rgIndexMotionCard = new TRadioGroup();
    TCheckBox   *SafeDoor1 = new TCheckBox();
    TCheckBox   *SafeDoor2 = new TCheckBox();
    TCheckBox   *SafeDoor3 = new TCheckBox();
    TCheckBox   *SafeDoor4 = new TCheckBox();
    TCheckBox   *SafeDoor5 = new TCheckBox();
    TCheckBox   *SafeDoor6 = new TCheckBox();
    TCheckBox   *SafeDoor7 = new TCheckBox();
    TCheckBox   *SafeDoor8 = new TCheckBox();
    TCheckBox   *SafeDoor9 = new TCheckBox();
    TCheckBox   *SafeDoor10 = new TCheckBox();
    TCheckBox   *HeaterDoor1 = new TCheckBox();
    TCheckBox   *HeaterDoor2 = new TCheckBox();
    TRadioGroup *rgInOutArmYPitch = new TRadioGroup();
    TEdit       *edtMinYPitch = new TEdit();
    TEdit       *edtMaxYPitch = new TEdit();
    TRadioGroup *rgLaserDistance = new TRadioGroup();
    TRadioGroup *rgOCR = new TRadioGroup();
    TRadioGroup *rgDeviceFlipper = new TRadioGroup();
    TRadioGroup *rgHotPlatePos = new TRadioGroup();
    TRadioGroup *rgHotplateType = new TRadioGroup();
    TRadioGroup *rgHotPlateLimit = new TRadioGroup();
    TRadioGroup *rgMagneticScale = new TRadioGroup();
    TRadioGroup *ArmZAtoZH = new TRadioGroup();
    TRadioGroup *rgPickerCount = new TRadioGroup();
    TRadioGroup *rgPreciser = new TRadioGroup();
    TRadioGroup *rgPreciserPos = new TRadioGroup();
    TRadioGroup *rgDieClean = new TRadioGroup();
    TRadioGroup *rgSafeDoor = new TRadioGroup();
    TRadioGroup *rgAOI = new TRadioGroup();
    TEdit       *ed24VMonitorPulseCount = new TEdit();
    TRadioGroup *rg2DBarcode = new TRadioGroup();
    TRadioGroup *rgShuttleFloating = new TRadioGroup();
    TRadioGroup *rgBottom2DID = new TRadioGroup();
    TEdit       *edCognexSystemCCD = new TEdit();
    TRadioGroup *rgShuttleSensor = new TRadioGroup();
    TRadioGroup *rgShuttleZType = new TRadioGroup();
    TRadioGroup *rgShuttleCrossSensor = new TRadioGroup();
    TRadioGroup *rgAutoShuttleSensor = new TRadioGroup();
    TRadioGroup *rgInShtLastSensor = new TRadioGroup();
    TRadioGroup *rgShuttleVibration = new TRadioGroup();
    TRadioGroup *rgUseOutSht = new TRadioGroup();
    TRadioGroup *rgUse4DUT = new TRadioGroup();
    TRadioGroup *rgOTDInstall = new TRadioGroup();
    TRadioGroup *rgIOChangeToque = new TRadioGroup();
    TRadioGroup *rgIndexMotorType = new TRadioGroup();
    TRadioGroup *rgSocketClamp = new TRadioGroup();
    TRadioGroup *rgRealTimeCCD = new TRadioGroup();
    TRadioGroup *rgRealTimeCCDTempNum = new TRadioGroup();
    TRadioGroup *rgIndexPressType = new TRadioGroup();
    TEdit       *edMaxMpaFB = new TEdit();
    TEdit       *edtMinMpaFB = new TEdit();
    TRadioGroup *rgDoubleEPControl = new TRadioGroup();
    TRadioGroup *rgATC = new TRadioGroup();
    TRadioGroup *rgHeaterType = new TRadioGroup();
    TEdit       *edATCSystemIP = new TEdit();
    TEdit       *edATCSystemPort = new TEdit();
    TEdit       *edATCSystemUseHeat = new TEdit();
    TRadioGroup *rgHighTempLimit = new TRadioGroup();
    TCheckBox   *chkUserDefMaxContactHeight = new TCheckBox();
    TEdit       *edtUserDefMaxContactHeight = new TEdit();
    TEdit       *edHotGunFlow_LineNo = new TEdit();
    TEdit       *edHotGunFlow_DevNo = new TEdit();
    TEdit       *edHotGunFlow_Gun1_ChannelNo = new TEdit();
    TEdit       *edHotGunFlow_Gun2_ChannelNo = new TEdit();
    TRadioGroup *rgHotGunFlow = new TRadioGroup();
    TRadioGroup *rgHeatGun = new TRadioGroup();
    TRadioGroup *rgHeater = new TRadioGroup();
    TRadioGroup *rgESDTemp = new TRadioGroup();
    TRadioGroup *rgAirConditioner = new TRadioGroup();
    TRadioGroup *ElectronPressure = new TRadioGroup();
    TRadioGroup *rgWeightCali = new TRadioGroup();
    TRadioGroup *rgIndexSuckerType = new TRadioGroup();
    TEdit       *edMaxKpa = new TEdit();
    TEdit       *edMinMpa = new TEdit();
    TRadioGroup *rgLBTemp = new TRadioGroup();
    TRadioGroup *rgCleanAir = new TRadioGroup();
    TRadioGroup *rgFinePitch = new TRadioGroup();
    TComboBox   *cbIndEPCnt = new TComboBox();
    TRadioGroup *rgDewpointHW = new TRadioGroup();
    TRadioGroup *rgCCDTemp = new TRadioGroup();
    TRadioGroup *rgIonFanType = new TRadioGroup();
    TRadioGroup *rgUsePulseType = new TRadioGroup();
    TEdit       *edIONPulseCount = new TEdit();
    TRadioGroup *rgGroundMan = new TRadioGroup();
    TRadioGroup *rgGroundMan_ScanPoint = new TRadioGroup();
    TEdit       *edGroundMan_AlarmOhm = new TEdit();
    TRadioGroup *rg_ControlPanelMode = new TRadioGroup();
    TComboBox   *cbComTTLRS232 = new TComboBox();
    TComboBox   *cbComTTLRS232_2 = new TComboBox();
    TRadioGroup *rgTTLUseAddress = new TRadioGroup();
    TRadioGroup *rgFixAICCD = new TRadioGroup();
    TRadioGroup *rgATCHeatGun = new TRadioGroup();
    TComboBox   *cbbRFIDReader = new TComboBox();
    TRadioGroup *rgCCDAutoAlignmentMode = new TRadioGroup();
    TRadioGroup *rgIndexMotorAxis = new TRadioGroup();
    TCheckBox   *chkUser_Define_IndexZ_SafePos = new TCheckBox();
    TEdit       *edtUserDefineIndexZSafePos = new TEdit();
    TRadioGroup *rgBottom2DID_CCD = new TRadioGroup();
    TRadioGroup *rgTrayArmMode = new TRadioGroup();
    TRadioGroup *rgBaseHeaterCount = new TRadioGroup();
    TRadioGroup *rgShuttleFloodgate = new TRadioGroup();
    TRadioGroup *rgNUECType = new TRadioGroup();
    TRadioGroup *rgCOLORSENSOR_MUN = new TRadioGroup();
    TRadioGroup *rgInOutArmXPitch = new TRadioGroup();
    TEdit       *edtMinXPitch = new TEdit();
    TEdit       *edtMaxXPitch = new TEdit();
    TEdit       *edtHPLimit = new TEdit();
    TRadioGroup *rgVibrationCommuncation = new TRadioGroup();
    TComboBox   *cbVibrationCardQty = new TComboBox();
    TRadioGroup *MachineTrack = new TRadioGroup();
    TRadioGroup *rgLoaderHinge = new TRadioGroup();
    TCheckBox   *cbLoaderCassette = new TCheckBox();
    TCheckBox   *cbEmptyCassette = new TCheckBox();
    TCheckBox   *cbColorCassette = new TCheckBox();
    TCheckBox   *cbAuto1Cassette = new TCheckBox();
    TCheckBox   *cbAuto2Cassette = new TCheckBox();
    TCheckBox   *cbAuto3Cassette = new TCheckBox();
    TCheckBox   *cbAuto4Cassette = new TCheckBox();
    TCheckBox   *cbAuto5Cassette = new TCheckBox();
    TCheckBox   *cbAuto6Cassette = new TCheckBox();
    TCheckBox   *chkLoader = new TCheckBox();
    TCheckBox   *chkEmpty = new TCheckBox();
    TCheckBox   *chkColor = new TCheckBox();
    TCheckBox   *chkAuto1 = new TCheckBox();
    TCheckBox   *chkAuto2 = new TCheckBox();
    TCheckBox   *chkAuto3 = new TCheckBox();
    TCheckBox   *chkAuto6 = new TCheckBox();
    TCheckBox   *chkAuto5 = new TCheckBox();
    TCheckBox   *chkAuto4 = new TCheckBox();
    TRadioGroup *rgInstallAutoRestest = new TRadioGroup();
    TRadioGroup *rgAuto1ART = new TRadioGroup();
    TRadioGroup *rgAuto2ART = new TRadioGroup();
    TRadioGroup *rgAuto3ART = new TRadioGroup();
    TRadioGroup *rgAuto6ART = new TRadioGroup();
    TRadioGroup *rgAuto5ART = new TRadioGroup();
    TRadioGroup *rgAuto4ART = new TRadioGroup();
    TComboBox   *cbbComBinDisp2 = new TComboBox();
    TRadioGroup *rgMagBinDispType = new TRadioGroup();
    TRadioGroup *rgTriTempMachine = new TRadioGroup();
    TRadioGroup *rgAirStreamSelect = new TRadioGroup();
    TEdit       *edtTriTemperature_MaxDegree = new TEdit();
    TEdit       *edtTriTemperature_MinDegree = new TEdit();
    TEdit       *edtTriTempTotalCh = new TEdit();
    TEdit       *edtOutShtMaxTemp = new TEdit();
    TEdit       *edtIndexMaxTemp = new TEdit();
    TEdit       *edtBaseMaxTemp = new TEdit();
    TEdit       *edt_Total_Compressor = new TEdit();
    TRadioGroup *rgDoubleBeltMode = new TRadioGroup();
    TRadioGroup *rg_IndexDoorHeater = new TRadioGroup();
    TEdit       *edtOutSortXPitchMin = new TEdit();
    TEdit       *edtOutSortXPitchMax = new TEdit();
    TRadioGroup *rgOutSortArm = new TRadioGroup();
    TRadioGroup *rgVacuUnitType = new TRadioGroup();
    TCheckBox   *cbEnableOutShtSensor = new TCheckBox();
    TCheckBox   *chkOutShtYSensorByLatch = new TCheckBox();
    TCheckBox   *chk2x3modeUseAxisYSensor = new TCheckBox();
    TCheckBox   *chkBiasModeUseYSensor = new TCheckBox();
    TRadioGroup *rgScanner_AOI = new TRadioGroup();
    TRadioGroup *rgTopScanner_AOI = new TRadioGroup();
    TRadioGroup *rgSafePlcIO = new TRadioGroup();
    TComboBox   *coNudn1Macid11 = new TComboBox();
    TComboBox   *coNudn1Macid12 = new TComboBox();
    TComboBox   *coNudn1Macid13 = new TComboBox();
    TComboBox   *coCanBusNudn1 = new TComboBox();
    TRadioGroup *rgLBTemp2 = new TRadioGroup();
    TRadioGroup *rgCustomerList = new TRadioGroup();  // golden :422 -- Items hydrated in ctor
                                                     //   (HandlerSys.cpp), NOT via NSDMI; see
                                                     //   ctor DEVIATION banner there. DEFERRAL
                                                     //   RESOLVED, see ADDENDUM below.
    TComboBox   *coNudn1Macid14 = new TComboBox();
    TComboBox   *cbSocketSenAmpCnt2nd = new TComboBox();
    TRadioGroup *rgInShtAutoLatch = new TRadioGroup();
    TRadioGroup *rgLDCarRotAtm = new TRadioGroup();
    TRadioGroup *rgE84Sensor = new TRadioGroup();
    TRadioGroup *rgLdUldCassetteMode = new TRadioGroup();
    TRadioGroup *rgOutArmYPitch = new TRadioGroup();
    TRadioGroup *rg3M_EM_AWARE_Monitor = new TRadioGroup();
    TRadioGroup *rgNovx3360 = new TRadioGroup();
    TRadioGroup *rgKasuga = new TRadioGroup();
    TRadioGroup *rgKasuga_Fan = new TRadioGroup();
    TRadioGroup *rgHTIonBar = new TRadioGroup();
    TRadioGroup *rgChamberUsePulseType = new TRadioGroup();
    TRadioGroup *rgAutoCleanIonFan = new TRadioGroup();
    TRadioGroup *rgOCRYStepMot = new TRadioGroup();
    TRadioGroup *rgCoverTrayID = new TRadioGroup();
    TRadioGroup *rgRFIDReader = new TRadioGroup();
    TRadioGroup *rg2ndLoader = new TRadioGroup();
    TRadioGroup *rgAuto3Magazine = new TRadioGroup();
    TRadioGroup *rgOHTSystem = new TRadioGroup();
    TRadioGroup *rgMRSystem = new TRadioGroup();
    TRadioGroup *rgTrayMapping = new TRadioGroup();
    TCheckBox   *chkLoaderY = new TCheckBox();
    TCheckBox   *chkEmptyY = new TCheckBox();
    TCheckBox   *chkColorY = new TCheckBox();
    TCheckBox   *chkAuto1Y = new TCheckBox();
    TCheckBox   *chkAuto2Y = new TCheckBox();
    TCheckBox   *chkAuto3Y = new TCheckBox();
    TRadioGroup *rgATCMixMode = new TRadioGroup();
    TComboBox   *cbSocketSenAmpCnt3rd = new TComboBox();

    // -----------------------------------------------------------------------
    //  Wave A translated methods (bodies: HandlerSys.cpp, this wave)
    // -----------------------------------------------------------------------
    void FormShow();          // golden :34-77 (Sender dropped, see below)
    void LoaderSystemSet();   // golden :79-457
    void LoaderSafeDoorSet(); // golden :990-1004

    // -----------------------------------------------------------------------
    //  Wave B translated member/method (body: HandlerSys.cpp, this wave) --
    //  resolves the DEFERRAL note above. See ADDENDUM below and the ctor/
    //  GetCustomerName banners in HandlerSys.cpp for extraction method +
    //  verification detail (211/211 items, 0 tokenizer anomalies).
    // -----------------------------------------------------------------------
    TStringList *slCustomerCode = new TStringList();  // golden :506; populated in
                                                       //   ctor (HandlerSys.cpp)
    AnsiString   GetCustomerName();                    // golden :1100-1130

    // =======================================================================
    //  WAVE FW-HSYS-W22 -- AI(W906-FW-HSYS-W22) 20260826
    //  27 methods + 1 widget.  Full rationale, denominator, DEVIATIONS,
    //  GOLDEN NOTES and GATE REGISTER: the "ADDENDUM -- WAVE FW-HSYS-W22"
    //  banner at the head of this file.  Bodies: HandlerSys.cpp (tail).
    // =======================================================================

    // -- new widget ---------------------------------------------------------
    //  golden HandlerSys.h:395 declares this as `TLabeledEdit`, NOT TEdit
    //  (cross-checked against tools/dfm2rc/layout_out/HandlerSys_layout.gen.
    //  cpp:434, which records the same type from the .dfm).  Read by
    //  edtSearchCodeChange (->Text) and written by ExitBtnMouseDown
    //  (->Visible) and by the un-translated pcSettingChange (golden :1146).
    TLabeledEdit *edtSearchCode = new TLabeledEdit();

    // -- (1) customer-code incremental search --------------------------------
    //  GATE (H1) does NOT reach this method -- it never touches TempComp.
    //  See W22 ADDENDUM item (1) and GATE (H22-2).
    void edtSearchCodeChange();                     // golden :1149-1168 (Sender dropped, never read)

    // -- (2)/(3) form chrome --------------------------------------------------
    void ExitBtnMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);   // golden :1062-1071 (full golden signature, W22-D3)
    void FormClose();                               // golden :1073-1079 (Sender/TCloseAction dropped, W22-D2;
                                                    //   :1078 Do_Log NOT translated -- GATE (H22-1))

    // -- (4)-(27) QwertyKey openers ------------------------------------------
    //  All 24 bodies are golden's single `fQwertyKey->ShowQwertyKey(
    //  (TEdit *)Sender, ...)` line; the cast target is collapsed into the
    //  signature (W22-D1).  NOT wired (GATE (H22-4)); `fQwertyKey` is NULL
    //  until a wiring wave constructs it (GATE (H22-3)).
    void edtUserDefMaxContactHeightClick(TEdit *Sender);   // golden :1138-1142
    void edtUserDefineIndexZSafePosClick(TEdit *Sender);   // golden :1257-1261
    void edtCustomerCodeClick(TEdit *Sender);              // golden :1263-1266
    void edMaxMpaFBClick(TEdit *Sender);                   // golden :1268-1271
    void edMaxKpaClick(TEdit *Sender);                     // golden :1273-1276
    void edMinMpaClick(TEdit *Sender);                     // golden :1278-1281
    void edATCSystemPortClick(TEdit *Sender);              // golden :1283-1286
    void edATCSystemUseHeatClick(TEdit *Sender);           // golden :1288-1291
    void edHotGunFlow_Gun1_ChannelNoClick(TEdit *Sender);  // golden :1293-1297
    void edHotGunFlow_DevNoClick(TEdit *Sender);           // golden :1299-1302
    void edtMinYPitchClick(TEdit *Sender);                 // golden :1304-1307
    void edtMaxYPitchClick(TEdit *Sender);                 // golden :1309-1312
    void edCognexSystemCCDClick(TEdit *Sender);            // golden :1314-1317
    void ed24VMonitorPulseCountClick(TEdit *Sender);       // golden :1319-1323
    void edGroundMan_AlarmOhmClick(TEdit *Sender);         // golden :1325-1328
    void edIONPulseCountClick(TEdit *Sender);              // golden :1330-1333
    void edtTriTempTotalChClick(TEdit *Sender);            // golden :1335-1338
    void edtOutShtMaxTempClick(TEdit *Sender);             // golden :1340-1343
    void edtTriTemperature_MaxDegreeClick(TEdit *Sender);  // golden :1345-1349
    void edtTriTemperature_MinDegreeClick(TEdit *Sender);  // golden :1351-1355
    void edtIndexMaxTempClick(TEdit *Sender);              // golden :1357-1360
    void edt_Total_CompressorChange(TEdit *Sender);        // golden :1362-1365
    void edtMinXPitchClick(TEdit *Sender);                 // golden :1367-1370
    void edtMaxXPitchClick(TEdit *Sender);                 // golden :1372-1375
};

// DEVIATION: FormShow drops its golden TObject* Sender parameter -- matches
// every other translated form facade's identical simplification in this
// tree. CONSEQUENCE: golden's own `myLog.Do_Log(Sender, asUser, asLogPath);`
// tail call (golden :76) is #if 0-gated in the body rather than translated,
// since it needs the parameter this facade intentionally does not carry
// (myLog/Do_Log themselves DO exist, handlerlog.h -- not a missing-
// dependency gate, just a dropped-parameter consequence).

// AI(W906-FW3-HandlerSys-WA) 20260819: integration-pending -- NO
// `extern THandlerSystem *HandlerSystem;` here. See INTEGRATION-PENDING note
// above.

#endif // FORMS_FHANDLERSYS_H

