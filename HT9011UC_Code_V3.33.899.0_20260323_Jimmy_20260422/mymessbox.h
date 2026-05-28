//---------------------------------------------------------------------------
#ifndef mymessboxH
#define mymessboxH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TMyMessageBox : public TForm
{
__published:    // IDE-managed Components
    TPanel *pnlPause;
    TPanel *pnlMain;
    TTimer *Timer1;
    TPanel *pnlNo;
    TPanel *pnlYes;
    TLabel *labStopTime;
    TLabel *lblSubMsg;
    TLabel *lblMainMsg;
    TLabel *lblChineseMsg;
    TPanel *pnlAlarmReset;
    TMemo *moSecsGem;
    TLabel *lblSecsMsg;
    TPanel *palWaitEAP;
   void __fastcall pnlPauseClick(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall pnlYesClick(TObject *Sender);
    void __fastcall pnlAlarmResetClick(TObject *Sender);
    void __fastcall FormShortCut(TWMKey &Msg, bool &Handled);

private:    // User declarations
    int Status;
public:     // User declarations
    __fastcall TMyMessageBox(TComponent* Owner);
    int ret;
    bool flushState;
    int flushCT;
    bool fShow;
    bool bNeedPassWord;
    bool fScanPanel;
    bool bDisableAlarmBuzzer;
    char Message[256];
    bool bMBoxNeedPassword;
    bool DoPassword_MBox();
};
//---------------------------------------------------------------------------
extern PACKAGE TMyMessageBox *MyMessageBox;
int  ShowMyMessagePWD(AnsiString S1, AnsiString S2="", AnsiString S3=NULL, bool Ok=false, bool bServoOff=false ,bool Password =false);
int  ShowMyMessageBox_ASEK(AnsiString S1, AnsiString S2="Message Form Server", AnsiString S3=NULL);
int  ShowMyMessageBox_YES_SKIP(AnsiString S1, AnsiString S2, AnsiString S3=NULL);//JerryYang 20151028 add for Amkor 新增選YES或SKIP
int  ShowMyMessageBox_YES_NO(AnsiString S1, AnsiString S2, AnsiString S3=NULL);
int  ShowLotEndMessage(AnsiString S1, AnsiString S2, bool bFullLotEnd=false);   //JerryYang 20170424 (wei) for JCET, handler要通知測試機lot end或是 full lot end

void ShowMyMessage(AnsiString S1, AnsiString S2="", AnsiString S3=NULL, bool Ok=false, bool bServoOff=false);
void ShowMyMessageUp(AnsiString S1, AnsiString S2,bool UseButton);              //kevin 20150203
void ShowUnloaderTrayMessage(AnsiString S1, AnsiString S2);                     //Jou 20150721 : 重新啟用功能
void CheckEmployeeID(AnsiString S1);                                            //Ifor 20180227 (Steven) add SECS GEM Confirm the Employee ID
//---------------------------------------------------------------------------
#endif
