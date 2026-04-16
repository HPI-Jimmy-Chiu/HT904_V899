#include "MachineDefine.h"
#pragma hdrstop

#include "Password.h"

#include "myQwertyKeyBoard.h"
#include "cMydef.h"
#include "cProd.h"
#include "common.h"
#include "BarcodeReader.h"
#include "FTPClient.h"
#include "mymessbox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfPassword *fPassword;
TfPassword *fPassword2;         //Steven 20150505 : 剛好在輸入時,Alarm會卡死
EvenLog_PASS_WORD EvenLogUSER;  //Steven 20181224 : For ASE-CL
//---------------------------------------------------------------------------
__fastcall TfPassword::TfPassword(TComponent* Owner)
    : TForm(Owner)
{
    bShow=false;
    bShowTab=0;                                                                 //Steven 20181224 : For ASE-CL
    ReadPasswordFile();                                                         //讀取密碼
}
//---------------------------------------------------------------------------
void TfPassword::ShowEventLogLogin()                                            //Steven 20181224 : For ASE-CL
{
    bShowTab=1;
    fPassword->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfPassword::FormShow(TObject *Sender)
{
    if(CUSTOMER_CODE==CC_Greatek)                                               //Sam 20200225 : 超豐的輸入帳號密碼位置改到右上，避免被他們的軟體視窗蓋掉。
    {
        Left=745;
        Top =0;
    }
    else
    {
        Left=(1024-Width)/2;
        Top =(768-Height)/2;
    }
    edPassword->Text="";
    edUserName->Visible=true;                                                   //Steven 20140626 : Fixed SetFocus問題
    edUserName->SetFocus();
    bShow=true;

    if(fQwertyKey->Showing)                                                     //jou 2013-04-16 必須強制關閉 fQwertyKey,不然密碼輸入錯誤偶發會hangup
    {
        fQwertyKey->Close();
    }

    //Steven 20140530 : 使用密碼本時,用下拉選單選使用者
    //{
    if(JCET_FOR_EVAN==1)                                                        //Steven 20221216 : 吳如春要改成從網路抓密碼本
    {
        edPassword->SetFocus();
        cbUserName->Visible=false;
        edUserName->Visible=false;
        lblUserName->Visible=false;
    }
    else
    {
        cbUserName->Visible=CosFunction.bTechComUseComboBox;
        edUserName->Visible=!CosFunction.bTechComUseComboBox;
    }

    if(bEnableEmployeeIDCheck==true)
    {
        cbUserName->Visible=false;
        edUserName->Visible=true;
        edUserName->Text="";        // name
        edPassword->Text="";        // password
    }
    cbUserName->Left=216;                                                       //Ifor 20190517 與Password切齊 200->216
    cbUserName->ItemIndex=0;

    if(CosFunction.bTechComUseComboBox)
        edUserName->Text=cbUserName->Text;
    //}

    btnDownload->Visible=IniConfig.bFtpPasswordDownload;                        //Steven 20221216 : 吳如春要改成從網路抓密碼本
    lblPWDownload->Visible=false;
    //Steven 20181224 : For ASE-CL
    //==>
    if(bShowTab==0)
    {
        tsPassword->TabVisible=true;
        tsEventlogLogin->TabVisible=false;
        tsEventLogLoginModify->TabVisible=false;
        PageControl1->ActivePage=tsPassword;
    }
    else if(bShowTab==0)
    {
        tsPassword->TabVisible=false;
        tsEventlogLogin->TabVisible=true;
        tsEventLogLoginModify->TabVisible=false;
        PageControl1->ActivePage=tsEventlogLogin;
    }
    //<==
    //Steven 20181224 : For ASE-CL
}
//---------------------------------------------------------------------------
void __fastcall TfPassword::SpeedButton1Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfPassword::spbCancelClick(TObject *Sender)
{
    edPassword->Text="";
    edUserName->Text="";
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfPassword::edUserNameKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    #ifndef SOFT_SIMULTE
    if(IniConfig.bVTESTFunction==true)                                          //jou 20220912 : 增加VTEST不可以用鍵盤輸入
    {
        edUserName->Text="";
        Clipboard()->Clear();
        edUserName->Text=InputBarcodeNumber("Input User Name:", "UserName");

        edPassword->Text="";
        Clipboard()->Clear();
        edPassword->Text=InputBarcodeNumber("Input Password:", "Password");
        if(edPassword->Text!="" && edUserName->Text!="")
            Close();
    }
    else if(CUSTOMER_CODE==CC_SCC ||                                            //Steven 20200302 : SCC楊恩民說只能用Barcode輸入
            CosFunction.bUseBarCoderAutoLogin)                                  //Sam 20221101 : 使用 BarCoder 自動登錄
    {
        edUserName->Text="";
        Clipboard()->Clear();
        AnsiString sBarcodeID=InputBarcodeNumber("Input User Name:", "UserName");
        edUserName->Text=sBarcodeID;
    }
    else
    #endif
    {
        if(Key==0x0d)                                                           //按下Enter
        {
            if(edPassword->Visible)                                             //JimmyChiu 20211122 fix get error bug without edpassword
                edPassword->SetFocus();
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfPassword::edPasswordKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    #ifndef SOFT_SIMULTE
    if(CUSTOMER_CODE==CC_SCC || IniConfig.bVTESTFunction==true)                 //Steven 20200302 : SCC楊恩民說只能用Barcode輸入  //jou 20220912 : 增加VTEST不可以用鍵盤輸入
    {
        edPassword->Text="";
        Clipboard()->Clear();
        AnsiString sBarcodeID=InputBarcodeNumber("Input Password:", "Password");
        edPassword->Text=sBarcodeID;
        if(edPassword->Text!="" && edUserName->Text!="")
            Close();
    }
    else
    #endif
    {
        if(Key==0x0d)                                                           //按下Enter
            Close();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfPassword::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    //Sam 20171019 (wei) : 超豐要求 BarCode 讀完 UserName 的字元要刪除 '$'
    //==>
    if(CUSTOMER_CODE==CC_Greatek)
    {
        if(edUserName->Text.Trim()!="" &&
           edUserName->Text.SubString(1, 1)=="$")
        {
            edUserName->Text=edUserName->Text.SubString(2, edUserName->Text.Length());
        }
    }
    //<==
    //Sam 20171019 (wei) : 超豐要求 BarCode 讀完 UserName 的字元要刪除 '$'
    edPasswordPassWord->Text="";    //Steven 20181224 : For ASE-CL
    edModifyPassword->Text="";      //Steven 20181224 : For ASE-CL
    edReModifyPassword->Text="";    //Steven 20181224 : For ASE-CL
    SystemStart=false;              //Ifor 20180928 :Add 避免密碼輸入完後機台直接動作
    bShow=false;
    bShowTab=0;                     //Steven 20181224 : For ASE-CL
}
//---------------------------------------------------------------------------
void __fastcall TfPassword::edPasswordMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    #ifndef SOFT_SIMULTE
    if(CUSTOMER_CODE==CC_KYEC_LEE)
    {
        fQwertyKey->ShowQwertyKey(edPassword, N_NO_SYMBOL|N_NO_SPACE|N_PASSWORD);
    }
    else if(CUSTOMER_CODE==CC_SCC ||                                            //Steven 20200302 : SCC楊恩民說只能用Barcode輸入
       USE_BARCODE_AS_KEYBOARD!=0 || IniConfig.bVTESTFunction==true)            //jou 20220912 : 增加VTEST不可以用鍵盤輸入
    {
        edPassword->Text="";
        Clipboard()->Clear();
        AnsiString sBarcodeID=InputBarcodeNumber("Input Password:", "Password");
        edPassword->Text=sBarcodeID;
        if(edPassword->Text!="" && edUserName->Text!="")
            Close();
    }
    else
    #endif
    {
        fQwertyKey->ShowQwertyKey(edPassword, N_NO_SYMBOL|N_NO_SPACE|N_PASSWORD);    //Steven 20120904 : 改用QWERTY鍵盤
    }
}
//---------------------------------------------------------------------------
void __fastcall TfPassword::edUserNameMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    #ifndef SOFT_SIMULTE
    if(IniConfig.bVTESTFunction==true)                                          //jou 20220912 : 增加VTEST不可以用鍵盤輸入
    {
        edUserName->Text="";
        Clipboard()->Clear();
        edUserName->Text=InputBarcodeNumber("Input User Name:", "UserName");

        edPassword->Text="";
        Clipboard()->Clear();
        edPassword->Text=InputBarcodeNumber("Input Password:", "Password");
        if(edPassword->Text!="" && edUserName->Text!="")
            Close();
    }
    else if(CUSTOMER_CODE==CC_SCC ||                                            //Steven 20200302 : SCC楊恩民說只能用Barcode輸入
            CosFunction.bUseBarCoderAutoLogin)                                  //Sam 20221101 : 使用 BarCoder 自動登錄
    {
        edUserName->Text="";
        Clipboard()->Clear();
        AnsiString sBarcodeID=InputBarcodeNumber("Input User Name:", "UserName");
        edUserName->Text=sBarcodeID;
    }
    else
    #endif
    {
        fQwertyKey->ShowQwertyKey(edUserName, N_NO_SYMBOL|N_NO_SPACE);          //Steven 20120904 : 改用QWERTY鍵盤
    }
}
//---------------------------------------------------------------------------
void __fastcall TfPassword::cbUserNameChange(TObject *Sender)
{
    if(CosFunction.bTechComUseComboBox)                                         //Steven 20140530 : 使用密碼本時,用下拉選單選使用者
        edUserName->Text=cbUserName->Text;
}
//Steven 20181224 : For ASE-CL
//==>
//---------------------------------------------------------------------------
void __fastcall TfPassword::sbPasswordCancelClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfPassword::sbPasswordOKClick(TObject *Sender)
{
    CheckPassword();
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfPassword::sbPasswordModifyOKClick(TObject *Sender)
{
    if(IniConfig.bA02DisableSaveParsWhenSwitchToOp==true &&                     //RogerYang 20260305 張寧說時間到都要登出，且不可被更改任何設定(A01_2)
        AccessLevel==0)
    {
        ShowMyMessage("[A01_2]目前已切換到Operator權限，\r\n請重新登入再做設定!");
    }
    else if(edModifyPassword->Text==edReModifyPassword->Text)
    {
        EvenLogUSER.PassWord[cob_UseIDList->ItemIndex]=edModifyPassword->Text;
        SavePasswordFile();
    }
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfPassword::sbPasswordModifyClick(TObject *Sender)
{
    CheckPassword();
    if(bLoginSuccess)
    {
        cob_ModifyUseIDList->ItemIndex=cob_UseIDList->ItemIndex;
        PageControl1->ActivePage=tsEventLogLoginModify;
    }
    else
    {
        cob_UseIDList->ItemIndex=0;
        Close();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfPassword::SetUserLevel(int iLevel)
{
    iNowEvenLogUserLevel=iLevel;
}
//---------------------------------------------------------------------------
void __fastcall TfPassword::CheckPassword()
{
    bLoginSuccess=false;
    int iLength=sizeof(EvenLogUSER.ID)/sizeof(EvenLogUSER.ID[0]);
    for(int i=0; i<iLength; i++)
    {
        if(EvenLogUSER.ID[i]==cob_UseIDList->Text ||
           JCET_FOR_EVAN==1)                                                    //Steven 20221216 : 吳如春要改成從網路抓密碼本
        {
            if(EvenLogUSER.PassWord[i]==edPasswordPassWord->Text)
            {
                bLoginSuccess=true;
                iNowEvenLogUserLevel=i;                                         //目前的EvenLog Level
                break;
            }
        }
    }
}
//---------------------------------------------------------------------------
bool __fastcall TfPassword::CheckLoginSuccess()
{
    return bLoginSuccess;
}
//---------------------------------------------------------------------------
int __fastcall TfPassword::GetLoginLevel()
{
    return iNowEvenLogUserLevel;
}
//---------------------------------------------------------------------------
void TfPassword::ReadPasswordFile()
{
    AnsiString sFileName="D:\\HT9045\\System\\EvenLogLevel.ini";

    EvenLogUSER.ID[OPERATOR]        ="OPERATOR";
    EvenLogUSER.PassWord[OPERATOR]  =CheckAndReadIniData(sFileName, "LEVEL", EvenLogUSER.ID[OPERATOR], AnsiString("1"));

    EvenLogUSER.ID[ENG_VENDOR]      ="ENG_VENDOR";
    EvenLogUSER.PassWord[ENG_VENDOR]=CheckAndReadIniData(sFileName, "LEVEL", EvenLogUSER.ID[ENG_VENDOR], AnsiString("1"));

    EvenLogUSER.ID[ENG_MI]          ="ENG_MI";
    EvenLogUSER.PassWord[ENG_MI]    =CheckAndReadIniData(sFileName, "LEVEL", EvenLogUSER.ID[ENG_MI] , AnsiString("1"));

    EvenLogUSER.ID[PM]              ="PM";
    EvenLogUSER.PassWord[PM]        =CheckAndReadIniData(sFileName, "LEVEL", EvenLogUSER.ID[PM], AnsiString("1"));

    EvenLogUSER.ID[AUDIT]           ="AUDIT";
    EvenLogUSER.PassWord[AUDIT]     =CheckAndReadIniData(sFileName, "LEVEL", EvenLogUSER.ID[AUDIT], AnsiString("1"));

    EvenLogUSER.ID[ADMIN]           ="ADMIN";
    EvenLogUSER.PassWord[ADMIN]     =CheckAndReadIniData(sFileName, "LEVEL", EvenLogUSER.ID[ADMIN], AnsiString("1"));
}
//---------------------------------------------------------------------------
void TfPassword::SavePasswordFile()
{
    AnsiString sFileName = "D:\\HT9045\\System\\EvenLogLevel.ini";

    WriteIniData(sFileName, "LEVEL", EvenLogUSER.ID[OPERATOR],    EvenLogUSER.PassWord[OPERATOR]);       //未登入也使用OPERATOR
    WriteIniData(sFileName, "LEVEL", EvenLogUSER.ID[ENG_VENDOR],  EvenLogUSER.PassWord[ENG_VENDOR]);
    WriteIniData(sFileName, "LEVEL", EvenLogUSER.ID[ENG_MI],      EvenLogUSER.PassWord[ENG_MI]);
    WriteIniData(sFileName, "LEVEL", EvenLogUSER.ID[PM],          EvenLogUSER.PassWord[PM]);
    WriteIniData(sFileName, "LEVEL", EvenLogUSER.ID[AUDIT],       EvenLogUSER.PassWord[AUDIT]);
    WriteIniData(sFileName, "LEVEL", EvenLogUSER.ID[ADMIN],       EvenLogUSER.PassWord[ADMIN]);
}
//---------------------------------------------------------------------------
//<==
//Steven 20181224 : For ASE-CL
void __fastcall TfPassword::btnOKClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfPassword::btnDownloadClick(TObject *Sender)
{
    //Steven 20221216 : 吳如春要改成從網路抓密碼本
    bool bFlag=fFTPClient->DownloadPasswordFormServer();                        //Sam 20210526 : 從 N06 DownloadPath 下載密碼本
    if(bFlag)
        lblPWDownload->Visible=true;
    btnDownload->Down=false;
}
//---------------------------------------------------------------------------
