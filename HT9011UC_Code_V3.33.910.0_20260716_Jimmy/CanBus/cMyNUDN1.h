//---------------------------------------------------------------------------
#ifndef cMyNUDN1H
#define cMyNUDN1H
//---------------------------------------------------------------------------
class PACKAGE TMyNUDN1 : public TComponent
{
    private:
        //Initail 取得的資料
        unsigned char   iActivedBoardNo;                //DNM100UD 卡號
        unsigned char   iDesMACID;                      //NUDN1 站號
        unsigned char   iType;                          //IO Communication Type
        unsigned short  iInputLen;                      //接收資料的長度
        unsigned short  iOutputLen;                     //傳送資料的長度
        unsigned short  iEPR;
        int             iAmplifier;                     //最多16個
    protected:

    public:
        __fastcall TMyNUDN1(unsigned char _iActivedBoardNo,
                            unsigned char _iDesMACID,
                            unsigned char _iType,
                            unsigned short _iInputLen,
                            unsigned short _iOutputLen,
                            unsigned short _iEPR,
                            int iAmplifier);

        __fastcall ~TMyNUDN1();
        void    Initail();
        int     iGetValue   (int iAmplifierNo);               //NUDN1 第幾個 Amplifier 的 Currect Value
        bool    bGetIO      (int iAmplifierNo);               //NUDN1 第幾個 Amplifier 的 IO (0~15)

        int     iSetIO_Value_Send(int iAmplifierNo,int iSetValue); //NUDN1 設定第幾個 Amplifier 的 IO 閥值(0~15)
        int     iSetIO_Value[16];                               //NUDN1 設定閥值
        int     iGetIO_Value[16];                               //NUDN1 所有 Amplifier IO 閥值陣列。

        //Use I/O Communication (pool)
        //==>
        int iReadInputData(int iOperatingMode);  //讀取 NUDN1 IO 資料 //default:2,  0:Standard mode; 1:External input mode; 2:Monitor mode; 3:Full mode; 4:Minimum mode
        union InArea_Output
        {
            unsigned short iRaw;
            struct
            {
                unsigned char No00:1;
                unsigned char No01:1;
                unsigned char No02:1;
                unsigned char No03:1;
                unsigned char No04:1;
                unsigned char No05:1;
                unsigned char No06:1;
                unsigned char No07:1;
                unsigned char No08:1;
                unsigned char No09:1;
                unsigned char No10:1;
                unsigned char No11:1;
                unsigned char No12:1;
                unsigned char No13:1;
                unsigned char No14:1;
                unsigned char No15:1;
            }IO;
        };

        union InArea_Status
        {
            unsigned short iRaw;
            struct
            {
                unsigned char ErrorID                   :4;
                unsigned char ErrorCode                 :3;
                unsigned char UpdatingSensorSetting     :1;
                unsigned char SensorEEPROMBusy          :1;
                unsigned char SensorExternalInputBusy   :1;
                unsigned char SensorSettingError        :1;
                unsigned char reserved                  :1;
                unsigned char SensorReady               :1;
                unsigned char WarningStatus             :1;
                unsigned char ErrorStatus               :1;
            }Status;
        };

        //I/O Communication (pool) For NUDN1 Monitor mode
        union InArea_Monitor_Mode
        {
            unsigned char iRawItem[40];
            struct
            {
                InArea_Output   Output     ;
                InArea_Status   Status     ;
                unsigned short  reserved:16;
                InArea_Output   Property   ;
                unsigned short  Value01 :16;
                unsigned short  Value02 :16;
                unsigned short  Value03 :16;
                unsigned short  Value04 :16;
                unsigned short  Value05 :16;
                unsigned short  Value06 :16;
                unsigned short  Value07 :16;
                unsigned short  Value08 :16;
                unsigned short  Value09 :16;
                unsigned short  Value10 :16;
                unsigned short  Value11 :16;
                unsigned short  Value12 :16;
                unsigned short  Value13 :16;
                unsigned short  Value14 :16;
                unsigned short  Value15 :16;
                unsigned short  Value16 :16;
            }InArea;
            int CurrectValue(int iAmplifierNo)
            {
                if(iAmplifierNo==0)         return InArea.Value01;
                else if(iAmplifierNo==1)    return InArea.Value02;
                else if(iAmplifierNo==2)    return InArea.Value03;
                else if(iAmplifierNo==3)    return InArea.Value04;
                else if(iAmplifierNo==4)    return InArea.Value05;
                else if(iAmplifierNo==5)    return InArea.Value06;
                else if(iAmplifierNo==6)    return InArea.Value07;
                else if(iAmplifierNo==7)    return InArea.Value08;
                else if(iAmplifierNo==8)    return InArea.Value09;
                else if(iAmplifierNo==9)    return InArea.Value10;
                else if(iAmplifierNo==10)   return InArea.Value11;
                else if(iAmplifierNo==11)   return InArea.Value12;
                else if(iAmplifierNo==12)   return InArea.Value13;
                else if(iAmplifierNo==13)   return InArea.Value14;
                else if(iAmplifierNo==14)   return InArea.Value15;
                else                        return InArea.Value16;
            };
            bool CurrectOutput(int iAmplifierNo)
            {
                if(iAmplifierNo==0)         return (InArea.Output.IO.No00==1)?true:false;
                else if(iAmplifierNo==1)    return (InArea.Output.IO.No01==1)?true:false;
                else if(iAmplifierNo==2)    return (InArea.Output.IO.No02==1)?true:false;
                else if(iAmplifierNo==3)    return (InArea.Output.IO.No03==1)?true:false;
                else if(iAmplifierNo==4)    return (InArea.Output.IO.No04==1)?true:false;
                else if(iAmplifierNo==5)    return (InArea.Output.IO.No05==1)?true:false;
                else if(iAmplifierNo==6)    return (InArea.Output.IO.No06==1)?true:false;
                else if(iAmplifierNo==7)    return (InArea.Output.IO.No07==1)?true:false;
                else if(iAmplifierNo==8)    return (InArea.Output.IO.No08==1)?true:false;
                else if(iAmplifierNo==9)    return (InArea.Output.IO.No09==1)?true:false;
                else if(iAmplifierNo==10)   return (InArea.Output.IO.No10==1)?true:false;
                else if(iAmplifierNo==11)   return (InArea.Output.IO.No11==1)?true:false;
                else if(iAmplifierNo==12)   return (InArea.Output.IO.No12==1)?true:false;
                else if(iAmplifierNo==13)   return (InArea.Output.IO.No13==1)?true:false;
                else if(iAmplifierNo==14)   return (InArea.Output.IO.No14==1)?true:false;
                else                        return (InArea.Output.IO.No15==1)?true:false;
            };
        }InAreaMonitor;
        //<==
        //Use I/O Communication (pool)

        __published:
            __property byte iGetActivedBoardNo={read=iActivedBoardNo,default=0};    //NUDN1 裝在哪張 DNM100UD
            __property byte iGetDesMACID={read=iDesMACID,default=0};                //NUDN1 站號
            __property int iGetAmplifier={read=iAmplifier,default=0};               //NUDN1 的 Amplifier 數量(1~16)
};
#endif

