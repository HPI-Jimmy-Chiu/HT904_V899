//---------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop

#include "uRecipeDefault.h"
#include "cmydef.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfRecipeDefault *fRecipeDefault;
//---------------------------------------------------------------------------
__fastcall TfRecipeDefault::TfRecipeDefault(TComponent* Owner)
    : TForm(Owner)
{
    WindowState=wsNormal;
    if(CUSTOMER_CODE==CC_Greatek)   //Sam 20201209 : Default Recipe ChangeLog
    {
        lbAutoClean_OutArmSpeed->Caption="AutoClean_InArm Speed";
        lbAutoClean_RotationStageSpeed->Caption="AutoClean_Index Arm Speed";
    }
}
//---------------------------------------------------------------------------
