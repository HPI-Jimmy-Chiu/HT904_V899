//---------------------------------------------------------------------------
#ifndef mysensorH
#define mysensorH
//---------------------------------------------------------------------------
//#include "MachineDefine.h"
#include "MachineType.h"            //Steven 20130809 : OK

bool GetTrayBuildState(int pos);
void SetTrayBuildState(int pos);
void ClrTrayBuildState(int pos);
class TMySensor{
    public:
    _fastcall TMySensor::TMySensor();
    _fastcall TMySensor::~TMySensor();
    AnsiString Name;
//    AnsiString OnCode;
//    AnsiString OffCode;
    AnsiString Using;
//    int  XPos;
//    int  YPos;
    int  Ring;
    int  IP;
    int  Port;
    int  Bit;
    int  Type;
    int  State;
    bool Status();
    bool IsOn();
    bool IsOff();
    bool Enable;
    int  ISABase;
//    bool CheckSenPortRangeErr(int Type);
};
extern class TMySensor Sen[MAX_SENSOR_ITEM];
extern class TMySensor SenBackUp[MAX_TTL_BIT];                                  //Alick 20161011 (Steven) : TTL¤ä´©8Site
//void InitialSensorName();
void CopySensor(TMySensor *Source ,TMySensor *Target);
#endif
