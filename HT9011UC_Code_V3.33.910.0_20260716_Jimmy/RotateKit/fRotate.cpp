#include "MachineDefine.h"
#pragma hdrstop

#include "fRotate.h"

#include "common.h"
#include "main.h"
#include "cprod.h"
#include "cmydef.h"
#include "HTray.h"
#include "cinitial.h"
#include "uhome.h"
#include "aRotateDegreeClass.h"
#include "aRotateKIT.h"
#include "csystem.h"
#include "myQwertyKeyBoard.h"
#include "mymessbox.h"
#include "mysensor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmRotate *FrmRotate;

TRotate tRotate;
TEdit *Rotate_Offset[2];                                                        //Ifor 20251210 add:Rotate Offset
//---------------------------------------------------------------------------
static bool IsRotateFlowNotFinish()                                             //KevinCheng 20260318 : Rotate流程未完，不可關閉。
{
    if(USE_ROTATE_KIT!=1)
        return false;

    if(iInArmRotateKit!=1 ||                                                    // rotate task不在初始狀態，表示流程仍在執行中
       iOutArmRotateKit!=1)
        return true;

    if(bWaitRotateFinish)
        return true;

    return false;
}
//---------------------------------------------------------------------------
__fastcall TFrmRotate::TFrmRotate(TComponent* Owner)
    : TForm(Owner)
{
    if(USE_ROTATE_KIT==1 &&
       (iRotate_Type==e4MotRotate ||
        iRotate_Type==e8MotRotate ||
        iRotate_Type==e2MotRotate2Dut ||
        iRotate_Type==e1MotRotate1Dut))
    {
        bShowRotateBySite=true;
    }
    else
    {
        for(int i=0; i<sgRotateIn->ColCount; i++)
        {
            for(int j=0; j<sgRotateIn->RowCount; j++)
            {
                sgRotateIn->Cells[i][j]=0;
                sgRotateOut->Cells[i][j]=0;                                     //Ifor 20170407 add Rotate Out 獨立旋轉角度

                if(CosFunction.bRotateUseRTmode==true)                          //jou 20231122 : Rotate Use RT mode
                {
                    sgRotateInRT->Cells[i][j]=0;
                    sgRotateOutRT->Cells[i][j]=0;                               //Ifor 20170407 add Rotate Out 獨立旋轉角度
                }
            }
        }
        bShowRotateBySite=false;
    }

    sgFTA->Cells[0][0]=0;

    Rotate_Offset[0]=edInRotate_0;
    Rotate_Offset[1]=edOutRotate_0;

    ZeroMemory(&iRotateDutDate, sizeof(iRotateDutDate));
    iFromTrayAngle=0;
}
//---------------------------------------------------------------------------
void __fastcall TFrmRotate::FormCreate(TObject *Sender)
{                                                                               //Steven 20170329 (wei) : Add individual rotate motor
    AnsiString str;
    int iTop[4]={0, 83, 165, 247};
    int iLeft[8]={0, 64, 128, 192, 256, 320, 384, 448};
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<8; j++)
        {
            Application->CreateForm(__classid(TfrmRot), &FrmRotDegree[0][i][j]);
            FrmRotDegree[0][i][j]->grpRotDegree->Parent=tsM8RotateIn;
            FrmRotDegree[0][i][j]->grpRotDegree->Visible=false;
            FrmRotDegree[0][i][j]->grpRotDegree->Left=iLeft[j];
            FrmRotDegree[0][i][j]->grpRotDegree->Top=iTop[i];
            str.sprintf("%c%c", 'A'+i, 'a'+j);
            FrmRotDegree[0][i][j]->SetCaption(str);
            FrmRotDegree[0][i][j]->rgRotateDegree->Parent=tsM8RotateIn;
            FrmRotDegree[0][i][j]->rgRotateDegree->Visible=false;
            FrmRotDegree[0][i][j]->SetTag(true, i, j);

            Application->CreateForm(__classid(TfrmRot), &FrmRotDegree[1][i][j]);
            FrmRotDegree[1][i][j]->grpRotDegree->Parent=tsM8RotateOut;
            FrmRotDegree[1][i][j]->grpRotDegree->Visible=false;
            FrmRotDegree[1][i][j]->grpRotDegree->Left=iLeft[j];
            FrmRotDegree[1][i][j]->grpRotDegree->Top=iTop[i];
            str.sprintf("Out %c%c", 'A'+i, 'a'+j);
            FrmRotDegree[1][i][j]->SetCaption(str);
            FrmRotDegree[1][i][j]->rgRotateDegree->Parent=tsM8RotateOut;
            FrmRotDegree[1][i][j]->rgRotateDegree->Visible=false;
            FrmRotDegree[1][i][j]->SetTag(false, i, j);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmRotate::FormDestroy(TObject *Sender)
{
    LogSoftwareOffTime("TFrmRotate, FormDestroy");                              //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void __fastcall TFrmRotate::FormShow(TObject *Sender)
{
    fShow=true;
    rgSelect->ItemIndex=1;
    Left=(1280-Width)/2;                                                        //Steven 20220407 : 避免跑到畫面外
    Top =(1024-Height)/2;

    fRotate_ReadFile();
    DoIniDataToForm();

    SetGridDraw();

    if(CUSTOMER_CODE==CC_PTI &&                                                 //JerryYang 20161202 PTI要求顯示Rotate kit擺放的位置
       TestIF.iTestMode==_8Site2X4 &&
       tRotate.DutNum==tDutType_8)                                              //先針對常用的模式顯示
    {
        FrmRotate->Width=1180;
        for(int i=0; i<MAX_SOCKET_ROW; i++)
        {
            for(int j=0; j<MAX_SOCKET_COL; j++)
            {
                tmyRotatePosion->SetCellColorIndex(j, i, Prod.fInArmSuck4x8[0][i][j] || Prod.fInArmSuck4x8[1][i][j]);
            }
        }
    }
    else
    {
        FrmRotate->Width=790;
    }
    Label_Err->Visible=false;

    if(CUSTOMER_CODE==CC_KYEC_LEE || CUSTOMER_CODE==CC_KYEC_XILINX)             //KYEC 低於權限顯示不能修改
    {
        if(AccessLevel<LevelSet.AccessLevel[89])
        {
            rgSelect->Enabled       =false;
            sgFTA->Enabled          =false;
            YPitch->Enabled         =false;
            XPitch->Enabled         =false;
            cbActiveRotate->Enabled =false;
            sgRotateIn->Enabled     =false;
            sgRotateOut->Enabled    =false;                                     //Ifor 20170407 add Rotate Out 獨立旋轉角度
        }
        else
        {
            rgSelect->Enabled       =true;
            sgFTA->Enabled          =true;
            YPitch->Enabled         =true;
            XPitch->Enabled         =true;
            cbActiveRotate->Enabled =true;
            sgRotateIn->Enabled     =true;
            sgRotateOut->Enabled    =true;                                      //Ifor 20170407 add Rotate Out 獨立旋轉角度
        }
    }

    rgDutNum->Enabled=(USE_ROTATE_KIT==1 && (iRotate_Type==e8MotRotate));       //Steven 20170329 : Add individual rotate motor

    if(bShowRotateBySite==true)
    {
        tsM1RotateIn->TabVisible    =false;
        tsM1RotateInRT->TabVisible  =false;
        tsM1RotateOut->TabVisible   =false;
        tsM1RotateOutRT->TabVisible =false;
        tsM8RotateIn->TabVisible    =true;
        tsM8RotateOut->TabVisible   =cbOutRotateDifferentAngle->Checked;
    }
    else
    {
        tsM1RotateIn->TabVisible    =true;
        tsM1RotateInRT->TabVisible  =(CosFunction.bRotateUseRTmode==true && tRotate.bRotateUseRTmode==true);
        tsM8RotateIn->TabVisible    =false;
        tsM8RotateOut->TabVisible   =false;
        tsM1RotateOut->TabVisible   =cbOutRotateDifferentAngle->Checked;
        tsM1RotateOutRT->TabVisible =(cbOutRotateDifferentAngle->Checked && CosFunction.bRotateUseRTmode && tRotate.bRotateUseRTmode==true);
    }

    for(int i=0; i<MAX_SOCKET_ROW; i++)
    {
        for(int j=0; j<MAX_SOCKET_COL; j++)
        {
            if(i<TestSocket.iShtRow && j<TestSocket.iShtCol)
            {
                FrmRotDegree[0][i][j]->grpRotDegree->Visible=true;
                FrmRotDegree[1][i][j]->grpRotDegree->Visible=true;
            }
            else
            {
                FrmRotDegree[0][i][j]->grpRotDegree->Visible=false;
                FrmRotDegree[1][i][j]->grpRotDegree->Visible=false;
            }
        }
    }
    //<==
    //Steven 20170329 : Add individual rotate motor

    chkPassBinNoRotate->Visible=CosFunction.bPassBinNoRotate;                   //jou 20231020 : Pass bin no rotate

    //jou 20231122 : Rotate Use RT mode
    if(USE_ROTATE_KIT==1 && bShowRotateBySite==true)
        chkRotateUseRTmode->Visible=false;
    else
        chkRotateUseRTmode->Visible=CosFunction.bRotateUseRTmode;

    grpRotateOffset->Visible=(iRotate_Type==eInOutArm1Motor);                   //Ifor 20251210 add:Rotate Offset

    chkART_RTNoRotate->Visible=(CosFunction.bART_RT_NoRotate && bCanRunSCKART); //Sam 20240809 : ART RT No Rotate
    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20170405 : add change log for rotate
}
//---------------------------------------------------------------------------
void TFrmRotate::fRotate_ReadFile()
{
    AnsiString tmpA ,tmpB;
    AnsiString S="", str;
    S=GetLastOpenFN();
    AnsiString szDir="";
//    int rt =-1;
    AnsiString tmpS ="";
    szDir.sprintf("%s%s\\Rotate.Data", DataPath, S);
    TStringList *slistIn  = new TStringList;
    TStringList *slistOut = new TStringList;                                    //Ifor 20170407 (Steven) add Rotate Out 獨立旋轉角度
    TStringList *slistInRT  = new TStringList;
    TStringList *slistOutRT = new TStringList;
    int tmpAngleIn, tmpAngleOut;                                                //Ifor 20170412 (Steven) add In/Out Rotate 旋轉角度暫存
    bool bInAngleNotExist=true;                                                 //Ifor 20170412 (Steven) add In/Out Rotate 旋轉角度不存在
    bool bOutAngleNotExist=true;                                                //Ifor 20170412 (Steven) add In/Out Rotate 旋轉角度不存在
    bool bInAngleNotExistRT=true;
    bool bOutAngleNotExistRT=true;

    tRotate.ActiveRotate        = ReadIniData(szDir, "SETTING", "ActiveRotate", false);
    tRotate.bUseDifferentAngle  = ReadIniData(szDir, "SETTING", "OutRotateDifferentAngle", false);  //Ifor 20170407 add Rotate Out 獨立旋轉角度
    if(CosFunction.bPassBinNoRotate==true)                                      //jou 20231020 : Pass bin no rotate
        tRotate.bPassBinNoRotate= ReadIniData(szDir, "SETTING", "bPassBinNoRotate", false);

    if(USE_ROTATE_KIT==1)
    {
        if(iRotate_Type==e4MotRotate)
        {
            tRotate.DutNum          = tDutType_4;
        }
        else if(iRotate_Type==e1MotRotate1Dut)
        {
            tRotate.DutNum          = tDutType_1;
        }
        else if(iRotate_Type==e2MotRotate2Dut)
        {
            tRotate.DutNum          = tDutType_2;
        }
        else
        {
            tRotate.DutNum          = ReadIniData(szDir, "SETTING", "Dut Num", 0);
        }
    }
    else
    {
        tRotate.DutNum          = ReadIniData(szDir, "SETTING", "Dut Num", 0);
    }
    tRotate.FromTrayAngle       = ReadIniData(szDir, "SETTING", "FromTrayAngle", 0.0);
    slistIn->CommaText          = ReadIniData(szDir, "SETTING", "DutAngle",    AnsiString("0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"));
    slistOut->CommaText         = ReadIniData(szDir, "SETTING", "DutAngleOut", AnsiString("0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"));    //Ifor 20170407 add Rotate Out 獨立旋轉角度

    if(CosFunction.bRotateUseRTmode==true)                                      //jou 20231122 : Rotate Use RT mode
    {
        tRotate.bRotateUseRTmode = ReadIniData(szDir, "SETTING", "bRotateUseRTmode", false);
        slistInRT->CommaText     = ReadIniData(szDir, "SETTING", "DutAngleRT",    AnsiString("0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"));
        slistOutRT->CommaText    = ReadIniData(szDir, "SETTING", "DutAngleOutRT", AnsiString("0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"));
    }

    if(CosFunction.bART_RT_NoRotate)                                            //Sam 20240809 : ART RT No Rotate
        tRotate.bART_RT_NoRotate= ReadIniData(szDir, "SETTING", "bART_RT_NoRotate", false);
    strInRotateDutAngle         = slistIn->CommaText;                           //wei 20180316
    strOutRotateDutAngle        = slistOut->CommaText;                          //wei 20180316

    if(USE_ROTATE_KIT==1 && bShowRotateBySite==true)
    {
        tRotate.RowCount        =TestSocket.iShtRow;
        tRotate.ColCount        =TestSocket.iShtCol;
    }
    else
    {
        tRotate.ColCount        = ReadIniData(szDir, "SETTING", "ColCount", 0);
        tRotate.RowCount        = ReadIniData(szDir, "SETTING", "RowCount", 0);
    }

    if(tRotate.DutNum==tDutType_8)                                              //20140320 wei   固定 4 Rotate Xpitch= 80mm; 8 Rotate Xpitch= 40mm
    {
        tRotate.RotateKit_PitchX=40;
        tRotate.RotateKit_PitchY=ReadIniData(szDir, "SETTING", "KitPitchY", 60.0);   //jou 2015-09-03 修正Rotate pitch無法存檔的問題
    }
    else if(tRotate.DutNum==tDutType_1)
    {
        tRotate.RotateKit_PitchX=0;
        tRotate.RotateKit_PitchY=0;
    }
    else if(tRotate.DutNum==tDutType_2)
    {
        tRotate.RotateKit_PitchX=80;
        tRotate.RotateKit_PitchY=0;
    }
    else
    {
        tRotate.RotateKit_PitchX=ReadIniData(szDir, "SETTING", "KitPitchX", 80.0);   //jou 2015-09-03 修正Rotate pitch無法存檔的問題
        tRotate.RotateKit_PitchY=ReadIniData(szDir, "SETTING", "KitPitchY", 60.0);   //jou 2015-09-03 修正Rotate pitch無法存檔的問題
    }
    ZeroMemory(&tRotate.RotationCount, sizeof(tRotate.RotationCount));
    ZeroMemory(&tRotate.OutRotationCount, sizeof(tRotate.OutRotationCount));    //kevin 20131003  清為0

    ZeroMemory(&tRotate.RotationCountRT, sizeof(tRotate.RotationCountRT));
    ZeroMemory(&tRotate.OutRotationCountRT, sizeof(tRotate.OutRotationCountRT));

    if(slistIn->Count==8)                                                       //Ifor 20170411 (Steven) add Rotate 舊資料僅八筆資料會發生異常，補插入八筆資料
    {
        for(int i=4; i<=12; i=i+8)
        {
            for(int j=0; j<4; j++)
            {
                slistIn->Insert(i, 0);
            }
        }
    }

    tRotate.RotationTimeIn =0;                                                  //清除 Count 數
    tRotate.RotationTimeOut=0;

    tRotate.RotationTimeInRT =0;
    tRotate.RotationTimeOutRT=0;

    if(slistIn->Count!=32)                                                      //Ifor 20170411 (Steven) : Rotate資料補成32site
    {
        do
        {
            slistIn->Add(0);
        }
        while(slistIn->Count<32);
    }

    if(slistOut->Count!=32)                                                     //Ifor 20170424 (Steven) : Out Rotate資料補成32site
    {
        do
        {
            slistOut->Add(0);
        }
        while(slistOut->Count<32);
    }

    if(slistInRT->Count!=32)                                                    //Ifor 20170411 (Steven) : Rotate資料補成32site
    {
        do
        {
            slistInRT->Add(0);
        }
        while(slistInRT->Count<32);
    }

    if(slistOutRT->Count!=32)                                                   //Ifor 20170424 (Steven) : Out Rotate資料補成32site
    {
        do
        {
            slistOutRT->Add(0);
        }
        while(slistOutRT->Count<32);
    }

    bIsAngleZero=false;                                                          //Eastsun 20260515 F023: E6 detect rotate 0 degree
    if(slistIn->Count>0)                                                        //Ifor 20170626 (wei) add 迴圈改用row col 避免tRotate.RotationTime 多1次
    {
        for(int i=0; i<2; i++)                                                  //Ifor 20171005 (Steven) : 避免1x2模式Rotate無法存檔問題 iRow -> 2
        {
            for(int j=0; j<8; j++)                                              //Steven 20201029 : iCol --> 8
            {
                //Ifor 20170407 (Steven) add Rotate Out 獨立旋轉角度
                //==>
                tRotate.RotateDutDate[0][i][j]=atoi(slistIn->Strings [i*8+j].c_str());
                tRotate.RotateDutDate[1][i][j]=atoi(slistOut->Strings[i*8+j].c_str());

                tRotate.RotateDutDate[2][i][j]=atoi(slistInRT->Strings [i*8+j].c_str());
                tRotate.RotateDutDate[3][i][j]=atoi(slistOutRT->Strings[i*8+j].c_str());

                if(USE_ROTATE_KIT==1 && bShowRotateBySite==true)
                {
                    tRotate.RotationTimeIn =1;
                    tRotate.RotationTimeOut=1;
                }
                else
                {
                    //====================計算 Rotate In 旋轉角度====================
                    tmpAngleIn=tRotate.RotateDutDate[0][i][j];
                    for(int k=0; k<tRotate.RotationTimeIn; k++)                 //Ifor 20170412 (Steven) 判斷In Rotate 旋轉角度是否已存在
                    {
                        if(tRotate.RotationCount[k]==tmpAngleIn || tmpAngleIn==0)   //Ifor 20240919 add:避免Count 多一次導致流程異常
                            bInAngleNotExist=false;
                    }

                    if(bInAngleNotExist==true)
                    {
                        tRotate.RotationCount[tRotate.RotationTimeIn]=tmpAngleIn;
                        if(tmpAngleIn==0) bIsAngleZero=true;                          //Eastsun 20260515 F023: E6 detect rotate 0 degree
                            tRotate.RotationTimeIn++;
                    }
                    bInAngleNotExist=true;                                      //避免有一次相同角度後無法再次更新

                    //====================計算 Rotate Out 旋轉角度====================
                    if(tRotate.bUseDifferentAngle==true)                        //Out Arm 使用不同角度
                    {
                        tmpAngleOut=360-tRotate.RotateDutDate[0][i][j]+tRotate.RotateDutDate[1][i][j]-360;
                    }
                    else
                    {
                        tmpAngleOut=0-tmpAngleIn;
                    }

                    for(int k=0; k<tRotate.RotationTimeOut; k++)                //Ifor 20170412 (Steven) 判斷Out Rotate 旋轉角度是否已存在
                    {
                        if(tRotate.OutRotationCount[k]==tmpAngleOut || tmpAngleOut==0)   //Ifor 20240919 add:避免Count 多一次導致流程異常
                            bOutAngleNotExist=false;
                    }

                    if(bOutAngleNotExist==true)
                    {
                        tRotate.OutRotationCount[tRotate.RotationTimeOut]=tmpAngleOut;
                        if(tmpAngleOut==0) bIsAngleZero=true;                         //Eastsun 20260515 F023: E6 detect rotate 0 degree
                            tRotate.RotationTimeOut++;
                    }
                    bOutAngleNotExist=true;                                     //避免有一次相同角度後無法再次更新
                    //<==
                    //Ifor 20170407 add Rotate Out 獨立旋轉角度

                    if(CosFunction.bRotateUseRTmode==true)                      //jou 20231122 : Rotate Use RT mode
                    {
                        tmpAngleIn=tRotate.RotateDutDate[2][i][j];
                        for(int k=0; k<tRotate.RotationTimeInRT; k++)           //Ifor 20170412 (Steven) 判斷In Rotate 旋轉角度是否已存在
                        {
                            if(tRotate.RotationCountRT[k]==tmpAngleIn)
                                bInAngleNotExistRT=false;
                        }

                        if(bInAngleNotExistRT==true)
                        {
                            tRotate.RotationCountRT[tRotate.RotationTimeInRT]=tmpAngleIn;
                            tRotate.RotationTimeInRT++;
                        }
                        bInAngleNotExistRT=true;                                //避免有一次相同角度後無法再次更新

                        //====================計算 Rotate Out 旋轉角度====================
                        if(tRotate.bUseDifferentAngle==true)                    //Out Arm 使用不同角度
                        {
                            tmpAngleOut=360-tRotate.RotateDutDate[2][i][j]+tRotate.RotateDutDate[3][i][j]-360;
                        }
                        else
                        {
                            tmpAngleOut=0-tmpAngleIn;
                        }

                        for(int k=0; k<tRotate.RotationTimeOutRT; k++)          //Ifor 20170412 (Steven) 判斷Out Rotate 旋轉角度是否已存在
                        {
                            if(tRotate.OutRotationCountRT[k]==tmpAngleOut)
                                bOutAngleNotExistRT=false;
                        }

                        if(bOutAngleNotExistRT==true)
                        {
                            tRotate.OutRotationCountRT[tRotate.RotationTimeOutRT]=tmpAngleOut;
                            tRotate.RotationTimeOutRT++;
                        }
                        bOutAngleNotExistRT=true;                               //避免有一次相同角度後無法再次更新
                    }
                }
            }
        }
    }
    else
    {
        ZeroMemory(&tRotate.RotateDutDate ,sizeof(tRotate.RotateDutDate));
    }

    if(bShowRotateBySite==true)                                                 //Steven 20170329 (wei) : Add individual rotate motor
    {
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<8; j++)
            {
                FrmRotDegree[0][i][j]->SetDegree(tRotate.RotateDutDate[0][i][j]/90);

                if(tRotate.bUseDifferentAngle==true)
                {
                    tRotate.RotateDutDate[1][i][j]=360-tRotate.RotateDutDate[0][i][j]+tRotate.RotateDutDate[1][i][j]-360;
                }
                else
                {
                    tRotate.RotateDutDate[1][i][j]=0-tRotate.RotateDutDate[0][i][j];
                }
                FrmRotDegree[1][i][j]->SetDegree(tRotate.RotateDutDate[1][i][j]/90);
            }
        }
    }

    if(iRotate_Type==eInOutArm1Motor)                                           //Ifor 20251210 add:Rotate Offset
    {
        for(int i=0; i<2; i++)
        {
            if(i==0)
                str.sprintf("In Rotate Offset");
            else
                str.sprintf("Out Rotate Offset");
            tRotate.iRotateOffset[i]=ReadIniData(szDir, "SETTING", str, 0);
        }
    }

    slistIn->Clear();                                                           //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    slistOut->Clear();                                                          //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    slistInRT->Clear();
    slistOutRT->Clear();
    delete slistIn;
    delete slistOut;                                                            //Ifor 20170407 add Rotate Out 獨立旋轉角度
    delete slistInRT;
    delete slistOutRT;
}
//---------------------------------------------------------------------------
void TFrmRotate::DoIniDataToForm()
{
    cbActiveRotate->Checked             =tRotate.ActiveRotate;
    cbOutRotateDifferentAngle->Checked  =tRotate.bUseDifferentAngle;
    if(CosFunction.bPassBinNoRotate==true)                                      //jou 20231020 : Pass bin no rotate
        chkPassBinNoRotate->Checked     =tRotate.bPassBinNoRotate;
    if(CosFunction.bRotateUseRTmode==true)                                      //jou 20231122 : Rotate Use RT mode
        chkRotateUseRTmode->Checked     =tRotate.bRotateUseRTmode;
    if(CosFunction.bART_RT_NoRotate==true)                                      //Sam 20240809 : ART RT No Rotate
        chkART_RTNoRotate->Checked      =tRotate.bART_RT_NoRotate;
    rgDutNum->ItemIndex                 =tRotate.DutNum;
    iFromTrayAngle                      =tRotate.FromTrayAngle;
    memcpy(&iRotateDutDate, &tRotate.RotateDutDate, sizeof(tRotate.RotateDutDate));

    if(rgDutNum->ItemIndex==tDutType_8)                                         //20140320 wei   固定 4 Rotate Xpitch= 80mm; 8 Rotate Xpitch= 40mm
    {
        XPitch->Text = tRotate.RotateKit_PitchX;
        YPitch->Text = tRotate.RotateKit_PitchY;
    }
    else if(rgDutNum->ItemIndex==tDutType_1)                                    //Frank 20180512 (Steven) : modify 1 Dut 1 Motor
    {
        XPitch->Text=0;
        YPitch->Text=0;
        XPitch->Visible=false;
        YPitch->Visible=false;
    }
    else if(rgDutNum->ItemIndex==tDutType_2)                                    //Frank 20180512 (Steven) : modify 1 Dut 1 Motor
    {
        XPitch->Text=80;
        YPitch->Text=0;
        XPitch->Enabled=false;
        YPitch->Visible=false;
    }
    else //if(rgDutNum->ItemIndex==tDutType_4)
    {
        XPitch->Text = tRotate.RotateKit_PitchX;
        YPitch->Text = tRotate.RotateKit_PitchY;
    }

    if(iRotate_Type==eInOutArm1Motor)                                           //Ifor 20251210 add:Rotate Offset
    {
        for(int i=0; i<2; i++)
        {
            Rotate_Offset[i]->Text=tRotate.iRotateOffset[i];
        }
    }
    SetWorkParameter();
}
//---------------------------------------------------------------------------
void TFrmRotate::SetGridDraw()
{
    if(USE_ROTATE_KIT==1 && bShowRotateBySite==true)
    {
    }
    else
    {
        //===自動調整Show 出來的StringGrid 尺寸
        if(rgDutNum->ItemIndex==tDutType_8)                                              //20140320 wei   固定 4 Rotate Xpitch= 80mm; 8 Rotate Xpitch= 40mm
        {
            sgRotateIn->ColCount    =4;
            sgRotateIn->RowCount    =2;

            sgRotateOut->ColCount   =4;
            sgRotateOut->RowCount   =2;
        }
        else if(rgDutNum->ItemIndex==tDutType_1)
        {
            sgRotateIn->ColCount    =1;
            sgRotateIn->RowCount    =1;

            sgRotateOut->ColCount   =1;
            sgRotateOut->RowCount   =1;
        }
        else if(rgDutNum->ItemIndex==tDutType_2)
        {
            sgRotateIn->ColCount    =2;
            sgRotateIn->RowCount    =1;

            sgRotateOut->ColCount   =2;
            sgRotateOut->RowCount   =1;
        }
        else
        {
            sgRotateIn->ColCount    =2;
            sgRotateIn->RowCount    =2;

            sgRotateOut->ColCount   =2;
            sgRotateOut->RowCount   =2;
        }

        for(int i=0; i<sgRotateIn->ColCount; i++)
        {
            for(int j=0; j<sgRotateIn->RowCount; j++)
            {
                sgRotateIn->Cells[i][j] = 0;
                sgRotateOut->Cells[i][j] = 0;
            }
        }

        sgRotateIn->DefaultColWidth =ChangeToFloatNonPcnt((double)((sgRotateIn->Width -10)), (double)(sgRotateIn->ColCount));
        sgRotateIn->DefaultRowHeight=ChangeToFloatNonPcnt((double)((sgRotateIn->Height-10)), (double)(sgRotateIn->RowCount));

        sgRotateOut->DefaultColWidth =ChangeToFloatNonPcnt((double)((sgRotateIn->Width -10)), (double)(sgRotateIn->ColCount));
        sgRotateOut->DefaultRowHeight=ChangeToFloatNonPcnt((double)((sgRotateIn->Height-10)), (double)(sgRotateIn->RowCount));

        int iInRotate=0;
        int iOutRotate=0;
        for(int i=0; i<sgRotateIn->ColCount; i++)
        {
            for(int j=0; j<sgRotateIn->RowCount; j++)
            {                                                                   //Ifor 20170407 旋轉角度與顯示角度不同需再換算一次
                iInRotate =(int)(iRotateDutDate[0][j][i]+tRotate.FromTrayAngle)%360;
                iOutRotate=(int)(iRotateDutDate[1][j][i]+tRotate.FromTrayAngle)%360;

                sgRotateIn->Cells[i][j] =iInRotate;
                sgRotateOut->Cells[i][j]=iOutRotate;
            }
        }

        if(CosFunction.bRotateUseRTmode==true)                                  //jou 20231122 : Rotate Use RT mode
        {
            if(rgDutNum->ItemIndex==tDutType_8)
            {
                sgRotateInRT->ColCount    =4;
                sgRotateInRT->RowCount    =2;

                sgRotateOutRT->ColCount   =4;
                sgRotateOutRT->RowCount   =2;
            }
            else if(rgDutNum->ItemIndex==tDutType_1)
            {
                sgRotateInRT->ColCount    =1;
                sgRotateInRT->RowCount    =1;

                sgRotateOutRT->ColCount   =1;
                sgRotateOutRT->RowCount   =1;
            }
            else if(rgDutNum->ItemIndex==tDutType_2)
            {
                sgRotateInRT->ColCount    =2;
                sgRotateInRT->RowCount    =1;

                sgRotateOutRT->ColCount   =2;
                sgRotateOutRT->RowCount   =1;
            }
            else
            {
                sgRotateInRT->ColCount    =2;
                sgRotateInRT->RowCount    =2;

                sgRotateOutRT->ColCount   =2;
                sgRotateOutRT->RowCount   =2;
            }

            for(int i=0; i<sgRotateIn->ColCount; i++)
            {
                for(int j=0; j<sgRotateIn->RowCount; j++)
                {
                    sgRotateInRT->Cells[i][j] = 0;
                    sgRotateOutRT->Cells[i][j] = 0;
                }
            }

            sgRotateInRT->DefaultColWidth =ChangeToFloatNonPcnt((double)((sgRotateIn->Width -10)), (double)(sgRotateIn->ColCount));
            sgRotateInRT->DefaultRowHeight=ChangeToFloatNonPcnt((double)((sgRotateIn->Height-10)), (double)(sgRotateIn->RowCount));

            sgRotateOutRT->DefaultColWidth =ChangeToFloatNonPcnt((double)((sgRotateIn->Width -10)), (double)(sgRotateIn->ColCount));
            sgRotateOutRT->DefaultRowHeight=ChangeToFloatNonPcnt((double)((sgRotateIn->Height-10)), (double)(sgRotateIn->RowCount));

            for(int i=0; i<sgRotateIn->ColCount; i++)
            {
                for(int j=0; j<sgRotateIn->RowCount; j++)
                {                                                               //Ifor 20170407 旋轉角度與顯示角度不同需再換算一次
                    iInRotate =(int)(iRotateDutDate[2][j][i]+tRotate.FromTrayAngle)%360;
                    iOutRotate=(int)(iRotateDutDate[3][j][i]+tRotate.FromTrayAngle)%360;

                    sgRotateInRT->Cells[i][j] =iInRotate;
                    sgRotateOutRT->Cells[i][j]=iOutRotate;
                }
            }
        }
    }
    sgFTA->DefaultColWidth  =(sgFTA->Width-10);
    sgFTA->DefaultRowHeight =(sgFTA->Height-10);
    sgFTA->Cells[0][0]      =iFromTrayAngle;
}
//---------------------------------------------------------------------------
void TFrmRotate::DrawState(TStringGrid *lo, int x, int y, TRect &Rect)
{
    int iW              =lo->DefaultColWidth;
    int iH              =lo->DefaultRowHeight;
    int Quarter_X       =lo->DefaultColWidth/4;
    int Quarter_Y       =lo->DefaultRowHeight/4;
    AnsiString iAngle   =lo->Cells[x][y];

    if(iAngle==180)                                                             //Close Site
    {
        lo->Canvas->Pen->Color  =clWhite;
        lo->Canvas->Brush->Color=clGray;
        lo->Canvas->Rectangle(Rect);
        lo->Canvas->Pen->Color  =clWhite;
        lo->Canvas->Brush->Color=clWhite;
        lo->Canvas->Ellipse((x*iW)+iW-Quarter_X, (y*iH)+iH-Quarter_Y, (x*iW)+iW, (y*iH)+iH);
    }
    else if(iAngle==270)                                                        //Close Site
    {
        lo->Canvas->Pen->Color  =clWhite;
        lo->Canvas->Brush->Color=clGray;
        lo->Canvas->Rectangle(Rect);
        lo->Canvas->Pen->Color  =clWhite;
        lo->Canvas->Brush->Color=clWhite;
        lo->Canvas->Ellipse((x*iW), (y*iH)+iH, (x*iW)+Quarter_X, (y*iH)+iH-Quarter_Y);
    }
    else if(iAngle==0)
    {
        lo->Canvas->Pen->Color  =clWhite;
        lo->Canvas->Brush->Color=clGray;
        lo->Canvas->Rectangle(Rect);
        lo->Canvas->Pen->Color  =clWhite;
        lo->Canvas->Brush->Color=clWhite;
        lo->Canvas->Ellipse((x*iW),(y*iH), (x*iW)+Quarter_X, (y*iH)+Quarter_Y);
    }
    else if(iAngle==90)
    {
        lo->Canvas->Pen->Color  =clWhite;
        lo->Canvas->Brush->Color=clGray;
        lo->Canvas->Rectangle(Rect);
        lo->Canvas->Pen->Color  =clWhite;
        lo->Canvas->Brush->Color=clWhite;
        lo->Canvas->Ellipse((x*iW)+iW-Quarter_X, (y*iH), (x*iW)+iW, (y*iH)+Quarter_Y);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmRotate::sgRotateInDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
    DrawState(sgRotateIn, ACol, ARow, Rect);
    DrawState(sgRotateOut, ACol, ARow, Rect);

    if(CosFunction.bRotateUseRTmode==true && chkRotateUseRTmode->Checked==true)      //jou 20231122 : Rotate Use RT mode
    {
        DrawState(sgRotateInRT, ACol, ARow, Rect);
        DrawState(sgRotateOutRT, ACol, ARow, Rect);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmRotate::rgDutNumClick(TObject *Sender)
{
    if(rgDutNum->ItemIndex==tDutType_8)
        XPitch->Text=40;
    else if(rgDutNum->ItemIndex==tDutType_1)
        XPitch->Text=0;
    else if(rgDutNum->ItemIndex==tDutType_2)
        XPitch->Text=80;
    else
        XPitch->Text=80;
    SetGridDraw();
}
//---------------------------------------------------------------------------
void __fastcall TFrmRotate::sgRotateInSelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
    int tmpAngle, tmpAngle1;

    if(pgcRotate->TabIndex==0)                                                  //旋轉角度
        tmpAngle=iRotateDutDate[0][ARow][ACol]+90;
    else if(pgcRotate->TabIndex==1)
        tmpAngle=iRotateDutDate[1][ARow][ACol]+90;
    else if(CosFunction.bRotateUseRTmode==true && pgcRotate->ActivePage==tsM1RotateInRT) //jou 20231122 : Rotate Use RT mode
        tmpAngle=iRotateDutDate[2][ARow][ACol]+90;
    else if(CosFunction.bRotateUseRTmode==true && pgcRotate->ActivePage==tsM1RotateOutRT) //jou 20231122 : Rotate Use RT mode
        tmpAngle=iRotateDutDate[3][ARow][ACol]+90;

    tmpAngle=tmpAngle%360;
    tmpAngle1=tmpAngle+iFromTrayAngle;                                          //顯示角度位置
    tmpAngle1=tmpAngle1%360;
    Edit1->Text=tmpAngle;

    if(cbOutRotateDifferentAngle->Checked==true)
    {
        if(pgcRotate->TabIndex==0)
        {
            if(rgSelect->ItemIndex==0)
            {
               for(int i=0; i<sgRotateIn->ColCount; i++)
                {
                    for(int j=0; j<sgRotateIn->RowCount; j++)
                    {
                        iRotateDutDate[0][j][i]     = tmpAngle;
                        sgRotateIn->Cells[i][j]     = tmpAngle1;
                    }
                }
            }
            else
            {
                iRotateDutDate[0][ARow][ACol]       = tmpAngle;
                sgRotateIn->Cells[ACol][ARow]       = tmpAngle1;
            }
        }
        else if(pgcRotate->TabIndex==1)
        {
            if(rgSelect->ItemIndex==0)
            {
               for(int i=0; i<sgRotateOut->ColCount; i++)
                {
                    for(int j=0; j<sgRotateOut->RowCount; j++)
                    {
                        iRotateDutDate[1][j][i]     = tmpAngle;
                        sgRotateOut->Cells[i][j]    = tmpAngle1;
                    }
                }
            }
            else
            {
                iRotateDutDate[1][ARow][ACol]       = tmpAngle;
                sgRotateOut->Cells[ACol][ARow]      = tmpAngle1;
            }
        }
        else if(CosFunction.bRotateUseRTmode==true && pgcRotate->ActivePage==tsM1RotateInRT)      //jou 20231122 : Rotate Use RT mode
        {
            if(rgSelect->ItemIndex==0)
            {
               for(int i=0; i<sgRotateInRT->ColCount; i++)
                {
                    for(int j=0; j<sgRotateInRT->RowCount; j++)
                    {
                        iRotateDutDate[2][j][i]     = tmpAngle;
                        sgRotateInRT->Cells[i][j]   = tmpAngle1;
                    }
                }
            }
            else
            {
                iRotateDutDate[2][ARow][ACol]       = tmpAngle;
                sgRotateInRT->Cells[ACol][ARow]     = tmpAngle1;
            }
        }
        else if(CosFunction.bRotateUseRTmode==true && pgcRotate->ActivePage==tsM1RotateOutRT)      //jou 20231122 : Rotate Use RT mode
        {
            if(rgSelect->ItemIndex==0)
            {
               for(int i=0; i<sgRotateOutRT->ColCount; i++)
                {
                    for(int j=0; j<sgRotateOutRT->RowCount; j++)
                    {
                        iRotateDutDate[3][j][i]     = tmpAngle;
                        sgRotateOutRT->Cells[i][j]  = tmpAngle1;
                    }
                }
            }
            else
            {
                iRotateDutDate[3][ARow][ACol]       = tmpAngle;
                sgRotateOutRT->Cells[ACol][ARow]    = tmpAngle1;
            }
        }
    }
    else
    {
        if(rgSelect->ItemIndex==0)
        {
            for(int i=0; i<sgRotateIn->ColCount; i++)
            {
                for(int j=0; j<sgRotateIn->RowCount; j++)
                {
                    iRotateDutDate[0][j][i]     = tmpAngle;
                    iRotateDutDate[1][j][i]     = tmpAngle;
                    sgRotateIn->Cells[i][j]     = tmpAngle1;
                    sgRotateOut->Cells[i][j]    = tmpAngle1;

                    if(CosFunction.bRotateUseRTmode==true && chkRotateUseRTmode->Checked==true)      //jou 20231122 : Rotate Use RT mode
                    {
                        iRotateDutDate[2][j][i]     = tmpAngle;
                        iRotateDutDate[3][j][i]     = tmpAngle;
                        sgRotateInRT->Cells[i][j]   = tmpAngle1;
                        sgRotateOutRT->Cells[i][j]  = tmpAngle1;
                    }
                }
            }
        }
        else
        {
            iRotateDutDate[0][ARow][ACol]       = tmpAngle;
            iRotateDutDate[1][ARow][ACol]       = tmpAngle;
            sgRotateIn->Cells[ACol][ARow]       = tmpAngle1;
            sgRotateOut->Cells[ACol][ARow]      = tmpAngle1;

            if(CosFunction.bRotateUseRTmode==true && chkRotateUseRTmode->Checked==true)      //jou 20231122 : Rotate Use RT mode
            {
                iRotateDutDate[2][ARow][ACol]       = tmpAngle;
                iRotateDutDate[3][ARow][ACol]       = tmpAngle;
                sgRotateInRT->Cells[ACol][ARow]     = tmpAngle1;
                sgRotateOutRT->Cells[ACol][ARow]    = tmpAngle1;
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmRotate::sgFTADrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
   DrawState(sgFTA, ACol, ARow, Rect);
}
//---------------------------------------------------------------------------
void __fastcall TFrmRotate::sgFTASelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
    int tmpAngle;

    if(HasICUnderMachine())                                                     //Ifor 20170410 機台內有IC 不可變更Rotate方向避免轉錯方向
        return;

    iFromTrayAngle=iFromTrayAngle+90;

    if(iFromTrayAngle>270)
        iFromTrayAngle=0;

    sgFTA->Cells[ACol][ARow]=iFromTrayAngle;

    for(int i=0; i<2; i++)                                                      //Ifor 20170410 (Steven) 更新Tray From Angle 需同時更新 各Rotate旋轉角度
    {
        for(int j=0; j<4; j++)
        {
            for(int k=0; k<8; k++)
            {
                //旋轉角度
                tmpAngle=iRotateDutDate[i][j][k];
                if(tmpAngle>0)
                {
                    tmpAngle-=90;
                    tmpAngle=tmpAngle%360;
                }
                else
                {
                    tmpAngle=270;
                }
                iRotateDutDate[i][j][k]=tmpAngle;
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmRotate::spbSaveClick(TObject *Sender)
{
    if(IniConfig.bA02DisableSaveParsWhenSwitchToOp==true &&                     //RogerYang 20260305 張寧說時間到都要登出，且不可被更改任何設定(A01_2)
        AccessLevel==0)
    {
        ShowMyMessage("[A01_2]目前已切換到Operator權限，\r\n請重新登入再做設定!");
        Close();
    }

    AnsiString tmpA="", tmpB="", tmpC="", tmpD="",tmpB_RT="",tmpD_RT="";
    AnsiString S="", str;
    bool bSettingErr = true;

    S=GetLastOpenFN();
    AnsiString szDir="";
    szDir.sprintf("%s%s\\Rotate.Data", DataPath, S);
    Label_Err->Visible =false;

    for(int i=0; i<4; i++)
    {
        for(int j=0; j<8; j++)
        {
            tmpA.sprintf("%0d,", iRotateDutDate[0][i][j]);
            tmpB+=tmpA;

            tmpC.sprintf("%0d,", iRotateDutDate[1][i][j]);                      //Ifor 20170407 add Rotate Out 獨立旋轉角度
            tmpD+=tmpC;

            if(iFromTrayAngle!=iRotateDutDate[0][i][j] ||
               iFromTrayAngle!=iRotateDutDate[1][i][j])
                bSettingErr=false;

            if(CosFunction.bRotateUseRTmode==true)                              //jou 20231122 : Rotate Use RT mode
            {
                tmpA.sprintf("%0d,", iRotateDutDate[2][i][j]);
                tmpB_RT+=tmpA;

                tmpC.sprintf("%0d,", iRotateDutDate[3][i][j]);                  //Ifor 20170407 add Rotate Out 獨立旋轉角度
                tmpD_RT+=tmpC;

                if(iFromTrayAngle!=iRotateDutDate[2][i][j] ||
                   iFromTrayAngle!=iRotateDutDate[3][i][j])
                    bSettingErr=false;
            }
        }
    }

    if(bSettingErr)
    {
        Label_Err->Visible =true;
        Label_Err->Caption ="*與入料角度相同不存檔!!!";
        return;
    }

    if(tRotate.ActiveRotate==true &&                                            //KevinCheng 20260318 : Rotate流程未完，不可關閉。
       cbActiveRotate->Checked==false &&
       IsRotateFlowNotFinish())
    {
        Label_Err->Visible =true;
        Label_Err->Caption ="*Rotate流程未完成，不可關閉Active Rotate!!!";
        cbActiveRotate->Checked=true;
        return;
    }

    if(CosFunction.bART_RT_NoRotate)                                            //Sam 20240809 : ART RT No Rotate
    {
        for(int iR=0; iR<sgRotateIn->RowCount; iR++)
        {
            for(int iC=0; iC<sgRotateIn->ColCount; iC++)
            {
                if(iRotateDutDate[0][0][0]!=iRotateDutDate[0][iR][iC])
                    bSettingErr=true;
            }
        }

        if(cbOutRotateDifferentAngle->Checked)
            bSettingErr=true;

        if(bSettingErr)
        {
            if(bCanRunSCKART && chkART_RTNoRotate->Checked)
            {
                Label_Err->Visible =true;
                Label_Err->Caption ="*ART RT No Rotate* function only support the same rotate angle!!!";
                return;
            }
            else
            {
                chkART_RTNoRotate->Checked=false;
            }
        }
    }

    WriteIniData(szDir, "SETTING", "ActiveRotate",  cbActiveRotate->Checked);
    WriteIniData(szDir, "SETTING", "Dut Num",       rgDutNum->ItemIndex);
    WriteIniData(szDir, "SETTING", "FromTrayAngle", iFromTrayAngle);

    tmpB.Delete(tmpB.Length() ,1);
    WriteIniData(szDir, "SETTING", "DutAngle",          tmpB);

    tmpD.Delete(tmpD.Length() ,1);                                              //Ifor 20170407 add Rotate Out 獨立旋轉角度
    WriteIniData(szDir, "SETTING", "DutAngleOut",       tmpD);

    if(CosFunction.bRotateUseRTmode==true)                                      //jou 20231122 : Rotate Use RT mode
    {
        tmpB_RT.Delete(tmpB_RT.Length() ,1);
        WriteIniData(szDir, "SETTING", "DutAngleRT",          tmpB_RT);

        tmpD_RT.Delete(tmpD_RT.Length() ,1);                                    //Ifor 20170407 add Rotate Out 獨立旋轉角度
        WriteIniData(szDir, "SETTING", "DutAngleOutRT",       tmpD_RT);
    }

    WriteIniData(szDir, "SETTING", "OutRotateDifferentAngle",  cbOutRotateDifferentAngle->Checked);
    if(CosFunction.bPassBinNoRotate==true)                                      //jou 20231020 : Pass bin no rotate
        WriteIniData(szDir, "SETTING", "bPassBinNoRotate",         chkPassBinNoRotate->Checked);
    if(CosFunction.bRotateUseRTmode==true)                                      //jou 20231122 : Rotate Use RT mode
        WriteIniData(szDir, "SETTING", "bRotateUseRTmode",         chkRotateUseRTmode->Checked);
    if(CosFunction.bART_RT_NoRotate==true)                                      //Sam 20240809 : ART RT No Rotate
        WriteIniData(szDir, "SETTING", "bART_RT_NoRotate",         chkART_RTNoRotate->Checked);
    if(USE_ROTATE_KIT==1 && bShowRotateBySite==true)
    {
//        WriteIniData(szDir, "SETTING", "ColCount",      tRotate.ColCount);
//        WriteIniData(szDir, "SETTING", "RowCount",      tRotate.RowCount);
    }
    else
    {
        WriteIniData(szDir, "SETTING", "ColCount",      sgRotateIn->ColCount);
        WriteIniData(szDir, "SETTING", "RowCount",      sgRotateIn->RowCount);
    }
    WriteIniData(szDir, "SETTING", "KitPitchX",     atof(XPitch->Text.c_str()));
    WriteIniData(szDir, "SETTING", "KitPitchY",     atof(YPitch->Text.c_str()));
    fRotate_ReadFile();
    if(IniConfig.bEnable_SECS_GEM==true)
    {
        bHasSaveSet=true;                                                       //Ifor 20151204 新增判斷機台有無修改設定檔
    }

    if(iRotate_Type==eInOutArm1Motor)                                           //Ifor 20251210 add:Rotate Offset
    {
        for(int i=0; i<2; i++)
        {
            if(i==0)
                str.sprintf("In Rotate Offset");
            else
                str.sprintf("Out Rotate Offset");

            WriteIniData(szDir, "SETTING", str, Rotate_Offset[i]->Text);
        }
    }

    fMain->BackupSetupFile();                                                   //Ifor 20170620 (wei) add Auto BackUp Setup File & Last Data
}
//---------------------------------------------------------------------------
void __fastcall TFrmRotate::sbtExitClick(TObject *Sender)
{
    rgDutNum->Enabled=true;
    rgSelect->Enabled=true;
    sbtExit->Down =false;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFrmRotate::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    bRotateChange=true;                                                         //Frank 20150626 : for矽格 要有IC方向顯示
    fShow=false;
    DoIniDataToForm();                                                          //JerryYang 20250411 : 離開頁面要刷新一次, 避免誤存檔

    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20170405 : add change log for rotate
}
//---------------------------------------------------------------------------
void __fastcall TFrmRotate::XPitchKeyPress(TObject *Sender, char &Key)
{
    if(OnlyNumberAndDotInPut(Key)==false)
        Key=NULL;
}
//---------------------------------------------------------------------------
void TFrmRotate::InitialInRotateHome()                                          //Steven 20170329 : Add individual rotate motor
{
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<4; j++)
        {
            b8RotateInHome[i][j]=false;
            InitProcessSingleMotorTask(MInRotate[i][j]);
        }
    }
}
//---------------------------------------------------------------------------
bool TFrmRotate::DoInRotateHome()                                               //Steven 20170329 : Add individual rotate motor
{
    bool bResult=true;
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<4; j++)
        {
            //if(b8RotateOutHome[i][j]==false)
            //==>
            if(b8RotateInHome[i][j]==false)                                     //Eastsun 20260518 修正
            {
                bResult=false;
                //b8RotateOutHome[i][j]=ProcessSingleMotorHome(MInRotate[i][j]);
                //==>
                b8RotateInHome[i][j]=ProcessSingleMotorHome(MInRotate[i][j]);   //Eastsun 20260518 修正
            }
        }
    }
    return bResult;
}
//---------------------------------------------------------------------------
void TFrmRotate::InitialOutRotateHome()                                         //Steven 20170329 : Add individual rotate motor
{
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<4; j++)
        {
            b8RotateOutHome[i][j]=false;
            InitProcessSingleMotorTask(MOutRotate[i][j]);                       //Eastsun 20260518 新增修正
        }
    }
}
//---------------------------------------------------------------------------
bool TFrmRotate::DoOutRotateHome()                                              //Steven 20170329 : Add individual rotate motor
{
    bool bResult=true;
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<4; j++)
        {
            if(b8RotateOutHome[i][j]==false)
            {
                bResult=false;
                b8RotateOutHome[i][j]=ProcessSingleMotorHome(MOutRotate[i][j]);
            }
        }
    }
    return bResult;
}
//---------------------------------------------------------------------------
void TFrmRotate::SetInRotateSpeed(int iACDCSP, int iBodySP)                     //Steven 20170329 : Add individual rotate motor
{
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<4; j++)
        {
            SetMotorAccelSpeed(MInRotate[i][j], iACDCSP);
            SetMotorScaleSpeed(MInRotate[i][j], iBodySP);
        }
    }
}
//---------------------------------------------------------------------------
void TFrmRotate::SetOutRotateSpeed(int iACDCSP, int iBodySP)                    //Steven 20170329 : Add individual rotate motor
{
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<4; j++)
        {
            SetMotorAccelSpeed(MOutRotate[i][j], iACDCSP);
            SetMotorScaleSpeed(MOutRotate[i][j], iBodySP);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmRotate::rgSelectClick(TObject *Sender)
{
    //Steven 20170329 : Add individual rotate motor
    //==>
    if(USE_ROTATE_KIT==1 && bShowRotateBySite==true)
    {
        if(rgSelect->ItemIndex==0)
        {
            for(int i=0; i<MAX_SOCKET_ROW; i++)
            {
                for(int j=0; j<MAX_SOCKET_COL; j++)
                {
                    FrmRotDegree[0][i][j]->rgRotateDegree->ItemIndex=0;
                    FrmRotDegree[0][i][j]->ReloadPicture();
                    FrmRotDegree[1][i][j]->rgRotateDegree->ItemIndex=0;
                    FrmRotDegree[1][i][j]->ReloadPicture();
                }
            }
        }
    }
    //<==
    //Steven 20170329 : Add individual rotate motor
}
//---------------------------------------------------------------------------
void __fastcall TFrmRotate::cbOutRotateDifferentAngleClick(TObject *Sender)
{
    if(USE_ROTATE_KIT==1 && bShowRotateBySite==true)
    {
        tsM8RotateOut->TabVisible=cbOutRotateDifferentAngle->Checked;
    }
    else
    {
        tsM1RotateOut->TabVisible   =cbOutRotateDifferentAngle->Checked;
        tsM1RotateInRT->TabVisible  =CosFunction.bRotateUseRTmode==true && chkRotateUseRTmode->Checked==true;    //jou 20231122 : Rotate Use RT mode
        tsM1RotateOutRT->TabVisible =CosFunction.bRotateUseRTmode==true && cbOutRotateDifferentAngle->Checked && chkRotateUseRTmode->Checked==true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmRotate::YPitchMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(IniConfig.bSPILFunction==true)
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 0.0, 150.0);
    else
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 3, true, 0.0, 150.0);
}
//---------------------------------------------------------------------------
void __fastcall TFrmRotate::FormShortCut(TWMKey &Msg, bool &Handled)
{
    if(Msg.CharCode==VK_F5)                                                     //Sam 20220725 : 介面新增鍵盤 F5 將視窗恢復初始位置
    {
        Left=75;
        Top=10;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmRotate::chkRotateUseRTmodeClick(TObject *Sender)
{
    if(USE_ROTATE_KIT==1 && bShowRotateBySite==true)
    {
        chkRotateUseRTmode->Checked=false;
        return;
    }

    tsM1RotateInRT->TabVisible  =(USE_ROTATE_KIT==1 && bShowRotateBySite==false) &&
                                 (CosFunction.bRotateUseRTmode==true && chkRotateUseRTmode->Checked==true);      //jou 20231122 : Rotate Use RT mode
    tsM1RotateOutRT->TabVisible =(USE_ROTATE_KIT==1 && bShowRotateBySite==false) &&
                                 (CosFunction.bRotateUseRTmode==true && cbOutRotateDifferentAngle->Checked && chkRotateUseRTmode->Checked==true);      //jou 20231122 : Rotate Use RT mode
}
//---------------------------------------------------------------------------
void __fastcall TFrmRotate::edInRotate_0MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 1, true, (double)1250.0, (double)-1250.0);
}
//---------------------------------------------------------------------------
