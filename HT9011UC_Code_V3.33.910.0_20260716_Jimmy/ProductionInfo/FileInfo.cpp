#include "MachineDefine.h"
#pragma hdrstop

#include "FileInfo.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
using namespace std;
//---------------------------------------------------------------------------
ParameterInfo::ParameterInfo(AnsiString &_asParaValue, AnsiString _asFileName)  //Jimmychiu 20220830 add ParameterInfo
{
    asParaValue=&_asParaValue;
    asFileParaName=_asFileName;
}
//---------------------------------------------------------------------------
ParameterInfo::~ParameterInfo()                                                 //Jimmychiu 20220830 add ParameterInfo Class
{
}
//---------------------------------------------------------------------------
FileInfo::FileInfo(AnsiString FilePath)                                         //Jimmychiu 20220830 add FileInfo Class
{
    asFilePath=FilePath;
}
//---------------------------------------------------------------------------
FileInfo::~FileInfo()                                                           //Jimmychiu 20220830 add FileInfo Class
{
    for(int i=0; i<GetSize(); i++)
    {
        delete mapGroup[i];
    }
    mapGroup.clear();
    Sleep(1);                                                                   //避免非同步互搶資源
}
//---------------------------------------------------------------------------
bool FileInfo::ReadFile(AnsiString sSeparatedValues)
{
    return ReadFile(sSeparatedValues,0);                                        //Jimmychiu 20220830 解碼並讀取資料
}
//---------------------------------------------------------------------------
bool FileInfo::ReadFile(AnsiString sSeparatedValues, int idecode)               //Jimmychiu 20220830 解碼並讀取資料
{
    TStringList *slSourceFile=new TStringList();
    AnsiString asGetString="";
    if(IsFilePathExist(asFilePath)==false)
        return false;

    if(idecode!=0)
    {
        TStringStream *ts;
        ts=new TStringStream(DecodeASCII(asFilePath,idecode));
        slSourceFile->LoadFromStream(ts);
        delete ts;
    }
    else
    {
        slSourceFile->LoadFromFile(asFilePath);
    }

    for(int i=0; i<slSourceFile->Count; i++)
        DecodeReadMap2Parameter(slSourceFile->Strings[i], sSeparatedValues);
    delete slSourceFile;
    return true;
}
//---------------------------------------------------------------------------
bool FileInfo::WriteFile(AnsiString sSeparatedValues)                           //Jimmychiu 20220830 解碼並讀取資料
{
    TStringList *slSourceFile=new TStringList();
    AnsiString asSetString="";
    if(IsFilePathExist(asFilePath)==false)
        return false;

    slSourceFile->LoadFromFile(asFilePath);
    for(int i=0; i<GetSize(); i++)
    {
        ParameterInfo* _p=mapGroup.find(i)->second;
        asSetString=_p->asFileParaName+sSeparatedValues+*(_p->asParaValue);
        slSourceFile->Add(asSetString);
    }
    delete slSourceFile;
    return true;
}
//---------------------------------------------------------------------------
bool FileInfo::IsFilePathExist(AnsiString sPath)
{
    if(sPath=="" || FileExists(sPath)==false)
        return false;
    return true;
}
//---------------------------------------------------------------------------
AnsiString FileInfo::DecodeASCII(char *cStream, int iLen, int iTransfor)        //Jimmychiu 20220830 解碼並讀取資料
{
    for(int i=0; i<iLen; i++)
        cStream[i]+=iTransfor;
    return AnsiString(cStream);
}
//---------------------------------------------------------------------------
AnsiString FileInfo::DecodeASCII(AnsiString asFileName, int iTransfor)
{
    fstream fp;
    AnsiString asBack="";
    char *a;
    fp.open(asFileName.c_str(), ios::in);                                       //開啟檔案
    if(fp.is_open())
    {
        fp.seekg(0, ios::end);
        int n=fp.tellg();
        a=new char[n];
        fp.seekg(0,ios::beg);
        fp.read(a,n);
        asBack=DecodeASCII(a,n,iTransfor);
        fp.close();
    }
    delete[] a;
    return asBack;
}
//---------------------------------------------------------------------------
void FileInfo::DecodeReadMap2Parameter(AnsiString asLineValue, AnsiString sSeparatedValues)
{
    AnsiString asKey="";
    int iStart=-1;
    for(int j=0;j<GetSize();j++)
    {
        ParameterInfo* _p=mapGroup.find(j)->second;
        asKey=_p->asFileParaName;
        if(asKey=="")continue;
        if(asLineValue.Pos(asKey+sSeparatedValues)==1)
        {
            iStart=asKey.Length()+sSeparatedValues.Length()+1;
            *(_p->asParaValue)=asLineValue.SubString(iStart,asLineValue.Length()-iStart+1);
            break;
        }
    }
}
//---------------------------------------------------------------------------
void FileInfo::SaveAsTxtFile(AnsiString asContent, AnsiString asNewFileWithPath)                        //Jimmychiu 20230223 帳密權限加密，提升資訊安全
{
    ofstream outfile;
    outfile.open(asNewFileWithPath.c_str());
    outfile<<asContent.c_str();
    outfile.close();
}
//---------------------------------------------------------------------------
void FileInfo::GetNameAndExtension(AnsiString asFileName, AnsiString &asFile, AnsiString &asExtension) //Jimmychiu 20230223 帳密權限加密，提升資訊安全
{
    int dot_pos = asFileName.LastDelimiter('.');
    asFile=asExtension="";
    if(dot_pos>0)
    {
        asFile=asFileName.SubString(1,dot_pos-1);
        asExtension=asFileName.SubString(dot_pos+1, asFileName.Length()-dot_pos+1);
    }
}
//---------------------------------------------------------------------------   //Jimmychiu 20240224 : Get all file name in the folder
std::vector<AnsiString> FileInfo::GetAllFileNamesInFolder(AnsiString folderPath, int &iErr)
{
    //iErr=>  0=normal 1=No Folder
    iErr=0;
    std::vector<AnsiString> fileNames;
    TSearchRec searchRec;
    if(folderPath.SubString(folderPath.Length(),1) != "\\")
    {
        folderPath+="\\";
    }
    int findResult=FindFirst(folderPath+"*.*",faAnyFile,searchRec);
    if(findResult==0)
    {
        do
        {
            if((searchRec.Attr & faDirectory)==0)//remove folder name
            {
                fileNames.push_back(searchRec.Name);
            }
        }while(FindNext(searchRec)==0);
        FindClose(searchRec);
    }
    else
    {
        iErr=1;
    }
    return fileNames;
}
//---------------------------------------------------------------------------
std::vector<AnsiString> FileInfo::GetAllFolderNamesInFolder(AnsiString folderPath, int &iErr)
{
    //iErr=>  0=normal 1=No Folder
    iErr=0;
    std::vector<AnsiString> folderNames;
    TSearchRec searchRec;
    if(folderPath.SubString(folderPath.Length(),1) != "\\")
    {
        folderPath+="\\";
    }
    int findResult=FindFirst(folderPath+"*.*",faDirectory,searchRec);
    if(findResult==0)
    {
        do
        {
            if ((searchRec.Attr & faDirectory) &&
                searchRec.Name != "." &&
                searchRec.Name != "..")
            {
                folderNames.push_back(searchRec.Name);
            }
        }while(FindNext(searchRec)==0);
        FindClose(searchRec);
    }
    else
    {
        iErr=1;
    }
    return folderNames;
}
//---------------------------------------------------------------------------
bool FileInfo::DeleteFolderContents(AnsiString folderPath,int &iErr)
{
    if(folderPath.SubString(folderPath.Length(),1)!="\\")
    {
        folderPath+="\\";
    }
    iErr=0; //iErr=>  0=normal 1=No Folder 2=delete fail
    std::vector<AnsiString> fileNames =FileInfo().GetAllFileNamesInFolder(folderPath,iErr);   //"D:\\PM RECORD\\";
    if(iErr==1)
    {
        return false;
    }
    else
    {
        AnsiString sFileName="";
        for(unsigned int i=0;i<fileNames.size();i++)
        {
            try
            {
                if(DeleteFile(folderPath+fileNames[i])==false)
                {
                    iErr=2;
                }
            }
            catch (Exception& e)
            {
                iErr=2;
            }
        }
    }
    return true;
}
//---------------------------------------------------------------------------
void FileInfo::SplitPath(const AnsiString& fullPath, AnsiString& path, AnsiString& filename, AnsiString& extension)
{
    int lastSlashIndex=fullPath.LastDelimiter("\\");
    if(lastSlashIndex!=0)
    {
        path=fullPath.SubString(1,lastSlashIndex-1);
        AnsiString filenameWithExtension=fullPath.SubString(lastSlashIndex+1,fullPath.Length()-lastSlashIndex);
        int lastDotIndex=filenameWithExtension.LastDelimiter(".");
        if(lastDotIndex!=0)
        {
            filename=filenameWithExtension.SubString(1,lastDotIndex-1);
            extension=filenameWithExtension.SubString(lastDotIndex+1,filenameWithExtension.Length()-lastDotIndex);
        }
        else
        {
            filename=filenameWithExtension;
            extension="";
        }
    }
    else
    {
        path="";
        filename=fullPath;
        extension="";
    }
}
//---------------------------------------------------------------------------
bool FileInfo::DirectoryExist(AnsiString folderPath)
{
    folderPath=RemoveAllTrailingBackslashes(folderPath);
    return DirectoryExists(folderPath);
}
//---------------------------------------------------------------------------
AnsiString FileInfo::RemoveAllTrailingBackslashes(const AnsiString& input)
{
    int ilen=input.Length();
    while(ilen>0 && input[ilen]=='\\')
    {
        ilen--;
    }
    return input.SubString(1, ilen);
}
//---------------------------------------------------------------------------
AnsiString FileInfo::PathCombin(const AnsiString& sPath, const AnsiString& sFile)
{
    AnsiString combinedPath=sPath;
    if(!combinedPath.IsEmpty())
    {
        if(combinedPath[combinedPath.Length()]=='/' || combinedPath.Pos("/"))   // FTP path format
        {
            if(combinedPath[combinedPath.Length()]!='/')
            {
                combinedPath+="/";
            }
        }
        else                                                                    // Local path
        {
            if(combinedPath[combinedPath.Length()]!='\\' &&
               sFile.AnsiPos("\\")!=1)                                          //Steven 20260708 : Add protection
            {
                combinedPath+="\\";
            }
        }
    }
    combinedPath+=sFile;
    return combinedPath;
}
//---------------------------------------------------------------------------
void FileInfo::EnsureDirectoriesExist(AnsiString sPath)
{
    if(sPath.Length()==0)
        return;

    if(sPath[sPath.Length()]!='\\')                                             //Steven 20260505 : Auto-detect filename component
    {
        DWORD attr=GetFileAttributes(sPath.c_str());
        if(attr!=INVALID_FILE_ATTRIBUTES)                                       //RogerYang 20260527 : Fix - INVALID treats as dir-to-create
        {
            if(!(attr & FILE_ATTRIBUTE_DIRECTORY))
                sPath=ExtractFilePath(sPath);                                   //路徑存在：確認是檔案才 strip
        }
        else
        {
            AnsiString sExt=ExtractFileExt(sPath).LowerCase();                  //RogerYang 20260603 : 只卡控明確的檔案副檔名，避免誤判含「.」的目錄名
            if(sExt==".txt" || sExt==".csv")
                sPath=ExtractFilePath(sPath);
        }
    }

    if(sPath[sPath.Length()]=='\\')
    {
        sPath=sPath.SubString(1,sPath.Length()-1);
    }
    int pos=1;
    while(pos<=sPath.Length())
    {
        int nextPos=sPath.SubString(pos, sPath.Length()-pos+1).Pos("\\");
        if(nextPos>0)
        {
            nextPos=pos+nextPos-1;
        }
        else
        {
            nextPos=sPath.Length()+1;
        }
        AnsiString currentPath=sPath.SubString(1, nextPos-1);
        if(GetFileAttributes(currentPath.c_str())==INVALID_FILE_ATTRIBUTES)
        {
            if(!CreateDirectory(currentPath.c_str(), NULL))
            {
                return;
            }
        }
        pos=nextPos+1;
    }
}
//---------------------------------------------------------------------------
cDatabaseMin::cDatabaseMin()
{
    Clear();
}
//---------------------------------------------------------------------------
cDatabaseMin::~cDatabaseMin()
{
//
}
//---------------------------------------------------------------------------
bool cDatabaseMin::LoadFile(AnsiString dbpath,AnsiString dbName)
{
    return true;
}
//---------------------------------------------------------------------------
void cDatabaseMin::SaveFile()
{
//
}
//---------------------------------------------------------------------------
cDBTStringGrid::cDBTStringGrid(TStringGrid* _sg)
{
    sg=_sg;
    iRowHeight=25;
}
//---------------------------------------------------------------------------
cDBTStringGrid::~cDBTStringGrid()
{
//
}
//---------------------------------------------------------------------------
void cDBTStringGrid::Clear()
{
    if(sg)
    {
        int recordCount=0;
        int fieldCount=0;
        GetRowCol(recordCount,fieldCount);
        for(int irow=0;irow<recordCount;irow++)
        {
            for(int icol=0;icol<fieldCount;icol++)
            {
                SetCell(irow,icol,"");
            }
        }
        SetRowCol(1,1);
    }
}
//---------------------------------------------------------------------------
void cDBTStringGrid::SetRowCol(int iRow,int iCol)
{
    if(sg)
    {
        sg->RowCount =iRow+1;//title
        sg->ColCount =iCol+1;//title
        sg->DefaultColWidth=100;
        sg->ColWidths[0]=30;
    }
}
//---------------------------------------------------------------------------
void cDBTStringGrid::GetRowCol(int &iRow,int &iCol)
{
    if(sg)
    {
        iRow=sg->RowCount-1;//remove title
        iCol=sg->ColCount-1;//remove title
    }
}
//---------------------------------------------------------------------------
void cDBTStringGrid::SetColTitle(int iCol,const AnsiString str)
{
    if(sg)
    {
        sg->Cells[iCol+1][0]=str;
    }
}
//---------------------------------------------------------------------------
AnsiString cDBTStringGrid::GetColTitle(int iCol)
{
    AnsiString str="";
    if(sg)
    {
        str=sg->Cells[iCol+1][0];
    }
    return str;
}
//---------------------------------------------------------------------------
void cDBTStringGrid::SetRowTitle(int iRow,const AnsiString str)
{
    if(sg)
    {
        sg->Cells[0][iRow+1]=str;
    }
}
//---------------------------------------------------------------------------
AnsiString cDBTStringGrid::GetRowTitle(int iRow)
{
    AnsiString sBack="";
    if(sg)
    {
        int recordCount=0;
        int fieldCount=0;
        GetRowCol(recordCount,fieldCount);
        if(iRow<recordCount)
        {
            sBack=sg->Cells[0][iRow+1];
        }
    }
    return sBack;
}
//---------------------------------------------------------------------------
void cDBTStringGrid::SetCell(int iRow,int iCol,const AnsiString str)
{
    if(sg)
    {
        iRow=iRow+1;
        iCol=iCol+1;
        int recordCount=0;
        int fieldCount=0;
        bool bReset=false;
        GetRowCol(recordCount,fieldCount);
        if(iRow>=recordCount)
        {
            recordCount=iRow;
            bReset=true;
        }

        if(iCol>=fieldCount)
        {
            fieldCount=iCol;
            bReset=true;
        }

        if(bReset)
            SetRowCol(recordCount,fieldCount);
        sg->Cells[iCol][iRow]=str;
    }
}
//---------------------------------------------------------------------------
AnsiString cDBTStringGrid::GetCell(int iRow,int iCol)
{
    AnsiString sBack="";
    if(sg)
    {
        int recordCount=0;
        int fieldCount=0;
        GetRowCol(recordCount,fieldCount);
        if(iCol<fieldCount && iRow<recordCount)
        {
            sBack=sg->Cells[iCol+1][iRow+1];
        }
    }
    return sBack;
}
//---------------------------------------------------------------------------
void cDBTStringGrid::AddRow()
{
    if(sg)
    {
        sg->RowCount++;
    }
}
//---------------------------------------------------------------------------
void cDBTStringGrid::AddCol()
{
    if(sg)
    {
        sg->ColCount++;
    }
}
//---------------------------------------------------------------------------
void cDBTStringGrid::CloneTo(TStringGrid* destination)
{
    if(sg)
    {
        destination->RowCount=sg->RowCount;
        destination->ColCount=sg->ColCount;
        for(int row=0;row<sg->RowCount;row++)
        {
            for(int col=0;col<sg->ColCount;col++)
            {
                destination->Cells[col][row]=sg->Cells[col][row];
            }
        }
    }
}
//---------------------------------------------------------------------------
void cDBTStringGrid::ClearRow(int iRow)
{
    if(sg)
    {
        iRow++;                                      //ignor title
        int recordCount=0;
        int fieldCount=0;
        GetRowCol(recordCount,fieldCount);
        if(iRow<=recordCount)
        {
            for(int row=iRow;row<recordCount;row++)
            {
                for(int col=0;col<fieldCount;col++)
                {
                    sg->Cells[col][row]=sg->Cells[col][row+1];
                }
            }
            sg->RowCount--;
        }
    }
}
//---------------------------------------------------------------------------
void cDBTStringGrid::HideRow(bool bOn,int iIndex)
{
    if(sg)
    {
        int iRowH=(bOn)?0:iRowHeight;
        iIndex++;                                      //ignor title
        int recordCount=0;
        int fieldCount=0;
        GetRowCol(recordCount,fieldCount);
        if(iIndex==0)
        {
            for(int row=0;row<recordCount;row++)
            {
                sg->RowHeights[row]=iRowH;
            }
        }
        else if(iIndex<=recordCount)
        {
            sg->RowHeights[iIndex]=iRowH;
        }
    }
}
//---------------------------------------------------------------------------
cMathTool::cMathTool()
{
    //
}
//---------------------------------------------------------------------------
cMathTool::~cMathTool()
{
    //
}
//---------------------------------------------------------------------------
void cMathTool::Clear()
{
    //
}
//---------------------------------------------------------------------------
bool cMathTool::IsDoubleEqual(const double a,const double b,const double tolerance)
{
    return fabs(a-b)<=tolerance;
}
//---------------------------------------------------------------------------
