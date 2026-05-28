//---------------------------------------------------------------------------

#ifndef HTEditH
#define HTEditH
//---------------------------------------------------------------------------
#include <vcl.h>
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>

//---------------------------------------------------------------------------
enum TEditContent
{
    ECText=0,           //純文字
    ECInteger=1,        //數字
    ECDouble=2,         //浮點數
    ECPosInt=3,         //正整數
    ECPosDouble=4,      //正浮點數
    ECNegInt=5,         //負數
    ECNegDouble=6,      //負浮點數
    ECFileName=7,       //檔案名稱或路徑
    ECPassword=8,       //密碼
    ECPercent=9,        //百分比
    ECPort=10,          //通訊埠
    ECIPAddr=11,        //IP
    ECBool=12           //布林值       //Steven 20230224 : 加入支援布林值
};
enum TEditUnit                                                                  //Steven 20230905 : HTEditList加入型態轉換的Flag
{
    EUNone      =0,     //不做任何處理
    EUuMToMM    =1,     //顯示為MM, 存檔為uM
    EUMSToSec   =2      //顯示為Sec, 存檔為MS
};
//---------------------------------------------------------------------------
class THTEdit:TObject //: public TEdit
{
private:
    Variant         FParemeter;
    TPopupMenu      *DisablePopupMenu;
    AnsiString      HTIniGroupName;
    AnsiString      HTIniKeyName;
    TEditContent    HTContentType;
    AnsiString      HTMax;
    AnsiString      HTMin;
    AnsiString      HTCustomMax;
    AnsiString      HTCustomMin;
    AnsiString      HTDefaultValue;
    bool            HTCheckRange;
    void __fastcall SetPath(AnsiString P);
    void __fastcall SetFileName(AnsiString P);
    void __fastcall SetIniGroupName(AnsiString P);
    void __fastcall SetIniKeyName(AnsiString P);
    void __fastcall SetContentType(TEditContent P);
    void __fastcall SetMax(AnsiString Max);
    void __fastcall SetMin(AnsiString Min);
    void __fastcall SetCustomMax(AnsiString Max);
    void __fastcall SetCustomMin(AnsiString Min);
    void __fastcall SetDefaultValue(AnsiString P);
    void __fastcall SetCheckRange(bool P);
    Variant __fastcall ReadParamter();
    void __fastcall SetParamter(Variant VData);

    //事件處理-----------
    Classes::TNotifyEvent   FOldOnClickEvent;
    TKeyPressEvent          FOldKeyPressEvent;
    Classes::TNotifyEvent   FOldOnChange;

protected:
//    DYNAMIC   void   __fastcall   Click();

public:
    __fastcall THTEdit();
    __fastcall ~THTEdit();
//    bool SaveToFile();        //改從List處理,這樣只要開一次檔案
//    bool ReadFromFile();
    void __fastcall SetDefaultLimitation(AnsiString Max, AnsiString Min);
    void __fastcall SetCustomLimitation(AnsiString Max, AnsiString Min);
    void __fastcall SetFileInformation(AnsiString GroupName, AnsiString KeyName);
    void ChangeProperty(bool Visible, bool Enable, bool ReadFromFile, AnsiString Default);
    void ChangeProperty(bool Visible, bool Enable, bool ReadFromFile, bool Default);

    void __fastcall EditIntKeyPress(TObject *Sender, char &Key);
    void __fastcall EditFloatKeyPress(TObject *Sender, char &Key);
    void __fastcall EditPathKeyPress(TObject *Sender, char &Key);
    void __fastcall EditClick(TObject *Sender);
    void __fastcall EditChange(TObject *Sender);
    void OnWriteEdit(TCustomEdit *SourceEdit);      //Steven 20230224 : Edit List改支援多型態元件 TEdit --> TCustomEdit
    void __fastcall DisableEventOverlap();  //取消覆蓋相關Event,也就是使用Edit本身的OnClick或OnChange Event

    __property AnsiString   IniGroupName    ={read=HTIniGroupName,  write=SetIniGroupName   };
    __property AnsiString   IniKeyName      ={read=HTIniKeyName,    write=SetIniKeyName     };
    __property TEditContent Content         ={read=HTContentType,   write=SetContentType    };
    __property AnsiString   MaxValue        ={read=HTMax,           write=SetMax            };
    __property AnsiString   MinValue        ={read=HTMin,           write=SetMin            };
    __property AnsiString   CustomMaxValue  ={read=HTCustomMax,     write=SetCustomMax      };
    __property AnsiString   CustomMinValue  ={read=HTCustomMin,     write=SetCustomMin      };
    __property AnsiString   DefaultValue    ={read=HTDefaultValue,  write=SetDefaultValue   };
    __property bool         CheckRange      ={read=HTCheckRange,    write=SetCheckRange     };
//    __property TEdit*       SourceEdit      ={read=FSourceEdit,     write=OnWriteEdit       };

    TControl    *SourceControl;         //Steven 20230224 : Edit List改支援多型態元件 TEdit --> TControl
    bool        *bParameter;            //Steven 20230502 : 修正bool沒有跟著存讀檔
    int         *iParameter;
    double      *dParameter;
    AnsiString  *sParameter;
    Variant     vParameter;
    TWinControl *Par;
    int  iTag;
    bool bEnable;
    bool bVisible;
    bool bReadFromFile;         //是否從檔案讀取
    int  iBarcodeReadType;      //是否要用Barcode檢查權限
    int  iTransformType;        //是否使用型態轉換 TEditUnit
//    __property Variant      Parameter       ={read=ReadParamter,    write=SetParamter       };
//    bool       bSaveFile;
    int iTop;
    int iLeft;
    int iHeight;
    int iWidth;
    void GetDefaultPosition();
    void SetToDefaultPosition();
};
//---------------------------------------------------------------------------
//class THTCheckBox//:TObject //: public
//{
//private:
//    bool            FParemeter;
//    TPopupMenu      *DisablePopupMenu;
//    AnsiString      HTIniGroupName;
//    AnsiString      HTIniKeyName;
//    bool            HTDefaultValue;
//    void __fastcall SetIniGroupName(AnsiString P);
//    void __fastcall SetIniKeyName(AnsiString P);
//    void __fastcall SetDefaultValue(bool P);
//    bool __fastcall ReadParamter();
//    void __fastcall SetParamter(bool VData);
//
//protected:
////    DYNAMIC   void   __fastcall   Click();
//
//public:
//    __fastcall THTCheckBox();
//    __fastcall ~THTCheckBox();
//    void __fastcall SetFileInformation(AnsiString GroupName, AnsiString KeyName);
//    void ChangeProperty(bool Visible, bool Enable, bool ReadFromFile, bool ForceValue);
//
//    __property AnsiString   IniGroupName    ={read=HTIniGroupName,  write=SetIniGroupName   };
//    __property AnsiString   IniKeyName      ={read=HTIniKeyName,    write=SetIniKeyName     };
//    __property bool         DefaultValue    ={read=HTDefaultValue,  write=SetDefaultValue   };
//    TCheckBox   *CheckBox;
//    bool        *bParameter;
//    bool bEnable;
//    bool bVisible;
//    bool bReadFromFile;        //是否從檔案讀取
//    bool bForceValue;          //如果不是從檔案讀取的預設值
//    TWinControl *Par;
//    int iTag;
//    int iTop;
//    int iLeft;
//    int iHeight;
//    int iWidth;
//    void GetDefaultPosition();
//    void SetToDefaultPosition();
//};
//---------------------------------------------------------------------------
#endif
