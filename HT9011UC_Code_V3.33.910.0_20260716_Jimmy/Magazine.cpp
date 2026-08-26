#include "MachineDefine.h"
#pragma hdrstop
//---------------------------------------------------------------------------

#include "Magazine.h"

#include "csystem.h"
#include "MyKitSuck.h"
#include "aoutarm9045.h"
#include "aoutarm.h"
#include "main.h"
#include "MyMotor.h"
#include "mysensor.h"
#include "note.h"
#include "cSortCT.h"
#include "cmydef.h"
#include "acatchtray.h"
#include "asendic.h"
#include "cTrayMapping.h"                                                       //wei 20161219 (Steven) Tray Mapping
#include "ainarm2.h"
#include "mycylin.h"
#include "mymessbox.h"
#include "common.h"                                                             //Sam 20221116 : Magazine TrayArm 自動補 Tray
#include "myQwertyKeyBoard.h"
#include "cShowBinSelect.h"
#include "AGV.h"
#include "cSocket.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "butPa1"
#pragma link "HTray"
#pragma resource "*.dfm"
//==> Eastsun 20260512 F012 整合 (Phase8a-fix prototypes)
extern bool CheckCylinderCanMove(int iAxis);                                    //a-side asendic.cpp L1447
extern TQPF_Timer DoPickFromAuto_AutoRTDelay[];                                 //c-side AutoRetest.cpp L509
//<== Eastsun 20260512
TfMagazine *fMagazine;

TCheckBox *chkMag[MAX_MGZ_TRAY];
TLabel *labSlot[MAX_MGZ_TRAY];
TLabel *labSlotBin[MAX_MGZ_TRAY];
int iTrayCol[8]={0, 0, 0, 0, 0, 0, 0, 0};
//---------------------------------------------------------------------------
__fastcall TfMagazine::TfMagazine(TComponent* Owner)
    : TForm(Owner)
{
    fShow=false;
    EditInMag[0]=edInMagOfs01;
    EditInMag[1]=edInMagOfs02;
    EditInMag[2]=edInMagOfs03;
    EditInMag[3]=edInMagOfs04;
    EditInMag[4]=edInMagOfs05;
    EditInMag[5]=edInMagOfs06;
    EditInMag[6]=edInMagOfs07;
    EditInMag[7]=edInMagOfs08;
    EditInMag[8]=edInMagOfs09;
    EditInMag[9]=edInMagOfs10;
    EditInMag[10]=edInMagOfs11;
    EditInMag[11]=edInMagOfs12;
    EditInMag[12]=edInMagOfs13;
    EditInMag[13]=edInMagOfs14;
    EditInMag[14]=edInMagOfs15;
    EditInMag[15]=edInMagOfs16;
    EditOutAuto[0]=edOutAuto3Ofs01;
    EditOutAuto[1]=edOutAuto3Ofs02;
    EditOutAuto[2]=edOutAuto3Ofs03;
    EditOutAuto[3]=edOutAuto3Ofs04;
    EditOutAuto[4]=edOutAuto3Ofs05;
    EditOutAuto[5]=edOutAuto3Ofs06;
    EditOutAuto[6]=edOutAuto3Ofs07;
    EditOutAuto[7]=edOutAuto3Ofs08;
    EditOutAuto[8]=edOutAuto3Ofs09;
    EditOutAuto[9]=edOutAuto3Ofs10;
    EditOutAuto[10]=edOutAuto3Ofs11;
    EditOutAuto[11]=edOutAuto3Ofs12;
    EditOutAuto[12]=edOutAuto3Ofs13;
    EditOutAuto[13]=edOutAuto3Ofs14;
    EditOutAuto[14]=edOutAuto3Ofs15;
    EditOutAuto[15]=edOutAuto3Ofs16;
}
//---------------------------------------------------------------------------
void __fastcall TfMagazine::FormShow(TObject *Sender)
{
    bool bFlag=false;
    fShow=true;
    AnsiString aUnloader="";
    ReadFile();
    DoIniDataToForm();                                                          //Sam 20221116 : Magazine TrayArm 自動補 Tray

    bFlag=CanChangeData(false);

    rgMagTraySource->Enabled=false;

    if(fMain->CheckCanChangeRealDummy()==true && bFlag==true)
    {
        rgMagDisplayOrder->Enabled=true;
        rgMagFixType->Enabled=true;
    }
    else
    {
        rgMagDisplayOrder->Enabled=false;
        rgMagFixType->Enabled=false;
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //Ifor 20240304 add:
    {
        rgMagDisplayOrder->Enabled=false;
    }

    labSlotBin[0]= labSlot1Bin;
    labSlotBin[1]= labSlot2Bin;
    labSlotBin[2]= labSlot3Bin;
    labSlotBin[3]= labSlot4Bin;
    labSlotBin[4]= labSlot5Bin;
    labSlotBin[5]= labSlot6Bin;
    labSlotBin[6]= labSlot7Bin;
    labSlotBin[7]= labSlot8Bin;
    labSlotBin[8]= labSlot9Bin;
    labSlotBin[9]= labSlot10Bin;
    labSlotBin[10]=labSlot11Bin;
    labSlotBin[11]=labSlot12Bin;
    labSlotBin[12]=labSlot13Bin;
    labSlotBin[13]=labSlot14Bin;

    labSlot[0]=labSlot1;
    labSlot[1]=labSlot2;
    labSlot[2]=labSlot3;
    labSlot[3]=labSlot4;
    labSlot[4]=labSlot5;
    labSlot[5]=labSlot6;
    labSlot[6]=labSlot7;
    labSlot[7]=labSlot8;
    labSlot[8]=labSlot9;
    labSlot[9]=labSlot10;
    labSlot[10]=labSlot11;
    labSlot[11]=labSlot12;
    labSlot[12]=labSlot13;
    labSlot[13]=labSlot14;

    labSlot[0] ->Caption="Slot1";
    labSlot[1] ->Caption="Slot2";
    labSlot[2] ->Caption="Slot3";
    labSlot[3] ->Caption="Slot4";
    labSlot[4] ->Caption="Slot5";
    labSlot[5] ->Caption="Slot6";
    labSlot[6] ->Caption="Slot7";
    labSlot[7] ->Caption="Slot8";
    labSlot[8] ->Caption="Slot9";
    labSlot[9] ->Caption="Slot10";
    labSlot[10]->Caption="Slot11";
    labSlot[11]->Caption="Slot12";
    labSlot[12]->Caption="Slot13";
    labSlot[13]->Caption="Slot14";

    for(int i=0; i<MAX_MGZ_TRAY; i++)
    {
        for(int j=0; j<iTestBinCount; j++)
        {
            int temp=Prod.iT6CatData[j];
            if(temp<0)
                continue;

            if(i+iMagMin==temp)
            {
                //Auto1-1,2_Auto2-3,4_Auto3-5,6_Fix1-15,16_Fix2-5_Fix3-7,8,9,error.
                aUnloader+=AnsiString(j)+",";
            }
        }

        if(Prod.iIfErrorT6==i+iMagMin)
        {
            aUnloader+="error";
        }
        else
        {
            aUnloader.Delete(aUnloader.Length(), 1);
        }
        AnsiString str, str1;
        str.sprintf("Mag. Slot%d(Bin:%s)", i+1, aUnloader);
        str1.sprintf("Slot %d(Bin:%s)", i+1, aUnloader);

        labSlotBin[i]->Caption=str1;
        aUnloader="";
    }

    if(TestIF_File.iMagFixTrayType==1)
    {
        mtTrayBuffer1->XBlockItem=MOT[MManualTray1].Tray.XBItem;
        mtTrayBuffer1->YBlockItem=MOT[MManualTray1].Tray.YBItem;

        mtTrayBuffer1->XBlockWidth=MOT[MManualTray1].Tray.XBWidth;
        mtTrayBuffer1->YBlockWidth=MOT[MManualTray1].Tray.YBWidth;

        mtTrayBuffer1->XItem=MOT[MManualTray1].Tray.XItem;
        mtTrayBuffer1->YItem=MOT[MManualTray1].Tray.YItem;

        mtTrayBuffer2->XBlockItem=MOT[MManualTray2].Tray.XBItem;
        mtTrayBuffer2->YBlockItem=MOT[MManualTray2].Tray.YBItem;

        mtTrayBuffer2->XBlockWidth=MOT[MManualTray2].Tray.XBWidth;
        mtTrayBuffer2->YBlockWidth=MOT[MManualTray2].Tray.YBWidth;

        mtTrayBuffer2->XItem=MOT[MManualTray2].Tray.XItem;
        mtTrayBuffer2->YItem=MOT[MManualTray2].Tray.YItem;

        mtTrayBuffer3->XBlockItem=MOT[MManualTray3].Tray.XBItem;
        mtTrayBuffer3->YBlockItem=MOT[MManualTray3].Tray.YBItem;

        mtTrayBuffer3->XBlockWidth=MOT[MManualTray3].Tray.XBWidth;
        mtTrayBuffer3->YBlockWidth=MOT[MManualTray3].Tray.YBWidth;

        mtTrayBuffer3->XItem=MOT[MManualTray3].Tray.XItem;
        mtTrayBuffer3->YItem=MOT[MManualTray3].Tray.YItem;

        int iRegion=5;

        for(int i=2; i<10; i++)
        {
            if(MOT[MManualTray1].Tray.YItem>=(i*iRegion)-1)
            {
                iYRegNum=i; //多少Y為一個區間
            }
            else
            {
                break;
            }
        }

        for(int i=iFixMin; i<=iFixMax; i++)
        {
            if(AUTO3_IS_MAGAZINE==1)
            {
                for(int j=1; j<MOT[iMMAuto[i]].Tray.YItem; j++)
                {
                    if(j%iYRegNum==0)
                    {
                        for(int x=0; x<MOT[iMMAuto[i]].Tray.XItem; x++)
                        {
                            MOT[iMMAuto[i]].SetTraySingleData(x, j-1, HAS_NULL_IC);
                        }
                    }
                }
            }
        }

        for(int i=0; i<MOT[MManualTray1].Tray.XItem; i++)
        {
            for(int j=0; j<MOT[MManualTray1].Tray.YItem; j++)
            {
                if(MOT[MManualTray1].Tray.Data[i][j]!=NULL_IC)
                {
                    if(INSTALL_OCR!=eocrUninstal && TestIF.bOcrFunction && MOT[MManualTray1].Tray.Data[i][j]==HAS_OCR_OK)        //wei 20160613 ocr連線測試
                    {
                        MOT[MManualTray1].SetTraySingleData(i, j, HAS_OCR_OK);
                        mtTrayBuffer1->SetCellColorIndex(i, j, HAS_OCR_OK);
                    }
                    else if(INSTALL_OCR!=eocrUninstal && TestIF.bOcrFunction && MOT[MManualTray1].Tray.Data[i][j]==HAS_OCR_Err)
                    {
                        MOT[MManualTray1].SetTraySingleData(i, j, HAS_OCR_Err);
                        mtTrayBuffer1->SetCellColorIndex(i, j, HAS_OCR_Err);
                    }
                    else if(MOT[MManualTray1].Tray.Data[i][j]==HAS_NULL_IC)  //Ifor 20170807 (wei) 避免Has Null IC進入編輯頁面變成Has Ic
                    {
                        MOT[MManualTray1].SetTraySingleData(i, j, HAS_NULL_IC);
                        mtTrayBuffer1->SetCellColorIndex(i, j, HAS_NULL_IC);
                    }
                    else
                    {
                        MOT[MManualTray1].SetTraySingleData(i, j, HAS_IC);
                        mtTrayBuffer1->SetCellColorIndex(i, j, 1);
                    }
                }
                else
                {
                    mtTrayBuffer1->SetCellColorIndex(i, j, 0);
                }
            }
        }

        for(int i=0; i<MOT[MManualTray2].Tray.XItem; i++)
        {
            for(int j=0; j<MOT[MManualTray2].Tray.YItem; j++)
            {
                if(MOT[MManualTray2].Tray.Data[i][j]!=NULL_IC)
                {
                    if(INSTALL_OCR!=eocrUninstal && TestIF.bOcrFunction && MOT[MManualTray2].Tray.Data[i][j]==HAS_OCR_OK)        //wei 20160613 ocr連線測試
                    {
                        MOT[MManualTray2].SetTraySingleData(i, j, HAS_OCR_OK);
                        mtTrayBuffer2->SetCellColorIndex(i, j, HAS_OCR_OK);
                    }
                    else if(INSTALL_OCR!=eocrUninstal && TestIF.bOcrFunction && MOT[MManualTray2].Tray.Data[i][j]==HAS_OCR_Err)
                    {
                        MOT[MManualTray2].SetTraySingleData(i, j, HAS_OCR_Err);
                        mtTrayBuffer2->SetCellColorIndex(i, j, HAS_OCR_Err);
                    }
                    else if(MOT[MManualTray2].Tray.Data[i][j]==HAS_NULL_IC)  //Ifor 20170807 (wei) 避免Has Null IC進入編輯頁面變成Has Ic
                    {
                        MOT[MManualTray2].SetTraySingleData(i, j, HAS_NULL_IC);
                        mtTrayBuffer2->SetCellColorIndex(i, j, HAS_NULL_IC);
                    }
                    else
                    {
                        MOT[MManualTray2].SetTraySingleData(i, j, HAS_IC);
                        mtTrayBuffer2->SetCellColorIndex(i, j, 1);
                    }
                }
                else
                {
                    mtTrayBuffer2->SetCellColorIndex(i, j, 0);
                }
            }
        }

        for(int i=0; i<MOT[MManualTray3].Tray.XItem; i++)
        {
            for(int j=0; j<MOT[MManualTray3].Tray.YItem; j++)
            {
                if(MOT[MManualTray3].Tray.Data[i][j]!=NULL_IC)
                {
                    if(INSTALL_OCR!=eocrUninstal && TestIF.bOcrFunction && MOT[MManualTray3].Tray.Data[i][j]==HAS_OCR_OK)        //wei 20160613 ocr連線測試
                    {
                        MOT[MManualTray3].SetTraySingleData(i, j, HAS_OCR_OK);
                        mtTrayBuffer3->SetCellColorIndex(i, j, HAS_OCR_OK);
                    }
                    else if(INSTALL_OCR!=eocrUninstal && TestIF.bOcrFunction && MOT[MManualTray3].Tray.Data[i][j]==HAS_OCR_Err)
                    {
                        MOT[MManualTray3].SetTraySingleData(i, j, HAS_OCR_Err);
                        mtTrayBuffer3->SetCellColorIndex(i, j, HAS_OCR_Err);
                    }
                    else if(MOT[MManualTray3].Tray.Data[i][j]==HAS_NULL_IC)  //Ifor 20170807 (wei) 避免Has Null IC進入編輯頁面變成Has Ic
                    {
                        MOT[MManualTray3].SetTraySingleData(i, j, HAS_NULL_IC);
                        mtTrayBuffer3->SetCellColorIndex(i, j, HAS_NULL_IC);
                    }
                    else
                    {
                        MOT[MManualTray3].SetTraySingleData(i, j, HAS_IC);
                        mtTrayBuffer3->SetCellColorIndex(i, j, 1);
                    }
                }
                else
                {
                    mtTrayBuffer3->SetCellColorIndex(i, j, 0);
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMagazine::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    fShow=false;
    DoIniDataToForm();                                                          //JerryYang 20250411 : 離開頁面要刷新一次, 避免誤存檔
}
//---------------------------------------------------------------------------
void __fastcall TfMagazine::Button10Click(TObject *Sender)
{
    fMain->BtnStartClick(fMain);
}
//------------------------------------------------------------------------------
bool CheckPlaceToMagazineTray(int iWhichTray)
{
    int ct=iWhichTray;
    if(ct==iAuto3MagazineIndex && MOT[MMAuto3].fHasTray==true && MOT[MMAuto3].FullIC()==false)
    {
        return true;
    }
    else if(MOT[MMMagazineBuffer].fHasTray==false || MOT[MMMagazineBuffer].FullIC()==true)
    {
        return true;
    }

    return false;
}
//------------------------------------------------------------------------------
bool CheckPlaceToBufferTray(int iWhichMagazine)                                 //檢查Buffer夠不夠放
{
    int iCnt=0;
    int iNeedPlace=0;
    int k=iWhichMagazine/3;
    for(int i=0; i<MOT[iMMAuto[iWhichMagazine]].Tray.XItem; i++)
    {
        for(int j=0; j<MOT[iMMAuto[iWhichMagazine]].Tray.YItem; j++)
        {
            if(j>=k*iYRegNum &&
               j<(k+1)*iYRegNum &&
               MOT[iMMAuto[iWhichMagazine]].Tray.Data[i][j]==NULL_IC)
            {
                iCnt++;
            }
        }
    }

    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            if(OutArmSuck.Item[i][j]!=NULL_IC &&
               OutArmSuck.Item[i][j]!=HAS_NULL_IC &&
               OutArmSuck.iWhichAuto[i][j]==iMagMin+iWhichMagazine)
            {
                iNeedPlace++;
            }
        }
    }

    if(iCnt<=iNeedPlace)
    {
        return false;
    }

    return true;
}
//------------------------------------------------------------------------------
//void InitialDoPlaceToMagazineBuffer()
//{
//    iPlaceToMagazineBufferTask = 1;
//}
//------------------------------------------------------------------------------
//bool DoPlaceToMagazineBuffer()
//{
//    int &Task = iPlaceToMagazineBufferTask;
//
//    switch( Task )
//    {
//        case 1:
//
//
//        case 100:
//
//        case 200:
//
//        case 300:
//
//            break;
//    }
//
//
//    return false;
//}
//------------------------------------------------------------------------------
void AddLoadingCount_Buffer(int iPosRow, int iPosCol, int iTrayRow, int iTrayCol, int WhichAuto, bool bIsATK_AMR)
{
    bSCKARTLoadCntAlarm=false;
    int iMotXpos=0,iMotYpos=0;
    iLoadPickX=iPosRow;
    iLoadPickY=iPosCol;

    bPickLoaderDuplicateErr[iPosRow][iPosCol]=false;

    if(bCanRunSCKART==true                                    &&
       LastSet.iRunStartMode!=rsmContinuRetest                &&                //Steven 20170513 (jou) : For SCK ART can do manual RT
       LastSet.iRunStartMode!=rsmCInitialRetest               &&
       iSCKARTLoadingStatus>=1                                &&                //數量到了要放下來,然後 Alarm
       TestIF_File.bSCKART_LotDeviceCheck                     &&
       bIsATK_AMR==false                                      )                 //RogerYang 20260330 : ATK做sorting(Fix->Auto)被數量卡會死
    {
        bSCKARTLoadCntAlarm=true;
        return;
    }
    int Motor=iMMAuto[iPickWhichBuff];

    OutArmSuck.CopyFromTray(iPosRow, iPosCol, HAS_IC, MOT[Motor], iTrayRow, iTrayCol, NULL_IC);

    if(OutArmSuck.Item[iPosRow][iPosCol]==HAS_IC && OutArmSuck.iBinData[iPosRow][iPosCol]>=0)
    {
        OutArmSuck.Item[iPosRow][iPosCol]=OutArmSuck.iBinData[iPosRow][iPosCol]+TEST_PASS;
    }
    else
    {
        OutArmSuck.Item[iPosRow][iPosCol]=TEST_PASS+iTestBinCount;
        OutArmSuck.PordRec[iPosRow][iPosCol].AddTestResultRecord(iTestBinCount, OutArmSuck.cSBin[iPosRow][iPosCol], "Pick from Mag. ERR");
        OutArmSuck.iWhichAuto[iPosRow][iPosCol]=Prod.iIfErrorT6;
    }

    iMotXpos=MOT[MInArmX].ReadPos();                                            //Sam 20250211 : Add Load Pos
    iMotYpos=MOT[MInArmY].ReadPos();
    OutArmSuck.PordRec[iPosRow][iPosCol].AddLoadRecord(ASE_InTrayNum, iTrayRow, iTrayCol, iMotXpos, iMotYpos, ASET_ScheduleNAME, ASET_StartTimeNAME);  //Steven 20160114 : Add Production Log -- Load

    if(WhichAuto==0 && bIsATK_AMR==false)                                       //RogerYang 20260330 : add
    {
        iUPH_LoaderCount++;
        LastSet.SendCT[1]++;                                                    //jou 2010-08-13 計數jam rate,改為Tray Feed為一單位
        iRecordJamRateByTime_LoaderCount++;                                     // 2015.11.11 , Joye , Add Jam Rate Record
        if(bRTLoaderTrayCount)
        {
            LastSet.iLoaderTrayCount_ART++;
            bRTLoaderTrayCount=false;                                           //wei 20160112 ART R1不算空盤
        }

        if((USE_AUTO_RETEST==eartInstall && (bAutoReTest_ART || IniConfig.bA10_AutoReTest)) || CosFunction.bUseARTSortCount) //kevin 20150615       //Ifor 20170315 (wei) add 新增使用ART Sort Count 計數功能
        {
            LastSet.SendCT_ART[0]++;
            LastSet.SendCT_ART[1]++;
        }

        if(TestIF.bContinuousLoader==true)
        {
            LastSet.SendCT[2]++;                                                //jou 2010-11-17 Piggy-Back Functions
            if((USE_AUTO_RETEST==eartInstall && (bAutoReTest_ART || IniConfig.bA10_AutoReTest)) || CosFunction.bUseARTSortCount) //kevin 20150615   //Ifor 20170315 (wei) add 新增使用ART Sort Count 計數功能
                LastSet.SendCT_ART[2]++;
        }
        iQAModeLoaderCT++;                                                      //jou 2012-05-03 增加QA mode使用獨立的Loader Count
        LastSet.iIndexInputOutPut[3]++;
    }                                                                           //jou 2015-08-21 add InArm Pick count

    fSortCT->ShowLoadingIC();
    iContinueAutoSkipAutoTrayEnd=0;
}
//------------------------------------------------------------------------------
void Find_OutArm_Single_Pick_Buffer(int iUseSuck, int &iRow, int &iCol)
{
    int itStartX, itStartY;
    int isStartCol;
    int Motor;
    int ixStart, ixEnd, ixStep;
    int iyStart, iyEnd, iyStep;
    int Direction=0;
    int serchtable[4]={0, 1, 2, 3};
    int Yitem=0;
    int iRegion=5;                                                              //Magazine 14bin, 1個fix盤5個bin
    int iRy=0;

    Motor=iMMAuto[iPickWhichBuff];

    itStartX=-1;
    itStartY=-1;

    for(int i=0; i<MAX_ARM_Row; i++)
        for(int j=0; j<MAX_ARM_Col; j++)
            bOutArmSuckActive[i][j]=false;

    if(IniConfig.bUseTrayBlockMode   &&                                         //2014-03-04    Dell    for SPIL WLP Add Tray Block
       (TestIF.iTestMode==_8Site2X4  ||
        TestIF.iTestMode==_16Site4X4 ||                                         //Sam 20190226 : 16Site4X4
        TestIF.iTestMode==SingleSite ||
        iInArmType==e9045_1x2_2_14   ||                                         //Steven 20200804 : fixed for 2x2 nn mode
        TestIF.iTestMode==DualSite)  &&                                         //Steven 20140716 Add
        ((AutoForm[0]->BlockNumberX>0) || (AutoForm[0]->BlockNumberY>0)))       //Steven 20150702 : Fixed 分Tray問題
    {
        TrayForm.iFixTrayMode=0;
    }

    if(IniConfig.bUseTrayBlockMode &&
       IniConfig.bP06_LoaderUseCarrierTray==true &&
       ((AutoForm[0]->BlockXItem>0) || (AutoForm[0]->BlockYItem>0)))            //Frank 20160928 add Subtray Function
    {
        TrayForm.iFixTrayMode=0;
    }

    if(Direction==0 || Direction==4)                                            //左上到右下    //Steven 20141118 : Add Out Arm Direction 4~7
    {
        ixStart=0;
        ixEnd=MOT[Motor].Tray.XItem;
        ixStep=1;
        if(AUTO3_IS_MAGAZINE==1 && MOT[Motor].Tray.YItem>2*iRegion-1)
        {
            iyStep=1;
            Yitem=MOT[Motor].Tray.YItem;

            for(int iR=2; iR<10; iR++)
            {
                if(Yitem>=(iR*iRegion)-1)
                {
                    iYRegNum=iR;                                                //多少Y為一個區間
                }
                else
                {
                    break;
                }
            }

            iRy=ChangeToFloatNonPcnt((double)(iPickWhichMag), (double)(iFixCnt));
            iyStart=iYRegNum*iRy;
            iyEnd=iYRegNum*iRy+(iYRegNum-1);
        }
        else if(TrayForm.iFixTrayMode==0  || iWhichAuto<=iAutoRight)
        {
            iyStart=0;
            iyEnd=MOT[Motor].Tray.YItem;
            iyStep=1;

            if(IniConfig.bUseTrayBlockMode &&                                   //Ifor 20161129 add P06 判斷避免OutArm 一顆一顆放料
               IniConfig.bP06_LoaderUseCarrierTray==true)                       //Frank 20160928 add Subtray Function
            {
                int BX, BY, BlockTotal;
                bool bBlockEmpty = true;
                if((AutoForm[iWhichAuto]->BlockXItem>0) ||
                   (AutoForm[iWhichAuto]->BlockYItem>0))
                {
                    BX = ChangeToFloatNonPcnt((double)(MOT[Motor].Tray.XItem), (double)(AutoForm[iWhichAuto]->BlockXItem));
                    BY = ChangeToFloatNonPcnt((double)(MOT[Motor].Tray.YItem), (double)(AutoForm[iWhichAuto]->BlockYItem));
                    BlockTotal = AutoForm[iWhichAuto]->BlockXItem*AutoForm[iWhichAuto]->BlockYItem;
                    iOutArmBlockIndex =0;
                    for(int k=0; k<BlockTotal; k++)
                    {
                        ixEnd = BX + (iOutArmBlockIndex%AutoForm[iWhichAuto]->BlockXItem * BX);
                        iyEnd = BY + (ChangeToFloatNonPcnt((double)(iOutArmBlockIndex), (double)(AutoForm[iWhichAuto]->BlockXItem)) * BY);

                        for(int iR=0; iR<iyEnd; iR++)
                        {
                            for(int iC=0; iC<ixEnd; iC++)
                            {
                                if(MOT[Motor].Tray.Data[iC][iR]==NULL_IC)
                                {
                                    bBlockEmpty = false;
                                }
                            }
                        }

                        if(bBlockEmpty)
                        {
                            iOutArmBlockIndex=k+1;
                            ixEnd = BX + (iOutArmBlockIndex%AutoForm[iWhichAuto]->BlockXItem * BX);
                            iyEnd = BY + (ChangeToFloatNonPcnt((double)(iOutArmBlockIndex), (double)(AutoForm[iWhichAuto]->BlockXItem)) * BY);
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }
        }
        else
        {
            iyStep=1;
            if(iWhichAuto>iFixRight)
            {
                if(TrayForm.bFixTrayLink[iWhichAuto])
                    iyStart=0;
                else
                    iyStart=MOT[Motor].Tray.YItem/2+1;
                iyEnd=MOT[Motor].Tray.YItem;
            }
            else
            {
                iyStart=0;
                if(TrayForm.bFixTrayLink[iWhichAuto-3])
                {
                    iyEnd=MOT[Motor].Tray.YItem;
                }
                else
                {
                    if(CosFunction.bUseTrayUpDownSet)                           //wei 20160224 TSMC FIX UPDOWN
                    {
                        if(TrayForm.bTrayUpDownSet[iWhichAuto])
                            iyEnd=(MOT[Motor].Tray.YItem-1)/2;
                        else
                            iyEnd=MOT[Motor].Tray.YItem;
                    }
                    else
                    {
                        iyEnd=(MOT[Motor].Tray.YItem-1)/2;
                    }
                }
            }
        }
    }

    if((TestIF.iTestMode==QualSite2X2 ||
        TestIF.iTestMode==DualSite2x1 ||
        (USE_PICKER_COUNT==0 && TestIF.iTestMode==QualSite1X4)) &&              //Steven 20170810 (Jou) : Add for HT-9045S
        Motor==53)                                                              //kevin 20150713 fix3 由右到左放
    {
        ixStart=MOT[Motor].Tray.XItem-1;
        ixEnd=0;
        ixStep=1;

        for(int iR=iyStart; iR!=iyEnd; iR+=iyStep)
        {
            for(int iC=ixStart; iC>=ixEnd; iC-=ixStep)
            {
                if(MOT[Motor].Tray.Data[iC][iR]!=NULL_IC)
                {
                    itStartX=iC;
                    itStartY=iR;
                    break;
                }
            }

            if(itStartX!=-1 && itStartY!=-1)
                break;
        }
    }
    else
    {
        for(int iR=iyStart; iR!=iyEnd; iR+=iyStep)
        {
            for(int iC=ixStart; iC!=ixEnd; iC+=ixStep)
            {
                if(MOT[Motor].Tray.Data[iC][iR]!=NULL_IC)
                {
                    itStartX=iC;
                    itStartY=iR;
                    break;
                }
            }

            if(itStartX!=-1 && itStartY!=-1)
                break;
        }
    }

    if(iPickWhichBuff==2 || iPickWhichBuff==5)                                  //near right side
    {
        for(int i=0; i<MAX_ARM_Row; i++)
        {
            for(int j=MAX_ARM_Col-1; j>=0; j--)
            {
                isStartCol=serchtable[j];
                if(OutArmSuck.Item[i][isStartCol]==NULL_IC)
                {
                    bOutArmSuckActive[i][isStartCol]=true;
                    iCol=itStartX;
                    iRow=itStartY;
                    return ;
                }
            }
        }
    }
    else
    {
        int iUseARMCol=MAX_ARM_Col;

        if(TestIF.iTestMode==_6Site2X3)
        {
            iUseARMCol=MAX_ARM_Col-1;
        }
        for(int i=0; i<MAX_ARM_Row; i++)
        {
            for(int j=0; j<iUseARMCol; j++)
            {
                isStartCol=serchtable[j];
                if(OutArmSuck.Item[i][j]==NULL_IC)
                {
                    bOutArmSuckActive[i][j]=true;
                    iCol=itStartX;
                    iRow=itStartY;
                    return ;
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
bool SearchUnLoadTrayUpDown_9045_Pick_Buffer(bool &bCanPick2ICAtOnceTime, int &iStep, int &iRow, int &iCol)
{
    AdjustOutArmClosePitchCondition(iPickWhichBuff, bCanPick2ICAtOnceTime);
    if(bOutArmXOverLimit)                                                       //Steven 20171206 (Wei) : In ATK, 修正Out Arm同時放料但是超出軟體極限縮Pitch導致疊料的問題
        bCanPick2ICAtOnceTime=false;

    if(bCanPick2ICAtOnceTime==true)
    {
        int ct=Find_OutArm_PickerBufferMaxUseCountOnTime(4, iStep, iRow, iCol, false);
        if(ct<=1)
        {
            bCanPick2ICAtOnceTime=false;
        }
        else
        {
            return true;
        }
    }
    Find_OutArm_Single_Pick_Buffer(4, iRow, iCol);
    return true;
}
//==============================================================================
bool DoMoveOutArmXYToPick_9045_Buffer(bool &bCanPick2ICAtOnceTime, int &iRow, int &iCol, int &iStep, bool RealMove)
{
    int iVariablePara[]={2, 1, 0, -1}, iOutPutTray=0;
    if(USE_OUT_ARM_Y_PITCH==iXYPitchVariable)                                   //ChungHung 20131231 alter AutoYPitch  //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        iVariablePara[0]=1;
        iVariablePara[1]=0;
        iVariablePara[2]=-1;
        iVariablePara[3]=-2;
    }
    else if(USE_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                            //QQ               //JerryYang 20251218 : IN/OUT ARM支援不同模組
            USE_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                               //Ztex 2023.12.06 Add HT-1032
    {
    }

    int iXVariable[X_PITCH_COUNT];
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iXPos=0, iYPos=0;
    int iTempXpos   =0;
    int iYVariable  =0;
    int iMovePitchX =AutoCalculateOutArmXClosePitch(iPickWhichBuff);
    int iMovePitchY =AutoCalculateOutArmYClosePitch(iPickWhichBuff);
    int iOffsetPos  =-1;
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    if(IsTrayArmMoveAvoidOutArmCrash())
    {
        MOT[MOutArmX].PCIL132_StopMotor();
        MOT[MOutArmY].PCIL132_StopMotor();
        return false;
    }

    if(iHWFix_BinBox==1 && iWhichAuto==eBulkBox)
    {
        iXPos=Prod.iOutArmBinBoxX;
        iYPos=Prod.iOutArmBinBoxY-iBinBoxShiftY*200;

        if(ArmSpeed[OutArm].bVariModeFIX==true || bUseOnebyOne==true)
            iMovePitchX=iXpitchMaxX3;
        else
            iMovePitchX=(iXpitchMaxX2*3)/2;

        for(int i=0; i<MAX_ARM_Row; i++)
        {
            for(int j=0; j<MAX_ARM_Col; j++)
            {
                if(bOutArmSuckActive[i][j])
                {
                    iXPos+=iMovePitchX*iVariablePara[j]/3;
                    break;
                }
            }
        }
    }
    else if(bCanPick2ICAtOnceTime==true)
    {
        iYPos    =Prod.YStart[iPickWhichBuff][iOutArmYBase][iOutArmXBase]-iRow*AutoForm[iPickWhichBuff]->YPitch;
        iTempXpos=Prod.XStart[iPickWhichBuff][iOutArmYBase][iOutArmXBase]+iCol*AutoForm[iPickWhichBuff]->XPitch;

        if(USE_OUT_ARM_Y_PITCH==iXYPitchVariable)                               //Steven 20160107 start: For auto 1 full place with pick G & H  //JerryYang 20251218 : IN/OUT ARM支援不同模組
        {
            iTempXpos+=OutArmClose_PitchX;                                      //吸嘴A
        }
        else if(USE_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                        //QQ           //JerryYang 20251218 : IN/OUT ARM支援不同模組
                USE_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                           //Ztex 2023.12.06 Add HT-1032
        {
        }
        else
        {
            iTempXpos+=OutArmClose_PitchX;
        }

        if(bOutArmSuckActive[iOutArmPickOrder][0]==false)
        {
            if(bOutArmSuckActive[iOutArmPickOrder][1]==false)
            {
                if(bOutArmSuckActive[iOutArmPickOrder][2]==false)
                    iTempXpos-=OutArmClose_PitchX*3;
                else
                    iTempXpos-=OutArmClose_PitchX*2;
            }
            else
            {
                iTempXpos-=OutArmClose_PitchX;
            }
        }
        iMovePitchX=OutArmClose_PitchX*3;

        if(iTempXpos<(MOT[MOutArmX].Motor->PSoftLimitN+10) ||                   //Steven 20141217 : For Auto 1使用G & H吸嘴
           iTempXpos>(MOT[MOutArmX].Motor->PSoftLimitP-50))                     //jou 2014-11-20 100->50 fix out arm device > 25mm place to Fix3 hang up
        {
            bOutArmXOverLimit=true;                                             //Steven 20171206 (Wei) : In ATK, 修正Out Arm同時放料但是超出軟體極限縮Pitch導致疊料的問題
            return false;
        }
        else
        {
            iXPos=iTempXpos;
            iVariablePara[0]=iCol;
            iVariablePara[1]=iCol+iStep;
            iVariablePara[2]=iCol+2*iStep;
            iVariablePara[3]=iCol+3*iStep;
            iOutArmYPosition=iRow;
            if(RealMove==false)
                return true;
        }
    }
    else
    {
        iOutPutTray=iPickWhichBuff;

        iXPos=Prod.XStart[iOutPutTray][iOutArmYBase][iOutArmXBase];
        iYPos=Prod.YStart[iOutPutTray][iOutArmYBase][iOutArmXBase]+Prod.iTrayKitStartY;

        for(int i=0; i<MAX_ARM_Row; i++)
        {
            for(int j=0; j<MAX_ARM_Col; j++)
            {
                if(bOutArmSuckActive[i][j])
                {
                    iYPos=Prod.YStart[iOutPutTray][i][j]-iRow*AutoForm[iOutPutTray]->YPitch;

                    if(ArmSpeed[OutArm].bVariModeFIX==true ||
                       bUseOnebyOne==true)                                      //jou 2010-09-07 當clean out時,out arm排列自動改為One by one排列
                        iMovePitchX=iXpitchMaxX3;
                    else
                        iMovePitchX=OutArmClose_PitchX*3;

                    if(IniConfig.bFix3PutAllFullIC)                             //ChungHung 20111215 嘗試將Fix3放滿
                    {
                        if(iWhichAuto==iFixRight)
                            if(j==2 || j==3)
                                iMovePitchX=iXpitchMaxX3;
                    }
                    iXPos+=iCol*AutoForm[iOutPutTray]->XPitch+iMovePitchX*iVariablePara[j]/3;
                    if(iXPos<(MOT[MOutArmX].Motor->PSoftLimitN+10) ||
                       iXPos>(MOT[MOutArmX].Motor->PSoftLimitP-50))             //Steven 20141217 : For Auto 1使用G & H吸嘴   //jou 2014-11-20 100->50 fix out arm device > 25mm place to Fix3 hang up
                    {
                        bOutArmXOverLimit   =true;
                        iMovePitchX         =AutoCalculateOutArmXClosePitch(iPickWhichBuff, true);
                    }
                    iXPos=Prod.XStart[iOutPutTray][iOutArmYBase][iOutArmXBase]+
                          iCol*AutoForm[iOutPutTray]->XPitch+
                          iMovePitchX*iVariablePara[j]/3;

                    iOutArmXPosition[i][j]=iCol;
                    iOutArmYPosition=iRow;
                    for(int k=i; k<MAX_ARM_Row; k++)
                        for(int l=j+1; l<MAX_ARM_Col; l++)
                            bOutArmSuckActive[k][l]=false;
                    iXPosition[i]=iCol;
//                    iYPosition=iRow;
                }
            }
        }
    }

    if(USE_OUT_Y_IS_AUTO_PITCH==true)                                           //ChungHung 20140304 add for AutoYPitch  //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        if(iOutArmPickOrder==0)
            iYPos=iYPos-iMovePitchY;
    }
    else
    {
        if(iOutArmPickOrder!=0)
            iYPos=iYPos+iMovePitchY;
    }

    if(iYPos<(MOT[MOutArmY].Motor->PSoftLimitN+10))                             //JerryYang 20180221 (Steven) : fix y-pitch 最小25mm會有行程的問題, y-pitch要縮到最小
    {
        if(USE_OUT_Y_IS_AUTO_PITCH==true)                                       //JerryYang 20251218 : IN/OUT ARM支援不同模組
        {
            if(iOutArmPickOrder==0)
            {
                iYPos=iYPos+iMovePitchY;
                iMovePitchY=IN_OUT_ARM_Y_PITCH_MIN;
                iYPos=iYPos-iMovePitchY;
            }
        }
    }

    TransferAutoRatio(iPickWhichBuff, &iXPos, &iYPos);                          //Steven 20141029 : Auto的軟體齒輪比

    if(IniConfig.bBinBox     &&                                                 //jou 2012-12-11 support Bin Box
       iWhichAuto==iBinBoxAtFix &&
       AutoForm[iBinBoxAtFix]->iTrayType==iBinBoxType)
    {
        iXPos=Prod.iOutArmBinBoxX;
        iYPos=Prod.iOutArmBinBoxY-iBinBoxShiftY*200;
    }

    if(IniConfig.bUseTrayBlockMode)                                             //2014-03-04    Dell    for SPIL WLP Add Tray Block  //Steven 20140716 Add
        OutArmAddBlockPitch(iXPos, iYPos, iRow, iCol);

    if(iXPos<(MOT[MOutArmX].Motor->PSoftLimitN+10) ||                           //jou 2010-02-01 fix 3 吸嘴超過 軟體極限時 , 設成 has null ic
       iXPos>(MOT[MOutArmX].Motor->PSoftLimitP-10))                             //jou 2014-11-20 100->10 fix out arm device > 25mm place to Fix3 hang up
    {
        if(iPickWhichBuff==2 &&
           iXPos<(MOT[MOutArmX].Motor->PSoftLimitP+500))                        //Steven 20190109 : 差5mm內的, 放不下就讓它歪歪的放下去!
        {
            iXPos=MOT[MOutArmX].Motor->PSoftLimitP-10;
        }
        else if(MOT[iMMAuto[iPickWhichBuff]].Tray.Data[iCol][iRow]==NULL_IC)    //ChungHung 20111215 嘗試將Fix3放滿
        {
            MOT[iMMAuto[iPickWhichBuff]].SetTraySingleData(iCol, iRow, HAS_NULL_IC);
            ReserveEmptyPoint(iMMAuto[iPickWhichBuff], iCol, iRow);             //Eliot 2009_03_06
            return false;
        }
        else
        {
            return false;
        }
    }

    if(IniConfig.bE34InOutArmPitchZOffsetSameOne==true)
    {
        if(iWhichAuto>=0 && iWhichAuto<iAutoCnt)
            iOffsetPos=OutOfsAuto1;
        else
            iOffsetPos=OutOfsFix1;
    }
    else
    {
        iOffsetPos=OutOfsAuto1+iPickWhichBuff;
    }

    iYVariable=GetOutArmPitchY_9045(iMovePitchY, iOffsetPos);
    for(int i=0; i<X_PITCH_COUNT; i++)
        iXVariable[i]=GetOutArmPitchX_9045(iMovePitchX, i, iOffsetPos);

    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            bZFlag[i][j]=bOutArmSuckActive[i][j];
            iZPos[i][j] =(bZFlag[i][j])?Prod.ZOutArm_Fix_Pick[iPickWhichBuff-eFix1][i][j]:ZSafePos;     //Steven 20260203 : iPickWhichBuff -->  iPickWhichBuff-eFix1
        }
    }

    if(OutArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, true))
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool SetOutArm_9045_Pick_Buffer()
{
    int iStep, iCol, iRow;
    bool bCanPick2ICAtOnceTime;
    if(IsTrayArmMoveAvoidOutArmCrash())
    {
        MOT[MOutArmX].PCIL132_StopMotor();
        MOT[MOutArmY].PCIL132_StopMotor();
        return false;
    }

    if(SearchUnLoadTrayUpDown_9045_Pick_Buffer(bCanPick2ICAtOnceTime, iStep, iRow, iCol)==false)
        return false;

    bOverTray=false;                                                            //jou 2012-09-04 修正極限無法到達時,回填Null_IC會造成找不到地方放,會放在 x=-1,y=-1的位置上
    if(iCol<0 || iRow<0)
    {
        bOverTray=true;
        return false;
    }

    if(MOT[MTrayX].ReadPos()>Prod.iXTrayColor)
    {
        MOT[MOutArmY].PCIL132_StopMotor();
        MOT[MOutArmY].PCIL132_StopMotor();
        return false;
    }

    return DoMoveOutArmXYToPick_9045_Buffer(bCanPick2ICAtOnceTime, iRow, iCol, iStep, true);
}
//==============================================================================
bool ArmFinish_Pick_Buffer()
{
    if(MagazineBufferIsEmpty(iPickWhichMag))
    {
        return true;
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------
bool OutArmFinish_2x3_6()                                                       //check Arm finish and  can go shuttle
{
    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col-1; j++)
        {
            if(bOutArmSuckActive[i][j] &&
               OutArmSuck.Item[i][j]==NULL_IC)
            {
                return false;
            }
            else if(LastSet.bUseTestSocket[0][i][j] &&
                    OutArmSuck.Item[i][j]==NULL_IC)                             //ChungHung 20130910 alter for SCK can close site by Index
            {
                return false;
            }
        }
    }
    return true;
}
//------------------------------------------------------------------------------
void InitialDoPickFromMagazineBuffer()
{
    iPickFromMagazineBufferTask=1;
}
//------------------------------------------------------------------------------
bool DoPickFromMagazineBuffer()
{
    static int iRetry=0;
    static int iRetryCT=0;
    static bool bSuckDuplicateErr[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false}, {false, false, false, false}};
    static bool bOutSuckEnd[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false}, {false, false, false, false}};

    int ct, ret;
    int &Task=iPickFromMagazineBufferTask;
    bool flag;
    bool bflag=false;
    bool bHasDuplicateErr=false;
    AnsiString asAutoOffset;
    AnsiString ErrPart="";

    switch(Task)
    {
        case 1:
            bOutArmXOverLimit=false;                                            //Steven 20171206 (Wei) : In ATK, 修正Out Arm同時放料但是超出軟體極限縮Pitch導致疊料的問題
            IfUseOnebyOne(0);                                                   //Steven 20111101 : 整合成function
            InitialFix3CanFullTask();                                           //ChungHung 20140313 add Fix3 can Full Tray
            Task=10;
        case 10:
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    bOutSuckEnd[i][j]=false;
                }
            }
            iPickWhichBuff=SearchTrayToPick_Buffer();                           //Steven 20160524 : 簡化Out Arm程式碼
            ct=iPickWhichBuff;

            if(fAGV->IsATK_AMR() &&
               LastSet.iUnloadFixTray==eAtkTfMoveFixIC)                         //Steven 20260202 : for ATK AMR
            {
                iFixToWhichAuto=iPickWhichBuff;
            }
            else
            {
                if(MOT[iMMAuto[iWhichAuto]].fHasTray==false ||
                   (MOT[iMMAuto[iWhichAuto]].FullIC() &&
                    CheckUseFixBinBoxFunction()==false))                        //kevin 20160822
                {
                    Task=1;
                    return true;
                }
            }

            if(ct!=eBulkBox &&                                                  //kevin 20160912 20160822 add error bin box
               (EnableFix3UseCylinder()))                                       //ChungHung 20140722 add for HT9046LA
            {
                if(UseFix3Cylinder(ct)==false)                                  //ChungHung 20140313 add Fix3 can Full Tray
                    break;
            }

            flag=SetOutArm_9045_Pick_Buffer();
            if(flag)
            {
                Task=50;
            }
            else                                                                //jou 2012-09-04 修正極限無法到達時,回填Null_IC會造成找不到地方放,會放在 x=-1,y=-1的位置上
            {
                if(bOverTray==true)
                {
                    return true;
                }
            }
            break;
        case 50:
            iRetry=0;
            Task=100;
            break;
        case 100:
            if((AccessLevel>=1 || CosFunction.bOPCanPressStepAndTStart) &&      //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
               (Sen[SnRKManualStep].IsOn()))
            {
                bEnterOffset=false;
                fMain->Pause("DoPickFromMagazineBuffer");
                Task=200;
                break;
            }

            iLoadPitchStepY=0;                                                  //JerryYang 20221125 : 先一支一支吸

             if(ArmSpeed[OutArm].bSuckOnDown==false &&
                ArmSpeed[OutArm].dDestroyPauseTime!=0)
            {
                DoPlaceToAutoDelay.SetSecAndOn(ArmSpeed[OutArm].dDestroyPauseTime);
                Task=110;
            }
            else
            {
                Task=300;
            }
            break;
        case 110:
            if(DoPlaceToAutoDelay.Off()==true)
            {
                OutArmSuck.ResetAll();
                Task=300;
            }
            break;
        case 200:
            if(bEnterOffset==false)
            {
                Task=300;
            }
            else
            {
                bEnterOffset=false;
                Task=220;
            }
            break;
        case 220:
            if(MoveOutArmToAutoSafe())
                Task=1;
            break;
        case 300:

            flag=true;
            sAskStartDetect="";                                                 //kevin 20170224

            #ifdef SOFT_SIMULTE
                if(fMain->chkInPickLoadError->Checked)
                {
//                    if(Pos==0)
//                    {
//                        bSuckEnd[0][0]=true;
//                        OutArmSuck.Suck[0][0].Error=true;
//                    }
//                    else
//                    {
//                        bSuckEnd[0][2]=true;
//                        OutArmSuck.Suck[0][2].Error=true;
//                    }
                }
            #endif

            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(bOutArmSuckActive[i][j] &&
                       OutArmSuck.Item[i][j]==NULL_IC)
                    {
                        if(bOutSuckEnd[i][j]==false &&
                           OutArmSuck.Suck[i][j].Suck())                        //wei 20150318 suck 判斷是否動作
                        {
                            int iTrayRow=iOutArmYPosition+iLoadPitchStepY*i;    //kevin 20141103
                            if(fAGV->IsATK_AMR() &&
                               LastSet.iUnloadFixTray==eAtkTfMoveFixIC)         //Steven 20260202 : for ATK AMR
                            {
                                AddLoadingCount_Buffer(i, j, iTrayRow, iOutArmXPosition[i][j], iPickWhichBuff, true);
                                OutArmSuck.iWhichAuto[i][j]=iFixToWhichAuto-eFix1;
                            }
                            else
                            {
                                AddLoadingCount_Buffer(i, j, iTrayRow, iTrayCol[j], iPickWhichBuff, false);
                            }
                            bOutSuckEnd[i][j]=true;
                        }
                        else if(OutArmSuck.Suck[i][j].Error==false)
                        {
                            flag=false;
                        }
                        else if(OutArmSuck.Suck[i][j].Error==true)
                        {
                            bOutSuckEnd[i][j]=true;
                        }
                    }
                    else
                    {
                        bOutSuckEnd[i][j]=true;
                    }
                }
            }

            if(flag==false)                                                     //KEVIN 20130421 按RETRY資料尚未交換會將其他IC丟掉
                break;

            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(OutArmSuck.Suck[i][j].Error)
                    {
                        bInArmSuckErr=true;
                        iRetryCT++;
                        if(iRetryCT<=ArmSpeed[OutArm].iRetryCT)                 //Steven 20141113 : Loader SKIP時在原地Retry
                        {
                            Task=310;
                        }
                        else
                        {
                            Task=400;
                        }
                        return false;
                    }
                }
            }

            iRetryCT=0;
            Task=350;
            break;
        case 310:
            if(MoveOutArmToAutoSafe())
            {
                Task=10;
            }
            break;
        case 350:
            if(MoveOutArmToAutoSafe())
            {
                if(CheckBuffIsSameBin()==false)
                {
                    for(int i=0; i<MAX_ARM_Row; i++)
                    {
                        for(int j=0; j<MAX_ARM_Col; j++)
                        {
                            if(OutArmSuck.Item[i][j]!=NULL_IC &&
                               OutArmSuck.Item[i][j]!=HAS_NULL_IC)
                            {
                                OutArmSuck.SetItemData(i, j, TEST_PASS+iTestBinCount);
                                OutArmSuck.PordRec[i][j].AddTestResultRecord(iTestBinCount, OutArmSuck.cSBin[i][j], "RESET"); //Frank 20160505 add
                                OutArmSuck.iWhichAuto[i][j]=Prod.iIfErrorT6;
                                OutArmSuck.iBinData[i][j]=iTestBinCount;
                            }
                        }
                    }
                    ShowMyMessage("Magazine pick from buffer tray error, set to error bin.");
                    Task=1;
                    return true;
                }

                if(TestIF.iTestMode==_6Site2X3)
                {
                    if(ArmFinish_Pick_Buffer() || OutArmFinish_2x3_6())
                    {
                        Task=1;

                        return true;
                    }
                    else
                    {
                        Task=10;
                    }
                }
                else
                {
                    if(ArmFinish_Pick_Buffer() || OutArmSuck.All_HasIC())
                    {
                        Task=1;
                        return true;
                    }
                    else
                    {
                        Task=10;
                    }
                }
            }
            break;
        case 400:
            if(MoveOutArmToAutoSafe())
            {
                Task=500;
            }
            break;
        case 500:
            iRetryCT=0;

            ErrPart=" ";
            bHasDuplicateErr=false;
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(bSuckDuplicateErr[i][j])
                    {
                        bHasDuplicateErr=true;
                    }

                    if(OutArmSuck.Suck[i][j].Error)
                    {
                        ErrPart+=InArmSuck.Suck[i][j].sName;
                        bPickLoaderDuplicateErr[i][j]=true;
//                        iYpos=iOutArmYPosition+iLoadPitchStepY*i;               //kevin 20171026 (wei) add Tray Ypos
                    }
                    else
                    {
                        bSuckDuplicateErr[i][j]=false;
                    }
                }
            }

            ret=ShowErrorMessage("JAM0211", K_RETRY|K_SKIP, MOutArmX, bHasDuplicateErr, ErrPart);
            if(ret==K_SKIP)
            {
                bflag=DoBufferTraySkipProcess_9045();                           //Steven 20190703 : 整合Load Tray Skip動作
                if(bflag==false)
                {
                    Task=1400;
                    return false;
                }
            }
            else// if(ret==K_RETRY)
            {
                OutArmSuck.ResetAll();                                          //Steven 20160323 : 避免未開啟真空

                if(IniConfig.bInOutArmCanPushHome)
                {
                    Task=310;                                                   //不回Home
                }
                else
                {
                    Task=600;
                }
                return false;
            }
            break;
        case 600:
            if(MoveOutArmToAutoSafe())
            {
                SetOutArmHome();
                Task=10;
            }
            break;
        case 1400:
            if(MoveOutArmToAutoSafe())
            {
                Task=10;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
void InitialAuto3MagazineTask()
{
    iAuto3MagazineTask=1;
}
//------------------------------------------------------------------------------
void InitialMagazineUpDown()
{
    iMagazineUpDownTask=1;
}
//------------------------------------------------------------------------------
TQPF_Timer htCatchTrayReceiveTray;
//------------------------------------------------------------------------------
TQPF_Timer htMagGetNewTray;                                                     //JerryYang 20221119 : add
void DoAuto3Magazine()
{
    int &Task=iAuto3MagazineTask;
    int ret;
    static int iwMag=-1;
    AnsiString str="";
    AnsiString str1="";
//    bool bTrayFull;
    static bool bflag1=false;                                                   //Sam 20221116 : Magazine TrayArm 自動補 Tray
    static int iCheckMagazineSlot=0;
    static bool bNeedTrayDelay=true, bResetTimer=true;

    switch(Task)
    {
        case 1:
            // Safe Check ------------------------------------------------------
            /*
            if( MOT[MMAuto3_Car].fHasTray == true )
            {
                // Alarm : Track has tray
                ret = ShowMagazineError( MMAuto3_Car , TRAY_MORE , K_SKIP );
                if( ret == K_SKIP )
                {
                    MOT[MMAuto3_Car].ClearTray(__FUNC__);
                }
                break;
            }
            */

            // -----------------------------------------------------------------

            if(bNeedAMRMagazineUnload==true)                                      //Eastsun 20260514 F010 AMR unload guard
            {
            }
            else if(MOT[MMAuto3].fHasTray==true)
            {
                bNeedTrayDelay=true;
                if(Sen[SnAuto3TrayDetect].IsOff()==true &&                      // Tray Miss
                    LastSet.iRealDummy!=DUMMY)
                {
                    ret=ShowErrorMessage("WAR1330", K_SKIP|K_RETRY, MMAuto3, false, "DoAuto3Magazine_1");  //JerryYang 20221119 : add
                    if(ret==K_SKIP)
                    {
                        MOT[MMAuto3].ClearTray(__FUNC__);
                        Cylinder[C_AutoEdgePush[iMagAtAuto]].Off();
                        iAuto3MagazineIndex=-1;
                    }

                    if(SoftStop)
                    {
                        Cylinder[C_AutoEdgePush[iMagAtAuto]].Off();
                    }
                }
                else if(iAuto3MagazineIndex==-1)                                //20230505
                {
                    ShowMyMessage("Please the tray at Auto 3 manually.", "請取下AUTO3的tray盤");
                    MOT[MMAuto3].ClearTray(__FUNC__);
                    Cylinder[C_AutoEdgePush[iMagAtAuto]].Off();
                    iAuto3MagazineIndex=-1;
                }
                else if(iWhichMag!=-1 &&
                        iWhichMag!=iAuto3MagazineIndex &&
                        (iWhichAuto==iMagAtAuto || bMagNeedChangeTray) &&
                        iAuto3MagazineIndex!=-1)
                {
                    InitialCatchTrayChangeTray();                               // Change Tray

                    if(TestIF_File.iMagTraySource==0)                           //Sam 20221116 : Magazine TrayArm 自動補 Tray
                    {
                        if(MOT[iMMgzTray[iWhichMag]].fHasTray==false)
                            Task=4000;                                          //Magazine 沒有 Tray 要先將 Auto3 的退回去
                        else
                            Task=1000;
                    }
                    else
                    {
                        Task=1000;
                    }
                }
                else if(MOT[MMAuto3].Tray.FullIC()==true)
                {
                    // Tray Full
                    iMagazineFullTrayIndex=iAuto3MagazineIndex;
//                    iWhichMag=iAuto3MagazineIndex;                            //JerryYang 20241226 : mark掉

                    InitialCatchTrayChangeTray();
                    if(TestIF_File.iMagTraySource==0)                           //Sam 20221116 : Magazine TrayArm 自動補 Tray
                    {
                        Task=4000;                                              //Auto3 滿 Tray  要先將 Auto3 的退回去
                    }
                    else
                    {
                        Task=1000;
                    }
                }
                else
                {
                    if(MOT[MCatchMgzTray].ReadPos()==Prod.iCatchMazTray_Rear &&
                       Cylinder[C_CatchMagazineTray].Status==false)             //JerryYang 20221130 : 避免回HOME後汽缸沒打出來
                    {
                        Cylinder[C_CatchMagazineTray].On();
                    }
                    Cylinder[C_AutoEdgePush[iMagAtAuto]].On();
                    if(iMagazineStatus==2)                                      //Ifor 20231130 add: 0: Normal 1:Full Tray 2:Manual put tray
                    {
                        InitialMagazineUpDown();
                        Task=2100;
                        break;
                    }
                }
            }
            else if(MOT[MMAuto3].fHasTray==false)
            {
                if(Sen[SnAuto3TrayDetect].IsOn()==true &&                       // Has Tray
                   LastSet.iRealDummy!=DUMMY)
                {
                    ShowErrorMessage("MES1322", K_RETRY, MMAuto3, 0, "");       //JerryYang 20221018 這裡要補一下ALARM
                    Cylinder[C_AutoEdgePush[iMagAtAuto]].Off();
                }
                else if(iWhichMag!=-1)
                {
                    if(MOT[iMMgzTray[iWhichMag]].Tray.FullIC()==true)
                    {
                        // Tray Full
                        InitialMagazineUpDown();
//                        Task=2000;
                    }
                    else                                                        // Need Tray
                    {
                        if(TestIF_File.iMagTraySource==0)                       //Empty進tray    //Sam 20221116 : Magazine TrayArm 自動補 Tray
                        {
                            if(MOT[iMMgzTray[iWhichMag]].fHasTray==true &&      //等待tray arm補空tray
                               iOutArmWhichAuto==2)                             //Magazine中已經有tray
                            {
                                InitialCatchTrayGetNewTray();
                                bMagGetNewTrayflag=true;
                                iwMag=iWhichMag;
                                Task=100;
                            }
                            else if(MOT[iMMgzTray[iWhichMag]].fHasTray==false)  //等待tray arm補tray到Magazine
                            {
                                if(bNeedTrayDelay==true)
                                {
                                    bNeedTrayDelay=false;
                                    htMagGetNewTray.SetMSAndOn(1000);
                                }

                                if(htMagGetNewTray.Off()==true)
                                {
                                    bNeedTrayDelay=true;
                                    Task=3000;                                  //Sam 20221116 : Magazine TrayArm 自動補 Tray
                                    bflag1=false;
                                }
                                break;
                            }
                        }
                        else                                                    //Magazine進tray

                        {
                            if(iOutArmWhichAuto==2)                             //buffer區滿了再入tray
                            {
                                InitialCatchTrayGetNewTray();
                                Task=100;
                            }
                        }
                    }
                }
            }

            iMagazineFullTrayIndex=-1;
            for(int i=0; i<MAX_MGZ_TRAY; i++)
            {
                if(MOT[iMMgzTray[i]].Tray.FullIC()==true)
                {
                    if(BinSelect[iTestRunMode].bMagazineLink[i+1]==false)
                    {
                        iMagazineFullTrayIndex=i;
                        InitialMagazineUpDown();
                        Task=2000;
                    }
                }
            }
            break;
        case 100:                                                               // Get New Tray
            bMagGetNewTrayflag=true;
            if(DoCatchTrayGetNewTray(iWhichMag)==true)
            {
                bMagGetNewTrayflag=false;
                iAuto3MagazineIndex=iWhichMag;
                Task=1;
            }
            break;
        case 1000:                                                              // Change Tray
            bChaneMagTrayflag=true;
            if(DoCatchTrayChangeTray(iWhichMag)==true)
            {
                bChaneMagTrayflag=false;

                if(MOT[MMAuto3].fHasTray==true)                                 //確認有換盤成功
                    iAuto3MagazineIndex=iWhichMag;
                else
                   iAuto3MagazineIndex=-1;
                Task=1;
            }
            break;
        case 2000:                                                              // Tray Full
            if(TrayForm.bEnableAMR)                                               //Eastsun 20260514 F010 AMR full tray skip
            {
                Task = 1;
                break;
            }

            if(DoMagazineUpDown(-1 , 0)==1)
            {
                AnsiString aUnloader="";
                for(int j=0; j<iTestBinCount; j++)
                {
                    int temp=Prod.iT6CatData[j];
                    if(temp<0)
                        continue;

                    if(iMagazineFullTrayIndex+iMagMin==temp)                    //Auto1-1,2_Auto2-3,4_Auto3-5,6_Fix1-15,16_Fix2-5_Fix3-7,8,9,error.
                    {
                        aUnloader+=AnsiString(j)+",";
                    }
                }

                if(Prod.iIfErrorT6==iMagazineFullTrayIndex+iMagMin)
                {
                    aUnloader+="error";
                }
                else
                {
                    aUnloader.Delete(aUnloader.Length(), 1);
                }
                str.sprintf("Magazine%d(Bin:%s)", iMagazineFullTrayIndex+1, aUnloader);
                asMagazineBinFull=str;
                RecordProcess(str);
//                #ifndef SOFT_SIMULTE
                    iUnLoaderCount=99;               // 必須不為0 Handler才不停機
                    str1.sprintf("Magazine tray out, please remove the tray in slot.");
                    ShowUnloaderTrayMessage(str1, str);

    //                MOT[MMMagazineTary1+iMagazineFullTrayIndex].SetTray(NULL_IC);

                    Task=2004;
                    iMagazineStatus=1;  //Ifor 20231130 add: 0: Normal 1:Full Tray 2:Manual put tray
                    bResetTimer=true;
//                #else
//                    Task=2010;
//                #endif
            }
            break;
        case 2004:
            #ifndef SOFT_SIMULTE
            if(Sen[SnMagazineSafeDoor].IsOff()==true ||
               bHasOpenMagDoor==true)//JerryYang 20241225 : 暫停狀態開Magazine安全門
            {
                Task=2005;
                bResetTimer=true;
            }
            else
            {
                if(MyMessageBox->fShow==false)
                {
                    if(bResetTimer)
                    {
                        htMagGetNewTray.SetSecAndOn(60);
                        bResetTimer=false;
                    }

                    if(htMagGetNewTray.Off()==true)
                    {
                        AnsiString str1;
                        str1.sprintf("Magazine tray out, please remove the tray in slot.");
                        ShowMyMessage(str1, asMagazineBinFull);
                        bResetTimer=true;
                    }
                }
            }
            #else
                Task=2005;
            #endif
            break;
        case 2005:
            #ifndef SOFT_SIMULTE
            if(MyMessageBox->fShow==false)
                Task=2006;
            #else
            if(MyMessageBox->fShow==true)
                MyMessageBox->Close();
            Task=2006;
            #endif
            break;
        case 2006:
            #ifndef SOFT_SIMULTE
            if(Sen[SnMagazineSafeDoor].IsOff()==false)
            {
                Task=2010;
            }
            #else
                Task=2010;

            #endif
            break;
        case 2010:
            if(DoMagazineUpDown(iMagazineFullTrayIndex, 2)==true)    //JerryYang 20221122 : alarm完再檢查一次tray盤是否已正常取出, tray arm進tray必須手動取出  //JerryYang Meed to check here
            {
                iMagazineStatus=0;  //Ifor 20231130 add: 0: Normal 1:Full Tray 2:Manual put tray
                MOT[MMMagazineTary1+iMagazineFullTrayIndex].ClearTray(__FUNC__);
                Task=1;
            }
            break;
        case 2100:
            if(DoMagazineUpDown(-1 , 3))
            {
                iUnLoaderCount=99;               // 必須不為0 Handler才不停機
                str.sprintf("Please Remove The Tray In Slot.");
                str1.sprintf("Manually Remove the Tray.");
                ShowUnloaderTrayMessage(str1, str);
                Task=2104;
            }
            break;
        case 2104:
            #ifndef SOFT_SIMULTE
            if(Sen[SnMagazineSafeDoor].IsOff()==true ||
               bHasOpenMagDoor==true)//JerryYang 20241225 : 暫停狀態開Magazine安全門
            {
                Task=2105;
            }
            #else
                Task=2105;
            #endif
            break;
        case 2105:
            if(MyMessageBox->fShow==false)
                Task=2106;
            break;
        case 2106:
            #ifndef SOFT_SIMULTE
            if(Sen[SnMagazineSafeDoor].IsOff()==false)
            {
                Task=2107;
            }
            #else
                Task=2107;

            #endif
            iCheckMagazineSlot=0;
            break;
        case 2107:  //Ifor 20231201 add:手動退盤後檢查無Tray時清除資料
            if(DoMagazineUpDown(iCheckMagazineSlot, 2)==true)    //JerryYang 20221122 : alarm完再檢查一次tray盤是否已正常取出, tray arm進tray必須手動取出  //JerryYang Meed to check here
            {
                htMagGetNewTray.SetMSAndOn(200);
                Task=2108;
            }
            break;
        case 2108:
            if(htMagGetNewTray.Off()==true)
            {
                iCheckMagazineSlot++;

                for(int i=iCheckMagazineSlot; i<MAX_MGZ_TRAY; i++)
                {
                    if(MOT[MMMagazineTary1+i].fHasTray==true)
                    {
                        iCheckMagazineSlot=i;
                        break;
                    }
                    else
                    {
                        iCheckMagazineSlot++;
                    }
                }

                if(iCheckMagazineSlot>=14)
                {
                    Task=2109;
//                    iCheckMagazineSlot=0;
//                    RecordProcess("Finish Manually Get Tray");
//                    iMagazineStatus=0;  //Ifor 20231130 add: 0: Normal 1:Full Tray 2:Manual put tray
                }
                else
                {
                    Task=2107;
                }
            }
            break;
        case 2109:
            if(DoMagazineUpDown(iAuto3MagazineIndex, 2)==true)
            {
                Task=1;
                RecordProcess("Finish Manually Get Tray");
                iMagazineStatus=0;  //Ifor 20231130 add: 0: Normal 1:Full Tray 2:Manual put tray
            }
            break;
        case 3000:                                                              //Sam 20221116 : Magazine TrayArm 自動補 Tray
            if(MOT[MMMagazineTary1+iWhichMag].fHasTray==true &&
               iWhichMag!=-1 &&
               iWhichAuto==2 &&
               MOT[MMMagazineTary1+iWhichMag].Tray.FullIC()==false)             //Eastsun 20260515 F023: E12 prevent drag-in/out when full
            {
                InitialCatchTrayGetNewTray();
                bMagCatchTrayfalg=false;
                //bMagGetNewTrayflag=true; //Eastsun 20260515 F023: E12 causes hangup
                Task=100;
                break;
            }
            else
            {
                bMagCatchTrayfalg=true;                                             //JerryYang 20221121 : AUTOI3 Magazine還在夾TRAY的旗標
                Cylinder[C_AutoEdgePush[iMagAtAuto]].Off();                         //Aut3 側推汽缸收起來 Tray Arm 好放 Tray
                bflag1=Cylinder[C_CatchMagazineTray].Pop();                         //Magazine 爪子收起來讓 Tray Arm 好放 Tray
                if(bflag1)
                {
                    bflag1=false;
                    Task=3100;
                }
                else
                {
                    break;
                }
            }
            break;
        case 3100:
            #ifdef SOFT_SIMULTE
                bflag1=true;
            #else
                bflag1=Cylinder[C_TrayX_UpDown].OffSensor();                    //Tray Arm 汽缸上升完成
            #endif

            if(MOT[MMMagazineTary1+iWhichMag].fHasTray==true &&
               iWhichMag!=-1 &&
               iWhichAuto==2)
            {
                InitialCatchTrayGetNewTray();
                bMagCatchTrayfalg=false;
                bMagGetNewTrayflag=true;
                Task=100;
                break;
            }
            else
            {
                if(MOT[MMAuto3].fHasTray && bflag1)                                 //Tray Arm 好放 Tray
                {
                    bflag1=false;
                    Task=3200;
                }
                else
                {
                    break;
                }
            }
        case 3200:
            bflag1=Cylinder[C_CatchMagazineTray].Push();
            if(bflag1)
            {
                Cylinder[C_AutoEdgePush[iMagAtAuto]].On();
                Task=3300;
                htMagGetNewTray.SetMSAndOn(500);                                //JerryYang 20221119 : 側推完要delay一下才檢查
            }
            else
            {
                break;
            }
        case 3300:
            if(htMagGetNewTray.Off()==false)                                    //JerryYang 20221119 : 側推完要delay一下才檢查
                break;
            bMagCatchTrayfalg=false;                                            //JerryYang 20221121 : AUTOI3 Magazine還在夾TRAY的旗標
            #ifdef SOFT_SIMULTE
            Task=1;
            #else
            if(LastSet.iRealDummy!=DUMMY)
            {
                if(Sen[SnAuto3EdgePush].IsOn() ||
                   Cylinder[C_CatchMagazineTray].OnSensor()==false ||
                   Sen[SnAuto3TrayDetect].IsOn()==false)                        //檢查是否有沒放好 Tray
                {
                    ret=ShowErrorMessage("JAM1311", K_RETRY|K_SKIP, MMAuto3,0, "DoAuto3Magazine_3300");
                    if(ret==K_SKIP)
                    {
                        MOT[MMAuto3].ClearTray(__FUNC__);
                        iAuto3MagazineIndex=-1;                                 //Ifor 20240606 add:清除Tray Data時清除狀態
                        Task=1;
                        break;
                    }
                    else
                    {
                        Cylinder[C_CatchMagazineTray].Off();
                        Cylinder[C_AutoEdgePush[iMagAtAuto]].Off();
                        Task=3200;
                        break;
                    }
                }
                else
                {
                    Task=1;
                }
            }
            else
            {
                Task=1;
            }
            #endif
            break;
        case 4000:
            bChaneMagTrayflag=true;
            if(iAuto3MagazineIndex>=0)                                            //Eastsun 20260514 F010 AMR full tray to AGV
            {
                if(MOT[MMMagazineTary1+iAuto3MagazineIndex].Tray.FullIC()==true &&
                   TrayForm.bEnableAMR)
                {
                    if(BinSelect[iTestRunMode].bMagazineLink[iAuto3MagazineIndex+1]==false)
                    {
                        Task=1;
                        bChaneMagTrayflag=false;
                        break;
                    }
                }
            }

            if(DoCatchTrayChangeTray(-1)==true)
            {
                bChaneMagTrayflag=false;
                iAuto3MagazineIndex=-1;
                if(iWhichMag!=-1 &&                                             //JerryYang 20250910 : fix Magazine
                   MOT[MMMagazineTary1+iWhichMag].fHasTray==true &&
                   iWhichAuto==2)
                {
                    bMagCatchTrayfalg=false;
                    bMagGetNewTrayflag=true;
                    Task=100;
                }
                else
                {
                    RecordProcess("Magazine Get New Tray");
                    bMagCatchTrayfalg=true;
                    Task=3000;
                }
            }
            break;
    }
}
//------------------------------------------------------------------------------
void InitialCatchTrayChangeTray()
{
    iCatchTrayChangeTrayTask=1;
}
//------------------------------------------------------------------------------
void InitialCatchTrayGetNewTray()
{
    iCatchTrayGetNewTrayTask=1;
}
//------------------------------------------------------------------------------
TQPF_Timer htCatchTrayGetNewTray;
//------------------------------------------------------------------------------
bool DoCatchTrayGetNewTray(int iWitchTray)
{
    int &Task=iCatchTrayGetNewTrayTask;

    int ret;
    bool bFlag[3]={false, false, false};
    AnsiString str;
    static HTimer hSenCheckTimer;

    if(iWitchTray<0 || iWitchTray>=MAX_MGZ_TRAY)
    {
        ShowMyMessage("DoCatchTrayGetNewTray:iWitchTray=%d", iWitchTray);
        return false;
    }

    if(MOT[MMMagazineTary1+iWitchTray].fHasTray==false &&
       Task!=1200)
    {
        ShowMyMessage("DoCatchTrayGetNewTray:iWitchTray=%d", iWitchTray);
        return false;
    }

    switch(Task)
    {
        case 1:                                                                 //AUTO3側推放掉, Magazine夾tray on才能移動Y方向
            bFlag[0]=Cylinder[C_CatchMagazineTray].OffSensor();
            Cylinder[C_AutoEdgePush[iMagAtAuto]].Off();                         //kevin 20140603
            bFlag[1]=Cylinder[C_AutoEdgePush[iMagAtAuto]].OffSensor();
            bFlag[2]=Cylinder[C_CatchMagazineTray1].OffSensor();
            #ifndef SOFT_SIMULTE
            if(bFlag[0]==true && bFlag[1]==true && bFlag[2]==true)
            {
                InitialMagazineUpDown();                                        // 2012.11.29 , Joye , AMD HT-7046M
                Task=100;
            }
            else
            {
                Task=50;
                htCatchTrayGetNewTray.SetSecAndOn(10);
            }
            #else
            InitialMagazineUpDown();                                            // 2012.11.29 , Joye , AMD HT-7046M
            Task=100;
            #endif
            break;
        case 50:                                                                //AUTO3側推放掉, Magazine夾tray on才能移動Y方向
            bFlag[0]=Cylinder[C_CatchMagazineTray].Pop();
            Cylinder[C_AutoEdgePush[iMagAtAuto]].Off();                         //kevin 20140603
            bFlag[1]=Cylinder[C_AutoEdgePush[iMagAtAuto]].OffSensor();
            bFlag[2]=Cylinder[C_CatchMagazineTray1].OffSensor()==true;
            #ifndef SOFT_SIMULTE
            if(bFlag[0]==true && bFlag[1]==true && bFlag[2]==true)
            {
                InitialMagazineUpDown();                                        // 2012.11.29 , Joye , AMD HT-7046M
                Task=100;
            }
            else if(htCatchTrayGetNewTray.Off()==true)
            {
                if(Cylinder[C_CatchMagazineTray].OffSensor()==false)
                {
                    ShowMyMessage("C_CatchMagazineTray off but the off sensor is not on.");
                }

                if(Cylinder[C_CatchMagazineTray1].OffSensor()==false)
                {
                    ShowMyMessage("C_CatchMagazineTray1 off but the off sensor is not on.");
                }

                if(Cylinder[C_AutoEdgePush[iMagAtAuto]].OffSensor()==false)
                {
                    ShowMyMessage("C_CatchMagazineTray push but the sesnsor is off.");
                }
                htCatchTrayGetNewTray.SetSecAndOn(10);
            }
            #else
            InitialMagazineUpDown();                                            // 2012.11.29 , Joye , AMD HT-7046M
            Task=100;
            #endif
            break;
        case 100:
            if(DoMagazineUpDown(iWitchTray ,3)==true)
            {
                if(bCheckMagazineSafeSensor())                                  //Sam 20221116 : Magazine TrayArm 自動補 Tray
                    break;
                if(Sen[SnMagazineDetect].IsOn()==true)                          // Alarm : Standby has tray can not Move Catch
                {
                    ShowMyMessage("SnMagazineDetect is on, Standby position has tray can not move to catch!");
                    break;
                }
                Task=500;
            }
            break;
        case 500:
            if(CatchMgzTrayMove(Prod.iCatchMazTray_Front))                      //Sam 20221116 : Magazine TrayArm 自動補 Tray
            {
                Task=600;
                htCatchTrayGetNewTray.SetSecAndOn(10);
            }
            break;
        case 600:
            if(Cylinder[C_CatchMagazineTray].Pop()==true && Cylinder[C_CatchMagazineTray1].OffSensor()==true)
            {
                InitialMagazineUpDown();                                        // 2012.11.29 , Joye , AMD HT-7046M
                Task=700;
            }
            else if(htCatchTrayGetNewTray.Off())
            {
                if(Cylinder[C_CatchMagazineTray].OffSensor()==false)
                {
                    ShowMyMessage("C_CatchMagazineTray off but the off sensor is not on.");
                }

                if(Cylinder[C_CatchMagazineTray1].OffSensor()==false)
                {
                    ShowMyMessage("C_CatchMagazineTray1 off but the off sensor is not on.");
                }
                htCatchTrayGetNewTray.SetSecAndOn(10);
            }
            break;
        case 700:
            if(DoMagazineUpDown(iWitchTray, 1)==true)                           //移動Magazine到換tray高度
            {
                if(LastSet.iRealDummy!=DUMMY)
                {
                    if(MOT[iMMgzTray[iWitchTray]].fHasTray==false)              // 2012.12.12 , Joye , AMD HT-7046M --------------------
                    {
                        MOT[iMMgzTray[iWitchTray]].Tray.XItem=AutoForm[iMagAtAuto]->XDivision;
                        MOT[iMMgzTray[iWitchTray]].Tray.YItem=AutoForm[iMagAtAuto]->YDivision;
                        MOT[iMMgzTray[iWitchTray]].InitNewTray(NULL_IC, false, __FUNC__);
                        MOT[iMMgzTray[iWitchTray]].fHasTray=true;
                        break;
                    }
                    else
                    {
                        if(Sen[SnMagazineDetect].IsOff()==true)                 //Magazine Tray Missing
                        {
                            str.sprintf("Magazine%d", iWitchTray+1);            // Alarm : Tray Miss
                            ShowErrorMessage("WAR2372", K_RETRY, MMagazine, 0, str);
                            break;
                        }
                    }
                }

                if(Sen[SnAuto3TrayDetect].IsOn()==true)                         //Auto 3不能有tray盤
                {
                    ShowErrorMessage("MES1322", K_RETRY, MMAuto3, 0, "");       // Alarm : Track has tray
                    break;
                }

                Task=800;
                htCatchTrayGetNewTray.SetSecAndOn(10);
            }
            break;
        case 800:
            if(Cylinder[C_CatchMagazineTray].Push()==true &&
               Cylinder[C_CatchMagazineTray1].OnSensor()==true)
            {
                MOT[MMAuto3_Car].fHasTray=true;
                iMagChangeStep=2;
                Task=1000;
            }
            else if(htCatchTrayGetNewTray.Off()==true)
            {
                if(Cylinder[C_CatchMagazineTray].OnSensor()==false)
                {
                    ShowMyMessage("C_CatchMagazineTray push but the sesnsor is off.");
                }

                if(Cylinder[C_CatchMagazineTray1].OnSensor()==false)
                {
                    ShowMyMessage("C_CatchMagazineTray push but the sesnsor is off.");
                }
                htCatchTrayGetNewTray.SetSecAndOn(10);
            }
            break;

        case 1000:
            if(CatchMgzTrayMove(Prod.iCatchMazTray_Rear))                       //Sam 20221116 : Magazine TrayArm 自動補 Tray
            {
                Task=1100;
            }
            break;
        case 1100:
             Cylinder[C_AutoEdgePush[iMagAtAuto]].Push();                       //kevin 20140603
             bFlag[0]=Sen[SnAuto3EdgePush].IsOn();                              //kevin 20140603 add sensor

            if(LastSet.iRealDummy==DUMMY)                                       // 2015.01.22 , Joye , AMD
            {
                bFlag[0]=false;
            }

            if(bFlag[0]==false &&
              ((Cylinder[C_FixTray_FullPlace].Enable &&
                Cylinder[C_AutoEdgePush[iMagAtAuto]].OffSensor()==false) ||
              (Cylinder[C_FixTray_FullPlace].Enable==false)))
            {
                if(LastSet.iRealDummy==DUMMY)                                   // Set Data to Auto 3 {
                {
                    if(MOT[iMMgzTray[iWitchTray]].fHasTray==false)
                        MOT[iMMgzTray[iWitchTray]].fHasTray=true;
                }
                MOT[MMAuto3].MoveTrayData(MOT[iMMgzTray[iWitchTray]]);
                MOT[MMAuto3_Car].ClearTray(__FUNC__);

                hSenCheckTimer.Set(TRAY_MISS_CHECK_TIME);                       // TRAY_MISS_CHECK_TIME X 0.1 Sec
                hSenCheckTimer.On();

                Task=1200;
            }
            else
                break;

        case 1200:
            if(LastSet.iRealDummy!=DUMMY &&
              (Sen[SnAuto3EdgePush].IsOn()==true ||
               Sen[SnAuto3TrayDetect].IsOff()==true))
            {
                if(hSenCheckTimer.Off())                                        //AUTO3 TRAY MISSING
                {
                    ret=ShowErrorMessage("WAR1330", K_SKIP|K_RETRY, MMAuto3, false, "DoCatchTrayGetNewTray 1200");
                    if(ret==K_SKIP)
                    {
                        MOT[MMAuto3].ClearTray(__FUNC__);
                        Cylinder[C_AutoEdgePush[iMagAtAuto]].Off();
                        iAuto3MagazineIndex=-1;                                 //Ifor 20240606 add:清除Tray Data時清除狀態
                        Task=1;
                        return true;
                    }

                    if(SoftStop)
                    {
                        Cylinder[C_AutoEdgePush[iMagAtAuto]].Off();
                    }
                    Task=1000;
                }
            }
            else
            {
                Task=1;
                return true;
            }
            break;
    }

    return false;
}
//----------------------------------------------------------------------------
TQPF_Timer htCatchTrayChangeTray;
bool DoCatchTrayChangeTray(int iWitchTray)
{
    int &Task=iCatchTrayChangeTrayTask;

    int ret;//,iType=0;
    bool bFlag[3] = {false, false, false};
    AnsiString str="", sBuffer="";
    AnsiString str1="";
    static HTimer hSenCheckTimer;
    static int iLinkCount=0;
    static bool bNeedCheckZ[14]={false, false, false,false, false, false,false, false, false,false, false, false,false, false};
    static bool bResetTimer=true;
//    bool C_Auto3_EdgePush=false;//kevin 20140603

    if(bStartCheckZ==true)
    {
        for(int i=0; i<14; i++)
        {
            bNeedCheckZ[i]=true;
        }
        bStartCheckZ=false;
    }

    switch(Task)
    {
        case 1:
            Cylinder[C_AutoEdgePush[iMagAtAuto]].On();   //kevin 20140603

            if(bNeedAMRMagazineUnload==true)                                  //Eastsun 20260514 F010 AMR magazine unload
            {
                Task=300;
                htCatchTrayChangeTray.SetSecAndOn(5);
            }
            else if(LastSet.iRealDummy!=DUMMY && Sen[SnAuto3EdgePush].IsOn()==true)
            {
                ret=ShowErrorMessage("MES1322", K_SKIP|K_RETRY, MMAuto3, 0, "");
                if(ret==K_SKIP)
                {
                    MOT[MMAuto3].ClearTray(__FUNC__);
                    Cylinder[C_AutoEdgePush[iMagAtAuto]].Off();
                    iAuto3MagazineIndex=-1;
                    return true;
                }

                if(SoftStop)
                {
                    Cylinder[C_AutoEdgePush[iMagAtAuto]].Off();
                }
            }
            else
            {
                Task=100;
                htCatchTrayChangeTray.SetSecAndOn(10);
            }
            iLinkCount=0;
        break;

        case 100:
            Cylinder[C_AutoEdgePush[iMagAtAuto]].Off();  //kevin 20140603 add sensor();   //kevin 20140603
            bFlag[0]=Cylinder[C_AutoEdgePush[iMagAtAuto]].OffSensor();  //kevin 20140603 add sensor
            bFlag[1]=Cylinder[C_CatchMagazineTray].Push();
            bFlag[2]=Cylinder[C_CatchMagazineTray1].OnSensor()==true;

            if(bFlag[0]==true && bFlag[1]==true && bFlag[2]==true)
            {
                MOT[MMAuto3_Car].fHasTray=true;

                InitialMagazineUpDown();    // 2012.11.29 , Joye , AMD HT-7046M
                Task=500;
                str1.sprintf("Magazine %d is Out Start.", iAuto3MagazineIndex+1);
                RecordProcess(str1);
            }
            else if(htCatchTrayChangeTray.Off()==true)
            {
                if(bFlag[0]==false)
                {
                    ShowMyMessage("C_CatchMagazineTray off but the off sensor is not on.");
                }

                if(bFlag[1]==false || bFlag[2]==false)
                {
                    ShowMyMessage("C_CatchMagazineTray push but the sesnsor is off.");
                }
                htCatchTrayChangeTray.SetSecAndOn(10);
            }
            break;
        //==> Eastsun 20260519:整合
        case 300:
            Cylinder[C_Auto3EdgePush].Off();  //kevin 20140603 add sensor();   //kevin 20140603
            bFlag[0]=Cylinder[C_Auto3EdgePush].OffSensor();  //kevin 20140603 add sensor

            if(bFlag[0]==true)
            {
                str1.sprintf("Magazine %d is Out Start.", iAuto3MagazineIndex+1);
                htCatchTrayChangeTray.SetSecAndOn(5);
                RecordProcess(str1);
                Task=310;
            }
            else if(htCatchTrayChangeTray.Off()==true)
            {
                if(bFlag[0]==false)
                    ShowMyMessage("C_Auto3EdgePush off but the off sensor is not on.");

                htCatchTrayChangeTray.SetSecAndOn(5);
            }
            break;
        case 310:
            if(Cylinder[C_CatchMagazineTray].Pop()==true && Cylinder[C_CatchMagazineTray1].OffSensor()==true)
            {
                Task=320;
            }
            else if(htCatchTrayChangeTray.Off()==true)
            {
                if(Cylinder[C_CatchMagazineTray].OffSensor()==false || Cylinder[C_CatchMagazineTray1].OffSensor()==false)
                {
                    ShowMyMessage("C_CatchMagazineTray pop but the off sensor is not on.");
                }
                htCatchTrayChangeTray.SetSecAndOn(5);
            }
            break;
        case 320:
            if(CatchMgzTrayMove(Prod.iCatchMazTray_Front))  //Sam 20221116 : Magazine TrayArm 自動補 Tray
            {
                InitialMagazineUpDown();
                Task=800;
            }
            else
            {
                break;
            }
        //<== Eastsun 20260519:整合
        case 500:
            if(DoMagazineUpDown(iAuto3MagazineIndex , 2))   //要把AUTO3的TRAY放回Magazine
            {
                bMgzTrayCatcCynHomeNeedOff=false;

                if(bCheckMagazineSafeSensor())  //Sam 20221116 : Magazine TrayArm 自動補 Tray
                    break;
                if(Sen[SnMagazineDetect].IsOn()==true)
                {
                    str.sprintf("Magazine%d", iAuto3MagazineIndex+1);
                    ShowErrorMessage("WAR2371", K_RETRY, MMagazine, 0, str);
                    return false;
                }

                if(bNeedCheckZ[iAuto3MagazineIndex]==true)
                {
                    Task=510;
                    bNeedCheckZ[iAuto3MagazineIndex]=false;
                }
                else
                {
                    Task=600;
                }
                iMagChangeStep=1;   //表示正要把AUTO3的TRAY退回Magazine中
            }
            else
                break;
        case 510:
                if(CatchMgzTrayMove(Prod.iCatchMazTray_Front+iMagazineCheckZPos))  //Sam 20221116 : Magazine TrayArm 自動補 Tray
                {
                    #ifdef SOFT_SIMULTE
                    Task=600;
                    #else
                    if(LastSet.iRealDummy!=DUMMY)
                    {
                        if(Sen[SnMagazineTrackDetect].IsOn()==true)
                        {
                            Task=600;
                        }
                        else
                        {
                            ShowMyMessage("SnMagazineTrackDetect is off, Please Check Magazine Z axis Sensor!");
                            Task=600;
                        }
                    }
                    else
                    {
                        Task=600;
                    }
                    #endif
                }
                break;

        case 600:
            if(CatchMgzTrayMove(Prod.iCatchMazTray_Front))  //Sam 20221116 : Magazine TrayArm 自動補 Tray
            {
                hSenCheckTimer.Set(10); // TRAY_MISS_CHECK_TIME X 0.1 Sec
                hSenCheckTimer.On();
                Task=700;
            }
            else
            {
                break;
            }
        case 700:
            // 2012.12.10 , Joye , AMD HT-7046M ------------------------------>>
            if(hSenCheckTimer.Off())    //Ifor  20231113
            {
                if(LastSet.iRealDummy!=DUMMY && Sen[SnMagazineDetect].IsOff()==true)
                {
                    if(MOT[MMAuto3_Car].fHasTray==true)
                    {
                        // Alarm : Tray Miss    //Magazine Tray Missing
                        str.sprintf("Magazine%d", iAuto3MagazineIndex+1);
                        ret=ShowErrorMessage("WAR2372", K_RETRY|K_SKIP, MMagazine, 0, str);

                        if(ret==K_SKIP)
                        {
                            // Set Data to Magazine Tray
//                            MOT[MMMagazineTary1+iAuto3MagazineIndex].MoveTrayData(MOT[MMAuto3]);
//                            MOT[MMAuto3_Car].fHasTray=false;
//                            MOT[MMMagazineTary1+iAuto3MagazineIndex].ClearTray();
                            Task=6000;
                        }
                        else
                        {
                        Task=600;
                    }
                    break;
                }
            }

                if(bCheckMagazineSafeSensor())  //Sam 20221116 : Magazine TrayArm 自動補 Tray
                {
                    // Alarm : Track has tray
                    //ShowMyMessage("SnMagazineTrackDetect is on, Magazine can not move");
                    Task = 600;
                    break;
                }
                Task=710;
                htCatchTrayChangeTray.SetSecAndOn(10);
            }
            break;
        case 710:
            if(Cylinder[C_CatchMagazineTray].Pop()==true && Cylinder[C_CatchMagazineTray1].OffSensor()==true)
            {
                if(LastSet.iRealDummy!=DUMMY && Sen[SnMagazineDetect].IsOff()==true)
                {
                    if(MOT[MMAuto3_Car].fHasTray==true)
                    {
                        // Alarm : Tray Miss
                        str.sprintf("Magazine%d", iAuto3MagazineIndex+1);
                        ret=ShowErrorMessage("WAR2372", K_SKIP, MMagazine, 0, str);
//                        ret =_ShowMagazineError(MMMagazineTary1+iAuto3MagazineIndex , TRAY_MISS , K_RETRY | K_SKIP  ,0,__FUNC__);     //pig 20180612 ModifyNewAlarmCode
                        if(ret==K_SKIP)
                        {
                            // Set Data to Magazine Tray {
//                            MOT[MMMagazineTary1+iAuto3MagazineIndex].MoveTrayData(MOT[MMAuto3]);
//                            MOT[MMAuto3_Car].fHasTray=false;
//                            MOT[MMMagazineTary1+iAuto3MagazineIndex].ClearTray();
                            // }
                            Task=6000;
                        }
                        else
                        {
                            Task=6000;
                        }

                        break;
                    }
                }
                else
                {
                    if(MOT[MMAuto3_Car].fHasTray==true)
                    {
                        MOT[iMMgzTray[iAuto3MagazineIndex]].MoveTrayData(MOT[MMAuto3]);
                        MOT[MMAuto3].ClearTray(__FUNC__);
                        MOT[MMAuto3_Car].fHasTray=false;
                    }
                }
                iMagChangeStep=0;   //AUTO3這時候已經退回Magazine, 狀態改為0
                //if(Sen[SnMagazineTrackDetect].IsOn()==true)
                if(bCheckMagazineSafeSensor())  //Sam 20221116 : Magazine TrayArm 自動補 Tray
                {
                    // Alarm : Track has tray
                    //ShowMyMessage("SnMagazineTrackDetect is on, Magazine can not move");
                    Task=600;
                    break;
                }

                InitialMagazineUpDown();    // 2012.11.29 , Joye , AMD HT-7046M

                if( MOT[iMMgzTray[iAuto3MagazineIndex]].Tray.FullIC()==true )
                {
                    if(BinSelect[iTestRunMode].bMagazineLink[iAuto3MagazineIndex+1]==false)
                    {
                        Task=2000;
                        break;
                    }
                }

                if(iWitchTray<0 || iWitchTray>=MAX_MGZ_TRAY)  // 不在補新Tray
                {
                    Task=3000;
                    break;
                }

                Task=800;
            }
            else if(htCatchTrayChangeTray.Off()==true)
            {
                if(Cylinder[C_CatchMagazineTray].OffSensor()==false || Cylinder[C_CatchMagazineTray1].OffSensor()==false)
                {
                    ShowMyMessage("C_CatchMagazineTray pop but the off sensor is not on.");
                }
                htCatchTrayChangeTray.SetSecAndOn(10);
            }
            break;
        case 800:
            if(TrayForm.bEnableAMR && iTrayFeed==1 && bNeedAMRMagazineUnload==false) //Eastsun 20260514 F010
            {
               if(DoMagazineUpDown(iWitchTray, 3)==true)
               {
                   Task=3000;
               }
               break;
            }
            else if(iTrayFeed==1 && !TrayForm.bEnableAMR)    //kevin 20140604
            {
               if(DoMagazineUpDown(iWitchTray, 3)==true)
               {
                   Task=3000;
               }
               break;
            }
            else if(DoMagazineUpDown(iWitchTray, 1)==true)     //Magazine移動到要取TRAY的高度，準備把TRAY夾出來
            {
                if(LastSet.iRealDummy!=DUMMY)
                {
                    // 2012.12.12 , Joye , AMD HT-7046M --------------------
                    if(MOT[iMMgzTray[iWitchTray]].fHasTray==false)
                    {
                        MOT[iMMgzTray[iWitchTray]].Tray.XItem=AutoForm[iMagAtAuto]->XDivision;
                        MOT[iMMgzTray[iWitchTray]].Tray.YItem=AutoForm[iMagAtAuto]->YDivision;
                        MOT[iMMgzTray[iWitchTray]].InitNewTray(NULL_IC, false, __FUNC__);
                        MOT[iMMgzTray[iWitchTray]].fHasTray=true;
                        break;
                    }
                    else    // ---------------------------------------------
                    {
                        // Alarm : Magazine no tray
                        if(LastSet.iRealDummy!=DUMMY && Sen[SnMagazineDetect].IsOff()==true)
                        {
                            str.sprintf("Magazine%d", iWitchTray+1);
                            ShowErrorMessage("WAR2372", K_RETRY, MMagazine, 0, str);
                            break;
                        }
                    }
                }

                //if(Sen[SnMagazineTrackDetect].IsOn()==true)
                if(bCheckMagazineSafeSensor())  //Sam 20221116 : Magazine TrayArm 自動補 Tray
                {
                    // Alarm : Track has tray
                    //ShowMyMessage("SnMagazineTrackDetect is on, Magazine can not move");
                    Task=600;
                    break;
                }

                Task=900;
                htCatchTrayChangeTray.SetSecAndOn(10);
            }
            else
                break;
        case 900:
            if(Cylinder[C_CatchMagazineTray].Push()==true && Cylinder[C_CatchMagazineTray1].OnSensor()==true)
            {
                if(Sen[SnAuto3TrayDetect].IsOn()==true)       //JerryYang 20240229 : Auto 3不能有tray盤
                {
                    // Alarm : Track has tray
                    ShowErrorMessage("MES1322", K_RETRY, MMAuto3, 0, "");
                    break;
                }
                MOT[MMAuto3_Car].fHasTray=true;
                iMagChangeStep=2;   //這時候要把Magazine中的tray移動到AUTO3
                Task=1000;
            }
            else if(htCatchTrayChangeTray.Off()==true)
            {
                if(Cylinder[C_CatchMagazineTray].OnSensor()==false)
                {
                    ShowMyMessage("C_CatchMagazineTray push but the sesnsor is off.");
                }

                if(Cylinder[C_CatchMagazineTray1].OnSensor()==false)
                {
                    ShowMyMessage("C_CatchMagazineTray push but the sesnsor is off.");
                }
                htCatchTrayChangeTray.SetSecAndOn(10);
            }
            break;
        case 1000:
            if(CatchMgzTrayMove(Prod.iCatchMazTray_Rear))   //Sam 20221116 : Magazine TrayArm 自動補 Tray
            {
                Task=1100;
            }
            else
                break;

        case 1100:
            Cylinder[C_AutoEdgePush[iMagAtAuto]].On();   //kevin 20140603
            if( LastSet.iRealDummy==DUMMY)
            {
                if(MOT[iMMgzTray[iWitchTray]].fHasTray==false)
                    MOT[iMMgzTray[iWitchTray]].fHasTray=true;
            }
            htCatchTrayChangeTray.SetMSAndOn(500);  //JerryYang 20221202 change timer

            Task=1150;
            break;
        case 1150:
            if(htCatchTrayChangeTray.Off()==true)
            {
                MOT[MMAuto3].MoveTrayData(MOT[iMMgzTray[iWitchTray]]);     //JerryYang 20221119 : 延後過資料的時間點
                MOT[MMAuto3_Car].fHasTray=false;
                Task=1200;
            }
            break;
        case 1200:
            if(LastSet.iRealDummy!=DUMMY &&
              (Sen[SnAuto3EdgePush].IsOn()==true || Sen[SnAuto3TrayDetect].IsOff()==true))
            {
                if(htCatchTrayChangeTray.Off()==true)
                {
                    ret=ShowErrorMessage("WAR1330", K_SKIP|K_RETRY, MMAuto3, false, "DoCatchTrayChangeTray 1200");
                    if(ret==K_SKIP)
                    {
                        MOT[MMAuto3].ClearTray(__FUNC__);
                        Cylinder[C_AutoEdgePush[iMagAtAuto]].Off();
                        iAuto3MagazineIndex=-1;                                 //Ifor 20240606 add:清除Tray Data時清除狀態
                        Task=1;
                        QueueTaskList[272].CheckTaskChange();                   //Ifor 20240722 add:
                        return true;
                    }
                    else
                    {
                        Cylinder[C_AutoEdgePush[iMagAtAuto]].On();
                        Task=1100;
                    }
                }
            }
            else
            {
                iMagChangeStep=0;   //換完TRAY了
                Task=1;
                str1.sprintf("Magazine %d is In finished.", iWitchTray+1);
                RecordProcess(str1);
                QueueTaskList[272].CheckTaskChange();                           //Ifor 20240722 add:
                return true;
            }
            break;

        case 2000:         //滿TRAY
            if(DoMagazineUpDown(iAuto3MagazineIndex , 0)==true)
            {
                AnsiString aUnloader="";
                int iRealIndex=999;
                AnsiString aLinkUnloader="";
                for(int j=0; j<iTestBinCount; j++)
                {
                    int temp=Prod.iT6CatData[j];                                //JerryYang 20250918 : modify
                    if(temp<0)
                      continue;

                    if(BinSelect[iTestRunMode].bMagazineLink[iAuto3MagazineIndex]==true)
                    {
                        for(int i=0; i<14; i++)
                        {
                            if(BinSelect[iTestRunMode].bMagazineLink[iAuto3MagazineIndex-i]==false)
                            {
                                iRealIndex=iAuto3MagazineIndex-i;
                                break;
                            }
                        }

                        if(iRealIndex+eMag1==temp)
                        {
                            //Auto1-1,2_Auto2-3,4_Auto3-5,6_Fix1-15,16_Fix2-5_Fix3-7,8,9,error.
                            aUnloader+=AnsiString(j)+",";
                        }
                    }
                    else
                    {
                        if(iAuto3MagazineIndex+eMag1==temp)
                        {
                            //Auto1-1,2_Auto2-3,4_Auto3-5,6_Fix1-15,16_Fix2-5_Fix3-7,8,9,error.
                            aUnloader+=AnsiString(j)+",";
                        }
                    }
                }

                if(Prod.iIfErrorT6==iAuto3MagazineIndex+iMagMin)
                {
                    aUnloader+="error";
                }
                else
                {
                    aUnloader.Delete(aUnloader.Length(), 1);
                }

                if(BinSelect[iTestRunMode].bMagazineLink[iAuto3MagazineIndex]==true)
                {
                    for(int i=0; i<=iAuto3MagazineIndex; i++)
                    {
                        if(iAuto3MagazineIndex-i>=iRealIndex)
                        {
                            aLinkUnloader=aLinkUnloader+IntToStr(iRealIndex+1+i);
                            if(iAuto3MagazineIndex-i!=iRealIndex)
                            {
                                aLinkUnloader=aLinkUnloader+",";
                            }
                        }
                    }
                    str.sprintf("Magazine%s(Bin:%s)", aLinkUnloader, aUnloader);
                }
                else
                {
                    str.sprintf("Magazine%d(Bin:%s)", iAuto3MagazineIndex+1, aUnloader);
                }

                asMagazineBinFull=str;
                RecordProcess(str);
//                #ifndef SOFT_SIMULTE
                    iUnLoaderCount=99;               // 必須不為0 Handler才不停機
                    str1.sprintf("Magazine tray out, please remove the tray in slot.");
                    ShowUnloaderTrayMessage(str1, str);
                    Task=2004;
                    bResetTimer=true;
                    iMagazineStatus=1;  //Ifor 20231130 add: 0: Normal 1:Full Tray 2:Manual put tray
//                #else
//                    Task=2010;
//                #endif
            }
            break;
        case 2004:
            #ifndef SOFT_SIMULTE
            if(Sen[SnMagazineSafeDoor].IsOff()==true ||
               bHasOpenMagDoor==true)//JerryYang 20241225 : 暫停狀態開Magazine安全門
            {
                Task=2005;
            }
            else
            {
                if(MyMessageBox->fShow==false)
                {
                    if(bResetTimer)
                    {
                        htCatchTrayChangeTray.SetSecAndOn(60);
                        bResetTimer=false;
                    }

                    if(htCatchTrayChangeTray.Off()==true)
                    {
                        AnsiString str1;
                        str1.sprintf("Magazine tray out, please remove the tray in slot.");
                        ShowMyMessage(str1, asMagazineBinFull);
                        bResetTimer=true;
                    }
                }
            }
            #else
                Task=2005;
            #endif
            break;
        case 2005:
            #ifndef SOFT_SIMULTE
            if(MyMessageBox->fShow==false)
                Task=2006;
            #else
            if(MyMessageBox->fShow==true)
                MyMessageBox->Close();
            Task=2006;
            #endif
            break;
        case 2006:
            #ifndef SOFT_SIMULTE
            if(Sen[SnMagazineSafeDoor].IsOff()==false)
            {
                Task=2010;
            }
            #else
                Task=2010;

            #endif
            break;
        case 2010:
            iMagazineStatus=0;  //Ifor 20231130 add: 0: Normal 1:Full Tray 2:Manual put tray
            if(TestIF_File.iMagTraySource==0)                                   //Sam 20221116 : Magazine TrayArm 自動補 Tray
            {
                if(BinSelect[iTestRunMode].bMagazineLink[iAuto3MagazineIndex]==true)
                {
                    for(int i=0; i<14; i++)
                    {
                        MOT[MMMagazineTary1+iAuto3MagazineIndex-i].ClearTray(__FUNC__);
                        if(BinSelect[iTestRunMode].bMagazineLink[iAuto3MagazineIndex-i]==false)
                        {
                            break;
                        }
                    }
                }
                else
                {
                    MOT[MMMagazineTary1+iAuto3MagazineIndex].ClearTray(__FUNC__);
                }

                if(BinSelect[iTestRunMode].bMagazineLink[iAuto3MagazineIndex]==true)
                {
                    Task=2030;
                }
                else
                {
                    Task=2020;
                }
                iLinkCount=0;
            }
            else
            {
                MOT[iMMgzTray[iAuto3MagazineIndex]].SetTray(NULL_IC, __FUNC__);
                Task=800;
            }
            break;
        case 2020:
            if(DoMagazineUpDown(iAuto3MagazineIndex, 2)==true)                  //JerryYang 20221122 : alarm完再檢查一次tray盤是否已正常取出, tray arm進tray必須手動取出  //JerryYang Meed to check here
            {
                Task=3000;
            }
            break;
        case 2030:
            if(DoMagazineUpDown(iAuto3MagazineIndex-iLinkCount, 2)==true)    //JerryYang 20221122 : alarm完再檢查一次tray盤是否已正常取出, tray arm進tray必須手動取出  //JerryYang Meed to check here
            {
                if(BinSelect[iTestRunMode].bMagazineLink[iAuto3MagazineIndex-iLinkCount]==true)
                {
                    htCatchTrayChangeTray.SetMSAndOn(200);
                    iLinkCount++;
                    Task=2040;
                }
                else
                {
                    Task=3000;
                    iWhichMag=iAuto3MagazineIndex-iLinkCount;
                    iLinkCount=0;
                }
            }
            break;
        case 2040:
            if(htCatchTrayChangeTray.Off()==true)
            {
                Task=2030;
            }
            break;
        case 3000:
            if(DoMagazineUpDown(iAuto3MagazineIndex, 3)==true)
            {
                htCatchTrayChangeTray.SetSecAndOn(10);
                iAuto3MagazineIndex=-1;
                Task=3050;
            }
            else
                break;
        case 3050:
            if(Cylinder[C_CatchMagazineTray].Pop()==true && Cylinder[C_CatchMagazineTray1].OffSensor()==true)
            {
                Task=3100;
            }
            else if(htCatchTrayChangeTray.Off()==true)
            {
                if(Cylinder[C_CatchMagazineTray].OnSensor()==false)
                {
                    ShowMyMessage("C_CatchMagazineTray push but the sesnsor is off.");
                }

                if(Cylinder[C_CatchMagazineTray1].OnSensor()==false)
                {
                    ShowMyMessage("C_CatchMagazineTray push but the sesnsor is off.");
                }
                htCatchTrayChangeTray.SetSecAndOn(10);
            }
            break;
        case 3100:
            if(CatchMgzTrayMove(Prod.iCatchMazTray_Rear))   //Sam 20221116 : Magazine TrayArm 自動補 Tray
            {
                Task=1;
                iAuto3MagazineIndex=-1;                                         //Ifor 20240606 add:清除Tray Data時清除狀態
                bMagazineGetNewTray=true;
                QueueTaskList[272].CheckTaskChange();                           //Ifor 20240722 add:
                bResetMagGetTime=true;
                sBuffer.sprintf("DoCatchTrayChangeTray Task:3100, iOutArmWhichAuto:%d, MOT[MMAuto3].fHasTray:%d, iWhichMag:%d, iAuto3MagazineIndex:%d, MOT[MMMagazineTary1+iWhichMag].fHasTray:%d",
                                iOutArmWhichAuto, MOT[MMAuto3].fHasTray, iWhichMag, iAuto3MagazineIndex, MOT[MMMagazineTary1+iWhichMag].fHasTray);
                RecordProcess(sBuffer);

                return true;
            }
            break;
        case 6000:
            if(Cylinder[C_CatchMagazineTray].Pop()==true && Cylinder[C_CatchMagazineTray1].OffSensor()==true)
            {
                if(DoMagazineUpDown(-1 , 3))
                {
                    bHasOpenMagDoor=false;
                    str.sprintf("請開門確認Magazine%d是否有Tray", iAuto3MagazineIndex+1);
                    str1.sprintf("please open door Check Magazine%d Has Tray", iAuto3MagazineIndex+1);
                    ShowMyMessage(str1, str);
                    iMagazineStatus=3;
                    Task=6001;
                }
            }
            break;
        case 6001:
            if(bHasOpenMagDoor)
            {
                MOT[MMMagazineTary1+iAuto3MagazineIndex].MoveTrayData(MOT[MMAuto3]);
                MOT[MMAuto3_Car].fHasTray=false;
                MOT[MMMagazineTary1+iAuto3MagazineIndex].ClearTray(__FUNC__);
                iMagazineStatus=0;
                Task=600;
            }
            break;
    }

    return false;
}
//----------------------------------------------------------------------------
// iType = 0 : Home
// iType = 1 : Get Tray
// iType = 2 : Recive Tray
// iType = 3 : Standby
bool DoMagazineUpDown(int iWitchTray ,int iType)
{
    bMgzTrayCatcCynHomeNeedOff = true;
    AnsiString str;
    int &Task  =iMagazineUpDownTask;
    int iMotPos=0;

    bMgzTrayCatcCynHomeNeedOff=true;

    MOT[MCatchMgzTray].ScanMotorStatus();
//    MOT[MMagYTrayOut].ScanMotorStatus();
    //if(Sen[SnMagazineTrackDetect].IsOn()==true)
    if(bCheckMagazineSafeSensor())  //Sam 20221116 : Magazine TrayArm 自動補 Tray
    {
        MOT[MMagazine].PCIL132_StopMotor(); // Alarm : Track has tray can not up/down
        //ShowMyMessage("SnMagazineTrackDetect is on, Magazine can not move");
        return false;
    }

    #ifndef SOFT_SIMULTE
    if(Sen[SnMagazineSafeDoor].IsOff()==true || Sen[SnMagazineSafeDoor2].IsOff()==true || Sen[SnMagazineSafeDoor3].IsOff()==true)
    {
        MOT[MMagazine].PCIL132_StopMotor();    // Alarm : Magazine Safe Door is Open can not up/down
        return false;
    }

    // 2013.11.11 , Joye , AMD China ------------------------->>
    int iCatchPos=MOT[MCatchMgzTray].Motor->ReadPos();
    if(iCatchPos<0)
    {
        if(Cylinder[C_CatchMagazineTray].OffSensor()==false || Cylinder[C_CatchMagazineTray1].OffSensor()==false)
        {
            ShowMyMessage( "Magazine Catch Tray Cylinder OFF Error!!" );
            return 0;
        }
    }
    // 2013.11.11 , Joye , AMD China -------------------------<<
    #endif

    switch(Task)
    {
        case 1:
            if(iWitchTray<0 || iWitchTray>=MAX_MGZ_TRAY || iType==0)
            {
                // up to Top(Home)
                Task=1000;
                break;
            }
            else if(iType==3)
            {
                // Move to Standby
                Task=2000;
            }
            else
            {
                Task=100;
            }
            break;
        case 100:

            iMotPos=Prod.iMagazineTrayPos[iWitchTray];

            // 2012.11.13 , Joye , HT-7046 ------
            /*
            if(iType==1)        // Get
                iMagMin+=3;
            else if(iType==2)   // Recive
                iMagMin-=3;
            */
            if(iType==1)        // Get
                iMotPos+=fMagazine->iOutAutoOfs[iWitchTray];
            else if(iType==2)   // Recive
                iMotPos+=fMagazine->iInMagOfs[iWitchTray];
            // ----------------------------------

            if(MOT[MMagazine].MotorMove(iMotPos)==true)
            {
                if(Sen[SnMagazineTrackSelectDetect].IsOff()==true)
                {
                    ShowMyMessage( "Magazine Track Position Error!!" );
                    Task = 100;
                    break;
                }

                if(iType==2)
                {
                    if(iMagazineStatus==2)                                      //Ifor 20231130 add: 0: Normal 1:Full Tray 2:Manual put tray
                    {
                        #ifndef SOFT_SIMULTE
                        if(MOT[MMMagazineTary1+iWitchTray].fHasTray==true &&
                           Sen[SnMagazineDetect].IsOff()==true)                 //Ifor 20231201 add:手動退盤後檢查無Tray時清除資料
                        {
                            MOT[MMMagazineTary1+iWitchTray].ClearTray(__FUNC__);
                            str.sprintf("Magazine%d Clear Data", iWitchTray+1);
                            RecordProcess(str);
                        }
                        #else
                        if(MOT[MMMagazineTary1+iWitchTray].fHasTray==true)
                        {
                            MOT[MMMagazineTary1+iWitchTray].ClearTray(__FUNC__);
                            str.sprintf("Magazine%d Clear Data", iWitchTray+1);
                            RecordProcess(str);
                        }
                        #endif
                    }
                    else if(Sen[SnMagazineDetect].IsOn()==true)
                    {
                        // Alarm : Magazine has tray , please move tray.
                        Task=6000;                                              //Ifor 20231115 add:Magazine 回到等待位置再報警
                        break;
//                        str.sprintf("Magazine%d", iWitchTray+1);
//                        ShowErrorMessage("WAR2371", K_RETRY, MMagazine, 0, str);
//                        return false;
                    }
                }
                else
                {
                    // Alarm : Magazine no tray
                   // if( Sen[SnMagazineDetect].IsOff() == true )
                    if(LastSet.iRealDummy!=DUMMY && Sen[SnMagazineDetect].IsOff()==true)
                    {
                        str.sprintf("Magazine%d", iWitchTray+1);
                        ShowErrorMessage("WAR2372", K_RETRY, MMagazine, 0, str);
                        Task=110;
                        return false;
                    }
                }

                Task=1;
                return true;
            }
            break;
        case 110: //kevin 20140603  空tray 升到可 放入tray高度
            if(MOT[MMagazine].MotorMove(Prod.iMagazineTrayPos[14])==true)
            {
                  str.sprintf("Please put the tray into Magazine%d", iWitchTray+1);
                  ShowMyMessage(str);
                  Task=100;
            }
            break;
        case 1000:  // Magazine up to top
            if(MOT[MMagazine].MotorMove(Prod.iMagazineTrayPos[14])==true) // Home
            {
                Task=1;
                return true;
            }
            break;
        case 2000:  // Magazine Move to standby
            if(MOT[MMagazine].MotorMove(Prod.iMagazineTrayPos[14])==true)
            {
//                if(Sen[SnMagazineTrackSelectDetect].IsOff()==true)
//                {
//                    ShowMyMessage( "Magazine Track Position Error!!" );
//                    Task = 100;
//                    break;
//                }

                // Alarm : Magazine has tray , please move tray.
                if(Sen[SnMagazineDetect].IsOn()==true)
                {
                    Task=6000;
                    break;
//                    str.sprintf("Magazine%d", iWitchTray+1);
//                    ShowErrorMessage("WAR2371", K_RETRY, MMagazine, 0, str);
//                    return false;
                }

                Task=1;
                return true;
            }
            break;
        case 6000:  //Ifor 20231115 add:Magazine 回到等待位置再報警
            if(MOT[MMagazine].MotorMove(Prod.iMagazineTrayPos[14])==true)
            {
                str.sprintf("Magazine%d", iWitchTray+1);
                ShowErrorMessage("WAR2371", K_RETRY, MMagazine, 0, str);
                Task = 1;
                QueueTaskList[269].CheckTaskChange();                                       //Ifor 20240722 add:
                return false;
            }
            break;
    }

    return false;
}
//----------------------------------------------------------------------------
void InitialDoMagazineTrayFeedTask()
{
    iMagazineTrayFeedTask=1;
}
//----------------------------------------------------------------------------
TQPF_Timer htMagazineTrayFeed;
bool DoMagazineTrayFeed(bool bSelect)  //bSelect=false, 正常tray feed流程
{                                      //bSelect=true,  只退出手動指定的Magazine tray盤
    int &Task=iMagazineTrayFeedTask;
    int ret;//,iType=0;

    switch(Task)
    {
        case 1:
            if(MOT[MMAuto3].fHasTray==true)
            {
                // Tray Miss
                if(LastSet.iRealDummy!=DUMMY &&
                    Sen[SnAuto3TrayDetect].IsOff()==true)
                {
//                    ret = ShowMagazineError( MMAuto3 , TRAY_MISS , K_SKIP | K_RETRY );
                    ret=ShowErrorMessage("WAR1330", K_SKIP|K_RETRY, MMAuto3, false, "DoMagazineTrayFeed_1");
                    if(ret==K_SKIP)
                    {
                        MOT[MMAuto3].ClearTray(__FUNC__);
                        Cylinder[C_AutoEdgePush[iMagAtAuto]].Off();
                        iAuto3MagazineIndex=-1;                                 //Ifor 20240606 add:清除Tray Data時清除狀態
                    }

                    if(SoftStop)
                    {
                        Cylinder[C_AutoEdgePush[iMagAtAuto]].Off();
                    }
                }
                else
                {
                    // Tray Feed
                    InitialCatchTrayChangeTray();

                    Task=1000;
                }
            }
            else
            {
                if(Sen[SnAuto3TrayDetect].IsOn()==true && LastSet.iRealDummy!=DUMMY)
                {
                    // Alarm : Track has tray
                    ShowErrorMessage("MES1322", K_RETRY, MMAuto3, 0, "");
                    Cylinder[C_AutoEdgePush[iMagAtAuto]].Off();
                }
                else if(iTrayFeed== 1)
                {
                    Task=2000;
                }
            }
            break;
        case 1000:
            bChaneMagTrayflag=true;
            if(DoCatchTrayChangeTray(-1)==true)
            {
                bChaneMagTrayflag=false;
                Task=2000;
            }
            break;
        case 2000://kevin 20140604//magazine先降到 抓tray馬達可以進到 auto3高度
             if( MOT[MMagazine].MotorMove(Prod.iMagazineTrayPos[14])==true)
             {
                Task=2010;
                htMagazineTrayFeed.SetSecAndOn(10);
             }
             break;
         case 2010:
            if(Cylinder[C_CatchMagazineTray].Pop()==true && Cylinder[C_CatchMagazineTray1].OffSensor()==true)  //JerryYang 20221119 : fix
            {
                Task=2100;
            }
            else if(htMagazineTrayFeed.Off())
            {
                if(Cylinder[C_CatchMagazineTray].OffSensor()==false)
                {
                    ShowMyMessage("C_CatchMagazineTray pop but the off sesnsor is not on.");
                }

                if(Cylinder[C_CatchMagazineTray1].OffSensor()==false)
                {
                    ShowMyMessage("C_CatchMagazineTray pop but the off sesnsor is not on.");
                }
                htMagazineTrayFeed.SetSecAndOn(10);
            }
            break;
        case 2100:
            if(MOT[MCatchMgzTray].Motor->ReadPos()<=Prod.iCatchMazTray_Rear-1000)     // 2012.12.10 , Joye , AMD HT-7046M
            {
                if(Sen[SnMagazineTrackSelectDetect].IsOff()==true)              // 2012.11.29 , Joye , AMD HT-7046M
                {
                    ShowMyMessage( "Magazine Track Position Error!!" );
                    return false;
                }
            }

            #ifndef SOFT_SIMULTE
            if(Sen[SnAuto3EdgePush].IsOn() || Cylinder[C_Auto3EdgePush].Status)
            {
                ShowMyMessage( "Magazine C_Auto3EdgePush Off Error!!" );
                ShowMyMessage("Please set the C_Auto3EdgePush cylinder of the Magazine to the 'off' position first!!!!", "請先將 Magazine 的 C_Auto3EdgePush 氣缸設定為Off位置！!!", "Teach");
                QueueTaskList[270].CheckTaskChange();                       //Ifor 20240722 add:
                return false;
            }
            #endif
            if(MOT[MCatchMgzTray].MotorMove(Prod.iCatchMazTray_Rear)==true)     // 2012.11.15 , Joye , HT-7046
            {
                InitialMagazineUpDown();
                Task=2200;
            }
            break;
        case 2200:
            if(DoMagazineUpDown(-1, 0))
            {
                iWhichMag=-1;
                iAuto3MagazineIndex=-1;
                bChaneMagTrayflag=false;        //Ifor 20240308 add: Clean Out 後清除旗標
                ShowMyMessage("Magazine tray out, please take out all trays in magazine then press START");
                bHandlerPause=true;                                             //Ifor 20241227 add:避免Auto tray goes inside arrival time up error!
                InitialDoMagazineScanHasTray();
                Task=3000;
            }
            break;
        case 3000:
            if(DoMagazineScanHasTray()==true)                                   //確保TRAY盤已經手動取出
            {                                                                   // 2012.12.12 , Joye , AMD HT-7046M -----------------
                for(int i=0; i<MAX_MGZ_TRAY; i++)
                {
                    MOT[iMMgzTray[i]].ClearTray(__FUNC__);
                }
                return true;
            }
    }
    return false;
}
//----------------------------------------------------------------------------
void InitialDoMagazineScanHasTray()
{
    iDoMagazineScanHasTrayTask=1;
}
//----------------------------------------------------------------------------
bool DoMagazineScanHasTray()
{
    int &Task=iDoMagazineScanHasTrayTask;
    static int iMag=0;

    switch(Task)
    {
        case 1:
            InitialMagazineUpDown();
            iMag=0;
            Task=100;
            break;
        case 100:
            if(iMag<14)
            {
                if(DoMagazineUpDown(iMag ,2)==true)     //檢查Magazine中tray盤是否已全部拿出來
                {
                    Task=200;
                }
            }
            else
            {
                Task=300;
            }
            break;
        case 200:
            if(iMag<14)
            {
                iMag++;
                Task=100;
            }
            break;
        case 300:
            if(DoMagazineUpDown(0, 3))
            {
                bHandlerPause=true;                                             //Ifor 20241227 add:避免Auto tray goes inside arrival time up error!
                RecordProcess("Magazine tray scan has been completed.");
                return true;
            }
            break;
    }
    return false;
}
//----------------------------------------------------------------------------
bool DoBufferTraySkipProcess_9045()
{
    for(int k=0; k<MAX_ARM_Row; k++)
    {
        for(int l=0; l<MAX_ARM_Col; l++)
        {
            if(OutArmSuck.Suck[k][l].Error)
            {
                bOutArmSuckActive[k][l]=false;
                OutArmSuck.Suck[k][l].Error=false;                              //jou 2012-01-11 Tray end會造成Vaccum OFF error JAM0127
                MOT[MManualTray1+(iPickWhichBuff-3)].SetTraySingleData(iTrayCol[l], iOutArmYPosition+iLoadPitchStepY*k, NULL_IC);               //QQQ
            }
        }
    }
    return false;
}
//----------------------------------------------------------------------------
bool CheckBuffIsSameBin()
{
    int iBin=-1, iBinData=-1, iTemp=-1;

    if(fAGV->IsATK_AMR() &&
       LastSet.iUnloadFixTray==eAtkTfMoveFixIC)                                 //Steven 20260202 : for ATK AMR
    {
        return true;
    }

    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            if(OutArmSuck.Item[i][j]!=NULL_IC && OutArmSuck.Item[i][j]!=HAS_NULL_IC)
            {
                if(iBin==-1)
                {
                    iBin=OutArmSuck.iWhichAuto[i][j];
                }
                else
                {
                    if(iBin!=OutArmSuck.iWhichAuto[i][j])
                    {
                        return false;
                    }
                }

                if(iBinData==-1)
                {
                    iBinData=OutArmSuck.iBinData[i][j];
                }
                else
                {
                    if(iBinData!=OutArmSuck.iBinData[i][j])
                    {
                        return false;
                    }
                }

                if(iTemp==-1)
                {
                    iTemp=OutArmSuck.Item[i][j];
                }
                else
                {
                    if(iTemp!=OutArmSuck.Item[i][j])
                    {
                        return false;
                    }
                }
            }
        }
    }

    if(iPickWhichMag!=(Prod.iT6PosCate[iBinData]-ePosMag1))
    {
        return false;
    }

    return true;
}
//----------------------------------------------------------------------------
void InitialMagazineCheck()
{
    iMagazineInitCheckTask=1;
}
//----------------------------------------------------------------------------
bool DoMagazineInitCheck()
{
    int &Task=iMagazineInitCheckTask;
    static int iMag=-1;
    static bool iMagHasBin[MAX_MGZ_TRAY]={false, false, false, false, false, false, false, false, false, false, false, false, false, false};
    AnsiString str="";

    switch(Task)
    {
        case 1:
            InitialMagazineUpDown();                                            // 2012.11.29 , Joye , AMD HT-7046M
            iMag=0;

            for(int i=0; i<MAX_MGZ_TRAY; i++)
            {
                iMagHasBin[i]=false;
            }

            for(int i=0; i<MAX_MGZ_TRAY; i++)
            {
                for(int j=0; j<iTestBinCount; j++)
                {
                    int temp=Prod.iT6PosCate[j];
                    if(temp<=0)
                      continue;

                    if(i+iMagMin==temp-1)
                    {
                        //Auto1-1,2_Auto2-3,4_Auto3-5,6_Fix1-15,16_Fix2-5_Fix3-7,8,9,error.
                        iMagHasBin[i]=true;
                    }
                    else if(Prod.iIfErrorT6==i+iMagMin)
                    {
                        iMagHasBin[i]=true;
                    }
                }
            }
            Task=1500;
            break;

        case 1500:
            if(iMag<MAX_MGZ_TRAY)
            {
                if(TestIF_File.iMagTraySource==0)  //由tray arm入TRAY, Magazine不可有tray
                {
                    if(DoMagazineUpDown(iMag, 2)==true)   //移動Magazine檢查有設定BIN別的TRAY
                    {
                        if(LastSet.iRealDummy!=DUMMY)
                        {
                            if(Sen[SnMagazineDetect].IsOn()==true)   //Magazine has tray
                            {
                                str.sprintf("Magazine%d", iMag+1);
                                ShowErrorMessage("WAR2371", K_RETRY, MMagazine, 0, str);
                                return false;
                            }
                        }

                        if(Sen[SnAuto3TrayDetect].IsOn()==true)       //Auto 3不能有tray盤
                        {
                            // Alarm : Track has tray
                            ShowErrorMessage("MES1322", K_RETRY, MMAuto3, 0, "");
                            break;
                        }
                        iMag++;
                    }
                }
                else    //手動入TRAY
                {
                    if(iMagHasBin[iMag]==true)
                    {
                        if(DoMagazineUpDown(iMag, 1)==true)   //移動Magazine檢查有設定BIN別的TRAY
                        {
                            if(LastSet.iRealDummy!=DUMMY)
                            {
                                if(Sen[SnMagazineDetect].IsOff()==true)    //Magazine Tray Missing
                                {
                                    // Alarm : Tray Miss
                                    str.sprintf("Magazine%d", iMag+1);
                                    ShowErrorMessage("WAR2372", K_RETRY, MMagazine, 0, str);
                                    break;
                                }
                            }

                            if(Sen[SnAuto3TrayDetect].IsOn()==true)       //Auto 3不能有tray盤
                            {
                                // Alarm : Track has tray
                                ShowErrorMessage("MES1322", K_RETRY, MMAuto3, 0, "");
                                break;
                            }
                            iMag++;
                        }
                    }
                    else
                    {
                        iMag++;
                    }
                }
            }
            else
            {
                Task=2000;
            }
            break;
        case 2000:
            if(DoMagazineUpDown(0, 3))
            {
                bInitCheckMag=false;
                return true;
            }
            break;
    }
    return false;
}
//----------------------------------------------------------------------------
void __fastcall TfMagazine::spbSaveClick(TObject *Sender)
{
    AnsiString sPath=AuthPath+"config.ini";
    AnsiString szDir=GetRecipeFileName("HandlerCondition.Data");
    int temp=0;
    bool bFixHasBin=false, bCanUseBuffer=true;

    WriteIniData(szDir, "Configuration", "Tray Magazine Source",                 (rgMagTraySource->ItemIndex)?1:0);

    for(int j=0; j<iTestBinCount; j++)
    {
        temp=Prod.iT6PosCate[j];
        if((temp>=ePosFix1 && temp<=9) ||           //QQQ
           (Prod.iIfErrorT6>=iFixRight && Prod.iIfErrorT6<=iFixRightHalf))
        {
            bFixHasBin=true;
        }
    }

    if(rgMagFixType->ItemIndex==1)
    {
        if(MachineTypeChoice==Type_HT9046_LS && FIX3_INSTALL==1 && FIX3_FULL_PLACE==Fix3K_Uninstall)
        {
            if(LoadForm->YDivision<10 || LoadForm->XPitch>4000 || DeviceForm_File.XDimension>=40 || DeviceForm_File.YDimension>=40)
            {
                bCanUseBuffer=false;
            }

            if(bCanUseBuffer==false && rgMagFixType->ItemIndex==1)
            {
                ShowMyMessage("Device dimension is bigger than 40mm, not supported Magazine buffer tray.");
            }
            else if(bFixHasBin && rgMagFixType->ItemIndex==1)
            {
                ShowMyMessage("Fix tray has set bin, can not use to be Magazine buffer tray.");
            }
            else
            {
                WriteIniData(szDir, "Configuration", "Mag Fix tray Type",                    (rgMagFixType->ItemIndex)?1:0);
            }
        }
        else
        {
            ShowMyMessage("Machince type is not support Magazine buffer tray");
        }
    }
    else
    {
        WriteIniData(szDir, "Configuration", "Mag Fix tray Type",                    (rgMagFixType->ItemIndex)?1:0);
    }

    for(int i=0; i<16; i++)
    {
        WriteIniData(sPath, "Magazine Z Offset", "In Magazine Offset "+IntToStr(i), atoi(EditInMag[i]->Text.c_str()));
        WriteIniData(sPath, "Magazine Z Offset", "Out Auto3 Offset "  +IntToStr(i), atoi(EditOutAuto[i]->Text.c_str()));
    }
    WriteIniData(szDir, "Configuration", "Mag display order",   (rgMagDisplayOrder->ItemIndex)?1:0);    //Ifor 20240227 add:Magazine 顯示順序變更

    if(TestIF_File.iMagDisplayOrder != rgMagDisplayOrder->ItemIndex)
        fShowBinSelect->InitShowBinDigital();

    ReadFile();
    DoIniDataToForm();
    spbSave->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfMagazine::ReadFile()
{
    AnsiString sPath=AuthPath+"config.ini";
    AnsiString szDir=GetRecipeFileName("HandlerCondition.Data");

//    TestIF_File.iMagTraySource       =ReadIniData(szDir, "Configuration", "Tray Magazine Source",                 0);

    TestIF_File.iMagTraySource=0;   //JerryYang 20230505 : 手放tray放不好可能會magazine撞機, 寫死強制由tray arm補tray

    TestIF_File.iMagFixTrayType      =ReadIniData(szDir, "Configuration", "Mag Fix tray Type",                    0);

    if(CUSTOMER_CODE==CC_KYEC_LEE || IniConfig.bSPILFunction)  //Ifor 20240304 add:
    {
        TestIF_File.iMagDisplayOrder=1;
    }
    else
    {
        TestIF_File.iMagDisplayOrder=ReadIniData(szDir, "Configuration", "Mag display order",                    0);   //Ifor 20240227 add:Magazine 顯示順序變更
    }

    if(MachineTypeChoice==Type_HT9046_LS && FIX3_INSTALL==1 && FIX3_FULL_PLACE==Fix3K_Uninstall)
    {
        //只有HT9046LS可以支援fix當magazine buffer tray
    }
    else
    {
        TestIF_File.iMagFixTrayType=0;
    }

    if(LoadForm->YDivision<10 || LoadForm->XPitch>4000)
    {
        TestIF_File.iMagFixTrayType=0;
    }

    for(int i=0;i<16;i++)
    {
        iInMagOfs[i]    =     ReadIniData(sPath, "Magazine Z Offset", "In Magazine Offset "+IntToStr(i),-3);
        iOutAutoOfs[i]  =     ReadIniData(sPath, "Magazine Z Offset", "Out Auto3 Offset "  +IntToStr(i),3);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMagazine::DoIniDataToForm()
{
    rgMagTraySource->ItemIndex=TestIF_File.iMagTraySource;
    rgMagFixType->ItemIndex=TestIF_File.iMagFixTrayType;
    rgMagDisplayOrder->ItemIndex=TestIF_File.iMagDisplayOrder;

    for(int i=0;i<16;i++)
    {
        EditInMag[i]->Text  =iInMagOfs[i];
        EditOutAuto[i]->Text=iOutAutoOfs[i];
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMagazine::sbtExitClick(TObject *Sender)
{
    sbtExit->Down=false;
    Close();
    fShow=false;
}
//---------------------------------------------------------------------------
bool bCheckMagazineSafeSensor() //Sam 20221116 : Magazine TrayArm 自動補 Tray
{
    if(Sen[SnMagazineTrackDetect].Enable && Sen[SnMagazineTrackDetect].IsOn())
    {
         ShowMyMessage("SnMagazineTrackDetect is on, Magazine Z axis can not move!");
         return true;
    }

    if(Sen[SnMagazineTrackDetect2].Enable && Sen[SnMagazineTrackDetect2].IsOn())
    {
        ShowMyMessage("SnMagazineTrackDetect2 is on, Magazine Z axis can not move!");
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
void __fastcall TfMagazine::edInMagOfs01Click(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 100, -100); //kevin 20170922 (wei) 小鍵盤上下限
}
//---------------------------------------------------------------------------
void __fastcall TfMagazine::rgMagFixTypeClick(TObject *Sender)
{
    if(CanChangeData(false))    //JerryYang 20221221 : 新增保護, 機台內有IC不能更改Magazine功能
    {
        return;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMagazine::rgMagTraySourceClick(TObject *Sender)
{
    if(CanChangeData(false))    //JerryYang 20221221 : 新增保護, 機台內有IC不能更改Magazine功能
    {
        return;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMagazine::btnMagazineTrayOutClick(TObject *Sender)
{
    if(InArmSuck.HasIC()==false && OutArmSuck.HasIC()==false &&
       ShuttleHasIC()==false && IndexHasIC()==false)
    {
        btnMagazineTrayOut->Down=!btnMagazineTrayOut->Down;
        bDoSelectMagTrayOut=btnMagazineTrayOut->Down;
        InitialDoMagazineTrayFeedTask();
        for(int i=0; i<14; i++)
        {
            bSelectMagNeedOut[i]=chkMag[i]->Checked;
        }
    }
}
//---------------------------------------------------------------------------
//==> Eastsun 20260512 : Phase3-B F011 KYEC AMR 補 AMRUnloadBin() (a-side Magazine.cpp L4394)
AnsiString AMRUnloadBin(int iPos)                                       //搜尋unload是哪個Bin
{
    int iData;
    AnsiString SS = "";

    for(int i=0; i<iTestBinCount; i++)
    {
        iData=BinSelect[iTestRunMode].iCatDataT3Pos[i];

        if(iPos==iData &&
           TastCategory.iTotalCategory[i] > 0) //Eastsun 20260710 Merge 實際測出來的BIN才寫出
        {
            if(SS=="")
            {
                SS=AnsiString(i);
            }
            else
            {
                SS+=","+AnsiString(i);
            }
        }
    }

    if(SS=="") SS="0"; //Eastsun 20260710 Merge

    return SS;
}
//---------------------------------------------------------------------------
//<== Eastsun 20260512

//==> Eastsun 20260512 F012 整合 (AMR Magazine helpers (DoPickFromAuto/DoPlaceToAuto1/SearchNeedAMRUnload x4/BinTansferMagazine/InitialDoMagazineAMRTrayFeed/DoMagazineAMRTrayFeed))
//---------------------------------------------------------------------------
static int TrayFullPercent = 80;                              //a-side L37 Eastsun 20260107 : tray full percent
int iDoPickFromAutoTask=1;                                    //a-side L3888
int iDoPlaceToAuto1Task=1;                                    //a-side L3889
int iDoMagazineAMRTrayFeedTask=1;                             //a-side L3890
TQPF_Timer DoPlaceToAuto1_MagDelay;                           //a-side L4113
TQPF_Timer DoPlaceToAuto1_MagTrayCoverTimeOut;                //a-side L4114 Frank 20161212 (Jou) Cover Cylinder Time Out
//Note: bStart (a-side L4541) skipped: not used in DoMagazineAMRTrayFeed body, avoid global name collision
//Note: DoPickFromAuto_AutoRTDelay defined as TQPF_Timer[MAX_AUTO_TRAY] in AutoRetest.cpp; a-side body usages rewritten to [0]
//---------------------------------------------------------------------------
bool DoPickFromAuto(bool bReset)
{
    bool bResult=false;
    static bool bC_Auto_Fixer_Off=false;
    bool ret;
    int AlarmRet;
    int &Task=iDoPickFromAutoTask;

    if(bReset==true)
    {
        Task=1;
        return bResult;
    }

    switch(Task)
    {
        case 1:
            ret=MoveOutArmXY_ToFix_Tray_Full();
            if(ret)
            {
                DoPickFromAuto_AutoRTDelay[0].SetSecAndOn(ArmSpeed[TrayArm].dTAHeadDownIT);
                Cylinder[C_TrayCover].Off();  //ChungHung 20140625 先讓蓋子在上
                Task=200;
            }
            break;
        case 200:         //再確認汽缸在上
            ret=Cylinder[C_TrayX_UpDown].Pop();

            if(ret)
            {
                Task=250;
            }
            break;
        case 250:
            ret=Cylinder[C_CatchMagazineTray1].Pop();
            if(ret)
            {
                bC_Auto_Fixer_Off=false;
                Task=300;
            }
            break;
        case 300:     //移動到Auto 上面
            ret=TrayArmMotorMove(Prod.iXTrayAuto_ART[2]);  //kevin 20170831 (Steven) add ART-offset
            Cylinder[C_Auto3EdgePush].Off();
            Cylinder[C_Auto3UpPress].Off();    //JerryYang 20190423 新增unloader壓tray
            bC_Auto_Fixer_Off=Cylinder[C_Auto3Side_Fixer].Pop();

            if(ret && bC_Auto_Fixer_Off)
            {
                if(Cylinder[C_Auto3UpPress].Enable==true)  //JerryYang 20190423 新增unloader壓tray
                {
                    DoPickFromAuto_AutoRTDelay[0].SetSecAndOn(3);
                    Task=350;
                }
                else
                {
                    Task=400;
                }
            }
            break;
        case 350:
            if(Sen[SnAuto3UpPress].IsOff())  //JerryYang 20190423 新增unloader壓tray
            {
                Task=400;
            }

            if(DoPickFromAuto_AutoRTDelay[0].Off())
            {
                ShowMyMessage("Unloader Up press pop error","Unloader 上壓汽缸放開異常");
                Cylinder[C_Auto3UpPress].Off();    //JerryYang 20190423 新增unloader壓tray
                DoPickFromAuto_AutoRTDelay[0].SetSecAndOn(3);
                break;
            }
            break;
        case 400:        //汽缸向下
            if(CheckCylinderCanMove(5)==false)                       //Ifor 20240201 add Tray ARM 下降前判斷是否有異常
            {
                break;
            }
            ret=Cylinder[C_TrayX_UpDown].Push();
            if(ret)
            {
                Task=150;  //kevin 20150812
                if(USE_AUTO_RETEST==eartInstall) //ChungHung 20140625 add AutoRetest catch Tray use 2 Output
                    C_CatchTray_Fix_Puch(true);

                DoPickFromAuto_AutoRTDelay[0].SetSecAndOn(ArmSpeed[TrayArm].dTAHeadDownIT+1.5);
            }
            break;
        case 150: //kevin 20150812 上蓋下來
            if(DoPickFromAuto_AutoRTDelay[0].Off()==false)
                break;

            Task=600;  //ChungHung 20140626 先夾在蓋蓋子
            if(USE_AUTO_RETEST==eartInstall) //ChungHung 20140625 add AutoRetest catch Tray use 2 Output
                C_CatchTray_Fix_Puch(true);
            //ChungHung 20141014 add
            DoPickFromAuto_AutoRTDelay[0].SetSecAndOn(ArmSpeed[TrayArm].dTAHeadDownIT*2);
            break;
        case 600:    //夾起來
            //ChungHung 20141014 add
            if(DoPickFromAuto_AutoRTDelay[0].Off()==false)
                break;

            if(LastSet.iRealDummy==DUMMY)   //Steven 20100618
            {
                ret=true;
            }
            else
            {
                if(USE_AUTO_RETEST==eartInstall) //ChungHung 20140625 add AutoRetest catch Tray use 2 Output
                    ret=C_CatchTray_Fix_Puch();
                else
                    ret=Cylinder[C_CatchTray_Fix].Push();

                if(ret)
                {
                    if(LastSet.iRealDummy!=DUMMY && Sen[SnCatchTrayFix1On].IsOn()==true && Sen[SnCatchTrayFix2On].IsOn()==true)
                    {
                        Task=700;
                        break;
                    }
                }
            }

            if(ret)
            {
                Task=650;
            }
            break;
        case 650:       //Steven 20190314 : Fix ART catch auto tray hang up
            DoPickFromAuto_AutoRTDelay[0].SetSecAndOn(ArmSpeed[TrayArm].dVacuumTI);
            if(CosFunction.bThickTrayNoNeedCover && UserDefForm[0].ZDepth>1500)  //Steven 20200723 : 太厚的Tray不能蓋蓋子
            {
            }
            else
            {
                Cylinder[C_TrayCover].On();//kevin 20150812
            }
            Task=500;  //ChungHung 20140626 先夾在蓋蓋子
            break;
        case 500:    //蓋蓋子
            #ifdef SOFT_SIMULTE
            Task=900;
            break;
            #else
            if(DoPickFromAuto_AutoRTDelay[0].Off()==false)
                break; //kevin 20150812

            if(LastSet.iRealDummy==DUMMY)   //Steven 20100618
            {
                ret=true;
            }
            else
            {
                if(CosFunction.bThickTrayNoNeedCover && UserDefForm[0].ZDepth>1500)  //Steven 20200723 : 太厚的Tray不能蓋蓋子
                {
                }
                else
                {
                    Cylinder[C_TrayCover].On();//kevin 20150812
                }

                Task=900;  //kevin 20150812 //ChungHung 20140626 先夾在蓋蓋子
            }
            break;
            #endif
        case 900://上升
            if(DoPickFromAuto_AutoRTDelay[0].Off())
            {
                ret=Cylinder[C_TrayX_UpDown].Pop();
                if(ret)
                {
                    MOT[MTrayX].fHasTray=true;
//                    TestIF_File.iAMRDeviceCount+=MOT[MMAuto3].Tray.HowManyIC();
                    MOT[MMAuto3].ClearTray("DoPickFromAuto_F012");
                    Task=1000;
                }
            }
            break;
        case 1000:           //結束
            bResult=true;
            break;

//例外處理----------------------------------
        case 700:         //夾取錯誤先將氣缸升上來
            Cylinder[C_TrayX_UpDown].Off();
            DoPickFromAuto_AutoRTDelay[0].SetSecAndOn(ArmSpeed[TrayArm].dTAHeadDownIT*2);
            Task=750;
        case 750:   //Steven 20190314 : Fix ART catch auto tray hang up
            if(DoPickFromAuto_AutoRTDelay[0].Off())
            {
                if(LastSet.iRealDummy!=DUMMY && Sen[SnCatchTrayFix1On].IsOn()==true && Sen[SnCatchTrayFix2On].IsOn()==true)
                {
                    Task=800;
                }
                else
                {
                    Task=650;
                }
            }
            break;
        case 800:         //夾取錯誤Alarm
            AlarmRet=ShowErrorMessage("JAM0601", K_SKIP|K_RETRY, MTrayX, false, __FUNC__);

            if(AlarmRet==K_RETRY)
            {
                Task=400;
            }
            else if(AlarmRet==K_SKIP)
            {
                Task=900;
                MOT[MTrayX].fHasTray=false;
                MOT[MMAuto3].ClearTray("DoPickFromAuto_F012");
            }
            break;
    }

    return bResult;
}
//---------------------------------------------------------------------------
bool DoPlaceToAuto1(int iwhichAuto, bool bReset)
{
    bool bResult=false;
    bool ret;
    static bool bDuplicateErr=false;
    int AlarmRet;
    int &Task=iDoPlaceToAuto1Task;
    AnsiString Str;

    if(bReset==true)
    {
        bDuplicateErr=false;
        Task=1;
        return bResult;
    }

    switch(Task)
    {
        case 1:
            Task=1;
        case 100:   //先確認Iut Arm在安全點
//            ret=MoveInArm2XYToWait();
            ret=MoveOutArmToAutoSafe_9045();
            if(ret)
            {
                Task=200;
            }
            break;
        case 200:             //再確認汽缸在上
            ret=Cylinder[C_TrayX_UpDown].Pop();
            if(ret)
            {
                Task=300;
            }
            break;
        case 300:         //移動到Loader 上面
            ret=TrayArmMotorMove(Prod.iXTrayAuto_ART[0]);//kevin 20170831 (Steven) add ART
            if(ret)
            {
                //ChungHung 20140625 add
                Cylinder[C_Auto1Side_Fixer].Off();
                Cylinder[C_Auto1EdgePush].Off();
                Task=400;
            }
            break;
        case 400:             //汽缸向下
            if(MOT[MMAuto1].fHasTray==false && MOT[MMAuto1_Car].fHasTray==false) //Sam 20210621 : 退出來時還要一直轉，不然會回車導致翻盤
            {
                if(CheckCylinderCanMove(0)==false)                              //Ifor 20240201 add Tray ARM 下降前判斷是否有異常
                {
                    break;
                }
                ret=Cylinder[C_TrayX_UpDown].Push();
                if(ret)
                {
                    if(USE_AUTO_RETEST==eartInstall) //ChungHung 20140624 add AutoRetest catch Tray use 2 Output
                        C_CatchTray_Fix_Pop(true);

                    DoPlaceToAuto1_MagDelay.SetSecAndOn(ArmSpeed[TrayArm].dTAHeadDownIT+1.5);     //Steven 20110720 : 等待Hand Down Time
                    Task=500;
                }
            }
            break;
        case 500:          //放開夾子
            if(DoPlaceToAuto1_MagDelay.Off())
            {
                if(LastSet.iRealDummy==DUMMY)   //Steven 20120417 : Dummy不要夾起來
                {
                    ret=true;
                }
                else //if(IniConfig.bC03UseCatchTray)
                {
                    if(USE_AUTO_RETEST==eartInstall) //ChungHung 20140624 add AutoRetest catch Tray use 2 Output
                        ret=C_CatchTray_Fix_Pop();
                    else
                        ret=Cylinder[C_CatchTray_Fix].Pop();
                }

                if(ret)
                {
                    Task=600;
                    DoPlaceToAuto1_MagDelay.SetSecAndOn(ArmSpeed[TrayArm].dCTAirOn);  //Steven 20110720 : 等待Counter Air On Time
                    DoPlaceToAuto1_MagTrayCoverTimeOut.SetSecAndOn(100); //kevin 20170810 (Steven) 5->100                      //Frank 20161212 (Jou) Cover Cylinder Time Out
                }
            }
            break;
        case 600:          //開蓋子
            if(DoPlaceToAuto1_MagDelay.Off())
            {
                if(LastSet.iRealDummy==DUMMY)   //Steven 20100618
                {
                    ret=true;
                }
                else
                {
                    Cylinder[C_TrayCover].Off();
                    ret=true;   //JerryYang 20170907 (Steven) 這裡不需要判斷
                }

                if(ret)
                {
                    Task=700;
                    DoPlaceToAuto1_MagDelay.SetSecAndOn(ArmSpeed[TrayArm].dCTAirOn);  //Steven 20110720 : 等待Counter Air On Time
                }
            }
            break;
        case 700:     //上升
            if(DoPlaceToAuto1_MagDelay.Off())
            {
                MOT[MTrayX].fHasTray=false;     //Ifor 20230926 add:往上移避免上升過程中報警Tray Lost
                ret=Cylinder[C_TrayX_UpDown].Pop();
                if(ret)
                {
                    if(LastSet.iRealDummy!=DUMMY && Sen[SnAuto1TrayDetect].IsOff()) //沒有偵測到放tray
                    {
                        Task=800;
                    }
                    else
                    {
                        Task=900;
                    }
                }
            }
            break;
        case 900:                //結束
            if(CatchTraySuck.iWhichKit!=-1)
                Str.sprintf("DoPlaceToAuto1 - From %s", sTrayPosName[CatchTraySuck.iWhichKit]);
            else
                Str.sprintf("DoPlaceToAuto1 - From Unkonwn");
            NewRecordProcess("MES0673", "Tray Arm - place tray to Auto1", Str);    //Steven 20220331 : Tray Arm動作加上紀錄
            CatchTraySuck.iWhichKit=-1;  //Steven 20220331 : 紀錄Tray從哪來的
            CatchTraySuck.SetItemData(0, 0, NULL_IC);
            MOT[MMAuto1].SetTray(HAS_IC, "DoPlaceToAuto1_F012");
            Task=1000;
            break;
//例外處理-------------------------------------
        case 800:      //放到Loader上檢查Sensor錯誤
            AlarmRet=ShowErrorMessage("JAM1108", K_RETRY|K_SKIP, MMTrayY, bDuplicateErr, __FUNC__);

            if(AlarmRet==K_RETRY)
            {
                Task=700;
                bDuplicateErr=true;
            }
            else if(AlarmRet==K_SKIP)
            {
                bDuplicateErr=false;
                Task=900;
            }
            break;
        case 1000:
            if(MOT[MMAuto1].fHasTray==false && MOT[MMAuto1_Car].fHasTray==false)
            {
                bDuplicateErr=false;
                bResult=true;
            }
            else
            {
                DoAutoReceiveBinTray(0);
            }
            break;
    }

    return bResult;
}
//---------------------------------------------------------------------------
int SearchNeedAMRUnload(bool bTrayfeed)                                         //搜尋哪個Bin要被取出
{
    int FindMagTray=-1;
    for(int i=0; i<iTestBinCount; i++)
    {
        //iBinPos=BinSelect[iTestRunMode].iCatDataT3Pos[i];
//        if(iBinPos<0)
//            continue;
        //==>
//        iMagaTrayIndex = iBinPos-e3PosMag1;
//        if(iBinPos<e3PosMag1 || e3PosMag14<iBinPos)
//            continue;

//        if((TestIF_File.iAMRMagzineDeviceCount[iBinPos-e3PosMag1]>10 ||
//           (TestIF_File.iAMRMagzineDeviceCount[iBinPos-e3PosMag1]>0 && bTrayfeed==true)) &&
//           (iBinPos>=e3PosMag1 && e3PosMag14>=iBinPos))
        //==>
        int UnloadPos=SearchAMRUnloadPos(i);
        if(UnloadPos>=0)
        {
            bNeedAMRMagazineUnload=true;

            if(iTrayFeed!=1)
            {
                if(FindMagTray == -1)
                {
                    FindMagTray=i;
                    bool b1 = TestIF_File.iAMRMagzineDeviceCount[UnloadPos]>=
                               MOT[MMMagazineTary1+UnloadPos].Tray.XItem*MOT[MMMagazineTary1+UnloadPos].Tray.YItem*TrayFullPercent/100;
                    bool b2 = MOT[MMMagazineTary1+UnloadPos].Tray.FullIC();
                    if(MOT[MMAuto3].fHasTray==false &&                          //Eastsun 20260116 : Auto3沒有盤 就從maga第一個做
                       (b1||b2))
                    {
                        return FindMagTray;
                    }
                }
                else if((TestIF_File.iAMRMagzineDeviceCount[UnloadPos]>=         //Eastsun 20260116 : 搜尋有沒有Tray在auto3 要先抓
                        MOT[MMMagazineTary1+UnloadPos].Tray.XItem*MOT[MMMagazineTary1+UnloadPos].Tray.YItem*TrayFullPercent/100||
                        MOT[MMAuto3].Tray.FullIC()) &&
                        MOT[MMMagazineTary1+UnloadPos].fHasTray==false &&
                        MOT[MMAuto3].fHasTray==true)
                {
                    FindMagTray=i;
                }
                else
                {
                    int j = UnloadPos + 1;
                    for(; j<(14-UnloadPos); j++)                                //Eastsun 20260116 : 後面link也要搜尋
                    {
                        if(BinSelect[iTestRunMode].bMagazineLink[j])
                        {
                            if((TestIF_File.iAMRMagzineDeviceCount[j]>=
                               MOT[MMMagazineTary1+j].Tray.XItem*MOT[MMMagazineTary1+j].Tray.YItem*TrayFullPercent/100||
                               MOT[MMAuto3].Tray.FullIC()) &&
                               MOT[MMMagazineTary1+j].fHasTray==false &&
                               MOT[MMAuto3].fHasTray==true)
                            {
                                FindMagTray=i;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }
            else
            {
                if(FindMagTray == -1)
                {
                    FindMagTray=i;
                    if(MOT[MMAuto3].fHasTray==false &&                          //Eastsun 20260116 : Auto3沒有盤 就從maga第一個做
                       TestIF_File.iAMRMagzineDeviceCount[UnloadPos]>0)
                    {
                        return FindMagTray;
                    }
                }
                else if(TestIF_File.iAMRMagzineDeviceCount[UnloadPos]>0 &&      //Eastsun 20260116 : 搜尋有沒有Tray在auto3 要先抓
                        MOT[MMMagazineTary1+UnloadPos].fHasTray==false &&
                        MOT[MMAuto3].fHasTray==true)
                {
                    FindMagTray=i;
                }
                else
                {
                    int j = UnloadPos + 1;
                    for(; j<(14-UnloadPos); j++)                                //Eastsun 20260116 : 後面link也要搜尋
                    {
                        if(BinSelect[iTestRunMode].bMagazineLink[j])
                        {
                            if(TestIF_File.iAMRMagzineDeviceCount[j]>0 &&
                               MOT[MMMagazineTary1+j].fHasTray==false &&
                               MOT[MMAuto3].fHasTray==true)
                            {
                                FindMagTray=i;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }
        }
    }

    return FindMagTray;
}
//---------------------------------------------------------------------------
int SearchNeedAMRUnloadCount()                                                  //搜尋有幾個Bin要給AGV
{
    int iBinPos=0;
    int iMagaTrayIndex=0;
    int iCount=1;
//    if(iUnloaderTrayCountCal[0]>0 || MOT[MMAuto1].Tray.HasIC() || iTrayFeed==1) //Eastsun 20260106 : Auto1 滿盤模式 如果都沒Auto1Bin Maga直接出盤
//    {
//        iCount++;
//    }
    //if(iUnloaderTrayCountCal[1]>0 || MOT[MMAuto2].Tray.HasIC())                 //Eastsun 20260106 : Auto2上有盤或IC
    //{
    //    if(iTrayFeed==1)                                                        //Eastsun 20260123 : 只有TrayFeed模式下才需要算Auto2
    //    {
    //        iCount++;
    //    }
    //
    //}
    for(int i=0; i<iTestBinCount; i++)
    {
        iBinPos=BinSelect[iTestRunMode].iCatDataT3Pos[i];

        if(iBinPos<e3PosMag1 || e3PosMag14<iBinPos)                             //Eastsun 20260106 : 不是在彈夾內的盤 continue
            continue;

        iMagaTrayIndex = iBinPos-e3PosMag1;                                     //Eastsun 20260106 : 換算第幾個彈夾
        if(iTrayFeed!=1)
        {
            if(TestIF_File.iAMRMagzineDeviceCount[iMagaTrayIndex]>=
               MOT[MMMagazineTary1+iMagaTrayIndex].Tray.XItem*MOT[MMMagazineTary1+iMagaTrayIndex].Tray.YItem*TrayFullPercent/100 ||
               MOT[MMMagazineTary1+iMagaTrayIndex].Tray.FullIC()) //Eastsun 20260107 : 滿盤
            {
                iCount++;
            }
        }
        else if(TestIF_File.iAMRMagzineDeviceCount[iMagaTrayIndex]>0)
        {
            //bNeedAMRMagazineUnload=true;                                      //Eastsun 20260106 : 最後會變成true 導致第二次作動 hang up
            iCount++;
        }
    }

    return iCount;
}
//---------------------------------------------------------------------------
int SearchAMRUnloadPos(int iWhichAMRAuto)                                       //輸入Bin編號 輸出Maga第幾層
{
    int i, iBinPos;//, iWhichTray;
    iBinPos=BinSelect[iTestRunMode].iCatDataT3Pos[iWhichAMRAuto];
    if(!(iBinPos>=e3PosMag1 && e3PosMag14>=iBinPos))
        return -1;

    int iWhichTray=iBinPos-e3PosMag1;

    for(i=iWhichTray; i<(14-iWhichTray); i++)
    {
        if(!bSearchNeedAMRUnloadMode(iTrayFeed,i))                               //Eastsun 20260116 : 加入反向!
        {
            continue;
        }

        if(i==iWhichTray)
        {
            if(BinSelect[iTestRunMode].bMagazineLink[iWhichTray]==false)
            {
                return i;
            }
        }
        else if(BinSelect[iTestRunMode].bMagazineLink[i])
        {
            return i;
        }
        else
        {
            break;
        }
    }

    return -1;
}
//---------------------------------------------------------------------------
bool bSearchNeedAMRUnloadMode(bool b,int iMageIndex)
{
    if(MOT[MMMagazineTary1+iMageIndex].fHasTray==false &&                       //Eastsun 20260106 : 沒有盤回傳否
       (MOT[MMAuto3].fHasTray==false ||
       TestIF_File.iAMRMagzineDeviceCount[iMageIndex]<=0))                      //Eastsun 20260119 : Auto3有盤 maga都沒盤時 要給過 ,會這樣寫是剛好邏輯符合 沒有其他可用判斷式
    {
        return false;
    }

    if(!b)                                                                      //Eastsun 20260106 : 滿盤
    {
        if(TestIF_File.iAMRMagzineDeviceCount[iMageIndex]>=
           MOT[MMMagazineTary1+iMageIndex].Tray.XItem*MOT[MMMagazineTary1+iMageIndex].Tray.YItem*TrayFullPercent/100||
           MOT[MMMagazineTary1+iMageIndex].Tray.FullIC()) //Eastsun 20260107 : 滿盤
        {
            return true;
        }
    }
    else                                                                        //Eastsun 20260106 : 搜尋彈夾要被取出Bin編號
    {
        if(TestIF_File.iAMRMagzineDeviceCount[iMageIndex]>0)                    //Eastsun 20260106 : 盤上有IC
        {
            return true;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
int BinTansferMagazine(int iAuto)
{
    return iAuto;
}
//---------------------------------------------------------------------------
void InitialDoMagazineAMRTrayFeed()
{
    iDoMagazineAMRTrayFeedTask=1;
}
//---------------------------------------------------------------------------
bool DoMagazineAMRTrayFeed(int iWhichMRAuto)                                    //輸出maga的Tray 到Auto1
{
    int &Task=iDoMagazineAMRTrayFeedTask;
    static int iAMRAuto=0;

    switch(Task)
    {
        case 1:
            iAMRAuto=SearchAMRUnloadPos(iWhichMRAuto);

            if(MOT[MMAuto3].fHasTray==true)
            {
                DoPickFromAuto(true);
                Task=2000;
                bNeedAMRMagazineUnload=true;
            }
            else if(iAMRAuto!=-1)
            {
                Task=50;
                bNeedAMRMagazineUnload=true;
            }
            else
            {
                Task=5000;
            }
            break;
        case 50:
            if(iAMRAuto==iAuto3MagazineIndex &&
               MOT[MMAuto3].fHasTray==true)                                     //Eastsun 20260116 : 修正一直夾空盤過去
            {
                DoPickFromAuto(true);
                Task=2000;
            }
            else
            {
                Task=100;
            }
            break;
        case 100:
            if(iAMRAuto!=iAuto3MagazineIndex ||
               MOT[MMAuto3].fHasTray==false)                                    //Eastsun 20260116 : 修正一直夾空盤過去
            {
                // Change Tray
                iAuto3MagazineIndex=iAMRAuto;
                InitialCatchTrayChangeTray();
                Task=1000;
            }
            else
            {
                DoPickFromAuto(true);
                Task=2000;
            }
            break;
        case 1000:
            bChaneMagTrayflag=true;
            if(DoCatchTrayChangeTray(iAMRAuto)==true)
            {
                bChaneMagTrayflag=false;

                if(MOT[MMAuto3].fHasTray==true)    //確認有換盤成功
                    iAuto3MagazineIndex=iAMRAuto;
                else
                    iAuto3MagazineIndex=-1;
                Task=1;
            }
            break;
        case 2000:
            if(DoPickFromAuto(false))
            {
                DoPlaceToAuto1(iWhichMRAuto, true);
                Task=3000;
            }
            break;
        case 3000:
            if(DoPlaceToAuto1(iWhichMRAuto,false))
            {
                Task=1;
            }
            break;
        case 5000:
            if(TrayArmMotorMove(Prod.iXTrayEmpty))
            {
                bNeedAMRMagazineUnload=false;
                return true;
            }
            break;
    }
    return false;
}
//---------------------------------------------------------------------------

//<== Eastsun 20260512
