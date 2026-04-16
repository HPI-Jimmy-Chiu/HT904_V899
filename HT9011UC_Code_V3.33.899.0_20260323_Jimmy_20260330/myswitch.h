//---------------------------------------------------------------------------
#ifndef myswitchH
#define myswitchH
//---------------------------------------------------------------------------

class TMySwitch{
    public:
    _fastcall TMySwitch::TMySwitch();
    AnsiString Name;
    AnsiString Using;
    int  Ring;
    int  IP;
    int  Port;
    int  Bit;
    bool OutValue;
    bool SetValue;
    int  Type;
    void On();
    void Off();
    bool Status();
    bool Enable;
    void OnOff(bool);
    int  ISABase;
};
#define MAX_SWITCH_ITEM 370
#define MAX_TTL_SWITCH  16                                                      //Steven 20161011 : TTL¤ä´©8Site
extern class TMySwitch SW[MAX_SWITCH_ITEM];
extern class TMySwitch SWBackup[MAX_TTL_SWITCH];                                //Alick 20161011 (Steven) : TTL¤ä´©8Site
void InitialSwitch();
void CopySwitch(TMySwitch *Source, TMySwitch *Target);
#endif
