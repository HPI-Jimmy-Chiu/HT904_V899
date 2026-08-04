# RECON_GateA_FormRegistry — golden CreateForm 順序與副作用盤點（GA-0 決策 2 材料）

> AI(W906-GateA-0) 20260804: GA-0 派出的唯讀 recon agent（Sonnet 5 high）產出，供 GA-3 的
> `ui/FormRegistry.{h,cpp}` 設計直接取用。
>
> **主迴圈驗證**（承重宣稱親自重導）：
> - ✅ golden `HT9045.cpp:127-301` 內 `Application->CreateForm(__classid(...))` 實為 **118 次**
>   （非「120」——:284/:285 是含 "CreateForm" 字樣的 log 呼叫）；唯一指標 **115 個**；
>   重複建構 3 組：`fBinAOISel`(:278,:279)、`fObserveMagazine`(:280,:282)、`frmFileTransfer`(:281,:283)
>   ——golden 自己的 copy-paste bug。主迴圈用獨立 regex 重數，逐項吻合。
> - 其餘逐列（118 列表格）未逐一複核；GA-3 動筆前對將真建構的列重核。

## 核心數字

- 118 呼叫 / 115 唯一指標 / 3 組重複建構（golden bug，FormRegistry 需決策重演或坍縮——建議坍縮，除非把「golden 崩潰對等」列為目標）。
- ported 樹狀態（115 唯一）：**FACADE 18**（forms/ 真物件）／**SHIM 16**（多為異名 shim 類）／
  **SHIM-NULL 2**／**ABSENT-TU-stub 9**／**ABSENT 70**。
- 副作用（heuristic：ctor/FormCreate >30 行或含 CheckAndReadIniData/WriteIniData/MyDBOpen/AddSV/AddEC/LoadFromFile）：**SIDE-EFFECT 48／TRIVIAL 67**。

## GA-3 必須處理的四個結構性事實

1. **18 個 facade 是 static-init 期建構**（`= new T...()` at namespace scope），時點由連結順序決定，
   **不是** golden CreateForm 順序。FormRegistry 要忠實 golden 順序就必須把 eager static 轉 lazy
   （registry 依 golden 序建構），否則「表單 B ctor 讀表單 A 已初始化成員」的跨表單依賴不可重現。
   （替代案：Gate A 接受 static-init 順序 + 文件揭露——GA-3 brief 時裁決。）
2. **SHIM-NULL 兩個是現成地雷**：`HGem`（`uHGemEquipment.cpp:3521` `= NULL`，靠註解約定
   「caller 先設 instance」）與 `DataModule1`（`database.cpp:103`，ctor 在 `#if 0` wave-BDE）。
   FormRegistry 要嘛在正確槽位（#70/#83）給真 placeholder，要嘛掛 deref 斷言。
3. **三重複建構**見上。4. **調和債**：`fContactCT`/`fCounterClear`/`fShowBinSelect` 各有
   **兩份未調和的 TU-local stand-in**（`csystem.cpp` 的 `W7C1_*` `#define` 與
   `Automation/auto9045.cpp` 的 `W5FA_*`）——與 `fSCKART` 已記錄的 5 處同型，GA-3 落 FormRegistry
   時是天然的整併點。

## 分類清單

**PORTED-FACADE (18)**：fMain, fLotInfo, fNote, fHome, fSetup, fTrayForm, fOffSet, fSortCT,
fShowMessage, FrmRotate, FrmAOI, fOCR, fSCKART, fProductionInfo, fAGV, fCleaning, fFixAICCD, fShuttleMove。

**PORTED-SHIM (16)**（括號=實際 underlying 類/位置）：MyMessageBox(TMyMessageBoxShim,
acatchtray_shims:98)、COM2(TCOM2Shim, atester_shims:376)、fiosetview(atester_shims:346)、
fContact(TfContactShim, atester_shims:278)、fObserver(atester_shims:342)、
ATC_InterfaceForm(acarry_shims:77)、CCDInterfaceForm(atester_shims:305)、
fAutomation(atester_shims:319)、LoadCCD(acatchtray_shims:103)、fTrayMapping(acatchtray_shims:93)、
fConfiguration(W5SckArtRem_ConfigStub, SCK_ART_Remainder:331)、FormHS(同檔:362)、
fLtcSensor(acarry_shims:66)、fBarCode(TfBarCode_Shim→static, aHotPlateSubstrate:977)、
Zteach(aHotPlateSubstrate:1463)、fYieldMonitoring(ainarm9045_2x4_16_shims:85)。

**SHIM-NULL (2)**：HGem、DataModule1（見上）。

**ABSENT-TU-stub (9)**：ASESendMessage/fContactCT/fCounterClear/fShowBinSelect（csystem.cpp
`W7C1_*`）；fMesSystem（asendic_Auto.cpp）；fBinSel/fTrayAssignment/fTestCategory/fMonitor
（auto9045.cpp `W5FA_*`；其中 fContactCT/fCounterClear/fShowBinSelect 雙份見調和債）。

**ABSENT (70)**：dmTrayMotor, ATCInterfaceForm, fOmron, fLan, FTool, fTeach, fInput, fLogin,
fMotorTest, TrayEditForm, fTemperFrom, fTemp_Set, FTestIF, fLd_ULd, fHotPlate, fSpeed, fDIOFrom,
fTeachShow, fCounterSel, fTowerLight, fStartCondition, fBuilder, fSecurity, HandlerSystem,
FormBarcodeReader, DataModule3, fCCLink, fFTPClient, fContactForce, ACTForm, fDynamicTemp,
fQwertyKey, fPassword, fSocketCommunication, fQAMode, CleanPadFrom, fAirCon, fLaserSensor,
FormSysTools, FSECS, fInputForm, fAdam6024, fShowBinSet, fPMAlarmInterFace, fPMAlarm_ShowData,
fWebService, fRecipeDefault, fRPDefault, fARMS, fARMSLog, ESDForm, fShowPmSOPForm, DataCompile,
fPrecaution, fGroundMan, frmXml, fPadInterface, fAutoTeach, fTesterTCP, fAutoAlignment,
fSmartDiagnostic, fWinway, frmDTME08, fMagazine, fVacuumUnit, fDefrostNote, fServerFrm,
fBinAOISel, fObserveMagazine, frmFileTransfer。

## SIDE-EFFECT ctor 的高優先 placeholder 候選（golden ctor 行數為代理指標）

fTeach(742L)、fCCLink(452L)、fTemp_Set(314L)、fMotorTest(246L)、fContactForce(232L,含
CheckAndReadIniData)、fSecurity(217L,THGem S125F4 依賴)、fObserver(209L)、fLaserSensor(166L)、
fBinSel(148L)、fLotInfo(143L)、fOCR(138L)、fTrayMapping(137L)、ATC_InterfaceForm(132L)、
fOmron(121L)、fShowBinSelect(118L)、fConfiguration(117L)、FSECS(113L)、fGroundMan(114L)、
ACTForm(112L)、fTrayForm(112L)、fSetup(94L)、fAutomation(91L)、fNote(88L)、fTemperFrom(87L)、
fTesterTCP(86L)、fSCKART(85L,LoadFromFile)、fAutoAlignment(79L)、fSortCT(79L)、fQwertyKey(72L)、
fLd_ULd(64L)、fShowBinSet(62L)、fStartCondition(65L)、fMagazine(37L)、FrmAOI(37L)、
fMesSystem(38L)、FrmRotate(38L+FC33L)、fTrayAssignment(36L)、fLtcSensor(35L)、fFixAICCD(34L)、
COM2(31L,CheckAndReadIniData)、fSocketCommunication(31L)、fiosetview(43L)、
fYieldMonitoring(48L+FC156L)、**HGem(231L+FC43L,SetSV/SetECDataPointer)**、
**fMain(926L,全清單最重)**、fOmron(121L)、fCleaning(10L→TRIVIAL)。
（其餘 TRIVIAL ≤30 行且零狀態關鍵字者共 67 個，可安全延後。）

**現況無 live crash 風險**：agent 抽驗的 deref 全在 `#if 0` 或 TU-local stand-in 內——但 W7-F1
之後每解鎖一個 override（THGem AddSV/AddEC 家族等）就會出現新 deref，FormRegistry 槽位要先就位。

## 118 列完整順序表

（保留 agent 原始產出的排序：# / golden 行 / 類→指標 / 來源 ctor / ported 狀態 / 副作用。
GA-3 使用時對「將真建構」的列逐列重核。）

|#|行|類→指標|來源(ctor行)|ported|副作用|
|---|---|---|---|---|---|
|1|166|TfMain→fMain|main.cpp:1340|FACADE|SIDE-EFFECT(926L)|
|2|167|TdmTrayMotor→dmTrayMotor|Motor/TrayStepMotor.cpp:36|ABSENT|TRIVIAL|
|3|168|TATCInterfaceForm→ATCInterfaceForm|ATC/ATCInterface.cpp:40|ABSENT|TRIVIAL|
|4|169|TfOmron→fOmron|EJ1N/OmronEJ1N.cpp:64|ABSENT|SIDE-EFFECT(121L)|
|5|170|TfLotInfo→fLotInfo|uLotInfo.cpp:157|FACADE|SIDE-EFFECT(143L)|
|6|171|TfLan→fLan|language.cpp:15|ABSENT|TRIVIAL|
|7|172|TMyMessageBox→MyMessageBox|mymessbox.cpp:56|SHIM|TRIVIAL|
|8|173|TfNote→fNote|note.cpp:161|FACADE|SIDE-EFFECT(88L+FC12L ini)|
|9|174|TCOM2→COM2|rs232.cpp:95|SHIM|SIDE-EFFECT(31L ini)|
|10|175|TfHome→fHome|uhome.cpp:106|FACADE|TRIVIAL|
|11|176|TFTool→FTool|tools.cpp:20|ABSENT|TRIVIAL|
|12|177|Tfiosetview→fiosetview|iosetview.cpp:49|SHIM|SIDE-EFFECT(43L)|
|13|178|TfTeach→fTeach|uteach.cpp:254|ABSENT|SIDE-EFFECT(742L LoadFromFile)|
|14|179|TfInput→fInput|INPUT.cpp:19|ABSENT|TRIVIAL|
|15|180|TfLogin→fLogin|login.cpp:12|ABSENT|TRIVIAL|
|16|181|TfMotorTest→fMotorTest|uMotorTest.cpp:145|ABSENT|SIDE-EFFECT(246L)|
|17|182|TTrayEditForm→TrayEditForm|uTrayEditForm.cpp:61|ABSENT|TRIVIAL|
|18|183|TfSetup→fSetup|cSetUp.cpp:123|FACADE|SIDE-EFFECT(94L)|
|19|184|TfContact→fContact|cContact.cpp:105|SHIM|SIDE-EFFECT(173L ini)|
|20|185|TfTemperFrom→fTemperFrom|cTemperFrom.cpp:40|ABSENT|SIDE-EFFECT(87L)|
|21|186|TfTemp_Set→fTemp_Set|uTemp_Set.cpp:95|ABSENT|SIDE-EFFECT(314L)|
|22|187|TfTrayAssignment→fTrayAssignment|cTrayAssignment.cpp:26|TU-stub|SIDE-EFFECT(36L)|
|23|188|TFTestIF→FTestIF|cTesterIF.cpp:39|ABSENT|TRIVIAL|
|24|189|TfBinSel→fBinSel|cBinSel.cpp:970|TU-stub|SIDE-EFFECT(148L)|
|25|190|TfLd_ULd→fLd_ULd|cLd_ULd.cpp:22|ABSENT|SIDE-EFFECT(64L)|
|26|191|TfTrayForm→fTrayForm|cTrayForm.cpp:28|FACADE|SIDE-EFFECT(112L)|
|27|192|TfHotPlate→fHotPlate|cHotPlate.cpp:28|ABSENT|TRIVIAL|
|28|193|TfSpeed→fSpeed|cSpeed.cpp:31|ABSENT|TRIVIAL|
|29|194|TfOffSet→fOffSet|cOffSet.cpp:186|FACADE|SIDE-EFFECT(255L)|
|30|195|TfDIOFrom→fDIOFrom|DIOInterFaceCFG.cpp:19|ABSENT|TRIVIAL|
|31|196|TfSortCT→fSortCT|cSortCT.cpp:89|FACADE|SIDE-EFFECT(79L)|
|32|197|TfTestCategory→fTestCategory|cTestCategory.cpp:17|TU-stub|TRIVIAL|
|33|198|TfContactCT→fContactCT|cContactCT.cpp:25|TU-stub×2|TRIVIAL|
|34|199|TfShowBinSelect→fShowBinSelect|cShowBinSelect.cpp:44|TU-stub×2|SIDE-EFFECT(118L)|
|35|200|TfObserver→fObserver|cObserver.cpp:137|SHIM|SIDE-EFFECT(209L ini)|
|36|201|TfTeachShow→fTeachShow|uTechShow.cpp:12|ABSENT|TRIVIAL|
|37|202|TfCounterSel→fCounterSel|cCounterSel.cpp:23|ABSENT|TRIVIAL|
|38|203|TfTowerLight→fTowerLight|cTowerLight.cpp:19|ABSENT|TRIVIAL|
|39|204|TfStartCondition→fStartCondition|cStartCondition.cpp:40|ABSENT|SIDE-EFFECT(65L)|
|40|205|TfBuilder→fBuilder|cBuilder.cpp:17|ABSENT|TRIVIAL|
|41|206|TfConfiguration→fConfiguration|cConfiguration.cpp:109|SHIM|SIDE-EFFECT(117L)|
|42|207|TfSecurity→fSecurity|cSecurity.cpp:23|ABSENT|SIDE-EFFECT(217L)|
|43|208|TfCounterClear→fCounterClear|cCounterClear.cpp:24|TU-stub×2|TRIVIAL|
|44|209|THandlerSystem→HandlerSystem|HandlerSys.cpp:18|ABSENT|TRIVIAL|
|45|210|TFormBarcodeReader→FormBarcodeReader|BarcodeReader.cpp:24|ABSENT|TRIVIAL|
|46|211|TfShowMessage→fShowMessage|uShowMessage.cpp:30|FACADE|TRIVIAL|
|47|212|TDataModule3→DataModule3|BinDisplay/MyBinDisp.cpp:32|ABSENT|TRIVIAL|
|48|213|TfCCLink→fCCLink|CCLink/MyCCLinkSensor.cpp:38|ABSENT|SIDE-EFFECT(452L)|
|49|214|TfYieldMonitoring→fYieldMonitoring|uYieldMonitoring.cpp:287|SHIM|SIDE-EFFECT(48L+FC156L)|
|50|215|TCCDInterfaceForm→CCDInterfaceForm|CCDInterface.cpp:30|SHIM|TRIVIAL|
|51|216|TfAutomation→fAutomation|Automation/automation.cpp:57|SHIM|SIDE-EFFECT(91L)|
|52|217|TfFTPClient→fFTPClient|KYECFTP/FTPClient.cpp:50|ABSENT|TRIVIAL|
|53|218|TfLtcSensor→fLtcSensor|LtcSensor.cpp:20|SHIM|SIDE-EFFECT(35L)|
|54|219|TfContactForce→fContactForce|ContactForce.cpp:421|ABSENT|SIDE-EFFECT(232L ini)|
|55|220|TACTForm→ACTForm|AutoTemperature.cpp:289|ABSENT|SIDE-EFFECT(112L)|
|56|221|TfDynamicTemp→fDynamicTemp|DynamicTemp.cpp:20|ABSENT|TRIVIAL|
|57|222|TfQwertyKey→fQwertyKey|myQwertyKeyBoard.cpp:64|ABSENT|SIDE-EFFECT(72L)|
|58|223|TfPassword→fPassword|Password.cpp:20|ABSENT|TRIVIAL|
|59|224|TfSocketCommunication→fSocketCommunication|ASE_K Socket/Socket_ASE_KR.cpp:45|ABSENT|SIDE-EFFECT(31L)|
|60|225|TfCleaning→fCleaning|AutoClean/uCleaning.cpp:44|FACADE|TRIVIAL|
|61|226|TFrmRotate→FrmRotate|RotateKit/fRotate.cpp:42|FACADE|SIDE-EFFECT(38L+FC33L)|
|62|227|TfQAMode→fQAMode|QAMode.cpp:22|ABSENT|TRIVIAL|
|63|228|TCleanPadFrom→CleanPadFrom|AutoClean/uShowcleanPad_ASEKH.cpp:13|ABSENT|TRIVIAL|
|64|229|TfAirCon→fAirCon|cAirCon.cpp:17|ABSENT|TRIVIAL|
|65|230|TZteach→Zteach|AutoTeach/InOutArmZteach.cpp:47|SHIM|TRIVIAL|
|66|231|TfLaserSensor→fLaserSensor|OmronLaser/LaserSensor.cpp:32|ABSENT|SIDE-EFFECT(166L)|
|67|232|TFormSysTools→FormSysTools|systools.cpp:12|ABSENT|TRIVIAL|
|68|233|TFrmAOI→FrmAOI|fAOI.cpp:128|FACADE|SIDE-EFFECT(37L)|
|69|234|TFSECS→FSECS|SECSGEM/UsecegemMainFrom.cpp:33|ABSENT|SIDE-EFFECT(113L)|
|70|235|THGem→HGem|SECSGEM/uHGemEquipment.cpp:444|**SHIM-NULL**|SIDE-EFFECT(231L+FC43L SV/EC)|
|71|236|TfInputForm→fInputForm|InputForm.cpp:19|ABSENT|TRIVIAL|
|72|237|TASESendMessage→ASESendMessage|ASE_K Socket/aseTest.cpp:34|TU-stub|TRIVIAL|
|73|238|TfAdam6024→fAdam6024|adam6024.cpp:122|ABSENT|TRIVIAL|
|74|239|TfOCR→fOCR|OCR.cpp:72|FACADE|SIDE-EFFECT(138L)|
|75|240|TfShowBinSet→fShowBinSet|cShowBinSet.cpp:40|ABSENT|SIDE-EFFECT(62L)|
|76|241|TATC_InterfaceForm→ATC_InterfaceForm|ATC/ATC_Handler_Side.cpp:338|SHIM|SIDE-EFFECT(132L)|
|77|242|TfBarCode→fBarCode|BarCode/BarCode.cpp:131|SHIM|SIDE-EFFECT(192L)|
|78|243|TfPMAlarmInterFace→fPMAlarmInterFace|PMAlarm/PMAlarmInterFace.cpp:18|ABSENT|TRIVIAL|
|79|244|TfPMAlarm_ShowData→fPMAlarm_ShowData|PMAlarm/PMAlarmShowData.cpp:14|ABSENT|TRIVIAL|
|80|245|TfShuttleMove→fShuttleMove|ShuttleMove.cpp:59|FACADE|TRIVIAL|
|81|246|TfMonitor→fMonitor|Monitor/MonitorInterface.cpp:23|TU-stub|TRIVIAL|
|82|247|TFormHS→FormHS|HS_Function.cpp:52|SHIM|TRIVIAL|
|83|248|TDataModule1→DataModule1|database.cpp:33|**SHIM-NULL**|TRIVIAL|
|84|249|TfWebService→fWebService|Automation/SCK_WebService.cpp:11|ABSENT|TRIVIAL|
|85|250|TfSCKART→fSCKART|Automation/SCK_ART.cpp:36|FACADE|SIDE-EFFECT(85L LoadFromFile)|
|86|251|TfTrayMapping→fTrayMapping|cTrayMapping.cpp:59|SHIM|SIDE-EFFECT(137L)|
|87|252|TfRecipeDefault→fRecipeDefault|uRecipeDefault.cpp:12|ABSENT|TRIVIAL|
|88|253|TfRPDefault→fRPDefault|RPDefault.cpp:20|ABSENT|TRIVIAL|
|89|254|TfARMS→fARMS|ARMS/ARMS.cpp:26|ABSENT|TRIVIAL|
|90|255|TfARMSLog→fARMSLog|ARMS/ARMSLog.cpp:13|ABSENT|TRIVIAL|
|91|256|TfProductionInfo→fProductionInfo|ProductionInfo/ProductionInfo.cpp:52|FACADE|TRIVIAL(30L)|
|92|257|TESDForm→ESDForm|ProductionInfo/uESDControl.cpp:27|ABSENT|TRIVIAL|
|93|258|TfShowPmSOPForm→fShowPmSOPForm|fShowPmSOP.cpp:12|ABSENT|TRIVIAL|
|94|259|TDataCompile→DataCompile|ASE_K Socket/DataCompileForm.cpp:17|ABSENT|TRIVIAL|
|95|260|TfPrecaution→fPrecaution|Precaution.cpp:14|ABSENT|TRIVIAL|
|96|261|TfGroundMan→fGroundMan|GroundMan/GroundMan.cpp:24|ABSENT|SIDE-EFFECT(114L)|
|97|262|TfrmXml→frmXml|BarCode/BarcodeXML.cpp:23|ABSENT|TRIVIAL|
|98|263|TfPadInterface→fPadInterface|uPadInterface.cpp:152|ABSENT|TRIVIAL|
|99|264|TfAutoTeach→fAutoTeach|AutoTeach/AutoTeach.cpp:38|ABSENT|TRIVIAL|
|100|265|TfFixAICCD→fFixAICCD|FixAICCD.cpp:42|FACADE|SIDE-EFFECT(34L)|
|101|266|TfTesterTCP→fTesterTCP|Interface/TesterTCP.cpp:24|ABSENT|SIDE-EFFECT(86L)|
|102|267|TLoadCCD→LoadCCD|LoadCCD/LoadCCDMap.cpp:21|SHIM|TRIVIAL|
|103|268|TfMesSystem→fMesSystem|Mes/fVATMesFileSys.cpp:127|TU-stub|SIDE-EFFECT(38L)|
|104|269|TfAutoAlignment→fAutoAlignment|AutoAlignment/AutoAlignment.cpp:48|ABSENT|SIDE-EFFECT(79L ini)|
|105|270|TfSmartDiagnostic→fSmartDiagnostic|SmartDiagnostic.cpp:14|ABSENT|TRIVIAL|
|106|271|TfWinway→fWinway|ATC/WinWaySetting.cpp:12|ABSENT|TRIVIAL|
|107|272|TfrmDTME08→frmDTME08|EJ1N/fDTME08.cpp:16|ABSENT|TRIVIAL|
|108|273|TfMagazine→fMagazine|Magazine.cpp:43|ABSENT|SIDE-EFFECT(37L)|
|109|274|TfVacuumUnit→fVacuumUnit|VacuumUnit/VacuumUnit.cpp:26|ABSENT|TRIVIAL|
|110|275|TfDefrostNote→fDefrostNote|cDefrostNote.cpp:13|ABSENT|TRIVIAL|
|111|276|TfServerFrm→fServerFrm|Automation/fRENESAS_ServerFrm.cpp:16|ABSENT|TRIVIAL(FC Listen)|
|112|277|TfAGV→fAGV|Automation/AGV.cpp:30|FACADE|TRIVIAL|
|113|278|TfBinAOISel→fBinAOISel|cBinAoiSel.cpp:19|ABSENT|TRIVIAL|
|114|279|**[DUP #113]**|〃|〃|〃|
|115|280|TfObserveMagazine→fObserveMagazine|ObserveMagazine.cpp:13|ABSENT|TRIVIAL|
|116|281|TfrmFileTransfer→frmFileTransfer|ATC/FileTransfer.cpp:13|ABSENT|TRIVIAL|
|117|282|**[DUP #115]**|〃|〃|〃|
|118|283|**[DUP #116]**|〃|〃|〃|
