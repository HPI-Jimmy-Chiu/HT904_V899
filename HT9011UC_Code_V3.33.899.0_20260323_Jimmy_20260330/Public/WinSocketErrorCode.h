//---------------------------------------------------------------------------

#ifndef WinSocketErrorCodeH
#define WinSocketErrorCodeH
//---------------------------------------------------------------------------

AnsiString GetErrorMsg(TObject *Sender, int ErrorCode);
void LogClientSocketExceptionError(TObject *Sender, AnsiString Msg);
#endif
