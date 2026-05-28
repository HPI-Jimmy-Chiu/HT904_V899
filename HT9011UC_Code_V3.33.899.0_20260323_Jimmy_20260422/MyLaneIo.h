//---------------------------------------------------------------------------

#ifndef MyLaneIoH
#define MyLaneIoH

#include "myMN200motor.h"
//---------------------------------------------------------------------------
class TLaneIO
{
    private:
        unsigned int lOutPortData[128];
        unsigned int lInPortData[128];
        int  OutPos;
        int  InPos;
        int  OutPortIndex[MAXRing][MAXIP][MAXPort];
        int  InPortIndex[MAXRing][MAXIP][MAXPort];
        byte OutPortData[MAXRing][MAXIP][MAXPort];
        byte InPortData[MAXRing][MAXIP][MAXPort];
        byte BackOutPortData[MAXRing][MAXIP][MAXPort];
    public:
        __fastcall TLaneIO::TLaneIO();   //Jimmychiu 20230313 fix _fastcall to __fastcall
        void InitialMyOutIOData();
        void InitialMyUseIP();
//        bool IODataError(int IP,int Port,int Bit);
        void IOBitOn(int Ring, int IP, int Port, int Bit, int iISABase=0, AnsiString Alias="");     //Sam 20230724 : 新增 PCI1203 IO 模組
        void IOBitOff(int Ring, int IP, int Port, int Bit, int iISABase=0, AnsiString Alias="");
        bool IOByteOut(int Ring, int IP, int Port, unsigned int Byte, int iISABase=0);
        bool IOOutBitStatus(int Ring, int IP, int Port, int Bit, int iISABase=0, AnsiString Alias="");
        bool IOInputBit(int Ring, int IP, int Port, int Bit, int iISABase, AnsiString Alias="");
        byte IOInputByte(int Ring, int IP, int Port, int iISABase=0);
        int iUseMNetIP[MAXRing][MAXIP];                                         //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
        void SetUseIP(int Ring, int IP, int iISABase, AnsiString Alias, bool bOutPut=true);
        void BackUpOutputData();
        void RestoreOutputData();
        int CheckPortRangeErr(bool DO_Type, int iISABase, int Ring, int IP, int Port, int Bit=999);
        double GetIOValue(int Ring, int IP, int Port, int Bit, int iISABase=0, AnsiString Alias="");            //Sam 20230210 : 新增 VacuumUnit 通訊模組
        double GetIOValueThread(int Ring, int IP, int Port, int Bit, int iISABase=0, AnsiString Alias="");
        bool SetIOValueThread(double dKpa, int Ring, int IP, int Port, int Bit, int iISABase=0, AnsiString Alias="");
        AnsiString GetIOErrStr(int iErr);
        AnsiString GetUseMNetIPDevTypeName(int iDevType);                       //AI(ht9045-v899) 20260424: 把 mn200 device type enum 翻成可讀字串給售服
        AnsiString BuildIOErrMessage(bool bDOType, AnsiString sFuncName,        //AI(ht9045-v899) 20260424: 統一 IO 錯誤訊息格式給售服
                                     AnsiString sAlias, int Ring, int IP,
                                     int Port, int Bit, int iErr);
};
extern class TLaneIO MyLaneIO;
#endif
