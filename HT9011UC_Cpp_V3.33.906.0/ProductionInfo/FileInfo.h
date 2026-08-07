// =============================================================================
//  ProductionInfo/FileInfo.h  --  file/parameter-encoding helpers used by the
//                                 SetUpConfiguration family and by TfFTP.
//
//  Faithful translation of golden ProductionInfo/FileInfo.h (111 lines, BCB6,
//  Big5/cp950).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 ("prodinfo" group), 1 of 3 units
//  (ProductionInfo/TfFTP.{h,cpp}, ProductionInfo/FileInfo.{h,cpp},
//  ProductionInfo/uSetUpConfiguration.{h,cpp}).  Written FIRST of the three
//  despite the group's nominal TfFTP-first ordering, because TfFTP.cpp calls
//  `FileInfo().PathCombin(...)` (golden TfFTP.cpp:333/334/516) -- this header
//  is a compile-time dependency of TfFTP.cpp, not merely a sibling.
//
//  ROLE: five small, mostly VCL-free value classes:
//    ParameterInfo   -- one (AnsiString* target, AnsiString key) binding used
//                       by FileInfo's "Add(target, key)" + ReadFile map.
//    FileInfo        -- key=value flat-file reader/writer (the ':'-delimited
//                       "HANDLER MODE:xxx" .bin format SetUpConfiguration.cpp
//                       reads) + small path/folder utility grab-bag
//                       (PathCombin/EnsureDirectoriesExist/GetAllFileNamesInFolder/...).
//    cDatabaseMin    -- tiny virtual stub base (LoadFile always true,
//                       SaveFile/Clear/CloseFile no-ops in golden itself --
//                       not something this wave weakens).
//    cDBTStringGrid  -- 1-based-row/col-with-title wrapper around a
//                       TStringGrid* (row 0 / col 0 reserved for titles).
//    cMathTool       -- one helper, IsDoubleEqual.
//
//  WAVE SCOPE -- ACTIVE vs satisfied-by-shim:
//   ACTIVE (faithful, verbatim -- all 45 golden FileInfo.cpp function bodies;
//     see FileInfo.cpp's own banner for the full golden-line inventory).
//   SATISFIED-BY-SUBSTRATE: vclcompat::TStringGrid (vclcompat/StringGrid.h,
//     already real) stands in for golden's VCL Grids.hpp TStringGrid.
//     vclcompat::TStringList/AnsiString/SysUtils (FileExists/DirectoryExists/
//     DeleteFile/ExtractFilePath/ExtractFileExt/CreateDir, all already real)
//     stand in for golden's SysUtils.hpp.  TSearchRec/FindFirst/FindNext/
//     FindClose/faAnyFile/faDirectory (vclcompat/SysUtils.h) stand in for
//     golden's own of the same names.
//
//  GATE REGISTER -- exactly TWO gates, both in cDBTStringGrid, both because
//  vclcompat::TStringGrid's OWN file banner (StringGrid.h:18-26) explicitly
//  scopes it to "headless Cells[ACol][ARow] + growable RowCount/ColCount. NO
//  rendering, NO FixedRows/FixedCols/Options/OnDrawCell/OnSelectCell/
//  ColWidths/RowHeights" -- i.e. these are DECLARED-NOWHERE members, not
//  merely bodyless functions, and vclcompat/StringGrid.h is a pre-existing
//  file this wave may not edit (HARD RULE 7):
//   (1) `sg->DefaultColWidth=100; sg->ColWidths[0]=30;`  golden :418-419
//       (cDBTStringGrid::SetRowCol).  WHY dropping is faithful: both are pure
//       column-RENDERING hints (pixel widths) with no effect on Cells/
//       RowCount/ColCount data -- this class's own Clear/GetCell/SetCell/
//       AddRow/AddCol/CloneTo/ClearRow never read a column width back. No
//       consumer of cDBTStringGrid exists anywhere in the port tree yet (this
//       is its first home), so there is no observable behaviour delta for
//       THIS wave; a future UI-facing wave that needs actual grid rendering
//       should extend vclcompat/StringGrid.h additively (that header's own
//       file banner already anticipates this) and then lift this gate.
//   (2) `sg->RowHeights[row]=iRowH;` (twice) golden :585/:590
//       (cDBTStringGrid::HideRow).  Same reasoning as (1) -- a pure rendering
//       hint (0 to visually hide a row, iRowHeight to restore it) that no
//       headless Cells/RowCount/ColCount reader can observe.
//
//  __property TRANSLATION (the one non-mechanical decision in this header,
//  same "flag for review" posture as Public/MyStringList.h's own property
//  note): golden declares 3 indexed read/write properties on cDBTStringGrid
//  (golden FileInfo.h:91-93):
//    __property AnsiString ColTitle[int iCol]={read=GetColTitle, write=SetColTitle};
//    __property AnsiString RowTitle[int iRow]={read=GetRowTitle, write=SetRowTitle};
//    __property AnsiString Cells[int iCol][int iRow]={read=GetCell, write=SetCell};
//  Unlike MyStringList.h's scalar properties, these are INDEXED (2 of them
//  double-indexed) -- BCB6's indexed-property sugar has no direct C++
//  equivalent short of building a dedicated 2-D proxy (the kind
//  vclcompat::TStringGrid's OWN Cells/ColAccessor already is, one layer
//  down). Building a second such proxy here was judged NOT worth it THIS
//  wave: cDBTStringGrid has ZERO consumers anywhere in the port tree (this is
//  its first translated home) and none of the 45 functions THIS wave
//  translates (all live in FileInfo/TfFTP/SetUpConfiguration, none of which
//  touch cDBTStringGrid's properties) ever spells `->ColTitle[i]=`/
//  `->RowTitle[i]=`/`->Cells[i][j]=` -- every one of the 6 underlying bodies
//  is called by cDBTStringGrid's OWN other methods, always through the plain
//  method name (SetCell/GetCell/...), never through property syntax. The
//  three __property lines are kept ABOVE as verbatim comments (reverse-
//  lookupable) and the 6 accessor methods are promoted from golden's
//  `private:` to `public:` (the only way to preserve their EXTERNAL callable
//  surface once the property sugar that used to expose them is gone) --
//  BEHAVIOUR IS UNCHANGED (golden's own method bodies are translated
//  verbatim in the .cpp), only the C++-level SPELLING a future caller would
//  use differs (`->SetCell(r,c,s)` instead of `->Cells[c][r]=s`). Flagged for
//  a human/future wave: if a later unit needs the bracket syntax, add a small
//  ColTitleProxy/RowTitleProxy/CellsProxy trio here (same idiom as
//  vclcompat::TStringGrid's own ColAccessor) rather than re-deciding this.
//
//  VCL/Borland conversions: <StdCtrls.hpp>/<Grids.hpp>/<DBTables.hpp> (golden
//  :6,10-11) dropped; replaced by vclcompat/StringGrid.h's TStringGrid. No
//  __fastcall / __published in this header (golden has none either).
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================
// BCB6 ORIGINAL include block (mirrored as a comment for provenance):
//   #include <StdCtrls.hpp> ; #include <map> ; #include <vector>
//   //#include <DBTables.hpp> (already commented out in golden)
//   #include <Grids.hpp>
// =============================================================================
#ifndef ProductionInfo_FileInfoH
#define ProductionInfo_FileInfoH

#include "vclcompat/vcl_compat.h"    // AnsiString, TStringList, FileExists/
                                     //   DirectoryExists/DeleteFile/ExtractFilePath/
                                     //   ExtractFileExt/CreateDir, TSearchRec/
                                     //   FindFirst/FindNext/FindClose/faAnyFile/faDirectory
#include "vclcompat/StringGrid.h"    // vclcompat::TStringGrid -- cDBTStringGrid's sg member
#include <map>
#include <vector>

using vclcompat::TStringGrid;        // golden spells it bare (Grids.hpp); see
                                     //   SECSGEM/uHGemEquipment.h:324 for the
                                     //   established "collision-free, grepped" precedent

//------------------------------------------------------------------------------
class ParameterInfo
{
public:                                                                         // User declarations
    AnsiString* asParaValue;
    AnsiString asFileParaName;
    ParameterInfo(AnsiString &_asParaValue, AnsiString _asFileParaName);        //Jimmychiu 20220830 add ParameterInfo Class
    ~ParameterInfo();
};
//------------------------------------------------------------------------------
class FileInfo                                                                  //JimmyChiu 20220122 add FileInfo class
{
private:                                                                        // User declarations
    AnsiString asFilePath;
    std::map<int,ParameterInfo*> mapGroup;
public:                                                                         // User declarations
    AnsiString GetFilePath(){return asFilePath;};
    FileInfo(AnsiString FilePath);                                             //Jimmychiu 20220830 add FileInfo Class
    FileInfo(){}
    ~FileInfo();
    void Add(AnsiString &_asParaValue, AnsiString _asFileParaName)             //Jimmychiu 20220830 解碼並讀取資料
    {
        mapGroup[GetSize()]=new ParameterInfo(_asParaValue,_asFileParaName);
    }
    int GetSize(){return mapGroup.size();}
    bool ReadFile(AnsiString sSeparatedValues);
    bool ReadFile(AnsiString sSeparatedValues,int idecode);                     //Jimmychiu 20220830 解碼並讀取資料
    bool WriteFile(AnsiString sSeparatedValues);                                //Jimmychiu 20220830 解碼並讀取資料
    bool IsFilePathExist(AnsiString sPath);                                                             //Jimmychiu 20220830 解碼並讀取資料
    AnsiString DecodeASCII(char *cStream,int iLen,int iTransfor);              //Jimmychiu 20220830 解碼並讀取資料
    AnsiString DecodeASCII(AnsiString asFileName,int iTransfor);               //Jimmychiu 20220830 解碼並讀取資料
    void DecodeReadMap2Parameter(AnsiString asLineValue,AnsiString sSeparatedValues);   //Jimmychiu 20220830 解碼並讀取資料
    void SaveAsTxtFile(AnsiString asContent,AnsiString asNewFileWithPath);              //Jimmychiu 20230223 帳密權限加密，提升資訊安全
    void GetNameAndExtension(AnsiString asFileName, AnsiString &asFile, AnsiString &asExtension); //Jimmychiu 20230223 帳密權限加密，提升資訊安全
    std::vector<AnsiString> GetAllFileNamesInFolder(AnsiString folderPath,int &iErr);   //Jimmychiu 20240224 : Get all file name in the folder
    std::vector<AnsiString> GetAllFolderNamesInFolder(AnsiString folderPath,int &iErr); //Jimmychiu 20250715 : Get all folder name in the folder
    bool DeleteFolderContents(AnsiString folderPath,int &iErr);                //Jimmychiu 20240224 : Delete Folder Contents
    void SplitPath(const AnsiString& fullPath, AnsiString& path, AnsiString& filename, AnsiString& extension);    //Jimmychiu 20240318 : add Split path function
    bool DirectoryExist(AnsiString folderPath);
    AnsiString RemoveAllTrailingBackslashes(const AnsiString& input);
    AnsiString PathCombin(const AnsiString& sPath, const AnsiString& sFile);
    void EnsureDirectoriesExist(AnsiString sPath);
    AnsiString GetNewLine(){return "\n";}
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------   //Jimmychiu 20240318 : add read write Paradox DB
class cDBTStringGrid;
class cDatabaseMin
{
private:
public:                                                                         // User declarations
    cDatabaseMin();
    ~cDatabaseMin();
    virtual void Clear(){}
    virtual bool LoadFile(AnsiString dbpath,AnsiString dbName);
    virtual void SaveFile();
    virtual void CloseFile(){}
};
//------------------------------------------------------------------------------
class cDBTStringGrid
{
private:
    TStringGrid *sg;
    int iRowHeight;
public:                                                                         // User declarations
    // AI(W906-PT-W2) 20260807: promoted from golden `private:` to `public:` --
    // see the header banner's "__property TRANSLATION" note. Golden's own
    // 3 indexed properties are kept as comments immediately below for
    // reverse lookup.
    void SetCell(int iRow,int iCol,const AnsiString str);
    AnsiString GetCell(int iRow,int iCol);
    void SetColTitle(int iCol,const AnsiString str);
    AnsiString GetColTitle(int iCol);
    void SetRowTitle(int iRow,AnsiString str);
    AnsiString GetRowTitle(int iRow);
    cDBTStringGrid(TStringGrid* _sg);
    ~cDBTStringGrid();
    void Clear();
    void SetRowCol(int iRow,int iCol);
    void GetRowCol(int &iRow,int &iCol);
    int GetRowCount(){return sg->RowCount-1;}
    int GetColCount(){return sg->ColCount-1;}
    // __property AnsiString ColTitle[int iCol]={read=GetColTitle, write=SetColTitle};
    // __property AnsiString RowTitle[int iRow]={read=GetRowTitle, write=SetRowTitle};
    // __property AnsiString Cells[int iCol][int iRow]={read=GetCell, write=SetCell};
    void AddRow();
    void AddCol();
    void CloneTo(TStringGrid* destination);
    void ClearRow(int iRow);
    void HideRow(bool bOn,int iIndex);
};
//------------------------------------------------------------------------------
class cMathTool
{
private:
public:                                                                         // User declarations
    cMathTool();
    ~cMathTool();
    void Clear();
    bool IsDoubleEqual(const double a,const double b,const double tolerance=1e-6);
};
//------------------------------------------------------------------------------
#endif // ProductionInfo_FileInfoH
