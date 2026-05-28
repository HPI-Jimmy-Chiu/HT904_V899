//---------------------------------------------------------------------------

#ifndef WinWaySettingH
#define WinWaySettingH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include "SPComm.hpp"
#include "ATC_WinWay.h"
#include "inifiles.hpp"

#define ATCWinWayPath    "D:\\HT9045\\config\\ATCWinWay.ini"
#define SiteNum 4
//---------------------------------------------------------------------------
// jimmychiu 20210903
class TfWinway : public TForm
{
__published:    // IDE-managed Components
        TGroupBox *grpcps;
        TLabel *lbl1;
        TLabel *lbl2;
        TLabel *lbl3;
        TLabel *lbl4;
        TLabel *lbl5;
        TComboBox *cbbBaudRate;
        TComboBox *cbbByteSize;
        TComboBox *cbbStopBit;
        TComboBox *cbbParity;
        TComboBox *cbbDevice;
        TComboBox *cbbWinwayATCIndex;
        TLabel *lblWinwayNumber;
        TSpeedButton *btnUpdate;
        TGroupBox *grpcommu;
        TLabel *lbl6;
        TEdit *edtSetTemp;
        TButton *btnSendTemp;
        TLabel *lbl7;
        TLabel *lblShowPT;
        TComm *WinWayATCComm1;
        TComm *WinWayATCComm2;
        TComm *WinWayATCComm3;
        TComm *WinWayATCComm4;
        TButton *btnGetPV;
        void __fastcall btnUpdateClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall cbbWinwayATCIndexChange(TObject *Sender);
        void __fastcall btnSendTempClick(TObject *Sender);
        void __fastcall btnGetPVClick(TObject *Sender);
        void __fastcall WinWayATCComm1ReceiveData(TObject *Sender,
          Pointer Buffer, WORD BufferLength);
    void __fastcall edtSetTempClick(TObject *Sender);
private:    // User declarations
public:     // User declarations
        __fastcall TfWinway(TComponent* Owner);
        //parameter
        int iWinwayATCIndex;
        ATC_WinWay* arrATC_Site[SiteNum];
        bool bShow;
        //function
        bool LoadCommData( TComm* Comm , int iIndex );
        bool SaveCommData( TComm* Comm , int iIndex );
        void ShowCommData( int iIndex );
        bool OpenCommPort( int _idx);
        void OpenCommPort();
        void SetST();
        bool CloseCommPort( int _idx);
        void SetTemprature(double _value);
        void SetTempratureAll(double _value);
};
//---------------------------------------------------------------------------
extern PACKAGE TfWinway *fWinway;
//---------------------------------------------------------------------------
#endif
