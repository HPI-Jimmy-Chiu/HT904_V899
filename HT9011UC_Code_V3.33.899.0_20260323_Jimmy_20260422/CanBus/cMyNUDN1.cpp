#include "MachineDefine.h"
#pragma hdrstop

#include "cMyNUDN1.h"
#include "cMyDNM100UD.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TMyNUDN1::TMyNUDN1(unsigned char _iActivedBoardNo,unsigned char _iDesMACID,unsigned char _iType,unsigned short _iInputLen,unsigned short _iOutputLen,unsigned short _iEPR,int _iAmplifier): TComponent(Owner)
{
    //int i;
    //unsigned char iInitailRaw[40];
    iActivedBoardNo=_iActivedBoardNo;
    iDesMACID=_iDesMACID;
    iType=_iType;
    iInputLen=_iInputLen;
    iOutputLen=_iOutputLen;
    iEPR=_iEPR;
    iAmplifier=_iAmplifier;
//    Initail();
//    for(int i=0; i<16; i++)
//    {
//        iGetIO_Value[i]=0;
//        iSetIO_Value[i]=-1;
//    }
//    DNM100_AddDevice(iActivedBoardNo,iDesMACID,2500);
//    DNM100_AddIOConnection(iActivedBoardNo,iDesMACID,iType,iInputLen,iOutputLen,iEPR);
//    for(i=0; i<40; i++)
//    {
//        InAreaMonitor.iRawItem[i]=iInitailRaw[i];
//    }
};
//---------------------------------------------------------------------------
__fastcall TMyNUDN1::~TMyNUDN1()
{

};
//---------------------------------------------------------------------------
void TMyNUDN1::Initail()
{
    int i;
    unsigned char iInitailRaw[40];
    for(int i=0; i<16; i++)
    {
        iGetIO_Value[i]=0;
        iSetIO_Value[i]=-1;
    }
    DNM100_AddDevice(iActivedBoardNo,iDesMACID,2500);
    DNM100_AddIOConnection(iActivedBoardNo,iDesMACID,iType,iInputLen,iOutputLen,iEPR);
    for(i=0; i<40; i++)
    {
        InAreaMonitor.iRawItem[i]=iInitailRaw[i];
    }
}

//---------------------------------------------------------------------------
int TMyNUDN1::iReadInputData(int iOperatingMode)
{
    int iRet,i;
    unsigned char iIOData[50];
    unsigned char iInputData[50];
    unsigned short iLen;    //Sam 20210518 : 修正讀取異常時 InputData 被清除0問題

    iLen=iInputLen; //Sam 20210518 : 修正讀取異常時 InputData 被清除0問題

    memset(iIOData,0,50);

    for(i=0; i<50; i++)
    {
        iInputData[i]=0;
    }

    if(iOperatingMode==0)           //NUDN1 I/O Communication Standard mode
    {

    }
    else if(iOperatingMode==1)      //NUDN1 I/O Communication External input mode
    {

    }
    else if(iOperatingMode==2)      //NUDN1 I/O Communication Monitor mode
    {
        iRet=DNM100_WriteOutputData(iActivedBoardNo,iDesMACID,iType,iOutputLen,iIOData);
        if(iRet!=0)
            return -iRet;

        iRet=DNM100_ReadInputData(iActivedBoardNo,iDesMACID,iType,&iLen,iInputData);    //Sam 20210518 : 修正讀取異常時 InputData 被清除0問題
        if(iRet==0)
        {
            for(i=0; i<iLen; i++) //Sam 20210518 : 修正讀取異常時 InputData 被清除0問題
            {
                InAreaMonitor.iRawItem[i]=iInputData[i];
            }
        }
        else
        {
            return -iRet;
        }
    }
    else if(iOperatingMode==3)      //NUDN1 I/O Communication Full mode
    {

    }
    else if(iOperatingMode==4)      //NUDN1 I/O Communication Minimum mode
    {

    }
    else
    {
        return -1;
    }
    return 0;
};
//---------------------------------------------------------------------------
int TMyNUDN1::iGetValue(int iAmplifierNo)
{
    if(iAmplifierNo>=0 && iAmplifierNo<=15)
    {
        return InAreaMonitor.CurrectValue(iAmplifierNo);
    }
    else
    {
        return 0;
    }
};
//---------------------------------------------------------------------------
bool TMyNUDN1::bGetIO(int iAmplifierNo)
{
    if(iAmplifierNo>=0 && iAmplifierNo<=15)
    {
        return InAreaMonitor.CurrectOutput(iAmplifierNo);
    }
    else
    {
        return false;
    }
};
//---------------------------------------------------------------------------
int TMyNUDN1::iSetIO_Value_Send(int iAmplifierNo,int iSetValue)
{

    int iRet;
    UNIONWORD iSetValue_HL;
    unsigned char AttributeID[3];
    //Input
    unsigned char ServiceID         =SETW;
    unsigned short ClassID          =NUDN1ID;
    unsigned short InstanceID       =(iAmplifierNo+1)*10+2;
    unsigned short AttributeIDLen   =3;
    iSetValue_HL.WHOLE_WORD         =(unsigned short)iSetValue;
    AttributeID[0]                  =SETVALUE;
    AttributeID[1]                  =iSetValue_HL.BYTES.Lo_BYTE;   //低位元
    AttributeID[2]                  =iSetValue_HL.BYTES.Hi_BYTE;   //高位元

    if(iAmplifierNo>=0 && iAmplifierNo<=15)
    {
        iRet=DNM100_SendExplicitMSG_W(iActivedBoardNo,iDesMACID,ServiceID,ClassID,InstanceID,AttributeIDLen,AttributeID);
    }
    else
    {
        iRet=-1;
    }
    return -iRet;
};
//---------------------------------------------------------------------------
