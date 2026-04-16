//---------------------------------------------------------------------------

#ifndef MyTempPanelH
#define MyTempPanelH

#include <vcl.h>
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>

//---------------------------------------------------------------------------
class TMyTempPanel
{
    private:    // User declarations
        void __fastcall SetCaption(AnsiString Name);
        void __fastcall edBaseMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall edLimitMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall edIndiviMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall edinitialMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);    //kevin 20210421 initial temp 獨立範圍值
        AnsiString GetCaption();
    public:
        __fastcall TMyTempPanel(AnsiString Alias, int iTag);
        __fastcall ~TMyTempPanel();
        void __fastcall SetParent(TTabSheet *tsTemp);
        void __fastcall SetIndexTag(int iTag);
        void __fastcall SetEnable(bool bflag);

        int iIndexTag;
        TPanel    *palTemp;
        TLabel    *labName;
        TEdit     *edLow;            //設定值
        TEdit     *edMid;            //設定值
        TEdit     *edLowbase;        //設定值
        TEdit     *edBase;           //設定值
        TEdit     *edHighBase;       //設定值
        TEdit     *edSHighBase;                                                 //Ztex 2024.07.27 Add 6 Point Temperature Offset

        TEdit     *edKit_Low;            //設定值
        TEdit     *edKit_Mid;            //設定值
        TEdit     *edKit_Lowbase;        //設定值
        TEdit     *edKit_Base;           //設定值
        TEdit     *edKit_HighBase;       //設定值

        TEdit     *edOffset;         //設定值
        TEdit     *edSingleLimit;    //設定值
        TEdit     *edIndiTemp;       //設定值
        TEdit     *edInitTempOffset; //設定值
        TEdit     *edEOTTempOffset;  //設定值
        TPanel    *palLine;

        __property AnsiString   Caption={read=GetCaption,     write=SetCaption      };
        int        iOffsetByRecipeMaxLimit;
        int        iOffsetByRecipeMinLimit;

};

extern TList *listNormal;
extern TList *listArm1;
extern TList *listArm2;
//extern TList *listDut;
//extern TList *listNoUse;
//extern TList *listHeater;

#endif
