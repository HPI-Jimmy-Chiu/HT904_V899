//---------------------------------------------------------------------------
#ifndef languageH
#define languageH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <DB.hpp>
#include <map>
//---------------------------------------------------------------------------
class TfLan : public TForm
{
__published:                                                                    // IDE-managed Components
    void __fastcall FormDestroy(TObject *Sender);
private:                                                                        // User declarations
    TStringList *LanList;
    TStringList *tLanList;
    std::map<AnsiString, AnsiString>mapLanList;
    std::map<AnsiString, AnsiString>::iterator mapLanIter;

public:                                                                         // User declarations
    __fastcall TfLan(TComponent* Owner);
    void __fastcall LoadLanguageFromFile();

    void __fastcall UpdateLanguageToStrList(TForm *P);                          //決定要被更新的對象
    void __fastcall SearchCompomentForSave(TWinControl *PCtrl);                 //搜尋畫面中的元件
    bool IsMainFormNotChange(AnsiString S);                                     //主畫面的例外清單
    bool __fastcall UpdateCompomentToStrList(AnsiString Name, AnsiString Cap);  //將元件資料存入陣列

    void __fastcall ChangeLanguage(TForm *P);                                   //決定要被更新的對象
    void __fastcall LoadCompomentForChangeLan(TWinControl *PCtrl);              //載入要被更新的對象
    bool GotLanString(AnsiString Name, AnsiString *CapStr, AnsiString *HintStr);//回傳對應的字串

    AnsiString LanguageFormName;
    bool ShowHint;
    AnsiString TableName;
    AnsiString FullFileName;
    bool bUseLanguageCSV;
};
//---------------------------------------------------------------------------
extern PACKAGE TfLan *fLan;

extern void FormChangeAction();                                                 // 20230601 Joseph , Auto Form Size //
//---------------------------------------------------------------------------
#endif
