#include "MachineDefine.h"
#pragma hdrstop

#include "LaserSensorProtocol.h"
#include "Common.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
st0101SendCommend::st0101SendCommend(int Addr, AnsiString Name)
{
    _DeviceName     =Name;
    _SendMessage    ="";
    _SendMessASCII  ="";
    _Head           =0;
    _MRCSRC         ="0101";
    _VariClass      ="E1";
    _Address        =Addr;
    _BitPosition    =0;
    _ElementNo      =1;
}
//---------------------------------------------------------------------------
void st0101SendCommend::SetSendMessage(AnsiString VC, int _ElementNo)
{
//     1     2    3  4    5  6
//[STX]00000 0101 C5 0001 00 0001[ETX][BCC]
// 1 : Node No. + Sub-Address + SID  (00000)
// 2 : MRC SRC
// 3 : Variable classification
// 4 : Read-out start address
// 5 : Bit position (00)
// 6 : The number of elements

    int BCC=0;
    AnsiString Temp;

              //STX 1   2 3 4   5   6   ETX
    Temp.sprintf("%c%05X%s%s%04X%02X%04X%c", STX, _Head, _MRCSRC, _VariClass, _Address, _BitPosition, _ElementNo, ETX);
    BCC=SetBCC(Temp);
    _SendMessage.sprintf("%c%s%c%c\r\n", STX, Temp, ETX, BCC);
    _SendMessASCII.sprintf("%s SEND: [STX]%s[ETX](0x%02X)", _DeviceName, Temp, BCC);
}
//---------------------------------------------------------------------------

