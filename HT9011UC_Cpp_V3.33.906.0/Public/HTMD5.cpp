/* ---------------------------------------------------------------------------
 * MIGRATION NOTE (W0 Foundation - HT9045 BCB6 -> Standard C++17)
 * Provenance: HT9011UC_Code_V3.33.906.0_20260618/Public/HTMD5.cpp
 *
 * Changes from BCB6 original:
 *   1. Removed #include "MachineDefine.h" and #pragma hdrstop (BCB6-only).
 *   2. HTMD5.h now #includes "vclcompat/vcl_compat.h" in place of <vcl.h>.
 *   3. Added #include <fstream>, <memory>, <cstdio>, <cstring>, <windows.h>
 *      (some were commented-out in the original; made explicit for portability).
 *   4. Replaced VCL TMask / auto_ptr<TMask>->Matches() with
 *      simple_wildcard_match() -- a portable '*'/'?' wildcard matcher that
 *      reproduces the BCB6 TMask behaviour for the patterns actually used in
 *      this file (e.g. "*.Data", "*.exe").
 *   5. Replaced MyDBIProcess() call (project logging stub) with a no-op
 *      inline stub declared here; production translation units that have the
 *      real MyDBIProcess in scope will shadow it.
 *   6. auto_ptr -> std::unique_ptr (C++17, same ownership semantics here).
 *   7. All MD5 algorithm logic is preserved bit-exact (RFC 1321).
 *   8. Ifor 20200826 buffer-overflow fix comments preserved verbatim.
 *   9. Win32 FindFirstFile / FindNextFile / FindClose kept as-is (MinGW
 *      windows.h exposes them natively on Windows).
 * ---------------------------------------------------------------------------*/

#include "Public/HTMD5.h"

#include <cstdio>
#include <cstring>
#include <fstream>
#include <memory>
#include <windows.h>    // FindFirstFile, FindNextFile, FindClose, WIN32_FIND_DATA

using std::ifstream;
using std::ios;
using std::streampos;

// ---------------------------------------------------------------------------
// Portable wildcard matcher replacing VCL TMask->Matches().
// Supports '*' (zero or more chars) and '?' (exactly one char).
// Case-insensitive to match BCB6 TMask default behaviour.
// BCB6 source: HTMD5.cpp -- CheckFilenameExtension used auto_ptr<TMask>.
// ---------------------------------------------------------------------------
static bool simple_wildcard_match(const char* pattern, const char* str)
{
    if (!pattern || !str) return false;
    const char* p = pattern;
    const char* s = str;
    const char* star_p = NULL;
    const char* star_s = NULL;

    while (*s)
    {
        if (*p == '?' || tolower((unsigned char)*p) == tolower((unsigned char)*s))
        {
            ++p; ++s;
        }
        else if (*p == '*')
        {
            star_p = p++;
            star_s = s;
        }
        else if (star_p)
        {
            p = star_p + 1;
            s = ++star_s;
        }
        else
        {
            return false;
        }
    }
    while (*p == '*') ++p;
    return *p == '\0';
}

// ---------------------------------------------------------------------------
// MyDBIProcess stub -- the BCB6 original calls this for exception logging.
// Translated units that include the real MyDBIProcess header will shadow this.
// ---------------------------------------------------------------------------
static void MyDBIProcess(const char* /*type*/, const char* /*location*/) {}


// Constants for MD5Transform routine.
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

//---------------------------------------------------------------------------
// F, G, H and I are basic MD5 functions.
//---------------------------------------------------------------------------
inline MD5::uint4 MD5::F(uint4 x, uint4 y, uint4 z)
{
    return x&y | ~x&z;
}
//---------------------------------------------------------------------------
inline MD5::uint4 MD5::G(uint4 x, uint4 y, uint4 z)
{
    return x&z | y&~z;
}
//---------------------------------------------------------------------------
inline MD5::uint4 MD5::H(uint4 x, uint4 y, uint4 z)
{
    return x^y^z;
}
//---------------------------------------------------------------------------
inline MD5::uint4 MD5::I(uint4 x, uint4 y, uint4 z)
{
    return y ^ (x | ~z);
}
//---------------------------------------------------------------------------
// rotate_left rotates x left n bits.
//---------------------------------------------------------------------------
inline MD5::uint4 MD5::rotate_left(uint4 x, int n)
{
    return (x << n) | (x >> (32-n));
}
//---------------------------------------------------------------------------
// FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
// Rotation is separate from addition to prevent recomputation.
//---------------------------------------------------------------------------
inline void MD5::FF(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac)
{
    a=rotate_left(a+F(b, c, d)+x+ac, s)+b;
}
//---------------------------------------------------------------------------
inline void MD5::GG(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac)
{
    a=rotate_left(a+G(b, c, d)+x+ac, s)+b;
}
//---------------------------------------------------------------------------
inline void MD5::HH(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac)
{
    a=rotate_left(a+H(b, c, d)+x+ac, s)+b;
}
//---------------------------------------------------------------------------
inline void MD5::II(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac)
{
    a=rotate_left(a+I(b, c, d)+x+ac, s)+b;
}
//---------------------------------------------------------------------------
// default ctor, just initailize
//---------------------------------------------------------------------------
MD5::MD5()
{
    init();
}
//---------------------------------------------------------------------------
// nifty shortcut ctor, compute MD5 for string and finalize it right away
//---------------------------------------------------------------------------
MD5::MD5(const std::string &text)
{
    init();
    update(text.c_str(), (size_type)text.length());
    finalize();
}
//---------------------------------------------------------------------------
void MD5::init()
{
    finalized=false;
    count[0] = 0;
    count[1] = 0;

    // load magic initialization constants.
    state[0] = 0x67452301;
    state[1] = 0xefcdab89;
    state[2] = 0x98badcfe;
    state[3] = 0x10325476;
}
//---------------------------------------------------------------------------
// decodes input (unsigned char) into output (uint4). Assumes len is a multiple of 4.
//---------------------------------------------------------------------------
void MD5::decode(uint4 output[], const uint1 input[], size_type len)
{
    for (unsigned int i=0, j=0; j<len; i++, j+=4)
    {
        output[i]=((uint4)input[j])         |
                  (((uint4)input[j+1])<<8)  |
                  (((uint4)input[j+2])<<16) |
                  (((uint4)input[j+3])<<24);
    }
}
//---------------------------------------------------------------------------
// encodes input (uint4) into output (unsigned char). Assumes len is
// a multiple of 4.
//---------------------------------------------------------------------------
void MD5::encode(uint1 output[], const uint4 input[], size_type len)
{
    for(size_type i=0, j=0; j<len; i++, j+=4)
    {
        output[j]  = input[i]      &0xff;
        output[j+1]=(input[i]>>8)  &0xff;
        output[j+2]=(input[i]>>16) &0xff;
        output[j+3]=(input[i]>>24) &0xff;
    }
}
//---------------------------------------------------------------------------
// apply MD5 algo on a block
//---------------------------------------------------------------------------
void MD5::transform(const uint1 block[blocksize])
{
    uint4 a=state[0], b=state[1], c=state[2], d=state[3], x[16];
    decode(x, block, blocksize);

    /* Round 1 */
    FF (a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */
    FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */
    FF (c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */
    FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */
    FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */
    FF (d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */
    FF (c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */
    FF (b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */
    FF (a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */
    FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */
    FF (c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
    FF (b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
    FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
    FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
    FF (c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
    FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

    /* Round 2 */
    GG (a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */
    GG (d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */
    GG (c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
    GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */
    GG (a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */
    GG (d, a, b, c, x[10], S22,  0x2441453); /* 22 */
    GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
    GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
    GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
    GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
    GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */
    GG (b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */
    GG (a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
    GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
    GG (c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */
    GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

    /* Round 3 */
    HH (a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */
    HH (d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
    HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
    HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
    HH (a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */
    HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */
    HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */
    HH (b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
    HH (a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
    HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */
    HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */
    HH (b, c, d, a, x[ 6], S34,  0x4881d05); /* 44 */
    HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
    HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
    HH (c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
    HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */

    /* Round 4 */
    II (a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */
    II (d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
    II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
    II (b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
    II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
    II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
    II (c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
    II (b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
    II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
    II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
    II (c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
    II (b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
    II (a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
    II (d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
    II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
    II (b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */

    state[0]+=a;
    state[1]+=b;
    state[2]+=c;
    state[3]+=d;

    // Zeroize sensitive information.
    memset(x, 0, sizeof x);
}
//---------------------------------------------------------------------------
// MD5 block update operation. Continues an MD5 message-digest
// operation, processing another message block
//---------------------------------------------------------------------------
void MD5::update(const unsigned char input[], size_type length)
{
    // compute number of bytes mod 64
    size_type index = count[0] / 8 % blocksize;

    // Update number of bits
    if((count[0]+=(length<<3))<(length<<3))
        count[1]++;
    count[1]+=(length>>29);

    // number of bytes we need to fill in buffer
    size_type firstpart=64-index;
    size_type i;

    // transform as many times as possible.
    if(length>=firstpart)
    {
        // fill buffer first, transform
        memcpy(&buffer[index], input, firstpart);
        transform(buffer);

        // transform chunks of blocksize (64 bytes)
        for(i=firstpart; i+blocksize<=length; i+=blocksize)
            transform(&input[i]);

        index=0;
    }
    else
    {
        i=0;
    }

    if(length-i>0) //Ifor 20200826 : fix MD5 buffer overflow issue
    {
        // buffer remaining input
        memcpy(&buffer[index], &input[i], length-i);
    }
}
//---------------------------------------------------------------------------
// for convenience provide a version with signed char
//---------------------------------------------------------------------------
void MD5::update(const char input[], size_type length)
{
    if(length>0) //Ifor 20200826 : fix MD5 buffer overflow issue
    {
        update((const unsigned char*)input, length);
    }
}
//---------------------------------------------------------------------------
// MD5 finalization. Ends an MD5 message-digest operation, writing the
// the message digest and zeroizing the context.
//---------------------------------------------------------------------------
MD5& MD5::finalize()
{
    static unsigned char padding[64] =
    {
        0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    if (!finalized)
    {
        // Save number of bits
        unsigned char bits[8];
        encode(bits, count, 8);

        // pad out to 56 mod 64.
        size_type index =count[0]/8%64;
        size_type padLen=(index<56)?(56-index):(120-index);
        update(padding, padLen);

        // Append length (before padding)
        update(bits, 8);

        // Store state in digest
        encode(digest, state, 16);

        // Zeroize sensitive information.
        memset(buffer, 0, sizeof buffer);
        memset(count, 0, sizeof count);

        finalized=true;
    }

    return *this;
}
//---------------------------------------------------------------------------
// return hex representation of digest as string
//---------------------------------------------------------------------------
AnsiString MD5::hexdigest() const
{
    if(!finalized)
        return "";

    char buf[33];
    for(int i=0; i<16; i++)
        sprintf(buf+i*2, "%02x", digest[i]);
    buf[32]=0;

    return AnsiString(buf);
}
//---------------------------------------------------------------------------
//std::ostream& operator<<(std::ostream& out, MD5 md5)
//{
//    return out << md5.hexdigest();
//}
//---------------------------------------------------------------------------
AnsiString md5(const AnsiString str)
{
    MD5 md5obj = MD5(str.c_str());
    return md5obj.hexdigest();
}
//---------------------------------------------------------------------------
// reading an entire binary file
//---------------------------------------------------------------------------
AnsiString md5_File(const AnsiString str)
{
    if(str=="" || !FileExists(str))
    {
        return "File not exists!";
    }

    streampos size;
    char * memblock;

    ifstream file(str.c_str(), ios::in|ios::binary|ios::ate);
    if(file.is_open())
    {
        size = file.tellg();
        memblock = new char [(std::streamsize)size];
        file.seekg(0, ios::beg);
        file.read(memblock, size);
        file.close();

        MD5 md5obj;
        md5obj.update(memblock, (MD5::size_type)size);
        md5obj.finalize();
        delete[] memblock;
        memblock=NULL;  //kevin 20180517
        return md5obj.hexdigest();
    }
    else
    {
        return "Unable to open file";
    }
}
//---------------------------------------------------------------------------
AnsiString md5_Folder(AnsiString FolderName)
{
    TStringList *tsFileName;
    MD5 md5obj;
    if(FolderName=="" || !DirectoryExists(FolderName))
    {
        return "File not exists!";
    }

    streampos size;
    char * memblock;
    tsFileName=new TStringList();

    SearchFile(FolderName, "*.Data", tsFileName);

    if(tsFileName->Count!=0)
    {
        for(int i=0; i<tsFileName->Count; i++)
        {
            AnsiString sEntry = (AnsiString)tsFileName->Strings[i];  // extract from proxy
            ifstream file(sEntry.c_str(), ios::in|ios::binary|ios::ate);
            if(file.is_open())
            {
                size = file.tellg();
                memblock = new char [(std::streamsize)size];
                file.seekg(0, ios::beg);
                file.read(memblock, size);
                file.close();

                md5obj.update(memblock, (MD5::size_type)size);
                delete[] memblock;
                memblock=NULL;  //kevin 20180517
            }
            else
            {
                delete tsFileName;
                return "Can Not Open File";
            }
        }
    }
    else
    {
        delete tsFileName;
        return "No File Exist";
    }

    delete tsFileName;
    md5obj.finalize();
    return md5obj.hexdigest();
}
//---------------------------------------------------------------------------
// Check whether filename matches wildcard pattern
// BCB6 original: used auto_ptr<TMask> (VCL Masks unit).
// Translated: replaced with simple_wildcard_match() -- portable, no VCL.
//---------------------------------------------------------------------------
bool CheckFilenameExtension(AnsiString sFileName, AnsiString sPattern)
{
    bool bCheck = false;
    try
    {
        bCheck = simple_wildcard_match(sPattern.c_str(), sFileName.c_str());
    }
    catch(...)
    {
        MyDBIProcess("Exception", "CheckFilenameExtension");
        return false;
    }
    return bCheck;
}
//---------------------------------------------------------------------------
// Search for files matching sPattern starting from sBeginPath (non-recursive)
//---------------------------------------------------------------------------
int SearchFile(AnsiString sBeginPath, AnsiString sPattern, TStrings *slResult)
{
    WIN32_FIND_DATA filedata;                           // Structure for file data
    HANDLE filehandle;                                  // Handle for searching
    AnsiString sPath;
    slResult->Clear();
    if(DirectoryExists(sBeginPath))
    {
        if(sBeginPath.SubString(sBeginPath.Length(), 1)!="\\")
        {
            sBeginPath=sBeginPath+"\\";
        }
        sPath=sBeginPath+"*.*";

        //Steven 20101118 Start : only enumerate files, not subdirectories
        filehandle=FindFirstFileA((sPath+"*").c_str(), &filedata);
        if(filehandle!=INVALID_HANDLE_VALUE)
        {
            do
            {
                /* skip . and .. entries */
                if((filedata.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)!=0 ||
                    strcmp(filedata.cFileName, ".")==0 ||
                    strcmp(filedata.cFileName, "..")==0)
                {
                    continue;
                }
                else
                {
                    if(CheckFilenameExtension(filedata.cFileName, sPattern)==true)
                    {
                        slResult->Add(sBeginPath+filedata.cFileName);
                    }
                }
            } while(FindNextFileA(filehandle, &filedata));
            FindClose(filehandle);
        }
        //Steven 20101118 End
    }
    return slResult->GetCount();  // TStrings* -- use virtual accessor (no Count member on base)
}
//---------------------------------------------------------------------------
// Search for files matching sPattern starting from sBeginPath (recursive)
//---------------------------------------------------------------------------
int SearchFileAll(AnsiString sBeginPath, AnsiString sPattern, TStrings *slResult)
{
    WIN32_FIND_DATA filedata;                           // Structure for file data
    HANDLE filehandle;                                  // Handle for searching
    AnsiString sPath;
    if(DirectoryExists(sBeginPath))
    {
        if(sBeginPath.SubString(sBeginPath.Length(), 1)!="\\")
        {
            sBeginPath=sBeginPath+"\\";
        }
        sPath=sBeginPath+"*.*";

        //Steven 20101118 Start : enumerate files and recurse into subdirectories
        filehandle=FindFirstFileA((sPath+"*").c_str(), &filedata);
        if(filehandle!=INVALID_HANDLE_VALUE)
        {
            do
            {
                /* skip . and .. entries */
                if((filedata.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)!=0 ||
                    strcmp(filedata.cFileName, ".")==0 ||
                   strcmp(filedata.cFileName, "..")==0)
                {
                    continue;
                }
                else if(filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    // BCB6 original used AnsiString::sprintf for path building
                    AnsiString subPath;
                    subPath.sprintf("%s%s", sBeginPath.c_str(), filedata.cFileName);
                    SearchFileAll(subPath, sPattern, slResult);
                }
                else
                {
                    if(CheckFilenameExtension(filedata.cFileName, sPattern)==true)
                    {
                        slResult->Add(sBeginPath+filedata.cFileName);
                    }
                }
            } while(FindNextFileA(filehandle, &filedata));
            FindClose(filehandle);
        }
        //Steven 20101118 End
    }
    return slResult->GetCount();  // TStrings* -- use virtual accessor (no Count member on base)
}
//---------------------------------------------------------------------------
// Search for immediate subdirectory names under sBeginPath
//---------------------------------------------------------------------------
int SearchFolder(AnsiString sBeginPath, TStrings *slResult)
{
    WIN32_FIND_DATA filedata;                           // Structure for file data
    HANDLE filehandle;                                  // Handle for searching
    AnsiString sPath;
    if(DirectoryExists(sBeginPath))
    {
        if(sBeginPath.SubString(sBeginPath.Length(), 1)!="\\")
        {
            sBeginPath=sBeginPath+"\\";
        }
        sPath=sBeginPath+"*.*";

        //Steven 20101118 Start : enumerate subdirectories only
        filehandle=FindFirstFileA((sPath+"*").c_str(), &filedata);
        if(filehandle!=INVALID_HANDLE_VALUE)
        {
            do
            {
                /* skip . and .. entries */
                if((filedata.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)!=0 ||
                    strcmp(filedata.cFileName, ".")==0 ||
                   strcmp(filedata.cFileName, "..")==0)
                {
                    continue;
                }
                else if(filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    slResult->Add(filedata.cFileName);
                }
            } while(FindNextFileA(filehandle, &filedata));
            FindClose(filehandle);
        }
        //Steven 20101118 End
    }
    return slResult->GetCount();  // TStrings* -- use virtual accessor (no Count member on base)
}
//---------------------------------------------------------------------------
