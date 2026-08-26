//---------------------------------------------------------------------------
#ifndef _EXTERN_FUNCTION_H
#define _EXTERN_FUNCTION_H

#include <Grids.hpp>
#include <ComCtrls.hpp>

// Input :  1. TStringGrid Point .
//          2. Row index.
// Output : StringGrid Row Size.
// Desc :   插入新的 row 在 iRow row. 但 focus 仍停留在 iRow row.
int StringGrid_Insert_Row( TStringGrid* pSG , int iRow );

// Input :  1. TStringGrid Point .
//          2. Row index.
//          3. Keep Last Data Row.
// Output : StringGrid Row Size.
// Desc :   刪除 iRow row .
//          如果 bKeepLastData = true 則保留最後一非 fixed row 之 row ,
//          並且 return fixed row count.
int StringGrid_Delete_Row( TStringGrid* pSG , int iRow , bool bKeepLastData = true );

// Input :  1. TStatusBar Point.
//          2. StatusBar's Item Index.
//          3. StatusBar's Item Text.
// Output : Return Ture or False.
// Desc :   設定狀態列 ( pSB ) 字串.
bool StatusBar_ItemText( TStatusBar* pSB , int iItemIndex , AnsiString sText );

// Input :  1. Angle .
//          2. Signal .
// Output : Adjust Angle.
// Desc :   調整角度, 使範圍介於 0~360 or 0~-360.
//          如果 bSign = false 則角度範圍只能介於 0~360 .
double Math_AdjustAngle( double fAngle , bool bSign = true );

// Input :  String Data.
// Output : New String .
// Desc :   輸出字串每個字元只能是 '0'~'9' , '+' , '-' , '.' .
//          而 '+' 或 '-' 字元只能出現在第一個字元, '.' 字元只能出現一次.
//          若出現不合法字元, 則直接從此字元截斷.
AnsiString String_JustNumber( AnsiString sStr );

// Input :  String Data.
// Desc :   刪除字串左邊所有空白字元.
void String_EraseLSpace( AnsiString &sStr );

// Input :  String Data.
// Desc :   刪除字串右邊所有空白字元.
void String_EraseRSpace( AnsiString &sStr );

int RecordTime( bool bIsStart );

// Input : iType = 0 is Normal , 1 is Min. , 2 is Max.
// Desc  : 顯示紀錄時間於 TEdit 元件中.
void ShowRecordTime( TEdit* edt , int iType = 0 );

// Input : Directory Path
// Desc  : 刪除整個資料夾(包含內容所有檔案)
bool DeleteDirectory( AnsiString sDir );

#endif
//---------------------------------------------------------------------------

