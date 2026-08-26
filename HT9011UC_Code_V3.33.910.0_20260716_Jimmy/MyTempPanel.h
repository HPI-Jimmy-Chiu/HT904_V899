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
        void __fastcall edinitialMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);    //kevin 20210421 initial temp 縒ミ絛瞅
        void __fastcall edBySetupOffSetMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);    //Ifor 20231219 add Use By Setup Offset Range
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
        TEdit     *edLow;            //砞﹚
        TEdit     *edMid;            //砞﹚
        TEdit     *edLowbase;        //砞﹚
        TEdit     *edBase;           //砞﹚
        TEdit     *edHighBase;       //砞﹚
        TEdit     *edSHighBase;                                                 //Ztex 2024.07.27 Add 6 Point Temperature Offset
        //==> Eastsun 20260526 #026-4.A7 Ifor 20241101 add :KYEC
        TEdit     *edPreOffset;      //箇干纕
        TEdit     *edPreOfsTime;     //干纕计
        TEdit     *edAfterOfs;       //干纕计挡干纕
        //<== Eastsun 20260526 #026-4.A7

        TEdit     *edKit_Low;            //砞﹚
        TEdit     *edKit_Mid;            //砞﹚
        TEdit     *edKit_Lowbase;        //砞﹚
        TEdit     *edKit_Base;           //砞﹚
        TEdit     *edKit_HighBase;       //砞﹚

        TEdit     *edOffset;         //砞﹚
        TEdit     *edSingleLimit;    //砞﹚
        TEdit     *edIndiTemp;       //砞﹚
        TEdit     *edInitTempOffset; //砞﹚
        TEdit     *edEOTTempOffset;  //砞﹚
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
