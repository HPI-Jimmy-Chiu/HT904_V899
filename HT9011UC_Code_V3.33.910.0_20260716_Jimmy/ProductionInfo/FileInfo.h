//---------------------------------------------------------------------------

#ifndef FileInfoH
#define FileInfoH
//---------------------------------------------------------------------------
#include <StdCtrls.hpp>
#include <map>
#include <vector>
//---------------------------------------------------------------------------
//#include <DBTables.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class ParameterInfo
{
public:     // User declarations
    AnsiString* asParaValue;
    AnsiString asFileParaName;
    ParameterInfo(AnsiString &_asParaValue,AnsiString _asFileParaName);                 //Jimmychiu 20220830 add ParameterInfo Class
    ~ParameterInfo();
};
//---------------------------------------------------------------------------
class FileInfo                                                                          //JimmyChiu 20220122 add FileInfo class
{
private:    // User declarations
    AnsiString asFilePath;
    std::map<int,ParameterInfo*> mapGroup;
public:     // User declarations
    AnsiString GetFilePath(){return asFilePath;};
    FileInfo(AnsiString FilePath);                                                      //Jimmychiu 20220830 add FileInfo Class
    FileInfo(){}
    ~FileInfo();
    void Add(AnsiString &_asParaValue, AnsiString _asFileParaName)                      //Jimmychiu 20220830 解碼並讀取資料
    {
        mapGroup[GetSize()]=new ParameterInfo(_asParaValue,_asFileParaName);
    }
    int GetSize(){return mapGroup.size();}
    bool ReadFile(AnsiString sSeparatedValues);
    bool ReadFile(AnsiString sSeparatedValues,int idecode);                             //Jimmychiu 20220830 解碼並讀取資料
    bool WriteFile(AnsiString sSeparatedValues);                                        //Jimmychiu 20220830 解碼並讀取資料
    bool IsFilePathExist(AnsiString sPath);                                                             //Jimmychiu 20220830 解碼並讀取資料
    AnsiString DecodeASCII(char *cStream,int iLen,int iTransfor);                       //Jimmychiu 20220830 解碼並讀取資料
    AnsiString DecodeASCII(AnsiString asFileName,int iTransfor);                        //Jimmychiu 20220830 解碼並讀取資料
    void DecodeReadMap2Parameter(AnsiString asLineValue,AnsiString sSeparatedValues);   //Jimmychiu 20220830 解碼並讀取資料
    void SaveAsTxtFile(AnsiString asContent,AnsiString asNewFileWithPath);                      //Jimmychiu 20230223 帳密權限加密，提升資訊安全
    void GetNameAndExtension(AnsiString asFileName, AnsiString &asFile, AnsiString &asExtension); //Jimmychiu 20230223 帳密權限加密，提升資訊安全
    std::vector<AnsiString> GetAllFileNamesInFolder(AnsiString folderPath,int &iErr);   //Jimmychiu 20240224 : Get all file name in the folder
    std::vector<AnsiString> GetAllFolderNamesInFolder(AnsiString folderPath,int &iErr); //Jimmychiu 20250715 : Get all folder name in the folder
    bool DeleteFolderContents(AnsiString folderPath,int &iErr);                         //Jimmychiu 20240224 : Delete Folder Contents
    void SplitPath(const AnsiString& fullPath, AnsiString& path, AnsiString& filename, AnsiString& extension);    //Jimmychiu 20240318 : add Split path function
    bool DirectoryExist(AnsiString folderPath);
    AnsiString RemoveAllTrailingBackslashes(const AnsiString& input);
    AnsiString PathCombin(const AnsiString& sPath, const AnsiString& sFile);
    void EnsureDirectoriesExist(AnsiString sPath);
    AnsiString GetNewLine(){return "\n";}
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------   //Jimmychiu 20240318 : add read write Paradox DB
class cDBTStringGrid;
class cDatabaseMin
{
private:
public:     // User declarations
    cDatabaseMin();
    ~cDatabaseMin();
    virtual void Clear(){}
    virtual bool LoadFile(AnsiString dbpath,AnsiString dbName);
    virtual void SaveFile();
    virtual void CloseFile(){}
};
//---------------------------------------------------------------------------
class cDBTStringGrid
{
private:
    TStringGrid *sg;
    int iRowHeight;
    void SetCell(int iRow,int iCol,const AnsiString str);
    AnsiString GetCell(int iRow,int iCol);
    void SetColTitle(int iCol,const AnsiString str);
    AnsiString GetColTitle(int iCol);
    void SetRowTitle(int iRow,AnsiString str);
    AnsiString GetRowTitle(int iRow);
public:     // User declarations
    cDBTStringGrid(TStringGrid* _sg);
    ~cDBTStringGrid();
    void Clear();
    void SetRowCol(int iRow,int iCol);
    void GetRowCol(int &iRow,int &iCol);
    int GetRowCount(){return sg->RowCount-1;}
    int GetColCount(){return sg->ColCount-1;}
    __property AnsiString ColTitle[int iCol]={read=GetColTitle, write=SetColTitle};
    __property AnsiString RowTitle[int iRow]={read=GetRowTitle, write=SetRowTitle};
    __property AnsiString Cells[int iCol][int iRow]={read=GetCell, write=SetCell};
    void AddRow();
    void AddCol();
    void CloneTo(TStringGrid* destination);
    void ClearRow(int iRow);
    void HideRow(bool bOn,int iIndex);
};
//---------------------------------------------------------------------------
class cMathTool
{
private:
public:     // User declarations
    cMathTool();
    ~cMathTool();
    void Clear();
    bool IsDoubleEqual(const double a,const double b,const double tolerance=1e-6);
};
//---------------------------------------------------------------------------
#endif
