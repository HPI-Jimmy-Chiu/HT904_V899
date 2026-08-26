//---------------------------------------------------------------------------

#ifndef MyStringListH
#define MyStringListH

#include "MyMemo.h"
//---------------------------------------------------------------------------
/*enum TMySaveType
{
    TByMaxLineCount=0,
    TByHour=1,
    TByDay=2,
    TByMonth=3,
    TByYear=4
};*/
//---------------------------------------------------------------------------
void SaveTryCatchLog(AnsiString asMessage, AnsiString Func);                    //KaiChen 20200706 ：增加 Try Catch 抓問題

class TMyStringList : public TStringList
{
    private:
        AnsiString  HTPath;
        AnsiString  HTFileName;
        AnsiString  HTFirstRow;
        AnsiString  HTLotID;                                                    //KaiChen 20181121 ：矽格-北興 Save Event Log by Lot ID
        AnsiString  HTLotStartTime;                                             //Steven 20250520 : 加入By Lot存檔
        int         HTMaxLineCount;
        TSaveType   HTSaveType;
        bool        HTAutoSave;
        bool        HTSaveSameFolder;                                           //KaiChen 20180322 ：矽格-湖口 新增 將 Event Log 放在同個資料夾
        bool        HTSaveByLotID;                                              //KaiChen 20181121 ：矽格-北興 Save Event Log by Lot ID
        bool        HTSaveFixedFile;                                            //Steven 20250716 : 固定檔名存檔
        void __fastcall SetPath(AnsiString P);
        void __fastcall SetFirstRow(AnsiString P);
        void __fastcall SetMaxLineCount(int Cnt);
        void __fastcall SetSaveType(TSaveType Type);
        void __fastcall SetFileName(AnsiString P);
        void __fastcall SetAutoSave(bool P);
        void __fastcall SetSaveSameFolder(bool P);                              //KaiChen 20180322 ：矽格-湖口 新增 將 Event Log 放在同個資料夾
        void __fastcall SetSaveByLotID(bool P);                                 //KaiChen 20181121 ：矽格-北興 Save Event Log by Lot ID
        void __fastcall SetSaveFixedFile(bool P);                               //Steven 20250716 : 固定檔名存檔
        Word SystemHour, SystemMin, SystemSec, SystemMSec;
        Word SystemYear, SystemMonth, SystemDate;
        void GetTimeInfo();
    protected:
    public:
        __fastcall TMyStringList();
        __fastcall TMyStringList(AnsiString sPath, AnsiString sFileName, AnsiString sFirstRow);
        __fastcall ~TMyStringList();
        void __fastcall AddText(AnsiString Msg);
        void __fastcall AddTextWithLineNo(AnsiString Msg);
        void __fastcall AddTextWithDateTime(AnsiString Msg);
        AnsiString __fastcall AddTextWithDateTime2(AnsiString Msg);
        void __fastcall AddTextWithDateTime3(AnsiString Msg, AnsiString Msg2="");   //Steven 20231120 : 把MSec分開存檔
        void MySaveToFile();
        void MySaveToFileShareMode();                                           //Jimmychiu 20250313 : Write data in shared mode
        void MyInsertToFile(AnsiString Msg, int iCount);                        //Steven 20191016 : 紀錄目前Alarm在檔案裡面的行數
        int GetLastLine();                                                      //Steven 20191016 : 取得目前檔案的行數

        AnsiString sLastFileName;                                               //Steven 20191107 : 紀錄現在的檔名
        AnsiString sLastNameNoPath;                                             //Steven 20191107 : 紀錄現在的檔名
        AnsiString sLastPathName;
        AnsiString sPrevFileName;                                               //Steven 20250809 : 紀錄前一個的檔名
        AnsiString sLotFileName;                                                //Steven 20250809 : 修正event log上傳
        bool bChangeFile;
        AnsiString GetFileName();
        bool bFilePathWithDate;
        bool bUseFTRT;
        bool bHanaTrayMap;                                                      //Steven 20250414 : HANA ART Function
        void MySaveFileByFileNameAndType(AnsiString asFileName, AnsiString asLotId, AnsiString asFileType);  //Sam 20171215 (wei) : 檔名尾巴增加JAM/INPUT //Sam 20170428 (wei) 增加帶入 LotId //RogerYAng 20170406 帶入檔案名稱
        void MySaveFileByFileName(AnsiString asPathName, AnsiString asFileName);   //Sam 20170516 (wei) 力成 SocketID Count 保存
        TStringList *MyList;                                                    //因為建構跟解構時,還沒有畫面,用到VCL得東西會死翹翹,所以用StringList做中介
        void MySaveSGJamCountToFile(bool bDelete, bool bflag);                  //KaiChen 20200618 ：矽格，增加Jam統計頁面
        void MyInsert2DMappingToFile(AnsiString s2DID, int iPlaceToUnload=-1);  //JerryYang 20230322 : add 2D mapping result
        void SetLotData(AnsiString ID, AnsiString Time, AnsiString LotFileName="");

    __published:
        __property AnsiString   Path                ={read=HTPath,                  write=SetPath,                  default=NULL};
        __property AnsiString   FileName            ={read=HTFileName,              write=SetFileName,              default=NULL};
        __property AnsiString   FirstRow            ={read=HTFirstRow,              write=SetFirstRow,              default=NULL};
        __property int          MaxLineCount        ={read=HTMaxLineCount,          write=SetMaxLineCount,          default=1};
        __property TSaveType    SaveType            ={read=HTSaveType,              write=SetSaveType,              default=TByDay};
        __property bool         AutoSave            ={read=HTAutoSave,              write=SetAutoSave,              default=true};
        __property bool         FixedFile           ={read=HTSaveFixedFile,         write=SetSaveFixedFile,         default=false};     //Steven 20250716 : 固定檔名存檔
        __property bool         SaveSameFolder      ={read=HTSaveSameFolder,        write=SetSaveSameFolder,        default=false};   //KaiChen 20180322 ：矽格-湖口 新增 將 Event Log 放在同個資料夾
        __property bool         SaveByLotID         ={read=HTSaveByLotID,           write=SetSaveByLotID,           default=false};   //KaiChen 20181121 ：矽格-北興 Save Event Log by Lot ID
};

#endif
