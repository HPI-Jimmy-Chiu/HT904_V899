// =============================================================================
//  ProductionInfo/uSetUpConfiguration.h  --  ".bin" setup-file value classes
//                                            (SITE MAP / BIN CATEGORY / Tray).
//
//  Faithful translation of golden ProductionInfo/uSetUpConfiguration.h
//  (233 lines, BCB6, Big5/cp950).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 ("prodinfo" group), 3 of 3 units (after
//  ProductionInfo/FileInfo.{h,cpp} and ProductionInfo/TfFTP.{h,cpp}).
//
//  ROLE: three plain value-holder classes read from FileInfo's ':'-delimited
//  flat "HANDLER MODE:xxx" .bin files (golden comment: "JimmyChiu 20220303 :
//  Add class SetUpConfiguration" / "JimmyChiu 20221226 : SetUpConfiguration
//  HandlerMode splite to SITE MAP and BIN CATEGORY"):
//    SetUpConfiguration -- the ORIGINAL combined (site-map + bin-category)
//                          shape, kept for backward compatibility (golden's
//                          own "<==" / "<==" bracketing comments mark this as
//                          the pre-split legacy class).
//    SetUpSiteMap       -- the SITE MAP half of the 20221226 split (Aa..Dh
//                          site-to-category letters + HANDLER/SITE MODE).
//    SetUpBinCategory   -- the BIN CATEGORY half (AUTO/FIX hardware-bin +
//                          pass/fail mapping).
//    SetTrayForm_Greatek -- unrelated Greatek-specific tray-geometry CSV
//                          row reader/writer (golden's own "<==" bracket
//                          places it INSIDE the SetUpConfiguration comment
//                          pair, but its own fields/behaviour are entirely
//                          separate from all three SetUp* classes above).
//
//  WAVE SCOPE -- ACTIVE (faithful, verbatim -- all 8 golden .cpp function
//    bodies; see uSetUpConfiguration.cpp's own banner for the golden-line
//    inventory). Every field in all 4 classes is a plain AnsiString (or, for
//    SetTrayForm_Greatek, one private enum) -- nothing here needs a gate.
//
//  SATISFIED-BY-SUBSTRATE: FileInfo/ParameterInfo (ProductionInfo/FileInfo.h,
//    this wave's own sibling, written first) supplies every ReadFile/Add/
//    PathCombin/IsFilePathExist call this header's classes' bodies make.
//
//  VCL/Borland conversions: `#include <StdCtrls.hpp>` (golden :6) dropped --
//  nothing in this header needs a VCL control type (every member is
//  AnsiString or int); golden's own include is unused dead weight already
//  (not one function signature here touches a StdCtrls.hpp type). No
//  __fastcall / __property / __published in this header (golden has none).
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================
// BCB6 ORIGINAL include block (mirrored as a comment for provenance):
//   #include <StdCtrls.hpp>
// =============================================================================
#ifndef ProductionInfo_uSetUpConfigurationH
#define ProductionInfo_uSetUpConfigurationH
//---------------------------------------------------------------------------
#include "vclcompat/vcl_compat.h"    // AnsiString
//---------------------------------------------------------------------------
//JimmyChiu 20220303 : Add class SetUpConfiguration
//<==
class SetUpConfiguration
{
private:                                                                        // User declarations
public:                                                                         // User declarations
    AnsiString sHANDLER_MODE;
    AnsiString sSITE_MODE;
    AnsiString sHANDLER_TYPE;
    AnsiString sAa;
    AnsiString sAb;
    AnsiString sAc;
    AnsiString sAd;
    AnsiString sAe;
    AnsiString sAf;
    AnsiString sAg;
    AnsiString sAh;
    AnsiString sBa;
    AnsiString sBb;
    AnsiString sBc;
    AnsiString sBd;
    AnsiString sBe;
    AnsiString sBf;
    AnsiString sBg;
    AnsiString sBh;
    AnsiString sCa;
    AnsiString sCb;
    AnsiString sCc;
    AnsiString sCd;
    AnsiString sCe;
    AnsiString sCf;
    AnsiString sCg;
    AnsiString sCh;
    AnsiString sDa;
    AnsiString sDb;
    AnsiString sDc;
    AnsiString sDd;
    AnsiString sDe;
    AnsiString sDf;
    AnsiString sDg;
    AnsiString sDh;
    AnsiString sAUTO_1_CAT_A_HARDWARE_BIN;
    AnsiString sAUTO_2_CAT_B_HARDWARE_BIN;
    AnsiString sAUTO_3_CAT_C_HARDWARE_BIN;
    AnsiString sAUTO_4_HARDWARE_BIN;
    AnsiString sAUTO_5_HARDWARE_BIN;
    AnsiString sAUTO_6_HARDWARE_BIN;
    AnsiString sFIX_1_CAT_D_HARDWARE_BIN;
    AnsiString sFIX_2_CAT_E_HARDWARE_BIN;
    AnsiString sFIX_3_JAM_HARDWARE_BIN;
    AnsiString sFIX_4_HARDWARE_BIN;
    AnsiString sFIX_5_HARDWARE_BIN;
    AnsiString sFIX_6_HARDWARE_BIN;
    AnsiString sFIX_7_HARDWARE_BIN;
    AnsiString sFIX_8_HARDWARE_BIN;
    AnsiString sFIX_9_HARDWARE_BIN;
    AnsiString sFIX_10_HARDWARE_BIN;
    AnsiString sFIX_11_HARDWARE_BIN;
    AnsiString sFIX_12_HARDWARE_BIN;
    AnsiString sAUTO_1_CAT_A_PASS_FAIL;
    AnsiString sAUTO_2_CAT_B_PASS_FAIL;
    AnsiString sAUTO_3_CAT_C_PASS_FAIL;
    AnsiString sAUTO_4_PASS_FAIL;
    AnsiString sAUTO_5_PASS_FAIL;
    AnsiString sAUTO_6_PASS_FAIL;
    AnsiString sFIX_1_CAT_D_PASS_FAIL;
    AnsiString sFIX_2_CAT_E_PASS_FAIL;
    AnsiString sFIX_3_JAM_PASS_FAIL;
    AnsiString sFIX_4_PASS_FAIL;
    AnsiString sFIX_5_PASS_FAIL;
    AnsiString sFIX_6_PASS_FAIL;
    AnsiString sFIX_7_PASS_FAIL;
    AnsiString sFIX_8_PASS_FAIL;
    AnsiString sFIX_9_PASS_FAIL;
    AnsiString sFIX_10_PASS_FAIL;
    AnsiString sFIX_11_PASS_FAIL;
    AnsiString sFIX_12_PASS_FAIL;
    AnsiString sSPECAIL_FUNCITON;
    AnsiString sNote;
    //function
    bool ReadSetUpFile(AnsiString asFileWithPath);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class SetTrayForm_Greatek
{
private:                                                                        // User declarations
    enum eTrayFormData
    {
        elTrayPN      =0,     // Tray 料號 (Part Number)
        elXStartPos,          // X 軸起始位置
        elYStartPos,          // Y 軸起始位置
        elXPitch,             // X 軸間距
        elYPitch,             // Y 軸間距
        elColumnsX,           // X 軸欄位數 (列數)
        elRowsY,              // Y 軸列數 (行數)
        elXWidth,             // X 軸寬度
        elYHeight,            // Y 軸高度
        elZThickness,         // Z 軸厚度
        elGroup,              // 群組
        elTrayFormDataCount   // 用於計算列舉項目總數
    };
public:                                                                         // User declarations
    AnsiString sTRAYPN;
    AnsiString sXStartPos;
    AnsiString sYStartPos;
    AnsiString sXPitch;
    AnsiString sYPitch;
    AnsiString sColumnsX;
    AnsiString sRowsY;
    AnsiString sXWidth;
    AnsiString sYHeight;
    AnsiString sZThickness;
    AnsiString Group;
    //function
    bool ReadFile(AnsiString asFileWithPath);
    AnsiString GetTrayTablePath(){return "D:\\HT9045\\System\\TrayForm.csv";}
    AnsiString GetAutoDownloadPN(){return "AutoDownload";}
    int AddAutoDownloadTrayFormData();                                                      //return index num
    void OverWriteTrayFormData();
    void Clear();
    bool AnalysisRowDataEqual(AnsiString sRow);
};
//<==
//JimmyChiu 20220303 : Add class SetUpConfiguration
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//JimmyChiu 20221226 : SetUpConfiguration HandlerMode splite to SITE MAP and BIN CATEGORY
//<==
//---------------------------------------------------------------------------
class SetUpSiteMap
{
private:                                                                        // User declarations
public:                                                                         // User declarations
    AnsiString sHANDLER_MODE;
    AnsiString sSITE_MODE;
    AnsiString sHANDLER_TYPE;
    AnsiString sAa;
    AnsiString sAb;
    AnsiString sAc;
    AnsiString sAd;
    AnsiString sAe;
    AnsiString sAf;
    AnsiString sAg;
    AnsiString sAh;
    AnsiString sBa;
    AnsiString sBb;
    AnsiString sBc;
    AnsiString sBd;
    AnsiString sBe;
    AnsiString sBf;
    AnsiString sBg;
    AnsiString sBh;
    AnsiString sCa;
    AnsiString sCb;
    AnsiString sCc;
    AnsiString sCd;
    AnsiString sCe;
    AnsiString sCf;
    AnsiString sCg;
    AnsiString sCh;
    AnsiString sDa;
    AnsiString sDb;
    AnsiString sDc;
    AnsiString sDd;
    AnsiString sDe;
    AnsiString sDf;
    AnsiString sDg;
    AnsiString sDh;
    AnsiString sNote;
    //function
    bool ReadFile(AnsiString asFileWithPath);
};
//---------------------------------------------------------------------------
class SetUpBinCategory
{
private:                                                                        // User declarations
public:                                                                         // User declarations
    AnsiString sHANDLER_MODE;
    AnsiString sSITE_MODE;
    AnsiString sHANDLER_TYPE;
    AnsiString sAUTO_1_CAT_A_HARDWARE_BIN;
    AnsiString sAUTO_2_CAT_B_HARDWARE_BIN;
    AnsiString sAUTO_3_CAT_C_HARDWARE_BIN;
    AnsiString sAUTO_4_HARDWARE_BIN;
    AnsiString sAUTO_5_HARDWARE_BIN;
    AnsiString sAUTO_6_HARDWARE_BIN;
    AnsiString sFIX_1_CAT_D_HARDWARE_BIN;
    AnsiString sFIX_2_CAT_E_HARDWARE_BIN;
    AnsiString sFIX_3_JAM_HARDWARE_BIN;
    AnsiString sFIX_4_HARDWARE_BIN;
    AnsiString sFIX_5_HARDWARE_BIN;
    AnsiString sFIX_6_HARDWARE_BIN;
    AnsiString sFIX_7_HARDWARE_BIN;
    AnsiString sFIX_8_HARDWARE_BIN;
    AnsiString sFIX_9_HARDWARE_BIN;
    AnsiString sFIX_10_HARDWARE_BIN;
    AnsiString sFIX_11_HARDWARE_BIN;
    AnsiString sFIX_12_HARDWARE_BIN;
    AnsiString sAUTO_1_CAT_A_PASS_FAIL;
    AnsiString sAUTO_2_CAT_B_PASS_FAIL;
    AnsiString sAUTO_3_CAT_C_PASS_FAIL;
    AnsiString sAUTO_4_PASS_FAIL;
    AnsiString sAUTO_5_PASS_FAIL;
    AnsiString sAUTO_6_PASS_FAIL;
    AnsiString sFIX_1_CAT_D_PASS_FAIL;
    AnsiString sFIX_2_CAT_E_PASS_FAIL;
    AnsiString sFIX_3_JAM_PASS_FAIL;
    AnsiString sFIX_4_PASS_FAIL;
    AnsiString sFIX_5_PASS_FAIL;
    AnsiString sFIX_6_PASS_FAIL;
    AnsiString sFIX_7_PASS_FAIL;
    AnsiString sFIX_8_PASS_FAIL;
    AnsiString sFIX_9_PASS_FAIL;
    AnsiString sFIX_10_PASS_FAIL;
    AnsiString sFIX_11_PASS_FAIL;
    AnsiString sFIX_12_PASS_FAIL;
    AnsiString sSPECAIL_FUNCITON;
    AnsiString sNote;
    //function
    bool ReadFile(AnsiString asFileWithPath);
};
//---------------------------------------------------------------------------
//<==
//JimmyChiu 20221226 : SetUpConfiguration HandlerMode splite to SITE MAP and BIN CATEGORY
#endif
