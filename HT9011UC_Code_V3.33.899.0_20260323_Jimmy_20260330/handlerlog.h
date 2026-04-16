#ifndef handlerlogH
#define handlerlogH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <vcl.h>
#include <stdio.h>
#include <Filectrl.hpp>
#include <map>
#include <vector>

class TMyLog
{
    private:
        std::vector<AnsiString> loglist;                                        //vector to save all changes
        std::map <AnsiString, AnsiString>   mapLogValue;                        //Map structure for saving all values
        AnsiString fName;
        AnsiString sUser;
        AnsiString sFolder;
        void Control_Log_Value(TWinControl *PCtrl,bool bInit);
        void Compare_Diff(AnsiString sFind, AnsiString sValue, AnsiString sCaption="");
        void Save_Log();
        void Find_Parent(TObject *PCtrl);

    protected:
    public:

        _fastcall TMyLog::TMyLog();
        AnsiString Delimiter;
        bool Do_Log(TObject *PCtrl, AnsiString  tmpUser, AnsiString tmpFolder="");
        void Init_Log(TObject *PCtrl, AnsiString  tmpUser, AnsiString tmpFolder="");
        void Comp_Log(TObject *PCtrl);
        void Save_SiteStatusLog();                                              //JerryYang 20160617 記錄開關Site的狀態
        void Save_AnyName(AnsiString ComponentName,AnsiString AData);           //kevin 20160728 記錄狀態
};
#endif
