#include "MachineDefine.h"
#pragma hdrstop

#include "cSecurity.h"

#include "login.h"
#include "cprod.h"
#include "cMyDef.h"
#include "main.h"
#include "note.h"
#include "common.h"
#include "cpublic.h"
#include "mymessbox.h"
#include "cMyDB.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfSecurity *fSecurity;
int iMaxLevelItem;
int iBit8;
Graphics::TBitmap *DefaultImg;                                                  //Steven 20160108 : release memory
//---------------------------------------------------------------------------
__fastcall TfSecurity::TfSecurity(TComponent* Owner)
    : TForm(Owner)
{                                                                               //Steven 20130611 : 權限設定改為分頁方式
    DefaultImg=fMain->sbSetting->Glyph;

    mySecurityPal.push_back(new TMySecurity("[00] Main - Tools",                                    fMain->sbSetting->Glyph,        sbMain));
    mySecurityPal.push_back(new TMySecurity("[01] Main - Config",                                   fMain->sbConfig->Glyph,         sbMain));
    mySecurityPal.push_back(new TMySecurity("[02] Main - Offset",                                   fMain->sbOffset->Glyph,         sbMain));
    mySecurityPal.push_back(new TMySecurity("[03] Main - Speed",                                    fMain->sbSpeed->Glyph,          sbMain));
    mySecurityPal.push_back(new TMySecurity("[04] Main - IO",                                       fMain->sbIO->Glyph,             sbMain));
    mySecurityPal.push_back(new TMySecurity("[05] Main - Message",                                  fMain->sbMessage->Glyph,        sbMain));
    mySecurityPal.push_back(new TMySecurity("[06] Main - Exit",                                     fMain->sbCloseProgram->Glyph,   sbMain));
    mySecurityPal.push_back(new TMySecurity("[07] Main - Hot Mode",                                 fMain->sbTempOffset->Glyph,     sbMain));
    mySecurityPal.push_back(new TMySecurity("[08] Main - Tester On/Off line",                       fMain->sbTester->Glyph,         sbMain));
    mySecurityPal.push_back(new TMySecurity("[09] Main - Setup File",                               fMain->sbTester->Glyph,         sbMain));                   //kevin 20180917 add setup file
    mySecurityPal.push_back(new TMySecurity("[10] Main - Site Select",                              fMain->sbTester->Glyph,         sbMain));
    mySecurityPal.push_back(new TMySecurity("[11] Main - Real/Dummy",                               fMain->sbStartMode->Glyph,      sbMain));
    mySecurityPal.push_back(new TMySecurity("[12] Main - Start Mode",                               fMain->sbStartMode->Glyph,      sbMain));
    mySecurityPal.push_back(new TMySecurity("[13] Main - Auto Teach Close",                         fMain->sbStartMode->Glyph,      sbMain));                   //JimmyChiu 20211020 : Auto alignment mode
    mySecurityPal.push_back(new TMySecurity("[14] Tools - Tray Form",                               fMain->sbTrayForm->Glyph,       sbTools));
    mySecurityPal.push_back(new TMySecurity("[15] Tools - Plate Form",                              fMain->sbPlateForm->Glyph,      sbTools));
    mySecurityPal.push_back(new TMySecurity("[16] Tools - Tray Assign",                             fMain->sbTrayAssign->Glyph,     sbTools));
    mySecurityPal.push_back(new TMySecurity("[17] Tools - Temp.OffSet",                             fMain->sbTempOffset->Glyph,     sbTools));
    mySecurityPal.push_back(new TMySecurity("[18] Tools - Contact",                                 fMain->sbContact->Glyph,        sbTools));
    mySecurityPal.push_back(new TMySecurity("[19] Tools - Test IF",                                 fMain->sbTester->Glyph,         sbTools));
    mySecurityPal.push_back(new TMySecurity("[20] Tools - Bin",                                     fMain->sbBin->Glyph,            sbTools));
    mySecurityPal.push_back(new TMySecurity("[21] Tools - Set Up",                                  fMain->sbSetup->Glyph,          sbTools));
    mySecurityPal.push_back(new TMySecurity("[22] Tools - Load/Unld",                               fMain->sbLdUld->Glyph,          sbTools));
    mySecurityPal.push_back(new TMySecurity("[23] Config - Builder",                                fMain->sbBuilder->Glyph,        sbConfig));
    mySecurityPal.push_back(new TMySecurity("[24] Config - Start Mode",                             fMain->sbStartMode->Glyph,      sbConfig));
    mySecurityPal.push_back(new TMySecurity("[25] Config - C.Select",                               fMain->sbSelete->Glyph,         sbConfig));
    mySecurityPal.push_back(new TMySecurity("[26] Config - C.Clear",                                fMain->sbClear->Glyph,          sbConfig));
    mySecurityPal.push_back(new TMySecurity("[27] TestIF - InterFace Type",                         fMain->sbTester->Glyph,         sbSetup));                  //JimmyChiu 20211228 : InterFace Type
    mySecurityPal.push_back(new TMySecurity("[28] Config - Tower Light",                            fMain->sbTowerLight->Glyph,     sbConfig));
    mySecurityPal.push_back(new TMySecurity("[29] Config - Password",                               fMain->sbPassword->Glyph,       sbConfig));
    mySecurityPal.push_back(new TMySecurity("[30] Config - Configuration",                          fMain->sbConfiguration->Glyph,  sbConfig));
    mySecurityPal.push_back(new TMySecurity("[31] Config - DIO Setting",                            fMain->sbDioSet->Glyph,         sbConfig));
    mySecurityPal.push_back(new TMySecurity("[32] Main - Temperature Deg Setup",                    fMain->spbSet->Glyph,           sbMain));
    mySecurityPal.push_back(new TMySecurity("[33] Set Offset Limit",                                fMain->sbOffset->Glyph,         sbOther));                  //沒用到
    mySecurityPal.push_back(new TMySecurity("[34] Tool Alarm",                                      DefaultImg,                     sbOther));                  //沒用到
    mySecurityPal.push_back(new TMySecurity("[35] Alarm - Trouble Shooting",                        DefaultImg,                     sbOther));
    mySecurityPal.push_back(new TMySecurity("[36] Alarm - Low Yield Alarm",                         fMain->sbYield->Glyph,          sbOther));                  //沒用到
    mySecurityPal.push_back(new TMySecurity("[37] Tools - CCD",                                     fMain->sbCCD->Glyph,            sbTools));
    mySecurityPal.push_back(new TMySecurity("[38] Contact - Contact Force",                         fMain->sbContact->Glyph,        sbContact));
    mySecurityPal.push_back(new TMySecurity("[39] Tools - Yield Monitoring",                        fMain->sbYield->Glyph,          sbTools));
    mySecurityPal.push_back(new TMySecurity("[40] Setup - Shuttle Mode",                            fMain->sbSetup->Glyph,          sbSetup));
    mySecurityPal.push_back(new TMySecurity("[41] Main - Change SetUp File",                        fMain->sbStartMode->Glyph,      sbMain));
    mySecurityPal.push_back(new TMySecurity("[42] Config - SECS\GEM",                               fMain->sbSecsGem->Glyph,        sbConfig));
    mySecurityPal.push_back(new TMySecurity("[43] Tools - Auto Clean",                              fMain->sbAutoClean->Glyph,      sbTools));
    mySecurityPal.push_back(new TMySecurity("[44] Tools - ATC",                                     fMain->sbATC->Glyph,            sbTools));
    mySecurityPal.push_back(new TMySecurity("[45] Tools - Sensor Adj.",                             fMain->sbShuttleSensor->Glyph,  sbTools));
    mySecurityPal.push_back(new TMySecurity("[46] Config - Sensor Latch",                           fMain->sbSensorLatch->Glyph,    sbConfig));
    mySecurityPal.push_back(new TMySecurity("[47] Config - Omron Temp.",                            fMain->sbOmron->Glyph,          sbConfig));
    mySecurityPal.push_back(new TMySecurity("[48] Tools - OCR",                                     fMain->sbOCR->Glyph,            sbTools));                  //Steven 20120716 : OCR
    mySecurityPal.push_back(new TMySecurity("[49] Config - Auto Temp.",                             fMain->sbAutoTemp->Glyph,       sbConfig));
    mySecurityPal.push_back(new TMySecurity("[50] Temp.OffSet - Temp. Mode",                        fMain->sbTempOffset->Glyph,     sbTemp));                   //Steven 30230803 : Temp Offset畫面控制
    mySecurityPal.push_back(new TMySecurity("[51] Temp.OffSet - Default Offset",                    fMain->sbTempOffset->Glyph,     sbTemp));
    mySecurityPal.push_back(new TMySecurity("[52] Temp.OffSet - Base Point",                        fMain->sbTempOffset->Glyph,     sbTemp));
    mySecurityPal.push_back(new TMySecurity("[53] Temp.OffSet - User Offser",                       fMain->sbTempOffset->Glyph,     sbTemp));
    mySecurityPal.push_back(new TMySecurity("[54] Temp.OffSet - Single Limits",                     fMain->sbTempOffset->Glyph,     sbTemp));
    mySecurityPal.push_back(new TMySecurity("[55] Temp.OffSet - Hot Mode",                          fMain->sbTempOffset->Glyph,     sbTemp));
    mySecurityPal.push_back(new TMySecurity("[56] Temp.OffSet - Amb Mode",                          fMain->sbTempOffset->Glyph,     sbTemp));
    mySecurityPal.push_back(new TMySecurity("[57] Temp.OffSet - Index Mode",                        fMain->sbTempOffset->Glyph,     sbTemp));
    mySecurityPal.push_back(new TMySecurity("[58] IO - STACK 1",                                    fMain->sbIO->Glyph,             sbIO));                     //Steven 20120803 : IO畫面控制
    mySecurityPal.push_back(new TMySecurity("[59] IO - STACK 2",                                    fMain->sbIO->Glyph,             sbIO));                     //Steven 20120803 : IO畫面控制
    mySecurityPal.push_back(new TMySecurity("[60] IO - SUCKER",                                     fMain->sbIO->Glyph,             sbIO));                     //Steven 20120803 : IO畫面控制
    mySecurityPal.push_back(new TMySecurity("[61] IO - SHUTTLE",                                    fMain->sbIO->Glyph,             sbIO));                     //Steven 20120803 : IO畫面控制
    mySecurityPal.push_back(new TMySecurity("[62] IO - KEY PAD",                                    fMain->sbIO->Glyph,             sbIO));                     //Steven 20120803 : IO畫面控制
    mySecurityPal.push_back(new TMySecurity("[63] IO - SYSTEM",                                     fMain->sbIO->Glyph,             sbIO));                     //Steven 20120803 : IO畫面控制
    mySecurityPal.push_back(new TMySecurity("[64] IO - INDEX",                                      fMain->sbIO->Glyph,             sbIO));                     //Steven 20120803 : IO畫面控制
    mySecurityPal.push_back(new TMySecurity("[65] IO - TTL",                                        fMain->sbIO->Glyph,             sbIO));                     //Steven 20120803 : IO畫面控制
    mySecurityPal.push_back(new TMySecurity("[66] IO - TOOLS",                                      fMain->sbIO->Glyph,             sbIO));                     //Steven 20120803 : IO畫面控制
    mySecurityPal.push_back(new TMySecurity("[67] Configure - A[Function]",                         fMain->sbConfiguration->Glyph,  sbConfiguration));          //Steven 20120803 : Configure畫面控制
    mySecurityPal.push_back(new TMySecurity("[68] Configure - C[Hardware]",                         fMain->sbConfiguration->Glyph,  sbConfiguration));          //Steven 20120803 : Configure畫面控制
    mySecurityPal.push_back(new TMySecurity("[69] Configure - D[Index]",                            fMain->sbConfiguration->Glyph,  sbConfiguration));          //Steven 20120803 : Configure畫面控制
    mySecurityPal.push_back(new TMySecurity("[70] Configure - E[In/Out Arm]",                       fMain->sbConfiguration->Glyph,  sbConfiguration));          //Steven 20120803 : Configure畫面控制
    mySecurityPal.push_back(new TMySecurity("[71] Configure - F[Shuttle]",                          fMain->sbConfiguration->Glyph,  sbConfiguration));          //Steven 20120803 : Configure畫面控制
    mySecurityPal.push_back(new TMySecurity("[72] Configure - G[Visible]",                          fMain->sbConfiguration->Glyph,  sbConfiguration));          //Steven 20120803 : Configure畫面控制
    mySecurityPal.push_back(new TMySecurity("[73] Configure - I[Tester]",                           fMain->sbConfiguration->Glyph,  sbConfiguration));          //Steven 20120803 : Configure畫面控制
    mySecurityPal.push_back(new TMySecurity("[74] Configure - L[Temperature]",                      fMain->sbConfiguration->Glyph,  sbConfiguration));          //Steven 20120803 : Configure畫面控制
    mySecurityPal.push_back(new TMySecurity("[75] Configure - O[Count]",                            fMain->sbConfiguration->Glyph,  sbConfiguration));          //Steven 20120803 : Configure畫面控制
    mySecurityPal.push_back(new TMySecurity("[76] Configure - N[Network]",                          fMain->sbConfiguration->Glyph,  sbConfiguration));          //Steven 20120803 : Configure畫面控制
    mySecurityPal.push_back(new TMySecurity("[77] Configure - P[Tray]",                             fMain->sbConfiguration->Glyph,  sbConfiguration));          //Steven 20120803 : Configure畫面控制
    mySecurityPal.push_back(new TMySecurity("[78] Configure - Tray Data",                           fMain->sbConfiguration->Glyph,  sbConfiguration));          //Steven 20120803 : Configure畫面控制
    mySecurityPal.push_back(new TMySecurity("[79] Configure - Hot Plate Data",                      fMain->sbConfiguration->Glyph,  sbConfiguration));          //Steven 20120803 : Configure畫面控制
    mySecurityPal.push_back(new TMySecurity("[80] Tools - Dyna. Temperature",                       fMain->sbDynamicTemp->Glyph,    sbTools));                  //Steven 20120807 : 動態溫度
    mySecurityPal.push_back(new TMySecurity("[81] Yield - Yield Alarm",                             fMain->sbYield->Glyph,          sbYield));                  //Steven 20120803 : Yield畫面控制
    mySecurityPal.push_back(new TMySecurity("[82] Yield - Piggy-Back Funtions",                     fMain->sbYield->Glyph,          sbYield));                  //Steven 20120803 : Yield畫面控制
    mySecurityPal.push_back(new TMySecurity("[83] Yield - Alarm",                                   fMain->sbYield->Glyph,          sbYield));                  //Steven 20120803 : Yield畫面控制
    mySecurityPal.push_back(new TMySecurity("[84] Yield - Failure rate Alarm",                      fMain->sbYield->Glyph,          sbYield));                  //Steven 20120803 : Yield畫面控制
    mySecurityPal.push_back(new TMySecurity("[85] Tools - QA Mode",                                 fMain->sbQAMode->Glyph,         sbTools));                  //Steven 20120803 : QA Mode
    mySecurityPal.push_back(new TMySecurity("[86] Config - Motion View",                            fMain->sbMotionView->Glyph,     sbConfig));                 //Steven 20120830 : Motion View按鈕權限
    mySecurityPal.push_back(new TMySecurity("[87] Main - Teaching",                                 fMain->sbTeaching->Glyph,       sbMain));                   //Steven 20120830 : Teaching按鈕權限
    mySecurityPal.push_back(new TMySecurity("[88] Tools - BarCode",                                 fMain->sbBarCode->Glyph,        sbTools));                  //Steven 20121009 : Bar Code權限
    mySecurityPal.push_back(new TMySecurity("[89] Tools - Rotate",                                  fMain->sbRotate->Glyph,         sbTools));                  //Steven 20130626 : Rotate權限
    mySecurityPal.push_back(new TMySecurity("[90] Config - Air Con.",                               fMain->spbAirConditioner->Glyph,sbConfig));                 //Steven 20131011 : 冷氣機
    mySecurityPal.push_back(new TMySecurity("[91] TestIF - Test Time",                              fMain->sbTester->Glyph,         sbSetup));                  //Steven 20140114 : 測試時間設定的權限
    mySecurityPal.push_back(new TMySecurity("[92] Contact - Contact parameter",                     fMain->sbContact->Glyph,        sbContact));                //jou 2014-03-26 SPIL 要求 Contact mode 權限分級
    mySecurityPal.push_back(new TMySecurity("[93] Contact - Handler Mode",                          fMain->sbContact->Glyph,        sbContact));                //jou 2014-03-26 SPIL 要求 Contact mode 權限分級
    mySecurityPal.push_back(new TMySecurity("[94] Setup - Site Map",                                fMain->sbSetup->Glyph,          sbSetup));                  //20140310 wei :Site Map權限 KYEC
    mySecurityPal.push_back(new TMySecurity("[95] Setup - Shuttle Mode",                            fMain->sbSetup->Glyph,          sbSetup));                  //20140310 wei :Shuttle Mode權限 KYEC   //Steven 20140428 : 重複的砍掉
    mySecurityPal.push_back(new TMySecurity("[96] Configure - M[Monitor]",                          fMain->sbConfiguration->Glyph,  sbConfiguration));
    mySecurityPal.push_back(new TMySecurity("[97] Other - Auto Clean Clear Count",                  fMain->sbAutoClean->Glyph,      sbOther));                  //Steven 20140519 : Security[97] Auto Clean Clear Count
    mySecurityPal.push_back(new TMySecurity("[98] Other - QA Mode Device Count",                    fMain->sbQAMode->Glyph,         sbOther));                  //Steven 20150209 : QA mode
    mySecurityPal.push_back(new TMySecurity("[99] Tools - Socket",                                  fMain->sbSocket_ASE_KR->Glyph,  sbTools));
    mySecurityPal.push_back(new TMySecurity("[100] Tools - Laser Sensor",                           fMain->spbLaser->Glyph,         sbTools));
    mySecurityPal.push_back(new TMySecurity("[101] Contact - Height",                               fMain->sbContact->Glyph,        sbContact));
    mySecurityPal.push_back(new TMySecurity("[102] Other - Loader Tray Mode",                       fMain->sbTester->Glyph,         sbOther));                  //Steven 20150416 : 新的權限設定
    mySecurityPal.push_back(new TMySecurity("[103] Contact - Test Socket IC check",                 fMain->sbContact->Glyph,        sbContact));                //wei 20150417 : 新的權限設定
    mySecurityPal.push_back(new TMySecurity("[104] Other - Tray Edit",                              fMain->sbTrayForm->Glyph,       sbOther));                  //Steven 20150505 : SPIL蘇州要求Tray編輯要有權限
    mySecurityPal.push_back(new TMySecurity("[105] Main - Big Fan",                                 fMain->spbFan->Glyph,           sbMain));                   //wei 20150521 Big Fan 台積電
    mySecurityPal.push_back(new TMySecurity("[106] Temp.OffSet - Initial Temp Offset",              fMain->sbTempOffset->Glyph,     sbTemp));                   //wei 20150930 Initial Temp Offset權限
    mySecurityPal.push_back(new TMySecurity("[107] Other - Yield Count Clean",                      fMain->sbContact->Glyph,        sbOther));                  //wei 20151022 Yield Count Clean權限設定
    mySecurityPal.push_back(new TMySecurity("[108] Other - Bin Clean Count",                        fMain->sbContact->Glyph,        sbOther));                  //wei 20151022 Bin Clean Count權限設定
    mySecurityPal.push_back(new TMySecurity("[109] Contact - Sensor Adjustment",                    fMain->sbContact->Glyph,        sbContact));                //Ifor 20151103 : 新增Contact 畫面中 Sensor Adjustment權限設定
    mySecurityPal.push_back(new TMySecurity("[110] PM Alarm - User PM Alarm Level",                 fMain->sbPMAlarm->Glyph,        sbOther));                  //wei 20160225 PMAlarmFunction
    mySecurityPal.push_back(new TMySecurity("[111] PM Alarm - Item Setup",                          fMain->sbPMAlarm->Glyph,        sbOther));                  //wei 20160225 PMAlarmFunction
    mySecurityPal.push_back(new TMySecurity("[112] PM Alarm - Visible Setup",                       fMain->sbPMAlarm->Glyph,        sbOther));                  //wei 20160225 PMAlarmFunction
    mySecurityPal.push_back(new TMySecurity("[113] PM Alarm - Visible Executive",                   fMain->sbPMAlarm->Glyph,        sbOther));                  //wei 20160225 PMAlarmFunction
    mySecurityPal.push_back(new TMySecurity("[114] ATC Control",                                    fMain->sbATC->Glyph,            sbOther));                  //Ifor 20160108 : 新增LotInfo畫面中 ATC 權限設定
    mySecurityPal.push_back(new TMySecurity("[115] Tray - Load/Unload",                             fMain->sbLdUld->Glyph,          sbTools));                  //Steven 20160329 : 敲擊汽缸參數調整可搭配工作檔處理
    mySecurityPal.push_back(new TMySecurity("[116] Tray - Knocker",                                 fMain->sbLdUld->Glyph,          sbTools));                  //Steven 20160329 : 敲擊汽缸參數調整可搭配工作檔處理
    mySecurityPal.push_back(new TMySecurity("[117] Yield - Alarm4",                                 fMain->sbYield->Glyph,          sbYield));                  //wei 20160406 Yield Alarm4
    mySecurityPal.push_back(new TMySecurity("[118] Yield - Yield Alarm Enable",                     fMain->sbYield->Glyph,          sbYield));                  //Steven 20120803 : Yield畫面控制
    mySecurityPal.push_back(new TMySecurity("[119] Yield - Piggy-Back Funtions Enable",             fMain->sbYield->Glyph,          sbYield));                  //Steven 20120803 : Yield畫面控制
    mySecurityPal.push_back(new TMySecurity("[120] Yield - Alarm Enable",                           fMain->sbYield->Glyph,          sbYield));                  //Steven 20120803 : Yield畫面控制
    mySecurityPal.push_back(new TMySecurity("[121] Yield - Alarm4 Enable",                          fMain->sbYield->Glyph,          sbYield));                  //wei 20160406 Yield Alarm4
    mySecurityPal.push_back(new TMySecurity("[122] Other - Shuttle Sensor Move",                    fMain->sbContact->Glyph,        sbOther));                  //wei 20160406 Yield Alarm4
    mySecurityPal.push_back(new TMySecurity("[123] TestIF -Use initial start delay in socket",      fMain->sbTester->Glyph,         sbSetup));                  //JerryYang 20170303 (wei) 修改為TEST I.F圖示//jou 2016-05-12 新增 "Use initial start delay in socket" 權限管控
    mySecurityPal.push_back(new TMySecurity("[124] Temp.OffSet - EOT Extra Temp Offset",            fMain->sbTempOffset->Glyph,     sbTemp));                   //jou 2016-05-17 新增 "[124] Temp.OffSet - EOT Extra Temp Offset" 權限管控
    mySecurityPal.push_back(new TMySecurity("[125] Yield - Alarm Control Access",                   fMain->sbYield->Glyph,          sbYield));                  //Alick 20160616 ASE CL 將啟用功能與計數控制分開
    mySecurityPal.push_back(new TMySecurity("[126] Yield - Failure Count",                          fMain->sbYield->Glyph,          sbYield));                  //Alick 20160616 ASE CL 將啟用功能與計數控制分開
    mySecurityPal.push_back(new TMySecurity("[127] Config - Monitor View",                          fMain->sbMonitorView->Glyph,    sbConfig));                 //JerryYang 20160705 : Monitor View按鈕權限
    mySecurityPal.push_back(new TMySecurity("[128] PE Model Control",                               DefaultImg,                     sbOther));                  //Ifor 20160822 : 新增PE Model 權限設定
    mySecurityPal.push_back(new TMySecurity("[129] Yield - ART User Fix",                           fMain->spbAutoRetest->Glyph,    sbYield));                  //Steven 20161201 : For SCK 93K ART
    mySecurityPal.push_back(new TMySecurity("[130] Yield - ART Setting",                            fMain->spbAutoRetest->Glyph,    sbYield));                  //Steven 20161201 : For SCK 93K ART
    mySecurityPal.push_back(new TMySecurity("[131] TestIF - Start Delay",                           fMain->sbTester->Glyph,         sbSetup));                  //JerryYang 20170302 (wei) ASE_CL Start delay權限控制
    mySecurityPal.push_back(new TMySecurity("[132] Bin - Double Contact",                           fMain->sbBin->Glyph,            sbSetup));                  //JerryYang 20170303 (wei) ASE_CL Double Contact權限控制
    mySecurityPal.push_back(new TMySecurity("[133] Speed - Auto Speed",                             fMain->sbSpeed->Glyph,          sbOther));                  //JerryYang 20170303 (wei) ASE_CL Auto Speed權限控制
    mySecurityPal.push_back(new TMySecurity("[134] Speed - [Index Arm] Wait Time",                  fMain->sbSpeed->Glyph,          sbOther));                  //JerryYang 20170303 (wei) ASE_CL Index Wait Time權限控制
    mySecurityPal.push_back(new TMySecurity("[135] Speed - [Index Arm] Destroy Time and Count",     fMain->sbSpeed->Glyph,          sbOther));                  //JerryYang 20170303 (wei) ASE_CL Index Destroy Time and Count權限控制
    mySecurityPal.push_back(new TMySecurity("[136] Speed - [Input Arm] Function for small package", fMain->sbSpeed->Glyph,          sbOther));                  //JerryYang 20170303 (wei) ASE_CL Input Arm權限控制
    mySecurityPal.push_back(new TMySecurity("[137] Speed - [In Arm] Release Delay",                 fMain->sbSpeed->Glyph,          sbOther));                  //JerryYang 20170303 (wei) ASE_CL Input Arm權限控制
    mySecurityPal.push_back(new TMySecurity("[138] Speed - [In Arm] Retry Count and Wait Time",     fMain->sbSpeed->Glyph,          sbOther));                  //JerryYang 20170303 (wei) ASE_CL Input Arm權限控制
    mySecurityPal.push_back(new TMySecurity("[139] Speed - [In Arm] Destroy Time and Count",        fMain->sbSpeed->Glyph,          sbOther));                  //JerryYang 20170303 (wei) ASE_CL Input Arm權限控制
    mySecurityPal.push_back(new TMySecurity("[140] Speed - [In Arm] Two Speed Move Down",           fMain->sbSpeed->Glyph,          sbOther));                  //JerryYang 20170303 (wei) ASE_CL Input Arm權限控制
    mySecurityPal.push_back(new TMySecurity("[141] Speed - [Out Arm] Function for small package",   fMain->sbSpeed->Glyph,          sbOther));                  //JerryYang 20170303 (wei) ASE_CL Output Arm權限控制
    mySecurityPal.push_back(new TMySecurity("[142] Speed - [Out Arm] Retry Count and Wait Time",    fMain->sbSpeed->Glyph,          sbOther));                  //JerryYang 20170303 (wei) ASE_CL Output Arm權限控制
    mySecurityPal.push_back(new TMySecurity("[143] Speed - [Out Arm] Destroy Time and Count",       fMain->sbSpeed->Glyph,          sbOther));                  //JerryYang 20170303 (wei) ASE_CL Output Arm權限控制
    mySecurityPal.push_back(new TMySecurity("[144] Speed - [Out Arm] Destroy Check",                fMain->sbSpeed->Glyph,          sbOther));                  //JerryYang 20170303 (wei) ASE_CL Output Arm權限控制
    mySecurityPal.push_back(new TMySecurity("[145] Speed - [Out Arm] Two Speed Move Down",          fMain->sbSpeed->Glyph,          sbOther));                  //JerryYang 20170303 (wei) ASE_CL Output Arm權限控制
    mySecurityPal.push_back(new TMySecurity("[146] Speed - [Tray Arm] Wait Time",                   fMain->sbSpeed->Glyph,          sbOther));                  //JerryYang 20170303 (wei) ASE_CL Tray Arm wait time權限控制
    mySecurityPal.push_back(new TMySecurity("[147] Speed - Auto Skip",                              fMain->sbSpeed->Glyph,          sbMain));                   //Isaac 20170327 (jou): AutoSkip權限控制
    mySecurityPal.push_back(new TMySecurity("[148] Tools - [Bin] Yield Control",                    fMain->sbBin->Glyph,            sbTools));                  //Steven 20170603 (wei) : Add for Korea
    mySecurityPal.push_back(new TMySecurity("[149] Tools - [Bin] Tray Setting",                     fMain->sbBin->Glyph,            sbTools));                  //Steven 20170603 (wei) : Add for Korea
    mySecurityPal.push_back(new TMySecurity("[150] Tools - RPDefault",                              fMain->sbSpeed->Glyph,          sbTools));                  //Isaac 20170630 (Steven) Recipe Parameter Default權限控制
    mySecurityPal.push_back(new TMySecurity("[151] Setup - ERMS Selection",                         fMain->sbSetup->Glyph,          sbSetup));                  //Steven 20170829 (wei) : ERMS Selsection新增權限控管
    mySecurityPal.push_back(new TMySecurity("[152] Contact - Contact Force Calibration",            fMain->sbContact->Glyph,        sbContact));                //JerryYang 20171102 (wei) add 權限控管
    mySecurityPal.push_back(new TMySecurity("[153] Yield - Alarm5 Option",                          fMain->sbYield->Glyph,          sbYield));                  //Sam 20171213 (Steven) : 超豐良率監控
    mySecurityPal.push_back(new TMySecurity("[154] Yield - Alarm5 Value",                           fMain->sbYield->Glyph,          sbYield));                  //Sam 20171213 (Steven) : 超豐良率監控
    mySecurityPal.push_back(new TMySecurity("[155] Setup - 12/16 Site Direct Heater Layout",        fMain->sbSetup->Glyph,          sbSetup));                  //JerryYang 20180416 (Steven) : MicroChip Philippine要求加入權限控管
    mySecurityPal.push_back(new TMySecurity("[156] Other - Pass Download MOFile",                   fMain->sbStartMode->Glyph,      sbOther));                  //Sam 20180416 (wei) : 超豐新增可以設定權限，權限以上，不用下載 MOFile 設定檔就可以按『Start』測試。
    mySecurityPal.push_back(new TMySecurity("[157] Other - Check Online/Real Status",               fMain->sbStartMode->Glyph,      sbOther));                  //Sam 20180416 (wei) : 超豐新增可以設定權限，權限以下，按『Start』時前檢查 Tester 必須為 『Online』，和 Real/Dummy 必須為 『Normal』。
    mySecurityPal.push_back(new TMySecurity("[158] Temp.OffSet - Active ATC Cooling",               fMain->sbTempOffset->Glyph,     sbTemp));                   //JerryYang 20181214 : 新增ATC權限管控
    mySecurityPal.push_back(new TMySecurity("[159] Setup - Socket sensor",                          fMain->sbSetup->Glyph,          sbSetup));                  //Steven 20191129 : Socket sensor加入權限控制
    mySecurityPal.push_back(new TMySecurity("[160] Other - Unloader Tray Edit",                     fMain->sbTrayForm->Glyph,       sbOther));                  //Steven 20191218 : Hana Micron要求unloaderTray編輯權限要不一樣
    mySecurityPal.push_back(new TMySecurity("[161] Tools - Barcode Sub-Function",                   fMain->sbBarCode->Glyph,        sbTools));                  //Steven 20200225 : SCK想要獨立控管部分2DID功能
    mySecurityPal.push_back(new TMySecurity("[162] Yield - Break ART",                              fMain->spbAutoRetest->Glyph,    sbYield));                  //Sam 20202015 : TCP ART 增加 OneCycle 可手動強制中斷流程
    mySecurityPal.push_back(new TMySecurity("[163] Contact Count Alarm Edit Permission",            DefaultImg,                     sbOther));                  //Ifor 20200114 add:Life Time Edit Permission
    mySecurityPal.push_back(new TMySecurity("[164] Other - Server on/off line",                     DefaultImg,                     sbOther));                  //Steven 20200629 : Murata要求可以按按鈕後停止Server功能
    mySecurityPal.push_back(new TMySecurity("[165] Setup - Arm 1 pick, arm 2 test function",        fMain->sbSetup->Glyph,          sbSetup));
    mySecurityPal.push_back(new TMySecurity("[166] Other - Level for continuous alarm [O17]",       fMain->sbConfiguration->Glyph,  sbOther));                  //Steven 20210127 : 逸昌要求在單位時間內相同Alarm發生多次,提昇解除alarm權限
    mySecurityPal.push_back(new TMySecurity("[167] Setup - Enable auto site mapping function",      fMain->sbSetup->Glyph,          sbSetup));                  //Isaac 20210714 : JECT要求獨立權限設定
    mySecurityPal.push_back(new TMySecurity("[168] Other - Default Recipe Setting",                 DefaultImg,                     sbOther));                  //Sam 20210414 : Recipe Parameter Default 功能新增權限控制
    mySecurityPal.push_back(new TMySecurity("[169] Other - Default Recipe Reply",                   DefaultImg,                     sbOther));                  //Sam 20210414 : Recipe Parameter Default 功能新增權限控制
    mySecurityPal.push_back(new TMySecurity("[170] Tools - ScanAOI",                                fMain->sbCCD->Glyph,            sbTools));
    mySecurityPal.push_back(new TMySecurity("[171] Contact - Contact height",                       fMain->sbContact->Glyph,        sbContact));                //Sam 20220117 : 新增一組只鎖 Contact height 的權限
    mySecurityPal.push_back(new TMySecurity("[172] Main - Disable Site Map",                        fMain->spbAutoRetest->Glyph,    sbMain));                   //Steven 20221228 : for 主畫面的cbDisableSiteMappingCheck
    mySecurityPal.push_back(new TMySecurity("[173] Contact - Release height",                       fMain->sbContact->Glyph,        sbContact));                //Sam 20221207 : 新增一組只鎖 Release height 的權限
    mySecurityPal.push_back(new TMySecurity("[174] Tools - Magazine",                               fMain->sbMagazine->Glyph,       sbTools));                  //JerryYang 20220909 : add magazine
    mySecurityPal.push_back(new TMySecurity("[175] Temp.OffSet - Kit Temp. offset",                 fMain->sbTempOffset->Glyph,     sbTemp));                   //JerryYang 20221005 : 新增溫度5點offset by recipe儲存
    mySecurityPal.push_back(new TMySecurity("[176] Contact - RTC Auto Tuning",                      fMain->sbContact->Glyph,        sbContact));                //Sam 20240711 : RTC Auto Tuning
    mySecurityPal.push_back(new TMySecurity("[177] [I49] - Offline clean out all ic",               fMain->spbAutoRetest->Glyph,    sbYield));                  //Sam 20240701 : Offline Clean out ic 新增權限設定
    mySecurityPal.push_back(new TMySecurity("[178] Tools - Tray Function",                          fMain->spbTrayMapping->Glyph,   sbTools));                  //Sam 20220117 : 新增一組只鎖 Contact height 的權限
    //目前到256, 超過時候要去修改AccessLevel的陣列大小----------------------------------------------

    iMaxLevelItem=mySecurityPal.size();
    for(int i=0; i<iMaxLevelItem; i++)
    {
        mySecurityPal[i]->SetVisible(true);
    }

    GetLevelSet();                                                              //Steven 20140222 : 存取LevelSet檔案改為Function
    FileNameJam000="D:\\HT9045\\Error\\English\\JAM0000.dat";
    if(FileExists(FileNameJam000)==false)                                       //Steven 20140222 End: Alarm Code設定權限
    {
        for(int i=0; i<cbJamArea->Items->Count; i++)
        {
            cbJamArea->ItemIndex=i;
            cbJamArea->Refresh();
            GetJameCodeOfAxis(cbJamArea->ItemIndex+1, cbJamCode);

            for(int j=0; j<cbJamCode->Items->Count; j++)
            {
                cbJamCode->ItemIndex=j;
                cbJamCode->Refresh();
                cbJamLang->ItemIndex=0;
                cbJamLang->Refresh();
                JamArea=cbJamArea->Text;
                JamCode=cbJamCode->Text.SubString(1, cbJamCode->Text.AnsiPos("  :")-1);
                ChangeJamMessage(false);
                ChangeJamMessage();
            }
        }
    }
    AddAlarmList();                                                             //jou 20171201 (Steven) : 新增統計jam code alarm次數,達到設定數量後提高一階權限才能解開alarm
}
//---------------------------------------------------------------------------
void __fastcall TfSecurity::FormDestroy(TObject *Sender)
{
    try
    {
        for(vector<TMySecurity *>::iterator iter=mySecurityPal.begin(); iter!=mySecurityPal.end(); ++iter)
        {
            delete *iter;
        }
        vec_clr(mySecurityPal);
        DefaultImg=NULL;
        delete DefaultImg;                                                      //Steven 20160108 : release memory
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfSecurity::FormDestroy");
    }
    LogSoftwareOffTime("TfSecurity, FormDestroy");                              //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void __fastcall TfSecurity::FormShow(TObject *Sender)
{
    GetLevelSet();                                                              //Steven 20140222 : 存取LevelSet檔案改為Function

    for(int i=0; i<iMaxLevelItem; i++)
    {
        if(CUSTOMER_CODE==CC_KYEC_LEE &&
           (i==35  ||                                                           //Ifor 20160914 京元喬智要求 Trouble Shooting強制設定為2不可修改
            i==104 ||                                                           //Tray Edit
            i==114 ||                                                           //Ifor 20170203 (Steven) 京元喬智要求ATC Control強制設定為2不可修改
            i==128))                                                            //Ifor 20160825 add PE模式強制設定為2不可修改
        {
            mySecurityPal[i]->SetEnabled(false);
        }
        else if(i==163)                                                         //Contact Count Alarm Edit Permission
        {
            mySecurityPal[i]->SetEnabled(false);
        }
        mySecurityPal[i]->SetLevel(LevelSet.AccessLevel[i]);
    }

    Left=50;                                                                    //Steven 20091103
    Top=10;                                                                     //Steven 20091103

    #ifndef SOFT_SIMULTE
    SecurityPalVisible();                                                       //Steven 20250430 : 包起來
    #endif

    cbUnlockPassWord->Visible   =CosFunction.bUseAlarmUnlockPassWord;           //Ifor 20170214 (wei) add 解除Alarm 需要獨立密碼
    cbIncludeMTBA->Visible      =CosFunction.bIncludeMTBA;                      //JerryYang 20180619 (wei) : 新增可自定義Jam code是否列入MTBA計算
    chkCheckContAlarm->Visible  =CosFunction.bConAlarmNeedKeyInPassword;        //Steven 20200513 : 連續alarm輸入密碼的alarm要可以自訂義
    chkO17->Visible             =CosFunction.bConAlarmInTimeLevelUp;            //Steven 20210127 : 逸昌要求在單位時間內相同Alarm發生多次,提昇解除alarm權限

    cbAddAlarmLog->Visible         =CUSTOMER_CODE==CC_PTI;                      //Sam 20210611 : Alarm Log 可以自定義哪個需要記 Log 上報

    cbN27AlarmSel->Visible          =CosFunction.bUseAlarmLogXml;
    cbN27AlarmSelByArea->Visible    =CosFunction.bUseAlarmLogXml;
    cbN27AddBoard->Visible          =(CosFunction.bUseAlarmLogXml &&
                                      CUSTOMER_CODE==CC_SIGURD_ChungXing);      //Sam 20210911 :  矽格中興廠 Alarm Message Board 是否顯示增加開關
    chkTCPAlarm->Visible            =CosFunction.bEnableHandlerResultServer;    //Sam 20230426 : 通知系統 Handler 已經密碼鎖定
    cbContAlarmNotUpload->Visible   =CosFunction.bOLPFunction;                  //Sam 20231116 : 連續 Alarm 不要上傳伺服器
    if(CosFunction.bSecurityHave5Level==true)                                   //jou 2014-06-19 Security Have 5 Level
    {
        if(rgJamLevel->Columns<=4)
        {
            rgJamLevel->Columns=5;
            if(CUSTOMER_CODE==CC_KYEC_LEE)                                      //wei 20160505 增加PE權限
                rgJamLevel->Items->Insert(0, "Operator");
            else
                rgJamLevel->Items->Insert(0, "Open");
        }

        switch(AccessLevel)
        {
            case 4:                                                             //HonPrec
                btnHonPrec->Visible=true;
                sbSupervisor->Visible=true;
                sbEngineer->Visible=true;
                PageControl1->Visible=true;
                break;
            case 3:                                                             //Supervisor
                btnHonPrec->Visible=false;
                sbSupervisor->Visible=true;
                sbEngineer->Visible=true;

                if(IniConfig.bSPILFunction==true ||                             //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
                   CUSTOMER_CODE==CC_SCS)                                       //jou 2015-08-27 SCS 要求 Jam Level 要可以自己選擇
                    PageControl1->Visible=false;
                else
                    PageControl1->Visible=true;
                break;
            case 2:                                                             //Engineer
                btnHonPrec->Visible=false;
                sbSupervisor->Visible=false;
                sbEngineer->Visible=true;
                PageControl1->Visible=false;
                break;
            case 1:                                                             //OP
            case 0:                                                             //Open
                btnHonPrec->Visible=false;
                sbSupervisor->Visible=false;
                sbEngineer->Visible=false;
                PageControl1->Visible=false;
                break;
        }
    }
    else
    {
        switch(AccessLevel)
        {
            case 3:                                                             //HonPrec
                btnHonPrec->Visible=true;
                sbSupervisor->Visible=true;
                sbEngineer->Visible=true;
                PageControl1->Visible=true;
                break;
            case 2:                                                             //Supervisor
                btnHonPrec->Visible=false;
                sbSupervisor->Visible=true;
                sbEngineer->Visible=true;

                if(IniConfig.bSPILFunction==true ||                             //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
                   CUSTOMER_CODE==CC_SCS)                                       //jou 2015-08-27 SCS 要求 Jam Level 要可以自己選擇
                    PageControl1->Visible=false;
                else
                    PageControl1->Visible=true;
                break;
            case 1:                                                             //Engineer
                btnHonPrec->Visible=false;
                sbSupervisor->Visible=false;
                sbEngineer->Visible=true;
                PageControl1->Visible=false;
                break;
            case 0:                                                             //OP
                btnHonPrec->Visible=false;
                sbSupervisor->Visible=false;
                sbEngineer->Visible=false;
                PageControl1->Visible=false;
                break;
        }
    }

    PageControl1->ActivePageIndex=0;
    int iPitch=52, iStart=2;                                                    //重新排列位置 Start
    for(int iP=0; iP<PageControl1->ControlCount; iP++)                          //找出所有TabSheet
    {
        TControl *P=PageControl1->Controls[iP];
        TTabSheet *TabPtr=dynamic_cast <TTabSheet *>(P);
        if(TabPtr!=NULL)
        {
            for(int j=0; j<TabPtr->ControlCount; j++)                           //找出TabSheet裡面的ScrollBox
            {
                TControl *S=TabPtr->Controls[j];
                TScrollBox *sbPtr=dynamic_cast <TScrollBox *>(S);
                if(sbPtr!=NULL)                                                 //找出所有ScrollBox中的所有Panel並排序
                {
                    sbPtr->VertScrollBar->Position=0;                           //jou 2012-11-07 修正畫面顯示問題
                    iStart=2;
                    for(int k=0; k<sbPtr->ControlCount; k++)
                    {
                        TControl *pal=sbPtr->Controls[k];
                        TPanel *palPtr=dynamic_cast <TPanel *>(pal);
                        if(palPtr!=NULL)
                        {
                            if(palPtr->Visible==true)
                            {
                                palPtr->Top=iStart;
                                iStart+=iPitch;
                            }
                        }
                    }
                }
            }
        }
    }

    cbJamNeedRed->Visible=(IniConfig.bAlarmMustRedColor==true);                 //Steven 20111116 : 特殊Alarm需要改紅底
    cbJamArea->ItemIndex=0;                                                     //Steven 20140222 Start: Alarm Code設定權限
    cbJamArea->Refresh();
    GetJameCodeOfAxis(cbJamArea->ItemIndex+1, cbJamCode);
    cbJamCode->ItemIndex=0;
    cbJamCode->Refresh();
    cbJamLang->ItemIndex=0;
    cbJamLang->Refresh();
    JamArea=cbJamArea->Text;
    JamCode=cbJamCode->Text.SubString(1, cbJamCode->Text.AnsiPos("  :")-1);
    ChangeJamMessage(false);

    btnOperator->Visible=CosFunction.bSecurityHave5Level;                       //jou 2014-06-19 Security Have 5 Level
    rgMachineStatusBit8->Visible=(CUSTOMER_CODE!=CC_SCK);
    chkAlarmAfterFullTray->Visible=(CosFunction.bNeedAlarmAfterUnloaderFull &&
                                    cbJamArea->Text=="02 Output Arm");          //Jimmychiu 20240902 : Need Alarm After Unloader Full
    fShow=true;

    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                    //Steven 20100629
}
//---------------------------------------------------------------------------
void __fastcall TfSecurity::FormClose(TObject *Sender, TCloseAction &Action)
{
    for(int i=0; i<iMaxLevelItem; i++)
    {
        LevelSet.AccessLevel[i]=mySecurityPal[i]->GetLevel();
        mySecurityPal[i]->SetEnabled(false);                                    //wei 20150803 消除選擇虛線
        mySecurityPal[i]->SetEnabled(true);
    }

    if(LevelSet.AccessLevel[87]<iDefSupervisorLevel)                            //Steven 20120830 : Teaching按鈕權限
        LevelSet.AccessLevel[87]=iDefSupervisorLevel;

    if(LevelSet.AccessLevel[129]>LevelSet.AccessLevel[130])                     //Steven 20161201 : For SCK 93K ART
        LevelSet.AccessLevel[129]=LevelSet.AccessLevel[130];

    if(CUSTOMER_CODE!=CC_SIGURD_PeiXing)                                        //Alick 20160901 modify for 矽格北興
    {
        if(LevelSet.AccessLevel[86]<iDefEngineerLevel)                          //Steven 20120830 : Motion View按鈕權限
            LevelSet.AccessLevel[86]=iDefEngineerLevel;
    }

    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                    //Steven 20100629

    SaveJamLevel();
    SavePassword();
    SetLevelSet();                                                              //Steven 20140222 : 存取LevelSet檔案改為Function

    ReadPassword();
    fShow=false;
}
//---------------------------------------------------------------------------
void TfSecurity::SecurityPalVisible()
{
    mySecurityPal[ 9]->SetVisible(true);                                        //kevin 20180917
    mySecurityPal[27]->SetVisible(CUSTOMER_CODE==CC_Greatek);                   //JimmyChiu 20211228 : InterFace Type
    mySecurityPal[33]->SetVisible(false);                                       //沒用到隱藏
    mySecurityPal[34]->SetVisible(false);                                       //沒用到隱藏
    mySecurityPal[35]->SetVisible(IniConfig.bSPILFunction==false &&             //Steven 20140430 : 矽品要求全自定  //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
                                  CUSTOMER_CODE!=CC_SCS);                       //jou 2015-08-27 SCS 要求 Jam Level 要可以自己選擇
    mySecurityPal[36]->SetVisible(false);                                       //沒用到隱藏
    mySecurityPal[37]->SetVisible(IniConfig.bEnableCCDUSETCPIP ||               //CCD
                                  REAL_TIME_CCD ||                              //RTC權限
                                  USE_Scanner_AOI_Inspection ||                 //Ifor 20200813 add: Scal AOI 權限
                                  USE_Top_Scanner_AOI_Inspection);              //Ifor 20200902 add: TFAMD Top AOI
    mySecurityPal[40]->SetVisible(IniConfig.bShuttleModeAccseeLevel);           //jou 2012-01-30 Yuedong Chen [Yuedong.Chen@amkor.com] //請將Setup裡面的Shuttle mode在password control單獨弄一個level，類似之前修改的contact force
    mySecurityPal[41]->SetVisible(CosFunction.bSetupFileNameControlByLevel);    //JerryYang 20160427 add 矽格北興    //Steven 20131108 Add Singapore  //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction  //Steven 20150505 : Add SPIL_CHINA_SUZHOU 要求Setup File更換權限 //jou 20171011 (wei) : bSetupFileName Control By Level
    mySecurityPal[42]->SetVisible(IniConfig.bEnable_SECS_GEM);
    mySecurityPal[43]->SetVisible(IniConfig.bEnableAutoCleanFunction);
    mySecurityPal[44]->SetVisible((ATC_SYSTEM==eATCHonPrecType));
    mySecurityPal[45]->SetVisible(SHUTTLE_SENSOR_TYPE==eSensorCCLink ||
                                  SHUTTLE_SENSOR_TYPE==eSensorCCLink3 ||        //Steven 20131008 : for HT9046AH
                                  UseCanBusOrEtherCAT());                       //Sam 20230707 : EtherCAT Shuttle sensor //Steven 20191126 : 補上Canbus
    mySecurityPal[46]->SetVisible((SYN_TEK_MOTION_MODULE==G9004_M204));
    mySecurityPal[47]->SetVisible((USE_16_HEATER==eht16HeaterEJ1N ||
                                   USE_16_HEATER==eht32HeaterEJ1N ||
                                   USE_16_HEATER==eht16HeaterDTME08 ||
                                   USE_16_HEATER==eht32HeaterDTME08));          //Steven 20140923 : Index使用EJ1N版32組加熱器
    mySecurityPal[48]->SetVisible(INSTALL_OCR);                                 //Steven 20120716 : OCR
    mySecurityPal[49]->SetVisible(CosFunction.bAutoKTemp);                      //Steven 20120719 : 自動K溫
    mySecurityPal[80]->SetVisible(IniConfig.bC04EnableTestTempIC);              //Steven 20120807 : 動態溫度
    mySecurityPal[83]->SetVisible(CUSTOMER_CODE!=CC_ASE_CL);                    //Alick 20160616 ASE CL時隱藏
    mySecurityPal[84]->SetVisible(false);                                       //沒用到隱藏
    mySecurityPal[85]->SetVisible(IniConfig.bQAMode);                           //Steven 20121016 : QA Mode
    mySecurityPal[88]->SetVisible(BAR_CODE_INSTALL!=ebctUninstall);             //Steven 20121009 : Bar Code
    mySecurityPal[89]->SetVisible(USE_ROTATE_KIT==1);                           //Steven 20130626 : Rotate權限
    mySecurityPal[90]->SetVisible(USE_AIR_CONDITIONER==2);                      //Steven 20131011 : 冷氣機
    mySecurityPal[95]->SetVisible(false);                                       //Steven 20140428 : 重複的砍掉
    mySecurityPal[96]->SetVisible(CUSTOMER_CODE==CC_SCK || CUSTOMER_CODE==CC_ASE_CL);
    mySecurityPal[97]->SetVisible(IniConfig.bEnableAutoCleanFunction);          //Steven 20140519
    mySecurityPal[98]->SetVisible(IniConfig.bQAMode);                           //Steven 20121016 : QA Mode
    mySecurityPal[99]->SetVisible(IniConfig.bSocketCommunication);
    mySecurityPal[100]->SetVisible(USE_LASER_DISTANCE);
    mySecurityPal[116]->SetVisible(CosFunction.bKnockerSetBySetupFile);         //Steven 20160329 : 敲擊汽缸參數調整可搭配工作檔處理
    mySecurityPal[110]->SetVisible(CosFunction.bUsePMAlarmFunction);            //wei 20160225 PMAlarmFunction
    mySecurityPal[111]->SetVisible(CosFunction.bUsePMAlarmFunction);            //wei 20160225 PMAlarmFunction
    mySecurityPal[112]->SetVisible(CosFunction.bUsePMAlarmFunction);            //wei 20160225 PMAlarmFunction
    mySecurityPal[113]->SetVisible(CosFunction.bUsePMAlarmFunction);            //wei 20160225 PMAlarmFunction
    mySecurityPal[117]->SetVisible(CosFunction.bYieldAlarm4);                   //wei 20160406 Yield Alarm4
    mySecurityPal[118]->SetVisible(CUSTOMER_CODE==CC_Greatek);                  //wei 20160406 Yield Alarm4
    mySecurityPal[119]->SetVisible(CUSTOMER_CODE==CC_Greatek);                  //wei 20160406 Yield Alarm4
    mySecurityPal[120]->SetVisible(CUSTOMER_CODE==CC_Greatek);                  //wei 20160406 Yield Alarm4
    mySecurityPal[121]->SetVisible(CosFunction.bYieldAlarm4);                   //Sam 20191109 YieldAlarm4 Fix //wei 20160406 Yield Alarm4
    mySecurityPal[122]->SetVisible(CUSTOMER_CODE==CC_Greatek);                  //wei 20160406 Yield Alarm4
    mySecurityPal[125]->SetVisible(CUSTOMER_CODE==CC_ASE_CL);                   //Alick 20160616 ASE CL 將啟用功能與計數分開,125:ON/OFF 126:Count
    mySecurityPal[126]->SetVisible(CUSTOMER_CODE==CC_ASE_CL);                   //Alick 20160616 ASE CL 將啟用功能與計數分開,125:ON/OFF 126:Count
    mySecurityPal[128]->SetVisible(CosFunction.bUsePEModelFunction);            //Ifor 20160822 PE Model 權限設定顯示 By CosFunction
    mySecurityPal[129]->SetVisible(CosFunction.bUseSCKART &&
                                   USE_AUTO_RETEST==eartInstall &&
                                   IniConfig.bA10_AutoReTest);                  //Steven 20161201 : For SCK 93K ART
    mySecurityPal[130]->SetVisible(CosFunction.bUseSCKART &&
                                   USE_AUTO_RETEST==eartInstall &&
                                   IniConfig.bA10_AutoReTest);                  //Steven 20161201 : For SCK 93K ART
    mySecurityPal[131]->SetVisible(CUSTOMER_CODE==CC_ASE_CL);                   //JerryYang 20170303 (wei) ASE_CL權限控制
    mySecurityPal[132]->SetVisible(CUSTOMER_CODE==CC_ASE_CL);                   //JerryYang 20170303 (wei) ASE_CL權限控制
    mySecurityPal[133]->SetVisible(CUSTOMER_CODE==CC_ASE_CL);                   //JerryYang 20170303 (wei) ASE_CL權限控制
    mySecurityPal[134]->SetVisible(CUSTOMER_CODE==CC_ASE_CL);                   //JerryYang 20170303 (wei) ASE_CL權限控制
    mySecurityPal[135]->SetVisible(CUSTOMER_CODE==CC_ASE_CL);                   //JerryYang 20170303 (wei) ASE_CL權限控制
    mySecurityPal[136]->SetVisible(CUSTOMER_CODE==CC_ASE_CL);                   //JerryYang 20170303 (wei) ASE_CL權限控制
    mySecurityPal[137]->SetVisible(CUSTOMER_CODE==CC_ASE_CL);                   //JerryYang 20170303 (wei) ASE_CL權限控制
    mySecurityPal[138]->SetVisible(CUSTOMER_CODE==CC_ASE_CL);                   //JerryYang 20170303 (wei) ASE_CL權限控制
    mySecurityPal[139]->SetVisible(CUSTOMER_CODE==CC_ASE_CL);                   //JerryYang 20170303 (wei) ASE_CL權限控制
    mySecurityPal[140]->SetVisible(CUSTOMER_CODE==CC_ASE_CL);                   //JerryYang 20170303 (wei) ASE_CL權限控制
    mySecurityPal[141]->SetVisible(CUSTOMER_CODE==CC_ASE_CL);                   //JerryYang 20170303 (wei) ASE_CL權限控制
    mySecurityPal[142]->SetVisible(CUSTOMER_CODE==CC_ASE_CL);                   //JerryYang 20170303 (wei) ASE_CL權限控制
    mySecurityPal[143]->SetVisible(CUSTOMER_CODE==CC_ASE_CL);                   //JerryYang 20170303 (wei) ASE_CL權限控制
    mySecurityPal[144]->SetVisible(CUSTOMER_CODE==CC_ASE_CL);                   //JerryYang 20170303 (wei) ASE_CL權限控制
    mySecurityPal[145]->SetVisible(CUSTOMER_CODE==CC_ASE_CL);                   //JerryYang 20170303 (wei) ASE_CL權限控制
    mySecurityPal[146]->SetVisible(CUSTOMER_CODE==CC_ASE_CL);                   //JerryYang 20170303 (wei) ASE_CL權限控制
    mySecurityPal[147]->SetVisible(CUSTOMER_CODE==CC_ASE_CL);                   //Isaac 20170327 (jou) : AutoSkip權限控制
    mySecurityPal[148]->SetVisible(CosFunction.bUseYieldControlFunction);       //Steven 20170603 (wei) : Add for Korea //Ifor 20180731 (wei) : IniConfig.bKoreaFunction => CosFunction.bUseYieldControlFunction
    mySecurityPal[149]->SetVisible(IniConfig.bKoreaFunction);                   //Steven 20170603 (wei) : Add for Korea
    mySecurityPal[150]->SetVisible(CosFunction.bRecipeParameterDefault);        //Isaac 20170630 (Steven) Recipe Parameter Default權限控制
    mySecurityPal[151]->SetVisible(IniConfig.bEnableErms);                      //Steven 20170829 (wei) : ERMS Selsection新增權限控管
    mySecurityPal[152]->SetVisible(WEIGHT_CALIBRATION ||                        //Steven 20220208 : 壓力校正的權限
                                   CosFunction.bUseDynamicKitDiameter);         //JerryYang 20171102 (wei) EP壓力對應電壓校正權限
    mySecurityPal[158]->SetVisible(CUSTOMER_CODE==CC_Microchip_Phil);           //JerryYang 20181214 : 新增ATC權限管控
    mySecurityPal[159]->SetVisible(IniConfig.bC08_SocketSensor);                //Steven 20191129 : Socket sensor加入權限控制
    mySecurityPal[160]->SetVisible(CosFunction.bUnloaderEditTrayLevelSet);      //Steven 20191218 : Hana Micron要求unloaderTray編輯權限要不一樣 //Steven 20191224 : Unloader編輯Tray改用另外一組權限
    mySecurityPal[161]->SetVisible(CUSTOMER_CODE==CC_SCK);                      //Steven 20200225 : SCK想要獨立控管部分2DID功能
    mySecurityPal[162]->SetVisible(CUSTOMER_CODE==CC_TERAPOWER &&
                                   CosFunction.bUseSCKART &&
                                   USE_AUTO_RETEST==eartInstall &&
                                   IniConfig.bA10_AutoReTest);                  //Sam 20202015 : TCP ART 增加 OneCycle 可手動強制中斷流程
    mySecurityPal[163]->SetVisible(CosFunction.bUseHeadContactCount);           //Ifor 20200114 add:Life Time Edit Permission
    mySecurityPal[164]->SetVisible(CUSTOMER_CODE==CC_Murata);                   //Steven 20200629 : Murata要求可以按按鈕後停止Server功能
    mySecurityPal[165]->SetVisible(IniConfig.bD58UseArm1PickPlaceArm2Test);
    mySecurityPal[166]->SetVisible(CosFunction.bConAlarmInTimeLevelUp);         //Steven 20210127 : 逸昌要求在單位時間內相同Alarm發生多次,提昇解除alarm權限
    mySecurityPal[167]->SetVisible(CosFunction.bI21EnableASMByRecipe);          //Isaac 20210714 : JECT要求獨立權限設定
    mySecurityPal[168]->SetVisible(CosFunction.bRecipeParameterDefault);        //Sam 20210414 : Recipe Parameter Default 功能新增權限控制
    mySecurityPal[169]->SetVisible(CosFunction.bRecipeParameterDefault);        //Sam 20210414 : Recipe Parameter Default 功能新增權限控制
    mySecurityPal[170]->SetVisible(USE_AOI_Inspection ||
                                   USE_Scanner_AOI_Inspection);                 //CCD || RTC權限
}
//---------------------------------------------------------------------------
bool __fastcall TfSecurity::Insufficient(int iType, bool bAlarm)
{
    if(iType==-1)                                                               //hontech權限
    {
        if(AccessLevel<iDefHonPrecLevel)                                        //jou 2014-06-19 Security Have 5 Level 3->iDefHonPrecLevel
        {
            return false;
        }
        return true;
    }

    if(iType<0)
        return false;
    if(iType>iMaxLevelItem)
        return false;

    if(AccessLevel<LevelSet.AccessLevel[iType])
    {
        if(bAlarm==true)
            ShowErrorMessage("WAR1676", 0, MMSystem);                           //Insufficient privileges

        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
void ChangePassword(int iLevel)
{
    AnsiString asLevel=iLevel;
    int iUseID=iLevel-1;
    bool bChange=false;

    TStringList *List, *ListChange;
    AnsiString str, asName, asLevel1, asPass, asList;
    char cStr[256];
    char dest[20];

    if(FileExists(pwPath))                                                      //2012-01-03    Dell modify
    {
        if(IniConfig.bPasswordSecret)                                           //jou 2013-01-04 Password Txt 加密
        {
            bChange=CheckAndReadIniDataGeneral("Password", "Change", false);
            if(bChange==false)
            {
                ListChange=new TStringList;
                ListChange->Clear();
            }
        }

        List=new TStringList;
        List->Clear();
        List->LoadFromFile(pwPath);                                             //2012-01-03    Dell modify
        int index, icount;

        fLogin->labUserName->Visible=true;
        fLogin->edUserName->Visible=true;
        fLogin->rgLoginOption->Visible=true;
        fLogin->cbLoginUserName->Visible=true;
        fLogin->cbLoginUserName->Clear();
        for(int i=0; i<List->Count; i++)
        {
            strncpy(cStr, List->Strings[i].c_str(), sizeof(cStr));

            SplitStrByDotSpaceOnly(cStr, dest, 20);                             //Name
            asName=dest;
            SplitStrByDotSpaceOnly(cStr, dest, 20);                             //Level
            asLevel1=dest;
            if(atoi(dest)==atoi(asLevel.c_str()))
            {
                fLogin->cbLoginUserName->Items->Add(asName);
            }

            if(IniConfig.bPasswordSecret && bChange==false)                     //jou 2013-01-04 Password Txt 加密
            {
                SplitStrByDotSpaceOnly(cStr, dest, 20);                         //Password
                asPass=dest;
                asPass=EncodeStr(asPass);

                asList.sprintf("%s %s %s", asName, asLevel1, asPass);
                ListChange->Add(asList);
            }
        }

        if(IniConfig.bPasswordSecret && bChange==false)                         //jou 2013-01-04 Password Txt 加密
        {
            WriteIniDataGeneral("Password","Change",true);
            List->Clear();                                                      //Ifor 20170603 (wei) TStringList 刪除前先 Clean
            delete List;
            ListChange->SaveToFile(pwPath);
            ListChange->Clear();                                                //Ifor 20170603 (wei) TStringList 刪除前先 Clean
            delete ListChange;
            ShowMyMessage("Password Secret finish!!");
            return;
        }

        fLogin->ShowModal();
        fLogin->labUserName->Visible=false;
        fLogin->edUserName->Visible=false;
        fLogin->rgLoginOption->Visible=false;
        fLogin->cbLoginUserName->Visible=false;
        fLogin->edUserName->Text=fLogin->edUserName->Text.Trim();
        fLogin->edLoginOldPassword->Text=fLogin->edLoginOldPassword->Text.Trim();

        if(IniConfig.bPasswordSecret && bChange==false)
        {
            ListChange->Clear();                                                //Ifor 20170603 (wei) TStringList 刪除前先 Clean
            delete ListChange;                                                  //Steven 20160912 : Add delete for save memory
        }

        if(fLogin->edUserName->Text.IsEmpty())
        {
            ShowErrorMessage("WAR1678", 0, MMSystem);                           //User Name or Password need KeyIn
            List->Clear();                                                      //Ifor 20170603 (wei) TStringList 刪除前先 Clean
            delete List;                                                        //Steven 20160912 : Add delete for save memory
            return;
        }
        else if(fLogin->edLoginOldPassword->Text.IsEmpty())
        {
            if(fLogin->rgLoginOption->ItemIndex !=1)                            //2012-01-03    Dell modify 刪除不需要password
            {
                ShowErrorMessage("WAR1678", 0, MMSystem);                       //User Name or Password need KeyIn
                List->Clear();                                                  //Ifor 20170603 (wei) TStringList 刪除前先 Clean
                delete List;                                                    //Steven 20160912 : Add delete for save memory
                return;
            }
        }

        switch(fLogin->rgLoginOption->ItemIndex)
        {
            case 0:                                                             //New
                for(int i=0; i<List->Count; i++)
                {
                    index=List->Strings[i].Pos(fLogin->edUserName->Text);
                    if(index==1)
                    {
                        ShowErrorMessage("WAR1672", 0, MMSystem);               //User Name Already exists
                        List->Clear();                                          //Ifor 20170603 (wei) TStringList 刪除前先 Clean
                        delete List;                                            //Steven 20160912 : Add delete for save memory
                        return;
                    }
                }

                if(IniConfig.bPasswordSecret)                                   //jou 2013-01-04 Password Txt 加密
                {
                    asPass=EncodeStr(fLogin->edLoginOldPassword->Text);
                    str   =fLogin->edUserName->Text+" "+asLevel+" "+asPass;
                }
                else
                {
                    str   =fLogin->edUserName->Text+" "+asLevel+" "+fLogin->edLoginOldPassword->Text;
                }
                List->Add(str);
                ShowErrorMessage("MES1673", 0, MMSystem);                       //New User Finish
                break;
            case 1:                                                             //Delete
                str  =fLogin->edUserName->Text+" "+asLevel;                     //2012-01-03    Dell modify 刪除不需要password
                index=-1;
                for(int i=0; i<List->Count; i++)
                {
                    icount=List->Strings[i].Pos(str);
                    if(icount>0)
                    {
                        index=i;
                        break;
                    }
                }

                if(Application->MessageBox("Sure delete??", "", MB_YESNO+MB_ICONQUESTION+MB_TOPMOST)==IDNO)
                {
                    break;
                }

                if(index!=-1)
                {
                    List->Delete(index);
                    ShowErrorMessage("MES1674", 0, MMSystem);                   //Delete User Finish
                }
                else
                {
                    ShowErrorMessage("WAR1677", 0, MMSystem);                   //UserName or PassWord Error
                }
                break;
            case 2:                                                             //Edit
                if(IniConfig.bPasswordSecret)                                   //jou 2013-01-04 Password Txt 加密
                {
                    asPass=EncodeStr(fLogin->edLoginOldPassword->Text);
                    str   =fLogin->edUserName->Text+" "+asLevel+" "+asPass;
                }
                else
                {
                    str   =fLogin->edUserName->Text+" "+asLevel+" "+fLogin->edLoginOldPassword->Text;
                }

                index=List->IndexOf(str);
                if(index!=-1)
                {
                    List->Delete(index);

                    if(IniConfig.bPasswordSecret)                               //jou 2013-01-04 Password Txt 加密
                    {
                        asPass=EncodeStr(fLogin->edLoginNewPassword->Text);
                        str   =fLogin->edUserName->Text+" "+asLevel+" "+asPass;
                    }
                    else
                    {
                        str   =fLogin->edUserName->Text+" "+asLevel+" "+fLogin->edLoginOldPassword->Text;
                    }
                    List->Add(str);
                    ShowErrorMessage("MES1675", 0, MMSystem);                   //Modify User Finish
                }
                else
                {
                    ShowErrorMessage("WAR1677", 0, MMSystem);                   //UserName or PassWord Error
                }
                break;
        }

        List->SaveToFile(pwPath);                                               //2012-01-03    Dell modify
        List->Clear();                                                          //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete List;
    }
    else
    {
        if(iLevel>=iDefHonPrecLevel)                                            //jou 2014-06-19 Security Have 5 Level 3->iDefHonPrecLevel
            return;

        fLogin->ShowModal();

        if(fLogin->edLoginNewPassword->Text.IsEmpty())                          //jou 2014-07-06 修正未輸入密碼就存檔錯誤
        {
            ShowErrorMessage("WAR1678", 0, MMSystem);                           //User Name or Password need KeyIn
            return;
        }

        strncpy(USER.ID[iUseID]      , fLogin->edUserName->Text.c_str(), sizeof(USER.ID[iUseID]));
        strncpy(USER.PassWord[iUseID], fLogin->edLoginNewPassword->Text.c_str(), sizeof(USER.PassWord[iUseID]));        //Steven 20120608 : edLoginOldPassword --> edLoginNewPassword
        SavePassword();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfSecurity::SecurityExitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
__fastcall TMySecurity::TMySecurity(AnsiString Caption, Graphics::TBitmap *Img, TWinControl *Sender) : TComponent(Owner)
{
    Panel=new TPanel(this);
    Panel->Left     =0;
    Panel->Top      =0;
    Panel->Height   =50;
    Panel->Color    =TColor(0x00C2B8A6);
    Panel->BevelInner=bvNone;
    Panel->BevelOuter=bvNone;
    RadioGroup=new TRadioGroup(this);
    RadioGroup->Parent=Panel;
    SpeedButton=new TSpeedButton(this);
    SpeedButton->Parent=Panel;
    SpeedButton->Glyph=Img;
    AnsiString s="";
    int iSt=0, iEd=0,iSn=0;
    s=Caption;
    if(Caption.Length()>6)                                                      //Sam 20250827 : 修正 Change Log 亂存問題
    {
        iSt=s.Pos("[");
        iEd=s.Pos("]");
        if(iSt>0 && iEd>0)
        {
            s=s.SubString(iSt+1,iEd-2);
            iSn=atoi(s.c_str());
        }
    }
    else
    {
        iSn=0;
    }
    SetCaption(Caption, iSn);
    SetParent(Sender);                                                          //Steven 20130611 : 權限設定改為分頁方式
    Panel->Width    =Panel->Parent->Width-50;
}
//---------------------------------------------------------------------------
__fastcall TMySecurity::~TMySecurity()
{
//    delete RadioGroup;
//    delete SpeedButton;
}
//---------------------------------------------------------------------------
void TMySecurity::SetParent(TWinControl *Sender)
{
    Panel->Parent=Sender;
    if(CosFunction.bSecurityHave5Level==true)                                   //jou 2014-06-19 Security Have 5 Level
    {
        RadioGroup->Columns=5;
        if(CUSTOMER_CODE==CC_KYEC_LEE)                                          //wei 20160505 增加PE權限
        {
            RadioGroup->Items->Add("Operator");
            RadioGroup->Items->Add("Engineer");
            RadioGroup->Items->Add("PEngineer");
            RadioGroup->Items->Add("Supervisor");
            RadioGroup->Items->Add("HonPrec");
        }
        else
        {
            RadioGroup->Items->Add("Open");
            RadioGroup->Items->Add("Operator");
            RadioGroup->Items->Add("Engineer");
            RadioGroup->Items->Add("Supervisor");
            RadioGroup->Items->Add("HonPrec");
        }
    }
    else
    {
        RadioGroup->Columns=4;
        RadioGroup->Items->Add("Operator");
        RadioGroup->Items->Add("Engineer");
        RadioGroup->Items->Add("Supervisor");
        RadioGroup->Items->Add("HonPrec");
    }
    RadioGroup->ItemIndex=0;
    RadioGroup->Width=458;
    RadioGroup->Height=42;
    RadioGroup->Top=1;
    RadioGroup->Left=310;
    SpeedButton->Width=300;
    SpeedButton->Height=42;
    SpeedButton->Top=4;
    SpeedButton->Left=7;
    SpeedButton->Layout=blGlyphLeft;
    SpeedButton->Margin=4;
}
//---------------------------------------------------------------------------
void TMySecurity::SetPosition(int Top, int Left)
{
    Panel->Top=Top;
    Panel->Left=Left;
}
//---------------------------------------------------------------------------
void TMySecurity::SetCaption(AnsiString Caption, int index)
{
    this->index=index;
    this->Caption=Caption;
    Panel->Name="MySecurity_Panel_"+AnsiString(index);
    Panel->Caption="";
    RadioGroup->Name="MySecurity_RadioGroup_"+AnsiString(index);
    RadioGroup->Caption="";
    SpeedButton->Name="MySecurity_SpeedButton_"+AnsiString(index);
    SpeedButton->Caption=Caption;
}
//---------------------------------------------------------------------------
AnsiString TMySecurity::GetCaption()
{
    return SpeedButton->Caption;
}
//---------------------------------------------------------------------------
void __fastcall TfSecurity::btnHonPrecClick(TObject *Sender)
{
    if(CosFunction.bSecurityHave5Level==true)                                   //jou 2014-06-19 Security Have 5 Level
        ChangePassword(4);
    else
        ChangePassword(3);
}
//---------------------------------------------------------------------------
void __fastcall TfSecurity::sbSupervisorClick(TObject *Sender)
{
    if(CosFunction.bSecurityHave5Level==true)                                   //jou 2014-06-19 Security Have 5 Level
        ChangePassword(3);
    else
        ChangePassword(2);
}
//---------------------------------------------------------------------------
void __fastcall TfSecurity::sbEngineerClick(TObject *Sender)
{
    if(CosFunction.bSecurityHave5Level==true)                                   //jou 2014-06-19 Security Have 5 Level
        ChangePassword(2);
    else
        ChangePassword(1);
}
//---------------------------------------------------------------------------
void __fastcall TfSecurity::cbJamAreaChange(TObject *Sender)
{
    GetJameCodeOfAxis(cbJamArea->ItemIndex+1, cbJamCode);
    cbJamCode->ItemIndex=0;
    cbJamCode->Refresh();
    cbJamLang->ItemIndex=0;
    cbJamLang->Refresh();
    ChangeJamMessage();
    AnsiString sJamArea=cbJamArea->Text;
    chkAlarmAfterFullTray->Visible=(CosFunction.bNeedAlarmAfterUnloaderFull &&
                                    sJamArea=="02 Output Arm");                 //Jimmychiu 20240902 : Need Alarm After Unloader Full
}
//---------------------------------------------------------------------------
void __fastcall TfSecurity::cbJamCodeChange(TObject *Sender)
{
    cbJamLang->ItemIndex=0;
    cbJamLang->Refresh();
    ChangeJamMessage();
}
//---------------------------------------------------------------------------
void __fastcall TfSecurity::cbJamLangChange(TObject *Sender)
{
    ChangeJamMessage();
}
//---------------------------------------------------------------------------
void TfSecurity::ChangeJamMessage(bool bSave)
{
    AnsiString FileName;

    if(bSave)
        SaveJamLevel();

    JamLang=cbJamLang->ItemIndex;
    JamArea=cbJamArea->Text;
    JamCode=cbJamCode->Text.SubString(1, cbJamCode->Text.AnsiPos("  :")-1);

    RichEditJamCode->Font->Charset=ANSI_CHARSET;
    if(JamLang==0)                                                              //English
    {
        FileName.sprintf("D:\\HT9045\\Error\\English\\%s.dat", JamCode);
        RichEditJamCode->Font->Name="Courier New";
    }
    else if(JamLang==1)                                                         //Chinese
    {
        FileName.sprintf("D:\\HT9045\\Error\\Chinese\\%s.dat", JamCode);
        RichEditJamCode->Font->Name="微軟正黑體";
    }
    else if(JamLang==2)                                                         //Korean
    {
        FileName.sprintf("D:\\HT9045\\Error\\Korea\\%s.dat", JamCode);
        RichEditJamCode->Font->Name="Batang";
        RichEditJamCode->Font->Charset=HANGEUL_CHARSET;
    }
    else if(JamLang==3)                                                         //Singapore
    {
        FileName.sprintf("D:\\HT9045\\Error\\Singapore\\%s.dat", JamCode);
        RichEditJamCode->Font->Name="微軟正黑體";
    }

    RichEditJamCode->Clear();
    if(FileExists(FileName))
        RichEditJamCode->Lines->LoadFromFile(FileName);

    cbJamNeedRed->Checked=GetJemRed(JamArea, JamCode);
    rgJamLevel->ItemIndex=GetJamLevel(JamArea, JamCode);
    cbSilentMode->Checked=GetJemSilent(JamArea, JamCode);                       //Steven 20150423 : SCK要求可以自訂Alarm是否要有蜂鳴器
    rgMachineStatusBit8->ItemIndex=GetBit8(JamArea, JamCode);                   //Isaac 20170922 (Steven) : ATP Machine Status Bit8 Issue

    if(CosFunction.bUseAlarmUnlockPassWord==true)                               //Ifor 20170214 (wei) add 解除Alarm 需要獨立密碼
        cbUnlockPassWord->Checked=GetJemUnlockPassWord(JamArea, JamCode);       //Ifor 20170214 (wei) add 可以自訂Alarm是否要解除密碼

    if(CosFunction.bIncludeMTBA==true)                                          //JerryYang 20180619 (wei) : 新增可自定義Jam code是否列入MTBA計算
        cbIncludeMTBA->Checked=GetJemIncludeMTBA(JamArea, JamCode);

    if(CosFunction.bUseAlarmLogXml)
    {
        cbN27AlarmSelByArea->Checked=GetN27AlarmSel(JamArea, "");
        cbN27AlarmSel->Checked=GetN27AlarmSel(JamArea, JamCode);
        cbN27AddBoard->Checked=GetN27AddBoard(JamArea, JamCode);                //Sam 20210911 :  矽格中興廠 Alarm Message Board 是否顯示增加開關
    }

    if(CosFunction.bOLPFunction)                                                //Sam 20231116 : 連續 Alarm 不要上傳伺服器
    {
        cbContAlarmNotUpload->Checked=GetContAlarmNotUpload(JamArea, JamCode);
    }

    if(CosFunction.bConAlarmNeedKeyInPassword)
        chkCheckContAlarm->Checked=GetJemContiAlarm(JamArea, JamCode);          //Steven 20200513 : 連續alarm輸入密碼的alarm要可以自訂義

    if(CosFunction.bConAlarmInTimeLevelUp)                                      //Steven 20210127 : 逸昌要求在單位時間內相同Alarm發生多次,提昇解除alarm權限
        chkO17->Checked=GetO17ContiAlarm(JamArea, JamCode);                     //Steven 20200513 : 連續alarm輸入密碼的alarm要可以自訂義

    if(CosFunction.bEnableHandlerResultServer)                                  //Sam 20230426 : 通知系統 Handler 已經密碼鎖定
        chkTCPAlarm->Checked=GetJemTCPAlarm(JamArea, JamCode);
    chkAlarmAfterFullTray->Checked=GetAlarmAfterUnloaderFull(JamArea, JamCode);                                         //Jimmychiu 20240902 : Need Alarm After Unloader Full

    if(CUSTOMER_CODE==CC_PTI)                                                   //Sam 20210611 : Alarm Log 可以自定義哪個需要記 Log 上報
    {
        if(JamCode.Pos("WAR")!=0 || JamCode.Pos("JAM")!=0)                      //Sam 20210823 :  JAM 也要自定義。
        {
            cbAddAlarmLog->Visible=true;
            cbAddAlarmLog->Checked=GetAddAlarmLog(JamArea, JamCode);
        }
        else
        {
            cbAddAlarmLog->Visible=false;
            cbAddAlarmLog->Checked=false;
        }
    }
}
//---------------------------------------------------------------------------
void TfSecurity::SaveJamLevel()                                                 //Steven 20140222 : Alarm Code需要的權限存檔
{
    AnsiString FileName;
    if(JamArea!="" && JamCode!="")
    {
        labMustCheck_35->Visible=false;
        if(IniConfig.bSPILFunction==true ||                                     //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction  //Steven 20140430 : 矽品要求全自定
           CUSTOMER_CODE==CC_SCS ||                                             //jou 2015-08-27 SCS 要求 Jam Level 要可以自己選擇
           CUSTOMER_CODE==CC_ASE_KaohSiung)                                     //kevin 20170413 (wei) add ASE_KH
        {
        }
        else if(CUSTOMER_CODE==CC_AMKOR_China ||
                CUSTOMER_CODE==CC_QUALCOMM)                                     //JerryYang 20170412 (Steven) add QUALCOMM
        {
            if(JamCode=="WAR07301" ||                                           //Socket consecutive failure
               JamCode=="WAR07321" ||                                           //Arm1: consecutive failure
               JamCode=="WAR07329" ||                                           //Arm2: consecutive failure
               JamCode=="JAM0508" || JamCode=="JAM0509")                        //Device lose at Output Shuttle
            {
                cbJamNeedRed->Checked=true;
                labMustCheck_35->Visible=true;
                if(rgJamLevel->ItemIndex<LevelSet.AccessLevel[35])
                {
                    rgJamLevel->ItemIndex=LevelSet.AccessLevel[35];
                }
            }
        }
        else if(CUSTOMER_CODE==CC_SCC)
        {
            if(JamCode=="WAR07301" ||                                           //Socket consecutive failure
               JamCode=="WAR07321" ||                                           //Arm1: consecutive failure
               JamCode=="WAR07329" ||                                           //Arm2: consecutive failure
               JamCode=="JAM0508"  || JamCode=="JAM0509" ||                     //kevin 20140612 可以設定 權限   //Device lose at Output Shuttle
               JamCode=="JAM0303"  || JamCode=="JAM0304" ||                     //Device drop error (Arm 1) & Device drop error (Arm 2)   //Jou 20140604 Add for SCC
               JamCode=="WAR0310"  ||                                           //Socket has IC error!                                    //Jou 20140604 Add for SCC
               JamCode=="JAM0314"  || JamCode=="JAM0315")                       //JerryYang 20160516 add JAM0314 JAM0315
            {
                cbJamNeedRed->Checked=true;
                labMustCheck_35->Visible=true;
                if(rgJamLevel->ItemIndex<LevelSet.AccessLevel[35])
                {
                    rgJamLevel->ItemIndex=LevelSet.AccessLevel[35];
                }
            }
        }
        else
        {
            if(JamCode=="JAM0203" || JamCode=="WAR0310" || JamCode=="WAR0346" ||
               JamCode=="WAR0343" || JamCode=="JAM0301" || JamCode=="JAM0302" ||
               JamCode=="WAR0701" || JamCode=="WAR0702" || JamCode=="WAR0703" || JamCode=="WAR0705" ||                  //Steven 20150709 : Add for Amkor China
               JamCode=="JAM0508" || JamCode=="JAM0509" || JamCode=="WAR07301" ||
               JamCode=="JAM0312" || JamCode=="JAM0313")                        //JerryYang 20160516 add JAM0312 JAM0313
            {
                labMustCheck_35->Visible=true;
                if(rgJamLevel->ItemIndex<LevelSet.AccessLevel[35])
                {
                    rgJamLevel->ItemIndex=LevelSet.AccessLevel[35];
                }
            }
        }
    }
    else
    {
        return;                                                                 //kevin 20190529 add  JamCode == NULL
    }
    WriteIniData(FileNameJam000, JamArea, JamCode+" Red", cbJamNeedRed->Checked);
    WriteIniData(FileNameJam000, JamArea, JamCode+" Silent", cbSilentMode->Checked);                                    //Steven 20150423 : SCK要求可以自訂Alarm是否要有蜂鳴器
    WriteIniData(FileNameJam000, JamArea, JamCode, rgJamLevel->ItemIndex);
    WriteIniData(FileNameJam000, JamArea, JamCode+" Bit8", rgMachineStatusBit8->ItemIndex);                             //Isaac 20170922 (Steven) : ATP Machine Status Bit8 Issue
    if(CosFunction.bUseAlarmUnlockPassWord==true)                               //Ifor 20170214 (wei) add 解除Alarm 需要獨立密碼
    {
        WriteIniData(FileNameJam000, JamArea, JamCode+" UnlockPassWord", cbUnlockPassWord->Checked);                    //Ifor 20170214 add 可以自訂Alarm是否要解除密碼
    }

    if(CosFunction.bIncludeMTBA==true)                                          //JerryYang 20180619 (wei) : 新增可自定義Jam code是否列入MTBA計算
    {
        if(CUSTOMER_CODE==CC_ASE_CL ||
           CUSTOMER_CODE==CC_TERAPOWER)
        {
            WriteIniData(FileNameJam000, JamArea, JamCode+" IncludeMTBF", cbIncludeMTBA->Checked);
        }
        else
        {
            WriteIniData(FileNameJam000, JamArea, JamCode+" IncludeMTBA", cbIncludeMTBA->Checked);
        }
    }

    if(CosFunction.bUseAlarmLogXml)
    {
        WriteIniData(FileNameJam000, JamArea, JamCode+" bN27AddBoard", cbN27AddBoard->Checked);                         //Sam 20210911 :  矽格中興廠 Alarm Message Board 是否顯示增加開關
        WriteIniData(FileNameJam000, JamArea, JamCode+" bN27AlarmSel", cbN27AlarmSel->Checked);
        WriteIniData(FileNameJam000, JamArea, "bN27AlarmSel", cbN27AlarmSelByArea->Checked);
    }

    if(CosFunction.bOLPFunction)                                                //Sam 20231116 : 連續 Alarm 不要上傳伺服器
    {
        WriteIniData(FileNameJam000, JamArea, JamCode+" ContAlarmNotUpload", cbContAlarmNotUpload->Checked);
    }

    if(CosFunction.bConAlarmNeedKeyInPassword && JamCode.Pos("JAM")>0)          //Steven 20200513 : 連續alarm輸入密碼的alarm要可以自訂義
    {
        WriteIniData(FileNameJam000, JamArea, JamCode+" ContiAlarm", chkCheckContAlarm->Checked);
    }

    if(CosFunction.bConAlarmInTimeLevelUp==true)                                //Steven 20210127 : 逸昌要求在單位時間內相同Alarm發生多次,提昇解除alarm權限
    {
        WriteIniData(FileNameJam000, JamArea, JamCode+" O17ContiAlarm", chkO17->Checked);
    }

    if(CUSTOMER_CODE==CC_PTI)                                                   //Sam 20210611 : Alarm Log 可以自定義哪個需要記 Log 上報
    {
         WriteIniData(FileNameJam000, JamArea, JamCode+" GetAddAlarmLog", cbAddAlarmLog->Checked);
    }

    if(CosFunction.bEnableHandlerResultServer)                                  //Sam 20230426 : 通知系統 Handler 已經密碼鎖定
    {
        WriteIniData(FileNameJam000, JamArea, JamCode+" TCPAlarm", chkTCPAlarm->Checked);
    }

    WriteIniData(FileNameJam000, JamArea, JamCode+" AlarmAfterUnloaderFull", chkAlarmAfterFullTray->Checked);           //Jimmychiu 20240902 : Need Alarm After Unloader Full

    if(JamLang==0)                                                              //English
    {
        FileName.sprintf("D:\\HT9045\\Error\\English\\%s.dat", JamCode);
    }
    else if(JamLang==1)                                                         //Chinese
    {
        FileName.sprintf("D:\\HT9045\\Error\\Chinese\\%s.dat", JamCode);
    }
    else if(JamLang==2)                                                         //Korean
    {
        FileName.sprintf("D:\\HT9045\\Error\\Korea\\%s.dat", JamCode);
    }
    else if(JamLang==3)                                                         //Singapore
    {
        FileName.sprintf("D:\\HT9045\\Error\\Singapore\\%s.dat", JamCode);
    }

    if(RichEditJamCode->Lines->Count!=0)
        RichEditJamCode->Lines->SaveToFile(FileName);
}
//---------------------------------------------------------------------------
int TfSecurity::GetJamLevel(AnsiString sJamArea, AnsiString sJamCode)           //Steven 20140222 : 取得Alarm Code需要的權限
{
    int iLevel=0;
    AnsiString asStr="";
    if(sJamArea!="" && sJamCode!="")
    {
        labMustCheck_35->Visible=false;
        asStr=CheckAndReadIniData(FileNameJam000, sJamArea, sJamCode, AnsiString("0"));
        iLevel=asStr.ToIntDef(0);                                               //jou 20240607 : 修正alarm權限設定異常
        if(CUSTOMER_CODE==CC_GIGAS && sJamCode=="JAM0201")                      //Richard 20220830 : 全智要求Jam0201 Operator可解掉
        {
            iLevel=0;
        }

//        if(IniConfig.bSPILFunction==true ||                                     //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction   //Steven 20140430 : 矽品要求全自定
//           CUSTOMER_CODE==CC_SCS ||                                             //jou 2015-08-27 SCS 要求 Jam Level 要可以自己選擇
        if(SPIL_FOR_QLE==1)                                                     //KevinCheng 20260521 : 渠梁jamCode權限
        {
        }
        else if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                     //kevin 20170413 (wei) add ASE_KH
        {
            if(sJamCode=="WAR0349" || sJamCode=="WAR16132")                     //JerryYang 20240111 : add  //JerryYang 20250423 : add WAR16132
            {
                if(iLevel<LevelSet.AccessLevel[35])
                {
                    iLevel=LevelSet.AccessLevel[35];
                }
            }
        }
        else if(CUSTOMER_CODE==CC_AMKOR_China ||                                //Steven 20150709 : Add for Amkor China
                CUSTOMER_CODE==CC_QUALCOMM)                                     //JerryYang 20170412 (Steven) add QUALCOMM
        {
            if(sJamCode=="WAR0701" || sJamCode=="JAM0301" ||
               sJamCode=="WAR0702" || sJamCode=="JAM0703" || JamCode=="WAR0705" ||
               sJamCode=="JAM0302" || sJamCode=="JAM0303" ||
               sJamCode=="JAM0304" || sJamCode=="JAM0305" ||
               sJamCode=="JAM0306" || sJamCode=="JAM0306" ||
               sJamCode=="WAR0310" || sJamCode=="WAR0343" ||
               sJamCode=="JAM0312" || sJamCode=="JAM0313" ||
               sJamCode=="JAM0314" || sJamCode=="JAM0315")                      //JerryYang 20160516 add JAM0312~0315
            {
                labMustCheck_35->Visible=true;
                if(iLevel<LevelSet.AccessLevel[35])
                {
                    iLevel=LevelSet.AccessLevel[35];
                }
            }
        }
        else if(CUSTOMER_CODE==CC_SCC)
        {
            if(sJamCode=="WAR07301" ||                                          //Socket consecutive failure
               sJamCode=="WAR07321" ||                                          //Arm1: consecutive failure
               sJamCode=="WAR07329" ||                                          //Arm2: consecutive failure
               sJamCode=="JAM0508"  || sJamCode=="JAM0509" ||                   //kevin 20140612 可以設定 權限  //Device lose at Output Shuttle
               sJamCode=="JAM0303"  || sJamCode=="JAM0304" ||                   //Device drop error (Arm 1) & Device drop error (Arm 2)    //Jou 20140604 Add for SCC
               sJamCode=="WAR0310" )                                            //Socket has IC error!                                     //Jou 20140604 Add for SCC
            {
                labMustCheck_35->Visible=true;
                if(iLevel<LevelSet.AccessLevel[35])
                {
                    iLevel=LevelSet.AccessLevel[35];
                }
            }
        }
        else if(CUSTOMER_CODE==CC_ASE_CL)
        {
            if(sJamCode=="WAR16126" || sJamCode=="WAR1685" ||  //WAR16118->WAR16126 Steven 20260331
               sJamCode.AnsiPos("WAR24")>0)                                     //JerryYang 20240626 : Song要求馬達ALARM都要Engineer以上權限才能解
            {
                if(iLevel<1)
                {
                    iLevel=1;
                }
            }
        }
        else
        {
            if(sJamCode=="JAM0203" || sJamCode=="WAR0310" || sJamCode=="WAR0346" ||
               sJamCode=="WAR0343" || sJamCode=="JAM0301" || sJamCode=="JAM0302" ||
               sJamCode=="WAR0701" || sJamCode=="WAR0702" || sJamCode=="WAR0703" || JamCode=="WAR0705" ||               //Steven 20150709 : Add for Amkor China
               sJamCode=="JAM0508" || sJamCode=="JAM0509" || sJamCode=="WAR07301"||
               sJamCode=="JAM0312" || sJamCode=="JAM0313" || sJamCode=="WAR0471" || sJamCode=="WAR0472" )               //JerryYang 20160516 add JAM0312 JAM0313  //wei 20160823  Lot check 錯誤需輸入密碼
            {
                labMustCheck_35->Visible=true;
                if(iLevel<LevelSet.AccessLevel[35])
                {
                    iLevel=LevelSet.AccessLevel[35];
                }
            }
            else if(sJamCode=="WAR16330")   //JerryYang 20260626 : add
            {
                if(iLevel<1)
                {
                    iLevel=1;
                }
            }
        }
    }

    return iLevel;
}
//---------------------------------------------------------------------------
int TfSecurity::GetBit8(AnsiString sJamArea, AnsiString sJamCode)               //Isaac 20170922 (Steven) : ATP Machine Status Bit8 Issue
{
    int iBit8=0;
    iBit8=CheckAndReadIniData(FileNameJam000, sJamArea, sJamCode+" Bit8", 0);

    return iBit8;
}
//---------------------------------------------------------------------------
bool TfSecurity::GetJemSilent(AnsiString sJamArea, AnsiString sJamCode)         //Steven 20150423 : SCK要求可以自訂Alarm是否要有蜂鳴器
{
    bool bNeedSilent=false;

    if(sJamArea!="" && sJamCode!="")
    {
        bNeedSilent=CheckAndReadIniData(FileNameJam000, sJamArea, sJamCode+" Silent", false);
    }
    return bNeedSilent;
}
//---------------------------------------------------------------------------
bool TfSecurity::GetJemUnlockPassWord(AnsiString sJamArea, AnsiString sJamCode)                                         //Ifor 20170214 (wei) add 可以自訂Alarm是否要解除密碼
{
    bool bUnlockPassWord=false;

    if(sJamArea!="" && sJamCode!="")
    {
        bUnlockPassWord=CheckAndReadIniData(FileNameJam000, sJamArea, sJamCode+" UnlockPassWord", false);
    }
    return bUnlockPassWord;
}
//---------------------------------------------------------------------------
bool TfSecurity::GetJemIncludeMTBA(AnsiString sJamArea, AnsiString sJamCode)    //JerryYang 20180619 (wei) : 新增可自定義Jam code是否列入MTBA計算
{
    bool bIncludeMTBA=false;

    if(sJamArea!="" && sJamCode!="")
    {
        if(CUSTOMER_CODE==CC_ASE_CL ||
           CUSTOMER_CODE==CC_TERAPOWER)
        {
            bIncludeMTBA=CheckAndReadIniData(FileNameJam000, sJamArea, sJamCode+" IncludeMTBF", true);
        }
        else
        {
            if(sJamCode.Pos("JAM")>0 &&
               (sJamArea=="01 Input Arm" ||
                sJamArea=="02 Output Arm" ||
                sJamArea=="03 Index Unit" ||
                sJamArea=="04 Input Shuttle" ||
                sJamArea=="05 Output Shuttle"))
            {
                bIncludeMTBA=CheckAndReadIniData(FileNameJam000, sJamArea, sJamCode+" IncludeMTBA", true);
            }
            else
            {
                bIncludeMTBA=CheckAndReadIniData(FileNameJam000, sJamArea, sJamCode+" IncludeMTBA", false);
            }
        }
    }
    return bIncludeMTBA;
}
//---------------------------------------------------------------------------
bool TfSecurity::GetJemContiAlarm(AnsiString sJamArea, AnsiString sJamCode)     //Steven 20200513 : 連續alarm輸入密碼的alarm要可以自訂義
{
    bool bContiAlarm=false;

    if(sJamArea!="" && sJamCode.Pos("JAM")>0)
    {
        bContiAlarm=CheckAndReadIniData(FileNameJam000, sJamArea, sJamCode+" ContiAlarm", true);
    }
    return bContiAlarm;
}
//---------------------------------------------------------------------------
bool TfSecurity::GetO17ContiAlarm(AnsiString sJamArea, AnsiString sJamCode)     //Steven 20210127 : 逸昌要求在單位時間內相同Alarm發生多次,提昇解除alarm權限
{
    bool bContiAlarm=false;

    if(sJamArea!="")
    {
        bContiAlarm=CheckAndReadIniData(FileNameJam000, sJamArea, sJamCode+" O17ContiAlarm", false);
    }
    return bContiAlarm;
}
//---------------------------------------------------------------------------
bool TfSecurity::GetAddAlarmLog(AnsiString sJamArea, AnsiString sJamCode)       //Sam 20210611 : Alarm Log 可以自定義哪個需要記 Log 上報
{
    bool bContiAlarm=false;

    if(sJamArea!="")
    {
        bContiAlarm=CheckAndReadIniData(FileNameJam000, sJamArea, sJamCode+" GetAddAlarmLog", false);
    }
    return bContiAlarm;
}
//---------------------------------------------------------------------------
bool TfSecurity::GetJemTCPAlarm(AnsiString sJamArea, AnsiString sJamCode)       //Sam 20230426 : 通知系統 Handler 已經密碼鎖定
{
    bool bTCPAlarm=false;
    if(sJamArea!="")
    {
        bTCPAlarm=CheckAndReadIniData(FileNameJam000, sJamArea, sJamCode+" TCPAlarm", false);
    }
    return bTCPAlarm;
}
//---------------------------------------------------------------------------
bool TfSecurity::GetAlarmAfterUnloaderFull(AnsiString sJamArea, AnsiString sJamCode)                                    //Jimmychiu 20240902 : Need Alarm After Unloader Full
{
    bool bAlarm=false;
    if(sJamArea=="02 Output Arm")
    {
        bAlarm=CheckAndReadIniData(FileNameJam000, sJamArea, sJamCode+" AlarmAfterUnloaderFull", false);
    }
    return bAlarm;
}
//---------------------------------------------------------------------------
AnsiString TfSecurity::GetJamArea(AnsiString sJamArea)
{
    return fMain->AlarmUnitMap[sJamArea];                                       //Steven 20231127 : 整理Alarm Unit
}
//---------------------------------------------------------------------------
bool TfSecurity::GetJemRed(AnsiString sJamArea, AnsiString sJamCode)            //Steven 20140222 : 取得Alarm畫面是否要紅底
{
    bool bNeedRed=false;

    if(sJamArea!="" && sJamCode!="")
    {
        cbJamNeedRed->Enabled=true;
        bNeedRed=CheckAndReadIniData(FileNameJam000, sJamArea, sJamCode+" Red", false);
        if(IniConfig.bSPILFunction==true ||                                     //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction  //Steven 20140430 : 矽品要求全自定
           CUSTOMER_CODE==CC_SCS)                                               //jou 2015-08-27 SCS 要求 Jam Level 要可以自己選擇
        {
            ;
        }
        else if(CUSTOMER_CODE==CC_AMKOR_China || CUSTOMER_CODE==CC_QUALCOMM)    //JerryYang 20170412 (Steven) add QUALCOMM
        {
            if(sJamCode=="WAR0701" || sJamCode=="JAM0301" ||
               sJamCode=="WAR0702" || sJamCode=="WAR0703" ||                    //Steven 20150709 : Add for Amkor China
               sJamCode=="JAM0302" || sJamCode=="JAM0303" ||
               sJamCode=="JAM0304" || sJamCode=="JAM0305" ||
               sJamCode=="JAM0306" || sJamCode=="JAM0306" ||
               sJamCode=="WAR0310" || sJamCode=="WAR0343" ||
               sJamCode=="JAM0312" || sJamCode=="JAM0313" ||
               sJamCode=="JAM0314" || sJamCode=="JAM0315")                      //JerryYang 20160516 add JAM0312~JAM0315
            {
                bNeedRed=true;
                cbJamNeedRed->Enabled=false;
            }
        }
        else if(CUSTOMER_CODE==CC_SCC)
        {
            if(sJamCode=="WAR07301" ||                                          //Socket consecutive failure
               sJamCode=="WAR07321" ||                                          //Arm1: consecutive failure
               sJamCode=="WAR07329" ||                                          //Arm2: consecutive failure
               sJamCode=="JAM0508" ||
               sJamCode=="JAM0509")                                             //Device lose at Output Shuttle
            {
                bNeedRed=true;
                cbJamNeedRed->Enabled=false;
            }
        }
        else
        {
            ;
        }
    }
    return bNeedRed;
}
//---------------------------------------------------------------------------
void TfSecurity::GetLevelSet()                                                  //Steven 20140222 : 存取LevelSet檔案改為Function
{
    AnsiString FileName="d:\\HT9045\\system\\levelset.dat";
    if(FileExists(FileName)==false)
    {
        WriteData(FileName.c_str(), (char *)&LevelSet.AccessLevel[0], sizeof(LevelSet));
    }
    ReadData(FileName.c_str(), (char *)&LevelSet.AccessLevel[0], sizeof(LevelSet));

    for(int i=0; i<256; i++)
    {
        if((CUSTOMER_CODE==CC_KYEC_LEE) &&                                      //Ifor 20160825 add PE模式強制設定為2不可修改
           (i==35 || i==114 || i==128 || i==104))                               //Ifor 20160914 京元喬智要求 Trouble Shooting強制設定為2
        {                                                                       //Ifor 20170203 (Steven) 京元喬智要求ATC Control強制設定為2不可修改
            if(i==104)
            {
                LevelSet.AccessLevel[i]=3;                                      //Ifor 20200330 :add KYEC 喬智要求強制設定最高權限
            }
            else
            {
                LevelSet.AccessLevel[i]=2;
            }
        }
        else if(i==163)
        {
            LevelSet.AccessLevel[i]=3;                                          //Ifor 20200114 add:Life Time Edit Permission 強制設定Hontech權限才可修改
        }
        else
        {
            if(CosFunction.bSecurityHave5Level==true)                           //jou 2014-06-19 Security Have 5 Level
                LevelSet.AccessLevel[i]=CheckRange(LevelSet.AccessLevel[i], 0, 4);                                      //Steven 20110907 : 檢查範圍
            else
                LevelSet.AccessLevel[i]=CheckRange(LevelSet.AccessLevel[i], 0, 3);                                      //Steven 20110907 : 檢查範圍
        }
    }
}
//---------------------------------------------------------------------------
void TfSecurity::SetLevelSet()                                                  //Steven 20140222 : 存取LevelSet檔案改為Function
{
    AnsiString FileName="d:\\HT9045\\system\\levelset.dat";
    WriteData(FileName.c_str(), (char *)&LevelSet.AccessLevel[0], sizeof(LevelSet));
}
//---------------------------------------------------------------------------
void __fastcall TfSecurity::spbImportClick(TObject *Sender)
{
    TStringList *MyStrList=new TStringList;
//    TStringList *Str=new TStringList;
    AnsiString Str;
    AnsiString MyJamArea;
    AnsiString MyJamCode;
    AnsiString Level;
    AnsiString RedBg;
    AnsiString Silent="0";
    AnsiString Message;
    AnsiString UnlockPassWord="0";
    AnsiString MachineStatusBit8;
    int iPos;

    if(OpenDialog1->Execute())
    {
        MyStrList->LoadFromFile(OpenDialog1->FileName);

        for(int i=1; i<MyStrList->Count; i++)
        {
            Str=MyStrList->Strings[i];                                          //Steven 20180404 (Jou) : Fixed for CSV被Excel存檔之後, 雙引號不見導致讀取異常的問題
            Str=StringReplace(Str, "\"", "", TReplaceFlags()<<rfReplaceAll);    //Isaac 20180427 (Steven) Jam cord csv file format changed & no work properly,把"去掉
            iPos=Str.AnsiPos(",");
            MyJamArea=Str.SubString(1, iPos-1);
            Str=Str.SubString(iPos+1, Str.Length());
            iPos=Str.AnsiPos(",");
            MyJamCode=Str.SubString(1, iPos-1);
            Str=Str.SubString(iPos+1, Str.Length());
            iPos=Str.AnsiPos(",");
            Level=Str.SubString(1, iPos-1);
            Str=Str.SubString(iPos+1, Str.Length());
            iPos=Str.AnsiPos(",");
            RedBg=Str.SubString(1, iPos-1);
            Str=Str.SubString(iPos+1, Str.Length());
            iPos=Str.AnsiPos(",");
            Silent=Str.SubString(1, iPos-1);
            Str=Str.SubString(iPos+1, Str.Length());
            iPos=Str.AnsiPos(",");
            UnlockPassWord=Str.SubString(1, iPos-1);
            Str=Str.SubString(iPos+1, Str.Length());
            if(CUSTOMER_CODE!=CC_SCK)                                           //Isaac 20180413 (Steven) 卡客戶碼，SCK說不要用
            {
                iPos=Str.AnsiPos(",");
                MachineStatusBit8=Str.SubString(1, iPos-1);
            }

            WriteIniData(FileNameJam000, MyJamArea, MyJamCode, Level);
            WriteIniData(FileNameJam000, MyJamArea, MyJamCode+" Silent", Silent);                                       //Steven 20150423 : SCK要求可以自訂Alarm是否要有蜂鳴器
            WriteIniData(FileNameJam000, MyJamArea, MyJamCode+" Red", RedBg);
            if(CUSTOMER_CODE!=CC_SCK)                                           //Isaac 20180413 (Steven) 卡客戶碼，SCK說不要用
            {
                WriteIniData(FileNameJam000, MyJamArea, MyJamCode+" Bit8", MachineStatusBit8);
            }

            if(CosFunction.bUseAlarmUnlockPassWord==true)                       //Ifor 20170214 (wei) add 解除Alarm 需要獨立密碼
            {
                WriteIniData(FileNameJam000, MyJamArea, MyJamCode+" UnlockPassWord", UnlockPassWord);                   //Ifor 20170214 add 可以自訂Alarm是否要解除密碼
            }
//            }
        }
    }
    ChangeJamMessage(false);

    spbImport->Down=false;

    MyStrList->Clear();                                                         //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete MyStrList;                                                           //Steven 20160912 : Add delete for save memory
}
//---------------------------------------------------------------------------
void __fastcall TfSecurity::spbExportClick(TObject *Sender)
{
    AnsiString MyJamArea;
    AnsiString MyJamCode;
    AnsiString Level;
    AnsiString RedBg;
    AnsiString Message;
    AnsiString CommaText;
    AnsiString OldJamCode="";

    TStringList *MyStrList=new TStringList;

    if(SaveDialog1->Execute())
    {
        if(CUSTOMER_CODE==CC_SCK)                                               //Isaac 20180413 (Steven) 卡客戶碼，SCK說不要用
        {
            CommaText.sprintf("JamArea, JamCode, JamLevel, JamNeedRed, SilentMode, UnlockPassWord, Message");
        }
        else
        {
            CommaText.sprintf("JamArea, JamCode, JamLevel, JamNeedRed, SilentMode, UnlockPassWord, MachineStatusBit8, Message");
        }
        MyStrList->Add(CommaText);

        for(int i=0; i<cbJamArea->Items->Count; i++)                            //最少要掃過一次
        {
            cbJamArea->ItemIndex=i;
            cbJamArea->Refresh();
            GetJameCodeOfAxis(cbJamArea->ItemIndex+1, cbJamCode);

            for(int j=0; j<cbJamCode->Items->Count; j++)
            {
                cbJamCode->ItemIndex=j;
                cbJamCode->Refresh();
                cbJamLang->ItemIndex=0;
                cbJamLang->Refresh();
                JamArea=cbJamArea->Text;
                JamCode=cbJamCode->Text.SubString(1, cbJamCode->Text.AnsiPos("  :")-1);
                Message=cbJamCode->Text.SubString(cbJamCode->Text.AnsiPos(":")+1, cbJamCode->Text.Length());
                ChangeJamMessage(false);

                if(OldJamCode!=JamCode)
                {
                    OldJamCode=JamCode;
                    if(CUSTOMER_CODE==CC_SCK)                                   //Isaac 20180413 (Steven) 卡客戶碼，SCK說不要用
                    {
                        CommaText.sprintf("\"%s\",\"%s\",\"%d\",\"%d\",\"%d\",\"%d\",\"%s\"",                           //Isaac 20180427 (Steven) Jam cord csv file format changed & no work properly,export出來時都加上雙引號
                                           JamArea,
                                           JamCode,
                                           rgJamLevel->ItemIndex,
                                           (cbJamNeedRed->Checked)?1:0,
                                           (cbSilentMode->Checked)?1:0,         //Steven 20150423 : SCK要求可以自訂Alarm是否要有蜂鳴器
                                           (cbUnlockPassWord->Checked)?1:0,
                                           Message);                            //Steven 20170331 (wei) : 這個請保持在最後一筆
                    }
                    else
                    {
                        CommaText.sprintf("\"%s\",\"%s\",\"%d\",\"%d\",\"%d\",\"%d\",\"%d\",\"%s\"",                    //Isaac 20180427 (Steven) Jam cord csv file format changed & no work properly,export出來時都加上雙引號
                                           JamArea,
                                           JamCode,
                                           rgJamLevel->ItemIndex,
                                           (cbJamNeedRed->Checked)?1:0,
                                           (cbSilentMode->Checked)?1:0,         //Steven 20150423 : SCK要求可以自訂Alarm是否要有蜂鳴器
                                           (cbUnlockPassWord->Checked)?1:0,
                                           (rgMachineStatusBit8->ItemIndex)?1:0,
                                           Message);                            //Steven 20170331 (wei) : 這個請保持在最後一筆
                    }
                    MyStrList->Add(CommaText);
                }
            }
        }

        MyStrList->SaveToFile(SaveDialog1->FileName);
    }

    spbExport->Down=false;
    MyStrList->Clear();                                                         //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete MyStrList;                                                           //Steven 20160912 : Add delete for save memory
}
//---------------------------------------------------------------------------
void __fastcall TfSecurity::btnOperatorClick(TObject *Sender)
{
    ChangePassword(1);                                                          //jou 2014-06-19 Security Have 5 Level
}
//---------------------------------------------------------------------------
void __fastcall TfSecurity::AddAlarmList()                                      //jou 20171201 (Steven) : 新增統計jam code alarm次數,達到設定數量後提高一階權限才能解開alarm
{
    if(CosFunction.bStatisticsJamCount==false)
    {
        tsStatisticsJam->TabVisible=false;
        return;
    }

    GetAlarmCodeList(sgStatisticsJam);                                          //Steven 20200331 : Alarm code list改用文字檔
    sgStatisticsJam->ColWidths[7]=50;                                           //Count
    sgStatisticsJam->Cells[7][0]="Count";
    ClearAllJamCount();
}
//------------------------------------------------------------------------------
void __fastcall TfSecurity::ClearAllJamCount()                                  //jou 20171201 (Steven) : 新增統計jam code alarm次數,達到設定數量後提高一階權限才能解開alarm
{
    if(CosFunction.bStatisticsJamCount==false)
    {
        return;
    }

    for(int i=1; i<sgStatisticsJam->RowCount; i++)
    {
        sgStatisticsJam->Cells[7][i]="0";
    }
}
//------------------------------------------------------------------------------
bool __fastcall TfSecurity::AddJamCount(AnsiString asJamCode)                   //jou 20171201 (Steven) : 新增統計jam code alarm次數,達到設定數量後提高一階權限才能解開alarm
{
    if(CosFunction.bStatisticsJamCount==false)
    {
        return false;
    }

    int iCount=0;
    for(int i=1; i<sgStatisticsJam->RowCount; i++)
    {
        if(sgStatisticsJam->Cells[2][i]==asJamCode)
        {
            iCount=atoi(sgStatisticsJam->Cells[7][i].c_str())+1;
            if(iCount>=5)
            {
                sgStatisticsJam->Cells[7][i]="0";
                return true;
            }
            else
            {
                sgStatisticsJam->Cells[7][i]=iCount;
                return false;
            }
        }
    }
    return false;
}
//------------------------------------------------------------------------------
bool TfSecurity::GetN27AlarmSel(AnsiString sJamArea, AnsiString sJamCode)
{
    bool bN27AlarmSel=false;

    if(sJamArea!="")
    {
        if(sJamCode=="")
            bN27AlarmSel=CheckAndReadIniData(FileNameJam000, sJamArea, "bN27AlarmSel", true);
        else
            bN27AlarmSel=CheckAndReadIniData(FileNameJam000, sJamArea, sJamCode+" bN27AlarmSel", true);
    }
    return bN27AlarmSel;
}
//---------------------------------------------------------------------------
bool TfSecurity::GetN27AddBoard(AnsiString sJamArea, AnsiString sJamCode)       //Sam 20210911 :  矽格中興廠 Alarm Message Board 是否顯示增加開關
{
    bool bN27AddBoard=false;

    if(sJamArea!="" && sJamCode!="")
    {
        bN27AddBoard=CheckAndReadIniData(FileNameJam000, sJamArea, sJamCode+" bN27AddBoard", false);
    }
    return bN27AddBoard;
}
//------------------------------------------------------------------------------
bool TfSecurity::GetContAlarmNotUpload(AnsiString sJamArea, AnsiString sJamCode)                                        //Sam 20231116 : 連續 Alarm 不要上傳伺服器
{
    bool bContAlarmNotUpload=false;
    if(sJamArea!="" && sJamCode!="")
    {
        bContAlarmNotUpload=CheckAndReadIniData(FileNameJam000, sJamArea, sJamCode+" ContAlarmNotUpload", false);
    }
    return bContAlarmNotUpload;
}
//------------------------------------------------------------------------------
AnsiString TfSecurity::GetPasswoard()                                           //Sam 20220106 : 整合密碼
{
    AnsiString sPathName="";
    AnsiString asPassword="16943420";
    if(IniConfig.bSIGURDFunction)
    {
        sPathName.sprintf("D:\\HT9045\\system");
        MyForceDirectories(sPathName);
        sPathName.sprintf("D:\\HT9045\\system\\SG_PW.ini");
        if(FileExists(sPathName)==false)                                        //檢查檔案
        {
            WriteIniData(sPathName, "SG",    "PW",       (AnsiString)16943420);
        }
        asPassword=CheckAndReadIniData(sPathName, "SG",    "PW",    (AnsiString)16943420);
    }

    return asPassword;
}
//------------------------------------------------------------------------------
