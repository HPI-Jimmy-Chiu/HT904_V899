//---------------------------------------------------------------------------

#ifndef uRENESAS_OtherH
#define uRENESAS_OtherH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TRENESAS_OnLine
{
    public:
    AnsiString CMD;
    int iRecv;
};
//---------------------------------------------------------------------------
class TRENESAS_ProdInfo
{
    public:
    AnsiString CMD;
    AnsiString LotNo;
    AnsiString LotID;
    AnsiString HdParameter;
    AnsiString Product;
    AnsiString StepCode;
    AnsiString Qty;
    AnsiString Date;
    int iRecv;
};
//---------------------------------------------------------------------------
class TRENESAS_ModeStart
{
    public:
    AnsiString CMD;
    AnsiString Status;
    AnsiString Date;
    int iRecv;
};
//---------------------------------------------------------------------------
class TRENESAS_HandlerStart
{
    public:
    AnsiString CMD;
    AnsiString Date;
    int iRecv;
};
//---------------------------------------------------------------------------
class TRENESAS_ModeEnd
{
    public:
    AnsiString CMD;
    AnsiString Status;
    AnsiString Date;
    int iRecv;
};
//---------------------------------------------------------------------------
class TRENESAS_LotEnd
{
    public:
    AnsiString CMD;
    AnsiString LECD;
    AnsiString Date;
    int iRecv;
};
//---------------------------------------------------------------------------
//Kirin 20220118 TestEnd
//==>
class TRENESAS_TestEnd
{
    public:
    AnsiString CMD;
    AnsiString Date;
    int iRecv;
};
//<==
//Kirin 20220118 TestEnd
//---------------------------------------------------------------------------
//Kirin 20211020 SendAlarm
//==>
class TRENESAS_SendAlarm
{
    public:
    AnsiString CMD;
    int iRecv;
};
//<==
//Kirin 20211020 SendAlarm
//---------------------------------------------------------------------------
//Kirin 20221115 SendCleanOut
//==>
class TRENESAS_SendCleanOut
{
    public:
    AnsiString CMD;
    AnsiString Date;
    int iRecv;
};
//<==
//Kirin 20221115 SendCleanOut
//---------------------------------------------------------------------------
class TRENESAS_RECV_CMD_DATA
{
    public:
    TRENESAS_OnLine         OnLine;
    TRENESAS_ProdInfo       ProdInfo;
    TRENESAS_ModeStart      ModeStart;
    TRENESAS_HandlerStart   HandlerStart;
    TRENESAS_ModeEnd        ModeEnd;
    TRENESAS_LotEnd         LotEnd;
    TRENESAS_TestEnd        TestEnd;  //Kirin 20220118 TestEnd
    TRENESAS_SendAlarm      SendAlarm;//Kirin 20211020 SendAlarm
    TRENESAS_SendCleanOut   SendCleanOut;//Kirin 20221115 SendCleanOut
};
//---------------------------------------------------------------------------
class TRENESAS_OnLine_Reply
{
    public:
    AnsiString CMD;
    AnsiString Result;
    AnsiString Comment;
};
//---------------------------------------------------------------------------
class TRENESAS_ProdInfo_Reply
{
    public:
    AnsiString CMD;
    AnsiString Result;
    AnsiString Comment;
    AnsiString HdParameter;
    AnsiString Product;
};
//---------------------------------------------------------------------------
class TRENESAS_ModeStart_Reply
{
    public:
    AnsiString CMD;
    AnsiString Status;
    AnsiString Result;
    AnsiString Comment;
    AnsiString Product;
};
//---------------------------------------------------------------------------
class TRENESAS_HandlerStart_Reply
{
    public:
    AnsiString CMD;
    AnsiString Result;
    AnsiString Comment;
};
//---------------------------------------------------------------------------
class TRENESAS_ModeEnd_Reply
{
    public:
    AnsiString CMD;
    AnsiString Status;
    AnsiString Result;
    AnsiString Comment;
    AnsiString TotalQty;
    AnsiString HdParameter;
};
//---------------------------------------------------------------------------
class TRENESAS_LotEnd_Reply
{
    public:
    AnsiString CMD;
    AnsiString Result;
    AnsiString Comment;
};
//---------------------------------------------------------------------------
//Kirin 20220118 TestEndReply
//==>
class TRENESAS_TestEnd_Reply
{
    public:
    AnsiString CMD;
    AnsiString Result;
    AnsiString Comment;
};
//<==
//Kirin 20220118 TestEndReply
//---------------------------------------------------------------------------
class TRENESAS_Send_CMD_DATA
{
    public:
    TRENESAS_OnLine_Reply       OnLine_Reply;
    TRENESAS_ProdInfo_Reply     ProdInfo_Reply;
    TRENESAS_ModeStart_Reply    ModeStart_Reply;
    TRENESAS_HandlerStart_Reply HandlerStart_Reply;
    TRENESAS_ModeEnd_Reply      ModeEnd_Reply;
    TRENESAS_LotEnd_Reply       LotEnd_Reply;
    TRENESAS_TestEnd_Reply      TestEnd_Reply;//Kirin 20220118 TestEndReply
};
//---------------------------------------------------------------------------
class TRENESAS_CMD
{
    private:
    AnsiString FGetOnLineCMD()             {   return "00";        };
    AnsiString FGetOnLineReplyCMD()        {   return "01";        };

    AnsiString FGetProductInfoCMD()        {   return "10";        };
    AnsiString FGetProductInfoReplyCMD()   {   return "11";        };

    AnsiString FGetStartCMD()              {   return "20";        };
    AnsiString FGetStartReplyCMD()         {   return "21";        };

    AnsiString FGetHandlerStartCMD()       {   return "50";        };
    AnsiString FGetHandlerStartReplyCMD()  {   return "51";        };

    AnsiString FGetEndCMD()                {   return "30";        };
    AnsiString FGetEndReplyCMD()           {   return "31";        };

    AnsiString FGetLotEndCMD()             {   return "40";        };
    AnsiString FGetLotEndReplyCMD()        {   return "41";        };
    //Kirin 20220118 TestEnd
    //==>
    AnsiString FGetTestEndCMD()             {   return "70";        };
    AnsiString FGetTestEndReplyCMD()        {   return "71";        };
    //<==
    //Kirin 20220118 TestEnd

    //Kirin 20211020 SendAlarm
    //==>
    AnsiString FGetSendAlarmCMD()          {   return "90";        };
    AnsiString FGetSendAlarmReplyCMD()     {   return "91";        };
    //<==
    //Kirin 20211020 SendAlarm

    //Kirin 20221115 SendCleanOut
    //==>
    AnsiString FGetSendCleanOutCMD()          {   return "B0";        };
    AnsiString FGetSendCleanOutReplyCMD()     {   return "B1";        };
    //<==
    //Kirin 20211020 SendCleanOut
    public:
    __property AnsiString OnLineCMD            ={read=FGetOnLineCMD      };
    __property AnsiString OnLineReplyCMD       ={read=FGetOnLineReplyCMD      };

    __property AnsiString ProductInfoCMD       ={read=FGetProductInfoCMD };
    __property AnsiString ProductInfoReplyCMD  ={read=FGetProductInfoReplyCMD };

    __property AnsiString StartCMD             ={read=FGetStartCMD    };
    __property AnsiString StartReplyCMD        ={read=FGetStartReplyCMD    };

    __property AnsiString HandlerStartCMD      ={read=FGetHandlerStartCMD       };
    __property AnsiString HandlerStartReplyCMD ={read=FGetHandlerStartReplyCMD       };

    __property AnsiString EndCMD               ={read=FGetEndCMD         };
    __property AnsiString EndReplyCMD          ={read=FGetEndReplyCMD         };

    __property AnsiString LotEndCMD            ={read=FGetLotEndCMD      };
    __property AnsiString LotEndReplyCMD       ={read=FGetLotEndReplyCMD      };

    //Kirin 20220118 TestEnd
    //==>
    __property AnsiString TestEndCMD         ={read=FGetTestEndCMD      };
    __property AnsiString TestEndReplyCMD    ={read=FGetTestEndReplyCMD };
    //<==
    //Kirin 20220118 TestEnd

    //Kirin 20211020 SendAlarm
    //==>
    __property AnsiString SendAlarmCMD         ={read=FGetSendAlarmCMD      };
    __property AnsiString SendAlarmReplyCMD    ={read=FGetSendAlarmReplyCMD };
    //<==
    //Kirin 20211020 SendAlarm

    //Kirin 20221115 SendCleanOut
    //==>
    __property AnsiString SendCleanOutCMD         ={read=FGetSendCleanOutCMD      };
    __property AnsiString SendCleanOutReplyCMD    ={read=FGetSendCleanOutReplyCMD };
    //<==
    //Kirin 20221115 SendCleanOut

};
//---------------------------------------------------------------------------
class TRENESAS_Status_Code
{
    private:
    AnsiString FGetTestStart()                  {   return "00";        };
    AnsiString FGetReTestStart()                {   return "10";        };
    AnsiString FGetReReTestStart()              {   return "20";        };

    AnsiString FGetTestEnd()                    {   return "01";        };
    AnsiString FGetReTestEnd()                  {   return "11";        };
    AnsiString FGetReReTestEnd()                {   return "21";        };
    public:
    __property AnsiString TestStart            ={read=FGetTestStart      };
    __property AnsiString ReTestStart          ={read=FGetReTestStart    };
    __property AnsiString ReReTestStart        ={read=FGetReReTestStart  };

    __property AnsiString TestEnd              ={read=FGetTestEnd      };
    __property AnsiString ReTestEnd            ={read=FGetReTestEnd    };
    __property AnsiString ReReTestEnd          ={read=FGetReReTestEnd  };
};

//---------------------------------------------------------------------------
class TRENESAS_EndFileData
{
    public:
    //Kirin 20220218 TRENESAS_EndFileData InitData
    //==>
    void InitData()
    {
        HdPass=0;
        HdFail=0;
        HdRejectATPass=0;
        HdRejectATFail=0;
        HdRejectBT=0;
        TesterMatch=0;
        TesterUnmatch=0;
        TesterIgnore=0;
    };
    //<==
    //Kirin 20220218 TRENESAS_EndFileData InitData
    TRENESAS_EndFileData()
    {
        InitData();
    };
    int HdPass;
    int HdFail;
    int HdRejectATPass;
    int HdRejectATFail;
    int HdRejectBT;
    int TesterMatch;
    int TesterUnmatch;
    int TesterIgnore;
    int HdPickUp;
    //Kirin 20221115 Add TwoPassBin
    //==>
    int HdPass1;
    int HdPass2;
    int HdRejectATPass1;
    int HdRejectATPass2;
    //<==
    //Kirin 20221115 Add TwoPassBin 
};
//---------------------------------------------------------------------------
//Kirin 20220127 TRENESAS_Err_Index
//==>
class TRENESAS_Err_Index
{
    private:
    int FGetOK()                    {   return 0;        };
    int FGetHandlerNoHome()         {   return 1;        };
    int FGetHandlerHaveDeviceData() {   return 2;        };
    int FGetHandlerHaveDevice()     {   return 3;        };
    int FGetCanNotLoadWorkFile()    {   return 4;        };
    int FGetDisiableFTCT()          {   return 5;        };
    int FGetOutOfOrder()            {   return 6;        };
    int FGetARTIsDisable()          {   return 7;        };
    int FGetRecvTestEnd()           {   return 8;        };
    int FGetNoInputQty()            {   return 9;        }; //Kirin 20220128
    int FGetNoRemoveTray()          {   return 10;       }; //Kirin 20220714
    int FCleanOutFail()             {   return 11;       }; //Kirin 20221123 CleanOutFail
    int FCleanCountFail()           {   return 12;       }; //Kirin 20230814 

    public:
    __property int OK                   ={read=FGetOK                   };
    __property int HandlerNoHome        ={read=FGetHandlerNoHome        };
    __property int HandlerHaveDeviceData={read=FGetHandlerHaveDeviceData};
    __property int HandlerHaveDevice    ={read=FGetHandlerHaveDevice    };
    __property int CanNotLoadWorkFile   ={read=FGetCanNotLoadWorkFile   };
    __property int DisiableFTCT         ={read=FGetDisiableFTCT         };
    __property int OutOfOrder           ={read=FGetOutOfOrder           };
    __property int ARTIsDisable         ={read=FGetARTIsDisable         };
    __property int RecvTestEnd          ={read=FGetRecvTestEnd          };
    __property int NoInputQty           ={read=FGetNoInputQty           }; //Kirin 20220128
    __property int NoRemoveTray         ={read=FGetNoRemoveTray         }; //Kirin 20220714
    __property int CleanOutFail         ={read=FCleanOutFail            }; //Kirin 20221123 CleanOutFail
    __property int CleanCountFail       ={read=FCleanCountFail          }; //Kirin 20230814
};
//<==
//Kirin 20220127 TRENESAS_Err_Index
//---------------------------------------------------------------------------
extern TRENESAS_CMD         RENESAS_CMD;
extern TRENESAS_Status_Code RENESAS_Status;
extern TRENESAS_Err_Index   RENESAS_Err_Index; //Kirin 20220127 TRENESAS_Err_Index
#endif
