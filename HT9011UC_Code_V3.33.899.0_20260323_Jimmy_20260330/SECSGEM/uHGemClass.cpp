#include "MachineDefine.h"
#pragma hdrstop

#include "uHGemClass.h"
#include "cmydef.h"    //wei 20150511
#include "ExternFunction.h"
#include "common.h"
#include "csystem.h"
#include "uShowMessage.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
AnsiString SYS_ECChangeID             = "";        //pig 2014.04.23 KYEC SECS
AnsiString SYS_ECChangeIDOriginaValue = "";        //pig 2014.04.23 KYEC SECS
AnsiString SYS_ECChangeIDNewValue     = "";        //pig 2014.04.23 KYEC SECS
//------------------------------------------------------------------------------
bool IsCorrectDateFormat(int y,int m,int d)
{
    int   month[]={31,0,31,30,31,30,31,31,30,31,30,31};
    if(y<=2000 || y>9999)
        return   false;
    if(m<1 || m>12)
        return   false;
    if(((y%4)==0 && (y%100)!=0) || (y%400==0))
        month[1]=29;
    else
        month[1]=28;
    if(d>=1 && d<=month[m-1])
        return true;
    else
        return false;
}
//---------------------------------------------------------------------------
HTGem::HTGem()
{
    HGemPtr=NULL;
    HandlerPath="";
    SecsAlarmMessage=new TStringList;                                           //Steven 20150519 : 修正SECS GEM使用ShowMyMessage會出現記憶體破壞
    FMessageList=new TStringList;                                               //Ifor 20251018 add:Secs Alarm List
};
//---------------------------------------------------------------------------
HTGem::HTGem(THGem *HGemTmp)
{
    HGemPtr=HGemTmp;
    HandlerPath="";
    SecsAlarmMessage=new TStringList;                                           //Steven 20150519 : 修正SECS GEM使用ShowMyMessage會出現記憶體破壞
    FMessageList=new TStringList;                                               //Ifor 20251018 add:Secs Alarm List
}
//---------------------------------------------------------------------------
HTGem::HTGem(AnsiString Path)
{
    HandlerPath=Path;
    SecsAlarmMessage=new TStringList;                                           //Steven 20150519 : 修正SECS GEM使用ShowMyMessage會出現記憶體破壞
    FMessageList=new TStringList;                                               //Ifor 20251018 add:Secs Alarm List
}
//---------------------------------------------------------------------------
HTGem::~HTGem()                                                                 //Steven 20150519 : 修正SECS GEM使用ShowMyMessage會出現記憶體破壞
{
    try
    {
        if(SecsAlarmMessage!=NULL)
        {
            SecsAlarmMessage->Clear();                                          //Ifor 20170603 (wei) TStringList 刪除前先 Clean
            delete SecsAlarmMessage;
        }
        SecsAlarmMessage=NULL;                                                  //kevin 20180907 add

        if(FMessageList!=NULL)                                                  //Ifor 20251018 add:Secs Alarm List
        {
            FMessageList->Clear();
            delete FMessageList;
        }
        FMessageList = NULL;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~HTGem");
    }
}
//---------------------------------------------------------------------------
void HTGem::UpdateDataPath(AnsiString Path)
{
    DataPath=Path;
}
//------------------------------------------------------------------------------
// V 1.0
// 對 與 remote 建立連線後的 Online[online local or remore] or offline的要求傳送
//------------------------------------------------------------------------------
void HTGem::S1F1_AreYouThereRequest()
{
    HGemPtr->bS1F2_OnLineData=false;
    HGemPtr->InitLocalHead(1,1,0);
    HGemPtr->StringOut("[Send]    AreYouThereRequest");
    HGemPtr->SendLocalData();
}
//---------------------------------------------------------------------------
// 2013/05/27
// V1.1
// Lee
// [S1F2] OnLineData : Data signifying that the equipment is alive.
//---------------------------------------------------------------------------
void HTGem::S1F2_OnLineData()
{
    if(HGemPtr->CheckSFFormatOnlyHead("S1,F1 Format error !!!")==false)
        return;
    HGemPtr->InitLocalHead(1, 2, 0);
    HGemPtr->DataItemOut(2, HType.LIST_TYPE, NULL);
    HGemPtr->DataItemOut(HType.ASCII_TYPE, HGemPtr->GemMDLN);
    HGemPtr->DataItemOut(HType.ASCII_TYPE, HGemPtr->GemSOFTREV);
    HGemPtr->SendLocalData();
}
//---------------------------------------------------------------------------
// V 1.0
// The equipment reports the value of each SVID requested in the order requested.
// The host remembers the names of values
// requested.
//
//---------------------------------------------------------------------------
void HTGem::S1F4_SelectedStatusReply()
{
    int SVlen, len, i;
    unsigned char Type;
    AnsiString S;

    if(HGemPtr->GetDataItemLenAndType(SVlen, Type)==1)
    {
        if(Type==HType.LIST_TYPE || SVlen==0)  // new
        {
            HGemPtr->GetDataItemLenAndTypeAndDelete(SVlen, Type);
            if(SVlen==0)                                                        // report all svid
            {
                HGemPtr->InitLocalHead(1, 4, 0);
                len=HGemPtr->SV_ID->Count;
                HGemPtr->DataItemOut(len, HType.LIST_TYPE, NULL);
                for(i=0; i<len; i++)
                {
                    HGemPtr->DataItemOutSV(atoi(HGemPtr->SV_ID->Strings[i].c_str()));
                }
                HGemPtr->SendLocalData();
            }
            else
            {
                HGemPtr->InitLocalHead(1, 4, 0);
                HGemPtr->DataItemOut(SVlen, HType.LIST_TYPE, NULL);
                for(i=0; i<SVlen; i++)
                {
                    if(HGemPtr->GetDataItemLenAndType(len,Type)==1)
                        if(HGemPtr->DataItemIn(len, Type, S)==1)                // only for ascii,i1,i2,i4,i5 ,u1,u2,u4,u8
                            if(HGemPtr->DataItemOutSV(S)==false)
                                return;
                }
                HGemPtr->SendLocalData();
            }
        }
        else // old type
        {
            HGemPtr->InitLocalHead(1, 4, 0);
            HGemPtr->DataItemOut(SVlen, HType.LIST_TYPE, NULL);
            if(Type==HType.UINT_2_TYPE)
            {
                unsigned short *P;
                P=new unsigned short [SVlen];
                if(HGemPtr->DataItemIn(SVlen, Type, P))
                {
                    for(i=0; i<SVlen; i++)
                        if(HGemPtr->DataItemOutSV(P[i])==false)
                        {
                            delete[] P;
                            return;
                        }
                }
                delete[] P;
            }
            else if(Type==HType.UINT_4_TYPE)
            {
                unsigned *P;
                P=new unsigned[SVlen];
                if(HGemPtr->DataItemIn(SVlen, Type, P))
                {
                    for(i=0; i<SVlen; i++)
                        if(HGemPtr->DataItemOutSV(P[i])==false)
                        {
                            delete[] P;
                            return;
                        }
                }
                delete[] P;
            }
            else if(Type==HType.UINT_8_TYPE)                                    //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
            {
                unsigned __int64 *P;
                P=new unsigned __int64[SVlen];
                if(HGemPtr->DataItemIn(SVlen, Type, P))
                {
                    for(i=0; i<SVlen; i++)
                        if(HGemPtr->DataItemOutSV(P[i])==false)
                        {
                            delete[] P;
                            return;
                        }
                }
                delete[] P;
            }
            else if(Type==HType.INT_2_TYPE)
            {
                short *P;
                P=new short[SVlen];
                if(HGemPtr->DataItemIn(SVlen, Type, P))
                {
                    for(i=0; i<SVlen; i++)
                        if(HGemPtr->DataItemOutSV(P[i])==false)
                        {
                            delete[] P;
                            return;
                        }
                }
                delete[] P;
            }
            else if(Type==HType.INT_4_TYPE)
            {
                int *P;
                P=new int [SVlen];
                if(HGemPtr->DataItemIn(SVlen, Type, P))
                {
                    for(i=0; i<SVlen; i++)
                        if(HGemPtr->DataItemOutSV(P[i])==false)
                        {
                            delete[] P;
                            return;
                        }
                }
                delete[] P;
            }
            else if(Type==HType.INT_8_TYPE)                                     //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
            {
                __int64 *P;
                P=new __int64 [SVlen];
                if(HGemPtr->DataItemIn(SVlen, Type, P))
                {
                    for(i=0; i<SVlen; i++)
                        if(HGemPtr->DataItemOutSV(P[i])==false)
                        {
                            delete[] P;
                            return;
                        }
                }
                delete[] P;
            }
            else
            {
                S9F7_IllegalData("S1,F3 data format error");
                return;
            }
            HGemPtr->SendLocalData();
        }
    }
    else
    {
        S9F7_IllegalData("S1,F3 data format error");
    }
}
// ---------------------------------------------------------------------------
// V 1.0
// The equipment reports to the host the name and units of the requested SVs.
// ---------------------------------------------------------------------------
void HTGem::S1F12_StatusVariableNamelistReply()
{
    int SVlen, i, len;
    unsigned char Type;
    AnsiString S;

    if(HGemPtr->GetDataItemLenAndTypeAndDelete(SVlen,Type)==1)
    {
        if(Type==HType.LIST_TYPE || SVlen==0)
        {
            HGemPtr->InitLocalHead(1,12,0);
            if(SVlen==0)
            {
                len=HGemPtr->SV_ID->Count;
                HGemPtr->DataItemOut(len, HType.LIST_TYPE, NULL);
                for(i=0; i<len; i++)
                    HGemPtr->DataItemOutSVNameList(HGemPtr->SV_ID->Strings[i]);
            }
            else
            {
                HGemPtr->DataItemOut(SVlen, HType.LIST_TYPE, NULL);
                for(i=0; i<SVlen; i++)
                {
                    if(HGemPtr->GetDataItemLenAndType(len,Type)==1)
                    {
                        if(HGemPtr->DataItemIn(len, Type, S)==1)
                        {
                            if(HGemPtr->DataItemOutSVNameList(S)==false)
                                return;
                        }
                        else
                        {
                            S9F7_IllegalData("S1,F11 data format error");
                            return;
                        }
                    }
                    else
                    {
                        S9F7_IllegalData("S1,F11 data format error");
                        return;
                    }
                }
            }
            HGemPtr->SendLocalData();
        }
        else
        {
            S9F7_IllegalData("S1,F11 data format error");
        }
    }
    else
    {
        S9F7_IllegalData("S1,F11 data format error");
    }
}
//------------------------------------------------------------------------------
// V 1.0
// 對 與 remote 建立連線的要求傳送
// Select.req
//------------------------------------------------------------------------------
void HTGem::S1F13_EstablishCommunicationsRequest()
{
    AnsiString S;
    HGemPtr->InitLocalHead(1, 13, 0);
    HGemPtr->DataItemOut(2, HType.LIST_TYPE, NULL);
    HGemPtr->DataItemOut(HType.ASCII_TYPE, HGemPtr->GemMDLN);
    HGemPtr->DataItemOut(HType.ASCII_TYPE, HGemPtr->GemSOFTREV);
    HGemPtr->SendLocalData();
}
// ---------------------------------------------------------------------------
// V 1.0
// Accept or deny Establish Communications Request (S1,F13).
// MDLN and SOFTREV are on-line data and are valid only if
// COMMACK = 0.
// ---------------------------------------------------------------------------
void HTGem::S1F14_ConnectRequestAcknowledge()
{
    unsigned char Command=0,Type;
    AnsiString S, ret;
    int len;

    // < L[0]
    // >.

    if(HGemPtr->chkMoreMessageAbortProcess->Checked)
    {
        ret=HGemPtr->GetDataItemLenAndTypeAndDelete(len, Type);
        if(ret!=1 || len!=0 || Type!=HType.LIST_TYPE)
        {
            S9F7_IllegalData("S1,F13 data format error");
            return;
        }
    }
    HGemPtr->InitLocalHead(1,14,0);

    HGemPtr->DataItemOut(2, HType.LIST_TYPE, NULL);
    HGemPtr->DataItemOut(1, HType.BINARY_TYPE, &Command);
    HGemPtr->DataItemOut(2, HType.LIST_TYPE, NULL);
    HGemPtr->DataItemOut(HType.ASCII_TYPE, HGemPtr->GemMDLN);
    HGemPtr->DataItemOut(HType.ASCII_TYPE, HGemPtr->GemSOFTREV);
    HGemPtr->SendLocalData();
    HGemPtr->bReceiveEstablishCommunicationsRequest=true;
}
//------------------------------------------------------------------------------
void HTGem::Process_S1F14_ConnectRequestAcknowledge()
{
    if(HGemPtr->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
    {
        unsigned char binarydata[1];
        if(HGemPtr->DataItemIn(1, HType.BINARY_TYPE, binarydata)==1)
        {
            if(HGemPtr->DataItemIn(0, HType.LIST_TYPE, NULL)!=1)
            {
                S9F7_IllegalData("S1,F14 data format error");
                return;
            }
            if(binarydata[0]==0x00)
            {
                HGemPtr->bWaitEstablishCommunicationsResponse=true;
                HGemPtr->bWaitEstablishCommunicationsResponseError=false;
                return;
            }
        }
        else
        {
            S9F7_IllegalData("S1,F14 data format error");
            return;
        }
    }
    else
    {
        S9F7_IllegalData("S1,F14 data format error");
        return;
    }
    HGemPtr->bWaitEstablishCommunicationsResponse=true;
    HGemPtr->bWaitEstablishCommunicationsResponseError=true;
}
// ---------------------------------------------------------------------------
// V 1.0
// The host requests that the equipment transition to the OFF-LINE state.
// ---------------------------------------------------------------------------
void HTGem::S1F16_OFFLINEAcknowledge()
{
    unsigned char Command=0;
    if(HGemPtr->CheckSFFormatOnlyHead("S1,F15 Format error !!!")==false)
        return;

    HGemPtr->InitLocalHead(1, 16, 0);
    HGemPtr->DataItemOut(1, HType.BINARY_TYPE, &Command);
    HGemPtr->SendLocalData();
    HGemPtr->OffLine();
}
// ---------------------------------------------------------------------------
// V 1.0
// Acknowledge or error
// ---------------------------------------------------------------------------
void HTGem::S1F18_ONLINEAcknowledge()
{
    unsigned char Command=0;
    if(HGemPtr->CheckSFFormatOnlyHead("S1,F17 Format error !!!")==false)
        return;

    HGemPtr->InitLocalHead(1, 18, 0);
    if(HGemPtr->bOnLine)
    {
        Command=2;
    }
    else if(HGemPtr->GemCheckBoxAcceptHostOnlineRequest->Checked)
    {
        HGemPtr->bOnLine=true;
        Command=0;
    }
    else
    {
        Command=1;
    }
    HGemPtr->DataItemOut(1, HType.BINARY_TYPE, &Command);
    HGemPtr->SendLocalData();
    if(Command==0)
        HGemPtr->OnLine(HGemPtr->GetOnLineMode());
}
// ---------------------------------------------------------------------------
//2014/01/01  lee  // add S1F23
void HTGem::S1F24_CollectionEventNamelist()
{
    int SVlen, len;
    unsigned char Type;

    int   pos;
    unsigned int   uint4SV;
    bool ret;
    AnsiString S;
    TStringList *CEIDList,*SVIDList,*BackCeid;

    CEIDList=new TStringList;
    SVIDList=new TStringList;
    BackCeid=new TStringList;

    CEIDList->Clear();
    BackCeid->Clear();

    if(HGemPtr->GetDataItemLenAndTypeAndDelete(SVlen, Type)==1)
    {
        if(Type==HType.LIST_TYPE || SVlen==0)
        {
            HGemPtr->InitLocalHead(1, 24, 0);
            if(SVlen==0)                                                        // report all CEID name and SVID
            {
                for(int i=1; i<HGemPtr->strGrdCEID->RowCount; i++)
                {
                    if(HGemPtr->strGrdCEID->Cells[0][i]!="" && HGemPtr->strGrdCEID->Cells[0][i]!=NULL)
                        CEIDList->Add(i);
                }
                len=CEIDList->Count;

                HGemPtr->DataItemOut(len, HType.LIST_TYPE, NULL);
                for(int j=0; j<len; j++)
                {
                    HGemPtr->DataItemOut(3, HType.LIST_TYPE, NULL);
                    pos=atoi(CEIDList->Strings[j].c_str());
                    uint4SV=atoi(HGemPtr->strGrdCEID->Cells[0][pos].c_str());
                    HGemPtr->DataItemOut(1, HType.UINT_4_TYPE, &uint4SV);
                    HGemPtr->DataItemOut(HType.ASCII_TYPE, HGemPtr->strGrdCEID->Cells[2][pos]);
                    SVIDList->Clear();
                    for(int k=3; k<HGemPtr->strGrdCEID->ColCount; k++)
                    {
                        S=HGemPtr->strGrdCEID->Cells[k][pos];
                        if(S!="")
                        {
                            for(int y=1; y<HGemPtr->stdGridReportID->RowCount; y++)
                            {
                                if(S==HGemPtr->stdGridReportID->Cells[0][y])
                                {
                                    for(int x=2; x<HGemPtr->stdGridReportID->ColCount; x++)
                                    {
                                        if(HGemPtr->stdGridReportID->Cells[x][y]!="")
                                            SVIDList->Add(HGemPtr->stdGridReportID->Cells[x][y]);
                                    }
                                }
                            }
                        }
                    }
                    HGemPtr->DataItemOut(SVIDList->Count, HType.LIST_TYPE, NULL);
                    for(int k=0; k<SVIDList->Count; k++)
                    {
                        uint4SV=atoi(SVIDList->Strings[k].c_str());
                        HGemPtr->DataItemOut(1, HType.UINT_4_TYPE, &uint4SV);
                    }
                }
            }
            else
            {
                for(int j=0; j<SVlen; j++)
                {
                    if(HGemPtr->GetDataItemLenAndType(len,Type)==1)
                    {
                        if(HGemPtr->DataItemIn(len, Type, S)==1)
                        {
                            ret=false;
                            for(int i=1; i<HGemPtr->strGrdCEID->RowCount; i++)
                            {
                                if(HGemPtr->strGrdCEID->Cells[0][i]==S)
                                {
                                    CEIDList->Add(i);
                                    ret=true;
                                    break;
                                }
                            }
                            BackCeid->Add(S);
                            if(ret==false)
                                CEIDList->Add(0);
                        }
                    }
                }
                len=CEIDList->Count;
                HGemPtr->DataItemOut(len, HType.LIST_TYPE, NULL);
                for(int j=0; j<len; j++)
                {
                    HGemPtr->DataItemOut(3, HType.LIST_TYPE, NULL);
                    pos=atoi(CEIDList->Strings[j].c_str());
                    if(pos==0)                                                  // no such CEID
                    {
                        uint4SV=atoi(BackCeid->Strings[j].c_str());
                        HGemPtr->DataItemOut(1, HType.UINT_4_TYPE,&uint4SV);
                        HGemPtr->DataItemOut(HType.ASCII_TYPE, AnsiString(""));
                        HGemPtr->DataItemOut(0, HType.LIST_TYPE, NULL);
                    }
                    else
                    {
                        uint4SV=atoi(HGemPtr->strGrdCEID->Cells[0][pos].c_str());
                        HGemPtr->DataItemOut(1, HType.UINT_4_TYPE,&uint4SV);
                        HGemPtr->DataItemOut(HType.ASCII_TYPE, HGemPtr->strGrdCEID->Cells[2][pos]);
                        SVIDList->Clear();
                        for(int k=3; k<HGemPtr->strGrdCEID->ColCount; k++)
                        {
                            S=HGemPtr->strGrdCEID->Cells[k][pos];
                            if(S!="")
                            {
                                for(int y=1; y<HGemPtr->stdGridReportID->RowCount; y++)
                                {
                                    if(S==HGemPtr->stdGridReportID->Cells[0][y])
                                    {
                                        for(int x=2; x<HGemPtr->stdGridReportID->ColCount; x++)
                                        {
                                            if(HGemPtr->stdGridReportID->Cells[x][y]!="")
                                                SVIDList->Add(HGemPtr->stdGridReportID->Cells[x][y]);
                                        }
                                    }
                                }
                            }
                        }
                        HGemPtr->DataItemOut(SVIDList->Count, HType.LIST_TYPE, NULL);
                        for(int k=0; k<SVIDList->Count; k++)
                        {
                            uint4SV=atoi(SVIDList->Strings[k].c_str());
                            HGemPtr->DataItemOut(1, HType.UINT_4_TYPE, &uint4SV);
                        }
                    }
                }
            }
            HGemPtr->SendLocalData();
        }
        else
        {
            S9F7_IllegalData("S1,F11 data format error");
        }
    }
    else
    {
        S9F7_IllegalData("S1,F11 data format error");
    }
    CEIDList->Clear();                                                          //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    SVIDList->Clear();                                                          //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    BackCeid->Clear();                                                          //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete CEIDList;
    delete SVIDList;
    delete BackCeid;
}
// ---------------------------------------------------------------------------
// V 1.0
// Data Response to S2,F13 in the order requested.
// ---------------------------------------------------------------------------
void HTGem::S2F14_EquipmentConstanData()
{
    int EClen, len;
    unsigned char Type;
    AnsiString S;

    if(HGemPtr->GetDataItemLenAndType(EClen, Type)==1)
    {
        if(Type==HType.LIST_TYPE || EClen==0)
        {
            HGemPtr->GetDataItemLenAndTypeAndDelete(EClen, Type);
            HGemPtr->InitLocalHead(2, 14, 0);
            if(EClen==0)
            {
                len=HGemPtr->EC_ID->Count;
                HGemPtr->DataItemOut(len, HType.LIST_TYPE, NULL);
                for(int i=0; i<len; i++)
                    HGemPtr->DataItemOutEC(HGemPtr->EC_ID->Strings[i]);
            }
            else
            {
                HGemPtr->DataItemOut(EClen, HType.LIST_TYPE, NULL);
                for(int i=0; i<EClen; i++)
                {
                    if(HGemPtr->GetDataItemLenAndType(len, Type)==1)
                    {
                        if(HGemPtr->DataItemIn(len, Type, S)==1)
                            HGemPtr->DataItemOutEC(S);
                    }
                    else
                    {
                        S9F7_IllegalData("S2,F13 data format error");
                        return;
                    }
                }
            }
            HGemPtr->SendLocalData();
        }
        else
        {
            HGemPtr->InitLocalHead(2, 14, 0);
            HGemPtr->DataItemOut(EClen, HType.LIST_TYPE, NULL);
            if(Type==HType.UINT_2_TYPE)
            {
                unsigned short *P;
                P=new unsigned short [EClen];
                if(HGemPtr->DataItemIn(EClen, Type, P))
                    for(int i=0; i<EClen; i++)
                        HGemPtr->DataItemOutEC(P[i]);
                delete[] P;
            }
            else if(Type==HType.UINT_4_TYPE)
            {
                unsigned *P;
                P=new unsigned [EClen];
                if(HGemPtr->DataItemIn(EClen, Type, P))
                    for(int i=0; i<EClen; i++)
                        HGemPtr->DataItemOutEC(P[i]);
                delete[] P;
            }
            else if(Type==HType.INT_2_TYPE)
            {
                short *P;
                P=new short [EClen];
                if(HGemPtr->DataItemIn(EClen, Type, P))
                    for(int i=0; i<EClen; i++)
                        HGemPtr->DataItemOutEC(P[i]);
                delete[] P;
            }
            else if(Type==HType.INT_4_TYPE)
            {
                int *P;
                P=new int[EClen];
                if(HGemPtr->DataItemIn(EClen, Type, P))
                    for(int i=0; i<EClen; i++)
                        HGemPtr->DataItemOutEC(P[i]);
                delete[] P;
            }
            else if(Type==HType.INT_8_TYPE)                                     //Steven 20130730 ADD
            {
                __int64 *P;
                P=new __int64[EClen];
                if(HGemPtr->DataItemIn(EClen, Type, P))
                    for(int i=0; i<EClen; i++)
                        HGemPtr->DataItemOutEC(P[i]);
                delete[] P;
            }
            else if(Type==HType.FT_4_TYPE)                                      //Steven 20130730 ADD
            {
                float *P;
                P=new float[EClen];
                if(HGemPtr->DataItemIn(EClen, Type, P))
                    for(int i=0; i<EClen; i++)
                        HGemPtr->DataItemOutEC(P[i]);
                delete[] P;
            }
            else if(Type==HType.FT_8_TYPE)                                      //Steven 20130730 ADD
            {
                double *P;
                P=new double[EClen];
                if(HGemPtr->DataItemIn(EClen, Type, P))
                    for(int i=0; i<EClen; i++)
                        HGemPtr->DataItemOutEC(P[i]);
                delete[] P;
            }
            else
            {
                S9F7_IllegalData("S21,F13 data format error");
                return;
            }
            HGemPtr->SendLocalData();
        }
    }
    else
    {
        S9F7_IllegalData("S2,F13 data format error");
    }
}
// ---------------------------------------------------------------------------
// V 1.0
// Data Response to S2,F13 in the order requested.
// ---------------------------------------------------------------------------
void HTGem::S2F16_NewEquipmentConstantSendAcknowledge()
{
    int ret;
    HGemPtr->SReceiveDataBackup->Assign(HGemPtr->SReceiveData);

    if(CUSTOMER_CODE==CC_SJ_Semiconductor_OS)                                   //Steven 20230213 : For SJSemi OS Tester
    {
        if(HasICUnderMachine())                                                 //kevin 20181127 add
        {
            HGemPtr->LocalAcknowledge(2, 16, 2);
            return ;
        }
    }
    else
    {
        if(HasICUnderMachine() || HasAnyICInMachine())                          //kevin 20181127 add
        {
            HGemPtr->LocalAcknowledge(2, 16, 2);
            return ;
        }
    }

    ret=S2F15_CheckNewEquipmentConstant();                                      //wei 20170417 (Steven) add S2F15
    if(ret!=0)
    {
        if(ret==-1)
        {
            S9F7_IllegalData("S2,F15 data format error");
            return ;
        }
        else
        {
            HGemPtr->LocalAcknowledge(2, 16, ret);
            return ;
        }
    }
    HGemPtr->SReceiveData->Assign(HGemPtr->SReceiveDataBackup);

    S2F15_UpdateNewEquipmentConstant();
    HGemPtr->LocalAcknowledge(2, 16, 0);
    if(HGemPtr->MoveCheckCallBack!=NULL)
        HGemPtr->MoveCheckCallBack();
}
// ---------------------------------------------------------------------------
// V 1.0
// Actual time data
// ---------------------------------------------------------------------------
void HTGem::S2F18_DateandTimeData()
{
    if(HGemPtr->CheckSFFormatOnlyHead("S2,F17 Format error !!!")==false)
        return;
    HGemPtr->GetTimeInfo();
    HGemPtr->InitLocalHead(2, 18, 0);
    HGemPtr->DataItemOut(HType.ASCII_TYPE, HGemPtr->GemClock);
    HGemPtr->SendLocalData();
}
// ---------------------------------------------------------------------------
// V 1.0
// S2,F24 Trace Initialize Acknowledge (TIA)
// ---------------------------------------------------------------------------
// L,5
//     1. <TRID>                // Trace request ID                          ASCII1 ,INT_8,UINT_8
//     2. <DSPER>               // Data sample period                        ASCII1
//     3. <TOTSMP>              // Total samples to be made                  ASCII1 ,INT_8,UINT_8
//     4. <REPGSZ>              // Reporting group size                      ASCII1 ,INT_8,UINT_8
//     5. L,n
//            1. <SVID1>                                                     ASCII1 ,INT_8,UINT_8
//            .
//            .
//            n. <SVIDn>
// ---------------------------------------------------------------------------
int HTGem::S2F24_TraceInitializeAcknowledgeSub()
{
    int len,ret;
    unsigned int i;
    unsigned char Type;
    char str[256],dsper[64];
    unsigned hh, mm, ss, cc=0;
    int iIndex;
    bool bSVIDError=false;
    AnsiString ID,S,SVID;

    if(HGemPtr->DataItemIn(5,HType.LIST_TYPE,NULL)==1)
    {
        HGemPtr->GetDataItemLenAndType(len,Type);
        if(HGemPtr->DataItemIn(len,Type,ID)!=1)                                        // ID=TRID
            return -1;
        iIndex=-1;
        for(i=0; i<10; i++)
        {
            if(HGemPtr->bTraceData[i]==true && HGemPtr->iTRID[i]==ID)
            {
                iIndex=i;
                break;
            }
        }
        if(iIndex==-1)                                                        // new TRACE
        {
            for(i=0; i<10; i++)
            {
                if(HGemPtr->bTraceData[i]==false)
                {
                    iIndex=i;
                    break;
                }
            }
        }
        if(iIndex==-1)                                                         // No more trace can use
            return 2;
        HGemPtr->iTRID[iIndex]=ID;

        ret=HGemPtr->GetDataItemLenAndType(len,Type);
        if(ret!=1 || Type!=HType.ASCII_TYPE || (len!=6 && len!=8))
            return -1;

        if(HGemPtr->DataItemIn(len,HType.ASCII_TYPE,dsper)!=1)
            return -1;
        // Format 1: hhmmss, 6 bytes
        // Format 2: hhmmsscc, 8 bytes

        str[0]=dsper[0];
        str[1]=dsper[1];
        str[2]=0;
        hh=atoi(str);

        str[0]=dsper[2];
        str[1]=dsper[3];
        str[2]=0;
        mm=atoi(str);

        str[0]=dsper[4];
        str[1]=dsper[5];
        str[2]=0;
        ss=atoi(str);
        if(len==8)
        {
            str[0]=dsper[6];
            str[1]=dsper[7];
            str[2]=0;
            cc=atoi(str);
        }

        HGemPtr->DSPER[iIndex]=hh*60*60*1000+mm*60*1000+ss*1000+cc;             // 以 0.1 sec 為單位
        HGemPtr->TraceDataResponseTask[iIndex]=1;                               //2013/07/18 lee

        if(HGemPtr->GetDataItemLenAndType(len, Type)!=1)
            return -1;
        if(len!=1)
            return 1;
        if(HGemPtr->DataItemIn(1, Type, S)!=1)
            return -1;
        HGemPtr->iTOTSMP[iIndex]=atoi(S.c_str());

        if(HGemPtr->GetDataItemLenAndType(len, Type)!=1)
            return -1;
        if(len!=1)
            return -1;
        if(HGemPtr->DataItemIn(1,Type,S)!=1)
            return -1;
        HGemPtr->iREPGSZ[iIndex]=atoi(S.c_str());

        HGemPtr->TraceData[iIndex]->Clear();

        if(HGemPtr->GetDataItemLenAndType(len, Type)!=1)
            return -1;
        if(Type==HType.LIST_TYPE)
        {
            if(HGemPtr->DataItemIn(len, HType.LIST_TYPE, NULL)==1)
            {
                HGemPtr->iREPGSZ[iIndex]=len;
                for(i=0; i<HGemPtr->iREPGSZ[iIndex];i++)
                {
                    if(HGemPtr->GetDataItemLenAndType(len, Type)==1)
                    {
                        if(len!=1)
                            return -1;
                        if(HGemPtr->DataItemIn(1, Type, SVID)!=1)
                            return -1;
                        if(HGemPtr->IsValidSVID(SVID)==false)                   // 無效的 SVID
                        {
                            S="SVID:"+AnsiString(SVID)+" not exist!!!";
                            HGemPtr->StringOut(S);
                            bSVIDError=true;
                        }
                        else
                        {
                            HGemPtr->TraceData[iIndex]->Add(SVID);
                        }
                    }
                }
            }
            else
            {
                HGemPtr->iREPGSZ[iIndex]=0;
            }
        }
        else if(Type==HType.UINT_4_TYPE)
        {
            unsigned *P;
            P=new unsigned [len];
            if(HGemPtr->DataItemIn(len, Type, P)==1)
            {
                HGemPtr->iREPGSZ[iIndex]=len;
                for(i=0; i<HGemPtr->iREPGSZ[iIndex]; i++)
                {
                    SVID=P[i];
                    if(HGemPtr->IsValidSVID(SVID)==false)                       // 無效的 SVID
                    {
                        S="SVID:"+AnsiString(SVID)+" not exist!!!";
                        HGemPtr->StringOut(S);
                        bSVIDError=true;
                    }
                    else
                    {
                        HGemPtr->TraceData[iIndex]->Add(SVID);
                    }
                }
            }
            else
            {
                delete[] P;
                return -1;
            }
        }
        else
        {
            return -1;
        }
        if(bSVIDError==true)
            return 4;
        if(HGemPtr->iREPGSZ[iIndex]!=0)
        {
            HGemPtr->iTOTSMP[iIndex]/=HGemPtr->iREPGSZ[iIndex];
            HGemPtr->bTraceData[iIndex]=true;
        }
        else
        {
            HGemPtr->bTraceData[iIndex]=false;
        }

        HGemPtr->iTOTSMP_Count[iIndex]=1;
        HGemPtr->TraceDataResponseTask[iIndex]=1;
    }
    else
    {
        return -1;
    }
    return 0;
}
//------------------------------------------------------------------------------
void HTGem::S2F24_TraceInitializeAcknowledge()
{
    int ret;
    ret=S2F24_TraceInitializeAcknowledgeSub();
    if(ret==-1)
        S9F7_IllegalData("S2,F23 Format error !!!");
    else
        HGemPtr->LocalAcknowledge(2,24,ret);
}
// ---------------------------------------------------------------------------
// V 1.0
// S2,F26 A diagnostic message for checkout of protocol and communication circuits.
//        The binary string sent is echoed back.
// ---------------------------------------------------------------------------
void HTGem::S2F26_DiagnosticLoopbackData()
{
    int len, ret;
    unsigned char Type;
    unsigned char *Temp;
    ret=HGemPtr->GetDataItemLenAndType(len,Type);
    if(Type==HType.BINARY_TYPE && ret==1)
    {
        Temp=new char [len+100];
        if(HGemPtr->DataItemIn(len, Type, Temp)==1)
        {
            HGemPtr->InitLocalHead(2, 26, 0);
            HGemPtr->DataItemOut(len, HType.BINARY_TYPE, Temp);
            HGemPtr->SendLocalData();
        }
        else
        {
            S9F7_IllegalData("S2,F25 Format error !!!");
        }
        delete[] Temp;
        Temp=NULL;                                                              //kevin 20180517
    }
    else
    {
        S9F7_IllegalData("S2,F25 Format error !!!");
    }
}
// ---------------------------------------------------------------------------
// V 1.0
// S2, F30 EquipmentConstantNamelistReply
// ---------------------------------------------------------------------------
void HTGem::S2F30_EquipmentConstantNamelistReply()
{
    int EClen, i, len;//,ret;
    unsigned char Type;
    AnsiString ECID;

    if(HGemPtr->GetDataItemLenAndTypeAndDelete(EClen, Type)==1)
    {
        if(Type==HType.LIST_TYPE)
        {
            HGemPtr->InitLocalHead(2, 30, 0);
            if(EClen==0)
            {
                len=HGemPtr->EC_ID->Count;
                HGemPtr->DataItemOut(len, HType.LIST_TYPE, NULL);
                for(i=0; i<len; i++)
                    HGemPtr->DataItemOutECNameList(HGemPtr->EC_ID->Strings[i]);
            }
            else
            {
                HGemPtr->DataItemOut(EClen, HType.LIST_TYPE, NULL);
                for(i=0; i<EClen; i++)
                {
                    if(HGemPtr->GetDataItemLenAndType(len, Type)==1)
                    {
                        if(HGemPtr->DataItemIn(len, Type, ECID)==1)
                        {
                            HGemPtr->DataItemOutECNameList(ECID);
                        }
                        else
                        {
                            S9F7_IllegalData("S2,F29 Format error !!!");
                            return;
                        }
                    }
                    else
                    {
                        S9F7_IllegalData("S2,F29 Format error !!!");
                        return;
                    }
                }
            }
            HGemPtr->SendLocalData();
        }
        else
        {
            S9F7_IllegalData("S2,F29 Format error !!!");
        }
    }
    else
    {
        S9F7_IllegalData("S2,F29 Format error !!!");
    }
}
// ---------------------------------------------------------------------------
// V 1.0
// S2,F32 Acknowledge the receipt of time and date.
// ---------------------------------------------------------------------------
void HTGem::S2F32_DateAndTimeAcknowledge()
{
    char str[256];
    int len,ret;
    unsigned char Type;
    bool Error=false;
    AnsiString S, S1;
//    unsigned YY,MM,DD,hh,mm,ss,cc;
    struct  time t;
    struct date reset;

    ret=HGemPtr->GetDataItemLenAndType(len, Type);
    if(Type==HType.ASCII_TYPE && ret==1)
    {
        if(HGemPtr->DataItemIn(len, Type, str)==1)
        {
            if(len==12)                                                         // "030601134700"
            {
                S=str;
                reset.da_year   =atoi(S.SubString(1, 2).c_str())+2000;
                reset.da_mon    =atoi(S.SubString(3, 2).c_str());
                reset.da_day    =atoi(S.SubString(5, 2).c_str());
                t.ti_hour       =atoi(S.SubString(7, 2).c_str());
                t.ti_min        =atoi(S.SubString(9, 2).c_str());
                t.ti_sec        =atoi(S.SubString(11, 2).c_str());
                t.ti_hund       =0;
            }
            else if(len==14)                                                    // "20030602134700"
            {
                S=str;
                reset.da_year   =atoi(S.SubString(1, 4).c_str());
                reset.da_mon    =atoi(S.SubString(5, 2).c_str());
                reset.da_day    =atoi(S.SubString(7, 2).c_str());
                t.ti_hour       =atoi(S.SubString(9, 2).c_str());
                t.ti_min        =atoi(S.SubString(11, 2).c_str());
                t.ti_sec        =atoi(S.SubString(13, 2).c_str());
                t.ti_hund       =0;
            }
            else if(len==16)                                                    // "2003060313401000"
            {
                S=str;
                reset.da_year   =atoi(S.SubString(1, 4).c_str());
                reset.da_mon    =atoi(S.SubString(5, 2).c_str());
                reset.da_day    =atoi(S.SubString(7, 2).c_str());
                t.ti_hour       =atoi(S.SubString(9, 2).c_str());
                t.ti_min        =atoi(S.SubString(11, 2).c_str());
                t.ti_sec        =atoi(S.SubString(13, 2).c_str());
                t.ti_hund       =atoi(S.SubString(15, 2).c_str());
            }
            else if(len==19)                                                    //  "2003-06-04T13:01:01"
            {
                S=str;
                reset.da_year   =atoi(S.SubString(1, 4).c_str());
                reset.da_mon    =atoi(S.SubString(6, 2).c_str());
                reset.da_day    =atoi(S.SubString(9, 2).c_str());
                t.ti_hour       =atoi(S.SubString(12, 2).c_str());
                t.ti_min        =atoi(S.SubString(15, 2).c_str());
                t.ti_sec        =atoi(S.SubString(18, 2).c_str());
                t.ti_hund       =0;
            }
            else if(len==21)                                                    //  "2003-06-05T13:01:01.2"
            {
                S=str;
                reset.da_year   =atoi(S.SubString(1, 4).c_str());
                reset.da_mon    =atoi(S.SubString(6, 2).c_str());
                reset.da_day    =atoi(S.SubString(9, 2).c_str());
                t.ti_hour       =atoi(S.SubString(12, 2).c_str());
                t.ti_min        =atoi(S.SubString(15, 2).c_str());
                t.ti_sec        =atoi(S.SubString(18, 2).c_str());
                t.ti_hund       =atoi(S.SubString(21, 1).c_str())*10;
            }
            else if(len==22)                                                    //  "2003-06-06T13:01:01.25"
            {
                S=str;
                reset.da_year   =atoi(S.SubString(1, 4).c_str());
                reset.da_mon    =atoi(S.SubString(6, 2).c_str());
                reset.da_day    =atoi(S.SubString(9, 2).c_str());
                t.ti_hour       =atoi(S.SubString(12, 2).c_str());
                t.ti_min        =atoi(S.SubString(15, 2).c_str());
                t.ti_sec        =atoi(S.SubString(18, 2).c_str());
                t.ti_hund       =atoi(S.SubString(21, 2).c_str());
            }
            else
            {
                Error=true;
            }

            if(Error==false)
            {
                if(IsCorrectDateFormat(reset.da_year, reset.da_mon, reset.da_day))
                {
                    if(t.ti_hour>23)                                            //Steven 20140404 : 避免Warning
                        Error=true;
                    if(t.ti_min>59)
                        Error=true;
                    if(t.ti_sec>59)
                        Error=true;
                    if(t.ti_hund>99)
                        Error=true;
                    if(Error==false)
                    {
                        settime(&t);
                        setdate(&reset);
                    }
                }
            }
        }
        else
        {
            Error=true;
        }
    }
    else
    {
        Error=true;
    }

    if(Error==true)
        HGemPtr->LocalAcknowledge(2, 32, 1);
    else
        HGemPtr->LocalAcknowledge(2, 32, 0);
}
// ---------------------------------------------------------------------------
int HTGem::S2F34_DefineReportAcknowledgeSub()
{
    int len, svlen, ret, temp;
    unsigned char Type;
    AnsiString DATAID, ReportID, sSVID;
    TStringList *P;

    for(int i=0; i<HGemPtr->slTempReportID->Count; i++)                         // 若上一次的資料尚未清除則要清除
    {
        P=(TStringList *) HGemPtr->lTempReportIDContent->Items[i];
        P->Clear();                                                             //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete P;
    }
    HGemPtr->slTempReportID->Clear();
    HGemPtr->lTempReportIDContent->Clear();

    if(HGemPtr->DataItemIn(2, HType.LIST_TYPE, NULL)!=1)
        return -1;

    HGemPtr->GetDataItemLenAndType(len, Type);
    if(HGemPtr->DataItemIn(len, Type, DATAID)!=1)
        return -1;

    if(CUSTOMER_CODE==CC_ONSEMI_M)
    {

    }
    else
    {
        if(DATAID!=1)                                                           //20130912
            return -1;
    }

    ret=HGemPtr->GetDataItemLenAndType(len, Type);                              // 取得多少 report id 要被定義
    if(ret!=1 || Type!=HType.LIST_TYPE)
        return -1;
    HGemPtr->DataItemIn(len, HType.LIST_TYPE, NULL);
    if(len==0)
    {
        HGemPtr->DeleteAllHostDefineReportID();
        return 1;
    }

    for(int i=0; i<len; i++)
    {
        if(HGemPtr->DataItemIn(2, HType.LIST_TYPE,NULL)!=1)
            return -1;
        if(HGemPtr->GetDataItemLenAndType(temp, Type)!=1)
            return -1;
        if(HGemPtr->DataItemIn(temp, Type, ReportID)!=1)
            return -1;
        ret=HGemPtr->GetDataItemLenAndTypeAndDelete(svlen, Type);
        if(ret!=1 || Type!=HType.LIST_TYPE)
            return -1;

        P=new TStringList;
        P->Clear();
        for(int j=0; j<svlen; j++)
        {
            HGemPtr->GetDataItemLenAndType(temp, Type);
            if(HGemPtr->DataItemIn(temp, Type, sSVID)==1)
            {
                P->Add(sSVID);
            }
            else
            {
                P->Clear();                                                     //Ifor 20170603 (wei) TStringList 刪除前先 Clean
                delete P;
                return -1;
            }
        }
        HGemPtr->slTempReportID->Add(ReportID);
        HGemPtr->lTempReportIDContent->Add(P);
    }
    return 1;
}
// ---------------------------------------------------------------------------
void HTGem::S2F34_ProcessHostSendReportID()
{
    AnsiString ReportID, SVID;
    TStringList *strPtr;

    // 檢查是否已經定義了
    for(int i=0; i<HGemPtr->slTempReportID->Count; i++)
    {
        ReportID=HGemPtr->slTempReportID->Strings[i];
        strPtr  =(TStringList *)HGemPtr->lTempReportIDContent->Items[i];
        if(HGemPtr->CheckReportAlreadyDefine(ReportID) && strPtr->Count!=0)
        {
            HGemPtr->DefineReportAcknowledgeAlreadyDefined();
            return;
        }
    }
    // 檢查 SVID 是否有不存在的
    for(int i=0; i<HGemPtr->slTempReportID->Count; i++)
    {
        strPtr=(TStringList *)HGemPtr->lTempReportIDContent->Items[i];
        for(int j=0; j<strPtr->Count; j++)
        {
            SVID=strPtr->Strings[j];
            if(HGemPtr->IsValidSVID(SVID)==false)
            {
                HGemPtr->DefineReportAcknowledgeInvalidSVID();
                return;
            }
        }
    }
    unsigned SVIDBuffer[1024];
    for(int i=0; i<HGemPtr->slTempReportID->Count; i++)
    {
        ReportID=HGemPtr->slTempReportID->Strings[i];
        strPtr=(TStringList *)HGemPtr->lTempReportIDContent->Items[i];
        if(strPtr->Count==0)
        {
            HGemPtr->DeleteReportID(atoi(ReportID.c_str()), 0);                 //  把 report id 表內地 report id 全部 delete  2013/11/20
            HGemPtr->DeleteReportIDOfCeid(atoi(ReportID.c_str()));              // 把 ceid   id 表內地 report id 全部 delete
        }
        else
        {
            if(strPtr->Count>1024)
            {
                HGemPtr->DefineReportAcknowledgeInsufficientSpace();
                return;
            }

            for(int j=0; j<strPtr->Count; j++)
            {
                SVID=strPtr->Strings[j];
                SVIDBuffer[j]=atoi(SVID.c_str());
            }

            if(HGemPtr->SetReportIDContent(atoi(ReportID.c_str()), strPtr->Count, SVIDBuffer, 0)==false)
            {
                HGemPtr->DefineReportAcknowledgeInsufficientSpace();
                return;
            }
        }
        strPtr->Clear();                                                        //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete strPtr;
    }
    HGemPtr->slTempReportID->Clear();
    HGemPtr->lTempReportIDContent->Clear();
    HGemPtr->DefineReportAcknowledgeAccept();
    HGemPtr->SaveEventReportData();
}
// ---------------------------------------------------------------------------
void HTGem::S2F34_DefineReportAcknowledge()
{
    int ret;
    ret=S2F34_DefineReportAcknowledgeSub();
    if(ret==-1)
        HGemPtr->DefineReportAcknowledgeFormatError();
    else
        S2F34_ProcessHostSendReportID();
}
// ---------------------------------------------------------------------------
/*
L,2
    1. <DATAID>
    2. L,a # events
      1. L,2 event 1
          1. <CEID1>
          2. L,b
               1. <RPTID1>
               .
               .
               b. <RPTIDb>
               .
               .
          a. L,2 event a
          1. <CEIDa> # RPTIDS this event
          2.L,c
              1.<RPTID1>
              .
              .
              c. <RPTIDc>
*/
// ---------------------------------------------------------------------------
int HTGem::S2F36_LinkEventReportAcknowledgeSub()
{
    int len, svlen, ret, temp;
    unsigned char Type;
    AnsiString DATAID,Ceid,sReportID;
    TStringList *P;

    for(int i=0; i<HGemPtr->slTempCeID->Count; i++)                             // 若上一次遞資料尚未清除記憶體則要清除
    {
        P=(TStringList *) HGemPtr->lTempCeIDContent->Items[i];
        P->Clear();                                                             //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete P;
    }
    HGemPtr->slTempCeID->Clear();
    HGemPtr->lTempCeIDContent->Clear();

    if(HGemPtr->DataItemIn(2, HType.LIST_TYPE, NULL)!=1)
        return -1;

    HGemPtr->GetDataItemLenAndType(len, Type);
    if(HGemPtr->DataItemIn(len, Type, DATAID)!=1)
        return -1;
    ret=HGemPtr->GetDataItemLenAndType(len, Type);                              // 取得多少 report id 要被定義
    if(ret!=1 || Type!=HType.LIST_TYPE)
        return -1;

    HGemPtr->DataItemIn(len, HType.LIST_TYPE, NULL);
    if(len==0)
    {
        HGemPtr->DeleteAllHostDefineCeid();
        return 1;
    }

    for(int i=0; i<len; i++)
    {
        if(HGemPtr->DataItemIn(2, HType.LIST_TYPE, NULL)!=1)
            return -1;

        HGemPtr->GetDataItemLenAndType(temp, Type);
        if(HGemPtr->DataItemIn(temp, Type, Ceid)!=1)
            return -1;

        ret=HGemPtr->GetDataItemLenAndTypeAndDelete(svlen, Type);
        if(ret!=1 || Type!=HType.LIST_TYPE)
            return -1;

        P=new TStringList;
        P->Clear();
        for(int j=0; j<svlen; j++)
        {
            HGemPtr->GetDataItemLenAndType(temp, Type);
            if(HGemPtr->DataItemIn(temp, Type, sReportID)!=1)
            {
                P->Clear();                                                     //Ifor 20170603 (wei) TStringList 刪除前先 Clean
                delete P;
                return -1;
            }
            P->Add(sReportID);
        }
        HGemPtr->slTempCeID->Add(Ceid);
        HGemPtr->lTempCeIDContent->Add(P);
    }
    return 1;
}
// ---------------------------------------------------------------------------
void HTGem::S2F36_LinkEventReportAcknowledge()
{
    int ret;
    ret=S2F36_LinkEventReportAcknowledgeSub();
    if(ret==-1)
        HGemPtr->LinkReportAcknowledgeFormatError();
    else
        HGemPtr->ProcessHostSendReportLinkID();
}
// ---------------------------------------------------------------------------
/*
L,2
1. <CEED> ChangeToFloatNonPcnt((double)(enable), (double)(disable))
2. L,n #CEIDs
    1. <CEID1>
      .
      .
    n. <CEIDn>
*/
// ---------------------------------------------------------------------------
void HTGem::S2F38_EnableDisableEventReportAcknowledge()
{
    int len, slen;
    unsigned char Type;
    bool CEED;
    unsigned CEID[1024];
    AnsiString sID;

    if(HGemPtr->DataItemIn(2, HType.LIST_TYPE,NULL)==1)
    {
        if(HGemPtr->DataItemIn(1, HType.BOOLEAN_TYPE, &CEED))
        {
            if(HGemPtr->GetDataItemLenAndTypeAndDelete(slen, Type)==1)
            {
                if(Type==HType.LIST_TYPE)
                {
                    for(int i=0; i<slen; i++)
                    {
                        HGemPtr->GetDataItemLenAndType(len, Type);
                        if(HGemPtr->DataItemIn(len, Type, sID)==1)
                        {
                            CEID[i]=atoi(sID.c_str());
                            if(HGemPtr->CheckCEIDExist(sID)==false)
                            {
                                HGemPtr->EnableDisableEventReportAcknowledgeCeidNotExist();
                                return;
                            }
                        }
                        else
                        {
                            HGemPtr->EnableDisableEventReportAcknowledgeFormatError();
                            return;
                        }
                    }
                    HGemPtr->EnableDisableEventReport(CEED, slen, CEID);
                    return;
                }
            }
        }
    }
    HGemPtr->EnableDisableEventReportAcknowledgeFormatError();
}
// ---------------------------------------------------------------------------
int HTGem::S2F42_Host_Command_Acknowledge()
{
    unsigned char HCACK=1;
    char CommandStr[1024];
    AnsiString PPID;
    int ret,len;
    unsigned char Type;
    AnsiString S, S1;

    if(HGemPtr->DataItemIn(2, HType.LIST_TYPE, NULL)==1)                        // 需要補充多重 Command
    {
        HGemPtr->GetDataItemLenAndType(len, Type);
        if(Type==HType.ASCII_TYPE)
        {
            ret=HGemPtr->DataItemIn(len, HType.ASCII_TYPE, CommandStr);
            if(ret==-1)
                S="";
        }
        else
        {
            S="";
        }
        S=CommandStr;
        S=S.UpperCase();
        HCACK=1;
        HGemPtr->InitLocalHead(2, 42, 0);

        if(HCACK==0)
        {
            HGemPtr->DataItemOut(2, HType.LIST_TYPE, NULL);
            HGemPtr->DataItemOut(1, HType.BINARY_TYPE, &HCACK);
            HGemPtr->DataItemOut(0, HType.LIST_TYPE, NULL);
        }
        else
        {
            HGemPtr->DataItemOut(2, HType.LIST_TYPE, NULL);
            HGemPtr->DataItemOut(1, HType.BINARY_TYPE, &HCACK);
            HGemPtr->DataItemOut(0, HType.LIST_TYPE, NULL);
        }

        HGemPtr->SendLocalData();
        return 1;
    }
    else
    {
        HCACK=3;
        HGemPtr->DataItemOut(2, HType.LIST_TYPE, NULL);
        HGemPtr->DataItemOut(1, HType.BINARY_TYPE, &HCACK);
        HGemPtr->DataItemOut(0, HType.LIST_TYPE, NULL);
    }
    return HCACK;
}
// ---------------------------------------------------------------------------
void HTGem::S2F44_ResetSpoolingAcknowledge()
{
    unsigned char C=0;
    HGemPtr->InitLocalHead(2, 44,0);
    HGemPtr->DataItemOut( 2,HType.LIST_TYPE, NULL);
    HGemPtr->DataItemOut( 1,HType.BINARY_TYPE,&C);
    HGemPtr->DataItemOut( 0,HType.LIST_TYPE, NULL);
    HGemPtr->SendLocalData();
}
// ---------------------------------------------------------------------------
// V 1.0
// S5,F4
// ---------------------------------------------------------------------------
//  L,2
//     1. <ALED>    bit 8=1 enable  bit 8=0 disable
//     2. <ALID>    若  length=0 表示 all
// ---------------------------------------------------------------------------
void HTGem::S5F4_EnableDisableAlarmAcknowledge()
{
    unsigned char ALED, Type;
    int len;
    AnsiString ID;

    if(HGemPtr->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
    {
        if(HGemPtr->DataItemIn(1, HType.BINARY_TYPE, &ALED))
        {
            HGemPtr->GetDataItemLenAndType(len, Type);
            if(len==0)
            {
                HGemPtr->EnableDisableAlarmAll(ALED);
                HGemPtr->LocalAcknowledge(5, 4, 0);
            }
            else
            {
                HGemPtr->DataItemIn(len, Type, ID);
                if(HGemPtr->EnableDisableAlarm(ID, ALED))
                    HGemPtr->LocalAcknowledge(5, 4, 0);
                else
                    HGemPtr->LocalAcknowledge(5, 4, 1);
            }
            return;
        }
    }
    S9F7_IllegalData("S5,F3 Data Format error !!!");
}
// ---------------------------------------------------------------------------
// V 1.0
// S5,F6 This message contains the alarm data known to the equipment. There are “m” alarms in the list.
// ---------------------------------------------------------------------------
void HTGem::S5F6_ListAlarmData()
{
    int SVlen,i, j,ret;
    unsigned char Type;

    __int64  int8SV;                                                            //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
    int   int4SV;
    short int2SV;
    unsigned __int64  uint8SV;                                                  //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
    unsigned int   uint4SV;
    unsigned short uint2SV;
    AnsiString str;
    AnsiString S;

    if(HGemPtr->GetDataItemLenAndType(SVlen,Type)==1)
    {
        if(SVlen!=0)
        {
            if(Type==HType.UINT_8_TYPE || Type==HType.UINT_4_TYPE || Type==HType.UINT_2_TYPE ||
               Type==HType.INT_8_TYPE  || Type==HType.INT_4_TYPE  || Type==HType.INT_2_TYPE)  //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
            {
                unsigned __int64 *uint8Ptr;
                unsigned *uint4Ptr;
                unsigned short *uint2Ptr;
                __int64     *int8Ptr;
                int      *int4Ptr;
                short    *int2Ptr;
                unsigned char Mode=0x80;

                ret=HGemPtr->GetDataItemLenAndType(SVlen,Type);
                if(ret!=1)
                {
                    S9F7_IllegalData("S5,F5 Data Format error !!!");
                    return;
                }

                if(Type==HType.UINT_8_TYPE)                                     //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                {
                    uint8Ptr=new unsigned __int64 [SVlen];
                    ret=HGemPtr->DataItemIn(SVlen, HType.UINT_8_TYPE,uint8Ptr);
                }
                else if(Type==HType.UINT_4_TYPE)
                {
                    uint4Ptr=new unsigned [SVlen];
                    ret=HGemPtr->DataItemIn(SVlen, HType.UINT_4_TYPE,uint4Ptr);
                }
                else if(Type==HType.UINT_2_TYPE)
                {
                    uint2Ptr=new unsigned short [SVlen];
                    ret=HGemPtr->DataItemIn(SVlen, HType.UINT_2_TYPE,uint2Ptr);
                }
                else if(Type==HType.INT_8_TYPE)                                 //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                {
                    int8Ptr=new __int64[SVlen];
                    ret=HGemPtr->DataItemIn(SVlen, HType.INT_8_TYPE,int8Ptr);
                }
                else if(Type==HType.INT_4_TYPE)
                {
                    int4Ptr=new int[SVlen];
                    ret=HGemPtr->DataItemIn(SVlen, HType.INT_4_TYPE,int4Ptr);
                }
                else if(Type==HType.INT_2_TYPE)
                {
                    int2Ptr=new short[SVlen];
                    ret=HGemPtr->DataItemIn(SVlen, HType.INT_2_TYPE,int2Ptr);
                }

                if(ret==1)
                {
                    HGemPtr->InitLocalHead(5,6,0);
                    HGemPtr->DataItemOut(SVlen, HType.LIST_TYPE, NULL);
                    for(i=0; i<SVlen; i++)
                    {
                        if(Type==HType.UINT_8_TYPE)                             //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                            S=uint8Ptr[i];
                        else if(Type==HType.UINT_4_TYPE)
                            S=uint4Ptr[i];
                        else if(Type==HType.UINT_2_TYPE)
                            S=uint2Ptr[i];
                        else if(Type==HType.INT_8_TYPE)                         //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                            S=int8Ptr[i];
                        else if(Type==HType.INT_4_TYPE)
                            S=int4Ptr[i];
                        else if(Type==HType.INT_2_TYPE)
                            S=int2Ptr[i];

                        j=HGemPtr->GetAlarmIndex(S);
                        if(j>=0)
                        {
                            HGemPtr->DataItemOut(3, HType.LIST_TYPE, NULL);
                            Mode=atoi(HGemPtr->strGrdAlarm->Cells[2][j].c_str())+0x80;
                            HGemPtr->DataItemOut(1, HType.BINARY_TYPE, &Mode);  // mode
                            if(Type==HType.UINT_8_TYPE)                         //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                            {
                                uint8SV=(unsigned)_atoi64(HGemPtr->strGrdAlarm->Cells[1][j].c_str());
                                HGemPtr->DataItemOut(1, HType.UINT_8_TYPE, &uint8SV);
                            }
                            else if(Type==HType.UINT_4_TYPE)
                            {
                                uint4SV=atoi(HGemPtr->strGrdAlarm->Cells[1][j].c_str());
                                HGemPtr->DataItemOut(1, HType.UINT_4_TYPE, &uint4SV);
                            }
                            else if(Type==HType.UINT_2_TYPE)
                            {
                                uint2SV=atoi(HGemPtr->strGrdAlarm->Cells[1][j].c_str());
                                HGemPtr->DataItemOut(1, HType.UINT_2_TYPE, &uint2SV);
                            }
                            else if(Type==HType.INT_8_TYPE)                     //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                            {
                                int8SV=_atoi64(HGemPtr->strGrdAlarm->Cells[1][j].c_str());
                                HGemPtr->DataItemOut(1, HType.INT_8_TYPE, &int8SV);
                            }
                            else if(Type==HType.INT_4_TYPE)
                            {
                                int4SV=atoi(HGemPtr->strGrdAlarm->Cells[1][j].c_str());
                                HGemPtr->DataItemOut(1, HType.INT_4_TYPE, &int4SV);
                            }
                            else if(Type==HType.INT_2_TYPE)
                            {
                                int2SV=atoi(HGemPtr->strGrdAlarm->Cells[1][j].c_str());
                                HGemPtr->DataItemOut(1, HType.INT_2_TYPE, &int2SV);
                            }

                            HGemPtr->DataItemOut(HType.ASCII_TYPE, HGemPtr->strGrdAlarm->Cells[4][j]);  // message
                        }
                        else
                        {
                            HGemPtr->DataItemOut(3, HType.LIST_TYPE, NULL);
                            HGemPtr->DataItemOut(0, HType.BINARY_TYPE,&Mode);   // mode
                            if(Type==HType.UINT_8_TYPE)                         //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                            {
                                uint8SV=uint8Ptr[i];
                                HGemPtr->DataItemOut(1, HType.UINT_8_TYPE, &uint8SV);
                            }
                            else if(Type==HType.UINT_4_TYPE)
                            {
                                uint4SV=uint4Ptr[i];
                                HGemPtr->DataItemOut(1, HType.UINT_4_TYPE, &uint4SV);
                            }
                            else if(Type==HType.UINT_2_TYPE)
                            {
                                uint2SV=uint2Ptr[i];
                                HGemPtr->DataItemOut(1, HType.UINT_2_TYPE, &uint2SV);
                            }
                            else if(Type==HType.INT_8_TYPE)                     //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                            {
                                int8SV=int8Ptr[i];
                                HGemPtr->DataItemOut(1, HType.INT_8_TYPE, &int8SV);
                            }
                            else if(Type==HType.INT_4_TYPE)
                            {
                                int4SV=int4Ptr[i];
                                HGemPtr->DataItemOut(1, HType.INT_4_TYPE, &int4SV);
                            }
                            else if(Type==HType.INT_2_TYPE)
                            {
                                int2SV=int2Ptr[i];
                                HGemPtr->DataItemOut(1, HType.INT_2_TYPE, &int2SV);
                            }

                            str="";
                            HGemPtr->DataItemOut(HType.ASCII_TYPE, str);
                        }
                    }
                    HGemPtr->SendLocalData();
                    if(Type==HType.UINT_8_TYPE)                                 //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                    {
                        delete[] uint8Ptr;                                      //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        uint8Ptr=NULL;                                          //kevin 20180517
                    }
                    else if(Type==HType.UINT_4_TYPE)
                    {
                        delete[] uint4Ptr;                                      //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        uint4Ptr=NULL;                                          //kevin 20180517
                    }
                    else if(Type==HType.UINT_2_TYPE)
                    {
                        delete[] uint2Ptr;                                      //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        uint2Ptr=NULL;                                          //kevin 20180517
                    }
                    else if(Type==HType.INT_8_TYPE)                             //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                    {
                        delete[] int8Ptr;                                       //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        int8Ptr=NULL;                                           //kevin 20180517
                    }
                    else if(Type==HType.INT_4_TYPE)
                    {
                        delete[] int4Ptr;                                       //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        int4Ptr=NULL;                                           //kevin 20180517
                    }
                    else if(Type==HType.INT_2_TYPE)
                    {
                        delete[] int2Ptr;                                       //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        int2Ptr=NULL;                                           //kevin 20180517
                    }
                    return;
                }
                else
                {
                    S9F7_IllegalData("S5,F5 Data Format error !!!");
                    if(Type==HType.UINT_8_TYPE)                                 //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                    {
                        delete[] uint8Ptr;                                      //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        uint8Ptr=NULL;                                          //kevin 20180517
                    }
                    else if(Type==HType.UINT_4_TYPE)
                    {
                        delete[] uint4Ptr;                                      //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        uint4Ptr=NULL;                                          //kevin 20180517
                    }
                    else if(Type==HType.UINT_2_TYPE)
                    {
                        delete[] uint2Ptr;                                      //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        uint2Ptr=NULL;                                          //kevin 20180517
                    }
                    else if(Type==HType.INT_8_TYPE)                             //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                    {
                        delete[] int8Ptr;                                       //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        int8Ptr=NULL;                                           //kevin 20180517
                    }
                    else if(Type==HType.INT_4_TYPE)
                    {
                        delete[] int4Ptr;                                       //Ifor 20170603 (wei) 修改陣列刪除方式 delete ==> delete[]
                        int4Ptr=NULL;                                           //kevin 20180517
                    }
                    else if(Type==HType.INT_2_TYPE)
                    {
                        delete[] int2Ptr;                                       //Ifor 20170603 修改陣列刪除方式 delete ==> delete[]
                        int2Ptr=NULL;                                           //kevin 20180517
                    }
                    return;
                }
            }
            else
            {
                S9F7_IllegalData("S5,F5 Data Format error !!!");
            }
        }
        else
        {
            HGemPtr->InitLocalHead(5, 6, 0);
            unsigned char Mode=0x80;
            HGemPtr->DataItemOut(HGemPtr->strGrdAlarm->RowCount-1, HType.LIST_TYPE, NULL);
            for(i=1; i<HGemPtr->strGrdAlarm->RowCount; i++)
            {
                HGemPtr->DataItemOut(3, HType.LIST_TYPE, NULL);
                Mode=atoi(HGemPtr->strGrdAlarm->Cells[2][i].c_str())+0x80;
                HGemPtr->DataItemOut(1, HType.BINARY_TYPE,&Mode);               // mode
                uint4SV=atoi(HGemPtr->strGrdAlarm->Cells[1][i].c_str());
                HGemPtr->DataItemOut(1, HType.UINT_4_TYPE, &uint4SV);           // ID
                HGemPtr->DataItemOut(HType.ASCII_TYPE, HGemPtr->strGrdAlarm->Cells[4][i]);
            }

            HGemPtr->SendLocalData();
            return;
        }
    }
    else
    {
        S9F7_IllegalData("S5,F5 Data Format error !!!");
    }
}
// ---------------------------------------------------------------------------
// V 1.0
// S5,F6 This message contains the alarm data known to the equipment. There are “m” alarms in the list.
// ---------------------------------------------------------------------------
void HTGem::S5F8_ListEnableAlarmAcknowledge()
{
    int SVlen, len;
    unsigned char Type;
    unsigned int   uint4SV;
    unsigned char ALT=0x80;
    if( HGemPtr->GetDataItemLenAndType(SVlen, Type)!=1)                         //pig 2014.08.01 SECS
    {
        S9F7_IllegalData("S5,F7 Data Format error !!!");
        return;
    }
    HGemPtr->InitLocalHead(5,8,0);
    len=0;
    for(int i=1; i<HGemPtr->strGrdAlarm->RowCount; i++)
        if(HGemPtr->strGrdAlarm->Cells[7][i]=="1")                              //wei 20180227 (Steven) S5F7 判斷位置錯誤 3-->7
            len++;

    HGemPtr->DataItemOut(len, HType.LIST_TYPE, NULL);

    for(int i=1; i<HGemPtr->strGrdAlarm->RowCount; i++)
    {
        if(HGemPtr->strGrdAlarm->Cells[7][i]=="1")                              //wei 20180227 (Steven) S5F7 判斷位置錯誤 3-->7
        {
            HGemPtr->DataItemOut(3, HType.LIST_TYPE, NULL);
            HGemPtr->DataItemOut(1, HType.BINARY_TYPE, &ALT);
            uint4SV=atoi(HGemPtr->strGrdAlarm->Cells[1][i].c_str());
            HGemPtr->DataItemOut(1, HType.UINT_4_TYPE, &uint4SV);
            HGemPtr->DataItemOut(HType.ASCII_TYPE, HGemPtr->strGrdAlarm->Cells[4][i]);
        }
    }
    HGemPtr->SendLocalData();
}
// ---------------------------------------------------------------------------
void HTGem::S6F16_EventReportData()
{
    int len, ret;
    unsigned char Type;
    AnsiString CEID;
    AnsiString S;
    unsigned iCeid,iDataID=1;

    ret=HGemPtr->GetDataItemLenAndType(len,Type);
    if(ret==1 && HGemPtr->DataItemIn(len, Type, CEID)==1)
    {
        iCeid=atoi(CEID.c_str());
        if(HGemPtr->CheckCEIDExist(CEID))
        {
            //if(IsEnableEvent(1,iCeid))                                        // 若 Host 要求 Equipment 送回某一個 CEID
            {                                                                   // 但此 CEID 並未被 enable 那麼依然回報嗎
                HGemPtr->InitLocalHead(6, 16, 0);                               // 目前先寫一定回報,若要依　enable 來決定
                HGemPtr->DataItemOut(3, HType.LIST_TYPE, NULL);                 // 則以下的　mark 打開就好
                HGemPtr->DataItemOut(1, HType.UINT_4_TYPE, &iDataID);
                HGemPtr->DataItemOut(1, HType.UINT_4_TYPE, &iCeid);
                HGemPtr->SendCeid(iCeid);
                HGemPtr->SendLocalData();
            }
        }
        else
        {
            HGemPtr->InitLocalHead(6, 16, 0);
            HGemPtr->DataItemOut(0, HType.UINT_4_TYPE, &iCeid);
            HGemPtr->SendLocalData();
        }
    }
    else
    {
        S9F7_IllegalData("S6,F15 Data Format error !!!");
    }
}
// ---------------------------------------------------------------------------
void HTGem::S6F18_AnnotatedEventReportData()
{
    int len, ret;
    unsigned char Type;
    AnsiString CEID;
    AnsiString S;
    unsigned iCeid,iDataID=1;

    ret=HGemPtr->GetDataItemLenAndType(len, Type);
    if(ret==1 && HGemPtr->DataItemIn(len, Type, CEID)==1)
    {
        iCeid=atoi(CEID.c_str());
        if(HGemPtr->CheckCEIDExist(CEID))
        {
            //if(IsEnableEvent(1,iCeid))                                        // 若 Host 要求 Equipment 送回某一個 CEID
            {                                                                   // 但此 CEID 並未被 enable 那麼依然回報嗎
                HGemPtr->InitLocalHead(6, 18, 0);                               // 目前先寫一定回報,若要依　enable 來決定
                HGemPtr->DataItemOut(3, HType.LIST_TYPE,NULL);                  // 則以下的　mark 打開就好
                HGemPtr->DataItemOut(1, HType.UINT_4_TYPE,&iDataID);
                HGemPtr->DataItemOut(1, HType.UINT_4_TYPE,&iCeid);
                HGemPtr->SendAnnotatedCeid(iCeid);
                HGemPtr->SendLocalData();
            }
        }
        else
        {
            HGemPtr->InitLocalHead(6, 16, 0);
            HGemPtr->DataItemOut(0, HType.UINT_4_TYPE, &iCeid);
            HGemPtr->SendLocalData();
        }
    }
    else
    {
        S9F7_IllegalData("S6,F17 Data Format error !!!");
    }
}
// ---------------------------------------------------------------------------
void HTGem::S6F20_IndividualReportData()
{
    int ret, x, y, ict, len;
    AnsiString RptID,SVID;
    unsigned char Type;

    ret=HGemPtr->GetDataItemLenAndType(len, Type);

    if(ret==1 && HGemPtr->DataItemIn(len, Type, RptID)==1)
    {
        for(y=1; y<HGemPtr->stdGridReportID->RowCount; y++)
        {
            if(HGemPtr->stdGridReportID->Cells[0][y]==RptID)
            {
                ict=0;
                for(x=2; x<HGemPtr->stdGridReportID->ColCount; x++)
                {
                    if(HGemPtr->stdGridReportID->Cells[x][y]!="")
                    {
                        SVID=HGemPtr->stdGridReportID->Cells[x][y];
                        if(HGemPtr->SV_ID->IndexOf(SVID)>0)
                        {
                            ict++;
                        }
                        else
                        {
                            S9F7_IllegalData("S6,F19 Invalid SVID in Report ");
                            return;
                        }

                    }
                }
                HGemPtr->InitLocalHead(6, 20, 0);
                HGemPtr->DataItemOut(ict, HType.LIST_TYPE, NULL);
                for(x=2; x<HGemPtr->stdGridReportID->ColCount; x++)
                {
                    if(HGemPtr->stdGridReportID->Cells[x][y]!="")
                    {
                        SVID=HGemPtr->stdGridReportID->Cells[x][y];
                        HGemPtr->DataItemOutSV(SVID);
                    }
                }
                HGemPtr->SendLocalData();
                return;
            }
        }
    }
    S9F7_IllegalData("S6,F19 Data Format error !!!");
}
// ---------------------------------------------------------------------------
// V 1.0
// S6,F24 The purpose of this message is to acknowledge the receipt of
//        the Request Spooled Data (S6,F23) and to respond with an
//        appropriate acknowledge code.
// ---------------------------------------------------------------------------
void HTGem::S6F24_RequestSpooledDataAcknowledgementSend()
{
    unsigned char RSDC;
    AnsiString S;
    if(HGemPtr->bSpoolActive==false)
    {
        HGemPtr->LocalAcknowledge(6, 24, 0);
        return;
    }
    HGemPtr->bBeginTransferSpool=true;
    if(HGemPtr->DataItemIn(1, HType.UINT_1_TYPE, &RSDC)==1)
    {
        if(RSDC==1)
        {
            S=AnsiString("del ")+IncludeTrailingPathDelimiter(HGemPtr->GemSpoolPath)+AnsiString("*.*/q/f");
            system(S.c_str());
        }
        HGemPtr->LocalAcknowledge(6, 24, 0);
    }
    else
    {
        HGemPtr->LocalAcknowledge(6, 24, 0);
    }
}
// ---------------------------------------------------------------------------
// V 1.0
// This message gives permission for the process program to be loaded.
// ---------------------------------------------------------------------------
int HTGem::S7F2_ProcessProgramLoadGrant()
{
// L,2
//    1. <PPID>          // ascii
//    2. <LENGTH>        // int ,uint
    int ret,len;
    unsigned char Type;
    AnsiString PPID;
    if(HGemPtr->DataItemIn(2, HType.LIST_TYPE, NULL)==1)
    {
        ret=HGemPtr->GetDataItemLenAndType(len, Type);
        if(ret==1 && Type==HType.ASCII_TYPE)
        {
            HGemPtr->DataItemIn(len, Type, PPID);
            ret=HGemPtr->GetDataItemLenAndTypeAndDelete(len, Type);
            if(ret==1 && len==1 &&
                 (Type==HType.UINT_1_TYPE || Type==HType.UINT_2_TYPE ||
                  Type==HType.UINT_4_TYPE || Type==HType.UINT_8_TYPE ||
                  Type==HType.INT_1_TYPE  || Type==HType.INT_2_TYPE  ||
                  Type==HType.INT_4_TYPE  || Type==HType.INT_8_TYPE))
            {
                HGemPtr->LocalAcknowledge(7, 2, 0);
                return 1;
            }
        }
    }
    S9F7_IllegalData("S7,F1 Data Format error !!!");
    return 0;
}
// ---------------------------------------------------------------------------
// V 1.0
// S7,F17 This message is used by the host to request the equipment to
// delete process programs from equipment storage.
// ---------------------------------------------------------------------------
void __fastcall HTGem::S7F18_DeleteProcessProgramAcknowledge()
{
    /*
    L,n     (Number of process programs to be deleted)
        1. <PPID1>
         .
         .
        n. <PPIDn>
    */
    char PPID[1024];
    int len, ret, slen;
    unsigned char Type;
    AnsiString S;
    AnsiString asLastFileName=GetLastOpenFN();                                  //JerryYang 20170626 (Steven) 使用中的工作檔不能被刪除

    ret=HGemPtr->GetDataItemLenAndTypeAndDelete(slen, Type);
    if(ret!=1 || Type!=HType.LIST_TYPE)
    {
        S9F7_IllegalData("S7,F17 Data Format error !!!");
        return;
    }

    for(int i=0; i<slen; i++)
    {
        ret=HGemPtr->GetDataItemLenAndType(len, Type);
        if(ret==1 && Type==HType.ASCII_TYPE)
        {
            HGemPtr->DataItemIn(len, Type, PPID);                               // PPID=filename;
            if(asLastFileName==PPID)                                            //JerryYang 20170626 (Steven) 使用中的工作檔不能被刪除
            {
                HGemPtr->LocalAcknowledge(7, 18, 1);
                return;
            }
            S=HGemPtr->UpLoadPath+"\\"+PPID;
            if(DirectoryExists(S)==false)                                       //JerryYang 20170626 (Steven) 不存在的工作檔要return 4
            {
                HGemPtr->LocalAcknowledge(7, 18, 4);
                return;
            }
            ret=DeleteDirectory(S);
            if(ret==0)
            {
                HGemPtr->LocalAcknowledge(7, 18, 1);
                return;
            }
        }
    }
    HGemPtr->LocalAcknowledge(7,18,0);
}
// ---------------------------------------------------------------------------
// V 1.0
// S7,F20 This message is used to transmit the current EPPD.
// ---------------------------------------------------------------------------
void HTGem::S7F20_CurrentEPPDData()
{
    int count;
    if(HGemPtr->SV_70_UNT1_ReceipeStruct!=2)                                    //Steven 20140902 : 0 --> 2
    {
        HGemPtr->InitLocalHead(7, 20, 1);
        HGemPtr->DataItemOut(0, HType.LIST_TYPE, NULL);
        HGemPtr->SendLocalData();
        return;
    }

    HGemPtr->SetReceipeDirectoryAndGlobalName(DataPath, "*.ini", 2);            //  0 : 只傳副檔名為 bld  //Steven 20140902 : 0 --> 2  //Steven 20150203 : 傳送之前要先重新整理
    count=HGemPtr->UploadFileString->Count;
    HGemPtr->InitLocalHead(7, 20, 0);

    HGemPtr->DataItemOut(count, HType.LIST_TYPE, NULL);
    for(int i=0; i<count; i++)
    {
        HGemPtr->DataItemOut(HType.ASCII_TYPE, HGemPtr->UploadFileString->Strings[i]);
    }
    HGemPtr->SendLocalData();
}
//-----------------------------------------------------------------------------
void HTGem::Process_S7F20_CurrentEPPIDData()
{
    int count;
    unsigned char Type;
    char str[256];
    if(HGemPtr->GemRemoteReceipeList==NULL)
        return;

    HGemPtr->GemRemoteReceipeList->Clear();

    if(HGemPtr->GetDataItemLenAndTypeAndDelete(count, Type)==1)
    {
        if(Type==HType.LIST_TYPE)
        {
            for(int i=0; i<count; i++)
            {
                if(HGemPtr->DataItemIn(256, HType.ASCII_TYPE, str))
                {
                    HGemPtr->GemRemoteReceipeList->Items->Add(str);
                }
                else
                {
                    return;
                }
            }
        }
    }
}
// ---------------------------------------------------------------------------
// S9,F3 Unrecognized Stream Type
// ---------------------------------------------------------------------------
void HTGem::S9F3_Unrecognized_Stream_Function_Type(AnsiString S)                //KenHsieh 20221006 : ASE_K1要求下錯誤S,F Code時回傳S9F3
{
    HGemPtr->StringOut(S);
    HGemPtr->InitLocalHead(9, 3, 0);
    HGemPtr->DataItemOut(HType.ASCII_TYPE, S);
    HGemPtr->SendLocalData();
}
// ---------------------------------------------------------------------------
// V 1.1
// S9,F7 Illegal Data
// ---------------------------------------------------------------------------
void HTGem::S9F7_IllegalData(AnsiString S)
{
    HGemPtr->StringOut(S);
    HGemPtr->InitLocalHead(9, 7, 0);
    HGemPtr->DataItemOut(HType.ASCII_TYPE, S);
    HGemPtr->SendLocalData();
}
// ---------------------------------------------------------------------------
// V 1.0
// S10,F3 Acknowledge or error
// ---------------------------------------------------------------------------
void HTGem::S10F4_TerminalDisplaySingleAcknowledge()
{
    int len;
    unsigned char Type;
    unsigned char  uint1EC=0;                                                   //Jimmychiu 20230307 initialized in the constructor
    char str[1024];

    int iDisplay=1;
    AnsiString S,S1;
    AnsiString sStartTime;
    sStartTime=Now().FormatString("yyyy:mm:dd_hh:nn:ss ");

    if(HGemPtr->TerminalDisplayIndex!=0)
    {
        if(HGemPtr->GetDataItemLenAndTypeAndDelete(len, Type)==1)
        {
            if(Type==HType.LIST_TYPE && len==2)
            {
                if(HGemPtr->DataItemIn(1, HType.BINARY_TYPE, &uint1EC)==1)
                {
                    iSECSMessageCanCloseByOperator=uint1EC;                     //Ifor 20171024 : add KYEC SECS GEM Can Close By Operator 0:一般流程 1:直接關閉Alarm 2:啟動工號檢查流程
                    if(HGemPtr->DataItemIn(1024, HType.ASCII_TYPE, str)==1)
                    {
                        HGemPtr->GetTimeInfo();
                        if(CUSTOMER_CODE==CC_MAXIM_THAILAND)                    //Ifor 20251018 add:Secs Alarm List
                        {
                            S=sStartTime+str;
                            FMessageList->Insert(0, S);
                            S="";
                            if(FMessageList->Count > 20)
                            {
                                FMessageList->Delete(20);
                            }

                            for(int i=0; i<FMessageList->Count; i++)
                            {
                                S1=IntToStr(i+1)+ ". ";
                                S=S+S1+FMessageList->Strings[(FMessageList->Count-1)-i]+"\r\n";
                            }
                            SecsAlarmMessage->Add(S);                           //Steven 20150519 : 修正SECS GEM使用ShowMyMessage會出現記憶體破壞
                        }
                        else
                        {
                            S=str;
                            SecsAlarmMessage->Add(S);                           //Steven 20150519 : 修正SECS GEM使用ShowMyMessage會出現記憶體破壞
                        }

                        S=HGemPtr->TimeString+AnsiString(" [R] <<==")+S;
                        if(HGemPtr->TerminalDisplayIndex==1)
                        {
                            if(uint1EC==0)
                                HGemPtr->TerminalMemoPtr->Clear();
                            HGemPtr->TerminalMemoPtr->Lines->Add(S);
                            iDisplay=0;
                        }
                        else if(HGemPtr->TerminalDisplayIndex==2)
                        {
                            if(uint1EC==0)
                                HGemPtr->TerminalListboxPtr->Clear();
                            HGemPtr->TerminalListboxPtr->Items->Add(S);
                            iDisplay=0;
                        }
                        else if(HGemPtr->TerminalDisplayIndex==3)
                        {
                            HGemPtr->TerminalEditPtr->Text=S;
                            iDisplay=0;
                        }
                        else if(HGemPtr->TerminalDisplayIndex==4)
                        {
                            HGemPtr->TerminalPanelPtr->Caption=S;
                            iDisplay=0;
                        }
                    }
                }
            }
        }
    }
    else
    {
        iDisplay=2;
    }

    if(HGemPtr->Remote.W_Bit==1)
        HGemPtr->LocalAcknowledge(10, 4, iDisplay);

    if(iDisplay==0 && HGemPtr->TerminalDisplayIndex2!=0)
    {
        if(HGemPtr->TerminalDisplayIndex2==1)
        {
            if(uint1EC==0)
                HGemPtr->TerminalMemoPtr2->Clear();
            HGemPtr->TerminalMemoPtr2->Lines->Add(S);
        }
        else if(HGemPtr->TerminalDisplayIndex2==2)
        {
            if(uint1EC==0)
                HGemPtr->TerminalListboxPtr2->Clear();
            HGemPtr->TerminalListboxPtr2->Items->Add(S);
        }
        else if(HGemPtr->TerminalDisplayIndex2==3)
        {
            HGemPtr->TerminalEditPtr2->Text=S;
        }
        else if(HGemPtr->TerminalDisplayIndex2==4)
        {
            HGemPtr->TerminalPanelPtr2->Caption=S;
        }
    }
}
// ---------------------------------------------------------------------------
// V 1.0
// S10,F6 Acknowledge or error
// ---------------------------------------------------------------------------
void HTGem::S10F6_TerminalDisplayMultiBlockAcknowledge()
{
    int len;
    unsigned char Type;

    unsigned char  uint1EC=0;                                                   //Jimmychiu 20230307 initialized in the constructor
    char str[1024];
    int iDisplay=1;
    int iDataLen;
    AnsiString S;

    if(HGemPtr->TerminalDisplayIndex!=0)
    {
        if(HGemPtr->GetDataItemLenAndTypeAndDelete(len, Type)==1)
        {
            if(Type==HType.LIST_TYPE && len==2)
            {
                if(HGemPtr->DataItemIn(1, HType.BINARY_TYPE, &uint1EC)==1)
                {
                    iSECSMessageCanCloseByOperator=uint1EC;                     //Ifor 20171024 : add KYEC SECS GEM Can Close By Operator 0:一般流程 1:直接關閉Alarm 2:啟動工號檢查流程
                    HGemPtr->GetDataItemLenAndType(len, Type);                  //取得資料長度與格式
                    if(HGemPtr->GetDataItemLenAndTypeAndDelete(len, Type)==1)
                    {
                        HGemPtr->GetTimeInfo();
                        S="";
                        for(int i=0; i<len; i++)                                //Ifor 20150708  顯示多行資料
                        {
                            HGem->GetDataItemLenAndType(iDataLen, Type);        //取得資料長度與格式
                            HGem->DataItemIn(iDataLen, Type, str);              //取值
                            S=S+str+"\r\n";
                        }
                        SecsAlarmMessage->Add(S);
                        iDisplay=0;
                    }
                }
            }
        }
    }
    else
    {
        iDisplay=2;
    }

    if(HGemPtr->Remote.W_Bit==1)
        HGemPtr->LocalAcknowledge(10, 6, iDisplay);
}
// ---------------------------------------------------------------------------
// V 1.0
// S100,F4
// ---------------------------------------------------------------------------
void HTGem::S100F4_ReportAllAlarm()
{
    bool type;
    HGemPtr->InitLocalHead(100, 4, 0);
    HGemPtr->DataItemOut(HGemPtr->strGrdAlarm->RowCount-1, HType.LIST_TYPE, NULL);
    for(int i=1; i<HGemPtr->strGrdAlarm->RowCount; i++)
    {
        HGemPtr->DataItemOut(3, HType.LIST_TYPE ,NULL);
        HGemPtr->DataItemOut(HType.ASCII_TYPE, HGemPtr->strGrdAlarm->Cells[1][i]);
        HGemPtr->DataItemOut(HType.ASCII_TYPE, HGemPtr->strGrdAlarm->Cells[4][i]);
        type=atoi(HGemPtr->strGrdAlarm->Cells[2][i].c_str());
        HGemPtr->DataItemOut(1, HType.BOOLEAN_TYPE, &type);
    }
    HGemPtr->SendLocalData();
}
// ---------------------------------------------------------------------------
// V 1.0
// S7,F20 This message is used to transmit the current EPPD.
// ---------------------------------------------------------------------------
void HTGem::S101F2_CurrentEPPDData()
{
    int count;
    if(HGemPtr->SV_70_UNT1_ReceipeStruct!=1)
    {
        HGemPtr->InitLocalHead(101, 2, 0);
        HGemPtr->DataItemOut(0, HType.LIST_TYPE, NULL);
        HGemPtr->SendLocalData();
        return;
    }
    count=HGemPtr->UploadFileString->Count;
    HGemPtr->InitLocalHead(101, 2, 0);

    HGemPtr->DataItemOut(2, HType.LIST_TYPE, NULL);
    HGemPtr->DataItemOut(HType.ASCII_TYPE, HGemPtr->SV_71_ASCII_FilenameExtened);
    HGemPtr->DataItemOut(count, HType.LIST_TYPE, NULL);
    for(int i=0; i<count; i++)
    {
        HGemPtr->DataItemOut(HType.ASCII_TYPE, HGemPtr->UploadFileString->Strings[i]);
    }
    HGemPtr->SendLocalData();
}
// ---------------------------------------------------------------------------
// V 1.0
// S7,F20 This message is used to transmit the current EPPD.
// ---------------------------------------------------------------------------
void HTGem::S101F4_CurrentEPPDData()
{
    int count;
    if(HGemPtr->SV_70_UNT1_ReceipeStruct!=2)
    {
        HGemPtr->InitLocalHead(101, 2, 0);
        HGemPtr->DataItemOut(0, HType.LIST_TYPE, NULL);
        HGemPtr->SendLocalData();
        return;
    }
    count=HGemPtr->UploadFileString->Count;
    HGemPtr->InitLocalHead(101, 2, 0);

    HGemPtr->DataItemOut(2, HType.LIST_TYPE, NULL);
    HGemPtr->DataItemOut(HType.ASCII_TYPE, HGemPtr->SV_71_ASCII_FilenameExtened);
    HGemPtr->DataItemOut(count, HType.LIST_TYPE,NULL);
    for(int i=0; i<count; i++)
    {
        HGemPtr->DataItemOut(HType.ASCII_TYPE, HGemPtr->UploadFileString->Strings[i]);
    }
    HGemPtr->SendLocalData();
}
//-----------------------------------------------------------------------------
void HTGem::S101F6()
{
    HGemPtr->bDisableBinaryShow=true;
    S101F6_StoreHostUploadFile();
    HGemPtr->bDisableBinaryShow=false;
    if(HGemPtr->MoveCheckCallBack!=NULL)
        HGemPtr->MoveCheckCallBack();
    HGemPtr->bReceiveS101F5=true;
    HGemPtr->LocalAcknowledge(101, 6, 0);
}
// ---------------------------------------------------------------------------
// V 1.0
// S101,F6
// ---------------------------------------------------------------------------
void HTGem::S101F6_StoreHostUploadFile()
{
    char str[256];
    int len, i, j;
    unsigned char Type;
    AnsiString S;
    FILE *P;
    int iStoreCT, iTotalCount, iFileCount;

    if(HGemPtr->DataItemIn(5, HType.LIST_TYPE, NULL)==1)
    {
        HGemPtr->DataItemIn(200, HType.ASCII_TYPE, str);                        //  file name
        HGemPtr->DataItemIn(1, HType.INT_4_TYPE, &iStoreCT);
        HGemPtr->DataItemIn(1, HType.INT_4_TYPE, &iTotalCount);
        HGemPtr->DataItemIn(1, HType.INT_4_TYPE, &iFileCount);

        if(HGemPtr->GetDataItemLenAndType(len, Type)==1)
        {
            //pig 2014.04.01 ASEM SECS start
            HGemPtr->bFinishDownloadFile=false;
            S = HGemPtr->UpLoadPath+"HGem\\";
            S+=str;
            //pig 2014.04.01 ASEM SECS end
            if(iStoreCT==1)
                P=fopen(S.c_str(), "wb");
            else
                P=fopen(S.c_str(), "ab+");

            if(P!=NULL)
            {
                fwrite(HGemPtr->DownLoadFilePtr, len, 1, P);
                fclose(P);
            }
        }

        if(iStoreCT==iTotalCount)
        {
            for(i=0; i<HGemPtr->GemRemoteReceipeList->Items->Count; i++)
            {
                if(HGemPtr->SV_70_UNT1_ReceipeStruct==0)
                {
                    S=HGemPtr->GemRemoteReceipeList->Items->Strings[i];
                }
                else if(HGemPtr->SV_70_UNT1_ReceipeStruct==1)
                {
                    S=HGemPtr->SV_71_ASCII_FilenameExtened;
                    j=S.LastDelimiter(".");
                    S=S.SubString(j, S.Length());
                    S=HGemPtr->GemRemoteReceipeList->Items->Strings[i]+S;
                }

                if(S==str)
                    HGemPtr->GemRemoteReceipeList->Checked[i]=false;
            }

            if(iFileCount<=1)
            {
                HGemPtr->bFinishDownloadFile=true;
            }
        }
        HGemPtr->LocalAcknowledge(101, 6, 0);
    }
}
//-----------------------------------------------------------------------------
void HTGem::S101F8()
{
    HGemPtr->bDisableBinaryShow=true;
    S101F8_StoreHostUploadFile();
    HGemPtr->bDisableBinaryShow=false;
    if(HGemPtr->MoveCheckCallBack!=NULL)
        HGemPtr->MoveCheckCallBack();
    HGemPtr->bReceiveS101F7=true;
    HGemPtr->LocalAcknowledge(101, 8, 0);
}
// ---------------------------------------------------------------------------
// V 1.0
// S101,F8
// ---------------------------------------------------------------------------
void HTGem::S101F8_StoreHostUploadFile()
{
    char str[2560], PathName[2560];
    int len;
    unsigned char Type;
    char *Ptr;
    AnsiString S;
    FILE *P;
    int iStoreCT, iTotalCount;

    if(HGemPtr->DataItemIn(5, HType.LIST_TYPE, NULL)==1)
    {
        HGemPtr->DataItemIn(200, HType.ASCII_TYPE, PathName);                   //  path name
        HGemPtr->DataItemIn(200, HType.ASCII_TYPE, str);                        //  file name
        HGemPtr->DataItemIn(1, HType.INT_4_TYPE, &iStoreCT);
        HGemPtr->DataItemIn(1, HType.INT_4_TYPE, &iTotalCount);
        if(HGemPtr->GetDataItemLenAndType(len, Type)==1)
        {
            Ptr=HGemPtr->DownLoadFilePtr;
            MyForceDirectories(IncludeTrailingPathDelimiter(HGemPtr->CurrentDirectory)+AnsiString(PathName), __FUNC__);
            S=IncludeTrailingPathDelimiter(HGemPtr->CurrentDirectory)+AnsiString(PathName)+AnsiString("\\")+AnsiString(str);
            if(iStoreCT==1)
                P=fopen(S.c_str(), "wb");
            else
                P=fopen(S.c_str(), "ab+");
            if(P!=NULL)
            {
                fwrite(Ptr,len,1,P);
                fclose(P);
            }
        }
        if(iStoreCT==iTotalCount)
        {
            for(int i=0; i<HGemPtr->GemRemoteReceipeList->Items->Count; i++)
            {
                S=HGemPtr->GemRemoteReceipeList->Items->Strings[i];
                if(S==PathName)
                    HGemPtr->GemRemoteReceipeList->Checked[i]=false;
            }
        }
    }
}
//==============================================================================
//                    S125F2 Enable or Disable EC Data
//==============================================================================
void HTGem::S125F2_EnableDisableECDataAcknowledge()                             //wei 20150630
{
    unsigned char ALED, Type;
    int len, SVLen;
    AnsiString ID;

    if(HGemPtr->DataItemIn(2, HType.LIST_TYPE ,NULL)==1)
    {
        if(HGemPtr->DataItemIn(1, HType.BINARY_TYPE, &ALED))
        {
            if(HGem->GetDataItemLenAndTypeAndDelete(SVLen,Type)==1)
            {
                if(SVLen==0)
                {
                    HGemPtr->EnableDisableECDataAll(ALED);
                    HGemPtr->LocalAcknowledge(125, 2, 0);
                }
                else
                {
                    for(int i=0; i<SVLen; i++)
                    {
                        HGem->GetDataItemLenAndType(len, Type);                 //取得資料長度與格式
                        HGemPtr->DataItemIn(len, Type, ID);
                        if(HGemPtr->EnableDisableECData(ID, ALED))
                            HGemPtr->LocalAcknowledge(125, 2, 0);
                        else
                            HGemPtr->LocalAcknowledge(125, 2, 1);
                    }
                }
                return;
            }
        }
    }
    S9F7_IllegalData("S125,F1 Data Format error !!!");
}
//pig 2014.08.27 KYEC end
//------------------------------------------------------------------------------
// V 1.0
//------------------------------------------------------------------------------
void HTGem::SetECValue(unsigned ECID, void *PtrSour)
{
    unsigned char Type;
    AnsiString ECName;
    AnsiString ECUnit;
    void *P;
    TObject *VclP;
    AnsiString S, IsVCL;

    TPanel      *PanelPtr;
    TCustomEdit *EditPtr;
    TComboBox   *ComboBoxPtr;
    TLabel      *LabelPtr;
    TCheckBox   *CheckBoxPtr;                                                   // 2013/03/14 Lee
    TRadioGroup *RadioGroupPtr;
    TStringList *StringListPtr;

    int i=HGem->EC_ID->IndexOf(ECID);                                           //Steven 20150910 : For迴圈改成 IndexOf
    if(i>=0)
    {
        Type    =atoi(HGem->EC_TYPE->Strings[i].c_str());
        P       =HGem->EC_Ptr->Items[i];
        IsVCL   =HGem->EC_VCL_NAME->Strings[i];
        if(IsVCL==1 || IsVCL==2)
        {
            if(IsVCL==1)
            {
                VclP=(TObject *)HGem->EC_Ptr->Items[i];
                PanelPtr        =dynamic_cast<TPanel        *>(VclP);
                EditPtr         =dynamic_cast<TCustomEdit   *>(VclP);
                ComboBoxPtr     =dynamic_cast<TComboBox     *>(VclP);
                LabelPtr        =dynamic_cast<TLabel        *>(VclP);
                CheckBoxPtr     =dynamic_cast<TCheckBox     *>(VclP);           // 2013/03/14 Lee
                RadioGroupPtr   =dynamic_cast<TRadioGroup   *>(VclP);           // 2013/03/14 Lee
                StringListPtr   =dynamic_cast<TStringList   *>(VclP);           //Steven 20140510
            }
            else
            {
                return;
            }

            char *Ptr2;
            Ptr2=(char *)PtrSour;
            if(Type==HType.ASCII_TYPE)
            {
                S=Ptr2;
            }
            else
            {
                S=*Ptr2;
            }

            if(StringListPtr!=NULL)
            {
                StringListPtr->CommaText=S;
            }
            else if(PanelPtr!=NULL)
            {
                PanelPtr->Caption=S;
            }
            else if(EditPtr!=NULL)
            {
                EditPtr->Text=S;
            }
            else if(ComboBoxPtr!=NULL)
            {
                if(Type==HType.ASCII_TYPE)                                      //Steven 20240802 : 根據Type回傳資料
                    ComboBoxPtr->Text=S;
                else
                    ComboBoxPtr->ItemIndex=atoi(S.c_str());
            }
            else if(LabelPtr!=NULL)
            {
                LabelPtr->Caption=S;
            }
            else if(CheckBoxPtr!=NULL)
            {
                CheckBoxPtr->Checked=(S!="0");
            }
            else if(RadioGroupPtr!=NULL)
            {
                if(Type==HType.ASCII_TYPE)                                      //Steven 20240802 : 根據Type回傳資料
                {
                    if(S!="")
                    {
                        for(int x=0; x<RadioGroupPtr->Items->Count; x++)
                        {
                            if(S==RadioGroupPtr->Items->Strings[x])
                                RadioGroupPtr->ItemIndex=x;
                        }
                    }
                    else
                    {
                        RadioGroupPtr->ItemIndex=-1;
                    }
                }
                else
                {
                    RadioGroupPtr->ItemIndex=atoi(S.c_str());
                }
            }
        }
        else
        {
            if(Type==HType.ASCII_TYPE)
            {
                char *Ptr, *Ptr2;
                Ptr=(char *)P;
                Ptr2=(char *)PtrSour;
                strcpy(Ptr, Ptr2);//lee
            }
            else if(Type==HType.BINARY_TYPE)
            {
                char *Ptr, *Ptr2;
                Ptr=(char *)P;
                Ptr2=(char *)PtrSour;
                *Ptr=*Ptr2;
            }
            else if(Type==HType.BOOLEAN_TYPE)
            {
                unsigned char *Ptr, *Ptr2;
                Ptr=(unsigned char *)P;
                Ptr2=(unsigned char *)PtrSour;
                *Ptr=*Ptr2;
            }
            else if(Type==HType.INT_1_TYPE)
            {
                char *Ptr,*Ptr2;
                Ptr=(char *)P;
                Ptr2=(char *)PtrSour;
                *Ptr=*Ptr2;
            }
            else if(Type==HType.INT_2_TYPE)
            {
                short *Ptr,*Ptr2;
                Ptr=(short *)P;
                Ptr2=(short *)PtrSour;
                *Ptr=*Ptr2;
            }
            else if(Type==HType.INT_4_TYPE)
            {
                int *Ptr, *Ptr2;
                Ptr=(int *)P;
                Ptr2=(int *)PtrSour;
                *Ptr=*Ptr2;
            }
            else if(Type==HType.INT_8_TYPE)                                     //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
            {
                __int64 *Ptr, *Ptr2;
                Ptr=(__int64 *)P;
                Ptr2=(__int64 *)PtrSour;
                *Ptr=*Ptr2;
            }
            else if(Type==HType.UINT_1_TYPE)
            {
                unsigned char *Ptr,*Ptr2;
                Ptr=(unsigned char *)P;
                Ptr2=(unsigned char *)PtrSour;
                *Ptr=*Ptr2;
            }
            else if(Type==HType.UINT_2_TYPE)
            {
                unsigned short *Ptr,*Ptr2;
                Ptr=(unsigned short *)P;
                Ptr2=(unsigned short *)PtrSour;
                *Ptr=*Ptr2;
            }
            else if(Type==HType.UINT_4_TYPE)
            {
                unsigned int  *Ptr,*Ptr2;
                Ptr=(unsigned int  *)P;
                Ptr2=(unsigned int  *)PtrSour;
                *Ptr=*Ptr2;
            }
            else if(Type==HType.UINT_8_TYPE)                                    //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
            {
                unsigned __int64  *Ptr,*Ptr2;
                Ptr=(unsigned __int64  *)P;
                Ptr2=(unsigned __int64  *)PtrSour;
                *Ptr=*Ptr2;
            }
            else if(Type==HType.FT_4_TYPE)
            {
                float  *Ptr,*Ptr2;
                Ptr=(float  *)P;
                Ptr2=(float  *)PtrSour;
                *Ptr=*Ptr2;
            }
            else if(Type==HType.FT_8_TYPE)                                      //Steven 20130730 : FT_4 --> FT_8      //20140308 wei
            {
                double  *Ptr,*Ptr2;
                Ptr=(double  *)P;
                Ptr2=(double  *)PtrSour;
                *Ptr=*Ptr2;
            }
        }
        return;
    }
}
//------------------------------------------------------------------------------
// V 1.0
//------------------------------------------------------------------------------
int HTGem::S2F15_UpdateNewEquipmentConstant()
{
    int EClen, i, len;
    unsigned char Type;

    __int64  int8EC;                                                            //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
    int   int4EC;
    short int2EC;
    char  int1EC;
    unsigned __int64 uint8EC;                                                   //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
    unsigned int   uint4EC, ECID;
    unsigned short uint2EC;
    unsigned char  uint1EC;
    float          floatEC;
    double         doubleEC;
    AnsiString S;

    if(HGem->GetDataItemLenAndTypeAndDelete(EClen, Type)==1)
    {
        if(Type==HType.LIST_TYPE)
        {
            for(i=0; i<EClen; i++)
            {
                if(HGem->GetDataItemLenAndTypeAndDelete(len, Type)==1)
                {
                    if(Type!=HType.LIST_TYPE || len!=2)
                        break;
                    // get ECID

                    HGem->GetDataItemLenAndType(len, Type);
                    HGem->DataItemIn(len, Type, S);
                    ECID=atoi(S.c_str());

                    HGem->GetDataItemLenAndType(len, Type);

                    if(Type==HType.UINT_1_TYPE)
                    {
                        if(HGem->DataItemIn(1, Type, &uint1EC)==1)
                            SetECValue(ECID, &uint1EC);
                    }
                    else if(Type==HType.UINT_2_TYPE)
                    {
                        if(HGem->DataItemIn(1, Type, &uint2EC)==1)
                            SetECValue(ECID, &uint2EC);
                    }
                    else if(Type==HType.UINT_4_TYPE)
                    {
                        if(HGem->DataItemIn(1, Type, &uint4EC)==1)
                            SetECValue(ECID, &uint4EC);
                    }
                    else if(Type==HType.UINT_8_TYPE)                            //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                    {
                        if(HGem->DataItemIn(1, Type, &uint8EC)==1)
                            SetECValue(ECID, &uint8EC);
                    }
                    else if(Type==HType.INT_1_TYPE)
                    {
                        if(HGem->DataItemIn(1, Type, &int1EC)==1)
                            SetECValue(ECID, &int1EC);
                    }
                    else if(Type==HType.INT_2_TYPE)
                    {
                        if(HGem->DataItemIn(1, Type, &int2EC)==1)
                            SetECValue(ECID, &int2EC);
                    }
                    else if(Type==HType.INT_4_TYPE)
                    {
                        if(HGem->DataItemIn(1, Type, &int4EC)==1)
                            SetECValue(ECID, &int4EC);
                    }
                    else if(Type==HType.INT_8_TYPE)                             //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
                    {
                        if(HGem->DataItemIn(1, Type, &int8EC)==1)
                            SetECValue(ECID, &int8EC);
                    }
                    else if(Type==HType.ASCII_TYPE)
                    {
                        char *Str;
                        Str=new char [len+100];
                        if(HGem->DataItemIn(len, Type, Str)==1)
                            SetECValue(ECID, Str);
                    }
                    else if(Type==HType.BINARY_TYPE)
                    {
                        if(HGem->DataItemIn(len, Type, &int1EC)==1)
                            SetECValue(ECID, &int1EC);
                    }
                    else if(Type==HType.BOOLEAN_TYPE)
                    {
                        if(HGem->DataItemIn(len, Type, &int1EC)==1)
                        {
//                            if(int1EC==0x00)                                    //Steven 20230815 : 將ASCII轉成CHAR避免0x00或0x01造成不可視字元
//                                int1EC='0';
//                            else
//                                int1EC='1';
                            SetECValue(ECID, &int1EC);
                        }
                    }
                    else if(Type==HType.FT_4_TYPE)
                    {
                        if(HGem->DataItemIn(len, Type, &floatEC)==1)
                            SetECValue(ECID, &floatEC);
                    }
                    else if(Type==HType.FT_8_TYPE)
                    {
                        if(HGem->DataItemIn(len, Type, &doubleEC)==1)
                            SetECValue(ECID, &doubleEC);
                    }
                    else                                                        // error format (SECS-II ASCII code ir correct ,but ITRI is failure,need confirm with ITRI
                    {
                        HGem->SendInvalidDataMessageToHost("error format");
                        return -1;
                    }
                }
                else
                {
                    return -1;
                }
            }
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
    ReloadParameter();
    return 1;
}
//==============================================================================
//  V 1.0
//     ret
//     0   = Acknowledge
//     1   = Denied. At least one constant does not exist
//     2   = Denied. Busy
//     3   = Denied. At least one constant out of range
//    >3   = Other equipment-specific error
//    4-63   Reserved
//==============================================================================
int HTGem::S2F15_CheckNewEquipmentConstant()                                    //wei 20170417 (Steven) add S2F15
{
    int EClen, i, len, ret;
    unsigned char Type;
    __int64  int8EC;                                                            //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
    int   int4EC;
    short int2EC;
    char  int1EC;
    unsigned __int64  uint8EC;                                                  //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
    unsigned int   uint4EC;
    AnsiString ECID;
    unsigned short uint2EC;
    unsigned char  uint1EC;
    float          floatEC;
    double         doubleEC;
//    void *P;

    /*
        L,n
        1. L,2
        1. <ECID1>
        2. <ECV1>
        2. L,2
        .
        .
        n. L,2
        1. <ECIDn>
        2. <ECVn>
    */

    if(HGem->GetDataItemLenAndTypeAndDelete(EClen, Type)!=1)
        return -1;
    if(Type!=HType.LIST_TYPE)
        return -1;
    if(EClen<1)
        return -1;
    for(i=0; i<EClen; i++)
    {
        if(HGem->GetDataItemLenAndTypeAndDelete(len, Type)!=1)
            return -1;
        if(Type!=HType.LIST_TYPE || len!=2)
            return -1;
        ret=1;
        if(HGem->GetDataItemLenAndType(len, Type)==1)
        {
            if(HGem->DataItemIn(len, Type, ECID)!=1)
                return -1;
        }
        else
        {
            return -1;
        }

        if(HGem->GetDataItemLenAndType(len, Type)==1)
        {
            if(Type==HType.UINT_1_TYPE)
            {
                if(HGem->DataItemIn(1, Type, &uint1EC)==1)
                    ret=CheckECValue(ECID, &uint1EC);
                else
                    return -1;
            }
            else if(Type==HType.UINT_2_TYPE)
            {
                if(HGem->DataItemIn(1, Type, &uint2EC)==1)
                    ret=CheckECValue(ECID, &uint2EC);
                else
                    return -1;
            }
            else if(Type==HType.UINT_4_TYPE)
            {
                if(HGem->DataItemIn(1, Type, &uint4EC)==1)
                    ret=CheckECValue(ECID, &uint4EC);
                else
                    return -1;
            }
            else if(Type==HType.UINT_8_TYPE)                                    //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
            {
                if(HGem->DataItemIn(1, Type, &uint8EC)==1)
                    ret=CheckECValue(ECID, &uint8EC);
                else
                    return -1;
            }
            else if(Type==HType.INT_1_TYPE)
            {
                if(HGem->DataItemIn(1, Type, &int1EC)==1)
                    ret=CheckECValue(ECID, &int1EC);
                else
                    return -1;
            }
            else if(Type==HType.INT_2_TYPE)
            {
                if(HGem->DataItemIn(1, Type, &int2EC)==1)
                    ret=CheckECValue(ECID, &int2EC);
                else
                    return -1;
            }
            else if(Type==HType.INT_4_TYPE)
            {
                if(HGem->DataItemIn(1, Type, &int4EC)==1)
                    ret=CheckECValue(ECID, &int4EC);
                else
                    return -1;
            }
            else if(Type==HType.INT_8_TYPE)                                     //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
            {
                if(HGem->DataItemIn(1, Type, &int8EC)==1)
                    ret=CheckECValue(ECID, &int8EC);
                else
                    return -1;
            }
            else if(Type==HType.ASCII_TYPE)
            {
                char *Str;
                Str=new char [len+100];
                if(HGem->DataItemIn(len, Type, Str)==1)
                    ret=CheckECValue(ECID, Str);
                else
                    return -1;
            }
            else if(Type==HType.BINARY_TYPE)
            {
                if(HGem->DataItemIn(len, Type, &int1EC)==1)
                    ret=CheckECValue(ECID, &int1EC);
                else
                    return -1;
            }
            else if(Type==HType.BOOLEAN_TYPE)
            {
                if(HGem->DataItemIn(len, Type, &int1EC)==1)
                    ret=CheckECValue(ECID, &int1EC);
                else
                    return -1;
            }
            else if(Type==HType.FT_4_TYPE)
            {
                if(HGem->DataItemIn(len, Type, &floatEC)==1)
                    ret=CheckECValue(ECID, &floatEC);
                else
                    return -1;
            }
            else if(Type==HType.FT_8_TYPE)
            {
                if(HGem->DataItemIn(len, Type, &doubleEC)==1)
                    ret=CheckECValue(ECID, &doubleEC);
                else
                    return -1;
            }
            else                                                                // error format (SECS-II ASCII code ir correct ,but ITRI is failure,need confirm with ITRI
            {
                HGem->SendInvalidDataMessageToHost("error format");
                return -1;
            }
            if(ret!=0)
                return ret;
        }
        else
        {
            HGem->SendInvalidDataMessageToHost("GetDataItemLenAndType Error");
            return -1;
        }
    }
    return 0;
}
//------------------------------------------------------------------------------
int HTGem::CheckECValue(AnsiString ECID, void *PtrSour)
{
    int i;
    unsigned char Type;
    AnsiString ECName;
    AnsiString ECUnit;
    void *PMax, *PMin;
    AnsiString PMax_Value, PMin_Value;
    AnsiString S;

    i=HGem->EC_ID->IndexOf(ECID);                                               //Steven 20150910 : For迴圈改成 IndexOf
    if(i>=0)
    {
        Type=atoi(HGem->EC_TYPE->Strings[i].c_str());
        PMax=HGem->EC_Ptr_Max->Items[i];
        PMin=HGem->EC_Ptr_Min->Items[i];

        PMax_Value=HGem->EC_Ptr_Max_Value->Strings[i];
        PMin_Value=HGem->EC_Ptr_Min_Value->Strings[i];

        if(Type==HType.ASCII_TYPE)
        {
            return 0;
        }
        else if(Type==HType.BINARY_TYPE)
        {
            return 0;
        }
        else if(Type==HType.BOOLEAN_TYPE)
        {
            return 0;
        }
        else if(Type==HType.INT_1_TYPE)
        {
            char *Ptr2, *PtrMax, *PtrMin;
            char PtrMax_Value, PtrMin_Value;

            PtrMax=(char *)PMax;
            PtrMin=(char *)PMin;

            PtrMax_Value=atoi(PMax_Value.c_str());
            PtrMin_Value=atoi(PMin_Value.c_str());                              //2013/09/17 lee

            Ptr2=(char *)PtrSour;

            if(PtrMax==NULL || PtrMin==NULL)
            {
                if((*Ptr2)>PtrMax_Value)
                    return 3;
                if((*Ptr2)<PtrMin_Value)
                    return 3;
                return 0;
            }
            else
            {
                if(PtrMax!=NULL)
                    if((*Ptr2)>(*PtrMax))
                        return 3;
                if(PtrMin!=NULL)
                    if((*Ptr2)<(*PtrMin))
                        return 3;
                return 0;
            }
        }
        else if(Type==HType.INT_2_TYPE)
        {
            short *Ptr2, *PtrMax, *PtrMin;
            short PtrMax_Value, PtrMin_Value;

            PtrMax=(short *)PMax;
            PtrMin=(short *)PMin;
            PtrMax_Value=atoi(PMax_Value.c_str());
            PtrMin_Value=atoi(PMin_Value.c_str());                              //2013/09/17 lee
            Ptr2=(short *)PtrSour;

            if(PtrMax==NULL || PtrMin==NULL)
            {
                if((*Ptr2)>PtrMax_Value)
                    return 3;
                if((*Ptr2)<PtrMin_Value)
                    return 3;
                return 0;
            }
            else
            {
                if(PtrMax!=NULL)
                    if((*Ptr2)>(*PtrMax))
                        return  3;
                if(PtrMin!=NULL)
                    if((*Ptr2)<(*PtrMin))
                        return 3;
                return 0;
            }
        }
        else if(Type==HType.INT_4_TYPE)
        {
            int *Ptr2, *PtrMax, *PtrMin;
            int PtrMax_Value, PtrMin_Value;
            PtrMax=(int *)PMax;
            PtrMin=(int *)PMin;

            PtrMax_Value=atoi(PMax_Value.c_str());
            PtrMin_Value=atoi(PMin_Value.c_str());

            Ptr2=(int *)PtrSour;

            if(PtrMax==NULL || PtrMin==NULL)
            {
                if((*Ptr2)>PtrMax_Value)
                    return 3;
                if((*Ptr2)<PtrMin_Value)
                    return 3;
                return 0;
            }
            else
            {
                if(PtrMax!=NULL)
                    if((*Ptr2)>(*PtrMax))
                        return 3;
                if(PtrMin!=NULL)
                    if((*Ptr2)<(*PtrMin))
                        return 3;
                return 0;
            }
        }
        else if(Type==HType.INT_8_TYPE)                                         //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
        {
            __int64 *Ptr2, *PtrMax, *PtrMin;
            __int64 PtrMax_Value, PtrMin_Value;
            PtrMax=(__int64 *)PMax;
            PtrMin=(__int64 *)PMin;

            PtrMax_Value=_atoi64(PMax_Value.c_str());
            PtrMin_Value=_atoi64(PMin_Value.c_str());

            Ptr2=(__int64 *)PtrSour;

            if(PtrMax==NULL || PtrMin==NULL)
            {
                if((*Ptr2)>PtrMax_Value)
                    return 3;
                if((*Ptr2)<PtrMin_Value)
                    return 3;
                return 0;
            }
            else
            {
                if(PtrMax!=NULL)
                    if((*Ptr2)>(*PtrMax))
                        return 3;
                if(PtrMin!=NULL)
                    if((*Ptr2)<(*PtrMin))
                        return 3;
                return 0;
            }
        }
        else if(Type==HType.UINT_1_TYPE)
        {
            unsigned char *Ptr2,*PtrMax,*PtrMin;
            unsigned char PtrMax_Value,PtrMin_Value;
            PtrMax=(unsigned char *)PMax;
            PtrMin=(unsigned char *)PMin;
            Ptr2=(unsigned char *)PtrSour;
            PtrMax_Value=atoi(PMax_Value.c_str());
            PtrMin_Value=atoi(PMin_Value.c_str());                              //2013/09/17 lee

            if(PtrMax==NULL || PtrMin==NULL)
            {
                if((*Ptr2)>PtrMax_Value)
                    return 3;
                if((*Ptr2)<PtrMin_Value)
                    return 3;
                return 0;
            }
            else
            {
                if(PtrMax!=NULL)
                    if((*Ptr2)>=(*PtrMax))
                        return 3;
                if(PtrMin!=NULL)
                    if((*Ptr2)<=(*PtrMin))
                        return 3;
                return 0;
            }
        }
        else if(Type==HType.UINT_2_TYPE)
        {
            unsigned short *Ptr2,*PtrMax,*PtrMin;
            unsigned short PtrMax_Value,PtrMin_Value;

            PtrMax=(unsigned short *)PMax;
            PtrMin=(unsigned short *)PMin;
            Ptr2=(unsigned short *)PtrSour;
            PtrMax_Value=atoi(PMax_Value.c_str());
            PtrMin_Value=atoi(PMin_Value.c_str());                              //2013/09/17 lee

            if(PtrMax==NULL || PtrMin==NULL)
            {
                if((*Ptr2)>PtrMax_Value)
                    return 3;
                if((*Ptr2)<PtrMin_Value)
                    return 3;
                return 0;
            }
            else
            {
                if(PtrMax!=NULL)
                    if((*Ptr2)>=(*PtrMax))
                        return 3;
                if(PtrMin!=NULL)
                    if((*Ptr2)<=(*PtrMin))
                        return 3;
                return 0;
            }
        }
        else if(Type==HType.UINT_4_TYPE)
        {
            unsigned int  *Ptr2,*PtrMax,*PtrMin;
            unsigned int PtrMax_Value,PtrMin_Value;
            PtrMax=(unsigned int *)PMax;
            PtrMin=(unsigned int *)PMin;
            Ptr2=(unsigned int *)PtrSour;
            PtrMax_Value=atoi(PMax_Value.c_str());
            PtrMin_Value=atoi(PMin_Value.c_str());                              //2013/09/17 lee

            if(PtrMax==NULL || PtrMin==NULL)
            {
                if((*Ptr2)>PtrMax_Value)
                    return 3;
                if((*Ptr2)<PtrMin_Value)
                    return 3;
                return 0;
            }
            else
            {
                if(PtrMax!=NULL)
                    if((*Ptr2)>(*PtrMax))
                        return 3;
                if(PtrMin!=NULL)
                    if((*Ptr2)<(*PtrMin))
                        return 3;
                return 0;
            }
        }
        else if(Type==HType.UINT_8_TYPE)                                        //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
        {
            unsigned __int64 *Ptr2,*PtrMax,*PtrMin;
            unsigned __int64 PtrMax_Value,PtrMin_Value;
            PtrMax=(unsigned __int64 *)PMax;
            PtrMin=(unsigned __int64 *)PMin;
            Ptr2=(unsigned __int64 *)PtrSour;
            PtrMax_Value=(unsigned)_atoi64(PMax_Value.c_str());
            PtrMin_Value=(unsigned)_atoi64(PMin_Value.c_str());                 //2013/09/17 lee

            if(PtrMax==NULL || PtrMin==NULL)
            {
                if((*Ptr2)>PtrMax_Value)
                    return 3;
                if((*Ptr2)<PtrMin_Value)
                    return 3;
                return 0;
            }
            else
            {
                if(PtrMax!=NULL)
                    if((*Ptr2)>(*PtrMax))
                        return 3;
                if(PtrMin!=NULL)
                    if((*Ptr2)<(*PtrMin))
                        return 3;
                return 0;
            }
        }
        else if(Type==HType.FT_4_TYPE)
        {
            float *Ptr2, *PtrMax, *PtrMin;
            float PtrMax_Value,PtrMin_Value;

            PtrMax=(float *)PMax;
            PtrMin=(float *)PMin;
            Ptr2=(float *)PtrSour;

            PtrMax_Value=atoi(PMax_Value.c_str());
            PtrMin_Value=atoi(PMin_Value.c_str());                              //2013/09/17 lee

            if(PtrMax==NULL || PtrMin==NULL)
            {
                if((*Ptr2)>PtrMax_Value)
                    return 3;
                if((*Ptr2)<PtrMin_Value)
                    return 3;
                return 0;
            }
            else
            {
                if(PtrMax!=NULL)
                    if((*Ptr2)>(*PtrMax))
                        return 3;
                if(PtrMin!=NULL)
                    if((*Ptr2)<(*PtrMin))
                        return 3;
                return 0;
            }
        }
        else if(Type==HType.FT_4_TYPE)
        {
            double  *Ptr2,*PtrMax,*PtrMin;
            double PtrMax_Value,PtrMin_Value;
            PtrMax=(double *)PMax;
            PtrMin=(double *)PMin;
            Ptr2=(double *)PtrSour;

            PtrMax_Value=atoi(PMax_Value.c_str());
            PtrMin_Value=atoi(PMin_Value.c_str());                              //2013/09/17 lee

            if(PtrMax==NULL || PtrMin==NULL)
            {
                if((*Ptr2)>PtrMax_Value)
                    return 3;
                if((*Ptr2)<PtrMin_Value)
                    return 3;
                return 0;
            }
            else
            {
                if(PtrMax!=NULL)
                    if((*Ptr2)>(*PtrMax))
                        return 3;
                if(PtrMin!=NULL)
                    if((*Ptr2)<(*PtrMin))
                        return 3;
                return 0;
            }
        }
        else if(Type==HType.FT_8_TYPE)                                          //Steven 20130730 : FT_4 --> FT_8
        {
            double  *Ptr2,*PtrMax,*PtrMin;
            double PtrMax_Value,PtrMin_Value;
            PtrMax=(double *)PMax;
            PtrMin=(double *)PMin;
            Ptr2=(double *)PtrSour;

            PtrMax_Value=atoi(PMax_Value.c_str());
            PtrMin_Value=atoi(PMin_Value.c_str());                              //2013/09/17 lee

            if(PtrMax==NULL || PtrMin==NULL)
            {
                if((*Ptr2)>PtrMax_Value)
                    return 3;
                if((*Ptr2)<PtrMin_Value)
                    return 3;
                return 0;
            }
            else
            {
                if(PtrMax!=NULL)
                    if((*Ptr2)>(*PtrMax))
                        return 3;
                if(PtrMin!=NULL)
                    if((*Ptr2)<(*PtrMin))
                        return 3;
                return 0;
            }
        }
        return 4;
    }
    HGem->StringOut("ECID:"+ECID+" not exist !!");
    return 1;
}
//---------------------------------------------------------------------------
void HTGem::S103F12_StatusVariableNamelistReply()                               //Steven 20140911 : Add S103F11
{
    int SVlen, i, len;
    unsigned char Type;

//    int   int4SV;
//    short int2SV;
//    char  int1SV;
//    unsigned int   uint4SV;
//    unsigned short uint2SV;
//    unsigned char  uint1SV;
//    void *P;
//    bool ret;
    AnsiString S;

    if(HGemPtr->GetDataItemLenAndTypeAndDelete(SVlen,Type)==1)
    {
        if(Type==HType.LIST_TYPE || SVlen==0)
        {
            HGemPtr->InitLocalHead(103,12,0);
            if(SVlen==0)
            {
                len=HGemPtr->SV_ID->Count;
                HGemPtr->DataItemOut(len, HType.LIST_TYPE, NULL);
                for(i=0; i<len; i++)
                    HGemPtr->DataItemOutSVNameListWithValue(HGemPtr->SV_ID->Strings[i]);
            }
            else
            {
                HGemPtr->DataItemOut(SVlen, HType.LIST_TYPE, NULL);
                for(i=0; i<SVlen; i++)
                {
                    if(HGemPtr->GetDataItemLenAndType(len,Type)==1)
                    {
                        if(HGemPtr->DataItemIn(len, Type, S)==1)
                        {
                            if(HGemPtr->DataItemOutSVNameListWithValue(S)==false)
                                return;
                        }
                        else
                        {
                            S9F7_IllegalData("S103,F11 data format error");
                            return;
                        }
                    }
                    else
                    {
                        S9F7_IllegalData("S103,F11 data format error");
                        return;
                    }
                }
            }
            HGemPtr->SendLocalData();
        }
        else
        {
            S9F7_IllegalData("S103,F11 data format error");
        }
    }
    else
    {
        S9F7_IllegalData("S103,F11 data format error");
    }
}
// ---------------------------------------------------------------------------

