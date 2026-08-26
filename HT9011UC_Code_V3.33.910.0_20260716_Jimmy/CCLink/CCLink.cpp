//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("CCLink.res");
USE("Mdfunc.h", File);
USEUNIT("MyCCLinkBoard.cpp");
USEUNIT("MyCCLink.cpp");
USELIB("mdfunc32_BCB.lib");
USEFORM("CCINPUT.cpp", CCInput);
USEFORM("MyCCLinkSensor.cpp", fCCLink);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
         Application->Initialize();
         Application->CreateForm(__classid(TfCCLink), &fCCLink);
         Application->CreateForm(__classid(TCCInput), &CCInput);
         Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------
