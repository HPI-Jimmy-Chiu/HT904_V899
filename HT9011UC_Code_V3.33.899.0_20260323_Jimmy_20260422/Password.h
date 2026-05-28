//---------------------------------------------------------------------------
#ifndef PasswordH
#define PasswordH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfPassword : public TForm
{
__published:    // IDE-managed Components
    TPageControl *PageControl1;
    TTabSheet *tsPassword;
    TTabSheet *tsEventlogLogin;
    TTabSheet *tsEventLogLoginModify;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TEdit *edPassword;
    TEdit *edUserName;
    TComboBox *cbUserName;
    TLabel *lbPasswordUserName;
    TLabel *lbPasswordPassWord;
    TComboBox *cob_UseIDList;
    TEdit *edPasswordPassWord;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label9;
    TComboBox *cob_ModifyUseIDList;
    TEdit *edModifyPassword;
    TEdit *edReModifyPassword;
    TSpeedButton *sbPasswordModify;
    TSpeedButton *sbPasswordModifyOK;
    TSpeedButton *sbPasswordModifyCancel;
    TSpeedButton *sbPasswordCancel;
    TSpeedButton *sbPasswordOK;
    TSpeedButton *btnOK;
    TSpeedButton *spbCancel;
    TLabel *lblUserName;
    TLabel *Label2;
    TSpeedButton *btnDownload;
    TLabel *lblPWDownload;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall SpeedButton1Click(TObject *Sender);
    void __fastcall spbCancelClick(TObject *Sender);
    void __fastcall edUserNameKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall edPasswordKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall edPasswordMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall edUserNameMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall cbUserNameChange(TObject *Sender);
    void __fastcall sbPasswordCancelClick(TObject *Sender);
    void __fastcall sbPasswordOKClick(TObject *Sender);
    void __fastcall sbPasswordModifyOKClick(TObject *Sender);
    void __fastcall sbPasswordModifyClick(TObject *Sender);
    void __fastcall btnOKClick(TObject *Sender);
    void __fastcall btnDownloadClick(TObject *Sender);
private:    // User declarations
    //Steven 20181224 : For ASE-CL
    //==>
        int iNowEvenLogUserLevel;                  //帳號帶入,下拉是選單顯示用
        bool bLoginSuccess;
        void __fastcall CheckPassword();
        void ReadPasswordFile();
        void SavePasswordFile();
    //<==
    //Steven 20181224 : For ASE-CL
public:     // User declarations

    __fastcall TfPassword(TComponent* Owner);
    bool bShow;

    //Steven 20181224 : For ASE-CL
    //==>
    enum EvenLog_Level                        //enum 型別
    {
        OPERATOR            = 0,
        ENG_VENDOR          = 1,
        ENG_MI              = 2,
        PM                  = 3,
        AUDIT               = 4,
        ADMIN               = 5
    };

    bool bShowTab;
    void __fastcall SetUserLevel(int iLevel);   //帳號帶入,下拉是選單顯示用
    bool __fastcall CheckLoginSuccess();        //檢查登入是否成功
    int __fastcall GetLoginLevel();             //目前登入權限
    void ShowEventLogLogin();
    //<==
    //Steven 20181224 : For ASE-CL
};
//---------------------------------------------------------------------------
//Steven 20181224 : For ASE-CL
//==>
typedef struct
{
    AnsiString ID[30];
    AnsiString PassWord[30];
    //int  Level[30];
}EvenLog_PASS_WORD;
extern EvenLog_PASS_WORD EvenLogUSER;
//<==
//Steven 20181224 : For ASE-CL
//---------------------------------------------------------------------------
extern PACKAGE TfPassword *fPassword;
extern PACKAGE TfPassword *fPassword2;       //Steven 20150505 : 剛好在輸入時,Alarm會卡死
//---------------------------------------------------------------------------
#endif
