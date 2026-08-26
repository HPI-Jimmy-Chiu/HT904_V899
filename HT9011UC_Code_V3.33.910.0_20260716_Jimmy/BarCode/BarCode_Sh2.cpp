#include "MachineDefine.h"
#pragma hdrstop

#include "BarCode_Sh2.h"

#include "BarCode.h"
#include "rs232.h"
#include "database.h"
#include "mymessbox.h"
#include "note.h"
#include "cMyDef.h"
#include "common.h"
#include "cprod.h"
#include "MyMotor.h"
#include "MyKitSuck.h"
#include "myQwertyKeyBoard.h"
#include "atester.h"
#include "Main.h"
#include "csystem.h"
#include "uLotInfo.h"
#include "MessageDef.h"
#include "cpublic.h"
#include "cContact.h"
#include "LtcSensor.h"
#include "acarry.h"                                                             //Isaac 20200206 : 2DID All site fail recheck twice and alarm
#include "fVATMesFileSys.h"                                                     //Jimmychiu 20230925 : read 2did in json file
#include "ckernel.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
void TfBarCode::InitialBarcodeScanInShuttle2(bool bClear2DID)                   //Steven 20160823 : 歸零後也要清空蝦頭的2DID
{
    AnsiString Log;
    int iBarCodeRowA=iBarCode2_1;
    int iBarCodeRowB=iBarCode2_2;
    iInitialBarcodeInShuttle2Task=1;
    iShtDupilcateFailCnt[1]=0;

    if(InArmSuck.iShtRow==1)// &&                                                  //Steven 20240227 : Shuttle使用一排的
//       BAR_CODE_INSTALL!=ebcUseOCR)                                             //Ifor 20220322 add 自制OCR不使用 //Steven 20250808 : Mark for OCR
    {
        if(TestIF_File.iSelectUseCCDSh2==1)
        {
            iBarCodeRowA=iBarCode2_1;
            iBarCodeRowB=iBarCode2_2;
        }
        else
        {
            iBarCodeRowA=iBarCode2_2;
            iBarCodeRowB=iBarCode2_1;
        }
    }

    if(bClear2DID)
    {
        for(int i=0; i<BLCarryKit.iMaxRow; i++)
        {
            for(int j=0; j<BLCarryKit.iMaxCol; j++)
            {
                if(BLCarryKit.Item[i][j]!=NULL_IC &&
                   BLCarryKit.Item[i][j]!=HAS_NULL_IC &&
                   BLCarryKit.cDeviceInf[i][j]!="")
                {
                    map2DIter=map2DList.find(BLCarryKit.cDeviceInf[i][j]);
                    if(map2DList.size()!=0 && map2DIter!=map2DList.end())
                    {
                        Log.sprintf("Delete, Shuttle2 %d_%d, Code, %s", i+1, j+1, BLCarryKit.cDeviceInf[i][j]);
                        if(i==0)
                            AddCCDCommunicationLog(iBarCodeRowA, Log);
                        else
                            AddCCDCommunicationLog(iBarCodeRowB, Log);

                        map2DList.erase(map2DIter);
                    }
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
// 1x4, 1x2時, 使用CCD4
//------------------------------------------------------------------------------
extern int SendHTTPRequest(int iSht);
bool TfBarCode::DoBarcodeCCDInShuttle_2(bool bVerify)                           //Steven 20160106 : 改用CCD拍完就跑的方式
{
    static int iNowMoveStep=0, iNowCheckStep=0;
    static int iNowMoveStepMulti2D=0, iNowMoveStepMulti2DX=0, iNowMoveStepMulti2DY=0;
    static int iRetryCount=0, iStepCount;
    static int i2DIDRetryCTSH2=0;                                               //Isaac 20200206 : 2DID All site fail recheck twice and alarm
    static int iClearBuffRetry=0;                                               //Steven 20241108 : flag分開處理
    static int iHttpErrRetryCT=0;
    static int iRetryCountExpTimo=0;                                            //Steven 20160425 : 修改拍照Time Out
    static bool bHasError=false;
    static bool bLotCheckError=false;                                           //Alick 20170313 (wei) add
    static bool bShtHasDupError=false;
    static bool bLotHasDupError=false;
    static bool bNeesScanCode[4][8];
    static bool bHasErr[4][8], bAlarm=false;
    static AnsiString ErrPart;
    static AnsiString sErrorPart="", sSimuCode[4];

    int &Task=iInitialBarcodeInShuttle2Task;
    int pos=0, ret, iPos=0;
    int iSht=1;
    int MSht=MInShuttle2;
    int iBarCodeRowA=iBarCode2_1;
    int iBarCodeRowB=iBarCode2_2;
    int iIndex;
    AnsiString cLastString, sCCDCommand, sLog;
    AnsiString Log;
    AnsiString Str, Str1, Str2, Str3;

    if(InArmSuck.iShtRow==1)// &&                                                  //Steven 20240227 : Shuttle使用一排的
//       BAR_CODE_INSTALL!=ebcUseOCR)                                             //Ifor 20220322 add 自制OCR不使用
    {
        if(TestIF_File.iSelectUseCCDSh2==1)
        {
            iBarCodeRowA=iBarCode2_1;
            iBarCodeRowB=iBarCode2_2;
        }
        else
        {
            iBarCodeRowA=iBarCode2_2;
            iBarCodeRowB=iBarCode2_1;
        }
    }

    switch(Task)
    {
        case 1:                                                                 //jou 20170509 (Steven) : 2D alway retry
            iRetryCount=0;
            iClearBuffRetry=0;
            i2DIDRetryCTSH2=0;
            Task=100;
        case 100:
            if(bVerify==false &&
               BLCarryKit.HasRealIC()==false)
                return true;

            iNowMoveStep=InArmSuck.iShtCol-1;                                   //遞減方式檢查

            if(TestIF_File.bEnableMulti2D)
            {
                iNowMoveStepMulti2D =TestIF_File.iMulti2DCount-1;
                iNowMoveStepMulti2DY=TestIF_File.iMulti2DYItem-1;
                iNowMoveStepMulti2DX=TestIF_File.iMulti2DXItem-1;
            }
            else
            {
                iNowMoveStepMulti2D =0;
                iNowMoveStepMulti2DX=0;
                iNowMoveStepMulti2DY=0;
            }
            iCurrentStep[iBarCodeRowA]=iNowMoveStep;                            //Steven 20160503 : 確認移動位置與拍照位置相同
            iCurrentStep[iBarCodeRowB]=iNowMoveStep;
            bStepError[iBarCodeRowA]=false;                                     //Steven 20160503 : 確認移動位置與拍照位置相同
            bStepError[iBarCodeRowB]=false;
            bCCDBarcodeExposureOK[iBarCodeRowA]=true;
            bCCDBarcodeExposureOK[iBarCodeRowB]=true;
            bCCDBarcodeGetResultOK[iBarCodeRowA]=true;
            bCCDBarcodeGetResultOK[iBarCodeRowB]=true;
            iRetryCountExpTimo=0;                                               //Steven 20160425 : 修改拍照Time Out

            for(int i=0; i<BLCarryKit.iMaxCol; i++)                             //清空畫面
            {
                mtBarcodeInSh->SetCellNumber(iBarCodeRowA, i+1, "");
                mtBarcodeInSh->SetCellNumber(iBarCodeRowB, i+1, "");

                bNeesScanCode[0][i]=false;
                bNeesScanCode[1][i]=false;
            }

            BLCarryKit.cDeviceInf[0][iNowMoveStep]="";
            BLCarryKit.cDeviceInf[1][iNowMoveStep]="";
            bHasError=false;
            iStepCount=0;

            if(dVisionVer>=2)                                                   //wei 20170119 (Steven) 版本號大於2
            {
                sSimuCode[iBarCodeRowA]="GV,0,1,11111111,";
                sSimuCode[iBarCodeRowB]="GV,0,1,11111111,";
            }
            else
            {
                sSimuCode[iBarCodeRowA]="GV,1,11111111,";
                sSimuCode[iBarCodeRowB]="GV,1,11111111,";
            }

            bGetSE9[iBarCodeRowA]=false;
            bGetSE9[iBarCodeRowB]=false;
            AddCCDCommunicationLog(iBarCodeRowA, "<<Cycle, Cycle start.------------------");
            ClearBuffer(iBarCodeRowA, "Clear buffer 1");
            ClearBuffer(iBarCodeRowB, "Clear buffer 1");
            BarcodePosDelay[iSht].SetSecAndOn(5);
            Task=500;
        case 500:
            if(bGetSE9[iBarCodeRowA] && bGetSE9[iBarCodeRowB])
            {
                Task=1000;
                iClearBuffRetry=0;
            }
            else if(cbBarCodeSimulate->Checked)
            {
                Task=1000;
                iClearBuffRetry=0;
            }
            else if(BarcodePosDelay[iSht].Off())
            {
                if(iClearBuffRetry>=3)
                {
                    ShowErrorMessage("WAR0462", K_RETRY, MSht, false, "Clear buffer");
                    Task=1;
                }
                else                                                            //Steven 20240821 : Retry一次
                {
                    bGetSE9[iBarCodeRowA]=false;
                    bGetSE9[iBarCodeRowB]=false;
                    iClearBuffRetry++;
                    ClearBuffer(iBarCodeRowA, "Clear buffer 500-"+AnsiString(iClearBuffRetry));
                    ClearBuffer(iBarCodeRowB, "Clear buffer 500-"+AnsiString(iClearBuffRetry));
                    BarcodePosDelay[iSht].SetSecAndOn(5);
                }
            }
            break;
        case 1000:
            if(InArmSuck.iShtRow==2)
            {
                if(bVerify==false &&
                   ((BLCarryKit.Item[0][iNowMoveStep]==HAS_NULL_IC ||
                     BLCarryKit.Item[0][iNowMoveStep]==NULL_IC) &&
                    (BLCarryKit.Item[1][iNowMoveStep]==HAS_NULL_IC ||
                     BLCarryKit.Item[1][iNowMoveStep]==NULL_IC)))
                {
                    BarcodePosDelay[iSht].SetMSAndOn(1);                        //Steven 20160307 : Go 1120 need reset deelay
                    Task=1120;
                    break;
                }
            }
            else
            {
                if(bVerify==false &&
                   (BLCarryKit.Item[0][iNowMoveStep]==HAS_NULL_IC ||
                    BLCarryKit.Item[0][iNowMoveStep]==NULL_IC))
                {
                    BarcodePosDelay[iSht].SetMSAndOn(1);                        //Steven 20160307 : Go 1120 need reset deelay
                    Task=1120;
                    break;
                }
            }

            pos=GetMovePos(iSht, iNowMoveStep, iNowMoveStepMulti2DX);
            pos+=Offset.iSHLeft2D[iSht];

            if(MOT[MSht].MotorMove(pos))
            {
                if(TestIF_File.bEnableMulti2D)                                  //Steven 20200810 : 一個IC使用多個2DID
                    Log.sprintf("Action, Shuttle %d move to step %d-%d (%d, %d)", iSht+1, iNowMoveStep, iNowMoveStepMulti2D, iNowMoveStepMulti2DY, iNowMoveStepMulti2DX);
                else
                    Log.sprintf("Action, Shuttle %d move to step %d", iSht+1, iNowMoveStep);
                AddCCDCommunicationLog(iBarCodeRowA, Log);
                if(iContactMode==CONTACT_NORMAL)
                {
                    if(iStepCount==0)
                    {
                        BarcodePosDelay[iSht].SetMSAndOn(TestIF_File.iBarCodePos1Delay);
                    }
                    else
                    {
                        BarcodePosDelay[iSht].SetMSAndOn(TestIF_File.iBarCodePosDelay);
                    }
                }
                else
                {
                    BarcodePosDelay[iSht].SetMSAndOn(100);
                }
                iStepCount++;
                Task=1120;
            }
            break;
        case 1120:
            if(BarcodePosDelay[iSht].Off())                                     //到位Delay
            {
                //==> Eastsun 20260527 整合#028.AAL.P13a Sh2 case 1120 AAL wrap-head :KYEC
                for(int i=0; i<2; i++)
                {
                    if(CosFunction.bUseBarcodeAutoAdjustLight==true && TestIF_File.bUseBarcodeAutoAdjustLight==true && bStartAutoAdjustLight==true)
                    {
                        if(bBarcodeNeedAutoAdjust[3-i]==true && (BLCarryKit.Item[i][iNowMoveStep]==HAS_IC || BLCarryKit.Item[i][iNowMoveStep]==HAS_HOT_IC))
                        {
                            sCCDCommand="SE0,2";
                            bCCDBarcodeAutoAdjustLight[3-i]=true;
                            SendCCDCommand(iBarCodeRowB-i, "AutoAdjustLight 1120: ", sCCDCommand);
                        }
                        else
                        {
                            bCCDBarcodeAutoAdjustLight[3-i]=false;
                        }
                    }
                    else
                    {
                        bCCDBarcodeAutoAdjustLight[3-i]=false;
                    }
                }

                if(bCCDBarcodeAutoAdjustLight[2]==true || bCCDBarcodeAutoAdjustLight[3]==true)
                {
                    BarcodeAutoAdjustLightTimeOut[iSht].SetSecAndOn(TestIF_File.iAutoAdjustLightTimeOut);
                    Task=1121;
                }
                else
                {
	                //<== Eastsun 20260527 整合#028.AAL.P13a
	                if(InArmSuck.iShtRow==2)                                        //IC在0的位置, 使用CCD4
	                {
	                    bCCDBarcodeExposureOK[iBarCodeRowA]=false;
	                }
	                else if(TestIF_File.bEnableMulti2D &&                           //Steven 20200810 : 一個IC使用多個2DID
	                        (TestIF_File.iMulti2DType==e2x1In2CCD ||
	                         TestIF_File.iMulti2DType==e2x2In2CCD))
	                {
	                    bCCDBarcodeExposureOK[iBarCodeRowA]=false;
	                }
	                else
	                {
	                    bCCDBarcodeExposureOK[iBarCodeRowA]=true;
	                }
	                bCCDBarcodeExposureOK[iBarCodeRowB]=false;
	                sCCDCommand="SE0,";
	
	                if(InArmSuck.iShtRow==2)
	                {
	                    if(bVerify==false &&
	                       (BLCarryKit.Item[0][iNowMoveStep]==HAS_NULL_IC ||
	                        BLCarryKit.Item[0][iNowMoveStep]==NULL_IC))
	                    {
	                        SendSE0(iBarCodeRowA, false, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");   //Steven 20200908 : 整合2DID SE0
	                        bNeesScanCode[0][iNowMoveStep]=false;
	                    }
	                    else
	                    {
	                        SendSE0(iBarCodeRowA, true, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");    //Steven 20200908 : 整合2DID SE0
	                        bNeesScanCode[0][iNowMoveStep]=true;
	                    }
	
	                    if(bVerify==false &&
	                       (BLCarryKit.Item[1][iNowMoveStep]==HAS_NULL_IC ||
	                        BLCarryKit.Item[1][iNowMoveStep]==NULL_IC))
	                    {
	                        SendSE0(iBarCodeRowB, false, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");   //Steven 20200908 : 整合2DID SE0
	                        bNeesScanCode[1][iNowMoveStep]=false;
	                    }
	                    else
	                    {
	                        SendSE0(iBarCodeRowB, true, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");    //Steven 20200908 : 整合2DID SE0
	                        bNeesScanCode[1][iNowMoveStep]=true;
	                    }
	                }
	                else if(TestIF_File.bEnableMulti2D &&                           //Steven 20240612 : for 對角2D
	                        (TestIF_File.iMulti2DType==e2x1In2CCD ||
	                         TestIF_File.iMulti2DType==e2x2In2CCD))
	                {
	                    if(bVerify==false &&
	                       (BLCarryKit.Item[0][iNowMoveStep]==HAS_NULL_IC ||
	                        BLCarryKit.Item[0][iNowMoveStep]==NULL_IC))
	                    {
	                        SendSE0(iBarCodeRowA, false, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");   //Steven 20200908 : 整合2DID SE0
	                        SendSE0(iBarCodeRowB, false, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");   //Steven 20200908 : 整合2DID SE0
	                        bNeesScanCode[0][iNowMoveStep]=false;
	                    }
	                    else
	                    {
	                        if(TestIF_File.iMulti2DMap[0][iNowMoveStepMulti2D]>0)
	                            SendSE0(iBarCodeRowA, true, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");
	                        else
	                            SendSE0(iBarCodeRowA, false, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");
	
	                        if(TestIF_File.iMulti2DMap[1][iNowMoveStepMulti2D]>0)
	                            SendSE0(iBarCodeRowB, true, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");
	                        else
	                            SendSE0(iBarCodeRowB, false, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");
	                        bNeesScanCode[0][iNowMoveStep]=true;
	                    }
	                }
	                else
	                {
	                    if(bVerify==false &&
	                       (BLCarryKit.Item[0][iNowMoveStep]==HAS_NULL_IC ||
	                        BLCarryKit.Item[0][iNowMoveStep]==NULL_IC))
	                    {
	                        SendSE0(iBarCodeRowB, false, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");   //Steven 20200908 : 整合2DID SE0
	                        bNeesScanCode[0][iNowMoveStep]=false;
	                    }
	                    else
	                    {
	                        SendSE0(iBarCodeRowB, true, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");    //Steven 20200908 : 整合2DID SE0
	                        bNeesScanCode[0][iNowMoveStep]=true;
	                    }
	                }
	
	                Task=1150;
	                iRetryCountExpTimo=0;                                           //Steven 20160425 : 修改拍照Time Out
	                BarcodeExposureDelay[iSht].SetMSAndOn(TestIF_File.i2DTriggerTime);
	            //==> Eastsun 20260527 整合#028.AAL.P13b Sh2 case 1120 AAL wrap-tail :KYEC
	            }
            //<== Eastsun 20260527 整合#028.AAL.P13b
            }
            break;
        //==> Eastsun 20260527 整合#028.AAL.P14 Sh2 case 1121 AAL exposure wait :KYEC
        case 1121:
            if(bCCDBarcodeAutoAdjustLight[2]==false && bCCDBarcodeAutoAdjustLight[3]==false)
            {
                if(bBarcodeNeedAutoAdjust[0]==false && bBarcodeNeedAutoAdjust[1]==false &&
                   bBarcodeNeedAutoAdjust[2]==false && bBarcodeNeedAutoAdjust[3]==false )
                {
                    bStartAutoAdjustLight=false;
                }
                Task=1120;
            }

            if(BarcodeAutoAdjustLightTimeOut[iSht].Off())
            {
                ret=ShowErrorMessage("WAR16200", K_RETRY|K_SKIP, MSht);
                if(ret==K_SKIP)
                {
                    bStartAutoAdjustLight=false;
                    for(int i=0; i<4; i++)
                    {
                        bBarcodeNeedAutoAdjust[i]=false;
                    }
                }
                Task=1120;
            }
            break;
        //<== Eastsun 20260527 整合#028.AAL.P14
        case 1150:
            if(bStepError[iBarCodeRowA]==true ||
               bStepError[iBarCodeRowB]==true)                                  //Steven 20160503 : 確認移動位置與拍照位置相同
            {
                ErrPart="";
                if(InArmSuck.iShtRow==2)                                        //RogerYang 20180605 (Steven) : 修正掃單排未判斷
                {
                    if(bStepError[iBarCodeRowA])
                        ErrPart+=IndexSuckName[0][iNowMoveStep];

                    if(bStepError[iBarCodeRowB])
                        ErrPart+=IndexSuckName[1][iNowMoveStep];
                }
                else if(TestIF_File.bEnableMulti2D &&                           //Steven 20240612 : for 對角2D
                        (TestIF_File.iMulti2DType==e2x1In2CCD ||
                         TestIF_File.iMulti2DType==e2x2In2CCD))
                {
                    if(bStepError[iBarCodeRowA] ||
                       bStepError[iBarCodeRowB])
                        ErrPart+=IndexSuckName[0][iNowMoveStep];
                }
                else
                {
                    if(bStepError[iBarCodeRowB])
                        ErrPart+=IndexSuckName[0][iNowMoveStep];
                }
                Task=1180;
            }
            else if(bCCDBarcodeExposureOK[iBarCodeRowA]==true &&
                    bCCDBarcodeExposureOK[iBarCodeRowB]==true)                  //拍照完成
            {
                if(bVerify==true ||
                   (BLCarryKit.Item[0][iNowMoveStep]!=HAS_NULL_IC &&
                    BLCarryKit.Item[0][iNowMoveStep]!=NULL_IC))
                {
                    mtBarcodeInSh->SetCellNumber(iBarCodeRowA, iNowMoveStep+1, "Waiting result..");
                }

                if(InArmSuck.iShtRow==2)                                        //IC在0的位置, 使用CCD4
                {
                    if(bVerify==true ||
                       (BLCarryKit.Item[1][iNowMoveStep]!=HAS_NULL_IC &&
                        BLCarryKit.Item[1][iNowMoveStep]!=NULL_IC))
                    {
                        mtBarcodeInSh->SetCellNumber(iBarCodeRowB, iNowMoveStep+1, "Waiting result..");
                    }
                }
                else if(TestIF_File.bEnableMulti2D &&                           //Steven 20240612 : for 對角2D
                        (TestIF_File.iMulti2DType==e2x1In2CCD ||
                         TestIF_File.iMulti2DType==e2x2In2CCD))
                {
                    if(bVerify==true ||
                       (BLCarryKit.Item[0][iNowMoveStep]!=HAS_NULL_IC &&
                        BLCarryKit.Item[0][iNowMoveStep]!=NULL_IC))
                    {
                        mtBarcodeInSh->SetCellNumber(iBarCodeRowB, iNowMoveStep+1, "Waiting result..");
                    }
                }
                Task=1200;
            }
            else if(cbBarCodeSimulate->Checked)
            {
                sSimuCode[iBarCodeRowB]+=GetSimuBarCodeName(iBarCodeRowB, iNowMoveStep)+",";

                if(InArmSuck.iShtRow==2)
                {
                    sSimuCode[iBarCodeRowA]+=GetSimuBarCodeName(iBarCodeRowA, iNowMoveStep)+",";
                }
                else if(TestIF_File.bEnableMulti2D &&                           //Steven 20240612 : for 對角2D
                        (TestIF_File.iMulti2DType==e2x1In2CCD ||
                         TestIF_File.iMulti2DType==e2x2In2CCD))
                {
                    sSimuCode[iBarCodeRowA]+=GetSimuBarCodeName(iBarCodeRowA, iNowMoveStep)+",";
                }
                Task=1200;
            }
            else if(BarcodeExposureDelay[iSht].Off())                           //拍照time out
            {
                if(InArmSuck.iShtRow==2)
                {
                    if(bCCDBarcodeExposureOK[iBarCodeRowA]==false)
                    {
                        mtBarcodeInSh->SetCellNumber(iBarCodeRowA, iNowMoveStep+1, "Exposure time out");
                        Log.sprintf("Action 1150, Exposure time out. Retry: %d", iRetryCountExpTimo);
                        AddCCDCommunicationLog(iBarCodeRowA, Log);
                    }
                    else
                    {
                        if(bVerify==true ||
                           (BLCarryKit.Item[0][iNowMoveStep]!=HAS_NULL_IC &&
                            BLCarryKit.Item[0][iNowMoveStep]!=NULL_IC))
                            mtBarcodeInSh->SetCellNumber(iBarCodeRowA, iNowMoveStep+1, "Waiting result..");
                    }

                    if(bCCDBarcodeExposureOK[iBarCodeRowB]==false)
                    {
                        mtBarcodeInSh->SetCellNumber(iBarCodeRowB, iNowMoveStep+1, "Exposure time out");
                        Log.sprintf("Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                        AddCCDCommunicationLog(iBarCodeRowB, Log);
                    }
                    else
                    {
                        if(bVerify==true ||
                           (BLCarryKit.Item[1][iNowMoveStep]!=HAS_NULL_IC &&
                            BLCarryKit.Item[1][iNowMoveStep]!=NULL_IC))
                        {
                            mtBarcodeInSh->SetCellNumber(iBarCodeRowB, iNowMoveStep+1, "Waiting result..");
                        }
                    }
                }
                else if(TestIF_File.bEnableMulti2D &&                           //Steven 20240612 : for 對角2D
                        (TestIF_File.iMulti2DType==e2x1In2CCD ||
                         TestIF_File.iMulti2DType==e2x2In2CCD))
                {
                    if(bCCDBarcodeExposureOK[iBarCodeRowA]==false)
                    {
                        mtBarcodeInSh->SetCellNumber(iBarCodeRowA, iNowMoveStep+1, "Exposure time out");
                        Log.sprintf("Action 1150, Exposure time out. Retry: %d", iRetryCountExpTimo);
                        AddCCDCommunicationLog(iBarCodeRowA, Log);
                    }
                    else
                    {
                        if(bVerify==true ||
                           (BLCarryKit.Item[0][iNowMoveStep]!=HAS_NULL_IC &&
                            BLCarryKit.Item[0][iNowMoveStep]!=NULL_IC))
                        {
                            mtBarcodeInSh->SetCellNumber(iBarCodeRowA, iNowMoveStep+1, "Waiting result..");
                        }
                    }

                    if(bCCDBarcodeExposureOK[iBarCodeRowB]==false)
                    {
                        mtBarcodeInSh->SetCellNumber(iBarCodeRowB, iNowMoveStep+1, "Exposure time out");
                        Log.sprintf("Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                        AddCCDCommunicationLog(iBarCodeRowB, Log);
                    }
                    else
                    {
                        if(bVerify==true ||
                           (BLCarryKit.Item[1][iNowMoveStep]!=HAS_NULL_IC &&    //JerryYang 20250701 : Fixed for 2DID
                            BLCarryKit.Item[1][iNowMoveStep]!=NULL_IC))
                        {
                            mtBarcodeInSh->SetCellNumber(iBarCodeRowB, iNowMoveStep+1, "Waiting result..");
                        }
                    }
                }
                else
                {
                    if(bCCDBarcodeExposureOK[iBarCodeRowB]==false)              //IC在0的位置, 使用CCD4
                    {
                        mtBarcodeInSh->SetCellNumber(iBarCodeRowA, iNowMoveStep+1, "Exposure time out");
                        Log.sprintf("Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                        AddCCDCommunicationLog(iBarCodeRowB, Log);
                    }
                    else
                    {
                        if(bVerify==true ||
                           (BLCarryKit.Item[0][iNowMoveStep]!=HAS_NULL_IC &&
                            BLCarryKit.Item[0][iNowMoveStep]!=NULL_IC))
                        {
                            mtBarcodeInSh->SetCellNumber(iBarCodeRowA, iNowMoveStep+1, "Waiting result..");
                        }
                    }
                }
                iRetryCountExpTimo++;                                           //Steven 20160425 : 修改拍照Time Out
                Task=1160;
            }
            break;
        case 1160:
            if(iRetryCountExpTimo<3)
            {
                Task=1170;
            }
            else
            {
                ret=ShowErrorMessage("WAR0464", K_RETRY|K_SKIP, MSht);          //2D function CCD 3 or 4 Exposure time out!
                if(ret==K_SKIP)
                {
                    Task=1200;
                }
                else
                {
                    Task=1170;
                }
                iRetryCountExpTimo=0;
            }
            break;
        case 1170:
            if(InArmSuck.iShtRow==2)
            {
                if(bCCDBarcodeExposureOK[iBarCodeRowA]==false)
                {
                    if(bVerify==false &&
                       (BLCarryKit.Item[0][iNowMoveStep]==HAS_NULL_IC ||
                        BLCarryKit.Item[0][iNowMoveStep]==NULL_IC))
                    {
                        SendSE0(iBarCodeRowA, false, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");   //Steven 20200908 : 整合2DID SE0v
                        bNeesScanCode[0][iNowMoveStep]=false;
                    }
                    else
                    {
                        SendSE0(iBarCodeRowA, true, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");   //Steven 20200908 : 整合2DID SE0
                        bNeesScanCode[0][iNowMoveStep]=true;
                    }
                }

                if(bCCDBarcodeExposureOK[iBarCodeRowB]==false)
                {
                    if(bVerify==false &&
                       (BLCarryKit.Item[1][iNowMoveStep]==HAS_NULL_IC ||
                        BLCarryKit.Item[1][iNowMoveStep]==NULL_IC))
                    {
                        SendSE0(iBarCodeRowB, false, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");   //Steven 20200908 : 整合2DID SE0
                        bNeesScanCode[1][iNowMoveStep]=false;
                    }
                    else
                    {
                        SendSE0(iBarCodeRowB, true, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");   //Steven 20200908 : 整合2DID SE0
                        bNeesScanCode[1][iNowMoveStep]=true;
                    }
                }
            }
            else if(TestIF_File.bEnableMulti2D &&                               //Steven 20240612 : for 對角2D
                    (TestIF_File.iMulti2DType==e2x1In2CCD ||
                     TestIF_File.iMulti2DType==e2x2In2CCD))
            {
                if(bCCDBarcodeExposureOK[iBarCodeRowA]==false)
                {
                    if(bVerify==false &&
                       (BLCarryKit.Item[0][iNowMoveStep]==HAS_NULL_IC ||
                        BLCarryKit.Item[0][iNowMoveStep]==NULL_IC))
                    {
                        bNeesScanCode[0][iNowMoveStep]=false;
                        SendSE0(iBarCodeRowA, false, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");   //Steven 20200908 : 整合2DID SE0v
                    }
                    else
                    {
                        bNeesScanCode[0][iNowMoveStep]=true;
                        if(TestIF_File.iMulti2DMap[0][iNowMoveStepMulti2D]>0)
                            SendSE0(iBarCodeRowA, true, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");   //Steven 20200908 : 整合2DID SE0
                        else
                            SendSE0(iBarCodeRowA, false, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");
                    }
                }

                if(bCCDBarcodeExposureOK[iBarCodeRowB]==false)
                {
                    if(bVerify==false &&
                       (BLCarryKit.Item[0][iNowMoveStep]==HAS_NULL_IC ||
                        BLCarryKit.Item[0][iNowMoveStep]==NULL_IC))
                    {
                        bNeesScanCode[0][iNowMoveStep]=false;
                        SendSE0(iBarCodeRowB, false, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");   //Steven 20200908 : 整合2DID SE0
                    }
                    else
                    {
                        bNeesScanCode[0][iNowMoveStep]=true;
                        if(TestIF_File.iMulti2DMap[1][iNowMoveStepMulti2D]>0)
                            SendSE0(iBarCodeRowB, true, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");
                        else
                            SendSE0(iBarCodeRowB, false, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");
                    }
                }
            }
            else
            {
                if(bCCDBarcodeExposureOK[iBarCodeRowB]==false)
                {
                    if(bVerify==false &&
                       (BLCarryKit.Item[0][iNowMoveStep]==HAS_NULL_IC ||
                        BLCarryKit.Item[0][iNowMoveStep]==NULL_IC))
                    {
                        SendSE0(iBarCodeRowB, false, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");   //Steven 20200908 : 整合2DID SE0v
                        bNeesScanCode[0][iNowMoveStep]=false;
                    }
                    else
                    {
                        SendSE0(iBarCodeRowB, true, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");   //Steven 20200908 : 整合2DID SE0v
                        bNeesScanCode[0][iNowMoveStep]=true;
                    }
                    SendCCDCommand(iBarCodeRowB, "exposure 1170: ", sCCDCommand);     //IC在0的位置, 使用CCD4
                }
            }
            Task=1150;
            BarcodeExposureDelay[iSht].SetMSAndOn(TestIF_File.i2DTriggerTime);
            break;
        case 1180:
            Log.sprintf("Alarm, In shuttle %d exposure position error!", iSht+1);
            AddCCDCommunicationLog(iBarCodeRowA, Log);
            ShowErrorMessage("WAR0470", K_RETRY, MSht, false, ErrPart);         //In shuttle 2 exposure position error!
            Task=1;
            break;
        case 1200:
            if(TestIF_File.bEnableMulti2D)                                      //Steven 20200810 : 一個IC使用多個2DID
            {
                iNowMoveStepMulti2D--;
                iNowMoveStepMulti2DY--;
                if(iNowMoveStepMulti2DY<0)
                {
                    iNowMoveStepMulti2DY=TestIF_File.iMulti2DYItem-1;
                    iNowMoveStepMulti2DX--;
                    if(iNowMoveStepMulti2DX<0)
                    {
                        iNowMoveStepMulti2DX=TestIF_File.iMulti2DXItem-1;
                        iNowMoveStepMulti2D=TestIF_File.iMulti2DCount-1;
                        iNowMoveStep--;
                    }
                }
            }
            else
            {
                iNowMoveStep--;
            }
            iCurrentStep[iBarCodeRowA]=iNowMoveStep;                            //Steven 20160503 : 確認移動位置與拍照位置相同
            iCurrentStep[iBarCodeRowB]=iNowMoveStep;

            if(iNowMoveStep>=0 && iNowMoveStep<InArmSuck.iShtCol)
            {
                Task=1000;
            }
            else
            {
                Task=1300;
            }
            break;
        case 1300:
            if(TestIF_File.bEnableMulti2D)                                      //Steven 20200810 : 一個IC使用多個2DID
            {
                for(int i=0; i<_MAX_SUCK_COL_ITEM; i++)
                {
                    cMilti2DBuffer[iBarCodeRowA][i]="";
                    cMilti2DBuffer[iBarCodeRowB][i]="";
                }
            }

            //==> Eastsun 20260527 整合#BCIR.P12 toggle GG/GV :KYEC
            if(TestIF_File.bBarCodeInspReport)                                  //Sam 20240426 : Add BarCoder Inspection Report
                sCCDCommand="GG,0";
            else if(dVisionVer>=2)                                              //wei 20170119 (Steven) 版本號大於2
                sCCDCommand="GV,0";
            //<== Eastsun 20260527 整合#BCIR.P12
            else
                sCCDCommand="GV0";                                              //Ifor 20151228 取得 CCD 讀取資料

            if(TestIF_File.i2DIDFormat==eAMD)                                   //JerryYang 20200422 2DID format選項改用下拉選單
            {
                fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(1, 1, "");
                fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(1, 2, "");
            }

            if(InArmSuck.iShtRow==2)                                            //IC在0的位置, 使用CCD4
            {
                SendCCDCommand(iBarCodeRowA, "get result 1300: ", sCCDCommand);
                bCCDBarcodeGetResultOK[iBarCodeRowA]=false;
            }
            else if(TestIF_File.bEnableMulti2D &&                               //Steven 20240612 : for 對角2D
                    (TestIF_File.iMulti2DType==e2x1In2CCD ||
                     TestIF_File.iMulti2DType==e2x2In2CCD))
            {
                SendCCDCommand(iBarCodeRowA, "get result 1300: ", sCCDCommand);
                bCCDBarcodeGetResultOK[iBarCodeRowA]=false;
            }
            else
            {
                bCCDBarcodeGetResultOK[iBarCodeRowA]=true;
            }

            if(cbBarCodeSimulate->Checked)
            {
                Socket2DReceiveList[iBarCodeRowB]->Add(sSimuCode[iBarCodeRowB]);
                if(InArmSuck.iShtRow==2)
                {
                    Socket2DReceiveList[iBarCodeRowA]->Add(sSimuCode[iBarCodeRowA]);
                }
                else if(TestIF_File.bEnableMulti2D &&                           //Steven 20240612 : for 對角2D
                        (TestIF_File.iMulti2DType==e2x1In2CCD ||
                         TestIF_File.iMulti2DType==e2x2In2CCD))
                {
                    Socket2DReceiveList[iBarCodeRowA]->Add(sSimuCode[iBarCodeRowA]);
                }
            }

            bCCDBarcodeGetResultOK[iBarCodeRowB]=false;
            SendCCDCommand(iBarCodeRowB, "get result 1130: ", sCCDCommand);
            bHasError=false;
            Task=1350;
            BarcodeDelay[iSht].SetMSAndOn(TestIF_File.iBarCodeDelay);           //這個是讀取Time Out用的
            break;
        case 1350:
            if(bCCDBarcodeGetResultOK[iBarCodeRowA]==true &&
               bCCDBarcodeGetResultOK[iBarCodeRowB]==true)                      //讀取完成
            {
                CCDBarcode_SortingForMulti2D(1);
                sErrorPart="";
                for(int i=0; i<InArmSuck.iShtCol; i++)                          //清空畫面
                {
                    if(InArmSuck.iShtRow==2)                                    //RogerYang 20180605 (Steven) : 修正掃單排未判斷
                    {
                        if(bNeesScanCode[0][i]==true &&
                           (BLCarryKit.Item[0][i]!=HAS_NULL_IC &&
                            BLCarryKit.Item[0][i]!=NULL_IC) &&
                           (BLCarryKit.cDeviceInf[0][i].AnsiPos(asBarCodeErrorSend)!=0 ||
                            BLCarryKit.cDeviceInf[0][i]==""))                   //wei 20160318 Barcode Error依客戶設定
                        {
                            bHasError=true;
                            sErrorPart+=IndexSuckName[0][i];
                            if(CUSTOMER_CODE==CC_ASE_CL)                        //KaiChen 20191121 ：中壢日月光 2D Check Sum
                            {
                                sErrorPart+="(Err)";
                            }
                        }
                        else if(bNeesScanCode[0][i]==true &&
                           (BLCarryKit.Item[0][i]!=HAS_NULL_IC &&
                            BLCarryKit.Item[0][i]!=NULL_IC) &&
                           (BLCarryKit.cDeviceInf[0][i].AnsiPos(asBarCodeErrorCheckSum) ||
                            BLCarryKit.cDeviceInf[0][i]==""))                   //KaiChen 20191121 ：中壢日月光 2D Check Sum
                        {
                            bHasError=true;
                            sErrorPart+=IndexSuckName[0][i];
                            if(CUSTOMER_CODE==CC_ASE_CL)                        //KaiChen 20191121 ：中壢日月光 2D Check Sum
                            {
                                sErrorPart+="(Sum)";
                            }
                        }
                        else if(bNeesScanCode[0][i]==true &&
                                (BLCarryKit.Item[0][i]!=HAS_NULL_IC &&
                                 BLCarryKit.Item[0][i]!=NULL_IC) &&
                                ((fBarCode->RunCheckBarcodeByServerData()==true) &&
                                 (fMesSystem->IsMatchServerData(BLCarryKit.cDeviceInf[0][i])==false) ))      //Jimmychiu 20230925 : read 2did in json file
                        {
                            bHasError=true;
                            sErrorPart+=IndexSuckName[0][i];
                        }
                        else
                        {
                            bNeesScanCode[0][i]=false;
                        }

                        if(bNeesScanCode[1][i]==true &&
                           (BLCarryKit.Item[1][i]!=HAS_NULL_IC &&
                            BLCarryKit.Item[1][i]!=NULL_IC) &&
                           (BLCarryKit.cDeviceInf[1][i].AnsiPos(asBarCodeErrorSend)!=0 ||
                            BLCarryKit.cDeviceInf[1][i]==""))                   //wei 20160318 Barcode Error依客戶設定
                        {
                            bHasError=true;
                            sErrorPart+=IndexSuckName[1][i];
                            if(CUSTOMER_CODE==CC_ASE_CL)                        //KaiChen 20191121 ：中壢日月光 2D Check Sum
                            {
                                sErrorPart+="(Err)";
                            }
                        }
                        else if(bNeesScanCode[1][i]==true &&
                                (BLCarryKit.Item[1][i]!=HAS_NULL_IC &&
                                 BLCarryKit.Item[1][i]!=NULL_IC) &&
                                (BLCarryKit.cDeviceInf[1][i].AnsiPos(asBarCodeErrorCheckSum)!=0 ||
                                 BLCarryKit.cDeviceInf[1][i]==""))              //KaiChen 20191121 ：中壢日月光 2D Check Sum
                        {
                            bHasError=true;
                            sErrorPart+=IndexSuckName[1][i];
                            if(CUSTOMER_CODE==CC_ASE_CL)                        //KaiChen 20191121 ：中壢日月光 2D Check Sum
                            {
                                sErrorPart+="(Sum)";
                            }
                        }
                        else if(bNeesScanCode[1][i]==true &&
                                (BLCarryKit.Item[1][i]!=HAS_NULL_IC &&
                                 BLCarryKit.Item[1][i]!=NULL_IC) &&
                                ((fBarCode->RunCheckBarcodeByServerData()==true) &&
                                 (fMesSystem->IsMatchServerData(BLCarryKit.cDeviceInf[1][i])==false) ))      //Jimmychiu 20230925 : read 2did in json file
                        {
                            bHasError=true;
                            sErrorPart+=IndexSuckName[1][i];
                        }
                        else
                        {
                            bNeesScanCode[1][i]=false;
                        }
                    }
                    else
                    {
                        if(bNeesScanCode[0][i]==true &&
                           (BLCarryKit.Item[0][i]!=HAS_NULL_IC &&
                            BLCarryKit.Item[0][i]!=NULL_IC) &&
                           (BLCarryKit.cDeviceInf[0][i].AnsiPos(asBarCodeErrorSend)!=0 ||
                            BLCarryKit.cDeviceInf[0][i]==""))                   //wei 20160318 Barcode Error依客戶設定
                        {
                            bHasError=true;
                            sErrorPart+=IndexSuckName[0][i];
                            sErrorPart+="(Err)";
                        }
                        else if(bNeesScanCode[0][i]==true &&
                                (BLCarryKit.Item[0][i]!=HAS_NULL_IC &&
                                 BLCarryKit.Item[0][i]!=NULL_IC) &&
                                (BLCarryKit.cDeviceInf[0][i].AnsiPos(asBarCodeErrorCheckSum)!=0 ||
                                 BLCarryKit.cDeviceInf[0][i]==""))              //KaiChen 20191121 ：中壢日月光 2D Check Sum
                        {
                            bHasError=true;
                            sErrorPart+=IndexSuckName[0][i];
                            sErrorPart+="(Sum)";
                        }
                        else
                        {
                            bNeesScanCode[0][i]=false;
                        }
                    }
                }

                Task=2000;
            }
            else if(BarcodeDelay[iSht].Off())                                   //讀取超時
            {
                sErrorPart="";
                if(InArmSuck.iShtRow==2)
                {
                    if(bCCDBarcodeGetResultOK[iBarCodeRowA]==false)
                    {
                        bCCDBarcodeGetResultOK[iBarCodeRowA]=true;
                        for(int i=0; i<BLCarryKit.iMaxCol; i++)                 //清空畫面
                        {
                            if(bNeesScanCode[0][i]==true)
                                mtBarcodeInSh->SetCellNumber(iBarCodeRowA, i+1, "Decode time Out");
                        }
                        AddCCDCommunicationLog(iBarCodeRowA, "Action 1350, Decode time out.");
                        bHasError=true;
                    }

                    if(bCCDBarcodeGetResultOK[iBarCodeRowB]==false)
                    {
                        bCCDBarcodeGetResultOK[iBarCodeRowB]=true;
                        for(int i=0; i<BLCarryKit.iMaxCol; i++)                 //清空畫面
                        {
                            if(bNeesScanCode[1][i]==true)
                                mtBarcodeInSh->SetCellNumber(iBarCodeRowB, i+1, "Decode time Out");
                        }
                        AddCCDCommunicationLog(iBarCodeRowB, "Action 1350, Decode time out.");
                        bHasError=true;
                    }
                }
                else if(TestIF_File.bEnableMulti2D &&                           //Steven 20240612 : for 對角2D
                        (TestIF_File.iMulti2DType==e2x1In2CCD ||
                         TestIF_File.iMulti2DType==e2x2In2CCD))
                {
                    if(bCCDBarcodeGetResultOK[iBarCodeRowA]==false)
                    {
                        bCCDBarcodeGetResultOK[iBarCodeRowA]=true;
                        for(int i=0; i<BLCarryKit.iMaxCol; i++)                 //清空畫面
                        {
                            if(bNeesScanCode[0][i]==true)
                                mtBarcodeInSh->SetCellNumber(iBarCodeRowA, i+1, "Decode time Out");
                        }
                        AddCCDCommunicationLog(iBarCodeRowA, "Action 1350, Decode time out.");
                        bHasError=true;
                    }

                    if(bCCDBarcodeGetResultOK[iBarCodeRowB]==false)
                    {
                        bCCDBarcodeGetResultOK[iBarCodeRowB]=true;
                        for(int i=0; i<BLCarryKit.iMaxCol; i++)                 //清空畫面
                        {
                            if(bNeesScanCode[0][i]==true)
                                mtBarcodeInSh->SetCellNumber(iBarCodeRowB, i+1, "Decode time Out");
                        }
                        AddCCDCommunicationLog(iBarCodeRowB, "Action 1350, Decode time out.");
                        bHasError=true;
                    }
                }
                else                                                            //IC在0的位置, 使用CCD4
                {
                    if(bCCDBarcodeGetResultOK[iBarCodeRowB]==false)
                    {
                        bCCDBarcodeGetResultOK[iBarCodeRowB]=true;
                        for(int i=0; i<BLCarryKit.iMaxCol; i++)                 //清空畫面
                        {
                            if(bNeesScanCode[0][i]==true)
                                mtBarcodeInSh->SetCellNumber(iBarCodeRowA, i+1, "Decode time Out");
                        }
                        AddCCDCommunicationLog(iBarCodeRowB, "Action 1350, Decode time out.");
                        bHasError=true;
                    }
                }
                Task=2000;
            }
            break;
        case 2000:
            if(bHasError==true)
            {
                if(iRetryCount<TestIF_File.iBarcodeRetryCount)                  //Steven 20160421 : 修正判斷方式
                {
                    Task=3000;
                }
                else
                {
                    if(TestIF_File.bNoCodeDeviceAutoSkip &&
                       fContact->IsRun2DCheck()==false)                         //JerryYang 20250220 : 2DID硬體順序檢查功能
                    {
                        AddCCDCommunicationLog(iBarCodeRowA, ">>Cycle 2000, Decode end with auto skip.---------------\r\n");
                        if(TestIF_File.bCheckCodeByShuttle==false &&
                           TestIF_File.bCheckCodeByLot==false)
                        {
                            Task=5000;
                        }
                        else
                        {
                            if(TestIF_File.bCheckLotHaveCode)                   //Steven 20170707 (wei) : Fixed 2DID for Korea
                                Task=3500;                                      //Alick 20170313 (wei) modify 4000==>3500
                            else
                                Task=4000;
                        }

                        if(CUSTOMER_CODE==CC_ASE_KaohSiung)                     //KEVIN 20211005 ADD CONTINUE FAIL
                        {
                            for(int i=0; i<InArmSuck.iShtCol; i++)              //JerryYang 20200616 2DID In-house版本支援累加fail alarm功能       //清空畫面
                            {
                                if(bNeesScanCode[0][i]==true &&
                                   (BLCarryKit.Item[0][i]!=HAS_NULL_IC &&
                                    BLCarryKit.Item[0][i]!=NULL_IC) &&
                                   (BLCarryKit.cDeviceInf[0][i]==asBarCodeErrorSend ||
                                    BLCarryKit.cDeviceInf[0][i]==""))           //wei 20160318 Barcode Error依客戶設定
                                {
                                    iBarcodeReject++;
                                }

                                if(bNeesScanCode[1][i]==true &&
                                   (BLCarryKit.Item[1][i]!=HAS_NULL_IC &&
                                    BLCarryKit.Item[1][i]!=NULL_IC) &&
                                   (BLCarryKit.cDeviceInf[1][i]==asBarCodeErrorSend ||
                                    BLCarryKit.cDeviceInf[1][i]==""))           //wei 20160318 Barcode Error依客戶設定
                                {
                                    iBarcodeReject++;
                                }
                            }

                            if(TestIF_File.bEnableConsecutiveFailure &&
                               iBarcodeReject>=TestIF_File.iConsecutiveFailure) //wei 20160823  Consecutive Failure
                            {
                                ShowErrorMessage("WAR0475", K_SKIP, MInShuttle2, false);
                                iBarcodeReject=0;
                            }
                        }
                    }
                    else
                    {
                        Task=2500;
                    }
                }
                bHasError=false;
            }
            else
            {
                AddCCDCommunicationLog(iBarCodeRowA, ">>Cycle 2000, Decode end.---------------\r\n");
                if(TestIF_File.bCheckCodeByShuttle==false &&
                   TestIF_File.bCheckCodeByLot==false)
                {
                    Task=5000;
                }
                else
                {
                    if(TestIF_File.bCheckLotHaveCode)                           //Steven 20170707 (wei) : Fixed 2DID for Korea
                        Task=3500;                                              //Alick 20170313 (wei) modify 4000==>3500
                    else
                        Task=4000;
                }
            }
            break;
        case 2500:
            if(MOT[MSht].MotorMove(Prod.InSHT[iSht].iLeft+1000))
            {
                Log.sprintf("Action 2500, Shuttle %d move to left", iSht+1);
                AddCCDCommunicationLog(iBarCodeRowA, Log);
                fLtcSensor->ClearLtcSensor(iSht);                               //Sam 20221101 : Latch 清除都要確認是否清清乾淨
                Task=2600;
            }
            break;
        case 2600:
            Log.sprintf("Alarm 2600, In Shuttle %d Barcode Error", iSht+1);
            AddCCDCommunicationLog(iBarCodeRowA, Log);
            if(TestIF_File.bCheckCodeByLot &&                                   //Alick 20170315 (wei) modify bRetryOffsetMove=>bCheckCodeByLot
               iRetryCount<=TestIF_File.iBarcodeRetryCount)                     //Alick 20170124 add Retry時bRetryOffsetMove=true先退出去
            {
                bHasError=false;
                iRetryCount++;
                Task=100;                                                       //jou 20170509 (Steven) : 2D alway retry
            }
            else
            {
                if(fContact->IsRun2DCheck()==true)                              //JerryYang 20250220 : 2DID硬體順序檢查功能
                {
                    ret=ShowErrorMessage("JAM0496", K_RETRY, MSht, false, sErrorPart);      //In Shuttle2 Barcode Error
                }
                else
                {
                    ret=ShowErrorMessage("JAM0461", K_RETRY|K_SKIP, MSht, false, sErrorPart);      //In Shuttle2 Barcode Error
                }

                iRetryCount=0;
                bHasError=false;

                if(ret==K_RETRY)
                {
                    if(TestIF_File.bRetryOffsetMove)                            //Alick 20170124 add Retry時bRetryOffsetMove=true先退出去
                        Task=3200;
                    else
                        Task=3000;
                }
                else
                {
                    AddCCDCommunicationLog(iBarCodeRowA, ">>Cycle 2600, Decode end with barcode error.---------------\r\n");

                    if(TestIF_File.bCheckCodeByShuttle==false &&
                       TestIF_File.bCheckCodeByLot==false)
                    {
                        Task=5000;
                    }
                    else
                    {
                        if(TestIF_File.bCheckLotHaveCode)                       //Steven 20170707 (wei) : Fixed 2DID for Korea
                            Task=3500;                                          //Alick 20170313 (wei) modify 4000==>3500
                        else
                            Task=4000;
                    }
                }
            }
            break;
        case 3000:                                                              //例外處理
            if(TestIF_File.bEnableMulti2D)
            {
                iNowCheckStep=InArmSuck.iShtCol-1;
                iNowMoveStepMulti2D=TestIF_File.iMulti2DCount;
                iNowMoveStepMulti2DY=TestIF_File.iMulti2DYItem;                 //遞減方式檢查, 故意+1, 因為到下一個Task會先--
                iNowMoveStepMulti2DX=TestIF_File.iMulti2DXItem-1;
            }
            else
            {
                iNowCheckStep=InArmSuck.iShtCol;                                //遞減方式檢查, 故意+1, 因為到下一個Task會先--
                iNowMoveStepMulti2D=0;
                iNowMoveStepMulti2DX=0;
                iNowMoveStepMulti2DY=0;
            }

            iCurrentStep[iBarCodeRowA]=iNowCheckStep;                           //Steven 20160503 : 確認移動位置與拍照位置相同
            iCurrentStep[iBarCodeRowB]=iNowCheckStep;
            for(int i=0; i<BLCarryKit.iMaxCol; i++)                             //全部重新掃描
            {
                if(BLCarryKit.Item[0][i]!=HAS_NULL_IC &&
                   BLCarryKit.Item[0][i]!=NULL_IC)
                {
                    mtBarcodeInSh->SetCellNumber(iBarCodeRowA, i+1, "");
                    bNeesScanCode[0][i]=true;
                }
                else
                {
                    bNeesScanCode[0][i]=false;
                }

                if(BLCarryKit.Item[1][i]!=HAS_NULL_IC &&
                   BLCarryKit.Item[1][i]!=NULL_IC)                              //Steven 20190904 : 0 --> 1
                {
                    mtBarcodeInSh->SetCellNumber(iBarCodeRowB, i+1, "");
                    bNeesScanCode[1][i]=true;
                }
                else
                {
                    bNeesScanCode[1][i]=false;
                }
            }
            Task=3100;
        case 3100:
            if(TestIF_File.bEnableMulti2D)                                      //Steven 20200810 : 一個IC使用多個2DID
            {
                iNowMoveStepMulti2D--;
                iNowMoveStepMulti2DY--;
                if(iNowMoveStepMulti2DY<0)
                {
                    iNowMoveStepMulti2DY=TestIF_File.iMulti2DYItem-1;
                    iNowMoveStepMulti2DX--;
                    if(iNowMoveStepMulti2DX<0)
                    {
                        iNowMoveStepMulti2DX=TestIF_File.iMulti2DXItem-1;
                        iNowMoveStepMulti2D=TestIF_File.iMulti2DCount-1;
                        iNowCheckStep--;
                    }
                }
            }
            else
            {
                iNowCheckStep--;
            }

            if(iNowCheckStep>=0)                                                //KenHsieh 20250610 : 避免因Delay 造成exposure position error
            {
                iCurrentStep[iBarCodeRowA]=iNowCheckStep;                       //Steven 20160503 : 確認移動位置與拍照位置相同
                iCurrentStep[iBarCodeRowB]=iNowCheckStep;
            }

            if(iNowCheckStep>=0 && iNowCheckStep<InArmSuck.iShtCol)
            {
//                if(bNeesScanCode[0][iNowCheckStep] ||
//                   bNeesScanCode[1][iNowCheckStep])                           //KenHsieh 20250610 : Mark, bNeesScanCode 為False 也要往下跑
//                {
                    Task=3200;
//                }
//                else
//                {
//                    Task=3100;
//                }
            }
            else
            {
                iRetryCount++;
                Task=1300;
            }
            break;
        case 3200:                                                              //移動馬達
            pos=GetMovePos(iSht, iNowCheckStep, iNowMoveStepMulti2DX);
            pos+=Offset.iSHLeft2D[iSht];

            if(MOT[MSht].MotorMove(pos))
            {
                Log.sprintf("Action 3200, Shuttle %d move to step %d", iSht, iNowCheckStep);
                AddCCDCommunicationLog(iBarCodeRowA, Log);
                BarcodePosDelay[iSht].SetMSAndOn(TestIF_File.iBarCodePosDelay); //KenHsieh 20250610 : 改為Start delay
                Task=3300;
            }
            break;
        case 3300:
            if(BarcodePosDelay[iSht].Off())
            {
                if(InArmSuck.iShtRow==2)                                        //IC在0的位置, 使用CCD4
                    bCCDBarcodeExposureOK[iBarCodeRowA]=false;
                else if(TestIF_File.bEnableMulti2D &&                           //Steven 20240612 : for 對角2D
                        (TestIF_File.iMulti2DType==e2x1In2CCD ||
                         TestIF_File.iMulti2DType==e2x2In2CCD))
                    bCCDBarcodeExposureOK[iBarCodeRowA]=false;
                else
                    bCCDBarcodeExposureOK[iBarCodeRowA]=true;
                bCCDBarcodeExposureOK[iBarCodeRowB]=false;
                sCCDCommand="SE0,";

                if(InArmSuck.iShtRow==2)                                        //IC在0的位置, 使用CCD4
                {
                    if(bNeesScanCode[0][iNowCheckStep]==false)
                    {
//                        bCCDBarcodeExposureOK[iBarCodeRowA]=true;             //KenHsieh 20250716 : Mark 避免到下一步直接通過，使得Step減1，導致exposure error
                        SendSE0(iBarCodeRowA, false, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3300: ");   //JerryYang 20201126 修正不拍照的位置沒有傳送指令給2DID
                    }
                    else
                    {
                        SendSE0(iBarCodeRowA, true, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3300: ");   //Steven 20200908 : 整合2DID SE0
                        BLCarryKit.cDeviceInf[0][iNowCheckStep]="";
                    }

                    if(bNeesScanCode[1][iNowCheckStep]==false)
                    {
//                        bCCDBarcodeExposureOK[iBarCodeRowB]=true;             //KenHsieh 20250716 : Mark 避免到下一步直接通過，使得Step減1，導致exposure error
                        SendSE0(iBarCodeRowB, false, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3300: ");   //JerryYang 20201126 修正不拍照的位置沒有傳送指令給2DID
                    }
                    else
                    {
                        SendSE0(iBarCodeRowB, true, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3300: ");   //Steven 20200908 : 整合2DID SE0
                        BLCarryKit.cDeviceInf[1][iNowCheckStep]="";
                    }
                }
                else if(TestIF_File.bEnableMulti2D &&                           //Steven 20240612 : for 對角2D
                        (TestIF_File.iMulti2DType==e2x1In2CCD ||
                         TestIF_File.iMulti2DType==e2x2In2CCD))
                {
                    if(bNeesScanCode[0][iNowCheckStep]==false)
                    {
//                        bCCDBarcodeExposureOK[iBarCodeRowA]=true;             //KenHsieh 20250716 : Mark 避免到下一步直接通過，使得Step減1，導致exposure error
//                        bCCDBarcodeExposureOK[iBarCodeRowB]=true;             //KenHsieh 20250716 : Mark 避免到下一步直接通過，使得Step減1，導致exposure error
                        SendSE0(iBarCodeRowA, false, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3300: ");   //JerryYang 20201126 修正不拍照的位置沒有傳送指令給2DID
                        SendSE0(iBarCodeRowB, false, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3300: ");   //JerryYang 20201126 修正不拍照的位置沒有傳送指令給2DID
                    }
                    else
                    {
                        bCCDBarcodeExposureOK[iBarCodeRowA]=false;
                        bCCDBarcodeExposureOK[iBarCodeRowB]=false;
                        BLCarryKit.cDeviceInf[0][iNowCheckStep]="";

                        if(TestIF_File.iMulti2DMap[0][iNowMoveStepMulti2D]>0)
                            SendSE0(iBarCodeRowA, true, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3300: ");   //Steven 20200908 : 整合2DID SE0
                        else
                            SendSE0(iBarCodeRowA, false, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3300: ");   //Steven 20200908 : 整合2DID SE0

                        if(TestIF_File.iMulti2DMap[1][iNowMoveStepMulti2D]>0)
                            SendSE0(iBarCodeRowB, true, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3300: ");   //Steven 20200908 : 整合2DID SE0
                        else
                            SendSE0(iBarCodeRowB, false, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3300: ");   //Steven 20200908 : 整合2DID SE0
                    }
                }
                else
                {
                    if(bNeesScanCode[0][iNowCheckStep]==false)
                    {
//                        bCCDBarcodeExposureOK[iBarCodeRowB]=true;             //KenHsieh 20250716 : Mark 避免到下一步直接通過，使得Step減1，導致exposure error
                        SendSE0(iBarCodeRowB, false, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3300: ");   //JerryYang 20201126 修正不拍照的位置沒有傳送指令給2DID
                    }
                    else
                    {
                        SendSE0(iBarCodeRowB, true, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3300: ");   //Steven 20200908 : 整合2DID SE0
                        BLCarryKit.cDeviceInf[0][iNowCheckStep]="";
                    }
                }
                iRetryCountExpTimo=0;                                           //Steven 20160425 : 修改拍照Time Out
                BarcodeExposureDelay[iSht].SetMSAndOn(TestIF_File.i2DTriggerTime);
                Task=3400;
            }
            break;
        case 3400:                                                              //拍照讀取2D Code
            if(bStepError[iBarCodeRowA]==true ||
               bStepError[iBarCodeRowB]==true)                                  //Steven 20160503 : 確認移動位置與拍照位置相同
            {
                ErrPart="";
                if(bStepError[iBarCodeRowA])
                    ErrPart+=IndexSuckName[0][iNowCheckStep];                   //Frank 20171011 (Steven) iNowMoveStep-->iNowCheckStep

                if(bStepError[iBarCodeRowB])
                    ErrPart+=IndexSuckName[1][iNowCheckStep];                   //Frank 20171011 (Steven) iNowMoveStep-->iNowCheckStep

                Task=1180;
            }
            else if(bCCDBarcodeExposureOK[iBarCodeRowA]==true &&
                    bCCDBarcodeExposureOK[iBarCodeRowB]==true)                  //拍照完成
            {
                if(bNeesScanCode[0][iNowCheckStep]==true)
                    mtBarcodeInSh->SetCellNumber(iBarCodeRowA, iNowCheckStep+1, "Waiting result..");

                if(InArmSuck.iShtRow==2)
                {
                    if(bNeesScanCode[1][iNowCheckStep]==true)
                        mtBarcodeInSh->SetCellNumber(iBarCodeRowB, iNowCheckStep+1, "Waiting result..");
                }
                else if(TestIF_File.bEnableMulti2D &&                           //Steven 20240612 : for 對角2D
                        (TestIF_File.iMulti2DType==e2x1In2CCD ||
                         TestIF_File.iMulti2DType==e2x2In2CCD))
                {
                    if(bNeesScanCode[0][iNowCheckStep]==true)
                        mtBarcodeInSh->SetCellNumber(iBarCodeRowB, iNowCheckStep+1, "Waiting result..");
                }
                Task=3100;
            }
            else if(BarcodeExposureDelay[iSht].Off())                           //拍照time out
            {
                if(InArmSuck.iShtRow==2)
                {
                    if(bCCDBarcodeExposureOK[iBarCodeRowA]==false)
                    {
                        mtBarcodeInSh->SetCellNumber(iBarCodeRowA, iNowCheckStep+1, "Exposure time out");
//                        bCCDBarcodeExposureOK[iBarCodeRowA]=true;             //KenHsieh 20250716 : Mark 需等待回復
                        Log.sprintf("Action 3400, Exposure time out. Retry : %d", iRetryCountExpTimo);
                        AddCCDCommunicationLog(iBarCodeRowA, Log);
                    }
                    else
                    {
                        if(bNeesScanCode[0][iNowCheckStep]==true)
                            mtBarcodeInSh->SetCellNumber(iBarCodeRowA, iNowCheckStep+1, "Waiting result..");
                    }

                    if(bCCDBarcodeExposureOK[iBarCodeRowB]==false)
                    {
                        mtBarcodeInSh->SetCellNumber(iBarCodeRowB, iNowCheckStep+1, "Exposure time out");
//                        bCCDBarcodeExposureOK[iBarCodeRowB]=true;             //KenHsieh 20250716 : Mark 需等待回復
                        Log.sprintf("Action 3400, Exposure time out. Retry : %d", iRetryCountExpTimo);
                        AddCCDCommunicationLog(iBarCodeRowB, Log);
                    }
                    else
                    {
                        if(bNeesScanCode[1][iNowCheckStep]==true)
                            mtBarcodeInSh->SetCellNumber(iBarCodeRowB, iNowCheckStep+1, "Waiting result..");
                    }
                }
                else if(TestIF_File.bEnableMulti2D &&                           //Steven 20240612 : for 對角2D
                        (TestIF_File.iMulti2DType==e2x1In2CCD ||
                         TestIF_File.iMulti2DType==e2x2In2CCD))
                {
                    if(bCCDBarcodeExposureOK[iBarCodeRowA]==false)
                    {
                        mtBarcodeInSh->SetCellNumber(iBarCodeRowA, iNowCheckStep+1, "Exposure time out");
//                        bCCDBarcodeExposureOK[iBarCodeRowA]=true;             //KenHsieh 20250716 : Mark 需等待回復
                        Log.sprintf("Action 3400, Exposure time out. Retry : %d", iRetryCountExpTimo);
                        AddCCDCommunicationLog(iBarCodeRowA, Log);
                    }
                    else
                    {
                        if(bNeesScanCode[0][iNowCheckStep]==true)
                            mtBarcodeInSh->SetCellNumber(iBarCodeRowA, iNowCheckStep+1, "Waiting result..");
                    }

                    if(bCCDBarcodeExposureOK[iBarCodeRowB]==false)
                    {
                        mtBarcodeInSh->SetCellNumber(iBarCodeRowB, iNowCheckStep+1, "Exposure time out");
//                        bCCDBarcodeExposureOK[iBarCodeRowB]=true;             //KenHsieh 20250716 : Mark 需等待回復
                        Log.sprintf("Action 3400, Exposure time out. Retry : %d", iRetryCountExpTimo);
                        AddCCDCommunicationLog(iBarCodeRowB, Log);
                    }
                    else
                    {
                        if(bNeesScanCode[0][iNowCheckStep]==true)
                            mtBarcodeInSh->SetCellNumber(iBarCodeRowB, iNowCheckStep+1, "Waiting result..");
                    }
                }
                else                                                            //IC在0的位置, 使用CCD4
                {
                    if(bCCDBarcodeExposureOK[iBarCodeRowB]==false)
                    {
                        mtBarcodeInSh->SetCellNumber(iBarCodeRowA, iNowCheckStep+1, "Exposure time out");
                        bCCDBarcodeExposureOK[iBarCodeRowB]=true;
                        Log.sprintf("Action 3400, Exposure time out. Retry : %d", iRetryCountExpTimo);
                        AddCCDCommunicationLog(iBarCodeRowB, Log);
                    }
                    else
                    {
                        if(bNeesScanCode[0][iNowCheckStep]==true)
                            mtBarcodeInSh->SetCellNumber(iBarCodeRowA, iNowCheckStep+1, "Waiting result..");
                    }
                }

                iRetryCountExpTimo++;                                           //Steven 20160425 : 修改拍照Time Out
                Task=3460;
            }
            break;
        case 3460:
            if(iRetryCountExpTimo<3)
            {
                Task=3470;
            }
            else
            {
                ret=ShowErrorMessage("WAR0464", K_RETRY|K_SKIP, MSht);          //2D function CCD 3 or 4 Exposure time out!
                if(ret==K_SKIP)
                {
                    Task=3100;
                }
                else
                {
                    Task=3470;
                }
                iRetryCountExpTimo=0;
            }
            break;
        case 3470:
            if(InArmSuck.iShtRow==2)
            {
                if(bCCDBarcodeExposureOK[iBarCodeRowA]==false)
                {
                    bNeesScanCode[0][iNowCheckStep]=true;                       //KenHsieh 20260114 : iNowMoveStep -> iNowCheckStep 修正取錯變數導致死當問題;
                    SendSE0(iBarCodeRowA, true, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3470: ");   //Steven 20200908 : 整合2DID SE0
                }

                if(bCCDBarcodeExposureOK[iBarCodeRowB]==false)
                {
                    bNeesScanCode[1][iNowCheckStep]=true;                       //KenHsieh 20260114 : iNowMoveStep -> iNowCheckStep 修正取錯變數導致死當問題
                    SendSE0(iBarCodeRowB, true, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3470: ");   //Steven 20200908 : 整合2DID SE0
                }
            }
            else if(TestIF_File.bEnableMulti2D &&                               //Steven 20240612 : for 對角2D
                    (TestIF_File.iMulti2DType==e2x1In2CCD ||
                     TestIF_File.iMulti2DType==e2x2In2CCD))
            {
                if(bCCDBarcodeExposureOK[iBarCodeRowA]==false)
                {
                    bNeesScanCode[0][iNowCheckStep]=true;                       //KenHsieh 20260114 : iNowMoveStep -> iNowCheckStep 修正取錯變數導致死當問題
                    if(TestIF_File.iMulti2DMap[0][iNowMoveStepMulti2D]>0)
                        SendSE0(iBarCodeRowA, true, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3470: ");   //Steven 20200908 : 整合2DID SE0
                    else
                        SendSE0(iBarCodeRowA, false, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3470: ");
                }

                if(bCCDBarcodeExposureOK[iBarCodeRowB]==false)
                {
                    bNeesScanCode[0][iNowCheckStep]=true;                       //KenHsieh 20260114 : iNowMoveStep -> iNowCheckStep 修正取錯變數導致死當問題
                    if(TestIF_File.iMulti2DMap[1][iNowMoveStepMulti2D]>0)
                        SendSE0(iBarCodeRowB, true, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3470: ");   //Steven 20200908 : 整合2DID SE0
                    else
                        SendSE0(iBarCodeRowB, false, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3470: ");   //Steven 20200908 : 整合2DID SE0
                }
            }
            else
            {
                if(bCCDBarcodeExposureOK[iBarCodeRowB]==false)
                {
                    bNeesScanCode[0][iNowCheckStep]=true;                       //KenHsieh 20260114 : iNowMoveStep -> iNowCheckStep 修正取錯變數導致死當問題
                    SendSE0(iBarCodeRowB, true, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3470: ");   //Steven 20200908 : 整合2DID SE0
                }
            }
            Task=3400;
            BarcodeExposureDelay[iSht].SetMSAndOn(TestIF_File.i2DTriggerTime);
            break;
        case 3500:
            if(TestIF_File.bCheckLotHaveCode)                                   //Alick 20170321 (wei) Check Lot Have Code
            {
                if(CosFunction.b2DCodeCheckByCoustomerLot)                      //Sam 20220223 : 2D Code Check by Coustomer Lot
                {
                    bLotCheckError=false;
                    Str1=fLotInfo->edCustomerLotId->Text;
                    iPos=Str1.Pos("\n");
                    if(iPos>0)
                        Str3=Str1.SubString(1, iPos-1);
                    else
                        Str3=Str1;

                    for(int i=0; i<InArmSuck.iShtCol; i++)
                    {
                        if(BLCarryKit.Item[0][i]!=HAS_NULL_IC &&
                           BLCarryKit.Item[0][i]!=NULL_IC &&
                           BLCarryKit.cDeviceInf[0][i]!=asBarCodeErrorSend &&
                           BLCarryKit.cDeviceInf[0][i]!="")
                        {
                            Str=BLCarryKit.cDeviceInf[0][i];
                            if(Str3.Length()>8)                                 //CustomerLotId 最多只取8碼來比對
                            {
                                Str2=Str.SubString(1, 8);
                                Str3=Str3.SubString(1, 8);
                            }
                            else
                            {
                                Str2=Str.SubString(1, Str3.Length());
                            }

                            if(Str2!=Str3)
                            {
                                bLotCheckError=true;
                                bHasErr[0][i]=true;
                            }
                        }

                        if(InArmSuck.iShtRow==2)
                        {
                            if(BLCarryKit.Item[1][i]!=HAS_NULL_IC &&
                               BLCarryKit.Item[1][i]!=NULL_IC &&
                               BLCarryKit.cDeviceInf[1][i]!=asBarCodeErrorSend &&
                               BLCarryKit.cDeviceInf[1][i]!="")
                            {
                                Str=BLCarryKit.cDeviceInf[1][i];
                                Str2=Str.SubString(1, Str3.Length());

                                if(Str2!=Str3)
                                {
                                    bLotCheckError=true;
                                    bHasErr[1][i]=true;
                                }
                            }
                        }
                    }

                    if(bLotCheckError)
                        Task=3600;
                    else
                        Task=4000;
                }
                else
                {
                    FILE *P=NULL;
                    char t[256];
                    ZeroMemory(bHasErr, sizeof(bHasErr));
                    bLotCheckError=false;
                    if(FileExists(asBarCodeDownLot)!=false)
                    {
                        if(LastSet.iTester==OFF_LINE)                           //Frank 20170611 (Steven) add Alex 多種Dummy IC 需要同時辨識
                        {
                            fBarCode->list2DByLot->Clear();
                            fBarCode->list2DByLot->LoadFromFile(asBarCodeDownLot);
                            fBarCode->list2DByLot->Sort();
                        }
                        else
                        {
                            P=fopen(asBarCodeDownLot.c_str(),"r");
                            if(P!=NULL)
                            {
                                fgets(t, 30, P);
                                Str1=t;
                                asBarcodeLotNumber="     Lot ID : "+Str1;       //wei Barcode Lot Alarm時，顯示出Lot
                                fclose(P);
                            }

                            iPos=Str1.Pos("\n");                                //Frank 20170426 (Steven) add 消除"\n"為一個字元的寬度
                            if(iPos>0)
                                Str3=Str1.SubString(1, iPos-1);
                            else
                                Str3=Str1;
                        }

                        for(int i=0; i<InArmSuck.iShtCol; i++)
                        {
                            if(BLCarryKit.Item[0][i]!=HAS_NULL_IC &&
                               BLCarryKit.Item[0][i]!=NULL_IC &&
                               BLCarryKit.cDeviceInf[0][i]!=asBarCodeErrorSend &&
                               BLCarryKit.cDeviceInf[0][i]!="" &&
                               BLCarryKit.cDeviceInf[0][i]!=asBarCodeErrorCheckSum)     //KaiHuang 20200826 : Add CheckSum
                            {
                                Str=BLCarryKit.cDeviceInf[0][i];
                                Str2=Str.SubString(1, 9);

                                if(!fBarCode->list2DByLot->Find(Str2, iIndex) && LastSet.iTester==OFF_LINE)     //Frank 20170611 (Steven) add Alex 多種Dummy IC 需要同時辨識
                                {
                                    bLotCheckError=true;
                                    bHasErr[0][i]=true;
                                }
                                else if(Str2!=Str3)
                                {
                                    bLotCheckError=true;
                                    bHasErr[0][i]=true;
                                }
                            }

                            if(InArmSuck.iShtRow==2)
                            {
                                if(BLCarryKit.Item[1][i]!=HAS_NULL_IC &&
                                   BLCarryKit.Item[1][i]!=NULL_IC &&
                                   BLCarryKit.cDeviceInf[1][i]!=asBarCodeErrorSend &&
                                   BLCarryKit.cDeviceInf[1][i]!="" &&
                                   BLCarryKit.cDeviceInf[1][i]!=asBarCodeErrorCheckSum)     //KaiHuang 20200826 : Add CheckSum
                                {
                                    Str=BLCarryKit.cDeviceInf[1][i];
                                    Str2=Str.SubString(1, 9);

                                    if(!fBarCode->list2DByLot->Find(Str2, iIndex) &&
                                       LastSet.iTester==OFF_LINE)               //Frank 20170611 add Alex 多種Dummy IC 需要同時辨識
                                    {
                                        bLotCheckError=true;
                                        bHasErr[1][i]=true;
                                    }
                                    else if(Str2!=Str3)
                                    {
                                        bLotCheckError=true;
                                        bHasErr[1][i]=true;
                                    }
                                }
                            }
                        }

                        if(bLotCheckError)
                        {
                            Task=3600;
                        }
                        else
                        {
                            Task=4000;
                        }
                    }
                    else
                    {
                        ShowErrorMessage("WAR0482", K_RETRY, MMSystem);
                    }
                }
            }
            else
            {
                Task=4000;
            }
            break;
        case 3600:
            if(MOT[MSht].MotorMove(Prod.InSHT[iSht].iLeft))                     //Steven 20160503 : 先移出來才Alarm
            {
                fNote->t2DCode->Visible=true;
                fNote->t2DCode->XItem=InArmSuck.iShtRow;
                fNote->t2DCode->YItem=InArmSuck.iShtCol;
                fLtcSensor->ClearLtcSensor(iSht);                               //Sam 20221101 : Latch 清除都要確認是否清清乾淨
                for(int i=0; i<InArmSuck.iShtRow; i++)
                {
                    for(int j=0; j<InArmSuck.iShtCol; j++)
                    {
                        fNote->t2DCode->SetCellNumber(i, j, BLCarryKit.cDeviceInf[i][j].c_str());   //RogerYang 20180605 (Steven) : 修正FLCarryKit->BLCarryKit
                        if(bHasErr[i][j]==true)
                            fNote->t2DCode->SetCellColorIndex(i, j, 3);
                        else
                            fNote->t2DCode->SetCellColorIndex(i, j, 0);
                    }
                }

                Log.sprintf("WAR0472, In shuttle %d check have Lot barcode error!\r\n", iSht+1);
                AddCCDCommunicationLog(iBarCodeRowA, Log);
                if(bLotCheckError==true)
                {
                    ret=ShowErrorMessage("WAR0472", K_RETRY, MInShuttle1, false);      //In Shuttle1 Barcode Error  //Alick 20170202 remove skip
                }

                if(ret==K_RETRY)
                {
                    Task=1;
                }
            }
            break;
        case 4000:                                                              //Steven 20160428 : 檢查蝦頭的2D重複碼
            if(TestIF_File.bCheckCodeByShuttle || TestIF_File.bCheckCodeByLot)  //Steven 20171128 (Wei) : check lot之前一定要先確認蝦頭內沒有重複的
            {
                map2DShuttleList[iBarCodeRowA].clear();
                map2DShuttleList[iBarCodeRowB].clear();
                ZeroMemory(bHasErr, sizeof(bHasErr));
                bShtHasDupError=false;
                ErrPart="";
                for(int i=0; i<InArmSuck.iShtCol; i++)
                {
                    if(BLCarryKit.Item[0][i]!=HAS_NULL_IC &&
                       BLCarryKit.Item[0][i]!=NULL_IC &&
                       BLCarryKit.cDeviceInf[0][i]!=asBarCodeErrorSend &&
                       BLCarryKit.cDeviceInf[0][i]!="" &&
                       BLCarryKit.cDeviceInf[0][i]!=asBarCodeErrorCheckSum)     //KaiHuang 20200826 : Add CheckSum
                    {
                        mapShtIter[iBarCodeRowA]=map2DShuttleList[iBarCodeRowA].find(BLCarryKit.cDeviceInf[0][i]);
                        if(mapShtIter[iBarCodeRowA]==map2DShuttleList[iBarCodeRowA].end())
                        {
                            map2DShuttleList[iBarCodeRowA][BLCarryKit.cDeviceInf[0][i]]=i;
                        }
                        else                                                    //有重複的就不用加進去了
                        {
                            iBarcodeDuplicate[iBarCodeRowA]++;
                            bShtHasDupError=true;
                            ErrPart+=IndexSuckName[0][i];
                            bHasErr[0][i]=true;
                            ret=atoi(map2DShuttleList[iBarCodeRowA][BLCarryKit.cDeviceInf[0][i]].c_str());   //Steven 20160503 : 重複的Code要設定為Error
                            ErrPart+=IndexSuckName[0][ret];
                            bHasErr[0][ret]=true;
                        }
                    }

                    if(InArmSuck.iShtRow==2)
                    {
                        if(BLCarryKit.Item[1][i]!=HAS_NULL_IC &&
                           BLCarryKit.Item[1][i]!=NULL_IC &&
                           BLCarryKit.cDeviceInf[1][i]!=asBarCodeErrorSend &&
                           BLCarryKit.cDeviceInf[1][i]!="" &&
                           BLCarryKit.cDeviceInf[1][i]!=asBarCodeErrorCheckSum) //KaiHuang 20200826 : Add CheckSum
                        {
                            mapShtIter[iBarCodeRowB]=map2DShuttleList[iBarCodeRowB].find(BLCarryKit.cDeviceInf[1][i]);
                            if(mapShtIter[iBarCodeRowB]==map2DShuttleList[iBarCodeRowB].end())
                            {
                                map2DShuttleList[iBarCodeRowB][BLCarryKit.cDeviceInf[1][i]]=i;
                            }
                            else
                            {
                                iBarcodeDuplicate[iBarCodeRowB]++;
                                bShtHasDupError=true;
                                ErrPart+=IndexSuckName[1][i];
                                bHasErr[1][i]=true;
                                ret=atoi(map2DShuttleList[iBarCodeRowB][BLCarryKit.cDeviceInf[1][i]].c_str());   //Steven 20160503 : 重複的Code要設定為Error
                                ErrPart+=IndexSuckName[1][ret];
                                bHasErr[1][ret]=true;
                            }
                        }
                    }
                }

                if(bShtHasDupError)
                {
                    Task=4050;
                }
                else
                {
                    Task=4100;
                }
            }
            else
            {
                Task=4100;
            }
            break;
        case 4050:
            if(MOT[MSht].MotorMove(Prod.InSHT[iSht].iLeft))                     //Steven 20160503 : 先移出來才Alarm
            {
                fNote->t2DCode->Visible=true;
                fNote->t2DCode->XItem=InArmSuck.iShtRow;
                fNote->t2DCode->YItem=InArmSuck.iShtCol;
                fLtcSensor->ClearLtcSensor(iSht);                               //Sam 20221101 : Latch 清除都要確認是否清清乾淨
                for(int i=0; i<InArmSuck.iShtRow; i++)
                {
                    for(int j=0; j<InArmSuck.iShtCol; j++)
                    {
                        fNote->t2DCode->SetCellNumber(i, j, BLCarryKit.cDeviceInf[i][j].c_str());
                        if(bHasErr[i][j]==true)
                            fNote->t2DCode->SetCellColorIndex(i, j, 3);
                        else
                            fNote->t2DCode->SetCellColorIndex(i, j, 0);
                    }
                }

                Log.sprintf("Alarm 4050, In shuttle %d check have duplicate barcode error!\r\n", iSht+1);
                AddCCDCommunicationLog(iBarCodeRowA, Log);
                if(TestIF_File.iShtDuplicateRetryCnt<=0 ||                              //Steven 20190520 : 修正等於0時會變成無窮迴圈
                   TestIF_File.iShtDuplicateRetryCnt>=iShtDupilcateFailCnt[iSht])       //Steven 20160823 : 蝦頭重複碼要可以自動Retry
                {
                    iShtDupilcateFailCnt[iSht]=0;
                    ret=ShowErrorMessage("WAR0466", K_RETRY|K_SKIP, MSht, false, ErrPart);  //In shuttle 2 check have duplicate barcode error!
                }
                else
                {
                    iShtDupilcateFailCnt[iSht]++;
                    ret=K_RETRY;
                }

                if(ret==K_SKIP)
                {
                    for(int i=0; i<InArmSuck.iShtRow; i++)
                    {
                        for(int j=0; j<InArmSuck.iShtCol; j++)
                        {
                            if(SPIL_FOR_QLE==1 &&
                               Prod.bF18InshuttleDetect==true)                  //JerryYang 20241009 : 渠梁陳勇要求重複2DID ALARM要求人員手動取出DEVICE
                            {
                                if(InArmSuck.iShtRow==2)
                                {
                                    if(bHasErr[0][j]==true || bHasErr[1][j]==true)
                                    {
                                        BLCarryKit.SetItemData(i, j, HAS_NULL_IC);
                                        BLCarryKit.PordRec[i][j].AddTestResultRecord(iTestBinCount, "2DID_Duplicate");
                                        BLCarryKit.cDeviceInf[i][j]=asBarCodeErrorSend;
                                    }
                                }
                                else
                                {
                                    if(bHasErr[i][j]==true)                     //Steven 20160503 : 重複的Code要設定為Error
                                    {
                                        BLCarryKit.SetItemData(i, j, HAS_NULL_IC);
                                        BLCarryKit.PordRec[i][j].AddTestResultRecord(iTestBinCount, "2DID_Duplicate");
                                        BLCarryKit.cDeviceInf[i][j]=asBarCodeErrorSend;
                                    }
                                }
                            }
                            else
                            {
                                if(bHasErr[i][j]==true)                         //Steven 20160503 : 重複的Code要設定為Error
                                {
                                    BLCarryKit.cDeviceInf[i][j]=asBarCodeErrorSend;
                                    BLCarryKit.PordRec[i][j].AddErrorRecordNoSave("Duplicate 2DID error");  //JerryYang 20230322 : Lot summary要計算各類型ERR的數量
                                }
                            }
                        }
                    }

                    if(SPIL_FOR_QLE==1 && Prod.bF18InshuttleDetect==true)
                    {
                        if(InArmSuck.iShtRow==2)
                        {
                            ShowMyMessage("請確認已將Shuttle2 Duplicate的Device同一行的都取出!!");
                        }
                        else
                        {
                            ShowMyMessage("請確認已將Shuttle2 Duplicate的device取出!!");
                        }
                    }

                    Task=4100;
                }
                else
                {
                    Task=1;
                }
            }
            break;
        case 4100:
            iShtDupilcateFailCnt[iSht]=0;
            if(fContact->fShow==true &&
               CUSTOMER_CODE==CC_ASE_CL &&
               (iContactMode==CONTACT_AUTO_GET_HEIGHT || iContactMode==CONTACT_TEST))
            {
                Task=4200;
            }
            else if(TestIF_File.bCheckCodeByLot &&
               fContact->IsRun2DCheck()==false)                                 //JerryYang 20250220 : 2DID硬體順序檢查功能
            {
                bLotHasDupError=false;
                ZeroMemory(bHasErr, sizeof(bHasErr));
                ErrPart="";
                for(int i=0; i<InArmSuck.iShtCol; i++)
                {
                    if(BLCarryKit.Item[0][i]!=HAS_NULL_IC &&
                       BLCarryKit.Item[0][i]!=NULL_IC &&
                       BLCarryKit.cDeviceInf[0][i]!=asBarCodeErrorSend &&
                       BLCarryKit.cDeviceInf[0][i]!="" &&
                       BLCarryKit.cDeviceInf[0][i]!=asBarCodeErrorCheckSum)     //KaiHuang 20200826 : Add CheckSum
                    {
                        map2DIter=map2DList.find(BLCarryKit.cDeviceInf[0][i]);
                        if(map2DList.size()!=0 && map2DIter!=map2DList.end())
                        {
                            iBarcodeDuplicate[iBarCodeRowA]++;
                            bLotHasDupError=true;
                            ErrPart+=IndexSuckName[0][i];
                            bHasErr[0][i]=true;
                        }
                    }

                    if(InArmSuck.iShtRow==2)
                    {
                        if(BLCarryKit.Item[1][i]!=HAS_NULL_IC &&
                           BLCarryKit.Item[1][i]!=NULL_IC &&
                           BLCarryKit.cDeviceInf[1][i]!=asBarCodeErrorSend &&
                           BLCarryKit.cDeviceInf[1][i]!="" &&
                           BLCarryKit.cDeviceInf[1][i]!=asBarCodeErrorCheckSum)     //KaiHuang 20200826 : Add CheckSum
                        {
                            map2DIter=map2DList.find(BLCarryKit.cDeviceInf[1][i]);
                            if(map2DList.size()!=0 && map2DIter!=map2DList.end())
                            {
                                iBarcodeDuplicate[iBarCodeRowB]++;
                                bLotHasDupError=true;
                                ErrPart+=IndexSuckName[1][i];
                                bHasErr[1][i]=true;
                            }
                        }
                    }
                }

                if(bLotHasDupError)
                {
                    Task=4150;
                }
                else
                {
                    Task=4200;
                }
            }
            else
            {
                Task=4200;
            }
            break;
        case 4150:
            if(MOT[MSht].MotorMove(Prod.InSHT[iSht].iLeft))                     //Steven 20160503 : 先移出來才Alarm
            {
                fNote->t2DCode->Visible=true;
                fNote->t2DCode->XItem=InArmSuck.iShtRow;
                fNote->t2DCode->YItem=InArmSuck.iShtCol;
                fLtcSensor->ClearLtcSensor(iSht);                               //Sam 20221101 : Latch 清除都要確認是否清清乾淨
                for(int i=0; i<InArmSuck.iShtRow; i++)
                {
                    for(int j=0; j<InArmSuck.iShtCol; j++)
                    {
                        fNote->t2DCode->SetCellNumber(i, j, BLCarryKit.cDeviceInf[i][j].c_str());
                        if(bHasErr[i][j]==true)
                            fNote->t2DCode->SetCellColorIndex(i, j, 3);
                        else
                            fNote->t2DCode->SetCellColorIndex(i, j, 0);
                    }
                }

                Log.sprintf("Alarm 4150, In shuttle %d check have duplicate barcode in lot error!\r\n", iSht+1);
                AddCCDCommunicationLog(iBarCodeRowA, Log);
                ret=ShowErrorMessage("WAR0468", K_RETRY|K_SKIP, MSht, false, ErrPart);  //In shuttle 2 check have duplicate barcode in lot error!
                if(ret==K_SKIP)
                {
                    for(int i=0; i<InArmSuck.iShtRow; i++)
                    {
                        for(int j=0; j<InArmSuck.iShtCol; j++)
                        {
                            if(SPIL_FOR_QLE==1 &&
                               Prod.bF18InshuttleDetect==true)                  //JerryYang 20241009 : 渠梁陳勇要求重複2DID ALARM要求人員手動取出DEVICE
                            {
                                if(InArmSuck.iShtRow==2)
                                {
                                    if(bHasErr[0][j]==true || bHasErr[1][j]==true)
                                    {
                                        BLCarryKit.SetItemData(i, j, HAS_NULL_IC);
                                        BLCarryKit.PordRec[i][j].AddTestResultRecord(iTestBinCount, "2DID_Duplicate");
                                        BLCarryKit.cDeviceInf[i][j]=asBarCodeErrorSend;
                                    }
                                }
                                else
                                {
                                    if(bHasErr[i][j]==true)                     //Steven 20160503 : 重複的Code要設定為Error
                                    {
                                        BLCarryKit.SetItemData(i, j, HAS_NULL_IC);
                                        BLCarryKit.PordRec[i][j].AddTestResultRecord(iTestBinCount, "2DID_Duplicate");
                                        BLCarryKit.cDeviceInf[i][j]=asBarCodeErrorSend;
                                    }
                                }
                            }
                            else
                            {
                                if(bHasErr[i][j]==true)                         //Steven 20160503 : 重複的Code要設定為Error
                                {
                                    BLCarryKit.cDeviceInf[i][j]=asBarCodeErrorSend;
                                }
                            }
                        }
                    }

                    if(SPIL_FOR_QLE==1 && Prod.bF18InshuttleDetect==true)
                    {
                        if(InArmSuck.iShtRow==2)
                        {
                            ShowMyMessage("請確認已將Shuttle1 2DID重複的Device同一行的都取出!!");
                        }
                        else
                        {
                            ShowMyMessage("請確認已將Shuttle1 2DID重複的Device取出!!");
                        }
                    }
                    Task=4200;
                }
                else
                {
                    Task=1;
                }
            }
            break;
        case 4200:
            if(fContact->fShow==true &&
               CUSTOMER_CODE==CC_ASE_CL &&
               (iContactMode==CONTACT_AUTO_GET_HEIGHT || iContactMode==CONTACT_TEST))
            {
            }
            else if(TestIF_File.bCheckCodeByLot &&                                   //jou 20191008 : Barcode duplicate file by out arm
               CosFunction.bBarcodeDuplicateFileByOutArm==false &&              //JerryYang 20250220 : 2DID硬體順序檢查功能
               fContact->IsRun2DCheck()==false)                                 //把新的值加到List裡面
            {
                list2DByLot->Clear();
                for(int i=0; i<InArmSuck.iShtCol; i++)
                {
                    if(BLCarryKit.Item[0][i]!=HAS_NULL_IC &&
                       BLCarryKit.Item[0][i]!=NULL_IC &&
                       BLCarryKit.cDeviceInf[0][i]!=asBarCodeErrorSend &&
                       BLCarryKit.cDeviceInf[0][i]!="" &&
                       BLCarryKit.cDeviceInf[0][i]!=asBarCodeErrorCheckSum)     //KaiHuang 20200826 : Add CheckSum
                    {
                        map2DList[BLCarryKit.cDeviceInf[0][i]]=i;
                        list2DByLot->Add(BLCarryKit.cDeviceInf[0][i]);
                    }

                    if(InArmSuck.iShtRow==2)
                    {
                        if(BLCarryKit.Item[1][i]!=HAS_NULL_IC &&
                           BLCarryKit.Item[1][i]!=NULL_IC &&
                           BLCarryKit.cDeviceInf[1][i]!=asBarCodeErrorSend &&
                           BLCarryKit.cDeviceInf[1][i]!="" &&
                           BLCarryKit.cDeviceInf[1][i]!=asBarCodeErrorCheckSum) //KaiHuang 20200826 : Add CheckSum
                        {
                            map2DList[BLCarryKit.cDeviceInf[1][i]]=i;           //KaiChen 20191002 ：修正 2D Map 資料異常
                            list2DByLot->Add(BLCarryKit.cDeviceInf[1][i]);
                        }
                    }
                }
                WriteDataToFile(asBarCodeLot, list2DByLot->Text);               //Steven 20160429 : 開程式要把2D List讀回來
            }

            if(TestIF_File.bEnableBarCode==true &&
               TestIF_File.b2DIDAllowList==true &&
               ((fLotInfo->cbRunMode->Text!="CORR" && IniConfig.iN23DownloadMethod!=2) ||
                IniConfig.iN23DownloadMethod==2) &&                             //JerryYang 20250320 : 2DID白名單功能
               fContact->IsRun2DCheck()==false)                                 //JerryYang 20250220 : 2DID硬體順序檢查功能
            {
                ZeroMemory(bHasErr, sizeof(bHasErr));
                bAlarm=false;
                ErrPart="";

                for(int i=0; i<InArmSuck.iShtCol; i++)
                {
                    if(bNeedCheckWhitleList==true ||
                       IniConfig.iN23DownloadMethod==2)                         //JerryYang 20250320 : 2DID白名單功能
                    {
                        if(BLCarryKit.Item[0][i]!=HAS_NULL_IC && BLCarryKit.Item[0][i]!=NULL_IC &&
                           BLCarryKit.cDeviceInf[0][i]!=asBarCodeErrorSend && BLCarryKit.cDeviceInf[0][i]!="" &&
                           BLCarryKit.cDeviceInf[0][i]!=asBarCodeErrorCheckSum) //KaiHuang 20200826 : Add CheckSum
                        {
                            if(fBarCode->bSHT_2DIDIsInsideList(1, 0, i, &iIndex)==0)
                            {
                                bHasErr[0][i]=true;
                                ErrPart+=IndexSuckName[0][i];
                                bAlarm=true;

                                //"2D Code_LOT ID_客批_Stage_Hard Bin_CheckResult"
                                sLog.sprintf("%s_%s_%s_Fail", BLCarryKit.cDeviceInf[0][i], sWhiteListLotID, sWhiteListProcess);
                                WriteDataToFile(as2DWhiteListLog, sLog);
                            }
                            else
                            {
                                if(iIndex>=0)
                                {
                                    fBarCode->list2DWhitleTemp->Delimiter='_';
                                    fBarCode->list2DWhitleTemp->CommaText=fBarCode->list2DWhitleResult->Strings[iIndex];
                                    fBarCode->list2DWhitleTemp->Delimiter='_';

                                    sLog.sprintf("%s_%s_%s_Pass", BLCarryKit.cDeviceInf[0][i], sWhiteListLotID, sWhiteListProcess);
                                    WriteDataToFile(as2DWhiteListLog, sLog);
                                }
                            }
                        }

                        if(InArmSuck.iShtRow==2)
                        {
                            if(BLCarryKit.Item[1][i]!=HAS_NULL_IC && BLCarryKit.Item[1][i]!=NULL_IC &&
                               BLCarryKit.cDeviceInf[1][i]!=asBarCodeErrorSend && BLCarryKit.cDeviceInf[1][i]!="" &&
                               BLCarryKit.cDeviceInf[1][i]!=asBarCodeErrorCheckSum)     //KaiHuang 20200826 : Add CheckSum
                            {
                                if(fBarCode->bSHT_2DIDIsInsideList(1, 1, i, &iIndex)==0)
                                {
                                    bHasErr[1][i]=true;
                                    ErrPart+=IndexSuckName[1][i];
                                    bAlarm=true;

                                    //"2D Code_LOT ID_客批_Stage_Hard Bin_CheckResult"
                                    sLog.sprintf("%s_%s_%s_Fail", BLCarryKit.cDeviceInf[1][i], sWhiteListLotID, sWhiteListProcess);
                                    WriteDataToFile(as2DWhiteListLog, sLog);
                                }
                                else
                                {
                                    if(iIndex>=0)
                                    {
                                        fBarCode->list2DWhitleTemp->Delimiter='_';
                                        fBarCode->list2DWhitleTemp->CommaText=fBarCode->list2DWhitleResult->Strings[iIndex];
                                        fBarCode->list2DWhitleTemp->Delimiter='_';

                                        sLog.sprintf("%s_%s_%s_Pass", BLCarryKit.cDeviceInf[1][i], sWhiteListLotID, sWhiteListProcess);
                                        WriteDataToFile(as2DWhiteListLog, sLog);
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        if(BLCarryKit.Item[0][i]!=HAS_NULL_IC && BLCarryKit.Item[0][i]!=NULL_IC &&
                           BLCarryKit.cDeviceInf[0][i]!=asBarCodeErrorSend && BLCarryKit.cDeviceInf[0][i]!="" &&
                           BLCarryKit.cDeviceInf[0][i]!=asBarCodeErrorCheckSum) //KaiHuang 20200826 : Add CheckSum
                        {
                            sLog.sprintf("%s_%s_%s_NA", BLCarryKit.cDeviceInf[0][i], sWhiteListLotID, sWhiteListProcess);
                            WriteDataToFile(as2DWhiteListLog, sLog);
                        }

                        if(InArmSuck.iShtRow==2)
                        {
                            if(BLCarryKit.Item[1][i]!=HAS_NULL_IC && BLCarryKit.Item[1][i]!=NULL_IC &&
                               BLCarryKit.cDeviceInf[1][i]!=asBarCodeErrorSend && BLCarryKit.cDeviceInf[1][i]!="" &&
                               BLCarryKit.cDeviceInf[1][i]!=asBarCodeErrorCheckSum)     //KaiHuang 20200826 : Add CheckSum
                            {
                                sLog.sprintf("%s_%s_%s_NA", BLCarryKit.cDeviceInf[1][i], sWhiteListLotID, sWhiteListProcess);
                                WriteDataToFile(as2DWhiteListLog, sLog);
                            }
                        }
                    }
                }
            }

            if(TestIF_File.bEnableBarCode==true && fLotInfo->cbRunMode->Text!="CORR" && TestIF_File.b2DIDAllowList==true && bAlarm==true)
            {
                Task=4300;
            }
            else
            {
                Task=5000;
            }
            break;
        case 4300:
            if(MOT[MSht].MotorMove(Prod.InSHT[iSht].iLeft))                     //Steven 20160503 : 先移出來才Alarm
            {
                bAlarm=false;
                fNote->t2DCode->Visible=true;
                fNote->t2DCode->XItem=InArmSuck.iShtRow;
                fNote->t2DCode->YItem=InArmSuck.iShtCol;
                fLtcSensor->ClearLtcSensor(iSht);                               //Sam 20221101 : Latch 清除都要確認是否清清乾淨
                for(int i=0; i<InArmSuck.iShtRow; i++)
                {
                    for(int j=0; j<InArmSuck.iShtCol; j++)
                    {
                        fNote->t2DCode->SetCellNumber(i, j, BLCarryKit.cDeviceInf[i][j].c_str());
                        if(bHasErr[i][j]==true)
                            fNote->t2DCode->SetCellColorIndex(i, j, 3);
                        else
                            fNote->t2DCode->SetCellColorIndex(i, j, 0);
                    }
                }

                Log.sprintf("Alarm 4300, In shuttle %d check 2DID is not in white list!\r\n", iSht+1);
                AddCCDCommunicationLog(iBarCodeRowA, Log);
                ret=ShowErrorMessage("WAR0486", K_SKIP, MSht, false, ErrPart);  //In shuttle 1 check have duplicate barcode in lot error!

                if(ret==K_SKIP)
                {
                    for(int i=0; i<InArmSuck.iShtRow; i++)
                    {
                        for(int j=0; j<InArmSuck.iShtCol; j++)
                        {
                            if(SPIL_FOR_QLE==1 &&
                               Prod.bF18InshuttleDetect==true)                  //JerryYang 20241009 : 渠梁陳勇要求重複2DID ALARM要求人員手動取出DEVICE
                            {
                                if(InArmSuck.iShtRow==2 && Prod.bF18InshuttleDetect)
                                {
                                    if(bHasErr[0][j]==true ||
                                       bHasErr[1][j]==true)                     //JerryYang 2024
                                    {
                                        BLCarryKit.SetItemData(i, j, HAS_NULL_IC);
                                        BLCarryKit.PordRec[i][j].AddTestResultRecord(iTestBinCount, "2DIDNotInAllowList");
                                    }
                                }
                                else
                                {
                                    if(bHasErr[i][j]==true)                     //Steven 20160503 : 重複的Code要設定為Error
                                    {
                                        BLCarryKit.SetItemData(i, j, HAS_NULL_IC);
                                        BLCarryKit.PordRec[i][j].AddTestResultRecord(iTestBinCount, "2DIDNotInAllowList");
                                    }
                                }
                            }
                            else
                            {
                                if(bHasErr[i][j]==true)                         //Steven 20160503 : 重複的Code要設定為Error
                                {
                                    BLCarryKit.cDeviceInf[i][j]=asBarCodeErrorSend;
                                    BLCarryKit.PordRec[i][j].AddErrorRecordNoSave("2DIDNotInAllowList 2DID error");  //JerryYang 20230322 : Lot summary要計算各類型ERR的數量
                                }
                            }
                        }
                    }

                    if(SPIL_FOR_QLE==1 && Prod.bF18InshuttleDetect==true)
                    {
                        if(InArmSuck.iShtRow==2 && Prod.bF18InshuttleDetect)
                        {
                            ShowMyMessage("請確認已Shuttle2將不在白名單的Device同一行的都取出!!");
                        }
                        else
                        {
                            ShowMyMessage("請確認已Shuttle2將不在白名單的Device取出!!");
                        }
                    }
                    Task=5000;
                }
            }
            break;
        case 5000:
            if(fBarCode->DoCheckSHT2DIDStatus(1)==true)                         //Steven 20200909 : 將2DID all site fail變成選項
            {
                if(CosFunction.bUse2DIDAllSiteFailSetToErrBin==true ||
                   TestIF_File.iEnableAllSite2DIDErr==2)
                {
                    for(int i=0; i<InArmSuck.iShtCol; i++)
                    {
                        if(BLCarryKit.Item[0][i]!=HAS_NULL_IC &&
                           BLCarryKit.Item[0][i]!=NULL_IC)
                        {
                            if(BLCarryKit.cDeviceInf[0][i]==asBarCodeErrorSend ||
                               BLCarryKit.cDeviceInf[0][i]=="" ||
                               BLCarryKit.cDeviceInf[0][i]==asBarCodeErrorCheckSum)       //KaiChen 20191121 ：中壢日月光 2D Check Sum
                            {
                                BLCarryKit.SetItemData(0, i, TEST_PASS+iTestBinCount);
                                BLCarryKit.iBinData[0][i]=iTestBinCount;
                                BLCarryKit.PordRec[0][i].AddTestResultRecord(iTestBinCount, BLCarryKit.cSBin[0][i], "NonTestToRBin");
                            }
                        }

                        if(InArmSuck.iShtRow==2)
                        {
                            if(BLCarryKit.Item[1][i]!=HAS_NULL_IC &&
                               BLCarryKit.Item[1][i]!=NULL_IC)
                            {
                                if(BLCarryKit.cDeviceInf[1][i]==asBarCodeErrorSend ||
                                   BLCarryKit.cDeviceInf[1][i]=="" ||                    //Steven 20160512 : 2D && --> ||
                                   BLCarryKit.cDeviceInf[1][i]==asBarCodeErrorCheckSum)  //KaiChen 20191121 ：中壢日月光 2D Check Sum
                                {
                                    BLCarryKit.SetItemData(1, i, TEST_PASS+iTestBinCount);
                                    BLCarryKit.iBinData[1][i]=iTestBinCount;
                                    BLCarryKit.PordRec[1][i].AddTestResultRecord(iTestBinCount, BLCarryKit.cSBin[1][i], "NonTestToRBin");
                                }
                            }
                        }
                    }
                }
                else
                {
                    Task=6000;                                                  //例外，要重掃
                    break;
                }
            }
            Task=5050;
        case 5050:
            for(int i=0; i<InArmSuck.iShtCol; i++)
            {
                if(InArmSuck.iShtRow==2)
                {
                    if(BLCarryKit.Item[0][i]!=HAS_NULL_IC &&
                       BLCarryKit.Item[0][i]!=NULL_IC)
                    {
                        iNeedBarcodeCount[iBarCodeRowA]++;                          //JerryYang 20250429 : fix雙排Pass/fail計數錯誤
                        if(BLCarryKit.cDeviceInf[0][i]==asBarCodeErrorSend ||
                           BLCarryKit.cDeviceInf[0][i]=="" ||
                           BLCarryKit.cDeviceInf[0][i]==asBarCodeErrorCheckSum)     //KaiChen 20191121 ：中壢日月光 2D Check Sum
                        {
                            iBarcodeErrorCount[iBarCodeRowA]++;
                        }
                        else
                        {
                            iBarcodePassCount[iBarCodeRowA]++;
                        }
                        BLCarryKit.PordRec[0][i].Add2DIDRecord(BLCarryKit.cDeviceInf[0][i]);
                    }

                    if(BLCarryKit.Item[1][i]!=HAS_NULL_IC &&                    //RogerYang 20181219 修正沒放IC還是會增加計數問題
                       BLCarryKit.Item[1][i]!=NULL_IC)
                    {
                        iNeedBarcodeCount[iBarCodeRowB]++;
                        if(BLCarryKit.cDeviceInf[1][i]==asBarCodeErrorSend ||
                           BLCarryKit.cDeviceInf[1][i]=="" ||
                           BLCarryKit.cDeviceInf[1][i]==asBarCodeErrorCheckSum) //KaiHuang 20200826 : Add CheckSum//Steven 20160512 : 2D && --> ||
                        {
                            iBarcodeErrorCount[iBarCodeRowB]++;
                        }
                        else
                        {
                            iBarcodePassCount[iBarCodeRowB]++;
                        }
                    }
                    BLCarryKit.PordRec[1][i].Add2DIDRecord(BLCarryKit.cDeviceInf[1][i]);
                }
                else
                {
                    if(BLCarryKit.Item[0][i]!=HAS_NULL_IC &&
                       BLCarryKit.Item[0][i]!=NULL_IC)
                    {
                        iNeedBarcodeCount[iBarCodeRowB]++;                          //JerryYang 20250429 : fix雙排Pass/fail計數錯誤
                        if(BLCarryKit.cDeviceInf[0][i]==asBarCodeErrorSend ||
                           BLCarryKit.cDeviceInf[0][i]=="" ||
                           BLCarryKit.cDeviceInf[0][i]==asBarCodeErrorCheckSum)     //KaiChen 20191121 ：中壢日月光 2D Check Sum
                        {
                            iBarcodeErrorCount[iBarCodeRowB]++;
                        }
                        else
                        {
                            iBarcodePassCount[iBarCodeRowB]++;
                        }
                        BLCarryKit.PordRec[0][i].Add2DIDRecord(BLCarryKit.cDeviceInf[0][i]);
                    }
                }
            }

            for(int i=0; i<InArmSuck.iShtCol; i++)                              //JerryYang 20200616 2DID In-house版本支援累加fail alarm功能       //清空畫面
            {
                if(bNeesScanCode[0][i]==true &&
                   (BLCarryKit.Item[0][i]!=HAS_NULL_IC &&
                    BLCarryKit.Item[0][i]!=NULL_IC) &&
                   (BLCarryKit.cDeviceInf[0][i]==asBarCodeErrorSend ||
                    BLCarryKit.cDeviceInf[0][i]==""))                           //wei 20160318 Barcode Error依客戶設定
                {
                    iBarcodeReject++;
                }

                if(bNeesScanCode[1][i]==true &&
                   (BLCarryKit.Item[1][i]!=HAS_NULL_IC &&
                    BLCarryKit.Item[1][i]!=NULL_IC) &&
                   (BLCarryKit.cDeviceInf[1][i]==asBarCodeErrorSend ||
                    BLCarryKit.cDeviceInf[1][i]==""))                           //wei 20160318 Barcode Error依客戶設定
                {
                    iBarcodeReject++;
                }
            }

            if(TestIF_File.bEnableConsecutiveFailure &&
               iBarcodeReject>=TestIF_File.iConsecutiveFailure)                 //wei 20160823  Consecutive Failure
            {
                ShowErrorMessage("WAR0475", K_SKIP, MInShuttle2, false);
                iBarcodeReject=0;
            }

            if(DoBarcodeCount())                                                //Steven 20171222 (Wei) : Yield Alarm of 2DID
            {
                Task=5200;
                break;
            }
            else
            {
                Task=5100;
            }
        case 5100:
            #ifndef SOFT_SIMULTE
            ret=SendHTTPRequest(1);                                             //Steven 20200909 : 修改Murata XML上傳資料
            #else
            ret=1;
            #endif
            if(ret==1)
            {
                Task=1;
                i2DIDRetryCTSH2=0;                                              //Isaac 20200206 : 2DID All site fail recheck twice and alarm
                iHttpErrRetryCT=0;
                return true;
            }
            else
            {
                BarcodePosDelay[iSht].SetSecAndOn(30);
                iHttpErrRetryCT++;
                Str.sprintf("Shuttle 2 SendHTTPRequest fail retry count: %d", iHttpErrRetryCT);
                RecordProcess(Str);
                if(iHttpErrRetryCT>3)
                    Task=5160;
                else
                    Task=5150;
            }
            break;
        case 5150:
            if(BarcodePosDelay[iSht].Off())
            {
                Task=5100;
            }
            break;
        case 5160:
            ret=ShowErrorMessage("WAR16320", K_RETRY|K_RESET, MMSystem, false, "DoBarcodeCCDInShuttle_2");
            iHttpErrRetryCT=0;
            if(ret==K_RETRY)
            {
                Task=5100;
            }
            else
            {
                fMain->Reset("DoBarcodeCCDInShuttle_2");
                i2DIDRetryCTSH2=0;                                              //Isaac 20200206 : 2DID All site fail recheck twice and alarm
                Task=1;
                return true;
            }
            break;
        case 5200:
            ShowErrorMessage("WAR0483", 0, MMSystem, false, s2DIDYield);
            Task=5100;
            break;
        case 6000:                                                              //Isaac 20200206 : 2DID All site fail recheck twice and alarm
            if(IniConfig.bF21InOutArmZMotorPrivate)
            {
                if(DoInOutARM_SHT_MoveSafe(1))                                  //kevin 20161005 SHUTTLE 1 移動安全保護
                    return false;
            }

            MOT[MInShuttle2].fCanMoveR=true;
            if(MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iLeft))
            {
                i2DIDRetryCTSH2++;
                if(i2DIDRetryCTSH2>1)
                {
                    i2DIDRetryCTSH2=0;
                    ret=ShowMyMessageBox_YES_SKIP("All sites 2DID Error On Shuttle 2!! Please check!", "Retry Again?");
                    if(ret==1)                                                  //Yes
                    {
                        Task=100;
                    }
                    else
                    {
                        Task=5050;
                    }
                }
            }
            break;
    }
    return false;
}
//---------------------------------------------------------------------------
bool TfBarCode::DoBarcodeTriggerInShuttle_2()                                   //Steven 20151225 : 改用拍完就跑的方式
{
    static int iNowCheckStep=0, iNowMoveStep=0;
    static int iRetryCount=0, iStepCount;
    static int i2DIDRetryCTSH2=0;                                               //Isaac 20200206 : 2DID All site fail recheck twice and alarm
    static bool bBarcodeNum[BAR_CODE_COUNT]={false};
    static bool bHasError=false;

    int &Task=iInitialBarcodeInShuttle2Task;
    int iSht=1;
    int pos=0,ret;
    AnsiString cLastString;

    switch(Task)
    {
        case 1:
            iNowMoveStep=InArmSuck.iShtCol-1;                                   //遞減方式檢查
            iNowCheckStep=iNowMoveStep;                                         //一開始移動的位置跟檢查位置是一樣的
            bBarcodeStartDelay[iBarCode2_1]=true;
            bBarcodeStartDelay[iBarCode2_2]=true;

            for(int i=0; i<BLCarryKit.iMaxCol; i++)                             //清空畫面
            {
                mtBarcodeInSh->SetCellNumber(iBarCode2_1, i+1, "");
                mtBarcodeInSh->SetCellNumber(iBarCode2_2, i+1, "");
                if(TestIF_File.i2DIDFormat==eAMD)                               //JerryYang 20200422 2DID format選項改用下拉選單
                {
                    fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(1, i+1, "");
                }
            }
            bBarcodeNum[iBarCode2_1]=true;
            bBarcodeNum[iBarCode2_2]=true;
            BLCarryKit.cDeviceInf[0][iNowMoveStep]="";
            BLCarryKit.cDeviceInf[1][iNowMoveStep]="";
            bHasError=false;
            iStepCount=0;
            iRetryCount=0;
            i2DIDRetryCTSH2=0;

            Task=1000;
        case 1000:
            if((BLCarryKit.Item[0][iNowMoveStep]==HAS_NULL_IC || BLCarryKit.Item[0][iNowMoveStep]==NULL_IC) &&
               (BLCarryKit.Item[1][iNowMoveStep]==HAS_NULL_IC || BLCarryKit.Item[1][iNowMoveStep]==NULL_IC))
            {
                Task=1200;
                break;
            }

            if(InArmSuck.iShtCol==1)
            {
                pos=Prod.iInSHBarCodeDetectPos1x1[1];
            }
            else if(InArmSuck.iShtCol==2)
            {
                pos=Prod.iInSHBarCodeDetectPos1x2[1][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==3)  //ChungHung 20140115 add for 2x3_6
            {
                pos=Prod.iInSHBarCodeDetectPos2x3[1][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==4)
            {
                pos=Prod.iInSHBarCodeDetectPos1x4[1][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==5)  //Steven 20221027 : Add for 2x5
            {
                pos=Prod.iInSHBarCodeDetectPos2x5[1][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==6)  //Steven 20170111 : fixed for 2x6
            {
                pos=Prod.iInSHBarCodeDetectPos2x6[1][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==8)
            {
                pos=Prod.iInSHBarCodeDetectPos2x8[1][iNowMoveStep];
            }
            else
            {
                ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "DoBarcodeTriggerInShuttle_2");
            }

            pos+=Offset.iSHLeft2D[1];

            if(MOT[MInShuttle2].MotorMove(pos))
            {
                if(BLCarryKit.Item[0][iNowCheckStep]==HAS_NULL_IC ||            //判斷要不要讀Code
                   BLCarryKit.Item[0][iNowCheckStep]==NULL_IC)
                {
                    bBarcodeNum[iBarCode2_1]=true;
                }
                else
                {
                    bBarcodeStartDelay[iBarCode2_1]=true;
                    bBarcodeNum[iBarCode2_1]=false;
                }

                if(InArmSuck.iShtRow==2)
                {
                    if(BLCarryKit.Item[1][iNowCheckStep]==HAS_NULL_IC ||
                       BLCarryKit.Item[1][iNowCheckStep]==NULL_IC)
                    {
                        bBarcodeNum[iBarCode2_2]=true;
                    }
                    else
                    {
                        bBarcodeStartDelay[iBarCode2_2]=true;
                        bBarcodeNum[iBarCode2_2]=false;
                    }
                }
                else
                {
                    bBarcodeNum[iBarCode2_2]=true;
                }

                if(iStepCount==0)
                {
                    BarcodePosDelay[iSht].SetMSAndOn(TestIF_File.iBarCodePos1Delay);
                    iStepCount++;
                }
                else
                {
                    BarcodePosDelay[iSht].SetMSAndOn(TestIF_File.iBarCodePosDelay);
                    iStepCount++;
                }
                Task=1120;
            }
            break;
        case 1120:
            if(BarcodePosDelay[iSht].Off())                                     //到位Delay
            {
                BarcodeExposureDelay[iSht].SetMSAndOn(TestIF_File.i2DTriggerTime);
                if(bBarcodeNum[iBarCode2_1]==false)
                {
                    if(iNowCheckStep==0)
                    {
                        cLastString="(NULL)";
                    }
                    else
                    {
                        cLastString=BLCarryKit.cDeviceInf[0][iNowCheckStep-1];
                    }
                    iSH2_1BarcodePosition=iNowCheckStep;
                    Barcode_StartScan_In(iBarCode2_1, iNowCheckStep, BLCarryKit.cDeviceInf[0][iNowCheckStep], cLastString);
                }

                if(bBarcodeNum[iBarCode2_2]==false)
                {
                    if(iNowCheckStep==0)
                    {
                        cLastString="(NULL)";
                    }
                    else
                    {
                        cLastString=BLCarryKit.cDeviceInf[1][iNowCheckStep-1];
                    }
                    iSH2_2BarcodePosition=iNowCheckStep;
                    Barcode_StartScan_In(iBarCode2_2, iNowCheckStep, BLCarryKit.cDeviceInf[1][iNowCheckStep], cLastString);
                }

                Task=1150;
            }
            break;
        case 1150:
            if(BarcodeExposureDelay[iSht].Off())                                //拍照Delay
            {
                BarcodeDelay[iSht].SetMSAndOn(TestIF_File.iBarCodeDelay);       //這個是讀取Time Out用的
                Task=1200;
            }
            break;
        case 1200:
            iRetryCount=0;
            iNowMoveStep--;

            if(iNowMoveStep>=0 && iNowMoveStep<InArmSuck.iShtCol)
            {
                if((BLCarryKit.Item[0][iNowMoveStep]==HAS_NULL_IC || BLCarryKit.Item[0][iNowMoveStep]==NULL_IC) &&
                   (BLCarryKit.Item[1][iNowMoveStep]==HAS_NULL_IC || BLCarryKit.Item[1][iNowMoveStep]==NULL_IC))    //直接判斷要不要移動馬達
                {
                    Task=1200;
                }
                else
                {
                    Task=1300;
                }
            }
            else
            {
                if(iNowMoveStep!=iNowCheckStep)                                 //最後一步
                {
                    Task=1350;
                }
                else
                {
                    Task=2000;
                }
            }
            break;
        case 1300:                                                              //偷跑
            if(InArmSuck.iShtCol==1)
            {
                pos=Prod.iInSHBarCodeDetectPos1x1[1];
            }
            else if(InArmSuck.iShtCol==2)
            {
                pos=Prod.iInSHBarCodeDetectPos1x2[1][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==3)                                       //ChungHung 20140115 add for 2x3_6
            {
                pos=Prod.iInSHBarCodeDetectPos2x3[1][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==4)
            {
                pos=Prod.iInSHBarCodeDetectPos1x4[1][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==5)                                       //Steven 20221027 : Add for 2x5
            {
                pos=Prod.iInSHBarCodeDetectPos2x5[1][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==6)                                       //Steven 20170111 : fixed for 2x6
            {
                pos=Prod.iInSHBarCodeDetectPos2x6[1][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==8)
            {
                pos=Prod.iInSHBarCodeDetectPos2x8[1][iNowMoveStep];
            }
            else
            {
                ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "DoBarcodeTriggerInShuttle_2");
            }

            pos+=Offset.iSHLeft2D[1];

            if(MOT[MInShuttle2].MotorMove(pos))
            {
                Task=1350;
            }
            break;
        case 1350:
            if(bBarcodeNum[iBarCode2_1]==false)
            {
                if(iNowCheckStep==0)
                {
                    cLastString="(NULL)";
                }
                else
                {
                    cLastString=BLCarryKit.cDeviceInf[0][iNowCheckStep-1];
                }
                bBarcodeNum[iBarCode2_1]=Barcode_StartScan_In(iBarCode2_1, iNowCheckStep, BLCarryKit.cDeviceInf[0][iNowCheckStep], cLastString);
            }

            if(bBarcodeNum[iBarCode2_2]==false)
            {
                if(iNowCheckStep==0)
                {
                    cLastString="(NULL)";
                }
                else
                {
                    cLastString=BLCarryKit.cDeviceInf[1][iNowCheckStep-1];
                }
                bBarcodeNum[iBarCode2_2]=Barcode_StartScan_In(iBarCode2_2, iNowCheckStep, BLCarryKit.cDeviceInf[1][iNowCheckStep], cLastString);
            }

            if(bBarcodeNum[iBarCode2_1]==true && bBarcodeNum[iBarCode2_2]==true)//讀取完成
            {
                if(BLCarryKit.cDeviceInf[0][iNowCheckStep]==asBarCodeErrorSend ||           //wei 20160318 Barcode Error依客戶設定
                   BLCarryKit.cDeviceInf[1][iNowCheckStep]==asBarCodeErrorSend ||
                   BLCarryKit.cDeviceInf[0][iNowCheckStep]==asBarCodeErrorCheckSum ||
                   BLCarryKit.cDeviceInf[1][iNowCheckStep]==asBarCodeErrorCheckSum)     //KaiHuang 20200826 : Add CheckSum
                {
                    bHasError=true;
                }

                Task=2000;
            }
            else if(BarcodeDelay[iSht].Off())                                   //讀取超時
            {
                if(bBarcodeNum[iBarCode2_1]==false)
                {
                    bHasError=true;
                    TurnOffInspection(iBarCode2_1);
                    BLCarryKit.cDeviceInf[0][iNowCheckStep]=asBarCodeErrorSend; //wei 20160318 Barcode Error依客戶設定
                    mtBarcodeInSh->SetCellNumber(iBarCode2_1, iNowCheckStep+1, asBarCodeErrorSend.c_str());
                    if(TestIF_File.i2DIDFormat==eAMD)                           //JerryYang 20200422 2DID format選項改用下拉選單
                    {
                        fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(1, iNowCheckStep+1, asBarCodeErrorSend.c_str());
                    }
                }

                if(bBarcodeNum[iBarCode2_2]==false)
                {
                    bHasError=true;
                    TurnOffInspection(iBarCode2_2);
                    BLCarryKit.cDeviceInf[1][iNowCheckStep]=asBarCodeErrorSend; //wei 20160318 Barcode Error依客戶設定
                    mtBarcodeInSh->SetCellNumber(iBarCode2_2, iNowCheckStep+1, asBarCodeErrorSend.c_str());
                }
                Task=2000;
            }
            break;
        case 2000:
            iNowCheckStep=iNowMoveStep;
            if(iNowMoveStep==-1)
            {
                if(TestIF_File.bNoCodeDeviceAutoSkip==false && bHasError==true)
                {
                    Task=3000;
                }
                else
                {
                    if(IniConfig.bKoreaFunction==true)                          //Isaac 20200206 : 2DID All site fail recheck twice and alarm
                    {
                        if(fBarCode->DoCheckSHT2DIDStatus(1)==true)
                        {
                            Task=5000;                                          //例外，要重掃
                            break;
                        }
                    }
                    DoBarcodeCount();                                           //wei 20160318
                    Task=1;
                    return true;
                }
            }
            else
            {
                Task=1000;
            }
            break;
        case 3000:                                                              //例外處理
            iNowCheckStep=InArmSuck.iShtCol;                                    //遞減方式檢查, 故意+1, 因為到下一個Task會先--
            Task=3100;
        case 3100:
            iRetryCount=0;
            iNowCheckStep--;

            if(iNowCheckStep>=0 && iNowCheckStep<InArmSuck.iShtCol)
            {
                if(BLCarryKit.cDeviceInf[0][iNowCheckStep]==asBarCodeErrorSend ||    //直接判斷要不要移動馬達          //wei 20160318 Barcode Error依客戶設定
                   BLCarryKit.cDeviceInf[1][iNowCheckStep]==asBarCodeErrorSend)
                {
                    Task=3200;
                }
                else
                {
                    Task=3100;
                }
            }
            else
            {
                if(IniConfig.bKoreaFunction==true)                              //Isaac 20200206 : 2DID All site fail recheck twice and alarm
                {
                    if(fBarCode->DoCheckSHT2DIDStatus(1)==true)
                    {
                        Task=5000;                                              //例外，要重掃
                        break;
                    }
                }
                DoBarcodeCount();                                               //wei 20160318
                Task=1;
                return true;
            }
            break;
        case 3200: //重置參數
            if(BLCarryKit.cDeviceInf[0][iNowCheckStep]==asBarCodeErrorSend)     //wei 20160318 Barcode Error依客戶設定
            {
                mtBarcodeInSh->SetCellNumber(iBarCode2_1, iNowCheckStep+1, "");
                if(TestIF_File.i2DIDFormat==eAMD)                               //JerryYang 20200422 2DID format選項改用下拉選單
                {
                    fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(1, iNowCheckStep+1, "");
                }
                BLCarryKit.cDeviceInf[0][iNowCheckStep]="";
                bBarcodeNum[iBarCode2_1]=false;
                bBarcodeStartDelay[iBarCode2_1]=true;
            }

            if(BLCarryKit.cDeviceInf[1][iNowCheckStep]==asBarCodeErrorSend)     //wei 20160318 Barcode Error依客戶設定
            {
                mtBarcodeInSh->SetCellNumber(iBarCode2_2, iNowCheckStep+1, "");
                BLCarryKit.cDeviceInf[1][iNowCheckStep]="";
                bBarcodeNum[iBarCode2_2]=false;
                bBarcodeStartDelay[iBarCode2_2]=true;
            }
            Task=3300;
        case 3300:                                                              //移動馬達
            if(InArmSuck.iShtCol==1)
            {
                pos=Prod.iInSHBarCodeDetectPos1x1[1];
            }
            else if(InArmSuck.iShtCol==2)
            {
                pos=Prod.iInSHBarCodeDetectPos1x2[1][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==3)                                       //ChungHung 20140115 add for 2x3_6
            {
                pos=Prod.iInSHBarCodeDetectPos2x3[1][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==4)
            {
                pos=Prod.iInSHBarCodeDetectPos1x4[1][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==5)                                       //Steven 20221027 : Add for 2x5
            {
                pos=Prod.iInSHBarCodeDetectPos2x5[1][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==6)                                       //Steven 20170111 : fixed for 2x6
            {
                pos=Prod.iInSHBarCodeDetectPos2x6[1][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==8)
            {
                pos=Prod.iInSHBarCodeDetectPos2x8[1][iNowMoveStep];
            }
            else
            {
                ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "DoBarcodeTriggerInShuttle_2");
            }

            pos+=Offset.iSHLeft2D[1];

            if(MOT[MInShuttle2].MotorMove(pos))
            {
                BarcodePosDelay[iSht].SetMSAndOn(TestIF_File.iBarCodePosDelay);
                Task=3350;
            }
            break;
        case 3350:
            if(BarcodePosDelay[iSht].Off())
            {
                BarcodeDelay[iSht].SetMSAndOn(TestIF_File.iBarCodeDelay);
                Task=3400;
            }
            break;
        case 3400:                                                              //讀取2D Code
            if(bBarcodeNum[iBarCode2_1]==false)
            {
                iSH1_1BarcodePosition=iNowCheckStep;
                if(iNowCheckStep==0)
                {
                    cLastString="(NULL)";
                }
                else
                {
                    cLastString=BLCarryKit.cDeviceInf[0][iNowCheckStep-1];
                }
                bBarcodeNum[iBarCode2_1]=Barcode_StartScan_In(iBarCode2_1, iNowCheckStep, BLCarryKit.cDeviceInf[0][iNowCheckStep], cLastString);

                if(bBarcodeNum[iBarCode2_1]==true)
                {
                    bBarcodeStartDelay[iBarCode2_1]=true;
                    bBarcodeDataSaveReady[iBarCode2_1]=false;
                }
            }

            if(bBarcodeNum[iBarCode2_2]==false)
            {
                iSH1_2BarcodePosition=iNowCheckStep;
                if(iNowCheckStep==0)
                {
                    cLastString="(NULL)";
                }
                else
                {
                    cLastString=BLCarryKit.cDeviceInf[1][iNowCheckStep-1];
                }
                bBarcodeNum[iBarCode2_2]=Barcode_StartScan_In(iBarCode2_2, iNowCheckStep, BLCarryKit.cDeviceInf[1][iNowCheckStep], cLastString);

                if(bBarcodeNum[iBarCode2_2]==true)
                {
                    bBarcodeStartDelay[iBarCode2_2]=true;
                    bBarcodeDataSaveReady[iBarCode2_2]=false;
                }
            }

            if(bBarcodeNum[iBarCode2_1]==true && bBarcodeNum[iBarCode2_2]==true)
            {
                Task=3100;
            }
            else if(BarcodeDelay[iSht].Off())
            {
                TurnOffInspection(iBarCode2_1);
                TurnOffInspection(iBarCode2_2);
                #ifdef SOFT_SIMULTE
                ret=K_SKIP;
                #else
                if(LastSet.iRealDummy==DUMMY)
                    ret=K_SKIP;
                else
                {
                    if(iRetryCount>=TestIF_File.iBarcodeRetryCount)             //Steven 20151221 : 讀不到Code時,自動Retry的次數
                    {
                        if(TestIF_File.bNoCodeDeviceAutoSkip==true &&
                           fContact->IsRun2DCheck()==false)                     //JerryYang 20250220 : 2DID硬體順序檢查功能             //Steven 20151221 : 讀不到Code時,自動Skip跳下一顆
                            ret=K_SKIP;
                        else if(fContact->IsRun2DCheck()==true)                 //JerryYang 20250220 : 2DID硬體順序檢查功能
                            ret=ShowErrorMessage("JAM0496", K_RETRY, MInShuttle2);      //In Shuttle2 Barcode Error
                        else
                            ret=ShowErrorMessage("JAM0461", K_RETRY|K_SKIP, MInShuttle2);      //In Shuttle2 Barcode Error
                        iRetryCount=-1;
                    }
                    else
                    {
                        ret=K_RETRY;
                    }
                }
                #endif
                if(ret==K_RETRY)
                {
                    iRetryCount++;
                    Task=3300;
                    return false;
                }
                else
                {
                    iRetryCount=0;
                    if(bBarcodeNum[iBarCode2_1]==false)
                    {
                        BLCarryKit.cDeviceInf[0][iNowCheckStep]=asBarCodeErrorSend;         //wei 20160318 Barcode Error依客戶設定
                        mtBarcodeInSh->SetCellNumber(iBarCode2_1, iNowCheckStep+1, asBarCodeErrorSend.c_str());
                        if(TestIF_File.i2DIDFormat==eAMD)                       //JerryYang 20200422 2DID format選項改用下拉選單
                        {
                            fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(1, iNowCheckStep+1, asBarCodeErrorSend.c_str());
                        }
                    }

                    if(bBarcodeNum[iBarCode2_2]==false)
                    {
                        BLCarryKit.cDeviceInf[1][iNowCheckStep]=asBarCodeErrorSend;         //wei 20160318 Barcode Error依客戶設定
                        mtBarcodeInSh->SetCellNumber(iBarCode2_2, iNowCheckStep+1, asBarCodeErrorSend.c_str());
                    }
                    Task=3100;
                }
            }
            break;
        case 5000:                                                              //rescan    //Isaac 20200206 : 2DID All site fail recheck twice and alarm
            if(IniConfig.bF21InOutArmZMotorPrivate)
            {
                if(DoInOutARM_SHT_MoveSafe(1))                                  //kevin 20161005 SHUTTLE 1 移動安全保護
                    return false;
            }

            MOT[MInShuttle2].fCanMoveR=true;
            if(MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iLeft))
            {
                i2DIDRetryCTSH2++;
                if(i2DIDRetryCTSH2>1)
                {
                    i2DIDRetryCTSH2=0;
                    ret=ShowMyMessageBox_YES_SKIP("All sites 2DID Error On Shuttle 2!! Please check!", "Retry Again?");
                    if(ret==1)                                                  //Yes
                    {
                        Task=1;
                    }
                    else
                    {
                        DoBarcodeCount();                                       //wei 20160318
                        Task=1;
                        return true;
                    }
                }
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
bool TfBarCode::DoBarcodeScanInShuttle_2(bool bErrorSkip)
{
    static int iNowCheckStep=0;
    static int iRetryCount=0;
    static int i2DIDRetryCTSH2=0;                                               //Isaac 20200206 : 2DID All site fail recheck twice and alarm
    static bool bErrorRetry=false;
    static bool bBarcodeNum[BAR_CODE_COUNT]={false};
    static AnsiString sErrorPart="", sCheckErrorPart="";

    int &Task=iInitialBarcodeInShuttle2Task;
    int iSht=1;
    int pos=0, ret;
//    int iOCRMap[]={1, 0, 3, 2};                                                 //Ifor 20211130 add:OCR Map
    int iBarCodeRowA=iBarCode2_1;                                               //JerryYang 20250512 : 修正CCD使用位置
    int iBarCodeRowB=iBarCode2_2;
    bool bMoveOK=false;                                                         //Ifor 20200421 add:OCR Alarm 不移動回Loader區
    AnsiString DataStr, Path, Data;
    AnsiString cLastString;
    AnsiString asString;

    if(InArmSuck.iShtRow==1)// &&                                                  //Steven 20240227 : Shuttle使用一排的
//       BAR_CODE_INSTALL!=ebcUseOCR)                                             //Ifor 20220322 add 自制OCR不使用
    {
        if(TestIF_File.iSelectUseCCDSh2==1)
        {
            iBarCodeRowA=iBarCode2_1;
            iBarCodeRowB=iBarCode2_2;
        }
        else
        {
            iBarCodeRowA=iBarCode2_2;
            iBarCodeRowB=iBarCode2_1;
        }
    }
    else if(BAR_CODE_INSTALL==ebcUseOCR)
    {
        iBarCodeRowA=iBarCode2_1;
        iBarCodeRowB=iBarCode2_2;
    }

    switch(Task)
    {
        case 1:
            iNowCheckStep=InArmSuck.iShtCol-1;                                  //遞減方式檢查
            InitSht2DCodeComPort(iSht);                                         //Ifor 20160322 Retry 時重置
            if(BAR_CODE_INSTALL==ebctUseCCDMode)                                //JerryYang 20201218 In house版本 shuttle 2 單排使用CCD4
            {
                if(InArmSuck.iShtRow==2)
                {
                    bBarcodeStartDelay[iBarCodeRowA]=true;
                    bBarcodeStartDelay[iBarCodeRowB]=true;
                }
                else
                {
                    bBarcodeStartDelay[iBarCodeRowB]=true;
                }
            }
//            else                                                                //shuttle 2 單排使用CCD3
//            {
//                bBarcodeStartDelay[iBarCodeRowA]=true;
//                if(InArmSuck.iShtRow==2)
//                {
//                    bBarcodeStartDelay[iBarCodeRowB]=true;
//                }
//            }

            for(int i=0; i<BLCarryKit.iMaxCol; i++)
            {
                mtBarcodeInSh->SetCellNumber(iBarCodeRowA, i+1, "");
                mtBarcodeInSh->SetCellNumber(iBarCodeRowB, i+1, "");
                if(TestIF_File.i2DIDFormat==eAMD)                               //JerryYang 20200422 2DID format選項改用下拉選單
                {
                    fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(1, i+1, "");
                }
            }

            for(int i=0; i<InArmSuck.iShtRow; i++)
            {
                for(int j=0; j<InArmSuck.iShtCol; j++)
                {
                    fNote->t2DCode->SetCellNumber(i, j, "");
                    fNote->t2DCode->SetCellColorIndex(i, j, 0);
                }
            }
            bBarcodeNum[iBarCodeRowA]=false;
            bBarcodeNum[iBarCodeRowB]=false;
            bBarcodeFirstAutoRetry[iBarCodeRowA]=false;
            bBarcodeFirstAutoRetry[iBarCodeRowB]=false;
            iRetryCount=0;
            if(bErrorRetry)
            {
                Task=1100;
                bErrorRetry=false;
            }
            else
            {
                Task=1000;
            }
        case 1000:
            if(InArmSuck.iShtRow==2)
            {
                if(BLCarryKit.Item[0][iNowCheckStep]!=HAS_NULL_IC &&
                   BLCarryKit.Item[0][iNowCheckStep]!=NULL_IC)                      //Ifor 20190129 : 修正計數 //wei 20160516 修改需讀取barcode顆數累計
                {
                    if(bBarcodeFirstAutoRetry[iBarCodeRowA]==false)
                        iNeedBarcodeCount[iBarCodeRowA]++;
                }

                if(BLCarryKit.Item[1][iNowCheckStep]!=HAS_NULL_IC &&
                   BLCarryKit.Item[1][iNowCheckStep]!=NULL_IC)                      //wei 20160516 修改需讀取barcode顆數累計
                {
                    if(bBarcodeFirstAutoRetry[iBarCodeRowB]==false)
                        iNeedBarcodeCount[iBarCodeRowB]++;
                }
            }
            else
            {
                if(BLCarryKit.Item[0][iNowCheckStep]!=HAS_NULL_IC &&
                   BLCarryKit.Item[0][iNowCheckStep]!=NULL_IC)                      //Ifor 20190129 : 修正計數 //wei 20160516 修改需讀取barcode顆數累計
                {
                    if(bBarcodeFirstAutoRetry[iBarCodeRowB]==false)
                        iNeedBarcodeCount[iBarCodeRowB]++;
                }
            }
            Task=1100;
        case 1100:
            if(InArmSuck.iShtRow==2)
            {
                if(bBarcodeNum[iBarCodeRowA]==false)
                {
                    BLCarryKit.cDeviceInf[0][iNowCheckStep]="";
                    mtBarcodeInSh->SetCellNumber(iBarCodeRowA, iNowCheckStep+1, "");
                    if(TestIF_File.i2DIDFormat==eAMD)                           //JerryYang 20200422 2DID format選項改用下拉選單
                    {
                        fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(1, iNowCheckStep+1, "");
                    }
                }

                if(bBarcodeNum[iBarCodeRowB]==false)
                {
                    BLCarryKit.cDeviceInf[1][iNowCheckStep]="";
                    mtBarcodeInSh->SetCellNumber(iBarCodeRowB, iNowCheckStep+1, "");
                }
            }
            else
            {
                if(bBarcodeNum[iBarCodeRowB]==false)
                {
                    BLCarryKit.cDeviceInf[0][iNowCheckStep]="";
                    mtBarcodeInSh->SetCellNumber(iBarCodeRowB, iNowCheckStep+1, "");
                }
            }

            if((BLCarryKit.Item[0][iNowCheckStep]==HAS_NULL_IC || BLCarryKit.Item[0][iNowCheckStep]==NULL_IC) &&
               (BLCarryKit.Item[1][iNowCheckStep]==HAS_NULL_IC || BLCarryKit.Item[1][iNowCheckStep]==NULL_IC))
            {
                Task=1200;
                break;
            }

            fNote->t2DCode->XItem=InArmSuck.iShtRow;                            //wei 20160824 清除顯示Barcode Alarm
            fNote->t2DCode->YItem=InArmSuck.iShtCol;
            for(int i=0; i<InArmSuck.iShtRow; i++)
            {
                for(int j=0; j<InArmSuck.iShtCol; j++)
                {
                    fNote->t2DCode->SetCellNumber(i, j, "");
                    fNote->t2DCode->SetCellColorIndex(i, j, 0);
                }
            }

            if(iNowCheckStep>=0 && iNowCheckStep<InArmSuck.iShtCol)
            {
                if(InArmSuck.iShtCol==1)
                {
                    pos=Prod.iInSHBarCodeDetectPos1x1[1];
                }
                else if(InArmSuck.iShtCol==2)
                {
                    pos=Prod.iInSHBarCodeDetectPos1x2[1][iNowCheckStep];
                }
                else if(InArmSuck.iShtCol==3)                                   //ChungHung 20140115 add for 2x3_6
                {
                    pos=Prod.iInSHBarCodeDetectPos2x3[1][iNowCheckStep];
                }
                else if(InArmSuck.iShtCol==4)
                {
                    pos=Prod.iInSHBarCodeDetectPos1x4[1][iNowCheckStep];
                }
                else if(InArmSuck.iShtCol==5)                                   //Steven 20221027 : Add for 2x5
                {
                    pos=Prod.iInSHBarCodeDetectPos2x5[1][iNowCheckStep];
                }
                else if(InArmSuck.iShtCol==6)                                   //Steven 20170111 : fixed for 2x6
                {
                    pos=Prod.iInSHBarCodeDetectPos2x6[1][iNowCheckStep];
                }
                else if(InArmSuck.iShtCol==8)
                {
                    pos=Prod.iInSHBarCodeDetectPos2x8[1][iNowCheckStep];
                }
                else
                {
                    ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "DoBarcodeScanInShuttle_2");
                }

                pos+=Offset.iSHLeft2D[1];                                       //Steven 20151218 : Offset for 2d reader

                if(TestIF_File.bRetryShiftOffsetMove && iRetryCount>0)          //wei 20161116 Retry時先退出再進去讀取(前中後)
                {
                    if(iRetryCount%3 == 1)
                        pos-=(TestIF_File.dRetryShiftOffsetMove*100);
                    else if(iRetryCount%3 == 2)
                        pos+=(TestIF_File.dRetryShiftOffsetMove*100);
                }

                if(MOT[MInShuttle2].MotorMove(pos))
                {
                    bBarcodeStartDelay[iBarCodeRowB]=true;
                    if(InArmSuck.iShtRow==2)
                    {
                        bBarcodeStartDelay[iBarCodeRowA]=true;
                    }

                    BarcodePosDelay[iSht].SetMSAndOn(TestIF_File.iBarCodePosDelay);
                    Task=1120;
                }
            }
            else
            {
                Task=2000;
            }
            break;
        case 1120:
            if(BarcodePosDelay[iSht].Off())
            {
                BarcodeDelay[iSht].SetMSAndOn(TestIF_File.iBarCodeDelay);
                //==> Eastsun 20260526 #026-4.P2 reset Pin1 Error :KYEC
                bHasPin1Error[iBarCodeRowA]=false;
                bHasPin1Error[iBarCodeRowB]=false;
                //<== Eastsun 20260526 #026-4.P2
                sErrorPart="";
                sCheckErrorPart="";
                Task=1150;
            }
            break;
        case 1150:                                                              //雙排時,Shuttle 2, Row A 使用第4隻CCD; Row B 使用第3隻CCD
            if(InArmSuck.iShtRow==2)                                            //JerryYang 20201218 In house版本 shuttle 2 單排使用CCD4
            {
//                if(BAR_CODE_INSTALL==ebctUseCCDMode)                            //In house版本 shuttle 2 單排使用CCD4
//                {
                    if(BLCarryKit.Item[0][iNowCheckStep]==HAS_NULL_IC ||
                       BLCarryKit.Item[0][iNowCheckStep]==NULL_IC)
                    {
                        bBarcodeNum[iBarCodeRowA]=true;
                    }
                    else if(bBarcodeNum[iBarCodeRowA]==false)
                    {
                        iSH2_2BarcodePosition=iNowCheckStep;
                        if(iNowCheckStep==InArmSuck.iShtCol-1)
                        {
                            cLastString="(NULL)";
                        }
                        else
                        {
                            cLastString=BLCarryKit.cDeviceInf[0][iNowCheckStep+1];
                        }

                        if(Barcode_StartScan_In(iBarCodeRowA, iNowCheckStep, BLCarryKit.cDeviceInf[0][iNowCheckStep], cLastString)==true)
                        {
                            bBarcodeStartDelay[iBarCodeRowA]=true;
                            bBarcodeDataSaveReady[iBarCodeRowA]=false;
                            bBarcodeNum[iBarCodeRowA]=true;
                        }

                        if((BAR_CODE_INSTALL==ebctEtherNetCCD || BAR_CODE_INSTALL==ebcUseOCR) && //Steven 20260420 : add OCR HandShake timeout
                           CosFunction.bUseHandShakeCommunication &&
                           TestIF_File.bUseHandShakeCommunication==true)
                        {
                            if(bCCDBarcodeWaitReply[iBarCodeRowA]==false)
                            {
                                BarcodeWaitReplyTimeOut[iBarCodeRowA].SetMSAndOn(TestIF_File.i2DHandShakeTimeOut);
                                bCCDBarcodeWaitReply[iBarCodeRowA]=true;
                            }
                        }
                    }

                    if(BLCarryKit.Item[1][iNowCheckStep]==HAS_NULL_IC ||
                       BLCarryKit.Item[1][iNowCheckStep]==NULL_IC)
                    {
                        bBarcodeNum[iBarCodeRowB]=true;
                    }
                    else if(bBarcodeNum[iBarCodeRowB]==false)
                    {
                        iSH2_1BarcodePosition=iNowCheckStep;
                        if(iNowCheckStep==InArmSuck.iShtCol-1)
                        {
                            cLastString="(NULL)";
                        }
                        else
                        {
                            cLastString=BLCarryKit.cDeviceInf[1][iNowCheckStep+1];
                        }

                        if(Barcode_StartScan_In(iBarCodeRowB, iNowCheckStep, BLCarryKit.cDeviceInf[1][iNowCheckStep], cLastString)==true)
                        {
                            bBarcodeStartDelay[iBarCodeRowB]=true;
                            bBarcodeDataSaveReady[iBarCodeRowB]=false;
                            bBarcodeNum[iBarCodeRowB]=true;
                        }

                        if((BAR_CODE_INSTALL==ebctEtherNetCCD || BAR_CODE_INSTALL==ebcUseOCR) && //Steven 20260420 : add OCR HandShake timeout
                           CosFunction.bUseHandShakeCommunication &&
                           TestIF_File.bUseHandShakeCommunication==true)
                        {
                            if(bCCDBarcodeWaitReply[iBarCodeRowB]==false)
                            {
                                BarcodeWaitReplyTimeOut[iBarCodeRowB].SetMSAndOn(TestIF_File.i2DHandShakeTimeOut);
                                bCCDBarcodeWaitReply[iBarCodeRowB]=true;
                            }
                        }
                    }
//                }
//                else
//                {
//                    if(BLCarryKit.Item[0][iNowCheckStep]==HAS_NULL_IC ||
//                       BLCarryKit.Item[0][iNowCheckStep]==NULL_IC)
//                    {
//                        bBarcodeNum[iBarCodeRowA]=true;
//                    }
//                    else if(bBarcodeNum[iBarCodeRowA]==false)
//                    {
//                        if(BAR_CODE_INSTALL!=ebcUseOCR)
//                            iSH2_1BarcodePosition=iNowCheckStep;
//                        else
//                            iSH2_2BarcodePosition=iNowCheckStep;
//                        if(iNowCheckStep==InArmSuck.iShtCol-1)
//                        {
//                            cLastString="(NULL)";
//                        }
//                        else
//                        {
//                            cLastString=BLCarryKit.cDeviceInf[0][iNowCheckStep+1];
//                        }
//                        if(Barcode_StartScan_In(iBarCodeRowA, iNowCheckStep, BLCarryKit.cDeviceInf[0][iNowCheckStep], cLastString)==true)
//                        {
//                            bBarcodeStartDelay[iBarCodeRowA]=true;
//                            bBarcodeDataSaveReady[iBarCodeRowA]=false;
//                            bBarcodeNum[iBarCodeRowA]=true;
//                        }
//
//                        if(BAR_CODE_INSTALL==ebctEtherNetCCD &&                 //Ifor 20190225 :add Bar Code Use HandShake Communication
//                           CosFunction.bUseHandShakeCommunication &&
//                           TestIF_File.bUseHandShakeCommunication==true)
//                        {
//                            if(bCCDBarcodeWaitReply[iBarCodeRowA]==false)
//                            {
//                                BarcodeWaitReplyTimeOut[iBarCodeRowA].SetMSAndOn(TestIF_File.i2DHandShakeTimeOut);
//                                bCCDBarcodeWaitReply[iBarCodeRowA]=true;
//                            }
//                        }
//                    }
//
//                    if(BLCarryKit.Item[1][iNowCheckStep]==HAS_NULL_IC ||
//                       BLCarryKit.Item[1][iNowCheckStep]==NULL_IC)
//                    {
//                        bBarcodeNum[iBarCodeRowB]=true;
//                    }
//                    else if(bBarcodeNum[iBarCodeRowB]==false)
//                    {
//                        if(BAR_CODE_INSTALL!=ebcUseOCR)
//                            iSH2_2BarcodePosition=iNowCheckStep;
//                        else
//                            iSH2_1BarcodePosition=iNowCheckStep;
//
//                        if(iNowCheckStep==InArmSuck.iShtCol-1)
//                        {
//                            cLastString="(NULL)";
//                        }
//                        else
//                        {
//                            cLastString=BLCarryKit.cDeviceInf[1][iNowCheckStep+1];
//                        }
//
//                        if(Barcode_StartScan_In(iBarCodeRowB, iNowCheckStep, BLCarryKit.cDeviceInf[1][iNowCheckStep], cLastString)==true)
//                        {
//                            bBarcodeStartDelay[iBarCodeRowB]=true;
//                            bBarcodeDataSaveReady[iBarCodeRowB]=false;
//                            bBarcodeNum[iBarCodeRowB]=true;
//                        }
//
//                        if(BAR_CODE_INSTALL==ebctEtherNetCCD &&                 //Ifor 20190225 :add Bar Code Use HandShake Communication
//                           CosFunction.bUseHandShakeCommunication &&
//                           TestIF_File.bUseHandShakeCommunication==true)
//                        {
//                            if(bCCDBarcodeWaitReply[iBarCodeRowB]==false)
//                            {
//                                BarcodeWaitReplyTimeOut[iBarCodeRowB].SetMSAndOn(TestIF_File.i2DHandShakeTimeOut);
//                                bCCDBarcodeWaitReply[iBarCodeRowB]=true;
//                            }
//                        }
//                    }
//                }
            }
            else
            {
//                if(BAR_CODE_INSTALL==ebctUseCCDMode)                            //JerryYang 20201218 In house版本 shuttle 2 單排使用CCD4
//                {
                    if(BLCarryKit.Item[0][iNowCheckStep]==HAS_NULL_IC ||
                       BLCarryKit.Item[0][iNowCheckStep]==NULL_IC)
                    {
                        bBarcodeNum[iBarCodeRowB]=true;
                    }
                    else if(bBarcodeNum[iBarCodeRowB]==false)
                    {
//                        if(BAR_CODE_INSTALL!=ebcUseOCR)                   //Steven 20250808 : for OCR
//                            iSH2_2BarcodePosition=iNowCheckStep;
//                        else
                            iSH2_1BarcodePosition=iNowCheckStep;

                        if(iNowCheckStep==InArmSuck.iShtCol-1)
                        {
                            cLastString="(NULL)";
                        }
                        else
                        {
                            cLastString=BLCarryKit.cDeviceInf[0][iNowCheckStep+1];
                        }

                        if(Barcode_StartScan_In(iBarCodeRowB, iNowCheckStep, BLCarryKit.cDeviceInf[0][iNowCheckStep], cLastString)==true)
                        {
                            bBarcodeStartDelay[iBarCodeRowB]=true;
                            bBarcodeDataSaveReady[iBarCodeRowB]=false;
                            bBarcodeNum[iBarCodeRowB]=true;
                        }

                        if((BAR_CODE_INSTALL==ebctEtherNetCCD || BAR_CODE_INSTALL==ebcUseOCR) && //Steven 20260420 : add OCR HandShake timeout
                           CosFunction.bUseHandShakeCommunication &&
                           TestIF_File.bUseHandShakeCommunication==true)
                        {
                            if(bCCDBarcodeWaitReply[iBarCodeRowB]==false)
                            {
                                BarcodeWaitReplyTimeOut[iBarCodeRowB].SetMSAndOn(TestIF_File.i2DHandShakeTimeOut);
                                bCCDBarcodeWaitReply[iBarCodeRowB]=true;
                            }
                        }
                    }
                    bBarcodeNum[iBarCodeRowA]=true;
//                }
//                else                                                            //shuttle 2 單排使用CCD3
//                {
//                    if(BLCarryKit.Item[0][iNowCheckStep]==HAS_NULL_IC ||
//                       BLCarryKit.Item[0][iNowCheckStep]==NULL_IC)
//                    {
//                        bBarcodeNum[iBarCodeRowA]=true;
//                    }
//                    else if(bBarcodeNum[iBarCodeRowA]==false)
//                    {
//                        if(BAR_CODE_INSTALL!=ebcUseOCR)
//                            iSH2_1BarcodePosition=iNowCheckStep;
//                        else
//                            iSH2_2BarcodePosition=iNowCheckStep;
//
//                        if(iNowCheckStep==InArmSuck.iShtCol-1)
//                        {
//                            cLastString="(NULL)";
//                        }
//                        else
//                        {
//                            cLastString=BLCarryKit.cDeviceInf[0][iNowCheckStep+1];
//                        }
//                        if(Barcode_StartScan_In(iBarCodeRowA, iNowCheckStep, BLCarryKit.cDeviceInf[0][iNowCheckStep], cLastString)==true)
//                        {
//                            bBarcodeStartDelay[iBarCodeRowA]=true;
//                            bBarcodeDataSaveReady[iBarCodeRowA]=false;
//                            bBarcodeNum[iBarCodeRowA]=true;
//                        }
//
//                        if(BAR_CODE_INSTALL==ebctEtherNetCCD &&                 //Ifor 20190225 :add Bar Code Use HandShake Communication
//                           CosFunction.bUseHandShakeCommunication &&
//                           TestIF_File.bUseHandShakeCommunication==true)
//                        {
//                            if(bCCDBarcodeWaitReply[iBarCodeRowA]==false)
//                            {
//                                BarcodeWaitReplyTimeOut[iBarCodeRowA].SetMSAndOn(TestIF_File.i2DHandShakeTimeOut);
//                                bCCDBarcodeWaitReply[iBarCodeRowA]=true;
//                            }
//                        }
//                    }
//                    bBarcodeNum[iBarCodeRowB]=true;
//                }
            }

            if(bBarcodeNum[iBarCodeRowA]==true &&
               bBarcodeNum[iBarCodeRowB]==true)
            {
                Task=1200;
                bCCDBarcodeWaitReply[iBarCodeRowA]=false;
                bCCDBarcodeWaitReply[iBarCodeRowB]=false;
            }
            else if((BAR_CODE_INSTALL==ebctEtherNetCCD || BAR_CODE_INSTALL==ebcUseOCR) && //Steven 20260420 : add OCR HandShake timeout
                    CosFunction.bUseHandShakeCommunication &&
                    TestIF_File.bUseHandShakeCommunication==true)
            {
                if(BarcodeWaitReplyTimeOut[iBarCodeRowB].Off())
                {
                    Task=1180;
                    bCCDBarcodeWaitReply[iBarCodeRowB]=false;
                }

                if(InArmSuck.iShtRow==2 &&  BarcodeWaitReplyTimeOut[iBarCodeRowA].Off())
                {
                    Task=1180;
                     bCCDBarcodeWaitReply[iBarCodeRowA]=false;
                }
                break;
            }
            //==> Eastsun 20260526 #026-4.P1 Pin1 Inspection dispatch :KYEC
            else if(CosFunction.b2DUsePinInspection && TestIF_File.b2DUsePinInspection)     //Ifor 20240624 add: Pin1 mode dispatch Task=1180
            {
                if(bHasPin1Error[iBarCodeRowA] || bHasPin1Error[iBarCodeRowB])
                {
                    Task=1180;
                }
            }
            //<== Eastsun 20260526 #026-4.P1
            else if(BarcodeDelay[iSht].Off())
            {
                if(iRetryCount<TestIF_File.iBarcodeRetryCount)                  //Steven 20151221 : 讀不到Code時,自動Retry的次數
                {
                    if(iRetryCount==0)
                    {
                        if(bBarcodeNum[iBarCodeRowB]==false &&
                           bBarcodeFirstAutoRetry[iBarCodeRowB]==false)
                        {
                            iBarcodeAutoRetry[iBarCodeRowB]++;
                            bBarcodeFirstAutoRetry[iBarCodeRowB]=true;
                        }

                        if(bBarcodeNum[iBarCodeRowA]==false &&
                           bBarcodeFirstAutoRetry[iBarCodeRowA]==false)
                        {
                            iBarcodeAutoRetry[iBarCodeRowA]++;
                            bBarcodeFirstAutoRetry[iBarCodeRowA]=true;
                        }
                    }

                    InitSht2DCodeComPort(iSht);                                 //Ifor 20160322 Retry 時重置
                    iRetryCount++;
                    iSH2_1BarcodePosition=iNowCheckStep;
                    iSH2_2BarcodePosition=iNowCheckStep;
                    CleanBarcodeError(2);
                    if(TestIF_File.bRetryOffsetMove)                            //wei 20161116 Retry時先退出再進去讀取
                        Task=1160;
                    else
                        Task=1100;
                    return false;
                }
                else if(bBarcodeAutoSkip[iBarCodeRowA]==true ||
                        bBarcodeAutoSkip[iBarCodeRowB]==true)                   //wei Barcode Lot 錯誤取出IC，需重新讀取確認是否取出IC
                {
                    if(InArmSuck.iShtRow==2)
                    {
                        if(bBarcodeAutoSkip[iBarCodeRowA])
                        {
                            BLCarryKit.SetItemData(0, iNowCheckStep, HAS_NULL_IC);     //wei 20160823  Skip就設為NHAS_NULL_IC
                        }
                        bBarcodeAutoSkip[iBarCodeRowA]=false;

                        if(bBarcodeAutoSkip[iBarCodeRowB])
                        {
                            BLCarryKit.SetItemData(1, iNowCheckStep, HAS_NULL_IC);     //wei 20160823  Skip就設為NHAS_NULL_IC
                        }
                        bBarcodeAutoSkip[iBarCodeRowB]=false;
                    }
                    else
                    {
                        if(bBarcodeAutoSkip[iBarCodeRowB])
                        {
                            BLCarryKit.SetItemData(0, iNowCheckStep, HAS_NULL_IC);     //wei 20160823  Skip就設為NHAS_NULL_IC
                        }
                        bBarcodeAutoSkip[iBarCodeRowB]=false;
                    }

                    InitSht2DCodeComPort(iSht);                                 //Ifor 20160322 Retry 時重置
                    iSH2_1BarcodePosition=iNowCheckStep;
                    iSH2_2BarcodePosition=iNowCheckStep;
                    CleanBarcodeError(2);
                    Task=1100;
                    return false;
                }
                else
                {
                    Task=1170;                                                  //wei 20160823 Shuttle移出來在alarm    //Frank 20171011 (Steven) 1180-->1170 增加Save Fail Image
                }
            }
            break;
        case 1160:                                                              //wei 20161116 Retry時先退出再進去讀取
            if(iNowCheckStep>=0 && iNowCheckStep<InArmSuck.iShtCol)
            {
                if(InArmSuck.iShtCol==1)
                {
                    pos=Prod.iInSHBarCodeDetectPos1x1[1];
                }
                else if(InArmSuck.iShtCol==2)
                {
                    pos=Prod.iInSHBarCodeDetectPos1x2[1][iNowCheckStep];
                }
                else if(InArmSuck.iShtCol==3)                                   //ChungHung 20140115 add for 2x3_6
                {
                    pos=Prod.iInSHBarCodeDetectPos2x3[1][iNowCheckStep];
                }
                else if(InArmSuck.iShtCol==4)
                {
                    pos=Prod.iInSHBarCodeDetectPos1x4[1][iNowCheckStep];
                }
                else if(InArmSuck.iShtCol==5)                                   //Steven 20221027 : Add for 2x5
                {
                    pos=Prod.iInSHBarCodeDetectPos2x5[1][iNowCheckStep];
                }
                else if(InArmSuck.iShtCol==6)                                   //Steven 20170111 : fixed for 2x6
                {
                    pos=Prod.iInSHBarCodeDetectPos2x6[1][iNowCheckStep];
                }
                else if(InArmSuck.iShtCol==8)
                {
                    pos=Prod.iInSHBarCodeDetectPos2x8[1][iNowCheckStep];
                }
                else
                {
                    ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "DoBarcodeScanInShuttle_2");
                }

                pos+=Offset.iSHLeft2D[1];                                       //Steven 20151218 : Offset for 2d reader

                pos+=(TestIF_File.dRetryOffsetMove*100);

                if(MOT[MInShuttle2].MotorMove(pos))
                {
                    Task=1100;
                    return false;
                }
            }
            break;
        case 1170:
            if(TestIF_File.bSaveFailImage)                                      //Frank 20171011 (Steven) 1180-->1170 增加Save Fail Image
            {
                for(int i=2; i<4; i++)
                {
                    if(bBarcodeNum[i]==false)
                        bImageSaveflag[i]=true;

                    if(bImageSaveflag[i]==true)
                        btSaveImage->Click();
                }
                Task=1175;
            }
            else
            {
                Task=1180;
            }
            break;
        case 1175:
            if(bImageSaveflag[iBarCodeRowA]==false &&
               bImageSaveflag[iBarCodeRowB]==false)
            {
                Task=1180;
            }
            else
            {
                fMain->ShowNowStatus(clNavy, "Save Fail Image processing...");
            }
            break;
        case 1180:
            bMoveOK=false;
            if(BAR_CODE_INSTALL==ebcUseOCR)                                     //Ifor 20200421 add:OCR Alarm 不移動回Loader區
            {
                bMoveOK=true;
            }
            else
            {
                if(MOT[MInShuttle2].MotorMove(Prod.InSHT[iSht].iLeft))
                {
                    bMoveOK=true;
                }
            }

            if(bMoveOK==true)
            {
                TurnOffInspection(iBarCodeRowA);
                TurnOffInspection(iBarCodeRowB);
                #ifdef SOFT_SIMULTE
                ret=K_SKIP;
                #else
                if(LastSet.iRealDummy==DUMMY)
                {
                    ret=K_SKIP;
                }
                //==> Eastsun 20260526 #026-4.B2 Pin1 Inspection Sh2 :KYEC
                else if(CosFunction.b2DUsePinInspection && TestIF_File.b2DUsePinInspection)     //Ifor 20240624 add:shuttle 2 Pin 1 inspection Error!
                {
                    if(bHasPin1Error[iBarCodeRowA]==false)
                        sErrorPart+=IndexSuckName[0][iNowCheckStep];
                    if(bHasPin1Error[iBarCodeRowB]==false)
                        sErrorPart+=IndexSuckName[1][iNowCheckStep];
                    if(bErrorSkip)                                              //Eastsun 20260327 KYEC pin1要求Contact 可以skip 正常做不能skip
                    {
                        ret=ShowErrorMessage("WAR04207", K_RETRY|K_SKIP, MInShuttle2, false, sErrorPart);      //In Shuttle2 Barcode Error
                    }
                    else
                    {
                        ret=ShowErrorMessage("WAR04207", K_RETRY, MInShuttle2, false, sErrorPart);      //In Shuttle2 Barcode Error
                    }
                }
                //<== Eastsun 20260526 #026-4.B2
                else
                {
                    if(TestIF_File.bNoCodeDeviceAutoSkip==true &&
                       fContact->IsRun2DCheck()==false)                         //JerryYang 20250220 : 2DID硬體順序檢查功能                 //Steven 20151221 : 讀不到Code時,自動Skip跳下一顆
                    {
                        ret=K_SKIP;
                    }
                    else if(bBarcodeAutoSkipError[iBarCodeRowA]==true ||
                            bBarcodeAutoSkipError[iBarCodeRowB]==true)          //wei Barcode Lot 錯誤取出IC，需重新讀取確認是否取出IC
                    {
                        sCheckErrorPart="";
                        if(InArmSuck.iShtRow==2)
                        {
                            if(bBarcodeAutoSkipError[iBarCodeRowA])             //wei 20160505 Barcode 比對Lot
                            {
                                sCheckErrorPart+=IndexSuckName[0][iNowCheckStep];
                            }

                            if(bBarcodeAutoSkipError[iBarCodeRowB])             //wei 20160505 Barcode 比對Lot
                            {
                                sCheckErrorPart+=IndexSuckName[1][iNowCheckStep];
                            }
                        }
                        else
                        {
                            if(bBarcodeAutoSkipError[iBarCodeRowB])             //wei 20160505 Barcode 比對Lot
                            {
                                sCheckErrorPart+=IndexSuckName[0][iNowCheckStep];
                            }
                        }
                        ret=ShowErrorMessage("WAR0477", K_SKIP, MInShuttle2, false, sCheckErrorPart);      //In Shuttle1 Barcode Error
                    }
                    else if(TestIF_File.bCheckCodeByLot &&
                            (bCheckCodeError[iBarCodeRowA] ||
                             bCheckCodeError[iBarCodeRowB]))
                    {
                        sCheckErrorPart="";
                        if(InArmSuck.iShtRow==2)
                        {
                            if(bCheckCodeError[iBarCodeRowA])           //wei 20160505 Barcode 比對Lot
                            {
                                sCheckErrorPart+=IndexSuckName[0][iNowCheckStep];
                            }

                            if(bCheckCodeError[iBarCodeRowB])           //wei 20160505 Barcode 比對Lot
                            {
                                sCheckErrorPart+=IndexSuckName[1][iNowCheckStep];
                            }
                        }
                        else
                        {
                            if(bCheckCodeError[iBarCodeRowB])           //wei 20160505 Barcode 比對Lot
                            {
                                sCheckErrorPart+=IndexSuckName[0][iNowCheckStep];
                            }
                        }
                        ret=ShowErrorMessage("WAR0468", K_RETRY|K_SKIP, MInShuttle2, false, sCheckErrorPart);      //In Shuttle1 Barcode Error  //Alick 20170126 (wei) 拿掉SKIP，避免跑錯LOT //Steven 20250808 : OCR add K_SKIP for WAR0468
                    }
                    else if(TestIF_File.bCheckLotHaveCode &&
                            (bCheckLotError[iBarCodeRowA] ||
                             bCheckLotError[iBarCodeRowB]))
                    {
                        sCheckErrorPart="";
                        if(InArmSuck.iShtRow==2)
                        {
                            if(bCheckLotError[iBarCodeRowA])                    //wei 20160505 Barcode 比對Lot
                            {
                                sCheckErrorPart+=IndexSuckName[0][iNowCheckStep];
                            }

                            if(bCheckLotError[iBarCodeRowB])                    //wei 20160505 Barcode 比對Lot
                            {
                                sCheckErrorPart+=IndexSuckName[1][iNowCheckStep];
                            }
                        }
                        else
                        {
                            if(bCheckLotError[iBarCodeRowB])                    //wei 20160505 Barcode 比對Lot
                            {
                                sCheckErrorPart+=IndexSuckName[0][iNowCheckStep];
                            }
                        }

                        sCheckErrorPart+=asBarcodeLotNumber;                    //wei Barcode Lot Alarm時，顯示出Lot
                        bBarcodePassword=true;                                  //wei 20160823  Lot check 錯誤需輸入密碼
                        ret=ShowErrorMessage("WAR0472", K_RETRY, MInShuttle2, false, sCheckErrorPart);      //In Shuttle1 Barcode Error  //Alick 20170202 (wei) remove skip
                    }
                    else
                    {
                        if(InArmSuck.iShtRow==2)
                        {
                            if(bBarcodeNum[iBarCodeRowA]==false)
                                sErrorPart+=IndexSuckName[0][iNowCheckStep];
                            if(bBarcodeNum[iBarCodeRowB]==false)
                                sErrorPart+=IndexSuckName[1][iNowCheckStep];
                        }
                        else
                        {
                            if(bBarcodeNum[iBarCodeRowB]==false)
                                sErrorPart+=IndexSuckName[0][iNowCheckStep];
                        }

                        if(fContact->IsRun2DCheck()==true)                      //JerryYang 20250220 : 2DID硬體順序檢查功能
                        {
                            ret=ShowErrorMessage("JAM0496", K_RETRY, MInShuttle2, false, sErrorPart);      //In Shuttle2 Barcode Error
                        }
                        else
                        {
                            ret=ShowErrorMessage("JAM0461", K_RETRY|K_SKIP, MInShuttle2, false, sErrorPart);      //In Shuttle2 Barcode Error
                        }
                    }
                    iRetryCount=-1;
                }
                #endif
                if(ret==K_RETRY)
                {
                    InitSht2DCodeComPort(iSht);                                 //Ifor 20160322 Retry 時重置
                    iRetryCount=0;
                    iSH2_1BarcodePosition=iNowCheckStep;
                    iSH2_2BarcodePosition=iNowCheckStep;
                    CleanBarcodeError(2);
//                    Task=1100;
                    Task=1;
                    bErrorRetry=true;
                    return false;
                }
                else
                {
                    iRetryCount=0;
                    Path.sprintf("%s\\%04d_%02d_%02d", asBarCodeLogPath, SystemYear, SystemMonth, SystemDate);
                    MyForceDirectories(Path);
                    Data.sprintf("%04d_%02d_%02d  %02d_%02d_%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
                    Path.sprintf("%s\\%04d_%02d_%02d\\%04d_%02d_%02d_%02d.txt", asBarCodeLogPath, SystemYear, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate , SystemHour);

                    if(TestIF_File.bCheckCodeByLot && (bCheckCodeError[iBarCodeRowA] || bCheckCodeError[iBarCodeRowB]))
                    {
                        if(InArmSuck.iShtRow==2)
                        {
                            if(bCheckCodeError[iBarCodeRowA])
                            {
                                DataStr.sprintf("%s InShuttle2-1 %d Site%d : %s", Data.c_str(), iSH2_1BarcodePosition, TestIF_File.iSiteMap[0][iSH2_1BarcodePosition], asBarCodeErrorSend.c_str()); //wei 20160325 Barcode 紀錄位置
                                WriteDataToFile(Path.c_str() , DataStr.c_str());
                                BLCarryKit.cDeviceInf[0][iNowCheckStep]=asBarCodeErrorSend;         //wei 20160318 Barcode Error依客戶設定
                                mtBarcodeInSh->SetCellNumber(iBarCodeRowA, iNowCheckStep+1, asBarCodeErrorSend.c_str());

                                if(TestIF_File.i2DIDFormat==eAMD)                   //JerryYang 20200422 2DID format選項改用下拉選單
                                {
                                    fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(1,  iNowCheckStep+1, asBarCodeErrorSend.c_str());
                                }
                                iBarcodeErrorCount[iBarCodeRowA]++;
                                bBarcodeNum[iBarCodeRowA]=true;
                                bCheckCodeError[iBarCodeRowA]=false;                //Steven 20250808 : for OCR
                            }

                            if(bCheckCodeError[iBarCodeRowB])
                            {
                                DataStr.sprintf("%s InShuttle2-2 %d Site%d : %s", Data.c_str(), iSH2_1BarcodePosition, TestIF_File.iSiteMap[1][iSH2_1BarcodePosition], asBarCodeErrorSend.c_str()); //wei 20160325 Barcode 紀錄位置
                                WriteDataToFile(Path.c_str() , DataStr.c_str());
                                BLCarryKit.cDeviceInf[1][iNowCheckStep]=asBarCodeErrorSend;         //wei 20160318 Barcode Error依客戶設定
                                mtBarcodeInSh->SetCellNumber(iBarCodeRowB, iNowCheckStep+1, asBarCodeErrorSend.c_str());

                                iBarcodeErrorCount[iBarCodeRowB]++;
                                bBarcodeNum[iBarCodeRowB]=true;
                                bCheckCodeError[iBarCodeRowB]=false;                //Steven 20250808 : for OCR
                            }
                        }
                        else
                        {
                            if(bCheckCodeError[iBarCodeRowB])
                            {
                                DataStr.sprintf("%s InShuttle2-1 %d Site%d : %s", Data.c_str(), iSH2_1BarcodePosition, TestIF_File.iSiteMap[0][iSH2_1BarcodePosition], asBarCodeErrorSend.c_str()); //wei 20160325 Barcode 紀錄位置
                                WriteDataToFile(Path.c_str() , DataStr.c_str());
                                BLCarryKit.cDeviceInf[0][iNowCheckStep]=asBarCodeErrorSend;         //wei 20160318 Barcode Error依客戶設定
                                mtBarcodeInSh->SetCellNumber(iBarCodeRowB, iNowCheckStep+1, asBarCodeErrorSend.c_str());

                                if(TestIF_File.i2DIDFormat==eAMD)                   //JerryYang 20200422 2DID format選項改用下拉選單
                                {
                                    fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(1,  iNowCheckStep+1, asBarCodeErrorSend.c_str());
                                }
                                iBarcodeErrorCount[iBarCodeRowB]++;
                                bBarcodeNum[iBarCodeRowB]=true;
                                bCheckCodeError[iBarCodeRowB]=false;                //Steven 20250808 : for OCR
                            }
                        }

                        if(bBarcodeNum[iBarCodeRowA]==false || bBarcodeNum[iBarCodeRowB]==false)
                        {
                            InitSht2DCodeComPort(iSht);                         //Ifor 20160322 Retry 時重置
                            iSH2_1BarcodePosition=iNowCheckStep;
                            iSH2_2BarcodePosition=iNowCheckStep;
                            CleanBarcodeError(2);
                            Task=1100;
                            return false;
                        }
                    }
                    else if((TestIF_File.bCheckLotHaveCode &&
                            (bCheckLotError[iBarCodeRowA] || bCheckLotError[iBarCodeRowB])) ||
                            (bBarcodeAutoSkipError[iBarCodeRowA] || bBarcodeAutoSkipError[iBarCodeRowB]))    //wei Barcode Lot 錯誤取出IC，需重新讀取確認是否取出IC
                    {
                        if(InArmSuck.iShtRow==2)
                        {
                            if(bCheckLotError[iBarCodeRowA] ||
                               bBarcodeAutoSkipError[iBarCodeRowA])
                            {
                                DataStr.sprintf("%s InShuttle2-1 %d Site%d : %s", Data.c_str(), iSH2_1BarcodePosition, TestIF_File.iSiteMap[0][iSH2_1BarcodePosition], asBarCodeErrorSend.c_str()); //wei 20160325 Barcode 紀錄位置
                                WriteDataToFile(Path.c_str() , DataStr.c_str());
                                BLCarryKit.cDeviceInf[0][iNowCheckStep]=asBarCodeErrorSend;         //wei 20160318 Barcode Error依客戶設定
                                mtBarcodeInSh->SetCellNumber(iBarCodeRowA, iNowCheckStep+1, asBarCodeErrorSend.c_str());
                                if(TestIF_File.i2DIDFormat==eAMD)                   //JerryYang 20200422 2DID format選項改用下拉選單
                                {
                                    fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(1,  iNowCheckStep+1, asBarCodeErrorSend.c_str());
                                }

                                iBarcodeErrorCount[iBarCodeRowA]++;
                                bBarcodeAutoSkip[iBarCodeRowA]=true;
                            }
                            bBarcodeNum[iBarCodeRowA]=false;
                            bBarcodeAutoSkipError[iBarCodeRowA]=false;

                            if(bCheckLotError[iBarCodeRowB] ||
                               bBarcodeAutoSkipError[iBarCodeRowB])
                            {
                                DataStr.sprintf("%s InShuttle2-2 %d Site%d : %s", Data.c_str(), iSH2_1BarcodePosition, TestIF_File.iSiteMap[0][iSH2_1BarcodePosition], asBarCodeErrorSend.c_str()); //wei 20160325 Barcode 紀錄位置
                                WriteDataToFile(Path.c_str() , DataStr.c_str());
                                BLCarryKit.cDeviceInf[0][iNowCheckStep]=asBarCodeErrorSend;         //wei 20160318 Barcode Error依客戶設定
                                mtBarcodeInSh->SetCellNumber(iBarCodeRowB, iNowCheckStep+1, asBarCodeErrorSend.c_str());
                                iBarcodeErrorCount[iBarCodeRowB]++;
                                bBarcodeAutoSkip[iBarCodeRowB]=true;
                            }
                            bBarcodeNum[iBarCodeRowB]=false;
                            bBarcodeAutoSkipError[iBarCodeRowB]=false;
                        }
                        else
                        {
                            if(bCheckLotError[iBarCodeRowB] ||
                               bBarcodeAutoSkipError[iBarCodeRowB])
                            {
                                DataStr.sprintf("%s InShuttle2-1 %d Site%d : %s", Data.c_str(), iSH2_1BarcodePosition, TestIF_File.iSiteMap[0][iSH2_1BarcodePosition], asBarCodeErrorSend.c_str()); //wei 20160325 Barcode 紀錄位置
                                WriteDataToFile(Path.c_str() , DataStr.c_str());
                                BLCarryKit.cDeviceInf[0][iNowCheckStep]=asBarCodeErrorSend;         //wei 20160318 Barcode Error依客戶設定
                                mtBarcodeInSh->SetCellNumber(iBarCodeRowB, iNowCheckStep+1, asBarCodeErrorSend.c_str());
                                if(TestIF_File.i2DIDFormat==eAMD)                   //JerryYang 20200422 2DID format選項改用下拉選單
                                {
                                    fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(1,  iNowCheckStep+1, asBarCodeErrorSend.c_str());
                                }

                                iBarcodeErrorCount[iBarCodeRowB]++;
                                bBarcodeAutoSkip[iBarCodeRowB]=true;
                            }
                            bBarcodeNum[iBarCodeRowB]=false;
                            bBarcodeAutoSkipError[iBarCodeRowB]=false;
                        }

                        if(bBarcodeNum[iBarCodeRowA]==false || bBarcodeNum[iBarCodeRowB]==false)
                        {
                            InitSht2DCodeComPort(iSht);                         //Ifor 20160322 Retry 時重置
                            iSH2_1BarcodePosition=iNowCheckStep;
                            iSH2_2BarcodePosition=iNowCheckStep;
                            CleanBarcodeError(2);
                            Task=1100;
                            return false;
                        }
                    }
                    else
                    {
                        if(InArmSuck.iShtRow==2)
                        {
                            if(bBarcodeNum[iBarCodeRowA]==false)
                            {
                                DataStr.sprintf("%s InShuttle2-1 %d Site%d : %s", Data.c_str(), iSH2_1BarcodePosition, TestIF_File.iSiteMap[0][iSH2_1BarcodePosition], asBarCodeErrorSend.c_str()); //wei 20160325 Barcode 紀錄位置
                                WriteDataToFile(Path.c_str() , DataStr.c_str());
                                BLCarryKit.cDeviceInf[0][iNowCheckStep]=asBarCodeErrorSend;         //wei 20160318 Barcode Error依客戶設定
                                mtBarcodeInSh->SetCellNumber(iBarCodeRowA, iNowCheckStep+1, asBarCodeErrorSend.c_str());
                                if(TestIF_File.i2DIDFormat==eAMD)                   //JerryYang 20200422 2DID format選項改用下拉選單
                                {
                                    fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(1,  iNowCheckStep+1, asBarCodeErrorSend.c_str());
                                }
                                iBarcodeErrorCount[iBarCodeRowA]++;
                                iBarcodeReject++;                                   //wei 20160823  Consecutive Failure
                            }

                            if(bBarcodeNum[iBarCodeRowB]==false)
                            {
                                DataStr.sprintf("%s InShuttle2-2 %d Site%d : %s", Data.c_str(), iSH2_2BarcodePosition, TestIF_File.iSiteMap[1][iSH2_2BarcodePosition], asBarCodeErrorSend.c_str()); //wei 20160325 Barcode 紀錄位置 //jou 2016-07-07 修正2D debug log記錄錯誤 TestIF_File.iSiteMap[0][iSH2_2BarcodePosition] -> TestIF_File.iSiteMap[1][iSH2_2BarcodePosition]
                                WriteDataToFile(Path.c_str() , DataStr.c_str());
                                BLCarryKit.cDeviceInf[1][iNowCheckStep]=asBarCodeErrorSend;         //wei 20160318 Barcode Error依客戶設定
                                mtBarcodeInSh->SetCellNumber(iBarCodeRowB, iNowCheckStep+1, asBarCodeErrorSend.c_str());
                                iBarcodeErrorCount[iBarCodeRowB]++;
                                iBarcodeReject++;                                   //wei 20160823  Consecutive Failure
                            }
                        }
                        else
                        {
                            if(bBarcodeNum[iBarCodeRowB]==false)
                            {
                                DataStr.sprintf("%s InShuttle2-1 %d Site%d : %s", Data.c_str(), iSH2_1BarcodePosition, TestIF_File.iSiteMap[0][iSH2_1BarcodePosition], asBarCodeErrorSend.c_str()); //wei 20160325 Barcode 紀錄位置
                                WriteDataToFile(Path.c_str() , DataStr.c_str());
                                BLCarryKit.cDeviceInf[0][iNowCheckStep]=asBarCodeErrorSend;         //wei 20160318 Barcode Error依客戶設定
                                mtBarcodeInSh->SetCellNumber(iBarCodeRowB, iNowCheckStep+1, asBarCodeErrorSend.c_str());
                                if(TestIF_File.i2DIDFormat==eAMD)                   //JerryYang 20200422 2DID format選項改用下拉選單
                                {
                                    fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(1,  iNowCheckStep+1, asBarCodeErrorSend.c_str());
                                }
                                iBarcodeErrorCount[iBarCodeRowB]++;
                                iBarcodeReject++;                                   //wei 20160823  Consecutive Failure
                            }
                        }
                    }

                    if(TestIF_File.bEnableConsecutiveFailure && iBarcodeReject>=TestIF_File.iConsecutiveFailure)
                    {
                        ShowErrorMessage("WAR0475", K_SKIP, MInShuttle2, false);
                        iBarcodeReject=0;
                    }
                    bBarcodePassword=false;                                     //wei 20160823  Lot check 錯誤需輸入密碼
                    Task=1200;
                }
            }
            break;
        case 1200:
            //==> Eastsun 20260526 #026-4.PinN.Sh2-5 Pin1 skip dup-check :KYEC
            if(CosFunction.b2DUsePinInspection && TestIF_File.b2DUsePinInspection)
            {
                Task=1300;
                break;
            }
            //<== Eastsun 20260526 #026-4.PinN.Sh2-5
            for(int i=0; i<InArmSuck.iShtRow; i++)                              //20161123 Frank 同一個Shuttle上有兩個一樣的ID的時候
            {
                for(int j=0; j<InArmSuck.iShtCol; j++)
                {
                    asString=mtBarcodeInSh->GetCellText(i+2, j+1);
                    if(asString!="" &&
                       asString!=asBarCodeErrorSend &&
                       asString!=asBarCodeErrorCheckSum)                        //KaiHuang 20200826 : Add CheckSum
                    {
                        if(InArmSuck.iShtRow==1)
                        {
                            if(BLCarryKit.cDeviceInf[0][iNowCheckStep]==asString && (i!=0 || j!=iNowCheckStep))
                            {
                                bCheckCodeError[iBarCodeRowB]=true;
                            }
                        }
                        else
                        {
                            if(BLCarryKit.cDeviceInf[0][iNowCheckStep]==asString && (i!=0 || j!=iNowCheckStep))
                            {
                                bCheckCodeError[iBarCodeRowA]=true;
                            }

                            if(BLCarryKit.cDeviceInf[1][iNowCheckStep]==asString && (i!=1 || j!=iNowCheckStep))
                            {
                                bCheckCodeError[iBarCodeRowB]=true;
                            }
                        }
                    }
                }
            }
            #ifndef SOFT_SIMULTE
            if(InArmSuck.iShtRow==2)
            {
                if(fBarCode->RunCheckBarcodeByServerData()==true &&                 //Jimmychiu 20230925 : read 2did in json file
                   fMesSystem->IsMatchServerData(BLCarryKit.cDeviceInf[0][iNowCheckStep])==false)
                {
                    bCheckCodeError[iBarCodeRowA]=true;
                }

                if(fBarCode->RunCheckBarcodeByServerData()==true &&                 //Jimmychiu 20230925 : read 2did in json file
                   fMesSystem->IsMatchServerData(BLCarryKit.cDeviceInf[1][iNowCheckStep])==false)
                {
                    bCheckCodeError[iBarCodeRowB]=true;
                }
            }
            else
            {
                if(fBarCode->RunCheckBarcodeByServerData()==true &&                 //Jimmychiu 20230925 : read 2did in json file
                   fMesSystem->IsMatchServerData(BLCarryKit.cDeviceInf[0][iNowCheckStep])==false)
                {
                    bCheckCodeError[iBarCodeRowB]=true;
                }
            }
            #endif
            if(bCheckCodeError[iBarCodeRowB] || bCheckCodeError[iBarCodeRowA])
            {
                Task=1180;
            }
            else
            {
                Task=1300;
            }
            break;
        case 1300:
            bBarcodeNum[iBarCodeRowA]=false;
            bBarcodeNum[iBarCodeRowB]=false;
            iRetryCount=0;
            iNowCheckStep--;
            bBarcodeFirstAutoRetry[iBarCodeRowA]=false;
            bBarcodeFirstAutoRetry[iBarCodeRowB]=false;

            if(iNowCheckStep>=0 && iNowCheckStep<InArmSuck.iShtCol)
            {
                Task=1000;
                break;
            }
            else
            {
                Task=2000;
            }
//            break;
        case 2000:
            if(TestIF_File.bCheckCodeByLot && fContact->IsRun2DCheck()==false)   //把新的值加到List裡面  //JerryYang 20250428 : fix 2DID map
            {
                list2DByLot->Clear();
                for(int i=0; i<InArmSuck.iShtCol; i++)
                {
                    if(BLCarryKit.Item[0][i]!=HAS_NULL_IC && BLCarryKit.Item[0][i]!=NULL_IC &&
                       BLCarryKit.cDeviceInf[0][i]!=asBarCodeErrorSend && BLCarryKit.cDeviceInf[0][i]!="" &&
                       BLCarryKit.cDeviceInf[0][i]!=asBarCodeErrorCheckSum)     //KaiHuang 20200826 : Add CheckSum
                    {
                        list2DByLot->Add(BLCarryKit.cDeviceInf[0][i]);
                        BLCarryKit.PordRec[0][i].Add2DIDRecord(BLCarryKit.cDeviceInf[0][i]);
                    }

                    if(InArmSuck.iShtRow==2)
                    {
                        if(BLCarryKit.Item[1][i]!=HAS_NULL_IC && BLCarryKit.Item[1][i]!=NULL_IC &&
                           BLCarryKit.cDeviceInf[1][i]!=asBarCodeErrorSend && BLCarryKit.cDeviceInf[1][i]!="" &&
                           BLCarryKit.cDeviceInf[1][i]!=asBarCodeErrorCheckSum) //KaiHuang 20200826 : Add CheckSum
                        {
                            list2DByLot->Add(BLCarryKit.cDeviceInf[1][i]);
                            BLCarryKit.PordRec[1][i].Add2DIDRecord(BLCarryKit.cDeviceInf[1][i]);
                        }
                    }
                }
                WriteDataToFile(asBarCodeLot, list2DByLot->Text);               //Steven 20160429 : 開程式要把2D List讀回來
            }
            else
            {
                for(int i=0; i<InArmSuck.iShtCol; i++)
                {
                    if(BLCarryKit.Item[0][i]!=HAS_NULL_IC && BLCarryKit.Item[0][i]!=NULL_IC &&
                       BLCarryKit.cDeviceInf[0][i]!=asBarCodeErrorSend && BLCarryKit.cDeviceInf[0][i]!="")
                    {
                        BLCarryKit.PordRec[0][i].Add2DIDRecord(BLCarryKit.cDeviceInf[0][i]);
                    }

                    if(InArmSuck.iShtRow==2)
                    {
                        if(BLCarryKit.Item[1][i]!=HAS_NULL_IC && BLCarryKit.Item[1][i]!=NULL_IC &&
                           BLCarryKit.cDeviceInf[1][i]!=asBarCodeErrorSend && BLCarryKit.cDeviceInf[1][i]!="")
                        {
                            BLCarryKit.PordRec[1][i].Add2DIDRecord(BLCarryKit.cDeviceInf[1][i]);
                        }
                    }
                }
            }

            Task=3000;
            break;
        case 3000:
            if(IniConfig.bKoreaFunction==true)
            {
                if(fBarCode->DoCheckSHT2DIDStatus(1)==true)
                {
                    Task=5000;                                                  //rescan    //Isaac 20200206 : 2DID All site fail recheck twice and alarm
                    break;
                }
            }
            Task=3100;
        case 3100:
            DoBarcodeCount();                                                   //wei 20160318
            Task=1;
            return true;
        case 5000:                                                              //Isaac 20200206 : 2DID All site fail recheck twice and alarm
            if(IniConfig.bF21InOutArmZMotorPrivate)
            {
                if(DoInOutARM_SHT_MoveSafe(1))                                  //kevin 20161005 SHUTTLE 1 移動安全保護
                    return false;
            }

            MOT[MInShuttle2].fCanMoveR=true;
            if(MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iLeft))
            {
                Task=1;
                i2DIDRetryCTSH2++;
                if(i2DIDRetryCTSH2>1)
                {
                    i2DIDRetryCTSH2=0;
                    ret=ShowMyMessageBox_YES_SKIP("All sites 2DID Error On Shuttle 2!! Please check!", "Retry Again?");
                    if(ret==1)                                                  //Yes
                    {
                        Task=1;
                        break;
                    }
                    else
                    {
                        DoBarcodeCount();                                       //wei 20160318
                        Task=1;
                        return true;
                    }
                }
            }
            break;
    }
    return false;
}
//---------------------------------------------------------------------------
bool TfBarCode::DoBarcodeScanOutShuttle_2()
{
    static int iNowCheckStep=0;
    static bool bBarcodeNum[BAR_CODE_COUNT]={false};

    int &Task=iInitialBarcodeOutShuttle2Task;
    int pos=0;

    switch(Task)
    {
        case 1:
            iNowCheckStep=InArmSuck.iShtCol-1;                                  //遞減方式檢查
            bBarcodeStartDelay[iBarCode2_1]=true;
            if(InArmSuck.iShtRow==2)
                bBarcodeStartDelay[iBarCode2_2]=true;
            Task=1000;
//            break;
        case 1000:
            if(iNowCheckStep>=0 && iNowCheckStep<InArmSuck.iShtCol)
            {
                if(InArmSuck.iShtCol==1)
                {
                    pos=Prod.iOutSHBarCodeDetectPos1x1[1];
                }
                else if(InArmSuck.iShtCol==2)
                {
                    pos=Prod.iOutSHBarCodeDetectPos1x2[1][iNowCheckStep];
                }
                else if(InArmSuck.iShtCol==3)                                   //ChungHung 20140115 add for 2x3_6
                {
                    pos=Prod.iOutSHBarCodeDetectPos2x3[1][iNowCheckStep];
                }
                else if(InArmSuck.iShtCol==4)
                {
                    pos=Prod.iOutSHBarCodeDetectPos1x4[1][iNowCheckStep];
                }
                else if(InArmSuck.iShtCol==5)                                   //Steven 20221027 : Add for 2x5
                {
                    pos=Prod.iOutSHBarCodeDetectPos2x5[0][iNowCheckStep];
                }
                else if(InArmSuck.iShtCol==6)
                {
                    pos=Prod.iOutSHBarCodeDetectPos2x6[1][iNowCheckStep];
                }
                else if(InArmSuck.iShtCol==8)
                {
                    pos=Prod.iOutSHBarCodeDetectPos2x8[1][iNowCheckStep];
                }
                else
                {
                    ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "DoBarcodeScanOutShuttle_2");
                }

                if(MOT[MInShuttle2].MotorMove(pos))
                {
                    bBarcodeNum[iBarCode2_1]=false;
                    bBarcodeNum[iBarCode2_2]=false;
                    Task=1100;
                }
            }
            else
            {
                Task=2000;
            }
            break;
        case 1100:                                                              //雙排時,Shuttle 2, Row A 使用第4隻CCD; Row B 使用第3隻CCD
            if(InArmSuck.iShtRow==2)
            {
                if(BRCarryKit.Item[0][iNowCheckStep]==HAS_NULL_IC ||
                   BRCarryKit.Item[0][iNowCheckStep]==NULL_IC)
                {
                    bBarcodeNum[iBarCode2_2]=true;
                }
                else if(bBarcodeNum[iBarCode2_2]==false &&
                        Barcode_1StartScan(3, MOutShuttle2)==true)
                {
                    bBarcodeStartDelay[iBarCode2_2]=true;
                    bBarcodeDataSaveReady[iBarCode2_2]=false;
                    bBarcodeNum[iBarCode2_2]=true;
                }

                if(BRCarryKit.Item[1][iNowCheckStep]==HAS_NULL_IC ||
                   BRCarryKit.Item[1][iNowCheckStep]==NULL_IC)
                {
                    bBarcodeNum[iBarCode2_1]=true;
                }
                else if(bBarcodeNum[iBarCode2_1]==false &&
                        Barcode_1StartScan(2, MOutShuttle2)==true)
                {
                    bBarcodeStartDelay[iBarCode2_1]=true;
                    bBarcodeDataSaveReady[iBarCode2_1]=false;
                    bBarcodeNum[iBarCode2_1]=true;
                }
            }
            else
            {
                if(BRCarryKit.Item[0][iNowCheckStep]==HAS_NULL_IC ||            //單排時,Shuttle 2, Row A 使用第3隻CCD;
                   BRCarryKit.Item[0][iNowCheckStep]==NULL_IC)
                {
                    bBarcodeNum[iBarCode2_1]=true;
                }
                else if(bBarcodeNum[iBarCode2_1]==false &&
                        Barcode_1StartScan(2, MOutShuttle2)==true)
                {
                    bBarcodeStartDelay[iBarCode2_1]=true;
                    bBarcodeDataSaveReady[iBarCode2_1]=false;
                    bBarcodeNum[iBarCode2_1]=true;
                }
                bBarcodeNum[iBarCode2_2]=true;
            }

            if(bBarcodeNum[iBarCode2_1]==true &&
               bBarcodeNum[iBarCode2_2]==true)
            {
                bBarcodeNum[iBarCode2_1]=false;
                bBarcodeNum[iBarCode2_2]=false;
                Task=1200;
            }
            break;
        case 1200:
            iNowCheckStep--;
            if(iNowCheckStep>=0 && iNowCheckStep<InArmSuck.iShtCol)
            {
                Task=1000;
                break;
            }
            else
            {
                Task=2000;
            }
//            break;
        case 2000:
            Task=1;
            return true;
    }
    return false;
}
//---------------------------------------------------------------------------
void TfBarCode::InitialShuttleFloatCheck2()                                     //Steven 20160920 : IC置偏檢查
{
    iShuttleFloatCheck2Task=1;
    bCCDcheckShuttle[1]=true;                                                   //wei 20161102 IC置偏檢查
}
//---------------------------------------------------------------------------
bool TfBarCode::DoShuttleFloatCheck_2()                                         //Steven 20160920 : IC置偏檢查
{
    static int iNowMoveStep=0;
    static int iRetryCount=0;
    static int iRetryCountExpTimo=0;
    static bool bHasError=false;
    static AnsiString ErrPart;
    static AnsiString sErrorPart="";

    int &Task=iShuttleFloatCheck2Task;
    int pos=0, ret;
    int iSht=1;
    int MSht=MInShuttle2;
    int iSFCRowA=iBarCode2_1;
    int iSFCRowB=iBarCode2_2;
    bool bResult=false;
    AnsiString cLastString, sCCDCommand;
    AnsiString Log;

    switch(Task)
    {
        case 1:
            if(BLCarryKit.HasRealIC()==false)
            {
                bResult=true;
                break;
            }
            AddCCDCommunicationLog(iSFCRowA, "<<SFC Cycle, Cycle start.------------------");

            SetSFCCheckStepCount();
            iNowMoveStep=iSFCTotalMoveStep-1;
            iSFCCurrentStep[iSFCRowA]=iNowMoveStep;
            iSFCCurrentStep[iSFCRowB]=iNowMoveStep;
            bSFCStepError[iSFCRowA]=false;
            bSFCStepError[iSFCRowB]=false;
            bSFCExposureOK[iSFCRowA]=true;
            bSFCExposureOK[iSFCRowB]=true;
            bSFCGetResultOK[iSFCRowA]=true;
            bSFCGetResultOK[iSFCRowB]=true;
            iRetryCountExpTimo=0;

            for(int i=0; i<16; i++)
            {
                iSFCCheckResult[iSFCRowA][i]=-1;
                iSFCCheckResult[iSFCRowB][i]=-1;
            }

            bHasError=false;
            if(bCCDcheckShuttle[1])                                             //wei 20161102 IC置偏檢查
            {
                iRetryCount=0;
                bCCDcheckShuttle[1]=false;
            }
            sCCDCommand.sprintf("E9,1,%d", iSFCTotalMoveStep);                  //清空暫存器
            bSFCGetSE9[iSFCRowA]=false;
            bSFCGetSE9[iSFCRowB]=false;
            SendCCDCommand(iSFCRowA, "SFC Clear buffer 1", sCCDCommand);        //Reset CCD Shuttle暫存器
            SendCCDCommand(iSFCRowB, "SFC Clear buffer 1", sCCDCommand);
            SFCStartDelay[iSht].SetMSAndOn(5000);
            Task=500;
        case 500:                                                               //確認Buffer清空
            if(bSFCGetSE9[iSFCRowA] && bSFCGetSE9[iSFCRowB])
            {
                Task=1000;
            }
            else if(SFCStartDelay[iSht].Off())
            {
                ShowErrorMessage("WAR0462", K_RETRY, MSht, false, "SFC Clear buffer");
                Task=1;
            }
            break;
        case 1000:                                                              //移動到拍照位置
            if(TestIF_File.bSFCUse2Photo)
            {
                if(InArmSuck.iShtCol==1)
                {
                    pos=Prod.iInSFCPos1x1_2[iSht][iNowMoveStep];
                }
                else if(InArmSuck.iShtCol==2)
                {
                    pos=Prod.iInSFCPos1x2_2[iSht][iNowMoveStep];
                }
                else if(InArmSuck.iShtCol==3)                                   //ChungHung 20140115 add for 2x3_6
                {
                    pos=Prod.iInSFCPos1x3_2[iSht][iNowMoveStep];
                }
                else if(InArmSuck.iShtCol==4)
                {
                    pos=Prod.iInSFCPos1x4_2[iSht][iNowMoveStep];
                }
                else if(InArmSuck.iShtCol==5)                                   //Steven 20221107 : Add for 2x5
                {
                    pos=Prod.iInSFCPos1x5_2[iSht][iNowMoveStep];
                }
                else if(InArmSuck.iShtCol==6)
                {
                    pos=Prod.iInSFCPos1x6_2[iSht][iNowMoveStep];
                }
                else if(InArmSuck.iShtCol==8)
                {
                    pos=Prod.iInSFCPos1x8_2[iSht][iNowMoveStep];
                }
                else
                {
                    ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "DoShuttleFloatCheck_2");
                }
            }
            else
            {
                if(InArmSuck.iShtCol==1)
                {
                    pos=Prod.iInSFCPos1x1[iSht];
                }
                else if(InArmSuck.iShtCol==2)
                {
                    pos=Prod.iInSFCPos1x2[iSht][iNowMoveStep];
                }
                else if(InArmSuck.iShtCol==3)                                   //ChungHung 20140115 add for 2x3_6
                {
                    pos=Prod.iInSFCPos1x3[iSht][iNowMoveStep];
                }
                else if(InArmSuck.iShtCol==4)
                {
                    pos=Prod.iInSFCPos1x4[iSht][iNowMoveStep];
                }
                else if(InArmSuck.iShtCol==5)                                   //Steven 20221107 : Add for 2x5
                {
                    pos=Prod.iInSFCPos1x5[iSht][iNowMoveStep];
                }
                else if(InArmSuck.iShtCol==6)
                {
                    pos=Prod.iInSFCPos1x6[iSht][iNowMoveStep];
                }
                else if(InArmSuck.iShtCol==8)
                {
                    pos=Prod.iInSFCPos1x8[iSht][iNowMoveStep];
                }
                else
                {
                    ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "DoShuttleFloatCheck_2");
                }
            }

            pos+=Offset.iSHLeft2D[iSht];

            if(MOT[MSht].MotorMove(pos))
            {
                Log.sprintf("SFC Action, Shuttle %d move to step %d", iSht+1, iNowMoveStep);
                AddCCDCommunicationLog(iSFCRowA, Log);
                SFCStartDelay[iSht].SetMSAndOn(TestIF_File.iSFCStartDelay);
                Task=1120;
            }
            break;
        case 1120:
            if(SFCStartDelay[iSht].Off())                                       //到位Delay
            {
                bSFCExposureOK[iSFCRowA]=false;
                if(InArmSuck.iShtRow==2)
                    bSFCExposureOK[iSFCRowB]=false;
                else
                    bSFCExposureOK[iSFCRowB]=true;

                //--------------------------------------
                //SE   1,        1,     X,   1
                //拍照 偵測置偏, 要拍, 位置, 有無IC
                //--------------------------------------
                if(InArmSuck.iShtRow==2)
                {
                    if(BLCarryKit.Item[0][iNowMoveStep]==HAS_NULL_IC || BLCarryKit.Item[0][iNowMoveStep]==NULL_IC)
                        sCCDCommand.sprintf("E1,1,%d,0", iNowMoveStep);
                    else
                        sCCDCommand.sprintf("E1,1,%d,1", iNowMoveStep);
                    mtSFC_Shuttle->SetCellNumber(iSFCRowA, iNowMoveStep+1, "SFC Exposure...");
                    SendCCDCommand(iSFCRowA, "SFC exposure 1120: ", sCCDCommand);

                    if(BLCarryKit.Item[1][iNowMoveStep]==HAS_NULL_IC || BLCarryKit.Item[1][iNowMoveStep]==NULL_IC)
                        sCCDCommand.sprintf("E1,1,%d,0", iNowMoveStep);
                    else
                        sCCDCommand.sprintf("E1,1,%d,1", iNowMoveStep);
                    mtSFC_Shuttle->SetCellNumber(iSFCRowB, iNowMoveStep+1, "SFC Exposure...");
                    SendCCDCommand(iSFCRowB, "SFC exposure 1120: ", sCCDCommand);
                }
                else
                {
                    if(BLCarryKit.Item[0][iNowMoveStep]==HAS_NULL_IC || BLCarryKit.Item[0][iNowMoveStep]==NULL_IC)
                        sCCDCommand.sprintf("E1,1,%d,0", iNowMoveStep);
                    else
                        sCCDCommand.sprintf("E1,1,%d,1", iNowMoveStep);
                    mtSFC_Shuttle->SetCellNumber(iSFCRowB, iNowMoveStep+1, "SFC Exposure...");
                    SendCCDCommand(iSFCRowB, "SFC exposure 1120: ", sCCDCommand);
                }

                Task=1150;
                iRetryCountExpTimo=0;                                           //Steven 20160425 : 修改拍照Time Out
                SFCExposureDelay[iSht].SetMSAndOn(TestIF_File.iSFCExposureTimeOut);
            }
            break;
        case 1150:
            if(bSFCStepError[iSFCRowA]==true || bSFCStepError[iSFCRowB]==true)  //Steven 20160503 : 確認移動位置與拍照位置相同
            {
                ErrPart="";
                if(InArmSuck.iShtRow==2)
                {
                    if(bSFCStepError[iSFCRowA])
                        ErrPart+=IndexSuckName[0][iNowMoveStep];

                    if(bSFCStepError[iSFCRowB])
                        ErrPart+=IndexSuckName[1][iNowMoveStep];
                }
                else
                {
                    if(bSFCStepError[iSFCRowB])
                        ErrPart+=IndexSuckName[0][iNowMoveStep];
                }

                Task=1180;
            }
            else if(bSFCExposureOK[iSFCRowA]==true &&
                    bSFCExposureOK[iSFCRowB]==true)                             //拍照完成
            {
                if(InArmSuck.iShtRow==2)
                {
                    mtSFC_Shuttle->SetCellNumber(iSFCRowA, iNowMoveStep+1, "Waiting result..");
                }
                mtSFC_Shuttle->SetCellNumber(iSFCRowB, iNowMoveStep+1, "Waiting result..");
                Task=1200;
            }
            else if(SFCExposureDelay[iSht].Off())                               //拍照time out
            {
                if(InArmSuck.iShtRow==2)
                {
                    if(bSFCStepError[iSFCRowA]==false)
                    {
                        mtSFC_Shuttle->SetCellNumber(iSFCRowA, iNowMoveStep+1, "Exposure time out");
                        Log.sprintf("SFC Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                        AddCCDCommunicationLog(iSFCRowA, Log);
                    }
                    else
                    {
                        mtSFC_Shuttle->SetCellNumber(iSFCRowA, iNowMoveStep+1, "Waiting result..");
                    }
                }

                if(bSFCStepError[iSFCRowB]==false)
                {
                    mtSFC_Shuttle->SetCellNumber(iSFCRowB, iNowMoveStep+1, "Exposure time out");
                    Log.sprintf("SFC Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                    AddCCDCommunicationLog(iSFCRowB, Log);
                }
                else
                {
                    mtSFC_Shuttle->SetCellNumber(iSFCRowB, iNowMoveStep+1, "Waiting result..");
                }

                iRetryCountExpTimo++;                                           //Steven 20160425 : 修改拍照Time Out
                Task=1160;
            }
            break;
        case 1160:
            if(iRetryCountExpTimo<3)
            {
                Task=1170;
            }
            else
            {
                ret=ShowErrorMessage("WAR0464", K_RETRY|K_SKIP, MSht);          //2D function CCD 3 or 4 Exposure time out!
                if(ret==K_SKIP)
                {
                    Task=1200;
                }
                else
                {
                    Task=1170;
                }
                iRetryCountExpTimo=0;
            }
            break;
        case 1170:
            if(InArmSuck.iShtRow==2)
            {
                if(bSFCStepError[iSFCRowA]==false)
                {
                    if(BLCarryKit.Item[0][iNowMoveStep]==HAS_NULL_IC || BLCarryKit.Item[0][iNowMoveStep]==NULL_IC)
                        sCCDCommand.sprintf("E1,1,%d,0", iNowMoveStep);
                    else
                        sCCDCommand.sprintf("E1,1,%d,1", iNowMoveStep);

                    SendCCDCommand(iSFCRowA, "SFC exposure 1170: ", sCCDCommand);
                }

                if(bSFCStepError[iSFCRowB]==false)
                {
                    if(BLCarryKit.Item[1][iNowMoveStep]==HAS_NULL_IC || BLCarryKit.Item[1][iNowMoveStep]==NULL_IC)
                        sCCDCommand.sprintf("E1,1,%d,0", iNowMoveStep);
                    else
                        sCCDCommand.sprintf("E1,1,%d,1", iNowMoveStep);
                    SendCCDCommand(iSFCRowB, "SFC exposure 1170: ", sCCDCommand);
                }
            }
            else
            {
                if(bSFCStepError[iSFCRowB]==false)
                {
                    if(BLCarryKit.Item[0][iNowMoveStep]==HAS_NULL_IC || BLCarryKit.Item[0][iNowMoveStep]==NULL_IC)
                        sCCDCommand.sprintf("E1,1,%d,0", iNowMoveStep);
                    else
                        sCCDCommand.sprintf("E1,1,%d,1", iNowMoveStep);
                    SendCCDCommand(iSFCRowB, "SFC exposure 1170: ", sCCDCommand);
                }
            }
            Task=1150;
            SFCExposureDelay[iSht].SetMSAndOn(TestIF_File.iSFCExposureTimeOut);
            break;
        case 1180:
            Log.sprintf("SFC Alarm, In shuttle %d exposure position error!", iSht+1);
            AddCCDCommunicationLog(iSFCRowA, Log);
            ShowErrorMessage("WAR0470", K_RETRY, MSht, false, ErrPart);         //In shuttle 2 exposure position error!
            Task=1;
            break;
        case 1200:
            iNowMoveStep--;
            iSFCCurrentStep[iSFCRowA]=iNowMoveStep;                             //Steven 20160503 : 確認移動位置與拍照位置相同
            iSFCCurrentStep[iSFCRowB]=iNowMoveStep;

            if(iNowMoveStep>=0 && iNowMoveStep<iSFCTotalMoveStep)
            {
                Task=1000;
            }
            else
            {
                Task=1300;
            }
            break;
        case 1300:
            sCCDCommand="GV,1";                                                 //Ifor 20151228 取得 CCD 讀取資料
            if(InArmSuck.iShtRow==2)
            {
                bSFCGetResultOK[iSFCRowA]=false;
                SendCCDCommand(iSFCRowA, "SFC get result 1130:", sCCDCommand);
            }
            else
            {
                bSFCGetResultOK[iSFCRowA]=true;
            }

            bSFCGetResultOK[iSFCRowB]=false;
            SendCCDCommand(iSFCRowB, "SFC get result 1130: ", sCCDCommand);

            bHasError=false;
            Task=1350;
            SFCResultDelay[iSht].SetMSAndOn(TestIF_File.iSFCGetResultTimeOut);  //這個是讀取Time Out用的
            break;
        case 1350:
            if(bSFCGetResultOK[iSFCRowA]==true &&
               bSFCGetResultOK[iSFCRowB]==true)                                 //讀取完成
            {
                sErrorPart="";
                for(int i=0; i<iSFCTotalMoveStep; i++)                          //清空畫面
                {
                    if(InArmSuck.iShtRow==2)
                    {
                        if(iSFCCheckResult[iSFCRowA][i]==0)
                        {
                            bHasError=true;
                            sErrorPart+=IndexSuckName[0][i/2];
                        }

                        if(iSFCCheckResult[iSFCRowB][i]==0)
                        {
                            bHasError=true;
                            sErrorPart+=IndexSuckName[1][i/2];
                        }
                    }
                    else
                    {
                        if(iSFCCheckResult[iSFCRowB][i]==0)
                        {
                            bHasError=true;
                            sErrorPart+=IndexSuckName[0][i/2];
                        }
                    }
                }

                Task=2000;
            }
            else if(SFCResultDelay[iSht].Off())                                 //讀取超時
            {
                sErrorPart="";
                if(InArmSuck.iShtRow==2)
                {
                    if(bSFCGetResultOK[iSFCRowA]==false)
                    {
                        bSFCGetResultOK[iSFCRowA]=true;
                        for(int i=0; i<iSFCTotalMoveStep; i++)                  //清空畫面
                        {
                            mtSFC_Shuttle->SetCellNumber(iSFCRowA, i+1, "Inspection time Out");
                        }
                        AddCCDCommunicationLog(iSFCRowA, "SFC Action 1350, Inspection time out.");
                        bHasError=true;
                    }

                    if(bSFCGetResultOK[iSFCRowB]==false)
                    {
                        bSFCGetResultOK[iSFCRowB]=true;
                        for(int i=0; i<iSFCTotalMoveStep; i++)                  //清空畫面
                        {
                            mtSFC_Shuttle->SetCellNumber(iSFCRowB, i+1, "Inspection time Out");
                        }
                        AddCCDCommunicationLog(iSFCRowB, "SFC Action 1350, Inspection time out.");
                        bHasError=true;
                    }
                }
                else
                {
                    if(bSFCGetResultOK[iSFCRowB]==false)
                    {
                        bSFCGetResultOK[iSFCRowB]=true;
                        for(int i=0; i<iSFCTotalMoveStep; i++)                  //清空畫面
                        {
                            mtSFC_Shuttle->SetCellNumber(iSFCRowB, i+1, "Inspection time Out");
                        }
                        AddCCDCommunicationLog(iSFCRowB, "SFC Action 1350, Inspection time out.");
                        bHasError=true;
                    }
                }
                Task=2000;
            }
            break;
        case 2000:
            if(bHasError==true)
            {
                if(iRetryCount<TestIF_File.iSFCAutoRetry)                       //Steven 20160421 : 修正判斷方式
                {
                    iRetryCount++;
                    Task=1;
                }
                else
                {
                    Task=2500;
                }
                bHasError=false;
            }
            else
            {
                AddCCDCommunicationLog(iSFCRowA, "SFC >>Cycle 2000, Inspection end.---------------\r\n");
                Task=5000;
            }
            break;
        case 2500:
            if(MOT[MSht].MotorMove(Prod.InSHT[iSht].iLeft+1000))
            {
                Log.sprintf("SFC Action 2500, Shuttle %d move to left", iSht+1);
                AddCCDCommunicationLog(iSFCRowA, Log);
                fLtcSensor->ClearLtcSensor(iSht);                               //Sam 20221101 : Latch 清除都要確認是否清清乾淨
                Task=2600;
            }
            break;
        case 2600:
            Log.sprintf("SFC Alarm 2600, In shuttle %d device floating error", iSht+1);
            AddCCDCommunicationLog(iSFCRowA, Log);
            ret=ShowErrorMessage("JAM0479", K_RETRY, MSht, false, sErrorPart);  //In Shuttle 2 IC float error
            iRetryCount=0;
            bHasError=false;
            Task=1;
            break;
        case 5000:
            iRetryCount=0;
            Task=1;
            return true;
    }

    return bResult;
}
//---------------------------------------------------------------------------
void TfBarCode::InitialSFCAutoTune2(bool bHasIC)
{
    if(bHasIC==false)
        iSFCAutoTune2Task=1;
    else
        iSFCAutoTune2Task=10000;
}
//---------------------------------------------------------------------------
bool TfBarCode::DoSFCAutoTune_2(bool bHasIC)
{
    static int iNowMoveStep=0;
    static int iRetryCount=0;
    static int iRetryCountExpTimo=0;
    static bool bHasError=false;
    static AnsiString ErrPart;
    static AnsiString sErrorPart="";

    int &Task=iSFCAutoTune2Task;
    int pos=0, ret;
    int iSht=0;
    int MSht=MInShuttle2;
    int iSFCRowA=iBarCode2_1;
    int iSFCRowB=iBarCode2_2;
    bool bResult=false;
    AnsiString cLastString, sCCDCommand;
    AnsiString Log;

    switch(Task)
    {
        //---------------------
        //AT無料的狀況
        //---------------------
        case 1:
            AddCCDCommunicationLog(iSFCRowA, "<<SFC AT Cycle, Cycle no device start.------------------");

            SetSFCCheckStepCount();
            iNowMoveStep=iSFCTotalMoveStep-1;
            iSFCCurrentStep[iSFCRowA]=iNowMoveStep;
            iSFCCurrentStep[iSFCRowB]=iNowMoveStep;
            bSFCStepError[iSFCRowA]=false;
            bSFCStepError[iSFCRowB]=false;
            bSFCExposureOK[iSFCRowA]=true;
            bSFCExposureOK[iSFCRowB]=true;
            bSFCGetResultOK[iSFCRowA]=true;
            bSFCGetResultOK[iSFCRowB]=true;
            iRetryCountExpTimo=0;

            for(int i=0; i<16; i++)
            {
                iSFCCheckResult[iSFCRowA][i]=-1;
                iSFCCheckResult[iSFCRowB][i]=-1;
            }

            bHasError=false;
            iRetryCount=0;
            sCCDCommand.sprintf("E9,1,%d", iSFCTotalMoveStep);                  //清空暫存器
            bSFCGetSE9[iSFCRowA]=false;
            bSFCGetSE9[iSFCRowB]=false;
            SendCCDCommand(iSFCRowA, "SFC AT Clear buffer 1", sCCDCommand);     //Reset CCD Shuttle暫存器
            SendCCDCommand(iSFCRowB, "SFC AT Clear buffer 1", sCCDCommand);
            SFCStartDelay[iSht].SetMSAndOn(5000);
            Task=500;
            break;
        case 500:                                                               //確認Buffer清空
            if(bSFCGetSE9[iSFCRowA] && bSFCGetSE9[iSFCRowB])
            {
                Task=1000;
            }
            else if(SFCStartDelay[iSht].Off())
            {
                ShowErrorMessage("WAR0462", K_RETRY, MSht, false, "SFC AT Clear buffer");
                Task=1;
            }
            break;
        case 1000:
            if(InArmSuck.iShtCol==1)
            {
                pos=Prod.iInSFCPos1x1[iSht];
            }
            else if(InArmSuck.iShtCol==2)
            {
                pos=Prod.iInSFCPos1x2[iSht][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==3)                                       //ChungHung 20140115 add for 2x3_6
            {
                pos=Prod.iInSFCPos1x3[iSht][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==4)
            {
                pos=Prod.iInSFCPos1x4[iSht][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==5)                                       //Steven 20221107 : Add for 2x5
            {
                pos=Prod.iInSFCPos1x5[iSht][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==6)
            {
                pos=Prod.iInSFCPos1x6[iSht][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==8)
            {
                pos=Prod.iInSFCPos1x8[iSht][iNowMoveStep];
            }
            else
            {
                ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "DoSFCAutoTune_2");
            }

            if(TestIF_File.bSFCUse2Photo)
            {
                if(iNowMoveStep%2==0)
                    pos+=TestIF_File.iSFCUse2PhotoOffset*100;
                else
                    pos-=TestIF_File.iSFCUse2PhotoOffset*100;
            }

            pos+=Offset.iSHLeft2D[iSht];

            if(MOT[MSht].MotorMove(pos))
            {
                Log.sprintf("SFC AT Action, Shuttle %d move to step %d", iSht+1, iNowMoveStep);
                AddCCDCommunicationLog(iSFCRowA, Log);
                SFCStartDelay[iSht].SetMSAndOn(TestIF_File.iSFCStartDelay);
                Task=1120;
            }
            break;
        case 1120:
            if(SFCStartDelay[iSht].Off())                                       //到位Delay
            {
                bSFCExposureOK[iSFCRowA]=false;
                if(InArmSuck.iShtRow==2)
                    bSFCExposureOK[iSFCRowB]=false;
                else
                    bSFCExposureOK[iSFCRowB]=true;

                //--------------------------------------
                //SE   1,        X,    1
                //拍照 偵測置偏, 位置, 左中右
                //--------------------------------------
                if(TestIF_File.bSFCUse2Photo)
                {
                    if(iNowMoveStep%2==0)
                        sCCDCommand.sprintf("E2,1,%d,0", iNowMoveStep);
                    else
                        sCCDCommand.sprintf("E2,1,%d,2", iNowMoveStep);
                }
                else
                    sCCDCommand.sprintf("E2,1,%d,1", iNowMoveStep);

                mtSFC_Shuttle->SetCellNumber(iSFCRowA, iNowMoveStep+1, "SFC Exposure...");
                SendCCDCommand(iSFCRowA, "SFC AT exposure 1120: ", sCCDCommand);

                if(InArmSuck.iShtRow==2)
                {
                    mtSFC_Shuttle->SetCellNumber(iSFCRowB, iNowMoveStep+1, "SFC Exposure...");
                    SendCCDCommand(iSFCRowB, "SFC AT exposure 1120: ", sCCDCommand);
                }

                Task=1150;
                iRetryCountExpTimo=0;                                           //Steven 20160425 : 修改拍照Time Out
                SFCExposureDelay[iSht].SetMSAndOn(TestIF_File.iSFCExposureTimeOut);
            }
            break;
        case 1150:
            if(bSFCStepError[iSFCRowA]==true || bSFCStepError[iSFCRowB]==true)  //Steven 20160503 : 確認移動位置與拍照位置相同
            {
                ErrPart="";
                if(bSFCStepError[iSFCRowA])
                    ErrPart+=IndexSuckName[0][iNowMoveStep];

                if(bSFCStepError[iSFCRowB])
                    ErrPart+=IndexSuckName[1][iNowMoveStep];

                Task=1180;
            }
            else if(bSFCExposureOK[iSFCRowA]==true &&
                    bSFCExposureOK[iSFCRowB]==true)                             //拍照完成
            {
                mtSFC_Shuttle->SetCellNumber(iSFCRowA, iNowMoveStep+1, "Waiting result..");

                if(InArmSuck.iShtRow==2)
                {
                    mtSFC_Shuttle->SetCellNumber(iSFCRowB, iNowMoveStep+1, "Waiting result..");
                }
                Task=1200;
            }
            else if(SFCExposureDelay[iSht].Off())                               //拍照time out
            {
                if(bSFCStepError[iSFCRowA]==false)
                {
                    mtSFC_Shuttle->SetCellNumber(iSFCRowA, iNowMoveStep+1, "Exposure time out");
                    Log.sprintf("SFC AT Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                    AddCCDCommunicationLog(iSFCRowA, Log);
                }
                else
                {
                    mtSFC_Shuttle->SetCellNumber(iSFCRowA, iNowMoveStep+1, "Waiting result..");
                }

                if(InArmSuck.iShtRow==2)
                {
                    if(bSFCStepError[iSFCRowB]==false)
                    {
                        mtSFC_Shuttle->SetCellNumber(iSFCRowB, iNowMoveStep+1, "Exposure time out");
                        Log.sprintf("SFC AT Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                        AddCCDCommunicationLog(iSFCRowB, Log);
                    }
                    else
                    {
                        mtSFC_Shuttle->SetCellNumber(iSFCRowB, iNowMoveStep+1, "Waiting result..");
                    }
                }
                iRetryCountExpTimo++;                                           //Steven 20160425 : 修改拍照Time Out
                Task=1160;
            }
            break;
        case 1160:
            if(iRetryCountExpTimo<3)
            {
                Task=1170;
            }
            else
            {
                ret=ShowErrorMessage("WAR0464", K_RETRY|K_SKIP, MSht);          //2D function CCD 3 or 4 Exposure time out!
                if(ret==K_SKIP)
                {
                    Task=1200;
                }
                else
                {
                    Task=1170;
                }
                iRetryCountExpTimo=0;
            }
            break;
        case 1170:
            if(TestIF_File.bSFCUse2Photo)
            {
                if(iNowMoveStep%2==0)
                    sCCDCommand.sprintf("E2,1,%d,0", iNowMoveStep);
                else
                    sCCDCommand.sprintf("E2,1,%d,2", iNowMoveStep);
            }
            else
                sCCDCommand.sprintf("E2,1,%d,1", iNowMoveStep);
            if(bSFCStepError[iSFCRowA]==false)
            {
                SendCCDCommand(iSFCRowA, "SFC AT exposure 1170: ", sCCDCommand);
            }

            if(InArmSuck.iShtRow==2)
            {
                if(bSFCStepError[iSFCRowB]==false)
                {
                    SendCCDCommand(iSFCRowB, "SFC AT exposure 1170: ", sCCDCommand);
                }
            }
            Task=1150;
            SFCExposureDelay[iSht].SetMSAndOn(TestIF_File.iSFCExposureTimeOut);
            break;
        case 1180:
            Log.sprintf("SFC AT Alarm, In shuttle %d exposure position error!", iSht+1);
            AddCCDCommunicationLog(iSFCRowA, Log);
            ShowErrorMessage("WAR0470", K_RETRY, MSht, false, ErrPart);         //In shuttle 2 exposure position error!
            Task=1;
            break;
        case 1200:
            iNowMoveStep--;
            iSFCCurrentStep[iSFCRowA]=iNowMoveStep;                             //Steven 20160503 : 確認移動位置與拍照位置相同
            iSFCCurrentStep[iSFCRowB]=iNowMoveStep;

            if(iNowMoveStep>=0 && iNowMoveStep<iSFCTotalMoveStep)
            {
                Task=1000;
            }
            else
            {
                Task=1300;
            }
            break;
        case 1300:
            if(MOT[MSht].MotorMove(Prod.OutSHT[1].iLeft))
            {
                Task=1400;
            }
            break;
        case 1400:
            AddCCDCommunicationLog(iSFCRowA, "<<SFC AT Cycle, Cycle no device end.------------------");
            Task=1300;
            bResult=true;
            break;
        //---------------------
        //AT有料的狀況
        //---------------------
        case 10000:
            AddCCDCommunicationLog(iSFCRowA, "<<SFC AT Cycle, Cycle has device start.------------------");

            SetSFCCheckStepCount();
            iNowMoveStep=iSFCTotalMoveStep-1;
            iSFCCurrentStep[iSFCRowA]=iNowMoveStep;
            iSFCCurrentStep[iSFCRowB]=iNowMoveStep;
            bSFCStepError[iSFCRowA]=false;
            bSFCStepError[iSFCRowB]=false;
            bSFCExposureOK[iSFCRowA]=true;
            bSFCExposureOK[iSFCRowB]=true;
            bSFCGetResultOK[iSFCRowA]=true;
            bSFCGetResultOK[iSFCRowB]=true;
            iRetryCountExpTimo=0;

            for(int i=0; i<16; i++)
            {
                iSFCCheckResult[iSFCRowA][i]=-1;
                iSFCCheckResult[iSFCRowB][i]=-1;
            }

            bHasError=false;
            iRetryCount=0;
            Task=11000;
        case 11000:
            if(InArmSuck.iShtCol==1)
            {
                pos=Prod.iInSFCPos1x1[iSht];
            }
            else if(InArmSuck.iShtCol==2)
            {
                pos=Prod.iInSFCPos1x2[iSht][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==3)                                       //ChungHung 20140115 add for 2x3_6
            {
                pos=Prod.iInSFCPos1x3[iSht][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==4)
            {
                pos=Prod.iInSFCPos1x4[iSht][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==5)                                       //Steven 20221107 : Add for 2x5
            {
                pos=Prod.iInSFCPos1x5[iSht][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==6)
            {
                pos=Prod.iInSFCPos1x6[iSht][iNowMoveStep];
            }
            else if(InArmSuck.iShtCol==8)
            {
                pos=Prod.iInSFCPos1x8[iSht][iNowMoveStep];
            }
            else
            {
                ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "DoSFCAutoTune_2");
            }

            if(TestIF_File.bSFCUse2Photo)
            {
                if(iNowMoveStep%2==0)
                    pos+=TestIF_File.iSFCUse2PhotoOffset*100;
                else
                    pos-=TestIF_File.iSFCUse2PhotoOffset*100;
            }

            pos+=Offset.iSHLeft2D[iSht];

            if(MOT[MSht].MotorMove(pos))
            {
                Log.sprintf("SFC AT Action, Shuttle %d move to step %d", iSht+1, iNowMoveStep);
                AddCCDCommunicationLog(iSFCRowA, Log);
                SFCStartDelay[iSht].SetMSAndOn(TestIF_File.iSFCStartDelay);
                Task=11120;
            }
            break;
        case 11120:
            if(SFCStartDelay[iSht].Off())                                       //到位Delay
            {
                bSFCExposureOK[iSFCRowA]=false;
                if(InArmSuck.iShtRow==2)
                    bSFCExposureOK[iSFCRowB]=false;
                else
                    bSFCExposureOK[iSFCRowB]=true;

                //--------------------------------------
                //SE   1,        X,    1
                //拍照 偵測置偏, 位置, 左中右
                //--------------------------------------
                if(TestIF_File.bSFCUse2Photo)
                {
                    if(iNowMoveStep%2==0)
                        sCCDCommand.sprintf("E3,1,%d,0", iNowMoveStep);
                    else
                        sCCDCommand.sprintf("E3,1,%d,2", iNowMoveStep);
                }
                else
                    sCCDCommand.sprintf("E3,1,%d,1", iNowMoveStep);

                mtSFC_Shuttle->SetCellNumber(iSFCRowA, iNowMoveStep+1, "SFC AT Exposure...");
                SendCCDCommand(iSFCRowA, "SFC AT exposure 1120: ", sCCDCommand);

                if(InArmSuck.iShtRow==2)
                {
                    mtSFC_Shuttle->SetCellNumber(iSFCRowB, iNowMoveStep+1, "SFC AT Exposure...");
                    SendCCDCommand(iSFCRowB, "SFC AT exposure 1120: ", sCCDCommand);
                }

                Task=11150;
                iRetryCountExpTimo=0;                                           //Steven 20160425 : 修改拍照Time Out
                SFCExposureDelay[iSht].SetMSAndOn(TestIF_File.iSFCExposureTimeOut);
            }
            break;
        case 11150:
            if(bSFCStepError[iSFCRowA]==true || bSFCStepError[iSFCRowB]==true)  //Steven 20160503 : 確認移動位置與拍照位置相同
            {
                ErrPart="";
                if(bSFCStepError[iSFCRowA])
                    ErrPart+=IndexSuckName[0][iNowMoveStep];

                if(bSFCStepError[iSFCRowB])
                    ErrPart+=IndexSuckName[1][iNowMoveStep];

                Task=11180;
            }
            else if(bSFCExposureOK[iSFCRowA]==true &&
                    bSFCExposureOK[iSFCRowB]==true)                             //拍照完成
            {
                mtSFC_Shuttle->SetCellNumber(iSFCRowA, iNowMoveStep+1, "Waiting result..");

                if(InArmSuck.iShtRow==2)
                {
                    mtSFC_Shuttle->SetCellNumber(iSFCRowB, iNowMoveStep+1, "Waiting result..");
                }
                Task=11200;
            }
            else if(SFCExposureDelay[iSht].Off())                               //拍照time out
            {
                if(bSFCStepError[iSFCRowA]==false)
                {
                    mtSFC_Shuttle->SetCellNumber(iSFCRowA, iNowMoveStep+1, "Exposure time out");
                    Log.sprintf("SFC AT Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                    AddCCDCommunicationLog(iSFCRowA, Log);
                }
                else
                {
                    mtSFC_Shuttle->SetCellNumber(iSFCRowA, iNowMoveStep+1, "Waiting result..");
                }

                if(InArmSuck.iShtRow==2)
                {
                    if(bSFCStepError[iSFCRowB]==false)
                    {
                        mtSFC_Shuttle->SetCellNumber(iSFCRowB, iNowMoveStep+1, "Exposure time out");
                        Log.sprintf("SFC AT Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                        AddCCDCommunicationLog(iSFCRowB, Log);
                    }
                    else
                    {
                        mtSFC_Shuttle->SetCellNumber(iSFCRowB, iNowMoveStep+1, "Waiting result..");
                    }
                }
                iRetryCountExpTimo++;                                           //Steven 20160425 : 修改拍照Time Out
                Task=11160;
            }
            break;
        case 11160:
            if(iRetryCountExpTimo<3)
            {
                Task=11170;
            }
            else
            {
                ret=ShowErrorMessage("WAR0464", K_RETRY|K_SKIP, MSht);          //2D function CCD 3 or 4 Exposure time out!
                if(ret==K_SKIP)
                {
                    Task=11200;
                }
                else
                {
                    Task=11170;
                }
                iRetryCountExpTimo=0;
            }
            break;
        case 11170:
            if(TestIF_File.bSFCUse2Photo)
            {
                if(iNowMoveStep%2==0)
                    sCCDCommand.sprintf("E3,1,%d,0", iNowMoveStep);
                else
                    sCCDCommand.sprintf("E3,1,%d,2", iNowMoveStep);
            }
            else
                sCCDCommand.sprintf("E3,1,%d,1", iNowMoveStep);
            if(bSFCStepError[iSFCRowA]==false)
            {
                SendCCDCommand(iSFCRowA, "SFC AT exposure 1170: ", sCCDCommand);
            }

            if(InArmSuck.iShtRow==2)
            {
                if(bSFCStepError[iSFCRowB]==false)
                {
                    SendCCDCommand(iSFCRowB, "SFC AT exposure 1170: ", sCCDCommand);
                }
            }
            Task=11150;
            SFCExposureDelay[iSht].SetMSAndOn(TestIF_File.iSFCExposureTimeOut);
            break;
        case 11180:
            Log.sprintf("SFC AT Alarm, In shuttle %d exposure position error!", iSht+1);
            AddCCDCommunicationLog(iSFCRowA, Log);
            ShowErrorMessage("WAR0470", K_RETRY, MSht, false, ErrPart);         //In shuttle 2 exposure position error!
            Task=10000;
            break;
        case 11200:
            iNowMoveStep--;
            iSFCCurrentStep[iSFCRowA]=iNowMoveStep;                             //Steven 20160503 : 確認移動位置與拍照位置相同
            iSFCCurrentStep[iSFCRowB]=iNowMoveStep;

            if(iNowMoveStep>=0 && iNowMoveStep<iSFCTotalMoveStep)
            {
                Task=11000;
            }
            else
            {
                Task=13000;
            }
            break;
        case 11300:
            if(MOT[MSht].MotorMove(Prod.OutSHT[1].iLeft))
            {
                Task=11400;
            }
            break;
        case 11400:
            AddCCDCommunicationLog(iSFCRowA, "<<SFC AT Cycle, Cycle has device end.------------------");
            bResult=true;
            break;
    }

    return bResult;
}
//---------------------------------------------------------------------------
bool TfBarCode::Do2DIDCheckSh2()
{
    int &Task=iInitialBarcodeInShuttle2Task;
    int iSht=0;
    int pos=0;
    static int iNowCheckStep=0;
    static int iRetryCount=0;
    static bool bBarcodeNum[BAR_CODE_COUNT]={false};
    AnsiString DataStr, Path, Data;
    AnsiString cLastString;
    static AnsiString sErrorPart="", sCheckErrorPart="";
    AnsiString asString;
    int iBarCodeRowA=iBarCode2_1;
    int iBarCodeRowB=iBarCode2_2;

    if(InArmSuck.iShtRow==1)
    {
        if(TestIF_File.iSelectUseCCDSh2==1)
        {
            iBarCodeRowA=iBarCode2_1;
            iBarCodeRowB=iBarCode2_2;
        }
        else
        {
            iBarCodeRowA=iBarCode2_2;
            iBarCodeRowB=iBarCode2_1;
        }
    }

    switch(Task)
    {
        case 1:
            iNowCheckStep=InArmSuck.iShtCol-1;     //遞減方式檢查
            InitSht2DCodeComPort(iSht);        //Ifor 20160322 Retry 時重置
            bBarcodeStartDelay[iBarCodeRowA]=true;
            if(InArmSuck.iShtRow==2)
            {
                bBarcodeStartDelay[iBarCodeRowB]=true;
            }

            for(int i=0; i<8; i++)
            {
                mtBarcodeInSh->SetCellNumber(iBarCodeRowA, i+1, "");
                mtBarcodeInSh->SetCellNumber(iBarCodeRowB, i+1, "");
            }

            for(int i=0; i<InArmSuck.iShtRow; i++)
            {
                for(int j=0; j<InArmSuck.iShtCol; j++)
                {
                    fNote->t2DCode->SetCellNumber(i, j, "");
                    fNote->t2DCode->SetCellColorIndex(i, j, 0);
                }
            }
            bBarcodeNum[iBarCodeRowA]=false;
            bBarcodeNum[iBarCodeRowB]=false;
            Task=1000;
            break;
        case 1000:
            if(iNowCheckStep>=0 && iNowCheckStep<InArmSuck.iShtCol)
            {
                if(InArmSuck.iShtCol==1)
                {
                    pos=Prod.iInSHBarCodeDetectPos1x1[1];
                }
                else if(InArmSuck.iShtCol==2)
                {
                    pos=Prod.iInSHBarCodeDetectPos1x2[1][iNowCheckStep];
                }
                else if(InArmSuck.iShtCol==3)  //ChungHung 20140115 add for 2x3_6
                {
                    pos=Prod.iInSHBarCodeDetectPos2x3[1][iNowCheckStep];
                }
                else if(InArmSuck.iShtCol==4)
                {
                    pos=Prod.iInSHBarCodeDetectPos1x4[1][iNowCheckStep];
                }
                else if(InArmSuck.iShtCol==5)  //Steven 20221027 : Add for 2x5
                {
                    pos=Prod.iInSHBarCodeDetectPos2x5[1][iNowCheckStep];
                }
                else if(InArmSuck.iShtCol==6)  //Steven 20170111 : fixed for 2x6
                {
                    pos=Prod.iInSHBarCodeDetectPos2x6[1][iNowCheckStep];
                }
                else if(InArmSuck.iShtCol==8)
                {
                    pos=Prod.iInSHBarCodeDetectPos2x8[1][iNowCheckStep];
                }
                else
                {
                    ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "Do2DIDCheckSh2");
                }

                pos+=Offset.iSHLeft2D[1];       //Steven 20151218 : Offset for 2d reader

                if(MOT[MInShuttle2].MotorMove(pos))
                {
                    bBarcodeStartDelay[iBarCodeRowA]=true;
                    if(InArmSuck.iShtRow==2)
                    {
                        bBarcodeStartDelay[iBarCodeRowB]=true;
                    }
                    BarcodePosDelay[iSht].SetMSAndOn(TestIF_File.iBarCodePosDelay);

                    if(WaitManualRetryKey())
                    {
                        Task=1000;
                    }
                    else if(WaitManualStepKey())
                    {
                        Task=1120;
                    }
                }
            }
            else
            {
                Task=2000;
            }
            break;
        case 1120:
            if(BarcodePosDelay[iSht].Off())
            {
                BarcodeDelay[iSht].SetMSAndOn(TestIF_File.iBarCodeDelay);
                sErrorPart="";
                sCheckErrorPart="";
                Task=1150;
            }
            break;
        case 1150:
            if(BAR_CODE_INSTALL==ebcUseOCR)//Ifor 20211129 OCR Shuttle1 Row A 使用第2隻CCD Row B使用第1隻CCD
            {
                if(bBarcodeNum[iBarCodeRowB]==false)
                {
                    iSH2_1BarcodePosition=iNowCheckStep;

                    if(Barcode_StartScan_In(iBarCodeRowB, iNowCheckStep, BLCarryKit.cDeviceInf[0][iNowCheckStep], cLastString)==true)
                    {
                        bBarcodeNum[iBarCodeRowB]=true;
                        mtBarcodeInSh->SetCellNumber(iBarCodeRowB, iNowCheckStep+1, BLCarryKit.cDeviceInf[0][iNowCheckStep]);
                    }
                }

                if(InArmSuck.iShtRow==2)
                {
                    if(bBarcodeNum[iBarCodeRowA]==false)
                    {
                        iSH2_2BarcodePosition=iNowCheckStep;

                        if(Barcode_StartScan_In(iBarCodeRowA, iNowCheckStep, BLCarryKit.cDeviceInf[1][iNowCheckStep], cLastString)==true)
                        {
                            bBarcodeNum[iBarCodeRowA]=true;
                            mtBarcodeInSh->SetCellNumber(iBarCodeRowA, iNowCheckStep+1, BLCarryKit.cDeviceInf[1][iNowCheckStep]);
                        }
                    }
                }
                else
                {
                    bBarcodeNum[iBarCodeRowA]=true;
                }
            }
            else    //Shuttle 1, Row A使用第1隻CCD, Row B使用第2隻CCD
            {
                if(bBarcodeNum[iBarCodeRowA]==false)
                {
                    iSH2_1BarcodePosition=iNowCheckStep;

                    if(Barcode_StartScan_In(iBarCodeRowA, iNowCheckStep, BLCarryKit.cDeviceInf[0][iNowCheckStep], cLastString)==true)
                    {
                        bBarcodeNum[iBarCodeRowA]=true;
                        mtBarcodeInSh->SetCellNumber(iBarCodeRowA, iNowCheckStep+1, BLCarryKit.cDeviceInf[0][iNowCheckStep]);
                    }
                }

                if(InArmSuck.iShtRow==2)
                {
                    if(bBarcodeNum[iBarCodeRowB]==false)
                    {
                        iSH2_2BarcodePosition=iNowCheckStep;

                        if(Barcode_StartScan_In(iBarCodeRowB, iNowCheckStep, BLCarryKit.cDeviceInf[1][iNowCheckStep], cLastString)==true)
                        {
                            bBarcodeNum[iBarCodeRowB]=true;
                            mtBarcodeInSh->SetCellNumber(iBarCodeRowA, iNowCheckStep+1, BLCarryKit.cDeviceInf[1][iNowCheckStep]);
                        }
                    }
                }
                else
                {
                    bBarcodeNum[iBarCodeRowB]=true;
                }
            }

            if(bBarcodeNum[iBarCodeRowA]==true && bBarcodeNum[iBarCodeRowB]==true)
            {
                Task=1200;
            }
            else if(BarcodeDelay[iSht].Off())
            {
                if(bBarcodeNum[iBarCodeRowA]==false)
                {
                    BLCarryKit.cDeviceInf[0][iNowCheckStep]=asBarCodeErrorSend;
                    mtBarcodeInSh->SetCellNumber(iBarCodeRowA, iNowCheckStep+1, asBarCodeErrorSend.c_str());
                }

                if(bBarcodeNum[iBarCodeRowB]==false)
                {
                    BLCarryKit.cDeviceInf[1][iNowCheckStep]=asBarCodeErrorSend;
                    mtBarcodeInSh->SetCellNumber(iBarCodeRowB, iNowCheckStep+1, asBarCodeErrorSend.c_str());
                }

                if(WaitManualStepKey())
                {
                    Task=1200;
                }
            }
            break;
        case 1200:
            bBarcodeNum[iBarCodeRowA]=false;
            bBarcodeNum[iBarCodeRowB]=false;
            iRetryCount=0;
            iNowCheckStep--;
            bBarcodeFirstAutoRetry[iBarCodeRowA]=false;
            bBarcodeFirstAutoRetry[iBarCodeRowB]=false;

            if(iNowCheckStep>=0 && iNowCheckStep<InArmSuck.iShtCol)
            {
                Task=1000;
                break;
            }
            else
            {
                Task=1300;
            }
        case 1300:
            if(MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iLeft))
            {
                Task=2000;
            }
            break;
        case 2000:
            Task=0;
            return true;
    }
    return false;
}
