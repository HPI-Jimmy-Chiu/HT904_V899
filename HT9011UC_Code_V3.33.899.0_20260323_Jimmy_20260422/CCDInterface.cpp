#include "MachineDefine.h"
#pragma hdrstop

#include "CCDInterface.h"
#include "cmydef.h"
//#include "uOffset.h"
#include "main.h"
#include "cprod.h"
#include "mymessbox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCCDInterfaceForm *CCDInterfaceForm;
typedef struct
{
    char cData[1024];
    char cDataCCD[1024];
}MAP_DATA;
MAP_DATA *mapData;
HANDLE hFileMapping;
TQPF_Timer CCDDelayTime;
//---------------------------------------------------------------------------
void __fastcall TCCDInterfaceForm::CloseCCDForm()
{
    fMain->HCCDVisionWnd=FindWindow("TIdentificationForm","Identification");
    if(fMain->HCCDVisionWnd!=NULL)
       fMain->SendCCD(HTCD_END,""); //kevin 20110811
}
//---------------------------------------------------------------------------
__fastcall TCCDInterfaceForm::TCCDInterfaceForm(TComponent* Owner)
    : TForm(Owner)
{
    if(IniConfig.bEnableCCDUSETCPIP)
    {
        bCCDProgramExistence=false;
        bCCDFileReady=false;
        bIdentificationFinish=false;
        bWakeupCCDProgram=false;
        bCCDProgramRunning=false;
        iIdentificationStatus=0;
        bGrabImageFinish=false;
        for(int i=0; i<10; i++)
        {
            sClientAlias[i]="";
            sClientAddress[i]="";
        }
        VideoCBox->ItemIndex=0;

        CloseCCDForm();
    }
    else
    {
        Timer1->Enabled=false;
        Timer2->Enabled=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TCCDInterfaceForm::ServerSocketClientRead(char *cDataCCD,int iCommandCode)
{
    int iStates;
    AnsiString asStartLot,asTemp;

    strncpy(pucInBuff, cDataCCD, sizeof(pucInBuff));
    GetCommandCode(pucInBuff);
    if(Memo->Lines->Count>30)
        Memo->Lines->Clear();

    if(iCommandCode==999)
        return;

    switch (iCommandCode)
    {
        case CDHT_EXISTENCE:
            asTemp=AnsiString(pucInBuff[0]);
            iStates=atoi(asTemp.c_str());
            if(iStates==CCD_YES)
            {
                bCCDProgramExistence=true;
                Memo->Lines->Add("EXISTENCE");
            }
            else if(iStates==CCD_NO)
            {
                bCCDProgramExistence=false;
                Memo->Lines->Add("NOT EXISTENCE");
                iStep=1;
            }
            break;
        case CDHT_FILE_READY:
            asTemp=AnsiString(pucInBuff[0]);
            iStates=atoi(asTemp.c_str());
            if(iStates==CCD_YES)
            {
                bCCDFileReady=true;
                Memo->Lines->Add("FILE EXISTENCE");
            }
            else if(iStates==CCD_NO)
            {
                bCCDFileReady=false;
                Memo->Lines->Add("FILE NOT EXISTENCE");
                iStep=10;
            }
            break;
        case CDHT_RESULT:
            asTemp=AnsiString(pucInBuff[0]);
            iStates=atoi(asTemp.c_str());
            if(iStates==CCD_PASS)
            {
                iIdentificationStatus=1;
                Memo->Lines->Add("CCD PASS");

                //結束
                fMain->SendCCD(HTCD_END,""); //kevin 20110811
            }
            else if(iStates==CCD_FAIL)
            {
                iIdentificationStatus=2;
                Memo->Lines->Add("CCD FAIL");
            }
            bIdentificationFinish=true;
            Memo->Lines->Add("Finish");
            Timer1->Enabled=false;
            break;
        case CDHT_ALARM_CLOSE:
//jou 2012-12-06 修正 CCD 關閉程式時會把 測試狀態值 覆蓋掉的問題
//            iIdentificationStatus=0;
            break;
        case CDHT_GRAB_FINISH:
            bGrabImageFinish=true;
            Memo->Lines->Add("Geab Image Finish!!");
            break;
//        default :   bFlag=false;
    }
}
//---------------------------------------------------------------------------
//iTesterIndex: 0~3
void __fastcall TCCDInterfaceForm::SendDataByAlias(int iTesterIndex, int Command, String sData)
{
    int iClientIndex;
    AnsiString asAssemblyData;
    asAssemblyData="["+IntToStr(Command)+"],";
    asAssemblyData+=sData;

    try //Eliot 2010_1006
    {
        iClientIndex=GetClientIndexByID(iTesterIndex);
        if(iClientIndex!=-1)
        {
        }
        else
        {
            asAssemblyData="Tester "+IntToStr(iTesterIndex+1)+" Alias error";
            Memo->Lines->Add(asAssemblyData);
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TCCDInterfaceForm::SendDataByAlias");
    }
}
//---------------------------------------------------------------------------
int __fastcall TCCDInterfaceForm::GetCommandCode(char *pucInBuff)
{
    if(pucInBuff[0]!='[' || pucInBuff[4]!=']' || pucInBuff[5]!=',')
        return 999;

    int iCommandCode;
    AnsiString asCommandCode;
    asCommandCode = AnsiString(pucInBuff[1])+AnsiString(pucInBuff[2])+AnsiString(pucInBuff[3]);

    try
    {
        iCommandCode=atoi(asCommandCode.c_str());
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TCCDInterfaceForm::GetCommandCode");
        iCommandCode=999;
    }

    return iCommandCode;
}
//---------------------------------------------------------------------------
int __fastcall TCCDInterfaceForm::RecordClientAlias(String Address, char *pucInBuff, int iLength)
{
    int iIndex=-1;
    AnsiString asTesterAlias="";
    for(int i=0; i<10; i++)
    {
        if(sClientAddress[i]==Address)
        {
            iIndex=i;
            break;
        }
    }
    if(iIndex!=-1)
    {
        for(int i=COMMAND_LENGTH; i<iLength; i++)
            asTesterAlias+=AnsiString(pucInBuff[i]);

        sClientAlias[iIndex]=asTesterAlias;
    }
    return iIndex;
}
//---------------------------------------------------------------------------
void __fastcall TCCDInterfaceForm::RemoveClientAlias(String Address)
{
    int iIndex=-1;
    for(int i=0; i<10; i++)
    {
        if(sClientAddress[i]==Address)
        {
            iIndex=i;
            break;
        }
    }
    if(iIndex!=-1)
    {
        for(int i=iIndex; i<9; i++)
        {
            sClientAlias[i]=sClientAlias[i+1];
            sClientAddress[i]=sClientAddress[i+1];
        }
    }
}
//---------------------------------------------------------------------------
int __fastcall TCCDInterfaceForm::GetClientIndexByAlias(String Alias)
{
    int iIndex=-1;
    for(int i=0; i<10; i++)
    {
        if(sClientAlias[i]==Alias)
        {
            iIndex=i;
            break;
        }
    }
    return iIndex;
}
//---------------------------------------------------------------------------
int __fastcall TCCDInterfaceForm::GetClientIndexByaddress(String Address)
{
    int iIndex=-1,iClientIndex;
    for(int i=0; i<10; i++)
    {
        if(sClientAddress[i]==Address)
        {
            iClientIndex=i;
            break;
        }
    }
    if(sClientAlias[iClientIndex]=="Tester1")
        iIndex=0;
    if(sClientAlias[iClientIndex]=="Tester2")
        iIndex=1;
    if(sClientAlias[iClientIndex]=="Tester3")
        iIndex=2;
    if(sClientAlias[iClientIndex]=="Tester4")
        iIndex=3;

    return iIndex;
}
//---------------------------------------------------------------------------
int __fastcall TCCDInterfaceForm::GetClientIndexByID(int iTesterIndex)
{
    int iIndex=-1;
    String sAlias;

    switch (iTesterIndex)
    {
        case 0: sAlias="Tester1"; break;
        case 1: sAlias="Tester2"; break;
        case 2: sAlias="Tester3"; break;
        case 3: sAlias="Tester4"; break;
    }

    for(int i=0; i<10; i++)
    {
        if(sClientAlias[i]==sAlias)
        {
            iIndex=i;
            break;
        }
    }
    return iIndex;
}
//---------------------------------------------------------------------------
void __fastcall TCCDInterfaceForm::Button1Click(TObject *Sender)
{
    //程式是否存在
     fMain->SendCCD(HTCD_EXISTENCE,""); //kevin 20110811
}
//---------------------------------------------------------------------------
void __fastcall TCCDInterfaceForm::Button2Click(TObject *Sender)
{
    //辨識檔案是否存在
     fMain->SendCCD(HTCD_FILE_READY,Edit1->Text); //kevin 20110811
}
//---------------------------------------------------------------------------
void __fastcall TCCDInterfaceForm::Button3Click(TObject *Sender)
{
    //辨識
    fMain->SendCCD(HTCD_IDENTIFICATION,""); //kevin 20110811
}
//---------------------------------------------------------------------------
void __fastcall TCCDInterfaceForm::Button4Click(TObject *Sender)
{
    //結束
    fMain->SendCCD(HTCD_END,""); //kevin 20110811

}
//---------------------------------------------------------------------------
void __fastcall TCCDInterfaceForm::Button6Click(TObject *Sender)
{
    //擷取
    bGrabImageFinish=false;
    AnsiString asTemp=IntToStr(VideoCBox->ItemIndex);
    asTemp+=","+CountCBox->Text+","+edSavePath->Text;
    fMain->SendCCD(HTCD_GRAB,asTemp); //kevin 20110811
}
//---------------------------------------------------------------------------
void __fastcall TCCDInterfaceForm::ScanCCDProgram(bool bEnableSet)  //kevin 20110801
{
    fMain->HCCDVisionWnd=FindWindow("TIdentificationForm","Identification");
    if(fMain->HCCDVisionWnd==NULL)
    {
        WakeupCCD();
        CCDIdentification();
    }
    else
    {
       //程式是否存在
       fMain->SendCCD(HTCD_EXISTENCE,""); //kevin 20110811
    }

   if(bEnableSet)
      fMain->SendCCD(HTCD_Setup,""); //kevin 20110811
}
//---------------------------------------------------------------------------
void __fastcall TCCDInterfaceForm::WakeupCCD()
{
    static int iRetryCount=0;
    if(bWakeupCCDProgram)
        return;
    bWakeupCCDProgram=true;

    SHELLEXECUTEINFO execinfo;
    memset(&execinfo,0,sizeof(execinfo));
    execinfo.cbSize=sizeof(execinfo);
    execinfo.lpVerb="open";
    execinfo.lpFile="D:\\Identification\\Identification.exe";

    if(FileExists(execinfo.lpFile))
    {
        iRetryCount=0;
        execinfo.lpParameters="";
        execinfo.fMask=SEE_MASK_NOCLOSEPROCESS;
        execinfo.nShow=SW_SHOWDEFAULT;
        ShellExecuteEx(&execinfo);
        bCCDProgramRunning=true;
    }
    else
    {
        iRetryCount++;
        if(iRetryCount>100)
        {
            iRetryCount=0;
            ShowMyMessage("The D:\\Identification\\Identification.exe file not existence");
            bCCDProgramRunning=false;
        }
    }
    bWakeupCCDProgram=false;
}
//---------------------------------------------------------------------------
void __fastcall TCCDInterfaceForm::Button5Click(TObject *Sender)
{
    ScanCCDProgram(true);  //kevin 20110801 判斷是否可進入
}
//---------------------------------------------------------------------------
//  開Sample檔 取 像
//---------------------------------------------------------------------------
bool __fastcall TCCDInterfaceForm::CCDRunExec()
{
    AnsiString CustomName;
    static iCount=0;
    CustomName = IntToStr(CUSTOMER_CODE)+","+fMain->cbSetupFileName->Text; //kevin 20110801
    switch (iStep)
    {
        case 1:
            CCDDelayTime.SetSecAndOn(5);
            bCCDFileReady=false;
            bIdentificationFinish=false;
            bCCDProgramExistence=false;  //jou 2012-12-06 修正CCD讀取工作檔沒有初始化的問題
            iCount=0;
            iStep=10;
        case 10:
            if(bCCDProgramExistence)
            {
                fMain->SendCCD(HTCD_FILE_READY,CustomName); //kevin 20110811
                iStep=20;
            }
            break;
        case 20:  //kevin 20110810  開完檔不要馬上取像
            if(bCCDFileReady)
            {
                fMain->SendCCD(HTCD_IDENTIFICATION,""); //kevin 20110811
                iStep=30;
            }
            break;
        case 30:
            if(bIdentificationFinish)
            {
                Memo->Lines->Add("Finish");
                iStep++;
                return true;
            }
            break;
    }
    return false;
}
//---------------------------------------------------------------------------
void __fastcall TCCDInterfaceForm::RetrunUpStep()
{
    if(iStep==30 && bIdentificationFinish==false)
    {
        iStep=20;
    }
    else if(iStep==20 && bCCDFileReady==false)
    {
        iStep=10;
        bCCDProgramExistence=true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TCCDInterfaceForm::MemoDblClick(TObject *Sender)
{
    CCDIdentification();
}
//---------------------------------------------------------------------------
void __fastcall TCCDInterfaceForm::CCDIdentification()
{
    bCCDFileReady=false;
    bIdentificationFinish=false;
    iIdentificationStatus=0;//0:未測試 1:Pass 2:Fail
    iStep=1;
}
//---------------------------------------------------------------------------
void __fastcall TCCDInterfaceForm::CCDIdentificationOpen()
{
    ScanCCDProgram(false); //kevin 20110801 判斷是否可進入
}
//---------------------------------------------------------------------------
void __fastcall TCCDInterfaceForm::FormShow(TObject *Sender)
{
    Edit1->Text=fMain->cbSetupFileName->Text;
}
//---------------------------------------------------------------------------
void __fastcall TCCDInterfaceForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Timer1->Enabled=false;
    Timer2->Enabled=false;
}
//---------------------------------------------------------------------------
void __fastcall TCCDInterfaceForm::CCDTimerOnOff(bool bOn)
{
    Timer1->Enabled=bOn;
    Timer2->Enabled=bOn;
}
//---------------------------------------------------------------------------
