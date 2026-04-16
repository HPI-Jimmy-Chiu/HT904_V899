//---------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop

#include "uSetUpConfiguration.h"
#include "FileInfo.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

extern int  AUTO_EMPTY_COLOR;
//JimmyChiu 20220303 : Add class SetUpConfiguration
//<==
bool SetUpConfiguration::ReadSetUpFile(AnsiString asFileWithPath)
{
    if(asFileWithPath.Pos(".bin")==0)
        return false;
    FileInfo moFileInfo(asFileWithPath);
    moFileInfo.Add(sHANDLER_MODE, "HANDLER MODE");
    moFileInfo.Add(sSITE_MODE, "SITE MODE");
    moFileInfo.Add(sHANDLER_TYPE, "HANDLER TYPE");
    moFileInfo.Add(sAa, "Aa");
    moFileInfo.Add(sAb, "Ab");
    moFileInfo.Add(sAc, "Ac");
    moFileInfo.Add(sAd, "Ad");
    moFileInfo.Add(sAe, "Ae");
    moFileInfo.Add(sAf, "Af");
    moFileInfo.Add(sAg, "Ag");
    moFileInfo.Add(sAh, "Ah");
    moFileInfo.Add(sBa, "Ba");
    moFileInfo.Add(sBb, "Bb");
    moFileInfo.Add(sBc, "Bc");
    moFileInfo.Add(sBd, "Bd");
    moFileInfo.Add(sBe, "Be");
    moFileInfo.Add(sBf, "Bf");
    moFileInfo.Add(sBg, "Bg");
    moFileInfo.Add(sBh, "Bh");
    moFileInfo.Add(sCa, "Ca");
    moFileInfo.Add(sCb, "Cb");
    moFileInfo.Add(sCc, "Cc");
    moFileInfo.Add(sCd, "Cd");
    moFileInfo.Add(sCe, "Ce");
    moFileInfo.Add(sCf, "Cf");
    moFileInfo.Add(sCg, "Cg");
    moFileInfo.Add(sCh, "Ch");
    moFileInfo.Add(sDa, "Da");
    moFileInfo.Add(sDb, "Db");
    moFileInfo.Add(sDc, "Dc");
    moFileInfo.Add(sDd, "Dd");
    moFileInfo.Add(sDe, "De");
    moFileInfo.Add(sDf, "Df");
    moFileInfo.Add(sDg, "Dg");
    moFileInfo.Add(sDh, "Dh");
    moFileInfo.Add(sAUTO_1_CAT_A_HARDWARE_BIN,  "AUTO 1 [CAT A] HARDWARE BIN");
    moFileInfo.Add(sAUTO_2_CAT_B_HARDWARE_BIN,  "AUTO 2 [CAT B] HARDWARE BIN");
    moFileInfo.Add(sAUTO_3_CAT_C_HARDWARE_BIN,  "AUTO 3 [CAT C] HARDWARE BIN");
    moFileInfo.Add(sFIX_1_CAT_D_HARDWARE_BIN,   "FIX 1 [CAT D] HARDWARE BIN");
    moFileInfo.Add(sFIX_2_CAT_E_HARDWARE_BIN,   "FIX 2 [CAT E] HARDWARE BIN");
    moFileInfo.Add(sFIX_3_JAM_HARDWARE_BIN,     "FIX 3 [JAM] HARDWARE BIN");
    moFileInfo.Add(sFIX_4_HARDWARE_BIN,         "FIX 4 HARDWARE BIN");
    moFileInfo.Add(sFIX_5_HARDWARE_BIN,         "FIX 5 HARDWARE BIN");
    moFileInfo.Add(sFIX_6_HARDWARE_BIN,         "FIX 6 HARDWARE BIN");
    moFileInfo.Add(sAUTO_1_CAT_A_PASS_FAIL,     "AUTO 1 [CAT A] PASS/FAIL");
    moFileInfo.Add(sAUTO_2_CAT_B_PASS_FAIL,     "AUTO 2 [CAT B] PASS/FAIL");
    moFileInfo.Add(sAUTO_3_CAT_C_PASS_FAIL,     "AUTO 3 [CAT C] PASS/FAIL");
    moFileInfo.Add(sFIX_1_CAT_D_PASS_FAIL,      "FIX 1 [CAT D] PASS/FAIL");
    moFileInfo.Add(sFIX_2_CAT_E_PASS_FAIL,      "FIX 2 [CAT E] PASS/FAIL");
    moFileInfo.Add(sFIX_3_JAM_PASS_FAIL,        "FIX 3 [JAM] PASS/FAIL");
    moFileInfo.Add(sFIX_4_PASS_FAIL,            "FIX 4 PASS/FAIL");
    moFileInfo.Add(sFIX_5_PASS_FAIL,            "FIX 5 PASS/FAIL");
    moFileInfo.Add(sFIX_6_PASS_FAIL,            "FIX 6 PASS/FAIL");
    moFileInfo.Add(sSPECAIL_FUNCITON,           "SPECAIL FUNCITON");
    moFileInfo.Add(sNote, "Note");
    if(AUTO_EMPTY_COLOR>=3)
    {
        moFileInfo.Add(sAUTO_4_HARDWARE_BIN,  "AUTO 4 HARDWARE BIN");
        moFileInfo.Add(sAUTO_5_HARDWARE_BIN,  "AUTO 5 HARDWARE BIN");
        moFileInfo.Add(sAUTO_6_HARDWARE_BIN,  "AUTO 6 HARDWARE BIN");
        moFileInfo.Add(sFIX_7_HARDWARE_BIN,   "FIX 7 HARDWARE BIN");
        moFileInfo.Add(sFIX_8_HARDWARE_BIN,   "FIX 8 HARDWARE BIN");
        moFileInfo.Add(sFIX_9_HARDWARE_BIN,   "FIX 9 HARDWARE BIN");
        moFileInfo.Add(sFIX_10_HARDWARE_BIN,  "FIX 10 HARDWARE BIN");
        moFileInfo.Add(sFIX_11_HARDWARE_BIN,  "FIX 11 HARDWARE BIN");
        moFileInfo.Add(sFIX_12_HARDWARE_BIN,  "FIX 12 HARDWARE BIN");
        moFileInfo.Add(sAUTO_4_PASS_FAIL,     "AUTO 4 PASS/FAIL");
        moFileInfo.Add(sAUTO_5_PASS_FAIL,     "AUTO 5 PASS/FAIL");
        moFileInfo.Add(sAUTO_6_PASS_FAIL,     "AUTO 6 PASS/FAIL");
        moFileInfo.Add(sFIX_7_PASS_FAIL,      "FIX 7 PASS/FAIL");
        moFileInfo.Add(sFIX_8_PASS_FAIL,      "FIX 8 PASS/FAIL");
        moFileInfo.Add(sFIX_9_PASS_FAIL,      "FIX 9 PASS/FAIL");
        moFileInfo.Add(sFIX_10_PASS_FAIL,     "FIX 10 PASS/FAIL");
        moFileInfo.Add(sFIX_11_PASS_FAIL,     "FIX 11 PASS/FAIL");
        moFileInfo.Add(sFIX_12_PASS_FAIL,     "FIX 12 PASS/FAIL");
    }
    if(moFileInfo.ReadFile(":",-100))
        return true;
    else
        return false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool SetTrayForm_Greatek::ReadFile(AnsiString asFileWithPath)
{
    if(asFileWithPath.Pos(".bin")==0)
        return false;
    FileInfo moFileInfo(asFileWithPath);
    moFileInfo.Add(sTRAYPN,"TRAY P/N");
    moFileInfo.Add(sXStartPos,"X Start Pos");
    moFileInfo.Add(sYStartPos,"Y Start Pos");
    moFileInfo.Add(sXPitch,"X Pitch");
    moFileInfo.Add(sYPitch,"Y Pitch");
    moFileInfo.Add(sColumnsX,"Columns X");
    moFileInfo.Add(sRowsY,"Rows Y");
    moFileInfo.Add(sXWidth,"X Width");
    moFileInfo.Add(sYHeight,"Y Height");
    moFileInfo.Add(sZThickness,"Z Thickness");
    moFileInfo.Add(Group,"Group");
    if(moFileInfo.ReadFile(":",-100))
    {
        return true;
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
void SetTrayForm_Greatek::OverWriteTrayFormData()
{
    TStringList *sList=new TStringList();
    AnsiString asTemp="";
    //Head
    asTemp="Package Type,X Start Pos,Y Start Pos,X Pitch,Y Pitch,Columns (X),Rows (Y),X Width,Y Height,Z Tray Tickness,Group,Memo,BlockNumberX,BlockNumberY,BlockPitchX,BlockPitchY,";
    sList->Add(asTemp);
    //
    asTemp=sTRAYPN+","+sXStartPos+","+sYStartPos+","+sXPitch+","+sYPitch+","+sColumnsX+","+sRowsY+","+sXWidth+","+sYHeight+","+sZThickness+","+Group;
    sList->Add(asTemp);
    //
    sList->SaveToFile(GetTrayTablePath());
    sList->Clear();
    delete sList;
}
//---------------------------------------------------------------------------
int SetTrayForm_Greatek::AddAutoDownloadTrayFormData()                                      //return index num
{
    int iret=1;
    AnsiString filePath=GetTrayTablePath();
    if(FileInfo().IsFilePathExist(filePath))
    {
        TStringList *sList=new TStringList();
        AnsiString asTemp="";
        //
        sList->LoadFromFile(filePath);
        unsigned int iSize=sList->Count;
        for(unsigned int i=1;i<iSize;i++)
        {
            if(AnalysisRowDataEqual(sList->Strings[i]))
            {
                iret=i;
                sList->Clear();
                delete sList;
                return iret;
            }
        }
        if(iSize<=0)
        {
            //Head
            asTemp="Package Type,X Start Pos,Y Start Pos,X Pitch,Y Pitch,Columns (X),Rows (Y),X Width,Y Height,Z Tray Tickness,Group,Memo,BlockNumberX,BlockNumberY,BlockPitchX,BlockPitchY,";
            sList->Add(asTemp);
        }
        asTemp=GetAutoDownloadPN()+","+sXStartPos+","+sYStartPos+","+sXPitch+","+sYPitch+","+sColumnsX+","+sRowsY+","+sXWidth+","+sYHeight+","+sZThickness+","+Group;
        sList->Add(asTemp);
        sList->SaveToFile(GetTrayTablePath());
        iret=(sList->Count-1);
        //
        sList->Clear();
        delete sList;
    }
    else
    {
        OverWriteTrayFormData();
    }
    return iret;
}
//---------------------------------------------------------------------------
bool SetTrayForm_Greatek::AnalysisRowDataEqual(AnsiString sRow)
{
    bool bret=false;
    TStringList *tsRow      =new TStringList();
    //
    tsRow->CommaText=sRow;
    AnsiString sTrayPN=tsRow->Strings[elTrayPN];
    bret=sTrayPN.Pos(GetAutoDownloadPN())>0;
    tsRow->Clear();
    delete tsRow;
    return bret;
}
//---------------------------------------------------------------------------
void SetTrayForm_Greatek::Clear()
{
    sTRAYPN = "";
    sXStartPos = "";
    sYStartPos = "";
    sXPitch = "";
    sYPitch = "";
    sColumnsX = "";
    sRowsY = "";
    sXWidth = "";
    sYHeight = "";
    sZThickness = "";
    Group = "";
}
//---------------------------------------------------------------------------
//<==
//JimmyChiu 20220303 : Add class SetUpConfiguration
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//JimmyChiu 20221226 : SetUpConfiguration HandlerMode splite to SITE MAP and BIN CATEGORY
//<==
//---------------------------------------------------------------------------
bool SetUpSiteMap::ReadFile(AnsiString asFileWithPath)
{
    if(asFileWithPath.Pos(".bin")==0)
        return false;
    FileInfo moFileInfo(asFileWithPath);
    moFileInfo.Add(sHANDLER_MODE, "HANDLER MODE");
    moFileInfo.Add(sSITE_MODE, "SITE MODE");
    moFileInfo.Add(sHANDLER_TYPE, "HANDLER TYPE");
    moFileInfo.Add(sAa,"Aa");
    moFileInfo.Add(sAb,"Ab");
    moFileInfo.Add(sAc,"Ac");
    moFileInfo.Add(sAd,"Ad");
    moFileInfo.Add(sAe,"Ae");
    moFileInfo.Add(sAf,"Af");
    moFileInfo.Add(sAg,"Ag");
    moFileInfo.Add(sAh,"Ah");
    moFileInfo.Add(sBa,"Ba");
    moFileInfo.Add(sBb,"Bb");
    moFileInfo.Add(sBc,"Bc");
    moFileInfo.Add(sBd,"Bd");
    moFileInfo.Add(sBe,"Be");
    moFileInfo.Add(sBf,"Bf");
    moFileInfo.Add(sBg,"Bg");
    moFileInfo.Add(sBh,"Bh");
    moFileInfo.Add(sCa,"Ca");
    moFileInfo.Add(sCb,"Cb");
    moFileInfo.Add(sCc,"Cc");
    moFileInfo.Add(sCd,"Cd");
    moFileInfo.Add(sCe,"Ce");
    moFileInfo.Add(sCf,"Cf");
    moFileInfo.Add(sCg,"Cg");
    moFileInfo.Add(sCh,"Ch");
    moFileInfo.Add(sDa,"Da");
    moFileInfo.Add(sDb,"Db");
    moFileInfo.Add(sDc,"Dc");
    moFileInfo.Add(sDd,"Dd");
    moFileInfo.Add(sDe,"De");
    moFileInfo.Add(sDf,"Df");
    moFileInfo.Add(sDg,"Dg");
    moFileInfo.Add(sDh,"Dh");
    moFileInfo.Add(sNote,"Note");
    if(moFileInfo.ReadFile(":",-100))
    {
        return true;
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool SetUpBinCategory::ReadFile(AnsiString asFileWithPath)
{
    if(asFileWithPath.Pos(".bin")==0)
        return false;
    FileInfo moFileInfo(asFileWithPath);
    moFileInfo.Add(sHANDLER_MODE,               "HANDLER MODE");
    moFileInfo.Add(sSITE_MODE,                  "SITE MODE");
    moFileInfo.Add(sHANDLER_TYPE,               "HANDLER TYPE");
    moFileInfo.Add(sAUTO_1_CAT_A_HARDWARE_BIN,  "AUTO 1 [CAT A] HARDWARE BIN");
    moFileInfo.Add(sAUTO_2_CAT_B_HARDWARE_BIN,  "AUTO 2 [CAT B] HARDWARE BIN");
    moFileInfo.Add(sAUTO_3_CAT_C_HARDWARE_BIN,  "AUTO 3 [CAT C] HARDWARE BIN");
    moFileInfo.Add(sFIX_1_CAT_D_HARDWARE_BIN,   "FIX 1 [CAT D] HARDWARE BIN");
    moFileInfo.Add(sFIX_2_CAT_E_HARDWARE_BIN,   "FIX 2 [CAT E] HARDWARE BIN");
    moFileInfo.Add(sFIX_3_JAM_HARDWARE_BIN,     "FIX 3 [JAM] HARDWARE BIN");
    moFileInfo.Add(sFIX_4_HARDWARE_BIN,         "FIX 4 HARDWARE BIN");
    moFileInfo.Add(sFIX_5_HARDWARE_BIN,         "FIX 5 HARDWARE BIN");
    moFileInfo.Add(sFIX_6_HARDWARE_BIN,         "FIX 6 HARDWARE BIN");
    moFileInfo.Add(sAUTO_1_CAT_A_PASS_FAIL,     "AUTO 1 [CAT A] PASS/FAIL");
    moFileInfo.Add(sAUTO_2_CAT_B_PASS_FAIL,     "AUTO 2 [CAT B] PASS/FAIL");
    moFileInfo.Add(sAUTO_3_CAT_C_PASS_FAIL,     "AUTO 3 [CAT C] PASS/FAIL");
    moFileInfo.Add(sFIX_1_CAT_D_PASS_FAIL,      "FIX 1 [CAT D] PASS/FAIL");
    moFileInfo.Add(sFIX_2_CAT_E_PASS_FAIL,      "FIX 2 [CAT E] PASS/FAIL");
    moFileInfo.Add(sFIX_3_JAM_PASS_FAIL,        "FIX 3 [JAM] PASS/FAIL");
    moFileInfo.Add(sFIX_4_PASS_FAIL,            "FIX 4 PASS/FAIL");
    moFileInfo.Add(sFIX_5_PASS_FAIL,            "FIX 5 PASS/FAIL");
    moFileInfo.Add(sFIX_6_PASS_FAIL,            "FIX 6 PASS/FAIL");
    moFileInfo.Add(sSPECAIL_FUNCITON,"SPECAIL FUNCITON");
    moFileInfo.Add(sNote,"Note");

    if(AUTO_EMPTY_COLOR>=3)
    {
        moFileInfo.Add(sAUTO_4_HARDWARE_BIN,    "AUTO 4 HARDWARE BIN");
        moFileInfo.Add(sAUTO_5_HARDWARE_BIN,    "AUTO 5 HARDWARE BIN");
        moFileInfo.Add(sAUTO_6_HARDWARE_BIN,    "AUTO 6 HARDWARE BIN");
        moFileInfo.Add(sFIX_7_HARDWARE_BIN,     "FIX 7 HARDWARE BIN");
        moFileInfo.Add(sFIX_8_HARDWARE_BIN,     "FIX 8 HARDWARE BIN");
        moFileInfo.Add(sFIX_9_HARDWARE_BIN,     "FIX 9 HARDWARE BIN");
        moFileInfo.Add(sFIX_10_HARDWARE_BIN,    "FIX 10 HARDWARE BIN");
        moFileInfo.Add(sFIX_11_HARDWARE_BIN,    "FIX 11 HARDWARE BIN");
        moFileInfo.Add(sFIX_12_HARDWARE_BIN,    "FIX 12 HARDWARE BIN");
        moFileInfo.Add(sAUTO_4_PASS_FAIL,       "AUTO 4 PASS/FAIL");
        moFileInfo.Add(sAUTO_5_PASS_FAIL,       "AUTO 5 PASS/FAIL");
        moFileInfo.Add(sAUTO_6_PASS_FAIL,       "AUTO 6 PASS/FAIL");
        moFileInfo.Add(sFIX_7_PASS_FAIL,        "FIX 7 PASS/FAIL");
        moFileInfo.Add(sFIX_8_PASS_FAIL,        "FIX 8 PASS/FAIL");
        moFileInfo.Add(sFIX_9_PASS_FAIL,        "FIX 9 PASS/FAIL");
        moFileInfo.Add(sFIX_10_PASS_FAIL,       "FIX 10 PASS/FAIL");
        moFileInfo.Add(sFIX_11_PASS_FAIL,       "FIX 11 PASS/FAIL");
        moFileInfo.Add(sFIX_12_PASS_FAIL,       "FIX 12 PASS/FAIL");
    }

    if(moFileInfo.ReadFile(":",-100))
    {
        return true;
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
//<==
//JimmyChiu 20221226 : SetUpConfiguration HandlerMode splite to SITE MAP and BIN CATEGORY
//---------------------------------------------------------------------------
