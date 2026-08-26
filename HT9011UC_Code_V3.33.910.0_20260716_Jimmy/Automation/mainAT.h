//---------------------------------------------------------------------------

#ifndef mainATH
#define mainATH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "APHeadLabel.hpp"
#include <ComCtrls.hpp>
bool bLockByServer;
//---------------------------------------------------------------------------
class TfMainAT : public TForm
{
__published:    // IDE-managed Components
    TGroupBox *GroupBox1;
    TComboBox *cbMachineModel;
    TLabel *Label1;
    TLabel *Label2;
    TEdit *edMachineID;
    TLabel *Label3;
    TEdit *enVersion;
    TLabel *Label4;
    TComboBox *cbConnectStatus;
    TLabel *Label5;
    TComboBox *cbHandlerStatus;
    TLabel *lbSystemStatus;
    TGroupBox *GroupBox2;
    TAPHeadLabel *APHeadLabel1;
    TAPHeadLabel *APHeadLabel2;
    TAPHeadLabel *APHeadLabel3;
    TAPHeadLabel *APHeadLabel4;
    TAPHeadLabel *APHeadLabel5;
    TAPHeadLabel *APHeadLabel6;
    TAPHeadLabel *APHeadLabel7;
    TAPHeadLabel *APHeadLabel8;
    TAPHeadLabel *APHeadLabel9;
    TAPHeadLabel *APHeadLabel10;
    TEdit *edTemp1;
    TEdit *edTemp2;
    TEdit *edTemp3;
    TEdit *edTemp4;
    TEdit *edTemp5;
    TEdit *edTemp6;
    TEdit *edTemp7;
    TEdit *edTemp8;
    TEdit *edTemp9;
    TEdit *edTemp10;
    TGroupBox *GroupBox3;
    TLabel *Label29;
    TEdit *edtSoakTime;
    TLabel *Label28;
    TEdit *edtTemperature;
    TCheckBox *cTempmode;
    TGroupBox *GroupBox4;
    TCheckBox *cConnectionTester;
    TRadioGroup *rgTesterMode;
    TGroupBox *gbCategory;
    TGroupBox *gbMapping;
    TGroupBox *gbDutOn;
    TComboBox *cbSetUpFileName;
    TLabel *Label30;
    TGroupBox *gbTrayForm;
    TGroupBox *gbPlateForm;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TEdit *Edit1;
    TEdit *Edit2;
    TEdit *Edit3;
    TEdit *Edit4;
    TEdit *Edit5;
    TEdit *Edit6;
    TLabel *Label12;
    TLabel *Label13;
    TLabel *Label14;
    TLabel *Label15;
    TLabel *Label16;
    TLabel *Label17;
    TEdit *Edit7;
    TEdit *Edit8;
    TEdit *Edit9;
    TEdit *Edit10;
    TEdit *Edit11;
    TEdit *Edit12;
private:    // User declarations
public:     // User declarations
    __fastcall TfMainAT(TComponent* Owner);
    void ShowOLPState(int iState);
    TLabel *lbCategory[16];
    TLabel *lbSiteMapping[16];
    TLabel *lbDutOnOff[16];
    TComboBox *cbCategory[16];
    TComboBox *cbSiteMapping[16];
    TComboBox *cbDutOnOff[16];
    TEdit *edTrayForm[6];
    TEdit *edPlateForm[6];
};
//---------------------------------------------------------------------------
extern PACKAGE TfMainAT *fMainAT;
//---------------------------------------------------------------------------
AnsiString GetMainStatus();
AnsiString GetHandlerType();
AnsiString GetDataPath();
AnsiString GetSoftwareVersion();
AnsiString GetSetUpFileName();
AnsiString GetActTemp(int i);
AnsiString GetJamCount();
AnsiString GetSoakTime();
AnsiString GetTemperature();
AnsiString GetTempMode();
AnsiString GetTesterConnect();
AnsiString GetTesterMode();
AnsiString GetLoadCount();

int GetCategory(AnsiString *Data);
int GetDutOnOff(AnsiString *Data);
int GetMapping(AnsiString *Data);
int GetTrayForm(AnsiString *Data);
int GetPlateForm(AnsiString *Data);
int GetProductivity(AnsiString *Data);
int GetSortingCount(AnsiString *Data);
int GetSocketCount(AnsiString *Data);
int GetHeadCount(AnsiString *Data);
int GetTestResult(AnsiString *Data);

int SetCategory(AnsiString *Data);
int SetDutOnOff(AnsiString *Data);
int SetMapping(AnsiString *Data);
int SetSetUpFileName(AnsiString *Data);
int SetSoakTime(AnsiString *Data);
int SetTemperature(AnsiString *Data);
int SetTempMode(AnsiString *Data);
int SetTesterConnect(AnsiString *Data);
int SetTesterMode(AnsiString *Data);

int ShowOLPState(int iState);
void DoOnLineReply(AnsiString *Data);
void DoULRequest(AnsiString *Data);
int  DoDLRequest(AnsiString sDLFileName);
void DoClearReportRequest(int iSwitchCase);
void AlarmMessage(AnsiString enMess, AnsiString chMess);
bool CheckCanChangeRealDummy();
bool CheckInitialOK();
bool CheckSystemStart();
bool CheckNeedCleanOut();
#endif
