//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("Simulator.res");
USEFORM("mainAT.cpp", fMainAT);
USEFORM("automation.cpp", fAutomation);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
         Application->Initialize();
         Application->CreateForm(__classid(TfMainAT), &fMainAT);
         Application->CreateForm(__classid(TfAutomation), &fAutomation);
         Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------
