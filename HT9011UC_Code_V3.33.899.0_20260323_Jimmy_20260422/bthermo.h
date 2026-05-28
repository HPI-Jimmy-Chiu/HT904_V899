//---------------------------------------------------------------------------
#ifndef bthermoH
#define bthermoH
//---------------------------------------------------------------------------
void DoThermo();
void DoSetThermo();
bool bGetHeaterUsed(int Addr);
bool bGet16HeaterUsedTo4Heater(int Addr);   //JerryYang 20160518 矽格湖口 柏均要求16組加熱器要能支援GPIB4組加熱器的指令
extern int iThermoTask;
void ClearAllHotBuffer();
void DoTemptureControl();           //溫控器新架構控制流程
void DoThermoReal();
void DoSetSVOfOmronEJ1N();          //Steven 20120220 : Omron EJ1N溫控器
void DoATC60Temperature();          //ChungHung 21041105 add for ATC6.0 and ATC3.0
double DOUN150ReadTemp(int Addr);   //Ifor 20160507 add ATC 4、8、32 Heat 溫度整合
void DoSetSVOfDTME08();             //JimmyChiu 20210923 : DTME08溫控器
void CheckLBTemp();                 //JerryYang 20220923 : LB吹氣function
double GetFactSetTemp(int Addr, double T);
#endif

