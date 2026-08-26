#include "MachineDefine.h"
#pragma hdrstop

#include "OCRInsp.h"
#include "OCR.h"
#include "mycylin.h"
#include "aArmHeader.h"
#include "mymotor.h"
#include "cprod.h"
#include "cmydef.h"
#include "note.h"
#include "main.h"
#include "mysensor.h"
#include "csystem.h"
#include "uhome.h"
#include "mymessbox.h"
#include "cinitial.h"
#include "acatchtray.h"
#include "adam6024.h"
#include "atester.h"
#include "uShowMessage.h"
#include "MyKitSuck.h"
#include "cSetUp.h"
#include "uLotInfo.h"
const int iInArmOCRX=7200;
const int iInArmOCRY=800;
//---------------------------------------------------------------------------
bool MoveOCRCCDToLoaderStage(int iRow, int iCol, int iOffsetX, int iOffsetY)
{
    int iXPos=0, iYPos=0;
    int iXVariable[X_PITCH_COUNT];
    int iYVariable  =0;
    int iMovePitchX =6000;
    int iMovePitchY =6000;
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iOffsetPos=InOfsLoader;
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    iXPos=Prod.XInArm_Tray_Pick[0][2]+iCol*Prod.LoadForm.iXPitch;               // 第三根吸嘴的位置
    if(iXPos>(MOT[MInArmX].Motor->PSoftLimitP-100))
    {
        if(Prod.LoadForm.iXPitch*3<=iXpitchMaxX3 &&
           Prod.LoadForm.iXPitch*3>=iXpitchMinX3)                               //Steven 20140520 : 大IC使用Fix時,吸嘴B吸Loader最右側可能會Close Pitch導致飛料
        {
            iMovePitchX=Prod.LoadForm.iXPitch*3;
        }
        else
        {
            iMovePitchX=6000;
        }
        iXPos=Prod.XInArm_Tray_Pick[0][2]+iCol*Prod.LoadForm.iXPitch;
    }
    else
    {
        iMovePitchX=iXpitchMaxX3;
    }

    iYPos=Prod.YInArm_Tray_Pick[0][2]-iRow*Prod.LoadForm.iYPitch;
    iXPos=iXPos-iInArmOCRX+iOffsetX;
    iYPos=iYPos-iInArmOCRY+iOffsetY;

    iYVariable=GetInArmPitchY_9045(iMovePitchY, iOffsetPos);
    for(int i=0; i<X_PITCH_COUNT; i++)
        iXVariable[i]=GetInArmPitchX_9045(iMovePitchX, i, iOffsetPos);

    if(IniConfig.bE30InArmUseDifferentScale)
    {
        double fi;
        fi=iXPos-Prod.XInArm_Tray_Pick[0][2];
        fi*=LastSet.fLoaderTrayXScale;
        iXPos=fi+Prod.XInArm_Tray_Pick[0][2];

        fi=iYPos-Prod.YInArm_Tray_Pick[0][2];
        fi*=LastSet.fLoaderTrayYScale;
        iYPos=fi+Prod.YInArm_Tray_Pick[0][2];
    }

    if(InArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, false))
    {
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
bool MoveTrayArmOCRCCDToLoaderStage(int iR, int iC, int iOffsetX, int iOffsetY)
{
    int iXPos, iYPos;
    if(INSTALL_OCR_YMot==eocrYMotInstal)                                        //Frank 20250214 add
    {
        iXPos=Prod.iXTrayOCR+LoadForm->XStart+iC*Prod.LoadForm.iXPitch;
        iYPos=Prod.iMLoaderYOCRPos+LoadForm->YStart+iR*Prod.LoadForm.iYPitch;
    }
    else
    {
        iXPos=Prod.iXTrayOCR+iC*Prod.LoadForm.iXPitch;
        iYPos=iOCRPosition+iR*Prod.LoadForm.iYPitch;
    }

    iXPos=iXPos+iOffsetX;
    iYPos=iYPos+iOffsetY;

    if(TrayArmContinuousMoveForOCR(iXPos, iYPos))                               //wei 20150923
    {
        //AI(ht9045-v899) 20260522: log simulated TrayArm X and LoaderY target after the combined OCR move completes
        AnsiString asSimLog;
        asSimLog.sprintf("Move done: TrayArmX=%d LoaderY=%d R=%d C=%d OffsetX=%d OffsetY=%d", iXPos, iYPos, iR+1, iC+1, iOffsetX, iOffsetY);
        OCRSimLogWrite(asSimLog.c_str());
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
bool SearchOCRLoadTrayUpDown(int &iRow, int &iCol, int data)
{
    if((IniConfig.bEnableOCRMoveSRead==false && TrayForm.Loader.Direction==0) ||
       (IniConfig.bEnableOCRMoveSRead==true  && iOCRMoveSRead==0))              //wei 20161118 OCR S型讀取
    {
        for(int iR=0; iR<MOT[MMOCR].Tray.YItem; iR++)
        {
            for(int iC=0; iC<MOT[MMOCR].Tray.XItem; iC++)
            {
                if(MOT[MMOCR].Tray.Data[iC][iR]==data)
                {
                    iCol=iC;
                    iRow=iR;
                    return true;
                }
            }
        }
    }
    else if((IniConfig.bEnableOCRMoveSRead==false && TrayForm.Loader.Direction==1) ||
            (IniConfig.bEnableOCRMoveSRead==true  && iOCRMoveSRead==1))         //wei 20161118 OCR S型讀取
    {
        for(int iR=0; iR<MOT[MMOCR].Tray.YItem; iR++)
        {
            for(int iC=(MOT[MMOCR].Tray.XItem-1); iC>=0; iC--)
            {
                if(MOT[MMOCR].Tray.Data[iC][iR]==data)
                {
                    iCol=iC;
                    iRow=iR;
                    return true;
                }
            }
        }
    }
    else if(TrayForm.Loader.Direction==2)
    {
        for(int iR=(MOT[MMOCR].Tray.YItem-1); iR>=0; iR--)
        {
            for(int iC=0; iC<MOT[MMOCR].Tray.XItem; iC++)
            {
                if(MOT[MMOCR].Tray.Data[iC][iR]==data)
                {
                    iCol=iC;
                    iRow=iR;
                    return true;
                }
            }
        }
    }
    else if(TrayForm.Loader.Direction==3)
    {
        for(int iR=(MOT[MMOCR].Tray.YItem-1); iR>=0; iR--)
        {
            for(int iC=(MOT[MMOCR].Tray.XItem-1); iC>=0; iC--)
            {
                if(MOT[MMOCR].Tray.Data[iC][iR]==data)
                {
                    iCol=iC;
                    iRow=iR;
                    return true;
                }
            }
        }
    }
    else if(TrayForm.Loader.Direction==4)
    {
        for(int iC=0; iC<MOT[MMOCR].Tray.XItem; iC++)
        {
            for(int iR=0; iR<MOT[MMOCR].Tray.YItem; iR++)
            {
                if(MOT[MMOCR].Tray.Data[iC][iR]==data)
                {
                    iCol=iC;
                    iRow=iR;
                    return true;
                }
            }
        }
    }
    else if(TrayForm.Loader.Direction==5)
    {
        for(int iC=0; iC<MOT[MMOCR].Tray.XItem; iC++)
        {
            for(int iR=(MOT[MMOCR].Tray.YItem-1); iR>=0; iR--)
            {
                if(MOT[MMOCR].Tray.Data[iC][iR]==data)
                {
                    iCol=iC;
                    iRow=iR;
                    return true;
                }
            }
        }
    }
    else if(TrayForm.Loader.Direction==6)
    {
        for(int iC=(MOT[MMOCR].Tray.XItem-1); iC>=0; iC--)
        {
            for(int iR=0; iR<MOT[MMOCR].Tray.YItem; iR++)
            {
                if(MOT[MMOCR].Tray.Data[iC][iR]==data)
                {
                    iCol=iC;
                    iRow=iR;
                    return true;
                }
            }
        }
    }
    else
    {
        for(int iC=(MOT[MMOCR].Tray.XItem-1); iC>=0; iC--)
        {
            for(int iR=(MOT[MMOCR].Tray.YItem-1); iR>=0; iR--)
            {
                if(MOT[MMOCR].Tray.Data[iC][iR]==data)
                {
                    iCol=iC;
                    iRow=iR;
                    return true;
                }
            }
        }
    }

    return false;
}
//------------------------------------------------------------------------------
bool SearchCheckMap(int &iRow, int &iCol, int iEnterCheckMap)                   //Frank 20161025 確認四個角落的OCR Code
{
    if(iEnterCheckMap==0)
    {
        for(int iR=0; iR<MOT[MMOCR].Tray.YItem; iR++)
        {
            for(int iC=0; iC<MOT[MMOCR].Tray.XItem; iC++)
            {
                if(MOT[MMOCR].Tray.Data[iC][iR]==HAS_OCR_OK)
                {
                    iCol=iC;
                    iRow=iR;
                    return true;
                }
            }
        }
    }
    else if(iEnterCheckMap==1)
    {
        for(int iR=0; iR<MOT[MMOCR].Tray.YItem; iR++)
        {
            for(int iC=(MOT[MMOCR].Tray.XItem-1); iC>=0; iC--)
            {
                if(MOT[MMOCR].Tray.Data[iC][iR]==HAS_OCR_OK)
                {
                    iCol=iC;
                    iRow=iR;
                    return true;
                }
            }
        }
    }
    else if(iEnterCheckMap==2)
    {
        for(int iR=(MOT[MMOCR].Tray.YItem-1); iR>=0; iR--)
        {
            for(int iC=(MOT[MMOCR].Tray.XItem-1); iC>=0; iC--)
            {
                if(MOT[MMOCR].Tray.Data[iC][iR]==HAS_OCR_OK)
                {
                    iCol=iC;
                    iRow=iR;
                    return true;
                }
            }
        }
    }
    else if(iEnterCheckMap==3)
    {
        for(int iR=(MOT[MMOCR].Tray.YItem-1); iR>=0; iR--)
        {
            for(int iC=0; iC<MOT[MMOCR].Tray.XItem; iC++)
            {
                if(MOT[MMOCR].Tray.Data[iC][iR]==HAS_OCR_OK)
                {
                    iCol=iC;
                    iRow=iR;
                    return true;
                }
            }
        }
    }
    return false;
}
//---------------------------------------------------------------------------
bool MoveInArmZ_To_Pick_HotPlate_9045_OCR(int iSelRow, int Num, int iCT)
{
    bool flag[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false}, {false, false, false, false}};

    flag[iSelRow][Num]=MOT[InArmZIndex[iSelRow][Num]].MotorMove(Prod.ZInArm_Tray_Pick[iSelRow][Num]+iCT*ArmSpeed[InArm].dRetryDown);

    if(flag[iSelRow][Num]==false)
        return false;

    return true;
}
//---------------------------------------------------------------------------
void OCRNGSetToHASIC()
{
    if(TrayForm.Loader.Direction==0)
    {
        for(int iR=0; iR<MOT[MMOCR].Tray.YItem; iR++)
        {
            for(int iC=0; iC<MOT[MMOCR].Tray.XItem; iC++)
            {
                if(MOT[MMOCR].Tray.Data[iC][iR]!=HAS_IC &&
                   MOT[MMOCR].Tray.Data[iC][iR]!=NULL_IC)
                {
                    MOT[MMOCR].SetTraySingleData(iC, iR, HAS_IC);
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
int iOCRFlow=1;
int iOCRPosC=0;
int iOCRPosR=0;
bool bOCRWaitTrainMsg=false;
int iOCRRetryCT=0;
const int iOCRTimeOut=100;
int iCheckOCRInArmSuckTask=1;
bool CheckOCRInArmSuckIC(int iSelRow, int Num)
{
    int &Task=iCheckOCRInArmSuckTask;

    switch(Task)
    {
        case 1:
            InArmSuck.ResetAll();                                               //Steven 20160323 : 避免未開啟真空
        case 50:
            if(InArmSuck.Suck[iSelRow][Num].Suck())
            {
                Task=1;
                return true;
            }
            else if(InArmSuck.Suck[iSelRow][Num].Error==true)
            {
                Task=1;
                return true;
            }
    }
    return false;
}
//---------------------------------------------------------------------------
void InitOCRFlow(bool bTrain)
{
    bOCRWaitTrainMsg=bTrain;
    iCheckOCRInArmSuckTask=1;
    iOCRRetryCT=0;
    iOCRFlow=1;
    iOCRPosC=0;
    iOCRPosR=0;
    iNewOCRCount[iOCRSearchCount]=0;
}
//---------------------------------------------------------------------------
TQPF_Timer hOCRTimeOut;
TQPF_Timer hOCRDealyTime;
int DoOCRFlow()                                                                 // 0:未完成 1:檢測完成(OK) 2:檢測結果(NG) 3:Training NG 4:No Device
{
    int &Task=iOCRFlow;
    int ret;
    int iOCRMaxCount=0;
    iOCRMaxCount=IniConfig.iOCRMaxInspDevices;
    static int iResult=0;
    switch(Task)
    {
        case 1:                                                                 //搜尋HAS_IC 並移至目標位置
            iResult=0;
            if(iOCRCount>=iOCRMaxCount)
            {
                Task=200;
                iResult=2;
            }
            else if(SearchOCRLoadTrayUpDown(iOCRPosR, iOCRPosC, HAS_IC))
            {
                if(MoveOCRCCDToLoaderStage(iOCRPosR, iOCRPosC, InArmOffSet[6]->GetX(), InArmOffSet[6]->GetY()))
                {
                    //送出LOTID to OCR
                    if(bGetLotIDFormTester)
                        fOCR->sOCR_Send[fOCR->ocrLot]        = "#LOT"+fOCR->sTesterLotId+"+";
                    else
                        fOCR->sOCR_Send[fOCR->ocrLot]        = "#LOT"+fSetup->edOcrText->Text+"+";

                    fOCR->bOcr_ReceiveOK[fOCR->ocrLot]=false;
                    fOCR->SendOCR(fOCR->ocrLot);
                    hOCRTimeOut.SetMSAndOn(iOCRTimeOut*100);
                    Task=25;
                }
            }
            else
            {
                Task=200;
                iResult=4;
            }
            break;
        case 25:
            if(fOCR->bOcr_ReceiveOK[fOCR->ocrLot]==false)
            {
                fOCR->SendOCR(fOCR->ocrRecipeOk);                               //檢查檢測檔
                fOCR->bOcr_ReceiveOK[fOCR->ocrRecipeNoModel]=false;
                fOCR->bOcr_ReceiveOK[fOCR->ocrRecipeNoROI]=false;
                fOCR->bOcr_ReceiveOK[fOCR->ocrRecipeNoFont]=false;
                fOCR->bOcr_ReceiveOK[fOCR->ocrRecipeNoLotID]=false;

                hOCRTimeOut.SetMSAndOn(iOCRTimeOut*100);
                Task=50;
            }

            if(hOCRTimeOut.Off())
            {
                ret=ShowErrorMessage("WAR0945", K_RETRY, MMTrayY);              //"OCR LOTID time out!!"
                if(ret==K_RETRY)
                {
                    Task=1;
                }
            }
            break;
        case 50:                                                                //取得檢測檔回應
            if(fOCR->bOcr_ReceiveOK[fOCR->ocrRecipeOk]==true)
            {
                if(bOCRWaitTrainMsg)
                {
                    fOCR->SendOCR(fOCR->ocrTrainOK);                            //送出TRAIN訊息
                    fOCR->bOcr_ReceiveOK[fOCR->ocrTrainNG]=false;
                    hOCRTimeOut.SetMSAndOn(iOCRTimeOut*100);
                }
                Task=80;
            }
            else if(fOCR->bOcr_ReceiveOK[fOCR->ocrRecipeNoModel] ||
                    fOCR->bOcr_ReceiveOK[fOCR->ocrRecipeNoROI]   ||
                    fOCR->bOcr_ReceiveOK[fOCR->ocrRecipeNoFont]  ||
                    fOCR->bOcr_ReceiveOK[fOCR->ocrRecipeNoLotID])               //檢測檔有問題
            {
                fOCR->SendOCR(fOCR->ocrEnd);                                    //送出檢測訊號
                if(fOCR->bOcr_ReceiveOK[fOCR->ocrRecipeNoModel])
                {
                    ShowErrorMessage("WAR0933", 0, MMTrayY);                    //"OCR no Model error!!"
                }
                else if(fOCR->bOcr_ReceiveOK[fOCR->ocrRecipeNoROI])
                {
                    ShowErrorMessage("WAR0932", 0, MMTrayY);                    //"OCR no ROI error!!"
                }
                else if(fOCR->bOcr_ReceiveOK[fOCR->ocrRecipeNoFont])
                {
                    ShowErrorMessage("WAR0931", 0, MMTrayY);                    //"OCR no NoFont error!!"
                }
                else if(fOCR->bOcr_ReceiveOK[fOCR->ocrRecipeNoLotID])
                {
                    ShowErrorMessage("WAR0956", 0, MMTrayY);                    //"OCR no No LOTID error!!"
                }

                fOCR->DoOCRReleaseAndInspEnd();
                Task=1;
            }
            else
            {
                if(hOCRTimeOut.Off())
                {
                    ret=ShowErrorMessage("WAR0946", K_RETRY, MMTrayY);          //"OCR Recipe time out!!"
                    if(ret==K_RETRY)
                    {
                        Task=1;
                    }
                }
            }
            break;
        case 80:
            if(bOCRWaitTrainMsg)
            {
                if(fOCR->bOcr_ReceiveOK[fOCR->ocrTrainOK])
                {
                    bRunOcrInsp=false;
                    return 1;
                }
                else if(fOCR->bOcr_ReceiveOK[fOCR->ocrTrainNG])
                {
                    Task=200;
                    iResult=3;
                }
                else
                {
                    if(hOCRTimeOut.Off())
                    {
                        ret=ShowErrorMessage("WAR0947", K_RETRY, MMTrayY);      //"OCR LOTID time out!!"
                        if(ret==K_RETRY)
                        {
                            Task=1;
                        }
                    }
                }
            }
            else
            {
                fOCR->InitInsp();
                fOCR->SendOCR(fOCR->ocrInspOk);
                hOCRTimeOut.SetMSAndOn(iOCRTimeOut*100);
                Task=100;
            }
            break;
        case 100:                                                               //取得OCR檢測結果
            if(fOCR->bOcr_ReceiveOK[fOCR->ocrInspOk])                           //回傳OK Task=1 return 1;
            {
                Task=1;
                bRunOcrInsp=false;
                return 1;
            }
            else if(fOCR->bOcr_ReceiveOK[fOCR->ocrInspNg])                      //回傳NG
            {
                MOT[MMOCR].SetTraySingleData(iOCRPosR, iOCRPosC, HAS_OCR_NG);
                iOCRCount++;
                Task=1;
            }
            else if(fOCR->bOcr_ReceiveOK[fOCR->ocrInspErr2001] ||
                    fOCR->bOcr_ReceiveOK[fOCR->ocrInspErr2002] ||
                    fOCR->bOcr_ReceiveOK[fOCR->ocrInspErr2003] ||
                    fOCR->bOcr_ReceiveOK[fOCR->ocrInspErr3001] ||
                    fOCR->bOcr_ReceiveOK[fOCR->ocrInspErr3002] )                //回傳ERR 不包含 @INSPERR2004+
            {
                MOT[MMOCR].SetTraySingleData(iOCRPosR, iOCRPosC, HAS_OCR_NG);
                iOCRCount++;
                Task=1;
            }
            else if(fOCR->bOcr_ReceiveOK[fOCR->ocrInspErr2004])                 //回傳ERR @INSPERR2004+ Task=1000; iRetryCT=0;
            {
                MOT[MMOCR].SetTraySingleData(iOCRPosR, iOCRPosC, HAS_OCR_NG);
                iOCRCount++;
                Task=1;
            }
            else
            {
                if(hOCRTimeOut.Off())
                {
                    //ShowMessage time out error
                    ret=ShowErrorMessage("WAR0944", K_RETRY, MMTrayY);          //"OCR inspection time out!!"
                    if(ret==K_RETRY)
                    {
                        Task=1;
                    }
                }
            }
            break;
        case 200:
            if(MoveInArm2XYToShuttle2Wait())
            {
                Task=1;
                bRunOcrInsp=false;
                return iResult;
            }
            break;
    }

    return 0;
}
//------------------------------------------------------------------------------
int CheckDataExistAndDuplicate()                                                //KenHsieh 20220825 : 新增OCR比對功能
{
    int iIndex, iPos;
    AnsiString Str="", asBarCodebyFile="", asFileName="", asPath="";

    iPos=asBarCode.Pos("/r/n");
    if(iPos>0)
        Str=asBarCode.SubString(1, iPos-1);
    else
        Str=asBarCode;

    MyForceDirectories(asOCRDownLoadLotPath);

    if(fLotInfo->bCheckOnlyOneFile(asOCRDownLoadLotPath, asFileName)==false)
        return 1;                                                               //檔案數量錯誤

    if(IniConfig.bN33_1_NetChangeFileAndData)
    {
        TStringList *ProductionInfo=new TStringList;
        ProductionInfo->Clear();
        ProductionInfo->LoadFromFile("D:\\HT9045_Log\\Production_Info\\TestInfo.txt");
        asBarCodebyFile=ProductionInfo->Strings[20];
        asBarCodebyFile=asBarCodebyFile.SubString(asBarCodebyFile.Pos(":")+1, asBarCodebyFile.Length()-asBarCodebyFile.Pos(":"));
        asFileName.Delete(asFileName.Pos(".txt"), 4);

        ProductionInfo->Clear();
        delete ProductionInfo;
        if(asBarCodebyFile!=asFileName)
            return 2;                                                           //檔名錯誤
        asFileName+=".txt";
    }

    asPath=asOCRDownLoadLotPath+"\\"+asFileName;
    if(FileExists(asPath))
    {
        fOCR->ListOCRByLot->Clear();
        fOCR->ListOCRByLot->LoadFromFile(asPath);
        fOCR->ListOCRByLot->Sort();
        if(fOCR->ListOCRByLot->Find(Str, iIndex)==false)
        {
            return 3;                                                           //比對無資料
        }
        else
        {
            fOCR->ListOCRByLot->Delete(iIndex);
            fOCR->ListOCRByLot->SaveToFile(asPath);
            return 0;
        }
    }
    else
    {
        return 4;                                                               //副檔名錯誤或檔案不存在
    }
}
//------------------------------------------------------------------------------
TQPF_Timer OCRMovePositionDelay;
int DoOCRFlow1()                                                                // 0:未完成 1:檢測完成(OK) 2:檢測結果(NG) 3:Training NG 4:No Device
{
    int &Task=iOCRFlow;
    int ret;
    int iOCRMaxCount=0;
    static int iRetryCount=0;
    static int iOCRPosition=0, iLoaderCount=0;
    AnsiString Buffer, Buffer1, Buffer2;
    AnsiString asOCRLog="";

    iOCRMaxCount=MOT[MMTrayY].Tray.XItem*MOT[MMTrayY].Tray.YItem;               //wei 20161118 強制使用Loader Tray資料

    static int iResult=0;
    static bool bEnterCheckMapflag=false;
    static int iEnterCheckMap=0;
    static int iTrayRow=0, iTrayCol=0;
    int iret, iret1;                                                            //KenHsieh 20220825 : 新增OCR比對功能

    switch(Task)
    {
        case 1:                                                                 //搜尋HAS_IC 並移至目標位置
            MOT[MLoaderY].SetSpeed(10);
            #ifdef SOFT_SIMULTE
            bOCRConnect=true;
            #endif
            if(bOCRSimBypassAOI)                                                //AI(ht9045-v899) 20260511: simulation bypass - treat AOI as connected so flow can proceed without vision PC
                bOCRConnect=true;
            if(bOCRConnect==false)
            {
                ShowErrorMessage("WAR09113", K_RETRY , MMOCR);                      //Frank 20250214 add
                fOCR->DoConnect();
            }
            else if(iNewOCRCount[iOCRSearchCount]>=iOCRMaxCount || MOT[MMOCR].Tray.HasOCRIC()==false)        //wei 20160606
            {
                Task=200;
                iResult=1;
            }
            else if(bOCRCheck && iOCRCheck>=LastSet.TrayCount[iOCRLotTrayCount-1])
            {
                for(int i=0; i<LastSet.TrayCount[iOCRLotTrayCount-1]; i++)
                {
                    if(asOCRCheck[i]!=fLotInfo->edOCRLotID->Text)
                    {
                        iResult=5;
                        Task=200;
                        break;
                    }
                }
                bOCRCheck=false;
                iOCRCheck=0;
                if(iOCRLotTrayCount>=10)
                    iOCRLotTrayCount=0;
            }
            else if(IniConfig.bEnabledOCRCheckIC==false && iNewOCRCount[iOCRSkipContinue]>=IniConfig.iOCRSkip)    //wei 20161228 確認Tray是否有IC
            {
                Task=200;
                iResult=1;
            }
            else if(IsMoveInArm2XYToWait()==false)
            {
                Task=19;
                break;
            }
            else if(SearchOCRLoadTrayUpDown(iOCRPosR, iOCRPosC, HAS_IC))
            {
                if(INSTALL_OCR_YMot!=eocrYMotInstal)                                //Frank 20250214 add
                {
                    if(bOCRStartPosition==false)
                    {
                        CleanOCRData();
                        iLoaderCount=0;
                        iOCRMoveSRead=0;
                        #ifdef SOFT_SIMULTE
                        Task=20;
                        #else
                        Task=195;
                        #endif
                        break;
                    }
                }
                else
                {
                    if(bOCRStartPosition==false)
                    {
                        bOCRStartPosition=true;
                        CleanOCRData();
                        iLoaderCount=0;
                        iOCRMoveSRead=0;
                    }
                }

                if(bOCRCheckNG)
                {
                    CleanOCRData();
                    bOCRCheckNG=false;
                }

                if(MTrayXCanSafeMove()==false)
                {
                    Task=196;
                    break;
                }

                if(MoveTrayArmOCRCCDToLoaderStage(iOCRPosR, iOCRPosC, InArmOffSet[6]->GetX(), InArmOffSet[6]->GetY()))  //(int ix,int iy,int iOffsetX,int iOffsetY)
                {
                    fOCR->DeleteOCRImage();
                    Task=30;
                }
            }
            else if(iNewOCRCount[iOCRSearchCount]>=iOCRMaxCount)
            {
                Task=300;
            }
            else
            {
                Task=200;
                iResult=4;
            }
            break;
        case 19:
            if(MoveInArm2XYToWait())
            {
                //AI(ht9045-v899) 20260522: mark InArm wait-position completion in simulation log without changing motion flow
                OCRSimLogWrite("InArm XY wait done");
                Task=1;
            }
            break;
        case 195:
            if(Sen[SnOCRPosition].IsOn())
            {
                ShowMyMessage("OCR Position Sensor is ON,Please Check Sensor!");
            }
            else if(Sen[SnLoaderSureTray].IsOn())
            {
                ShowMyMessage("Please remove the loader tray manually!");
            }
            else
            {
                Task=20;
                OCRMovePositionDelay.SetSecAndOn(30);
            }
            break;
        case 196:
            if(Cylinder[C_TrayX_UpDown].Pop())
            {
                //AI(ht9045-v899) 20260522: log TrayX safety-cylinder recovery during OCR simulation only
                OCRSimLogWrite("TrayX up/down cylinder pop done");
                Task=1;
            }
            break;
        case 20:
            if(iLoaderCount==0)
                MOT[MLoaderY].SetSpeed(40);
            else
                MOT[MLoaderY].SetSpeed(10);
            MOT[MLoaderY].MotorMove(99999);
            #ifdef SOFT_SIMULTE
                if(IniConfig.OCRLightChange)    //wei 20181225 光源auto change
                {
                    fOCR->ChangeLightValue(1, IniConfig.iBlueLight);
                    fOCR->ChangeLightValue(2, IniConfig.iRedLight);
                }
                Task=1;
                bOCRStartPosition=true;
            #else
            if(Sen[SnOCRPosition].IsOn())
            {
                MOT[MLoaderY].PCIL132_StopMotor();
                if(iLoaderCount<1)
                {
                    Task=21;
                    iLoaderCount++;
                }
                else
                {
                    if(IniConfig.OCRLightChange)    //wei 20181225 光源auto change
                    {
                        fOCR->ChangeLightValue(1, IniConfig.iBlueLight);
                        fOCR->ChangeLightValue(2, IniConfig.iRedLight);
                    }
                    hOCRTimeOut.SetMSAndOn(100);
                    Task=25;
                }
            }
            else if(OCRMovePositionDelay.Off())
            {
                ShowMyMessage("Tray No Arrive Position Sensor,Please Check Tray!!");
                OCRMovePositionDelay.SetSecAndOn(30);
            }
            #endif
            break;
        case 21:
            MOT[MLoaderY].SetSpeed(10);
            MOT[MLoaderY].MotorMove(-99999);
            if(Sen[SnOCRPosition].IsOff())
            {
                MOT[MLoaderY].PCIL132_StopMotor();
                iOCRPosition=MOT[MLoaderY].ReadPos();
                Task=22;
            }
            break;
        case 22:
            if(MOT[MLoaderY].MotorMove(iOCRPosition-1500))
            {
                iOCRPosition=0;
                MOT[MLoaderY].PCIL132_StopMotor();
                OCRMovePositionDelay.SetSecAndOn(30);
                Task=20;
            }
            break;
        case 25:
            if(hOCRTimeOut.Off())
            {
                bOCRStartPosition=true;
                if(bEnterCheckMapflag)
                {
                    if(INSTALL_OCR_YMot==eocrYMotUninstal)                      //Frank 20250214 add
                        MOT[MLoaderY].PCIL132_SetPos(0);
                    Task=400;
                }
                else if(IniConfig.bEnableStartposshift) //wei 20181225 初始點位
                {
                    iOCRPosition=MOT[MLoaderY].ReadPos();
                    Task=26;
                }
                else
                {
                    iOCRPosition=0;
                    MOT[MLoaderY].PCIL132_StopMotor();                          //JerryYang 20241118 : fix
                    Task=251;
                }
            }
            break;
        case 251:
            if(MOT[MLoaderY].Motor->MotionDone()==true)                         //JerryYang 20241118 : fix
            {
                if(INSTALL_OCR_YMot==eocrYMotUninstal)                          //Frank 20250214 add
                    MOT[MLoaderY].PCIL132_SetPos(0);
                Task=1;
            }
            break;
        case 26:
            if(MOT[MLoaderY].MotorMove(iOCRPosition+IniConfig.iStartposshift))
            {
                iOCRPosition=0;
                if(INSTALL_OCR_YMot==eocrYMotUninstal)                          //Frank 20250214 add
                    MOT[MLoaderY].PCIL132_SetPos(0);
                MOT[MLoaderY].PCIL132_StopMotor();
                Task=1;
            }
            break;
        case 30:
            if(IniConfig.OCRLightNoDown)                                        //wei 20181225 光源不下降
            {
                //AI(ht9045-v899) 20260526: keep OCR light cylinder output off when no-down mode is selected
                Cylinder[C_OCRLight_Up].Off();
                OCRSimLogWrite("OCR light no-down: cylinder OFF command");
                hOCRDealyTime.SetSecAndOn(0.2);
                if(IniConfig.OCRLightChange)                                    //wei 20181225 光源auto change
                {
                    fOCR->ChangeLightValue(1, IniConfig.iBlueLight);
                    fOCR->ChangeLightValue(2, IniConfig.iRedLight);
                }
                Task=31;
            }
            else
            {
                Cylinder[C_OCRLight_Up].On();
                //AI(ht9045-v899) 20260522: log OCR light down command for simulation flow validation
                OCRSimLogWrite("OCR light cylinder ON command");
                if(IniConfig.OCRLightChange)                                    //wei 20181225 光源auto change
                {
                    fOCR->ChangeLightValue(1, IniConfig.iBlueLight);
                    fOCR->ChangeLightValue(2, IniConfig.iRedLight);
                }
                hOCRDealyTime.SetSecAndOn(1);
                Task=301;
            }
            break;
        case 301:
            if(Cylinder[C_OCRLight_Up].OffSensor()==true)                       //wei 20150720
            {
                //AI(ht9045-v899) 20260522: log OCR light ready sensor before trigger starts
                OCRSimLogWrite("OCR light cylinder down sensor ready");
                hOCRDealyTime.SetSecAndOn(0.2);
                Task=31;
            }
            else if(hOCRDealyTime.Off())
            {
                ShowMyMessage("OCR Light Up off sensor no ON!!");
                Cylinder[C_OCRLight_Up].On();
                hOCRDealyTime.SetSecAndOn(1);
            }
            break;
        case 31:
            if(hOCRDealyTime.Off())
            {
                if(bOCRConnect==true || bOCRSimBypassAOI)                       //AI(ht9045-v899) 20260511: bypass-AOI also enters trigger sequence so handler motion path matches production
                {
                    bOCRReadOK=false;
                    //AI(ht9045-v899) 20260522: route OCR capture through selected switch/SE8 trigger mode
                    if(fOCR->StartOCRCaptureTrigger(bOCRSimBypassAOI)==false)
                    {
                        if(bOCRSimLogEnable)                                    //AI(ht9045-v899) 20260526: stop OCR simulation after trigger failure alarm so vision setup can be adjusted
                        {
                            asOCRLog.sprintf("Simulation alarm stop: capture trigger failed R=%d C=%d", iOCRPosR+1, iOCRPosC+1);
                            OCRSimLogWrite(asOCRLog.c_str());
                            ShowErrorMessage("WAR09113", K_RETRY , MMOCR);
                            fOCR->DoConnect();
                            return OCR_SIM_RET_TRIGGER_FAIL;
                        }
                        ShowErrorMessage("WAR09113", K_RETRY , MMOCR);
                        fOCR->DoConnect();
                        break;
                    }

                    if(fOCR->Memo2->Lines->Count>1024)
                    {
                        fOCR->Button4->Click();
                        fOCR->Memo2->Clear();
                    }
                    asOCRLog.sprintf("%s Start,OCR Tigger ON,%02d_%02d Count %d", Now().FormatString("yyyymmddhhmmss"), iOCRPosR, iOCRPosC, iRetryCount);
                    fOCR->Memo2->Lines->Add(asOCRLog.c_str());
                    //AI(ht9045-v899) 20260522: mirror OCR trigger-on memo event to simulation log
                    OCRSimLogWrite(asOCRLog.c_str());
                    hOCRDealyTime.SetSecAndOn(0.1);
                    Task=32;
                }
                else
                {
                    if(bOCRSimLogEnable)                                        //AI(ht9045-v899) 20260526: stop OCR simulation when real AOI is not connected during validation
                    {
                        asOCRLog.sprintf("Simulation alarm stop: OCR connect failed R=%d C=%d", iOCRPosR+1, iOCRPosC+1);
                        OCRSimLogWrite(asOCRLog.c_str());
                        ShowErrorMessage("WAR09113", K_RETRY , MMOCR);              //Frank 20250214 add
                        fOCR->DoConnect();
                        return OCR_SIM_RET_TRIGGER_FAIL;
                    }
                    ShowErrorMessage("WAR09113", K_RETRY , MMOCR);                  //Frank 20250214 add
                    fOCR->DoConnect();
                }
            }
            break;
        case 32:
            if(hOCRDealyTime.Off())
            {
                fOCR->StopOCRCaptureTrigger();                                  //AI(ht9045-v899) 20260522: switch mode turns off switch; command mode leaves switch untouched
                if(fOCR->IsOCRCommandTrigger())                                 //AI(ht9045-v899) 20260522: keep logs accurate because SE8 mode has no switch-off action
                    asOCRLog.sprintf("%s Dealy Time off,OCR Command Tigger SE8", Now().FormatString("yyyymmddhhmmss"));
                else
                    asOCRLog.sprintf("%s Dealy Time off,OCR Tigger OFF", Now().FormatString("yyyymmddhhmmss"));
                fOCR->Memo2->Lines->Add(asOCRLog.c_str());
                //AI(ht9045-v899) 20260522: mirror OCR trigger-off memo event to simulation log
                OCRSimLogWrite(asOCRLog.c_str());
                hOCRDealyTime.SetSecAndOn(2);
                bOCRNoIC=false;

                if(bOCRSimBypassAOI)                                           //AI(ht9045-v899) 20260522: bypass AOI result must override SOFT_SIMULTE NoIC so OCR Simulation can validate normal OK flow without vision PC
                {
                    bOCRReadOK=true;
                    Buffer.sprintf("T%02dAA%02d", iOCRPosR, iOCRPosC);
                    asBarCode=Buffer.c_str();
                    //AI(ht9045-v899) 20260522: log fabricated bypass-AOI barcode so simulation evidence shows non-socket result source
                    asOCRLog.sprintf("Bypass AOI result: %s", asBarCode.c_str());
                    OCRSimLogWrite(asOCRLog.c_str());
                }
                #ifdef SOFT_SIMULTE
                else                                                            //AI(ht9045-v899) 20260522: keep legacy SOFT_SIMULTE result only when Bypass AOI is not selected
                {
                    if(fMain->cb1->Checked)
                    {
                        bOCRReadOK=true ;                                       //wei 20150713 模擬
                        Buffer.sprintf("T%02dAA%02d", iOCRPosR, iOCRPosC);
                        asBarCode=Buffer.c_str();
                    }
                    else
                    {
                        bOCRReadOK=true ;                                       //wei 20150713 模擬
                        Buffer.sprintf("NoIC", iOCRPosR, iOCRPosC);
                        asBarCode=Buffer.c_str();
                    }
                }
                #endif
                Task=33;
            }
            break;
        case 33:
            if(bOCRReadOK)
            {
                if(IniConfig.bEnabledOCRCheckIC)                                //wei 20161228 確認Tray是否有IC
                {
                    if(asBarCode=="NoIC")
                    {
                        Buffer1.sprintf("%02d,%02d,%s", iOCRPosR+1, iOCRPosC+1, asBarCode);
                        Buffer2.sprintf("%s_%02d,%02d,%s", Now().FormatString("yyyymmddhhmmss"), iOCRPosR+1, iOCRPosC+1, asBarCode);  //Alick 20170104 (Steven) add 多一組LOG紀錄+上時間
                        fLotInfo->Memo2->Lines->Add(Buffer1.c_str());
                        fLotInfo->Memo3->Lines->Add(Buffer2.c_str());           //Alick 20170104 (Steven) add 多一組LOG紀錄+上時間

                        MOT[MMOCR].SetTraySingleData(iOCRPosC, iOCRPosR, HAS_OCR_Err);
                        iNewOCRCount[iOCRSkipCount]++;
                        iNewOCRCount[iOCRSearchCount]++;
                        //AI(ht9045-v899) 20260522: record NoIC write-back path in OCR simulation log
                        asOCRLog.sprintf("Result NoIC: R=%d C=%d set HAS_OCR_Err", iOCRPosR+1, iOCRPosC+1);
                        OCRSimLogWrite(asOCRLog.c_str());
                        bOCRReadOK=false;
                        Task=40;
                    }
                    else if(asBarCode.Pos("T")==1 || asBarCode.Pos("F")==1 || asBarCode.Pos("?")!=0)
                    {
                        asBarCode.Delete(1,1);
                        Buffer1.sprintf("%02d,%02d,%s", iOCRPosR+1, iOCRPosC+1, asBarCode);
                        Buffer2.sprintf("%s_%02d,%02d,%s", Now().FormatString("yyyymmddhhmmss"), iOCRPosR+1, iOCRPosC+1, asBarCode);  //Alick 20170104 (Steven) add 多一組LOG紀錄+上時間
                        fLotInfo->Memo2->Lines->Add(Buffer1.c_str());
                        fLotInfo->Memo3->Lines->Add(Buffer2.c_str());           //Alick 20170104 (Steven) add 多一組LOG紀錄+上時間

                        MOT[MMOCR].SetTraySingleData(iOCRPosC, iOCRPosR, HAS_OCR_OK);
                        iNewOCRCount[iOCRPassCount]++;
                        iNewOCRCount[iOCRSearchCount]++;
                        //AI(ht9045-v899) 20260522: record CheckIC OK write-back path in OCR simulation log
                        asOCRLog.sprintf("Result OK(CheckIC): R=%d C=%d Barcode=%s", iOCRPosR+1, iOCRPosC+1, asBarCode.c_str());
                        OCRSimLogWrite(asOCRLog.c_str());
                        bOCRReadOK=false;
                        Task=40;
                    }
                }
                else
                {
                    if(asBarCode=="NoIC")
                    {
                        bOCRNoIC=true;
                        //AI(ht9045-v899) 20260522: log NoIC retry entry for OCR simulation validation
                        asOCRLog.sprintf("Result NoIC: R=%d C=%d enter retry", iOCRPosR+1, iOCRPosC+1);
                        OCRSimLogWrite(asOCRLog.c_str());
                        Task=39;
                    }
                    else if(asBarCode.Pos("F")==1)
                    {
                        bInspectError=true;
                        //AI(ht9045-v899) 20260522: log AOI inspection error result before retry/error handling
                        asOCRLog.sprintf("Result InspectError: R=%d C=%d Barcode=%s", iOCRPosR+1, iOCRPosC+1, asBarCode.c_str());
                        OCRSimLogWrite(asOCRLog.c_str());
                        Task=39;
                    }
                    else if(asBarCode.Pos("?")!=0)
                    {
                        //AI(ht9045-v899) 20260522: log unknown OCR result before retry/error handling
                        asOCRLog.sprintf("Result Unknown: R=%d C=%d Barcode=%s", iOCRPosR+1, iOCRPosC+1, asBarCode.c_str());
                        OCRSimLogWrite(asOCRLog.c_str());
                        Task=39;
                    }
                    else if(asBarCode.Pos("T")==1)
                    {
                        asBarCode.Delete(1,1);
                        if(fOCR->CheckOCRWordType(asBarCode)==false)            //wei 20161128 (Steven) 確認字串各自Type是否正確
                        {
                            ShowErrorMessage("WAR09100", K_RETRY , MMOCR);
                            Task=39;
                            break;
                        }

                        if(asBarCode.Length()!=IniConfig.iOCRWordCount &&       //wei 20220413 開關字數判斷功能
                           IniConfig.bEnabledOCRCheckWordCount)
                        {
                            ShowErrorMessage("WAR09112", K_RETRY , MMOCR);      //KenHsieh 20251009 : WAR09110 -> WAR09112
                            Task=39;
                            break;
                        }

                        if(IniConfig.bCompareOCRData)                           //KenHsieh 20220825 : 新增OCR比對功能
                        {
                            iret=CheckDataExistAndDuplicate();
                            if(iret==1)
                            {
                                ShowMyMessage("More than one file or file not exist in folder, Please check barcode file");
                                asBarCode="T"+asBarCode;
                                break;
                            }
                            else if(iret==2)
                            {
                                ShowMyMessage("Barcode file name dose not match TestInfo.txt, Please check TestInfo.txt and Barcode file");
                                asBarCode="T"+asBarCode;
                                break;
                            }
                            else if(iret==3)
                            {
                                iret1=ShowErrorMessage("WAR0995", K_RETRY|K_SKIP , MMOCR);
                                if(iret1==K_RETRY)
                                {
                                    Task=30;
                                    break;
                                }
                            }
                            else if(iret==4)
                            {
                                ShowMyMessage("Barcode file is not exist or file extension is wrong, Please check Barcode file");
                                asBarCode="T"+asBarCode;
                                break;
                            }
                            else
                            {
                                WriteDataToFile(asOCRLotPath, asBarCode.c_str());
                            }
                        }

                        MOT[MMTrayY].Tray.cDeviceInf[iOCRPosC][iOCRPosR]=asBarCode.c_str();
                        Buffer1.sprintf("%02d,%02d,%s", iOCRPosR+1, iOCRPosC+1, asBarCode);
                        Buffer2.sprintf("%s_%02d,%02d,%s", Now().FormatString("yyyymmddhhmmss"), iOCRPosR+1, iOCRPosC+1, asBarCode);  //Alick 20170104 (Steven) add 多一組LOG紀錄+上時間
                        fLotInfo->Memo2->Lines->Add(Buffer1.c_str());
                        fLotInfo->Memo3->Lines->Add(Buffer2.c_str());           //Alick 20170104 (Steven) add 多一組LOG紀錄+上時間

                        MOT[MMOCR].SetTraySingleData(iOCRPosC, iOCRPosR, HAS_OCR_OK);
                        iNewOCRCount[iOCRPassCount]++;
                        iNewOCRCount[iOCRSearchCount]++;
                        //AI(ht9045-v899) 20260522: record normal OCR OK write-back path in simulation log
                        asOCRLog.sprintf("Result OK: R=%d C=%d Barcode=%s", iOCRPosR+1, iOCRPosC+1, asBarCode.c_str());
                        OCRSimLogWrite(asOCRLog.c_str());
                        if(IniConfig.bP26_OCRCheckLot && bOCRCheck)
                        {
                            Buffer2=asBarCode.SubString(1,asBarCode.Length()-4);
                            asOCRCheck[iOCRCheck]=Buffer2.c_str();
                            iOCRCheck++;
                        }
                        bOCRReadOK=false;
                        iNewOCRCount[iOCRSkipContinue]=0;
                        Task=40;
                    }
                }
            }
            else
            {
                if(hOCRDealyTime.Off())
                {
                    if(bOCRSimLogEnable)                                        //AI(ht9045-v899) 20260526: stop OCR simulation immediately on capture-result timeout
                    {
                        asOCRLog.sprintf("Simulation alarm stop: capture result timeout R=%d C=%d", iOCRPosR+1, iOCRPosC+1);
                        OCRSimLogWrite(asOCRLog.c_str());
                        ShowErrorMessage("WAR0944", K_RETRY, MMTrayY);
                        return OCR_SIM_RET_CAPTURE_TIMEOUT;
                    }
                    Task=39;
                }
            }
            break;
        case 39:
            iRetryCount++;
            //AI(ht9045-v899) 20260522: log retry count for the current OCR simulation position
            asOCRLog.sprintf("Retry path: R=%d C=%d Retry=%d/%d", iOCRPosR+1, iOCRPosC+1, iRetryCount, IniConfig.iOCRRetry);
            OCRSimLogWrite(asOCRLog.c_str());
            if(iRetryCount<IniConfig.iOCRRetry)
            {
                Task=30;
            }
            else
            {
                iBarCodeErrorC=iOCRPosC;
                iBarCodeErrorR=iOCRPosR;
                bBarCodeError=true;
                //AI(ht9045-v899) 20260522: log final barcode error coordinate before simulation stops
                asOCRLog.sprintf("Barcode error set: R=%d C=%d", iBarCodeErrorR+1, iBarCodeErrorC+1);
                OCRSimLogWrite(asOCRLog.c_str());
                Task=40;
            }
            bOCRReadOK=false;
            break;
        case 40:
            iRetryCount=0;
            if(IniConfig.bEnableOCRMoveSRead)                                   //wei 20161118 OCR S型讀取
            {
                if(iOCRMoveSRead==0 && iOCRPosC>=MOT[MMTrayY].Tray.XItem-1)
                {
                    iOCRMoveSRead=1;
                }
                else if(iOCRMoveSRead==1 && iOCRPosC<=0)
                {
                    iOCRMoveSRead=0;
                }
            }
            //AI(ht9045-v899) 20260522: log end of one OCR simulation tray position before returning to search state
            asOCRLog.sprintf("Position done: R=%d C=%d SearchCount=%d", iOCRPosR+1, iOCRPosC+1, iNewOCRCount[iOCRSearchCount]);
            OCRSimLogWrite(asOCRLog.c_str());
            Task=1;
            break;
        case 200:
            Cylinder[C_OCRLight_Up].Off();
            if(IniConfig.OCRLightChange)
            {
                fOCR->ChangeLightValue(1, 0);
                fOCR->ChangeLightValue(2, 0);
            }

            if(Cylinder[C_OCRLight_Up].OnSensor()==true)                        //wei 20150720
            {
                MOT[MLoaderY].SetSpeed(100);
                //AI(ht9045-v899) 20260522: log finish handling when light is confirmed up and LoaderY speed is restored
                OCRSimLogWrite("OCR finish handling: light up sensor ready, LoaderY speed restored");
                if(CUSTOMER_CODE==CC_KYEC_LEE && iNewOCRCount[iOCRSkipCount]>=IniConfig.iOCRSkip)
                {
                    ret=ShowErrorMessage("WAR0999", K_RETRY|K_TRAY_END , MMOCR);
                    iNewOCRCount[iOCRSkipContinue]=0;                           //Alick 20170110 (Steven) modify iOCRSkipCount=>iOCRSkipContinue
                    if(ret==K_RETRY)
                    {
                        Task=300;
                        return 0;
                    }
                    else
                    {
                        if(IniConfig.bCheckBarCodeMap)                          //Frank 20161025 確認四個角落的OCR Code
                        {
                            bEnterCheckMapflag=true;
                            Task=300;
                        }
                        else
                        {
//                            Task=1;
//                            bRunOcrInsp=false;
//                            return iResult;
                            Task=250;
                        }
                    }
                }
                else
                {
                    if(IniConfig.bCheckBarCodeMap)                              //Frank 20161025 確認四個角落的OCR Code
                    {
                        bEnterCheckMapflag=true;
                        Task=300;
                    }
                    else
                    {
                        if(IniConfig.OCRLightChange)                            //wei 20181225 光源auto change
                        {
                            fOCR->ChangeLightValue(1, 0);
                            fOCR->ChangeLightValue(2, 0);
                        }
//                        Task=1;
//                        bRunOcrInsp=false;
//                        return iResult;
                        Task=250;
                    }
                }
            }
            break;
        case 250:
            if(MTrayXCanSafeMove()==false ||                                    //Ifor 20170626 (wei) Tray Arm Move 前判斷是否可移動
               Cylinder[C_OCRLight_Up].OnSensor()==false ||
               Cylinder[C_OCRLight_Up].GetOutBit())                             //KenHsieh 20250718 : 增加保護OCR汽缸要在上才能移動
            {
                Cylinder[C_OCRLight_Up].Off();
                break;
            }

            if(MOT[MTrayX].MotorMove(Prod.iXTrayColor))
            {
                //AI(ht9045-v899) 20260522: log final TrayArm color-position return before OCR simulation loop returns
                OCRSimLogWrite("TrayArm move to Color done, loop returns OCR result");
                Task=1;
                bRunOcrInsp=false;
                return iResult;
            }
            break;
        case 300:
            Cylinder[C_OCRLight_Up].Off();
            if(Cylinder[C_OCRLight_Up].OnSensor()==true)                        //wei 20150720
            {
                Task=310;
            }
            break;
        case 310:
            if(TrayMoveHome())
            {
                if(IniConfig.bCheckBarCodeMap && bEnterCheckMapflag)            //Frank 20161025 確認四個角落的OCR Code
                {
                    Buffer1.sprintf("PASS: %02d,Key In: %02d,No IC: %02d,Total: %02d", iNewOCRCount[iOCRPassCount] ,iNewOCRCount[iOCRErrCount] ,iNewOCRCount[iOCRSkipCount] ,iNewOCRCount[iOCRTotalCount]);
                    Buffer2.sprintf("%s_PASS: %02d,Key In: %02d,No IC: %02d,Total: %02d", Now().FormatString("yyyymmddhhmmss"), iNewOCRCount[iOCRPassCount] ,iNewOCRCount[iOCRErrCount] ,iNewOCRCount[iOCRSkipCount] ,iNewOCRCount[iOCRTotalCount]);    //Alick 20170104 add 多一組LOG紀錄+上時間
                    fLotInfo->Memo2->Lines->Add(Buffer1.c_str());
                    fLotInfo->Memo3->Lines->Add(Buffer2.c_str());               //Alick 20170104 add 多一組LOG紀錄+上時間
                    fLotInfo->Save_BarCodeLog();
                    CleanOCRData();
                    iLoaderCount=0;
                    iEnterCheckMap=0;
                    Task=20;
                }
                else
                {
                    return 6;
                }
            }
            break;
        case 400:
            bEnterCheckMapflag=false;
            Task=401;
            break;
        case 401:
            if(SearchCheckMap(iTrayRow, iTrayCol, iEnterCheckMap))
            {
                Task=403;
            }
            break;
        case 403:
            if(MoveTrayArmOCRCCDToLoaderStage(iTrayRow, iTrayCol, InArmOffSet[6]->GetX(), InArmOffSet[6]->GetY()))  //(int ix,int iy,int iOffsetX,int iOffsetY)
            {
                fOCR->DeleteOCRImage();
                iEnterCheckMap++;
                Task=410;
            }
            break;
        case 410:
            if(IniConfig.OCRLightNoDown)                                        //wei 20181225 光源不下降
            {
                //AI(ht9045-v899) 20260526: keep secondary OCR check-map trigger aligned with no-down mode
                Cylinder[C_OCRLight_Up].Off();
                OCRSimLogWrite("OCR check-map no-down: cylinder OFF command");
                if(IniConfig.OCRLightChange)                                    //wei 20181225 光源auto change
                {
                    fOCR->ChangeLightValue(1, IniConfig.iBlueLight);
                    fOCR->ChangeLightValue(2, IniConfig.iRedLight);
                }
                hOCRDealyTime.SetSecAndOn(0.2);
                Task=420;
            }
            else
            {
                Cylinder[C_OCRLight_Up].On();
                if(IniConfig.OCRLightChange)                                    //wei 20181225 光源auto change
                {
                    fOCR->ChangeLightValue(1, IniConfig.iBlueLight);
                    fOCR->ChangeLightValue(2, IniConfig.iRedLight);
                }

                if(Cylinder[C_OCRLight_Up].OffSensor()==true)                   //wei 20150720
                {
                    hOCRDealyTime.SetSecAndOn(0.2);
                    Task=420;
                }
            }
            break;
        case 420:
            if(hOCRDealyTime.Off())
            {
                bOCRReadOK=false;
                //AI(ht9045-v899) 20260522: keep second OCR check trigger branch aligned with selected switch/SE8 mode
                if(fOCR->StartOCRCaptureTrigger(bOCRSimBypassAOI)==false)
                {
                    if(bOCRSimLogEnable)                                        //AI(ht9045-v899) 20260526: stop OCR simulation after check-map trigger failure alarm
                    {
                        asOCRLog.sprintf("Simulation alarm stop: check-map trigger failed R=%d C=%d", iTrayRow+1, iTrayCol+1);
                        OCRSimLogWrite(asOCRLog.c_str());
                        ShowErrorMessage("WAR09113", K_RETRY , MMOCR);
                        fOCR->DoConnect();
                        return OCR_SIM_RET_TRIGGER_FAIL;
                    }
                    ShowErrorMessage("WAR09113", K_RETRY , MMOCR);
                    fOCR->DoConnect();
                    break;
                }
                hOCRDealyTime.SetSecAndOn(0.1);
                Task=430;
            }
            break;
        case 430:
            if(hOCRDealyTime.Off())
            {
                fOCR->StopOCRCaptureTrigger();                                  //AI(ht9045-v899) 20260522: command mode leaves switch untouched; switch mode turns output off
                hOCRDealyTime.SetSecAndOn(2);
                bOCRNoIC=false;
                Task=440;
            }
            break;
        case 440:
            if(bOCRReadOK)
            {
                if(asBarCode=="NoIC")
                {
                    bOCRNoIC=true;
                    Task=450;
                }
                else if(asBarCode.Pos("F")==1)
                {
                    bInspectError=true;
                    Task=450;
                }
                else if(asBarCode.Pos("?")!=0)
                {
                    Task=450;
                }
                else if(asBarCode.Pos("T")==1)
                {
                    asBarCode.Delete(1,1);
                    if(MOT[MMTrayY].Tray.cDeviceInf[iTrayCol][iTrayRow]==asBarCode.c_str())
                    {
                        if(iEnterCheckMap>3)
                        {
                            Task=500;
                        }
                        else
                        {
                            Task=401;
                        }
                    }
                    else
                    {
                        Task=460;
                    }
                }
            }
            else
            {
                if(hOCRDealyTime.Off())
                {
                    if(bOCRSimLogEnable)                                        //AI(ht9045-v899) 20260526: stop OCR simulation on check-map capture-result timeout
                    {
                        asOCRLog.sprintf("Simulation alarm stop: check-map result timeout R=%d C=%d", iTrayRow+1, iTrayCol+1);
                        OCRSimLogWrite(asOCRLog.c_str());
                        ShowErrorMessage("WAR0944", K_RETRY, MMTrayY);
                        return OCR_SIM_RET_CAPTURE_TIMEOUT;
                    }
                    Task=450;
                }
            }
            break;
        case 450:
            iRetryCount++;
            if(iRetryCount<IniConfig.iOCRRetry)
            {
                Task=410;
            }
            else
            {
                iBarCodeErrorC=iOCRPosC;
                iBarCodeErrorR=iOCRPosR;
                bBarCodeError=true;
                Task=460;
            }
            bOCRReadOK=false;
            break;
        case 460:
            iRetryCount=0;
            Task=200;
            break;
        case 500:
            Cylinder[C_OCRLight_Up].Off();
            if(IniConfig.OCRLightChange)
            {
                fOCR->ChangeLightValue(1, 0);
                fOCR->ChangeLightValue(2, 0);
            }

            if(Cylinder[C_OCRLight_Up].OnSensor()==true)                        //wei 20150720
            {
                iResult=1;
                Task=1;
                iEnterCheckMap=0;
                bRunOcrInsp=false;
                return iResult;
            }
            break;
    }

    return 0;
}
//------------------------------------------------------------------------------
void CleanOCRData()
{
    fLotInfo->Memo2->Lines->Clear();
    fLotInfo->Memo3->Lines->Clear();
    iNewOCRCount[iOCrLotPassCount]+=iNewOCRCount[iOCRPassCount];
    iNewOCRCount[iOCrLotErrCount]+=iNewOCRCount[iOCRErrCount];
    iNewOCRCount[iOCrLotSkipCount]+=iNewOCRCount[iOCRSkipCount];
    iNewOCRCount[iOCRLotTotalCount]+=iNewOCRCount[iOCRTotalCount];
    iNewOCRCount[iOCRErrCount]=0;
    iNewOCRCount[iOCRSkipCount]=0;
    iNewOCRCount[iOCRPassCount]=0;
    iNewOCRCount[iOCRTotalCount]=0;
    iNewOCRCount[iOCRSkipContinue]=0;                                           //Alick 20170119 add
}
//------------------------------------------------------------------------------
