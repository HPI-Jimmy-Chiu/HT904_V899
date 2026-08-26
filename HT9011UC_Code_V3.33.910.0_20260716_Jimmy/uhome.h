//---------------------------------------------------------------------------
#ifndef uhomeH
#define uhomeH
//---------------------------------------------------------------------------
//#include "cmydef.h"

//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include "ALed.hpp"
#include <Vector>

using namespace std;
//---------------------------------------------------------------------------
class THomeClass  : public TComponent       //Stven 20120823 : Home元件改用Vector新增
{
    public:
        THomeClass(int MotNo, int Order, bool bView=true);
        TLabel  *labName;
        TEdit   *edPos;
        TALed   *ledHome;
        int     HomeOrder;
        int     index;
        int     THomeOrder;
        int     THomeFlag;
        bool    Visible;
};
//---------------------------------------------------------------------------
class TfHome : public TForm
{
__published:    // IDE-managed Components
     TPanel *Panel1;
     TPanel *Panel104;
    TPanel *Panel2;
    TLabel *Label26;
    TBevel *Bevel1;
    TSpeedButton *SpeedButton1;
    TTimer *Timer1;
    TPanel *Panel3;
    TSpeedButton *sbAbortHome;
    TListBox *ListBox1;
    TPanel *palUnUse;
    TLabel *Name;
    TALed *ledHome;
    TEdit *edPos;
    TPanel *pnlLine1;
    TPanel *pnlLine2;
    TPanel *pnlLine3;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall SpeedButton1Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall sbAbortHomeClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
private:    // User declarations
//     void InitialLed();
//     vector<TALed *> LedPtr;         //Steven 20120706 : 加入OCR與Y變距
//     vector<TEdit *> EditPtr;        //Steven 20120706 : 加入OCR與Y變距
//     bool fLedInitial;
public:     // User declarations
    __fastcall TfHome(TComponent* Owner);
    void ResetAllMotorLed();
    void ShowLed(int index,int attr);
    bool ShowMotorHomePos(int i);
    int TestZTask;
    int iHomeStep;
    bool fShow;
    bool fAbort;
    int Data;
    void __fastcall ScanKey();
    void  __fastcall InitDoTestZHome();
//    long __fastcall GetHomePitch(int GetMot);
    void InitGali_HomeTask();
    void GaliMotorServoOff(AnsiString sFunc);                                   //Steven 20230712 : 修正SwServoOn.Off時, 要抓住Z煞車
    void __fastcall RotateCheckClear();
    vector<THomeClass *>HomeClass;      //Stven 20120823 : Home元件改用Vector新增
    bool MoveInArmPitch_X(int *iSetPos, bool bCheckHome);                       //Ztex 2023.12.15 Add Pitch X Home Twice
    bool MoveOutArmPitch_X(int *iSetPos, bool bCheckHome);                      //Ztex 2023.12.15 Add Pitch X Home Twice
    void __fastcall InitialHomeClass();                                         //Steven 20240603 : 修正歸零馬達名稱消失問題
};
//---------------------------------------------------------------------------
extern PACKAGE TfHome *fHome;
bool ProcessMotorHome(bool Flag);
//ChungHung 20120110 add
bool ProcessSingleMotorHome(int Index);
void InitProcessSingleMotorTask(int Index);
bool PrePushLoaderCylinder(bool bReset=false);                                  //Steven 20150429 : 預先打兩下Loader汽缸
//extern int PrePushLifterCylinderTask[MAX_TRACK];
//---------------------------------------------------------------------------
#endif
