//---------------------------------------------------------------------------

#ifndef DeltaH
#define DeltaH
#define Register_X_Max 256
#define Register_M_Max 4096
#define Register_D_Max 5000
#define Register_Y_Max 256

//---------------------------------------------------------------------------
class DeltaPLC
{
    private:
                                                         // 0:RS-232 , 1:Ethernet
        unsigned char sendbuf[1024];
        unsigned char recvbuf[1024];
        bool bConnection;
        AnsiString asErrMessage;                                                //錯誤訊息//
    protected:
        int conn_num;                                                           //連線識別碼 (支援多條連線)//
        int comm_type;
        int ip;
    public:
        __fastcall DeltaPLC();
        ~DeltaPLC();
        AnsiString GetErrMessage();                                             //取得錯誤訊息//
        void SetConnNumeCommTypeIP(int iConnNum, int iCommType,AnsiString asIP);//設定連線識別碼、通訊Type、IP//

        bool OpenModbusSocket();                                                //開啟連線//
        void CloseModbusSocket();                                               //關閉連線//
        bool GetConnection();                                                   //取得連線狀態//

        bool ReadCoils(char *cDev_addr,int qty, unsigned int *data_r);          //從指定MODBUS位址(Coil)讀取指定筆數資料//
        bool ReadInputs(char *cDev_addr,int qty, unsigned int *data_r);         //從指定MODBUS位址(Discrete Input)讀取指定筆數資料//
        bool ReadHoldRegs(char *cDev_addr,int qty,unsigned int *data_r);        //從指定MODBUS位址(Holding Register)讀取指定筆數資料//
        bool ReadInputRegs(char *cDev_addr);                                    //從指定MODBUS位址(Input Register)讀取指定筆數資料//

        bool WriteSingleCoil(char *cDev_addr,int data_w);                       //寫入數據到指定MODBUS位址(Coil)當中//
        bool WriteSingleReg(char *cDev_addr,unsigned int data_w);               //寫入數據到指定MODBUS位址(Holding Register)當中//
        bool WriteMultiCoils(char *cDev_addr,unsigned int *data_w,int qty);     //寫入數據到指定MODBUS位址(Coil)當中            //(M Register)
        bool WriteMultiRegs(char *cDev_addr,unsigned int *data_w,int qty);      //寫入數據到指定MODBUS位址(Holding Register)當中//(D Register)

        bool CheckRequestComm(AnsiString strReq);                               //確認command格式//

        bool SendRequestComm(AnsiString strReq);                                //Send command//
        bool SendRequestComm_D(int iSegment);                                   //Send command//
        bool WriteRequestComm_D(int iSegment);
        bool SendRequestComm_X(int iSegment);                                   //Send command//
        bool SendRequestComm_M(int iSegment);                                   //Send command//
        bool WriteRequestComm_M(int iSegment);
        bool SendRequestComm_Y(int iSegment);
        bool WriteRequestComm_Y(int iSegment);

        unsigned int iRegisterX[Register_X_Max];
        unsigned int iRegisterM[Register_M_Max];
        unsigned int iRegisterD[Register_D_Max];
        unsigned int iRegisterY[Register_Y_Max];

};
//---------------------------------------------------------------------------
#endif
