// =============================================================================
//  ProductionInfo/uSetUpConfiguration.cpp  --  implementation of the 4
//                                              uSetUpConfiguration.h classes.
//
//  Faithful translation of golden ProductionInfo/uSetUpConfiguration.cpp
//  (339 lines, BCB6, Big5/cp950).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 ("prodinfo" group), 3 of 3 units.  See
//  uSetUpConfiguration.h's banner for the class-level role summary.
//
//  WAVE SCOPE -- ACTIVE (faithful, verbatim, all 8 golden function bodies):
//    SetUpConfiguration::ReadSetUpFile           golden :15-101
//    SetTrayForm_Greatek::ReadFile                golden :104-128
//    SetTrayForm_Greatek::OverWriteTrayFormData   golden :130-144
//    SetTrayForm_Greatek::AddAutoDownloadTrayFormData golden :146-187
//    SetTrayForm_Greatek::AnalysisRowDataEqual    golden :189-200
//    SetTrayForm_Greatek::Clear                   golden :202-215
//    SetUpSiteMap::ReadFile                       golden :224-273
//    SetUpBinCategory::ReadFile                   golden :276-335
//  8 of 8 golden functions get a real body.  ZERO stubs.  ZERO gates --
//  every call this file makes (FileInfo::Add/ReadFile/IsFilePathExist,
//  TStringList::LoadFromFile/SaveToFile/CommaText/Strings[]/Count/Clear) is
//  already real, either translated in this wave's own FileInfo.cpp sibling
//  or pre-existing in vclcompat.
//
//  DEPENDENCY NOTE: every one of the 4 classes' ReadFile bodies constructs a
//  local `FileInfo moFileInfo(asFileWithPath);` and calls
//  `moFileInfo.ReadFile(":",-100)` -- the idecode=-100 branch of
//  FileInfo::ReadFile(AnsiString,int) (this wave's own ProductionInfo/
//  FileInfo.cpp, MINIMAL NECESSARY ADAPTATION (a) in that file's banner).
//  This is the CONCRETE real-world caller that adaptation's banner note
//  references -- confirmed here, not a hypothetical.
//
//  VCL/Borland conversions: #pragma hdrstop + #pragma package(smart_init)
//  dropped (golden :3 and :10). No __fastcall / __property / __published in
//  this .cpp (golden has none). SOFT_SIMULTE is NOT defined and golden does
//  not reference it in this unit. Numeric semantics kept EXACT (no int/float
//  rewrites).
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8, in
//  its original column. Final gate: ZERO U+FFFD.
// =============================================================================

// BCB6 ORIGINAL include block (mirrored as a comment for provenance):
//   #include "MachineDefine.h" ; #pragma hdrstop
//   #include "uSetUpConfiguration.h" ; #include "FileInfo.h"
//   #pragma package(smart_init)
//   extern int  AUTO_EMPTY_COLOR;
// =============================================================================

#include "MachineDefine.h"                       // de-VCL'd include hub: vclcompat umbrella +
                                                  //   TStringList, `using namespace std;`
#include "ProductionInfo/uSetUpConfiguration.h"
#include "ProductionInfo/FileInfo.h"

//------------------------------------------------------------------------------

// golden's own bare local extern (cmydef.h:2919 already declares the SAME
// `extern int AUTO_EMPTY_COLOR;`, defined for real in cmydef.cpp -- this
// local redeclaration is golden's own literal spelling, kept verbatim so
// this TU's include list matches golden's exactly; no ODR conflict, both
// declarations agree in type).
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
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
//<==
//JimmyChiu 20220303 : Add class SetUpConfiguration
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//JimmyChiu 20221226 : SetUpConfiguration HandlerMode splite to SITE MAP and BIN CATEGORY
//<==
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
//<==
//JimmyChiu 20221226 : SetUpConfiguration HandlerMode splite to SITE MAP and BIN CATEGORY
//------------------------------------------------------------------------------
