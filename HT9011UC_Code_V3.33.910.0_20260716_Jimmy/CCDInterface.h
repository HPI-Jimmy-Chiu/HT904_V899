//---------------------------------------------------------------------------

#ifndef CCDInterfaceH
#define CCDInterfaceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
#define COMMAND_LENGTH      6
#define CCD_PASS            1
#define CCD_FAIL            0
#define CCD_YES             1
#define CCD_NO              0
#define ALL_TESTER          1
//---------------------------------------------------------------------------
#define HTCD_EXISTENCE      100 //程式是否存在.
#define HTCD_FILE_READY     101 //辨識檔案是否存在.
#define HTCD_IDENTIFICATION 102 //辨識.
#define HTCD_END            103 //結束.
#define HTCD_GRAB           104 //擷取.
#define HTCD_Setup          105 //設定檔進入CCD  kevin 20110722
//---------------------------------------------------------------------------
#define CDHT_EXISTENCE      200
#define CDHT_FILE_READY     201
#define CDHT_RESULT         202
#define CDHT_ALARM_CLOSE    203
#define CDHT_GRAB_FINISH    204 //擷取完成.
//---------------------------------------------------------------------------

class TCCDInterfaceForm : public TForm
{
__published:    // IDE-managed Components
    TMemo *Memo;
    TButton *Button1;
    TButton *Button2;
    TButton *Button3;
    TButton *Button4;
    TEdit *Edit1;
    TButton *Button5;
    TTimer *Timer1;
    TGroupBox *GroupBox1;
    TComboBox *CountCBox;
    TButton *Button6;
    TEdit *edSavePath;
    TLabel *Label1;
    TLabel *Label2;
    TComboBox *VideoCBox;
    TTimer *Timer2;
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall Button3Click(TObject *Sender);
    void __fastcall Button4Click(TObject *Sender);
    void __fastcall Button5Click(TObject *Sender);
    void __fastcall MemoDblClick(TObject *Sender);
    void __fastcall Button6Click(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:    // User declarations
    //void __fastcall ServerSocketClientRead();  kevin 20110811
    HANDLE hFileMap;
public:     // User declarations
    __fastcall TCCDInterfaceForm(TComponent* Owner);
    void __fastcall CloseCCDForm();
    int iStep;
    int BinData[8];
    int iIdentificationStatus;
    char pucSendBuf[8192];
    char pucInBuff[8192];
    //bool TesterOnline[4];
    //bool ReceiveData[4];
    bool bNeedTest[4];
    bool bCCDProgramExistence;
    bool bCCDFileReady;
    bool bIdentificationFinish;
    bool bCCDProgramRunning;
    bool bWakeupCCDProgram;
    bool bGrabImageFinish;
    String sClientAlias[10];
    String sClientAddress[10];

    int __fastcall  GetCommandCode(char *pucInBuff);
    int __fastcall  GetClientIndexByAlias(String Alias);
    int __fastcall  GetClientIndexByaddress(String Address);
    int __fastcall  GetClientIndexByID(int iTesterIndex);
    int __fastcall  RecordClientAlias(String Address, char *pucInBuff, int iLength);
    void __fastcall RemoveClientAlias(String Address);
    void __fastcall SendDataByAlias(int iTesterIndex, int Command, String sData);
    void __fastcall ScanCCDProgram(bool bEnableSet); //kevin 20110801 判斷是否可進入
    void __fastcall WakeupCCD();
    void __fastcall CCDIdentification();
    void __fastcall CCDIdentificationOpen();
    void __fastcall RetrunUpStep();
    void __fastcall CCDTimerOnOff(bool bOn);     //Steven 20110809 : 決定要不要開啟Timer
    bool __fastcall CCDRunExec(); //KEVIN 20110811 程式執行較順暢
    void __fastcall ServerSocketClientRead(char *cDataCCD,int iCommandCode);//kevin 20110811

    bool bAtestScanCCDProgram;//kevin 20110811
};
//---------------------------------------------------------------------------
extern PACKAGE TCCDInterfaceForm *CCDInterfaceForm;
//---------------------------------------------------------------------------
#endif
