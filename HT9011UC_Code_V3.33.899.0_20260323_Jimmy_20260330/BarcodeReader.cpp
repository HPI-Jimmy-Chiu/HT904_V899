#include "MachineDefine.h"
#pragma hdrstop

#include "BarcodeReader.h"
#include "MachineType.h"
#include "cmydef.h"
#include "cMyDB.h"
#include "uLotInfo.h"
#include "uHGemHT9045.h"
#include "FTPClient.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormBarcodeReader *FormBarcodeReader;
//---------------------------------------------------------------------------
AnsiString sLastKeyin = "";
// 20140103 wei KYEC Barcode Reader
AnsiString BarcodeChange[bcTotal]={" User Name", " Tray Form", " Plate Form", " TrayAssign", " Temperature",
                                   " Contact", " Tester IF", " Bin", " Setup", " Yield",
                                   " Auto Clean", " Builder", " Offset", " Setup File",
                                   " Exit"};

//---------------------------------------------------------------------------
__fastcall TFormBarcodeReader::TFormBarcodeReader(TComponent* Owner)
    : TForm(Owner)
{
    iKeyType=KEY_NONE;
    sBarcodeInfo="";
    bShow=false;                                                                //Steven 20140508
}
//---------------------------------------------------------------------------
void __fastcall TFormBarcodeReader::btnEnterClick(TObject *Sender)
{
    if(_sInputType=="RunMode")                                                  //RogerYang 20251215 : JCET 2D FT1白名單/FT2比對功能
    {
    }
    else if(edtBarcodeNumber->Text.Length()<4)                                  //Ifor 20160830 add 避免客戶手動輸入跳過卡控
    {
        return;
    }

    iKeyType=KEY_ENTER;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormBarcodeReader::FormShow(TObject *Sender)
{
    bShow=true;                                                                 //Steven 20140508
    sLastKeyin="";

    iKeyType=KEY_NONE;
    edtBarcodeNumber->Text="";
    edtBarcodeNumber->Enabled=true;                                             //KevinC 20250913 : fixed
    Left=(1280-Width)/2;
    Top =(1024-Height)/2;
    edtBarcodeNumber->SetFocus();

    #ifdef SOFT_SIMULTE
        TimerKeyIn->Enabled=false;
    #else
        if(CUSTOMER_CODE==CC_PTI)                                               //RogerYang 20170327 (Steven) 力成 LotID 輸入，工程師權限以上才能 Key In
        {
            //lblInputType->Caption="Input Lot ID:";
            if((CosFunction.bUseBarCoderAutoLogin && bBarCoderAutoLogin) ||     //Sam 20230320 : 使用 BarCodeReader 來輸入切換 SetupFile。
               (CosFunction.bUseBarCoderChangeSetupFile && bBarCoderSetupFile))
            {
                TimerKeyIn->Enabled=true;
            }
            else
            {
                if(AccessLevel>0)
                    TimerKeyIn->Enabled=false;
                else
                    TimerKeyIn->Enabled=true;
            }
        }
        else if(CosFunction.bFTPUseBarcodeReader==true)
        {
            if((IniConfig.bN06_UseBarcode==false && fFTPClient->bShow==false) ||
               (CUSTOMER_CODE==CC_CYUEAN && fFTPClient->bShow))                 //Sam 20230706 : 确安科技 FTP Setup File 鍵盤輸入要失效
            {
                TimerKeyIn->Enabled=true;
            }
        }
        else
        {
            TimerKeyIn->Enabled=true;
        }
        RegisterHotKey(Handle, 2, MOD_CONTROL, 'V');                            //wei 20151120 此畫面ctrl+V無效
    #endif
}
//---------------------------------------------------------------------------
void __fastcall TFormBarcodeReader::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    TimerKeyIn->Enabled=false;

    if(iKeyType==KEY_ENTER)
    {
        sBarcodeInfo = edtBarcodeNumber->Text;
        asSecsGemBarCode =sBarcodeInfo;                                         //wei 20150811 SecsGem BarcodeReader   //Frank 20170109 modify asBarCode=>asSecsGemBarCode
        EventReport(SECS_EVENT.BarcodeReaderEnter);                             //wei 20150811 SecsGem BarcodeReader Event
    }
    else
    {
        sBarcodeInfo="";
    }

    if(CUSTOMER_CODE==CC_SJ_Semiconductor ||                                    //Steven 20240612 : 去掉結尾
       CUSTOMER_CODE==CC_SJ_Semiconductor_OS)
    {
        sBarcodeInfo=StringReplace(sBarcodeInfo, "\r", "", TReplaceFlags()<<rfReplaceAll);
        sBarcodeInfo=StringReplace(sBarcodeInfo, "\n", "", TReplaceFlags()<<rfReplaceAll);
    }
    sBarcodeInfo=sBarcodeInfo.Trim();

    bShow=false;                                                                //Steven 20140508
    UnregisterHotKey(Handle, 2);                                                //wei 20151120 此畫面ctrl+V無效
    _sInputType="";
}
//---------------------------------------------------------------------------
AnsiString InputBarcodeNumber(AnsiString sLabelCaption, AnsiString sInputType)
{
    AnsiString sBarcode;

    FormBarcodeReader->lblInputType->Caption=sLabelCaption;
    FormBarcodeReader->_sInputType=sInputType;
    if(sInputType=="Password")                                                  //Steven 20200313
    {
        FormBarcodeReader->edtBarcodeNumber->PasswordChar='*';
    }
    FormBarcodeReader->ShowModal();
    FormBarcodeReader->edtBarcodeNumber->PasswordChar=NULL;

    sBarcode=FormBarcodeReader->sBarcodeInfo;
//    FormBarcodeReader->lblInputType->Caption="Input Operator ID:";
    if(sInputType=="SetupFile")                                                 //Sam 20230320 : 使用 BarCodeReader 來輸入切換 SetupFile。
    {
        return sBarcode.UpperCase();
    }
    else
    {
        return sBarcode;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormBarcodeReader::edtBarcodeNumberKeyDown(
      TObject *Sender, WORD &Key, TShiftState Shift)
{
    if(IniConfig.bSPILFunction && TestIF_File.b2DIDAllowList)                   //JerryYang 20241104 : 支援2DID白名單功能
    {
        if(edtBarcodeNumber->Text.Length()>=8 && Key=='\r')
        {
            edtBarcodeNumber->Enabled=false;
        }
    }
}
//---------------------------------------------------------------------------
bool bBarcodeFirstKeyIn=false;                                                  //Ifor 20190924 : add Barcode 輸入判斷避免讀碼失敗
void __fastcall TFormBarcodeReader::edtBarcodeNumberKeyUp(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
    if(Key==17)
    {
        edtBarcodeNumber->Text="";
        sLastKeyin="";
    }

    if(CUSTOMER_CODE==CC_PTI ||
       CUSTOMER_CODE==CC_SCC ||                                                 //Ifor 20181029 add :SCC Barcode Delay Timer
       CUSTOMER_CODE==CC_TFME_CHINA)                                            //RogerYang 20170407 (Steven) 只要有東西進來就計時
    {
        HDelayTime.SetMSAndOn(80);
    }

    if(bBarcodeFirstKeyIn==false)                                               //Ifor 20190924 : add Barcode 輸入判斷避免讀碼失敗
    {
        bBarcodeFirstKeyIn=true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormBarcodeReader::TimerKeyInTimer(TObject *Sender)
{
    static int iCount=0;

    if(InitialOK==false)
        return;

    if(bBarcodeFirstKeyIn==true)                                                //Ifor 20190924 : add Barcode 輸入判斷避免讀碼失敗
    {
        bBarcodeFirstKeyIn=false;
        return;
    }

    if(bShow)
        FormBarcodeReader->BringToFront();                                      //Steven 20140508
    else
        return;

    if(IniConfig.bO23_InputLotIDByBarcode)                                      //Steven 20241224 : LotID只能用Barcode
    {
        if(_sInputType=="RunMode")                                              //RogerYang 20251215 : JCET 2D FT1白名單/FT2比對功能
        {
        }
        else if(edtBarcodeNumber->Text.Length()>30 ||                           //Steven 20250217 : 凌中心這邊限制5~30字元
           edtBarcodeNumber->Text.Length()<5)                                   //Steven 20240612 : SJSM要求Lot ID最大100碼, 最小15碼
        {
            edtBarcodeNumber->Text="";
            sLastKeyin="";
        }
    }
    else if(CUSTOMER_CODE==CC_JCET)                                             //Steven 20210304 : 如春說要改用Barcode輸入
    {
        if(_sInputType=="RunMode")                                              //RogerYang 20251215 : JCET 2D FT1白名單/FT2比對功能
        {
        }
        else if(edtBarcodeNumber->Text.Length()<15)
        {
            edtBarcodeNumber->Text="";
            sLastKeyin="";
        }

        if(edtBarcodeNumber->Text!="")
        {
            btnEnter->Click();
        }
    }
    else if(CUSTOMER_CODE==CC_SJ_Semiconductor ||
            CUSTOMER_CODE==CC_SJ_Semiconductor_OS)
    {
        if((_sInputType=="LotID" || _sInputType=="UserName") &&
           (edtBarcodeNumber->Text.Length()>15 ||                               //Steven 20240612 : SJSM要求Lot ID最大100碼, 最小15碼
            edtBarcodeNumber->Text.Length()<6))
        {
            edtBarcodeNumber->Text="";
            sLastKeyin="";
        }
        else if(_sInputType=="")                                                //工作檔
        {
            if(edtBarcodeNumber->Text.Length()>100 ||
               edtBarcodeNumber->Text.Length()<15)
            {
                edtBarcodeNumber->Text="";
                sLastKeyin="";
            }
        }

        if(edtBarcodeNumber->Text!="")
        {
            btnEnter->Click();
        }
    }
    else if(CUSTOMER_CODE==CC_SCC || CUSTOMER_CODE==CC_TFME_CHINA)
    {
        if((_sInputType=="LotID" || _sInputType=="UserName") &&
           (edtBarcodeNumber->Text.Length()>30 ||                               //Steven 20200306 : SCC要求Lot ID最大30碼, 最小4碼
            edtBarcodeNumber->Text.Length()<5))
        {
            edtBarcodeNumber->Text="";
            sLastKeyin="";
        }
        else if(_sInputType=="")                                                //工作檔
        {
            if(CosFunction.bHiSiliconFunction==true)
            {
                if(edtBarcodeNumber->Text.Length()>12 &&
                   !(edtBarcodeNumber->Text.Pos("LAC")==1 ||
                     edtBarcodeNumber->Text.Pos("KL")==1))                      //Ifor 20181224 : add SCC 工作檔卡控卡控
                {
                    edtBarcodeNumber->Text="";
                    sLastKeyin="";
                }
            }
            else
            {
                if(edtBarcodeNumber->Text.Length()>30 ||
                   edtBarcodeNumber->Text.Length()<5)
                {
                    edtBarcodeNumber->Text="";
                    sLastKeyin="";
                }
            }
        }

        if(edtBarcodeNumber->Text!="")
        {
            btnEnter->Click();
        }
    }
    else if(IniConfig.bVTESTFunction==true ||                                   //jou 20220912 : 增加VTEST不可以用鍵盤輸入
           (CosFunction.bUseBarCoderAutoLogin && bBarCoderAutoLogin) ||         //Sam 20221101 : 使用 BarCoder 自動登錄
           (CosFunction.bUseBarCoderChangeSetupFile && bBarCoderSetupFile))     //Sam 20230320 : 使用 BarCodeReader 來輸入切換 SetupFile。
    {
        if(edtBarcodeNumber->Text.Length()<=3)
        {
            edtBarcodeNumber->Text="";
            sLastKeyin="";
            iCount=0;
        }
        else
        {
            iCount++;
            if(iCount>3)
            {
                iCount=0;
                btnEnter->Click();
            }
        }
    }
    else if(CUSTOMER_CODE==CC_PTI)                                              //Sam 20221101 往下移          //RogerYang 20170407 (Steven) 力成 LotID 輸入  共12碼
    {
        if(edtBarcodeNumber->Text.Length()<3 ||
          (edtBarcodeNumber->Text.Length()!=12 && HDelayTime.Off()))
        {
            edtBarcodeNumber->Text="";
            sLastKeyin="";
        }
    }
    else if(CUSTOMER_CODE==CC_AMD_M)
    {
        if(edtBarcodeNumber->Text.Length()<=3)
        {
            edtBarcodeNumber->Text="";
            sLastKeyin="";
        }
    }
    else if(IniConfig.bSPILFunction && TestIF_File.b2DIDAllowList &&            //JerryYang 20241104 : 支援2DID白名單功能
            SPIL_FOR_QLE==0)
    {
        if(edtBarcodeNumber->Text.Length()<=8)
        {
            edtBarcodeNumber->Text="";
            sLastKeyin="";
        }
    }
    else if(IniConfig.bSPILFunction &&
            TestIF_File.b2DIDAllowList && SPIL_FOR_QLE==1)                      //KevinC 20250912 : 渠梁Lot ID輸入消失問題
    {
        if(edtBarcodeNumber->Text.Length()<8)
        {
            edtBarcodeNumber->Text="";
            sLastKeyin="";
        }
    }
    else if(CUSTOMER_CODE==CC_CYUEAN && fFTPClient->bShow)                      //Sam 20230706 : 确安科技 FTP Setup File 鍵盤輸入要失效
    {
        if(edtBarcodeNumber->Text.Length()<10 || edtBarcodeNumber->Text.Length()>45)
        {
            edtBarcodeNumber->Text="";
            sLastKeyin="";
        }
    }
    else
    {
        if(edtBarcodeNumber->Text.Length()<6 ||
           edtBarcodeNumber->Text.Length()>7)                                   // 2013.12.17 , Joye , KYEC Barcdoe Reader
        {
            edtBarcodeNumber->Text="";
            sLastKeyin="";
        }
        else
        {
            if(sLastKeyin=="")
            {
                sLastKeyin=edtBarcodeNumber->Text;
                AnsiString sHeadNum;
                int iHeadNum=0;
                if(sLastKeyin.Length()==6)                                      // 2013.12.17 , Joye , KYEC Barcdoe Reader ------->>
                {
                    sHeadNum=sLastKeyin.SubString(1, 2);
                    iHeadNum=atoi(sHeadNum.c_str());
                }
                else if(sLastKeyin.Length()==7)
                {
                    sHeadNum=sLastKeyin.SubString(1, 3);
                    iHeadNum=atoi(sHeadNum.c_str());
                }

                if(bEnable_KLT_Function==true)                                  //Ifor 20180802 (Steven) : add KLT bBarCodeRules -> bEnable_KLT_Function 0:KYEC 1: KLT
                {                                                               //Ifor 20180517 : add BarCode 編碼規則 0:民國 1: 西元
                    if(iHeadNum<3 || iHeadNum>31)                               // KYEC 3~31 (2003~2031)
                    {
                        edtBarcodeNumber->Text="";
                        sLastKeyin="";
                    }
                }
                else
                {
                    if(iHeadNum<85 || iHeadNum>120)                             // KYEC 85~120 (85~120)
                    {
                        edtBarcodeNumber->Text="";
                        sLastKeyin="";
                    }
                }
            }
            else
            {
                edtBarcodeNumber->Text=sLastKeyin;
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormBarcodeReader::miClearClick(TObject *Sender)
{
    edtBarcodeNumber->Text="";
    sLastKeyin="";
}
//---------------------------------------------------------------------------
void __fastcall TFormBarcodeReader::miCloseClick(TObject *Sender)
{
    Close();
}
//------------------------------------------------------------------------------
int Barcode_Reader(int Barcode)                                                 // 20140103 wei KYEC Barcode Reader
{
    int iFlag=2;

    if(USE_BARCODE_AS_KEYBOARD!=0 && bBarcodeReader && ReEnterBarcode[Barcode]==false &&
       (CUSTOMER_CODE==CC_KYEC_LEE || CUSTOMER_CODE==CC_KYEC_XILINX))
    {
        AnsiString sBarcodeID=InputBarcodeNumber("Input Operator ID:");
        if(sBarcodeID=="")
        {
            iFlag=0;
        }
        else
        {
            AnsiString sRecord="ID:"+sBarcodeID+" Login --- Change "+BarcodeChange[Barcode];
            RecordProcess(sRecord.c_str());

            if(Barcode==bcOffset)
            {
                bOffsetEnterBarcode=true;
            }
            else if(Barcode!=bcSECSGEM)                                         //wei 20150904 SECSGEM 每次都要輸入
            {
                ReEnterBarcode[Barcode]=true;
            }
            iFlag=1;
        }
    }
    return iFlag;
}
//------------------------------------------------------------------------------
