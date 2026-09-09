//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("PMAlarmMainForm.res");
USEFORM("fMain.cpp", Form1);
USEUNIT("PMAlarmSystem.cpp");
USEFORM("PMAlarmInterFace.cpp", fPMAlarmInterFace);
USEUNIT("ExternFunction.cpp");
USEFORM("PMAlarmShowData.cpp", fPMAlarm_ShowData);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
         Application->Initialize();
         Application->CreateForm(__classid(TForm1), &Form1);
         Application->CreateForm(__classid(TfPMAlarmInterFace), &fPMAlarmInterFace);
         Application->CreateForm(__classid(TfPMAlarm_ShowData), &fPMAlarm_ShowData);
         Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------
