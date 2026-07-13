//---------------------------------------------------------------------------
//  SECSGEM/SecsWireCodec.cpp -- SECS-II wire-level codec primitives
//
//  Translation wave: W5-Final (SECSGEM wire codec)
//  Translator: AI(W5-Final-SecsWireCodec) 20260711
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemEquipment.cpp
//  (per-function golden line citations are given at each function below).
//
//  SCOPE DISCREPANCY (flag per task instructions, item 8): the task's
//  original line range for the DataItemInNew/DataItemIn pair was
//  "~7114-7134". Reading golden confirms DataItemInNew (:7114-7122) directly
//  calls `GetDataItemLenAndType(len, Type)` (golden :7290-7296), a public
//  wrapper around the private `GetDataItemLenAndTypeSub` (golden :2440-2447)
//  -- NEITHER of which was in the task's enumerated function list. Both are
//  trivial (~7 lines each), touch ONLY `SReceiveData` + `iReturnCode` (both
//  already in this unit's scope per the task's own dependency list), and are
//  a genuine, unavoidable prerequisite for DataItemInNew to do anything at
//  all (without them DataItemInNew would have to be gated). Per the "verify
//  everything yourself" instruction, both are included here as faithful
//  translations rather than gating DataItemInNew -- flagged for review.
//
//  vclcompat NOTE: golden repeats `SReceiveData->Strings[i].c_str()`
//  everywhere. vclcompat's TStringList `Strings[i]` returns a `StringsProxy`
//  (an operator-AnsiString() proxy, not an AnsiString), which has no
//  `.c_str()` member -- calling `.c_str()` directly on it does not compile.
//  Translated throughout this file as `SReceiveData->GetString(i).c_str()`,
//  the same substitution already established at mycylin.cpp:826/844 (see
//  that file's own inline comment) -- purely a vclcompat-proxy accommodation,
//  zero behavior change.
//---------------------------------------------------------------------------
#include "SECSGEM/SecsWireCodec.h"
#include <cstdlib>   // atoi/atof
#include <cstring>   // strlen/strncpy

//---------------------------------------------------------------------------
//  HType -- the shared global SECS-II format-byte table (see .h file-head
//  note "SHARED GLOBAL: HType"). Golden defines the (uninitialized-by-
//  declaration) global at uHGemEquipment.cpp:22 and seeds it once via
//  THGem::InitHType() (golden :353-368), called from the THGem constructor.
//
//  AI(W5-Final-SecsWireCodec) 20260711 JUDGMENT CALL (flag for review):
//  THGem does not exist in this unit's scope, so there is no constructor to
//  hang InitHType()'s call on. Seeded instead via a file-static initializer
//  object (g_HTypeInit below), which C++ guarantees runs once, before any
//  use of HType from a static-init context, and -- because HType and
//  g_HTypeInit are both defined in THIS translation unit -- runs strictly
//  after HType's own (zero-fill) initialization, in declaration order.
//  Alternative considered and rejected: reseed HType from every
//  SecsWireCodec constructor call (redundant -- HType is process-global
//  truth shared by every instance, exactly as in golden where every THGem
//  instance would stomp the same global with the same values; reseeding
//  on each construction also could not deterministically order itself
//  against pre-main() consumers the way a static initializer object does).
//---------------------------------------------------------------------------
struct HTypeStruct HType;

namespace {
struct HTypeInitializer
{
    HTypeInitializer()
    {
        // golden uHGemEquipment.cpp:355-367, literal values verbatim.
        HType.LIST_TYPE    = 0x00;
        HType.ASCII_TYPE   = 0x40;
        HType.JIS_TYPE     = 0x44;
        HType.BINARY_TYPE  = 0x20;
        HType.BOOLEAN_TYPE = 0x24;
        HType.INT_1_TYPE   = 0x64;
        HType.INT_2_TYPE   = 0x68;
        HType.INT_4_TYPE   = 0x70;
        HType.INT_8_TYPE   = 0x60;
        HType.UINT_1_TYPE  = 0xa4;
        HType.UINT_2_TYPE  = 0xa8;
        HType.UINT_4_TYPE  = 0xb0;
        HType.UINT_8_TYPE  = 0xa0;
        HType.FT_4_TYPE    = 0x90;
        HType.FT_8_TYPE    = 0x80;
        // HType.VCL_TYPE: declared in golden HTypeStruct (uHGemEquipment.h:58)
        // but NEVER assigned by golden InitHType() (uHGemEquipment.cpp:353-368)
        // and never compared against by any of this file's functions --
        // faithfully left unset here too (value-initialized to 0 by HType's
        // own zero-fill; the gap itself is the faithful behavior).
    }
};
HTypeInitializer g_HTypeInit;
} // anonymous namespace

//---------------------------------------------------------------------------
SecsWireCodec::SecsWireCodec()
    : LocalBuffer(64u * 1024u * 1024u, 0),
      LocalLength(0),
      LocalLength_4(4),   // see ResetLocalBuffer()'s comment: golden's
                           // CreateLocalHead() (out of scope) reserves the
                           // first 4 bytes for the message-length header
                           // before any DataItemOut() call runs; primed here
                           // so a freshly-constructed codec is immediately
                           // usable for an encode burst.
      SReceiveData(new TStringList()),
      WaitShowString(new TStringList()),
      LogDataString(new TStringList()),
      iReturnCode(1)      // golden convention: "初始值=1" (see every
                           // DataItemIn*/GetDataItemLenAndType* golden
                           // comment) -- sticky-error-code carrier starts
                           // clean.
{
}

SecsWireCodec::~SecsWireCodec()
{
    delete SReceiveData;
    delete WaitShowString;
    delete LogDataString;
}

//---------------------------------------------------------------------------
// ADDITIVE (not golden) -- see .h declaration comment.
//---------------------------------------------------------------------------
void SecsWireCodec::ResetLocalBuffer()
{
    LocalLength = 0;
    LocalLength_4 = 4;
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:392-396
//   void __fastcall THGem::StringOut(AnsiString S)
//   {
//       WaitShowString->Add(S);
//       LogDataString->Add(S);
//   }
//---------------------------------------------------------------------------
void SecsWireCodec::StringOut(AnsiString S)
{
    WaitShowString->Add(S);
    LogDataString->Add(S);
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:917-932
// Returns how many bytes one item of `Type` occupies (1/2/4/8), or 0 (+ a
// StringOut diagnostic) for an unrecognized format byte.
//---------------------------------------------------------------------------
int SecsWireCodec::GetLengthOfType(unsigned char Type)
{
    if (Type == HType.LIST_TYPE  || Type == HType.ASCII_TYPE  || Type == HType.BINARY_TYPE ||
        Type == HType.BOOLEAN_TYPE || Type == HType.UINT_1_TYPE || Type == HType.INT_1_TYPE)
        return 1;
    else if (Type == HType.UINT_2_TYPE || Type == HType.INT_2_TYPE)
        return 2;
    else if (Type == HType.UINT_4_TYPE || Type == HType.INT_4_TYPE || Type == HType.FT_4_TYPE)
        return 4;
    else if (Type == HType.UINT_8_TYPE || Type == HType.INT_8_TYPE || Type == HType.FT_8_TYPE)
        return 8;
    else
    {
        StringOut("SECSII format byte not define error?");
        return 0;
    }
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:941-974
// Encodes `len` as 1-3 big-endian bytes into Ptr[0..], returns the byte
// count (the SML "Length of Byte" bit-field value, 1/2/3). >3-byte lengths
// are a hard error (golden: StringOut + return 0 -- never happens for any
// real SECS message, since len<=0xFFFFFF).
//---------------------------------------------------------------------------
unsigned char SecsWireCodec::GetLengthByte(unsigned len, unsigned char *Ptr)
{
    unsigned char ct = 0;
    unsigned char Temp[3];
    do
    {
        if (ct >= 3)
        {
            StringOut("data length byte over 3 error");
            return 0;
        }
        Ptr[ct] = (unsigned char)(len & 0xff);
        len >>= 8;
        ct++;
    } while (len > 0);

    if (ct == 2)                    // put MSB first
    {
        Temp[0] = Ptr[1];
        Temp[1] = Ptr[0];
        Ptr[0] = Temp[0];
        Ptr[1] = Temp[1];
    }
    else if (ct == 3)                // put MSB first
    {
        Temp[0] = Ptr[2];
        Temp[1] = Ptr[1];
        Temp[2] = Ptr[0];
        Ptr[0] = Temp[0];
        Ptr[1] = Temp[1];
        Ptr[2] = Temp[2];
    }
    return ct;
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:986-1183
// Encodes one SECS-II data item (Format byte | Length-of-length bits,
// followed by the length bytes, followed by `len` values of `Type`) into
// LocalBuffer starting at LocalLength_4, then rewrites the 4-byte message
// length header at LocalBuffer[0..3] from the new total.
//---------------------------------------------------------------------------
void SecsWireCodec::DataItemOut(int len, unsigned char Type, void *P)
{
    int i, k;
    unsigned char SMLLength;
    unsigned char SMLLengthData[3];
    unsigned char DataSize;

    DataSize = (unsigned char)GetLengthOfType(Type);
    SMLLength = GetLengthByte((unsigned)len * DataSize, SMLLengthData);
    LocalBuffer[LocalLength_4] = (unsigned char)(Type | SMLLength);
    LocalLength_4++;
    for (i = 0; i < SMLLength; i++)
        LocalBuffer[LocalLength_4 + (unsigned)i] = SMLLengthData[i];
    LocalLength_4 += SMLLength;

    if (Type == HType.ASCII_TYPE || Type == HType.BINARY_TYPE)
    {
        unsigned char *ptr;
        ptr = (unsigned char *)P;
        for (i = 0; i < len; i++)
            LocalBuffer[LocalLength_4 + (unsigned)i] = ptr[i];
        LocalLength_4 += (unsigned)len;
    }
    else if (Type == HType.BOOLEAN_TYPE)
    {
        bool *ptr;
        ptr = (bool *)P;
        for (i = 0; i < len; i++)
        {
            if (ptr[i] == true)
                LocalBuffer[LocalLength_4] = 1;
            else
                LocalBuffer[LocalLength_4] = 0;
            LocalLength_4++;
        }
    }
    else if (Type == HType.UINT_4_TYPE)
    {
        unsigned int *ptr, j;
        ptr = (unsigned int *)P;
        for (i = 0; i < len; i++)
        {
            j = ptr[i];
            for (k = 0; k < DataSize; k++)
            {
                LocalBuffer[LocalLength_4 + (unsigned)(DataSize - 1 - k)] = (unsigned char)(j & 0xff);
                j >>= 8;
            }
            LocalLength_4 += DataSize;
        }
    }
    else if (Type == HType.UINT_2_TYPE)
    {
        unsigned short *ptr, j;
        ptr = (unsigned short *)P;
        for (i = 0; i < len; i++)
        {
            j = ptr[i];
            for (k = 0; k < DataSize; k++)
            {
                LocalBuffer[LocalLength_4 + (unsigned)(DataSize - 1 - k)] = (unsigned char)(j & 0xff);
                j = (unsigned short)(j >> 8);
            }
            LocalLength_4 += DataSize;
        }
    }
    else if (Type == HType.UINT_1_TYPE)
    {
        unsigned char *ptr, j;
        ptr = (unsigned char *)P;
        for (i = 0; i < len; i++)
        {
            j = ptr[i];
            for (k = 0; k < DataSize; k++)
            {
                LocalBuffer[LocalLength_4 + (unsigned)(DataSize - 1 - k)] = (unsigned char)(j & 0xff);
                // Steven 20150814 (golden): UINT_1/INT_1 are always 1 byte
                // long, so no right-shift is needed here (faithfully omitted).
            }
            LocalLength_4 += DataSize;
        }
    }
    else if (Type == HType.UINT_8_TYPE)
    {
        unsigned __int64 *ptr, j;
        ptr = (unsigned __int64 *)P;
        for (i = 0; i < len; i++)
        {
            j = ptr[i];
            for (k = 0; k < DataSize; k++)
            {
                LocalBuffer[LocalLength_4 + (unsigned)(DataSize - 1 - k)] = (unsigned char)(j & 0xff);
                j >>= 8;
            }
            LocalLength_4 += DataSize;
        }
    }
    else if (Type == HType.INT_1_TYPE)
    {
        char *ptr, j;
        ptr = (char *)P;
        for (i = 0; i < len; i++)               // len entries in the array
        {
            j = ptr[i];
            for (k = 0; k < DataSize; k++)
            {
                LocalBuffer[LocalLength_4 + (unsigned)(DataSize - 1 - k)] = (unsigned char)(j & 0xff);
                // Steven 20150814 (golden): UINT_1/INT_1 are always 1 byte
                // long, so no right-shift is needed here (faithfully omitted).
            }
            LocalLength_4 += DataSize;
        }
    }
    else if (Type == HType.INT_2_TYPE)
    {
        short *ptr, j;
        ptr = (short *)P;
        for (i = 0; i < len; i++)
        {
            j = ptr[i];
            for (k = 0; k < DataSize; k++)
            {
                LocalBuffer[LocalLength_4 + (unsigned)(DataSize - 1 - k)] = (unsigned char)(j & 0xff);
                j = (short)(j >> 8);
            }
            LocalLength_4 += DataSize;
        }
    }
    else if (Type == HType.INT_4_TYPE)
    {
        int *ptr, j;
        ptr = (int *)P;
        for (i = 0; i < len; i++)
        {
            j = ptr[i];
            for (k = 0; k < DataSize; k++)
            {
                LocalBuffer[LocalLength_4 + (unsigned)(DataSize - 1 - k)] = (unsigned char)(j & 0xff);
                j >>= 8;
            }
            LocalLength_4 += DataSize;
        }
    }
    else if (Type == HType.INT_8_TYPE)
    {
        __int64 *ptr, j;
        ptr = (__int64 *)P;

        for (i = 0; i < len; i++)
        {
            j = ptr[i];
            for (k = 0; k < DataSize; k++)
            {
                LocalBuffer[LocalLength_4 + (unsigned)(DataSize - 1 - k)] = (unsigned char)(j & 0xff);
                j >>= 8;
            }
            LocalLength_4 += DataSize;
        }
    }
    else if (Type == HType.FT_4_TYPE)
    {
        float *floatPtr;
        floatPtr = (float *)P;
        unsigned char *ptr;
        for (i = 0; i < len; i++)
        {
            ptr = (unsigned char *)(&floatPtr[i]);
            for (k = 0; k < DataSize; k++)
            {
                LocalBuffer[LocalLength_4 + (unsigned)(DataSize - 1 - k)] = ptr[0];
                ptr++;
            }
            LocalLength_4 += DataSize;
        }
    }
    else if (Type == HType.FT_8_TYPE)
    {
        double *doublePtr;
        unsigned char *ptr;
        doublePtr = (double *)P;
        for (i = 0; i < len; i++)
        {
            ptr = (unsigned char *)(&doublePtr[i]);
            for (k = 0; k < DataSize; k++)
            {
                LocalBuffer[LocalLength_4 + (unsigned)(DataSize - 1 - k)] = *ptr;
                ptr++;
            }
            LocalLength_4 += DataSize;
        }
    }

    LocalLength = LocalLength_4 - 4;
    k = (int)LocalLength;
    for (i = 0; i < 4; i++)
    {
        LocalBuffer[(unsigned)(3 - i)] = (unsigned char)(k & 0xff);
        k >>= 8;
    }
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:979-984
//   void THGem::DataItemOut(unsigned char Type, AnsiString S)
//   {
//       char *Ptr;
//       Ptr=S.c_str();
//       DataItemOut(strlen(Ptr), Type, Ptr);
//   }
// AI(W5-Final-SecsWireCodec) 20260711: golden assigns AnsiString::c_str()'s
// `const char*` result into a raw (non-const) `char *Ptr` -- BCB6 accepted
// this const-discarding assignment; standard C++ does not. Declared `Ptr` as
// `const char*` here and const_cast only at the void* call boundary (the
// ASCII_TYPE/BINARY_TYPE branch of the 3-arg overload only ever READS
// through the pointer, never writes) -- zero behavior change, pure
// const-correctness accommodation.
//---------------------------------------------------------------------------
void SecsWireCodec::DataItemOut(unsigned char Type, AnsiString S)
{
    const char *Ptr;
    Ptr = S.c_str();
    DataItemOut((int)strlen(Ptr), Type, const_cast<char *>(Ptr));
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:1235-1244
// Reads back `TypeChar`'s length-of-length bits (bit0,1) worth of big-endian
// bytes starting at Ptr[RunLength] -- the inverse arithmetic of
// GetLengthByte, used when walking a received SML byte stream. Does NOT
// mutate RunLength (a plain by-value parameter, not a reference, faithfully
// preserved from golden's own `int RunLength` signature).
//---------------------------------------------------------------------------
int SecsWireCodec::GetSMLLenthByte(unsigned char TypeChar, unsigned char *Ptr, int RunLength)
{
    int len = TypeChar & 0x03;
    int ct = 0;
    for (int i = 0; i < len; i++)
    {
        ct <<= 8;
        ct += Ptr[RunLength + i];
    }
    return ct;
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:2225-2432
// Consumes ("Type","len",value,value,...) decimal-string tokens from the
// FRONT of SReceiveData and writes `len` values of `Type` into *P.
//   return -1 : wrong Type token, or SReceiveData ran dry
//   return -2 : wrong length token (ASCII: declared len > caller's buffer;
//               everything else: declared len != caller's len)
//   return  1 : ok
//---------------------------------------------------------------------------
int SecsWireCodec::DataItemInSub(int len, unsigned char Type, void *P)
{
    unsigned char t;
    int l, i;

    if (SReceiveData->Count == 0)
        return -1;
    t = (unsigned char)atoi(SReceiveData->GetString(0).c_str());   // Type

    if (t != Type)
        return -1;
    SReceiveData->Delete(0);
    if (SReceiveData->Count == 0)
        return -1;
    l = atoi(SReceiveData->GetString(0).c_str());                  // length of data

    if (t == HType.ASCII_TYPE)               // guard against caller's buffer being too small
    {
        if (l > len)
            return -2;
    }
    else
    {
        if (len != l)
            return -2;
    }
    SReceiveData->Delete(0);                 // length checked out from here on

    if (t == HType.LIST_TYPE)
    {
        return 1;
    }
    else if (t == HType.ASCII_TYPE)
    {
        char *temp;
        temp = (char *)P;
        if (SReceiveData->Count == 0)
            return -1;

        // JerryYang 20230112 (golden): len -> len+1, so the copied ASCII
        // payload keeps its NUL terminator (the earlier len-only strncpy
        // dropped the string's closing byte).
        strncpy(temp, SReceiveData->GetString(0).c_str(), (size_t)(len + 1));
        SReceiveData->Delete(0);
    }
    else if (t == HType.BINARY_TYPE)
    {
        unsigned char *temp;
        temp = (unsigned char *)P;

        for (i = 0; i < len; i++)
        {
            if (SReceiveData->Count != 0)     // Joseph 20221111 (golden): guard against underrun before Delete(0)
            {
                temp[i] = (unsigned char)atoi(SReceiveData->GetString(0).c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if (t == HType.BOOLEAN_TYPE)
    {
        bool *temp;
        temp = (bool *)P;
        for (i = 0; i < len; i++)
        {
            if (SReceiveData->Count != 0)     // Joseph 20221111 (golden): guard against underrun before Delete(0)
            {
                if (atoi(SReceiveData->GetString(0).c_str()) == 1)
                    temp[i] = true;
                else
                    temp[i] = false;
                SReceiveData->Delete(0);
            }
        }
    }
    else if (t == HType.INT_1_TYPE)
    {
        char *temp;
        temp = (char *)P;
        for (i = 0; i < len; i++)
        {
            if (SReceiveData->Count != 0)     // Joseph 20221111 (golden): guard against underrun before Delete(0)
            {
                temp[i] = (char)atoi(SReceiveData->GetString(0).c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if (t == HType.INT_2_TYPE)
    {
        short *temp;
        temp = (short *)P;
        for (i = 0; i < len; i++)
        {
            if (SReceiveData->Count != 0)     // Joseph 20221111 (golden): guard against underrun before Delete(0)
            {
                temp[i] = (short)atoi(SReceiveData->GetString(0).c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if (t == HType.INT_4_TYPE)
    {
        int *temp;
        temp = (int *)P;
        for (i = 0; i < len; i++)
        {
            if (SReceiveData->Count != 0)     // Joseph 20221111 (golden): guard against underrun before Delete(0)
            {
                temp[i] = atoi(SReceiveData->GetString(0).c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if (t == HType.INT_8_TYPE)
    {
        __int64 *temp;
        temp = (__int64 *)P;
        for (i = 0; i < len; i++)
        {
            if (SReceiveData->Count != 0)     // Joseph 20221111 (golden): guard against underrun before Delete(0)
            {
                temp[i] = atoi(SReceiveData->GetString(0).c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if (t == HType.UINT_1_TYPE)
    {
        unsigned char *temp;
        temp = (unsigned char *)P;
        for (i = 0; i < len; i++)
        {
            if (SReceiveData->Count != 0)     // Joseph 20221111 (golden): guard against underrun before Delete(0)
            {
                temp[i] = (unsigned char)atoi(SReceiveData->GetString(0).c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if (t == HType.UINT_2_TYPE)
    {
        unsigned short *temp;
        temp = (unsigned short *)P;
        for (i = 0; i < len; i++)
        {
            if (SReceiveData->Count != 0)     // Joseph 20221111 (golden): guard against underrun before Delete(0)
            {
                temp[i] = (unsigned short)atoi(SReceiveData->GetString(0).c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if (t == HType.UINT_4_TYPE)
    {
        unsigned *temp;
        temp = (unsigned *)P;
        for (i = 0; i < len; i++)
        {
            if (SReceiveData->Count != 0)     // Joseph 20221111 (golden): guard against underrun before Delete(0)
            {
                temp[i] = (unsigned)atoi(SReceiveData->GetString(0).c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if (t == HType.UINT_8_TYPE)
    {
        unsigned __int64 *temp;
        temp = (unsigned __int64 *)P;
        for (i = 0; i < len; i++)
        {
            if (SReceiveData->Count != 0)     // Joseph 20221111 (golden): guard against underrun before Delete(0)
            {
                temp[i] = (unsigned __int64)atoi(SReceiveData->GetString(0).c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if (t == HType.FT_4_TYPE)
    {
        float *temp;
        temp = (float *)P;
        for (i = 0; i < len; i++)
        {
            if (SReceiveData->Count != 0)     // Joseph 20221111 (golden): guard against underrun before Delete(0)
            {
                temp[i] = (float)atof(SReceiveData->GetString(0).c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else if (t == HType.FT_8_TYPE)
    {
        double *temp;
        temp = (double *)P;
        for (i = 0; i < len; i++)
        {
            if (SReceiveData->Count != 0)     // Joseph 20221111 (golden): guard against underrun before Delete(0)
            {
                temp[i] = atof(SReceiveData->GetString(0).c_str());
                SReceiveData->Delete(0);
            }
        }
    }
    else
    {
        return -1;
    }
    return 1;
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:2440-2447 (private in golden -- peek-only: does
// NOT Delete from SReceiveData, unlike GetDataItemLenAndTypeAndDeleteSub,
// which is out of this unit's scope).
//---------------------------------------------------------------------------
int SecsWireCodec::GetDataItemLenAndTypeSub(int &len, unsigned char &Type)
{
    if (SReceiveData->Count < 2)
        return -2;
    Type = (unsigned char)atoi(SReceiveData->GetString(0).c_str());  // Type
    len  = atoi(SReceiveData->GetString(1).c_str());                 // length of data
    return 1;
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:7290-7296
//---------------------------------------------------------------------------
int SecsWireCodec::GetDataItemLenAndType(int &len, unsigned char &Type)
{
    int ret;
    ret = GetDataItemLenAndTypeSub(len, Type);
    if (iReturnCode == 1)
        iReturnCode = ret;
    return ret;
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:7114-7122
// Peeks the next token's (len,Type) then decodes+consumes it into an
// AnsiString via the AnsiString-typed DataItemIn overload below.
//---------------------------------------------------------------------------
int SecsWireCodec::DataItemInNew(AnsiString &P)
{
    int len;
    unsigned char Type;
    GetDataItemLenAndType(len, Type);
    int ret = DataItemIn(len, Type, P);
    if (iReturnCode == 1)
        iReturnCode = ret;
    return ret;
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:7125-7130
//---------------------------------------------------------------------------
int SecsWireCodec::DataItemIn(int len, unsigned char Type, void *P)
{
    int ret;
    ret = DataItemInSub(len, Type, P);
    if (iReturnCode == 1)
        iReturnCode = ret;
    return ret;
}

//---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:7134-7281
// Decodes exactly one scalar item of `t`/`len` into an AnsiString: ASCII as
// text, every numeric format via its native C type then AnsiString's
// number-formatting operator=.
//---------------------------------------------------------------------------
int SecsWireCodec::DataItemIn(int len, unsigned char t, AnsiString &Str)
{
    int ret;

    if (len <= 0)                    // JerryYang 20250725 (golden): guard added
    {
        iReturnCode = 0;
        Str = "";
        return 0;
    }

    if (t == HType.ASCII_TYPE)
    {
        // Steven 20250726 (golden): dynamically-sized buffer (was a fixed
        // char[10240] before JerryYang 20240328's earlier fix); +100 slack
        // preserved verbatim from golden.
        char *P;
        P = new char[(size_t)len + 100];
        ret = DataItemInSub(len, t, P);
        if (ret == 1)
        {
            Str.sprintf("%s", P);
            iReturnCode = ret;
        }
        delete[] P;
        P = NULL;
        return ret;
    }

    if (len != 1)
        return -1;

    if (t == HType.UINT_1_TYPE)
    {
        unsigned char P;
        ret = DataItemInSub(len, t, &P);
        if (ret == 1)
        {
            Str = (unsigned)P;
            iReturnCode = ret;
        }
        return ret;
    }
    else if (t == HType.UINT_2_TYPE)
    {
        unsigned short P;
        ret = DataItemInSub(len, t, &P);
        if (ret == 1)
        {
            Str = (unsigned)P;
            iReturnCode = ret;
        }
        return ret;
    }
    else if (t == HType.UINT_4_TYPE)
    {
        unsigned P;
        ret = DataItemInSub(len, t, &P);
        if (ret == 1)
        {
            Str = P;
            iReturnCode = ret;
        }
        return ret;
    }
    else if (t == HType.UINT_8_TYPE)
    {
        unsigned __int64 P;
        ret = DataItemInSub(len, t, &P);
        if (ret == 1)
        {
            // AI(W5-Final-SecsWireCodec) 20260711: vclcompat AnsiString has
            // operator=(long long) but no operator=(unsigned long long)
            // overload (ambiguous otherwise) -- narrowed via (long long),
            // matching the realistic SECS-II UINT_8 payload range.
            Str = (long long)P;
            iReturnCode = ret;
        }
        return ret;
    }
    else if (t == HType.INT_1_TYPE)
    {
        char P;
        ret = DataItemInSub(len, t, &P);
        if (ret == 1)
        {
            Str = int(P);
            iReturnCode = ret;
        }
        return ret;
    }
    else if (t == HType.INT_2_TYPE)
    {
        short P;
        ret = DataItemInSub(len, t, &P);
        if (ret == 1)
        {
            Str = (int)P;
            iReturnCode = ret;
        }
        return ret;
    }
    else if (t == HType.INT_4_TYPE)
    {
        int P;
        ret = DataItemInSub(len, t, &P);
        if (ret == 1)
        {
            Str = P;
            iReturnCode = ret;
        }
        return ret;
    }
    else if (t == HType.INT_8_TYPE)                                          // Steven 20130730 (golden)
    {
        __int64 P;
        ret = DataItemInSub(len, t, &P);
        if (ret == 1)
        {
            Str = (long long)P;
            iReturnCode = ret;
        }
        return ret;
    }
    else if (t == HType.FT_4_TYPE)                                           // Steven 20130730 (golden)
    {
        float P;
        ret = DataItemInSub(len, t, &P);
        if (ret == 1)
        {
            Str = (double)P;
            iReturnCode = ret;
        }
        return ret;
    }
    else if (t == HType.FT_8_TYPE)                                           // Steven 20130730 (golden)
    {
        double P;
        ret = DataItemInSub(len, t, &P);
        if (ret == 1)
        {
            Str = P;
            iReturnCode = ret;
        }
        return ret;
    }
    else
    {
        return -1;
    }
}
