#include "MachineDefine.h"
#pragma hdrstop

#include "uTrayEditForm.h"
#include "cprod.h"
#include "cpublic.h"
#include "mymotor.h"
//#include "INPUT.h"
#include "main.h"
#include "mymessbox.h"
//#include "note.h"
#include "cMyDB.h"
#include "common.h"
#include "cSecurity.h"
#include "cmydef.h"
#include "cSortCT.h"
#include "myQwertyKeyBoard.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma link "HTray"
#pragma resource "*.dfm"
TTrayEditForm *TrayEditForm;
bool bMouseDown=false;
int iStartX,iStartY,iEndX,iEndY,iOldStartX,iOldStartY;
int BackTray[100][100];
TRAY_TYPE_PARA *TempTray=&TrayForm.Loader;
int iEditMotorIndex;
int ICType;

bool bEnterSave=true; //ChungHung 20130715 add
//---------------------------------------------------------------------------
void EditTray(int MotorIndexIndex, int iHasMap)
{
    int iTrayNum[16]={MMTrayY, MMAuto1, MMAuto2, MMAuto3,
                      MManualTray1, MManualTray2, MManualTray3, MMScanAOI,
                      MMPlate1, MMPlate2,                                       //Steven 20230301 : Add for 確安
                      MMAuto4, MMAuto5, MMAuto6,                                //Steven 20230907 : For HT-9011UC
                      MManualTray4, MManualTray5, MManualTray6};
    AnsiString asName[16]={"Loader", "Auto 1", "Auto 2", "Auto 3",
                           "Fix 1", "Fix 2", "Fix 3", "AOI",
                           "Hot Plate 1", "Hot Plate 2",                        //Steven 20230301 : Add for 確安
                           "Auto 4", "Auto 5", "Auto 6",
                           "Fix 4", "Fix 5", "Fix 6"};
    AnsiString asString;

    for(int i=0; i<16; i++)
    {
        if(MotorIndexIndex==iTrayNum[i])
        {
            asString=asName[i];
        }
    }
    NewRecordProcess("MES21108", "Enter Tray Edit Form", asString);

    iEditMotorIndex=MotorIndexIndex;
    TrayEditForm->iHasMap=iHasMap;                                              //Steven 20150224 : Auto Contact Test
    TrayEditForm->ShowModal();
}
//---------------------------------------------------------------------------
__fastcall TTrayEditForm::TTrayEditForm(TComponent* Owner)
    : TForm(Owner)
{
    fShow=false;
    #ifdef SOFT_SIMULTE
        BitBtn1->Visible=true;
    #endif
}
//---------------------------------------------------------------------------
void __fastcall TTrayEditForm::FormShow(TObject *Sender)
{
    CheckBox1->Checked=false;    //Steven 20131226 : 避免異常無法跳出
    iSendCT=LastSet.SendCT[0];
    Left=(1024-Width)/2;
    Top =(768-Height)/2;
    fShow=true;

    mtLoaderBuffer->XBlockItem=MOT[iEditMotorIndex].Tray.XBItem;        //Frank 20160928 add Subtray Function
    mtLoaderBuffer->YBlockItem=MOT[iEditMotorIndex].Tray.YBItem;

    mtLoaderBuffer->XBlockWidth=MOT[iEditMotorIndex].Tray.XBWidth;
    mtLoaderBuffer->YBlockWidth=MOT[iEditMotorIndex].Tray.YBWidth;

    mtLoaderBuffer->XItem=MOT[iEditMotorIndex].Tray.XItem;
    mtLoaderBuffer->YItem=MOT[iEditMotorIndex].Tray.YItem;
    int i, j;
    AnsiString S;
    ICType=HAS_IC;

    if(INSTALL_OCR!=eocrUninstal && TestIF.bOcrFunction)        //wei 20160613 ocr連線測試
    {
        mtLoaderBuffer->SetColorMap(HAS_OCR_OK, (TColor)clRed);
    }

    if(iHasMap==2)
    {
        cbBinCount->Clear();
        for(i=0; i<=6; i++)
            cbBinCount->Items->Add(i);
    }

    if(iHasMap==2)  //Steven 20150915 : For TSMC 手動整盤功能
    {
        for(i=0; i<MOT[iEditMotorIndex].Tray.XItem; i++)
        {
            for(j=0; j<MOT[iEditMotorIndex].Tray.YItem; j++)
            {
                if(MOT[iEditMotorIndex].Tray.Data[i][j]==NULL_IC)
                {
                    BackTray[i][j]=0;
                    mtLoaderBuffer->SetCellColorIndex(i, j, 0);
                }
                else
                {
                    BackTray[i][j]=MOT[iEditMotorIndex].Tray.iTarget[i][j];
                    mtLoaderBuffer->SetCellColorIndex(i, j, BackTray[i][j]);
                }
            }
        }
    }
    else if(iHasMap==3) //Sam 20240108 : 新增退 Tray 時顯示裡面有多少 Error Bin
    {
        for(i=0; i<MOT[iEditMotorIndex].Tray.XItem; i++)
        {
            for(j=0; j<MOT[iEditMotorIndex].Tray.YItem; j++)
            {
                if((MOT[iEditMotorIndex].Tray.Data[i][j]==HAS_IC && MOT[iEditMotorIndex].Tray.iBinCode[i][j]==TEST_PASS+iTestBinCount) ||
                    MOT[iEditMotorIndex].Tray.Data[i][j]==HAS_BARCODEERROR_IC)      //Sam 20240703 : 修正 2D Error 沒有顯示位置
                {
                    mtLoaderBuffer->SetCellColorIndex(i, j, 4); //顯示紅色
                }
                else if(MOT[iEditMotorIndex].Tray.Data[i][j]==HAS_IC)
                {
                    mtLoaderBuffer->SetCellColorIndex(i, j, 1); //顯示綠色
                }
            }
        }
    }
    else if(iHasMap==4) //Sam 20240325 : 新增 DamageTrayMapping 功能
    {
        for(i=0; i<MOT[iEditMotorIndex].Tray.XItem; i++)
        {
            for(j=0; j<MOT[iEditMotorIndex].Tray.YItem; j++)
            {
                if(MOT[iEditMotorIndex].Tray.Data[i][j]==HAS_IC && MOT[iEditMotorIndex].Tray.iAOIResult[i][j]==4)
                {
                    mtLoaderBuffer->SetCellColorIndex(i, j, 4); //顯示紅色
                }
                else if(MOT[iEditMotorIndex].Tray.Data[i][j]==HAS_IC)
                {
                    mtLoaderBuffer->SetCellColorIndex(i, j, 1); //顯示綠色
                }
            }
        }
    }
    else
    {
        for(i=0; i<MOT[iEditMotorIndex].Tray.XItem; i++)
        {
            for(j=0; j<MOT[iEditMotorIndex].Tray.YItem; j++)
            {
                if(MOT[iEditMotorIndex].Tray.Data[i][j]!=NULL_IC)
                {
                    if(INSTALL_OCR!=eocrUninstal && TestIF.bOcrFunction && MOT[iEditMotorIndex].Tray.Data[i][j]==HAS_OCR_OK)        //wei 20160613 ocr連線測試
                    {
                        MOT[iEditMotorIndex].SetTraySingleData(i, j, HAS_OCR_OK);
                        mtLoaderBuffer->SetCellColorIndex(i, j, HAS_OCR_OK);
                        ICType=MOT[iEditMotorIndex].Tray.Data[i][j];
                    }
                    else if(INSTALL_OCR!=eocrUninstal && TestIF.bOcrFunction && MOT[iEditMotorIndex].Tray.Data[i][j]==HAS_OCR_Err)
                    {
                        MOT[iEditMotorIndex].SetTraySingleData(i, j, HAS_OCR_Err);
                        mtLoaderBuffer->SetCellColorIndex(i, j, HAS_OCR_Err);
                        ICType=MOT[iEditMotorIndex].Tray.Data[i][j];
                    }
                    else if(MOT[iEditMotorIndex].Tray.Data[i][j]==HAS_NULL_IC)  //Ifor 20170807 (wei) 避免Has Null IC進入編輯頁面變成Has Ic
                    {
                        MOT[iEditMotorIndex].SetTraySingleData(i, j, HAS_NULL_IC);
                        mtLoaderBuffer->SetCellColorIndex(i, j, HAS_NULL_IC);
                        ICType=MOT[iEditMotorIndex].Tray.Data[i][j];
                    }
                    else
                    {
                        MOT[iEditMotorIndex].SetTraySingleData(i, j, HAS_IC);
                        mtLoaderBuffer->SetCellColorIndex(i, j, 1);
                        ICType=MOT[iEditMotorIndex].Tray.Data[i][j];
                    }
                }
                else
                {
                    mtLoaderBuffer->SetCellColorIndex(i, j, 0);
                }

                if(MOT[iEditMotorIndex].Tray.Data[i][j]==HAS_HOT_IC || MOT[iEditMotorIndex].Tray.Data[i][j]==HAS_IC) //kevin 20150819  load tray 無法編輯
                {
                    BackTray[i][j]=1;
                }
                else if(INSTALL_OCR!=eocrUninstal && TestIF.bOcrFunction && MOT[iEditMotorIndex].Tray.Data[i][j]==HAS_OCR_OK)       //wei 20160613 ocr連線測試
                {
                    BackTray[i][j]=HAS_OCR_OK;
                }
                else if(INSTALL_OCR!=eocrUninstal && TestIF.bOcrFunction && MOT[iEditMotorIndex].Tray.Data[i][j]==HAS_OCR_Err)
                {
                    BackTray[i][j]=HAS_OCR_Err;
                }
                else if(MOT[iEditMotorIndex].Tray.Data[i][j]==HAS_NULL_IC)      //Ifor 20170807 (wei) 避免Has Null IC進入編輯頁面變成Has Ic
                {
                    BackTray[i][j]=HAS_NULL_IC;
                }
                else
                {
                    BackTray[i][j]=0;
                }

                if(iHasMap==0)  //Steven 20150224 : Auto Contact Test
                {
                    if(i==0 || j==0)
                    {
                        if(i==0)
                            mtLoaderBuffer->SetCellNumber(i, j, j+1);
                        else
                            mtLoaderBuffer->SetCellNumber(i, j, i+1);
                    }
                }
            }
        }
    }
    bEnterSave=true; //ChungHung 20130715 add

    spbUpdate->Visible=!(iEditMotorIndex==MMPlate1 || iEditMotorIndex==MMPlate2);   //Steven 20230301 : Add for 確安
}
//---------------------------------------------------------------------------
void __fastcall TTrayEditForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    //ChungHung 20130715 add
    SaveJPG("Leave");
    MySleep(100);
    fShow=false;

    if(CosFunction.bUseEditLDTrayNeedManualRemoveTray==true)    //Ifor 20200331 : add Edit Loader Tray Need Manual Remove Tray
    {
        iLDTrayNeedManualRemoveTray=0;  //0:Normal 1:Edit Loader Tray 2:Data Change
    }
    else
    {
        iLDTrayNeedManualRemoveTray=0;  //0:Normal 1:Edit Loader Tray 2:Data Change
        bNeedManualRemoveTray=false;
    }

    if(CosFunction.bShowHPICCount)     //Steven 20221228 : 計算加熱盤IC數量
    {
        fSortCT->pnlLoad->Caption=MOT[MMTrayY].Tray.HowManyIC();
    }
}
//---------------------------------------------------------------------------
void __fastcall TTrayEditForm::mtLoaderBufferMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(iEditMotorIndex==MMPlate1 || iEditMotorIndex==MMPlate2)                  //Steven 20230301 : Add for 確安
        return;

    if(CUSTOMER_CODE==CC_SIGURD_HUKOU)  //KaiChen 20201112 ：矽格-湖口，確認是否可以編輯 Loader/Unloader資料
    {
        if(CheckCanEdit_SG(iEditMotorIndex)==false)
        {
            if(iEditMotorIndex==MMTrayY)
            {
                ShowMyMessage("Tray Data Not Edit! (Loader)");
            }
            else
            {
                ShowMyMessage("Tray Data Not Edit! (Unloader)");
            }

            return;
        }
    }

    if(CosFunction.bUnloaderEditTrayLevelSet)     //Steven 20191224 : Unloader編輯Tray改用另外一組權限
    {
        if(iEditMotorIndex==MMTrayY)
        {
            if(fSecurity->Insufficient(104)==false)
                return;
        }
        else
        {
            if(fSecurity->Insufficient(160)==false)
                return;
        }
    }
    else
    {
        if(fSecurity->Insufficient(104)==false)         //Steven 20150505 : SPIL蘇州要求Tray編輯要有權限
        {
            return;
        }
    }

    mtLoaderBuffer->ConvertIndexCells(X, Y);
    if(X<0 || X>=mtLoaderBuffer->XItem || Y<0 || Y>=mtLoaderBuffer->YItem)
        return;
    bMouseDown=true;
    iStartX=X;
    iStartY=Y;
    iEndX=X;
    iEndY=Y;

    if(CosFunction.bUseEditLDTrayNeedManualRemoveTray==true && iLDTrayNeedManualRemoveTray==1)//0:Normal 1:Edit Loader Tray 2:Data Change
    {
        iLDTrayNeedManualRemoveTray=2;
    }
}
//---------------------------------------------------------------------------
void __fastcall TTrayEditForm::mtLoaderBufferMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    if(iEditMotorIndex==MMPlate1 || iEditMotorIndex==MMPlate2)                  //Steven 20230301 : Add for 確安
        return;

    if(bMouseDown)
    {
        mtLoaderBuffer->ConvertIndexCells(X, Y);
        if(X<0 || X>=mtLoaderBuffer->XItem || Y<0 || Y>=mtLoaderBuffer->YItem)
            return;
        iEndX=X;
        iEndY=Y;
        ShowTray();
    }
}
//---------------------------------------------------------------------------
void __fastcall TTrayEditForm::mtLoaderBufferMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(iEditMotorIndex==MMPlate1 || iEditMotorIndex==MMPlate2)                  //Steven 20230301 : Add for 確安
        return;

    if(bMouseDown)
    {
        bMouseDown=false;
        SetTray();
    }
}
//---------------------------------------------------------------------------
void __fastcall TTrayEditForm::Change()
{
    int iSX,iSY,iEX,iEY;
    if(iStartX>iEndX)
    {
        iSX=iEndX;
        iEX=iStartX;
    }
    else
    {
        iSX=iStartX;
        iEX=iEndX;
    }
    if(iStartY>iEndY)
    {
        iSY=iEndY;
        iEY=iStartY;
    }
    else
    {
        iSY=iStartY;
        iEY=iEndY;
    }
    iStartX=iSX;
    iStartY=iSY;
    iEndX=iEX;
    iEndY=iEY;
}
//---------------------------------------------------------------------------
void __fastcall TTrayEditForm::ShowTray()
{
    int iSX, iSY, iEX, iEY, i, j;
    if(iStartX>iEndX)
    {
        iSX=iEndX;
        iEX=iStartX;
    }
    else
    {
        iSX=iStartX;
        iEX=iEndX;
    }
    if(iStartY>iEndY)
    {
        iSY=iEndY;
        iEY=iStartY;
    }
    else
    {
        iSY=iStartY;
        iEY=iEndY;
    }
    //============
    for(i=0; i<MOT[iEditMotorIndex].Tray.XItem; i++)
    {
        for(j=0; j<MOT[iEditMotorIndex].Tray.YItem; j++)
        {
            if(i>=iSX && i<=iEX)
            {
                if(j>=iSY && j<=iEY)
                {
                    mtLoaderBuffer->SetCellColorIndex(i, j, 2);
                }
                else
                {
                    if(iHasMap==2)  //Steven 20150915 : For TSMC 手動整盤功能
                    {
                        if(BackTray[i][j]==0)
                            mtLoaderBuffer->SetCellColorIndex(i, j, 0);
                        else if(BackTray[i][j]==2)   //Steven 20150224 : Auto Contact Test
                            mtLoaderBuffer->SetCellColorIndex(i, j, BackTray[i][j]);
                    }
                    else
                    {
                        if(BackTray[i][j]==1)
                            mtLoaderBuffer->SetCellColorIndex(i, j, 1);
                        else if(BackTray[i][j]==2)   //Steven 20150224 : Auto Contact Test
                            mtLoaderBuffer->SetCellColorIndex(i, j, 3);
                        else if(BackTray[i][j]==HAS_NULL_IC)  //Ifor 20170807 (wei) 避免Has Null IC進入編輯頁面變成Has Ic
                            mtLoaderBuffer->SetCellColorIndex(i, j, HAS_NULL_IC);
                        else
                            mtLoaderBuffer->SetCellColorIndex(i, j, 0);
                    }
                }
            }
            else
            {
                if(iHasMap==2)  //Steven 20150915 : For TSMC 手動整盤功能
                {
                    if(BackTray[i][j]==0)
                        mtLoaderBuffer->SetCellColorIndex(i, j, 0);
                    else if(BackTray[i][j]==2)   //Steven 20150224 : Auto Contact Test
                        mtLoaderBuffer->SetCellColorIndex(i, j, BackTray[i][j]);
                }
                else
                {
                    if(BackTray[i][j]==1)
                        mtLoaderBuffer->SetCellColorIndex(i, j, 1);
                    else if(BackTray[i][j]==2)   //Steven 20150224 : Auto Contact Test
                        mtLoaderBuffer->SetCellColorIndex(i, j, 3);
                    else if(BackTray[i][j]==HAS_NULL_IC)  //Ifor 20170807 (wei) 避免Has Null IC進入編輯頁面變成Has Ic
                        mtLoaderBuffer->SetCellColorIndex(i, j, HAS_NULL_IC);
                    else
                        mtLoaderBuffer->SetCellColorIndex(i, j, 0);
                }
            }
        }
    }
}
//----------------------------------------------------------------------------
void __fastcall TTrayEditForm::SetTray()
{
    int i, j;
    Change();
    for(i=iStartX; i<=iEndX; i++)
    {
        for(j=iStartY; j<=iEndY; j++)
        {
            if(iHasMap==1)  //Steven 20150224 : Auto Contact Test
            {
                BackTray[i][j]++;
                if(BackTray[i][j]>=3)
                {
                    BackTray[i][j]=0;
                }
            }
            else if(iHasMap==2)  //Steven 20150915 : For TSMC 手動整盤功能
            {
                BackTray[i][j]=atoi(cbBinCount->Text.c_str());
            }
            else
            {
                if(BackTray[i][j]==1)
                    BackTray[i][j]=0;
                else
                    BackTray[i][j]=1;
            }

            if(iHasMap==1 || iHasMap==2)  //Steven 20150224 : Auto Contact Test
            {
                if(BackTray[i][j]==0)
                    mtLoaderBuffer->SetCellNumber(i, j, "");
                else
                    mtLoaderBuffer->SetCellNumber(i, j, BackTray[i][j]);
            }

            mtLoaderBuffer->SetCellColorIndex(i, j, BackTray[i][j]);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TTrayEditForm::Timer1Timer(TObject *Sender)
{
    char str[256];
    if(fShow==false)
        return;
    if(SystemStart==true && iSendCT!=LastSet.SendCT[0])
        FormShow(this);
    sprintf(str, "(%d,%d)...(%d,%d)", iStartX, iStartY, iEndX, iEndY);
    Caption=str;

    //ChungHung 20130715 add
    if(bEnterSave==true)
    {
        bEnterSave=false;

        //MySleep(1000);
        TrayEditForm->SaveJPG("Enter");
    }
}
//---------------------------------------------------------------------------
void __fastcall TTrayEditForm::spbUpdateClick(TObject *Sender)
{
    int i, j;

    if((CUSTOMER_CODE==CC_KYEC_LEE ||
        CUSTOMER_CODE==CC_UTAC_TW) &&                                           //Sam 20231122 : 新增矽格聯測也要鎖 FIFO
        LastSet.iRunStartMode==rsmFIFOMode)
    {
        ShowMyMessage("Not Support FIFO Mode!", "不支援FIFO Mode。");
    }
    else
    {
        for(i=0; i<MOT[iEditMotorIndex].Tray.XItem; i++)
        {
            for(j=0; j<MOT[iEditMotorIndex].Tray.YItem; j++)
            {
                if(iHasMap==1)  //Steven 20150224 : Auto Contact Test
                {
                    if(BackTray[i][j]==1)
                        MOT[iEditMotorIndex].SetTraySingleData(i, j, HAS_IC);
                    else if(BackTray[i][j]==2)
                        MOT[iEditMotorIndex].SetTraySingleData(i, j, HAS_HOT_IC);       //這個要放Shuttle 2
                    else
                        MOT[iEditMotorIndex].SetTraySingleData(i, j, NULL_IC);
                }
                else if(iHasMap==2)  //Steven 20150915 : For TSMC 手動整盤功能
                {
                    if(BackTray[i][j]==0)
                        MOT[iEditMotorIndex].SetTraySingleData(i, j, NULL_IC);
                    else
                        MOT[iEditMotorIndex].SetTraySingleData(i, j, HAS_IC, BackTray[i][j]);
                }
                else
                {
                    if(BackTray[i][j]==1)
                        MOT[iEditMotorIndex].SetTraySingleData(i, j, HAS_IC);
                 /*   else if(BAR_CODE_INSTALL==ebctInShtIntel &&
                            TestIF_File.bEnableBarCode &&
                            BackTray[i][j]==HAS_BARCODEERROR_IC)
                        MOT[iEditMotorIndex].SetTraySingleData(i, j, HAS_BARCODEERROR_IC);   */  //Steven 20160108 : Mark for 2D code
                    else if(INSTALL_OCR!=eocrUninstal && TestIF.bOcrFunction && BackTray[i][j]==HAS_OCR_OK)     //wei 20160613 ocr連線測試
                        MOT[iEditMotorIndex].SetTraySingleData(i, j, HAS_OCR_OK);
                    else if(INSTALL_OCR!=eocrUninstal && TestIF.bOcrFunction && BackTray[i][j]==HAS_OCR_Err)    //wei 20160613 ocr連線測試
                        MOT[iEditMotorIndex].SetTraySingleData(i, j, HAS_OCR_Err);
                    else if(BackTray[i][j]==HAS_NULL_IC)                        //Ifor 20170807 (wei) 避免Has Null IC進入編輯頁面變成Has Ic
                        MOT[iEditMotorIndex].SetTraySingleData(i, j, HAS_NULL_IC);
                    else
                        MOT[iEditMotorIndex].SetTraySingleData(i, j, NULL_IC);
                }
            }
        }
        if(CosFunction.bUseEditLDTrayNeedManualRemoveTray==true)                //Ifor 20200331 : add Edit Loader Tray Need Manual Remove Tray
        {
            if(iLDTrayNeedManualRemoveTray==2)                                  //0:Normal 1:Edit Loader Tray 2:Data Change
            {
                if(IniConfig.bSIGURDFunction)                                   //Sam 20230410 : 矽格 P24 & P39 都關閉時，不需在Loader手動移除Tray
                {
                    if(IniConfig.bP24SkipEventNeedRemoveEmptyAndColorTray ||
                       IniConfig.bP24SkipEventNeedRemoveColorTrayForIDT)        //Sam 20220530 : 當 Loader 發生 Skip/Edit 時，此盤做完後搬到 Empty 軌道後，會收盤起來並報警提示人員收盤(連兩盤)
                    {
                        //bEmptyAndColorMustManualRemove=true;
                        if(CosFunction.bSpecialP24)                             //Sam 20230320 : 整合 P24 北興版為獨立功能碼
                        {
                            MOT[MMTrayY].Tray.iNeedManualRemoved=1;             //to color alarm
                        }
                        else
                        {
                            if(TrayForm.LoaderToEmptyColor[iRunStartMode]==0)
                                MOT[MMTrayY].Tray.iNeedManualRemoved=0;         //to empty alarm
                            else
                                MOT[MMTrayY].Tray.iNeedManualRemoved=1;         //to color alarm
                        }
                        iManualRemoveTrayCnt=2;
                    }
                    else if(IniConfig.bP39LoaderHasSkipPlaceToEmpty)            //Sam 20230220 : 移除客戶碼
                    {
                        //bLoaderHasSkip_SG=true;
                        MOT[MMTrayY].Tray.bMustToEmpty=true;                    //Sam 20230317 : 修正 P24 功能
                    }
                }
                else
                {
                    if(Tri_Temp_Machine!=1)
                        bNeedManualRemoveTray=true;
                }
            }
        }
        Close();
    }
}
//---------------------------------------------------------------------------
void __fastcall TTrayEditForm::SpeedButton2Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TTrayEditForm::Button1Click(TObject *Sender)
{
    int iFromY, iFromX;
    if((CUSTOMER_CODE==CC_KYEC_LEE ||
        CUSTOMER_CODE==CC_UTAC_TW) &&                                           //Sam 20231122 : 新增矽格聯測也要鎖 FIFO
        LastSet.iRunStartMode==rsmFIFOMode)
    {
        ShowMyMessage("Not Support FIFO Mode!", "不支援FIFO Mode。");
    }
    else
    {
        iFromX=atoi(edtXPos->Text.c_str());
        iFromY=atoi(edtYPos->Text.c_str());
        if(iFromX<0 || iFromX>MOT[iEditMotorIndex].Tray.XItem)
        {
            ShowMyMessage("X parameter error", "");
            return;
        }
        if(iFromY<0 || iFromY>MOT[iEditMotorIndex].Tray.YItem)
        {
            ShowMyMessage("X parameter error", "");
            return;
        }

        for(int j=0; j<MOT[iEditMotorIndex].Tray.YItem; j++)
        {
            for(int i=0; i<MOT[iEditMotorIndex].Tray.XItem; i++)
            {
                if(((j+1)<iFromY) || ((j+1)==iFromY && (i+1)<iFromX))
                {
                    BackTray[i][j]=0;
                    MOT[iEditMotorIndex].SetTraySingleData(i, j, NULL_IC);
                }
                else
                {
                    BackTray[i][j]=1;
                    MOT[iEditMotorIndex].SetTraySingleData(i, j, ICType);
                }
            }
        }
        Close();
    }
}
//---------------------------------------------------------------------------
void __fastcall TTrayEditForm::edtXPosClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 100, 0);
}
//---------------------------------------------------------------------------
void __fastcall TTrayEditForm::BitBtn1Click(TObject *Sender)
{
    if(SystemStart==false)
        fMain->BtnStartClick(fMain);
    else
        fMain->BtnPauseClick(fMain);
}
//---------------------------------------------------------------------------
void __fastcall TTrayEditForm::Button2Click(TObject *Sender)
{
    SaveJPG("Enter");
}
//---------------------------------------------------------------------------
//ChungHung 20130715 add
void __fastcall TTrayEditForm::SaveJPG(AnsiString S)
{
    if(CheckBox1->Checked==true)  //Steven 20131226 : 避免異常無法跳出
        return;

    Word Year, Month, Day, Hour, Min, Sec, MSec;
    TDateTime dtPresent = Now();
    DecodeDate(dtPresent, Year, Month, Day);
    DecodeTime(dtPresent, Hour, Min, Sec, MSec);
    if(!DirectoryExists(asTrayLogPath))
    {
        if(!MyForceDirectories(asTrayLogPath))
            return;
    }
    AnsiString subDir = IntToStr(Year)+IntToStr(Month)+IntToStr(Day);
    if(!DirectoryExists(asTrayLogPath+"\\"+subDir))
    {
        if(!MyForceDirectories(asTrayLogPath+"\\"+subDir))
            return;
    }
    AnsiString filename = IntToStr(Year)+IntToStr(Month)+IntToStr(Day)+IntToStr(Hour)+IntToStr(Min)+IntToStr(Sec)+S;

    AnsiString varb=asTrayLogPath+"\\"+subDir+"\\"+filename;

    Graphics::TBitmap* pBmp = new Graphics::TBitmap();
    pBmp->Width = Screen->Width; // Screen: bcb global var: screen
    pBmp->Height = Screen->Height;
    // 取得 full screen 的 canvas
    TCanvas* pCanvas = new TCanvas();
    HDC hDc = ::GetDC(NULL); // 取得 full screen 的 dc
    pCanvas->Handle = hDc;
    // copy 到 bmp
    TRect aRect(0, 0, Screen->Width, Screen->Height); // full screen 大小
    pBmp->Canvas->CopyRect(aRect, pCanvas, aRect);

    //Graphics::TBitmap* tmp=mtLoaderBuffer->Controls-  //mtLoaderBuffer->Canvas;
    TJPEGImage* jpeg= new TJPEGImage; // 宣告一 JPEG 圖存檔用
    jpeg->Assign(pBmp); // 將所抓取的暫存圖指入
    jpeg->CompressionQuality=80; //設定 Jpeg 的壓縮品質
    jpeg->SaveToFile(varb+".jpg"); // 儲存

    // 釋放記憶體
    ::ReleaseDC(0, hDc);
    delete pBmp;        //Steven 20140305 : 沒刪除會導致記憶體不足
    delete jpeg;//刪除動態宣告之 jpeg
    delete pCanvas;
}
//---------------------------------------------------------------------------
bool TTrayEditForm::CheckCanEdit_SG(int MotorIndexIndex)                        //KaiChen 20201112 ：矽格-湖口，確認是否可以編輯 Loader/Unloader資料
{
    bool bLoaderNotEdit=false, bUnloaderNotEdit=false;
    AnsiString sPathName="";
    sPathName.sprintf("D:\\HT9045\\system");
    MyForceDirectories(sPathName, "TTrayEditForm::CheckCanEdit_SG");
    sPathName.sprintf("D:\\HT9045\\system\\Gerneral.ini");
    if(FileExists(sPathName)==false)                                            //檢查檔案
    {
        WriteIniData(sPathName, "TrayEdit",    "LoaderTrayCanNotEdit",          false);
        WriteIniData(sPathName, "TrayEdit",    "UnloaderTrayCanNotEdit",        true);
    }

    bLoaderNotEdit      =CheckAndReadIniData(sPathName, "TrayEdit",    "LoaderTrayCanNotEdit",    false);
    bUnloaderNotEdit    =CheckAndReadIniData(sPathName, "TrayEdit",    "UnloaderTrayCanNotEdit",    true);

    if(MotorIndexIndex==MMTrayY)
    {
        return !bLoaderNotEdit;
    }
    else
    {
        return !bUnloaderNotEdit;
    }
}
//---------------------------------------------------------------------------
