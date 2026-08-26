// =============================================================================
//  forms/fTrayMapping.h  --  non-VCL facade for golden's TfTrayMapping
//  (cTrayMapping.h / cTrayMapping.cpp) -- the Tray-Mapping / Tray-ID /
//  device-remain / Loader laser-scan engineering form.
//
//  AI(W906-FW-TRAYMAP-W32) 20260826: NEW FILE, wave FW-TRAYMAP-W32.
//  Golden source of truth:
//    D:\HT9045\HT9011UC_Code_V3.33.906.0_20260618\cTrayMapping.cpp (7,268 lines)
//    D:\HT9045\HT9011UC_Code_V3.33.906.0_20260618\cTrayMapping.h   (  767 lines)
//  Both cp950/Big5; decoded this wave with io.open(p, encoding='cp950') and
//  written out UTF-8.  Zero U+FFFD (checked on the finished files, see the
//  ENCODING section at the end of this banner).
//
//  =========================================================================
//  *** 1. THE CLASS BELOW IS **NOT** CALLED `TfTrayMapping` -- READ THIS ***
//  =========================================================================
//  Golden's class is `class TfTrayMapping : public TForm` (cTrayMapping.h:149)
//  and golden's global is `extern PACKAGE TfTrayMapping *fTrayMapping;`
//  (cTrayMapping.h:751).  In THIS port tree BOTH of those names are ALREADY
//  TAKEN, by the engine-facing stand-in:
//
//      acatchtray_shims.h:219   class TfTrayMapping { ... };   (minimal mirror)
//      acatchtray_shims.h:313   extern TfTrayMapping *fTrayMapping;
//      acatchtray_shims.cpp:93  TfTrayMapping *fTrayMapping = new TfTrayMapping();
//
//  That header is `#include`d by **81 translation units** (measured 20260826 at
//  wave close: a Python walk of the tree excluding build*/ and .git/, per file
//  blanking comments/strings and then blanking `#if 0` blocks, matching a real
//  `#include "...acatchtray_shims.h"` DIRECTIVE -- 81 .cpp, 0 .h).
//  ⚠ THE FIRST PASS OF THIS WAVE SAID 111 AND WAS WRONG: it counted files whose
//  TEXT anywhere contains the string "acatchtray_shims.h", which sweeps in 30
//  files that only MENTION it in a comment.  Corrected here rather than left,
//  because the whole argument below rests on this number being a real blast
//  radius.  So:
//
//    * Reusing the name `TfTrayMapping` here would be an ODR violation the
//      moment any TU saw both definitions, and would collide at LINK time on
//      every member name the two classes share (DoTrayIDCheck, DoAutoSkip,
//      CheckRunAutoRetry, IsRunTrayMapLaser, GetTrayDeviceCheckIsRemainIC,
//      DoSettingBeforeMotorMove, DoGetValueAfterMotorArrival,
//      GetTrayMapHasNullIC, WritePickMapLog, ... -- acatchtray_shims.cpp:80-92
//      defines offline bodies for all of them).
//    * So the facade class is named **`TfTrayMappingForm`**, and this header
//      declares **NO global pointer at all** -- `fTrayMapping` keeps pointing
//      at the shim object, exactly as it does today.  Nothing in the tree
//      changes behaviour because of this file.
//
//  This is the same situation forms/fContact.h documents under "THE GLOBAL
//  fContact IS ALREADY TAKEN", with one difference that matters: there only
//  the GLOBAL was taken (the shim class was named `TfContactShim`), so
//  fContact.h could keep golden's class name.  Here the shim took the CLASS
//  name too, so the class had to be renamed as well.
//
//  DO NOT "unify" the two by deleting/renaming the shim from inside a
//  translation wave.  That is a 111-TU change with its own gate story (the
//  shim's members carry deliberate offline DECISIONS -- see
//  acatchtray_shims.h:296-306 on DoTrayIDCheck returning TRUE, which is a
//  semantic choice, not a default).  It belongs in an integration wave.
//
//  =========================================================================
//  2. DENOMINATOR -- MEASURED THIS WAVE, NOT INHERITED
//  =========================================================================
//  The wave brief said "golden 有 161 處 `TfTrayMapping::`, port 0", and
//  flagged that number as probably wrong.  It is.  Re-measured here:
//
//    Method: a CHARACTER STATE MACHINE that blanks `//` comments, `/* */`
//    comments, and string/char literals (preserving newlines so line numbers
//    stay exact) -- not a regex, because golden has `}` inside comments and
//    whole functions wrapped in `/* */`, both of which have silently
//    mis-scoped past waves.  Then `\bTfTrayMapping\s*::\s*(~?\w+)` over the
//    blanked text, then a forward scan from each hit for the first
//    paren-depth-0 `{` (a DEFINITION) or `;` (a DECLARATION), then brace
//    matching for the span.
//
//    RESULT (20260826):
//      naive regex on the RAW text            : 161 hits   <- the brief's number
//      after comment/string blanking          : 150 hits
//      of which DEFINITIONS                   : 150
//      of which DECLARATIONS                  :   0
//    So 11 of the brief's 161 were text inside comments.  **The denominator
//    for every percentage in this file is 150 golden METHOD DEFINITIONS in
//    cTrayMapping.cpp.**
//
//    Signature-line-to-closing-brace spans of those 150 definitions total
//    6,040 lines of the file's 7,268.
//
//  =========================================================================
//  3. WHAT THIS WAVE DELIVERS -- BOTH DENOMINATORS, ALWAYS QUOTE BOTH
//  =========================================================================
//      DELIVERED : 42 of 150 golden method definitions   = 28.0% BY METHOD
//                  355 of 6,040 golden body lines        =  5.9% BY LINES
//      GATED     : 108 of 150 = 94 declared-not-defined + 12 omitted entirely
//                  + golden's ctor and dtor, which become PORT ctor/dtor
//                  (DEVIATION (D-2) / (D-6)).
//
//  (Both figures re-measured from the FINISHED files at wave close, not from
//  the plan: the plan said 44/376 and the wave LOST TWO late -- AddAOIMemeber
//  and InitialAOIGroup, see GATE (G-8).)
//
//  ⚠ QUOTING ONLY THE METHOD COUNT OVERSTATES THIS WAVE BY ~4.7x.  The 42
//  delivered bodies are the SMALL ones -- accessors, AOI-object delegation
//  reads, grid/tab UI setters and numeric-keypad launchers.  The 106 gated
//  ones include every 100-to-450-line state machine in the file
//  (TimerProcessTrayDataTimer 445, DoTrayMapCCD 398, DoTrayIDCCD 379,
//  DoLaserScanFromLoader 325, DoTrayMapAutoTuneCCD 320, DoTrayID2CCD 260,
//  ClientSocket_Keyence1Read 216, DoCoverTrayIDKeyence 207, ...).
//
//  BATCH CRITERION (the wave brief asked for "one batch of 30-50, say what
//  the rule was").  A method is in this batch iff ALL THREE hold:
//    (a) READ-ONLY by the wave's own exit rules -- its body, and everything it
//        transitively calls, performs no machine motion (motor / cylinder /
//        vacuum / home / servo / TrayArm), no persistence (WriteIniData,
//        CheckAndReadIniData, fopen/CreateFile, Save*, SaveToFile,
//        WriteDataToFile, DeleteFile), and no outbound command (SECS
//        EventReport, TCP socket send, GPIB, FTP);
//    (b) every symbol its body needs ALREADY EXISTS in this port tree -- no
//        new shim was invented for it (wave rule 5); and
//    (c) it is small enough to verify by reading golden end-to-end.  Every one
//        of the 42 was read IN FULL from the cp950-decoded golden this wave;
//        none was classified from its name or from a grep alone.
//    (d) -- LEARNED DURING THIS WAVE, see GATE (G-8) -- it must not CALL a
//        gated method.  An active body that does emits an unconditional
//        undefined reference into this object file, which turns the linker
//        interlock from a tripwire into a permanent build break.
//
//  REMAINING AFTER THIS WAVE: 108 of 150 methods (5,685 of 6,040 body lines).
//  The next batch has a natural shape -- the four CCD/Keyence socket-protocol
//  state machines are one cluster, the laser-scan motion engine is another,
//  the tray-log writers a third -- but each needs its own gate story, not a
//  "finish the facade" pass.
//
//  =========================================================================
//  4. SAFETY POSTURE -- HOW THE GATES ARE ENFORCED
//  =========================================================================
//  Every gated method that CAN be spelled in this port is DECLARED here and
//  DELIBERATELY NOT DEFINED in forms/fTrayMapping.cpp.  The LINKER is the
//  interlock: the moment any future wave -- or the browser write path -- tries
//  to CALL one, the build fails with
//      undefined reference to `TfTrayMappingForm::<method>'
//  naming the exact entry point.  This is forms/fMotorTest.h's idiom and
//  forms/fContact.h's idiom, chosen for the same reason both state: an `#if 0`
//  body is one keystroke from moving a motor, and an empty stub reads to the
//  next person as "already translated".  `TfTrayMappingForm` has no virtual
//  member, so an undefined member function costs nothing at link time until
//  somebody calls it.
//
//  Twelve gated methods cannot even be DECLARED, because their golden
//  signatures name types with zero port (TCustomWinSocket, TCloseAction,
//  TWMKey, Pointer/WORD).  Those are OMITTED ENTIRELY and listed by name in
//  the GATE REGISTER so they are never mistaken for an oversight.
//
//  UN-GATING PROTOCOL: translate the body in a wave that owns the story for
//  what it touches (who may issue it, what must be homed / connected / powered
//  first, and what re-checks it on the C++ side).  Never as a side effect of
//  "finishing the facade".
//
//  =========================================================================
//  5. GATE REGISTER -- all 106, grouped, each with the evidence that put it
//     there.  Golden line numbers are signature-line to closing brace.
//  =========================================================================
//
//  (G-1) OUTBOUND TCP -- the CCD / Keyence / device-count socket protocol.
//        All of these reach `SendCCDTrayCommand` (golden :631-726, which calls
//        the TClientSocket send path) or `SendKeyenceCommand` (:3881-3986), or
//        flip a TClientSocket ->Active / arm the timer that does.
//          SendCCDTrayCommand            :631-726    (96)
//          SendKeyenceCommand            :3881-3986  (106)
//          btTray_ID_TriggerClick        :593-629    (37)
//          TimerDownCCDTrayConnectTimer  :782-966    (185)
//          TimerCCDTrayInitialTimer      :973-1094   (122)
//          BtTray_ID_ConnectClick        :1096-1102  (7)   arms TimerDownCCDTrayConnect
//          BtTray_ID_DisconnectClick     :1104-1136  (33)
//          BtTray_ID_ConnectKYClick      :4286-4292  (7)   arms TimerDownCCDTrayConnect
//          BtTray_ID_DisconnectKYClick   :4294-4329  (36)
//          btTray_ID_TriggerKYClick      :4331-4375  (45)
//          btLoaderDC_TriggerClick       :4898-4902  (5)   SendCommandSocket
//          BtLoaderDC_ConnectClick       :4874-4881  (8)   socket Close()/DoCommuncation()
//          CloseAllSocket                :5339-5343  (5)   closes every AOI socket
//          DoTrayIDCCD                   :1997-2375  (379)
//          DoTrayID2CCD                  :3458-3717  (260)
//          DoTrayMapCCD                  :2389-2786  (398)
//          DoTrayMapAutoTuneCCD          :2821-3140  (320)
//          DoTrayIDKeyence               :4377-4466  (90)
//          DoTrayIDKeyence2              :4469-4582  (114)
//          DoMultileTrayIDKeyence3       :4591-4671  (81)
//          DoTrayIDKeyence4              :4673-4762  (90)
//          DoCoverTrayID                 :5812-5834  (23)  dispatches to Keyence or NFC
//          DoCoverTrayIDKeyence          :5836-6042  (207)
//          TimerProcessTrayDataTimer     :1264-1708  (445) also calls DoLaserScanFromLoader
//          btnLoadFileClick              :5478-5482  (5)   } every one of these
//          btnClearBufferClick           :5484-5488  (5)   } delegates straight
//          btnGrabClick                  :5490-5494  (5)   } into a TfAOI
//          btnGetValueClick              :5496-5500  (5)   } AOI-command
//          btnSetLotClick                :5511-5515  (5)   } virtual, which is
//          btnCreateFileClick            :5517-5521  (5)   } an ASCII command
//          btnGoldenImageClick           :5523-5527  (5)   } over the AOI socket
//          DoSettingBeforeMotorMove      :5472-5475  (4)   } (TfAOILaserScan.h
//          DoGetValueAfterMotorArrival   :5462-5465  (4)   }  DoSendCommand)
//          cbSimulationAOICommandClick   :5502-5509  (8)   flips AOI sim mode
//
//  (G-2) PERSISTENCE -- writes a file, or writes an ini key back.
//        NOTE (wave rule): `CheckAndReadIniData` COUNTS AS A WRITE -- common.cpp
//        :603 WriteInteger's the default back when the key is absent; only the
//        `ReadIniData` family (common.cpp:685-757) is pure read.
//          spbSaveClick                  :459-510    (52)  WriteIniData
//          SaveFile_ScanLine()           :5697-5723  (27)  WriteIniData
//          SaveFile_ScanLine(4 args)     :5725-5750  (26)  WriteIniData
//          ReadFile_ScanLine()           :5631-5655  (25)  CheckAndReadIniData
//          ReadFile_ScanLine(4 args)     :5657-5695  (39)  CheckAndReadIniData
//          ReadFile                      :512-591    (80)  calls ReadFile_ScanLine
//          DoIniDataToForm               :407-457    (51)  calls ReadFile + dmTrayMotor->StartSetSpeed()
//          FormShow                      :203-285    (83)  calls DoIniDataToForm
//          WriteTrayMapLog               :3186-3241  (56)  WriteDataToFile
//          WritePickMapLog               :3243-3272  (30)  WriteTrayMapLog
//          WriteUnloadMapLog             :3411-3448  (38)  WriteDataToFile
//          AddCCDTrayCommunicationLog    :728-780    (53)  WriteDataToFile
//          AddKeyenceCommunicationLog    :3988-4028  (41)  WriteDataToFile
//          ShowMSDCCommLog               :4783-4798  (16)  WriteDataToFile (Sim/AutoLearning)
//          RecordMsgLaser                :4770-4775  (6)   AddCCDTrayCommunicationLog + ShowMSDCCommLog
//          RecordErrorMsgLaser           :4777-4781  (5)   RecordMsgLaser
//          RecordSimTaskMsg              :4800-4806  (7)   ShowMSDCCommLog
//          WriteLaserScanFlowLog         :4808-4821  (14)  WriteDataToFile, ALWAYS (not sim-only)
//          CCDTrayID_Decoding            :1710-1797  (88)  WriteTrayMapLog
//          CCDTrayMap_Decoding           :1799-1975  (177) WriteDataToFile + WriteTrayMapLog
//          Button1Click                  :1977-1981  (5)   CCDTrayMap_Decoding
//          Button2Click                  :1983-1987  (5)   CCDTrayID_Decoding
//          Button4Click                  :3284-3334  (51)  SaveToFile + ExecZipCommand
//          AutoConnectNetwork            :3346-3372  (27)  SaveToFile + ExecZipCommand ("net use z:")
//          DoReadTrayData                :3142-3184  (43)  DeleteFile
//          ClearTrayIDByLot              :7104-7109  (6)   SaveToFile
//          FormDestroy                   :335-405    (71)  socket teardown + LogSoftwareOffTime
//
//  (G-3) MACHINE MOTION / PNEUMATICS.
//          MoveTrayArmToSafeColor        :4967-4970  (4)   MOT[MTrayX].MotorMove
//          DoLaserScanFromLoader         :4972-5296  (325) MLoaderY move + auto-home + cylinders
//          ReleaseLoaderBreak            :5426-5455  (30)  Cylinder[C_LoaderEdgePush/UpPress/TrayY_Fixer]
//          btnMoveInClick                :5529-5534  (6)   TrayMoveIn(Prod.iMLoaderYSurePos)
//          btnMoveOutClick               :5536-5542  (7)   TrayMoveOut(Prod.iMLoaderYCarPos)
//          DoMoveOut                     :5621-5624  (4)   TfAOI::DoMoveOut -> Loader-Y motor SM
//          DoMoveIn                      :5626-5629  (4)   TfAOI::DoMoveIn  -> Loader-Y motor SM
//          btnSimulateRemainICClick      :4904-4952  (49)  DoLaserScanFromLoader
//
//  (G-4) ALARM / OPERATOR STOP.  `ShowErrorMessage` raises a handler alarm and
//        stops motion; it is a machine action, not a message box.
//          iComparUnloadIC               :3724-3751  (28)  ShowErrorMessage + NewRecordProcess
//          ShowTrayFunErrMsg             :3786-3853  (68)  ShowErrorMessage x12
//
//  (G-5) CROSS-FILE GAP -- the body needs a symbol this port does not have,
//        and wave rule 5 forbids inventing one here.
//        (a) `fTrayMapping->iTrayDeviceCountDate[...]`.  Golden reaches its own
//            members through the GLOBAL pointer inside its own member
//            functions.  In this port that global is the SHIM type
//            (acatchtray_shims.h:219), which has `iTrayMappingDate` but NOT
//            `iTrayDeviceCountDate` -- a hard compile blocker, not dead code:
//              iVisonAndHandlerComparLessCnt :3753-3768 (16)
//              iVisionICCnt                  :3770-3784 (15)
//        (b) `cLineScanRemainICYieldRecord` (golden cTrayMapping.h:47-67) and
//            its `cDatabaseJson` (golden :29-45) have ZERO port -- grepped the
//            whole tree 20260826, only text hits are in this file's own
//            comments.  It is also itself a WRITER (AddYieldData appends to
//            D:\HT9045_Log\LineScanRemainICYield\*.json), so it is (G-2) as
//            well.  Member `yieldRemainIC` is therefore NOT declared below:
//              btnAddRandomClick             :5564-5567 (4)
//              SetYieldDatas                 :5569-5574 (6)
//              btnRefreshYieldClick          :5576-5579 (4)
//        (c) `TfRFID` (golden cTrayMapping.h:69-147) has ZERO port -- the only
//            `TfRFID` text in the tree is in two generated
//            tools/dfm2rc/.../RFID_layout.gen.cpp files.  Member `ldRFID` is
//            NOT declared below; these 12 bodies + the timer all deref it:
//              btnLdRFIDConnClick        :7007-7023 (17)
//              btnLdRFIDDisConnClick     :7032-7044 (13)
//              btnLdRFIDClear1Click      :7046-7051 (6)
//              btnReadID1Click           :7053-7057 (5)
//              btnReadUID1Click          :7059-7063 (5)
//              btnReadMemData1Click      :7065-7068 (4)
//              btnLdRFIDReadBlock1Click  :7070-7073 (4)
//              btnLdRFIDClear2Click      :7075-7080 (6)
//              btnReadUID2Click          :7082-7086 (5)
//              btnLdRFIDReadBlock2Click  :7088-7091 (4)
//              btnReadMemData2Click      :7093-7096 (4)
//              btnReadID2Click           :7098-7102 (5)
//              tmrNFCTimer               :7111-7239 (129)  also drives the serial RFID reader
//        (d) `TPageControl::Pages[i]` / `->PageCount`.  vclcompat models only
//            ->ActivePageIndex, and forms/fConfiguration.h:270-280 states why
//            ->Pages is deliberately NOT modelled (a page list needs a real
//            parent/child tree this facade family does not have; same gap as
//            forms/fSpeed.h GATE (S4) / forms/fHandlerSys.h GATE (H1) /
//            forms/fLotInfo.h GATE (WA-4)).  Following that precedent:
//              SearchPageCtrl            :5779-5790 (12)
//        (e) golden's plain VCL `ShowMessage(const AnsiString&)` (Dialogs.hpp)
//            has NO tree-wide stand-in; every consumer TU makes its own gated
//            local (ATC/ATCSystem.cpp:193, SECSGEM/uHGemEquipment.cpp:3264,
//            SECSGEM/SecsSvEcRegistration.cpp:44, ProductionInfo/TfFTP.cpp:167).
//            Making a 5th one here would be exactly the shim rule 5 forbids,
//            and this method also depends on (d):
//              ShowCheckDevicePage       :5792-5797 (6)
//        (f) `TStringGrid`-based tray file browse.  SearchTrayMapFile derefs
//            `DirectoryListBox1` / `FileListBox1` (golden cTrayMapping.h:211-212,
//            TDirectoryListBox / TFileListBox) and reads the ATC network drive
//            `z:\<lot>`; vclcompat/FileListBox.h models a file list but not a
//            TDirectoryListBox, and the method's whole purpose is the Z: mount
//            that (G-2) AutoConnectNetwork creates:
//              SearchTrayMapFile         :3374-3404 (31)
//
//  (G-6) OMITTED ENTIRELY -- not even declared, because the golden SIGNATURE
//        names a type with zero port in this tree.  Listed so the member list
//        is honestly incomplete rather than silently so.
//          ClientSocket_Tray_IDConnect       :1216-1262 (47) } TCustomWinSocket*
//          ClientSocket_Tray_IDDisconnect    :1150-1160 (11) } (+ TErrorEvent
//          ClientSocket_Tray_IDError         :1162-1187 (26) }  for the Error
//          ClientSocket_Tray_IDRead          :1189-1214 (26) }  handlers)
//          ClientSocket_Keyence1Connect      :3864-3879 (16) }
//          ClientSocket_Keyence1Disconnect   :4030-4040 (11) }
//          ClientSocket_Keyence1Error        :4042-4067 (26) }
//          ClientSocket_Keyence1Read         :4069-4284 (216)}
//          ClientSocket_CoverTray_ID_LoaderCarConnect :6106-6121 (16)
//          FormClose                         :319-333   (15) TCloseAction&
//          FormShortCut                      :3855-3862 (8)  TWMKey& -- the
//              tree-wide convention for this one signature is OMIT ENTIRELY
//              (forms/fContact.h GATE (X-27), uTemp_Set.cpp:6618,
//               uYieldMonitoring.cpp:2613, cBinSel.cpp:4195, cSetUp.cpp:250).
//          commRFIDReceiveData               :7025-7030 (6)  Pointer / WORD
//
//  (G-8) TRANSITIVELY GATED -- the body itself is harmless, but it CALLS a
//        gated method, so delivering it would emit an unconditional
//        `undefined reference` into forms/fTrayMapping.o and break every link
//        that pulls this TU in.  Both of these were written as ACTIVE bodies
//        and moved here before the wave closed.
//          AddAOIMemeber     :5419-5424 (6)   installs RecordMsgLaser /
//              RecordErrorMsgLaser as the TfAOI objects' log callbacks
//              (golden `aoi->RecordMsgLaser=RecordMsgLaser;`).  Both callbacks
//              are GATE (G-2) file writers.  Note the port would ALSO have to
//              deviate here: golden assigns a BCB6 `__closure` bound method and
//              TfAOILaserScan.h's TPointVoidShowMsg is `std::function<void
//              (AnsiString)>`, so it needs a `[this](AnsiString s){...}` lambda
//              (TfAOILaserScan.cpp:300 sets that precedent).  Do both in the
//              wave that un-gates the two writers, not before.
//          InitialAOIGroup   :5398-5412 (15)  calls AddAOIMemeber twice.  It is
//              otherwise ready: TfTrayDeviceCheckFromLoader and
//              TfTrayMapFromLoader are fully translated (TfAOILaserScan.cpp)
//              and `SetTrayIC(mtTrayMap)` only passes an incomplete-type
//              pointer, which compiles.
//
//  (G-7) THE CTOR AND DTOR -- see DEVIATION (D-2) / (D-6).  Golden's ctor body
//        (:59-195, 137 lines) is NOT translated; it is declared as `Init()` and
//        left undefined.  Golden's dtor body (:197-201) needs `ldRFID`, i.e.
//        (G-5c), so it is not translated either.
//
//  =========================================================================
//  6. DEVIATIONS FROM GOLDEN (all deliberate; each says what changed and why)
//  =========================================================================
//  (D-1) `__fastcall` dropped from every signature -- tree-wide facade
//        convention (forms/fContact.h:615).  On MinGW `__fastcall` is a REAL
//        calling convention that decorates the mangled name
//        (vclcompat/vcl_compat.h:24-55), so it must be all-or-nothing per
//        declaration/definition pair; the facades chose "none".
//  (D-2) CTOR / Init() SPLIT (wave rule 2, and it is load-bearing here).
//        Golden's ctor (:59-195) does far more than fill fields: it `new`s 8
//        TStringLists, wires 6 widget-pointer arrays, READS CUSTOMER_CODE /
//        USE_MR_SYSTEM / USE_TRAY_MAPPING / USE_KEYENCE_EMPTY / CosFunction /
//        USE_LD_Rot_Arm, calls TrayMapSetDefaultView() -- which reads
//        MOT[MMTrayY].Tray.XItem, i.e. LIVE MOTOR CONFIG -- then
//        InitialAOIGroup(), InitialcbLaserScanMode(), SetSimulationState() and
//        constructs a TfRFID.  A translation unit's ctor runs BEFORE main();
//        this tree has already paid for that once (forms/fLaserSensor's ctor
//        called InitLaserEdtList() while elLaser was still NULL -> 88 of 134
//        ctest binaries SEGFAULTed).  So: the PORT ctor below fills fields
//        only, and golden's ctor body is `Init()` -- DECLARED, NOT DEFINED
//        (it is also blocked by (G-5c) TfRFID and by the socket/edit widget
//        arrays this wave does not declare).
//  (D-3) Widget members are OWNING raw pointers with in-class `new`
//        initialisers, matching forms/fContact.h (D-3).  Safe here for a
//        stronger reason than there: this header declares NO global, so
//        nothing constructs a TfTrayMappingForm at static-initialisation time
//        -- or at all, today.
//  (D-4) `Close()` / `Show()` / `ShowModal()` / `BringToFront()` are permanent
//        offline no-ops (no window, no modal loop).  Only `Close()` is reached
//        from a delivered body (sbtExitClick, golden :1146).
//  (D-5) `UpdateConnectionState`'s parameter is `TfTrayMappingBitBtn *`, not
//        golden's `TBitBtn *`.  Golden writes `btn->Font->Color=clRed;`
//        (:4889/:4894) and vclcompat::TBitBtn has no ->Font.  The facade-local
//        subclass below adds exactly that one member -- the same shape
//        forms/fContact.h's TfContactGroupBox (adds ->Height) and
//        forms/fConfiguration.h's TfConfigurationGroupBox (adds ->Color) use.
//        Only widget of this type in golden's list that a delivered body ever
//        passes is BtLoaderDC_Connect, which is declared with the subclass
//        type, so no call site needs a cast.
//  (D-6) The PORT dtor is EMPTY, and it is a deviation, not a translation:
//        golden's `~TfTrayMapping()` (:197-201) is
//        `if(USE_LD_Rot_Arm==true) delete ldRFID;`, and `ldRFID` cannot be
//        declared here -- (G-5c).  Nothing leaks in the port because `ldRFID`
//        does not exist here to leak.
//  (D-8) `True` -> `true` in CheckRunAutoRetry (golden :5610).  BCB6's Delphi
//        compatibility constant `True` has ZERO port (grepped 20260826; this
//        was the only `== True` in any .cpp in the tree).  Semantically
//        identical; recorded because it is the one token in the delivered
//        bodies that is not a byte-for-byte copy of golden.
//  (D-7) MEMBER SURFACE IS MINIMAL BY DESIGN.  Golden's class publishes ~200
//        __published widgets plus ~90 data members.  This header declares ONLY
//        what a DELIVERED body actually dereferences (plus the two enumerators
//        and the two golden one-line inline getters that come with them).
//        That is acatchtray_shims.h's own stated rule ("Adding it would be
//        inventing surface nothing dereferences", :299-305) and it keeps this
//        wave from guessing ~200 widget types it never exercises.  The next
//        batch adds members as its bodies need them.
//
//  =========================================================================
//  7. SYMBOLS THIS TU NEEDS (for whoever wires CMakeLists -- NOT this wave)
//  =========================================================================
//  forms/fTrayMapping.cpp resolves against, and only against:
//    * vclcompat            -- AnsiString, TStringList (Sort/Find/LoadFromFile),
//                              TStringGrid, Controls.h widget family,
//                              ShiftState.h (TMouseButton/TShiftState),
//                              LedCore.h (TColor/clRed/clGreen), SysUtils.h
//                              (FileExists)
//    * ht9045_globals       -- CUSTOMER_CODE-family config ints
//                              (USE_TRAY_MAPPING, INSTALL_OCR_YMot,
//                              C_LoaderUpPress / C_LoaderEdgePush /
//                              C_TrayY_Fixer, MLoaderY, MMTrayY, N_INTEGER,
//                              N_PORT, DUMMY), CosFunction, LastSet, Prod,
//                              TestIF_File, asTrayIDByLot, OnlyNumberInPut
//    * ht9045_motor         -- MOT[]      (CMakeLists.txt:826  Motor/mymotor.cpp)
//    * ht9045_io            -- Cylinder[] (CMakeLists.txt:973  mycylin.cpp)
//    * ht9045_sm            -- TfAOI + TfTrayDeviceCheckFromLoader +
//                              TfTrayMapFromLoader (CMakeLists.txt:2223
//                              TfAOILaserScan.cpp); ShowMyMessage
//                              (CMakeLists.txt:1421 canary_support.cpp)
//    * ht9045_forms         -- fQwertyKey  (CMakeLists.txt:699 forms/fQwertyKey.cpp)
//    * ht9045_core          -- common.cpp (asTrayIDByLot / OnlyNumberInPut)
//
//  SUGGESTED LANDING (the main loop decides, not this wave): **ht9045_sm**,
//  for exactly the reason forms/fContact.cpp (CMakeLists.txt:2260) and
//  forms/fMotorTest.cpp (:2262) are already there rather than in ht9045_forms
//  -- this TU reaches ht9045_motor + ht9045_io + ht9045_sm, which is outside
//  ht9045_forms' declared dependency layer (CMakeLists.txt:714 links
//  ht9045_forms against vclcompat / ht9045_globals / ht9045_core only), and
//  CMakeLists.txt:589-593 records that widening that edge does not even
//  configure.
//
//  It does NOT reach the SECS stack, the tester interface, or the CCD/Keyence
//  socket family -- and it must NOT be given `fTrayMapping`.  Landing it in
//  the SAME archive as acatchtray_shims.cpp (:1776, also ht9045_sm) is safe:
//  the two classes have different names and share no symbol.
//
//  ⚠ ONE KNOWN BUILD-OUTPUT CHANGE: ClearAllAOI (golden :4850-4856) emits
//  `-Wdelete-non-virtual-dtor` under this tree's -Wall.  That is a REAL GOLDEN
//  DEFECT translated verbatim -- see the GOLDEN NOTE at that body in the .cpp
//  before deciding to silence it.  If a warning-free build matters more than
//  carrying that one 7-line body, gate ClearAllAOI as well: its only golden
//  caller, FormDestroy, is already GATE (G-2), so nothing else changes.
//
//  =========================================================================
//  8. ENCODING
//  =========================================================================
//  Golden decoded cp950 -> written UTF-8.  This file and its .cpp are bare LF
//  (asserted: bytes.count(b'\r') - bytes.count(b'\r\n') == 0 AND
//  bytes.count(b'\r\n') == 0), decode as UTF-8, and contain zero U+FFFD.
// =============================================================================
#ifndef FORMS_FTRAYMAPPING_H
#define FORMS_FTRAYMAPPING_H

#include "vclcompat/vcl_compat.h"    // AnsiString, TStringList, TObject
#include "vclcompat/Controls.h"      // TControl/TLabel/TEdit/TCheckBox/TComboBox/
                                     // TButton/TBitBtn/TSpeedButton/TPageControl/
                                     // TTabSheet/TFont   (all `using`-ed to global)
#include "vclcompat/StringGrid.h"    // vclcompat::TStringGrid (NOT global -- see below)
#include "vclcompat/ShiftState.h"    // TMouseButton / TShiftState / mbLeft / mbRight
#include "TfAOILaserScan.h"          // TfAOI, TfTrayDeviceCheckFromLoader,
                                     // TfTrayMapFromLoader, and its own `class TTMyTray;`

#include <map>                       // golden cTrayMapping.h:571 std::map<int,TfAOI*>

// vclcompat/StringGrid.h deliberately does not publish its type globally
// ("Consumers add their own `using vclcompat::TStringGrid;`", StringGrid.h tail).
// golden spells it unqualified, so do that here -- this header is only reached
// by forms/fTrayMapping.cpp today, and the name is collision-free tree-wide.
using vclcompat::TStringGrid;

// ===========================================================================
//  Facade-local widget stand-ins.  Same idiom as forms/fContact.h:902-948 and
//  forms/fConfiguration.h:269-294: subclass the vclcompat stock widget and add
//  ONLY the member golden actually touches that the stock one lacks.
// ===========================================================================

// golden `TBitBtn *BtLoaderDC_Connect` (cTrayMapping.h:346).  vclcompat::TBitBtn
// (Controls.h:467) carries Caption but no ->Font; golden writes
// `btn->Font->Color=clRed;` / `clGreen` in UpdateConnectionState
// (cTrayMapping.cpp:4889 / :4894).  See DEVIATION (D-5).
class TfTrayMappingBitBtn : public TBitBtn
{
public:
    TFont *Font;
    TfTrayMappingBitBtn() : Font(new TFont()) {}
    virtual ~TfTrayMappingBitBtn() { delete Font; }
};

// golden `TPageControl *PageControl1` (cTrayMapping.h:152).  vclcompat models
// only ->ActivePageIndex; golden assigns ->ActivePage (a TTabSheet*) in
// ShowLaserScanSheet (cTrayMapping.cpp:5416).  ->Pages / ->PageCount are NOT
// modelled -- their only delivered-adjacent use site, SearchPageCtrl, is
// GATE (G-5d), exactly as forms/fConfiguration.h:270-280 decided for the same
// gap.
class TfTrayMappingPageControl : public TPageControl
{
public:
    TTabSheet *ActivePage;
    TfTrayMappingPageControl() : ActivePage(0) {}
    virtual ~TfTrayMappingPageControl() {}
};

// ===========================================================================
//  TfTrayMappingForm -- non-VCL facade for golden
//  `class TfTrayMapping : public TForm` (cTrayMapping.h:149-749).
//  ⚠ The name is NOT TfTrayMapping.  See banner section 1 before "fixing" it.
// ===========================================================================
class TfTrayMappingForm
{
private:
    // -- golden private: User declarations (cTrayMapping.h:559-584) ----------
    // Only the members a DELIVERED body dereferences -- DEVIATION (D-7).
    std::map<int, TfAOI *> mapAOI;                       // golden h:571
    void AddAOIMemeber(TfAOI *aoi);                      // golden h:572 / cpp :5419-5424  GATE (G-8)
    int  GetAOISize() { return mapAOI.size(); }          // golden h:573 (inline, verbatim)
    void DeleteAOI()  { mapAOI.erase(GetAOISize() - 1); }// golden h:574 (inline, verbatim)
    void ClearAllAOI();                                  // golden h:575 / cpp :4850-4856  ACTIVE
    void ShowMSDCCommLog(AnsiString asStr);              // golden h:577 / cpp :4783-4798  GATE (G-2)
    void UpdateConnectionState(TfTrayMappingBitBtn *btn);// golden h:578 / cpp :4883-4896  ACTIVE (D-5)
    bool IsNumber(AnsiString &asValue);                  // golden h:579 / cpp :5298-5309  ACTIVE
    int  iLoaderYCWSpeedPerc;                            // golden h:580
    bool bSimulate;                                      // golden h:581
    int  iSettingCount;                                  // golden h:582

public:
    // -- golden __published widgets (cTrayMapping.h:151-476) -----------------
    // Declared in golden's own relative order, keeping golden's .dfm leaf
    // names verbatim.  ONLY the ones a delivered body touches -- DEVIATION (D-7).
    TfTrayMappingPageControl *PageControl1;      // golden h:152
    TSpeedButton             *sbtExit;           // golden h:156
    TStringGrid              *sgTraymap;         // golden h:207
    TStringGrid              *sgSuckmap;         // golden h:209
    TStringGrid              *sgTrayDeviceCnt1;  // golden h:234
    TStringGrid              *sgTrayDeviceCnt2;  // golden h:235
    TStringGrid              *sgTrayDeviceCnt3;  // golden h:236
    TEdit                    *edLoaderDC_Address;// golden h:344
    TEdit                    *edLoaderDC_Port;   // golden h:345
    TfTrayMappingBitBtn      *BtLoaderDC_Connect;// golden h:346
    TComboBox                *cbLaserScanMode;   // golden h:348
    TButton                  *btnSimulateRemainIC;// golden h:355
    TTabSheet                *tsCommand;         // golden h:367
    TCheckBox                *cbPassAOI;         // golden h:378
    TCheckBox                *cbPassActive;      // golden h:379
    TTMyTray                 *mtTrayMap;         // golden h:388  (incomplete type -- pointer only)
    TLabel                   *lbResult;          // golden h:391
    TButton                  *btnAddRandom;      // golden h:393
    TStringGrid              *sgTrayDeviceYield; // golden h:395
    TCheckBox                *cbEnableTrayMapLaser;     // golden h:396
    TCheckBox                *cbEnableDeviceRemainLaser;// golden h:397

    // -- golden public: User declarations (cTrayMapping.h:585-748) -----------
    TfTrayMappingForm();                    // PORT ctor -- fields only, DEVIATION (D-2)
    ~TfTrayMappingForm();                   // PORT dtor -- empty,       DEVIATION (D-6)
    // Init() carries golden's ctor BODY (cpp :59-195).  DECLARED, NOT DEFINED
    // -- GATE (G-7).  Un-gating needs the socket/edit widget arrays (G-1/G-6)
    // and TfRFID (G-5c) first.
    void Init();

    void DoIniDataToForm();                                                     // golden h:588 / cpp :407-457    GATE (G-2)
    void SendCCDTrayCommand(int index, AnsiString Msg, AnsiString Msg2 = "");   // golden h:589 / cpp :631-726    GATE (G-1)
    void AddCCDTrayCommunicationLog(int index, AnsiString Str);                 // golden h:590 / cpp :728-780    GATE (G-2)
    void ChangeTraySetupFile();                                                 // golden h:591 / cpp :968-971    ACTIVE
    bool bDoTrayMapTune;                                                        // golden h:607
    TStringList *listTrayIDByLot;                                               // golden h:610
    void ReadFile();                                                            // golden h:611 / cpp :512-591    GATE (G-2)
    bool CCDTrayID_Decoding(int Channel);                                       // golden h:615 / cpp :1710-1797  GATE (G-2)
    bool CCDTrayMap_Decoding(int Channel);                                      // golden h:616 / cpp :1799-1975  GATE (G-2)
    bool DoTrayIDCCD();                                                         // golden h:623 / cpp :1997-2375  GATE (G-1)
    bool DoTrayID2CCD();                                                        // golden h:624 / cpp :3458-3717  GATE (G-1)
    bool DoTrayMapCCD(int iTrayType);                                           // golden h:626 / cpp :2389-2786  GATE (G-1)
    void TrayMapSetDefaultView();                                               // golden h:633 / cpp :287-299    ACTIVE
    void TrayMapSetDefault(TStringGrid *tsGrid);                                // golden h:634 / cpp :301-317    ACTIVE
    bool fShow;                                                                 // golden h:646
    bool DoTrayMapAutoTuneCCD(int iiTuneType, bool Reset = false);              // golden h:647 / cpp :2821-3140  GATE (G-1)
    bool DoReadTrayData(AnsiString FileName);                                   // golden h:649 / cpp :3142-3184  GATE (G-2)
    int  iComparUnloadIC(int iAuto);                                            // golden h:650 / cpp :3724-3751  GATE (G-4)
    int  iVisonAndHandlerComparLessCnt(int iAuto);                              // golden h:651 / cpp :3753-3768  GATE (G-5a)
    int  iVisionICCnt(int iAuto);                                               // golden h:652 / cpp :3770-3784  GATE (G-5a)
    void WriteTrayMapLog(int index, AnsiString Message);                        // golden h:653 / cpp :3186-3241  GATE (G-2)
    void WritePickMapLog();                                                     // golden h:654 / cpp :3243-3272  GATE (G-2)
    void AutoConnectNetwork();                                                  // golden h:656 / cpp :3346-3372  GATE (G-2)
    void SearchTrayMapFile();                                                   // golden h:657 / cpp :3374-3404  GATE (G-5f)
    void WriteUnloadMapLog(int iWhichAuto, bool bTray = false);                 // golden h:658 / cpp :3411-3448  GATE (G-2)
    int  ShowTrayFunErrMsg(int iErr, int iTrayType);                            // golden h:667 / cpp :3786-3853  GATE (G-4)
    void SendKeyenceCommand(int index, AnsiString Msg, AnsiString Msg2 = "");   // golden h:668 / cpp :3881-3986  GATE (G-1)
    void AddKeyenceCommunicationLog(int index, AnsiString Str);                 // golden h:669 / cpp :3988-4028  GATE (G-2)
    bool DoTrayIDKeyence();                                                     // golden h:671 / cpp :4377-4466  GATE (G-1)
    bool DoTrayIDKeyence2(int iFunction);                                       // golden h:672 / cpp :4469-4582  GATE (G-1)
    bool DoCoverTrayIDKeyence(int iPos, bool bAlarm = false);                   // golden h:673 / cpp :5836-6042  GATE (G-1)
    bool DoCoverTrayID(int iFunction, bool bAlarm = false);                     // golden h:674 / cpp :5812-5834  GATE (G-1)
    bool DoTrayIDKeyence4();                                                    // golden h:676 / cpp :4673-4762  GATE (G-1)
    bool DoMultileTrayIDKeyence3();                                             // golden h:678 / cpp :4591-4671  GATE (G-1)

    // -- 殘料檢查 From Loader block (golden h:679-743) -----------------------
    TfAOI *GetAOI(int iIndex);                                                  // golden h:681 / cpp :4858-4862  ACTIVE
    TfAOI *GetAOI(AnsiString asName);                                           // golden h:682 / cpp :4864-4872  ACTIVE
    void RecordMsgLaser(AnsiString asMsg);                                      // golden h:688 / cpp :4770-4775  GATE (G-2)
    void RecordErrorMsgLaser(AnsiString asMsg);                                 // golden h:689 / cpp :4777-4781  GATE (G-2)
    void RecordSimTaskMsg(AnsiString asMsg);                                    // golden h:690 / cpp :4800-4806  GATE (G-2)
    void WriteLaserScanFlowLog(AnsiString asMsg);                               // golden h:691 / cpp :4808-4821  GATE (G-2)
    AnsiString DumpLaserScanState();                                            // golden h:692 / cpp :4823-4848  ACTIVE
    void SetSimulationState(bool bswitch = false);                              // golden h:693 / cpp :4954-4965  ACTIVE
    bool GetSimulationState() { return bSimulate; }                             // golden h:694 (inline, verbatim)
    bool MoveTrayArmToSafeColor();                                              // golden h:695 / cpp :4967-4970  GATE (G-3)
    bool DoLaserScanFromLoader(int iAOIType, bool bIsFirst,
                               bool bIsRunMoveOut = false);                     // golden h:696 / cpp :4972-5296  GATE (G-3)
    enum eAOI_Type                                                              // golden h:697-701 (verbatim)
    {
        eAOIType_TrayDeviceCheckLaser = 0,
        eAOIType_TrayMapLaser,
    };
    TfAOI *GetNowAOI();                                                         // golden h:702 / cpp :5323-5337  ACTIVE
    void CloseAllSocket();                                                      // golden h:703 / cpp :5339-5343  GATE (G-1)
    int  iAutoRetryCount;                                                       // golden h:707
    bool IsRunMotor();                                                          // golden h:708 / cpp :5345-5355  ACTIVE
    bool IsCommAOI();                                                           // golden h:709 / cpp :5357-5360  ACTIVE
    bool IsRunTrayMapLaser();                                                   // golden h:710 / cpp :5362-5368  ACTIVE
    bool IsRunDeviceRemainLaser();                                              // golden h:711 / cpp :5370-5376  ACTIVE
    int  GetLoaderCWSpeed() { return iLoaderYCWSpeedPerc; }                     // golden h:712 (inline, verbatim)
    void InitialcbLaserScanMode();                                              // golden h:714 / cpp :5378-5386  ACTIVE
    void ResetcbLaserScanMode();                                                // golden h:715 / cpp :5388-5396  ACTIVE
    void InitialAOIGroup();                                                     // golden h:716 / cpp :5398-5412  GATE (G-8)
    void ShowLaserScanSheet(TTabSheet *tsShow);                                 // golden h:717 / cpp :5414-5417  ACTIVE
    void ReleaseLoaderBreak(int iAOIType, bool bswitch = true);                 // golden h:718 / cpp :5426-5455  GATE (G-3)
    bool GetTrayDeviceCheckIsRemainIC();                                        // golden h:719 / cpp :5457-5460  ACTIVE
    bool DoGetValueAfterMotorArrival(int iMode, bool bIsFirst,
                                     AnsiString &asError);                      // golden h:720 / cpp :5462-5465  GATE (G-1)
    bool GetTrayMapHasNullIC();                                                 // golden h:721 / cpp :5467-5470  ACTIVE
    bool DoSettingBeforeMotorMove(int iMode, bool bIsFirst,
                                  AnsiString &asError);                         // golden h:722 / cpp :5472-5475  GATE (G-1)
    void SetYieldDatas(AnsiString sData);                                       // golden h:723 / cpp :5569-5574  GATE (G-5b)
    void InitialTrayDeviceYield();                                              // golden h:725 / cpp :5581-5593  ACTIVE
    void SetTtileTrayDeviceYield(int icol, int irow, AnsiString sValue);        // golden h:726 / cpp :5595-5599  ACTIVE
    void SetValueTrayDeviceYield(int icol, int irow, AnsiString sValue);        // golden h:727 / cpp :5601-5605  ACTIVE
    bool CheckRunAutoRetry(int iAOIType);                                       // golden h:728 / cpp :5607-5619  ACTIVE
    bool DoMoveOut(int iAOIType, bool bIsFirst);                                // golden h:729 / cpp :5621-5624  GATE (G-3)
    bool DoMoveIn(int iAOIType, bool bIsFirst);                                 // golden h:730 / cpp :5626-5629  GATE (G-3)
    void ReadFile_ScanLine();                                                   // golden h:731 / cpp :5631-5655  GATE (G-2)
    void ReadFile_ScanLine(AnsiString szDir, int iAoi,
                           AnsiString sHead, bool bUpdateUI);                   // golden h:732 / cpp :5657-5695  GATE (G-2)
    void SaveFile_ScanLine();                                                   // golden h:733 / cpp :5697-5723  GATE (G-2)
    void SaveFile_ScanLine(AnsiString szDir, int iAoi, AnsiString sHead);       // golden h:734 / cpp :5725-5750  GATE (G-2)
    bool DoAutoSkip(int iAOIType);                                              // golden h:735 / cpp :5752-5755  ACTIVE
    bool DoRunAtInitialStart(int iAOIType);                                     // golden h:736 / cpp :5757-5767  ACTIVE
    void SetRealRunInitialStart(int iAOIType, bool bSwitch);                    // golden h:737 / cpp :5769-5772  ACTIVE
    bool GetRealRunInitialStart(int iAOIType);                                  // golden h:738 / cpp :5774-5777  ACTIVE
    void SearchPageCtrl(AnsiString sFilter);                                    // golden h:739 / cpp :5779-5790  GATE (G-5d)
    void ShowCheckDevicePage();                                                 // golden h:740 / cpp :5792-5797  GATE (G-5d,e)
    void ShowCheckDeviceResult(AnsiString sValue);                              // golden h:741 / cpp :5799-5802  ACTIVE

    bool DoTrayIDCheck(AnsiString Str);                                         // golden h:747 / cpp :7241-7266  ACTIVE
    void ClearTrayIDByLot();                                                    // golden h:748 / cpp :7104-7109  GATE (G-2)

    // -- golden event handlers reached from the .dfm (cTrayMapping.h:477-558) -
    // Wave rule 3: bodies are translated but NOT WIRED -- this tree has no
    // message loop, so nothing dispatches to them.
    void FormShow(TObject *Sender);                                             // golden h:482 / cpp :203-285    GATE (G-2)
    void FormDestroy(TObject *Sender);                                          // golden h:485 / cpp :335-405    GATE (G-2)
    void sbtExitClick(TObject *Sender);                                         // golden h:484 / cpp :1138-1148  ACTIVE
    void spbSaveClick(TObject *Sender);                                         // golden h:495 / cpp :459-510    GATE (G-2)
    void btTray_ID_TriggerClick(TObject *Sender);                               // golden h:477 / cpp :593-629    GATE (G-1)
    void TimerDownCCDTrayConnectTimer(TObject *Sender);                         // golden h:478 / cpp :782-966    GATE (G-1)
    void TimerCCDTrayInitialTimer(TObject *Sender);                             // golden h:479 / cpp :973-1094   GATE (G-1)
    void BtTray_ID_ConnectClick(TObject *Sender);                               // golden h:480 / cpp :1096-1102  GATE (G-1)
    void BtTray_ID_DisconnectClick(TObject *Sender);                            // golden h:481 / cpp :1104-1136  GATE (G-1)
    void TimerProcessTrayDataTimer(TObject *Sender);                            // golden h:496 / cpp :1264-1708  GATE (G-1)
    void Button1Click(TObject *Sender);                                         // golden h:497 / cpp :1977-1981  GATE (G-2)
    void Button2Click(TObject *Sender);                                         // golden h:498 / cpp :1983-1987  GATE (G-2)
    void PageControl1Change(TObject *Sender);                                   // golden h:500 / cpp :2814-2817  ACTIVE
    void edTrayStartDelayClick(TObject *Sender);                                // golden h:501 / cpp :3274-3277  ACTIVE
    void edTrayAutoRetryClick(TObject *Sender);                                 // golden h:502 / cpp :3279-3282  ACTIVE
    void Button4Click(TObject *Sender);                                         // golden h:503 / cpp :3284-3334  GATE (G-2)
    void edtTrayMapCatchClick(TObject *Sender);                                 // golden h:504 / cpp :3336-3339  ACTIVE
    void edtTrayMapShiftClick(TObject *Sender);                                 // golden h:505 / cpp :3341-3344  ACTIVE
    void edtTrayIDReadShiftClick(TObject *Sender);                              // golden h:506 / cpp :3406-3409  ACTIVE
    void edtTrayDeciveCntStartClick(TObject *Sender);                           // golden h:507 / cpp :3719-3722  ACTIVE
    void BtTray_ID_ConnectKYClick(TObject *Sender);                             // golden h:518 / cpp :4286-4292  GATE (G-1)
    void BtTray_ID_DisconnectKYClick(TObject *Sender);                          // golden h:519 / cpp :4294-4329  GATE (G-1)
    void btTray_ID_TriggerKYClick(TObject *Sender);                             // golden h:520 / cpp :4331-4375  GATE (G-1)
    void edTray_ID_PortMouseDown(TObject *Sender, TMouseButton Button,
                                 TShiftState Shift, int X, int Y);              // golden h:521 / cpp :4764-4768  ACTIVE
    void BtLoaderDC_ConnectClick(TObject *Sender);                              // golden h:523 / cpp :4874-4881  GATE (G-1)
    void btLoaderDC_TriggerClick(TObject *Sender);                              // golden h:524 / cpp :4898-4902  GATE (G-1)
    void btnClearBufferClick(TObject *Sender);                                  // golden h:525 / cpp :5484-5488  GATE (G-1)
    void btnLoadFileClick(TObject *Sender);                                     // golden h:526 / cpp :5478-5482  GATE (G-1)
    void btnGrabClick(TObject *Sender);                                         // golden h:527 / cpp :5490-5494  GATE (G-1)
    void btnGetValueClick(TObject *Sender);                                     // golden h:528 / cpp :5496-5500  GATE (G-1)
    void btnSetLotClick(TObject *Sender);                                       // golden h:529 / cpp :5511-5515  GATE (G-1)
    void btnCreateFileClick(TObject *Sender);                                   // golden h:530 / cpp :5517-5521  GATE (G-1)
    void btnGoldenImageClick(TObject *Sender);                                  // golden h:531 / cpp :5523-5527  GATE (G-1)
    void cbLaserScanModeChange(TObject *Sender);                                // golden h:532 / cpp :5318-5321  ACTIVE
    void btnSimulateRemainICClick(TObject *Sender);                             // golden h:533 / cpp :4904-4952  GATE (G-3)
    void edLoopTimesKeyPress(TObject *Sender, char &Key);                       // golden h:534 / cpp :5311-5316  ACTIVE
    void cbSimulationAOICommandClick(TObject *Sender);                          // golden h:535 / cpp :5502-5509  GATE (G-1)
    void btnMoveOutClick(TObject *Sender);                                      // golden h:536 / cpp :5536-5542  GATE (G-3)
    void btnMoveInClick(TObject *Sender);                                       // golden h:537 / cpp :5529-5534  GATE (G-3)
    void Label18MouseDown(TObject *Sender, TMouseButton Button,
                          TShiftState Shift, int X, int Y);                     // golden h:538 / cpp :5544-5562  ACTIVE
    void btnAddRandomClick(TObject *Sender);                                    // golden h:540 / cpp :5564-5567  GATE (G-5b)
    void btnRefreshYieldClick(TObject *Sender);                                 // golden h:541 / cpp :5576-5579  GATE (G-5b)
    void btnLdRFIDConnClick(TObject *Sender);                                   // golden h:544 / cpp :7007-7023  GATE (G-5c)
    void btnLdRFIDDisConnClick(TObject *Sender);                                // golden h:547 / cpp :7032-7044  GATE (G-5c)
    void btnLdRFIDClear1Click(TObject *Sender);                                 // golden h:548 / cpp :7046-7051  GATE (G-5c)
    void btnReadID1Click(TObject *Sender);                                      // golden h:549 / cpp :7053-7057  GATE (G-5c)
    void btnReadUID1Click(TObject *Sender);                                     // golden h:550 / cpp :7059-7063  GATE (G-5c)
    void btnReadMemData1Click(TObject *Sender);                                 // golden h:551 / cpp :7065-7068  GATE (G-5c)
    void btnLdRFIDReadBlock1Click(TObject *Sender);                             // golden h:552 / cpp :7070-7073  GATE (G-5c)
    void btnLdRFIDClear2Click(TObject *Sender);                                 // golden h:553 / cpp :7075-7080  GATE (G-5c)
    void btnReadUID2Click(TObject *Sender);                                     // golden h:554 / cpp :7082-7086  GATE (G-5c)
    void btnLdRFIDReadBlock2Click(TObject *Sender);                             // golden h:555 / cpp :7088-7091  GATE (G-5c)
    void btnReadMemData2Click(TObject *Sender);                                 // golden h:556 / cpp :7093-7096  GATE (G-5c)
    void btnReadID2Click(TObject *Sender);                                      // golden h:557 / cpp :7098-7102  GATE (G-5c)
    void tmrNFCTimer(TObject *Sender);                                          // golden h:558 / cpp :7111-7239  GATE (G-5c)

    // OMITTED ENTIRELY -- GATE (G-6).  Not declared, because the golden
    // signature names a type with zero port in this tree:
    //   ClientSocket_Tray_IDConnect / ...Disconnect / ...Error / ...Read
    //   ClientSocket_Keyence1Connect / ...Disconnect / ...Error / ...Read
    //   ClientSocket_CoverTray_ID_LoaderCarConnect   (TCustomWinSocket*)
    //   FormClose                                    (TCloseAction&)
    //   FormShortCut                                 (TWMKey&)
    //   commRFIDReceiveData                          (Pointer / WORD)

    // -- DEVIATION (D-4): golden TForm::Close()/Show()/ShowModal()/
    //    BringToFront() -- permanent offline no-ops (no window, no modal loop).
    //    Only Close() is reached from a delivered body (sbtExitClick, :1146).
    void Close()        {}
    void Show()         {}
    void ShowModal()    {}
    void BringToFront() {}
};

// golden cTrayMapping.h:751 `extern PACKAGE TfTrayMapping *fTrayMapping;` is
// DELIBERATELY NOT declared here.  Both that global AND the class name
// `TfTrayMapping` are already owned by acatchtray_shims.h (:219 / :313), which
// 81 TUs include.  See banner section 1.


// ===========================================================================
//  AI(W906-FW-TRAYMAP-W34) 20260827 -- SECOND BATCH.
//  TARGET POPULATION: golden's FILE-SCOPE FREE FUNCTIONS -- a family the
//  wave-W32 census above structurally CANNOT SEE.
//
//  ---------------------------------------------------------------------
//  1. SECTION 2's DENOMINATOR IS CONFIRMED, NOT INHERITED
//  ---------------------------------------------------------------------
//  Re-measured from scratch this wave with an independently written
//  character state machine (blanks //, /* */, and string/char literals,
//  preserving newlines; then \bTfTrayMapping\s*::\s*(~?\w+), then a
//  forward scan to the first paren-depth-0 '{' or ';', then brace matching):
//      raw regex on RAW text ......... 161   (section 2 said 161)
//      after comment/string blanking .. 150
//      of which DEFINITIONS ........... 150
//      of which DECLARATIONS ...........  0
//      signature-to-closing-brace span  6,040 lines
//  Byte-identical to section 2.  Delivered/remaining re-derived from the
//  FINISHED .cpp the same way: 42 delivered, 108 remaining, 5,685 lines.
//
//  ---------------------------------------------------------------------
//  2. BUT `TfTrayMapping::` IS A CLASS-MEMBER CENSUS, AND THE FILE IS BIGGER
//  ---------------------------------------------------------------------
//  golden cTrayMapping.cpp is 7,268 lines; the 150 member definitions span
//  6,040.  The ~1,228-line remainder is NOT whitespace.  Brace-matching
//  EVERY top-level definition in the blanked text (196 in total) this wave
//  found four families a member census cannot reach:
//
//    TfRFID::*                        17 defs   576 lines  (:6350-7005)
//    cLineScanRemainICYieldRecord::*  11 defs   110 lines  (:6229-6348)
//    cDatabaseJson::*                  7 defs    99 lines  (:6123-6227)
//    file-scope FREE FUNCTIONS        11 defs   206 lines  (:51-6475)
//
//  The first three are classes golden declares in cTrayMapping.h (:69-147,
//  :47-67, :29-45).  Section 5 (G-5b) / (G-5c) already names two of them --
//  but ONLY as zero-port BLOCKERS, never as REMAINING WORK IN THIS FILE.
//  They are both: see section 6 below for the next wave's batch.
//
//  ALSO CLOSED THIS WAVE: golden's class body cTrayMapping.h:149-749 was
//  scanned for INLINE member bodies (a second population the .cpp-only
//  census cannot see).  There are EXACTLY FOUR -- GetAOISize (h:573),
//  DeleteAOI (h:574), GetSimulationState (h:694), GetLoaderCWSpeed (h:712)
//  -- and wave W32 already delivered all four.  That avenue is exhausted;
//  nobody needs to re-check it.
//
//  ---------------------------------------------------------------------
//  3. BATCH CRITERION FOR THIS WAVE  (5 of the 11 free functions)
//  ---------------------------------------------------------------------
//  A free function is in this batch iff ALL FOUR hold:
//   (a) read-only by section 3's exit rules -- no machine motion, no
//       persistence (incl. CheckAndReadIniData / MyForceDirectories), no
//       outbound command;
//   (b) every symbol it needs already exists in this port (no new shim);
//   (c) it was read IN FULL from the cp950-decoded golden this wave; and
//   (d) -- NEW, AND IT COST THIS WAVE FOUR CANDIDATES -- the port does not
//       ALREADY OWN THAT SYMBOL.  See (W-1) directly below.
//
//  ---------------------------------------------------------------------
//  4. EXCLUDED, WITH EVIDENCE  (6 of the 11)
//  ---------------------------------------------------------------------
//  (W-1) ALREADY DEFINED IN THIS PORT -- defining them here would be a
//        DUPLICATE SYMBOL, not a translation.  This is the SAME shim
//        collision banner section 1 documents for the CLASS NAME, one level
//        down at free-function scope, and it is live:
//          InitialTrayIDTask()        golden :1991-1995 (5)
//          InitialTrayID2Task()       golden :3452-3456 (5)
//          InitialTrayMapTask(int)    golden :2379-2387 (9)
//          InitialCoverTrayIDTask(int) golden :5807-5810 (4)
//        all four are DEFINED as offline no-ops at acatchtray_shims.cpp
//        :232-235, declared extern at acatchtray.h:55-57 and :66, and CALLED
//        from 20+ live sites (acatchtray.cpp :3089/:4126/:4497/:7865/:7924/
//        :8004/:8764, asendic_Auto.cpp :2055/:2060/:2065/:2104/:2146/:2187,
//        asendic_Color.cpp :841/:1138/:1164, asendic_Loader.cpp :1104/:1664/
//        :1765/:1776/:1841, AutoRetest.cpp :603/:619, csystem.cpp :13900/
//        :13905/:13910/:13962/:13992/:14021/:31542).  Landing golden's real
//        bodies is a RETIREMENT job on acatchtray_shims.cpp (the idiom that
//        file already uses at its own :228-231 `#if 0 ... RETIRED` block) --
//        an integration wave with a behaviour delta, not a translation wave:
//        the stubs currently do NOTHING, so retiring them makes four state
//        machines actually reset.  NOT this wave's call.
//
//        ⚠ RELATED DIVERGENCE FOUND WHILE CHECKING THIS, RECORDED NOT FIXED:
//        golden defines `int iCoverTrayIDTask[iKeyenceTotalFunction]=
//        {1,1,1,1,1,1,1,1,1,1,1,1,1};` (golden :5805) -- every element ONE.
//        The port defines `int iCoverTrayIDTask[64] = {0};`
//        (acatchtray_shims.cpp:151, extern at acatchtray_shims.h:429) --
//        every element ZERO, and a different length.  Task cursor 1 vs 0 is
//        a real initial-state difference for whatever reads it.  Left alone
//        here because that array is the shim's property, not this facade's.
//
//  (W-2) OUTBOUND COMMAND -- section 5 (G-1) by the same test as the rest:
//          DoCoverTrayID_NFC(int,bool)  golden :6044-6104 (61)
//        calls EventReport(SECS_EVENT.BundleEnd_IDREAD_Auto1..Auto6) at
//        golden :6062/:6069/:6076/:6083/:6090/:6097 -- six SECS event
//        reports to the host.
//
//  (W-3) WOULD EMIT A NEW WARNING FOR CODE NOTHING CAN REACH:
//          ShouldParseAutoBinLabelList()  golden :51-56 (6)
//        is `static` (internal linkage) and its ONLY three call sites are
//        golden :4123, :4187, :4235 -- all inside ClientSocket_Keyence1Read
//        (:4069-4284), which section 5 (G-6) OMITS ENTIRELY because its
//        signature names TCustomWinSocket*.  Translating it would therefore
//        add a `static` function with zero callers in this TU, i.e. a NEW
//        -Wunused-function under this tree's -Wall -- for a body that cannot
//        be called until (G-6) is solved.  It becomes free the moment
//        ClientSocket_Keyence1Read lands; take it in that wave.
//        (Its three IniConfig fields DO exist here -- Config.h:355 / :359 /
//        :1464 -- so (b) is satisfied; only (c)-adjacent cleanliness is not.)
//
//  ---------------------------------------------------------------------
//  5. THE 5 DELIVERED -- LINKAGE, AND WHY THEY ARE DECLARED HERE
//  ---------------------------------------------------------------------
//  (D-9) DECLARATIONS ADDED THAT GOLDEN DOES NOT HAVE.  All five are
//        defined at file scope in golden cTrayMapping.cpp and declared
//        NOWHERE -- verified this wave by an os.walk over the WHOLE golden
//        BCB6 tree D:\HT9045\HT9011UC_Code_V3.33.906.0_20260618 (every
//        .h/.hpp/.cpp, cp950-decoded): the ONLY hits for
//        iWhichTrayNeedDeviceCount / CheckNeedDeviceCount /
//        InitialMultileTrayIDKeyence3Task are their own definitions, and the
//        only hits for Crc_16_create / BlockToString are their definition
//        plus call sites inside cTrayMapping.cpp itself.  golden needs no
//        declaration because every caller is in the same translation unit.
//        This port will NOT be able to keep them in one TU (TfRFID is a
//        separate future wave), so they are declared here -- deliberately,
//        so that wave REUSES them instead of defining a second copy and
//        rediscovering (W-1) the hard way.
//  (D-10) `iMultileTrayIDKeyence3Task` (golden :4584) is given INTERNAL
//        linkage in an anonymous namespace in the .cpp, exactly as wave W32
//        did for `iCCDConntectionOkTask` (golden :46) and for the same
//        reason.  Its only golden reader is DoMultileTrayIDKeyence3
//        (:4591-4671, `int &Task=iMultileTrayIDKeyence3Task;` at :4593),
//        which is GATE (G-1).  Consequence, stated plainly: if a future wave
//        lands DoMultileTrayIDKeyence3 in a DIFFERENT TU it will not link
//        against this cursor and must promote it to external linkage first.
//
//  ZERO new #include is required for any of the five -- bNeedCCDTrayDeviceCount
//  (cmydef.h:4188 / defined cmydef.cpp:4525), USE_TRAY_MAPPING (cmydef.cpp
//  :3224), etmUninstall (MachineType.h:910), TestIF_File.bEnableTrayDeviceCnt
//  (cprod.h:2283) and BYTE (<windows.h>, hoisted by vclcompat/vcl_compat.h)
//  all arrive through headers forms/fTrayMapping.cpp already includes.
//
//  ---------------------------------------------------------------------
//  6. WHAT THE NEXT WAVE SHOULD TAKE  (measured, not guessed)
//  ---------------------------------------------------------------------
//  The 108 remaining `TfTrayMapping::` methods are a GATED CORE: all 108
//  were read in full this wave and every one trips section 3's exit rules
//  (motion / persistence / outbound / alarm / zero-port type / gated
//  callee).  There is no read-only remainder left in the class -- do not
//  spend another wave looking for one.  The genuine next batch is the two
//  zero-port HELPER CLASSES this file already blocks on:
//      cDatabaseJson                  golden h:29-45  / cpp :6123-6227
//                                     7 defs, 99 lines.  cJSON EXISTS in
//                                     this port (Public/cJSON.h, Public/
//                                     cJSON.c, already in CMakeLists).
//                                     ⚠ but golden declares it
//                                     `: public uBasicPickPlace` (h:29) --
//                                     resolve that base first.
//                                     SaveFile (:6208-6212) writes a file =
//                                     (G-2); the AddColumnTitle / AddRowData
//                                     / UpdateRowData trio is in-memory.
//      cLineScanRemainICYieldRecord   golden h:47-67  / cpp :6229-6348
//                                     11 defs, 110 lines (+3 inline bodies
//                                     in the header at h:50/:51/:54).
//                                     Unblocks (G-5b) -- i.e. member
//                                     yieldRemainIC, btnAddRandomClick,
//                                     SetYieldDatas, btnRefreshYieldClick.
//                                     ⚠ AddYieldData (:6305-6319) and
//                                     RefreshTotalNum (:6321-6348) are
//                                     WRITERS; GetTotalNumDir (:6276-6286)
//                                     and GetFileNameWithDir (:6261-6274)
//                                     call MyForceDirectories, which CREATES
//                                     DIRECTORIES and is therefore (G-2) too
//                                     -- it is not a pure path builder.
//
//  ---------------------------------------------------------------------
//  7. ONE CORRECTION TO SECTION 5's EVIDENCE (the gate itself STANDS)
//  ---------------------------------------------------------------------
//  Section 5 files `cbSimulationAOICommandClick :5502-5509` under
//  (G-1) OUTBOUND TCP with the note "flips AOI sim mode".  The GATE IS
//  RIGHT; THE REASON AS FILED IS NOT.  Read in full this wave, the body is
//      bSimulationAOICommand=cbSimulationAOICommand->Checked;
//      for(int i=0;i<GetAOISize();i++)
//          GetAOI(i)->bSimulationAOICommand=bSimulationAOICommand;
//  -- three in-memory assignments.  It opens no socket and sends no byte, so
//  it is NOT (G-1), and a regex hunting outbound verbs will keep reporting
//  it "clean" (this wave's own signal cross-check did exactly that).
//  It is a MODE SWITCH, and that is a stronger reason to keep it gated, not
//  a weaker one: TfAOI::bSimulationAOICommand (TfAOILaserScan.h:217) is the
//  flag 15 sites in TfAOILaserScan.cpp (:431, :634, :939, :976, :1012,
//  :1048, :1094, :1130, :1181, :1222, :1258, :1294, :1376, :1412, :1448)
//  test to decide whether to FAKE an AOI reply or issue the real command.
//  Wiring an unchecked offline checkbox to it drives every AOI into REAL
//  command mode -- the dangerous direction.  Both members golden needs
//  (bSimulationAOICommand h:684, cbSimulationAOICommand h:380) are
//  declarable here and TfAOI::bSimulationAOICommand exists, so this one is
//  blocked by POLICY, not by a missing symbol.  Re-file it as a MODE SWITCH
//  and give it a write-path interlock story before un-gating.
//
//  Same correction shape, recorded so the next reader does not "fix" them:
//  DoMoveIn / DoMoveOut (:5626-5629 / :5621-5624) also read clean to a
//  signal scan, and are correctly gated (G-3) -- the motor is inside the
//  callee TfAOI::DoMoveIn/DoMoveOut, not in the two-line body.
// ===========================================================================
int          iWhichTrayNeedDeviceCount();                                       // golden cpp :2789-2800 (12)  ACTIVE
bool         CheckNeedDeviceCount(int iAuto);                                   // golden cpp :2802-2812 (11)  ACTIVE
void         InitialMultileTrayIDKeyence3Task();                                // golden cpp :4586-4589 (4)   ACTIVE
unsigned int Crc_16_create(unsigned char *string, unsigned char length);        // golden cpp :6443-6464 (22)  ACTIVE
AnsiString   BlockToString(const BYTE* data, int length);                       // golden cpp :6466-6475 (10)  ACTIVE

#endif // FORMS_FTRAYMAPPING_H
