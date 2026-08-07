// =============================================================================
//  cAuthority.h  --  per-form / per-function Enable/Disable authority tables
//                    (Security_new.def-backed) + Input Limit / Dummy Vacuum /
//                    ATP critical-parameter lock tables.
//
//  Faithful translation of golden cAuthority.h (38 lines, BCB6, Big5/cp950).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 (rootleaf group).
//
//  ROLE: see cAuthority.cpp's own banner for the full function-by-function
//  tally. This header is pure declarations -- the bool[] authority tables, the
//  AnsiString[] Security_new.def key-name tables, and the 11 free functions
//  that load them.
//
//  MINIMAL NECESSARY ADAPTATION -- ChangeCompomentEnabled's TWinControl* param:
//  golden declares `void __fastcall ChangeCompomentEnabled(TWinControl *PCtrl,
//  bool bEnable, bool bMustEnable=false);` where real VCL's TWinControl is the
//  common ANCESTOR of TPanel/TTabSheet/TForm/TGroupBox/TRadioGroup/
//  TPageControl/TTabControl/THeader/TPage/TScrollBox (the "has children"
//  windowed-control family). vclcompat/Controls.h (pre-existing, not editable
//  per this wave's hard rules) deliberately flattens that split away: TPanel/
//  TGroupBox/TRadioGroup/TPageControl/TTabSheet all derive directly from
//  TControl, and there is no TWinControl there at all (confirmed by reading
//  the whole header). language.h independently hit the identical gap for
//  TfLan's own TForm-derived methods and added its own minimal TU-visible
//  TWinControl/TForm stand-ins rather than editing vclcompat/Controls.h --
//  see that file's own "TWinControl / TForm -- TU-local minimal stand-ins"
//  note. This header does NOT redeclare a second, conflicting `class
//  TWinControl` (that would be a same-name/different-shape ODR risk the
//  moment both headers are ever included together) -- it reuses language.h's
//  ALREADY-ESTABLISHED, header-guarded TWinControl/TForm by including
//  language.h itself. ChangeCompomentEnabled's body is GATED (see
//  cAuthority.cpp's own banner) for the identical reason language.cpp's own
//  SearchCompomentForSave/LoadCompomentForChangeLan are gated: it walks a
//  live TWinControl tree via ->ControlCount/->Controls[i], which language.h's
//  stand-in deliberately does not model ("no live widget hierarchy model
//  exists anywhere in this port yet"). No new widget-tree capability is
//  invented here.
//
//  VCL/Borland conversions: __fastcall dropped tree-wide (matches every other
//  ported header's convention, e.g. SECSGEM/uHGemEquipment.h's own citation of
//  this same house rule). No __property / __published / #pragma in golden
//  cAuthority.h to strip.
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8, in
//  its original column. Final gate: ZERO U+FFFD.
// =============================================================================
//---------------------------------------------------------------------------

#ifndef cAuthorityH
#define cAuthorityH

#include "MachineType.h"
#include "language.h"     // TU-shared TWinControl/TForm stand-ins (see banner "MINIMAL NECESSARY ADAPTATION" above) -- reused, not redefined
//---------------------------------------------------------------------------
extern bool authMainForm[12];
extern bool authTool[9];
extern bool authMaintaince[9];
extern bool authConfig[5];
extern bool authConf[14];         //JerryYang 20180511 12 -> 14
extern bool authCounterClr[9];
extern bool authObserver[2];
extern bool authBinSetting[8];    //Jou 20141110 : 3 --> 5  //Ifor 20170316 (wei) 5--->8
extern bool bAuthCriticalPara[26];//JerryYang 20220310 : ATP要求鎖定critical parameter
//extern bool authSetup[7];
extern bool authSetup[8]; //ChungHung 20130118 add OCR - 關閉要密碼
extern AnsiString funLimit[39];
extern int  iSetupSiteMapping;
extern AnsiString asRunStartModePassword;   //jou 20170214 (Steven) : RunStartMode need keyin Password

extern AnsiString BinSetting[eBinTypeTotal];
extern AnsiString funCriticalPara[];//JerryYang 20220310 : ATP要求鎖定critical parameter

void GetMainAuth();
void GetConfAuth();
void GetCountClrAuth();
void GetLimitAuth();
void GetDummyVacuum();
void GetObserAuth();
void GetSetupAuth();
void GetBinSettingAuth();    //Steven 20120330 : FT跟RT的Enable
void ChangeCompomentEnabled(TWinControl *PCtrl , bool bEnable, bool bMustEnable=false);
AnsiString CheckFile(AnsiString szDir, AnsiString str);
void GetCriticalParaAuth();  //JerryYang 20220310 : ATP要求鎖定critical parameter

#endif
