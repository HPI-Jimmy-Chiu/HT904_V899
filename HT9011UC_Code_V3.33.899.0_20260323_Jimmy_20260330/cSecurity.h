//---------------------------------------------------------------------------

#ifndef cSecurityH
#define cSecurityH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Forms.hpp>
#include <StdCtrls.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <Grids.hpp>
#include <vector>
//---------------------------------------------------------------------------
class PACKAGE TMySecurity : public TComponent
{
    private:    // User declarations
        int index;
        AnsiString Caption;
        TPanel *Panel;
        TSpeedButton *SpeedButton;
        bool Visible;
    public:
        __fastcall TMySecurity(AnsiString Caption, Graphics::TBitmap *Img, TWinControl *Sender);
        __fastcall ~TMySecurity();
        void SetCaption(AnsiString Caption, int index);
        AnsiString GetCaption();
        void SetParent(TWinControl *Sender);
        void SetVisible(bool bShow) {Panel->Visible=bShow; Visible=bShow;};
        bool GetVisivle() {return Visible;};
        void SetPosition(int Top, int Left=7);
        void SetLevel(int Level) {RadioGroup->ItemIndex=Level;};
        int  GetLevel() {return RadioGroup->ItemIndex;};
        TRadioGroup *RadioGroup;
        void SetEnabled(bool Enabled) {RadioGroup->Enabled=Enabled;};      //wei 20150803
};
//---------------------------------------------------------------------------
class TfSecurity : public TForm
{
__published:    // IDE-managed Components
    TPanel *Panel13;
    TPanel *Panel1;
    TSpeedButton *sbSupervisor;
    TSpeedButton *sbEngineer;
    TPanel *Panel17;
    TPanel *SecurityExit;
    TSpeedButton *btnHonPrec;
    TPageControl *PageControl1;
    TTabSheet *tsMain;
    TTabSheet *tsTools;
    TTabSheet *tsConfig;
    TTabSheet *tsContact;
    TTabSheet *tsSetup;
    TTabSheet *tsTemp;
    TScrollBox *sbMain;
    TTabSheet *tsIo;
    TTabSheet *tsConfiguration;
    TTabSheet *tsYield;
    TTabSheet *tsOther;
    TScrollBox *sbTools;
    TScrollBox *sbConfig;
    TScrollBox *sbContact;
    TScrollBox *sbSetup;
    TScrollBox *sbTemp;
    TScrollBox *sbIO;
    TScrollBox *sbConfiguration;
    TScrollBox *sbYield;
    TScrollBox *sbOther;
    TTabSheet *tsJamCode;
    TRadioGroup *rgJamLevel;
    TLabel *labJamArea;
    TComboBox *cbJamArea;
    TLabel *labJamCode;
    TComboBox *cbJamCode;
    TLabel *Label1;
    TLabel *labLang;
    TComboBox *cbJamLang;
    TRichEdit *RichEditJamCode;
    TCheckBox *cbJamNeedRed;
    TSpeedButton *spbExport;
    TSpeedButton *spbImport;
    TOpenDialog *OpenDialog1;
    TSaveDialog *SaveDialog1;
    TLabel *labMustCheck_35;
    TSpeedButton *btnOperator;
    TCheckBox *cbSilentMode;
    TCheckBox *cbUnlockPassWord;
    TRadioGroup *rgMachineStatusBit8;
    TTabSheet *tsStatisticsJam;
    TStringGrid *sgStatisticsJam;
    TCheckBox *cbIncludeMTBA;
    TCheckBox *chkCheckContAlarm;
    TCheckBox *chkO17;
    TCheckBox *cbAddAlarmLog;
    TCheckBox *cbN27AddBoard;
    TCheckBox *cbN27AlarmSel;
    TCheckBox *cbN27AlarmSelByArea;
    TCheckBox *chkTCPAlarm;
    TCheckBox *cbContAlarmNotUpload;
    TCheckBox *chkAlarmAfterFullTray;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall SecurityExitClick(TObject *Sender);
    void __fastcall btnHonPrecClick(TObject *Sender);
    void __fastcall sbSupervisorClick(TObject *Sender);
    void __fastcall sbEngineerClick(TObject *Sender);
    void __fastcall cbJamAreaChange(TObject *Sender);
    void __fastcall cbJamCodeChange(TObject *Sender);
    void __fastcall cbJamLangChange(TObject *Sender);
    void __fastcall spbImportClick(TObject *Sender);
    void __fastcall spbExportClick(TObject *Sender);
    void __fastcall btnOperatorClick(TObject *Sender);
private:    // User declarations
    void ChangeJamMessage(bool bSave=true);
    void SaveJamLevel();

    AnsiString JamArea, JamCode;

    AnsiString FileNameJam000;
    int JamLang,MachineStatusBit8;

    void __fastcall AddAlarmList();                                             //jou 20171201 (Steven) : 新增統計jam code alarm次數,達到設定數量後提高一階權限才能解開alarm
    void SecurityPalVisible();
public:     // User declarations
    __fastcall TfSecurity(TComponent* Owner);

    bool __fastcall Insufficient(int iType, bool bAlarm=true);                  //jou 981207 權限控制
    std::vector<TMySecurity *> mySecurityPal;
    int  GetJamLevel(AnsiString sJamArea, AnsiString sJamCode);
    bool GetJemRed(AnsiString sJamArea, AnsiString sJamCode);
    bool GetJemSilent(AnsiString sJamArea, AnsiString sJamCode);                        //Steven 20150423 : SCK要求可以自訂Alarm是否要有蜂鳴器
    void GetLevelSet();
    void SetLevelSet();
    bool GetJemUnlockPassWord(AnsiString sJamArea, AnsiString sJamCode);        //Ifor 20170214 add 可以自訂Alarm是否要有蜂鳴器
    bool GetJemIncludeMTBA(AnsiString sJamArea, AnsiString sJamCode);           //JerryYang 20180619 (wei) : 新增可自定義Jam code是否列入MTBA計算
    bool GetJemContiAlarm(AnsiString sJamArea, AnsiString sJamCode);            //Steven 20200513 : 連續alarm輸入密碼的alarm要可以自訂義
    bool GetO17ContiAlarm(AnsiString sJamArea, AnsiString sJamCode);            //Steven 20210127 : 逸昌要求在單位時間內相同Alarm發生多次,提昇解除alarm權限
    bool GetAddAlarmLog(AnsiString sJamArea, AnsiString sJamCode);              //Sam 20210611 : Alarm Log 可以自定義哪個需要記 Log 上報
    AnsiString GetJamArea(AnsiString sJamArea);

    int  GetBit8(AnsiString sJamArea, AnsiString sJamCode);
    int  iBit8;
    bool fShow;
    void __fastcall ClearAllJamCount();                                         //jou 20171201 (Steven) : 新增統計jam code alarm次數,達到設定數量後提高一階權限才能解開alarm
    bool __fastcall AddJamCount(AnsiString asJamCode);                          //jou 20171201 (Steven) : 新增統計jam code alarm次數,達到設定數量後提高一階權限才能解開alarm
    bool GetN27AlarmSel(AnsiString sJamArea, AnsiString sJamCode="");
    bool GetN27AddBoard(AnsiString sJamArea, AnsiString sJamCode);              //Sam 20210911 :  矽格中興廠 Alarm Message Board 是否顯示增加開關
    AnsiString GetPasswoard();                                                  //Sam 20220106 : 整合密碼
    bool GetJemTCPAlarm(AnsiString sJamArea, AnsiString sJamCode);              //Sam 20230426 : 通知系統 Handler 已經密碼鎖定
    bool GetContAlarmNotUpload(AnsiString sJamArea, AnsiString sJamCode);       //Sam 20231116 : 連續 Alarm 不要上傳伺服器
    bool GetAlarmAfterUnloaderFull(AnsiString sJamArea, AnsiString sJamCode);   //Jimmychiu 20240902 : Need Alarm After Unloader Full
};
//---------------------------------------------------------------------------
extern PACKAGE TfSecurity *fSecurity;
//---------------------------------------------------------------------------
#endif
