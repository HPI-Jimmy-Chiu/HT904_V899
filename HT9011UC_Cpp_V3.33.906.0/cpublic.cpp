//AI(W0-TAIL) 20260626: de-VCL cpublic.cpp. Dropped MachineDefine.h (VCL+drivers)
//  and the app headers (cmydef/rs232/main/common/MyMotor/MD5/note/...). Kept only
//  cpublic.h + vclcompat + windows.h for the standalone W0 compile.
//  ENTIRE body gated TODO(W3/W5/W6/W7): even the union/queue method bodies reach
//  cmydef globals (SystemHour/SystemYear/asGalilCmdPath/QueueCycleTime/tCycleTimer)
//  and the free fns reach rs232/main/common/MyMotor/HTMD5/note. cpublic.cpp defines
//  NO extern global declared in the W0 headers (only free fns + the cpublic.h methods),
//  so gating it loses nothing for the W0 header-compile contract. Bodies are preserved
//  verbatim below for faithful re-activation when those globals/headers land.
#include "vclcompat/vcl_compat.h"
#include <windows.h>
#include "cpublic.h"
// =============================================================================
//  AI(W4) 20260626: Ungated queue ctors/dtors + ClearData helpers.
//
//  These are required because cmydef.cpp defines static global objects of these
//  types (QueueTaskList[], QueueGalilCmd, QueueTestTime, etc.) whose constructors
//  fire at static-init time.  The ctors and their ClearData helpers have NO
//  external dependencies (only zero-fill loops and AnsiString assignment), so
//  they can be activated now even though the rest of cpublic.cpp is gated W6/W7.
//  Without these the test_motor_w4 link fails with "undefined reference to ctor".
// =============================================================================

// ---------------------------------------------------------------------------
//  TMyQueue10
// ---------------------------------------------------------------------------
TMyQueue10::TMyQueue10()
{
    Alias = "";
    ClearData();
    bInt = true;
}

void TMyQueue10::ClearData()
{
    for (int i = 0; i < MAX_Q_10; i++)
    {
        iData[i]    = 0;
        dData[i]    = 0.0;
        DateTime[i] = "";
    }
    iIndex = -1;
    iCount = -1;
}

// ---------------------------------------------------------------------------
//  TMyQueue100
// ---------------------------------------------------------------------------
TMyQueue100::TMyQueue100()
{
    ClearData();
}

void TMyQueue100::ClearData()
{
    for (int i = 0; i < MAX_Q_100; i++)
    {
        iData[i]    = 0;
        dData[i]    = 0.0;
        DateTime[i] = "";
    }
    iIndex = 0; //AI(ht9045-v899) 20260626: Queue100 init 0 not -1; Add() uses iData[iCount] as write slot
    iCount = 0; //AI(ht9045-v899) 20260626: same
}

// ---------------------------------------------------------------------------
//  TMyStrQueue100
// ---------------------------------------------------------------------------
TMyStrQueue100::TMyStrQueue100()
{
    sLastFileName = "";
    ClearData();
}

void TMyStrQueue100::ClearData()
{
    for (int i = 0; i < MAX_Q_100; i++)
    {
        sData1[i]   = "";
        sData2[i]   = "";
        DateTime[i] = "";
    }
    iIndex = 0;
    iCount = 0;
}

// ---------------------------------------------------------------------------
//  TMyTimerQueue100
// ---------------------------------------------------------------------------
TMyTimerQueue100::TMyTimerQueue100()
{
    sLastFileName = "";
    ClearData();
}

void TMyTimerQueue100::ClearData()
{
    for (int i = 0; i < MAX_Q_100; i++)
    {
        sStartTime[i] = "";
        sEndTime[i]   = "";
        DateTime[i]   = "";
        sTimeStr[i]   = "";
        iTime[i]      = 0;
    }
    iIndex = -1;
    iCount = -1;
}

// =============================================================================
//  AI(ht9045-v899) 20260626: UNGATE foundation-only bodies.
//
//  This wave ungates everything that depends only on:
//    vclcompat (AnsiString / TStringList / TDateTime / SysUtils / Comm.h)
//    windows.h (Win32 version/process API)
//    cpublic.h (own class declarations + VKINFO / VERSION_INFO_KEY macros)
//    cmydef.h  (System* time globals, QueueXxx, tXxxTimer, StartXxxTime)
//    common.h  (asGalilCmdPath, IniConfig)
//    Public/HTMD5.h  (md5_Folder, SearchFile)
//    cUnitConvert.h  (ChangeToFloatNonPcnt)
//
//  Additional headers needed for the ungated bodies:
// =============================================================================
#include "cmydef.h"           // SystemYear/Month/Date/Hour/Min/Sec/MSec, QueueXxx, tXxxTimer
#include "common.h"           // asGalilCmdPath, IniConfig, cUnitConvert via include chain
#include "Public/HTMD5.h"     // md5_Folder, SearchFile
#include "cUnitConvert.h"     // ChangeToFloatNonPcnt
#include <psapi.h>            // EnumProcesses / GetModuleBaseName
#include <stdexcept>          // std::runtime_error (substitute for BCB6 Exception)
#include <cmath>              // fabs, pow, cos, sin

// ---------------------------------------------------------------------------
//  Local shims for SysUtils helpers not yet in vclcompat
// ---------------------------------------------------------------------------
// IncludeTrailingPathDelimiter -- BCB6 synonym for IncludeTrailingBackslash
static inline AnsiString IncludeTrailingPathDelimiter(const AnsiString& p)
{
    return IncludeTrailingBackslash(p);
}
// ExtractFileDir -- BCB6: returns path without trailing separator.
// IncludeTrailingBackslash(asDir) ends in '\', so ExtractFilePath of that
// returns the same string (already a dir).  Strip the trailing sep.
static inline AnsiString ExtractFileDir(const AnsiString& p)
{
    AnsiString s = ExtractFilePath(p);
    if (s.Length() > 0 && (s[s.Length()] == '\\' || s[s.Length()] == '/'))
        s = s.SubString(1, s.Length() - 1);
    return s;
}

// ---------------------------------------------------------------------------
//  DTK4848 serial seam
//  Foundation-level pointer set by rs232 wiring (W5) or by tests.
//  cpublic.h declares: extern Spcomm::TComm* g_pDTKComm;
// ---------------------------------------------------------------------------
Spcomm::TComm* g_pDTKComm = nullptr;

// =============================================================================
//  FOUNDATION free functions  (no app/UI/rs232/motor deps)
// =============================================================================

//AI(ht9045-v899) 20260626: ungated -- pure arithmetic, no external deps
//------------------------------------------------------------------------------
AnsiString ConvertSecondToSPC(long s)                                           //Steven 20141111 : ���ର�ɤ���
{
    AnsiString str;
    long secs, mins, hours;
    secs=s%60L;
    mins=s/60L;
    hours=mins/60L;
    mins=mins%60L;
    hours=hours%24L;
    str.sprintf("%02d:%02d:%02d", int(hours), int(mins), int(secs));
    return str;
}
//------------------------------------------------------------------------------
AnsiString ConvertMSecToSPC(long s)                                             //Steven 20190714 : MS�ɶ��ഫ���ɤ���
{
    AnsiString str;
    long MS, secs, mins, hours;

    MS=s%1000L;
    secs=(s/1000L)%60L;
    mins=(s/1000L)/60L;
    hours=mins/60L;
    mins=mins%60L;
    hours=hours%24L;
    str.sprintf("%02d:%02d:%02d.%03d", int(hours), int(mins), int(secs), int(MS));
    return str;
}
//------------------------------------------------------------------------------
AnsiString ConvertMSecToTime(long s)                                            //Steven 20190714 : MS�ɶ��ഫ����ɤ���
{
    AnsiString str;
    long MS, secs, mins, hours, days;

    MS=s%1000L;
    secs=(s/1000L)%60L;
    mins=(s/1000L)/60L;
    hours=mins/60L;
    mins=mins%60L;
    days=hours/24L;
    hours=hours%24L;
    str.sprintf("%04d days %02d:%02d:%02d.%03d", (int)days, int(hours), int(mins), int(secs), int(MS));
    return str;
}
//------------------------------------------------------------------------------
void CutSpaceAtHead(char *S)
{
    char str2[256]={""};
    int i=0, pos=0;
    while(1)
    {
        if(S[i]!=' ')
            break;
        i++;
    }

    if(S[i]==0)
        return;
    while(1)
    {
        str2[pos]=S[i];
        if(S[i]==0)
            break;
        pos++;
        i++;
    }
    strcpy(S, str2);                                                            //���Ф���ϥ� strncpy
}
//------------------------------------------------------------------------------
char *ConvertSecondToTime(long s)
{
    static char str[256];
    long secs, mins, hours, days;
    secs    =s%60L;
    mins    =s/60L;
    hours   =mins/60L;
    mins    =mins%60L;
    days    =hours/24L;
    hours   =hours%24L;
    sprintf(str, "%04d days  %02d:%02d:%02d", (int)days, int(hours), int(mins), int(secs));
    return (char *) &str[0];
}
#define STX       2
#define ETX       3
#define CRRS232   13
#define LFRS232   10

typedef struct
{
    char cHeader[5];
    char cAddress[5];
    char cFuncCode[5];
    char cResponse[5];
    char cData[5];
    char cErrCheck[5];
}TEMP_CONTROL;
TEMP_CONTROL Temp_Control;
//------------------------------------------------------------------------------
//AI(ht9045-v899) 20260626: ungated -- pure HexStrToInt/IntToHex, no I/O
// �p��LRC (�ˬd�X)
//------------------------------------------------------------------------------
AnsiString DTK4848_LRC(AnsiString str)                                          //KaiHuang 20190821 : �s�W�x�F DTK4848�ű���
{
    AnsiString str2="";
    str=str.SubString(2, 12);                                                   //�����Ĥ@��:
    int LRC=0;
    for(int i=1; i<str.Length(); i+=2)
    {
        str2=str.SubString(i, 2);                                               //���o��Ӧr��
        LRC+=HexStrToInt(str2.c_str());                                         //�p���Ӧr����16�i����10�i��
    }

    LRC=0xFF-LRC+1;                                                             //�p��2�ɼ�
    str=IntToHex(LRC, 2);
    str=str.SubString(str.Length()-1, 2) ;
    return str;
}
#if 0 // TODO(W5: GetEveryCode/Change_Tempture_Value need T_ASXII2HEX table; UT100 needs A_Create_LCR/T_HEX2ASCII_Mac + COM2)
//------------------------------------------------------------------------------
void GetEveryCode(AnsiString AnsiData)                                          //Steven 20111028 : �令AnsiString
{
    sprintf(Temp_Control.cHeader   , "%s", AnsiData.SubString(1, 1));
    sprintf(Temp_Control.cAddress  , "%s", AnsiData.SubString(2, 2));
    sprintf(Temp_Control.cFuncCode , "%s", AnsiData.SubString(4, 2));
    sprintf(Temp_Control.cResponse , "%s", AnsiData.SubString(6, 2));
    sprintf(Temp_Control.cData     , "%s", AnsiData.SubString(8, 4));
    sprintf(Temp_Control.cErrCheck , "%s", AnsiData.SubString(12, 2));
}
//------------------------------------------------------------------------------
int Change_Tempture_Value()
{
    int iData[4];
    iData[0]=T_ASXII2HEX[Temp_Control.cData[0]-'0'];
    if(iData[0]!=15)
    {
        iData[1]=T_ASXII2HEX[Temp_Control.cData[1]-'0'];
        iData[2]=T_ASXII2HEX[Temp_Control.cData[2]-'0'];
        iData[3]=T_ASXII2HEX[Temp_Control.cData[3]-'0'];
        return iData[1]*256 + iData[2]*16 + iData[3];
    }
    else                                                                        //�p�G����F���ܭt��
    {
        iData[1]=T_ASXII2HEX[Temp_Control.cData[1]-'0'];
        iData[2]=T_ASXII2HEX[Temp_Control.cData[2]-'0'];
        iData[3]=T_ASXII2HEX[Temp_Control.cData[3]-'0'];
        return -(4096 - (iData[1]*256 + iData[2]*16 + iData[3]));
    }
}
//------------------------------------------------------------------------------
//Panasonic KT4H
//------------------------------------------------------------------------------
void UT100WordWriteNoSucm(int Addr, int Command, int Value)
{
    unsigned char Btmp1;
    int iFunctionCode=6;
    AnsiString tmp1;
    tmp1=IntToHex(Value, 4);
    ::sprintf(SENDBUFF, ":%02X%02X%04X%4s00%c%c", Addr+1, iFunctionCode, Command, tmp1, CRRS232, LFRS232);
    Btmp1=A_Create_LCR(&SENDBUFF[1], 12);
    SENDBUFF[13]=T_HEX2ASCII_Mac(Btmp1>>4);
    SENDBUFF[14]=T_HEX2ASCII_Mac(Btmp1);
    COM2->Comm2->WriteCommData(SENDBUFF, ::strlen(SENDBUFF));
}
//------------------------------------------------------------------------------
void UT100WordReadNoSucm(int Addr, int Command)
{
    int iFunctionCode=3;
    unsigned char Btmp1;
    ::sprintf(READBUFF, ":%02X%02X%04X000100%c%c", Addr+1, iFunctionCode, Command, CRRS232, LFRS232);
    Btmp1=A_Create_LCR(&READBUFF[1], 12);
    READBUFF[13]=T_HEX2ASCII_Mac(Btmp1>>4);                                     //Set return number by byte
    READBUFF[14]=T_HEX2ASCII_Mac(Btmp1);
    COM2->Comm2->WriteCommData(READBUFF, ::strlen(READBUFF));
}
#endif // TODO(W5)
//AI(ht9045-v899) 20260626: ungated DTK4848WordWriteNoSucm -- COM2->Comm2 rerouted to g_pDTKComm seam
//------------------------------------------------------------------------------
//KaiHuang 20190821 : �s�W�x�F DTK4848�ű���
//------------------------------------------------------------------------------
void DTK4848WordWriteNoSucm(int Addr, int Value)
{
    int iCommand = 4701;
    AnsiString TempStr="";
    char SENDBUFF_DTK[1024];
    ::sprintf(SENDBUFF_DTK, ":%02X06%04d%s", Addr+1, iCommand, IntToHex(Value, 4).c_str()); //Steven 20210511 : for Delta DT4848 %02d --> %02X

    AnsiString LRC = DTK4848_LRC(SENDBUFF_DTK);
    ::sprintf(SENDBUFF_DTK, ":%02X06%04d%s%s\r\n", Addr+1, iCommand, IntToHex(Value, 4).c_str(), LRC.c_str()); //AI(ht9045-v899) 20260626: .c_str() for AnsiString->%s in vclcompat (no implicit const char*)

    //AI(ht9045-v899) 20260626: rerouted from COM2->Comm2 to g_pDTKComm seam
    if (g_pDTKComm) g_pDTKComm->WriteCommData(SENDBUFF_DTK, (Word)(::strlen(SENDBUFF_DTK)+1)); //+1 (preserve NUL on wire)
}
//AI(ht9045-v899) 20260626: ungated DTK4848WordReadNoSucm -- COM2->Comm2 rerouted to g_pDTKComm seam
//------------------------------------------------------------------------------
void DTK4848WordReadNoSucm(int Addr)
{
    int iCommand = 4700;
    AnsiString TempStr="";
    char READBUFF_DTK[1024];
    ::sprintf(READBUFF_DTK, ":%02X03%04d0002", Addr+1, iCommand);               //Steven 20210511 : for Delta DT4848 %02d --> %02X

    AnsiString LRC = DTK4848_LRC(READBUFF_DTK);
    ::sprintf(READBUFF_DTK, ":%02X03%04d0002%s\r\n", Addr+1, iCommand, LRC.c_str()); //AI(ht9045-v899) 20260626: same; Polling

    //AI(ht9045-v899) 20260626: rerouted from COM2->Comm2 to g_pDTKComm seam
    if (g_pDTKComm) g_pDTKComm->WriteCommData(READBUFF_DTK, (Word)(::strlen(READBUFF_DTK)+1));
}
//AI(ht9045-v899) 20260626: ungated foundation free functions (ConvertGearValue..CRC_Check, all pure logic)
//------------------------------------------------------------------------------
void ConvertGearValue(int *Ref, double GearRatio)
{
    int i;
    i=*Ref;
    i=ChangeToFloatNonPcnt((double)(i), (double)(GearRatio));
    i*=GearRatio;
    *Ref=i;
}
//------------------------------------------------------------------------------
char *GetFloatFormatString(double v, int P1, int P2)
{
    static char str[256];
    char fstr[256]={"%5.3f"};
    fstr[1]='0'+(P1+P2);
    fstr[3]='0'+P2;
    sprintf(str, fstr, v);

    CutSpaceAtHead(str);

    return str;
}
//------------------------------------------------------------------------------
char *ConvertToMMType(int i)
{
    return GetFloatFormatString((double) i/100.0, 4, 2);
}
//------------------------------------------------------------------------------
int Get0_01MMType(char *str)
{
    double i=atof(str);
    if(i>=0.0)
        return (i*100.0+0.5);
    else
        return (i*100.0-0.5);
}
//------------------------------------------------------------------------------
char *ConvertTouMType(int i)
{
    return GetFloatFormatString((double) i/100.0, 6, 3);
}
//------------------------------------------------------------------------------
int ConvertDoubleStrToInt(AnsiString Str, double dDigit)
{
    double d=0.0;
    try
    {
        d=Str.ToDouble();
    }
    catch(...)
    {
        d=atof(Str.c_str());
    }

    if(d>=0)
        return d*dDigit+0.5;
    else
        return (d*dDigit-0.5);
}
//------------------------------------------------------------------------------
double ConvertIntStrToDouble(AnsiString Str, double dDigit)
{
    double d=0.0;
    d=Str.ToIntDef(0);
    return ChangeToFloatNonPcnt((double)(d), (double)(dDigit));
}
//------------------------------------------------------------------------------
void GetTimeInfo()
{
    static TDateTime dtPresent;
    dtPresent= Now();
    DecodeDate(dtPresent, SystemYear, SystemMonth, SystemDate);
    DecodeTime(dtPresent, SystemHour, SystemMin, SystemSec, SystemMSec);
}
//------------------------------------------------------------------------------
void GetYesterdayInfo()
{
    static TDateTime dtPresent;
    dtPresent=TDateTime(Now().Val()-1.0); //AI(ht9045-v899) 20260626: Now()-1 ambiguous -> Val()-1.0 (TDateTime has no operator-(int))
    DecodeDate(dtPresent, SystemYearYesterday, SystemMonthYesterday, SystemDateYesterday);
}
//------------------------------------------------------------------------------
bool SplitStrByDotSpaceOnly(char *str, char *dest, int Max)
{
    char Buffer[10240];
    int ct1=0,ct2=0;

    while(1)                                                                    // find first character
    {
        if(str[ct1]=='\x0')
            return false;
        if((str[ct1]!=' '  &&
            str[ct1]!=','  &&
            str[ct1]!='\t' &&
            str[ct1]!='\r' &&
            str[ct1] !=':'))
            break;
        ct1++;
    }
    while(1)
    {
        dest[ct2]=str[ct1];
        ct2++;
        ct1++;
        dest[ct2]='\x0';
        if((ct2+1) >=Max ) break;

        if(str[ct1]!=' '  &&
           str[ct1]!=','  &&
           str[ct1]!='\t' &&
           str[ct1]!='\0' &&
           str[ct1]!='\r' &&
           str[ct1]!=':')
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
        if( str[ct1]=='\x0')
            break;
        ct1++;
        ct2++;
        Buffer[ct2]='\x0';
        if(ct2>=10240)
            break;
    }
    strcpy(str, Buffer);
    return true;
}
//------------------------------------------------------------------------------
void ConvertTimeFormat(char *s, int &H, int &M, int &S)
{
    char str2[256];
    char str[256];
    strncpy(str, s, sizeof(str));
    SplitStrByDotSpaceOnly(str, str2, 20);
    H=atoi(str2);
    SplitStrByDotSpaceOnly(str, str2, 20);
    M=atoi(str2);
    SplitStrByDotSpaceOnly(str, str2, 20);
    S=atoi(str2);
}
//------------------------------------------------------------------------------
int GetScreenPos(int NowPos, int ScreenTop, int ScreenBottom, int RealTop, int RealBottom)
{
    int ScreenPos;
//    ScreenPos    - ScreenTop     NowPos     -RealTop
//    ----------------------    = -----------------------
//    ScreenBottom - ScreenTop     RealBottom -RealTop
//
//           NowPos     -RealTop
//    Scale= -----------------------
//           RealBottom -RealTop
//
//    ScreenPos=(ScreenBottom - ScreenTop)*Scale+ScreenTop;
    double Scale;
    if((RealBottom -RealTop)==0)
        return 0;
    Scale=(double)(NowPos-RealTop)/(double)(RealBottom -RealTop);
    ScreenPos=(double)(ScreenBottom - ScreenTop)*Scale+ScreenTop;
    return ScreenPos;
}
//------------------------------------------------------------------------------
unsigned int CRC_Check(unsigned char *ary, unsigned int len)
{
    unsigned int crc;
    unsigned char Bitloop;
    crc = 0xFFFF;
    while(len--)
    {
        crc^=*ary++;
        for(Bitloop=0; Bitloop<8; Bitloop++)
        {
            if(crc&01)
            {
                crc=(crc>>1)^0xa001;
            }
            else
            {
                crc=crc>>1;
            }
        }
    }
    return (crc);
}
//------------------------------------------------------------------------------
#if 0 // TODO(W5: TMC401WriteTemp/ReadTemp/E5DCReadTemp/WriteTemp need COM2->Comm2)
void TMC401WriteTemp(int Addr, int CH, int Temp)
{
    unsigned char CRCL,CRCH;
    DWord Tempdata,data;
    char str[128]={'\0'};
    Tempdata=Temp;

    str[0]=Addr+1;
    str[1]=0x06; //function code
    str[2]=0x00;
    str[3]=0xC8+CH;
    str[4]=(Tempdata & 0xff00)>>8;
    str[5]=Tempdata & 0xff;
    data=CRC_Check(&str[0], 6);
    CRCL=data&0xff;
    CRCH=data>>8;
    str[6]=CRCL;
    str[7]=CRCH;
    COM2->Comm2->WriteCommData(str, 8);
}
//------------------------------------------------------------------------------
void TMC401ReadTemp(int Addr, int CH)
{
    unsigned char CRCL,CRCH;
    DWord data;
    char str[128]={'\0'};

    str[0]=Addr+1;
    str[1]=0x03;                                                                //function code
    str[2]=0x00;
    str[3]=CH;
    str[4]=0x00;
    str[5]=0x01;
    data=CRC_Check(&str[0], 6);
    CRCL=data&0xff;
    CRCH=data>>8;
    str[6]=CRCL;
    str[7]=CRCH;
    COM2->Comm2->WriteCommData(str, 8);
}
//------------------------------------------------------------------------------
void E5DCReadTemp(int Addr)
{
    int BCC = 0;
    AnsiString Command, Str;
    Command.sprintf("%02d0000101C00000000002", Addr+1);
                  // %02d000 : �椸�s��+SID
                  // 0101    : Ū���h�ӳs��Ѽ�
                  // C0      : ����
                  // 0000    : 0000->PV��       // 0001    : ���A
                  // 00      : �쪺��m
                  // 0002    : �����ƶq, 2��

    BCC=SetBCC(Command);
    Str.sprintf("%c%s%c%c\r\n", STX, Command, ETX, BCC);
    COM2->Comm2->WriteCommData(Str.c_str(), Str.Length());
}
//------------------------------------------------------------------------------
void E5DCWriteTemp(int Addr, int Temp)
{
    int BCC=0;
    AnsiString Command, Str;
    Command.sprintf("%02d0000102C100030000010000%04X", Addr+1, Temp);
    BCC=SetBCC(Command);
    Str.sprintf("%c%s%c%c\r\n", STX, Command, ETX, BCC);
    COM2->Comm2->WriteCommData(Str.c_str(), Str.Length());
}
#endif // TODO(W5)
#if 0 // TODO(W6: TTLLog/HeaterLog/HeaterSVLog/OutShuttleLog/HomeLog/RespondASECom/ProductionLog need fMain UI + TestIF/SW[])
//------------------------------------------------------------------------------
void TTLLog(AnsiString Message)                                                 //Steven 20161115 : TTL Log��s���s��
{
    AnsiString Str;

    if(TestIF.iTestType==TTL_MODE || Message=="Close")
    {
        Str.sprintf("%s, ", Message);

        Str+=(SW[SwClear0].Status())?"1, ":"0, ";
        Str+=(SW[SwClear1].Status())?"1, ":"0, ";
        Str+=(SW[SwClear2].Status())?"1, ":"0, ";
        Str+=(SW[SwClear3].Status())?"1, ":"0, ";
        Str+=(SW[SwStart0].Status())?"1, ":"0, ";
        Str+=(SW[SwStart1].Status())?"1, ":"0, ";
        Str+=(SW[SwStart2].Status())?"1, ":"0, ";
        Str+=(SW[SwStart3].Status())?"1, ":"0, ";
        Str+=(SW[SwDut0  ].Status())?"1, ":"0, ";
        Str+=(SW[SwDut1  ].Status())?"1, ":"0, ";
        Str+=(SW[SwDut2  ].Status())?"1, ":"0, ";
        Str+=(SW[SwDut3  ].Status())?"1 " :"0 ";

        fMain->slTTLLog->AddTextWithDateTime(Str);
    }
}
//------------------------------------------------------------------------------
void HeaterLog(AnsiString Message, bool bOnOff)                                 //Steven 20151123 : Log for Heater Relay
{
    static AnsiString OldMessage="";
    AnsiString NewMess;

    NewMess.sprintf("Heater %s, by %s", (bOnOff)?"On":"Off", Message);
    if(OldMessage!=NewMess)
    {
        OldMessage=NewMess;
        fMain->slHeaterLog->AddTextWithDateTime(NewMess);
    }
}
//------------------------------------------------------------------------------
void HeaterSVLog(int Addr, double dTemp)                                        //Steven 20200121 : Log for Heater SV
{
    AnsiString NewMess;
    NewMess.sprintf("Set Heater %s SV to %f", asTempCtrl[Addr], dTemp);
    fMain->slHeaterLog->AddTextWithDateTime(NewMess);
}
//------------------------------------------------------------------------------
void OutShuttleLog(bool bFlag)                                                  //Steven 20110531
{
    AnsiString sFileName;
    if(fMain->cbShowShuttleSensor->Checked==true &&
       (fMain->meShuttle1->Lines->Count>1024 ||
        fMain->meShuttle2->Lines->Count>1024 ||
        bFlag==true))
    {
        GetTimeInfo();
        sFileName.sprintf("%04d-%02d-%02d %02d:%02d:%02d.%d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec);
        fMain->meShuttle2->Lines->Add(sFileName);

        sFileName.sprintf("%s\\%04d%02d\\", asShtLogPath, SystemYear, SystemMonth);
        MyForceDirectories(sFileName);
        sFileName.sprintf("%s\\%04d%02d\\SH2_%04d%02d%02d%02d%02d%02d.logs", asShtLogPath, SystemYear, SystemMonth, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
        fMain->meShuttle2->Lines->SaveToFile(sFileName);
        fMain->meShuttle2->Clear();

        sFileName.sprintf("%04d-%02d-%02d %02d:%02d:%02d.%d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec);
        fMain->meShuttle1->Lines->Add(sFileName);
        sFileName.sprintf("%s\\%04d%02d\\SH1_%04d%02d%02d%02d%02d%02d.logs", asShtLogPath, SystemYear, SystemMonth, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
        fMain->meShuttle1->Lines->SaveToFile(sFileName);
        fMain->meShuttle1->Clear();
    }
}
//------------------------------------------------------------------------------
// ����Home���y�{���
//------------------------------------------------------------------------------
void HomeLog(AnsiString Message)                                                //Kevin  20110525
{
    AnsiString sFileName;
    GetTimeInfo();
    sFileName.sprintf("%04d-%02d-%02d %02d:%02d:%02d.%d --> %s", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec, Message);
    fMain->MemoHome->Lines->Add(sFileName);
    sFileName.sprintf("%s\\Home_Home.logs", asHomeLogPath);
    static AnsiString aTemp="";
    if(aTemp!=Message)                                                          //ChungHung 20150114 add fix EFCreateError
    {
        aTemp=Message;
        if(fMain->MemoHome->Lines->Count>1024 || Message=="Close")
        {
            fMain->MemoHome->Lines->SaveToFile(sFileName);
            fMain->MemoHome->Clear();
            DeleteFile(sFileName);
        }
    }
}
//------------------------------------------------------------------------------
// kevin 20150415 �^�� ase ���p
//------------------------------------------------------------------------------
bool RespondASECom(AnsiString S1)
{
    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
    {
        ASESendMessage->SendToASEData(S1);                                      //kevin 20150415 �^�� ase Reset finish
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
void ProductionLog(AnsiString Message, bool bSaveToFile, AnsiString JamCode)    //JerryYang 20151225 Production log for SPIL Ĭ�{
{
    AnsiString sFileName;
    AnsiString Str;
    if(IniConfig.bO06SaveLogTimePeriod==false)                                  //JerryYang 20160217 �S�}�ҴN���OLog
        return;
    GetTimeInfo();

    sFileName.sprintf("%s\\%s_%04d%02d%02d.logs", asProductionLogPath, IniConfig.SocketHandlerID, SystemYear, SystemMonth, SystemDate);
    if(FileExists(sFileName)==false)                                            //JerryYang 20160120 ���ɤ����ưO��,���M��Memo���e
    {
        fMain->MemoProductionLog->Clear();
    }

    if(JamCode=="")
        Str.sprintf("%04d-%02d-%02d %02d:%02d:%02d.%d --> %s", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec, Message);
    else
        Str.sprintf("%04d-%02d-%02d %02d:%02d:%02d.%d %s --> %s", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec, JamCode, Message);
    fMain->MemoProductionLog->Lines->Add(Str);
    if(fMain->MemoProductionLog->Lines->Count>32768 ||                          //JerryYang 20160303 �@�Ѧs�@��log�ɡA1024->32768��
       Message=="Close" || bSaveToFile)
    {
        fMain->MemoProductionLog->Lines->SaveToFile(sFileName);
        if(fMain->MemoProductionLog->Lines->Count>32768)                        //JerryYang 20160303 �@�Ѧs�@��log�ɡA1024->32768��
        {
            fMain->MemoProductionLog->Clear();
        }
    }
}
#endif // TODO(W6)
//AI(ht9045-v899) 20260626: ungated ExecZipCommand -- Win32 CreateProcess only
//------------------------------------------------------------------------------
bool ExecZipCommand(AnsiString Path, AnsiString Param)                          //Steven 20160205 : �s�ɮɭԤ��n��DOS����
{
    STARTUPINFO  FStartupInfo;
    PROCESS_INFORMATION  FProcessInformation;

    BOOL result=FALSE; //AI(ht9045-v899) 20260626: BCB6 False->Win32 FALSE
    ZeroMemory(&FStartupInfo, sizeof(STARTUPINFO));
    ZeroMemory(&FProcessInformation,sizeof(PROCESS_INFORMATION));
    GetStartupInfo(&FStartupInfo);
    FStartupInfo.dwFlags=STARTF_USESHOWWINDOW;                                  //16.01.26.01 kirin xto3016
    FStartupInfo.wShowWindow=SW_HIDE;
    AnsiString ExecFile=Path+" "+Param;
    AnsiString ExecPath=ExtractFilePath(Path);
    if(ExecFile!="")
    {
        result=CreateProcess(NULL,
        const_cast<char*>(ExecFile.c_str()), //AI(ht9045-v899) 20260626: BCB6 implicit const cast -> explicit const_cast
        NULL,
        NULL,
        false,
        NORMAL_PRIORITY_CLASS,
        NULL,
        ExecPath.c_str(),
        &FStartupInfo,
        &FProcessInformation);
        return result;
    }
    else
    {
        return false;
    }
}
//AI(ht9045-v899) 20260626: ungated GetOnlyTimeInfoByString/GetDateInfoByString -- System* globals only
//------------------------------------------------------------------------------
AnsiString GetOnlyTimeInfoByString(AnsiString asSign)                           //ChungHung 20151125 modify for KYEC  //ChungHung 20150902 add
{
    AnsiString Str;
    GetTimeInfo();
    if(asSign.IsEmpty())
        Str.sprintf("%02d%02d%02d", SystemHour, SystemMin , SystemSec);
    else
        Str.sprintf("%02d%s%02d%s%02d", SystemHour, asSign, SystemMin, asSign, SystemSec);
    return Str;
}
//------------------------------------------------------------------------------
AnsiString GetDateInfoByString(AnsiString asSign)                               //ChungHung 20151125 modify for KYEC  //ChungHung 20150902 add
{
    AnsiString Str;
    GetTimeInfo();
    if(asSign.IsEmpty())
        Str.sprintf("%04d%02d%02d", SystemYear, SystemMonth, SystemDate);
    else
        Str.sprintf("%04d%s%02d%s%02d", SystemYear, asSign, SystemMonth, asSign, SystemDate);
    return Str;
}
#if 0 // TODO(W6: ProductionDataLog needs fSCKART/fLotInfo/LastSet + MyForceDirectories (gated))
//------------------------------------------------------------------------------
// kevin 20160724 �Ͳ����
//------------------------------------------------------------------------------
void ProductionDataLog()
{
    if(IniConfig.bVTESTFunction==true)
        return;

    FILE * pFile;
    AnsiString asStr, asPath, asFileName="";
    AnsiString tmps, tmps1,str1;
    GetTimeInfo();
    asPath.sprintf("%s\\%04d%02d\\", asProductDataPath, SystemYear, SystemMonth);
    if(iRunStartMode==FT)
    {
        tmps1="FT";
    }
    else
    {
        tmps1="RT";
    }

    if(fSCKART->bShow==true && fSCKART->palLotNumber->Caption!="")              //Steven 20210517 : �ɤWART��Lot No.
        str1.sprintf("%s_%s_%s_%s_%s.ini", IniConfig.SocketHandlerID, fSCKART->palLotNumber->Caption, tmps1, GetDateInfoByString(), GetOnlyTimeInfoByString());
    else
        str1.sprintf("%s_%s_%s_%s_%s.ini", IniConfig.SocketHandlerID, fLotInfo->edtSysLotID->Text, tmps1, GetDateInfoByString(), GetOnlyTimeInfoByString());    //Steven 20210517 : �ɦW�[�W�ɶ��W�O

    MyForceDirectories(asPath);

    asFileName = asPath+ str1;

    if(FileExists(asFileName))
    {
        DeleteFile(asFileName);
    }

    pFile=fopen(asFileName.c_str() ,"a+");
    if(pFile!=NULL)
    {
        tmps.sprintf("[Sort Summary]\n");
        fputs(tmps.c_str(), pFile);
        tmps.sprintf("Load=%d\n", LastSet.SendCT[0]);
        fputs(tmps.c_str(), pFile);

        tmps.sprintf("Auto1=%d\n",  LastSet.BinCT[0][e3Auto1]);
        fputs(tmps.c_str(), pFile);
        tmps.sprintf("Auto2=%d\n",  LastSet.BinCT[0][e3Auto2]);
        fputs(tmps.c_str(), pFile);
        tmps.sprintf("Auto3=%d\n",  LastSet.BinCT[0][e3Auto3]);
        fputs(tmps.c_str(), pFile);
        tmps.sprintf("Fix1=%d\n",   LastSet.BinCT[0][e3Fix1]);
        fputs(tmps.c_str(), pFile);
        tmps.sprintf("Fix2=%d\n",   LastSet.BinCT[0][e3Fix2]);
        fputs(tmps.c_str(), pFile);
        tmps.sprintf("Fix3=%d\n\n", LastSet.BinCT[0][e3Fix3]);
        fputs(tmps.c_str(), pFile);

        tmps.sprintf("[Bin Summary]\n");
        fputs(tmps.c_str(), pFile);
        for(int iCat=0; iCat<iTestBinCount; iCat++)
        {
            tmps.sprintf("Bin%d=%d\n", iCat, LastSet.iBinData32[0][iCat]);
            fputs(tmps.c_str(), pFile);
        }

        if(CUSTOMER_CODE==CC_PTI)                                               //Jimmychiu 20251208 : PTI ����n�DBin Summary�����[�JError bin
        {
            tmps.sprintf("Error Bin=%d\n", LastSet.iBinData32[0][iTestBinCount]);
            fputs(tmps.c_str(), pFile);
        }
        fclose(pFile);
    }
}
#endif // TODO(W6)
//AI(ht9045-v899) 20260626: ungated CompareMD5ByFolder/SetMD5ByFolder -- HTMD5 (md5_Folder/SearchFile) active
//------------------------------------------------------------------------------
//V3.27N.546 Steven 20170927 (wei) : ���u�@�ɪ��ˬd�X�O�_���T -1:�S��MD5,  1:Pass, 0:Fail
//------------------------------------------------------------------------------
int CompareMD5ByFolder(AnsiString FolderName)
{
    int bResult=-1;

    TStringList *tsFileName;
    AnsiString exMsg;
    tsFileName=new TStringList();
    SearchFile(FolderName, "*.MD5", tsFileName);

    if(tsFileName->Count==1)
    {
        AnsiString exMsg=md5_Folder(FolderName);
        if(AnsiString(tsFileName->Strings[0]).Pos(exMsg)!=0) //AI(ht9045-v899) 20260626: StringsProxy has no Pos -> explicit AnsiString cast
        {
            bResult=1;
        }
        else
        {
            bResult=0;
        }
    }
    else if(tsFileName->Count>1)
    {
        for(int i=0; i<tsFileName->Count; i++)
            DeleteFile(tsFileName->Strings[i]);
        bResult=-1;
    }
    else
    {
        bResult=-1;
    }

    delete tsFileName;
    return bResult;
}
//------------------------------------------------------------------------------
//V3.27N.546 Steven 20170927 (wei) : �N�u�@�ɥ[�J�ˬd�X
//------------------------------------------------------------------------------
void SetMD5ByFolder(AnsiString FolderName)
{
    TStringList *tsFileName;
    tsFileName=new TStringList();
    SearchFile(FolderName, "*.MD5", tsFileName);

    if(tsFileName->Count>0)
    {
        for(int i=0; i<tsFileName->Count; i++)
            DeleteFile(tsFileName->Strings[i]);
    }

    AnsiString exMsg=md5_Folder(FolderName);

    AnsiString FileName=FolderName+"\\"+exMsg+".MD5";
    tsFileName->Clear();
    SearchFile(FolderName, "*.Data", tsFileName);
    tsFileName->SaveToFile(FileName);

    delete tsFileName;
}
//------------------------------------------------------------------------------
//union��Byte�PBit����
//AI(ht9045-v899) 20260626: ungated ByteUnionBit methods -- pure bit ops + IntToHex
//------------------------------------------------------------------------------
int ByteUnionBit::Bit(int i)
{
    if(i==0)
        return bit.b0;
    else if(i==1)
        return bit.b1;
    else if(i==2)
        return bit.b2;
    else if(i==3)
        return bit.b3;
    else if(i==4)
        return bit.b4;
    else if(i==5)
        return bit.b5;
    else if(i==6)
        return bit.b6;
    else if(i==7)
        return bit.b7;
    else
        return -1;
}
//------------------------------------------------------------------------------
void ByteUnionBit::SetBit(int i, bool Value)
{
    if(i==0)
        bit.b0=Value?1:0;
    else if(i==1)
        bit.b1=Value?1:0;
    else if(i==2)
        bit.b2=Value?1:0;
    else if(i==3)
        bit.b3=Value?1:0;
    else if(i==4)
        bit.b4=Value?1:0;
    else if(i==5)
        bit.b5=Value?1:0;
    else if(i==6)
        bit.b6=Value?1:0;
    else
        bit.b7=Value?1:0;
}
//------------------------------------------------------------------------------
bool ByteUnionBit::SetByte(int Value)
{
    if(Value>255 || Value<0)
        return false;
    else
        byte=Value;
    return true;
}
//------------------------------------------------------------------------------
AnsiString ByteUnionBit::BitString()
{
    AnsiString Str;
    Str.sprintf("%d%d%d%d%d%d%d%d", bit.b7, bit.b6, bit.b5, bit.b4, bit.b3, bit.b2, bit.b1, bit.b0);
    return Str;
}
//------------------------------------------------------------------------------
AnsiString ByteUnionBit::Hex(bool bWith0x)
{
    if(bWith0x)
        return "0x"+IntToHex(byte, 2);
    else
        return IntToHex(byte, 2);
}
//------------------------------------------------------------------------------
int ByteUnionBit::BitCount(bool bOn)
{
    int iCount=0;
    for(int i=0; i<8; i++)
    {
        if(Bit(i)==bOn)
            iCount++;
    }
    return iCount;
}
#if 0 // TODO(W3-dup: TMyQueue10 ctor/ClearData already ungated at file top (L27-44))
//------------------------------------------------------------------------------
TMyQueue10::TMyQueue10()
{
    Alias="";
    ClearData();
    bInt=true;
}
//------------------------------------------------------------------------------
void TMyQueue10::ClearData()
{
    for(int i=0; i<MAX_Q_10; i++)
    {
        iData[i]=0;
        dData[i]=0.0;
        DateTime[i]="";
    }
    iIndex=-1;
    iCount=-1;
}
#endif // TODO(W3-dup)
//AI(ht9045-v899) 20260626: ungated TMyQueue10 method bodies (Add/CheckTaskChange/GetData/GetDateTime/GetLastData/ShowCommaText/SetAliasAndTask)
//------------------------------------------------------------------------------
void TMyQueue10::Add(int data)
{
    GetTimeInfo();
    if(iCount<MAX_Q_10)
    {
        iCount++;
    }

    iIndex++;
    if(iIndex>=MAX_Q_10 || iIndex<0)                                            //Steven 20200826 : �s�W�O�@,�קK����
        iIndex=0;
    iData[iIndex]=data;
    DateTime[iIndex].sprintf("%02d:%02d:%02d.%03d", SystemHour, SystemMin, SystemSec, SystemMSec);
}
//------------------------------------------------------------------------------
void TMyQueue10::Add(double data)
{
    bInt=false;
    GetTimeInfo();
    if(iCount<MAX_Q_10)
    {
        iCount++;
    }

    iIndex++;
    if(iIndex>=MAX_Q_10 || iIndex<0)                                            //Steven 20200826 : �s�W�O�@,�קK����
        iIndex=0;
    dData[iIndex]=data;
    DateTime[iIndex].sprintf("%02d:%02d:%02d.%03d", SystemHour, SystemMin, SystemSec, SystemMSec);
}
//------------------------------------------------------------------------------
bool TMyQueue10::CheckTaskChange()
{
    bool bAdd=false;

    if(iTask==NULL)
    {
        bAdd=false;
    }
    else
    {
        if(iCount<=0)
        {
            bAdd=true;
        }
        else
        {
            if(iIndex>=0 && iIndex<MAX_Q_10)                                    //Steven 20200826 : �s�W�O�@,�קK����
            {
                if(*iTask!=iData[iIndex])
                {
                    bAdd=true;
                }
            }
        }
    }

    if(bAdd)
    {
        Add(*iTask);
    }
    return bAdd;
}
//------------------------------------------------------------------------------
double TMyQueue10::GetData(int i)
{
    if(i<=iCount)
    {
        if(iCount<MAX_Q_10)                                                     //Steven 20200730 : �ץ�Task List��������
        {
            if(i>=0)
            {
                if(bInt)
                    return iData[i];
                else
                    return dData[i];
            }
            else
            {
                return -1;
            }
        }
        else
        {
            if(((iIndex+i)%MAX_Q_10)<MAX_Q_10 || ((iIndex+i)%MAX_Q_10)>=0)      //Steven 20200826 : �s�W�O�@,�קK����
            {
                if(bInt)
                    return iData[((iIndex+i)%MAX_Q_10)];
                else
                    return dData[((iIndex+i)%MAX_Q_10)];
            }
        }
    }
    return 0.0;
}
//------------------------------------------------------------------------------
AnsiString TMyQueue10::GetDateTime(int i)
{
    AnsiString Str="";
    int flag=((iIndex+i)%MAX_Q_10);

    if(i<=iCount)                                                               //Steven 20200730 : �ץ�Task List��������
    {
        if(iCount<MAX_Q_10)
        {
            if(i>=0)
            {
                if(bInt)
                    Str.sprintf("%s, %d",  DateTime[i], iData[i]);
                else
                    Str.sprintf("%s, %0.2f",  DateTime[i], dData[i]);
            }
            else
            {
                return -1;
            }
        }
        else
        {
            if(flag>=0)                                                         //Steven 20200826 : �s�W�O�@,�קK����
            {
                if(bInt)
                    Str.sprintf("%s, %d",  DateTime[flag], iData[flag]);
                else
                    Str.sprintf("%s, %0.2f",  DateTime[flag], dData[flag]);
            }
        }
    }

    return Str;
}
//------------------------------------------------------------------------------
double TMyQueue10::GetLastData()
{
    if(bInt)
        return iData[iIndex];
    else
        return dData[iIndex];
}
//------------------------------------------------------------------------------
AnsiString TMyQueue10::ShowCommaText(bool bWithDateTime)
{
    AnsiString Str=Alias+",", Str2;
    for(int i=0; i<=iCount; i++)                                                //Steven 20200730 : �ץ�Task List��������
    {
        if(bWithDateTime)
        {
            Str2=GetDateTime(i);
        }
        else
        {
            if(bInt)
                Str2.sprintf("%0.0f", GetData(i));
            else
                Str2.sprintf("%0.2f", GetData(i));
        }
        Str=Str+Str2;
        if(i<MAX_Q_10-1)
            Str=Str+AnsiString(",");
    }
    return Str;
}
//------------------------------------------------------------------------------
void TMyQueue10::SetAliasAndTask(AnsiString sAlias, int *Task)
{
    Alias=sAlias;
    iTask=Task;
}
#if 0 // TODO(W3-dup: TMyQueue100 ctor/ClearData already ungated at file top (L49-64))
//------------------------------------------------------------------------------
TMyQueue100::TMyQueue100()
{
    Alias="";
    ClearData();
    bInt=true;
}
//------------------------------------------------------------------------------
void TMyQueue100::ClearData()
{
    for(int i=0; i<MAX_Q_100; i++)
    {
        iData[i]=0;
        dData[i]=0.0;
        DateTime[i]="";
    }
    iIndex=0;
    iCount=0;
}
#endif // TODO(W3-dup)
//AI(ht9045-v899) 20260626: ungated TMyQueue100 method bodies (Add/GetData/GetDateTime/ShowCommaText)
//------------------------------------------------------------------------------
void TMyQueue100::Add(int data)
{
    GetTimeInfo();
    if(iCount<MAX_Q_100)
    {
        iData[iCount]=data;
        DateTime[iCount].sprintf("%02d:%02d:%02d.%03d", SystemHour, SystemMin, SystemSec, SystemMSec);
        iCount++;
    }
    else
    {
        iData[iIndex]=data;
        DateTime[iIndex].sprintf("%02d:%02d:%02d.%03d", SystemHour, SystemMin, SystemSec, SystemMSec);
        iIndex++;
        if(iIndex>=MAX_Q_100)
            iIndex=0;
    }
}
//------------------------------------------------------------------------------
void TMyQueue100::Add(double data)
{
    bInt=false;
    if(iCount<MAX_Q_100)
    {
        dData[iCount]=data;
        DateTime[iCount].sprintf("%02d:%02d:%02d.%03d", SystemHour, SystemMin, SystemSec, SystemMSec);
        iCount++;
    }
    else
    {
        dData[iIndex]=data;
        DateTime[iIndex].sprintf("%02d:%02d:%02d.%03d", SystemHour, SystemMin, SystemSec, SystemMSec);
        iIndex++;
        if(iIndex>=MAX_Q_100)
            iIndex=0;
    }
}
//------------------------------------------------------------------------------
double TMyQueue100::GetData(int i)
{
    int flag=((iIndex+i)%MAX_Q_100);
    if(i<iCount)
    {
        if(bInt)
            return iData[flag];
        else
            return dData[flag];
    }
    else
    {
        return 0.0;
    }
}
//------------------------------------------------------------------------------
AnsiString TMyQueue100::GetDateTime(int i)
{
    AnsiString Str="";
    int flag=((iIndex+i)%MAX_Q_100);
    if(i<iCount)
    {
        if(bInt)
            Str.sprintf("%s %d",  DateTime[flag], iData[flag]);
        else
            Str.sprintf("%s %0.2f",  DateTime[flag], dData[flag]);
    }

    return Str;
}
//------------------------------------------------------------------------------
AnsiString TMyQueue100::ShowCommaText(bool bWithDateTime)
{
    AnsiString Str=Alias+",", Str2;
    for(int i=0; i<iCount; i++)
    {
        if(bWithDateTime)
        {
            Str2=GetDateTime(i);
        }
        else
        {
            if(bInt)
                Str2.sprintf("%0.0f", GetData(i));
            else
                Str2.sprintf("%0.2f", GetData(i));
        }
        Str=Str+Str2;
        if(i<MAX_Q_100-1)
            Str=Str+AnsiString(",");
    }
    return Str;
}
#if 0 // TODO(W3-dup: TMyStrQueue100 ctor/ClearData already ungated at file top (L69-85))
//------------------------------------------------------------------------------
TMyStrQueue100::TMyStrQueue100()
{
    sLastFileName="";
    ClearData();
}
//------------------------------------------------------------------------------
void TMyStrQueue100::ClearData()
{
    for(int i=0; i<MAX_Q_100; i++)
    {
        sData1[i]="";
        sData2[i]="";
        DateTime[i]="";
    }
    iIndex=0;
    iCount=0;
}
#endif // TODO(W3-dup)
//AI(ht9045-v899) 20260626: ungated TMyStrQueue100 method bodies (Add/GetData/GetDateTime/ShowCommaText)
//------------------------------------------------------------------------------
void TMyStrQueue100::Add(AnsiString data1, AnsiString data2)
{
    GetTimeInfo();
    if(iCount<MAX_Q_100)
    {
        sData1[iCount]=data1;
        sData2[iCount]=data2;
        DateTime[iCount].sprintf("%02d:%02d:%02d.%03d", SystemHour, SystemMin, SystemSec, SystemMSec);
        iCount++;
    }
    else
    {
        sData1[iIndex]=data1;
        sData2[iIndex]=data2;
        DateTime[iIndex].sprintf("%02d:%02d:%02d.%03d", SystemHour, SystemMin, SystemSec, SystemMSec);
        iIndex++;
        if(iIndex>=MAX_Q_100)
            iIndex=0;
    }
}
//------------------------------------------------------------------------------
AnsiString TMyStrQueue100::GetData(int i)
{
    int flag=((iIndex+i)%MAX_Q_100);
    if(i<iCount)
    {
        return sData1[flag];
    }
    else
    {
        return "";
    }
}
//------------------------------------------------------------------------------
AnsiString TMyStrQueue100::GetDateTime(int i)
{
    AnsiString Str="";
    int flag=((iIndex+i)%MAX_Q_100);
    if(i<iCount)
    {
        Str.sprintf("%s, \"%s\", \"%s\"",  DateTime[flag], sData1[flag], sData2[flag]);
    }

    return Str;
}
//------------------------------------------------------------------------------
AnsiString TMyStrQueue100::ShowCommaText(bool bWithDateTime)
{
    AnsiString Str="", Str2;
    for(int i=0; i<iCount; i++)
    {
        if(bWithDateTime)
        {
            Str2=GetDateTime(i);
        }
        else
        {
            Str2.sprintf("%0.2f", GetData(i));
        }
        Str=Str2;
        if(i<MAX_Q_100-1)
            Str=Str+AnsiString(",");
    }
    return Str;
}
#if 0 // TODO(W5-safe: TMyStrQueue100::SafeData needs MyForceDirectories (gated in common.cpp TODO(wave-file)))
//------------------------------------------------------------------------------
void TMyStrQueue100::SafeData()
{
//    AnsiString asFileName, asPath, Str="";
//    TStringList *List=new TStringList();
////    static int iCount=0;                                                      //JerryYang 20250826 : fix galil log
//
//    for(int i=0; i<iCount; i++)
//    {
//        if(i<MAX_Q_100-1)
//        {
//            Str.sprintf("%s, %s", DateTime[i], sData1[i]);
//            List->Add(Str);
//        }
//    }
//
//    asPath.sprintf("%s\\%04d%02d\\", asGalilCmdPath, SystemYear, SystemMonth);
//    MyForceDirectories(asPath);
//    asFileName.sprintf("%s%04d%02d%02d_%02d%02d%02d.%03d.csv", asPath, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec);
//    if(sLastFileName==asFileName)
//    {
//        asFileName.sprintf("%s%04d%02d%02d_%02d%02d%02d.%03d_%d.csv", asPath, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec, iCount);
//        iCount++;
//    }
//    else
//    {
//        iCount=0;
//    }
//    sLastFileName=asFileName;
//    List->SaveToFile(asFileName);
//    List->Clear();
//    ClearData();
//    delete List;

    AnsiString asFileName, asPath, asBaseFileName, Str="";                      //RogerYang 20260505 : FixGalilLog
    TStringList *List=new TStringList();
    static int iSaveCount=0;

    for(int i=0; i<iCount; i++)
    {
        if(i<MAX_Q_100-1)
        {
            Str.sprintf("%s, %s", DateTime[i], sData1[i]);
            List->Add(Str);
        }
    }

    asPath.sprintf("%s\\%04d%02d\\", asGalilCmdPath, SystemYear, SystemMonth);
    MyForceDirectories(asPath);
    asBaseFileName.sprintf("%s%04d%02d%02d_%02d%02d%02d.%03d.csv",
        asPath, SystemYear, SystemMonth, SystemDate,
        SystemHour, SystemMin, SystemSec, SystemMSec);

    if(sLastFileName==asBaseFileName)       // compare base name
    {
        iSaveCount++;
        asFileName.sprintf("%s%04d%02d%02d_%02d%02d%02d.%03d_%d.csv",
            asPath, SystemYear, SystemMonth, SystemDate,
            SystemHour, SystemMin, SystemSec, SystemMSec, iSaveCount);
    }
    else
    {
        iSaveCount=0;
        asFileName=asBaseFileName;
    }
    sLastFileName=asBaseFileName;            // save base name
    if(List->Count>0)
    {
        List->SaveToFile(asFileName);
    }
    List->Clear();
    ClearData();
    delete List;
}
#endif // TODO(W5-safe)
#if 0 // TODO(W3-dup: TMyTimerQueue100 ctor/ClearData already ungated at file top (L90-108))
//------------------------------------------------------------------------------
TMyTimerQueue100::TMyTimerQueue100()
{
    sLastFileName="";
    ClearData();
}
//------------------------------------------------------------------------------
void TMyTimerQueue100::ClearData()
{
    for(int i=0; i<MAX_Q_100; i++)
    {
        sStartTime[i]="";
        sEndTime[i]="";
        DateTime[i]="";
        sTimeStr[i]="";
        iTime[i]=0;
    }
    iIndex=-1;
    iCount=-1;
}
#endif // TODO(W3-dup)
//AI(ht9045-v899) 20260626: ungated TMyTimerQueue100 method bodies (Add/GetTimeData/GetStartTime/GetEndTime/GetTimeString/GetDateTime/ShowCommaText)
//------------------------------------------------------------------------------
void TMyTimerQueue100::Add(AnsiString sTime, AnsiString eTime, int iCurrTime)
{
    GetTimeInfo();

    if(iCount<MAX_Q_100)
    {
        iCount++;
    }

    iIndex++;
    if(iIndex>=MAX_Q_100)
        iIndex=0;

    DateTime[iIndex].sprintf("%02d:%02d:%02d.%03d", SystemHour, SystemMin, SystemSec, SystemMSec);
    sStartTime[iIndex]  =sTime;
    sEndTime[iIndex]    =eTime;
    iTime[iIndex]       =iCurrTime;
    sTimeStr[iIndex]    =ConvertMSecToSPC(iCurrTime);
}
//------------------------------------------------------------------------------
int TMyTimerQueue100::GetTimeData(int i)
{
    int flag=(iIndex-i);
    if(flag<0)
        flag=MAX_Q_100+flag;

    if(i>=0 && i<=iCount)
    {
        return iTime[flag];
    }
    else
    {
        return 0;
    }
}
//------------------------------------------------------------------------------
AnsiString TMyTimerQueue100::GetStartTime(int i)
{
    int flag=(iIndex-i);
    if(flag<0)
        flag=MAX_Q_100+flag;

    if(i>=0 && i<=iCount)
    {
        return sStartTime[flag];
    }
    else
    {
        return "";
    }
}
//------------------------------------------------------------------------------
AnsiString TMyTimerQueue100::GetEndTime(int i)
{
    int flag=(iIndex-i);
    if(flag<0)
        flag=MAX_Q_100+flag;

    if(i>=0 && i<=iCount)
    {
        return sEndTime[flag];
    }
    else
    {
        return "";
    }
}
//------------------------------------------------------------------------------
AnsiString TMyTimerQueue100::GetTimeString(int i)
{
    int flag=(iIndex-i);
    if(flag<0)
        flag=MAX_Q_100+flag;

    if(i>=0 && i<=iCount)
    {
        return sTimeStr[flag];
    }
    else
    {
        return "";
    }
}
//------------------------------------------------------------------------------
AnsiString TMyTimerQueue100::GetDateTime(int i)
{
    AnsiString Str="";
    int flag=(iIndex-i);
    if(flag<0)
        flag=MAX_Q_100+flag;

    if(i>=0 && i<=iCount)
    {
        Str.sprintf("%s, \"%s\", \"%s\", \"%s\"", DateTime[flag], sStartTime[flag], sEndTime[flag], sTimeStr[flag]);
    }

    return Str;
}
//------------------------------------------------------------------------------
AnsiString TMyTimerQueue100::ShowCommaText(bool bWithDateTime)
{
    AnsiString Str="", Str2;
    for(int i=0; i<iCount; i++)
    {
        if(bWithDateTime)
        {
            Str2=GetDateTime(i);
        }
        else
        {
            Str2.sprintf("%s", ConvertMSecToSPC(GetTimeData(i)));
        }
        Str=Str2;
        if(i<MAX_Q_100-1)
            Str=Str+AnsiString(",");
    }
    return Str;
}
#if 0 // TODO(W5-safe: TMyTimerQueue100::SafeData needs MyForceDirectories (gated))
//------------------------------------------------------------------------------
void TMyTimerQueue100::SafeData()
{
    AnsiString asFileName, asPath, Str="";
    TStringList *List=new TStringList();
    static int iCnt=0;

    for(int i=0; i<iCount; i++)
    {
        Str=GetDateTime(i);
        if(i<MAX_Q_100-1)
            List->Add(Str);
    }

    asPath.sprintf("%s\\%04d%02d\\", asGalilCmdPath, SystemYear, SystemMonth);
    MyForceDirectories(asPath);
    asFileName.sprintf("%s%04d%02d%02d_%02d%02d%02d.%03d.csv", asPath, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec);
    if(sLastFileName==asFileName)
    {
        asFileName.sprintf("%s%04d%02d%02d_%02d%02d%02d.%03d_%d.csv", asPath, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec, iCnt);
        iCnt++;
    }
    else
    {
        iCnt=0;
    }
    sLastFileName=asFileName;
    List->SaveToFile(asFileName);
    List->Clear();
    ClearData();
    delete List;
}
#endif // TODO(W5-safe)
//AI(ht9045-v899) 20260626: ungated StartTestTimeStamp/EndTestTimeStamp -- System* + QueueCycleTime/QueueTestTime + tXxxTimer
//------------------------------------------------------------------------------
void StartTestTimeStamp(bool bAdd)
{
    StartTestTime.sprintf("%02d:%02d:%02d.%03d", SystemHour, SystemMin, SystemSec, SystemMSec);
    EndCycleTime.sprintf("%02d:%02d:%02d.%03d", SystemHour, SystemMin, SystemSec, SystemMSec);
    if(bAdd)
        QueueCycleTime.Add(StartCycleTime, EndCycleTime, tCycleTimer.LatchCycleTime());
    tTestTimer.LatchCycleTime(true);
}
//------------------------------------------------------------------------------
void EndTestTimeStamp(int iAdd)
{
    if(iAdd && StartTestTime!=NULL)
    {
        EndTestTime.sprintf("%02d:%02d:%02d.%03d", SystemHour, SystemMin, SystemSec, SystemMSec);
        QueueTestTime.Add(StartTestTime, EndTestTime, tTestTimer.LatchCycleTime());
        StartTestTime="";
    }

    if(iAdd!=0)
    {
        StartTestTime="";
        StartCycleTime.sprintf("%02d:%02d:%02d.%03d", SystemHour, SystemMin, SystemSec, SystemMSec);
        tCycleTimer.LatchCycleTime(true);
    }
}
//AI(ht9045-v899) 20260626: ungated FindAndKillProcess -- Win32 psapi only
//------------------------------------------------------------------------------
int FindAndKillProcess(LPCTSTR lpszProcessName)                                 //JerryYang 20200430 Kill Eventlogsaver
{
    DWORD dwProcessIdentify[MAX_PATH]={0};
    DWORD dwTrueBytes=0;
    HANDLE hProcess=NULL;
    int x=0;
    if(!EnumProcesses(dwProcessIdentify, MAX_PATH*sizeof(DWORD), &dwTrueBytes))
    {
        std::cout<<"enum process fail "<<std::endl; //AI(ht9045-v899) 20260626: BCB6 implicit using->std:: qualified
        return x;
    }
    int nProcessNum=dwTrueBytes/sizeof(DWORD);

    TCHAR moduleBaseName[MAX_PATH]={0};
    for(int nIndex=0; nIndex<nProcessNum; ++nIndex)
    {
        hProcess=OpenProcess(PROCESS_ALL_ACCESS, false, dwProcessIdentify[nIndex]);
        memset(moduleBaseName, 0, MAX_PATH*sizeof(TCHAR));
        GetModuleBaseName(hProcess, NULL, moduleBaseName, MAX_PATH);
        if(!strcmp(moduleBaseName, lpszProcessName)) //AI(ht9045-v899) 20260626: _tcscmp->strcmp (ANSI, not UNICODE)
        {
            x=1;
            TerminateProcess(hProcess, 0);
            WaitForSingleObject(hProcess, INFINITE);
            CloseHandle(hProcess);
            return x;
        }
        else
        {
            x=0;
        }
    }
    return x;
}
#if 0 // TODO(W6: LogIndexMaxMinPos needs fMain->slIndexYMaxMinShift + iMax/MinCommandY* globals + InitialMaxMinValue)
//------------------------------------------------------------------------------
void LogIndexMaxMinPos(AnsiString str)                                          //Isaac 20201012 : �p��Encoder�Mcommandpos/Teaching���t�ȡA�O���æs�ɡA�@�Ltray�O���@��
{
    AnsiString Message="", StrPosRecord="";

    GetTimeInfo();

    Message.sprintf("Index Y1: MaxCMDY1:%d,MinCMDY1:%d,MaxY1F:%d,MinY1F:%d,MaxY1M:%d,MinY1M:%d", iMaxCommandY1, iMinCommandY1, iMaxTeachY1F, iMinTeachY1F, iMaxTeachY1M, iMinTeachY1M);
    fMain->slIndexYMaxMinShift->AddTextWithDateTime(Message);
    fMain->AddIndexPosLog(Message);

    Message.sprintf("Index Y2: MaxCMDY2:%d,MinCMDY2:%d,MaxY2M:%d,MinY2M:%d,MaxY2R:%d,MinY2R:%d", iMaxCommandY2, iMinCommandY2, iMaxTeachY2M, iMinTeachY2M, iMaxTeachY2R, iMinTeachY2R);
    fMain->slIndexYMaxMinShift->AddTextWithDateTime(Message);
    fMain->AddIndexPosLog(Message);;

    fMain->slIndexYMaxMinShift->MySaveToFile();

    InitialMaxMinValue(str);                                                    //Isaac 20201012 : �p��Encoder�Mcommandpos/Teaching���t�ȡA�k�s
}
#endif // TODO(W6)
//AI(ht9045-v899) 20260626: ungated Round(double,double), Round(double), RotationCoordinates -- pure math
//------------------------------------------------------------------------------
double Round(double x, double point)                                            //ChungHung 20210113 add for Alignment CCD start
{
    double w=pow(10, point);
    if(x>0)
        x=x+ChangeToFloatNonPcnt((double)(0.5), (double)(w));
    else
        x=x-ChangeToFloatNonPcnt((double)(0.5), (double)(w));

    int y=x*w;
    x=ChangeToFloatNonPcnt((double)(y), (double)(w));

    return x;
}
//------------------------------------------------------------------------------
int Round(double x)                                                             //ChungHung 20210113 add for Alignment CCD start
{
    if(x>0)
        x=x+0.5;
    else
        x=x-0.5;

    //int y = x * w;
    //x = y/w;

    return  (int)x;
}
//------------------------------------------------------------------------------
void RotationCoordinates(double px, double py, double &px1, double &py1, double RadianX, double RadianY, bool bCCD)
{
    //if(bCCD)
    {
        px1 = px*cos(RadianX) - py*sin(RadianY);
        py1 = px*sin(RadianX) + py*cos(RadianY);
        //px1 = px;
        //py1 = py;
    }
}
#if 0 // TODO(W6: UDPErrorLog needs asUDPLogPath + MyForceDirectories (gated) + WriteDataToFile (gated))
//------------------------------------------------------------------------------
void UDPErrorLog(AnsiString aTitle, AnsiString Command)                         //kevin 20211020 UDP error log
{
    AnsiString sFileName="", sMegTime="", asLog;
    GetTimeInfo();
    sFileName.sprintf("%s\\%04d\\%02d", asUDPLogPath, SystemYear, SystemMonth);
    MyForceDirectories(sFileName);
    sFileName.sprintf("%s\\%04d\\%02d\\%02d%02d%02d%02d.txt", asUDPLogPath, SystemYear, SystemMonth, SystemYear, SystemMonth, SystemDate, SystemHour);
    sMegTime.sprintf("%04d-%02d-%02d %02d:%02d:%02d %03d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec);
    asLog.sprintf("%s,   %s, %s", sMegTime, aTitle, Command);
    WriteDataToFile(sFileName.c_str(), asLog.c_str());
}
#endif // TODO(W6)
#if 0 // TODO(W6: RecordErrorLog case 1 needs fMain->ListBox14; WriteDataToFile (gated))
//==============================================================================
void RecordErrorLog(int iSaveToFile,AnsiString FilePth, AnsiString Command)     //kevin 20211022 any error log
{
    AnsiString sFileName="", sMegTime="", asLog;
    GetTimeInfo();
    sFileName.sprintf("D:\\HT9045_Log\\%s\\%04d\\%02d", FilePth, SystemYear, SystemMonth);
    MyForceDirectories(sFileName);
    sFileName.sprintf("D:\\HT9045_Log\\%s\\%04d\\%02d\\%02d%02d%02d%02d.txt", FilePth, SystemYear, SystemMonth, SystemYear, SystemMonth, SystemDate, SystemHour);
    sMegTime.sprintf("%04d-%02d-%02d %02d:%02d:%02d %03d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec);
    asLog.sprintf("%s, :, %s", sMegTime, Command);

    switch(iSaveToFile)
    {
        case 0:
            WriteDataToFile(sFileName.c_str(), asLog.c_str());
            break;
        case 1:
            fMain->ListBox14->Items->SaveToFile(sFileName);
            break;
    }
}
#endif // TODO(W6)
//AI(ht9045-v899) 20260626: ungated sDataTimelog -- System* + GetTimeInfo + AnsiString only
//==============================================================================
void sDataTimelog(AnsiString &Msg)                                              //kevin 20211027 log + DataTime
{
    AnsiString sMegTime="", asLog="";
    GetTimeInfo();
    sMegTime.sprintf("%04d-%02d-%02d %02d:%02d:%02d %03d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec);
    asLog.sprintf("%s : %s", sMegTime, Msg);
    Msg= asLog;
}
//AI(ht9045-v899) 20260626: ungated b_Check_Dir_Exist_And_Creak_Dir -- local shims for ExtractFileDir/IncludeTrailingPathDelimiter at file top
//==============================================================================
//��ܻ���:�ˬd�����ؿ��O�_�s�b�A�åB���s�إ�
//V1.0 :Kirin 20170206 (han) �H�e�N�� 20170206 ���s�ק�C
//V1.1 :Kirin 20170210 (han) �W�[�ɤW�T�{�����C
//==============================================================================
bool b_Check_Dir_Exist_And_Creak_Dir(AnsiString asDir)
{                                                                               //�إ߻P�ˬd�ؿ��O�_�s�b.
    AnsiString asStr;
    asStr.sprintf("Cannot create %s.", asDir);

    AnsiString asDir1=ExtractFileDir(IncludeTrailingPathDelimiter(asDir));      //Kirin 20170210.12 (han) �W�[�ɤW�T�{����
    if(asDir1.Length()>0)
    {
        if(DirectoryExists(asDir1)==false)
        {
            if(ForceDirectories(asDir1)==true)
            {
                return true;
            }
            else
            {
                throw std::runtime_error(asStr.c_str()); //AI(ht9045-v899) 20260626: BCB6 Exception->std::runtime_error
            }
        }
        else
        {
            return true;
        }
    }
    return false;
}
//AI(ht9045-v899) 20260626: ungated SetSocketHandlerID -- IniConfig.SocketHandlerID (active)
//==============================================================================
void SetSocketHandlerID(AnsiString strID)                                       //Jimmychiu 20220805 SocketHandlerID can not be NULL
{
    if(strID=="")
        strID=AnsiString(" ");
    IniConfig.SocketHandlerID=strID;
}
#if 0 // TODO(W6: ShuttleLog needs iSensor[][]/ UseCanBusOrEtherCAT (HAL) + WriteDataToFile (gated) + asShtSenLogPath)
//==============================================================================
void ShuttleLog()                                                               //kevin 20220912 add shuttle sensor record
{
    AnsiString sFileName, Sbuffer0="", Sbuffer1="", str2="";
    GetTimeInfo();

    sFileName.sprintf("%s%04d%02d", asShtSenLogPath, SystemYear, SystemMonth);
    MyForceDirectories(sFileName);
    Sbuffer0.sprintf("%s\\%04d%02d%02d.logs", sFileName,SystemYear, SystemMonth, SystemDate);
    Sbuffer1.sprintf("%02d:%02d:%3d Start", SystemHour, SystemMin, SystemSec);
    WriteDataToFile(Sbuffer0.c_str(), Sbuffer1.c_str());

    for(int i=0; i<16; i++)
    {
        if(UseCanBusOrEtherCAT())                                               //Sam 20230707 : EtherCAT Shuttle sensor
        {
            if(i<8)                                                             //kevin 20220912 shuttle 2 data log
            {
                str2.printf("CH%d", 8-i%8);                                     //kevin 20220907
                Sbuffer1.sprintf("Shuttle2 %s  Read:%d  Set:%d", str2, iSensor[1][i], iSensor[3][i]);
                WriteDataToFile(Sbuffer0.c_str(), Sbuffer1.c_str());
            }
            else
            {
                str2.printf("CH%d", 8-i%8);                                     //kevin 20220907
                Sbuffer1.sprintf("Shuttle1 %s  Read:%d  Set:%d", str2, iSensor[0][i-8], iSensor[2][i-8]);
                WriteDataToFile(Sbuffer0.c_str(), Sbuffer1.c_str());
            }
        }
        else
        {
            str2.printf("CH%d", 8-i%8);                                         //kevin 20220907
            if(i<8)                                                             //kevin 20220912 shuttle 2 data log
            {
                Sbuffer1.sprintf("Shuttle2 %s  Read:%d  Set:%d", str2, iSensor[1][i], iSensor[3][i]);
                WriteDataToFile(Sbuffer0.c_str(), Sbuffer1.c_str());
            }
            else
            {
                Sbuffer1.sprintf("Shuttle1 %s  Read:%d  Set:%d", str2, iSensor[0][i-8], iSensor[2][i-8]);
                WriteDataToFile(Sbuffer0.c_str(), Sbuffer1.c_str());
            }
        }
    }
    Sbuffer1.sprintf("End",SystemHour, SystemMin, SystemSec);
    WriteDataToFile(Sbuffer0.c_str(), Sbuffer1.c_str());
}
#endif // TODO(W6)
//AI(ht9045-v899) 20260626: ungated GetSoftwareFileVersion -- uses VerInfo (Win32 version API)
//------------------------------------------------------------------------------
//Sam 20230328 : �۰ʧ�s�W�[�����ˬd
//==>
AnsiString GetSoftwareFileVersion(AnsiString sFilePatch)                        //���~����
{
    AnsiString sFileVer="";
    VerInfo *myVerInfo=new VerInfo();
    myVerInfo->SetFileName(sFilePatch); //AI(ht9045-v899) 20260626: __property FileName=->SetFileName() (no BCB6 prop)
    sFileVer=myVerInfo->ProductVersion(); //AI(ht9045-v899) 20260626: __property ProductVersion->getter call
    delete myVerInfo;
    return sFileVer;
}
//AI(ht9045-v899) 20260626: ungated VerInfo ctor/m_ClearData/m_SetFileName/m_strGetFixed*/GetAppVersion -- Win32 GetFileVersionInfo
//------------------------------------------------------------------------------
VerInfo::VerInfo()
{
    m_ClearData();
}
//------------------------------------------------------------------------------
void VerInfo::m_ClearData(void)
{
    ZeroMemory((void *)(&m_stFileInfo), sizeof(m_stFileInfo));
    m_wLangID               =0;
    m_wCharsetID            =0;
    m_strProductName        ="";
    m_strProductVersion     ="";
    m_strOriginalFilename   ="";
    m_strFileDescription    ="";
    m_strFileVersion        ="";
    m_strCompanyName        ="";
    m_strLegalCopyright     ="";
    m_strLegalTrademarks    ="";
    m_strInternalName       ="";
    m_strComments           ="";
}
//------------------------------------------------------------------------------
void VerInfo::m_SetFileName(AnsiString strNewName)
{
    m_strFileName=strNewName;
    m_GetVerInfo();
}
//------------------------------------------------------------------------------
AnsiString VerInfo::m_strGetFixedFileVersion(void)
{
    AnsiString s0((int)HIWORD(m_stFileInfo.dwFileVersionMS));
    AnsiString s1((int)LOWORD(m_stFileInfo.dwFileVersionMS));
    AnsiString s2((int)HIWORD(m_stFileInfo.dwFileVersionLS));
    AnsiString s3((int)LOWORD(m_stFileInfo.dwFileVersionLS));
    return (s0+"."+s1+"."+s2+"."+s3);
}
//------------------------------------------------------------------------------
AnsiString VerInfo::m_strGetFixedProductVersion(void)
{
    AnsiString s0((int)HIWORD(m_stFileInfo.dwProductVersionMS));
    AnsiString s1((int)LOWORD(m_stFileInfo.dwProductVersionMS));
    AnsiString s2((int)HIWORD(m_stFileInfo.dwProductVersionLS));
    AnsiString s3((int)LOWORD(m_stFileInfo.dwProductVersionLS));
    return (s0+"."+s1+"."+s2+"."+s3);
}
//------------------------------------------------------------------------------
void VerInfo::GetAppVersion(AnsiString sAppExeName, WORD& major, WORD& minor, WORD& build, WORD& revision)
{
    DWORD handle = 0;
    AnsiString fileName=sAppExeName;
    DWORD size=GetFileVersionInfoSize(fileName.c_str(), &handle);
    if(size==0)
        return;                                                                 //"Version Info Not Found"

    void* data=malloc(size);
    if(GetFileVersionInfo(fileName.c_str(), handle, size, data)==false)
    {
        free(data);
        return;                                                                 // "Version Info Error";
    }
    VS_FIXEDFILEINFO* fileInfo=NULL;
    UINT len = 0;

    if(VerQueryValue(data, "\\", (void**)&fileInfo, &len)==false)
    {
        free(data);
        return;                                                                 // "Version Info Parse Error";
    }
    major   =HIWORD(fileInfo->dwFileVersionMS);
    minor   =LOWORD(fileInfo->dwFileVersionMS);
    build   =HIWORD(fileInfo->dwFileVersionLS);
    revision=LOWORD(fileInfo->dwFileVersionLS);
    free(data);
}
#if 0 // TODO(W5: GetSVNRev/GetFileVersion/GetMainVersion need Application->ExeName (VCL singleton, no stub in foundation))
//------------------------------------------------------------------------------
AnsiString VerInfo::GetSVNRev()
{
    AnsiString sret="";
    unsigned short iFileVerMajor=0, iFileVerMinor=0, iFileVerRelease=0, iFileVerBuild=0;
    AnsiString strFilePath=Application->ExeName;
    VerInfo().GetAppVersion(strFilePath, iFileVerMajor, iFileVerMinor, iFileVerRelease, iFileVerBuild);
    sret=AnsiString().sprintf("%d.%d", iFileVerRelease, iFileVerBuild);
    return sret;
}
//------------------------------------------------------------------------------
AnsiString VerInfo::GetFileVersion()
{
    AnsiString sret="";
    unsigned short iFileVerMajor=0, iFileVerMinor=0, iFileVerRelease=0, iFileVerBuild=0;
    AnsiString strFilePath=Application->ExeName;
    VerInfo().GetAppVersion(strFilePath, iFileVerMajor, iFileVerMinor, iFileVerRelease, iFileVerBuild);
    sret=AnsiString().sprintf("V%d.%d.%d.%d", iFileVerMajor, iFileVerMinor, iFileVerRelease, iFileVerBuild);
    return sret;
}
//------------------------------------------------------------------------------
AnsiString VerInfo::GetMainVersion()
{
    AnsiString sret="";
    unsigned short iFileVerMajor=0, iFileVerMinor=0, iFileVerRelease=0, iFileVerBuild=0;
    AnsiString strFilePath=Application->ExeName;
    VerInfo().GetAppVersion(strFilePath, iFileVerMajor, iFileVerMinor, iFileVerRelease, iFileVerBuild);

    if(IniConfig.bSPILFunction==true)       //JeryYang 20260611 : SPIL��ܧ��㪩����
    {
        sret=AnsiString().sprintf("V%d.%d.%d.%d", iFileVerMajor, iFileVerMinor, iFileVerRelease, iFileVerBuild);
    }
    else
    {
        sret=AnsiString().sprintf("V%d.%d", iFileVerMajor, iFileVerMinor);
    }
    return sret;
}
#endif // TODO(W5)
//AI(ht9045-v899) 20260626: ungated m_GetVerInfo -- Win32 GetFileVersionInfo + GlobalAlloc, no app/UI deps
//------------------------------------------------------------------------------
void VerInfo::m_GetVerInfo(void)
{
    const static TCHAR *VersionKeys[]=
    {
        TEXT("ProductName"),
        TEXT("ProductVersion"),
        TEXT("OriginalFilename"),
        TEXT("FileDescription"),
        TEXT("FileVersion"),
        TEXT("CompanyName"),
        TEXT("LegalCopyright"),
        TEXT("LegalTrademarks"),
        TEXT("InternalName"),
        TEXT("Comments")
    };

    VKINFO gVKArray[10];
    m_ClearData();

    DWORD dwHandle=0;
    DWORD dwLength;
    dwLength = GetFileVersionInfoSize(m_strFileName.c_str(), &dwHandle);
    if(1>dwLength)
    {
        m_dwLastError=GetLastError();
        return;
    }

    HANDLE hMem;
    LPVOID lpvMem;
    hMem = GlobalAlloc(GMEM_MOVEABLE, dwLength);
    if(NULL==hMem)
    {
        m_dwLastError=GetLastError();
        return;
    }
    lpvMem=GlobalLock(hMem);
    if(NULL==lpvMem)
    {
        m_dwLastError=GetLastError();
        GlobalUnlock(hMem);
        GlobalFree(hMem);
        return;
    }

    BOOL fRet;
    fRet = GetFileVersionInfo(m_strFileName.c_str(), dwHandle, dwLength, (LPVOID)lpvMem);
    if(FALSE==fRet)
    {
        m_dwLastError=GetLastError();
        GlobalUnlock(hMem);
        GlobalFree(hMem);
        return;
    }

    LPVOID lpInfo;
    UINT cch;
    if(VerQueryValue(lpvMem, "\\", &lpInfo, &cch))
    {
        CopyMemory((void*)&m_stFileInfo,(const void*)lpInfo, sizeof(m_stFileInfo));
    }
    else
    {
        m_dwLastError=GetLastError();
        GlobalUnlock(hMem);
        GlobalFree(hMem);
        return;
    }

    AnsiString strLangID;
    AnsiString strCharset;
    if(VerQueryValue(lpvMem, VERSION_INFO_KEY_TRANS, &lpInfo, &cch))
    {
        m_wLangID   =((WORD *)lpInfo)[0];
        m_wCharsetID=((WORD *)lpInfo)[1];
        strLangID   =IntToHex((int)m_wLangID, 4); //AI(ht9045-v899) 20260626: BCB6 AnsiString::IntToHex->free fn IntToHex
        strCharset  =IntToHex((int)m_wCharsetID, 4); //AI(ht9045-v899) 20260626: same
    }
    else
    {
        m_dwLastError=GetLastError();
        GlobalUnlock(hMem);
        GlobalFree(hMem);
        return;
    }
    AnsiString strVerInfoLangID(strLangID+strCharset);

    TCHAR key[80];
    static TCHAR szNull[1]=TEXT("");
    for(UINT i=0; i<10; i++)
    {
        lstrcpy(key, VERSION_INFO_KEY_ROOT);
        lstrcat(key, strVerInfoLangID.c_str());
        lstrcat(key, "\\");
        lstrcat(key, VersionKeys[i]);
        gVKArray[i].szKey = VersionKeys[i];

        if(VerQueryValue(lpvMem, key, &lpInfo, &cch))
        {
            gVKArray[i].szValue = (char *)lpInfo;
        }
        else
        {
            gVKArray[i].szValue = szNull;
        }
    }
    m_strProductName        = gVKArray[0].szValue;
    m_strProductVersion     = gVKArray[1].szValue;
    m_strOriginalFilename   = gVKArray[2].szValue;
    m_strFileDescription    = gVKArray[3].szValue;
    m_strFileVersion        = gVKArray[4].szValue;
    m_strCompanyName        = gVKArray[5].szValue;
    m_strLegalCopyright     = gVKArray[6].szValue;
    m_strLegalTrademarks    = gVKArray[7].szValue;
    m_strInternalName       = gVKArray[8].szValue;
    m_strComments           = gVKArray[9].szValue;

    GlobalUnlock(hMem);
    GlobalFree(hMem);

    m_dwLastError = GetLastError();
}
#if 0 // TODO(W7: GetBundleInfo needs cJSON + TestSocket/LotSummary/Prod/fSCKART/fNote + asBundleTrayID/bUnloading)
//<==
//Sam 20230328 : �۰ʧ�s�W�[�����ˬd
//------------------------------------------------------------------------------
AnsiString GetBundleInfo(int iAuto)                                             //JerryYang 20240318 : add
{
    int iCount=3, iSetBinCnt=0, iUnloadCount=0;
    AnsiString sName[TEST_MAX_BIN];

    int iBinCnt[TEST_MAX_BIN], temp, k=0;
    AnsiString str, str1;
    cJSON *root=cJSON_CreateObject();
    cJSON *likeObject=cJSON_CreateArray();
    cJSON *array_obj[TEST_MAX_BIN];

    for(int i=0; i<TEST_MAX_BIN; i++)
    {
        sName[i]="";
        iBinCnt[i]=0;
    }

    if(iAuto==-1)
    {
        for(int iBin=0; iBin<=iTestBinCount; iBin++)
        {
            iCount=0;
            for(int i=0; i<TestSocket.iShtRow; i++)
            {
                for(int j=0; j<TestSocket.iShtCol; j++)
                {
                    iUnloadCount+=LotSummary.iCountCategory[iCount][iBin];
                    iCount++;
                }
            }
        }

        for(int i=0; i<=iTestBinCount; i++)
        {
            temp=Prod.iT6PosCate[i];
            if(temp<=0 && i!=iTestBinCount && iAuto==temp && iAuto>=0)
                continue;
            if(temp<=0 && i!=iTestBinCount && iAuto==-1)
                continue;

            sName[iSetBinCnt]=i;
            iBinCnt[iSetBinCnt]=LotSummary.iTotalCategory[i];

            if(i==iTestBinCount)
            {
                sName[iSetBinCnt]="Error";
            }
            else
            {
            }
            iSetBinCnt++;
        }

        for(int i=0; i<iSetBinCnt; i++)
        {
            array_obj[i]=cJSON_CreateObject();
        }

        cJSON_AddItemToObject(root, "TOTAL_QTY", cJSON_CreateString(IntToStr(iUnloadCount).c_str()));
        cJSON_AddItemToObject(root, "LOT_NO", cJSON_CreateString(fSCKART->sLotID.c_str()));

        for(int i=0; i<iSetBinCnt; i++)
        {
            str=IntToStr(iBinCnt[i]);

            cJSON_AddItemToObject(array_obj[i], "BIN", cJSON_CreateString(sName[i].c_str()));
            cJSON_AddItemToObject(array_obj[i], "QTY", cJSON_CreateString(str.c_str()));
        }

        for(int i=0; i<iSetBinCnt; i++)
        {
            cJSON_AddItemToArray(likeObject, array_obj[i]);
        }
        cJSON_AddItemToObject(root, "Detail", likeObject);
    }
    else
    {
        if(Prod.iTrayType[iAuto]!=tNotUse)
        {
            for(int i=0; i<=iTestBinCount; i++)
            {
                temp=Prod.iT6PosCate[i];
                if(temp<=0 && i!=iTestBinCount && iAuto==temp && iAuto>=0)
                    continue;

                if(iAuto==temp-1)
                {
                    sName[iSetBinCnt]=i;
                    iBinCnt[iSetBinCnt]=LotSummary.iTotalCategory[i]-LotSummary.iLastTotalCategory[i];
                    LotSummary.iLastTotalCategory[i]=LotSummary.iTotalCategory[i];
                    iSetBinCnt++;
                }

                if(Prod.iIfErrorT6==iAuto && i==iTestBinCount)
                {
                    sName[iSetBinCnt]="Error";
                    iBinCnt[iSetBinCnt]=LotSummary.iTotalCategory[i];
                    iSetBinCnt++;
                }
            }
        }

        for(int i=0; i<iSetBinCnt; i++)
        {
            array_obj[i]=cJSON_CreateObject();
        }

        k=iAuto+2;

        if(USE_COVER_TRAYID!=tCIDNotUse &&
           IniConfig.bA68_AutoLoadUnload==true)                                 //JerryYang 20250521 : For AMR
        {
            if(bUnloading==true)
            {
                cJSON_AddItemToObject(root, "FINISH", cJSON_CreateString("O"));
            }
            else
            {
                cJSON_AddItemToObject(root, "FINISH", cJSON_CreateString("X"));
            }
        }

        if(iAuto>=0 && iAuto<=2)
        {
            cJSON_AddItemToObject(root, "BUNDLE_ID", cJSON_CreateString(asBundleTrayID[k].c_str()));
        }
        else
        {
            if(iAuto==6)
            {
                cJSON_AddItemToObject(root, "BUNDLE_ID", cJSON_CreateString(fNote->edBundleID->Text.c_str()));
                sUnloadBundleID=fNote->edBundleID->Text;
            }
            else if(iAuto==7)
            {
                cJSON_AddItemToObject(root, "BUNDLE_ID", cJSON_CreateString(fNote->edBundleID->Text.c_str()));
                sUnloadBundleID=fNote->edBundleID->Text;
            }
            else if(iAuto==8)
            {
                cJSON_AddItemToObject(root, "BUNDLE_ID", cJSON_CreateString(fNote->edBundleID->Text.c_str()));
                sUnloadBundleID=fNote->edBundleID->Text;
            }
        }
        cJSON_AddItemToObject(root, "LOT_NO", cJSON_CreateString(fSCKART->sLotID.c_str()));

        iBundleDieQty=0;                                                        //JerryYang 20250220 : fix AUTO IN OUT
        for(int i=0; i<iSetBinCnt; i++)
        {
            str=IntToStr(iBinCnt[i]);
            iBundleDieQty+=iBinCnt[i];                                          //JerryYang 20250220 : fix AUTO IN OUT

            cJSON_AddItemToObject(array_obj[i], "BIN", cJSON_CreateString(sName[i].c_str()));
            cJSON_AddItemToObject(array_obj[i], "QTY", cJSON_CreateString(str.c_str()));
        }

        for(int i=0; i<iSetBinCnt; i++)
        {
            cJSON_AddItemToArray(likeObject, array_obj[i]);
        }
        cJSON_AddItemToObject(root, "Detail", likeObject);
    }
//    cJSON_Delete(root);
    return cJSON_Print(root);
}
#endif // TODO(W7)
//AI(ht9045-v899) 20260626: ungated GetErrorMessage -- Win32 FormatMessage only
//------------------------------------------------------------------------------
AnsiString GetErrorMessage(DWORD dwErrorMessageCode)                            //Steven 20240911 : ����t�ο��~���T��
{
    AnsiString strMsg;
    LPVOID lpMsgBuf;

    FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                   NULL,
                   dwErrorMessageCode,
                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),                   // �w�]�y��
                   (LPTSTR) &lpMsgBuf,
                   0,
                   NULL);

    strMsg.sprintf(("Error Code : 0x%02X ==> Error Message : %s "), dwErrorMessageCode, lpMsgBuf);
    { AnsiString t; for(int i=1;i<=strMsg.Length();i++){char c=strMsg[i];if(c!='\r'&&c!='\n')t+=c;} strMsg=t; } //AI(ht9045-v899) 20260626: StringReplace(rfReplaceAll)->manual CR/LF strip (vclcompat has no TReplaceFlags)
    // (CR and LF both stripped in loop above)
    LocalFree(lpMsgBuf);                                                        // �O�ofree���Ŷ��A�i���n�ߺD
    return strMsg;
}
//AI(ht9045-v899) 20260626: ungated VC8ToKpa/KpaToVC8/IsDoubleEqual -- pure math
//------------------------------------------------------------------------------
double VC8ToKpa(int iVal) //intput 0~32767         output -116.0~148.0 Kpa      //Sam 20230210 : �s�W VacuumUnit �q�T�Ҳ�
{
    double dKpa=0.0;
    double dValue=0.0;
    dValue=(double)iVal;

    dKpa=((3.3*(dValue/32767.0))-1.45)/0.0125;                                  //�l�洣�Ѫ�����

    if(dKpa>148)
        dKpa=148.0;
    else if(dKpa<-116)
        dKpa=-116.0;
    return dKpa;
}
//------------------------------------------------------------------------------
int KpaToVC8(double dKpa) //intput -116.0~148.0 Kpa    output 0~32767           //Sam 20230210 : �s�W VacuumUnit �q�T�Ҳ�
{
    int iVal=0;

    iVal=32767.0*(((0.0125*dKpa)+1.45)/3.3);                                    //�l�洣�Ѫ�����

    if(iVal>32767)
        iVal=32767.0;
    else if(iVal<0)
        iVal=0;

   return iVal;
}
//------------------------------------------------------------------------------
bool IsDoubleEqual(const double a, const double b, const double tolerance)
{
    return fabs(a-b)<=tolerance;
}
//------------------------------------------------------------------------------
