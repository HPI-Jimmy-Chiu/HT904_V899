//---------------------------------------------------------------------------
/*
Hontech KeyPro DLL Header File
Version         :   1.00
Release Date    :   2013.08.02
Design by Joye
*/
//---------------------------------------------------------------------------
#ifndef HTKeyProH
#define HTKeyProH
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include <string>
using namespace std;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Get KeyPro Command Error Code
extern "C" __declspec(dllimport) string __stdcall KEYPRO_GET_ERROR_CODE();

// Get KeyPro Initial Date
extern "C" __declspec(dllimport) bool __stdcall KEYPRO_GET_DATE_INFO( string &sYear , string &sMonth , string &sDay , string &sUseDay );

// Get KeyPro Use Level
// iIndex : Function Number
// return : Function Level
extern "C" __declspec(dllimport) int __stdcall KEYPRO_GET_LEVEL( unsigned int iIndex );

// Set KeyPro ID
// sID Length : 10 Byte Only
extern "C" __declspec(dllimport) bool __stdcall KEYPRO_WRITE_ID_INOF( string sID );

// Get KeyPro ID
extern "C" __declspec(dllimport) string __stdcall KEYPRO_GET_ID();

//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
