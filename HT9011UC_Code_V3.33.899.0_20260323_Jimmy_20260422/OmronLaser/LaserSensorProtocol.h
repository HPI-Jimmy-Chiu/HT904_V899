//---------------------------------------------------------------------------

#ifndef LaserSensorProtocolH
#define LaserSensorProtocolH
//---------------------------------------------------------------------------


//------------------------------------------------------------------------------
struct st0101SendCommend
{
    st0101SendCommend(int Addr, AnsiString Name);
    AnsiString  _DeviceName;
    AnsiString  _SendMessage;
    AnsiString  _SendMessASCII;
    int         _Head;
    AnsiString  _MRCSRC;
    AnsiString  _VariClass;
    int         _Address;
    int         _BitPosition;
    int         _ElementNo;

    void SetSendMessage(AnsiString VC, int _ElementNo);
};
//------------------------------------------------------------------------------
struct st0101RecvCommend
{
    st0101RecvCommend(int Addr, AnsiString Name);
    AnsiString  _DeviceName;
    AnsiString  _SendMessage;
    AnsiString  _SendMessASCII;
    int         _Head;
    AnsiString  _MRCSRC;
    AnsiString  _VariClass;
    int         _Address;
    int         _BitPosition;
    int         _ElementNo;

//    void SetSendMessage(AnsiString VC, int _ElementNo);
};
//------------------------------------------------------------------------------

#endif
