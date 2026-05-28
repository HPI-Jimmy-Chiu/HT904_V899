#ifndef CosFunctionH
#define CosFunctionH
//-------------------------------------------------------------------------
// HT9045_COUSTOMER_FUNCTION
// 只For客戶開啟的功能選項
//-------------------------------------------------------------------------
typedef struct
{
    bool bOneCycleCanChangeArm;                                                 //ChungHung 20111108 Amb/Hot Mode OneCycle Can Change Arm
    bool bOffLineBin;                                                           //Hung 20111110 新增Offline Bin
    bool bFullTestBeforeContactHeight;                                          //ChungHung 20130122 add for SCK Contact Height前做FullView
    bool bEnable6Site;                                                          //ChungHung 20140115 add for 2x3_6
    bool bEnableOctal_12Kit;                                                    //ChungHung 20140508 add 使用12 layout kit Octal Shutle kit
    bool bEnable12SiteUse16SLK;                                                 //Steven 20240807 : 12Site使用16Site Kit
    bool bShowLastShuttleHigh;                                                  //ChungHung 20140516 add Show 上次Shuttle Auto Height 的值
    bool bChamberModeEvenBlowNeedWaitTime;                                      //ChungHung 20140519 add Chamber Mode Even Blow need Wait Initial Wait time in Temp_Set
    bool bLoseDeviceOutShuttleServoOff;                                         //ChungHung 20140522 add OutShuttle lose devices can servo off
    bool bEnableAfterTestedDelay;                                               //ChungHung 20140730 add for ATK function after tested delay time
    bool bContinueAutoSkipAutoTrayEnd;                                          //ChungHung 20141014 add for KYEC AutoRetest
    bool bIndexJamInArmMoveSafePostionByAutoClaen;                              //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion
    bool bBuilderImportSingleFolder;                                            //ChungHung 20150414 add for TSMC
//    bool bSendJamCodeByTcpIp;                                                 //ChungHung 20150518 add for SCK Send JamCode By TcpIp    //Steven 20161201 : Mark of SCK no use TCP IP
    bool bOneCycleCanChangeContinuesFailBin;                                    //ChungHung 20150519 add Continues Failure BIN can select after "OneCycle".
    bool bAutoSetContFailAfterInitialStart;                                     //ChungHung 20150519 add Auto Set ContFail After InitialStart.
    bool bManualProcess;                                                        //ChungHung 20150526 add for QualComm US
    bool bTopPogoPinMode;                                                       //ChungHung 20150528 add for 海思 _8Site1x4
    bool bEnableSoftWareControlButton;                                          //ChungHung 20150609 add only for TSMC
    bool bContactHaveOffset;                                                    //ChungHung 20140327 add by Customer 強制Contact Offset為0

    bool bUseLoadCellOffsetByHeater;                                            //2014-06-26    Dell    for TSMC 高溫Load cell offset

    bool bIndexZDownToAboveSocket;                                              //Eliot 2015-02-25 Initial Check時手臂只下降到Above Socket Offset位置,避免壓壞Socket pogoPin

    bool bTTLCanUse8Site;                                                       //Alick 20161011 (Steven) : TTL支援8Site
//    bool bAutoCleanUse2RowBy46LS;                                             //Alick 20170223 (wei) add 9046LS可開啟雙排吸嘴做AutoClean

    bool bJAM0301NeedOpenChamberDoor;                                           //Steven : JAM0301 & JAM0302需要開啟Chamber門10秒
    bool bJAM0303NeedOpenChamberDoor;                                           //Steven : JAM0303 & JAM0403需要開啟Chamber門10秒
    bool bEnableAutoSpeed;                                                      //Steven 20110526 : 開啟Auto Speed功能
    bool bStopMustTestTimeOut;                                                  //Steven 20110920 : 暫停時也要計算Tester Time Out
    bool bLastSetInSetUpFile;                                                   //Steven 20111019 : 把LastSet選項存在Set Up File    //Steven 20161211 : bLastSetInSetUpFile換成CosFunction
//    bool bEnableTimeOutCanSkip;                                               //Steven 20111220 : 測試TimeOut可以Skip
    bool bAutoKTemp;                                                            //Steven 20120719 : 自動K溫
    bool bEnable12Site;                                                         //Steven 20120813 : 支援12Site模式
    bool bEnable2x1Site;                                                        //Steven 20120814 : 支援2x1Site模式
    bool bMES0101CanCleanOut;                                                   //Steven 20120830 : Loader吸取異常可以按CleanOut
    bool bOnlyUseContactModeDiffSpeed;                                          //Steven 20131101 : 強制使用兩段速Contact
    bool bFixedDropSpeed;                                                       //Steven 20131101 : 使用固定的Drop速度
    int  iFixedDropSpeed;                                                       //Steven 20131101 : 使用固定的Drop速度
    bool bLimitMaxSpeed;                                                        //Steven 20131101 : 限制最高速度
    int  iLimitMaxSpeed;                                                        //Steven 20131101 : 限制最高速度
    bool bPiggyBackForASE;                                                      //Steven 20131101 : 高雄ASE不要Continual Pass Bin(Total )跟 Continual Loader兩種
    bool bPiggyBackShowMainForm;                                                //Steven 20131101 : PiggyBack數量到達時,顯示在Main Form上面
    bool bAutoCleanOnHotPlate;                                                  //Steven 20131101 : 將Clean Pad放在HotPlate上,也就是高雄版
    double dLimitMinDropOffset;                                                 //Steven 20131101 : 限制最小距離
    bool bEnableOctal_16Kit;                                                    //Steven 20140312 : 8Site使用16Site Kit
    bool bContactShowOffset;                                                    //Steven 20140409 : 矽品要求Contact畫面顯示Offset
    bool bShowLastContactHigh;                                                  //Steven 20140409 : 顯示之前的Contact高度
    bool bHaveIndexContactDelay;                                                //Steven 20140519 : [D29] Index下壓後多Delay 0.4秒 (For SPIL Low Yield)
    bool bAfterRTChangeToInitialStart;                                          //Steven 20140521 : RT後自動切回FT
    bool bRTCFullViewErrorOnlyRetry;                                            //Steven 20140529
    bool bTechComUseComboBox;                                                   //Steven 20140530 : 使用密碼本時,用下拉選單選使用者
    bool bLockF06ByFile;                                                        //Steven 20140627 : Add for ASE-CL
    bool bLockD41ByFile;                                                        //Steven 20140627 : Add for ASE-CL
    bool bUseNew12Site;                                                         //Steven 20140716
    bool bUseLowYieldAlarmByBin;                                                //Steven 20140828 : By Bin Yield Monitor
    bool bYieldAlarmClearAllCount;                                              //Steven 20140830 : Yield相關的Alarm, 要清掉全部的Ignore的Count重算
    bool bTemperatureMax130;                                                    //Steven 20140911 : Xilinx最高只要130度
    bool bArm2ForFingerPrintTest;                                               //Steven 20140922 : Arm2當作指紋測試  (Arm 1要先測試一次,才換Arm2測)
    bool bUseIndividulTempSet;                                                  //Steven 20140924 : 各個加熱區獨立有自己的設定值
    bool bRCMDStart;                                                            //Steven 20141006 : SECS GEM使用Remote Start功能
    bool bEnable_SECS_GEM;                                                      //Steven 20141008 : By客戶啟用SECS GEM功能
    bool bIndexAreaOnlyCanUseSkip;                                              //Steven 20141105 : Index內的所有異常都只能用Skip
    bool bYieldControlUseContactCount;                                          //Steven 20141212 : Yield控制使用Contact Count
    bool bOLPFunction;                                                          //Steven 20141229 : OLP功能
    bool bOneCycleCanChangeBin;                                                 //Steven 20150304 : ATK Young Jeon keep asking the Continues Failure BIN can select after "OneCycle".
    bool bBin1CanNotInFix;                                                      //Steven 20150427 : Bin1不能放到Fix盤
    bool bAmbientNoShowTemp;                                                    //Steven 20150518 : 蘇州矽品要求常溫不顯示溫度
    bool b12SiteCloseSiteRun2x4;                                                //Steven 20150721 : 12Site關Site跑2x4 Mode
    bool b16SiteCloseSiteRun2x4;                                                //Steven 20191124 : 16Site關Site跑2x4 Mode
    bool bUseGPIB_V108Command;                                                  //Steven 20150901 : For GPIB V1.08 Command
    bool bManualSortMode;                                                       //Steven 20150915 : For TSMC 手動整盤功能
    bool bHiSiliconFunction;                                                    //Steven 20151029 : 海思專用功能
    bool bOutShuttleSensorCanNotDisable;                                        //Steven 20151202 : Out Shuttle Sensor不能關閉檢查
    bool bHaveFIFOMode;                                                         //Steven 20160303 : FIFO Mode
    bool bKnockerSetBySetupFile;                                                //Steven 20160329 : 敲擊汽缸參數調整可搭配工作檔處理
    bool bUseERMS;                                                              //Steven 20160711 : 使用進階版RMS
    bool bUseSCKART;                                                            //Steven 20161201 : For SCK 93K ART
    bool bUseLoginDatToSetLevel;                                                //Steven 20170301 (wei) : 使用Login.dat當密碼本
    bool bLowYieldDoOneCycle;                                                   //Steven 20170308 (wei) : 良率相關的都做One Cycle
    bool bUseInitialDelayAsSoakTime;                                            //Steven 20170329 (Wei) : 使用initial delay當 Soak time
    bool bUse32ChanelSiteMap;                                                   //Steven 20170530 (wei) : Use 32CH site map
    bool bUseDynamicKitDiameter;                                                //Steven 20170605 (wei) : 可以自定義Kit直徑
    bool b2x4SupportCenterPitch;                                                //Steven 20170706 (wei) : 2x4中間的Pitch不同 for SCC
    bool bLowYieldAutoSiteOff;                                                  //Steven 20170905 (wei) : Low Yield Auto Site Off for Ambient
    bool bAmkorFunction;                                                        //Steven 20171017 (wei) : Functions for Amkor Group

    bool bAutoCleanShuttleDisable;                                              //jou 2013-02-27 Auto Clean disable shuttle sensor detect
    bool bSecurityHave5Level;                                                   //jou 2014-06-19 Security Have 5 Level
    bool bRTCAutoModelVerify;                                                   //jou 2014-06-24 RTC 自動進行Model驗證
    bool bSiteCmpYield;                                                         //jou 2014-08-14 Site Compare Low Yield alarm
    bool bFuncStateStopFirtDelay;                                               //jou 2014-09-03 Function State Stop Firt Initital Delay Time
    bool bConAlarmNeedKeyInPassword;                                            //jou 2014-09-04 Continuous Same Alarm N time Need KeyIn Password
    bool bConAlarmInTimeLevelUp;                                                //Steven 20210127 : 逸昌要求在單位時間內相同Alarm發生多次,提昇解除alarm權限
    bool bShowHandlerStopTime;                                                  //jou 2014-09-21 Show Handler Stop Time
    bool bTesterLowYieldOneCycle;                                               //jou 2014-09-23 Tester Low Yield Handler need One Cycle & Alarm
    bool bConFailFoolProofing;                                                  //jou 2014-09-28 continue fail 防呆提醒,總開關開啟為On時,BIN別未勾選任何continue fail alarm
    bool bQAmodeSupplyTrayDir;                                                  //jou 2014-10-24 QA mode 增加支援吸Tray方向
    bool bTempLess30degShowLight;                                               //jou 2014-12-19 Temperature Less 30 deg. Show Light
    bool bInitTempOffsetByWorkFile;                                             //jou 2015-06-13 Initial Temperture Offset by WorkFile
    bool bAutoCleanAutoSelIndexArm;                                             //jou 2015-08-12 Auto Clean Auto Select Index Arm
    bool bAutoRetestGPIBmode;                                                   //jou 2015-10-02 Auto Retest GPIB mode
    bool bDownloadRecipeLevelMode;                                              //jou 2016-01-06 download recipe 增加權限模式選擇
    bool bRTCalarmUnload;                                                       //jou 20170210 (Steven) : RTC alarm image unload
    bool bInOutArmUseBackRowSuck;                                               //jou 20161122 (Steven) In & Out arm use back row suck

    bool bStartESDAutoDecayFunction;                                            //Ifor 20150924 :Start ESD Auto Decay Test Function
    bool bESDAutoDecayTeachFunction;                                            //Ifor 20151222 :新增Coustomer ESD Auto Decay Teach Function
    bool bUseLogUploadToFTPFunction;                                            //Ifor 20160304 :新增Log Up to FTP Function
    bool bUse1_16KgEPForm;                                                      //Ifor 20160712 add Use 1~16 Kg EP對照表功能
    bool bUsePEModelFunction;                                                   //Ifor 20160822 Use PE Model Function (PE 工程模式)
    bool bUseFixTryCheckRemainingAmount;                                        //Ifor 20160829 add Use Fix Try Check Remaining Amount
//    bool bUseLoaderTryFeedContinueRun;                                        //Ifor 20160829 add Use Loader Try Feed Continue Run
    bool bUseMonitorView;                                                       //Ifor 20160930 add MonitorView CosFunction
    bool bUseFix3FullTray;                                                      //Ifor 20161121 add Fix3 Full Tray 功能
    bool bUseAlarmUnlockPassWord;                                               //Ifor 20170214 (wei) add 解除Alarm 需要獨立密碼
    bool bUseARTSortCount;                                                      //Ifor 20170315 (wei) add 新增使用ART Sort Count 計數功能
    bool bUseEmptyColorTrayPreAlarm;                                            //Ifor 20170315 (wei) add 新增Empty/Color Tray Pre Alarm 功能
    bool bUseAutoTrayPreAlarm;                                                  //Ifor 20170315 (wei) add 新增Auto Tray Pre Alarm 功能
    bool bUseMRTMode;                                                           //Ifor 20170316 (wei) add KYEC MRT Mode
    bool bUseAutoBackUpSetupFile;                                               //Ifor 20170620 (wei) add Auto BackUp Setup File & Last Data
    bool bUseARMSFunction;                                                      //Ifor 20170621 (wei) add ARMS Function
    bool bHisiLogUploadNetwork;                                                 //Ifor 20170913 (wei) add 海思 Log 上傳至網路磁碟
    bool bAutoSiteMappingUseHotPlate;                                           //Ifor 20170919 (wei) : add Auto Site Mapping Hotplate Mode
    bool bUseAuto1OnlyBin1;                                                     //Ifor 20171018 (wei) : Only Bin 1 is allowed to set to Auto 1
    bool bUseDefineAutoCleanOffset;                                             //Ifor 20181106 add 新增Auto Clean Offset By 本機
    bool bUse12SiteAutoCleanFixedPosition;                                      //Ifor 20181222 add 新增Auto Clean 12 site取放位置固定
    bool bUseAutoCleanCloseSiteAlsoDo;                                          //Ifor 20181222 add 新增Auto Clean Close Site 一樣執行
    bool bCanChangeAutoCleanCount;                                              //Ifor 20171024 (wei) : add 可更改Auto Clean 計數
    bool bFTPDownLoadSiteBySetupFile;                                           //Ifor 20171123 (Steven) : add FTP DownLoad Site By SetupFile
    bool bAutoSiteMappingUseFailBinSetting;                                     //Ifor 20171128 (Steven) : add Auto Site Mapping Use Fail Bin Setting
    bool bUsePassBinOnlyCanSetOneBin;                                           //Ifor 20171213 (Steven) : Auto Tray 設定Pass時僅可設定一個Bin
    bool bUseFTPDownloadDataCheck;                                              //Ifor 20180125 (Steven) : Use FTP Download Data Check
    bool bFTPDownLoadTempModeBySetupFile;                                       //Ifor 20180212 (Steven) : add FTP DownLoad Temp Mode By SetupFile
    bool bFTPDataTrayFeedAutoUpdata;                                            //Ifor 20181214 (Steven) : add Tray Feed 後自動上傳海思檔案至FTP
    bool bUseOneByOneIndexCheck;                                                //Ifor 20180322 : add Use One By One Index Check
    bool bUseYieldControlFunction;                                              //Ifor 20180731 : add Use Yield Control Function
    bool bUseOpenCloseSiteMapAtAnyTime;                                         //Ifor 20190308 add Open/Close Site Mapping Function at Any Time
    bool bUseLocalRecipeOffset;                                                 //Ifor 20181211 : add Use Local Recipe Offset
    bool bATC32UseTJMode;                                                       //Ifor 20190121 : add ATC32 Use TJ Mode
    bool bATCUseTempAdjustment;                                                 //Ifor 20190215 : add ATC 使用 三點校正功能
    bool bATCUsePackageOffset;                                                  //Ifor 20190215 : add ATC 使用 Package Offset 功能
    bool bUseATCTJControlMode;                                                  //Ifor 20190328 : add TJ Temp Over Range
    bool bUseHandShakeCommunication;                                            //Ifor 20190225 :add Bar Code Use HandShake Communication
    bool bSortingBinTraywhenCleanOut;                                           //JerryYang 20151001 : 整盤功能
    bool bSortingBinTrayWhenTrayFeed;                                           //JerryYang 20170911 (Steven) : 整盤功能,執行時機由clean out改至tray feed前
    bool bResetModeIncludeCleanOut;                                             //JerryYang 20151007 : ResetMode可選OneCycle或CleanOut
    bool bInitTestHeadByTestSiteUse;                                            //JerryYang 20151016 : TestSuck檢查 關Site時就不開真空偵測
    bool bRecipeCheck;                                                          //JerryYang 20151028 add for Amkor 如果download的工作檔不一樣,要求做Height Calibration後才能start
    bool bForceSetAuto3Fix3SideHasNullIC;                                       //JerryYang 20151120 add for 京元Xilinx, IC 大於 40x40mm時會敲到,Auto3和Fix3最右邊要設為 HasNullIC
    bool bNotClearAllHotBuffer;                                                 //JerryYang 20151226 For 矽格 由GPIB設定完溫度後，會馬上再問一次溫度。所以不清除暫存溫度
    bool bLockD42ByFile;                                                        //JerryYang 20160220 add for Amkor-Philippine
    bool bLockD43ByFile;                                                        //JerryYang 20160220 add for Amkor-Philippine
    bool bLockD44ByFile;                                                        //JerryYang 20160220 add for Amkor-Philippine
    bool bLockRTC;                                                              //JerryYang 20160220 add for Amkor-Philippine
    bool bPickupErrorAtLoaderNeedOpenDoor;                                      //JerryYang 20160220 add for Amkor-Philippine,In ARM至Loader吸取異常要開門確認後才能繼續
    bool bLockF19AlwaysOn;                                                      //JerryYang 20160224  add for Amkor-Philippine
    bool bOutShuttleLoseICNeedHome;                                             //JerryYang 20160224 add for Amkor-Philippine, OutShuttle lose IC要回Home 為了檢查index arm上的殘料
    bool bContactTestVacOffByCloseSite;                                         //JerryYang 20160328 Contact test及auto height時,關site的部分不吸取IC
    //AI(ht9045-v899) 20260421: 全智(GIGAS)要求 Auto/Contact Test 過程加強 IC 掉料/吸取失敗偵測，避免下一刀下壓壓壞 IC 與配件
    bool bContactTestICDropGuard;
    bool bFullTestBeforeAutoClean;                                              //JerryYang 20160331 Auto Clean之前要先做Full view check
    bool bYieldAlmNeedOneCycle;                                                 //JerryYang 20160407 Yield相關Alarm要先做完one cycle才show alarm
    bool bContinueFailNeedAlarmDirectly;                                        //Steven 20230831 : 當使用bYieldAlmNeedOneCycle, 連續Fail要直接Alarm
    bool bG09NeedPasswordWhenEditSiteMap;                                       //JerryYang 20160425 修改Site map需要密碼
    bool bLockP24ByFile;                                                        //JerryYang 20160425 Lock P24
    bool bYieldAlarmUseDouble;                                                  //JerryYang 20160615 Yield相關limit可以設定到小數點
    bool bRTCAlarmSetIndexToErrBin;                                             //JerryYang 20160712 for 力成,發生RTC Alarm時把Index上所有IC設為Errorbin
    bool bShowAlarmIfCCLinkConnectFail;                                         //JerryYang 20160729 for 矽品, CC-link連線異常要Show alarm
    bool bAutoFullDoNotStopImmediately;                                         //JerryYang 20160810 當Auto滿tray時,要先show message鳴叫且繼續生產,不要馬上停機show alrm
    bool bIndexArmVacOffErrNeedPiggyBack;                                       //JerryYang 20160815 for 矽品, Index arm 發生Vaccum off error要做piggy back
//    bool bRotateDetectErrNeedShake;                                           //JerryYang 20160825 Rotate sensor偵測異常,要先試著旋轉三次再跳alarm      //Steven 20161116 : Mark
    bool bAllSiteSameFailBinShowAlarm;                                          //JerryYang 20160913 矽品要求當測試結果中所有site的bin都是所設定一樣的fail bin要跳alarm
    bool bPurgeBeforePickShuttle;                                               //JerryYang 20160906 PurgeBeforePickShuttle
    bool bBeforeAutoCleanOnlyUseRTC;                                            //JerryYang 20161216 auto clean的前後只靠RTC來檢查socket,不做index下壓至socket吸真空
    bool bAfterAutoCleanNoIndexCheck;                                           //Steven 20191212 : 劉仁洲說Auto Clean只要作一次Index Check
    bool bIndexCheckCanTurnOff;                                                 //Isaac 20211019 : 可選擇做index check的時機
    bool bOPCanPressStepAndTStart;                                              //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
    bool bSuckDevicesDuringTest;                                                //JerryYang 20170522 drop contact改為邊吸邊測
    bool bFTRTDifferentDutOnOff;                                                //JerryYang 20170516 (wei) JSCC要求FT RT要有不同的開關site
    bool bIndexPickErrSkipNeedCheckVac;                                         //JerryYang 20170610 (wei) JSCC要求index pick up error 需再慢速下降吸一次
    bool bOutShtLoseICSetErrUntilOneCycle;                                      //JerryYang 20170610 (wei) JSCC要求Out shuttle lose IC需自動one cycle,並將對應的site設為Error bin
    bool bBySiteByBinPercentCompare;                                            //JerryYang 20170712 (Steven) by site by bin compare percent
    bool bNewAutoTrayDetect;                                                    //JerryYang 20170822 (Steven)更改Auto區偵測tray盤時機 避免out arm吸嘴下降時造成誤判
//    bool bSaveAutoCleanCnt;                                                   //JerryYang 20171031 (wei) 儲存auto clean count計數 //Steven 20180525 : 儲存auto clean count計數功能改為強制開啟
    bool bUniversalKit;                                                         //JerryYang 20171102 (wei) UniversalKit改成客戶功能
    bool bIndexProtect;                                                         //JerryYang 20180410 (jou) : 加入index arm移動前位置保護
    bool bPopAutoClean;                                                         //JerryYang 20190918 add pop auto clean
    bool bSteupTeach;                                                           //JerryYang 20180921 Setup Teach功能
    bool bSortingBy2DList;                                                      //JerryYang 20190313 : 2D sorting
//    bool bLowYieldAutoClean;                                                  //wei 20141201 Low Yield Auto Clean
    bool bEPUseNSSLK;                                                           //wei 20150303   京元NS浮動頭
    bool bDisableRTBinSet;                                                      //wei 20150622 不顯示設定RT Bin set
    bool bYieldAlarmNoWait1Min;                                                 //wei 20150820  Yield Alarm No Wait 1Min
    bool bSECS_GEM_OneCycle;                                                    //wei 20150824 Secs_Gem 斷線Onecycle
    bool bTrayOCR;                                                              //wei 20150926 TSMC Tray Arm OCR
    bool bLowYeildByTotal;                                                      //wei 20151116 Low Yield By Total
    bool bUseTrayUpDownSet;                                                     //wei 20160224 TSMC FIX UPDOWN      //這個變數改變會影響分BIN, 切換時要提醒客戶注意!!!
    bool bUsePMAlarmFunction;                                                   //wei 20160225 PMAlarmFunction
    bool bContactTestWaitSoakTime;                                              //wei 20160329 Contact Test Wait SoakTime
    bool bYieldAlarm4;                                                          //wei 20160406 Yield Alarm4
    bool bYieldAlarm5;                                                          //Sam 20171213 (Steven) : 超豐良率監控
    bool bCheckInOutArmZHomeSensor;                                             //Frank 20160612 add Check In/Out Arm Home Sensor Function
    bool bEnableBarcodeReader;                                                  //Frank 20150909 : CC_AMKOR 需要使用BarcodeReader讀取工作檔 //Ifor 20161213 IniConfig -> CosFunction

    bool bCancelErrorBin;                                                       //kevin 20160706 取消 ERROR BIN設定
    bool bHWBinBox;                                                             //kevin 20160819 第7個 HARD WARE BIN support Bin Box
    bool bSetupStep;                                                            //wei 20180614 TSMC Setup Step

//    bool bResetFirstTestToErr;                                                //jou 20180103 : Reset第一個測試完的IC要丟到Error bin
    bool bTempHeaterOkShowLight;                                                //jou 20180529 : Temperature Heater Ok Show Light

    bool bUseLotIDWithoutSECS;                                                  //RogerYang 20170327 (Steven) 開放LotID但又不使用SECS/GEM

    bool bInShuttleDetectByLatch;                                               //Isaac 201704:  //Isaac 20170418 用Y Latch檢測Inshuttle有無IC
    bool bRecipeParameterDefault;                                               //Isaac 20170527 defalut值比較功能
    bool bRecipeParameterDefaultLog;                                            //Sam 20201209 : Default Recipe ChangeLog
    bool bProgramStartOnLine;                                                   //Sam 20210423 : 程式開啟時切換為 OnLine/Real
    bool bPiggybackFunctionByHandler;                                           //Isaac 20170712 (wei) :Piggyback function By Handlder(save file to config.ini)
    bool bEnableHandlerResultServer;                                            //Isaac 20170613 (wei) TCP/IP
    int iHandlerCommandServerPort;                                              //Isaac 20170613 (wei) TCP/IP
    int iHandlerResultServerPort;                                               //Isaac 20170613 (wei) TCP/IP
    bool bFTBin2RTBinNoYield;                                                   //jou 20170925 (wei) : FT/RT Bin設定不變,yield control可設定.
    bool bSetupFileNameControlByLevel;                                          //jou 20171011 (wei) : bSetupFileName Control By Level
    bool bByBinAlarmFromYieldForm;                                              //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
    bool bOEEFunction;                                                          //Steven 20180417 (Jou) : OEE功能
    bool bRemoteLotStart;                                                       //AI(ht9045-v899) 20260331: Greatek TCP Remote Start Lot (HTSET 720)
    bool bRTC_ROICount;                                                         //jou 20171201 (Steven) : RTC ROI 確認數量是否正確
    bool bStatisticsJamCount;                                                   //jou 20171201 (Steven) : 新增統計jam code alarm次數,達到設定數量後提高一階權限才能解開alarm
//    bool bIndexPickErrNeedPiggyBack;                                          //jou 20171211 (Steven) : After index pick up error, device go to Error bin ,follow by Index check or piggy back check
    bool bIndexPickErrSkipBlowAirTime;                                          //Frank 20171213 (Steven) : Index Pick Err In Shuttle Skip and Blow Air
    bool bInOutArmZCalibration;                                                 //Frank 20171213 (Steven) : In/OutArmZCalibration
    bool bColorSensorUseCCLink;                                                 //wei 20180409 color sensor
    bool bRotateNoUseFix1;                                                      //Frank 20170531 (Steven) add Open Rotate不能選Fix1
    bool bYieldControlUseEACount;                                               //wei 20180606 Yield控制使用EA Count
    bool IntervalYieldCount;                                                    //wei 20180606 Interval Low Yield By Site
    bool bTestTimeOutOnlyShowSkip;                                              //Steven 20180627 : Time out預設只顯示SKIP
    bool bTestTimeOutShowSkipAndHome;                                           //JerryYang 20231208
    bool bIncludeMTBA;                                                          //JerryYang 20180619 : 新增可自定義Jam code是否列入MTBA計算
    bool bAfterInitialDelayUseOtherArm;                                         //Jerryyang 20180607 : Initail delay後不直接測試,換用另外一支arm下去測,用意是避免device溫度被socket帶走後直接測試容易fail
    bool bTTLUseUSec;                                                           //Steven 20180808 (wei) : TTL的時間單位改成microsecond
    bool bCCLinkValueSaveFile;                                                  //wei 20180622 (Steven) : CCLink Value Save File
    bool bYPitchNotUseSearchLastMode;                                           //JerryYang 20181112 (Steven) : ATP要求修改unloader擺放方式，盡量不要有空格
    bool bDLLCommands;                                                          //JerryYang 20181126 (Steven) : support Epson DLL function
    bool bFTPFunction;                                                          //FTP功能   //Steven 20181224 : 換位置
    bool bGPIBLotEnd;                                                           //Sam 20181030 : 透過 GPIB 送 LotEnd 給測試機。
    bool bSiteStopVisblieSpecialColor;                                          //Sam 20181112 : 開啟 Site 又將 Site 停止時以特別顏色顯示。
    bool bUseOldATCTempOffset;                                                  //JerryYang 20190123 ATC Offset UI使用舊版
    bool bCanUseSearch2DIDByLot;                                                //Steven 20190508
    bool bFTPDownloadAlwaysCover;                                               //JerryYang 20190523 KYEC download工作檔因為是by機台上傳, 所有參數always以server的為主
    bool bCanUse2x2Bias;                                                        //Steven 20190724 : 2x2偏心開關
    bool bCanUse2x2NNMode;                                                      //Steven 20200722 : 2x2 NN mode
    bool bCanUse2x3NNMode;                                                      //Steven 20220425 : 2x3 NN mode
    bool bCanUse2x4NNMode;                                                      //Wei 20231211 : 2X4NN Mode
    bool bForecePerPinKGf;                                                      //JerryYang 20180515 : 輸入每個pin的力量,單位以kgf為主,避免轉換成牛頓又換回KGf會有小數點兩位的差異
    bool bCanUseBias;                                                           //Steven 20190801 : SCC李國旗說要取消BIAS選項
    bool bNextTrayNeedManualRemoveTray;                                         //Sam 20190731 : 當 Load 發生 Pickup Error Skip 時，當前這盤除了要手動移除，下一盤也要手動移除。
    bool bAMDFunction;
    bool bUseHeadContactCount;                                                  //Ifor 20160516 京元要求銦片 Life Time 功能
    bool bHeadContactCountByRecipe;                                             //Steven 20241030 : 銦片 Life Time 功能by工作檔
    bool bDeviceMapTest;                                                        //Steven 20190910 : Qualcomm功能
    bool bSetupTeach;                                                           //JerryYang 20180921 Setup Teach功能
    bool bBarcodeTrayRecFile;                                                   //jou 20190930 : Barcode Tray record file
    bool bBarcodeErrNoTestAndShowH;                                             //jou 20191007 : Barcode Error No Test & Show "H"
    bool bBarcodeDuplicateFileByOutArm;                                         //jou 20191008 : Barcode duplicate file by out arm
    bool bUseChangeLogByLot;                                                    //Ifor 20191002 : add Change Log By Lot
    bool bUseFFCTempOffset;                                                     //Ifor 20190730 : add FFC Temperature Offset
    bool bUseFTPDownLoadATCRecipe;                                              //Ifor 20191115 : add FTP DownLoad ATC Recipe
    bool bUseChamberBoostMode;                                                  //Steven 20191128 : Chamber Boost Function
    bool bATCUseHandlerRecipeName;                                              //Ifor 20191126 : add ATC Use Handler Recipe Name
    int  iAutoRetestTCPmode;                                                    //RogerYang 20251110 : change bool->int //Sam 20191113 : TCP ART
    bool bUnloaderEditTrayLevelSet;                                             //Steven 20191224 : Unloader編輯Tray改用另外一組權限
    bool bAutoCloseSiteWhenRT;                                                  //Steven 20200225 : 切到RT的時候,要關閉Socket
    bool bAutoSkipNoDropError;
    bool bUseN07_5;                                                             //Steven 20200309 : [N07-5]改成by客戶開啟
    bool bAutoHeightSHTReleaseByFile;                                           //Sam 20200217 : K高後 Shuuttle Release Height offset By SetupFile
    bool bCleanOutCanTrayEndBySetupFile;                                        //Steven 20200317 : CleanOut後,可以選擇Tray End, 且要退的Tray要在工作檔設定
    bool bCanDisableQAMode;                                                     //JerryYang 20200312 EQC mode新增function on/off，功能關閉時無法切EQC mode
    bool bEnableDual_1x4Kit;                                                    //Isaac 20200311 : 1x2Site使用1x4Site Kit
    bool bEnable_1x3Kit;                                                        //KevinC 20260109 : 1x2Site and 2x2 NN mode 使用1x3Site Kit
    bool bSaveProductionLogByUnloaderTray;                                      //Steven 20200330 : production log by unloader tray存檔
    bool bUseHandlerSetChillerTemp;                                             //Ifor 20191218 : add ATC Chiller 溫度由Handler設定
    bool bUseEditLDTrayNeedManualRemoveTray;                                    //Ifor 20200331 : add Edit Loader Tray Need Manual Remove Tray
    bool bKeepOnly1SetupFile;                                                   //Steven 20200511 : 改成客戶功能 //wei 20131115 FTP下載後保留下載檔案，其餘Data刪除
    bool bContactHeightSaveToContactIni;                                        //Steven 20200616 : JSCC要求把Contact Height放到別的檔案
    bool bShakeShuttleEveryTime;                                                //Steven 20200616 : ATK要求每次放料都要搖搖蝦頭
    bool bAutoSiteMappingSetOpenBIN;                                            //jou 20200928 : Auto Site Mapping Set Open BIN
    bool bECChangeEventReportCanOnOff;                                          //JerryYang 20200520 舊電腦做DoReportECDataChangeCheck函式會影響UPH,改成功能選項
    bool bOutShtLoseICInArmAway;                                                //JerryYang 20200610 Out shuttle lose IC需先讓位再發出alarm
    bool bNonCenterModeCanUseShtOffset;                                         //Steven 20200715 : for Tinton
    bool bThickTrayNoNeedCover;                                                 //Steven 20200723 : 太厚的Tray不能蓋蓋子
    bool b1x4OnlyAaUse1x1Mode;                                                  //Steven 20200720 : 1x4只開site Ac, 跑1x1模式
    bool bQAModeFinishOfflineShowOnLinePicture;                                 //Sam 20200804 : QA 模式完成後切回 OffLine 時，強制顯示 OnLine 圖片。
    bool bUse2DIDAllSiteFailSetToErrBin;                                        //Steven 20200825 : 2DID all site fail
    bool bEnableMulti2D;                                                        //Steven 20200810 : 一個IC使用多個2DID
    bool b32SiteYOffsetMode;
    bool bShow0Xbin;                                                            //jou 20220719 : show 0X bin
    bool bTempCalByRecipe;                                                      //jou 20220725 : Temperature calibration by recipe
//    bool bLoadPickUpErrorMES0101Record;                                         //Sam 20200922 : Pick up Error Production_Log 也要記錄
    bool bAutoTrayFullAlarmCanNotSkip;                                          //Steven 20210219 : Unloader滿盤alarm不能Skip
    bool bIndexCycleTimeRecord;                                                 //Sam 20200916 : Add Index Cycle Time Record
    bool bUseAfterHomeShtChkLoseICNeedSlow;                                     //Sam 20201020 : 回 Home 第一次的 Shuttle Check IC lose 需要變慢。
    bool LoaderFloating;
    bool bRecordVacuumOnOffTime;                                                //Sam 20210204 : Auto Teach Loader Z Record Vacuum On off time
    bool bI21EnableASMByRecipe;                                                 //Steven 20210518 : 吳如春希望Auto site map從工作檔開關
    bool bUseMDB;                                                               //Steven 20210526 : 部分客戶取消使用MDB
    bool bSpecailLowYeild;                                                      //Sam 20210505 : PTI 要求的兩段 Low Yeild
    bool bTraySortCntFunc;                                                      //Steven 20210608 : Tray指定數量退盤功能
    bool bIOTriggerIonFanAutoClean;                                             //Isaac 20210609 : IO觸發IonFan清針
    bool bUSEJCETSiteMapMode;                                                   //Ifor 20190215 : add ATC 使用 Package Offset 功能
    bool bAutoCleanUseHPSetByRecipe;                                            //Steven 20210825 : Auto Clean使用加熱盤要改成在工作檔設定
    bool b2DUseSubJobFunction;                                                  //Ifor 20200807 add:In House 2D Use Sub Job Function
    bool bFTPUseBarcodeReader;                                                  //Ifor 20210413 add:FTP Use Barcode Reader
    bool b2DUseAnyCharFunction;                                                 //Ifor 20210723 add:2D Use Any Char 收到2D資料不判斷
    bool bOpenAutoCloseSiteInNormal;                                            //JimmyChiu 20211015 : only user disabled site keep disabled and other sites change to enabled
    bool bManualSteplAutoTeach;                                                 //JimmyChiu 20211020 : Auto alignment mode
    bool ShowLotCategoryCount;                                                  //KaiChen 20181002 ：Category Count by Lot
    bool ShowLotCount;                                                          //KaiChen 20181002 ：改成CosFunction   //wei 20171130 Count By Lot
    bool bYieldControlBinSelectUseContactCount;                                 //KaiChen 20181115 : BinSelect裡面 Yield控制使用 Contact Count
    bool bSaveEventLogByLotID;                                                  //KaiChen 20181121 ：矽格-北興 Save Event Log by Lot ID
    bool bConfigStandard;                                                       //Sam 20210324 : 程式開啟，直接讀取 config_Standard.ini
    bool bRotateUseHT7000HPKit;                                                 //Sam 20210416 : 新增特殊模式 For Rotate Function HT7000 HP Kit
    bool bUseJamRawData;                                                        //Sam 20210224 : Auto Upload FTP JAMRawData 功能
    bool bUseAlarmLogXml;                                                       //Sam 20210621 : 新增 Event Log xml  Updata
    bool bUse1CableLayoutKitByConfig;                                           //Sam 20210715 : Use 1CableLayoutKit By Config
    bool bTempSetDisenableNotUseHeater;                                         //Sam 20210805 : TempSet 加熱區地方沒用到的就停用
    bool bNeedToRestartGroundWhenInitialStart;                                  //Sam 20220107 : 矽格北興 Initail Start 要重啟 GroundMan
    bool bOffsetContactForce;                                                   //JimmyChiu 20220114 : Index 總壓力 Offset，加總後數值不顯示於Contact Form
    bool bRTCHalfViewAutoVerify;                                                //JerryYang 20220215 : RTC Auto Verify half view check
    bool bCylinderOnOffTimeLog;                                                 //JerryYang 20220215 : cylinder作動次數計數
    bool b2DUsePinInspection;                                                   //Ifor 20230207 add:In House 2D Use Pin1 Inspection Function
    bool bUseATCRecordFileBy2DIDFunction;                                       //ben 20240927 : add CCD Bar Code to ATC
    bool bS5F1UseJamRate;                                                       //Steven 20220331 : S5F1的ALTX裡面加上JAM Rate判斷
    bool bF18F22InshuttleDetectSaveByRecipe;                                    //JimmyChiu 20220601 : F18/F22 in shuttle A7 sensor進出檢測功能跟隨recipe是否啟用
    bool bLowYieldAlarmIntervalTimeBySetting;                                   //JimmyChiu 20220601 : 修改low yield報警時間邏輯，報警間隔時間固定1min改為可以自行設定報警間隔時間
    bool bAlarmForIonFanCleaning;                                               //JimmyChiu 20220606 : 增加一個離子風扇清潔報警，時間間隔可設定。需報警提示，需輸入密碼才可消除
    bool bRecordGroundESDByTestIC;                                              //Sam 20211223 : 每顆 IC 測試完畢都要記錄當時的 Ground & ESD 數值。
    bool b2DCodeCheckByCoustomerLot;                                            //Sam 20220223 : 2D Code Check by Coustomer Lot
    bool bAutoTmpeOfsByFTP;                                                     //Sam 20220406 : 溫度自動補償功能 By FTP
    bool bLockF26ByFile;                                                        //Sam 20220527 : 鎖住 Enable 彈性人修改
    bool bLockI06ByFile;                                                        //Sam 20220527 : for 矽格-北興
    bool bManuallyRemoveForceInColor;                                           //Sam 20220530 : 當 Loader 發生 Skip/Edit 時，此盤做完後搬到 Empty 軌道後，會收盤起來並報警提示人員收盤(連兩盤)
    bool bUnloadTrayModeByRecipe;                                               //Steven 20220710 : 甬矽要求Unload Tray Mode by機台設置
    bool bFullTrayAlarmAfterUnloadEnd;                                          //Sam 20210602 : Unload 做完後再 Alarm
    bool PassworDownloadByFTP;                                                  //Sam 20210526 : 從 N06 DownloadPath 下載密碼本
    bool bShowYieldMonitor;                                                     //Sam 20210916 : 新增 Yiled Monitor 到畫面上
    bool RecordIndexAirOnTime;                                                  //Sam 20220329 : Record Index Air On Time
    bool bUse2X4_4AutoCleanFixedPosition;                                       //Sam 20220423 : 新增 Auto Clean 2X4_4 模式取放位置固定
    bool bAutoCleanOffsetUseSingleSetting;                                      //Sam 20220720 : AutoClean Offset 僅使用 Clean 設定 Offset 來補償
    bool bUseSocketContactCount;                                                //Sam 20220720 : 新增一組 Socket Count
    bool bDownloadUpdateAutomatically;                                          //Sam 20220824 : FTP 自動下載安裝更新包
    bool bTrayDeviceCheckFromLoader;                                            //JimmyChiu 20220219 : 殘料檢查 From Loader
    bool bTrayMapFromLoader;                                                    //JimmyChiu 20220219 : 殘料檢查 From Loader
    bool bSaveArmSpeedByMachine;                                                //JimmyChiu 20220618 : save by machine
    bool bSaveTemperatureByMachine;                                             //JimmyChiu 20220618 : save by machine
    bool bSaveOffsetByMachine;                                                  //JimmyChiu 20220618 : save by machine
    bool bUnloadCylinerUpRelief;                                                //Sam 20220916 : 退 Tray 前 Unloader 上升汽缸跑減壓動作防止震盤。
    bool bD44Once4Suck;                                                         //Sam 20220920 : D44 Index 回黏檢查一次最多吸四顆
    bool bART_SECSGEM_93K;                                                      //JerryYang 20220923 : SECS GEM版本ART
    bool bOutArmPickUpErrBySetup;                                               //JerryYang 20220923 : E50改成選項
    bool bNonATCSupportLBFunction;                                              //JerryYang 20220923 : LB吹氣function
    bool bIndexDropICSetErrUntilOneCycle;                                       //JerryYang 20220923 : index arm drop error設ERROR BIN
    bool bFTRTDiffInitStartDelayTime;                                           //JerryYang 20220330 : FT RT可設定不同的initial delay time
    bool bUseTrayThickAdjustZHeight;                                            //Ifor 20221214 add: 使用Tray 厚度自動校正吸嘴高度
    bool bUseShuttlePickShiftDetect;                                            //Ifor 20221220 add: Shuttle 吸料後 先上升設定高度再檢查有無IC
    bool bLowYieldUseContactCounts;                                             //Sam 20221020 : LowYield 改使用 ContactCounts 的資料來計算
    bool bShowHPICCount;                                                        //Steven 20221228 : 計算加熱盤IC數量
    bool bTesterSidePushFunction;                                               //Richard 20220321 : 渠梁Side Push
    bool bUserDefineIndexZSafePos;                                              //Richard 20230107 : SPIL客戶自定義機台安裝高度(200~800間)
    bool bAutoSideFixerCheckAlarmAutoRetry;                                     //Sam 20220929 : 增加 AutoSideFixer Check Alarm Auto Retry
    bool bAutoOpenConfigA01;                                                    //Sam 20220929 : Config A01 自動開啟。
    bool bVerifyMode;                                                           //Sam 20221012 : 新增 VerifyMode 功能
    bool bLoginShowUserName;                                                    //Steven 20230317 : 登入時顯示帳號名稱
    bool bMaualRemoveTray3Alarm;                                                //Jimmychiu 20231116 : add detail MES code for Previous tray pick up error
    bool bSmartAutoClean;                                                       //Sam 20230111 : Smart Auto Clean
    bool bAdaptiveYield;                                                        //Sam 20230914 : 自適應性良率監控
    bool bAfterOneCycleShowTrayQty;                                             //Sam 202221101 : OneCycle 完成後顯示所有 Tray 各別的IC數量
    bool bUseBarCoderAutoLogin;                                                 //Sam 20221101 : 使用 BarCoder 自動登錄
    bool bUseBarCoderChangeSetupFile;                                           //Sam 20230320 : 使用 BarCodeReader 來輸入切換 SetupFile。
    bool bSpecialP24;                                                           //Sam 20230320 : 整合 P24 北興版為獨立功能碼
    bool bHandlerStateChangeUploadServer;                                       //Sam 20230511 : 機台資料變更後須上傳 FTP
    bool bLimitSensorOnNeedManualMove;                                          //Sam 20230522 : InOutArm Server Off 後，重新 Server On 時檢查 CCW CW 若有 On 時需要手動排除異常。
    bool bQAModeUseUnloadCnt;                                                   //JerryYang 20221005 : QA mode count 用BIN1數量計數
    bool bTemp5PointKitOffset;                                                  //JerryYang 20221005 : 新增溫度5點offset by recipe儲存
    bool bLotStartLockCriticalPara;                                             //JerryYang 20220310 : ATP要求鎖定critical parameter
    bool bTowerLightUseLD;                                                      //JerryYang 20230721 : LD/ULD區分不同的異常音樂
    bool bThickTrayUseDiffHeight;                                               //JerryYang 20230620 : add厚tray選項
    bool bRead2DIDFromServer;                                                   //Jimmychiu 20230925 : read 2did in json file
    bool bMakeWhite2DIDList;                                                    //RogerYang 20251202 : JCET 2D FT1白名單/FT2比對功能
    bool bATC_SlopeSaveOnHandler;                                               //JerryYang 20231016 : Add handler端設定TJ參數(Slope、Offset)
    bool bUseSecondATCTempOffset;                                               //Ifor 20241118 : 測試中變溫
    bool bPassBinNoRotate;                                                      //jou 20231020 : Pass bin no rotate
    bool bGPIB_Command_DOPAUSE;                                                 //Jimmychiu 20231102 : GPIB Command SETHANDLERDOPAUSE
    bool bRotateUseRTmode;                                                      //jou 20231122 : Rotate Use RT mode
    bool bCategoryInfoByContactCT;                                              //Sam 20240131 : 新增一組 CategoryInfo 紀錄資料使用 ContactCT
    bool bShowCloseSiteAlarmWhenStart;                                          //Jimmychiu 20230925 : Show Close Sites Alarm When Start
    bool bA67TriggerOneCycleWhenAlarm;                                          //JerryYang 20241028 : 矽品彰化要求 特定ALARM要觸發ONE CYCLE
    bool bSECSGEM_UseKeyPro;                                                    //Ifor 20230721 : add SECS GEM 使用 KeyPro 控管
    bool bLockF11ByFile;                                                        //Sam 20240202 : 新增 F11 Lock by file 功能
    bool bLockRTCByFile;                                                        //Sam 20240311 : 新增 RTC Lock by file 功能

    bool bRTCAutoTuning;                                                        //Sam 20230419 : 新增 RTC Auto Tuning 功能
    bool bRTCStepAsideForPhoto;                                                 //Ifor 20230803 add : 新增 RTC Step Aside For Photo功能
    bool bSmartSetup;

    bool bUseSECSGEMAutoStart;                                                  //Ifor 20200521 add: SECS/GEM Auto Start
    bool bUseEESUGOffsetFunction;                                               //Ifor 20200529 add: SECS/GEM EESUG Offset Function
    bool bUseIndexCheckHasICNeedPurge;                                          //Ifor 20200622 add:Index Check Has IC Need Purge
    bool bUseIndexPickShuttleErrNeedPurge;                                      //Ifor 20200622 add:Index Pick Shuttle Err Need Purge
    bool bUseLocalTraySpeed;                                                    //Ifor 20200825 add: Use Local Tray Speed
    bool bUseBarcodeAutoAdjustLight;                                            //Ifor 20210408 add:Barcode 自動調整光源
    bool bUseLoaderToEmptyColorToAuto;                                          //Ifor 20211104 add: Loader 強制到Empty Auto 強制取Color
    bool bUseSimcoESDGiveWayFunction;                                           //Ifor 20211108 add: SIMCO ESD Give Way Function
    bool bContactModeNeedCheckIndexHasIC;                                       //Ifor 20221020 add: Contact Mode每次START 檢查是否有IC在手臂上，有IC需手動取走
    bool bContactModeNeedOneByOneCheckIC;                                       //Ifor 20221104 add:Contact Mode 負壓使用One By One Check IC

    bool bCreateManualEOCAP;                                                    //jou 20221104 : VTest CreateManualEOCAP function;
    bool bAutoSiteMappingSetPassBIN;                                            //jou 20230221 : Auto Site Mapping Set Pass BIN

    bool bLotIDVerify;                                                          //Steven 20240704 : Lot Verification function for ATK
    bool bFixNameOfForcePerPinG;                                                //Steven 20240821 : 凌中心說要修正存檔名稱

    bool bART_RT_NoRotate;                                                      //Sam 20240809 : ART RT No Rotate
    bool bUseTSVFunction;                                                       //Steven 20240904 : for ATK的TSV功能
    bool bCheckTempDuringIndexArmTesting;                                       //Jimmychiu 20240916 : Check the temperature during index arm testing
    bool bNeedAlarmAfterUnloaderFull;                                           //Jimmychiu 20240902 : Need Alarm After Unloader Full
    bool bRMSNoNeedToDownloadEveryTime;                                         //Steven 20240926 : RMS不要每次下載包成功能    //ChungHung 20130621 add SCK RMS SCK 不要每次載入檔案   //ChungHung 20130621 add CC_SPIL_SHINCHU RMS CC_SPIL_SHINCHU 不要每次載入檔案
    bool bGPIBUseSECSGENData;                                                   //Sam 20240826 : GPIB 通訊資料使用 SECSGEM Data
    bool bEnableSocketSensorByPassIndexCheck;                                   //Ifor 20230731 add: 開啟Socket Sensor Index Check不使用真空檢查殘料
    bool bFillTheTrayAfterOutArmPlace;                                          //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
    bool bDisableOpenAllSiteWhenChangeShtMod;                                   //Jimmychiu 20241203 : Disable opening all sites when changing shuttle mode.
    bool bContactAlmNeedOneCycle;                                               //Sam 20241226 : Contact Alarm 需要先做 OneCycle
    bool bEnableRMS;                                                            //Jimmychiu 20241213 : add coustomer func EnableRMS
    bool bContactCountSavedByRecipe;                                            //Jimmychiu 20240826 : Contact count saved by recipe
    bool bScanBarcodeAndDownloadFileInRMS;                                      //Jimmychiu 20241226 : Scan Barcode And Download File In RMS
    bool bAutoCleanTimeCT;                                                      //jou 20250102 : auto clean triger time count
    bool bUploadLogByTypeFolder;                                                //Steven 20250217 : 根據Log內容作資料夾分類
    bool bRunModeFollowLotInfo;                                                 //Steven 20250603 : 根據Lot Info的Run mode進行切換
    bool bUploadTestArmTorqueLog;                                               //Steven 20250609 : 統一整理
    bool bCanRemoteStart;                                                       //Steven 20250712 : 有通過風險告知可以遠端start的客戶
    bool bEndLotAfterTrayFeed;                                                  //Jimmychiu 20250115 : Auto End Lot After Tray Feed
    bool bUseRenesasServer;                                                     //RogerYang 20250812 : 瑞薩FT-CT
    bool bReadClipCodeFromUnloader;                                             //Jimmychiu 20250818 : Read Clip Code From Unloader(Auto1-3、Fix1-3)
    bool bSocketSensorCheckICAtArmDown;                                         //Jimmychiu 20250826 : 每次下壓確認有IC在socket
    bool bOpenDoorCheckLoaderAfterTrayEnd;                                      //Jimmychiu 20250826 : Open Door Check Loader After TrayEnd
    bool bOneByOneWhenPickErrAtLoader;                                          //Jimmychiu 20250924 : Suck one by one when a pickup error occurs at the loader.
    bool bSetContactForceToAutoClean;                                           //Steven 20250915 : 複製Contact Force給Auto Clean用
    bool bTempAlarmBinNeedToError;                                              //Steven 20251022 : Temp alarm need put to error bin
    bool bInspectSuckICWhenSoftStart;                                           //Jimmychiu 20240217 : 汶彥要求移除檢查
    bool bCheckOutSuckICFallDownOnlySkip;                                       //Jimmychiu 20240813 : Only Skip When Checking Out Suck IC Fall Down
    bool bAutoTeachOutShuttleLantch;                                            //Jimmychiu 20240923 : Auto teach in/out shuttle site kit position
    bool bMonitorLBonAmbient;                                                   //Jimmychiu 20251030 : Monitor the LB temperature under ambient temperature
    bool bLoaderTrayToAuto1;
    bool bFirstTrayCheckOnUnloader;                                             //Jimmychiu 20251205 : First Tray Check On Unloader
    bool bDeviceMapTestPandP;                                                   //Jimmychiu 20251222 : Device Map Test By P&P
    bool bATCModeNoUseCheckHeaterFunction;                                      //Ifor 20260113 add:ATC Mode No Use CheckHeater Function
    bool bOffsetTempByRecipeMinMaxLimit;                                        //StevenHong 20260119 : Add Offset By Recipe Max Limit    
    bool bCleanCountAlarmByMin;                                                 //Jimmychiu 20260212 : Gigas Clear alarms based on minimum usage count
} HT9045_COUSTOMER_FUNCTION;
extern HT9045_COUSTOMER_FUNCTION CosFunction;
extern void InitialCosFunction();
extern void VTEST_Funtion();
extern void SPILFunction();
extern void KoreaFunction();
extern void SIGURDFunction();
extern void MaximFunction();
#endif

