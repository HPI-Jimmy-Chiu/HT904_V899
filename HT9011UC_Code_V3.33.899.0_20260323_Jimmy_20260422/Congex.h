#ifndef CongexBCBH
#define CongexBCBH

 //設定Rs232的BaudRate (Default:115200)
void SetBaudRate(int Value);

 //設定Rs232的同位元檢查 (Default:Parity.None)
void SetParity(int Value);

//設定Rs232的資料bit (Default:8)
void SetDatabit(int Value);

 //設定Rs232的停止位元數 (Default:StopBits.One)
void SetStopBit(int Value);


//開啟COM Port 並送出讀取檔案,回傳是否完成
String openSerialSendLoadFile(string comP, string FileName);

#endif