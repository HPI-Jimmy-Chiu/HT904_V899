#include "MachineDefine.h"
#pragma hdrstop

#include "MessageDef.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

AnsiString GPIBVersion="12.13.883";
AnsiString RS232Version="12.13.883";
double GPIBVersionCheck=12.13;                                                  //Steven 20191007 : 改成判斷兩組版號, 所以使用Double
double TTLRS232VerCheck=10051302;                                               //Isaac 20210511 : TTLRS232板子版本檢查

#define X(idx,name,str,note) const unsigned int name = idx;
MSG_CMD_LIST(X)
#undef X
const char * const MSG_CMD_NAMES[] =
{
#define X(idx,name,str,note) str,
MSG_CMD_LIST(X)
#undef X
};
const int MSG_CMD_COUNT = (int)(sizeof(MSG_CMD_NAMES) / sizeof(MSG_CMD_NAMES[0]));

//For Handler----------
VM *HGpib2Handler;
MV HHandler2Gpib;

//For GPIB-------------
VM GGpib2Handler;
MV *GHandler2Gpib;
//---------------------
VM1 *RefeshReceve;
MV1 RefeshSend;
VM2 *CCDReceve;
MV2 CCDSend;
