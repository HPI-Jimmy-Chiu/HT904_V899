#include "MachineDefine.h"
#pragma hdrstop

#include "uHGemEquipment.h"
//#include "cpublic.h"
#include "cMydef.h"
#include "database.h"
#include "myQwertyKeyBoard.h"
#include "common.h"
#include "uHGemHT9045.h"
#include "SgdToXLS.h"
//#include "TasmInfo.h"
//#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// check
//2013/07/18 lee
THGem *HGem;
//THGem *HGem2;
struct HTypeStruct HType;
extern  bool InitialOK;
bool bSECSGEM_DoSeparate=false;                                                 //Ifor 20180913 (Steven) : add KYEC SECS GEM Connectionª¬ºA¬°OffLine°ÝÃD®É¡Aµ¥«Ý30¬í«á°e¥XÂ_½u©R¥O«áµ¥«Ý¤W¼h¨t²Î­«·s³s½u
extern TMyStringList *tsSoftwareExeTime;                                        //Steven 20210526 : ¬ö¿ý³nÅé°õ¦æ®É¶¡
extern TQPF_Timer  SoftwareExeTimer;
extern int iSoftwareExeTag;

extern void LogSoftwareOffTime(AnsiString Flag);
//---------------------------------------------------------------------------
// 2013/04/02 lee
__int64 EVENT_SYSTEM_DATA_ID              =1          ;
//---------------------------------------------------------------------------
__int64 EVENT_SYSTEM_CEID_ONLINE          =1          ;
__int64 EVENT_SYSTEM_CEID_OFFLINE         =2          ;
__int64 EVENT_SYSTEM_CEID_ONLINE_LOCAL    =3          ;
__int64 EVENT_SYSTEM_CEID_ONLINE_REMOTE   =4          ;
__int64 EVENT_SYSTEM_CEID_CHANGE_CONST    =5          ;
//---------------------------------------------------------------------------
const iTraceDataCT=10;
//---------------------------------------------------------------------------
//Read me
// [1] ¤£½×¬O¥Ø«e¥\¯à¬O­n·í Host ©Î Equpiment , ÅÜ¼Æ¦a©w¸q³W«h
//     ¤@«ß§â¥»¾÷­n¶Ç°e¦a¸ê®Æ©R¦W¬° Local , ¦¬¶i¨Ó¦a¸ê®Æ¤@«ßºÙ Remote
//     Local ªº Head struct ¬° struct HSMS_Head_Struct Local
//     LocalLength_4  «h¬ö¿ý¾ã­Ó Buffer Á`ªø«×,¥]§t«e4­Ó byte ªº Buffer ªø«×
//     LocalLength    «h¬ö¿ý¾ã­Ó Buffer Á`ªø«×,¦ý¤£¥]§t«e4­Ó byte ªº Buffer ªø«×
//     LocalBuffer    «h¬O«Ý¶Ç°eªº¸ê®Æ Buffer
//
// Remote
//---------------------------------------------------------------------------
// HSMS Head struct
// ¶Ç¤@µ§ unsigned int ¦p 0x01020304
// ¤@¯ë§Ú­Ì·|¦b°}¦C©ñ 0x04,0x03,0x02,0x01
// ¦ý HSMS «h¤£·|,¦Ó«o©ñ 0x01,0x02,0x03,0x04
// ¦]¦¹¤£¯à§â Head struct ª½±µ«ü¦V¦¬¨ìªºBuffer
//---------------------------------------------------------------------------
//SMLSECS Message Language
//---------------------------------------------------------------------------
AnsiString SFCodeAndMean[]=
{
    "[S1F1] Are you There",
    "[S1F2] On Line Data",
    "[S1F3] Selected Status Request",
    "[S1F4] Selected Status Request",
    "[S1F5] Formatted Status Request",
    "[S1F6] Formatted Status Data",
    "[S1F9] Material Transfer Status Data",
    "[S1F10] Material Transfer Status Data",
    "[S1F11] Status Variable Namelist Request",
    "[S1F12] Status Variable Namelist Reply",
    "[S1F13] Connect Request",
    "[S1F14] Connect Request Acknowledge",
    "[S1F15] Request OFF-LINE",
    "[S1F16] OFF-LINE Acknowledge",
    "[S1F17] Request ON-LINE",
    "[S1F18] ON-LINE Acknowledge",
    "[S1F23] Collection Event Namelist Request",                                //2014/01/01  lee
    "[S1F24] Collection Event Namelist",                                        //2014/01/01  lee

    "[S2F13] Equipment Constant Request",
    "[S2F14] Equipment Constant Data",
    "[S2F15] New Equipment Constant Send",
    "[S2F16] New Equipment Constant Send Acknowledge",
    "[S2F17] Date and Time Request",
    "[S2F18] Date and Time Data",
    "[S2F21] Remote Command Send",
    "[S2F22] Remote Command Acknowledge",
    "[S2F23] Trace Initial Send",
    "[S2F24] Trace Initial Acknowledge",
    "[S2F25] Diagnostic Loopback Request",
    "[S2F26] Diagnostic Loopback Data",
    "[S2F27] Initiate Processing Request",
    "[S2F28] Initiate Processing Acknowledge",
    "[S2F29] Equipment Constant Namelist Request",
    "[S2F30] Equipment Constant Namelist Reply",
    "[S2F31] Date and Time Send",
    "[S2F32] Date and Time Acknowledge",
    "[S2F33] Define Report",
    "[S2F34] Define Report Acknowledge",
    "[S2F35] Link Event Report",
    "[S2F36] Link Event Report Acknowledge",
    "[S2F37] Enable/Disable Event Report",
    "[S2F38] Enable/Disable Event Report Acknowledge",
    "[S2F39] Multi-Block Inquire",
    "[S2F40] Multi-Block Grant",
    "[S2F41] Remote Command with Parameters",
    "[S2F42] Remote Command Acknowledge",
    "[S2F43] Reset Spooling Streams and Functions",
    "[S2F44] Reset Spooling Acknowledge",
    "[S2F47] Variable Limit Attribute Request",
    "[S2F48] Variable Limit Attribute Send",
    "[S2F49] Enhanced Remote Command",
    "[S2F50] Enhanced Remote Command Acknowledge",
    "[S2F65] Reset Spooling Streams and Functions",
    "[S2F66] Reset Spooling Streams and Functions Ack.",
    "[S2F85] Variable Limit Attribute Request",
    "[S2F86] Variable Limit Attribute Send",
    "[S2F87] Define Variable Limit Attributes",
    "[S2F88] Define Variable Limit Attributes Ack",
    "[S3F1] Material Status Request",
    "[S3F2] Material Status Data",
    "[S3F11] Material ID Request",
    "[S3F12] Material ID Request Acknowledge",
    "[S3F13] Material ID Send",
    "[S3F14] Material ID Acknowledge",
    "[S4F1] Ready to Send Material",
    "[S4F2] Ready to Send Acknowledge",
    "[S4F3] Handshake Complete",
    "[S4F5] Not Ready to Send",
    "[S4F17] Request to Receive Material",
    "[S4F18] Request to Receive Material Acknowledge",
    "[S5F1] Alarm Report Send",
    "[S5F2] Alarm Report Acknowledge",
    "[S5F3] Enable/Disable Alarm Send",
    "[S5F4] Enable/Disable Alarm Acknowledge",
    "[S5F5] List Alarm Request",
    "[S5F6] List Alarm Data",
    "[S5F7] List Enable Alarm Request",
    "[S5F8] List Enable Alarm Acknowledge",
    "[S5F73] Alarm Notification Send",
    "[S5F73] Alarm Notification Acknowledge",
    "[S6F1] Trace Data Send",
    "[S6F2] Trace Data Acknowledge",
    "[S6F3] Discrete Variable Data Send",
    "[S6F4] Discrete Variable Data Acknowledge",
    "[S6F5] Multi-block Data Send Inquire",
    "[S6F6] Multi-block Grant",
    "[S6F9] Formatted Variable Send",
    "[S6F10] Formatted Variable Acknowledge",
    "[S6F11] Event Report Send",
    "[S6F12] Event Report Acknowledge",
    "[S6F13] Annotated Event Report Send",
    "[S6F14] Annotated Event Report Acknowledge",
    "[S6F15] Event Report Request",
    "[S6F16] Event Report Data",
    "[S6F17] Annotated Event Report Request",
    "[S6F18] Annotated Event Report Data",
    "[S6F19] Individual Report Request",
    "[S6F20] Individual Report Data",
    "[S6F21] Annotated Individual Report Request",
    "[S6F22] Annotated Individual Report Data",
    "[S6F23] Request Spooled Data",
    "[S6F24] Request Spooled Data Acknowledgement Send",
    "[S6F65] Request Spooled Data",
    "[S6F66] Request Spooled Data Acknowledge",
    "[S7F1] Process Program Load Inquire",
    "[S7F2] Process Program Load Grant",
    "[S7F3] Process Program Send",
    "[S7F4] Process Program Acknowledge",
    "[S7F5] Process Program Request",
    "[S7F6] Process Program Data",
    "[S7F7] Process Program ID Request",
    "[S7F8] Process Program ID Data",
    "[S7F17] Delete Process Program Send",
    "[S7F18] Delete Process Program Acknowledge",
    "[S7F19] Current EPPID Request",
    "[S7F20] Current EPPID Data",
    "[S7F23] Formatted Process Program Send",                                   // 2013/09/10 lee
    "[S7F24] Formatted Process Program Acknowledge",                            // 2013/09/10 lee
    "[S9F1] Unrecognized Device ID",
    "[S9F3] Unrecognized Stream Type",
    "[S9F5] Unrecognized Function Type",
    "[S9F7] Illegal Data",
    "[S9F9] Transaction Timer Timeout",
    "[S9F11] Data Too Long",
    "[S9F13] Conversation Timeout",
    "[S9F67] Incorrect System Bytes Received",
    "[S10F1] Terminal Request",
    "[S10F2] Terminal Request Acknowledge",
    "[S10F3] Terminal Display Single",
    "[S10F4] Terminal Display Single Acknowledge",
    "[S10F5] Terminal Display Multi-block",
    "[S10F6] Terminal Display Multi-block Acknowledge",
    "[S10F9] Broadcast",
    "[S10F10] Broadcast Acknowledge",
    "[S14F3] Device ID Bin Map Send",                                           //JerryYang 20230322 : Get 2DID bin map
    "[S14F4] Device ID Bin Map Acknowledge",                                    //JerryYang 20230322 : Get 2DID bin map

    "[S101F1] ¦C¥X¦hÀÉ¦¡ÀÉ¦W"                                                   ,
    "[S101F2] ¦^¶Ç¦C¥X¦hÀÉ¦¡ÀÉ¦W"                                               ,
    "[S101F3] ¦C¥X¥Ø¿ý¦¡ÀÉ¦W"                                                   ,
    "[S101F4] ¦^¶Ç¦C¥X¥Ø¿ý¦¡ÀÉ¦W"                                               ,
    "[S101F5] Program file upload/download"                                     ,
    "[S101F6] Program file upload/download Acknowledge"                         ,
    "[S101F7] Program file upload/download use Directory"                       ,
    "[S101F11] ¶Ç°e¤@­ÓÀÉ®×"                                                    ,
    "[S101F12] ¶Ç°e¤@­ÓÀÉ®×¦^ÂÐ"                                                ,
    "[S101F8] Program file upload/download use Directory Acknowledge"           ,
    "[S102F1] ¨ú±oºû­×³æ³æ¸¹"                                                   ,
    "[S102F2] ¶Ç¦^ºû­×³æ³æ¸¹"                                                   ,
    "[S102F3] ¶Ç°eºû­×½Ð¨D"                                                     ,
    "[S102F4] ¶Ç°eºû­×½Ð¨D¦^ÂÐ"                                                 ,
    "[S102F5] ºû­×§¹¦¨¤JÀÉ"                                                     ,
    "[S102F6] ºû­×§¹¦¨¤JÀÉ¦^ÂÐ"                                                 ,
    "[S102F7] ¶}±Ò¬d¸ß³æ"                                                       ,
    "[S102F8] ¶Ç¦^¥Ø«e¬d¸ß³æ¸¹¤º®e"                                             ,
    "[S102F9] ­n¨DHost ¶Ç¦^¥Ø«e©Ò¦³³Q©w¸qªººû­×²M³æ"                            ,
    "[S102F10] Host ¶Ç¦^¥Ø«e©Ò¦³³Q©w¸qªººû­×²M³æ¨ì Equ"                         ,
    "[S102F21] ¶Ç¦^ Socket or Load Board or Kit  ¥Ø«e¨Ï¥Î¦¸¼Æ"                  ,
    "[S102F22] ¦^¶Ç Socket or Load Board or Kit ¥Ø«e¨Ï¥Î¦¸¼Æ"                   ,
    "[S102F23] ³]©w Socket or Load Board or Kit ¥Ø«e¨Ï¥Î¦¸¼Æ"                   ,
    "[S102F24] ¶Ç¦^³]©w Socket or Load Board or Kit ¥Ø«e¨Ï¥Î¦¸¼Æreply"          ,
    "[S102F25] «Ø¥ß·sSocket"                                                    ,
    "[S102F26] «Ø¥ß·s Socket or Load Board or Kit reply"                        ,
    "[S102F27] ±N Site ¥Î¨ìªº Socket or Load Board or Kit ¨ø¤U ( Host Database ¤£²M¸ê®Æ )",
    "[S102F28] ¦^À³ ±N Site ¥Î¨ìªº Socket or Load Board or Kit ¨ø¤U ( Host Database ¤£²M¸ê®Æ )©R¥O",
    "[S102F41] ¾Ú Run Card ¤U¸ü  work parameter"                                ,
    "[S102F42] ¾Ú Run Card ¤U¸ü  work parameter ¨ì Equipment"                   ,
    "[S103F11] Status Variable Namelist Request with Value"                     ,
    "[S103F12] Status Variable Namelist Reply with Value"                       ,
    "[S110F1] Process Program Data Check Request"                               ,
    "[S110F2] Process Program Data Check Acknowledge"                           ,
    "[S110F3] Process Program Download For ID Request"                          ,
    "[S110F4] Process Program Data Acknowledge"                                 ,
    "[S110F5] Customer Name List Acknowledge"                                   ,
    "[S110F6] Customer Name List Send"                                          ,
    "[S110F7] Receipe Information Acknowledge"                                  ,
    "[S110F8] Receipe Information Send"                                         ,
    "[S120F1] Setup File Information Acknowledge"                               ,
    "[S120F2] Setup File Information Send"                                      ,
    "[S125F1] Enable/Disable EC Data Send"                                      , //pig 2014.08.27 KYEC
    "[S125F2] Enable/Disable EC Data Acknowledge"                               , //pig 2014.08.27 KYEC
};
//---------------------------------------------------------------------------
// V 1.0
//  ³]©w­p®É¾¹
//  iTime:­p®É®É¶¡(³æ¦ì0.1¬í)
//---------------------------------------------------------------------------
void GemTimer::TimerSet(int iTime)
{
    iTimeLen=iTime*100;
}
//---------------------------------------------------------------------------
void GemTimer::TimerSetSecAndOn(double iTime)
{
    iTimeLen=iTime*1000;
    ulStartTicks=MyTickCount();
}
//---------------------------------------------------------------------------
void GemTimer::TimerSetMSAndOn(int iTime)
{
    iTimeLen=iTime;
    ulStartTicks=MyTickCount();
}
//---------------------------------------------------------------------------
// V 1.0
//  ¶}©l­p®É¾¹
//---------------------------------------------------------------------------
void GemTimer::TimerOn()
{
    ulStartTicks=MyTickCount();
}
//---------------------------------------------------------------------------
// V 1.0
//  Åª¨ú­p®É¾¹¬O§_®É¶¡¨ì
//---------------------------------------------------------------------------
bool GemTimer::TimerOff()
{
    if(ulStartTicks==0)
        return false;

    if(iTimeLen<=0)
    {                                                                           //jou 2012-01-04 µ{¦¡³Q³]¤J­tªºµ¥«Ý®É¶¡·|hang up¡C
        return true;
    }
    DWORD ulLimited=ulStartTicks+iTimeLen;
    DWORD ulNowTicks=MyTickCount();
    DWORD ulNowTicksOver=0;
    if(ulLimited<ulStartTicks)                                                  // ¦pªG¶W¹LDWORD½d³ò
    {
         ulLimited=0xFFFFFFFF-ulStartTicks+iTimeLen;
         ulNowTicksOver=0xFFFFFFFF-ulStartTicks+ulNowTicks;

        if(ulNowTicksOver<ulStartTicks &&                                       // ¦pªG¤p©ó¶}©lªºTicks¥B
            ulNowTicksOver>ulLimited)                                           // ¤j©ó¬É­­­È
        {
            return true;
        }
    }
    else
    {
        if(ulNowTicks>=(ulStartTicks+iTimeLen))
        {
            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------
//  V 1.0
//  ³]©w Time String Format and get pc information
//---------------------------------------------------------------------------
void THGem::GetTimeInfo()
{
    TDateTime dtPresent;

    dtPresent= Now();
    DecodeDate(dtPresent, SystemYear, SystemMonth, SystemDate);
    DecodeTime(dtPresent, SystemHour, SystemMin, SystemSec, SystemMSec);
    TimeString.sprintf("%04d-%02d-%02d %02d:%02d:%02d.%03d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec ,SystemMSec);

    if(iTimeFormat==1)                                                                                                                 //16 byte
        GemClock.sprintf("%04d%02d%02d%02d%02d%02d%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec/10);
    else if(iTimeFormat==2)                                                                                                            //14 byte
        GemClock.sprintf("%04d%02d%02d%02d%02d%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    else if(iTimeFormat==3)                                                                                                            //19 byte
        GemClock.sprintf("%04d-%02d-%02dT%02d:%02d:%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    else
        GemClock.sprintf("%02d%02d%02d%02d%02d%02d", SystemYear%100, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
//    if(GemTimeLabeTime!=NULL)
//        GemTimeLabeTime->Caption=GemClock;
    //2013/09/30  lee start
    if(LastGetDiskInfoMin==-1 || (LastGetDiskInfoMin!=SystemMin && (SystemMin%10)==0))
    {
        LastGetDiskInfoMin=SystemMin;
        GetDiskFreeSpaceMB('c', Disk_C_TotalSpaceMB, Disk_C_TotalFreeSpaceMB);
        GetDiskFreeSpaceMB('d', Disk_D_TotalSpaceMB, Disk_D_TotalFreeSpaceMB);
    }

    if(LastGetMemoryStatus==-1 || (LastGetMemoryStatus!=SystemMin && (SystemMin%5)==0))
    {
        LastGetMemoryStatus=SystemMin;
        GetGlobalMemoryStatusKB(ulMemoryLoad, ulTotalPhys, ulAvailPhys);
    }
    //2013/09/30  lee end
}
//---------------------------------------------------------------------------
//  V 1.0
//  ³]©w SECSII Format Byte , ¦ý Bit 0,1  ³]¦¨ 0
//---------------------------------------------------------------------------
void THGem::InitHType()
{
    HType.LIST_TYPE   =0x00;
    HType.ASCII_TYPE  =0x40;
    HType.JIS_TYPE    =0x44;
    HType.BINARY_TYPE =0x20;
    HType.BOOLEAN_TYPE=0x24;
    HType.INT_1_TYPE  =0x64;
    HType.INT_2_TYPE  =0x68;
    HType.INT_4_TYPE  =0x70;
    HType.INT_8_TYPE  =0x60;
    HType.UINT_1_TYPE =0xa4;
    HType.UINT_2_TYPE =0xa8;
    HType.UINT_4_TYPE =0xb0;
    HType.UINT_8_TYPE =0xa0;
    HType.FT_4_TYPE   =0x90;
    HType.FT_8_TYPE   =0x80;
}
//---------------------------------------------------------------------------
//  V 1.0
//  ·í Message Head ¤º SType or PType ¤£¬° 0 ºÙ¬° system byte,«O¯dµ¹ connect
//  command ¨Ï¥Î
//---------------------------------------------------------------------------
void THGem::InitSTypeStruct()
{
    SType.Data_Message =0;
    SType.Select_req   =1;
    SType.Select_rsp   =2;
    SType.Deselect_req =3;
    SType.Deselect_rsp =4;
    SType.Linktest_req =5;
    SType.Linktest_rsp =6;
    SType.Reject_req   =7;
    SType.Separate_req =9;
}
//---------------------------------------------------------------------------
// V 1.0
// ±N Data ±q³]­pªÌ«ü©w¤§ TMemo ¤¸¥ó show ¥X
//---------------------------------------------------------------------------
void __fastcall THGem::StringOut(AnsiString S)
{
    WaitShowString->Add(S);
    LogDataString->Add(S);
}
//---------------------------------------------------------------------------
// V 1.0
// ±N Data ±q³]­pªÌ«ü©w¤§ TMemo ¤¸¥ó ¥HBinary show ¥X
//---------------------------------------------------------------------------
void __fastcall THGem::StringBinaryOut(AnsiString S)
{
//    LogDataString->Add(S);
}
//---------------------------------------------------------------------------
// V 1.0
// ±N Data ±q³]­pªÌ«ü©w¤§ TMemo ¤¸¥ó show ¥X ¥]§tÃC¦â
//---------------------------------------------------------------------------
void __fastcall THGem::StringOut(AnsiString S, TColor C)
{
    if(DB!=NULL)
    {
        DB->Lines->Add(S);
        LogDataString->Add(S);
        DB->SelStart=DB->Lines->Count-1;
    }
}
//---------------------------------------------------------------------------
// V 1.0
// Àx¦sSECS LOG
//---------------------------------------------------------------------------
void __fastcall THGem::SaveSECSGEMErrToLog(AnsiString asSaveStr)                //20130409 Daver add
{
    TDateTime tdSaveTime=Now();
    AnsiString asPath, asFN;
    FILE *P;
    DateSeparator='_';                                                          // Sam 20210225 : ¼W¥[«OÅ@
    asPath.sprintf("D:\\SECS_GEM_LOGS\\%s\\%s", tdSaveTime.FormatString("yyyy"), tdSaveTime.FormatString("mm_dd"));         //JerryYang 20170309 (wei) ­×¥¿secs gem log path¿ù»~
    MyForceDirectories(asPath);
    asFN.sprintf("%s\\SECSGEM_ErrLog_%s.txt", asPath, tdSaveTime.FormatString("hh"));       //JerryYang 20170309 ­×¥¿secs gem log path¿ù»~

    P=fopen(asFN.c_str(), "a+");
    if(P!=NULL)
    {
        fputs(asSaveStr.c_str(), P);
        fputs("\n", P);
        fclose(P);
    }
}
//---------------------------------------------------------------------------
// V 1.0
// Initial function
//---------------------------------------------------------------------------
__fastcall THGem::THGem(TComponent* Owner)
        : TForm(Owner)
{
    SetCurrentDirectory("D:\\HT9045\\SECS\\SECS");
    LastGetDiskInfoMin=-1;                                                      //2013/09/30
    LastGetMemoryStatus=-1;                                                     //2013/09/30
    bFirstEntry     =true;
    DefaultAddress  ="";                                                        //°²¦p²Ä¤@¦¸µn¤J,Address , Port ³£¤@¼Ë,®e©ö¨Ò¥~
    DefaultPort     ="";                                                        //¬G§Q¥Î "" ¨Ó¤£¨Ï¤¸¥ó enable
    DefaultDeviceID ="";
    T3TimeOut       =30;                                                        //pig 2014.07.28 KYEC_SECS
    T5TimeOut       =30;
    T6TimeOut       =30 ;
    T7TimeOut       =30;
    T8TimeOut       =30;

    bReportSECS_GEM_Message=false;
    bServoSocketConnect=false;
    bReceiveS7F6=false;
    bReceiveS101F5=false;
    bReceiveS101F6=false;
    bReceiveS101F7=false;
    bReceiveS101F8=false;

    bUseClientSocket=false;
    fALID_ALED_Exist=false;
    bALEDflag=true;
    ctSpoolFile=0;
    bFirstBlock=true;

    iEthernetBufferLen=10240;
    EthernetBuffer=new unsigned char [iEthernetBufferLen];

    iOpenCommuncationTask=1;
    countConnect=0;
    iConnectTryCount=0;
    iReportECDataChangeCheckOldSecond=0;
    iSpoolTask=1;
    iUploadFileToHost_ForSingleFile=1;
    iUploadFileToHost_ForMultiFileTask=1;
    iUploadFileToHost_ForDirectoryFileTask=1;
    iAutoConnectSec=0;
    iAutoConnectDelay=2;
    ctUpdateStatus=0;

    flag1UpdateStatus=true;
    flag2UpdateStatus=true;
    flag3UpdateStatus=true;
    iServoConnectCT=-1;
    bConnectUpdateStatus=true;
    Timer1Task=1;
    Timer1ct=0;
    iRetryCTDownLoadRemoteFile=0;
    iDownLoadRemoteFileTask=1;

    GemLinkState=0;
    SECSCommunicationMode=0;

    StringOutColor=clBlack;
    MoveCheckCallBack=NULL;
    GemRemoteReceipeList=NULL;

    GemDownLoadRemoteFileName=NULL;
    GemDeleteRemoteFileName=NULL;
    GemSelectAllRemoteFile=NULL;
    GemDisSelectAllRemoteFile=NULL;
    GemLocalFileLixtBox=NULL;
    GemRefreshLocalFile=NULL;
    GemUpLoadLocalFileName=NULL;
    GemSelectAllLocalFile=NULL;
    GemDisSelectAllLocalFile=NULL;

    iSMLSpace=0;
    SpaceStr="";
    DeviceID=0;
    EquipmentSystemByte=0;
    Local.DeviceID=DeviceID;                                                    // need define by user
    Local.SystemByte=EquipmentSystemByte;

    bConnect=false;
    iStartConnectTask=1;
    bStartConnect=false;

    bOnLineLocal=false;
    iStartOnLineTask=1;
    bStartOnLine=false;
    bOnLine=false;

    bWaitSelectRsp=false;
    bWaitDeSelectRsp=false;

    bWaitEstablishCommunicationsResponse=false;
    bWaitEstablishCommunicationsResponseError=false;
    DB=NULL;
    bAreYouThereRequest=false;
    bS1F2_OnLineData=false;
    EvenCount=1;
    bSeprate=false;
    SReceiveData            =new TStringList;
    SReceiveDataBackup      =new TStringList;

    SReceiveData->Clear();
    SReceiveDataBackup->Clear();
    bReceiveData=false;

    SV_ID                   =new TStringList;
    SV_TYPE                 =new TStringList;
    SV_NAME                 =new TStringList;
    SV_UNIT                 =new TStringList;
    SV_Ptr                  =new TList;
    VCL_NAME                =new TStringList;
    SV_LEN                  =new TStringList;
    SV_Remark               =new TStringList;

    SV_ID->Clear();
    SV_TYPE->Clear();
    SV_NAME->Clear();
    SV_UNIT->Clear();
    SV_Ptr->Clear();
    VCL_NAME->Clear();
    SV_LEN->Clear();
    SV_Remark->Clear();

    UploadFileString        =new TStringList;
    EC_ID                   =new TStringList;
    EC_TYPE                 =new TStringList;
    EC_NAME                 =new TStringList;
    EC_UNIT                 =new TStringList;
    EC_Ptr                  =new TList;
    EC_Ptr_Min              =new TList;
    EC_Ptr_Max              =new TList;
    EC_Ptr_Default          =new TList;
    EC_Remark               =new TStringList;
    EC_VCL_NAME             =new TStringList;
    EC_Ptr_Min_Value        =new TStringList;
    EC_Ptr_Max_Value        =new TStringList;
    EC_Ptr_Default_Value    =new TStringList;
    EC_OldValue             =new TStringList;
    RequestRemoteDownLoad   =new TStringList;
    UploadFileName          =new TStringList;
    TimeLeft                =new TStringList;

    RequestRemoteDownLoad->Clear();

    for(int i=0; i<iTraceDataCT; i++)
    {
        TraceData[i]=new TStringList;
        TraceData[i]->Clear();
    }

    UploadFileName->Clear();
    TimeLeft->Clear();
    for(int i=0; i<iTraceDataCT; i++)
        bTraceData[i]=false;

    TerminalDisplayIndex=0;
    TerminalDisplayIndex2=0;

    InitHType();
    InitSTypeStruct();
    bBeginTransferSpool=false;
    bSpooling=false;
    bAutoConnect=false;
    bSpoolActive=false;
    bTCPIP_Error=false;
    bOpenCommuncation=false;
    bCloseCommuncation=false;
    SECSConnectionState=NULL;
    GEMCommunicatingState=NULL;
    GemPanelControlState=NULL;
    bReceiveEstablishCommunicationsRequest=false;
    bDisableBinaryShow=false;
    EnableOrDisablePtr=NULL;
    bShow=false;
    LogDataString       =new TStringList;
    slTempReportID      =new TStringList;
    lTempReportIDContent=new TList;
    slTempCeID          =new TStringList;
    lTempCeIDContent    =new TList;

    LogDataString->Clear();
    slTempReportID->Clear();
    lTempReportIDContent->Clear();
    slTempCeID->Clear();
    lTempCeIDContent->Clear();

    strGrdCEID->Cells[0][0]="CEID";
    strGrdCEID->Cells[1][0]="Enable";
    strGrdCEID->Cells[2][0]="Alias";
    strGrdCEID->Cells[3][0]="ReportID";
    strGrdCEID->ColWidths[2]=200;

    stdGridReportID->Cells[0][0]="ReportID";
    stdGridReportID->Cells[1][0]="Type";
    stdGridReportID->Cells[2][0]="SVID";


    sgSECSECData->ColWidths[0]=40;                                                  //pig 2014.08.27 KYEC start
    sgSECSECData->ColWidths[1]=100;
    sgSECSECData->ColWidths[2]=60;
    sgSECSECData->ColWidths[3]=200;

    sgSECSECData->Cells[0][0]="No.";
    sgSECSECData->Cells[1][0]="ECID";
    sgSECSECData->Cells[2][0]="Enable";
    sgSECSECData->Cells[3][0]="Function";

    iMinTimeFormat=0;
    iMaxTimeFormat=3;

    rgRole->OnClick=ManualCreatergRoleClick;
    bReceiveMultiConnect=false;

    SocketReceiveList  =new TList;
    SocketReceiveList->Clear();
    SocketReceiveLength=new TStringList;
    SocketReceiveLength->Clear();

    WaitShowString=new TStringList;
    WaitShowString->Clear();

    SECSReportIDChange = new TStringList;                                       //pig 2014.04.23 KYEC SECS
    SECSOriginalValue  = new TStringList;                                       //pig 2014.04.23 KYEC SECS
    SECSNewECValue     = new TStringList;                                       //pig 2014.04.23 KYEC SECS

    pLockOnSocketRecvice=new TFixedCriticalSection;                             //16.10.05.00 Roy Add (Debug from kirin)
    RecvMemoryBuffer    =new TMemoryStream;
    ProcBuffer          =new TMemoryStream;
    TempProcBuffer      =new TMemoryStream;
    iFileCount          =0;
    csSFCodeResponse    =new TCriticalSection();                                // 20221111 Joseph (Jason) ·s¼W TCriticalSection //
}
//---------------------------------------------------------------------------
__fastcall THGem::~THGem()                                                      //16.09.14.00s Roy Add
{
    try
    {
        delete [] MulitBuffer;
        delete pLockOnSocketRecvice;                                            //16.10.05.00 Roy Add (Debug from kirin)
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~THGem");
    }
    LogSoftwareOffTime("THGem::~THGem()");
}                                                                               //16.09.14.00e
//---------------------------------------------------------------------------
// V 1.0
// close
//---------------------------------------------------------------------------
void __fastcall THGem::FormDestroy(TObject *Sender)
{
    AnsiString str;
    try
    {
        clientGem->Active=false;
        srvGem->Active=false;                                                   //Steven 20140530
        memset(EthernetBuffer,    0x00, sizeof(EthernetBuffer));
        delete[] EthernetBuffer;                                                //20140308 wei
        EthernetBuffer=NULL;                                                    //kevin 20180517
        //===================  TStringList  ================
        SReceiveData->Clear();                                                  //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        SReceiveDataBackup->Clear();                                            //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        SV_ID->Clear();                                                         //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        SV_TYPE->Clear();                                                       //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        SV_NAME->Clear();                                                       //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        SV_UNIT->Clear();                                                       //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        VCL_NAME->Clear();                                                      //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        SV_LEN->Clear();                                                        //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        SV_Remark->Clear();                                                     //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        UploadFileString->Clear();                                              //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        EC_ID->Clear();                                                         //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        EC_TYPE->Clear();                                                       //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        EC_NAME->Clear();                                                       //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        EC_UNIT->Clear();                                                       //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        EC_VCL_NAME->Clear();                                                   //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        EC_Remark->Clear();                                                     //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        EC_Ptr_Min_Value->Clear();                                              //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        EC_Ptr_Max_Value->Clear();                                              //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        EC_Ptr_Default_Value->Clear();                                          //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        EC_OldValue->Clear();                                                   //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        RequestRemoteDownLoad->Clear();                                         //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        UploadFileName->Clear();                                                //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        TimeLeft->Clear();                                                      //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        LogDataString->Clear();                                                 //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        slTempReportID->Clear();                                                //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        slTempCeID->Clear();                                                    //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        SocketReceiveLength->Clear();                                           //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        WaitShowString->Clear();                                                //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        SECSReportIDChange->Clear();                                            //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        SECSOriginalValue->Clear();                                             //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        SECSNewECValue->Clear();                                                //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        delete SReceiveData;
        delete SReceiveDataBackup;
        delete SV_ID;
        delete SV_TYPE;
        delete SV_NAME;
        delete SV_UNIT;
        delete VCL_NAME;
        delete SV_LEN;
        delete SV_Remark;
        delete UploadFileString;
        delete EC_ID;
        delete EC_TYPE;
        delete EC_NAME;
        delete EC_UNIT;
        delete EC_VCL_NAME;
        delete EC_Remark;
        delete EC_Ptr_Min_Value;
        delete EC_Ptr_Max_Value;
        delete EC_Ptr_Default_Value;
        delete EC_OldValue;
        delete RequestRemoteDownLoad;
        delete UploadFileName;
        delete TimeLeft;
        delete LogDataString;
        delete slTempReportID;
        delete slTempCeID;
        delete SocketReceiveLength;
        delete WaitShowString;
        delete SECSReportIDChange;                                              //pig 2014.04.23 KYEC SECS
        delete SECSOriginalValue;                                               //pig 2014.04.23 KYEC SECS
        delete SECSNewECValue;                                                  //pig 2014.04.23 KYEC SECS
        for(int i=0; i<iTraceDataCT; i++)
        {
            if(TraceData[i]!=NULL)                                              //Steven 20161220 (jou) : ­×¥¿delete¤è¦¡
            {
                TraceData[i]->Clear();
                delete TraceData[i];
            }
        }
        //===================  TStringList  ================

        //================  TList  ================
        SV_Ptr->Clear();                                                        //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        EC_Ptr->Clear();                                                        //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        EC_Ptr_Min->Clear();                                                    //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        EC_Ptr_Max->Clear();                                                    //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        EC_Ptr_Default->Clear();                                                //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        lTempReportIDContent->Clear();                                          //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        lTempCeIDContent->Clear();                                              //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        SocketReceiveList->Clear();                                             //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        delete SV_Ptr;
        delete EC_Ptr;
        delete EC_Ptr_Min;
        delete EC_Ptr_Max;
        delete EC_Ptr_Default;
        delete lTempReportIDContent;
        delete lTempCeIDContent;
        delete SocketReceiveList;
        //================  TList  ================

        RecvMemoryBuffer->Clear();
        ProcBuffer->Clear();
        TempProcBuffer->Clear();
        delete RecvMemoryBuffer;
        delete ProcBuffer;
        delete TempProcBuffer;
        delete csSFCodeResponse;                                                // 20221111 Joseph (Jason) §R°£ TCriticalSection //
    }
    catch(...)
    {
        MyDBIProcess("Exception", "THGem::FormDestroy");
    }
    LogSoftwareOffTime("THGem, FormDestroy");
}
//---------------------------------------------------------------------------
void THGem::SetCurrentDirectory(AnsiString Path)
{
    CurrentDirectory=Path;
    GemSystemPath.sprintf("%sSYSTEM", IncludeTrailingPathDelimiter(CurrentDirectory));
    GemSpoolPath.sprintf("%sSPOOL", IncludeTrailingPathDelimiter(CurrentDirectory)+AnsiString());
    GemSystemIniPath.sprintf("%ssecs_gem.ini", IncludeTrailingPathDelimiter(GemSystemPath));

    MyForceDirectories(GemSystemPath, __FUNC__);
    MyForceDirectories(GemSpoolPath, __FUNC__);
}
//---------------------------------------------------------------------------
// V 1.0
//  read Alarm is need report value
// ---------------------------------------------------------------------------
AnsiString __fastcall THGem::ReadALED(AnsiString ALID)
{
    AnsiString S, FileName;
    if(bALEDflag==true)
    {
        bALEDflag=false;
    }

    FileName.sprintf("%sALID_ALED.ini", IncludeTrailingPathDelimiter(GemSystemPath));
    S=ReadIniData(FileName, "ALID", ALID, AnsiString("1"));
    return S;
}
//------------------------------------------------------------------------------
void __fastcall THGem::WriteALED(AnsiString ALID, AnsiString S1)
{
    AnsiString FileName;
    FileName.sprintf("%sALID_ALED.ini", IncludeTrailingPathDelimiter(GemSystemPath));
    WriteIniData(FileName, "ALID", ALID, S1);
}
//---------------------------------------------------------------------------
// V 1.0
//  §â­n¶Ç°e¸ê®Æ set to buffer ¥H MSB ¤è¦¡¦s
// ---------------------------------------------------------------------------
void THGem::ConvertLocalData(int len, void *Value)
{
    unsigned char *p;
    p=(unsigned char *)Value;
    for(int i=0; i<len; i++)
        LocalBuffer[LocalLength_4+i]=p[len-1-i];
    LocalLength_4+=len;
}
//---------------------------------------------------------------------------
// V 1.0
//  §â­n¶Ç°eªº Head ¸ê®Æ ¥H MSB ¤è¦¡¦s¨ì Buffer
//---------------------------------------------------------------------------
void THGem::CreateLocalHead()
{
    unsigned int i, j;
    unsigned char c;
    AnsiString S;
    memset(LocalBuffer, 0, sizeof(LocalBuffer));
    LocalLength=0;
    LocalLength_4=4;
    ConvertLocalData(2, &Local.DeviceID);
    c=Local.MessageID_S;
    if(Local.W_Bit==1)
        c|=0x80;
    ConvertLocalData(1, &c);
    ConvertLocalData(1, &Local.MessageID_F);
    ConvertLocalData(1, &Local.PType);
    ConvertLocalData(1, &Local.SType);
    j=Local.SystemByte;
    for(i=0; i<4; i++)
    {
        LocalBuffer[LocalLength_4+3-i]=j&0xff;
        j>>=8;
    }
    LocalLength_4+=4;
    LocalLength=LocalLength_4-4;
    j=LocalLength;
    for(i=0; i<4; i++)
    {
        LocalBuffer[3-i]=j&0xff;
        j>>=8;
    }
}
//---------------------------------------------------------------------------
//  V 1.0
//  §â­n°e¥Xªº°T®§ Message Head ¤©¥H³]©w
//---------------------------------------------------------------------------
void THGem::InitLocalHead(int SCode, int FCode, int WBit)
{
    Local.MessageID_S=SCode;
    Local.MessageID_F=FCode;
    Local.W_Bit=WBit;
    Local.PType=0;
    Local.SType=0;
    if((FCode%2)==0)
    {
        Local.SystemByte=Remote.SystemByte;
    }
    else
    {
        EquipmentSystemByte++;
        Local.SystemByte=EquipmentSystemByte;
    }
    LocalLength=0;
    CreateLocalHead();
}
//---------------------------------------------------------------------------
// V 1.0
// ¶Ç¦^¤£¦P Type ¤À§O¥e´X­Ó¦ì¤¸²Õ
//---------------------------------------------------------------------------
int THGem::GetLengthOfType(unsigned char Type)
{
    if(Type==HType.LIST_TYPE        || Type==HType.ASCII_TYPE   || Type==HType.BINARY_TYPE  ||
       Type==HType.BOOLEAN_TYPE     || Type==HType.UINT_1_TYPE  || Type==HType.INT_1_TYPE   )
        return 1;
    else if(Type==HType.UINT_2_TYPE || Type==HType.INT_2_TYPE)
        return 2;
    else if(Type==HType.UINT_4_TYPE || Type==HType.INT_4_TYPE   || Type==HType.FT_4_TYPE)
        return 4;
    else if(Type==HType.UINT_8_TYPE || Type==HType.INT_8_TYPE   || Type==HType.FT_8_TYPE)
        return 8;
    else
    {
        StringOut("SECSII format byte not define error?");
        return 0;
    }
}
//---------------------------------------------------------------------------
// V 1.0
// ±N­n¿é¥X¤§ data ,®Ú¾Ú¨ä Byte ªø«×¨M©w SML ªº°_©l Byte ¤¤ªº Length of Byte
// §Y Bit 0,1  ¾l¬° Type bit
// ¨Ã±N¹ê»Ú¿é¥X¤§ data total Byte ©ñ¨ì ¶Ç¤J¤§ ptr pointer ¤ºµ¹ call function
// ¨Ã¦^¶Ç¦@»Ý¦h¤Ö byte ¥H«Kcall function ¯à³]©w Type »P buffer pointer ¼Æ
//---------------------------------------------------------------------------
unsigned char THGem::GetLengthByte(unsigned  len, unsigned char *Ptr)
{
    unsigned char ct=0;
    unsigned char Temp[3];
    do
    {
        if(ct>=3)
        {
            StringOut("data length byte over 3 error");
            return 0;
        }
        Ptr[ct]=len&0xff;
        len>>=8;
        ct++;
    }while(len>0);

    if(ct==2)
    {
        Temp[0]=Ptr[1]; // §â MSB ¥ý©ñ
        Temp[1]=Ptr[0];
        Ptr[0]=Temp[0];
        Ptr[1]=Temp[1];
    }
    else if(ct==3)
    {
        Temp[0]=Ptr[2]; // §â MSB ¥ý©ñ
        Temp[1]=Ptr[1];
        Temp[2]=Ptr[0];
        Ptr[0]=Temp[0];
        Ptr[1]=Temp[1];
        Ptr[2]=Temp[2];
    }
    return ct;
}
//---------------------------------------------------------------------------
// V 1.0
//±N­n°e¥Xªº¸ê®ÆÂà¦¨ Data stream format
//---------------------------------------------------------------------------
void THGem::DataItemOut(unsigned char Type, AnsiString S)
{
    char *Ptr;
    Ptr=S.c_str();
    DataItemOut(strlen(Ptr), Type, Ptr);
}
//---------------------------------------------------------------------------
void THGem::DataItemOut(int len, unsigned char Type, void *P)
{
    int i, k;
    unsigned char SMLLength;
    unsigned char SMLLengthData[3];
    unsigned char DataSize;

    DataSize=GetLengthOfType(Type);
    SMLLength=GetLengthByte(len*DataSize, SMLLengthData);
    LocalBuffer[LocalLength_4]=Type|SMLLength;
    LocalLength_4++;
    for(int i=0; i<SMLLength; i++)
        LocalBuffer[LocalLength_4+i]=SMLLengthData[i];
    LocalLength_4+=SMLLength;

    if(Type==HType.ASCII_TYPE || Type==HType.BINARY_TYPE)
    {
        unsigned char *ptr;
        ptr=(unsigned char *)P;
        for(i=0; i<len; i++)
            LocalBuffer[LocalLength_4+i]=ptr[i];
        LocalLength_4+=len;
    }
    else if(Type==HType.BOOLEAN_TYPE)
    {
        bool *ptr;
        ptr=(bool *)P;
        for(i=0; i<len; i++)
        {
            if(ptr[i]==true)
                LocalBuffer[LocalLength_4]=1;
            else
                LocalBuffer[LocalLength_4]=0;
            LocalLength_4++;
        }
    }
    else if(Type==HType.UINT_4_TYPE)
    {
        unsigned int *ptr, j;
        ptr=(unsigned int *)P;
        for(i=0; i<len; i++)
        {
            j=ptr[i];
            for(k=0; k<DataSize; k++)
            {
                LocalBuffer[LocalLength_4+DataSize-1-k]=(unsigned char)(j&0xff);
                j>>=8;
            }
            LocalLength_4+=DataSize;
        }
    }
    else if(Type==HType.UINT_2_TYPE)
    {
        unsigned short *ptr, j;
        ptr=(unsigned short *)P;
        for(i=0; i<len; i++)
        {
            j=ptr[i];
            for(k=0; k<DataSize; k++)
            {
                LocalBuffer[LocalLength_4+DataSize-1-k]=(unsigned char)(j&0xff);
                j>>=8;
            }
            LocalLength_4+=DataSize;
        }
    }
    else if(Type==HType.UINT_1_TYPE)
    {
        unsigned char *ptr, j;
        ptr=(unsigned char *)P;
        for(i=0; i<len; i++)
        {
            j=ptr[i];
            for(k=0; k<DataSize; k++)
            {
                LocalBuffer[LocalLength_4+DataSize-1-k]=(unsigned char)(j&0xff);
//                j>>=8;                                                        //Steven 20150814 : mark UINT_1¸òINT_1¥»¨Óªø«×´N¬O1, ¤£»Ý­n¦ì²¾
            }
            LocalLength_4+=DataSize;
        }
    }
    else if(Type==HType.UINT_8_TYPE)
    {
        unsigned __int64 *ptr, j;
        ptr=(unsigned __int64 *)P;
        for(i=0; i<len; i++)
        {
            j=ptr[i];
            for(k=0; k<DataSize; k++)
            {
                LocalBuffer[LocalLength_4+DataSize-1-k]=(unsigned char)(j&0xff);
                j>>=8;
            }
            LocalLength_4+=DataSize;
        }
    }
    else if(Type==HType.INT_1_TYPE)
    {
        char *ptr, j;
        ptr=(char *)P;
        for(i=0; i<len; i++)                                                    // °}¦C¦³len µ§
        {
            j=ptr[i];
            for(k=0; k<DataSize; k++)
            {
                LocalBuffer[LocalLength_4+DataSize-1-k]=(unsigned char)(j&0xff);
//                j>>=8;                                                        //Steven 20150814 : mark UINT_1¸òINT_1¥»¨Óªø«×´N¬O1, ¤£»Ý­n¦ì²¾
            }
            LocalLength_4+=DataSize;
        }
    }
    else if(Type==HType.INT_2_TYPE)
    {
        short *ptr, j;
        ptr=(short *)P;
        for(i=0; i<len; i++)
        {
            j=ptr[i];
            for(k=0; k<DataSize; k++)
            {
                LocalBuffer[LocalLength_4+DataSize-1-k]=(unsigned char)(j&0xff);
                j>>=8;
            }
            LocalLength_4+=DataSize;
        }
    }
    else if(Type==HType.INT_4_TYPE)
    {
        int *ptr, j;
        ptr=(int *)P;
        for(i=0; i<len; i++)
        {
            j=ptr[i];
            for(k=0; k<DataSize; k++)
            {
                LocalBuffer[LocalLength_4+DataSize-1-k]=(unsigned char)(j&0xff);
                j>>=8;
            }
            LocalLength_4+=DataSize;
        }
    }
    else if(Type==HType.INT_8_TYPE)
    {
        __int64 *ptr, j;
        ptr=(__int64 *)P;

        for(i=0; i<len; i++)
        {
            j=ptr[i];
            for(k=0; k<DataSize; k++)
            {
                LocalBuffer[LocalLength_4+DataSize-1-k]=(unsigned char)(j&0xff);
                j>>=8;
            }
            LocalLength_4+=DataSize;
        }
    }
    else if(Type==HType.FT_4_TYPE)
    {
        float  *floatPtr;
        floatPtr=(float *)P;
        unsigned char *ptr;
        for(i=0; i<len; i++)
        {
            ptr=(unsigned char *)(&floatPtr[i]);
            for(k=0; k<DataSize; k++)
            {
                LocalBuffer[LocalLength_4+DataSize-1-k]=ptr[0];
                ptr++;
            }
            LocalLength_4+=DataSize;
        }
    }
    else if(Type==HType.FT_8_TYPE)
    {
        double *doublePtr;
        unsigned char *ptr;
        doublePtr=(double *)P;
        for(i=0; i<len; i++)
        {
            ptr=(unsigned char *)(&doublePtr[i]);
            for(k=0; k<DataSize; k++)
            {
                LocalBuffer[LocalLength_4+DataSize-1-k]=*ptr;
                ptr++;
            }
            LocalLength_4+=DataSize;
        }
    }

    LocalLength=LocalLength_4-4;
    k=LocalLength;
    for(i=0; i<4; i++)
    {
        LocalBuffer[3-i]=k&0xff;
        k>>=8;
    }
}
//---------------------------------------------------------------------------
// V 1.0
// ¬°¤F«K©ó¾\Åª¥[¤J¬q¸¨«e­±¦h¤ÖªÅ¥Õ¨Ó¤À¬q ¦p:
// L[2]
//     <
//         <A[4] "Hello">
//         <INT4[2] 100 200 >
//     >
// Make SML Tab space for easy to read
//---------------------------------------------------------------------------
void THGem::MakeSMLSpaceString()
{
//    char spacestr[512];
    if(iSMLSpace<=0)
    {
        SpaceStr="";
        return ;
    }

//    for(int i=0; i<iSMLSpace*2; i++)
//        spacestr[i]=' ';
//    spacestr[iSMLSpace*2]=0;
    SpaceStr.sprintf("% *s", iSMLSpace*2, AnsiString(" "));                     //Steven 20250709 : ÅÜ§ó¶ñ¤JªÅ®æªº¤è¦¡
}
//---------------------------------------------------------------------------
// V 1.0
// ±N­n show ªº string ¥[¤W Tab space ¨Ã show ¥X
//---------------------------------------------------------------------------
void THGem::ShowSMLSpaceString(AnsiString S)
{
    S=SpaceStr+S;
    StringOut(S);
}
//------------------------------------------------------------------------------
void THGem::ShowSMLSpaceBinaryString(AnsiString S)
{
    if(GemCheckBoxShowBinary->Checked==false)                                   //Steven 20211109 : SECS¤£­n¨C¦¸³£Åã¥Übinary code
        return ;
    S=SpaceStr+S;
    StringBinaryOut(S);
}
//---------------------------------------------------------------------------
// V 1.0
// Function¥Øªº : ¨ú±o¦¹ data item ¦@¦³´Xµ§¸ê®Æ¥Î¨ìªº Byte ¼Æ ,¦p INT4 ªº¸ê®Æ¦³ 1 µ§«h·|¦^¶Ç 4
//
// TypeChar  : ¬° Format Bits | Length bits ªº²Õ¦X ,bit 0,1 ¬°¤§«á¦@¥Î´X­Ó Byte ¨Óªí¥Ü¨C¤@ Item ¤º¦³¦h¤Öµ§¼Æ
// Ptr       : ¬°¸ê®Æ Buffer ¤§°_©l«ü¼Ð
// RunLength : ¬°³o¦¸ Length Pointer ¤§¬Û¹ï¯Á¤Þ¦ì¸m
// ¶Ç¦^¦¹ Data Item ¤§ªø«×©Ò¥Î¨ì¤§ Byte ¼Æ¶q
// ¦ý¬O¤£·|§ïÅÜ RunLength ªº¤º®e
//---------------------------------------------------------------------------
int __fastcall THGem::GetSMLLenthByte(unsigned char TypeChar, unsigned char *Ptr, int RunLength)
{
    int len=TypeChar&0x03;                                                      // get length of byte
    int ct=0;
    for(int i=0; i<len; i++)
    {
        ct<<=8;
        ct+=Ptr[RunLength+i];
    }
    return ct;
}
//---------------------------------------------------------------------------
// V 1.0
// ±N ¦¬°e¸ê®Æ ¥H SML ®æ¦¡ »¼¦^©I¥s ¦L¥X Hex data
//---------------------------------------------------------------------------
int __fastcall THGem::ProcessSMLBinary(unsigned char *Ptr, int Len, int &RunLength)
{
    int ItemSize, lct, ret;
    AnsiString str;
    unsigned char ct, TypeChar, c;
    AnsiString S;

    MakeSMLSpaceString();
    if(RunLength>=Len)
        return -1;

    TypeChar=Ptr[RunLength];
    c=TypeChar&0xfc;
    if(c!=HType.LIST_TYPE)
    {
        lct=1;
    }
    else
    {
        RunLength++;
        lct=GetSMLLenthByte(TypeChar, Ptr, RunLength);                          // ¨ú±o¦¹ data item ¦@¦³´Xµ§
        ct=TypeChar & 0x03;
        if(ct==1)
            str.sprintf("%02X %02X", TypeChar, Ptr[RunLength]);
        else if(ct==2)
            str.sprintf("%02X %02X %02X", TypeChar, Ptr[RunLength], Ptr[RunLength+1]);
        else if(ct==3)
            str.sprintf("%02X %02X %02X %02X", TypeChar, Ptr[RunLength], Ptr[RunLength+1], Ptr[RunLength+2]);
        RunLength+=ct;
        ShowSMLSpaceBinaryString(str.c_str());
        iSMLSpace++;
        MakeSMLSpaceString();
    }

    for(int i=0; i<lct; i++)
    {
        TypeChar=Ptr[RunLength];
        c=TypeChar & 0xfc;
        RunLength++;
        if(c==HType.LIST_TYPE)
        {
            RunLength--;
            if(RunLength>=Len)
                return -2;
            ret=ProcessSMLBinary(Ptr, Len, RunLength);
            if(ret<0)
                return ret;
            else
                continue;
        }

        ct=TypeChar&0x03;

        if(ct==1)
            str.sprintf("%02X %02X", TypeChar, Ptr[RunLength]);
        else if(ct==2)
            str.sprintf("%02X %02X %02X", TypeChar, Ptr[RunLength], Ptr[RunLength+1]);
        else if(ct==3)
            str.sprintf("%02X %02X %02X %02X", TypeChar, Ptr[RunLength], Ptr[RunLength+1], Ptr[RunLength+2]);

        S=str;
        ItemSize=GetSMLLenthByte(TypeChar, Ptr, RunLength);
        RunLength+=ct;
        for(int j=0; j<ItemSize; j++)
        {
            if(RunLength>=Len)
            {
                ShowSMLSpaceBinaryString(S);
                return -2;
            }
            str.sprintf(" %02X",Ptr[RunLength]);
            S+=AnsiString(str);
            RunLength++;
        }
        ShowSMLSpaceBinaryString(S);
    }
    iSMLSpace--;
    MakeSMLSpaceString();
    return 0;
}
//---------------------------------------------------------------------------
// V1.0
// ±N ¦¬°e¸ê®Æ©I¥s ProcessSMLBinary ¥H SML ®æ¦¡¦L¥X Hex format
//---------------------------------------------------------------------------
int __fastcall THGem::ShowSMLBinary(unsigned char *Ptr, int len)
{
    int RunLength=14;
    int ret=0;
    iSMLSpace=0;
    if(GemCheckBoxShowBinary->Checked==true)                                    //Steven 20211109 : SECS¤£­n¨C¦¸³£Åã¥Übinary code
    {
        ret=ProcessSMLBinary(Ptr, len, RunLength);
        StringBinaryOut("");
    }
    return ret;
}
//---------------------------------------------------------------------------
// V1.0
// µø³]©w¬O§_±N­n°e¥X¸ê®Æ ¥H Binary ªº®æ¦¡±N¥þ³¡¸ê®Æ¦L¥X
//---------------------------------------------------------------------------
void THGem::ShowLocalBufferBinaryData()
{
    if(GemCheckBoxShowBinary->Checked==false)
        return ;

    AnsiString S="";
    AnsiString str;
    StringOut("");

    for(int i=0; i<4; i++)
    {
        str.sprintf("%02X,", LocalBuffer[i]);
        S+=AnsiString(str);
    }
    StringBinaryOut(S);
    S="";
    for(int i=4; i<14; i++)
    {
        str.sprintf("%02X,", LocalBuffer[i]);
        S+=AnsiString(str);
    }
    StringBinaryOut(S);
    ShowSMLBinary(LocalBuffer, LocalLength_4);
}
//---------------------------------------------------------------------------
// V1.0
// µø³]©w¬O§_±N­n°e¥X¸ê®Æ (local) ªº Head µ²ºc¸ê°T¦L¥X
//---------------------------------------------------------------------------
void THGem::ShowLocalHeadInfo()
{
    AnsiString S;
    if(GemCheckBoxShowHeadInformation->Checked==true)
    {
        StringOut("");
        S.sprintf("HSMS_Head.Length=%d",LocalLength);
        StringOut(S);

        S.sprintf("HSMS_Head.DeviceID=%d",Local.DeviceID);
        StringOut(S);

        S.sprintf("HSMS_MessageID=S%d F%d",Local.MessageID_S&0x7f,Local.MessageID_F);
        StringOut(S);
        S.sprintf("WBit=%d",Local.W_Bit);
        StringOut(S);

        S.sprintf("PType=%d",Local.PType);
        StringOut(S);

        S.sprintf("SType=%d",Local.SType);
        StringOut(S);

        S.sprintf("Remote.SystemByte=%d",Local.SystemByte);
        StringOut(S);
        StringOut("");
    }
}
//---------------------------------------------------------------------------
//  V 1.0
//  ¨Ì¾Ú S,F code show ¥X¦¹ Message ªºÂ²­n»¡©ú
//  Find Description of Stream and show out
//---------------------------------------------------------------------------
AnsiString THGem::ShowSFDescription(unsigned char SCode, unsigned char FCode)   //Steven 20180815 : °w¹ïS6F12³s¦¬¨â¦¸ªº­×¥¿ ¦^¶ÇSFCode
{
    int i=0;
    int arrayCount=sizeof(SFCodeAndMean)/sizeof(AnsiString);
    AnsiString SFCode;
    SFCode.sprintf("S%dF%d", SCode, FCode);
    while(1)
    {
        if(SFCodeAndMean[i].Pos(SFCode)==2)
        {
            StringOut(SFCodeAndMean[i]);
            return SFCode;
        }
        else if(SFCodeAndMean[i]=="")
        {
            StringOut(SFCodeAndMean[i]);
            return SFCode;
        }
        i++;
        if(i>=arrayCount)
        {
            StringOut("");
            return SFCode;
        }
    }
}
//---------------------------------------------------------------------------
// V 1.0
// ±N¦¬¨ì data stream Âà¦¨ Type ,Len ,Data array ¤À§O©ñ¤J TString ¤º
// ¤è«Kµ¥µ¥¥Î ItemIn ¨ÓÅª¨ú
//---------------------------------------------------------------------------
void __fastcall THGem::StoreToReceiveString(AnsiString S)
{
    if(bReceiveData==false)                                                     // ­Y¸ê®Æ¬O¶Ç°e«D±µ¦¬«h¤£»Ý­n©ñ¤J TString ¤º,¦]¬°¤£»Ý­nÅª¥X
        return;
    SReceiveData->Add(S);
}
//------------------------------------------------------------------------------
// V 1.0
// ±N Data stream ¥H SML ®æ¦¡ show ¥X¨Ó ,¨Ã¥B¦s¨ì TStringList  ¨Ñ­Y¬O Host data µ¹ ItemIn ¨ÓÅª¨ú
//------------------------------------------------------------------------------
int __fastcall THGem::ProcessSML(unsigned char *Ptr,int Len,int &RunLength)
{
    int j, k, ItemSize, TypeSize, ret;

    AnsiString str;
    unsigned char TypeChar;
    unsigned int ct, i;
    AnsiString S;
    unsigned char c;
    bool isListType=false;
    MakeSMLSpaceString();
    if(RunLength>=Len)
        return -1;

    TypeChar=Ptr[RunLength];
    c=TypeChar&0xfc;                                                            // get Format Code (Bit 876543)
    if(c!=HType.LIST_TYPE)
    {
        ct=1;
    }
    else
    {
        StoreToReceiveString(HType.LIST_TYPE);
        RunLength++;

        ct=GetSMLLenthByte(TypeChar, Ptr, RunLength);
        RunLength+=TypeChar&0x03;

        str.sprintf("<L[%d]", ct);
        ShowSMLSpaceString(str);
        StoreToReceiveString(ct);

        iSMLSpace++;
        MakeSMLSpaceString();
        isListType=true;
    }

    for(i=0; i<ct; i++)
    {
        if(RunLength>=Len)
        {
            if(isListType==true)
            {
                iSMLSpace--;
                MakeSMLSpaceString();
                ShowSMLSpaceString(">");
            }
            return -2;
        }
        TypeChar=Ptr[RunLength];
        c=Ptr[RunLength]&0xfc;
        RunLength++;
        if(c==HType.LIST_TYPE)
        {
            RunLength--;
            ret=ProcessSML(Ptr, Len, RunLength);
            if(ret<0)
                return ret;
            else
                continue;
        }
        ItemSize=GetSMLLenthByte(TypeChar, Ptr, RunLength);
        RunLength+=TypeChar&0x03;
        if(c==HType.ASCII_TYPE)
        {
            StoreToReceiveString(c);
            StoreToReceiveString(ItemSize);
            S.sprintf("<A[%d] %c", ItemSize, '"');
            char *Temp;
            Temp=new char [ItemSize+100];
            for(j=0; j<ItemSize; j++)
                Temp[j]=Ptr[RunLength+j];
            RunLength+=ItemSize;
            Temp[ItemSize]=0;
            StoreToReceiveString(Temp);
            Temp[ItemSize]='"';
            Temp[ItemSize+1]=0;
            S+=AnsiString(Temp);
            S+=AnsiString(">");
            ShowSMLSpaceString(S);
            delete []Temp;
            Temp=NULL;
        }
        else if(c==HType.BINARY_TYPE)
        {
            StoreToReceiveString(c);
            StoreToReceiveString(ItemSize);
            S.sprintf("<B[%d]",ItemSize);
            char Temp[16];

            if(bDisableBinaryShow)
            {
                DownLoadFilePtr=&Ptr[RunLength];
                RunLength+=ItemSize;
            }
            else
            {
                if(ItemSize>10)
                {
                    ShowSMLSpaceString(S);
                    S="";
                }
                for(j=0; j<ItemSize; j++)
                {
                    if(RunLength>=Len)
                        return -2;

                    sprintf(Temp," 0x%02X",(unsigned char)Ptr[RunLength]);
                    StoreToReceiveString((unsigned)(Ptr[RunLength]));
                    RunLength++;
                    S+=AnsiString(Temp);
                    if((j%10)==9)
                    {
                        ShowSMLSpaceString(S);
                        S="";
                    }
                }
            }
            S+=">";
            ShowSMLSpaceString(S);
        }
        else if(c==HType.BOOLEAN_TYPE)
        {
            StoreToReceiveString(c);
            StoreToReceiveString(ItemSize);
            S.sprintf("<Boolean[%d]", ItemSize);
            char Temp[16];

            for(j=0; j<ItemSize; j++)
            {
                if(RunLength>=Len)
                   return -2;

                if(Ptr[RunLength]==1 || Ptr[RunLength]==0xFF)
                {
                    sprintf(Temp," 0x%02X",1);
                    StoreToReceiveString(1);
                }
                else
                {
                    sprintf(Temp," 0x%02X",0);
                    StoreToReceiveString(0);
                }
                RunLength++;
                S+=AnsiString(Temp);
            }
            S+=">";
            ShowSMLSpaceString(S);
        }
        else if(c==HType.UINT_1_TYPE ||
                c==HType.UINT_2_TYPE ||
                c==HType.UINT_4_TYPE)
        {
            StoreToReceiveString(c);
            if(c==HType.UINT_1_TYPE)   TypeSize=1;
            if(c==HType.UINT_2_TYPE)   TypeSize=2;
            if(c==HType.UINT_4_TYPE)   TypeSize=4;

            StoreToReceiveString((ItemSize/TypeSize));

            S.sprintf("<U%d[%d] ", TypeSize, (ItemSize/TypeSize));
            unsigned Temp;
            for(j=0; j<(ChangeToFloatNonPcnt((double)(ItemSize), (double)(TypeSize))); j++)
            {
                Temp=0;
                for(k=0; k<TypeSize; k++)
                {
                    if(RunLength>=Len)
                        return -2;

                    Temp<<=8;
                    Temp+=Ptr[RunLength];
                    RunLength++;
                }
                StoreToReceiveString(Temp);
                S+=AnsiString(Temp);
                if((j+1)<(ChangeToFloatNonPcnt((double)(ItemSize), (double)(TypeSize))))
                    S+=AnsiString(" ");
            }
            S+=AnsiString(">");
            ShowSMLSpaceString(S);
        }
        else if(c==HType.UINT_8_TYPE)
        {
            StoreToReceiveString(c);
            TypeSize=8;

            S.sprintf("<U%d[%d] ", TypeSize, (ItemSize/TypeSize));
            StoreToReceiveString(ItemSize/TypeSize);
            unsigned __int64 Temp;
            for(j=0; j<(ItemSize/TypeSize); j++)
            {
                Temp=0;
                for(k=0; k<TypeSize; k++)
                {
                    if(RunLength>=Len)
                        return -2;

                    Temp<<=8;
                    Temp+=Ptr[RunLength];
                    RunLength++;
                }
                StoreToReceiveString(Temp);
                S+=AnsiString(Temp);
                if((j+1)<(ChangeToFloatNonPcnt((double)(ItemSize), (double)(TypeSize))))
                    S+=AnsiString(" ");
            }
            S+=AnsiString(">");
            ShowSMLSpaceString(S);
        }
        else if(c==HType.INT_1_TYPE)    //ok
        {
            char temp;
            unsigned char *p;
            StoreToReceiveString(c);
            TypeSize=1;

            StoreToReceiveString((ItemSize/TypeSize));

            S.sprintf("<I%d[%d] ", TypeSize, (ItemSize/TypeSize));
            for(j=0; j<(ItemSize/TypeSize); j++)
            {
                if(RunLength>=Len)
                    return -2;

                temp=0;
                p=(unsigned char *)&temp;
                for(k=0; k<TypeSize; k++)
                {
                    if((RunLength+TypeSize-1-k)>=Len)
                        return -2;

                    p[k]=Ptr[RunLength+TypeSize-1-k];
                }
                RunLength+=TypeSize;
                S+=AnsiString(short (temp));
                StoreToReceiveString(AnsiString(short (temp)));
                if((j+1)<(ChangeToFloatNonPcnt((double)(ItemSize), (double)(TypeSize))))
                    S+=AnsiString(" ");
            }
            S+=AnsiString(">");
            ShowSMLSpaceString(S);
        }
        else if(c==HType.INT_2_TYPE)   // ok
        {
            short temp;
            unsigned char *p;
            StoreToReceiveString(c);
            TypeSize=2;

            StoreToReceiveString((ItemSize/TypeSize));

            S.sprintf("<I%d[%d] ", TypeSize, (ItemSize/TypeSize));
            for(j=0; j<(ItemSize/TypeSize); j++)
            {
                temp=0;
                if(RunLength>=Len)
                    return -2;

                p=(unsigned char *)&temp;
                for(k=0; k<TypeSize; k++)
                {
                    if((RunLength+TypeSize-1-k)>=Len)
                        return -2;
                    p[k]=Ptr[RunLength+TypeSize-1-k];
                }
                RunLength+=TypeSize;
                S+=AnsiString(temp);
                StoreToReceiveString(temp);
                if((j+1)<(ChangeToFloatNonPcnt((double)(ItemSize), (double)(TypeSize))))
                    S+=AnsiString(" ");
            }
            S+=AnsiString(">");
            ShowSMLSpaceString(S);
        }
        else if(c==HType.INT_4_TYPE) // ok
        {
            int temp;
            unsigned char *p;
            StoreToReceiveString(c);
            TypeSize=4;

            StoreToReceiveString((ItemSize/TypeSize));

            S.sprintf("<I%d[%d] ", TypeSize, (ItemSize/TypeSize));
            for(j=0; j<(ItemSize/TypeSize); j++)
            {
                temp=0;
                if(RunLength>=Len)
                    return -2;

                p=(unsigned char *)&temp;
                for(k=0; k<TypeSize; k++)
                {
                    if((RunLength+TypeSize-1-k)>=Len)
                        return -2;
                    p[k]=Ptr[RunLength+TypeSize-1-k];
                }
                RunLength+=TypeSize;
                S+=AnsiString(temp);
                StoreToReceiveString(temp);
                if((j+1)<(ChangeToFloatNonPcnt((double)(ItemSize), (double)(TypeSize))))
                    S+=AnsiString(" ");
            }
            S+=AnsiString(">");
            ShowSMLSpaceString(S);
        }
        else if(c==HType.INT_8_TYPE) // ?
        {
            __int64 temp;
            unsigned char *p;
            StoreToReceiveString(c);
            TypeSize=8;

            StoreToReceiveString((ItemSize/TypeSize));

            S.sprintf("<I%d[%d] ", TypeSize, (ItemSize/TypeSize));
            for(j=0; j<(ItemSize/TypeSize); j++)
            {
                temp=0;
                if(RunLength>=Len)
                    return -2;

                p=(unsigned char *)&temp;
                for(k=0; k<TypeSize; k++)
                {
                    if((RunLength+TypeSize-1-k)>=Len)
                        return -2;
                    p[k]=Ptr[RunLength+TypeSize-1-k];
                }
                RunLength+=TypeSize;
                S+=AnsiString(temp);
                StoreToReceiveString(temp);
                if((j+1)<(ChangeToFloatNonPcnt((double)(ItemSize), (double)(TypeSize))))
                    S+=AnsiString(" ");
            }
            S+=AnsiString(">");
            ShowSMLSpaceString(S);
        }
        else if(c==HType.FT_4_TYPE)     // how to avoid 0.7==> 0.699999999
        {
            float temp;
            unsigned char *p;
            StoreToReceiveString(c);
            TypeSize=4;

            StoreToReceiveString((ItemSize/TypeSize));

            S.sprintf("<F%d[%d] ", TypeSize, (ItemSize/TypeSize));
            for(j=0; j<(ItemSize/TypeSize); j++)
            {
                temp=0;
                if(RunLength>=Len)
                    return -2;

                p=(unsigned char *)&temp;
                for(k=0; k<TypeSize; k++)
                {
                    if((RunLength+TypeSize-1-k)>=Len)
                        return -2;
                    p[k]=Ptr[RunLength+TypeSize-1-k];
                }
                RunLength+=TypeSize;
                S+=AnsiString(temp);
                StoreToReceiveString(temp);
                if((j+1)<(ChangeToFloatNonPcnt((double)(ItemSize), (double)(TypeSize))))
                    S+=AnsiString(" ");
            }
            S+=AnsiString(">");
            ShowSMLSpaceString(S);
        }
        else if(c==HType.FT_8_TYPE)     //ok
        {
            double temp;
            unsigned char *p;
            StoreToReceiveString(c);
            TypeSize=8;

            StoreToReceiveString((ItemSize/TypeSize));

            S.sprintf("<F%d[%d] ", TypeSize, (ItemSize/TypeSize));
            for(j=0; j<(ItemSize/TypeSize); j++)
            {
                temp=0;
                if(RunLength>=Len)
                    return -2;

                p=(unsigned char *)&temp;
                for(k=0; k<TypeSize; k++)
                {
                    p[k]=Ptr[RunLength+TypeSize-1-k];
                    if((RunLength+TypeSize-1-k)>=Len)
                        return -2;
                }
                RunLength+=TypeSize;
                S+=AnsiString(temp);
                StoreToReceiveString(temp);
                if((j+1)<(ChangeToFloatNonPcnt((double)(ItemSize), (double)(TypeSize))))
                    S+=AnsiString(" ");
            }
            S+=AnsiString(">");
            ShowSMLSpaceString(S);
        }
    }
    if(isListType==true)
    {
        iSMLSpace--;
        MakeSMLSpaceString();
        ShowSMLSpaceString(">");
    }
    return 0;
}
//---------------------------------------------------------------------------
// V1.0
// ±N ¦¬°e¸ê®Æ ¥H SML ®æ¦¡¦L¥X
//---------------------------------------------------------------------------
void __fastcall THGem::ShowSML(unsigned char *Ptr, int len)
{
    bOutputBusy=true;
    int RunLength=14;
    iSMLSpace=0;
    DB->Clear();                                                                //pig 20170802 (Steven) Åã¥Ü·sªº¸ê®Æ¤§«e´N²M°£­ì¥»¸ê®Æ¡A«O¯d§¹¾ã°T®§Åã¥Ü
    ProcessSML(Ptr, len, RunLength);
    if(len!=RunLength)
        ProcessSML(Ptr, len, RunLength);

    ShowSMLSpaceString(".");
    StringOut("");
    bOutputBusy=false;
}
//---------------------------------------------------------------------------
// V1.0
// online ®É spool function enable ³B²z
//---------------------------------------------------------------------------
void THGem::WriteToSpoolFile()
{
    FILE *P;
    AnsiString str;
    unsigned char Ptr[1024];
    unsigned int i;
    GetTimeInfo();
//    WORD OldYear, OldMonth, OldDate, OldHour, OldMin, OldSec;
    bool ErrorFlag;
    int iTryCT=0;
    bool bUpdateSpoolStartTime=false;
/*       //¼È®É¨S¥Î¨ìMark
    if(bSpoolActive==false)
    {
        StringOut("Host not connect ,data will be purge !!! ");
        return;
    }
    if(OldYear  ==SystemYear   &&
       OldMonth ==SystemMonth  &&
       OldDate  ==SystemDate   &&
       OldHour  ==SystemHour   &&
       OldMin   ==SystemMin    &&
       OldSec   ==SystemSec)
    {
        ctSpoolFile++;

    }
    else
    {
        ctSpoolFile=0;
        OldYear    = SystemYear  ;
        OldMonth   = SystemMonth ;
        OldDate    = SystemDate  ;
        OldHour    = SystemHour  ;
        OldMin     = SystemMin   ;
        OldSec     = SystemSec   ;

    }
*/
    str.sprintf("%s%04d_%02d_%02d %02d_%02d_%02d %03d.dat", IncludeTrailingPathDelimiter(GemSpoolPath), SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, ctSpoolFile);
    do
    {
        if(FileListBox1->Items->Count==0)
        {
            bUpdateSpoolStartTime=true;
            sprintf(GemSpoolStartTime, "%04d%02d%02d%02d%02d%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
        }
        ErrorFlag=false;
        P=fopen(str.c_str(), "wb");
        if(P!=NULL)
        {
            fwrite(LocalBuffer, LocalLength_4, 1, P);
            fclose(P);
            P=fopen(str.c_str(), "rb");
            fread(Ptr, LocalLength_4, 1, P);
            fclose(P);
            if(bUpdateSpoolStartTime==true)
            {
                FileListBox1->Refresh();
                FileListBox1->Update();
            }
            for(i=0; i<LocalLength_4; i++)
            {
                if(LocalBuffer[i]!=Ptr[i])
                {
                    ErrorFlag=true;
                    break;
                }
            }
            if(ErrorFlag==true)
            {
                DeleteFile(str.c_str());
                iTryCT++;
                if(iTryCT>20)
                {
                    ShowMessage("Err");
                    break;
                }
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }

    }while(1);
    StringOut("Host not connect ,write to spool buffer !!! ");
}
//---------------------------------------------------------------------------
// V1.0
// 1.±N Data ³z¹L TCP/IP ¶Ç°e
// 2.show ¥X ³¡¥÷ stream ¸ê°T
// 3.µø»Ý¨D±N Head struct ¦L¥X
// 3.µø»Ý¨D±N Stram data ¥Î Hex ¦L¥X
//---------------------------------------------------------------------------
void THGem::SendLocalData()
{
    AnsiString S, SFCode;
    bReceiveData=false;

    int SCode=Local.MessageID_S&0x7f;
    int FCode=Local.MessageID_F;
    StringOutColor=clBlack;

    try                                                                         //20130204 Daver add
    {
        if(bUseClientSocket==true)
        {
            clientGem->Socket->SendBuf(LocalBuffer, LocalLength_4);
        }
        else
        {
            if(srvGem->Active==true && bServoSocketConnect==true)
            {
                if(srvGem->Socket->ActiveConnections>0)
                {
                    if(srvGem->Socket->Connections[0]->Connected)
                    {
                        if(srvGem->Socket->ActiveConnections==1)
                        {
                            srvGem->Socket->Connections[0]->SendBuf(LocalBuffer, LocalLength_4);
                        }
                    }
                }
                else
                {
                    bServoSocketConnect=false;
                    srvGem->Close();
                    srvGem->Open();
                }
            }
            else
            {
                return;
            }
        }
    }
    catch(...)                                                                  //Daver 20130308 add
    {
        StringOut("--------------- Exception Start------------------------------------");
        StringOut("clientGem->Socket->SendBuf Fail!!");
        GetTimeInfo();
        S=AnsiString("[Send]    Send Fail!");
        StringOut(S);
        SFCode=ShowSFDescription(LocalBuffer[6]&0x7f, LocalBuffer[7]);
        MyDBIProcess("Exception", "THGem::SendLocalData - clientGem->Socket->SendBuf Fail!!", SFCode);
        ShowSML(LocalBuffer, LocalLength_4);
        bServoSocketConnect=false;
        srvGem->Close();
        srvGem->Open();
        bTCPIP_Error=true;
        StringOut("--------------- Exception End------------------------------------");
        return;
    }

    try
    {
        StringOut("---------------------------------------------------");
        GetTimeInfo();
        S=AnsiString("[Send]    ")+TimeString;
        StringOut(S);

        SFCode=ShowSFDescription(LocalBuffer[6]&0x7f, LocalBuffer[7]);
        ShowLocalBufferBinaryData();
        ShowLocalHeadInfo();
        ShowSML(LocalBuffer, LocalLength_4);
        if((FCode%2)==1 && Local.W_Bit==1)
        {
            S=AnsiString(SCode)+" "+AnsiString(FCode+1)+" "+AnsiString(Local.SystemByte);
            csSFCodeResponse->Acquire();                                        // Joseph 20221111 (Jason) ·s¼W TCriticalSection S //
            try
            {
                SFCodeResponseList->Items->Add(S);
                TimeLeft->Add(T3TimeOut*10);                                    //pig 2014.07.28 KYEC_SECS
            }
            __finally
            {
                csSFCodeResponse->Release();
            }
        }
    }
    catch(...)                                                                  //Daver 20130308 add
    {
        StringOut("--------------- Exception Start------------------------------------");
        GetTimeInfo();
        S=AnsiString("[Send]    Send Fail!");
        StringOut(S);
        SFCode=ShowSFDescription(LocalBuffer[6]&0x7f, LocalBuffer[7]);
        MyDBIProcess("Exception", "THGem::SendLocalData", SFCode);
        ShowSML(LocalBuffer, LocalLength_4);
        bServoSocketConnect=false;
        srvGem->Close();
        srvGem->Open();
        bTCPIP_Error=true;
        StringOut("--------------- Exception End------------------------------------");
        return;
    }
    //==========================================================================
}
//---------------------------------------------------------------------------
// V1.0
// ³]©w³q°T°T®§Åã¥Üµøµ¡
//---------------------------------------------------------------------------
void THGem::SetDisplayPtr(TMemo *DispBox)
{
    DB=DispBox;
}
//---------------------------------------------------------------------------
// V1.0
//---------------------------------------------------------------------------
void __fastcall THGem::clientGemConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    StringOut("Connect");
}
//---------------------------------------------------------------------------
// V1.0
//---------------------------------------------------------------------------
void __fastcall THGem::clientGemDisconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    GetTimeInfo();
    StringOut("disconnect  "+TimeString);                                       //JerryYang 20190411 Â_½u®É­n°O¿ý®É¶¡
    if(bConnect==true)
        bAutoConnect=true;
    bConnect=false;
}
//---------------------------------------------------------------------------
// V1.0
//---------------------------------------------------------------------------
void __fastcall THGem::clientGemError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    clientGem->Active=false;
    ErrorCode=0;
    bTCPIP_Error=true;
    /*try
    {
        clientGem->Close();
    }
    catch(...)
    {
        LogClientSocketExceptionError(clientGem, "clientGem Error");
    }*/
}
//---------------------------------------------------------------------------
// V1.0
//---------------------------------------------------------------------------
void __fastcall THGem::clientGemConnecting(TObject *Sender,
      TCustomWinSocket *Socket)
{
    StringOut("connecting");
}
//---------------------------------------------------------------------------
// V 1.0
// ProcessBuffer : binary data buffer
// len           : len byte ªº¸ê®Æ
// Value         : Value ¬°Àò±oª¾¤§¸ê®Æ
// RunLen        : ³B²zlen byte «áÁÙ³Ñ¦h¤Öbyte ­n³B²z
//---------------------------------------------------------------------------
unsigned char *THGem::HostConvertToData(unsigned char *ProcessBuffer, int len, void *Value)
{
    unsigned char *p;
    p=(unsigned char *)Value;
    for(int i=0; i<len; i++)
        p[len-1-i]=ProcessBuffer[i];
    ProcessBuffer+=len;
    Host_HSMS_RunLength-=len;
    return ProcessBuffer;
}
//---------------------------------------------------------------------------
// V 1.0
// ¨ú±o Remote (Host) ªº Head µ²ºc ¨Ã¥B show ¥X¨Ó
//---------------------------------------------------------------------------
void THGem::ProcessRemoteHead(unsigned char *ProcessBuffer)
{
    AnsiString S;
    ProcessBuffer=HostConvertToData(ProcessBuffer, 4, &RemoteLength);
    Host_HSMS_RunLength=RemoteLength;
    ProcessBuffer=HostConvertToData(ProcessBuffer, 2, &Remote.DeviceID);
    ProcessBuffer=HostConvertToData(ProcessBuffer, 1, &Remote.MessageID_S);
    if(Remote.MessageID_S &0x80)
        Remote.W_Bit=1;
    else
        Remote.W_Bit=0;
    Remote.MessageID_S=Remote.MessageID_S&0x7f;
    ProcessBuffer=HostConvertToData(ProcessBuffer, 1, &Remote.MessageID_F);
    ProcessBuffer=HostConvertToData(ProcessBuffer, 1, &Remote.PType);
    ProcessBuffer=HostConvertToData(ProcessBuffer, 1, &Remote.SType);
    ProcessBuffer=HostConvertToData(ProcessBuffer, 4, &Remote.SystemByte);
    if(GemCheckBoxShowHeadInformation->Checked==true)
    {
        StringOut("");
        S.sprintf("HSMS_Head.Length=%d", RemoteLength);
        StringOut(S);
        S.sprintf("HSMS_Head.DeviceID=%d", Remote.DeviceID);
        StringOut(S);

        S.sprintf("HSMS_MessageID=S%d F%d", Remote.MessageID_S&0x7f, Remote.MessageID_F);
        StringOut(S);
        S.sprintf("WBit=%d", Remote.W_Bit);
        StringOut(S);

        S.sprintf("PType=%d", Remote.PType);
        StringOut(S);
        S.sprintf("SType=%d", Remote.SType);
        StringOut(S);
        S.sprintf("Remote.SystemByte=%d", Remote.SystemByte);
        StringOut(S);
        StringOut("");
    };
}
//---------------------------------------------------------------------------
// V 1.0
//  command=0     : Acknowledge
//  command=1     : Denied
//---------------------------------------------------------------------------
void THGem::LocalAcknowledge(unsigned char SCode, unsigned char FCode , unsigned char Command)
{
    unsigned char C;
    C=Command;
    InitLocalHead(SCode, FCode, 0);
    DataItemOut(1, HType.BINARY_TYPE, &C);
    SendLocalData();
}
//---------------------------------------------------------------------------
// V 1.0
//
// Åª¨ú®æ¦¡¬° Type ªº¸ê®Æ¨ì P ¤º
// ªø«×¬° len ­Y®æ¦¡©Îªø«×¤£¥¿½T«h
//
// ¦pªG¸ê®Æ«¬ºA©Î¤º®e¤£¥¿½T    return -1
// ¦pªG¸ê®Æ lenth ¤£¥¿½T       return -2
// ¥¿½T                        return 1
//---------------------------------------------------------------------------
int THGem::DataItemInSub(int len, unsigned char Type, void *P)
{
    unsigned char t;
    int l, i;

    if(SReceiveData->Count==0)
        return -1;
    t=atoi(SReceiveData->Strings[0].c_str());                                   // Type

    if(t!=Type)
        return -1;
    SReceiveData->Delete(0);
    if(SReceiveData->Count==0)
        return -1;
    l=atoi(SReceiveData->Strings[0].c_str());                                   // length of data

    if(t==HType.ASCII_TYPE)                                                     // ­Y¬O ASCII_TYPE ­n¨¾¤îbuffer ¤£¨¬
    {
        if(l>len)                                                               // +1 ¬O¦]¬° ³Ì«ábyte­n¥[¤W¤@­Ó 0 §@¬°¦r¦êµ²§ô
            return -2;
    }
    else
    {
        if(len!=l)
            return -2;
    }
    SReceiveData->Delete(0);                                                    // ¨ì¦¹ªø«×¹ï¤F

    if(t==HType.LIST_TYPE)
    {
        return 1;
    }
    else if(t==HType.ASCII_TYPE)
    {
        char *temp;
        temp=(char *)P;
        if(SReceiveData->Count==0)
            return -1;

        strncpy(temp, SReceiveData->Strings[0].c_str(), len+1);                 //JerryYang 20230112 : len->len+1, ­×¥¿¤Ö¤Fµ²§ô¦r¤¸ªº°ÝÃD                   // 20221111 Joseph , for ASCII mode use //
        SReceiveData->Delete(0);
    }
    else if(t==HType.BINARY_TYPE)
    {
        unsigned char *temp;
        temp=(unsigned char *)P;

        for(i=0; i<len; i++)
        {
            if(SReceiveData->Count!=0)                                        // 20221111 Joseph , Add Safe SReceiveData->Delete() //
            {
                temp[i]=atoi(SReceiveData->Strings[0].c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if(t==HType.BOOLEAN_TYPE)
    {
        bool *temp;
        temp=(bool *)P;
        for(i=0; i<len; i++)
        {
            if(SReceiveData->Count!=0)                                          // 20221111 Joseph , Add Safe SReceiveData->Delete() //
            {
                if(atoi(SReceiveData->Strings[0].c_str())==1)
                    temp[i]=true;
                else
                    temp[i]=false;
                SReceiveData->Delete(0);
            }
        }
    }
    else if(t==HType.INT_1_TYPE)
    {
        char  *temp;
        temp=(char *)P;
        for(i=0; i<len; i++)
        {
            if(SReceiveData->Count!=0)                                          // 20221111 Joseph , Add Safe SReceiveData->Delete() //
            {
                temp[i]=atoi(SReceiveData->Strings[0].c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if(t==HType.INT_2_TYPE)
    {
        short  *temp;
        temp=(short *)P;
        for(i=0; i<len; i++)
        {
            if(SReceiveData->Count!=0)                                          // 20221111 Joseph , Add Safe SReceiveData->Delete() //
            {
                temp[i]=atoi(SReceiveData->Strings[0].c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if(t==HType.INT_4_TYPE)
    {
        int  *temp;
        temp=(int *)P;
        for(i=0; i<len; i++)
        {
            if(SReceiveData->Count!=0)                                          // 20221111 Joseph , Add Safe SReceiveData->Delete() //
            {
                temp[i]=atoi(SReceiveData->Strings[0].c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if(t==HType.INT_8_TYPE)
    {
        __int64  *temp;
        temp=(__int64 *)P;
        for(i=0; i<len; i++)
        {
            if(SReceiveData->Count!=0)                                          // 20221111 Joseph , Add Safe SReceiveData->Delete() //
            {
                temp[i]=atoi(SReceiveData->Strings[0].c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if(t==HType.UINT_1_TYPE)
    {
        unsigned char *temp;
        temp=(unsigned char *)P;
        for(i=0; i<len; i++)
        {
            if(SReceiveData->Count!=0)                                          // 20221111 Joseph , Add Safe SReceiveData->Delete() //
            {
                temp[i]=atoi(SReceiveData->Strings[0].c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if(t==HType.UINT_2_TYPE)
    {
        unsigned short *temp;
        temp=(unsigned short *)P;
        for(i=0; i<len; i++)
        {
            if(SReceiveData->Count!=0)                                          // 20221111 Joseph , Add Safe SReceiveData->Delete() //
            {
                temp[i]=atoi(SReceiveData->Strings[0].c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if(t==HType.UINT_4_TYPE)
    {
        unsigned *temp;
        temp=(unsigned *)P;
        for(i=0; i<len; i++)
        {
            if(SReceiveData->Count!=0)                                          // 20221111 Joseph , Add Safe SReceiveData->Delete() //
            {
                temp[i]=atoi(SReceiveData->Strings[0].c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if(t==HType.UINT_8_TYPE)
    {
        unsigned __int64 *temp;
        temp=(unsigned __int64 *)P;
        for(i=0; i<len; i++)
        {
            if(SReceiveData->Count!=0)                                          // 20221111 Joseph , Add Safe SReceiveData->Delete() //
            {
                temp[i]=atoi(SReceiveData->Strings[0].c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if(t==HType.FT_4_TYPE)
    {
        float *temp;
        temp=(float*)P;
        for(i=0; i<len; i++)
        {
            if(SReceiveData->Count!=0)                                          // 20221111 Joseph , Add Safe SReceiveData->Delete() //
            {
                temp[i]=atof(SReceiveData->Strings[0].c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if(t==HType.FT_8_TYPE)
    {
        double *temp;
        temp=(double *)P;
        for(i=0; i<len; i++)
        {
            if(SReceiveData->Count!=0)                                          // 20221111 Joseph , Add Safe SReceiveData->Delete() //
            {
                temp[i]=atof(SReceiveData->Strings[0].c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else
    {
        return -1;
    }
    return 1;
}
//---------------------------------------------------------------------------
// V 1.0
//
// Åª¨ú®æ¦¡»Pªø«×
// ¦pªG¸ê®Æ lenth ¤£¥¿½T       return -2
// ¥¿½T                        return 1
//---------------------------------------------------------------------------
int THGem::GetDataItemLenAndTypeSub(int &len, unsigned char &Type)
{
    if(SReceiveData->Count<2)
        return -2;
    Type=atoi(SReceiveData->Strings[0].c_str()); // Type
    len =atoi(SReceiveData->Strings[1].c_str()); // length of data
    return 1;
}
//---------------------------------------------------------------------------
// V 1.0
//
// Åª¨ú®æ¦¡»Pªø«×¤§«á¨Ã±N¸ê®Æ Buffer delete
//---------------------------------------------------------------------------
int THGem::GetDataItemLenAndTypeAndDeleteSub(int &len, unsigned char &Type)
{
    if(SReceiveData->Count<2)
        return -2;
    Type=atoi(SReceiveData->Strings[0].c_str()); // Type
    len=atoi(SReceiveData->Strings[1].c_str()); // length of data
    SReceiveData->Delete(0);
    SReceiveData->Delete(0);
    return 1;
}
//=============================================================================
// =                                                                           =
// =                       S,F code process                                    =
// =                                                                           =
//=============================================================================
//---------------------------------------------------------------------------
//  V 1.0
//  SV data output
//---------------------------------------------------------------------------
bool THGem::DataItemOutSV(AnsiString SVID)
{
    int i, Len;
    unsigned char Type;
    AnsiString SVName;
    AnsiString SVUnit;
    void *P;
    TObject *VclP;
    AnsiString S, IsVCL, VCLStr, *SS;
//    int VCLIndex(-1);                                                           // 2013.09.17 , Joye , KYEC SECS/GEM

    char  VCL_ASCII[10000];
    char  VCL_BINARY;
    char  VCL_INT1;
    short VCL_INT2;
    int   VCL_INT4;
    __int64  VCL_INT8;                                                          //Steven 20140911 : ­×¥¿INT_8_TYPE & UINT_8_TYPE
    unsigned char  VCL_UINT1,VCL_BOOLEAN;
    unsigned short VCL_UINT2;
    unsigned int   VCL_UINT4;
    unsigned __int64  VCL_UINT8;                                                //Steven 20140911 : ­×¥¿INT_8_TYPE & UINT_8_TYPE

    // lee new start
    float VCL_FT4;
    double VCL_FT8;
    bool ret=true;
    // lee new end

    i=SV_ID->IndexOf(SVID);                                                     //Steven 20150910 : For°j°é§ï¦¨ IndexOf
    if(i>=0)
    {
        Type    =atoi(SV_TYPE->Strings[i].c_str());
        SVName  =SV_NAME->Strings[i];
        SVUnit  =SV_UNIT->Strings[i];
        IsVCL   =VCL_NAME->Strings[i];
        P       =SV_Ptr->Items[i];
        Len     =atoi(SV_LEN->Strings[i].c_str());
//        VCLIndex=-1;                                                            // 2013.09.17 , Joye , KYEC SECS/GEM

        if(IsVCL==1 || IsVCL==2)
        {
            if(IsVCL==1)
            {
                VclP=(TObject *)SV_Ptr->Items[i];

                TPanel      *PanelPtr;
                TCustomEdit *EditPtr;
                TComboBox   *ComboBoxPtr;
                TLabel      *LabelPtr;
                TCheckBox   *CheckBoxPtr;                                       // 2013/03/14 Lee
                TRadioGroup *RadioGroupPtr;
                TStringList *StringListPtr;                                     //Steven 20140510

                PanelPtr     =dynamic_cast<TPanel       *>(VclP);
                EditPtr      =dynamic_cast<TCustomEdit  *>(VclP);
                ComboBoxPtr  =dynamic_cast<TComboBox    *>(VclP);
                LabelPtr     =dynamic_cast<TLabel       *>(VclP);
                CheckBoxPtr  =dynamic_cast<TCheckBox    *>(VclP);               // 2013/03/14 Lee
                RadioGroupPtr=dynamic_cast<TRadioGroup  *>(VclP);               // 2013/03/14 Lee
                StringListPtr=dynamic_cast<TStringList  *>(VclP);               //Steven 20140510

                if(StringListPtr!=NULL)                                         //Steven 20140510
                {
                    VCLStr=StringListPtr->CommaText;
                }
                else if(PanelPtr!=NULL)
                {
                    VCLStr=PanelPtr->Caption;
                }
                else if(EditPtr!=NULL)
                {
                    VCLStr=EditPtr->Text;
                }
                else if(ComboBoxPtr!=NULL)
                {
                    if(Type==HType.ASCII_TYPE)                                  //Steven 20240802 : ®Ú¾ÚType¦^¶Ç¸ê®Æ
                        VCLStr=ComboBoxPtr->Text;
                    else
                        VCLStr=ComboBoxPtr->ItemIndex;
                }
                else if(RadioGroupPtr!=NULL)
                {
                    if(RadioGroupPtr->ItemIndex>=0)
                    {
                        if(Type==HType.ASCII_TYPE)                              //Steven 20240802 : ®Ú¾ÚType¦^¶Ç¸ê®Æ
                            VCLStr=RadioGroupPtr->Items->Strings[RadioGroupPtr->ItemIndex];
                        else
                            VCLStr=RadioGroupPtr->ItemIndex;
                    }
                    else
                    {
                        if(Type==HType.ASCII_TYPE)                              //Steven 20240802 : ®Ú¾ÚType¦^¶Ç¸ê®Æ
                            VCLStr="";
                        else
                            VCLStr="-1";
                    }
                }
                else if(LabelPtr!=NULL)
                {
                    VCLStr=LabelPtr->Caption;
                }
                else if(CheckBoxPtr!=NULL)                                      // 2013/03/14 Lee start
                {
                    Type=HType.BOOLEAN_TYPE;
                    if(CheckBoxPtr->Checked)
                        VCLStr="1";
                    else
                        VCLStr="0";
                }
                else                                                            // 2013/03/14 Lee end
                {
                    VCLStr="";
                }
            }
            else
            {
                SS=(AnsiString *)SV_Ptr->Items[i];
                VCLStr=*SS;
            }

            if(Type==HType.ASCII_TYPE)
            {
                strcpy(VCL_ASCII, VCLStr.c_str());
                P=VCL_ASCII;
            }
            else if(Type==HType.BINARY_TYPE)
            {
                VCL_BINARY=atoi(VCLStr.c_str());
                P=&VCL_BINARY;
            }
            else if(Type==HType.BOOLEAN_TYPE)
            {
                VCL_BOOLEAN=atoi(VCLStr.c_str());
                P=&VCL_BOOLEAN;
            }
            else if(Type==HType.INT_1_TYPE)
            {
                VCL_INT1=atoi(VCLStr.c_str());
                P=&VCL_INT1;
            }
            else if(Type==HType.INT_2_TYPE)
            {
                VCL_INT2=atoi(VCLStr.c_str());
                P=&VCL_INT2;
            }
            else if(Type==HType.INT_4_TYPE)
            {
                VCL_INT4=atoi(VCLStr.c_str());
                P=&VCL_INT4;
            }
            else if(Type==HType.INT_8_TYPE)                                     //Steven 20140911 : ­×¥¿INT_8_TYPE & UINT_8_TYPE
            {
                VCL_INT8=_atoi64(VCLStr.c_str());
                P=&VCL_INT8;
            }
            else if(Type==HType.UINT_1_TYPE)
            {
                VCL_UINT1=atoi(VCLStr.c_str());
                P=&VCL_UINT1;
            }
            else if(Type==HType.UINT_2_TYPE)
            {
                VCL_UINT2=atoi(VCLStr.c_str());
                P=&VCL_UINT2;
            }
            else if(Type==HType.UINT_4_TYPE)
            {
                VCL_UINT4=atoi(VCLStr.c_str());
                P=&VCL_UINT4;
            }
            else if(Type==HType.UINT_8_TYPE)                                    //Steven 20140911 : ­×¥¿INT_8_TYPE & UINT_8_TYPE
            {
                VCL_UINT8=(unsigned)_atoi64(VCLStr.c_str());
                P=&VCL_UINT8;
            }
            else if(Type==HType.FT_4_TYPE)
            {
                VCL_FT4=atof(VCLStr.c_str());
                P=&VCL_FT4;
            }
            else if(Type==HType.FT_8_TYPE)
            {
                VCL_FT8=atof(VCLStr.c_str());
                P=&VCL_FT8;
            }
        }
        else
        {
            P=SV_Ptr->Items[i];
        }

        if(Type==HType.LIST_TYPE)
        {
            int *Ptr;
            Ptr=(int *)P;
            DataItemOut(Len, HType.LIST_TYPE, NULL);
            for(i=0; i<Len; i++)
            {
                ret=DataItemOutSV(Ptr[i]);
                if(ret==false)
                    return false;
            }
        }

        if(Type==HType.ASCII_TYPE)
        {
            char *Ptr;
            Ptr=(char *)P;
            DataItemOut(strlen(Ptr), Type, Ptr);
        }
        else if(Type==HType.BINARY_TYPE)
        {
            char *Ptr;
            Ptr=(char *)P;
            DataItemOut(Len, Type, Ptr);
        }
        else if(Type==HType.BOOLEAN_TYPE)
        {
            unsigned char *Ptr;
            Ptr=(unsigned char *)P;
            DataItemOut(Len, Type, Ptr);
        }
        else if(Type==HType.INT_1_TYPE)
        {
            char *Ptr;
            Ptr=(char *)P;
            DataItemOut(Len, Type, Ptr);
        }
        else if(Type==HType.INT_2_TYPE)
        {
            short *Ptr;
            Ptr=(short *)P;
            DataItemOut(Len, Type, Ptr);
        }
        else if(Type==HType.INT_4_TYPE)
        {
            int *Ptr;
            Ptr=(int *)P;
            DataItemOut(Len, Type, Ptr);
        }
        else if(Type==HType.INT_8_TYPE)                                         //Steven 20140911 : ­×¥¿INT_8_TYPE & UINT_8_TYPE
        {
            __int64 *Ptr;
            Ptr=(__int64 *)P;
            DataItemOut(Len, Type, Ptr);
        }
        else if(Type==HType.UINT_1_TYPE)
        {
            unsigned char *Ptr;
            Ptr=(unsigned char *)P;
            DataItemOut(Len, Type, Ptr);
        }
        else if(Type==HType.UINT_2_TYPE)
        {
            unsigned short *Ptr;
            Ptr=(unsigned short *)P;
            DataItemOut(Len, Type, Ptr);
        }
        else if(Type==HType.UINT_4_TYPE)
        {
            unsigned int *Ptr;
            Ptr=(unsigned int *)P;
            DataItemOut(Len, Type, Ptr);
        }
        else if(Type==HType.UINT_8_TYPE)                                        //Steven 20140911 : ­×¥¿INT_8_TYPE & UINT_8_TYPE
        {
            unsigned __int64 *Ptr;
            Ptr=(unsigned __int64 *)P;
            DataItemOut(Len, Type, Ptr);
        }
        else if(Type==HType.FT_4_TYPE)
        {
            float *Ptr;
            Ptr=(float *)P;
            DataItemOut(Len, Type, Ptr);
        }
        else if(Type==HType.FT_8_TYPE)
        {
            double *Ptr;
            Ptr=(double *)P;
            DataItemOut(Len, Type, Ptr);
        }
        return true;
    }

    S="SVID:"+SVID+" not define ";
    SendInvalidDataMessageToHost(S);                                            // 2013_09_06 steven
    DB->Lines->Add(S);
    return false;
}
// ---------------------------------------------------------------------------
// V 1.0
// ---------------------------------------------------------------------------
bool THGem::DataItemOutSVNameList(AnsiString SVID)
{
    unsigned int j;
    AnsiString SVName;
    AnsiString SVUnit;
    AnsiString S;

    int iSV=SV_ID->IndexOf(SVID);                                               //Steven 20150910 : For°j°é§ï¦¨ IndexOf
    if(iSV>=0)
    {
        SVName=SV_NAME->Strings[iSV];
        SVUnit=SV_UNIT->Strings[iSV];

        DataItemOut(3, HType.LIST_TYPE, NULL);

        j=atoi(SVID.c_str());
        S=j;

        if(SVID=="0" || S==SVID)
        {
            DataItemOut(1, HType.UINT_4_TYPE, &j);
        }
        else
        {
            DataItemOut(HType.ASCII_TYPE, SVID);
        }

        DataItemOut(HType.ASCII_TYPE, SVName);
        DataItemOut(HType.ASCII_TYPE, SVUnit);
        return true;
    }

    S="SVID:"+SVID+" not define ";
    SendInvalidDataMessageToHost(S);                                            //2013_09_06  steven
    DB->Lines->Add(S);
    return false;
}
// ---------------------------------------------------------------------------
bool THGem::DataItemOutSVNameListWithValue(AnsiString SVID)                     //Steven 20140911 : Add S103F11
{
    unsigned int j;
    AnsiString SVName;
    AnsiString SVUnit;
    AnsiString S;

    int iSV=SV_ID->IndexOf(SVID);                                               //Steven 20150910 : For°j°é§ï¦¨ IndexOf
    if(iSV>=0)
    {
        SVName=SV_NAME->Strings[iSV];
        SVUnit=SV_UNIT->Strings[iSV];

        DataItemOut(4, HType.LIST_TYPE, NULL);

        j=atoi(SVID.c_str());
        S=j;

        if(SVID=="0" || S==SVID)
        {
            DataItemOut(1, HType.UINT_4_TYPE, &j);
        }
        else
        {
            DataItemOut(HType.ASCII_TYPE, SVID);
        }

        DataItemOut(HType.ASCII_TYPE, SVName);
        DataItemOut(HType.ASCII_TYPE, SVUnit);
        DataItemOutSV(SVID);
        return true;
    }

    S="SVID:"+SVID+" not define ";
    SendInvalidDataMessageToHost(S);                                            //2013_09_06  steven
    DB->Lines->Add(S);
    return false;
}
// ---------------------------------------------------------------------------
// V 1.0
// EC output
// ---------------------------------------------------------------------------
void THGem::DataItemOutEC(AnsiString ECID)
{
    int i;
    unsigned char Type;
    AnsiString ECName;
    AnsiString ECUnit;
    void *P;
    TObject *VclP;
    AnsiString S, IsVCL, VCLStr, *SS;

    AnsiString S1, S2;

    char  VCL_ASCII[10000];
    char  VCL_BINARY;
    char  VCL_INT1;
    short VCL_INT2;
    int   VCL_INT4;
    __int64  VCL_INT8;                                                          //Steven 20140911 : ­×¥¿INT_8_TYPE & UINT_8_TYPE
    unsigned char  VCL_UINT1,VCL_BOOLEAN;
    unsigned short VCL_UINT2;
    unsigned int   VCL_UINT4;
    unsigned __int64  VCL_UINT8;                                                //Steven 20140911 : ­×¥¿INT_8_TYPE & UINT_8_TYPE

    float VCL_FT4;
    double VCL_FT8;

    i=EC_ID->IndexOf(ECID);                                                     //Steven 20150910 : For°j°é§ï¦¨ IndexOf
    if(i>=0)
    {
        IsVCL   =EC_VCL_NAME->Strings[i];
        Type    =atoi(EC_TYPE->Strings[i].c_str());
        ECName  =EC_NAME->Strings[i];
        ECUnit  =EC_UNIT->Strings[i];
        P       =EC_Ptr->Items[i];

        if(IsVCL==1 || IsVCL==2)
        {
            if(IsVCL==1)
            {
                Type    =atoi(EC_TYPE->Strings[i].c_str());
                ECName  =EC_NAME->Strings[i];
                ECUnit  =EC_UNIT->Strings[i];
                VclP    =(TObject *)EC_Ptr->Items[i];

                TPanel      *PanelPtr;
                TCustomEdit *EditPtr;
                TComboBox   *ComboBoxPtr;
                TLabel      *LabelPtr;
                TCheckBox   *CheckBoxPtr;                                       // 2013/03/14 Lee
                TRadioGroup *RadioGroupPtr;
                TStringList *StringListPtr;                                     //Steven 20140510

                PanelPtr     =dynamic_cast<TPanel       *>(VclP);
                EditPtr      =dynamic_cast<TCustomEdit  *>(VclP);
                ComboBoxPtr  =dynamic_cast<TComboBox    *>(VclP);
                LabelPtr     =dynamic_cast<TLabel       *>(VclP);
                CheckBoxPtr  =dynamic_cast<TCheckBox    *>(VclP);               // 2013/03/14 Lee
                RadioGroupPtr=dynamic_cast<TRadioGroup  *>(VclP);               // 2013/03/14 Lee
                StringListPtr=dynamic_cast<TStringList  *>(VclP);               //Steven 20140510

                if(StringListPtr!=NULL)                                         //Steven 20140510
                {
                    VCLStr=StringListPtr->CommaText;
                }
                else if(PanelPtr!=NULL)
                {
                    VCLStr=PanelPtr->Caption;
                }
                else if(EditPtr!=NULL)
                {
                    VCLStr=EditPtr->Text;
                }
                else if(ComboBoxPtr!=NULL)
                {
                    if(Type==HType.ASCII_TYPE)                                  //Steven 20240802 : ®Ú¾ÚType¦^¶Ç¸ê®Æ
                        VCLStr=ComboBoxPtr->Text;
                    else
                        VCLStr=ComboBoxPtr->ItemIndex;
                }
                else if(RadioGroupPtr!=NULL)
                {
                    if(RadioGroupPtr->ItemIndex>=0)
                    {
                        if(Type==HType.ASCII_TYPE)                              //Steven 20240802 : ®Ú¾ÚType¦^¶Ç¸ê®Æ
                            VCLStr=RadioGroupPtr->Items->Strings[RadioGroupPtr->ItemIndex];
                        else
                            VCLStr=RadioGroupPtr->ItemIndex;
                    }
                    else
                    {
                        if(Type==HType.ASCII_TYPE)                              //Steven 20240802 : ®Ú¾ÚType¦^¶Ç¸ê®Æ
                            VCLStr="";
                        else
                            VCLStr="-1";
                    }
                }
                else if(LabelPtr!=NULL)
                {
                    VCLStr=LabelPtr->Caption;
                }
                else if(CheckBoxPtr!=NULL)                                      // 2013/03/14 Lee start
                {
                    Type=HType.BOOLEAN_TYPE;
                    if(CheckBoxPtr->Checked)
                        VCLStr="1";
                    else
                        VCLStr="0";
                }
                else
                {
                    VCLStr="";
                }
            }
            else
            {
                SS=(AnsiString *)EC_Ptr->Items[i];
                VCLStr=*SS;
            }

            if(Type==HType.ASCII_TYPE)
            {
                strcpy(VCL_ASCII, VCLStr.c_str());
                P=VCL_ASCII;
            }
            else if(Type==HType.BINARY_TYPE)
            {
                VCL_BINARY=atoi(VCLStr.c_str());
                P=&VCL_BINARY;
            }
            else if(Type==HType.BOOLEAN_TYPE)
            {
                VCL_BOOLEAN=atoi(VCLStr.c_str());
                P=&VCL_BOOLEAN;
            }
            else if(Type==HType.INT_1_TYPE)
            {
                VCL_INT1=atoi(VCLStr.c_str());
                P=&VCL_INT1;
            }
            else if(Type==HType.INT_2_TYPE)
            {
                VCL_INT2=atoi(VCLStr.c_str());
                P=&VCL_INT2;
            }
            else if(Type==HType.INT_4_TYPE)
            {
                VCL_INT4=atoi(VCLStr.c_str());
                P=&VCL_INT4;
            }
            else if(Type==HType.INT_8_TYPE)                                     //Steven 20140911 : ­×¥¿INT_8_TYPE & UINT_8_TYPE
            {
                VCL_INT8=_atoi64(VCLStr.c_str());
                P=&VCL_INT8;
            }
            else if(Type==HType.UINT_1_TYPE)
            {
                VCL_UINT1=atoi(VCLStr.c_str());
                P=&VCL_UINT1;
            }
            else if(Type==HType.UINT_2_TYPE)
            {
                VCL_UINT2=atoi(VCLStr.c_str());
                P=&VCL_UINT2;
            }
            else if(Type==HType.UINT_4_TYPE)
            {
                VCL_UINT4=atoi(VCLStr.c_str());
                P=&VCL_UINT4;
            }
            else if(Type==HType.UINT_8_TYPE)                                    //Steven 20140911 : ­×¥¿INT_8_TYPE & UINT_8_TYPE
            {
                VCL_UINT8=(unsigned)_atoi64(VCLStr.c_str());
                P=&VCL_UINT8;
            }
            else if(Type==HType.FT_4_TYPE)                                      // Lee
            {
                VCL_FT4=atof(VCLStr.c_str());
                P=&VCL_FT4;
            }
            else if(Type==HType.FT_8_TYPE)
            {
                VCL_FT8=atof(VCLStr.c_str());
                P=&VCL_FT8;
            }
        }

        if(Type==HType.ASCII_TYPE)
        {
            char *Ptr;
            Ptr=(char *)P;
            DataItemOut(strlen(Ptr), Type, Ptr);
        }
        else if(Type==HType.BINARY_TYPE)
        {
            char *Ptr;
            Ptr=(char *)P;
            DataItemOut(1, Type, Ptr);
        }
        else if(Type==HType.BOOLEAN_TYPE)
        {
            unsigned char *Ptr;
            Ptr=(unsigned char *)P;
            DataItemOut(1, Type, Ptr);
        }
        else if(Type==HType.INT_1_TYPE)
        {
            char *Ptr;
            Ptr=(char *)P;
            DataItemOut(1, Type, Ptr);
        }
        else if(Type==HType.INT_2_TYPE)
        {
            short *Ptr;
            Ptr=(short *)P;
            DataItemOut(1, Type, Ptr);
        }
        else if(Type==HType.INT_4_TYPE)
        {
            int *Ptr;
            Ptr=(int *)P;
            DataItemOut(1, Type, Ptr);
        }
        else if(Type==HType.INT_8_TYPE)                                         //Steven 20140911 : ­×¥¿INT_8_TYPE & UINT_8_TYPE
        {
            __int64 *Ptr;
            Ptr=(__int64 *)P;
            DataItemOut(1, Type, Ptr);
        }
        else if(Type==HType.UINT_1_TYPE)
        {
            unsigned char *Ptr;
            Ptr=(unsigned char *)P;
            DataItemOut(1, Type, Ptr);
        }
        else if(Type==HType.UINT_2_TYPE)
        {
            unsigned short *Ptr;
            Ptr=(unsigned short *)P;
            DataItemOut(1, Type, Ptr);
        }
        else if(Type==HType.UINT_4_TYPE)
        {
            unsigned int *Ptr;
            Ptr=(unsigned int *)P;
            DataItemOut(1, Type, Ptr);
        }
        else if(Type==HType.UINT_8_TYPE)                                        //Steven 20140911 : ­×¥¿INT_8_TYPE & UINT_8_TYPE
        {
            unsigned __int64 *Ptr;
            Ptr=(unsigned __int64 *)P;
            DataItemOut(1, Type, Ptr);
        }
        else if(Type==HType.FT_4_TYPE)
        {
            float *Ptr;
            Ptr=(float *)P;
            DataItemOut(1, Type, Ptr);
        }
        else if(Type==HType.FT_8_TYPE)
        {
            double *Ptr;
            Ptr=(double *)P;
            DataItemOut(1, Type, Ptr);
        }
        return;
    }
    DataItemOut(0, HType.LIST_TYPE, NULL);
}
//------------------------------------------------------------------------------
bool THGem::IsValidSVID(AnsiString SVID)
{
    int i=SV_ID->IndexOf(SVID);                                                 //Steven 20150910 : For°j°é§ï¦¨ IndexOf
    if(i>=0)
    {
        return true;
    }
    return false;
}
//==============================================================================
// V 1.0
// for EC Name List
//==============================================================================
void THGem::DataItemOutECNameList(AnsiString ECID)
{
    int i, j;
    unsigned char Type;
    AnsiString ECName;
    AnsiString ECUnit;
    char MaxForAscii[256], MinForAscii[256], DefaultForAscii[256];              // 2013/05/08 lee
    void *PMax,*PMin,*PDefault;
    AnsiString S;
    AnsiString S1,S2;
    AnsiString PMax_Value,PMin_Value,PDefault_Value;

    i=EC_ID->IndexOf(ECID);                                                     //Steven 20150910 : For°j°é§ï¦¨ IndexOf
    if(i>=0)
    {
        Type  =atoi(EC_TYPE->Strings[i].c_str());
        ECName=EC_NAME->Strings[i];
        ECUnit=EC_UNIT->Strings[i];

        PMax=EC_Ptr_Max->Items[i];
        PMin=EC_Ptr_Min->Items[i];
        PDefault=EC_Ptr_Default->Items[i];

        PMax_Value      =EC_Ptr_Max_Value->Strings[i];
        PMin_Value      =EC_Ptr_Min_Value->Strings[i];
        PDefault_Value  =EC_Ptr_Default_Value->Strings[i];

        if(PMax==NULL || PMin==NULL || PDefault==NULL)
        {
            if(Type==HType.ASCII_TYPE )                                         // 2013/05/08 lee
            {
                strcpy(MaxForAscii, PMax_Value.c_str());
                strcpy(MinForAscii, PMin_Value.c_str());
                strcpy(DefaultForAscii, PDefault_Value.c_str());
                PMax=(void *)&MaxForAscii;
                PMin=(void *)&MinForAscii;
                PDefault=(void *)&DefaultForAscii;
            }
            else if(Type==HType.BINARY_TYPE)                                    // 2013/05/08 lee
            {
                unsigned char Max,Min,Default;
                Max=atoi(PMax_Value.c_str());
                Min=atoi(PMin_Value.c_str());
                Default=atoi(PDefault_Value.c_str());
                PMax=(void *)&Max;
                PMin=(void *)&Min;
                PDefault=(void *)&Default;
            }
            else if(Type==HType.BOOLEAN_TYPE)
            {
                bool Max,Min,Default;
                Max=atoi(PMax_Value.c_str());
                Min=atoi(PMin_Value.c_str());
                Default=atoi(PDefault_Value.c_str());
                PMax=(void *)&Max;
                PMin=(void *)&Min;
                PDefault=(void *)&Default;
            }
            else if(Type==HType.UINT_1_TYPE ||
                    Type==HType.UINT_2_TYPE ||
                    Type==HType.UINT_4_TYPE )
            {
                unsigned int Max,Min,Default;
                Max=atoi(PMax_Value.c_str());
                Min=atoi(PMin_Value.c_str());
                Default=atoi(PDefault_Value.c_str());
                PMax=(void *)&Max;
                PMin=(void *)&Min;
                PDefault=(void *)&Default;
            }
            else if(Type==HType.UINT_8_TYPE)
            {
                unsigned __int64 Max,Min,Default;
                Max=atoi(PMax_Value.c_str());
                Min=atoi(PMin_Value.c_str());
                Default=atoi(PDefault_Value.c_str());
                PMax=(void *)&Max;
                PMin=(void *)&Min;
                PDefault=(void *)&Default;

            }
            else if(Type==HType.INT_1_TYPE ||
                    Type==HType.INT_2_TYPE ||
                    Type==HType.INT_4_TYPE )
            {
                int Max,Min,Default;
                Max=atoi(PMax_Value.c_str());
                Min=atoi(PMin_Value.c_str());
                Default=atoi(PDefault_Value.c_str());
                PMax=(void *)&Max;
                PMin=(void *)&Min;
                PDefault=(void *)&Default;
            }
            else if(Type==HType.INT_8_TYPE)
            {
                __int64 Max,Min,Default;
                Max=atoi(PMax_Value.c_str());
                Min=atoi(PMin_Value.c_str());
                Default=atoi(PDefault_Value.c_str());
                PMax=(void *)&Max;
                PMin=(void *)&Min;
                PDefault=(void *)&Default;
            }
            else if(Type==HType.FT_4_TYPE)
            {
                float Max,Min,Default;
                Max=atoi(PMax_Value.c_str());
                Min=atoi(PMin_Value.c_str());
                Default=atoi(PDefault_Value.c_str());
                PMax=(void *)&Max;
                PMin=(void *)&Min;
                PDefault=(void *)&Default;
            }
            else if(Type==HType.FT_8_TYPE)
            {
                double Max,Min,Default;
                Max=atoi(PMax_Value.c_str());
                Min=atoi(PMin_Value.c_str());
                Default=atoi(PDefault_Value.c_str());
                PMax=(void *)&Max;
                PMin=(void *)&Min;
                PDefault=(void *)&Default;
            }
        }
        //2013/05/08 lee start

        // 1. L,6
        //    1. <ECID1>
        //    2. <ECNAME1>
        //    3. <ECMIN1>
        //    4. <ECMAX1>
        //    5. <ECDEF1>
        //    6. <UNITS1>

        //2013/05/08 lee end
        DataItemOut(6, HType.LIST_TYPE, NULL);

        j=atoi(ECID.c_str());
        S=j;
        if(ECID=="0" || S==ECID)
        {
            DataItemOut(1, HType.UINT_4_TYPE, &j);
        }
        else
        {
            DataItemOut(HType.ASCII_TYPE, ECID);
        }

        DataItemOut(HType.ASCII_TYPE, ECName);

        if(PMin==NULL)
        {
            DataItemOut(0, Type, "");
        }
        else
        {
            if(Type==HType.ASCII_TYPE)
                DataItemOut(Type, MinForAscii);                                 // 2013/05/08 lee
            else
                DataItemOut(1, Type, PMin);
        }

        if(PMax==NULL)
        {
            DataItemOut(0, Type, "");
        }
        else
        {
            if(Type==HType.ASCII_TYPE)
                DataItemOut(Type, MaxForAscii);                                 // 2013/05/08 lee
            else
                DataItemOut(1, Type, PMax);
        }

        if(PDefault==NULL)
        {
            DataItemOut(0, Type, "");
        }
        else
        {
            if(Type==HType.ASCII_TYPE)                                          // 2013/05/08 lee
                DataItemOut(Type, DefaultForAscii);
            else
                DataItemOut(1, Type, PDefault);
        }

        DataItemOut(HType.ASCII_TYPE, ECUnit);
        return;
    }
    DataItemOut(6, HType.LIST_TYPE, NULL);
    j=atoi(ECID.c_str());
    S=j;
    if(ECID=="0" || S==ECID)
    {
        DataItemOut(1, HType.UINT_4_TYPE, &j);
    }
    else
    {
        DataItemOut(HType.ASCII_TYPE, ECID);
    }

    DataItemOut(0, HType.ASCII_TYPE, "");
    DataItemOut(0, HType.ASCII_TYPE, "");
    DataItemOut(0, HType.ASCII_TYPE, "");
    DataItemOut(0, HType.ASCII_TYPE, "");
    DataItemOut(0, HType.ASCII_TYPE, "");
}
// ---------------------------------------------------------------------------
// V 1.0
// S5,F4
// ---------------------------------------------------------------------------
bool THGem::EnableDisableAlarm(AnsiString S, unsigned char T)
{
    for(int y=1; y<strGrdAlarm->RowCount; y++)
    {
        if(strGrdAlarm->Cells[8][y]==S)                                         //wei 20180227 (Steven) S5F3 §PÂ_¦ì¸m¿ù»~ 1-->8
        {
            if(T&0x80)
                strGrdAlarm->Cells[7][y]="1";
            else
                strGrdAlarm->Cells[7][y]="0";
            WriteAlamData();
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
void THGem::EnableDisableAlarmAll(unsigned char T)
{
    for(int y=1; y<strGrdAlarm->RowCount; y++)
    {
        if(T&0x80)
            strGrdAlarm->Cells[7][y]="1";
        else
            strGrdAlarm->Cells[7][y]="0";
    }
    WriteAlamData();
}
//------------------------------------------------------------------------------
int THGem::GetAlarmIndex(AnsiString S)
{
    for(int y=1; y<strGrdAlarm->RowCount; y++)
        if(strGrdAlarm->Cells[8][y]==S)
            return y;
    return -1;
}
//=============================================================================
// =                                                                           =
// =            ¹ï Host ªº­n¨D°µ Polling ³B²z©Ò¥Î¨ìªºµ{§Ç¶°¦X                  =
// =                                                                           =
//=============================================================================
//------------------------------------------------------------------------------
// V 1.0
// ¹ï Socket ªº Enable ³B²z
//------------------------------------------------------------------------------
bool THGem::DoOpenCommuncation()
{
    int &Task=iOpenCommuncationTask;

    if(bUseClientSocket==true)
    {
        switch(Task)
        {
            case 1:
                if(bOpenCommuncation==true)
                    Task=100;
                break;
            case 100:
                if(clientGem->Address=="" || clientGem->Port==0)
                    return false;

                //16.04.01.01s Roy Change
                //clientGem->Active=true;
                try
                {
                    clientGem->Active=true;
                }
                catch(...)
                {
                    MyDBIProcess("Exception", "THGem::DoOpenCommuncation");
                }
                //16.04.01.01e

                if(clientGem->Active==true)
                {
                    Task=200;
                }
                else
                {
                    DelayOpenCommuncation.TimerSetSecAndOn(60);
                    Task=150;
                }
                break;
            case 150:
                if(DelayOpenCommuncation.TimerOff())
                    Task=100;
                    break;
            case 200:
                if(bTCPIP_Error==true)
                {
                    bTCPIP_Error=false;
                    Task=100;
                }
                else
                {
                    Task=1;
                    return true;
                }
                break;
        }
        return false;
    }
    else
    {
        switch(Task)
        {
            case 1:
                if(bOpenCommuncation==true)
                {
                    //srvGem->Close();                                          // Â_½u«á³s¤£°_¨Ó 2013/07/18 lee §â Servo Socket close ¦A open ¬Ý¬Ý // ¤£¦æ·|·í !!!!!!!!!!!
                    DelayOpenCommuncation.TimerSetSecAndOn(0.5);
                    Task=100;
                }
                break;
            case 100:
                if(DelayOpenCommuncation.TimerOff()==false)
                    break;

                if(srvGem->Port==0)
                    return false;

                if(srvGem->Active==false)                                       //Eliot 2012_1108
                    srvGem->Open();

                if(srvGem->Active==true)
                {
                    Task=200;
                }
                else
                {
                    DelayOpenCommuncation.TimerSetSecAndOn(2);
                    Task=150;
                }
                break;
            case 150:
                if(DelayOpenCommuncation.TimerOff())
                    Task=100;
                    break;
            case 200:
                if(bTCPIP_Error==true)
                {
                    bTCPIP_Error=false;
                    Task=100;
                }
                else if(srvGem->Socket->ActiveConnections!=0)
                {
                    Task=1;
                    return true;
                }
                break;
        }
        return false;
    }
}
//------------------------------------------------------------------------------
// V 1.0
// ¹ï »P remote «Ø¥ß³s½uªº­n¨D¶Ç°e
// Select.req
//------------------------------------------------------------------------------
void THGem::DoSelect()
{
    Local.MessageID_S=0;
    Local.MessageID_F=0;
    Local.W_Bit=0;
    Local.PType=0;
    Local.SType=SType.Select_req;
    EquipmentSystemByte++;
    Local.SystemByte=EquipmentSystemByte;
    LocalLength=0;
    CreateLocalHead();
    StringOut("[Send]    Select.req");
    SendLocalData();
}
//---------------------------------------------------------------------------
//  control message Separate_req
//---------------------------------------------------------------------------
//------------------------------------------------------------------------------
// V 1.0
// ¹ï »P remote Â÷½u­n¨D¶Ç°e
// Separate_req
//------------------------------------------------------------------------------
void THGem::DoSeparate()
{
    Local.MessageID_S=0;
    Local.MessageID_F=0;
    Local.W_Bit=0;
    Local.PType=0;
    Local.SType=SType.Separate_req;
    EquipmentSystemByte++;
    Local.SystemByte=EquipmentSystemByte;
    LocalLength=0;
    CreateLocalHead();
    StringOut("[Send]    Separate_req");
    SendLocalData();
}
//------------------------------------------------------------------------------
// V 1.0
// ¹ï »P remote «Ø¥ß³s½uªº­n¨D¶Ç°e
//------------------------------------------------------------------------------
int THGem::DoConnect()
{
    int &Task=iStartConnectTask;

    switch(Task)
    {
        case 1:
            bWaitSelectRsp=false;
            bTCPIP_Error=false;
            DoSelect();
            ConnectDelay.TimerSetSecAndOn(3);
            Task=100;
            break;
        case 100:
            if(bWaitSelectRsp==true)
            {
                iConnectTryCount=iEstablishCommunicationsTryCount;
                bWaitEstablishCommunicationsResponse=false;
                bWaitEstablishCommunicationsResponseError=false;
                HSys.MyGem->S1F13_EstablishCommunicationsRequest();
                Task=200;
            }
            else if(ConnectDelay.TimerOff())
            {
                Task=1;
                return 2;
            }
            break;
        case 200:
           if(bWaitEstablishCommunicationsResponse==true)
           {
                if(bWaitEstablishCommunicationsResponseError==true)
                {
                    if(iEstablishCommunicationsTryCount!=0)
                    {
                        if(iConnectTryCount>iEstablishCommunicationsTryCount)
                            iConnectTryCount=iEstablishCommunicationsTryCount;
                        iConnectTryCount--;
                        if(iConnectTryCount<=0)
                            return 2;
                    }
                    countConnect=0;
                    Task=500;
                    break;
                }
                bConnect=true;
                Task=1;
                return 1;
           }
           break;
        case 500:
            countConnect++;
            if(countConnect>100)
                Task=600;
            break;
        case 600:
            bWaitEstablishCommunicationsResponse=false;
            bWaitEstablishCommunicationsResponseError=false;
            HSys.MyGem->S1F13_EstablishCommunicationsRequest();
            Task=400;
            break;
    }
    return 0;
}
//------------------------------------------------------------------------------
// V 1.0
// ¹ï »P remote «Ø¥ß³s½u«áªº Online[online local or remore] or offlineªº­n¨D¶Ç°e
//------------------------------------------------------------------------------
void THGem::OnlineLocalOrRemote()
{
//    char str[64];
//    AnsiString S;
//    int i;
//    int data=5;

    bS1F2_OnLineData=false;

    if(bOnLineLocal==true)
    {
//        EventReport(1, 92);     //pig 20170323 2DBarcode SECS/GEM  //JerryYang 20230204 : SECS/GEM GControl StateÂà´«§¹¦¨¦Aµo³øEvent report
    }
    else
    {
//        EventReport(1, 93);     //pig 20170323 2DBarcode SECS/GEM  //JerryYang 20230204 : SECS/GEM GControl StateÂà´«§¹¦¨¦Aµo³øEvent report
    }
}
//------------------------------------------------------------------------------
// V 1.0
// ¹ï »P remote «Ø¥ß³s½u«áªº Online[online local or remore] or offlineªº­n¨D¶Ç°e
//------------------------------------------------------------------------------
bool THGem::DoOnLine()
{
    int &Task=iStartOnLineTask;
    switch(Task)
    {
        case 1:
            Task=200;
            break;
        case 100:
            if(bS1F2_OnLineData)
                Task=200;
            break;
        case 200:
            OnlineLocalOrRemote();
            bOnLine=true;
            bStartOnLine=false;
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------
// V 1.0
// ¨ú±o EC ¸ê®Æ
//------------------------------------------------------------------------------
AnsiString THGem::GetECDataValue(AnsiString ECID)
{
    int i;
    unsigned char Type;
    AnsiString ECName;
    AnsiString ECUnit, IsVCL, VCLStr, *SS;
    void *P;
    TObject *VclP;
    AnsiString S;

    char  VCL_ASCII[10000];
    char  VCL_BINARY;
    char  VCL_INT1;
    short VCL_INT2;
    int   VCL_INT4;
    __int64  VCL_INT8;                                                          //Steven 20140911 : ­×¥¿INT_8_TYPE & UINT_8_TYPE
    unsigned char  VCL_UINT1,VCL_BOOLEAN;
    unsigned short VCL_UINT2;
    unsigned int   VCL_UINT4;
    unsigned __int64  VCL_UINT8;                                                //Steven 20140911 : ­×¥¿INT_8_TYPE & UINT_8_TYPE

    float VCL_FT4;
    double VCL_FT8;
//    bool ret=true;

    i=EC_ID->IndexOf(ECID);                                                     //Steven 20150910 : For°j°é§ï¦¨ IndexOf
    if(i>=0)
    {
        if(EC_VCL_NAME->Count>i)
            IsVCL=EC_VCL_NAME->Strings[i];
        else
            IsVCL=0;
        if(IsVCL==1 || IsVCL==2)                                                //Steven 20151111 : ¥[¤WVCLÅª¨úEC
        {
            if(IsVCL==1)
            {
                Type    =atoi(EC_TYPE->Strings[i].c_str());
                ECName  =EC_NAME->Strings[i];
                ECUnit  =EC_UNIT->Strings[i];
                VclP    =(TObject *)EC_Ptr->Items[i];

                TPanel      *PanelPtr;
                TCustomEdit *EditPtr;
                TComboBox   *ComboBoxPtr;
                TLabel      *LabelPtr;
                TCheckBox   *CheckBoxPtr;                                       // 2013/03/14 Lee
                TRadioGroup *RadioGroupPtr;
                TStringList *StringListPtr;                                     //Steven 20140510

                PanelPtr     =dynamic_cast<TPanel       *>(VclP);
                EditPtr      =dynamic_cast<TCustomEdit  *>(VclP);
                ComboBoxPtr  =dynamic_cast<TComboBox    *>(VclP);
                LabelPtr     =dynamic_cast<TLabel       *>(VclP);
                CheckBoxPtr  =dynamic_cast<TCheckBox    *>(VclP);               // 2013/03/14 Lee
                RadioGroupPtr=dynamic_cast<TRadioGroup  *>(VclP);               // 2013/03/14 Lee
                StringListPtr=dynamic_cast<TStringList  *>(VclP);               //Steven 20140510

                if(StringListPtr!=NULL)                                         //Steven 20140510
                {
                    VCLStr=StringListPtr->CommaText;
                }
                else if(PanelPtr!=NULL)
                {
                    VCLStr=PanelPtr->Caption;
                }
                else if(EditPtr!=NULL)
                {
                    VCLStr=EditPtr->Text;
                }
                else if(ComboBoxPtr!=NULL)
                {
                    if(Type==HType.ASCII_TYPE)                                  //Steven 20240802 : ®Ú¾ÚType¦^¶Ç¸ê®Æ
                        VCLStr=ComboBoxPtr->Text;
                    else
                        VCLStr=ComboBoxPtr->ItemIndex;
                }
                else if(RadioGroupPtr!=NULL)
                {
                    if(RadioGroupPtr->ItemIndex>=0)
                    {
                        if(Type==HType.ASCII_TYPE)                              //Steven 20240802 : ®Ú¾ÚType¦^¶Ç¸ê®Æ
                            VCLStr=RadioGroupPtr->Items->Strings[RadioGroupPtr->ItemIndex];
                        else
                            VCLStr=RadioGroupPtr->ItemIndex;
                    }
                    else
                    {
                        if(Type==HType.ASCII_TYPE)                              //Steven 20240802 : ®Ú¾ÚType¦^¶Ç¸ê®Æ
                            VCLStr="";
                        else
                            VCLStr="-1";
                    }
                }
                else if(LabelPtr!=NULL)
                {
                    VCLStr=LabelPtr->Caption;
                }
                else if(CheckBoxPtr!=NULL)                                      // 2013/03/14 Lee start
                {
                    Type=HType.BOOLEAN_TYPE;
                    if(CheckBoxPtr->Checked)
                        VCLStr="1";
                    else
                        VCLStr="0";
                }
                else
                {
                    VCLStr="";
                }
            }
            else
            {
                SS=(AnsiString *)EC_Ptr->Items[i];
                VCLStr=*SS;
            }

            if(Type==HType.ASCII_TYPE)
            {
                strcpy(VCL_ASCII, VCLStr.c_str());
                P=VCL_ASCII;
            }
            else if(Type==HType.BINARY_TYPE)
            {
                VCL_BINARY=atoi(VCLStr.c_str());
                P=&VCL_BINARY;
            }
            else if(Type==HType.BOOLEAN_TYPE)
            {
                VCL_BOOLEAN=atoi(VCLStr.c_str());
                P=&VCL_BOOLEAN;
            }
            else if(Type==HType.INT_1_TYPE)
            {
                VCL_INT1=atoi(VCLStr.c_str());
                P=&VCL_INT1;
            }
            else if(Type==HType.INT_2_TYPE)
            {
                VCL_INT2=atoi(VCLStr.c_str());
                P=&VCL_INT2;
            }
            else if(Type==HType.INT_4_TYPE)
            {
                VCL_INT4=atoi(VCLStr.c_str());
                P=&VCL_INT4;
            }
            else if(Type==HType.INT_8_TYPE)                                     //Steven 20140911 : ­×¥¿INT_8_TYPE & UINT_8_TYPE
            {
                VCL_INT8=_atoi64(VCLStr.c_str());
                P=&VCL_INT8;
            }
            else if(Type==HType.UINT_1_TYPE)
            {
                VCL_UINT1=atoi(VCLStr.c_str());
                P=&VCL_UINT1;
            }
            else if(Type==HType.UINT_2_TYPE)
            {
                VCL_UINT2=atoi(VCLStr.c_str());
                P=&VCL_UINT2;
            }
            else if(Type==HType.UINT_4_TYPE)
            {
                VCL_UINT4=atoi(VCLStr.c_str());
                P=&VCL_UINT4;
            }
            else if(Type==HType.UINT_8_TYPE)                                    //Steven 20140911 : ­×¥¿INT_8_TYPE & UINT_8_TYPE
            {
                VCL_UINT8=(unsigned)_atoi64(VCLStr.c_str());
                P=&VCL_UINT8;
            }
            else if(Type==HType.FT_4_TYPE)
            {
                VCL_FT4=atof(VCLStr.c_str());
                P=&VCL_FT4;
            }
            else if(Type==HType.FT_8_TYPE)
            {
                VCL_FT8=atof(VCLStr.c_str());
                P=&VCL_FT8;
            }
        }
        else
        {
            Type    =atoi(EC_TYPE->Strings[i].c_str());
            ECName  =EC_NAME->Strings[i];
            ECUnit  =EC_UNIT->Strings[i];
            P       =EC_Ptr->Items[i];
        }

        if(Type==HType.ASCII_TYPE)
        {
            char *Ptr;
            Ptr=(char *)P;
            return AnsiString(Ptr);
        }
        else if(Type==HType.BINARY_TYPE)
        {
            char *Ptr;
            Ptr=(char *)P;
            return AnsiString(*Ptr);
        }
        else if(Type==HType.BOOLEAN_TYPE)
        {
            unsigned char *Ptr;
            Ptr=(unsigned char *)P;
            return AnsiString(*Ptr);
        }
        else if(Type==HType.INT_1_TYPE)
        {
            char *Ptr;
            Ptr=(char *)P;
            return AnsiString(*Ptr);
        }
        else if(Type==HType.INT_2_TYPE)
        {
            short *Ptr;
            Ptr=(short *)P;
            return AnsiString(*Ptr);
        }
        else if(Type==HType.INT_4_TYPE)
        {
            int *Ptr;
            Ptr=(int *)P;
            return AnsiString(*Ptr);
        }
        else if(Type==HType.INT_8_TYPE)                                         //Steven 20140911 : ­×¥¿INT_8_TYPE & UINT_8_TYPE
        {
            __int64 *Ptr;
            Ptr=(__int64 *)P;
            return AnsiString(*Ptr);
        }
        else if(Type==HType.UINT_1_TYPE)
        {
            unsigned char *Ptr;
            Ptr=(unsigned char *)P;
            return AnsiString(*Ptr);
        }
        else if(Type==HType.UINT_2_TYPE)
        {
            unsigned short *Ptr;
            Ptr=(unsigned short *)P;
            return AnsiString(*Ptr);
        }
        else if(Type==HType.UINT_4_TYPE)
        {
            unsigned int *Ptr;
            Ptr=(unsigned int *)P;
            return AnsiString(*Ptr);
        }
        else if(Type==HType.UINT_8_TYPE)                                        //Steven 20140911 : ­×¥¿INT_8_TYPE & UINT_8_TYPE
        {
            unsigned __int64 *Ptr;
            Ptr=(unsigned __int64 *)P;
            return AnsiString(*Ptr);
        }
        else if(Type==HType.FT_4_TYPE)                                          //Steven 20130730 ADD
        {
            float *Ptr;
            Ptr=(float *)P;
            return AnsiString(*Ptr);
        }
        else if(Type==HType.FT_8_TYPE)                                          //Steven 20130730 ADD
        {
            double *Ptr;
            Ptr=(double *)P;
            return AnsiString(*Ptr);
        }
        return "";
    }
    return "";
}
//------------------------------------------------------------------------------
// V 1.0
// ¹ï EC §ïÅÜ°e¥X EC §ïÅÜªº Event
// 2013/04/02 lee
//------------------------------------------------------------------------------
void THGem::ReplyECDataChange()
{
    bS1F2_OnLineData=false;
    for(int i=0; i<SECSReportIDChange->Count; i++)
    {
        SYS_ECChangeID             = SECSReportIDChange->Strings[i];
        SYS_ECChangeIDOriginaValue = SECSOriginalValue->Strings[i];
        SYS_ECChangeIDNewValue     = SECSNewECValue->Strings[i];

        if(GetECEnableData(SYS_ECChangeID)==true)                               //pig 2014.08.27 KYEC
            EventReport(1, 48);                                                 //SECS_EVENT.ChangeEC
    }
    SECSReportIDChange->Clear();
    SECSOriginalValue->Clear();
    SECSNewECValue->Clear();
}
//------------------------------------------------------------------------------
// V 1.0
// ¹ï EC ¬O§_§ïÅÜÀË¬d
//------------------------------------------------------------------------------
void THGem::DoReportECDataChangeCheck()
{
    AnsiString S;
    bool bChange=false;
    if(bOnLine==false)
        return;
    if(cbECChaneEventReport->Checked==false)                                    //JerryYang 20200520 ÂÂ¹q¸£°µDoReportECDataChangeCheck¨ç¦¡·|¼vÅTUPH,§ï¦¨¥\¯à¿ï¶µ
        return;
    if(iReportECDataChangeCheckOldSecond!=SystemSec)
    {
        if((SystemSec%2)==0)
        {
            iReportECDataChangeCheckOldSecond=SystemSec;
            for(int i=0; i<EC_ID->Count; i++)
            {
                S=GetECDataValue(atoi(EC_ID->Strings[i].c_str()));
                if(S!=EC_OldValue->Strings[i])
                {
                    SECSReportIDChange->Add(EC_ID->Strings[i].c_str());
                    SECSOriginalValue->Add(EC_OldValue->Strings[i]);
                    SECSNewECValue->Add(S);
                    EC_OldValue->Strings[i]=S;
                    bChange=true;
                }
            }
        }
    }
    if(bChange)
    {
        ReplyECDataChange();
    }
}
//---------------------------------------------------------------------------
//Steven 20200807 : ¶XµÛSleepªº®É­Ô¥hÀË¬dEC change report
//==>
int THGem::DoReportECChange(int iIndex)
{
    AnsiString S;
    bool bChange=false;
    if(bOnLine==false || InitialOK==false)
        return -1;
//    if(iReportECDataChangeCheckOldSecond!=SystemSec)
//    {
//        if((SystemSec%2)==0)
//        {
            iReportECDataChangeCheckOldSecond=SystemSec;
            S=GetECDataValue(atoi(EC_ID->Strings[iIndex].c_str()));
            if(S!=EC_OldValue->Strings[iIndex])
            {
                SECSReportIDChange->Add(EC_ID->Strings[iIndex].c_str());
                SECSOriginalValue->Add(EC_OldValue->Strings[iIndex]);
                SECSNewECValue->Add(S);
                EC_OldValue->Strings[iIndex]=S;
                bChange=true;
            }
//        }
        if(bChange)
        {
            ReplyECDataChange();
        }
        return 1;
//    }
//    return -1;
}
//<==
//Steven 20200807 : ¶XµÛSleepªº®É­Ô¥hÀË¬dEC change report
//------------------------------------------------------------------------------
// V 1.0
// Spool process loop
//------------------------------------------------------------------------------
int THGem::DoSpoolSendLocalData(unsigned char *Ptr)
{
    AnsiString S;
    char str[8];
    unsigned int i,RunLength=0;
    bReceiveData=false;

    RunLength=0;
    for(i=0; i<4; i++)
    {
        RunLength<<=8;
        RunLength|=Ptr[i];
    }

    RunLength+=4;
    if(bUseClientSocket)
        clientGem->Socket->SendBuf(Ptr, RunLength);
    else
        srvGem->Socket->Connections[0]->SendBuf(Ptr, RunLength);

    StringOut("---------------------------------------------------");
    S=AnsiString("[Send]    ")+DateTimeToStr(Now());

    StringOut(S);
    ShowSFDescription(Ptr[6]&0x7f, Ptr[7]);

    if(GemCheckBoxShowBinary->Checked==true)                                    //Steven 20211109 : SECS¤£­n¨C¦¸³£Åã¥Übinary code
    {
        StringOut("");
        S="";
        for(i=0; i<4; i++)
        {
            sprintf(str, "%02X,", Ptr[i]);
            S+=AnsiString(str);
        }
        StringOut(S);
        S="";
        for(i=4; i<14; i++)
        {
            sprintf(str, "%02X,", Ptr[i]);
            S+=AnsiString(str);
        }

        StringOut(S);

        ShowSMLBinary(Ptr, RunLength);
    }
    ShowSML(Ptr, RunLength);
    return RunLength;
}
//------------------------------------------------------------------------------
// V 1.0
// Spool process loop
//------------------------------------------------------------------------------
void THGem::DoSpool()
{
    int &Task=iSpoolTask;
    FILE *P;
    unsigned TotalFileSize;
    int handle;

    AnsiString S;

    FileListBox1->Refresh();
    FileListBox1->Update();

    if(bSpoolActive==false)
    {
        bSpooling=false;
        bBeginTransferSpool=false;
        if(OldSpoolSystemMin!=SystemMin)
        {
            OldSpoolSystemMin=SystemMin;
            FileListBox1->Refresh();
            FileListBox1->Update();
            if(FileListBox1->Items->Count!=0)
            {
                S=AnsiString("del ")+IncludeTrailingPathDelimiter(GemSpoolPath)+AnsiString("*.*/q/f");
                system(S.c_str());
            }
            GemSpoolCountTotal=FileListBox1->Items->Count;
            GemSpoolCountActual=GemSpoolCountTotal;
        }
        return;
    }
    if(bBeginTransferSpool==false)
    {
        if(OldSpoolSystemMin!=SystemMin)
        {
            OldSpoolSystemMin=SystemMin;
            FileListBox1->Refresh();
            FileListBox1->Update();
            GemSpoolCountTotal=FileListBox1->Items->Count;
            GemSpoolCountActual=GemSpoolCountTotal;
        }
        return;
    }
    switch(Task)
    {
        case 1:
            if(bConnect==false)
            {
                bSpooling=false;
                return;
            }
            if(FileListBox1->Items->Count==0)
            {
                FileListBox1->Refresh();
                FileListBox1->Update();
                if(FileListBox1->Items->Count==0)
                {
                    bSpooling=false;
                    bBeginTransferSpool=false;
                    break;
                }
            }
            else
            {
                bSpooling=true;
            }
            S=IncludeTrailingPathDelimiter(GemSpoolPath)+FileListBox1->Items->Strings[0];
            handle=open(S.c_str(), O_RDONLY);
            if(handle!=-1)
            {
                TotalFileSize=filelength(handle);
                close(handle);
                P=fopen(S.c_str(), "rb");
                if(P!=NULL)
                {
                    SpoolPtr=new char [TotalFileSize+100];
                    fread(SpoolPtr, TotalFileSize, 1, P);
                    SpoolRunPtr=SpoolPtr;
                    fclose(P);
                }
                DeleteFile(S);
                GemSpoolCountActual--;
                DoSpoolSendLocalData(SpoolRunPtr);
                Task=100;
            }
            else
            {
                DeleteFile(S);
            }
            break;
        case 100:
            delete[] SpoolPtr;                                                  //Ifor 20170603 ­×§ï°}¦C§R°£¤è¦¡ delete ==> delete[]
            SpoolPtr=NULL;                                                      //kevin 20180517
            Task=1;
            break;
        case 200:
            SpoolDelay.TimerSetSecAndOn(0.1);
            Task=300;
            break;
        case 300:
            if(SpoolDelay.TimerOff())
                Task=1;
            break;
    }
}
//------------------------------------------------------------------------------
// V 1.0
// ¹ï Trace Data resopne
//------------------------------------------------------------------------------
void THGem::DoTraceDataResponse(int iIndex)
{
    AnsiString str;
    unsigned itrid;

    int &Task=TraceDataResponseTask[iIndex];
    if(bTraceData[iIndex]==false)
    {
        TraceDataResponseTask[iIndex]=1;
        return;
    }

    switch(Task)
    {
        case 1:
            TraceDataResponseDelay[iIndex].TimerSetMSAndOn(DSPER[iIndex]);
            Task=100;
        case 100:
            if(TraceDataResponseDelay[iIndex].TimerOff())
            {
                InitLocalHead(6, 1, 0);
                DataItemOut(4, HType.LIST_TYPE, NULL);
                itrid=atoi(iTRID[iIndex].c_str());
                DataItemOut(1, HType.UINT_4_TYPE, &itrid);
                DataItemOut(1, HType.UINT_2_TYPE, &iTOTSMP_Count[iIndex]);
                iTOTSMP_Count[iIndex]++;

                GetTimeInfo();
                if(iTimeFormat==1)                                                                                                                 //16 byte
                    str.sprintf("%04d%02d%02d%02d%02d%02d%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec/10);
                else if(iTimeFormat==2)                                                                                                            //14 byte
                    str.sprintf("%04d%02d%02d%02d%02d%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
                else if(iTimeFormat==3)                                                                                                            //19 byte
                    str.sprintf("%04d-%02d-%02dT%02d:%02d:%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
                else
                    str.sprintf("%02d%02d%02d%02d%02d%02d", SystemYear%100, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
                DataItemOut(HType.ASCII_TYPE, str);

                DataItemOut(iREPGSZ[iIndex], HType.LIST_TYPE, NULL);
                for(unsigned i=0; i<iREPGSZ[iIndex]; i++)
                    DataItemOutSV(TraceData[iIndex]->Strings[i]);
                SendLocalData();
                if(iTOTSMP_Count[iIndex]>iTOTSMP[iIndex])
                {
                    bTraceData[iIndex]=false;
                    Task=1;
                }
                Task=1;
                break;
            }
            break;
    }
}
//------------------------------------------------------------------------------
// V 1.0
// ¹ï »P remote ³s½u«á¸ê®Æ¦^À³[»Ý­n§Q¥Î¤£¦P S,F code ªº³¡¤À]
// ³æ¿W¤@­ÓÀÉ®×
//------------------------------------------------------------------------------
// EC69_UNT1_MaxTranslateLen
void THGem::DoUploadFileToHost_ForSingleFile()
{
    int &Task=iUploadFileToHost_ForSingleFile;
    int i, j;
    long &TotalFileSize=UploadFileToHost_ForSingleFileTotalFileSize;
    FILE *P;
    P=UploadFileToHost_ForSingleFileFilePtr;
    AnsiString S, FileName;
    int &iStoreCT=iUploadFileToHost_ForSingleFileStoreCT, &iTotalCount=iUploadFileToHost_ForSingleFileTotalCount;
    int handle,iReadSize;
    int iMaxSend=iUploadFileToHost_ForSingleFileMaxSend;

    switch(Task)
    {
        case 1:
            if(UploadFileName->Count==0)
                break;
            Task=100;
            break;
        case 100:
            iMaxSend=EC69_UNT1_MaxTranslateLen;
            if(iMaxSend==1)
                iMaxSend=8192;
            else if(iMaxSend==2)
                iMaxSend=64*1024;
            else if(iMaxSend==3)
                iMaxSend=128*1024;
            else if(iMaxSend==4)
                iMaxSend=256*256*256;
            else
                iMaxSend=4096;

            S=FileListBox2->Mask;
            i=S.LastDelimiter("\\");
            S=S.SubString(1, i);
            FileName=UploadFileName->Strings[0];
            j=GemLocalFileLixtBox->Items->IndexOf(FileName);
            if(j!=-1)
                GemLocalFileLixtBox->Checked[j]=false;

            S+=FileName;
            UploadFileName->Delete(0);
            handle = open(S.c_str(),O_RDONLY);
            if(handle==-1)
                return;
            TotalFileSize=filelength(handle);
            close(handle);

            iStoreCT=0;
            iTotalCount=ChangeToFloatNonPcnt((double)(TotalFileSize), (double)(iMaxSend));
            if((TotalFileSize%iMaxSend)!=0)
                iTotalCount++;
            P=fopen(S.c_str(),"rb");
            if(P==NULL)
            {
                Task=1;
                break;
            }
            Task=200;
            break;
        case 200:
            iStoreCT++;
            if(TotalFileSize>iMaxSend)
            {
                iReadSize=iMaxSend;
                Task=300;
            }
            else
            {
                iReadSize=TotalFileSize;
                Task=1;
            }
            if(iReadSize!=0)
                fread(PtrUploadFileToHost_ForSingleFile, iReadSize, 1, P);
            TotalFileSize-=iReadSize;
            if(TotalFileSize==0)
                fclose(P);
            bDisableBinaryShow=true;
            InitLocalHead(101,5,1);
            DataItemOut(4, HType.LIST_TYPE, NULL);

            DataItemOut(HType.ASCII_TYPE, FileName);                            // 1.filename
            DataItemOut(1, HType.INT_4_TYPE, &iStoreCT);                        // 2.iStoreCT
            DataItemOut(1, HType.INT_4_TYPE, &iTotalCount);                     // 3.iTotalCount
            DataItemOut(iReadSize, HType.BINARY_TYPE, PtrUploadFileToHost_ForSingleFile);   // 4.data body
            bReceiveS101F6=false;
            SendLocalData();
            bDisableBinaryShow=false;
            break;
        case 300:
            if(bReceiveS101F6==true)
                Task=200;
            break;
    }
}
//------------------------------------------------------------------------------
// V 1.0
// ¹ï »P remote ³s½u«á¸ê®Æ¦^À³[»Ý­n§Q¥Î¤£¦P S,F code ªº³¡¤À]
// ¦hÀÉ®×
//------------------------------------------------------------------------------
void THGem::DoUploadFileToHost_ForMultiFile()
{
    int &Task=iUploadFileToHost_ForMultiFileTask;
    int &iCT=iUploadFileToHost_ForMultiFileCT;

    int j;
    long &TotalFileSize=UploadFileToHost_ForMultiFileTotalFileSize;
    char Ptr[8000];
    AnsiString S, FileName;

    int &iStoreCT=iStoreCTUploadFileToHost_ForMultiFile;
    int &iTotalCount=iTotalCountUploadFileToHost_ForMultiFile;
    int handle, iReadSize;

    switch(Task)
    {
        case 1:
            if(UploadFileName->Count==0)
                break;
            Task=100;
            break;
        case 100:
            S=UpLoadPath;
            UploadFileToHost_ForMultiFileMultiFile=S;
            Task=120;
            break;
        case 120:
            ForMultiFileFileName=UploadFileName->Strings[0];
            j=GemLocalFileLixtBox->Items->IndexOf(ForMultiFileFileName);
            if(j!=-1)
                GemLocalFileLixtBox->Checked[j]=false;

            UploadFileName->Delete(0);
            S=UploadFileToHost_ForMultiFileMultiFile+ForMultiFileFileName;
            S+=".*";
            FileListBox2->Mask=S;
            FileListBox2->Update();
            FileListBox2->Refresh();
            iCT=0;
            Task=140;
            break;
        case 140:
            if(iCT>=FileListBox2->Items->Count)
            {
                Task=1;
                break;
            }
            S=UploadFileToHost_ForMultiFileMultiFile+FileListBox2->Items->Strings[iCT];
            ForMultiFileFileName=FileListBox2->Items->Strings[iCT];
            handle=open(S.c_str(), O_RDONLY);
            if(handle==-1)
                return;
            TotalFileSize=filelength(handle);
            close(handle);

            iStoreCT=0;
            iTotalCount=TotalFileSize/8000;
            if((TotalFileSize%8000)!=0)
                iTotalCount++;

            PFileUploadFileToHost_ForMultiFile=fopen(S.c_str(), "rb");   //2014/02/01 lee   // ASEM
            if(PFileUploadFileToHost_ForMultiFile==NULL)                 //2014/02/01 lee   // ASEM
            {
                Task=1;
                break;
            }
            Task=200;
            break;
        case 200:
            iStoreCT++;
            if(TotalFileSize>8000)
            {
                iReadSize=8000;
                Task=300;
            }
            else
            {
                iReadSize=TotalFileSize;
                iCT++;
                Task=400;
            }
            if(iReadSize!=0)
                fread(Ptr, iReadSize, 1, PFileUploadFileToHost_ForMultiFile);   //2014/02/01 lee   // ASEM
            TotalFileSize-=iReadSize;
            if(TotalFileSize==0)
                fclose(PFileUploadFileToHost_ForMultiFile);                     //2014/02/01 lee   // ASEM
            bDisableBinaryShow=true;
            InitLocalHead(101, 5, 1);
            DataItemOut(5, HType.LIST_TYPE, NULL);

            DataItemOut(HType.ASCII_TYPE, ForMultiFileFileName);                // 1.filename
            DataItemOut(1, HType.INT_4_TYPE, &iStoreCT);                        // 2.iStoreCT
            DataItemOut(1, HType.INT_4_TYPE, &iTotalCount);                     // 3.iTotalCount
            DataItemOut(iReadSize, HType.BINARY_TYPE, Ptr);                     // 4.data body
            bReceiveS101F6=false;
            SendLocalData();
            bDisableBinaryShow=false;
            UploadFileToHost_ForMultiFileDelay.TimerSetSecAndOn(1);
            break;
        case 300:
            if(bReceiveS101F6==true)
            {
                Task=200;
            }
            else if(UploadFileToHost_ForMultiFileDelay.TimerOff())
            {
                Task=1;
            }
            break;
        case 400:
            if(bReceiveS101F6==true)
            {
                Task=140;
            }
            else if(UploadFileToHost_ForMultiFileDelay.TimerOff())
            {
                Task=1;
            }
            break;
    }
}
//------------------------------------------------------------------------------
// V 1.0
// ¹ï »P remote ³s½u«á¸ê®Æ¦^À³[»Ý­n§Q¥Î¤£¦P S,F code ªº³¡¤À]
// ¾ã­Ó¥Ø¿ý
//------------------------------------------------------------------------------
void THGem::DoUploadFileToHost_ForDirectoryFile()
{
    int &Task=iUploadFileToHost_ForDirectoryFileTask;
    int j;
    int &iCT=iCTUploadFileToHost_ForDirectoryFile;
    long &TotalFileSize=TotalFileSizeUploadFileToHost_ForDirectoryFile;
    char Ptr[8000];
    AnsiString S;
    int handle, iReadSize;

    switch(Task)
    {
        case 1:
            if(UploadFileName->Count==0)
                break;
            Task=100;
            break;
        case 100:
            S=UpLoadPath;
            MultiFileUploadFileToHost_ForDirectoryFile=S;
            Task=120;
            break;
        case 120:
            PathNameUploadFileToHost_ForDirectoryFile=UploadFileName->Strings[0];
            j=GemLocalFileLixtBox->Items->IndexOf(PathNameUploadFileToHost_ForDirectoryFile);
            if(j!=-1)
                GemLocalFileLixtBox->Checked[j]=false;

            UploadFileName->Delete(0);
            S=MultiFileUploadFileToHost_ForDirectoryFile+"\\"+PathNameUploadFileToHost_ForDirectoryFile+"\\*.*";
            FileListBox2->Mask=S;
            FileListBox2->Update();
            FileListBox2->Refresh();
            iCT=0;
            Task=140;
            break;
        case 140:
            if(iCT>=FileListBox2->Items->Count)
            {
                Task=1;
                break;
            }
            S=MultiFileUploadFileToHost_ForDirectoryFile+"\\"+PathNameUploadFileToHost_ForDirectoryFile+"\\"+FileListBox2->Items->Strings[iCT];
            FileNameUploadFileToHost_ForDirectoryFile=FileListBox2->Items->Strings[iCT];
            handle = open(S.c_str(), O_RDONLY);
            if(handle==-1)
                return;
            TotalFileSize=filelength(handle);
            close(handle);

            iStoreCTUploadFileToHost_ForDirectoryFile=0;
            iTotalCountUploadFileToHost_ForDirectoryFile=TotalFileSize/8000;
            if((TotalFileSize%8000)!=0)
                iTotalCountUploadFileToHost_ForDirectoryFile++;

            FilePUploadFileToHost_ForDirectoryFile=fopen(S.c_str(), "rb");     //2014/02/01 lee   // ASEM
            if(FilePUploadFileToHost_ForDirectoryFile==NULL)                   //2014/02/01 lee   // ASEM
            {
                Task=1;
                break;
            }
            Task=200;
            break;
        case 200:
            iStoreCTUploadFileToHost_ForDirectoryFile++;
            if(TotalFileSize>8000)
            {
                iReadSize=8000;
                Task=300;
            }
            else
            {
                iReadSize=TotalFileSize;
                iCT++;
                Task=400;
            }
            if(iReadSize!=0)
                fread(Ptr, iReadSize, 1, FilePUploadFileToHost_ForDirectoryFile);           //2014/02/01 lee   // ASEM
            TotalFileSize-=iReadSize;
            if(TotalFileSize==0)
                  fclose(FilePUploadFileToHost_ForDirectoryFile);               //2014/02/01 lee   // ASEM
            bDisableBinaryShow=true;

            InitLocalHead(101,7,1);
            DataItemOut(5, HType.LIST_TYPE, NULL);

            DataItemOut(HType.ASCII_TYPE, PathNameUploadFileToHost_ForDirectoryFile);       // 1.Directory
            DataItemOut(HType.ASCII_TYPE, FileNameUploadFileToHost_ForDirectoryFile);       // 1.filename
            DataItemOut(1, HType.INT_4_TYPE, &iStoreCTUploadFileToHost_ForDirectoryFile);   // 2.iStoreCT
            DataItemOut(1, HType.INT_4_TYPE, &iTotalCountUploadFileToHost_ForDirectoryFile);// 3.iTotalCount
            DataItemOut(iReadSize, HType.BINARY_TYPE, Ptr);                                 // 4.data body
            bReceiveS101F8=false;
            SendLocalData();
            bDisableBinaryShow=false;
            break;
        case 300:
            if(bReceiveS101F8==true)
            {
                Task=200;
            }
            break;
        case 400:
            if(bReceiveS101F8==true)
            {
                Task=140;
            }
            break;
    }
}
//------------------------------------------------------------------------------
// V 1.0
// ¹ï »P remote ³s½u«á¸ê®Æ¦^À³[»Ý­n§Q¥Î¤£¦P S,F code ªº³¡¤À]
//------------------------------------------------------------------------------
void THGem::DoUploadFileToHost()
{
    if(SV_70_UNT1_ReceipeStruct==0)
        DoUploadFileToHost_ForSingleFile();
    else if(SV_70_UNT1_ReceipeStruct==1)
        DoUploadFileToHost_ForMultiFile();
    else if(SV_70_UNT1_ReceipeStruct==2)
        DoUploadFileToHost_ForDirectoryFile();
}
//------------------------------------------------------------------------------
// V 1.0
// ¹ï S,F check respone
//------------------------------------------------------------------------------
void __fastcall THGem::DoProcessSFNoResponse()
{
    int iT;
    WORD &iOldSec=iOldSecProcessSFNoResponse;
    bool bClear=false;
    AnsiString S;

    if(iOldSec!=SystemSec)
    {
        iOldSec=SystemSec;
        do
        {
            bClear=false;
            if(TimeLeft->Count!=SFCodeResponseList->Items->Count)
            {
                // Joseph 20221111 (Jason) ·s¼W TCriticalSection S //
                //==>
                csSFCodeResponse->Acquire();
                try
                {
                    SFCodeResponseList->Clear();
                    TimeLeft->Clear();
                }
                __finally
                {
                    csSFCodeResponse->Release();
                }

                //SFCodeResponseList->Clear();
                //TimeLeft->Clear();
                //<==
                // Joseph 20221111 (Jason) ·s¼W TCriticalSection E //
            }
            for(int i=0; i<TimeLeft->Count; i++)
            {
                S=TimeLeft->Strings[i];
                iT=atoi(S.c_str());
                iT--;
                if(iT<=0)
                {
                    S=SFCodeResponseList->Items->Strings[i];
                    S+="    T3   time out";
                    StringOut(S);

                    // Joseph 20221111 (Jason) ·s¼W TCriticalSection S //
                    //==>
                    csSFCodeResponse->Acquire();
                    try
                    {
                        SFCodeResponseList->Items->Delete(i);
                        TimeLeft->Delete(i);
                    }
                    __finally
                    {
                        csSFCodeResponse->Release();
                    }

                    //SFCodeResponseList->Items->Delete(i);
                    //TimeLeft->Delete(i);
                    //<==
                    // Joseph 20221111 (Jason) ·s¼W TCriticalSection E //

                    bClear=true;
                    break;
                }
                else
                {
                    // Joseph 20221111 (Jason) ·s¼W TCriticalSection S //
                    //==>
                    csSFCodeResponse->Acquire();
                    try
                    {
                        TimeLeft->Strings[i]=iT;
                    }
                    __finally
                    {
                        csSFCodeResponse->Release();
                    }

                    //TimeLeft->Strings[i]=iT;
                    //<==
                    // Joseph 20221111 (Jason) ·s¼W TCriticalSection E //
                }
            }
            if(bClear==false)
                break;
        }while(1);
    }
}
//------------------------------------------------------------------------------
// V 1.0
// ¹ï »P remote ³s½u«á¸ê®Æ¦^À³[»Ý­n§Q¥Î¤£¦P S,F code ªº³¡¤À]
//------------------------------------------------------------------------------
void THGem::DoLocalAllProcessLoop()
{
//    DoReportECDataChangeCheck();  //Steven 20200807 : mark for¶XµÛSleepªº®É­Ô¥hÀË¬dEC change report

    if(bAutoConnect==false)
        iAutoConnectDelay=2;

    if(bConnect==false && bAutoConnect)
    {
        if(SystemSec%3==0)
        {
            if(iAutoConnectSec!=SystemSec)
            {
                if(iAutoConnectDelay>0)
                    iAutoConnectDelay--;
                if(iAutoConnectDelay<0)
                    iAutoConnectDelay=1;
                if(iAutoConnectDelay==0)
                {
                    iAutoConnectSec=SystemSec;
                    iStartConnectTask=1;
                    bStartConnect=true;
                }
            }
        }
    }
    if(bConnect==true)
        bAutoConnect=false;
    DoSpool();
    for(int i=0; i<10; i++)
        DoTraceDataResponse(i);
    if(bSeprate==true)
    {
        if(bWaitEstablishCommunicationsResponse==false)
            bSeprate=false;
        else
            bSeprate=false;
    }
    DoUploadFileToHost();                                                       // need debug
    DoProcessSFNoResponse();
    DoDownLoadRemoteFile();
}
//------------------------------------------------------------------------------
// V 1.0
// update application screen online,offline,connect status and son on
//------------------------------------------------------------------------------
HTimer SECSGEM_DoSeparate;                                                      //Ifor 20180913 (Steven) : add SECS GEM ¥¼³s½u°e¥XÂ_½u©R¥O
void THGem::DoUpdateStatus()
{
    int &ct=ctUpdateStatus;

    AnsiString S;
    static int iControlState=0;
    static bool bHasDisconnection=false;                                        //Ifor 20180913 (Steven) : add KYEC SECS GEM Connectionª¬ºA¬°OffLine°ÝÃD®É¡Aµ¥«Ý30¬í«á°e¥XÂ_½u©R¥O«áµ¥«Ý¤W¼h¨t²Î­«·s³s½u
    static unsigned char OldGemControlState=1;                                  //JerryYang 20230204 : SECS/GEM GControl StateÂà´«§¹¦¨¦Aµo³øEvent report
//    TStringList *P;
//    int i;

    ct++;
    if(ct<10)
        return;
    ct=0;
    SECSCommunicationMode=ComboBox1->ItemIndex;

    if(SECSConnectionState!=NULL)
    {
        if(bUseClientSocket==true)
        {
            if(flag1UpdateStatus || bClientSocketActive!=clientGem->Active)
            {
                bClientSocketActive=clientGem->Active;
                if(bClientSocketActive==true)
                {
                    SECSConnectionState->Caption="SECS GEM Connection";
                    SECSConnectionState->Color=clLime;
                    bConnect=false;
                }
                else
                {
                    SECSConnectionState->Caption="SECS GEM Disconnection";
                    SECSConnectionState->Color=clRed;
                }
            }
            flag1UpdateStatus=false;
        }
        else
        {
            if(iServoConnectCT!=srvGem->Socket->ActiveConnections)
            {
                iServoConnectCT=srvGem->Socket->ActiveConnections;
                if(iServoConnectCT==0)
                {
                    SECSConnectionState->Caption="SECS GEM Disconnection";
                    SECSConnectionState->Color=clRed;
                    bConnect=false;

                }
                else
                {
                    SECSConnectionState->Caption="SECS GEM Connection";
                    SECSConnectionState->Color=clLime;

                }
            }
        }
        //Ifor 20180913 (Steven) : add KYEC SECS GEM Connectionª¬ºA¬°OffLine°ÝÃD®É¡Aµ¥«Ý30¬í«á°e¥XÂ_½u©R¥O«áµ¥«Ý¤W¼h¨t²Î­«·s³s½u
        //==>
        if(CUSTOMER_CODE==CC_KYEC_LEE                               &&
           SECSConnectionState->Caption == "SECS GEM Connection"    &&
           GEMCommunicatingState->Caption == "1:OffLine"            )
        {
            if(bHasDisconnection==true && bSECSGEM_DoSeparate==false)
            {
                bHasDisconnection=false;
                SECSGEM_DoSeparate.SetSecAndOn(30);
            }

            if(SECSGEM_DoSeparate.Off())
            {
                DoSeparate();
                try
                {
                    HGem->srvGem->Close();
                    HGem->clientGem->Close();
                }
                catch(...)
                {
                    MyDBIProcess("Exception", "THGem::DoUpdateStatus");
                }
                bSECSGEM_DoSeparate=true;
                bHasDisconnection=true;
            }
        }
        else
        {
            bHasDisconnection=true;
        }
        //<==
        //Ifor 20180913 (Steven) : add KYEC SECS GEM Connectionª¬ºA¬°OffLine°ÝÃD®É¡Aµ¥«Ý30¬í«á°e¥XÂ_½u©R¥O«áµ¥«Ý¤W¼h¨t²Î­«·s³s½u
    }

    if(GEMCommunicatingState!=NULL)
    {
        if(bOldConnect!=bConnect || flag2UpdateStatus)
        {
            bOldConnect=bConnect;
            if(bConnect==true)
            {
                if(CUSTOMER_CODE==CC_KYEC_LEE ||                                //Ifor 20170531 (wei) KYEC ³ì´¼­n¨DÅã¥Ü³s½uª¬ºA¥H§Q¤H­û¤À¿ë
                   CUSTOMER_CODE==CC_SIGURD_ChungXing)                          //Sam 20250606 : ª¿®æ¤¤¿³¤]­nÅÜ§óSECSGEMª¬ºAÅã¥Ü
                {
                    GEMCommunicatingState->Caption="4:OnLine";
                }
                else
                {
                    GEMCommunicatingState->Caption="4:Enable";
                }
                GEMCommunicatingState->Color=clLime;
                bSECSGEMbyPass=false;                                           //wei 20150817 SECSGEM Â_½u¿é¤J±K½X
                bSECSGEMConnectionFail=false;                                   //wei 20150817 SECSGEM Â_½u
            }
            else
            {
                if(CUSTOMER_CODE==CC_KYEC_LEE ||                                //Ifor 20170531 (wei) KYEC ³ì´¼­n¨DÅã¥Ü³s½uª¬ºA¥H§Q¤H­û¤À¿ë
                   CUSTOMER_CODE==CC_SIGURD_ChungXing)                          //Sam 20250606 : ª¿®æ¤¤¿³¤]­nÅÜ§óSECSGEMª¬ºAÅã¥Ü
                {
                    GEMCommunicatingState->Caption="1:OffLine";
                }
                else
                {
                    GEMCommunicatingState->Caption="1:Disable";
                }
                GEMCommunicatingState->Color=clRed;
                bSECSGEMConnectionFail=true;                                    //wei 20150817 SECSGEM Â_½u
                bHasDisconnection=true;
            }
        }
        flag2UpdateStatus=false;
    }

    if(GemPanelControlState!=NULL)
    {
        if(bOnLine)
        {
            if(bOnLineLocal)
                S="On Line Local";
            else
                S="On Line Remote";
        }
        else
        {
            S="Off Line";
        }
        if(flag3UpdateStatus || S!=OldSUpdateStatus)
        {
            OldSUpdateStatus=S;
            flag3UpdateStatus=false;
            GemPanelControlState->Caption=S;
            if(S=="On Line Local")
                GemPanelControlState->Color=clLime;
            else if(S=="On Line Remote")
                GemPanelControlState->Color=clYellow;
            else
                GemPanelControlState->Color=clRed;
        }
    }

    if(IsOnLine())
    {
        GemBtnOfflineRequest->Enabled=true;
        GemBtnOnlineRequest->Enabled=false;
    }
    else
    {
        GemBtnOfflineRequest->Enabled=false;
        GemBtnOnlineRequest->Enabled=true;
    }
    if(GetOnLineMode())
    {
        GemBtnOnlineRemote->Enabled=true;
        GemBtnOnlineLocal->Enabled=false;
    }
    else
    {
        GemBtnOnlineRemote->Enabled=false;
        GemBtnOnlineLocal->Enabled=true;
    }
    //Ifor 20170803 (wei) Mark Á×§KÂÂ¹q¸£¨t²Î©µ¿ð³y¦¨UPH¤U­°
    //==>
//    if(bOutputBusy==false)
//    {
//        while(DB->Lines->Count>3000)
//        {
//            P=new TStringList;
//            for(i=(DB->Lines->Count-1000);i<DB->Lines->Count; i++)
//                P->Add(DB->Lines->Strings[i]);
//            DB->Clear();
//            for(i=0; i<P->Count; i++)
//                DB->Lines->Add(P->Strings[i]);
//            delete P;
//        }
//    }
    //<==
    //Ifor 20170803 (wei) Mark Á×§KÂÂ¹q¸£¨t²Î©µ¿ð³y¦¨UPH¤U­°
    if(bConnectUpdateStatus!=IsConnect())
    {
        bConnectUpdateStatus=IsConnect();
        BtnEnableComm->Enabled=!IsConnect();
    }

    if(bOnLine==false)
    {
        GemControlState=1;
    }
    else
    {
        if(bOnLineLocal==true)
            GemControlState=2;
        else
            GemControlState=3;
    }

    if(iControlState!=GemControlState)
    {
        GemControlPreState=iControlState;
        iControlState=GemControlState;
        EventReport(1, 141);  //Ifor 20221018 add:GEM Control State Change Report
    }

    if(OldGemControlState!=GemControlState) //JerryYang 20230204 : SECS/GEM GControl StateÂà´«§¹¦¨¦Aµo³øEvent report
    {
        OldGemControlState=GemControlState;
        if(GemControlState==1)
        {
            EventReport(1, 91);     //Offline
        }
        else if(GemControlState==2)
        {
            EventReport(1, 92);     //Online local
        }
        else if(GemControlState==3)
        {
            EventReport(1, 93);     //Online remote
        }
    }
}
//------------------------------------------------------------------------------
//2013/11/20  lee start
void __fastcall THGem::ClearDefaultEvenReport()
{
    int y;
    unsigned iReportID;
    bool bSearchOK;
    do
    {
        bSearchOK=false;
        for(y=1; y<stdGridReportID->RowCount; y++)
        {
            if(stdGridReportID->Cells[1][y]=="1")
            {
                iReportID=atoi(stdGridReportID->Cells[0][y].c_str());
                DeleteReportID(iReportID, 1);
                DeleteReportIDOfCeid(iReportID);
                bSearchOK=true;
                break;
            }
        }
    }while(bSearchOK==true) ;
}
//2013/11/20  lee end
//------------------------------------------------------------------------------
void __fastcall THGem::InitialHGem()
{
    bool bRead=true;
    bool bCompare=true;
    AnsiString sPath=GemSystemIniPath;

    if(EnableOrDisablePtr!=NULL)
    {
        EnableOrDisablePtr->ItemIndex=ReadWriteIni(sPath, "GEM", "EnableOrDisable", 0, 0, bRead, bCompare, 0, EnableOrDisablePtr->Items->Count);
    }

    if(OnLineOrOffLine!=NULL)
    {
        OnLineOrOffLine->ItemIndex=ReadWriteIni(sPath, "GEM", "OnLineOrOffLine", 0, 0, bRead, bCompare, 0, OnLineOrOffLine->Items->Count);
    }

    if(RemoteOrLocal!=NULL)
    {
        RemoteOrLocal->ItemIndex=ReadWriteIni(sPath, "GEM", "RemoteOrLocal", 0, 0, bRead, bCompare, 0, RemoteOrLocal->Items->Count);
    }

    rgRole->ItemIndex=ReadWriteIni(sPath, "GEM", "ActiveOrPassive", 0, 0, bRead, bCompare, 0, rgRole->Items->Count);
    bUseClientSocket=(rgRole->ItemIndex==1);

    edtIP->Text         =ReadIniData(sPath, "GEM", "Address", DefaultAddress);
    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //wei 20160309 SecsGem Port ±j¨î6000
        edtPort->Text   =6000;
    else
        edtPort->Text   =ReadIniData(sPath, "GEM", "Port", DefaultPort);

    edDeviceID->Text    =ReadIniData(sPath, "GEM", "DeviceID", DefaultDeviceID);
    edtT3TimeOut->Text  =ReadIniData(sPath, "GEM", "T3", T3TimeOut);            //pig 2014.07.28 KYEC_SECS start
    edtT5TimeOut->Text  =ReadIniData(sPath, "GEM", "T5", T5TimeOut);
    edtT6TimeOut->Text  =ReadIniData(sPath, "GEM", "T6", T6TimeOut);
    edtT7TimeOut->Text  =ReadIniData(sPath, "GEM", "T7", T7TimeOut);
    edtT8TimeOut->Text  =ReadIniData(sPath, "GEM", "T8", T8TimeOut);

    T3TimeOut=atoi(edtT3TimeOut->Text.c_str());
    T5TimeOut=atoi(edtT5TimeOut->Text.c_str());
    T6TimeOut=atoi(edtT6TimeOut->Text.c_str());
    T7TimeOut=atoi(edtT7TimeOut->Text.c_str());
    T8TimeOut=atoi(edtT8TimeOut->Text.c_str());

    clientGem->Address=edtIP->Text;
    clientGem->Port=0;
    srvGem->Port=0;
    Local.DeviceID=atoi(edDeviceID->Text.c_str());
    if(bUseClientSocket==true)
    {
        clientGem->Port=atoi(edtPort->Text.c_str());
        edtIP->Enabled=true;
    }
    else
    {
        srvGem->Port=atoi(edtPort->Text.c_str());
        edtIP->Enabled=false;
    }

    GemCheckBoxAcceptHostOnlineRequest->Checked =ReadIniData(sPath, "GEM","AcceptHostOnlineRequest",true);
    GemCheckBoxShowBinary->Checked              =false;                         //Steven 20211109 : SECS¤£­n¨C¦¸³£Åã¥Übinary code
    GemCheckBoxShowHeadInformation->Checked     =ReadIniData(sPath, "GEM", "ShowHeadInformation", false);
    GemCheckBoxUseExtendedAlarm->Checked        =ReadIniData(sPath, "GEM", "UseExtendedAlarm", false);
    chkAnnotatedEventReport->Checked            =ReadIniData(sPath, "GEM", "AnnotatedEventReport", false);
    chkMoreMessageAbortProcess->Checked         =ReadIniData(sPath, "GEM", "CheckMoreData", false);
    if(CUSTOMER_CODE==CC_MAXIM_THAILAND)                                        //Ifor 20251018 add:Analog ®õ°ê«È¤á­n¨DAddDefaultReport»Ý¥i³]©w
    {
        ckAddDefaultReport->Checked                 =ReadIniData(sPath, "GEM", "AddDefaultReport", true);          //2013/11/20  lee
    }
    else
    {
        ckAddDefaultReport->Checked=true;                                           //2014/01/01  lee
    }
    if(CosFunction.bECChangeEventReportCanOnOff)                                //JerryYang 20200520 ÂÂ¹q¸£°µDoReportECDataChangeCheck¨ç¦¡·|¼vÅTUPH,§ï¦¨¥\¯à¿ï¶µ
    {
        cbECChaneEventReport->Checked           =ReadIniData(sPath, "GEM", "ECChangeEventReport", false);
    }
    else
    {
        cbECChaneEventReport->Checked           =true;
    }

    ReadEventReportData();

    if(ckAddDefaultReport->Checked==false)
        ClearDefaultEvenReport();
}
//------------------------------------------------------------------------------
void __fastcall THGem::SaveSystemDefault()
{
    AnsiString sPath=GemSystemIniPath;
    if(EnableOrDisablePtr!=NULL)
    {
        WriteIniData(sPath, "GEM", "EnableOrDisable", EnableOrDisablePtr->ItemIndex);
    }

    if(OnLineOrOffLine!=NULL)
    {
        WriteIniData(sPath, "GEM", "OnLineOrOffLine", OnLineOrOffLine->ItemIndex);
    }

    if(RemoteOrLocal!=NULL)
    {
        WriteIniData(sPath, "GEM", "RemoteOrLocal", RemoteOrLocal->ItemIndex);
    }

    WriteIniData(sPath, "GEM", "ActiveOrPassive",   rgRole->ItemIndex);
    WriteIniData(sPath, "GEM", "Address",           edtIP->Text);
    WriteIniData(sPath, "GEM", "Port",              edtPort->Text);
    WriteIniData(sPath, "GEM", "DeviceID",          edDeviceID->Text);

    WriteIniData(sPath, "GEM", "T3",                edtT3TimeOut->Text);        //pig 2014.07.28 KYEC_SECS start
    WriteIniData(sPath, "GEM", "T5",                edtT5TimeOut->Text);
    WriteIniData(sPath, "GEM", "T6",                edtT6TimeOut->Text);
    WriteIniData(sPath, "GEM", "T7",                edtT7TimeOut->Text);
    WriteIniData(sPath, "GEM", "T8",                edtT8TimeOut->Text);

    WriteIniData(sPath, "GEM", "AcceptHostOnlineRequest",   GemCheckBoxAcceptHostOnlineRequest->Checked);
    WriteIniData(sPath, "GEM", "ShowBinaryData",            GemCheckBoxShowBinary->Checked);
    WriteIniData(sPath, "GEM", "ShowHeadInformation",       GemCheckBoxShowHeadInformation->Checked);
    WriteIniData(sPath, "GEM", "UseExtendedAlarm",          GemCheckBoxUseExtendedAlarm->Checked);
    WriteIniData(sPath, "GEM", "AnnotatedEventReport",      chkAnnotatedEventReport->Checked);
    WriteIniData(sPath, "GEM", "CheckMoreData",             chkMoreMessageAbortProcess->Checked);
    WriteIniData(sPath, "GEM", "AddDefaultReport",          ckAddDefaultReport->Checked);       //2013/11/20  lee
    WriteIniData(sPath, "GEM", "ECChangeEventReport",       cbECChaneEventReport->Checked);     //JerryYang 20200520 ÂÂ¹q¸£°µDoReportECDataChangeCheck¨ç¦¡·|¼vÅTUPH,§ï¦¨¥\¯à¿ï¶µ
}
//=============================================================================
// =   V 1.0                                                                   =
// =   ¹ï Host ªº­n¨D°µ Polling ³B²z                                           =
// =                                                                           =
//=============================================================================
bool THGem::CheckSocketActiveFalse()
{
    if(bUseClientSocket==true)
    {
        if(clientGem->Active==false)
            return true;
    }
    else
    {
        if(srvGem->Active==false)
            return true;
    }
    return false;
}
// =============================================================================
// =   V 1.0                                                                   =
// =   Åã¥Ü±µ¦¬©Î¶Ç°e¸ê®Æ                                                      =
// =                                                                           =
// =============================================================================
void __fastcall THGem::ProcessShow()
{
    if(HGem->WaitShowString->Count!=0)
    {
        HGem->DB->Clear();
        HGem->DB->Lines->Assign(HGem->WaitShowString);
        HGem->WaitShowString->Clear();
    }
}
//------------------------------------------------------------------------------
HTimer SECSGEM_DoSeparateWait;                                                  //Ifor 20180913 (Steven) :add °e¥XSECS GEM Â_½u©R¥O«á»Ýµ¥«ÝÂ_½u§¹¦¨
void __fastcall THGem::Timer1Timer(TObject *Sender)
{
    static bool bSendDoSeparate=false;
    static bool bSetDoSeparateWait=true;                                        //Ifor 20180913 (Steven) :add ³]©w©µ¿ð®É¶¡
    static bool bWaitHTimer=false;                                              //Ifor 20180913 (Steven) :add µ¥«Ý©µ¿ð®É¶¡
    static bool bTimerRunning=false;
    if(InitialOK==false || bTimerRunning==true)
    {
        return;
    }
    bTimerRunning=true;

    if(IniConfig.bEnable_SECS_GEM==true)                                        //Ifor 20170425 add Ãö³¬ SECS GEM Function »Ý¥ý¤UÂ_½u©R¥O
    {
        bSendDoSeparate=false;
    }
    else
    {
        if(srvGem->Socket->ActiveConnections!=0)
        {
            if(bSendDoSeparate==false)
            {
                bSendDoSeparate=true;
                DoSeparate();
                try
                {
                    HGem->srvGem->Close();
                    HGem->clientGem->Close();
                }
                catch(...)
                {
                    MyDBIProcess("Exception", "THGem::Timer1Timer");
                }
                bSECSGEM_DoSeparate=true;
                bSetDoSeparateWait=true;
            }
        }
        bTimerRunning=false;
        return;
    }

    if(bSetDoSeparateWait && bSECSGEM_DoSeparate)                               //Ifor 20180913 (Steven) : add SECS GEM Â_½u«á»Ý©µ¿ð¶}±ÒÁ×§K³y¦¨µLªk³s½u°ÝÃD
    {
        bSetDoSeparateWait=false;
        SECSGEM_DoSeparateWait.SetSecAndOn(5);
        bWaitHTimer=true;
    }

    if(bWaitHTimer)
    {
        if(SECSGEM_DoSeparateWait.Off())
        {
            bSetDoSeparateWait=true;
            bSECSGEM_DoSeparate=false;
            bWaitHTimer=false;
        }
        else
        {
            bTimerRunning=false;
            return;
        }
    }

    int &Task=Timer1Task;
    int &ct=Timer1ct;
    int ret;
    if(bFirstEntry==true)
    {
        InitialHGem();
        bFirstEntry=false;
    }
    ProcessShow();
    ProcessSocketReceiveData();
    GetTimeInfo();
    DoUpdateStatus();
    Caption=Alias;
    switch(Task)
    {
        case 1:
            DelayForServoError.TimerSet(100);
            DelayForServoError.TimerOn();
            Task=10;
            break;
        case 10:
            if(DoOpenCommuncation())
            {
                if(EnableOrDisablePtr!=NULL)
                {
                    if(EnableOrDisablePtr->ItemIndex==0)
                        Connect();
                }
                Task=100;
            }
            if(bReceiveMultiConnect==true && DelayForServoError.TimerOff())
            {
                try
                {
                    if(bUseClientSocket==false)
                        srvGem->Close();
                }
                catch(...)
                {
                    MyDBIProcess("Exception", "THGem::Timer1Timer");
                }
                bReceiveMultiConnect=false;
                iOpenCommuncationTask=1;
                Task=1;
                StringOut("¦³2¥x¥H¤WEAP³s±µHandler,½Ð½T»{");                    //13.09.30.01   klutter
            }
            break;
        case 100:
            DelayForServoError.TimerSet(100);
            DelayForServoError.TimerOn();
            Task=110;
            break;
        case 110:
            if(CheckSocketActiveFalse())
            {
                iOpenCommuncationTask=1;
                Task=1;
                break;
            }

            if(bUseClientSocket && (bStartConnect==true || bAutoConnect==true))
            {
                bStartConnect=false;
                iStartConnectTask=1;
                Task=200;
            }
            else if(bReceiveEstablishCommunicationsRequest==true)
            {
                bReceiveEstablishCommunicationsRequest=false;
                bConnect=true;
                if(bOnLine)
                {
                    iStartOnLineTask=1;
                    Task=300;
                }
                else
                {
                    Task=250;
                }
            }

            if(bReceiveMultiConnect==true && DelayForServoError.TimerOff())
            {
                try
                {
                    if(bUseClientSocket==false)
                        srvGem->Close();
                }
                catch(...)
                {
                    MyDBIProcess("Exception", "THGem::Timer1Timer");
                }
                bReceiveMultiConnect=false;
                iOpenCommuncationTask=1;
                Task=1;
                StringOut("¦³2¥x¥H¤WEAP³s±µHandler,½Ð½T»{");                    //13.09.30.01   klutter
            }
            break;
        case 200:
            if(CheckSocketActiveFalse())
            {
                Task=1;
                break;
            }
            if(bReceiveEstablishCommunicationsRequest==true)
            {
                bReceiveEstablishCommunicationsRequest=false;
                bConnect=true;
                if(bOnLine)
                {
                    iStartOnLineTask=1;
                    Task=300;
                }
                else
                {
                    Task=250;
                }
                break;
            }

            ret=DoConnect();
            if(ret==1)
            {
                if(OnLineOrOffLine!=NULL)
                {
                    if(OnLineOrOffLine->ItemIndex==0)
                        OnLine(0);
                    else
                        OffLine();
                }
                if(bOnLine)
                {
                    iStartOnLineTask=1;
                    Task=300;
                }
                else
                {
                    Task=250;
                }
            }
            else if(ret==2)
                Task=100;
            break;
        case 250:
            DelayForServoError.TimerSet(100);
            DelayForServoError.TimerOn();
            Task=260;
            break;
        case 260:
            if(CheckSocketActiveFalse())
            {
                Task=1;
                break;
            }
            else if(bConnect==false)
            {
                Task=100;
                break;
            }
            if(bOnLine)
            {
                iStartOnLineTask=1;
                Task=300;
            }
            else if(bStartOnLine==true)
            {
                Task=300;
            }
            if(bReceiveMultiConnect==true && DelayForServoError.TimerOff())
            {
                try
                {
                    if(bUseClientSocket==false)
                        srvGem->Close();
                }
                catch(...)
                {
                    MyDBIProcess("Exception", "THGem::Timer1Timer");
                }
                bReceiveMultiConnect=false;
                iOpenCommuncationTask=1;
                Task=1;
                StringOut("¦³2¥x¥H¤WEAP³s±µHandler,½Ð½T»{");                    //13.09.30.01   klutter
            }
            break;
        case 300:
            DelayForServoError.TimerSet(100);
            DelayForServoError.TimerOn();
            Task=310;
            break;
        case 310:
            bReceiveEstablishCommunicationsRequest=false;
            if(CheckSocketActiveFalse())
            {
                Task=1;
                break;
            }

            if(DoOnLine()==true)
            {
                Task=400;
            }
            if(bReceiveMultiConnect==true && DelayForServoError.TimerOff())
            {
                try
                {
                    if(bUseClientSocket==false)
                        srvGem->Close();
                }
                catch(...)
                {
                    MyDBIProcess("Exception", "THGem::Timer1Timer");
                }
                bReceiveMultiConnect=false;
                iOpenCommuncationTask=1;
                Task=1;
                StringOut("¦³2¥x¥H¤WEAP³s±µHandler,½Ð½T»{");                    //13.09.30.01   klutter
            }
            break;
        case 350:
            if(RemoteOrLocal->ItemIndex==0)
                OnLineRemote();
            else
                OnLineLocal();
            Task=360;
            ct=0;
            break;
        case 360:
            ct++;
            if(ct>30)
                Task=400;
            break;
        case 400:
            DelayForServoError.TimerSet(100);
            DelayForServoError.TimerOn();
            Task=410;
            break;
        case 410:
            bReceiveEstablishCommunicationsRequest=false;
            if(CheckSocketActiveFalse())
            {
                Task=1;
                break;
            }

            if(bCloseCommuncation==true)
            {
                bCloseCommuncation=false;
                Task=1;
            }
            else if(bConnect==false)
            {
                Task=100;
            }
            else if(bOnLine==false)
            {
                iStartOnLineTask=1;
                Task=250;
            }
            else if(bReceiveMultiConnect==true && DelayForServoError.TimerOff())
            {
                try
                {
                    if(bUseClientSocket==false)
                        srvGem->Close();
                }
                catch(...)
                {
                    MyDBIProcess("Exception", "THGem::Timer1Timer");
                }
                bReceiveMultiConnect=false;
                iOpenCommuncationTask=1;
                Task=1;
                StringOut("¦³2¥x¥H¤WEAP³s±µHandler,½Ð½T»{");                    //13.09.30.01   klutter
            }
            else
            {
                DoLocalAllProcessLoop();
            }
            break;
    }
    bTimerRunning=false;
}
//==============================================================================
// V 1.0
// for Application call for TCP/IP port initial
//==============================================================================
//void THGem::OpenCommuncation(AnsiString Address, int Port)
//{
//    bOpenCommuncation=true;
//}
//==============================================================================
// V 1.0
// for Application call for TCP/IP port initial
//==============================================================================
//void THGem::OpenCommuncation(int Port)
//{
//    bOpenCommuncation=true;
//}
//==============================================================================
// V 1.0
// for Application call for disable TCP/IP port
//==============================================================================
void THGem::CloseCommuncation()
{
    bCloseCommuncation=true;
    clientGem->Active=false;
}
//==============================================================================
// V 1.0
// for Application call for connect with remore
//==============================================================================
void THGem::Connect()
{
    if(bConnect==true)
        return;
    bStartConnect=true;
    bAutoConnect=true;
}
//==============================================================================
// V 1.0
//==============================================================================
void THGem::DisConnect()
{
    bConnect=false;
    bStartConnect=false;
    bAutoConnect=false;
}
//==============================================================================
// V 1.0
//==============================================================================
bool THGem::IsConnect()
{
    return bConnect;
}
//==============================================================================
// V 1.0
//==============================================================================
void THGem::SetEstablishCommunicationsTryCount(int ct)
{
    iEstablishCommunicationsTryCount=ct;
}
//==============================================================================
// V 1.0
// Mode=true   OnLineLocal
// Mode=false  OnLineRemote
//==============================================================================
void THGem::OnLine(bool Mode)
{
    bOnLineLocal=Mode;
    iStartOnLineTask=1;
    bStartOnLine=true;
    bOnLine=false;
}
//==============================================================================
// V 1.0
//==============================================================================
void THGem::OnLineLocal()
{
    bOnLineLocal=true;
    OnlineLocalOrRemote();
}
//==============================================================================
// V 1.0
//==============================================================================
void THGem::OnLineRemote()
{
    bOnLineLocal=false;
    OnlineLocalOrRemote();
}
//==============================================================================
// V 1.1
// 2013/04/02 Lee
//==============================================================================
void THGem::OffLine()
{
    bOnLine=false;
    bStartOnLine=false;
//    EventReport(1, 91);                                                       //pig 20170323 2DBarcode SECS/GEM  //JerryYang 20230204 : SECS/GEM GControl StateÂà´«§¹¦¨¦Aµo³øEvent report
}
//==============================================================================
// V 1.0
//==============================================================================
bool THGem::IsOnLine()
{
    return bOnLine;
}
//==============================================================================
// V 1.0
//==============================================================================
bool THGem::GetOnLineMode()
{
    return bOnLineLocal;
}
//==============================================================================
// V 1.0
//==============================================================================
void THGem::SetCanAcceptHostOnLineRequest(bool flag)
{
}
//==============================================================================
// V 1.0
//==============================================================================
void THGem::SetSVDataPointer(AnsiString SVID, unsigned char Type, AnsiString SVName, AnsiString SVUnit, void *P, AnsiString Remark)
{
    AnsiString S;
    if(SV_ID->IndexOf(SVID)>=0)
    {
        S="SVID:"+SVID+" duplicate error !";
        ShowMessage(S);
        return;
    }
    SV_ID->Add(SVID);
    SV_TYPE->Add(Type);
    SV_NAME->Add(SVName);
    SV_UNIT->Add(SVUnit);
    SV_Ptr->Add(P);
    VCL_NAME->Add(0);
    SV_LEN->Add(1);
    SV_Remark->Add(Remark);
}
//==============================================================================
// V 1.0
//==============================================================================
void THGem::SetSVDataPointer(AnsiString SVID, unsigned char Type, AnsiString SVName, AnsiString SVUnit, AnsiString *P, AnsiString Remark)
{
    AnsiString S;
    if(SV_ID->IndexOf(SVID)>=0)
    {
        S="SVID:"+SVID+" duplicate error !";
        ShowMessage(S);
        return;
    }
    SV_ID->Add(SVID);
    SV_TYPE->Add(Type);
    SV_NAME->Add(SVName);
    SV_UNIT->Add(SVUnit);
    SV_Ptr->Add(P);
    VCL_NAME->Add(2);
    SV_LEN->Add(1);
    SV_Remark->Add(Remark);
}
//==============================================================================
// V 1.0
//==============================================================================
void THGem::SetSVDataPointer(AnsiString SVID, unsigned char Type, AnsiString SVName, AnsiString SVUnit, void *P, int Len, AnsiString Remark)
{
    AnsiString S;
    if(SV_ID->IndexOf(SVID)>=0)
    {
        S="SVID:"+SVID+" duplicate error !";
        ShowMessage(S);
        return;
    }

    SV_ID->Add(SVID);
    SV_TYPE->Add(Type);
    SV_NAME->Add(SVName);
    SV_UNIT->Add(SVUnit);
    SV_Ptr->Add(P);
    VCL_NAME->Add(0);
    SV_LEN->Add(Len);
    SV_Remark->Add(Remark);
}
//==============================================================================
// V 1.0
//==============================================================================
void THGem::SetSVDataPointer(AnsiString SVID, unsigned char Type, AnsiString SVName, AnsiString SVUnit, TObject *P, AnsiString Remark)
{
    AnsiString S;
    if(SV_ID->IndexOf(SVID)>=0)
    {
        S="SVID:"+SVID+" duplicate error !";
        ShowMessage(S);
        return;
    }

    SV_ID->Add(SVID);
    SV_TYPE->Add(Type);
    SV_NAME->Add(SVName);
    SV_UNIT->Add(SVUnit);
    SV_Ptr->Add(P);
    VCL_NAME->Add("1");
    SV_LEN->Add(1);
    SV_Remark->Add(Remark);
}
//------------------------------------------------------------------------------
void THGem::SetECDataPointer(AnsiString ECID, unsigned char Type, AnsiString ECName, AnsiString ECUnit, AnsiString *P, AnsiString MinValue, AnsiString MaxValue, AnsiString DefaultValue, AnsiString Remark)
{
    AnsiString S;
    if(EC_ID->IndexOf(ECID)>=0)
    {
        S="ECID:"+ECID+" duplicate error !";
        ShowMessage(S);
        return;
    }

    EC_ID->Add(ECID);
    EC_TYPE->Add(Type);
    EC_NAME->Add(ECName);
    EC_UNIT->Add(ECUnit);
    EC_Ptr->Add(P);

    EC_Ptr_Min->Add(NULL);
    EC_Ptr_Max->Add(NULL);
    EC_Ptr_Default->Add(NULL);

    if(atof(MinValue.c_str())>atof(MaxValue.c_str()))
    {
        EC_Ptr_Min_Value->Add(MaxValue);
        EC_Ptr_Max_Value->Add(MinValue);
    }
    else
    {
        EC_Ptr_Min_Value->Add(MinValue);
        EC_Ptr_Max_Value->Add(MaxValue);
    }
    EC_Ptr_Default_Value->Add(DefaultValue);

    S=GetECDataValue(ECID);
    EC_OldValue->Add(S);
    EC_VCL_NAME->Add("2");
    EC_Remark->Add(Remark);
    SetSVDataPointer(ECID, Type, ECName, ECUnit, P, Remark);
}
//------------------------------------------------------------------------------
void THGem::SetECDataPointer(AnsiString ECID, unsigned char Type, AnsiString ECName, AnsiString ECUnit, void *P, int MinValue, int MaxValue, int DefaultValue, AnsiString Remark)
{
    AnsiString S;
    if(EC_ID->IndexOf(ECID)>=0)
    {
        S="ECID:"+ECID+" duplicate error !";
        ShowMessage(S);
        return;
    }

    EC_ID->Add(ECID);
    EC_TYPE->Add(Type);
    EC_NAME->Add(ECName);
    EC_UNIT->Add(ECUnit);
    EC_Ptr->Add(P);

    EC_Ptr_Min->Add(NULL);
    EC_Ptr_Max->Add(NULL);
    EC_Ptr_Default->Add(NULL);

    if(MinValue>MaxValue)
    {
        EC_Ptr_Min_Value->Add(MaxValue);
        EC_Ptr_Max_Value->Add(MinValue);
    }
    else
    {
        EC_Ptr_Min_Value->Add(MinValue);
        EC_Ptr_Max_Value->Add(MaxValue);
    }
    EC_Ptr_Default_Value->Add(DefaultValue);

    S=GetECDataValue(ECID);
    EC_OldValue->Add(S);
    EC_VCL_NAME->Add("0");
    EC_Remark->Add(Remark);
    SetSVDataPointer(ECID, Type, ECName, ECUnit, P, Remark);
    SetECEnableData(ECID, ECName);      //pig 2014.08.27 KYEC
}
//------------------------------------------------------------------------------
void THGem::SetECDataPointer(AnsiString ECID, unsigned char Type, AnsiString ECName, AnsiString ECUnit, TObject *P, char *MinValue, char *MaxValue, char *DefaultValue, AnsiString Remark)
{
    AnsiString S;
    if(EC_ID->IndexOf(ECID)>=0)
    {
        S="ECID:"+ECID+" duplicate error !";
        ShowMessage(S);
        return;
    }

//    int  intMin,intMax,intDefault;

    if(Type==HType.UINT_1_TYPE)
    {
//        unsigned char *Min,*Max,*Default;
//        Min=new unsigned char ;
//        Max=new unsigned char ;
//        Default=new unsigned char ;
//
//        *Min     =atoi(MinValue);
//        *Max     =atoi(MaxValue);
//        *Default =atoi(DefaultValue);
    }
    else if(Type==HType.UINT_2_TYPE)
    {
//        unsigned short *Min,*Max,*Default;
//        Min=new unsigned short ;
//        Max=new unsigned short ;
//        Default=new unsigned short ;
//        *Min     =atoi(MinValue);
//        *Max     =atoi(MaxValue);
//        *Default =atoi(DefaultValue);
    }
    else if(Type==HType.UINT_4_TYPE)
    {
//        unsigned *Min,*Max,*Default;
//        Min=new unsigned;
//        Max=new unsigned;
//        Default=new unsigned;
//        *Min     =atoi(MinValue);
//        *Max     =atoi(MaxValue);
//        *Default =atoi(DefaultValue);
    }
    else if(Type==HType.UINT_8_TYPE)
    {
//        unsigned __int64 *Min,*Max,*Default;
//        Min=new unsigned __int64;
//        Max=new unsigned __int64;
//        Default=new unsigned __int64;
//        *Min     =atoi(MinValue);
//        *Max     =atoi(MaxValue);
//        *Default =atoi(DefaultValue);
    }
    else if(Type==HType.INT_1_TYPE || Type==HType.BINARY_TYPE)
    {
//        char *Min,*Max,*Default;
//        Min=new char ;
//        Max=new char ;
//        Default=new char ;
//        *Min     =atoi(MinValue);
//        *Max     =atoi(MaxValue);
//        *Default =atoi(DefaultValue);
    }
    else if(Type==HType.INT_2_TYPE)
    {
//        short *Min,*Max,*Default;
//        Min=new short ;
//        Max=new short ;
//        Default=new short ;
//        *Min     =atoi(MinValue);
//        *Max     =atoi(MaxValue);
//        *Default =atoi(DefaultValue);
    }
    else if(Type==HType.INT_4_TYPE)
    {
//        int  *Min,*Max,*Default;
//        Min=new int ;
//        Max=new int ;
//        Default=new int ;
//        *Min     =atoi(MinValue);
//        *Max     =atoi(MaxValue);
//        *Default =atoi(DefaultValue);
    }
    else if(Type==HType.INT_8_TYPE)
    {
//        __int64 *Min,*Max,*Default;
//        Min=new __int64;
//        Max=new __int64;
//        Default=new __int64;
//        *Min     =atoi(MinValue);
//        *Max     =atoi(MaxValue);
//        *Default =atoi(DefaultValue);
    }
    else if(Type==HType.ASCII_TYPE)
    {
//        int *Min,*Max,*Default;
//        Min=new int ;
//        Max=new int;
//        Default=new int;
//
//        *Max=atoi(MaxValue);
//        *Min=atoi(MinValue);
//        *Default=atoi(DefaultValue);
    }
    else if(Type==HType.BOOLEAN_TYPE)
    {
//        bool *Min,*Max,*Default;
//        Min=new bool;
//        Max=new bool;
//        Default=new bool;
//        *Min     =atoi(MinValue);
//        *Max     =atoi(MaxValue);
//        *Default =atoi(DefaultValue);
    }
    else if(Type==HType.FT_4_TYPE)                                              //Steven 20130730 ADD
    {
//        float *Min,*Max,*Default;
//        Min=new float;
//        Max=new float;
//        Default=new float;
//        *Min     =atof(MinValue);
//        *Max     =atof(MaxValue);
//        *Default =atof(DefaultValue);
    }
    else if(Type==HType.FT_8_TYPE)                                              //Steven 20130730 ADD
    {
//        double *Min,*Max,*Default;
//        Min=new double;
//        Max=new double;
//        Default=new double;
//        *Min     =atof(MinValue);
//        *Max     =atof(MaxValue);
//        *Default =atof(DefaultValue);
    }
    else
    {
        return;
    }

    EC_ID->Add(ECID);
    EC_TYPE->Add(Type);
    EC_NAME->Add(ECName);
    EC_UNIT->Add(ECUnit);
    EC_Ptr->Add(P);

    EC_Ptr_Min->Add(NULL);
    EC_Ptr_Max->Add(NULL);
    EC_Ptr_Default->Add(NULL);

    double *Min,*Max;
    Min=new double;
    Max=new double;
    *Min     =atof(MinValue);
    *Max     =atof(MaxValue);

    if(*Min>*Max)
    {
        EC_Ptr_Min_Value->Add(MaxValue);
        EC_Ptr_Max_Value->Add(MinValue);
    }
    else
    {
        EC_Ptr_Min_Value->Add(MinValue);
        EC_Ptr_Max_Value->Add(MaxValue);
    }
    EC_Ptr_Default_Value->Add(DefaultValue);

    S=GetECDataValue(ECID);
    EC_OldValue->Add(S);
    EC_VCL_NAME->Add("1");
    EC_Remark->Add(Remark);
    SetSVDataPointer(ECID, Type, ECName,ECUnit,P,Remark);
    SetECEnableData(ECID, ECName);                                              //pig 2014.08.27 KYEC

    delete Min;
    delete Max;
}
//------------------------------------------------------------------------------
void THGem::SetECDataPointer(AnsiString ECID, unsigned char Type, AnsiString ECName, AnsiString ECUnit, void *P, AnsiString Min, AnsiString Max, AnsiString Default, AnsiString Remark)
{
    AnsiString S;
    if(EC_ID->IndexOf(ECID)>=0)
    {
        S="ECID:"+ECID+" duplicate error !";
        ShowMessage(S);
        return;
    }
    EC_ID->Add(ECID);
    EC_TYPE->Add(Type);
    EC_NAME->Add(ECName);
    EC_UNIT->Add(ECUnit);
    EC_Ptr->Add(P);

    EC_Ptr_Min->Add(NULL);
    EC_Ptr_Max->Add(NULL);
    EC_Ptr_Default->Add(NULL);

    if(atof(Min.c_str())>atof(Max.c_str()))
    {
        EC_Ptr_Max_Value->Add(Min);
        EC_Ptr_Min_Value->Add(Max);
    }
    else
    {
        EC_Ptr_Min_Value->Add(Min);
        EC_Ptr_Max_Value->Add(Max);
    }
    EC_Ptr_Default_Value->Add(Default);

    S=GetECDataValue(ECID);
    EC_OldValue->Add(S);
    EC_VCL_NAME->Add("0");
    EC_Remark->Add(Remark);
    SetSVDataPointer(ECID, Type, ECName, ECUnit, P, Remark);
    SetECEnableData(ECID, ECName);                                              //pig 2014.08.27 KYEC
}
//extern char szEC_SerialNo[64];
//extern bool bEC_ShowYields;
//extern int  iEC_MaxSpeed;
//==============================================================================
// V 1.0
//==============================================================================
void THGem::SetTimeFormat(int Format)                                           // 0:12-bytes, 1:16-bytes, 2:14-bytes, 3:ISO8601 format
{
    iTimeFormat=Format;
}
//-------------------------------------------------------------------------------
// V 1.0
//-------------------------------------------------------------------------------
void THGem::SetTerminalWindows(TObject *Ptr)
{
    TerminalDisplayIndex=0;
    TerminalMemoPtr=dynamic_cast<TMemo *>(Ptr);
    if(TerminalMemoPtr!=NULL)
    {
        TerminalDisplayIndex=1;
        return;
    }
    TerminalListboxPtr=dynamic_cast<TListBox*>(Ptr);
    if(TerminalListboxPtr!=NULL)
    {
        TerminalDisplayIndex=2;
        return;
    }
    TerminalEditPtr=dynamic_cast<TCustomEdit*>(Ptr);                            //Steven 20240604 : TEdit --> TCustomEdit
    if(TerminalEditPtr!=NULL)
    {
        TerminalDisplayIndex=3;
        return;
    }
    TerminalPanelPtr=dynamic_cast<TPanel*>(Ptr);
    if(TerminalPanelPtr!=NULL)
    {
        TerminalDisplayIndex=4;
        return;
    }
}
//------------------------------------------------------------------------------
void THGem::SetTerminalWindows2(TObject *Ptr)
{
    TerminalDisplayIndex2=0;
    TerminalMemoPtr2=dynamic_cast<TMemo *>(Ptr);
    if(TerminalMemoPtr2!=NULL)
    {
        TerminalDisplayIndex2=1;
        return;
    }
    TerminalListboxPtr2=dynamic_cast<TListBox*>(Ptr);
    if(TerminalListboxPtr2!=NULL)
    {
        TerminalDisplayIndex2=2;
        return;
    }
    TerminalEditPtr2=dynamic_cast<TCustomEdit*>(Ptr);                           //Steven 20240604 : TEdit --> TCustomEdit
    if(TerminalEditPtr2!=NULL)
    {
        TerminalDisplayIndex2=3;
        return;
    }
    TerminalPanelPtr2=dynamic_cast<TPanel*>(Ptr);
    if(TerminalPanelPtr2!=NULL)
    {
        TerminalDisplayIndex2=4;
        return;
    }
}
//-------------------------------------------------------------------------------
// V 1.0
//-------------------------------------------------------------------------------
void THGem::TerminalRequest(AnsiString S)
{
    unsigned char TID=1;
    InitLocalHead(10,1,0);
    DataItemOut(2, HType.LIST_TYPE, NULL);
    DataItemOut(1, HType.BINARY_TYPE, &TID);
    DataItemOut(HType.ASCII_TYPE, S);
    SendLocalData();

    GetTimeInfo();
    S=TimeString+AnsiString(" [S]==>> ")+S;

    if(TerminalDisplayIndex==1)
        TerminalMemoPtr->Lines->Add(S);
    else if(TerminalDisplayIndex==2)
        TerminalListboxPtr->Items->Add(S);
    else if(TerminalDisplayIndex==3)
        TerminalEditPtr->Text=S;
    else if(TerminalDisplayIndex==4)
        TerminalPanelPtr->Caption=S;

    if(TerminalDisplayIndex2==1)
        TerminalMemoPtr2->Lines->Add(S);
    else if(TerminalDisplayIndex2==2)
        TerminalListboxPtr2->Items->Add(S);
    else if(TerminalDisplayIndex2==3)
        TerminalEditPtr2->Text=S;
    else if(TerminalDisplayIndex2==4)
        TerminalPanelPtr2->Caption=S;
}
//-------------------------------------------------------------------------------
// V 1.0
//-------------------------------------------------------------------------------
void THGem::SetSpoolActive(bool Active)
{
    AnsiString S;
    if(Active==false)
    {
        FileListBox1->Refresh();
        FileListBox1->Update();
        if(FileListBox1->Items->Count!=0)
        {
            if(MessageDlg("All spool data will be clear,Sure to clear ?", mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0)==mrYes)
            {
                S=AnsiString("del ")+IncludeTrailingPathDelimiter(GemSpoolPath)+AnsiString("*.*/q/f");
                system(S.c_str());
            }
            else
            {
              return;
            }
        }
    }
    bSpoolActive=Active;
}
//-------------------------------------------------------------------------------
// V 1.0
//-------------------------------------------------------------------------------
bool THGem::GetSpoolActive()
{
    return bSpoolActive;
}
//-------------------------------------------------------------------------------
// V 1.0
//-------------------------------------------------------------------------------
void __fastcall THGem::FormCreate(TObject *Sender)
{
    //2013/09/16 lee start
    AnsiString S;
    lCPUFreq=GetCPUFreq();
    S=GetManID();
    strcpy(szManID, S.c_str());
    S=GetCPUType();
    strcpy(szGetCPUType, S.c_str());
//    2013/09/16 lee end

    SetSVDataPointer(3, HType.ASCII_TYPE,   "GemClock",                         "",         &GemClock,                      "Equipment ¥Ø«e®ÉÄÁ");
    SetSVDataPointer(4, HType.UINT_1_TYPE,  "GemControlState",                  "",         &GemControlState,               "Equipment ¥Ø«e³s½uª¬ºA    1: OffLine               , 2:OnLine Local                               , 3:OnLine Remote");
    SetSVDataPointer(5, HType.UINT_1_TYPE,  "GemLinkState",                     "",         &GemLinkState,                  "¥Ø«e³s±µª¬ºA    0:Disabled              , 1:Enabled/Not Communicating       , 2: Communicating");
    SetSVDataPointer(6, HType.INT_1_TYPE,   "SECSCommunicationMode",            "",         &SECSCommunicationMode,         "0:HSMS Mode        , 1:SECS Mode (Set By AP)");

    SetSVDataPointer(9, HType.UINT_1_TYPE,  "PreviousGemControlState",                  "", &GemControlPreState,            "Equipment ¥Ø«e³s½uª¬ºA    1: OffLine               , 2:OnLine Local                               , 3:OnLine Remote");
    //2013/09/16 lee start
    SetSVDataPointer(10, HType.INT_4_TYPE,  "CPU Frequence",                    "",         &lCPUFreq,                      "CPU operate MHZ");
    SetSVDataPointer(11, HType.ASCII_TYPE,  "CPU  Manufacturer",                "",         szManID,                        "");
    SetSVDataPointer(12, HType.ASCII_TYPE,  "CPU  Type",                        "",         szGetCPUType,                   "");
    //2013/09/16 lee end

    //2013/09/30 lee start
    SetSVDataPointer(13, HType.INT_4_TYPE,  "Total Space Of Disk C",            "MB",       &Disk_C_TotalSpaceMB,           "Total Space Of Disk C");
    SetSVDataPointer(14, HType.INT_4_TYPE,  "Total Space Of Disk D",            "MB",       &Disk_D_TotalSpaceMB,           "Total Space Of Disk D");
    SetSVDataPointer(15, HType.INT_4_TYPE,  "Total FreeSpace Of Disk C",        "MB",       &Disk_C_TotalFreeSpaceMB,       "Total FreeSpace Of Disk C");
    SetSVDataPointer(16, HType.INT_4_TYPE,  "Total FreeSpace Of Disk D",        "MB",       &Disk_D_TotalFreeSpaceMB,       "Total FreeSpace Of Disk D");
    SetSVDataPointer(17, HType.UINT_4_TYPE, "Memory Load Percent",              "%",        &ulMemoryLoad,                  "Memory Load Percent");
    SetSVDataPointer(18, HType.UINT_4_TYPE, "Memory Total Physic",              "%",        &ulTotalPhys,                   "Memory Total Physic");
    SetSVDataPointer(19, HType.UINT_4_TYPE, "Memory Avail Physic",              "%",        &ulAvailPhys,                   "Memory Avail Physic");
    //2013/09/30 lee end

    SetSVDataPointer(24, HType.ASCII_TYPE,  "GemMDLN",                          "",         &GemMDLN,                       "¾÷¥x«¬¸¹");
    SetSVDataPointer(25, HType.ASCII_TYPE,  "GemSOFTREV",                       "",         &GemSOFTREV,                    "³nÅéª©¥»");
    SetSVDataPointer(54, HType.UINT_4_TYPE, "GemSpoolCountActual",              "record",   &GemSpoolCountActual,           "Spool enable or disable");
    SetSVDataPointer(57, HType.ASCII_TYPE,  "GemSpoolStartTime",                "",         GemSpoolStartTime,              "Spool start time");

    SetECDataPointer(68, HType.UINT_1_TYPE, "Time Format",                      "",         &iTimeFormat, iMinTimeFormat, iMaxTimeFormat, iTimeFormatDefault, "");

    SetSVDataPointer(70, HType.UINT_1_TYPE, "Receipe Struct",                   "",         &SV_70_UNT1_ReceipeStruct,      "¥Î¨Ó©w¸q¥Ø«e¤u§@ÀÉªº«¬ºA 0:³æ¤@File ªºµ²ºc 1:¦hÀÉµ²ºc¦p .BLD ,.OFF µ¥²Õ¦X 2:¥H¥Ø¿ý·í¤u§@ÀÉ");
    SetSVDataPointer(71, HType.ASCII_TYPE,  "Receipe Extend",                   "",         &SV_71_ASCII_FilenameExtened,   "Receipe extend filename");
}
//-------------------------------------------------------------------------------
// V 1.0
//-------------------------------------------------------------------------------
void __fastcall THGem::SetAlamData(int iRowCount, AnsiString ALID, AnsiString Class, AnsiString ALTX, AnsiString Position)
{
    strGrdAlarm->Cells[7][iRowCount] =1;
    strGrdAlarm->Cells[8][iRowCount] =ALID;
    strGrdAlarm->Cells[9][iRowCount] =Class;
    strGrdAlarm->Cells[10][iRowCount]=Position;
}
//------------------------------------------------------------------------------
void __fastcall THGem::ReadAlamData()
{
    TStringList *memoPtr;
    AnsiString Filename;

    memoPtr=new TStringList;
    Filename=IncludeTrailingPathDelimiter(GemSystemPath)+AnsiString("AlarmData.def");

    if(FileExists(Filename)==false)
    {
        delete memoPtr;                                                         //16.09.14.00 Roy Add
        return;
    }

    memoPtr->LoadFromFile(Filename);

    try                                                                         //Ifor 20170523 (wei) add Á×§KAlarmData.def¸ê®ÆªÅ¥Õ³y¦¨SECS GEM µLªk¥¿±`±Ò°Ê
    {
        while(1)
        {
            if(memoPtr->Count==0)                                               //Ifor 20170523 (wei) ©¹¤W²¾Á×§K¸ê®Æ0µ§³y¦¨¿ù»~
                break;

            if(memoPtr->Strings[memoPtr->Count-1]=="")
                memoPtr->Delete(memoPtr->Count-1);
            else
                break;
        }
        strGrdAlarmOld->RowCount=memoPtr->Count;
        PasteStringGridAsTabFormat(strGrdAlarmOld, memoPtr);
    }
    catch(...)
    {
        MyDBIProcess("Exception", "THGem::ReadAlamData");
        memoPtr->Clear();                                                       //Ifor 20170603 (wei) TStringList §R°£«e¥ý Clean
        delete memoPtr;
    }
    strGrdAlarmOld->RowCount=memoPtr->Count;
    PasteStringGridAsTabFormat(strGrdAlarmOld, memoPtr);
    memoPtr->Clear();                                                           //Ifor 20170603 TStringList §R°£«e¥ý Clean
    delete memoPtr;
}
//------------------------------------------------------------------------------
void __fastcall THGem::WriteAlamData()
{
    TStringList *memoPtr;
    AnsiString Filename;

    memoPtr=new TStringList;
    CopyStringGridAsTabFormat(strGrdAlarm, memoPtr);


    Filename=IncludeTrailingPathDelimiter(GemSystemPath)+AnsiString("AlarmData.def");
    memoPtr->SaveToFile(Filename);
    memoPtr->Clear();
    delete memoPtr;
}
//------------------------------------------------------------------------------
void __fastcall THGem::ReportAlarm(AnsiString AlarmCode, bool bIsJam, int iDuplicateError, AnsiString SubMessage, bool bReleaseAlm)     //JerryYang 20170504 (Steven) bReleaseAlm¬°trueªí¥Ü¸Ñ°£alarm»Ý¤W³ø0x00
{
    unsigned int iALID,iALClass;
    unsigned char cALClass;
    AnsiString aALClass, aMessType="";
    AnsiString str;
    TDateTime dtPresent;
    if(CUSTOMER_CODE==CC_TFME_CHINA && GemControlState<=1)                      //JerryYang 20200527 Offline¤£¤W³ø
        return;

    dtPresent=Now();
    DecodeDate(dtPresent, SystemYear, SystemMonth, SystemDate);
    DecodeTime(dtPresent, SystemHour, SystemMin, SystemSec, SystemMSec);
    str.sprintf("%04d%02d%02d%02d%02d%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);

    for(int i=1; i<strGrdAlarm->RowCount; i++)
    {
        if(strGrdAlarm->Cells[2][i]==AlarmCode)
        {
            if(strGrdAlarm->Cells[7][i]=="1")
            {
                InitLocalHead(5, 1, 1);                                         //S5F1
                if(CUSTOMER_CODE==CC_KYEC_JCTHIU ||
                   CUSTOMER_CODE==CC_KYEC_CHEN ||
                   CUSTOMER_CODE==CC_KYEC_LEE ||
                   CUSTOMER_CODE==CC_DL_TEK)                                    //JerryYang 20170317 (wei) ¥u¦³¨Ê¤¸ªºAlarm Report¤~¦^¶Ç6¶µ
                {
                    DataItemOut(6, HType.LIST_TYPE, NULL);
                    DataItemOut(HType.ASCII_TYPE, str);                         //®É¶¡

                    iALClass=atoi(strGrdAlarm->Cells[9][i].c_str());            //Alarm Class
                    DataItemOut(1, HType.UINT_4_TYPE, &iALClass);

                    iALID=atoi(strGrdAlarm->Cells[8][i].c_str());               //Alarm Code ¤E½X
                    DataItemOut(1, HType.UINT_4_TYPE, &iALID);

                    DataItemOut(HType.ASCII_TYPE, strGrdAlarm->Cells[6][i]);    //Alarm Text
                    DataItemOut(HType.ASCII_TYPE, strGrdAlarm->Cells[10][i]);   //Alarm Position

                    aMessType+=(bIsJam)?"1,":"0,";
                    aMessType+=AnsiString(iDuplicateError)+",";
                    aMessType+=SubMessage;
                    DataItemOut(HType.ASCII_TYPE, aMessType);                   //Alarm Sub Message
                }
                else                                                            //wei 20161102 TSMC ReportAlarm¦^¶Ç3¶µ
                {
                    DataItemOut(3, HType.LIST_TYPE, NULL);

                    if(bReleaseAlm)                                             //JerryYang (Steven) 20170504 bReleaseAlm¬°trueªí¥Ü¸Ñ°£alarm»Ý¤W³ø0x00
                        cALClass=0x00;
                    else
                        cALClass=0x80;                                          //JerryYang (Steven) 20170504 ªí¥Üµo¥Íalarm»Ý¤W³ø0x80
                    DataItemOut(1, HType.BINARY_TYPE, &cALClass);               //ALCD
                    iALID=atoi(strGrdAlarm->Cells[8][i].c_str());               //ALID ¤E½X
                    DataItemOut(1, HType.UINT_4_TYPE, &iALID);                  //ALID ¤E½X
                    if(CosFunction.bS5F1UseJamRate)                             //Steven 20220331 : S5F1ªºALTX¸Ì­±¥[¤WJAM Rate§PÂ_
                    {
                        aMessType.sprintf("%d,%d,%s", bIsJam, iDuplicateError, SubMessage);
                    }
                    else
                    {
                        aMessType=SubMessage;                                   //ALTX
                    }
                    DataItemOut(HType.ASCII_TYPE, aMessType);                   //ALTX Alarm Sub Message
                }
                SendLocalData();
            }
            return;
        }
    }
}
//------------------------------------------------------------------------------
bool __fastcall THGem::CheckNeedReportAlarm(AnsiString S)
{
    for(int i=1; i<strGrdAlarm->RowCount; i++)
    {
        if(S==strGrdAlarm->Cells[1][i])
        {
            if(strGrdAlarm->Cells[7][i]=="1")
                return true;
            else
                return false;
        }
    }
    return true;
}
//------------------------------------------------------------------------------
void __fastcall THGem::ReportAlarmWithMessage(AnsiString ALID, AnsiString ALTX, AnsiString sHappenedTime, AnsiString sProcessKey, bool SameCase)
{
    bool ret;
    if(bReportSECS_GEM_Message)
    {

    }
    else
    {
        ret=CheckNeedReportAlarm(ALID);
        if(ret==false)
            return;

        if(GemCheckBoxUseExtendedAlarm->Checked)
        {
            InitLocalHead(100, 1, 0);
            DataItemOut(5, HType.LIST_TYPE, NULL);
            DataItemOut(HType.ASCII_TYPE, ALID);
            DataItemOut(HType.ASCII_TYPE, ALTX);
            DataItemOut(HType.ASCII_TYPE, sHappenedTime);
            DataItemOut(HType.ASCII_TYPE, sProcessKey);
            DataItemOut(1, HType.BOOLEAN_TYPE, &SameCase);
            SendLocalData();
        }
        else
        {
            unsigned int ALCD=1;
            InitLocalHead(5, 1, 1);
            DataItemOut(3, HType.LIST_TYPE, NULL);
            DataItemOut(1, HType.UINT_4_TYPE, &ALCD);
            DataItemOut(HType.ASCII_TYPE, ALID);
            DataItemOut(HType.ASCII_TYPE, ALTX);
            SendLocalData();
        }
    }
}
//==============================================================================
// V 1.0
//  0 : ¥u¶Ç°ÆÀÉ¦W¬° bld
//  1 : ¥DÀÉ¦W¬Û¦P¦a©Ò¦³ÀÉ®×³£­n¶Ç
//  2 : ¸ô®|¤Uªº¥Ø¿ý¦W,¤Î¤U­±ªºÀÉ®×
//==============================================================================
int THGem::SetReceipeDirectoryAndGlobalName(AnsiString Path, AnsiString FileMask, int Type)
{
    int ct;
    AnsiString S="", S1;
    Path=Path.UpperCase();
    FileMask=FileMask.UpperCase();
    UpLoadPath=Path;
    SV_71_ASCII_FilenameExtened=FileMask;
    SV_70_UNT1_ReceipeStruct=Type;
    if(Type==2)
    {
        DirectoryListBox1->Directory=Path;
        UpLoadPath=DirectoryListBox1->Directory;
        for(int i=0; i<DirectoryListBox1->Items->Count; i++)
        {
            S+=DirectoryListBox1->Items->Strings[i];
            S=S.UpperCase();
            S1=S+"\\";
            if(S==Path || S1==Path)
            {
                ct=i+1;
                break;
            }
            if(i!=0)
                S=S1;
        }
        UploadFileString->Clear();
        for(int i=ct; i<DirectoryListBox1->Items->Count; i++)
        {
            UploadFileString->Add(DirectoryListBox1->Items->Strings[i]);
        }
        return 0;
    }
    else if(Type==1)
    {
        FileListBox2->Mask=Path+FileMask;
        FileListBox2->Update();
        FileListBox2->Refresh();
        UploadFileString->Clear();
        for(int i=0; i<FileListBox2->Items->Count; i++)
        {
            S=FileListBox2->Items->Strings[i];
            int j=S.LastDelimiter(".");
            S=S.SubString(1, j-1);
            UploadFileString->Add(S);
        }
        return FileListBox2->Items->Count;
    }
    else
    {
        FileListBox2->Mask=Path+FileMask;
        FileListBox2->Update();
        FileListBox2->Refresh();
        UploadFileString->Clear();
        for(int i=0; i<FileListBox2->Items->Count; i++)
        {
            S=FileListBox2->Items->Strings[i];
            UploadFileString->Add(S);
        }
        return FileListBox2->Items->Count;
    }
}
//==============================================================================
// V 1.0
//==============================================================================
void __fastcall THGem::GemBtnSendTerminalMessageClick(TObject *Sender)
{
    TerminalRequest(GemTerminalSendEdit->Text);
    GemTerminalSendEdit->Text="";
}
//==============================================================================
// V 1.0
//==============================================================================
void __fastcall THGem::GemTerminalSendEditKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
    if(Key==0x0d)
        GemBtnSendTerminalMessageClick(this);
}
//==============================================================================
// V 1.0
//==============================================================================
void __fastcall THGem::SetMachineTypeAndSoftwarseVer(AnsiString Mdln, AnsiString SoftVer)
{
    GemMDLN     =Mdln;
    GemSOFTREV  =SoftVer;
}
//==============================================================================
// V 1.0
//==============================================================================
void __fastcall THGem::BtnEnableCommClick(TObject *Sender)
{
     Connect();
}
//==============================================================================
// V 1.0
//==============================================================================
void __fastcall THGem::BtnDisableCommClick(TObject *Sender)
{
    DisConnect();
}
//==============================================================================
// V 1.0
//==============================================================================
void __fastcall THGem::GemBtnOnlineRequestClick(TObject *Sender)
{
    OnLine(HGem->GetOnLineMode());
}
//==============================================================================
// V 1.0
//==============================================================================
void __fastcall THGem::GemBtnOfflineRequestClick(TObject *Sender)
{
    OffLine();
}
//==============================================================================
// V 1.0
//==============================================================================
void __fastcall THGem::GemBtnOnlineRemoteClick(TObject *Sender)
{
    OnLineRemote();
}
//==============================================================================
// V 1.0
//==============================================================================
void __fastcall THGem::GemBtnOnlineLocalClick(TObject *Sender)
{
    OnLineLocal();
}
//==============================================================================
// V 1.0
//==============================================================================
void __fastcall THGem::GemListRemoteFileNameClick(TObject *Sender)
{
    if(SV_70_UNT1_ReceipeStruct==0)
    {
        InitLocalHead(7, 19, 1);
        SendLocalData();
    }
    else if(SV_70_UNT1_ReceipeStruct==1)
    {
        InitLocalHead(101, 1, 1);
        SendLocalData();
    }
    else if(SV_70_UNT1_ReceipeStruct==2)
    {
        InitLocalHead(101, 3, 1);
        SendLocalData();
    }
}
//---------------------------------------------------------------------------
// 2013/05/27
// V1.1
// Lee
//---------------------------------------------------------------------------
void __fastcall THGem::GemDownLoadRemoteFileNameClick(TObject *Sender)
{
    int count=0;
    AnsiString S;
    for(int i=0; i<GemRemoteReceipeList->Items->Count; i++)
        if(GemRemoteReceipeList->Checked[i])
            count++;
    if(count<=0)
        return;

    for(int i=0; i<GemRemoteReceipeList->Items->Count; i++)
    {
        if(GemRemoteReceipeList->Checked[i])
        {
            S=GemRemoteReceipeList->Items->Strings[i];
            InitLocalHead(7, 5, 1);
            DataItemOut(1, HType.LIST_TYPE, NULL);
            DataItemOut(HType.ASCII_TYPE, S);
            bReceiveS7F6=false;
            SendLocalData();
            break;
        }
    }
}
//---------------------------------------------------------------------------
// 2013/05/27
// V1.1
// Lee
//---------------------------------------------------------------------------
void __fastcall THGem::GemDeleteRemoteFileNameClick(TObject *Sender)
{
}
//---------------------------------------------------------------------------
// 2013/05/27
// V1.1
// Lee
//---------------------------------------------------------------------------
void __fastcall THGem::GemUpLoadLocalFileNameClick(TObject *Sender)
{
    for(int i=0; i<GemLocalFileLixtBox->Items->Count; i++)
    {
        if(GemLocalFileLixtBox->Checked[i])
            UploadFileName->Add(GemLocalFileLixtBox->Items->Strings[i]);
    }
}
//---------------------------------------------------------------------------
// 2013/05/27
// V1.1
// Lee
//---------------------------------------------------------------------------
void __fastcall THGem::GemRefreshLocalFileClick(TObject *Sender)
{
    AnsiString S,S1;
    int i, j,ct;
    if(SV_70_UNT1_ReceipeStruct==0)
    {
        S=UpLoadPath+"\\"+SV_71_ASCII_FilenameExtened;
        FileListBox2->Mask=S;
        FileListBox2->Refresh();
        FileListBox2->Update();
        GemLocalFileLixtBox->Clear();
        for(i=0; i<FileListBox2->Items->Count; i++)
        {
            GemLocalFileLixtBox->Items->Add(FileListBox2->Items->Strings[i]);
        }
    }
    else if(SV_70_UNT1_ReceipeStruct==1)
    {
        S=UpLoadPath+"\\"+SV_71_ASCII_FilenameExtened;
        FileListBox2->Mask=S;
        FileListBox2->Refresh();
        FileListBox2->Update();
        GemLocalFileLixtBox->Clear();
        for(i=0; i<FileListBox2->Items->Count; i++)
        {
            S=FileListBox2->Items->Strings[i];
            j=S.LastDelimiter(".");
            S=S.SubString(1,j-1);
            GemLocalFileLixtBox->Items->Add(S);
        }
    }
    else if(SV_70_UNT1_ReceipeStruct==2)
    {
        DirectoryListBox1->Directory=UpLoadPath;
        DirectoryListBox1->Refresh();
        DirectoryListBox1->Update();
        S="";
        UpLoadPath=UpLoadPath.UpperCase();
        for(i=0; i<DirectoryListBox1->Items->Count; i++)
        {
            S+=DirectoryListBox1->Items->Strings[i];
            S=S.UpperCase();
            S1=S+"\\";
            if(S==UpLoadPath || S1==UpLoadPath)
            {
                ct=i+1;
                break;
            }
            if(i!=0)
                S=S1;
        }
        GemLocalFileLixtBox->Clear();
        for(i=ct; i<DirectoryListBox1->Items->Count; i++)
        {
            GemLocalFileLixtBox->Items->Add(DirectoryListBox1->Items->Strings[i]);
        }
    }
}
//---------------------------------------------------------------------------
// 2013/05/27
// V1.1
// Lee
//---------------------------------------------------------------------------
void __fastcall THGem::GemSelectAllRemoteFileClick(TObject *Sender)
{
    if(GemRemoteReceipeList==NULL)
        return;
    for(int i=0; i<GemRemoteReceipeList->Items->Count; i++)
        GemRemoteReceipeList->Checked[i]=true;
}
//---------------------------------------------------------------------------
// 2013/05/27
// V1.1
// Lee
//---------------------------------------------------------------------------
void __fastcall THGem::GemDisSelectAllRemoteFileClick(TObject *Sender)
{
    if(GemRemoteReceipeList==NULL)
        return;
    for(int i=0; i<GemRemoteReceipeList->Items->Count; i++)
        GemRemoteReceipeList->Checked[i]=false;
}
//---------------------------------------------------------------------------
// 2013/05/27
// V1.1
// Lee
//---------------------------------------------------------------------------
void __fastcall THGem::GemSelectAllLocalFileClick(TObject *Sender)
{
    if(GemLocalFileLixtBox==NULL)
        return;
    for(int i=0; i<GemLocalFileLixtBox->Items->Count; i++)
        GemLocalFileLixtBox->Checked[i]=true;
}
//---------------------------------------------------------------------------
// 2013/05/27
// V1.1
// Lee
//---------------------------------------------------------------------------
void __fastcall THGem::GemDisSelectAllLocalFileClick(TObject *Sender)
{
    if(GemLocalFileLixtBox==NULL)
        return;
    for(int i=0; i<GemLocalFileLixtBox->Items->Count; i++)
        GemLocalFileLixtBox->Checked[i]=false;
}
//---------------------------------------------------------------------------
// 2013/05/27
// V1.1
// Lee
// ³o­Ó function ¬O°õ¦æ ¦h­Ó receipe file ¨Ì§Ç¤U¸ü
//---------------------------------------------------------------------------
void __fastcall THGem::DoDownLoadRemoteFile()
{
    int &iRetryCT=iRetryCTDownLoadRemoteFile;
    int &Task=iDownLoadRemoteFileTask;

    switch(Task)
    {
        case 1:
            if(RequestRemoteDownLoad->Count!=0)
            {
                iRetryCT=0;
                Task=100;
            }
            break;
        case 100:
            InitLocalHead(7, 5, 1);
            DataItemOut(HType.ASCII_TYPE, RequestRemoteDownLoad->Strings[0]);
            bReceiveS7F6=false;
            DelayDownLoadRemoteFile.TimerSetSecAndOn(1);
            SendLocalData();
            Task=200;
            break;
        case 200:
            if(bReceiveS7F6==true)
            {
                bReceiveS7F6=false;
                RequestRemoteDownLoad->Delete(0);
                DelayDownLoadRemoteFile.TimerSetSecAndOn(1);
                Task=300;
                break;
            }
            if(DelayDownLoadRemoteFile.TimerOff())
            {
                iRetryCT++;
                if(iRetryCT>5)
                {
                    bReceiveS7F6=false;
                    RequestRemoteDownLoad->Delete(0);
                    Task=1;
                    break;
                }
            }
            else
            {
                Task=100;
            }
            break;
        case 300:
            if(DelayDownLoadRemoteFile.TimerOff())
            {
                Task=1;
            }
            else if(bReceiveS101F5==true || bReceiveS101F7==true)
            {
                bReceiveS101F5=false;
                bReceiveS101F7=false;
                DelayDownLoadRemoteFile.TimerSetSecAndOn(1);
            }
            break;
    }
}
//---------------------------------------------------------------------------
// 2013/05/27
// V1.1
//---------------------------------------------------------------------------
void __fastcall THGem::srvGemClientConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AnsiString S, S0, S1, S2, S3;
    for(int i=0; i<srvGem->Socket->ActiveConnections; i++)
    {
        S="Connect "+AnsiString(i)+":" +srvGem->Socket->Connections[i]->LocalAddress;
        StringOut(S);
    }

    S="Local Port:"+AnsiString(Socket->LocalPort);
    StringOut(S);
    S="Local Address:"+AnsiString(Socket->LocalAddress);
    StringOut(S);
    StringOut("==================");
    StringOut("connect");
    bServoSocketConnect=true;
    if(srvGem->Socket->ActiveConnections>1)
    {
        bReceiveMultiConnect=true;
        if(TerminalMemoPtr!=NULL)                                               // 2013.10.28 , Joye , KYEC SECS/GEM
        {
            TerminalMemoPtr->Lines->Add("¦³2¥x¥H¤WEAP³s±µHandler,½Ð½T»{");      //2013/09/30  lee
        }
    }
}
//---------------------------------------------------------------------------
// 2013/05/27
// V1.1
//---------------------------------------------------------------------------
AnsiString __fastcall THGem::GetSocketErrorMsg(TObject *Sender, int iErrCode)
{
    return GetErrorMsg(Sender, iErrCode);
}
//---------------------------------------------------------------------------
// 2013/05/27
// V1.1
//---------------------------------------------------------------------------
void __fastcall THGem::srvGemClientError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    //20130308 Daver add
    AnsiString S;
    StringOut("---------------------------------------------------");
    GetTimeInfo();
    S=GetSocketErrorMsg(Sender, ErrorCode)+"  "+TimeString;
    StringOut(S);
    SaveSECSGEMErrToLog(S);
    //================

    bServoSocketConnect=false;                                                  //Eliot 2012_1105
    //srvGem->Active=false;
    try
    {
        srvGem->Close();
    }
    catch(...)
    {
        LogClientSocketExceptionError(Sender, "THGem");                         //Steven 20231113 : ¬ö¿ýºô¸ô³s½u¨Ò¥~
    }
    ErrorCode=0;
    bTCPIP_Error=true;
}
//---------------------------------------------------------------------------
// 2013/05/27
// V1.1
//---------------------------------------------------------------------------
void __fastcall THGem::GemSBSetupClick(TObject *Sender)
{
    ShowModal();
}
//---------------------------------------------------------------------------
// 2013/05/27
// V1.1
//---------------------------------------------------------------------------
void __fastcall THGem::FormClose(TObject *Sender, TCloseAction &Action)
{
    SaveSystemDefault();
    SaveEventReportData();
    bShow=false;
}
//---------------------------------------------------------------------------
// 2013/05/27
// V1.1
//---------------------------------------------------------------------------
void __fastcall THGem::srvGemClientDisconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    bServoSocketConnect=false;
    GetTimeInfo();
    StringOut("disconnect  "+TimeString);                                       //JerryYang 20190411 Â_½u®É­n°O¿ý®É¶¡

    // Â_½u«á³s¤£°_¨Ó 2013/07/18 lee §â Servo Socket close ¦A open ¬Ý¬Ý
    // start
    //srvGem->Close();
    Timer1Task=1;                                                               // ¦n¹³¬O answer
    iOpenCommuncationTask=1;                                                    // ¦n¹³¬O answer
    // end
}
//---------------------------------------------------------------------------
// 2013/05/27
// V1.1
//---------------------------------------------------------------------------
void __fastcall THGem::ManualCreatergRoleClick(TObject *Sender)
{
    if(bShow==true)
    {
        ShowMessage("Need Restart Program After Modify !!!");

        if(rgRole->ItemIndex==0)
            bUseClientSocket=false;
        else
            bUseClientSocket=true;

        if(bUseClientSocket==true)
            edtIP->Enabled=true;
        else
            edtIP->Enabled=false;
    }
}
//---------------------------------------------------------------------------
// 2013/05/27
// V1.1
//---------------------------------------------------------------------------
void __fastcall THGem::FormShow(TObject *Sender)
{
    Left=(1280-Width)/2;                                                        //Steven 20220407 : Á×§K¶]¨ìµe­±¥~
    Top =(1024-Height)/2;
    bShow=true;
    PageControl1->ActivePageIndex=0;                                            //13.08.26.01   klutter
    cbECChaneEventReport->Visible=CosFunction.bECChangeEventReportCanOnOff;     //JerryYang 20200520 ÂÂ¹q¸£°µDoReportECDataChangeCheck¨ç¦¡·|¼vÅTUPH,§ï¦¨¥\¯à¿ï¶µ
}
//---------------------------------------------------------------------------
// 2013/05/27
// V1.1
// Daver
// §â¥Ø«e¦¬¨ìªº¸ê®Æ¥Î HEX ®æ¦¡¦s¦¨¤å¦rÀÉ
// ¦sÀÉ®æ¦¡¬°  \\µ{¦¡¥Ø¿ý\\logs\\SECS_GEM\\¥Ø«eªº¦~_¤ë_¤é\\SECSGEM_BinaryLog_®É.txt
//---------------------------------------------------------------------------
/*
void __fastcall THGem::SaveTCPIPRecieveData(unsigned char *DataBuf, int iLen)
{
    Word wTmpYear,wTmpMonth,wTmpDate,wTmpHour,wTmpMin,wTmpSec,wTmpMSec;

    TDateTime tdSaveTime=Now();
    AnsiString asPath,asFN,asTmpStr1,asTmpStr2;
    int iBufLen;
    unsigned char *SaveBuffer;

    iBufLen=iLen;
    SaveBuffer=new unsigned char [iBufLen];
    memcpy(SaveBuffer,DataBuf,iBufLen);

    FILE *P;

    asPath.sprintf("%s\\logs\\SECS_GEM\\%s",CurrentDirectory.c_str(),tdSaveTime.FormatString("yyyy_mm_dd").c_str());
    MyForceDirectories(asPath);

    //Save BinaryData
    asFN.sprintf("%s\\SECSGEM_BinaryLog_%s.dat",asPath.c_str(),tdSaveTime.FormatString("hh").c_str());
    P=fopen(asFN.c_str(),"a+");
    if(P!=NULL)
    {
        fwrite(SaveBuffer,iBufLen,1,P);
        fclose(P);
    }

    //Save Text
    asFN.sprintf("%s\\SECSGEM_Log_%s.txt",asPath.c_str(),tdSaveTime.FormatString("hh").c_str());

    asTmpStr1="HEX Data : ";
    for(int i=0; i<iBufLen; ++i)
    {
        asTmpStr2.sprintf("%02X ",SaveBuffer[i]);
        asTmpStr1+=asTmpStr2;                                                   // ³oºØ¤èªk«ÜºC ­n«Å§i char or new char
    }
    DecodeDate(tdSaveTime,wTmpYear,wTmpMonth,wTmpDate);
    DecodeTime(tdSaveTime,wTmpHour,wTmpMin,wTmpSec,wTmpMSec);
    asTmpStr2.sprintf("%04d-%02d-%02d %02d:%02d:%02d.%03d",wTmpYear,wTmpMonth,wTmpDate,wTmpHour,wTmpMin,wTmpSec,wTmpMSec);

    P=fopen(asFN.c_str(),"a+");
    if(P!=NULL)
    {
        fputs(asTmpStr2.c_str(),P);
        fputs("\n",P);
        fputs(asTmpStr1.c_str(),P);
        fputs("\n",P);
        fputs("===========================================================",P);
        fputs("\n",P);
        fclose(P);
    }

    delete SaveBuffer;
}
*/
//---------------------------------------------------------------------------
// 2013/05/27
// V1.1
// Lee
// ÀË¬d¥Ø«eªº SF code ¬O§_¬° response code ,¤ñ¦p·íªì°e S1,F13 «h¬O§_¬° S1,F14 ­Y¬O«h§â¦¹ item delete ,§_«h¤@¬q®É¶¡¨S¦¬¨ìreturn code ­nµo¥X T3 error
//---------------------------------------------------------------------------
void __fastcall THGem::CheckSFCodeResponse()
{
    int Index;
    AnsiString S;

    S=AnsiString(Remote.MessageID_S)+" "+AnsiString(Remote.MessageID_F)+" "+AnsiString(Remote.SystemByte);

    // Joseph 20221111 (Jason) ·s¼W TCriticalSection S //
    //==>
    csSFCodeResponse->Acquire();
    try
    {
        if(TimeLeft->Count!=SFCodeResponseList->Items->Count)
        {
            SFCodeResponseList->Clear();
            TimeLeft->Clear();
        }
        else
        {
            Index=SFCodeResponseList->Items->IndexOf(S);
            if(Index!=-1)
            {
                SFCodeResponseList->Items->Delete(Index);
                TimeLeft->Delete(Index);
            }
        }
    }
    __finally
    {
        csSFCodeResponse->Release();
    }

    //Index=SFCodeResponseList->Items->IndexOf(S);
    //if(Index!=-1)
    //{
    //    SFCodeResponseList->Items->Delete(Index);
    //    TimeLeft->Delete(Index);
    //}
    //<==
    // Joseph 20221111 (Jason) ·s¼W TCriticalSection E //
}
//---------------------------------------------------------------------------
// 2013/05/27
// V1.1
// Daver
// §â¥Ø«e¦¬¨ìªº¸ê®Æ¥Î Text ¤è¦¡¤©¥H¦sÀÉ
// ¦sÀÉ®æ¦¡¬°  \\µ{¦¡¥Ø¿ý\\logs\\SECS_GEM\\¥Ø«eªº¦~_¤ë_¤é\\®É.txt
//---------------------------------------------------------------------------
void __fastcall THGem::SaveSECSGEMTextToLog()
{
    TDateTime tdSaveTime=Now();
    AnsiString asPath, asFN, asTmpStr1, asTmpStr2;
    FILE *P;
    int i;
    DateSeparator='_';                                                          // Sam 20210225 : ¼W¥[«OÅ@
//    asPath.sprintf("D:\\SECS_GEM_LOGS\\%s", tdSaveTime.FormatString("yyyy_mm_dd")); //Steven 20141230
    asPath.sprintf("D:\\SECS_GEM_LOGS\\%s\\%s", tdSaveTime.FormatString("yyyy"), tdSaveTime.FormatString("mm_dd"));         //JerryYang 20170309 (wei) ­×¥¿secs gem log path¿ù»~
    MyForceDirectories(asPath);
    asFN.sprintf("%s\\SECSGEM_TextLog_%s.txt", asPath, tdSaveTime.FormatString("hh"));

    P=fopen(asFN.c_str(), "a+");
    if(P!=NULL)
    {
        for(i=0; i<LogDataString->Count; i++)
        {
            fputs(LogDataString->Strings[i].c_str(), P);
            fputs("\n", P);
        }
        fclose(P);
        LogDataString->Clear();
    }
}
//---------------------------------------------------------------------------
// 2013/05/27
// V1.1
// Lee
// ¨ú±o¤@¦æ¸ê®Æ¤Îªø«×¤Î«¬ºA,¦ý¥»µ§¸ê®Æ¨ú±o«á·|§â¸ê®ÆÃìµ²¤º¸ê®Æ§R°£
// iReturnCode ªº¥Î·N¬Oªì©l­È=1,·í³sÄò©I¥s¥»¨ç¼Æ­Y«e¤@µ§¸ê®Æ¬O err ¦ý¥»µ§¸ê®Æ«o¬O ok, Á×§K¿ù»~½X³Q«á¨Óªº¥¿½T½X©Ò¨ú¥N
//---------------------------------------------------------------------------
int THGem::GetDataItemLenAndTypeAndDelete(int &len, unsigned char &Type)
{
    int ret;
    ret=GetDataItemLenAndTypeAndDeleteSub(len, Type);
    if(iReturnCode==1)
        iReturnCode=ret;
    return ret;
}
//---------------------------------------------------------------------------
// 2013/05/27
// V1.1
// Lee
// ¨ú±o¤@¦æ¸ê®Æ¤Î«ü©wªø«×,«¬ºAªº¸ê®Æ¤º®e ¨Ã¦s¨ì P ©Ò«ü©w¦a«ü¼Ð
// iReturnCode ªº¥Î·N¬Oªì©l­È=1,·í³sÄò©I¥s¥»¨ç¼Æ­Y«e¤@µ§¸ê®Æ¬O err ¦ý¥»µ§¸ê®Æ«o¬O ok, Á×§K¿ù»~½X³Q«á¨Óªº¥¿½T½X©Ò¨ú¥N
//---------------------------------------------------------------------------
int THGem::DataItemInNew(AnsiString &P)
{
    int len;
    unsigned char Type;
    GetDataItemLenAndType(len, Type);
    int ret=DataItemIn(len, Type, P);
    if(iReturnCode==1)
        iReturnCode=ret;
    return ret;
}
//---------------------------------------------------------------------------
int THGem::DataItemIn(int len, unsigned char Type, void *P)
{
    int ret;
    ret=DataItemInSub(len, Type, P);
    if(iReturnCode==1)
        iReturnCode=ret;
    return ret;
}
//------------------------------------------------------------------------------
int THGem::DataItemIn(int len, unsigned char t, AnsiString &Str)
{
    int ret;

    if(len<=0)                                                                  //JerryYang 20250725 : ¼W¥[«OÅ@
    {
        iReturnCode=0;
        Str="";
        return 0;
    }

    if(t==HType.ASCII_TYPE)
    {
//        char P[10240];                                                          //JerryYang 20240328
        char *P;                                                                //Steven 20250726 : §ï¥Î°ÊºA«Å§i
        P=new char [len+100];

        ret=DataItemInSub(len, t, P);
//        AnsiString temp;                                                        //Jimmychiu 20250628 : for SECS GEM
//        temp.SetLength(len);
//        ret=DataItemInSub(len, t, &temp[1]);
        if(ret==1)
        {
            Str.sprintf("%s", P);
            iReturnCode=ret;
        }
        delete[] P;
        P=NULL;
        return ret;
    }
    if(len!=1)
        return -1;

    if(t==HType.UINT_1_TYPE)
    {
        unsigned char P;
        ret=DataItemInSub(len, t, &P);
        if(ret==1)
        {
            Str=(unsigned)P;
            iReturnCode=ret;
        }
        return ret;
    }
    else if(t==HType.UINT_2_TYPE)
    {
        unsigned short P;
        ret=DataItemInSub(len, t, &P);
        if(ret==1)
        {
            Str=P;
            iReturnCode=ret;
        }
        return ret;
    }
    else if(t==HType.UINT_4_TYPE)
    {
        unsigned P;
        ret=DataItemInSub(len, t, &P);
        if(ret==1)
        {
            Str=P;
            iReturnCode=ret;
        }
        return ret;
    }
    else if(t==HType.UINT_8_TYPE)
    {
        unsigned __int64 P;
        ret=DataItemInSub(len, t, &P);
        if(ret==1)
        {
            Str=P;
            iReturnCode=ret;
        }
        return ret;
    }
    else if(t==HType.INT_1_TYPE)
    {
        char P;
        ret=DataItemInSub(len, t, &P);
        if(ret==1)
        {
            Str=int(P);
            iReturnCode=ret;
        }
        return ret;
    }
    else if(t==HType.INT_2_TYPE)
    {
        short P;
        ret=DataItemInSub(len, t, &P);
        if(ret==1)
        {
            Str=P;
            iReturnCode=ret;
        }
        return ret;
    }
    else if(t==HType.INT_4_TYPE)
    {
        int P;
        ret=DataItemInSub(len, t, &P);
        if(ret==1)
        {
            Str=P;
            iReturnCode=ret;
        }
        return ret;
    }
    else if(t==HType.INT_8_TYPE)
    {
        __int64 P;
        ret=DataItemInSub(len, t, &P);
        if(ret==1)
        {
            Str=P;
            iReturnCode=ret;
        }
        return ret;
    }
    else if(t==HType.FT_4_TYPE)                                                 //Steven 20130730 ADD
    {
        float P;
        ret=DataItemInSub(len, t, &P);
        if(ret==1)
        {
            Str=P;
            iReturnCode=ret;
        }
        return ret;
    }
    else if(t==HType.FT_8_TYPE)                                                 //Steven 20130730 ADD
    {
        double P;
        ret=DataItemInSub(len, t, &P);
        if(ret==1)
        {
            Str=P;
            iReturnCode=ret;
        }
        return ret;
    }
    else
    {
        return -1;
    }
}
//---------------------------------------------------------------------------
// 2013/05/27
// V1.1
// Lee
// ¨ú±o¤@¦æ¸ê®Æ¤Îªø«×¤Î«¬ºA,¦ý¥»µ§¸ê®Æ¨ú±o«á¨Ã¨ÌµM¦s¦b¸ê®ÆÃìµ²¤º
// iReturnCode ªº¥Î·N¬Oªì©l­È=1,·í³sÄò©I¥s¥»¨ç¼Æ­Y«e¤@µ§¸ê®Æ¬O err ¦ý¥»µ§¸ê®Æ«o¬O ok, Á×§K¿ù»~½X³Q«á¨Óªº¥¿½T½X©Ò¨ú¥N
//---------------------------------------------------------------------------
int THGem::GetDataItemLenAndType(int &len, unsigned char &Type)
{
    int ret;
    ret=GetDataItemLenAndTypeSub(len, Type);
    if(iReturnCode==1)
        iReturnCode=ret;
    return ret;
}
//==============================================================================
// 2013/05/27
// V1.1
// Lee
// ³]©w³q°T address , port ªì©l­È
//==============================================================================
void THGem::SetDefaultAddressAndPort(AnsiString Address, AnsiString Port, AnsiString DeviceID)
{
    DefaultAddress  =Address;
    DefaultPort     =Port;
    DefaultDeviceID =DeviceID;
    bOpenCommuncation=true;
}
//------------------------------------------------------------------------------
bool THGem::CheckSFFormatDataRequest(AnsiString ErrString)
{
    int ret, len, iTotalItem;
    unsigned char Type;

    ret=GetDataItemLenAndTypeAndDelete(len, Type);                              // L,2
    if(ret!=1 || Type!=HType.LIST_TYPE)                                         // data error
    {
        StringOut(ErrString);
        SendInvalidDataMessageToHost(ErrString);                                //2013_09_06  steven
        return false;
    }
    else
    {
        iTotalItem=len;
        for(int i=0; i<iTotalItem; i++)
        {
            ret=GetDataItemLenAndTypeAndDelete(len, Type);                      // 2. L,n # of parameters
            if(ret!=1)
            {
                StringOut(ErrString);
                SendInvalidDataMessageToHost(ErrString);
                return false;
            }
            else
            {
                if(Type==HType.ASCII_TYPE)
                {
                    SReceiveData->Delete(0);                                    // ¸ê®Æ®³¨«
                }
                else
                {
                    for(int j=0; j<len; j++)
                        SReceiveData->Delete(0);                                // ¸ê®Æ®³¨«
                }
            }
        }
    }
    return true;
}
//------------------------------------------------------------------------------
void THGem::SendInvalidDataMessageToHost(AnsiString S)
{
    InitLocalHead(9, 7, 0);
    DataItemOut(HType.ASCII_TYPE, S);
    SendLocalData();
}
//------------------------------------------------------------------------------
//Report
void __fastcall THGem::SetCEIDContent(unsigned iCeid, AnsiString CeidAlias, unsigned iReportCount, unsigned *iReportIDData, int Mode) // mode=1 ¬OHandler ,=0 ¬O Host ©w¸q¦a
{
    int pos;
    bool bSearchOK=false;
    unsigned int iBackID[SECS_EVENT.TotalEvent], ct=0;
    AnsiString S;

    for(int y=1; y<strGrdCEID->RowCount; y++)
    {
        if(strGrdCEID->Cells[0][y]==iCeid)
        {
            bSearchOK=true;
            pos=y;
            break;
        }
    }

    if(bSearchOK==false)
    {
        for(int y=1; y<strGrdCEID->RowCount; y++)
        {
            if(strGrdCEID->Cells[0][y]=="")
            {
                strGrdCEID->Cells[0][y]=iCeid;
                strGrdCEID->Cells[1][y]=Mode;
                strGrdCEID->Cells[2][y]=CeidAlias;

                for(int i=3; i<strGrdCEID->ColCount; i++)
                    strGrdCEID->Cells[i][y]="";
                for(unsigned int x=3; x<(iReportCount+3); x++)
                    strGrdCEID->Cells[x][y]=iReportIDData[x-3];
                break;
            }
        }
    }
    else
    {
        strGrdCEID->Cells[2][pos]=CeidAlias;
        for(int i=3; i<strGrdCEID->ColCount; i++)
        {
            S=strGrdCEID->Cells[i][pos];
            if(S=="")
                continue;
            if(GetReportIDType(S)==(1-Mode))
            {
                iBackID[ct]=atoi(S.c_str());
                ct++;
            }
        }
        for(int i=3; i<strGrdCEID->ColCount; i++)
            strGrdCEID->Cells[i][pos]="";
        for(unsigned int x=0; x<iReportCount; x++)
            strGrdCEID->Cells[x+3][pos]=iReportIDData[x];
        for(unsigned int x=0; x<ct; x++)
            strGrdCEID->Cells[x+iReportCount+3][pos]=iBackID[x];
    }
    SaveEventReportData();
}
//------------------------------------------------------------------------------
void __fastcall THGem::SetCEIDContent(unsigned iCeid, unsigned iReportCount, unsigned *iReportIDData, int Mode) // mode=1 ¬OHandler ,=0 ¬O Host ©w¸q¦a
{
    SetCEIDContent(iCeid, "", iReportCount, iReportIDData, Mode);
}
//------------------------------------------------------------------------------
unsigned __fastcall THGem::GetCEIDContent(unsigned iCeid, unsigned *iReportIDData, int Mode)
{
    int ct=0;
    for(int y=1; y<strGrdCEID->RowCount; y++)
    {
        if(strGrdCEID->Cells[0][y]==iCeid)
        {
            for(int x=3; x<strGrdCEID->RowCount; x++)
            {
                if(strGrdCEID->Cells[x][y]=="")
                    break;
                iReportIDData[ct]=atoi(strGrdCEID->Cells[x][y].c_str());
                ct++;
            }
            return ct;
        }
    }
    return 0;
}
//------------------------------------------------------------------------------
bool __fastcall THGem::SetReportIDContent(unsigned iCeid, unsigned iReportCount, unsigned *iReportIDData, int Mode) // mode=1 ¬OHandler ,=0 ¬O Host ©w¸q¦a
{
    int pos;
    bool bSearchOK=false;

    if((int)(iReportCount+2)>stdGridReportID->ColCount)
        return false;

    for(int y=1; y<stdGridReportID->RowCount; y++)
    {
        if(stdGridReportID->Cells[0][y]==iCeid)
        {
            bSearchOK=true;
            pos=y;
            break;
        }
    }
    if(bSearchOK==false)
    {
        for(int y=1; y<stdGridReportID->RowCount; y++)
        {
            if(stdGridReportID->Cells[0][y]=="")
            {
                stdGridReportID->Cells[0][y]=iCeid;
                stdGridReportID->Cells[1][y]=Mode;
                for(int x=2; x<stdGridReportID->ColCount; x++)
                    stdGridReportID->Cells[x][y]="";
                for(unsigned int i=2; i<(iReportCount+2); i++)
                    stdGridReportID->Cells[i][y]=iReportIDData[i-2];
                bSearchOK=true;
                break;
            }
        }
    }
    else
    {
        for(int x=2; x<stdGridReportID->ColCount; x++)
            stdGridReportID->Cells[x][pos]="";
        for(unsigned int i=2; i<(iReportCount+2); i++)
            stdGridReportID->Cells[i][pos]=iReportIDData[i-2];
    }
    SaveEventReportData();
    return bSearchOK;
}
//------------------------------------------------------------------------------
bool __fastcall THGem::DeleteReportID(unsigned iCeid, int Type)                 //2013/11/20 lee
{
    int pos, iY;
    bool bSearchOK=false;

    for(int y=1; y<stdGridReportID->RowCount; y++)
    {
        if(stdGridReportID->Cells[0][y]==iCeid &&
           stdGridReportID->Cells[1][y]==Type)                                  //2013/11/20 lee
        {
            pos=y;
            bSearchOK=true;
            break;
        }
    }

    if(bSearchOK==true)
    {
        for(int y=pos; y<(stdGridReportID->RowCount-1); y++)
        {
            for(int x=0; x<stdGridReportID->ColCount; x++)
                stdGridReportID->Cells[x][y]=stdGridReportID->Cells[x][y+1];
        }

        iY=stdGridReportID->RowCount-1;
        for(int x=0; x<stdGridReportID->ColCount; x++)
            stdGridReportID->Cells[x][iY]="";
        return true;
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------
bool __fastcall THGem::DeleteReportIDOfCeid(unsigned iCeid)
{
    int ct;
    AnsiString S[1024];

    for(int y=1; y<strGrdCEID->RowCount; y++)
    {
        for(int x=3; x<strGrdCEID->ColCount; x++)
        {
            if(strGrdCEID->Cells[x][y]==iCeid)
                strGrdCEID->Cells[x][y]="";
        }
        ct=0;
        for(int x=3; x<strGrdCEID->ColCount; x++)
        {
            if(strGrdCEID->Cells[x][y]!="")
            {
                S[ct]=strGrdCEID->Cells[x][y];
                ct++;
            }
        }

        for(int x=3; x<strGrdCEID->ColCount; x++)
            strGrdCEID->Cells[x][y]="";

        for(int i=0; i<ct; i++)
            strGrdCEID->Cells[3+i][y]=S[i];
    }
    return true;
}
//------------------------------------------------------------------------------
bool __fastcall THGem::DeleteAllHostDefineReportID()
{
    unsigned iReportID;
    bool bSearchOK;
    do
    {
        bSearchOK=false;
        for(int y=1; y<stdGridReportID->RowCount; y++)
        {
            if(stdGridReportID->Cells[1][y]=="0")
            {
                iReportID=atoi(stdGridReportID->Cells[0][y].c_str());
                DeleteReportID(iReportID, 0);                                   //2013/11/20 lee
                DeleteReportIDOfCeid(iReportID);
                bSearchOK=true;
                break;
            }
        }
    }while(bSearchOK==true) ;

    return true;
}
//------------------------------------------------------------------------------
unsigned __fastcall THGem::GetReportIDContent(unsigned iCeid, unsigned *iReportIDData, int Mode)
{
    int ct=0;
    for(int y=1; y<stdGridReportID->RowCount; y++)
    {
        if(stdGridReportID->Cells[0][y]==iCeid)
        {
            for(int x=2; x<stdGridReportID->RowCount; x++)
            {
                if(stdGridReportID->Cells[x][y]=="")
                    break;
                iReportIDData[ct]=atoi(stdGridReportID->Cells[x][y].c_str());
                ct++;
            }
            return ct;
        }
    }
    return 0;

}
//------------------------------------------------------------------------------
/*
L,3
    1. <DATAID>
    2. <CEID>
    3. L,a
        1. L,2
            1. <RPTID1>
            2. L,b
                1. <V1>
                 .
                 .
                b. <Vb>
            .
            .
        a. L,2               report a
           1. <RPTIDa>
           2. L,c            #Vs this report
               1. <V1>
                .
                .
               c. <Vc>
*/
//------------------------------------------------------------------------------
void __fastcall THGem::SendRepoerID(unsigned iReportID)
{
    unsigned ct;
    unsigned iSVIDData[1024];
    ct=GetReportIDContent(iReportID, iSVIDData, 2);
    DataItemOut(1, HType.UINT_4_TYPE, &iReportID);
    DataItemOut(ct, HType.LIST_TYPE, NULL);
    for(unsigned i=0; i<ct; i++)
        DataItemOutSV(iSVIDData[i]);
}
//------------------------------------------------------------------------------
/*
                       1. <RPTIDa>
                       2. L,c
                                   1. L,2
                                            1. <VID1>
                                            2. <V1>
                                             .
                                             .
                                  c. L,2
                                            1. <VIDc>
                                            2. <Vc>

*/
//------------------------------------------------------------------------------
void __fastcall THGem::SendAnnotatedRepoerID(unsigned iReportID)
{
    unsigned ct, svid;
    unsigned iSVIDData[10240];
    ct=GetReportIDContent(iReportID, iSVIDData, 2);
    DataItemOut(1, HType.UINT_4_TYPE, &iReportID);
    DataItemOut(ct, HType.LIST_TYPE, NULL);
    for(unsigned i=0; i<ct; i++)
    {
        DataItemOut(2, HType.LIST_TYPE, NULL);
        svid=iSVIDData[i];
        DataItemOut(1, HType.UINT_4_TYPE, &svid);
        DataItemOutSV(iSVIDData[i]);
    }
}
//------------------------------------------------------------------------------
void __fastcall THGem::SendCeid(unsigned iCeid)
{
    unsigned ct;
    unsigned iReportIDData[10240];
    ct=GetCEIDContent(iCeid, iReportIDData, 2);
    DataItemOut(ct, HType.LIST_TYPE, NULL);
    for(unsigned i=0; i<ct; i++)
    {
        DataItemOut(2, HType.LIST_TYPE, NULL);
        SendRepoerID(iReportIDData[i]);
    }
}
//------------------------------------------------------------------------------
void __fastcall THGem::SendAnnotatedCeid(unsigned iCeid)
{
    unsigned ct;
    unsigned iReportIDData[10240];
    ct=GetCEIDContent(iCeid, iReportIDData, 2);
    DataItemOut(ct, HType.LIST_TYPE, NULL);
    for(unsigned i=0; i<ct; i++)
    {
        DataItemOut(2, HType.LIST_TYPE, NULL);
        SendAnnotatedRepoerID(iReportIDData[i]);
    }
}
//------------------------------------------------------------------------------
bool __fastcall THGem::IsEnableEvent(unsigned iDataID, unsigned iCeid)
{
    for(int y=1; y<strGrdCEID->RowCount; y++)
    {
        if(strGrdCEID->Cells[0][y]==iCeid)
        {
            if(strGrdCEID->Cells[1][y]=="1")
                return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
void __fastcall THGem::EventReport(unsigned iDataID, unsigned iCeid)
{
    AnsiString S;
    if(IniConfig.bEnable_SECS_GEM==false)
        return ;

    if(CUSTOMER_CODE==CC_TFME_CHINA && GemControlState<=1)                      //JerryYang 20200527 Offline¤£¤W³ø
        return;

    if(IsEnableEvent(iDataID, iCeid))
    {
        if(chkAnnotatedEventReport->Checked)
        {
            InitLocalHead(6, 13, 1);
            DataItemOut(3, HType.LIST_TYPE, NULL);
            DataItemOut(1, HType.UINT_4_TYPE, &iDataID);
            DataItemOut(1, HType.UINT_4_TYPE, &iCeid);
            SendAnnotatedCeid(iCeid);
            SendLocalData();
        }
        else
        {
            InitLocalHead(6, 11, 1);
            DataItemOut(3, HType.LIST_TYPE, NULL);
            DataItemOut(1, HType.UINT_4_TYPE, &iDataID);
            DataItemOut(1, HType.UINT_4_TYPE, &iCeid);
            SendCeid(iCeid);
            SendLocalData();
        }
    }
    else
    {
        S=AnsiString("[Send]    ")+TimeString;
        StringOut(S);
        S=S.sprintf("Event Report(6,11) , DataID=%d , CEID=%d be disabled , abort send !!!", iDataID, iCeid);         //16.10.13.01 Roy Debug
        StringOut(S);
    }
    //Ifor 20170428 (Steven) add Ãö³¬µ{¦¡«e°e¥XSECS GEMÂ÷½u­n¨D
    //==>
    if(iCeid==SECS_EVENT.DoExit)                                                //24 Exit Pressed (SECS_EVENT.DoExit)
    {
        DoSeparate();
        try
        {
            srvGem->Close();
            clientGem->Close();
        }
        catch(...)
        {
            MyDBIProcess("Exception", "THGem::EventReport");
        }
        srvGem->Active=false;
        clientGem->Active=false;
        Timer1->Enabled=false;                                                  //Ifor 20170428 add Ãö³¬ SECS GEM Timer
        Close();
    }
    //<==
    //Ifor 20170428 (Steven) add Ãö³¬µ{¦¡«e°e¥XSECS GEMÂ÷½u­n¨D
}
//------------------------------------------------------------------------------
bool __fastcall THGem::CheckCeidExist(AnsiString Ceid)
{
    for(int y=1; y<strGrdCEID->RowCount; y++)
    {
        if(strGrdCEID->Cells[0][y]==Ceid)
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------
int __fastcall THGem::GetReportIDType(AnsiString ReportID)
{
    for(int y=1; y<stdGridReportID->RowCount; y++)
    {
        if(stdGridReportID->Cells[0][y]==ReportID)
        {
            if(stdGridReportID->Cells[1][y]=="0")
                return 0;
            else if(stdGridReportID->Cells[1][y]=="1")
                return 1;
            else
                return 2;
        }
    }
    return 3;
}
//------------------------------------------------------------------------------
bool __fastcall THGem::CheckCeidAlreadyDefine(AnsiString Ceid)
{
    AnsiString ReportID;
    for(int y=1; y<strGrdCEID->RowCount; y++)
    {
        if(strGrdCEID->Cells[0][y]==Ceid)
        {
            for(int x=3; x<strGrdCEID->ColCount; x++)                           //2014/01/01  lee  // add S1F23
            {
                ReportID=strGrdCEID->Cells[x][y];
                if(ReportID=="")
                    continue;
                if(GetReportIDType(ReportID)==0)
                    return true;
            }
        }
    }
    return false;
}
//------------------------------------------------------------------------------
bool __fastcall THGem::CheckReportIDExist(AnsiString ReportID)
{
    for(int y=1; y<stdGridReportID->RowCount; y++)
    {
        if(stdGridReportID->Cells[0][y]==ReportID)
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------
void __fastcall THGem::AddCeidReportID(AnsiString Ceid,AnsiString ReportID)
{
    for(int y=1; y<strGrdCEID->RowCount; y++)
    {
        if(strGrdCEID->Cells[0][y]==Ceid)
        {
            for(int x=3; x<strGrdCEID->ColCount; x++)                           // 2014/01/01  lee
            {
                if(strGrdCEID->Cells[x][y]=="" || strGrdCEID->Cells[x][y]==NULL)
                {
                    strGrdCEID->Cells[x][y]=ReportID;
                    return;
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall THGem::ProcessHostSendReportLinkID()
{
    AnsiString ReportID, SVID, Ceid;
    TStringList *strPtr;

    for(int i=0; i<slTempCeID->Count; i++)
    {
        Ceid=slTempCeID->Strings[i];
        strPtr=(TStringList *)lTempCeIDContent->Items[i];
        if(CheckCeidExist(Ceid)==false)
        {
            LinkReportAcknowledgeInvalidCeID();
            return;
        }
        if(CheckCeidAlreadyDefine(Ceid) && strPtr->Count!=0)
        {
            //DefineReportAcknowledgeAlreadyDefined();
            LinkReportAcknowledgeAlreadyDefined();                              // 2013.10.22 , Joye , Link Report
            return;
        }
        else
        {
            for(int j=0; j<strPtr->Count; j++)
            {
                ReportID=strPtr->Strings[j];
                if(CheckReportIDExist(ReportID)==false)
                {
                    LinkReportAcknowledgeInvalidReportID();
                    return;
                }
            }
        }
    }
    for(int i=0; i<slTempCeID->Count; i++)
    {
        Ceid=slTempCeID->Strings[i];
        strPtr=(TStringList *)lTempCeIDContent->Items[i];
        if(strPtr->Count==0)
        {
            DeleteHostReportIDOfAssignCeid(Ceid);
        }
        else
        {
            for(int j=0; j<strPtr->Count; j++)
            {
                ReportID=strPtr->Strings[j];
                AddCeidReportID(Ceid,ReportID);
            }
        }
        delete strPtr;
    }
    slTempCeID->Clear();
    lTempCeIDContent->Clear();

    unsigned char OK=0x00;
    InitLocalHead(2, 36, 0);
    DataItemOut(1, HType.BINARY_TYPE, &OK);
    SendLocalData();
    SaveEventReportData();
}
//------------------------------------------------------------------------------
void __fastcall THGem::DeleteAllHostReportIDOfCeid(unsigned ReportID)
{
    bool bSearch=true;
    TStringList *P;
    P=new TStringList;
    for(int y=1; y<strGrdCEID->RowCount; y++)
    {
        bSearch=false;
        for(int x=3; x<strGrdCEID->ColCount; x++)
        {
            if(strGrdCEID->Cells[x][y]==ReportID)
            {
                strGrdCEID->Cells[x][y]="";
                bSearch=true;
            }
        }
        if(bSearch )
        {
            P->Clear();
            for(int x=3; x<strGrdCEID->ColCount; x++)
            {
                if(strGrdCEID->Cells[x][y]!="")
                    P->Add(strGrdCEID->Cells[x][y]);
            }

            for(int x=3; x<strGrdCEID->ColCount; x++)
                strGrdCEID->Cells[x][y]="";

            for(int i=0; i<P->Count; i++)
                strGrdCEID->Cells[3+i][y]=P->Strings[i];
        }
    }
    delete P;
}
//------------------------------------------------------------------------------
void __fastcall THGem::DeleteHostReportIDOfAssignCeid(AnsiString Ceid)
{
    AnsiString ReportID;
    for(int y=1; y<strGrdCEID->RowCount; y++)
    {
        if(strGrdCEID->Cells[0][y]==Ceid)
        {
            for(int x=3; x<strGrdCEID->ColCount; x++)
            {
                ReportID=strGrdCEID->Cells[x][y];
                strGrdCEID->Cells[x][y]="";
            }
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall THGem::DeleteAllHostDefineCeid()
{
    for(int y=1; y<strGrdCEID->RowCount; y++)
    {
        if(strGrdCEID->Cells[0][y]!="")
            DeleteHostReportIDOfAssignCeid(strGrdCEID->Cells[0][y]);
    }
}
//------------------------------------------------------------------------------
bool __fastcall THGem::CheckReportAlreadyDefine(AnsiString ReportID)
{
    for(int y=1; y<stdGridReportID->RowCount; y++)
        if(stdGridReportID->Cells[0][y]==ReportID)
            return true;
    return false;
}
//------------------------------------------------------------------------------
void __fastcall THGem::ReportAcknowledge(unsigned char Code)
{
    InitLocalHead(2, 34, 0);
    DataItemOut(1, HType.BINARY_TYPE, &Code);
    SendLocalData();
}
//------------------------------------------------------------------------------
void __fastcall THGem::DefineReportAcknowledgeAccept()
{
    ReportAcknowledge(0x00);
}
//------------------------------------------------------------------------------
void __fastcall THGem::DefineReportAcknowledgeInsufficientSpace()
{
    ReportAcknowledge(0x01);
}
//------------------------------------------------------------------------------
void __fastcall THGem::DefineReportAcknowledgeFormatError()
{
    ReportAcknowledge(0x02);
}
//------------------------------------------------------------------------------
void __fastcall THGem::DefineReportAcknowledgeAlreadyDefined()
{
    ReportAcknowledge(0x03);
}
//------------------------------------------------------------------------------
void __fastcall THGem::DefineReportAcknowledgeInvalidSVID()
{
    ReportAcknowledge(0x04);
}
//------------------------------------------------------------------------------
void __fastcall THGem::ReportLinkAcknowledgeError(unsigned char ErrCode)
{
    InitLocalHead(2, 36, 0);
    DataItemOut(1, HType.BINARY_TYPE, &ErrCode);
    SendLocalData();
}
//------------------------------------------------------------------------------
void __fastcall THGem::LinkReportAcknowledgeFormatError()
{
    ReportLinkAcknowledgeError(0x02);
}
//------------------------------------------------------------------------------
void __fastcall THGem::LinkReportAcknowledgeAlreadyDefined()
{
    ReportLinkAcknowledgeError(0x03);
}
//------------------------------------------------------------------------------
void __fastcall THGem::LinkReportAcknowledgeInvalidCeID()
{
    ReportLinkAcknowledgeError(0x04);
}
//------------------------------------------------------------------------------
void __fastcall THGem::LinkReportAcknowledgeInvalidReportID()
{
    ReportLinkAcknowledgeError(0x05);
}
//------------------------------------------------------------------------------
/*
0 = Accepted
1 = Denied. At least one CEID does not exist
>1 = Other Errors
2-63 Reserved
*/
void __fastcall THGem::EnableDisableEventReportAcknowledgeError(unsigned char ErrCode)
{
    InitLocalHead(2, 38, 0);
    DataItemOut(1, HType.BINARY_TYPE, &ErrCode);
    SendLocalData();
}
//------------------------------------------------------------------------------
void __fastcall THGem::EnableDisableEventReportAcknowledgeCeidNotExist()
{
    EnableDisableEventReportAcknowledgeError(0x01);
}
//------------------------------------------------------------------------------
void __fastcall THGem::EnableDisableEventReportAcknowledgeFormatError()
{
    EnableDisableEventReportAcknowledgeError(0x02);
}
//------------------------------------------------------------------------------
bool __fastcall THGem::CheckCEIDExist(AnsiString sID)
{
    for(int y=1; y<strGrdCEID->RowCount; y++)
        if(strGrdCEID->Cells[0][y]==sID)
            return true;
    return false;
}
//------------------------------------------------------------------------------
void __fastcall THGem::CopyStringGridAsTabFormat(TStringGrid *strGrd, TStringList *memoPtr)
{
    int endx;
    AnsiString S;

    memoPtr->Clear();
    for(int y=0; y<strGrd->RowCount; y++)
    {
        S="";
        endx=0;
        for(int x=(strGrd->ColCount-1);x>=0;x--)
        {
            if(strGrd->Cells[x][y]!="")
            {
                endx=x;
                break;
            }
        }
        for(int x=0; x<=endx; x++)
        {
            S+=strGrd->Cells[x][y];
            S+="\t";
        }
        memoPtr->Add(S);
    }
}
//------------------------------------------------------------------------------
bool SplitStrByTabOnly(char *str, char *dest, int Max)
{
    char Buffer[10240];
    int ct1=0, ct2=0;

    if(str[ct1]=='\t')
    {
        strcpy(dest, "");
        for(;;)
        {
            Buffer[ct1]=str[ct1+1];
            if(Buffer[ct1]==0)
            {
                strcpy(str, Buffer);
                return true;
            }
            ct1++;
        }
    }
    else
    {
        while(1)                                                                // find first character
        {
            if(str[ct1]=='\x0' || str[ct1]=='\r' ||  str[ct1]=='\n')
                return false;
            if((str[ct1]!='\t' && str[ct1]!='\r'))
                break;
            ct1++;
        }
        while(1)
        {
            dest[ct2]=str[ct1];
            ct2++;
            ct1++;
            dest[ct2]='\x0';
            if((ct2+1)>=Max)
                break;

            if(str[ct1]!='\t' && str[ct1]!='\0' && str[ct1]!='\r')
            {
            }
            else
            {
                break;
            }
        }
        ct2=0;
        while(1)
        {
            Buffer[ct2]=str[ct1];
            if(str[ct1]=='\x0')
                break;
            ct1++;
            ct2++;
            Buffer[ct2]='\x0';
            if(ct2>=10240)
                break;
        }
        strcpy(str, Buffer);
        ct1=0;
        if(str[0]=='\t')
        {
            for(;;)
            {
                Buffer[ct1]=str[ct1+1];
                if(Buffer[ct1]==0)
                {
                    strcpy(str, Buffer);
                    return true;
                }
                ct1++;
            }
        }
    }
    return true;
}
//------------------------------------------------------------------------------
void __fastcall THGem::PasteStringGridAsTabFormat(TStringGrid *strGrd, TStringList *memoPtr)
{
    int x, y, StartX, StartY;
    char str[8192], str2[256];
    bool flag;
    AnsiString S;

    for(y=0; y<strGrd->RowCount; y++)
        for(x=0; x<strGrd->ColCount; x++)
            strGrd->Cells[x][y]="";

    for(y=0; y<memoPtr->Count; y++)
    {
        if(y>=strGrd->RowCount)
            break;
        strncpy(str, memoPtr->Strings[y].c_str(), sizeof(str));
        x=0;
        do
        {
            flag=SplitStrByTabOnly(str, str2, 256);
            if(flag==false)
                break;
            StartX=x;
            StartY=y;

            strGrd->Cells[StartX][StartY]=str2;
            x++;
            if(x>=strGrd->ColCount)
                break;
        }while(1);
    }
}
//------------------------------------------------------------------------------
void __fastcall THGem::ReadEventReportData()
{
    TStringList *memoPtr;
    AnsiString Filename;

    memoPtr=new TStringList;
    Filename="D:\\HT9045\\SECS\\SECS\\SYSTEM\\EventReport_CEID.def";
    if(FileExists(Filename)==false)
    {
        delete memoPtr;                                                         //16.09.14.00 Roy Add
        return;
    }

    memoPtr->LoadFromFile(Filename);
    PasteStringGridAsTabFormat(strGrdCEID, memoPtr);

    Filename="D:\\HT9045\\SECS\\SECS\\SYSTEM\\EventReport_ReportID.def";
    if(FileExists(Filename)==false)
    {
        delete memoPtr;                                                         //16.09.14.00 Roy Add
        return;
    }

    memoPtr->LoadFromFile(Filename);
    PasteStringGridAsTabFormat(stdGridReportID,memoPtr);

    delete memoPtr;
}
//------------------------------------------------------------------------------
void __fastcall THGem::GetAllSVInformation(TMemo *Ptr)
{
    // ID   SV  EC  SV Name     Type    Length  Unit    »¡©ú
    AnsiString ID="", IsSV="V", IsEC="", Name="", Type="", Length="", Unit="", Max="", Min="", Default="", Remark="";
    int i, Index;

    unsigned char t;
    AnsiString S;
    Ptr->Lines->Clear();
    for(i=0; i<SV_ID->Count; i++)
    {
        ID=SV_ID->Strings[i];
        Index=EC_ID->IndexOf(ID);
        if(Index>=0)
        {
//            GetECInformation(Ptr,Index);                      //20140213   wei
            continue;
        }

        //IsSV="V";
        t=atoi(SV_TYPE->Strings[i].c_str());
        if(t==HType.LIST_TYPE)            Type="LIST";
        else if(t==HType.ASCII_TYPE)      Type="ASCII";
        else if(t==HType.BOOLEAN_TYPE)    Type="BOOLEAN";
        else if(t==HType.BINARY_TYPE)     Type="BINARY";
        else if(t==HType.UINT_1_TYPE)     Type="UINT_1";
        else if(t==HType.UINT_2_TYPE)     Type="UINT_2";
        else if(t==HType.UINT_4_TYPE)     Type="UINT_4";
        else if(t==HType.UINT_8_TYPE)     Type="UINT_8";
        else if(t==HType.INT_1_TYPE)      Type="INT_1";
        else if(t==HType.INT_2_TYPE)      Type="INT_2";
        else if(t==HType.INT_4_TYPE)      Type="INT_4";
        else if(t==HType.INT_8_TYPE)      Type="INT_8";
        else if(t==HType.FT_4_TYPE)       Type="FT_4";
        else if(t==HType.FT_8_TYPE)       Type="FT_8";

        Name=SV_NAME->Strings[i];
        Unit=SV_UNIT->Strings[i];
        Remark=SV_Remark->Strings[i];

        S=ID+'\t'+IsSV+'\t'+IsEC+'\t'+Name+'\t'+Type+'\t'+Length+'\t'+Unit+'\t'+Max+'\t'+Min+'\t'+Default+'\t'+Remark;
        Ptr->Lines->Add(S);
    }
}
//------------------------------------------------------------------------------
void __fastcall THGem::GetALLECInformation(TObject * Ptr)
{
    for(int i=0; i<HGem->EC_ID->Count; i++)
    {
        GetECInformation(Ptr, i);
    }
}
//------------------------------------------------------------------------------
void __fastcall THGem::GetECInformation(TObject * Ptr, int Index)
{
    AnsiString ID="", IsSV="", IsEC="", Name="", Type="", Length="", Unit="", Max="", Min="", Default="", Remark="";
    int i, iRow;
    unsigned char t;
    AnsiString S;

    i=Index;
    TMemo       *memoPtr  = dynamic_cast <TMemo *>(Ptr);
    TStringGrid *sgPtr    = dynamic_cast <TStringGrid *>(Ptr);

    //Ptr->Lines->Clear();
    //for(i=0; i<EC_ID->Count; i++)
    {
        ID=EC_ID->Strings[i];
        IsEC="V";
        IsSV="V";

        t=atoi(EC_TYPE->Strings[i].c_str());
        if(t==HType.LIST_TYPE)            Type="LIST";
        else if(t==HType.ASCII_TYPE)      Type="ASCII";
        else if(t==HType.BOOLEAN_TYPE)    Type="BOOLEAN";
        else if(t==HType.BINARY_TYPE)     Type="BINARY";
        else if(t==HType.UINT_1_TYPE)     Type="UINT_1";
        else if(t==HType.UINT_2_TYPE)     Type="UINT_2";
        else if(t==HType.UINT_4_TYPE)     Type="UINT_4";
        else if(t==HType.UINT_8_TYPE)     Type="UINT_8";
        else if(t==HType.INT_1_TYPE)      Type="INT_1";
        else if(t==HType.INT_2_TYPE)      Type="INT_2";
        else if(t==HType.INT_4_TYPE)      Type="INT_4";
        else if(t==HType.INT_8_TYPE)      Type="INT_8";
        else if(t==HType.FT_4_TYPE)       Type="FT_4";
        else if(t==HType.FT_8_TYPE)       Type="FT_8";

        Name=EC_NAME->Strings[i];
        Unit=EC_UNIT->Strings[i];


        if(t==HType.ASCII_TYPE)
        {
            char *P;
            P=(char *)EC_Ptr_Min->Items[i];
            if(P==NULL)
                Min="";
            else
                Min=*P;

            P=(char *)EC_Ptr_Max->Items[i];
            if(P==NULL)
                Max="";
            else
                Max=*P;

            P=(char *)EC_Ptr_Default->Items[i];
            if(P==NULL)
                Default="";
            else
                Default=*P;
        }
        else if(t==HType.BOOLEAN_TYPE)
        {
            unsigned char *P;
            P=(unsigned char *)EC_Ptr_Min->Items[i];
            if(P!=NULL)
                Min=*P;
            else
                Min=EC_Ptr_Min_Value->Strings[i];

            P=(unsigned char *)EC_Ptr_Max->Items[i];
            if(P!=NULL)
                Max=*P;
            else
                Max=EC_Ptr_Max_Value->Strings[i];

            P=(unsigned char *)EC_Ptr_Default->Items[i];
            if(P!=NULL)
            {
                if((*P)==0)
                    Default="FALSE";
                else
                    Default="TRUE";
            }
            else
            {
                Default=EC_Ptr_Default_Value->Strings[i];
            }
        }
        else if(t==HType.BINARY_TYPE || t==HType.UINT_1_TYPE)
        {
            unsigned char *P;
            P=(unsigned char *)EC_Ptr_Min->Items[i];
            if(P!=NULL)
                Min=*P;
            else
                Min=EC_Ptr_Min_Value->Strings[i];

            P=(unsigned char *)EC_Ptr_Max->Items[i];
            if(P!=NULL)
                Max=*P;
            else
                Max=EC_Ptr_Max_Value->Strings[i];

            P=(unsigned char *)EC_Ptr_Default->Items[i];
            if(P!=NULL)
                Default=*P;
            else
                Default=EC_Ptr_Default_Value->Strings[i];
        }
        else if(t==HType.UINT_2_TYPE)
        {
            unsigned short *P;
            P=(unsigned short *)EC_Ptr_Min->Items[i];
            if(P!=NULL)
                Min=*P;
            else
                Min=EC_Ptr_Min_Value->Strings[i];

            P=(unsigned short *)EC_Ptr_Max->Items[i];
            if(P!=NULL)
                Max=*P;
            else
                Max=EC_Ptr_Max_Value->Strings[i];

            P=(unsigned short *)EC_Ptr_Default->Items[i];
            if(P!=NULL)
                Default=*P;
            else
                Default=EC_Ptr_Default_Value->Strings[i];
        }
        else if(t==HType.UINT_4_TYPE)
        {
            unsigned *P;
            P=(unsigned *)EC_Ptr_Min->Items[i];
            if(P!=NULL)
                Min=*P;
            else
                Min=EC_Ptr_Min_Value->Strings[i];

            P=(unsigned *)EC_Ptr_Max->Items[i];
            if(P!=NULL)
                Max=*P;
            else
                Max=EC_Ptr_Max_Value->Strings[i];


            P=(unsigned *)EC_Ptr_Default->Items[i];
            if(P!=NULL)
                Default=*P;
            else
                Default=EC_Ptr_Default_Value->Strings[i];
        }
        else if(t==HType.UINT_8_TYPE)
        {
            unsigned __int64 *P;
            P=(unsigned __int64*)EC_Ptr_Min->Items[i];
            if(P!=NULL)
                Min=*P;
            else
                Min=EC_Ptr_Min_Value->Strings[i];

            P=(unsigned __int64*)EC_Ptr_Max->Items[i];
            if(P!=NULL)
                Max=*P;
            else
                Max=EC_Ptr_Max_Value->Strings[i];

            P=(unsigned __int64*)EC_Ptr_Default->Items[i];
            if(P!=NULL)
                Default=*P;
            else
                Default=EC_Ptr_Default_Value->Strings[i];
        }
        else if(t==HType.INT_1_TYPE)
        {
            char *P;
            P=(char*)EC_Ptr_Min->Items[i];
            if(P!=NULL)
                Min=*P;
            else
                Min=EC_Ptr_Min_Value->Strings[i];

            P=(char*)EC_Ptr_Max->Items[i];
            if(P!=NULL)
                Max=*P;
            else
                Max=EC_Ptr_Max_Value->Strings[i];

            P=(char*)EC_Ptr_Default->Items[i];
            if(P!=NULL)
                Default=*P;
            else
                Default=EC_Ptr_Default_Value->Strings[i];
        }
        else if(t==HType.INT_2_TYPE)
        {
            short *P;
            P=(short *)EC_Ptr_Min->Items[i];
            if(P!=NULL)
                Min=*P;
            else
                Min=EC_Ptr_Min_Value->Strings[i];

            P=(short *)EC_Ptr_Max->Items[i];
            if(P!=NULL)
                Max=*P;
            else
                Max=EC_Ptr_Max_Value->Strings[i];

            P=(short *)EC_Ptr_Default->Items[i];
            if(P!=NULL)
                Default=*P;
            else
                Default=EC_Ptr_Default_Value->Strings[i];
        }
        else if(t==HType.INT_4_TYPE)
        {
            int *P;
            P=(int *)EC_Ptr_Min->Items[i];
            if(P!=NULL)
                Min=*P;
            else
                Min=EC_Ptr_Min_Value->Strings[i];

            P=(int *)EC_Ptr_Max->Items[i];
            if(P!=NULL)
                Max=*P;
            else
                Max=EC_Ptr_Max_Value->Strings[i];


            P=(int *)EC_Ptr_Default->Items[i];
            if(P!=NULL)
                Default=*P;
            else
                Default=EC_Ptr_Default_Value->Strings[i];
        }
        else if(t==HType.INT_8_TYPE)
        {
            __int64 *P;                                                         //Steven 20140911 : ­×¥¿INT_8_TYPE & UINT_8_TYPE
            P=(__int64 *)EC_Ptr_Min->Items[i];
            if(P!=NULL)
                Min=*P;
            else
                Min=EC_Ptr_Min_Value->Strings[i];

            P=(__int64 *)EC_Ptr_Max->Items[i];                                  //Steven 20140911 : ­×¥¿INT_8_TYPE & UINT_8_TYPE
            if(P!=NULL)
                Max=*P;
            else
                Max=EC_Ptr_Max_Value->Strings[i];

            P=(__int64 *)EC_Ptr_Default->Items[i];                              //Steven 20140911 : ­×¥¿INT_8_TYPE & UINT_8_TYPE
            if(P!=NULL)
                Default=*P;
            else
                Default=EC_Ptr_Default_Value->Strings[i];
        }
        else if(t==HType.FT_4_TYPE)                                             //Steven 20130730 ADD
        {
            float *P;
            P=(float *)EC_Ptr_Min->Items[i];
            if(P!=NULL)
                Min=*P;
            else
                Min=EC_Ptr_Min_Value->Strings[i];

            P=(float *)EC_Ptr_Max->Items[i];
            if(P!=NULL)
                Max=*P;
            else
                Max=EC_Ptr_Max_Value->Strings[i];

            P=(float *)EC_Ptr_Default->Items[i];
            if(P!=NULL)
                Default=*P;
            else
                Default=EC_Ptr_Default_Value->Strings[i];
        }
        else if(t==HType.FT_8_TYPE)                                             //Steven 20130730 ADD
        {
            double *P;
            P=(double *)EC_Ptr_Min->Items[i];
            if(P!=NULL)
                Min=*P;
            else
                Min=EC_Ptr_Min_Value->Strings[i];

            P=(double *)EC_Ptr_Max->Items[i];
            if(P!=NULL)
                Max=*P;
            else
                Max=EC_Ptr_Max_Value->Strings[i];

            P=(double *)EC_Ptr_Default->Items[i];
            if(P!=NULL)
                Default=*P;
            else
                Default=EC_Ptr_Default_Value->Strings[i];
        }

        Remark=EC_Remark->Strings[i];

        if(memoPtr!=NULL)
        {
            S=ID+'\t'+IsSV+'\t'+IsEC+'\t'+Name+'\t'+Type+'\t'+Length+'\t'+Unit+'\t'+Max+'\t'+Min+'\t'+Default+'\t'+Remark;
            memoPtr->Lines->Add(S);
        }
        else if(sgPtr!=NULL)
        {
            iRow=sgPtr->RowCount;
            sgPtr->RowCount=sgPtr->RowCount+1;
            sgPtr->Cells[0][iRow]=ID;
            sgPtr->Cells[1][iRow]=Name;
            sgPtr->Cells[2][iRow]=Length;
            sgPtr->Cells[3][iRow]=Type;
            sgPtr->Cells[4][iRow]=Min;
            sgPtr->Cells[5][iRow]=Max;
            sgPtr->Cells[6][iRow]=Unit;
            sgPtr->Cells[7][iRow]=Default;
            sgPtr->Cells[8][iRow]=Remark;
        }
    }
}
//##############################################################################
//##############################################################################
//
// ¥D­nªº Define Report ¥Î¨ìªº»²§U¨ç¼Æ
//
//##############################################################################
//##############################################################################
/*
L,2
1. <CEED> ChangeToFloatNonPcnt((double)(enable), (double)(disable))  boolean
2. L,n #CEIDs
    1. <CEID1>
      .
      .
    n. <CEIDn>
*/
//------------------------------------------------------------------------------
void __fastcall THGem::EnableDisableEventReport(bool CEED, int slen, unsigned *CEID)
{
    if(slen==0)                                                                 // all
    {
        for(int y=1; y<strGrdCEID->RowCount; y++)
        {
            if(strGrdCEID->Cells[0][y]!="")
            {
                if(CEED==true)
                    strGrdCEID->Cells[1][y]="1";
                else
                    strGrdCEID->Cells[1][y]="0";
            }
        }
    }
    else
    {
        for(int i=0; i<slen; i++)
        {
            for(int y=1; y<strGrdCEID->RowCount; y++)
            {
                if(strGrdCEID->Cells[0][y]==CEID[i])
                {
                    if(CEED==true)
                        strGrdCEID->Cells[1][y]="1";
                    else
                        strGrdCEID->Cells[1][y]="0";
                }
            }
        }
    }
    SaveEventReportData();
    EnableDisableEventReportAcknowledgeError(0x00);
}
//------------------------------------------------------------------------------
void __fastcall THGem::SaveEventReportData()
{
    TStringList *memoPtr;
    AnsiString Filename;

    memoPtr=new TStringList;
    CopyStringGridAsTabFormat(strGrdCEID,memoPtr);
    Filename="D:\\HT9045\\SECS\\SECS\\SYSTEM\\EventReport_CEID.def";
    memoPtr->SaveToFile(Filename);

    CopyStringGridAsTabFormat(stdGridReportID,memoPtr);
    Filename="D:\\HT9045\\SECS\\SECS\\SYSTEM\\EventReport_ReportID.def";
    memoPtr->SaveToFile(Filename);

    delete memoPtr;
}
//##############################################################################
//##############################################################################
//
// ¥D­nªº S,F ªº³B²z¥Î¨ìªº»²§U¨ç¼Æ
//
//##############################################################################
//##############################################################################
//==============================================================================
// 2013/06/29
// V1.1
// Lee
// ³]©w³q°T address , port ªì©l­È
//==============================================================================
bool THGem::CheckSFFormatOnlyHead(AnsiString ErrStr)
{
    int ret, len;
    unsigned char Type;

    if(chkMoreMessageAbortProcess->Checked==false)
        return true;

    ret=GetDataItemLenAndTypeAndDelete(len, Type);
    if(ret!=1)
        return true;
    StringOut(ErrStr);
    SendInvalidDataMessageToHost(ErrStr);
    return false;
}
//##############################################################################
//##############################################################################
//
// ¥D­nªº S,F ªº³B²z
//
//##############################################################################
//##############################################################################

//---------------------------------------------------------------------------
// 2013/05/27
// V1.1
// control message SelectRequest Response
//---------------------------------------------------------------------------
void THGem::SelectRsp()
{
    unsigned short back;
    Local.MessageID_S=0;
    Local.MessageID_F=0;
    Local.W_Bit=0;
    Local.PType=0;
    Local.SType=SType.Select_rsp;
    Local.SystemByte=RemoteSystemByte;
    back=Local.DeviceID;
    Local.DeviceID=Remote.DeviceID;
    LocalLength=0;
    CreateLocalHead();
    StringOut("[Send]    Select.rsp");
    SendLocalData();
    Local.DeviceID=back;
}
//---------------------------------------------------------------------------
// 2013/05/27
// V1.1
// control message DeSelectRequest Response
//---------------------------------------------------------------------------
void THGem::DeselectRsp()
{
    unsigned short back;
    Local.MessageID_S=0;
    Local.MessageID_F=0;
    Local.W_Bit=0;
    Local.PType=0;
    Local.SType=SType.Deselect_rsp;
    Local.SystemByte=RemoteSystemByte;
    back=Local.DeviceID;
    Local.DeviceID=Remote.DeviceID;
    LocalLength=0;
    CreateLocalHead();
    StringOut("[Send]    Deselect.rsp");
    SendLocalData();
    Local.DeviceID=back;
}
//---------------------------------------------------------------------------
// 2013/05/27
// V1.1
// control message LinkTest Response
//---------------------------------------------------------------------------
void THGem::LinktestRsp()
{
    Local.MessageID_S=0;
    Local.MessageID_F=0;
    Local.W_Bit=0;
    Local.PType=0;
    Local.SType=SType.Linktest_rsp;
    Local.SystemByte=RemoteSystemByte;
    LocalLength=0;
    CreateLocalHead();
    StringOut("[Send]    Linktest.rsp");
    LocalBuffer[4]=0xff;
    LocalBuffer[5]=0xff;
    SendLocalData();
}
//==============================================================================
// 2013/05/27
// V1.1
// Lee
// ¥D­n¹ï Stream  ©M function code ªº³B²z
//==============================================================================
void THGem::ProcessReceiceData()
{
    AnsiString S;

    if(Remote.SType==SType.Select_req)
    {
        RemoteSystemByte=Remote.SystemByte;
        StringOut("[Receive] Select.req");
        SelectRsp();
    }
    else if(Remote.SType==SType.Select_rsp)
    {
        StringOut("[Receive] Select.rsp");
        bWaitSelectRsp=true;
    }
    else if(Remote.SType==SType.Deselect_req)
    {
        RemoteSystemByte=Remote.SystemByte;
        StringOut("[Receive] Deselect.req");
        DeselectRsp();
    }
    else if(Remote.SType==SType.Deselect_rsp)
    {
        StringOut("[Receive] Deselect.rsp");
        bWaitDeSelectRsp=true;
    }
    else if(Remote.SType==SType.Linktest_req)
    {
        RemoteSystemByte=Remote.SystemByte;
        StringOut("[Receive] Linktest.req");
        LinktestRsp();
    }
    else if(Remote.SType==SType.Separate_req)
    {
        RemoteSystemByte=Remote.SystemByte;
        StringOut("[Receive] Separate.req");
        if(bConnect==true)
            bAutoConnect=true;
        bConnect=false;
    }
    else
    {
        if(bDataFormatOK==false)
        {
            SendInvalidDataMessageToHost("Data Format Error");
            return ;
        }
        CheckSFCodeResponse();

        if(Remote.MessageID_S==1 && Remote.MessageID_F==1)                      // [S1F1] Are you There
        {
            HSys.MyGem->S1F2_OnLineData();                                      // [S1F2] On Line Data",
        }
        else if(Remote.MessageID_S==1 && Remote.MessageID_F==2)                 // [S1F2] On Line Data
        {
            StringOut("[On Line Data]");
            bS1F2_OnLineData=true;
        }
        if(Remote.MessageID_S==1 && Remote.MessageID_F==13)                     // [S1F13] Connect Request
        {
            HSys.MyGem->S1F14_ConnectRequestAcknowledge();                      // [S1F14] Connect Request Acknowledge
        }
        else if(Remote.MessageID_S==1 && Remote.MessageID_F==17)                // [S1F17] Request ON-LINE
        {
            HSys.MyGem->S1F18_ONLINEAcknowledge();                              // [S1F18] ON-LINE Acknowledge
        }
        else if(Remote.MessageID_S==2 && Remote.MessageID_F==15)                // [S2F15] New Equipment Constant Send
        {
            HSys.MyGem->S2F16_NewEquipmentConstantSendAcknowledge();            // [S2F16] New Equipment Constant Send Acknowledge
        }
        else
        {
            if((CUSTOMER_CODE==CC_TFME_CHINA && GemControlState>1) || CUSTOMER_CODE!=CC_TFME_CHINA)  //JerryYang 20200527 Onlineª¬ºA¤~±µ¦¬³s½u¥H¥~S,F code
            {
                if(MoveCheckCallBack!=NULL)
                    if(MoveCheckCallBack()==1)                                  // ¦¹ S,F Code ¬O§_­n¥Ñ Application ¨Ó¦^À³
                        return;

                if(Remote.MessageID_S==1 && Remote.MessageID_F==3)              // [S1F3] Selected Status Request
                    HSys.MyGem->S1F4_SelectedStatusReply();                     // [S1F4] Selected Status Reply
                else if(Remote.MessageID_S==1 && Remote.MessageID_F==11)        // [S1F11] Status Variable Namelist Request
                    HSys.MyGem->S1F12_StatusVariableNamelistReply();            // [S1F12] Status Variable Namelist Reply
                else if(Remote.MessageID_S==1 && Remote.MessageID_F==14)        // [S1F14] Connect Request Acknowledge
                    HSys.MyGem->Process_S1F14_ConnectRequestAcknowledge();
                else if(Remote.MessageID_S==1 && Remote.MessageID_F==15)        // [S1F15] Request OFF-LINE
                    HSys.MyGem->S1F16_OFFLINEAcknowledge();                     // [S1F16] OFF-LINE Acknowledge
                else if(Remote.MessageID_S==1 && Remote.MessageID_F==23)        // [S1F23] Collection Event Namelist Request          //2014/01/01  lee
                    HSys.MyGem->S1F24_CollectionEventNamelist();                // [S1F24] Collection Event Namelist                  //2014/01/01  lee
                else if(Remote.MessageID_S==2 && Remote.MessageID_F==13 )       // [S2F13] Equipment Constant Request
                    HSys.MyGem->S2F14_EquipmentConstanData();                   // [S2F14] Equipment Constant Data
                else if(Remote.MessageID_S==2 && Remote.MessageID_F==17)        // [S2F17] Date and Time Request
                    HSys.MyGem->S2F18_DateandTimeData();                        // [S2F18] Date and Time Data
                else if(Remote.MessageID_S==2 && Remote.MessageID_F==23)        // [S2F23] Trace Initial Send
                    HSys.MyGem->S2F24_TraceInitializeAcknowledge();             // [S2F24] Trace Initial Acknowledge
                else if(Remote.MessageID_S==2 && Remote.MessageID_F==25)        // [S2F25] Diagnostic Loopback Request
                    HSys.MyGem->S2F26_DiagnosticLoopbackData();                 // [S2F26] Diagnostic Loopback Data
                else if(Remote.MessageID_S==2 && Remote.MessageID_F==29)        // [S2F29] Equipment Constant Namelist Request
                    HSys.MyGem->S2F30_EquipmentConstantNamelistReply();         // [S2F30] Equipment Constant Namelist Reply
                else if(Remote.MessageID_S==2 && Remote.MessageID_F==31)        // [S2F31] Date and Time Send
                    HSys.MyGem->S2F32_DateAndTimeAcknowledge();                 // [S2F32] Date and Time Acknowledge
                else if(Remote.MessageID_S==2 && Remote.MessageID_F==33)        // [S2F33] Define Report
                    HSys.MyGem->S2F34_DefineReportAcknowledge();                // [S2F34] Define Report Acknowledge
                else if(Remote.MessageID_S==2 && Remote.MessageID_F==35)        // [S2F35] Link Event Report
                    HSys.MyGem->S2F36_LinkEventReportAcknowledge();             // [S2F36] Link Event Report Acknowledge
                else if(Remote.MessageID_S==2 && Remote.MessageID_F==37)        // [S2F37] Enable/Disable Event Report
                    HSys.MyGem->S2F38_EnableDisableEventReportAcknowledge();    // [S2F38] Enable/Disable Event Report Acknowledge
                else if(Remote.MessageID_S==2 && Remote.MessageID_F==43)        // [S2F37] Enable/Disable Event Report
                    HSys.MyGem->S2F44_ResetSpoolingAcknowledge();               // [S2F43] Enable/Disable Event Report Acknowledge
                else if(Remote.MessageID_S==5 && Remote.MessageID_F==3 )        // [S5F3] Enable/Disable Alarm Send
                    HSys.MyGem->S5F4_EnableDisableAlarmAcknowledge();           // [S5F4] Enable/Disable Alarm Acknowledge
                else if(Remote.MessageID_S==5 && Remote.MessageID_F==5)         // [S5F5] List Alarm Request
                    HSys.MyGem->S5F6_ListAlarmData();                           // [S5F6] List Alarm Data
                else if(Remote.MessageID_S==5 && Remote.MessageID_F==7)         // [S5F7] List Enable Alarm Request
                    HSys.MyGem->S5F8_ListEnableAlarmAcknowledge();              // [S5F8] List Enable Alarm Acknowledge
                else if(Remote.MessageID_S==6 && Remote.MessageID_F==15)        // [S6F15] Event Report Request
                    HSys.MyGem->S6F16_EventReportData();                        // [S6F16] Event Report Data
                else if(Remote.MessageID_S==6 && Remote.MessageID_F==17)        // [S6F17] Annotated Event Report Request
                    HSys.MyGem->S6F18_AnnotatedEventReportData();               // [S6F18] Annotated Event Report Data
                else if(Remote.MessageID_S==6 && Remote.MessageID_F==19)        // [S6F19] Individual Report Request
                    HSys.MyGem->S6F20_IndividualReportData();                   // [S6F20] Individual Report Data
                else if(Remote.MessageID_S==6 && Remote.MessageID_F==23)        // [S6F23] Request Spooled Data
                    HSys.MyGem->S6F24_RequestSpooledDataAcknowledgementSend();  // [S6F24] Request Spooled Data Acknowledgement Send
                else if(Remote.MessageID_S==7 && Remote.MessageID_F==1)         // [S7F1] Process Program Load Inquire
                    HSys.MyGem->S7F2_ProcessProgramLoadGrant();                 // [S7F2] Process Program Load Grant !!!!!!!!!!!!!!
                else if(Remote.MessageID_S==7 && Remote.MessageID_F==3)         // [S7F1] Process Program Load Inquire
                    HSys.MyGem->S7F4_ProcessProgramAcknowledge();               // [S7F2] Process Program Load Grant !!!!!!!!!!!!!!
                else if(Remote.MessageID_S==7 && Remote.MessageID_F==5)         // [S7F5] Process Program Request
                    HSys.MyGem->S7F6_ProcessProgramData();                      // [S7F6] Process Program Data
                else if(Remote.MessageID_S==7 && Remote.MessageID_F==6)         // [S7F6] Process Program Data
                    bReceiveS7F6=true ;
                else if(Remote.MessageID_S==7 && Remote.MessageID_F==17)        // [S7F17] Delete Process Program Send
                    HSys.MyGem->S7F18_DeleteProcessProgramAcknowledge();        // [S7F18] Delete Process Program Acknowledge
                else if(Remote.MessageID_S==7 && Remote.MessageID_F==19)        // [S7F19] Current EPPID Request
                    HSys.MyGem->S7F20_CurrentEPPDData();                        // [S7F20] Current EPPID Data
                else if(Remote.MessageID_S==7 && Remote.MessageID_F==20)        // [S7F20] Current EPPID Data
                    HSys.MyGem->Process_S7F20_CurrentEPPIDData();
                else if(Remote.MessageID_S==10 && Remote.MessageID_F==3)        // [S10F3] Terminal Display Single
                    HSys.MyGem->S10F4_TerminalDisplaySingleAcknowledge();       // [S10F4] Terminal Display Single Acknowledge
                else if(Remote.MessageID_S==10 && Remote.MessageID_F==5)        // [S10F5] Terminal Display Multi-block
                    HSys.MyGem->S10F6_TerminalDisplayMultiBlockAcknowledge();   // [S10F6] Terminal Display Multi-block Acknowledge
                else if(HGem->Remote.MessageID_S==14 && HGem->Remote.MessageID_F==3)    // [S14F3] receive 2DID & bin code
                    HSys.MyGem->S14F4_Get2DID_BinCode();
                else if(Remote.MessageID_S==100 && Remote.MessageID_F==3)
                    HSys.MyGem->S100F4_ReportAllAlarm();
                else if(Remote.MessageID_S==101 && Remote.MessageID_F==1)
                    HSys.MyGem->S101F2_CurrentEPPDData();
                else if(Remote.MessageID_S==101 && Remote.MessageID_F==3)
                    HSys.MyGem->S101F4_CurrentEPPDData();
                else if(Remote.MessageID_S==101 && Remote.MessageID_F==5)
                    HSys.MyGem->S101F6();                                       //¤U¸üHOST RECIPE
                else if(Remote.MessageID_S==101 && Remote.MessageID_F==6)
                    bReceiveS101F6=true;
                else if(Remote.MessageID_S==101 && Remote.MessageID_F==7)
                    HSys.MyGem->S101F8();
                else if(Remote.MessageID_S==101 && Remote.MessageID_F==8)
                    bReceiveS101F8=true;
                else if(Remote.MessageID_S==103 && Remote.MessageID_F==11)      // [S103F11] Status Variable Namelist Request with Value   //Steven 20140911 : Add S103F1
                    HSys.MyGem->S103F12_StatusVariableNamelistReply();          // [S103F12] Status Variable Namelist Reply with Value
                //pig 2014.04.01 ASEM SECS start
                else if(Remote.MessageID_S==110 && Remote.MessageID_F==2)
                    bReceiveS110F2=true;
                //pig 2014.07.04 ASEM SECS GEM start
                else if(Remote.MessageID_S==110 && Remote.MessageID_F==6)
                    HSys.MyGem->S110F6_ListCustomerName();
                else if(Remote.MessageID_S==110 && Remote.MessageID_F==8)
                    HSys.MyGem->S110F8_ListReceipeInformation();
                else if(Remote.MessageID_S==120 && Remote.MessageID_F==2)
                    HSys.MyGem->S120F2_ListReceipeSetupFile();
                //pig 2014.07.04 ASEM SECS GEM end
                //pig 2014.08.27 start
                else if(Remote.MessageID_S==125 && Remote.MessageID_F==1)       // [S125F1] Enable/Disable EC Data Send
                    HSys.MyGem->S125F2_EnableDisableECDataAcknowledge();        // [S125F2] Enable/Disable EC Data Acknowledge
                //pig 2014.08.27 end
                else if(Remote.MessageID_S==125 && Remote.MessageID_F==3)       // [S125F3] Level Setting Change Request
                    HSys.MyGem->S125F4_LevelSettingChangeAcknowledge();         // [S125F4] Level Setting Change Acknowledge    //Steven 20150605 : S125F3 LevelSettingChangeRequest
                else
                {
                    //  LocalAcknowledge(Remote.MessageID_S,Remote.MessageID_F+1,0);
                    //  if(MoveCheckCallBack!=NULL)
                    //      MoveCheckCallBack();
                    if(CUSTOMER_CODE==CC_ASE_KaohSiung_K3)                      //KenHsieh 20221006 : ASE_K1­n¨D¤U¿ù»~S,F Code®É¦^¶ÇS9F3
                        HSys.MyGem->S9F3_Unrecognized_Stream_Function_Type("S,F Code Not Define");
                }
            }
        }
    }
}
//==============================================================================
// 2013/05/27
// V1.1
// Lee
// ¥D­n§ïµ½·í Host ©ó·¥µu®É¶¡°e¥X 2 ­Ó SF code ¥X¨Ó
// ¨º¸ê®Æ±N·|³Q¦X¦¨¤@µ§¸ê®Æ»Ý­n³Q¥¿±`³B²z
//==============================================================================
void reverae_array(unsigned char arr[], int length)                             //Ifor 20161007 °}¦C°ª§C¦ì¸mÂà´«
{
    int temp;
    for(int i=0; i<length/2; i++)
    {
        temp=arr[i];
        arr[i]=arr[length-i-1];
        arr[length-i-1]=temp;
    }
}
//==============================================================================
void __fastcall THGem::clientGemRead(TObject *Sender, TCustomWinSocket *Socket)
{
    pLockOnSocketRecvice->Acquire();                                            //16.10.05.00 Roy Add (Debug from kirin) ¥ýÂê¦í­n³B²zªº¸ê®Æ.
    try                                                                         //16.10.05.00 Roy Add (Debug from kirin)
    {
        unsigned char *EthernetBuffer;
        int iBufferLenght=0;
        iBufferLenght=Socket->ReceiveLength();
        if(iBufferLenght>0)                                                     //13.09.30.01   klutter
        {
            EthernetBuffer=new unsigned char [iBufferLenght];                   //checked
            Socket->ReceiveBuf(EthernetBuffer, iBufferLenght);                  //§â³o¤@¦¸­n³B²zªº head + data Åª§¹
            RecvMemoryBuffer->WriteBuffer(EthernetBuffer, iBufferLenght);
            //delete []EthernetBuffer;                                          //16.09.14.00 Roy Add  ³o¸Ì¤d¸U¤£¯àdelete®@!!
        }
    }
    __finally                                                                   //16.10.05.00s Roy Add (Debug from kirin)
    {
        pLockOnSocketRecvice->Release();                                        //µ¥¸ê®Æ³B²z§¹¦A©ñ¶}.
    }                                                                           //16.10.05.00e
}
//------------------------------------------------------------------------------
void __fastcall THGem::ProcessSocketReceiveData()
{
    AnsiString S="", SFCode;
    char str[64];
    int i, ret;
    unsigned char *EthernetBuffer;
    unsigned char *TempEthernetBuffer=NULL;
    unsigned char *ProcessBuffer;
    int Value,iBufferLenght=0;
    int TempLen=0;
    unsigned char *p;
    static bool bInProcess=false;
    int iProcCount=0;
    AnsiString asFileName="";

    if(bInProcess)
        return;

    bInProcess=true;

    if(srvGem->Socket->ActiveConnections>1)                                     //2013/09/30  lee
    {
        bInProcess=false;
        return;                                                                 //2013/09/30  lee
    }

    if(RecvMemoryBuffer->Size==0 && ProcBuffer->Size==0)
    {
        bInProcess=false;
        return;
    }

    if(RecvMemoryBuffer->Size>0)
    {
        TempLen=RecvMemoryBuffer->Size;
        TempEthernetBuffer=new unsigned char[TempLen];
        RecvMemoryBuffer->Position=0;
        RecvMemoryBuffer->ReadBuffer(TempEthernetBuffer, TempLen);
        RecvMemoryBuffer->Clear();

        ProcBuffer->Seek(0, soFromEnd);
        ProcBuffer->WriteBuffer(TempEthernetBuffer, TempLen);

        iFileCount++;
        if(iFileCount==0x7FFFFFFF)                                              //16.11.04.01 s kirin
        {
            iFileCount=0;
        }                                                                       //16.11.04.01 e kirin
    }

    iBufferLenght=ProcBuffer->Size;
    EthernetBuffer=new unsigned char[iBufferLenght]; //test
    ProcBuffer->Seek(0, soFromBeginning);
    ProcBuffer->ReadBuffer(EthernetBuffer, iBufferLenght);

    do
    {
        SReceiveData->Clear();                                                  //16.11.04.01 kirin
        bReceiveData=true;                                                      //16.11.04.01 kirin
        {
            if((iBufferLenght)<14)                                              //¹w¨¾ªø«×¤p©ó14 ¦]¬° Size byte=4 ,Head Byte=10 ¬G¦Ü¤Ö 14 byte    2013/04/11 Lee
            {
                if(iBufferLenght!=0)
                {
                    ProcBuffer->Clear();                                        //test
                    StringOut(AnsiString("Err : Socket Buffer Length less than 14 byte "));
                    bInProcess=false;
                    return;
                }
                else
                {
                    ProcBuffer->Clear();                                        //test
                    bInProcess=false;
                    return;
                }
            }
            p=(unsigned char *)&Value;
            for(i=0; i<4; i++)
                p[3-i]=EthernetBuffer[i+iProcCount];                            //Value=¨ú±o³o¦¸¶Ç°eªºÁ`ªø«×

            if(Value>104857600)// || Value>16000000)                            //­Y¶Ç°eªº size «Ê¥]ªø«×¤j©ó100MB«h»{¬°¬O²§±`       //16.10.05.00 Roy Change    (Debug from kirin,Add : Value>16000000)
            {
                ProcBuffer->Clear(); //test
                StringOut(AnsiString("Err : Size Length over 100M byte "));
                bInProcess=false;
                return;
            }
            else if((iBufferLenght-iProcCount)>=(Value+4))                      //¦Ü¤Ö¦³¤@µ§¸ê®Æ¶i¨Ó¤F   //16.11.02.01 kirin test
            {
                ProcessBuffer=new unsigned char[Value+4];
                memcpy(ProcessBuffer, &EthernetBuffer[iProcCount], Value+4);
                iProcCount+=(Value+4);
            }
            else if((iBufferLenght-iProcCount)<(Value+4))                       //­Y Buffer ªº¸ê®Æ¤ñ­n¶Çªº¤Ö,ªí¥Ü·|¦A¦¸¶i¨Ó(¨C­Ó«Ê¥]ªø«×¦³­­¨î) //16.11.02.01 kirin test
            {
                TempProcBuffer->Position=0;
                TempProcBuffer->WriteBuffer(&EthernetBuffer[iProcCount], iBufferLenght-iProcCount);
                ProcBuffer->Clear();
                TempProcBuffer->Position=0;
                ProcBuffer->LoadFromStream(TempProcBuffer);
                TempProcBuffer->Clear();
                bFirstBlock =false;
                delete []EthernetBuffer;                                        //13.09.25.01   klutter
                EthernetBuffer=NULL;                                            //test
                bInProcess=false;
                return;
            }
        }

        StringOut("===================================================");
        GetTimeInfo();
        S=AnsiString("[Receive] ")+TimeString;
        StringOut(S);

        SFCode=ShowSFDescription(ProcessBuffer[6]&0x7f, ProcessBuffer[7]);      //Steven 20180815 : °w¹ïS6F12³s¦¬¨â¦¸ªº­×¥¿ ¦^¶ÇSFCode

        bDataFormatOK=true;
        if(GemCheckBoxShowBinary->Checked==true)                                //Steven 20211109 : SECS¤£­n¨C¦¸³£Åã¥Übinary code
        {
            StringBinaryOut("");
            S="";
            for(i=0; i<4; i++)                                                  //¼ÐÀY¥|½X
            {
                sprintf(str, "%02X,", ProcessBuffer[i]);
                S+=AnsiString(str);
            }
            StringBinaryOut(S);
            S="";
            for(i=4; i<14; i++)                                                 //SF Code 10½X
            {
                sprintf(str,"%02X,", ProcessBuffer[i]);
                S+=AnsiString(str);
            }
            StringBinaryOut(S);
            ret=ShowSMLBinary(ProcessBuffer, Value+4);                          //¤À¸Ñ¶Ç¤Jªº¸ê®Æ
            if(ret==-2)
                bDataFormatOK=false;
        }
        ProcessRemoteHead(ProcessBuffer);
        SReceiveData->Clear();

        if((Remote.MessageID_S==101 && Remote.MessageID_F==5) ||                //pig 2014.04.01 ASEM SECS
           (Remote.MessageID_S==101 && Remote.MessageID_F==11))
            bDisableBinaryShow=true;

        ShowSML(ProcessBuffer, Value+4);

        bDisableBinaryShow=false;
        ProcessReceiceData();
        SaveSECSGEMTextToLog();

        if((iBufferLenght-iProcCount)==0)
        {
            if(bFirstBlock==true)
            {
                EthernetBuffer=NULL;                                            //test
            }
            ProcBuffer->Clear();                                                //test
            bReceiveData=false;
            bFirstBlock =true ;
            bInProcess=false;
            return;
        }
    }while(1);
}
//---------------------------------------------------------------------------
//2013/12/01 lee start
void __fastcall THGem::GemRemoteReceipeListClick(TObject *Sender)
{
    int y=GemRemoteReceipeList->ItemIndex;
    for(int i=0; i<GemRemoteReceipeList->Items->Count; i++)
    {
        if(i==y)
            continue;
        else
            GemRemoteReceipeList->Checked[i]=false;
    }
}
//---------------------------------------------------------------------------
//2013/12/01 lee end
//==============================================================================
//                              Read EC Enable Data
//==============================================================================
bool __fastcall THGem::ReadECEnableData()
{
    TStringList *memoPtr;
    AnsiString Filename;

    memoPtr=new TStringList;
    Filename=IncludeTrailingPathDelimiter(GemSystemPath)+AnsiString("ECEnableData.def");
    if(FileExists(Filename)==false)
    {
        delete memoPtr;                                                         //16.09.14.00 Roy Add
        return false;
    }
    memoPtr->LoadFromFile(Filename);
    while(1)
    {
        if(memoPtr->Strings[memoPtr->Count-1]=="")
            memoPtr->Delete(memoPtr->Count-1);
        else
            break;

        if( memoPtr->Count==0)
            break;
    }
    sgSECSECData->RowCount=memoPtr->Count;
    PasteStringGridAsTabFormat(sgSECSECData, memoPtr);
    delete memoPtr;
    return true;
}
//==============================================================================
//                              Write EC Enable Data
//==============================================================================
void __fastcall THGem::WriteECEnableData()
{
    TStringList *memoPtr;
    AnsiString Filename;

    memoPtr=new TStringList;
    CopyStringGridAsTabFormat(sgSECSECData, memoPtr);

    Filename=IncludeTrailingPathDelimiter(GemSystemPath)+AnsiString("ECEnableData.def");
    memoPtr->SaveToFile(Filename);

    delete memoPtr;
}
//==============================================================================
//                         Enable Disable EC Data
//==============================================================================
bool THGem::EnableDisableECData(AnsiString ID, unsigned char T)
{
    for(int y=1; y<sgSECSECData->RowCount; y++)
    {
        if(sgSECSECData->Cells[1][y]==ID)
        {
            if(T&0x80)                                                          //Steven 20150603 : T&0x10 --> T&0x80
                sgSECSECData->Cells[2][y]="1";
            else
                sgSECSECData->Cells[2][y]="0";
            WriteECEnableData();
            return true;
        }
    }
    return false;
}
//==============================================================================
//                         Enable Disable All EC Data
//==============================================================================
void THGem::EnableDisableECDataAll(unsigned char T)
{
    for(int y=1; y<sgSECSECData->RowCount; y++)
    {
        if(T&0x80)                                                              //Steven 20150603 : T&0x10 --> T&0x80
            sgSECSECData->Cells[2][y]="1";
        else
            sgSECSECData->Cells[2][y]="0";
    }
    WriteECEnableData();
}
//==============================================================================
//                              Get EC Enable
//==============================================================================
bool __fastcall THGem::GetECEnableData(AnsiString ID)
{
    for(int i=1; i<sgSECSECData->RowCount; i++)
    {
        if(sgSECSECData->Cells[1][i]==ID)
        {
            if(sgSECSECData->Cells[2][i]=="1")
                return true;
            else
                return false;
        }
    }
    return false;
}
//==============================================================================
//                            SECS Set EC Data Code
//==============================================================================
void __fastcall THGem::SetECEnableData(AnsiString ECID,AnsiString Function)
{
    int ct=sgSECSECData->RowCount;
    sgSECSECData->RowCount++;
    if(ct==1)
        sgSECSECData->FixedRows=1;

    sgSECSECData->Cells[0][ct]=ct;
    sgSECSECData->Cells[1][ct]=ECID;
    sgSECSECData->Cells[2][ct]="0";                                             // enable or disable
    sgSECSECData->Cells[3][ct]=Function;
}
//---------------------------------------------------------------------------
void __fastcall THGem::edtPortClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_PORT);
}
//---------------------------------------------------------------------------
void __fastcall THGem::edtT3TimeOutClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 10, 2000);
}
//---------------------------------------------------------------------------
void __fastcall THGem::btnExportClick(TObject *Sender)
{
    SaveDialog1->FileName="D:\\AlarmList.xls";                                  //Steven 20120921 : ¦sÀÉ«OÅ@
    if(SaveDialog1->Execute())
        SGDToXLS(strGrdAlarm, SaveDialog1->FileName);
}
//---------------------------------------------------------------------------

