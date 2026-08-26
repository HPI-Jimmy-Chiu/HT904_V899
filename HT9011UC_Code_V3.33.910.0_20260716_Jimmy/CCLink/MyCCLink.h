#ifndef MyCCLinkH
#define MyCCLinkH
#include "MyCCLinkBoard.h"
//---------------------------------------------------------------------------
class PACKAGE TMyCCLinkCtrl : public TComponent
{
    private:
        int  iNucl1No;
        bool iCCLinkStatus;
        AnsiString ErrMess;
        AnsiString SysMess;
        long  lPath;
        bool bSetValue;
        bool bInitialOK;
        bool bHasErr;
        bool bReady;
    protected:
        TTimer *Timer1;
        int iCCLinkTask;
        TQPF_Timer CCLinkTimer;

        //Steven 20100517 Start: 為了不要#include "cpublic.h"
//        unsigned char T_HEX2ASCII_Mac(unsigned char hex2ascii);
//        unsigned char T_ASXII2HEX_Mac(unsigned char ascii2hex);

        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall WriteCCLinkStatus(bool Value);
    public:
        __fastcall TMyCCLinkCtrl::TMyCCLinkCtrl(TComponent* Owner);
        __fastcall TMyCCLinkCtrl::~TMyCCLinkCtrl();
        bool bSetMultiValue;
        TMyCCLinkBoard *CCLinkBoard;
        void DoSetValue(short sNu, short sSen, int Value);    //針對單一Sensor設定值
        void DoSetMultiValue(short sNu, short sSen, int *Value, int Count);    //針對多個Sensor設定值
    __published:
        __property bool         HasError        ={read=bHasErr,       write=bHasErr,            default=false};   //是否有錯誤
        __property bool         CCLinkStatus    ={read=iCCLinkStatus, write=WriteCCLinkStatus,  default=false};   //系統狀態
        __property long         Path            ={read=lPath,         write=lPath,              default=81};      //系統位置
        __property int          Nucl1No         ={read=iNucl1No,      write=iNucl1No,           default=2};       //NuCl1的個數
        __property AnsiString   ErrorMessage    ={read=ErrMess,       write=ErrMess,            default=NULL};
        __property AnsiString   SystemMessage   ={read=SysMess,       write=SysMess,            default=NULL};    //錯誤訊息
        __property bool         SetValue        ={read=bSetValue,     write=bSetValue,          default=false};   //開始寫入
        __property bool         InitialOK       ={read=bInitialOK,    write=bInitialOK,         default=false};
        __property bool         Ready           ={read=bReady,        write=bReady,             default=false};
};
#endif
