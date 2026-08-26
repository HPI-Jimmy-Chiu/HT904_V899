//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("OmronThermo.res");
USEFORM("OmronEJ1N.cpp", fOmron);
USE("Pcomm.h", File);
USELIB("Pcommb.lib");
USEUNIT("TextProcess.cpp");
USEUNIT("MyOmronPanel.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
         Application->Initialize();
         Application->CreateForm(__classid(TfOmron), &fOmron);
         Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------
