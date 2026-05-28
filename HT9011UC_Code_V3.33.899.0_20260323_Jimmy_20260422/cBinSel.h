//******************************************************************************
//
//  注意!! cBinSel為Handler Bin設定相關, 修改時要小心!!
//
//******************************************************************************
#ifndef cBinSelH
#define cBinSelH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
#include <ExtCtrls.hpp>

#include "MachineType.h"
#include "HTray.h"
#include <Forms.hpp>
//#include <vector>
//using namespace std;

/*enum eTrayName                  //使用的Harware Bin數量與名稱
{   eAuto1          =0,
    eAuto2          =1,
    eAuto3          =2,
    eFix1           =3,
    eFix2           =4,
    eFix3           =5,
    eFix4           =6,
    eFix5           =7,
    eFix6           =8,
    eBulkBox        =9,  //kevin 20160819 add bulk box
    eTrayCount
};

enum eBinPositionName           //Bin要放在Hardware上的位置
{   ePosNoUse       =0,
    ePosAuto1       =1,
    ePosAuto2       =2,
    ePosAuto3       =3,
    ePosFix1        =4,
    ePosFix2        =5,
    ePosFix3        =6,
    ePosFix4        =7,
    ePosFix5        =8,
    ePosFix6        =9,
    eBulkbox        =10,
    ePosTrayCount
};*/    //Steven 20200825 : eTrayCount定義換位置
//---------------------------------------------------------------------------
class TfBinSel : public TForm
{
__published:    // IDE-managed Components
    TPageControl *PageControl1;
    TTabSheet *tsNormal;
    TTabSheet *tsRetest;
    TPopupMenu *PopupMenu1;
    TMenuItem *Cancel1;
    TMenuItem *spilt;
    TMenuItem *Check;
    TMenuItem *UnCheck;
    TTabSheet *tsOffline;
    TPanel *Panel1;
    TLabel *labWarning;
    TSpeedButton *spbSave;
    TSpeedButton *sbtExit;
    TComboBox *cbTestMode;
    TSpeedButton *spbNormal;
    TSpeedButton *spbPrime;
    TPanel *Panel;
    TTMyTray *mtTrayName;
    TTMyTray *mtTrayItem;
    TEdit *edBinSetting;
    TEdit *edBinSettingError;
    TEdit *edBinSettingPassFail;
    TEdit *edBinSettingLink;
    TEdit *edBinSettingEnableScan;
    TEdit *edBinSetting2Contact;
    TEdit *edBinSettingConFail;
    TEdit *edBinSettingFailPersentEnable;
    TEdit *edBinSettingFailPersentNumber;
    TEdit *edBinSettingFailPersentIgnore;
    TEdit *edBinSettingFailCountEnable;
    TEdit *edBinSettingFailCountNumber;
    TEdit *edBinSettingFailCountIgnore;
    TEdit *edInput;
    TScrollBox *sbBinSetting;
    TTMyTray *mtBinSelect;
    TEdit *edBinAutoRetest;
    TEdit *edSpecialBinByArm;
    TEdit *edSpecialBinCountByArm;
    TEdit *edSpecialBinCountBySocket;
    TEdit *edSpecialBinBySocket;
    TTabSheet *tsArtFT;
    TTabSheet *tsArtRT;
    TLabel *Label1;
    TCheckBox *CancelErrorBin;
    TPanel *BulkBox;
    TLabel *LabBulkBox;
    TLabel *Label2;
    TLabel *Label3;
    TRadioGroup *rg_FixBinBox;
    TEdit *ed_FixBinBoxAlarmCount;
    TEdit *edTrayCateR;
    TSpeedButton *btnSettingSpecificBin;
    TPanel *palSpecificBin;
    TLabel *lblAllsiteSameBinNeedAlarm;
    TStringGrid *sgSpecificBin;
    TTabSheet *tsMrtFT;
    TTabSheet *tsMrtRT;
    TCheckBox *cbUseMRTMode;
    TEdit *edSpecBinBySiteCompareEnable;
    TEdit *edSpecBinBySiteCompareIgnore;
    TEdit *edSpecBinBySiteComparePercent;
    TEdit *edSpecBinByArmPerSiteCompareEnable;
    TEdit *edSpecBinByArmPerSiteCompareIgnore;
    TEdit *edSpecBinByArmPerSiteComparePercent;
    TTMyTray *mtBinSelectBy;
    TEdit *edControlBinCheckPoint;
    TLabel *lblControlBinCheckPoint;
    TCheckBox *cbOutShtLoseICSetErrUntilOneCycle;
    TCheckBox *cbIndexDropErrSetErrUntilOneCycle;
    TButton *btnSetAll2NotUse;
    TPanel *pnlAutoSiteMap;
    TLabel *labAutoSiteMap;
    TLabel *labASMPassBin;
    TComboBox *cbbAutoSiteMap;
    TCheckBox *chkShow0Xbin;
    TComboBox *cbbASMPassBin;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall cbTestModeChange(TObject *Sender);
    void __fastcall spbSaveClick(TObject *Sender);
    void __fastcall sbtExitClick(TObject *Sender);
    void __fastcall spbNormalClick(TObject *Sender);
    void __fastcall spbPrimeClick(TObject *Sender);
    void __fastcall mtTrayNameMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall mtBinSelectMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall mtBinSelectMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
    void __fastcall mtBinSelectMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall rg_FixBinBoxClick(TObject *Sender);
    void __fastcall ed_FixBinBoxAlarmCountMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall CancelErrorBinClick(TObject *Sender);
    void __fastcall sgSpecificBinMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall btnSettingSpecificBinClick(TObject *Sender);
    void __fastcall palSpecificBinClick(TObject *Sender);
    void __fastcall FormShortCut(TWMKey &Msg, bool &Handled);
    void __fastcall btnSetAll2NotUseClick(TObject *Sender);
    void __fastcall mtTrayItemMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
private:    // User declarations
    bool bUseContinueFail;
    bool bUseFailRate;
    bool bSaveBin;
    bool bCheckOffLineLevel;
    int  iMode;                                                                 //Steven 20101116
    bool bMovFlag;                                                              //Alick 20160729 add for SCC
    void ShowChangeBinMessage();

    void __fastcall SetBinTray(int tag);
    void __fastcall ShowBinTray(int tag);
    void __fastcall Change();                                                   // check change pos //
    void __fastcall mtTrayNameSetColor(int tag);
    void __fastcall InitDataToEdit(int tag);
//    void __fastcall InitEditToData(int tag);
//    vector<TMyBinPanel*> MyBinPanel;
    bool bMouseDown;
    int iStartX, iStartY, iEndX, iEndY, iOldStartX, iOldStartY;

    bool SeteDoubleContact(int X, int tag);
    bool SeteConsFail(int X, int tag);
    bool SetePersentEnable(int X, int tag);
    bool SetePersentIgnore(int X, int tag);
    bool SetePersentNumber(int X, int tag);
    bool SeteCountEnable(int X, int tag);
    bool SeteCountIgnore(int X, int tag);
    bool SeteCountNumber(int X, int tag);
    bool SeteSpecialBinByArm(int X, int tag);
    bool SeteSpecialBinCountByArm(int X, int tag);
    bool SeteSpecialBinBySocket(int X, int tag);
    bool SeteSpecialBinCountBySocket(int X, int tag);
    bool SeteLowYield(int X, int tag);
    bool SeteArmYield(int X, int tag);
    bool SeteSiteYield(int X, int tag);
    bool SeteAutoCleanByBin(int X, int tag);
    bool SeteAutoCleanBySite(int X, int tag);
    bool SeteSpecBinBySiteCompareEnable(int X, int tag);
    bool SeteSpecBinBySiteCompareIgnore(int X, int tag);
    bool SeteSpecBinBySiteComparePercent(int X, int tag);
    bool SeteSpecBinByArmPerSiteCompareEnable(int X, int tag);
    bool SeteSpecBinByArmPerSiteCompareIgnore(int X, int tag);
    bool SeteSpecBinByArmPerSiteComparePercent(int X, int tag);    

public:     // User declarations
    bool bShow;
    __fastcall TfBinSel(TComponent* Owner);
    void __fastcall ReadParam();
    //void __fastcall ReadFile(bool bDelOffline);
    void __fastcall ReadFile(bool bDelOffline, bool bChangeNeme, AnsiString sFileName);  //kevin 20160928 新增舊bin 檔名轉換
    bool CheckFix2Tray();
    TStringList *sBinDoubleContact[eBinTypeTotal];                              //Steven 20140510 : Secs Gem
    TStringList *sBinConsFail[eBinTypeTotal];
    TStringList *sBinEnableFail[eBinTypeTotal];
    TStringList *sBinFailPercent[eBinTypeTotal];
//---------------
    //Steven 20140529 Start: Fail Persent & Count同時存在
    TStringList *sBinFailIgnore[eBinTypeTotal];
    TStringList *sBinCountEnable[eBinTypeTotal];
    TStringList *sBinCountIgnore[eBinTypeTotal];
    TStringList *sBinCountNumber[eBinTypeTotal];
    //Steven 20140529 End: Fail Persent & Count同時存在

    //ChungHung 20140730 add ContinuousFailHaveOneCycle start
    TStringList *sSpecialBinByArm[eBinTypeTotal];
    TStringList *sSpecialBinCountByArm[eBinTypeTotal];
    TStringList *sSpecialBinBySocket[eBinTypeTotal];
    TStringList *sSpecialBinCountBySocket[eBinTypeTotal];
    //ChungHung 20140730 add ContinuousFailHaveOneCycle end

    TStringList *sLowYield[eBinTypeTotal];                                      //Steven 20140828 : By Bin Yield Monitor
    TStringList *sArmYield[eBinTypeTotal];                                      //Steven 20140828 : By Bin Arm Yield Monitor
    TStringList *sSiteYield[eBinTypeTotal];                                     //Steven 20140828 : By Bin Site Yield Monitor
//---------------
    TStringList *sBinTraySetT3Pos[eBinTypeTotal];
    TStringList *sBinTraySetT3PosName[eBinTypeTotal];                           //Steven 20241205 : Bin to tray增加文字顯示
    void TransferBinTrayStrToName(int iTag);
    TStringList *sBinType[eBinTypeTotal];

    TStringList *sBySiteClean[eBinTypeTotal];                                   //Steven 20160308 : By Bin count do auto clean
    TStringList *sByBinClean[eBinTypeTotal];                                    //Steven 20160308 : By Bin pre site count do auto clean
    TStringList *sT3TrayType[eBinTypeTotal];
    TStringList *sT6Retest[eBinTypeTotal];
    TStringList *sT3CateR[eBinTypeTotal];

    TStringList *sSpecBinBySiteCompareEnable[eBinTypeTotal];                    //JerryYang 20170712 by site by bin compare percent
    TStringList *sSpecBinBySiteCompareIgnore[eBinTypeTotal];
    TStringList *sSpecBinBySiteComparePercent[eBinTypeTotal];
    TStringList *sSpecBinByArmPerSiteCompareEnable[eBinTypeTotal];
    TStringList *sSpecBinByArmPerSiteCompareIgnore[eBinTypeTotal];
    TStringList *sSpecBinByArmPerSiteComparePercent[eBinTypeTotal];

    bool bCheckTrayCanUse(int iT6Tray);
    void __fastcall SaveOther(AnsiString szDir);                                //kevin 20150105  另存新檔
    void __fastcall SetConFail(int iBinasgnMode);                               //ChungHung 20150519 add Auto Set ContFail After InitialStart.
    void ReadFunctionData(int tag);
    void SaveFunctionData(int tag);
    void SetPrimeButton();
    void ReadPrimeDara();
    void WritePrimeDara(int Mode);
    void ReadWriteMRTMode(int iMode);                                           //Ifor 20170417 (wei) add MRT Mode Read Write Function
    void ReadWriteSpecialFunction(bool bReadWrite);                             //Mode 0:Read 1:Write    //JerryYang 20220923 : add
    void __fastcall Save(int iECIDCode,int iFileCode);                          //kevin 20181120
    void ChangeActivePageIndex();                                               //Sam 20230711 : 修正 OLP SetCategroy 異常
    bool SetOSBin(int iBin);                                                    //Sam 20250115 : Add GPIB SETOSBIN_
    bool CheckOSBin();                                                          //Sam 20250115 : 新增 OSBIN 保護

    TStringList *sBinTrayLinked[eBinTypeTotal];
    TStringList *sBinLinked[eBinTypeTotal];
    TStringList *sMagazineSetup[eBinTypeTotal];    
};
//---------------------------------------------------------------------------
extern PACKAGE TfBinSel *fBinSel;
//******************************************************************************
//
//  注意!! cBinSel為Handler Bin設定相關, 修改時要小心!!
//
//******************************************************************************
#endif
