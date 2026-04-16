//---------------------------------------------------------------------------

#ifndef HonNewDelH
#define HonNewDelH
//---------------------------------------------------------------------------
//#include "HonNewCheck.h"    //yunghsin 20170612 add

//---------------------------------------------------------------------------
// 解構時需要Delete TList資料
// 偵測Memory使用量
//

//void SetVersion( AnsiString sVer );
template <typename T1>              T1* HonNew      (T1* Ptr, AnsiString sFile, AnsiString sFunc, AnsiString sLine);
template <typename T1>              T1* HonNew      (T1* Ptr, AnsiString sFile, AnsiString sFunc, AnsiString sLine, TComponent* pOwner);

template <typename T1>              T1* HonNew      (T1* Ptr, AnsiString sFile, AnsiString sFunc, AnsiString sLine, int iArrayIndex);
template <typename T1>              T1* HonNew      (T1* Ptr, AnsiString sFile, AnsiString sFunc, AnsiString sLine, TComponent* pOwner, int iArrayIndex);
template <typename T1>              T1* HonNewArray (T1* Ptr, AnsiString sFile, AnsiString sFunc, AnsiString sLine, int iArraySize);

template <typename T1, typename T2> T1* HonNew      (T2* Ptr, AnsiString sFile, AnsiString sFunc, AnsiString sLine, int iArrayIndex);
template <typename T1, typename T2> T1* HonNew      (T2* Ptr, AnsiString sFile, AnsiString sFunc, AnsiString sLine, TComponent* pOwner, int iArrayIndex);
template <typename T1, typename T2> T1* HonNewArray (T2* Ptr, AnsiString sFile, AnsiString sFunc, AnsiString sLine, int iArraySize);
template <typename T1> void HonDelete     (T1* Ptr);
template <typename T1> void HonDeleteArray(T1* Ptr);


#endif
