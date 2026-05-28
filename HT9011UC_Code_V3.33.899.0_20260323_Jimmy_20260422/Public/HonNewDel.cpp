#include "MachineDefine.h"
#pragma hdrstop

#include "HonNewDel.h"

#include "MemoryAlarm.h"    //yunghsin 20170612 add
//---------------------------------------------------------------------------
//void SetVersion( AnsiString sVer )
//{
//    HonNewForm->sVersion = sVer;
//}
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
template <typename T1> T1* HonNew(T1* Ptr, AnsiString sFile, AnsiString sFunc, AnsiString sLine)
{
    int pos=0;
    AnsiString szFileLine="", szAlarm="";
    AnsiString sError="";

    szFileLine=sFile+"_"+sFunc+"_Line"+sLine;
    pos=MemoryAlarmForm->ansiFile->IndexOf(szFileLine);
    if(pos!=-1)
    {
        szAlarm.sprintf("File:%s\r\nFunc:%s\r\nPos:Line%s", sFile, sFunc, sLine);
        ShowMemoryAlarm(szAlarm);
    }
    MemoryAlarmForm->ansiFile->Add(szFileLine);
    Ptr = new T1;
    MemoryAlarmForm->listPtr->Add(Ptr);

    return Ptr;
}
//---------------------------------------------------------------------------
template <typename T1> T1* HonNew(T1* Ptr, AnsiString sFile, AnsiString sFunc, AnsiString sLine, int iArrayIndex)
{
    int pos=0;
    AnsiString szFileLine="", szAlarm="";
    AnsiString sError="";

    if(iArrayIndex<0 || iArrayIndex>10000)
    {
        sError="Array Index Error : "+AnsiString(iArrayIndex);
        ShowMemoryAlarm(sError);
        return NULL;
    }

    szFileLine=sFile+"_"+sFunc+"_Line"+sLine+"_Index"+AnsiString(iArrayIndex);
    pos=MemoryAlarmForm->ansiFile->IndexOf(szFileLine);
    if(pos!=-1)
    {
        szAlarm.sprintf("File:%s\r\nFunc:%s\r\nPos:Line%s_Index%d", sFile, sFunc, sLine, iArrayIndex);
        ShowMemoryAlarm(szAlarm);
    }
    MemoryAlarmForm->ansiFile->Add(szFileLine);
    Ptr = new T1;
    MemoryAlarmForm->listPtr->Add(Ptr);

    return Ptr;
}
//---------------------------------------------------------------------------
template <typename T1, typename T2>  T1* HonNew(T2* Ptr, AnsiString sFile, AnsiString sFunc, AnsiString sLine, int iArrayIndex)
{
    int pos=0;
    AnsiString szFileLine="", szAlarm="";
    AnsiString sError="";

    if(iArrayIndex<0 || iArrayIndex>10000)
    {
        sError="Array Index Error : "+AnsiString(iArrayIndex);
        ShowMemoryAlarm(sError);
        return NULL;
    }

    szFileLine=sFile+"_"+sFunc+"_Line"+sLine+"_Index"+AnsiString(iArrayIndex);
    pos=MemoryAlarmForm->ansiFile->IndexOf(szFileLine);
    if(pos!=-1)
    {
        szAlarm.sprintf("File:%s\r\nFunc:%s\r\nPos:Line%s_Index%d", sFile, sFunc, sLine, iArrayIndex);
        ShowMemoryAlarm(szAlarm);
    }
    MemoryAlarmForm->ansiFile->Add(szFileLine);
    Ptr=new T2;
    MemoryAlarmForm->listPtr->Add(Ptr);

    return Ptr;
}
//---------------------------------------------------------------------------
template <typename T1>  T1* HonNew(T1* Ptr, AnsiString sFile, AnsiString sFunc, AnsiString sLine, TComponent* pOwner)
{
    int pos=0;
    AnsiString szFileLine="", szAlarm="";
    AnsiString sError="";

    szFileLine=sFile+"_"+sFunc+"_Line"+sLine;
    pos=MemoryAlarmForm->ansiFile->IndexOf(szFileLine);
    if(pos!=-1)
    {
        szAlarm.sprintf("File:%s\r\nFunc:%s\r\nPos:Line%s", sFile, sFunc, sLine);
        ShowMemoryAlarm(szAlarm);
    }

    MemoryAlarmForm->ansiFile->Add(szFileLine);
    Ptr=new T1(pOwner);
    MemoryAlarmForm->listPtr->Add(Ptr);

    return Ptr;
}
//---------------------------------------------------------------------------
template <typename T1>  T1* HonNew(T1* Ptr, AnsiString sFile, AnsiString sFunc, AnsiString sLine, TComponent* pOwner, int iArrayIndex)
{
    int pos=0;
    AnsiString szFileLine="", szAlarm="";
    AnsiString sError="";

    if(iArrayIndex<0 || iArrayIndex>10000)
    {
        sError="Array Index Error : "+AnsiString(iArrayIndex);
        ShowMemoryAlarm(sError);
        return NULL;
    }

    szFileLine=sFile+"_"+sFunc+"_Line"+sLine+"_Index"+AnsiString(iArrayIndex);
    pos=MemoryAlarmForm->ansiFile->IndexOf(szFileLine);
    if(pos!=-1)
    {
        szAlarm.sprintf("File:%s\r\nFunc:%s\r\nPos:Line%s_Index%d", sFile, sFunc, sLine, iArrayIndex);
        ShowMemoryAlarm(szAlarm);
    }

    MemoryAlarmForm->ansiFile->Add(szFileLine);
    Ptr=new T1(pOwner);
    MemoryAlarmForm->listPtr->Add(Ptr);

    return Ptr;
}
//---------------------------------------------------------------------------
template <typename T1,typename T2> T1* HonNew(T2* Ptr, AnsiString sFile, AnsiString sFunc, AnsiString sLine, TComponent* pOwner, int iArrayIndex)
{
    int pos=0;
    AnsiString szFileLine="", szAlarm="";
    AnsiString sError="";

    if(iArrayIndex<0 || iArrayIndex>10000)
    {
        sError="Array Index Error : "+AnsiString(iArrayIndex);
        ShowMemoryAlarm(sError);
        return NULL;
    }

    szFileLine=sFile+"_"+sFunc+"_Line"+sLine+"_Index"+AnsiString(iArrayIndex);
    pos=MemoryAlarmForm->ansiFile->IndexOf(szFileLine);
    if(pos!=-1)
    {
        szAlarm.sprintf("File:%s\r\nFunc:%s\r\nPos:Line%s_Index%d", sFile, sFunc, sLine, iArrayIndex);
        ShowMemoryAlarm(szAlarm);
    }
    MemoryAlarmForm->ansiFile->Add(szFileLine);
    Ptr = new T2(pOwner);
    MemoryAlarmForm->listPtr->Add(Ptr);

    return Ptr;
}
//---------------------------------------------------------------------------
template <typename T1>  T1* HonNewArray(T1* Ptr, AnsiString sFile, AnsiString sFunc, AnsiString sLine, int iArraySize)
{
    int pos=0;
    AnsiString szFileLine="", szAlarm="";
    AnsiString sError="";

    if(iArraySize<0 || iArraySize>10000)
    {
        sError="Array Index Error : "+AnsiString(iArraySize);
        ShowMemoryAlarm(sError);
        return NULL;
    }

    szFileLine=sFile+"_"+sFunc+"_Line"+sLine+"_Size"+AnsiString(iArraySize);
    pos=MemoryAlarmForm->ansiFile->IndexOf(szFileLine);
    if(pos!=-1)
    {
        szAlarm.sprintf("File:%s\r\nFunc:%s\r\nPos:Line%s_Size%d", sFile, sFunc, sLine, iArraySize);
        ShowMemoryAlarm(szAlarm);
    }
    MemoryAlarmForm->ansiFile->Add(szFileLine);
    Ptr = new T1[iArraySize];
    MemoryAlarmForm->listPtr->Add(Ptr);

    return Ptr;
}
//---------------------------------------------------------------------------
template <typename T1,typename T2>  T1* HonNewArray(T2* Ptr, AnsiString sFile, AnsiString sFunc, AnsiString sLine, int iArraySize)
{
    int pos=0;
    AnsiString szFileLine="", szAlarm="";
    AnsiString sError="";

    if(iArraySize<=0 || iArraySize>10000)
    {
        sError="Array Size Error : "+AnsiString(iArraySize);
        ShowMemoryAlarm( sError );
        return NULL;
    }

    szFileLine=sFile+"_"+sFunc+"_Line"+sLine+"_Size"+AnsiString(iArraySize);
    pos=MemoryAlarmForm->ansiFile->IndexOf(szFileLine);
    if(pos!=-1)
    {
        szAlarm.sprintf("File:%s\r\nFunc:%s\r\nPos:Line%s_Size%d", sFile, sFunc, sLine, iArraySize);
        ShowMemoryAlarm(szAlarm);
    }
    MemoryAlarmForm->ansiFile->Add(szFileLine);
    Ptr = new T2[iArraySize];
    MemoryAlarmForm->listPtr->Add(Ptr);

    return Ptr;
}
//---------------------------------------------------------------------------
template <typename T1>  void HonDelete(T1* Ptr)
{
    int pos=0;
    pos=MemoryAlarmForm->listPtr->IndexOf(Ptr);
    if(pos!=-1)
    {
        Ptr=(T1*)MemoryAlarmForm->listPtr->Items[pos];
        delete Ptr;
        MemoryAlarmForm->listPtr->Delete(pos);
        MemoryAlarmForm->ansiFile->Delete(pos);
    }
}
//---------------------------------------------------------------------------
