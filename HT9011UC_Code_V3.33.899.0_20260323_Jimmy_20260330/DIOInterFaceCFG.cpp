#include "MachineDefine.h"
#pragma hdrstop

#include "DIOInterFaceCFG.h"

#include "common.h"
//#include "INPUT.h"
#include "cprod.h"
#include "cmydef.h"
#include "mymessbox.h"
#include "myQwertyKeyBoard.h"
#include "cTesterIF.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfDIOFrom *fDIOFrom;
//---------------------------------------------------------------------------
__fastcall TfDIOFrom::TfDIOFrom(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfDIOFrom::FormShow(TObject *Sender)
{
    OpenDialog1->InitialDir=DIOCFGPath;
    InitData();
    Top=30;
    Left=200;

    if(CosFunction.bTTLUseUSec)     //Steven 20180808 (wei) : TTL的時間單位改成microsecond
    {
        Label55->Caption="μs";
        Label4 ->Caption="μs";
        Label2 ->Caption="μs";
    }

    if(JCET_FOR_EVAN==1)        //Steven 20220506 : 吳如春要求只用10 bit bit
    {
        rgBinBitLength->Enabled=false;
        rgBinDataType ->Enabled=false;
    }

    fShow=true;
}
//---------------------------------------------------------------------------
AnsiString TfDIOFrom::GetDIOFileName()          //Steven 20180626 (wei) : TTL設定存到工作檔裡面
{
    AnsiString S, S1;
    AnsiString szDir="";
    if(IniConfig.bI16TTLSaveInSetupFile)
    {
        S=GetLastOpenFN();

        S1.sprintf("%s%s\\%s.ini", DataPath, S, FTestIF->cbDIOType->Text);
        if(FileExists(S1)==false)        //檔案不存在的話,就去複製一份過來
        {
            szDir.sprintf("%s%s.ini", DIOCFGPath, FTestIF->cbDIOType->Text);
            if(FileExists(szDir))
            {
                CopyFile(szDir.c_str(), S1.c_str(), false);
            }
        }
    }
    else
    {
        S1.sprintf("%s%s.ini", DIOCFGPath, FTestIF->cbDIOType->Text);
    }
    return S1;
}
//---------------------------------------------------------------------------
void __fastcall TfDIOFrom::LoadData(AnsiString S)
{
    AnsiString asString="";

    if(FileExists(S)==false)    //Steven 20180620 (Jou) : 加上DIO檔案遺失的保護判斷
    {
        if(TestIF_File.iTestType==TTL_MODE) //Steven 20210723 : 修正不是TTL模式會Alarm
        {
            ShowMyMessage("Current DIO data has been lossed, please check!!", "DIO資料遺失，請檢查！！");
            SystemStart=false;
        }
        return;
    }

    TTLCfg.iSTLogicMode         =ReadIniData(S, "Start Signal",  "Logic", 0);
    TTLCfg.iStartType           =ReadIniData(S, "Start Signal",  "Channel", 0);
    TTLCfg.iOneSTChannel        =ReadIniData(S, "Start Signal",  "SelSignalCH", 0);
    TTLCfg.iSTPluseWidth        =ReadIniData(S, "Start Signal",  "Pluse Width", 0);
    TTLCfg.iDutType             =ReadIniData(S, "DUT Signal",    "Type", 0);
    if(TTLCfg.iDutType<0)   //Steven 20110105
        TTLCfg.iDutType=0;

    TTLCfg.iDutBfOnTime         =ReadIniData(S, "DUT Signal",    "Before On", 0);
    TTLCfg.iDutAfOffTime        =ReadIniData(S, "DUT Signal",    "After Off", 0);

    TTLCfg.iCateLogicMode       =ReadIniData(S, "Cate Signal",   "Logic", 0);
    if(JCET_FOR_EVAN==1)        //Steven 20220506 : 吳如春要求只用10 bit bit
    {
        TTLCfg.iCateBitLength       =3;
        TTLCfg.iCateDataType        =0;
    }
    else
    {
        TTLCfg.iCateBitLength       =ReadIniData(S, "Cate Signal",   "Channel status", 0);
        TTLCfg.iCateDataType        =ReadIniData(S, "Cate Signal",   "Data Type", 0);
    }

    asString=ReadIniData(S, "Name",          "Data Type", AnsiString("AAA"));
    strncpy(TTLCfg.cModeName, asString.c_str(), sizeof(TTLCfg.cModeName));
    if(TTLCfg.iCateBitLength==_5BitPE || TTLCfg.iCateBitLength==_10BitPE)
        TTLCfg.iCateParity=2;//even
    else if(TTLCfg.iCateBitLength==_5BitPO || TTLCfg.iCateBitLength==_10BitPO)
        TTLCfg.iCateParity=1;//odd
    else
        TTLCfg.iCateParity=0;//not use
    if(CosFunction.bTTLUseUSec)     //Steven 20180808 (wei) : TTL的時間單位改成microsecond
    {
        TTLCfg.iSTPluseWidth    =CheckRange((int)TTLCfg.iSTPluseWidth, 1, 500000);    //Ifor 20171229 : UNISEM TTL SOT訊號寬度統一修正為1ms ~ 500ms , 主要是保護在短秒數測試時SOT過長會造成多測問題
        TTLCfg.iDutBfOnTime     =CheckRange(TTLCfg.iDutBfOnTime, 1, 500000);
        TTLCfg.iDutAfOffTime    =CheckRange(TTLCfg.iDutAfOffTime, 1, 500000);
    }
    else
    {
        TTLCfg.iSTPluseWidth    =CheckRange((int)TTLCfg.iSTPluseWidth, 10, 500);   //Steven 20161221 (jou) : TTL SOT訊號寬度統一修正為10ms ~ 500ms , 主要是保護在短秒數測試時SOT過長會造成多測問題
        TTLCfg.iDutBfOnTime     =CheckRange(TTLCfg.iDutBfOnTime, 10, 500);         //Steven 20161221 (jou) : TTL SOT訊號寬度統一修正為10ms ~ 500ms , 主要是保護在短秒數測試時SOT過長會造成多測問題
        TTLCfg.iDutAfOffTime    =CheckRange(TTLCfg.iDutAfOffTime, 10, 500);        //Steven 20161221 (jou) : TTL SOT訊號寬度統一修正為10ms ~ 500ms , 主要是保護在短秒數測試時SOT過長會造成多測問題
    }

    DoIniDataToForm();
}
//---------------------------------------------------------------------------
void __fastcall TfDIOFrom::DoIniDataToForm()
{
    rgStartLogic->ItemIndex     =TTLCfg.iSTLogicMode;
    rgStartChannel->ItemIndex   =TTLCfg.iStartType;
    edPulseWidth->Text          =TTLCfg.iSTPluseWidth;
    cbOneSTChannel->ItemIndex   =TTLCfg.iOneSTChannel;
    cbSignalType->ItemIndex     =TTLCfg.iDutType;
    edSignalBeforeOn->Text      =TTLCfg.iDutBfOnTime;
    edSignalAfterOff->Text      =TTLCfg.iDutAfOffTime;
    rgBinLogic->ItemIndex       =TTLCfg.iCateLogicMode;
    rgBinBitLength->ItemIndex   =TTLCfg.iCateBitLength;
    rgBinDataType->ItemIndex    =TTLCfg.iCateDataType;
    edTTLModeName->Text         =TTLCfg.cModeName;
}
//---------------------------------------------------------------------------
void __fastcall TfDIOFrom::InitData()
{
    rgStartLogic->ItemIndex=0;
    rgStartChannel->ItemIndex=0;
    edPulseWidth->Text="";

    cbSignalType->ItemIndex=0;
    edSignalBeforeOn->Text="";
    edSignalAfterOff->Text="";

    rgBinLogic->ItemIndex=0;
    rgBinBitLength->ItemIndex=0;
    rgBinDataType->ItemIndex=0;

    edTTLModeName->Text="";
    DIOFileName="";
}
//---------------------------------------------------------------------------
void __fastcall TfDIOFrom::Edit3KeyPress(TObject *Sender, char &Key)
{
    if(OnlyNumberAndDotInPut(Key)==false)
        Key=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfDIOFrom::edSignalBeforeOnKeyPress(TObject *Sender, char &Key)
{
    if(OnlyNumberInPut(Key)==false)
        Key=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfDIOFrom::edPulseWidthMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(CosFunction.bTTLUseUSec)     //Steven 20180808 (wei) : TTL的時間單位改成microsecond
    {
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1, 500000);     //Ifor 20171229 : UNISEM TTL SOT訊號寬度統一修正為1ms ~ 500ms , 主要是保護在短秒數測試時SOT過長會造成多測問題
    }
    else
    {
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 10, 500);    //Steven 20161221 (jou) : TTL SOT訊號寬度統一修正為10ms ~ 500ms , 主要是保護在短秒數測試時SOT過長會造成多測問題
    }
}
//---------------------------------------------------------------------------
void __fastcall TfDIOFrom::spbSaveClick(TObject *Sender)
{
    if(IniConfig.bA02DisableSaveParsWhenSwitchToOp==true &&                     //RogerYang 20260305 張寧說時間到都要登出，且不可被更改任何設定(A01_2)
        AccessLevel==0)
    {
        ShowMyMessage("[A01_2]目前已切換到Operator權限，\r\n請重新登入再做設定!");
        Close();
    }

    if(DIOFileName=="")
        return;

    SaveDialog1->InitialDir=DIOCFGPath;
    SaveDialog1->FileName=DIOFileName;

    if(SaveDialog1->Execute())
    {
        if(CosFunction.bTTLUseUSec)     //Steven 20180808 (wei) : TTL的時間單位改成microsecond
        {
            TTLCfg.iSTPluseWidth        =CheckRange(atoi(edPulseWidth->Text.c_str()), 1, 500000);      //Ifor 20171229 : UNISEM TTL SOT訊號寬度統一修正為1ms ~ 500ms , 主要是保護在短秒數測試時SOT過長會造成多測問題
            TTLCfg.iDutBfOnTime         =CheckRange(atoi(edSignalBeforeOn->Text.c_str()), 1, 500000);
            TTLCfg.iDutAfOffTime        =CheckRange(atoi(edSignalAfterOff->Text.c_str()), 1, 500000);
        }
        else
        {
            TTLCfg.iSTPluseWidth        =CheckRange(atoi(edPulseWidth->Text.c_str()), 10, 500);        //Steven 20161221 (jou) : TTL SOT訊號寬度統一修正為10ms ~ 500ms , 主要是保護在短秒數測試時SOT過長會造成多測問題
            TTLCfg.iDutBfOnTime         =CheckRange(atoi(edSignalBeforeOn->Text.c_str()), 10, 500);    //Steven 20161221 (jou) : TTL SOT訊號寬度統一修正為10ms ~ 500ms , 主要是保護在短秒數測試時SOT過長會造成多測問題
            TTLCfg.iDutAfOffTime        =CheckRange(atoi(edSignalAfterOff->Text.c_str()), 10, 500);    //Steven 20161221 (jou) : TTL SOT訊號寬度統一修正為10ms ~ 500ms , 主要是保護在短秒數測試時SOT過長會造成多測問題
        }

        DIOFileName=SaveDialog1->FileName;
        WriteIniData(DIOFileName, "Start Signal",  "Logic",           rgStartLogic->ItemIndex);
        WriteIniData(DIOFileName, "Start Signal",  "Channel",         rgStartChannel->ItemIndex);
        WriteIniData(DIOFileName, "Start Signal",  "Pluse Width",     (int)TTLCfg.iSTPluseWidth);
        WriteIniData(DIOFileName, "Start Signal",  "SelSignalCH",     cbOneSTChannel->ItemIndex);
        WriteIniData(DIOFileName, "DUT Signal",    "Type",            cbSignalType->ItemIndex);
        WriteIniData(DIOFileName, "DUT Signal",    "Before On",       TTLCfg.iDutBfOnTime);
        WriteIniData(DIOFileName, "DUT Signal",    "After Off",       TTLCfg.iDutAfOffTime);
        WriteIniData(DIOFileName, "Cate Signal",   "Logic",           rgBinLogic->ItemIndex);
        WriteIniData(DIOFileName, "Cate Signal",   "Channel status",  rgBinBitLength->ItemIndex);
        WriteIniData(DIOFileName, "Cate Signal",   "Data Type",       rgBinDataType->ItemIndex);
        WriteIniData(DIOFileName, "Name",          "Data Type",       edTTLModeName->Text);
    }
    spbSave->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfDIOFrom::spbLoadClick(TObject *Sender)
{
    OpenDialog1->Title="Open DIO File";
    if(OpenDialog1->Execute())
    {
        DIOFileName=OpenDialog1->FileName;
        LoadData(DIOFileName);
    }
    spbLoad->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfDIOFrom::spbDeleteClick(TObject *Sender)
{
    OpenDialog1->Title="Select file to delete";
    if(OpenDialog1->Execute())
    {
        DeleteFile(OpenDialog1->FileName);
    }
    spbDelete->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfDIOFrom::spbExitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfDIOFrom::FormClose(TObject *Sender, TCloseAction &Action)
{
    fShow=false;
    DoIniDataToForm();                                                          //JerryYang 20250411 : 離開頁面要刷新一次, 避免誤存檔
}
//---------------------------------------------------------------------------
