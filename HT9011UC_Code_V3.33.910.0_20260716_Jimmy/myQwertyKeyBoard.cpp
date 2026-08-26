#include "MachineDefine.h"
#pragma hdrstop

#include "myQwertyKeyBoard.h"

#include "MachineType.h"
//
//#ifdef HT9045
//    #include "MachineDefine.h"
//#endif
#include "cmydef.h"
#include "common.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//新增至cMydef.h ==============
//    const int  N_INTEGER   =0x0001; //只有整數
//    const int  N_DOUBLE    =0x0002; //只有浮點數
//    const int  N_NO_SYMBOL =0x0004; //沒有特殊符號
//    const int  N_PASSWORD  =0x0008; //密碼文
//    const int  N_NO_SPACE  =0x0010; //無空白鍵
//    const int  N_UPPERCASE =0x0020; //大寫優先
//    const int  N_NO_NUM_PAD=0x0040; //不需要數字鍵
//    const int  N_PORT      =0x0080; //通訊埠 //Steven 20230221 : 小鍵盤新增通訊埠上下限
//    const int  N_IP_ADDR   =0x0100; //IP位置

//新增至MachineDefine.h========
//    template<class T>
//    T CheckRange(const T Value, const T Maximum, const T Minimum)
//    {
//        if(Maximum<Minimum) //如果大小相反
//        {
//            if(Value>Minimum)
//                return Minimum;
//            else if(Value<Maximum)
//                return Maximum;
//            else
//                return Value;
//        }
//        else
//        {
//            if(Value>Maximum)
//                return Maximum;
//            else if(Value<Minimum)
//                return Minimum;
//            else
//                return Value;
//        }
//    };
//=============================

TfQwertyKey *fQwertyKey;
TfQwertyKey *fQwertyKey2;                                                       //Steven 20150505 : 剛好在輸入時,Alarm會卡死
//---------------------------------------------------------------------------
TQwertyKeyClass::TQwertyKeyClass(AnsiString sUpper, AnsiString sLower, TSpeedButton *SPB, eKeyType Type)
{
    UpperCase=sUpper;
    LowerCase=sLower;
    spbKey   =SPB;
    KeyType  =Type;
}
//---------------------------------------------------------------------------
__fastcall TfQwertyKey::TfQwertyKey(TComponent* Owner)
    : TForm(Owner)
{
    bKey2=false;
    bShow=false;
    iDecimalPoint=0;
    bNoSymbol    =false;
    QwertyKey.push_back(new TQwertyKeyClass("1", "!",  speedbNo01, eKeyNumAndSymbol));
    QwertyKey.push_back(new TQwertyKeyClass("2", "@",  speedbNo02, eKeyNumAndSymbol));
    QwertyKey.push_back(new TQwertyKeyClass("3", "#",  speedbNo03, eKeyNumAndSymbol));
    QwertyKey.push_back(new TQwertyKeyClass("4", "$",  speedbNo04, eKeyNumAndSymbol));
    QwertyKey.push_back(new TQwertyKeyClass("5", "%",  speedbNo05, eKeyNumAndSymbol));
    QwertyKey.push_back(new TQwertyKeyClass("6", "^",  speedbNo06, eKeyNumAndSymbol));
    QwertyKey.push_back(new TQwertyKeyClass("7", "&&", speedbNo07, eKeyNumAndSymbol));
    QwertyKey.push_back(new TQwertyKeyClass("8", "*",  speedbNo08, eKeyNumAndSymbol));
    QwertyKey.push_back(new TQwertyKeyClass("9", "(",  speedbNo09, eKeyalphabet));                                      //Steven 20210805 : eKeyNumAndSymbol --> eKeyalphabet
    QwertyKey.push_back(new TQwertyKeyClass("0", ")",  speedbNo00, eKeyalphabet));                                      //10    //Steven 20210805 : eKeyNumAndSymbol --> eKeyalphabet
    QwertyKey.push_back(new TQwertyKeyClass("-", "_",  speedbSyb_, eKeyalphabet));                                      //Steven 20210805 : eKeyNumAndSymbol --> eKeyalphabet
    QwertyKey.push_back(new TQwertyKeyClass("=", "+",  speedbSyPl, eKeyalphabet));                                      //Steven 20210805 : eKeyNumAndSymbol --> eKeyalphabet
    QwertyKey.push_back(new TQwertyKeyClass("q", "Q",  speedbChrQ, eKeyalphabet));
    QwertyKey.push_back(new TQwertyKeyClass("w", "W",  speedbChrW, eKeyalphabet));
    QwertyKey.push_back(new TQwertyKeyClass("e", "E",  speedbChrE, eKeyalphabet));
    QwertyKey.push_back(new TQwertyKeyClass("r", "R",  speedbChrR, eKeyalphabet));
    QwertyKey.push_back(new TQwertyKeyClass("t", "T",  speedbChrT, eKeyalphabet));
    QwertyKey.push_back(new TQwertyKeyClass("y", "Y",  speedbChrY, eKeyalphabet));
    QwertyKey.push_back(new TQwertyKeyClass("u", "U",  speedbChrU, eKeyalphabet));
    QwertyKey.push_back(new TQwertyKeyClass("i", "I",  speedbChrI, eKeyalphabet));                                      //20
    QwertyKey.push_back(new TQwertyKeyClass("o", "O",  speedbChrO, eKeyalphabet));
    QwertyKey.push_back(new TQwertyKeyClass("p", "P",  speedbChrP, eKeyalphabet));
    QwertyKey.push_back(new TQwertyKeyClass("[", "{",  speedbSybL, eKeyalphabet));                                      //Steven 20210805 : eKeyNumAndSymbol --> eKeyalphabet
    QwertyKey.push_back(new TQwertyKeyClass("]", "}",  speedbSybR, eKeyalphabet));                                      //Steven 20210805 : eKeyNumAndSymbol --> eKeyalphabet
    QwertyKey.push_back(new TQwertyKeyClass("\\", "|", speedbSyOr, eKeySymbol));
    QwertyKey.push_back(new TQwertyKeyClass("a", "A",  speedbChrA, eKeyalphabet));
    QwertyKey.push_back(new TQwertyKeyClass("s", "S",  speedbChrS, eKeyalphabet));
    QwertyKey.push_back(new TQwertyKeyClass("d", "D",  speedbChrD, eKeyalphabet));
    QwertyKey.push_back(new TQwertyKeyClass("f", "F",  speedbChrF, eKeyalphabet));
    QwertyKey.push_back(new TQwertyKeyClass("g", "G",  speedbChrG, eKeyalphabet));                                      //30
    QwertyKey.push_back(new TQwertyKeyClass("h", "H",  speedbChrH, eKeyalphabet));
    QwertyKey.push_back(new TQwertyKeyClass("j", "J",  speedbChrJ, eKeyalphabet));
    QwertyKey.push_back(new TQwertyKeyClass("k", "K",  speedbChrK, eKeyalphabet));
    QwertyKey.push_back(new TQwertyKeyClass("l", "L",  speedbChrL, eKeyalphabet));
    QwertyKey.push_back(new TQwertyKeyClass(";", ":",  speedbSyb1, eKeySymbol));
    QwertyKey.push_back(new TQwertyKeyClass("'", "\"", speedbSyb2, eKeySymbol));
    QwertyKey.push_back(new TQwertyKeyClass("z", "Z",  speedbChrZ, eKeyalphabet));
    QwertyKey.push_back(new TQwertyKeyClass("x", "X",  speedbChrX, eKeyalphabet));
    QwertyKey.push_back(new TQwertyKeyClass("c", "C",  speedbChrC, eKeyalphabet));
    QwertyKey.push_back(new TQwertyKeyClass("v", "V",  speedbChrV, eKeyalphabet));                                      //40
    QwertyKey.push_back(new TQwertyKeyClass("b", "B",  speedbChrB, eKeyalphabet));
    QwertyKey.push_back(new TQwertyKeyClass("n", "N",  speedbChrN, eKeyalphabet));
    QwertyKey.push_back(new TQwertyKeyClass("m", "M",  speedbChrM, eKeyalphabet));
    QwertyKey.push_back(new TQwertyKeyClass(",", "<",  speedbSyb4, eKeySymbol));
    QwertyKey.push_back(new TQwertyKeyClass(".", ">",  speedbSyb5, eKeySymbol));
    QwertyKey.push_back(new TQwertyKeyClass("/", "?",  speedbSyb6, eKeySymbol));
    QwertyKey.push_back(new TQwertyKeyClass("`", "~",  speedbSyb3, eKeySymbol));                                        //47
    QwertyKey.push_back(new TQwertyKeyClass(" ", " ",  speedbSpac, eKeySymbol));                                        //48
    QwertyKey.push_back(new TQwertyKeyClass("1", "1",  speedbNo11, eKeyNumOnly));
    QwertyKey.push_back(new TQwertyKeyClass("2", "2",  speedbNo12, eKeyNumOnly));
    QwertyKey.push_back(new TQwertyKeyClass("3", "3",  speedbNo13, eKeyNumOnly));
    QwertyKey.push_back(new TQwertyKeyClass("4", "4",  speedbNo14, eKeyNumOnly));
    QwertyKey.push_back(new TQwertyKeyClass("5", "5",  speedbNo15, eKeyNumOnly));
    QwertyKey.push_back(new TQwertyKeyClass("6", "6",  speedbNo16, eKeyNumOnly));
    QwertyKey.push_back(new TQwertyKeyClass("7", "7",  speedbNo17, eKeyNumOnly));
    QwertyKey.push_back(new TQwertyKeyClass("8", "8",  speedbNo18, eKeyNumOnly));
    QwertyKey.push_back(new TQwertyKeyClass("9", "9",  speedbNo19, eKeyNumOnly));
    QwertyKey.push_back(new TQwertyKeyClass("0", "0",  speedbNo10, eKeyNumOnly));

    bUpperCase=false;
    for(unsigned int i=0; i<QwertyKey.size(); i++)
    {
        QwertyKey[i]->spbKey->OnClick=spbKeyClick;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfQwertyKey::FormShow(TObject *Sender)
{
    bShow=true;
    palContent->SetFocus();
    edQwertyContent->SetFocus();
//    edQwertyContent->SelStart=edQwertyContent->Text.Length();   //把游標移到最後面
}
//---------------------------------------------------------------------------
void __fastcall TfQwertyKey::FormClose(TObject *Sender,
      TCloseAction &Action)
{
     bShow=false;
     bKey2=false;                                                               //Steven 20150505 : 剛好在輸入時,Alarm會卡死
}
//---------------------------------------------------------------------------
void __fastcall TfQwertyKey::FormDestroy(TObject *Sender)
{
    try
    {
        for(vector<TQwertyKeyClass *>::iterator iter=QwertyKey.begin(); iter!=QwertyKey.end(); ++iter)
        {
            delete *iter;
        }
        vec_clr(QwertyKey);
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfQwertyKey::FormDestroy");
    }
    LogSoftwareOffTime("TfQwertyKey, FormDestroy");                             //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void __fastcall TfQwertyKey::ShowQwertyKey(TWinControl *Ptr, int iFunction, int iDP, bool bCheckRange, double min, double max)
{
    if(fQwertyKey->bShow==true &&
       fQwertyKey2->bKey2==false &&
       fQwertyKey2->bShow==false)                                               //Steven 20150505 : 剛好在輸入時,Alarm會卡死
    {
        fQwertyKey2->bKey2=true;
        fQwertyKey2->ShowQwertyKey(Ptr, iFunction, iDP, bCheckRange, min, max);
        return;
    }

    if(bShow==true)
        return;

    char c;
    bIntegerOnly=(iFunction&N_INTEGER);                                         //Wei 20190626 改位置
    iDecimalPoint=iDP;
    KeyCode      =iFunction;
    ChangeDecimalPoint();
    palQwertyKey ->Visible=true;
    palNumKey    ->Visible=true;
    spbBackSpace2->Visible=((iFunction&N_INTEGER || iFunction&N_DOUBLE || iFunction&N_PORT || iFunction&N_IP_ADDR));
    spbClear2    ->Visible=((iFunction&N_INTEGER || iFunction&N_DOUBLE || iFunction&N_PORT || iFunction&N_IP_ADDR));
    spbCancel2   ->Visible=((iFunction&N_INTEGER || iFunction&N_DOUBLE || iFunction&N_PORT || iFunction&N_IP_ADDR));
    spbSummit2   ->Visible=((iFunction&N_INTEGER || iFunction&N_DOUBLE || iFunction&N_PORT || iFunction&N_IP_ADDR));
    spbAdd1      ->Visible=((iFunction&N_INTEGER || iFunction&N_DOUBLE) && !(iFunction&N_PASSWORD));
    spbAdd10     ->Visible=((iFunction&N_INTEGER || iFunction&N_DOUBLE) && !(iFunction&N_PASSWORD));
    spbAdd100    ->Visible=((iFunction&N_INTEGER || iFunction&N_DOUBLE) && !(iFunction&N_PASSWORD));
    spbMinus1    ->Visible=((iFunction&N_INTEGER || iFunction&N_DOUBLE) && !(iFunction&N_PASSWORD));
    spbMinus10   ->Visible=((iFunction&N_INTEGER || iFunction&N_DOUBLE) && !(iFunction&N_PASSWORD));
    spbMinus100  ->Visible=((iFunction&N_INTEGER || iFunction&N_DOUBLE) && !(iFunction&N_PASSWORD));
    spbDP        ->Visible=((iFunction&N_IP_ADDR || iFunction&N_DOUBLE) && !(iFunction&N_PASSWORD));
    spbPercent   ->Visible=((iFunction&N_INTEGER || iFunction&N_DOUBLE) && !(iFunction&N_PASSWORD));
    spbMinus     ->Visible=((iFunction&N_INTEGER || iFunction&N_DOUBLE) && !(iFunction&N_PASSWORD));
    this   ->Top    =((iFunction&N_INTEGER || iFunction&N_DOUBLE || iFunction&N_PORT || iFunction&N_IP_ADDR) && !(iFunction&N_PASSWORD))?200:400;
    this   ->Left   =((iFunction&N_INTEGER || iFunction&N_DOUBLE || iFunction&N_PORT || iFunction&N_IP_ADDR) && !(iFunction&N_PASSWORD))?500:100;

    c=edQwertyContent->PasswordChar;
    if(iFunction&N_INTEGER || iFunction&N_DOUBLE ||                             //只有數字
       iFunction&N_PORT    || iFunction&N_IP_ADDR)
    {
        palNumKey->Visible=true;
        palQwertyKey->Visible=false;
        this->Width=290;
    }
    else
    {
        this      ->Width               =(iFunction&N_NO_NUM_PAD)?745:910;
        speedbSpac      ->Visible       =!(iFunction&N_NO_SPACE);               //空白鍵
        bUpperCase                      =(iFunction&N_UPPERCASE);               //大寫優先
        edQwertyContent ->PasswordChar  =(iFunction&N_PASSWORD)?'*':c;          //密碼文

        palNumKey       ->Visible       =!(iFunction&N_NO_NUM_PAD);
        bNoSymbol                       =(iFunction&N_NO_SYMBOL);               //不能有符號
        spbChangeCase->Click();
    }

    TPanel       *PanelPtr  = dynamic_cast <TPanel *>       (Ptr);
    TEdit        *EditPtr   = dynamic_cast <TEdit *>        (Ptr);
    TLabeledEdit *LabEditPtr= dynamic_cast <TLabeledEdit *> (Ptr);              //Steven 20200602 : add 使用TLabeledEdit

    if(PanelPtr!=NULL)
    {
        sBackup                 =PanelPtr->Caption;
        edQwertyContent->Text   =PanelPtr->Caption;
        edCurrValue->Text       =PanelPtr->Caption;
    }
    else if(EditPtr!=NULL)
    {
        sBackup                 =EditPtr->Text;
        edQwertyContent->Text   =EditPtr->Text;
        edCurrValue->Text       =EditPtr->Text;
    }
    else if(LabEditPtr!=NULL)
    {
        sBackup                 =LabEditPtr->Text;
        edQwertyContent->Text   =LabEditPtr->Text;
        edCurrValue->Text       =LabEditPtr->Text;
    }

    if(iFunction&N_PORT)                                                        //Steven 20230221 : 小鍵盤新增通訊埠上下限
    {
        bCheckRange=true;
        if(min<0 || max<=0)
        {
            min=0;
            max=65535;
        }
    }

    if(bCheckRange)                                                             //Steven 20140520 : fQwertyKey加入目前值,最大最小值顯示
    {
        if(max>min)
        {
            edMaxValue->Text=AnsiString(max);
            edMinValue->Text=AnsiString(min);
        }
        else
        {
            edMaxValue->Text=AnsiString(min);
            edMinValue->Text=AnsiString(max);
        }
    }

    palValueLimit->Visible=!(iFunction&N_PASSWORD);                             //Steven 20140609 : 輸入密碼時,不顯示CurrentValue

    labMinValue->Visible=bCheckRange;
    labMaxValue->Visible=bCheckRange;
    edMaxValue->Visible=bCheckRange;
    edMinValue->Visible=bCheckRange;
    palValueLimit->Height=(bCheckRange)?140:48;
    this->Height=(bCheckRange)?503:411;

    this->Height=(iFunction&N_PASSWORD)?365:this->Height;                       //Steven 20140609 : 輸入密碼時,不顯示CurrentValue
    this->ShowModal();

    if(iFunction&N_INTEGER || iFunction&N_DOUBLE)                               //只有數字
    {
        double d=atof(edQwertyContent->Text.c_str());
        if(bCheckRange)
        {
            edQwertyContent->Text=AnsiString(CheckRange(d, min, max));
        }
    }

    edQwertyContent->PasswordChar=c;

    if(PanelPtr!=NULL)
        PanelPtr->Caption=edQwertyContent->Text;
    else if(EditPtr!=NULL)
        EditPtr->Text=edQwertyContent->Text;
    else if(LabEditPtr!=NULL)
        LabEditPtr->Text=edQwertyContent->Text;
}
//---------------------------------------------------------------------------
void __fastcall TfQwertyKey::spbChangeCaseClick(TObject *Sender)
{
    bUpperCase=!bUpperCase;
    for(unsigned int i=0; i<47; i++)
    {
        QwertyKey[i]->spbKey->Caption=(bUpperCase)?QwertyKey[i]->UpperCase:QwertyKey[i]->LowerCase;

        if(bNoSymbol && QwertyKey[i]->KeyType==eKeyNumAndSymbol)
        {
            QwertyKey[i]->spbKey->Caption=QwertyKey[i]->UpperCase;
        }

        QwertyKey[i]->spbKey->Enabled=!(bNoSymbol && QwertyKey[i]->KeyType==eKeySymbol);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfQwertyKey::spbKeyClick(TObject *Sender)
{
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;

    if(edQwertyContent->SelLength>0)
    {
        edQwertyContent->Text="";
    }

    if(Ptr->Caption=="&&")                                                      //例外處理
    {
        edQwertyContent->Text=edQwertyContent->Text+"&";
    }
    else
    {
        edQwertyContent->Text=edQwertyContent->Text+Ptr->Caption;
    }
    edQwertyContent->SelStart=edQwertyContent->Text.Length();                   //把游標移到最後面
}
//---------------------------------------------------------------------------
void __fastcall TfQwertyKey::spbBackSpaceClick(TObject *Sender)
{
    AnsiString Str=edQwertyContent->Text;
    edQwertyContent->Text=Str.SubString(1, Str.Length()-1);
}
//---------------------------------------------------------------------------
void __fastcall TfQwertyKey::spbSummitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfQwertyKey::spbClearClick(TObject *Sender)
{
    edQwertyContent->Text="";
}
//---------------------------------------------------------------------------
void __fastcall TfQwertyKey::FormResize(TObject *Sender)
{
    edQwertyContent->Width=palContent->Width-8;
}
//---------------------------------------------------------------------------
void __fastcall TfQwertyKey::spbCancelClick(TObject *Sender)
{
    edQwertyContent->Text=sBackup;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfQwertyKey::spbPercentClick(TObject *Sender)
{
    iDecimalPoint++;
    if(iDecimalPoint>3)
    {
        iDecimalPoint=0;
    }

    ChangeDecimalPoint();
}
//---------------------------------------------------------------------------
void __fastcall TfQwertyKey::ChangeDecimalPoint()
{
    if(bIntegerOnly && iDecimalPoint>1)
        iDecimalPoint=0;

    switch(iDecimalPoint)
    {
        case 0:
            spbAdd1     ->Caption="+10"  ;
            spbAdd10    ->Caption="+100" ;
            spbAdd100   ->Caption="+1000";
            spbMinus1   ->Caption="-10"  ;
            spbMinus10  ->Caption="-100" ;
            spbMinus100 ->Caption="-1000";
            break;
        case 1:
            spbAdd1     ->Caption="+1"  ;
            spbAdd10    ->Caption="+10" ;
            spbAdd100   ->Caption="+100";
            spbMinus1   ->Caption="-1"  ;
            spbMinus10  ->Caption="-10" ;
            spbMinus100 ->Caption="-100";
            break;
        case 2:
            spbAdd1     ->Caption="+1.0" ;
            spbAdd10    ->Caption="+0.1" ;
            spbAdd100   ->Caption="+0.01";
            spbMinus1   ->Caption="-1.0" ;
            spbMinus10  ->Caption="-0.1" ;
            spbMinus100 ->Caption="-0.01";
            break;
        case 3:
            spbAdd1     ->Caption="+0.1"  ;
            spbAdd10    ->Caption="+0.01" ;
            spbAdd100   ->Caption="+0.001";
            spbMinus1   ->Caption="-0.1"  ;
            spbMinus10  ->Caption="-0.01" ;
            spbMinus100 ->Caption="-0.001";
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfQwertyKey::spbMinusClick(TObject *Sender)
{
    AnsiString S;
    S=edQwertyContent->Text;
    if(S.Pos("-")==1)
        S=S.SubString(2, S.Length());
    else
        S="-"+S;
    edQwertyContent->Text=S;
    edQwertyContent->SelStart=edQwertyContent->Text.Length();                   //把游標移到最後面
}
//---------------------------------------------------------------------------
void __fastcall TfQwertyKey::spbAdd1Click(TObject *Sender)
{
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;
    AnsiString str;
    double dResult, dAddValue;

    dAddValue=atof(Ptr->Caption.c_str());
    dResult  =atof(edQwertyContent->Text.c_str());
    dResult +=dAddValue;

    if(bIntegerOnly)
        edQwertyContent->Text=AnsiString(int(dResult));
    else
        edQwertyContent->Text=str.sprintf("%1.6f", dResult);
    edQwertyContent->SelStart=edQwertyContent->Text.Length();                   //把游標移到最後面
}
//---------------------------------------------------------------------------
void __fastcall TfQwertyKey::spbDPClick(TObject *Sender)
{
    if(bIntegerOnly)
        return;
    if(edQwertyContent->Text.Pos(".")==0)
        edQwertyContent->Text=edQwertyContent->Text+AnsiString(".");
    edQwertyContent->SelStart=edQwertyContent->Text.Length();                   //把游標移到最後面
}
//---------------------------------------------------------------------------
void __fastcall TfQwertyKey::edQwertyContentKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
    if(Key==0x0d)                                                               //Enter
    {
        Close();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfQwertyKey::edQwertyContentKeyPress(TObject *Sender,
      char &Key)
{
    AnsiString S;
    S=edQwertyContent->Text;

    if(KeyCode&N_DOUBLE)
    {
        if(OnlyNumberAndDotInPut(Key)==false)
        {
            Key=NULL;
        }
        else
        {
            if(Key=='.' && edQwertyContent->Text.Pos(".")!=0)
                Key=NULL;

            if(Key=='-')
            {
                if(S.Pos("-")==1)
                    S=S.SubString(2, S.Length());
                else
                    S="-"+S;
                Key=NULL;
                edQwertyContent->Text=S;
            }
        }
    }
    else if(KeyCode&N_INTEGER)
    {
        if(OnlyNumberInPut(Key)==false)
            Key=NULL;

        if(Key=='-')
        {
            if(S.Pos("-")==1)
                S=S.SubString(2, S.Length());
            else
                S="-"+S;
            Key=NULL;
            edQwertyContent->Text=S;
        }
    }
    else if(Key==8)                                                             //iKey==8 back space
    {
        ;
    }
    else
    {
        if(CUSTOMER_CODE!=CC_JCET)                                              //jou 20180212 (Steven) : 長電舊廠密碼有特殊字元 "o04#I1VW7%)!F$*"
        {
            if(KeyCode&N_NO_SYMBOL)
            {
                if((Key>='0' && Key<='9') || Key=='.' ||
                   (Key>='a' && Key<='z') ||
                   (Key>='A' && Key<='Z') ||
                   Key=='-' || Key=='_')                                        //Steven 20161216 : Fixed for 鍵盤輸入 - or _
                    ;
                else
                    Key=NULL;
            }
        }

        if(KeyCode&N_NO_SPACE)
        {
            if(Key==' ')
                Key=NULL;
        }
    }
}
//---------------------------------------------------------------------------
