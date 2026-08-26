//---------------------------------------------------------------------------
#ifndef MyNUEC1H
#define MyNUEC1H
//---------------------------------------------------------------------------
class PACKAGE TMyNUEC1 : public TComponent
{
    private:
        unsigned char iRing;                    //使用的 PCI1203 Ring 哪一個 Ring
        unsigned char iDesMACID;                //NUEC1 站號 11、12、13
        unsigned char iAmplifier;               //NUEC1 啟用幾個 Amplifier。最多16顆
        unsigned short DataSize;                //固定值 128
        unsigned short CurValue_N;
        unsigned short CurValue_Index;
        unsigned short CurValue_SubIndex;
        unsigned short SetValue_Index;
        unsigned short SetValue_SubIndex;
        unsigned short IO_Index;
        unsigned short IO_SubIndex;
        int iCurrectValue[16];                  //取得的目前數值放此陣列
        int iSettingValue[16];                  //取得的設定數值放此陣列
        int iMethod;                            //0:舊方法 HT505S。 1:新方法 讀取優化

    protected:

    public:
        __fastcall TMyNUEC1(unsigned char _iRing,
                            unsigned char _iDesMACID,
                            unsigned char _iAmplifier);

        __fastcall ~TMyNUEC1();

        bool bNeedDosetValue[16];                                               //開啟寫入設定值的旗標陣列
        int iNeedSettingValue[16];                                              //寫入設定數值陣列

        bool ReadCurrentValue();                                                //讀取1組 NU-EC1 全部 iAmplifier 目前數值
        bool ReadCurrentValue(int iAmplifierNo);                                //讀取1個 iAmplifier 目前數值
        int  iGetValue(int iAmplifierNo);                                       //回傳 iAmplifier 目前數值

        bool ReadCurrentIO();                                                   //讀取 1組 NU-EC1 目前IO
        bool bGetIO(int iAmplifierNo);                                          //回傳 Amplifier 的 IO (0~15)

        bool ReadSettingValue();                                                //讀取1組 NU-EC1 全部 iAmplifier 設定數值
        bool ReadSettingValue(int iAmplifierNo);                                //讀取1個 iAmplifier 設定數值
        int iGetIO_Value(int iAmplifierNo);                                     //回傳 iAmplifier 設定數值

        void SetSetIOValue(int iAmplifierNo,int iValue);
        bool WriteSettingValue();                                               //寫入1組 NU-EC1 全部 iAmplifier 設定數值
        bool WriteSettingValue(int iAmplifierNo,int iValue);                    //寫入1個 iAmplifier 設定數值  iSlaveNo=0~2, iAmplifierNo=0~15
        int RetWriteSettingValue[16];                                           //當寫入異常時會將異常的回傳值放入此陣列
        void ClearError();

        bool CheckAmplifierExist(int iAmplifierNo);                             //讀取此Amplifier是否通訊正常
        AnsiString CheckAmplifierExist();                                       //Sam 20230814 : EtherCAT 新增 Amplifier 檢查

        union _IOWord
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

        union _IOChar
        {
            unsigned char iRaw;
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
            }IO;
        };

        union _IOArea
        {
            struct
            {
                _IOWord IOWord;
                _IOChar IOCharL;
                _IOChar IOCharH;
            }Area;

            bool CurrectOutput(int iAmplifierNo)
            {
                if(iAmplifierNo==0)         return (Area.IOWord.IO.No00==1)?true:false;
                else if(iAmplifierNo==1)    return (Area.IOWord.IO.No01==1)?true:false;
                else if(iAmplifierNo==2)    return (Area.IOWord.IO.No02==1)?true:false;
                else if(iAmplifierNo==3)    return (Area.IOWord.IO.No03==1)?true:false;
                else if(iAmplifierNo==4)    return (Area.IOWord.IO.No04==1)?true:false;
                else if(iAmplifierNo==5)    return (Area.IOWord.IO.No05==1)?true:false;
                else if(iAmplifierNo==6)    return (Area.IOWord.IO.No06==1)?true:false;
                else if(iAmplifierNo==7)    return (Area.IOWord.IO.No07==1)?true:false;
                else if(iAmplifierNo==8)    return (Area.IOWord.IO.No08==1)?true:false;
                else if(iAmplifierNo==9)    return (Area.IOWord.IO.No09==1)?true:false;
                else if(iAmplifierNo==10)   return (Area.IOWord.IO.No10==1)?true:false;
                else if(iAmplifierNo==11)   return (Area.IOWord.IO.No11==1)?true:false;
                else if(iAmplifierNo==12)   return (Area.IOWord.IO.No12==1)?true:false;
                else if(iAmplifierNo==13)   return (Area.IOWord.IO.No13==1)?true:false;
                else if(iAmplifierNo==14)   return (Area.IOWord.IO.No14==1)?true:false;
                else                        return (Area.IOWord.IO.No15==1)?true:false;
            };
            bool CurrectOutput2(int iAmplifierNo)
            {
                if(iAmplifierNo==0)         return (Area.IOCharL.IO.No00==1)?true:false;
                else if(iAmplifierNo==1)    return (Area.IOCharL.IO.No01==1)?true:false;
                else if(iAmplifierNo==2)    return (Area.IOCharL.IO.No02==1)?true:false;
                else if(iAmplifierNo==3)    return (Area.IOCharL.IO.No03==1)?true:false;
                else if(iAmplifierNo==4)    return (Area.IOCharL.IO.No04==1)?true:false;
                else if(iAmplifierNo==5)    return (Area.IOCharL.IO.No05==1)?true:false;
                else if(iAmplifierNo==6)    return (Area.IOCharL.IO.No06==1)?true:false;
                else if(iAmplifierNo==7)    return (Area.IOCharL.IO.No07==1)?true:false;
                else if(iAmplifierNo==8)    return (Area.IOCharH.IO.No00==1)?true:false;
                else if(iAmplifierNo==9)    return (Area.IOCharH.IO.No01==1)?true:false;
                else if(iAmplifierNo==10)   return (Area.IOCharH.IO.No02==1)?true:false;
                else if(iAmplifierNo==11)   return (Area.IOCharH.IO.No03==1)?true:false;
                else if(iAmplifierNo==12)   return (Area.IOCharH.IO.No04==1)?true:false;
                else if(iAmplifierNo==13)   return (Area.IOCharH.IO.No05==1)?true:false;
                else if(iAmplifierNo==14)   return (Area.IOCharH.IO.No06==1)?true:false;
                else                        return (Area.IOCharH.IO.No07==1)?true:false;
            };
        }IOArea;

        __published:

            __property byte iGetRing={read=iRing,default=0};                                //NUEC1 裝在哪張 DNM100UD
            __property byte iGetDesMACID={read=iDesMACID,default=0};                        //NUEC1 站號
            __property unsigned char iGetAmplifier={read=iAmplifier,default=0};             //NUEC1 的 Amplifier 數量(1~16)
};
#endif
