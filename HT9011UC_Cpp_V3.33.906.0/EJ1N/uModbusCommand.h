// =============================================================================
//  uModbusCommand.h  --  Modbus RTU/TCP master frame encode/decode helper
//                         (little state machine of its own: StartAddress/
//                         SlaveID/bisWrite + a fixed-size iListValue[] value
//                         list).
//
//  Faithful translation of golden EJ1N/uModbusCommand.h (58 lines, BCB6,
//  Big5/cp950 -- this header carries no Chinese comments).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 group "ej1n".  Fourth (and dependency-wise
//  FIRST-NEEDED) EJ1N/ file: uDTME08Control.cpp's global `uModbusCommand um;`
//  and every DoGet*/DoSet* body needs this class's real methods, so this
//  file's own contract was read in full before uDTME08Control.cpp was
//  written, even though it is translated last in the assigned file order.
//
//  ROLE: pure Modbus function-code-3/6/16 frame builder + CRC16/decode
//  helper. No socket, no VCL, no state machine cursor -- StartAddress/
//  SlaveID/bisWrite/iListValue[]/iListValueLen are plain caller-driven state,
//  read and written directly by uDTME08Control's DoGet*/DoSet* family.
//
//  WAVE SCOPE -- ACTIVE vs golden-dead-declaration:
//   ACTIVE (faithful, verbatim declarations): every member EXCEPT
//     GetClientDecodeReadSingleRTU (see GOLDEN QUIRK below).
//   GOLDEN QUIRK, preserved, NOT a gate: `int
//     GetClientDecodeReadSingleRTU(char* cSet,int &iLen);` (golden :41) is
//     declared but has NO DEFINITION anywhere in golden's OWN
//     uModbusCommand.cpp (confirmed: golden's 160-line .cpp defines every
//     other declared member and never mentions this name at all) and is
//     never CALLED anywhere in golden EJ1N/ either (grepped
//     uDTME08Control.cpp/MyOmronPanel.cpp/uSocketServerClient.cpp/
//     uModbusCommand.cpp -- zero call sites). Golden itself ships this as a
//     declared-but-unimplemented, declared-but-uncalled symbol -- legal in
//     BCB6 (and in this port) precisely BECAUSE nothing ever ODR-uses it, so
//     the linker never needs a definition. Declared here, verbatim,
//     undefined here too -- reproducing golden's own dead declaration is the
//     faithful outcome, not a gap this translation introduces.
//
//  VCL/Borland conversions: `#include <StdCtrls.hpp>` (golden :6) dropped --
//  grepped this header's own declarations end-to-end and NOTHING in this
//  class touches a StdCtrls type (no TEdit/TButton/... appears anywhere in
//  uModbusCommand.h or .cpp); golden's own include is a dead carry-over
//  (this file has no VCL surface at all), same "unused-include, drop it"
//  judgment EJ1N/TextProcess.h already made for its own golden `<map>`.
//  `byte` (golden :52-53) resolves via <windows.h> -> rpcndr.h, reached
//  through MachineDefine.h, same as every other EJ1N unit.
//
//  Big5: this golden header has ZERO Chinese comments (confirmed by decoding
//  the raw bytes as cp950 and inspecting every line) -- nothing to preserve,
//  nothing to gate on U+FFFD.
// =============================================================================

// BCB6 ORIGINAL header (mirrored as a comment for provenance):
//   //---------------------------------------------------------------------------
//   #ifndef uModbusCommandH / #define uModbusCommandH
//   #include <StdCtrls.hpp>   -- NOT included, see banner above (dead/unused)
// =============================================================================

//---------------------------------------------------------------------------

#ifndef uModbusCommandH
#define uModbusCommandH

#include "vclcompat/vcl_compat.h"   // AnsiString (unused by this class directly,
                                    //   kept for house-style parity with every
                                    //   other EJ1N header); byte via <windows.h>

//---------------------------------------------------------------------------
#define CommLength 255
#define MaxListValue 10
//---------------------------------------------------------------------------
class uModbusCommand
{
private:
    int iDataCount;
public: // User declarations
    uModbusCommand();
    ~uModbusCommand();
    void Clear();
    //---------------------------------------------------------------------------
    //Encode
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------
    //TCP
    //---------------------------------------------------------------------------
    void GetClientEncodeSingleTCP(char* cSet,int &iLen);
    void GetClientEncodeWriteMultipleTCP(char* cSet,int &iLen);
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------

    //---------------------------------------------------------------------------
    //RTU
    //---------------------------------------------------------------------------
    void GetClientEncodeSingleRTU(char* cSet,int &iLen);
    unsigned int CRC_Check(unsigned char *ary, unsigned int len);
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------

    //---------------------------------------------------------------------------
    //Decode
    //---------------------------------------------------------------------------
    //AI(W906-PT-W2) 20260807: GOLDEN QUIRK -- declared, never defined anywhere
    //  in golden, never called anywhere in golden EJ1N/. See this header's own
    //  banner. Kept declared, deliberately left undefined here too.
    int GetClientDecodeReadSingleRTU(char* cSet,int &iLen);
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------
    int StartAddress;
    int SlaveID;
    bool bisWrite;
    //Receive Data Decode
    int iListValue[MaxListValue];
    int iListValueLen;
    void DecodeToListIntRTU(char* cSet,int iLen);
    void DecodeToListIntTCP(char* cSet,int iLen);
    int  TwoBytes2Int(byte _H,byte _L);
    int TwoBytes2Short(byte _H , byte _L);      //Jimmychiu 20230216 新增可讀寫負數
    void AddListValue(int ivalue);
    void ClearListValue();
    int GetDataLength(int HexData);
};
#endif
