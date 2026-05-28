//---------------------------------------------------------------------------

#ifndef systoolsH
#define systoolsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <inifiles.hpp>
//---------------------------------------------------------------------------
class TFormSysTools : public TForm
{
__published:    // IDE-managed Components
private:    // User declarations
    TIniFile *INIFile1;
public:     // User declarations
    __fastcall TFormSysTools(TComponent* Owner);

    void __fastcall OpenFormData(AnsiString Filename);
    void __fastcall CloseFormData();

    TDateTime   __fastcall CheckAndReadIniData(AnsiString Group, AnsiString Name, TDateTime Value);
    AnsiString  __fastcall CheckAndReadIniData(AnsiString Group, AnsiString Name, AnsiString Value);
    int         __fastcall CheckAndReadIniData(AnsiString Group, AnsiString Name, int Value);
    bool        __fastcall CheckAndReadIniData(AnsiString Group, AnsiString Name, bool Value);
    double      __fastcall CheckAndReadIniData(AnsiString Group, AnsiString Name, double Value);

    void __fastcall WriteIniData(AnsiString Group, AnsiString Name, bool bValue);
    void __fastcall WriteIniData(AnsiString Group, AnsiString Name, int Value);
    void __fastcall WriteIniData(AnsiString Group, AnsiString Name, unsigned int Value);
    void __fastcall WriteIniData(AnsiString Group, AnsiString Name, double Value);
    void __fastcall WriteIniData(AnsiString Group, AnsiString Name, AnsiString Value);
    void __fastcall WriteIniData(AnsiString Group, AnsiString Name, TDateTime Value);

    bool __fastcall CheckSectionExist(AnsiString Group);
    bool __fastcall CheckKeyExist(AnsiString Group, AnsiString Name);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSysTools *FormSysTools;
//---------------------------------------------------------------------------
#endif
