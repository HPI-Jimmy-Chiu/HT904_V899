//---------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop

#include "MyNUEC1.h"

#include "MyEtherCAT.h"
#include "cmydef.h"
#include "AdvMotDrv.h"
#include "AdvMotApi.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TMyNUEC1::TMyNUEC1(unsigned char _iRing,unsigned char _iDesMACID,unsigned char _iAmplifier): TComponent(Owner)
{
    iRing=_iRing;
    iDesMACID=_iDesMACID;
    iAmplifier=_iAmplifier;
    DataSize=128;

    if(NUEC_TYPE==eNUEC1)  //0:NUEC1
    {
        //CurValue
        //方法1 高低位元 Index=6+N*4(低位元) Index=7+N*4(低位元)
        CurValue_N=4;
        //方法2 Index=0x6100+N*0x10  SubIndex=0x11
        CurValue_Index=0x6100;
        CurValue_SubIndex=0x11;

        //Set IO Value
        SetValue_Index=0x8002;
        SetValue_SubIndex=0x1;

        //IO
        IO_Index=0x3600;        //NG NU-EC1 不支援一次讀16顆，所以還是用舊方法一次讀8顆
        IO_SubIndex=0x01;

        iMethod=1;              //NUEC1 新舊方法都可以讀到16顆
    }
    else                  //1:NUEC1A(HT505S 在用比較貴)
    {
        //CurValue
        //方法1 高低位元 Index=6+N*2(低位元) Index=7+N*2(低位元) 此方法在 NU-EC1A 只能讀取到8顆
        CurValue_N=2;
        //方法2 Index=0x8001+N*0x10  SubIndex=0x05
        //CurValue_Index=0x8001;    //手冊寫這個也可以
        //CurValue_SubIndex=0x05;   //手冊寫這個也可以
        CurValue_Index=0x6100;
        CurValue_SubIndex=0x11;

        //Set IO Value
        SetValue_Index=0x6100;
        SetValue_SubIndex=0x12;

        //IO
        IO_Index=0x3600;       //NG NU-EC1A 支援一次讀16顆，但速度比較慢，還是用舊方法一次讀8顆
        IO_SubIndex=0x01;

        iMethod=1;              //NUEC1A 舊方法最多只能讀到8顆，所以使用新方法
    }
    ZeroMemory(iCurrectValue, sizeof(iCurrectValue));
    ZeroMemory(iSettingValue, sizeof(iSettingValue));
    ZeroMemory(iNeedSettingValue, sizeof(iNeedSettingValue));
    ZeroMemory(RetWriteSettingValue, sizeof(RetWriteSettingValue));
};
//---------------------------------------------------------------------------
__fastcall TMyNUEC1::~TMyNUEC1()
{
};
//---------------------------------------------------------------------------
bool TMyNUEC1::ReadCurrentValue()
{
    bool bRet=true;
    for(int i=0;i<iAmplifier;i++)
    {
        if(ReadCurrentValue(i)==false)
            bRet=false;
    }
    return bRet;
}
//---------------------------------------------------------------------------
bool TMyNUEC1::ReadCurrentValue(int iAmplifierNo)                               //讀取1個 iAmplifier 目前數值
{
    bool bRet=true;

    if(iMethod==0)
    {
        unsigned long Result1,Result2;
        unsigned char HighValue;
        unsigned char LowValue;
        AnsiString strLow="",strHigh="",strLowHigh="";

        Result1=Acm_DaqDiGetByteEx(uiDevhand, iRing, iDesMACID, 6+(iAmplifierNo*CurValue_N), &LowValue);
        Result2=Acm_DaqDiGetByteEx(uiDevhand, iRing, iDesMACID, 7+(iAmplifierNo*CurValue_N), &HighValue);

        if(Result1==SUCCESS && Result2==SUCCESS)
        {
            strLow=IntToHex(LowValue,2);
            strHigh=IntToHex(HighValue,2);
            AnsiString strLowHigh=strHigh+strLow;
            strLowHigh=HexStrToInt(strLowHigh);
            iCurrectValue[iAmplifierNo]=atoi(strLowHigh.c_str());
        }
        else
        {
            iCurrectValue[iAmplifierNo]=-99999;
            bRet=false;
        }
    }
    else
    {
        unsigned long Result;
        short iCurValue;
        Result=Acm_DevReadSDOData(uiDevhand, iRing, iDesMACID, CurValue_Index+(iAmplifierNo*0x0010), CurValue_SubIndex,  ECAT_TYPE_I16, DataSize, &iCurValue);
        if(Result==SUCCESS)
        {
            iCurrectValue[iAmplifierNo]=iCurValue;
        }
        else
        {
            iCurrectValue[iAmplifierNo]=-99999;
            bRet=false;
        }
    }
    return bRet;
}
//---------------------------------------------------------------------------
int TMyNUEC1::iGetValue(int iAmplifierNo)
{
    if(0<=iAmplifierNo && iAmplifierNo<MaxFSN12)
        return iCurrectValue[iAmplifierNo];
    else
        return 0;
};
//---------------------------------------------------------------------------
bool TMyNUEC1::ReadCurrentIO()
{
    bool bRet=false;
    if(NUEC_TYPE==eNUEC1 || NUEC_TYPE==eNUEC1A)  //因為比較快 NUEC1 & NUEC1A 都用這個方式讀 IO
    {
        unsigned long Result1;
        unsigned long Result2;
        unsigned char HighValue;
        unsigned char LowValue;
        AnsiString strLamp;
        //讀取後要轉換為2進制 0:滅 1:亮
        //10000010 >> 2 8 亮
        Result1=Acm_DaqDiGetByteEx(uiDevhand, iRing,  iDesMACID, 0, &LowValue);
        Result2=Acm_DaqDiGetByteEx(uiDevhand, iRing,  iDesMACID, 1, &HighValue);
        if(Result1==SUCCESS && Result2==SUCCESS)
        {
            IOArea.Area.IOCharL.iRaw=LowValue;
            IOArea.Area.IOCharH.iRaw=HighValue;
            bRet=true;
        }
        else
        {
            bRet=false;
        }
    }
    else     //僅 NUEC1A 也可以用這個方法，但會比較慢。
    {
        unsigned long Result;
        unsigned short iValue;
        Result=Acm_DevReadSDOData(uiDevhand, iRing, iDesMACID, IO_Index, IO_SubIndex,  ECAT_TYPE_I16, DataSize, &iValue);
        if(Result==SUCCESS)
        {
            IOArea.Area.IOWord.iRaw=iValue;
            bRet=true;
        }
        else
        {
            bRet=false;
        }
    }
    return bRet;
}
//---------------------------------------------------------------------------
bool TMyNUEC1::bGetIO(int iAmplifierNo)
{
    if(iAmplifierNo>=0 && iAmplifierNo<=MaxFSN12)
    {
        if(NUEC_TYPE==eNUEC1 || NUEC_TYPE==eNUEC1A)    //因為比較快 NUEC1 & NUEC1A 都用這個方式讀 IO
            return IOArea.CurrectOutput2(iAmplifierNo);
        else
            return IOArea.CurrectOutput(iAmplifierNo);
    }
    else
    {
        return false;
    }
};
//---------------------------------------------------------------------------
bool TMyNUEC1::ReadSettingValue()
{
    bool bRet=true;
    for(int i=0;i<iAmplifier;i++)
    {
        if(ReadSettingValue(i)==false)
            bRet=false;
    }
    return bRet;
}
//---------------------------------------------------------------------------
bool TMyNUEC1::ReadSettingValue(int iAmplifierNo)
{
    bool bRet=true;
    unsigned long Result;
    short iSetValue;
    if(NUEC_TYPE==eNUEC1)
    {
        Result=Acm_DevReadSDOData(uiDevhand, iRing, iDesMACID, SetValue_Index+iAmplifierNo*0x0010, SetValue_SubIndex,  ECAT_TYPE_I16, DataSize, &iSetValue);
        if(Result==SUCCESS)
        {
            iSettingValue[iAmplifierNo]=iSetValue;
        }
        else
        {
            iSettingValue[iAmplifierNo]=-99999;
            bRet=false;
        }
    }
    else
    {
        Result=Acm_DevReadSDOData(uiDevhand, iRing, iDesMACID, SetValue_Index+(iAmplifierNo*0x0010), SetValue_SubIndex,  ECAT_TYPE_I16, DataSize, &iSetValue);
        if(Result==SUCCESS)
        {
            iSettingValue[iAmplifierNo]=iSetValue;
        }
        else
        {
            iSettingValue[iAmplifierNo]=-99999;
            bRet=false;
        }
    }
    return bRet;
}
//---------------------------------------------------------------------------
int TMyNUEC1::iGetIO_Value(int iAmplifierNo)
{
    if(0<=iAmplifierNo && iAmplifierNo<=MaxFSN12)
        return iSettingValue[iAmplifierNo];
    else
        return 0;
};
//---------------------------------------------------------------------------
void TMyNUEC1::SetSetIOValue(int iAmplifierNo,int iValue)
{
    bNeedDosetValue[iAmplifierNo]=true;
    iNeedSettingValue[iAmplifierNo]=iValue;
}
//---------------------------------------------------------------------------
bool TMyNUEC1::WriteSettingValue()
{
    bool bRet=true;
    for(int i=0;i<iAmplifier;i++)
    {
        if(bNeedDosetValue[i])
        {
            if(WriteSettingValue(i,iNeedSettingValue[i])==false)
                 bRet=false;
        }
    }
    return bRet;
}
//---------------------------------------------------------------------------
bool TMyNUEC1::WriteSettingValue(int iAmplifierNo,int iValue)
{
    bool bRet=true;
    unsigned long Result;

    int InputValue=0;
    short InputValue2=0;
    bool bAllow=true;
    bool bSettingError;
    short iSettingValue;
    short iSettingValue2;
    RetWriteSettingValue[iAmplifierNo]=0;  //寫入前狀態清除
    //Index 7004, SubIndex 17-32
    //寫入 Setting Value
    InputValue=iValue;
    InputValue2=iValue;
    if(NUEC_TYPE==eNUEC1)
    {
        Result=Acm_DevWriteSDOData(uiDevhand, iRing, iDesMACID, SetValue_Index+iAmplifierNo*0x0010, SetValue_SubIndex,  ECAT_TYPE_I16, DataSize, &InputValue2);
        if(Result==SUCCESS)
        {
            Result=Acm_DevReadSDOData(uiDevhand, iRing,  iDesMACID, SetValue_Index+iAmplifierNo*0x0010, SetValue_SubIndex,  ECAT_TYPE_I16, DataSize, &iSettingValue2);
            if(Result==SUCCESS)
            {
                if(InputValue2==iSettingValue2)
                {
                    bNeedDosetValue[iAmplifierNo]=false;
                    RetWriteSettingValue[iAmplifierNo]=0;
                }
                else
                {
                    RetWriteSettingValue[iAmplifierNo]=3;
                    bRet=false;
                }
            }
            else
            {
                RetWriteSettingValue[iAmplifierNo]=2;
                bRet=false;
            }
        }
        else
        {
            RetWriteSettingValue[iAmplifierNo]=1;
            bRet=false;
        }
    }
    else    //NUEC1A 寫入資料的方式比較麻煩
    {
        Result=Acm_DevWriteSDOData(uiDevhand, iRing, iDesMACID, 0x7004, 17+iAmplifierNo,  ECAT_TYPE_I16, DataSize, &InputValue);
        if(Result==SUCCESS)
        {
            //允許寫入
            //Index 7004, SubIndex 1-16, 0:不允許 1:允許
            Result=Acm_DevWriteSDOData(uiDevhand, iRing,  iDesMACID, 0x7004, 1+iAmplifierNo,  ECAT_TYPE_BOOL, DataSize, &bAllow);
            if(Result==SUCCESS)
            {
                //確認寫入無錯誤
                //Index 6004, SubIndex 17-32, False=沒錯誤 True=錯誤
                Result=Acm_DevReadSDOData(uiDevhand, iRing,  iDesMACID, 0x6004, 17+iAmplifierNo,  ECAT_TYPE_BOOL, DataSize, &bSettingError);
                if(Result==SUCCESS)
                {
                    if(bSettingError)
                    {
                        RetWriteSettingValue[iAmplifierNo]=4;
                        bRet=false;
                    }
                    else
                    {
                        Result=Acm_DevReadSDOData(uiDevhand, iRing, iDesMACID, SetValue_Index+iAmplifierNo*0x0010, SetValue_SubIndex,  ECAT_TYPE_I16, DataSize, &iSettingValue);
                        if(Result==SUCCESS)
                        {
                            if(InputValue==iSettingValue)
                            {
                                bNeedDosetValue[iAmplifierNo]=false;
                                RetWriteSettingValue[iAmplifierNo]=0;
                            }
                            else
                            {
                                RetWriteSettingValue[iAmplifierNo]=6;
                                bRet=false;
                            }
                        }
                        else
                        {
                            RetWriteSettingValue[iAmplifierNo]=5;
                            bRet=false;
                        }
                    }
                }
                else
                {
                    RetWriteSettingValue[iAmplifierNo]=3;
                    bRet=false;
                }
            }
            else
            {
                RetWriteSettingValue[iAmplifierNo]=2;
                bRet=false;
            }
        }
        else
        {
            RetWriteSettingValue[iAmplifierNo]=1;
            bRet=false;
        }
    }
    return bRet;
}
//---------------------------------------------------------------------------
void TMyNUEC1::ClearError()
{
    ZeroMemory(bNeedDosetValue, sizeof(bNeedDosetValue));
    ZeroMemory(RetWriteSettingValue, sizeof(RetWriteSettingValue));
}
//------------------------------------------------------------------------------
bool TMyNUEC1::CheckAmplifierExist(int iAmplifierNo)                            //Sam 20230814 : EtherCAT 新增 Amplifier 檢查
{
    bool bRet=false;
    unsigned long Result;
    unsigned short iValue;

    Result=Acm_DevReadSDOData(uiDevhand, iRing, iDesMACID, 0x8000+iAmplifierNo*0x0010, 0x00,  ECAT_TYPE_U16, DataSize, &iValue);
    if(Result==SUCCESS && iValue==20)
        bRet=true;
    return bRet;
}
//------------------------------------------------------------------------------
AnsiString TMyNUEC1::CheckAmplifierExist()                                      //Sam 20230814 : EtherCAT 新增 Amplifier 檢查
{
    AnsiString sRet="";
    int AmpCnt=0;
    if(iAmplifier>0)
    {
        for(int i=0; i<iAmplifier; i++)
        {
            if(CheckAmplifierExist(i))
                AmpCnt++;
        }

        if(iAmplifier==AmpCnt)
            sRet="OK";
        else
            sRet.printf("NU-EC1[%d] Amp is %d less than %d", iDesMACID, AmpCnt, iAmplifier);
    }
    else
    {
        sRet="OK";
    }
    return sRet;
}
//------------------------------------------------------------------------------
