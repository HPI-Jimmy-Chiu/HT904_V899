//---------------------------------------------------------------------------
// MyDBC : Creat Table
// MyDBI : Insert Record into Table
// MyDBU : Update Record into Table
// MyDBD : Drop Table or Delete Record
// MyDBQ : Query Data from Table
// MyDBV : View All Record in Table
//---------------------------------------------------------------------------

#ifndef cMyDBH
#define cMyDBH

#include <Grids.hpp>
#include <Chart.hpp>

//---------------------------------------------------------------------------
// 新增資料到表單中
//---------------------------------------------------------------------------
int  __fastcall MyDBIEvent(AnsiString AlarmCode, int MotorID, int *AlarmID, int *UnitNo, int *AxleNo, int *Type, AnsiString *Message, AnsiString *UnitName, AnsiString asTemperature=" ", int bDuplicateErr=0, AnsiString errPart=" ", bool bDate=false, TDateTime date=0); //Chunghung 2012 0416 add date
void __fastcall MyDBIProcess(AnsiString asTable, AnsiString S1, AnsiString S2="");
void __fastcall MyDBIProcessNew(AnsiString asTable, AnsiString AlarmCode, AnsiString S1, AnsiString S2=" "); //Steven 20161220 : Process加上Alarm Code
void __fastcall MyDBITotalLoader(int iLoader);
int  __fastcall MyDBITimeData(long StartTime, long HomeTime, long ContactTest, long PauseTime, long ProductTime, long JamTime, long PowerOn);
void __fastcall MyDBIUPH(int UPH);                                              //Steven 20190906 : Add UPH in EventLog
void __fastcall RecordTimeData(int iDataType);                                  //JerryYang 20180515 記錄Time data
void __fastcall MyDBIProductionData(AnsiString sAction);
//---------------------------------------------------------------------------
// 更新表單資料
//---------------------------------------------------------------------------
void __fastcall MyDBULotInfo();                                                 //查詢是否有相同的LitID存在，若無就新增一筆
void __fastcall MyDBULotInfo(TStringGrid *strGrid);                             //將StringGrid的內容存入資料庫
void __fastcall MyDBULotData(AnsiString TableName, TStringGrid *strGrid);
void __fastcall MyDBULotEndTime(AnsiString EndTime);
void __fastcall MyDBUEventRecover(int RowID, AnsiString Recovery, int StopTime);
void __fastcall MyDBUpdateDB();                                                 //所有資料庫更新事項

//---------------------------------------------------------------------------
// 資料庫查詢
//---------------------------------------------------------------------------
void        __fastcall MyDBQLotData(AnsiString Query, TStringList *strList);    //將LotData的內容存成StringList並回傳
AnsiString  __fastcall MyDBQMotMess(int MotorID);                               //回傳MotorAlarm的Message
bool        __fastcall MyDBQAlarmCodeList();                                    //Steven 20170202 (wei): Fixed for Unknown Alarm Code
AnsiString  __fastcall MyDBQClearDT();                                          //回傳最近一次的資料清除時間
AnsiString  __fastcall MyDBQTotalLoader(AnsiString StartDateTime, AnsiString EndDateTime);
void        __fastcall MyDBQTimeData(AnsiString StartDateTime, AnsiString EndDateTime, int sgRow, TStringGrid *strGrid);

void GetJameCodeOfAxis(int iAxis, TComboBox *ComboBox);                         //Steven 20140221 : 根據Axis取出對應的Jam Code
AnsiString __fastcall GetMyDBIMessage(AnsiString AlarmCode);                    //Sam 20230218 : 新增用 AlarmCode 去找 Alarm Message 資料
//---------------------------------------------------------------------------
// 檢視資料表
//---------------------------------------------------------------------------
int         __fastcall MyDBVEventFreq(AnsiString asQuery, TStringGrid *strGrid);//次數統計
int         __fastcall MyDBVAxleEventCount(int min, int max, AnsiString StartTime, AnsiString EndTime, TChart *Chart);//將結果直接畫到Chart上
int         __fastcall MyDBVUnitEventCount(AnsiString asQuery, TChart *Chart);
int         __fastcall MyDBVProcess(AnsiString asQuery, TStringGrid *strGrid);  //取得所有按鍵的紀錄
int         __fastcall MyDBVProcessFilter(AnsiString asQuery, TStringGrid *strGrid);  //過濾掉Duplicate的訊息
void        __fastcall GetAlarmCodeList(TStringGrid *strGrid);                  //Steven 20200331 : Alarm code list改用文字檔
//---------------------------------------------------------------------------
// 其他
//---------------------------------------------------------------------------
void        __fastcall MyDBVACUUM();                                            //壓縮資料庫
void NewRecordProcess(AnsiString AlarmCode, AnsiString S, AnsiString Debug=" ");//Steven 20161220 : Process加上Alarm Code
void RecordProcess(AnsiString S, AnsiString S2="");
void RecordChangeLogProcess(AnsiString S, AnsiString S2="");                    //wei 20180625 offset Change log紀錄
void __fastcall MyDBOpenDB();
void __fastcall MyDBCloseDB();

void __fastcall SaveEventLogInfo(AnsiString aAlarmCode, AnsiString aMess, int iType, AnsiString aStatus);   //Steven 20200116 : 從uLotInfor改到cMyDB, 避免uLotInfo被解構後沒辦法存取
extern AnsiString aBackEventLogFile;
extern AnsiString aBackEventLogMessage;
extern TQPF_Timer tEventLogTimer;
extern int iPerMinuteNumberofError;
extern int iRecordEventLogUPH;
extern AnsiString ExString;

#define iAlarmUnitTotal 32                                                      //Steven 20231127 : 整理Alarm Unit
extern AnsiString AlarmUnit[iAlarmUnitTotal];
extern AnsiString AlarmUnitNo[iAlarmUnitTotal];
//---------------------------------------------------------------------------
#endif
