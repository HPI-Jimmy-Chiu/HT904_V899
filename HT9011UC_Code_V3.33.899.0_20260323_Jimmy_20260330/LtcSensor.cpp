#include "MachineDefine.h"
#pragma hdrstop
//------------------------------------------------------------------------------

#include "LtcSensor.h"

#include "MyMotor.h"
#include "cmydef.h"
#include "cprod.h"
#include "cinitial.h"
#include "main.h"
#include "common.h"
//------------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ALed"
#pragma resource "*.dfm"
TfLtcSensor *fLtcSensor;

//------------------------------------------------------------------------------
__fastcall TfLtcSensor::TfLtcSensor(TComponent* Owner)
    : TForm(Owner)
{
    TimerLtcSensor->Enabled=false;
    bShow=false;

    AxisNumShuttle1=0;
    AxisNumShuttle2=1;

    LatchInputNo0=0;
    LatchInputNo1=1;
    LatchInputNo2=2;
    LatchInputNo3=3;
    LatchInputNo4=4;
    LatchInputNo5=5;
    LatchInputNo6=6;
    LatchInputNo7=7;

    str = new TStringList;
}
//------------------------------------------------------------------------------
void __fastcall TfLtcSensor::FormDestroy(TObject *Sender)
{
    try
    {
        str->Clear();                                                           //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete str;                                                             //Steven 20160108 : release memory
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfLtcSensor::FormDestroy");
    }
    LogSoftwareOffTime("TfLtcSensor, FormDestroy");                             //Steven 20210526 : 紀錄軟體執行時間
}
//------------------------------------------------------------------------------
void __fastcall TfLtcSensor::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    TimerLtcSensor->Enabled=false;
    bShow=false;
}
//------------------------------------------------------------------------------
void __fastcall TfLtcSensor::FormShow(TObject *Sender)
{
    TimerLtcSensor->Enabled=true;
    bShow=true;

    gbOutSh1Y->Visible=(MOTION_CARD_TYPE==1);
    gbOutSh2Y->Visible=(MOTION_CARD_TYPE==1);
    gbInSh1Y ->Visible=(MOTION_CARD_TYPE==1);
    gbInSh2Y ->Visible=(MOTION_CARD_TYPE==1);

    gbOutSh3Z->Visible   =(USE_OUT_SORT_ARM!=eartUninstall);                    //Steven 20240822 : For HT-9046AU
    gbOutSh3Y->Visible   =(USE_OUT_SORT_ARM!=eartUninstall);
    btSh3Servo->Visible  =(USE_OUT_SORT_ARM!=eartUninstall);
    ledSht3Servo->Visible=(USE_OUT_SORT_ARM!=eartUninstall);
    lblSht3Servo->Visible=(USE_OUT_SORT_ARM!=eartUninstall);
}
//------------------------------------------------------------------------------
void __fastcall TfLtcSensor::btSh1ServoClick(TObject *Sender)
{
    MOT[MInShuttle1].ServoOnOff(!MOT[MInShuttle1].Led[iServoOn]);
    fAllMotorHome=false;
}
//------------------------------------------------------------------------------
void __fastcall TfLtcSensor::btSh2ServoClick(TObject *Sender)
{
    MOT[MInShuttle2].ServoOnOff(!MOT[MInShuttle2].Led[iServoOn]);
    fAllMotorHome=false;
}
//------------------------------------------------------------------------------
void __fastcall TfLtcSensor::btSh3ServoClick(TObject *Sender)
{
    MOT[MOutSortSht].ServoOnOff(!MOT[MOutSortSht].Led[iServoOn]);
    fAllMotorHome=false;
}
//------------------------------------------------------------------------------
void __fastcall TfLtcSensor::btSetLtcClick(TObject *Sender)
{
    SetLtcSensor(0);                                                            //JerryYang 20230223 : 清除latch函式拆成shuttle 1,2
    SetLtcSensor(1);
    SetLtcSensor(2);                                                            //Steven 20240822 : For HT-9046AU
}
//------------------------------------------------------------------------------
void __fastcall TfLtcSensor::TimerLtcSensorTimer(TObject *Sender)
{
    if(InitialOK==false)
        return;

    MOT[MInShuttle1].ScanMotorStatus();
    MOT[MInShuttle2].ScanMotorStatus();

    ledSht1Servo->Value=MOT[MInShuttle1].Led[iServoOn];
    ledSht2Servo->Value=MOT[MInShuttle2].Led[iServoOn];

    length = GetLatchTotalLength();
    lblLtcCount->Caption="Total Ltc Counter:"+AnsiString(length);

    ledOut1_1->Value=MOT[MInShuttle1].Motor->GetLatchIOStatus(LatchInputNo0);
    ledOut1_2->Value=MOT[MInShuttle1].Motor->GetLatchIOStatus(LatchInputNo1);
    ledOut2_1->Value=MOT[MInShuttle2].Motor->GetLatchIOStatus(LatchInputNo2);
    ledOut2_2->Value=MOT[MInShuttle2].Motor->GetLatchIOStatus(LatchInputNo3);
    if(MOTION_CARD_TYPE==1)                                                     //Steven 20140818 : 康泰克Out Sensor Y改用Latch
    {
        ledIn1_Y->Value =MOT[MInShuttle1].Motor->GetLatchIOStatus(LatchInputNo4);
        ledOut1_Y->Value=MOT[MInShuttle1].Motor->GetLatchIOStatus(LatchInputNo5);
        ledIn2_Y->Value =MOT[MInShuttle2].Motor->GetLatchIOStatus(LatchInputNo6);
        ledOut2_Y->Value=MOT[MInShuttle2].Motor->GetLatchIOStatus(LatchInputNo7);
    }

    if(USE_OUT_SORT_ARM!=eartUninstall)                                         //Steven 20240822 : For HT-9046AU
    {
        MOT[MOutSortSht].ScanMotorStatus();
        ledSht3Servo->Value=MOT[MOutSortSht].Led[iServoOn];
        ledOut3_1->Value=MOT[MOutSortSht].Motor->GetLatchIOStatus(LatchInputNo0);
        ledOut3_2->Value=MOT[MOutSortSht].Motor->GetLatchIOStatus(LatchInputNo1);
        ledOut3_Y->Value=MOT[MOutSortSht].Motor->GetLatchIOStatus(LatchInputNo2);
    }
}
//------------------------------------------------------------------------------
void TfLtcSensor::SetLtcSensor_SMC(int iShuttle)                                //JerryYang 20230223 : 清除latch函式拆成shuttle 1,2
{
    if(iShuttle==0)                                                             //shuttle 1
    {
        if(MOTION_CARD_TYPE==1)
        {
            AxisNumShuttle1=MOT[MInShuttle1].Motor->iPortID;                    //2013-01-15 Dell  contec 必需要指定
        }

        MOT[MInShuttle1].Motor->ResetLatch();                                   //重置Latch資料
        MOT[MInShuttle1].Motor->SetFIFOLatchSrc(AxisNumShuttle1, 0x33, 0);      //先Disable
        MOT[MInShuttle1].Motor->SetFIFOLatchSrc(AxisNumShuttle1, 0x33 ,0x33);   //Enable 設定哪個Sensor要去偵測哪個Encoder
    }
    else if(iShuttle==1)                                                        //shuttle 2
    {
        if(MOTION_CARD_TYPE==1)
        {
            AxisNumShuttle2=MOT[MInShuttle2].Motor->iPortID;                    //2013-01-15 Dell  contec 必需要指定
        }
        MOT[MInShuttle2].Motor->ResetLatch();
        MOT[MInShuttle2].Motor->SetFIFOLatchSrc(AxisNumShuttle2, 0xCC, 0);      
        MOT[MInShuttle2].Motor->SetFIFOLatchSrc(AxisNumShuttle2, 0xCC ,0xCC);   
    }
    else if(iShuttle==2)                                                        //Steven 20240822 : For HT-9046AU
    {
        if(USE_OUT_SORT_ARM!=eartUninstall)
        {
            AxisNumShuttle3=MOT[MOutSortSht].Motor->iPortID;                    //2013-01-15 Dell  contec 必需要指定
            MOT[MOutSortSht].Motor->ResetLatch();                                   
            MOT[MOutSortSht].Motor->SetFIFOLatchSrc(AxisNumShuttle3, 0xFF, 0);      
            MOT[MOutSortSht].Motor->SetFIFOLatchSrc(AxisNumShuttle3, 0xFF ,0xFF);
        }
    }
}
//------------------------------------------------------------------------------
void TfLtcSensor::SetLtcSensor_SYNTEK(int iShuttle)                             //JerryYang 20230223 : 清除latch函式拆成shuttle 1,2
{
    if(InitialOK==false)
        return;

    if(iShuttle==0)
    {
        MOT[MInShuttle1].Motor->ResetLatch();                                       //重置Latch資料
        MOT[MInShuttle1].Motor->SetFIFOLatchSrc(AxisNumShuttle1, LatchInputNo0, 0); //先Disable
        MOT[MInShuttle1].Motor->SetFIFOLatchSrc(AxisNumShuttle1, LatchInputNo1, 0);
        MOT[MInShuttle1].Motor->SetFIFOLatchSrc(AxisNumShuttle1, LatchInputNo0, 1); //Enable 設定哪個Sensor要去偵測哪個Encoder
        MOT[MInShuttle1].Motor->SetFIFOLatchSrc(AxisNumShuttle1, LatchInputNo1, 1);
    }
    else if(iShuttle==1)
    {
        MOT[MInShuttle2].Motor->ResetLatch();
        MOT[MInShuttle2].Motor->SetFIFOLatchSrc(AxisNumShuttle2, LatchInputNo2, 0);
        MOT[MInShuttle2].Motor->SetFIFOLatchSrc(AxisNumShuttle2, LatchInputNo3, 0);
        MOT[MInShuttle2].Motor->SetFIFOLatchSrc(AxisNumShuttle2, LatchInputNo2, 1);
        MOT[MInShuttle2].Motor->SetFIFOLatchSrc(AxisNumShuttle2, LatchInputNo3, 1);
    }
}
//------------------------------------------------------------------------------
void __fastcall TfLtcSensor::SetLtcSensor(int iSht)                             //JerryYang 20230223 : 清除latch函式拆成shuttle 1,2
{
    if(InitialOK==false)
        return;

    if(MOTION_CARD_TYPE==1)
    {
        SetLtcSensor_SMC(iSht);
    }
    else
    {
        SetLtcSensor_SYNTEK(iSht);
    }
}
//------------------------------------------------------------------------------
void TfLtcSensor::GetLtcSensor_SYNTEK(int iShuttle)
{
    if(iShuttle==0)
    {
        LatchDataCnt0 = MOT[MInShuttle1].Motor->GetLatchBuffer(LatchInputNo0, &uGetAxisCounterNo0, LatchDataTable0);
        LatchDataCnt1 = MOT[MInShuttle1].Motor->GetLatchBuffer(LatchInputNo1, &uGetAxisCounterNo1, LatchDataTable1);
    }
    else
    {
        LatchDataCnt2 = MOT[MInShuttle2].Motor->GetLatchBuffer(LatchInputNo2, &uGetAxisCounterNo0, LatchDataTable2);
        LatchDataCnt3 = MOT[MInShuttle2].Motor->GetLatchBuffer(LatchInputNo3, &uGetAxisCounterNo1, LatchDataTable3);
    }
}
//------------------------------------------------------------------------------
void TfLtcSensor::ABSLatchDataTable(U16 uLatchDataCnt,I32 *LatchDataTable0)     //Jimmychiu 20221208 abs shuttle1 LatchData
{
    I32 ltcTmp[1024];
    if(uLatchDataCnt<1024)
    {
        for(int i=0; i<uLatchDataCnt; i++)
        {
            ltcTmp[i]=abs(LatchDataTable0[i]);
            LatchDataTable0[i]=ltcTmp[i];
        }
    }
}
//------------------------------------------------------------------------------
void TfLtcSensor::GetLtcSensor_SMC(int iShuttle)
{
    I32 ltcTmp[1024];

    if(iShuttle==0)
    {
        LatchDataCnt0 = MOT[MInShuttle1].Motor->GetLatchBuffer(LatchInputNo0 +1, &iGetAxisCounterNo0, LatchDataTable0);
        LatchDataCnt1 = MOT[MInShuttle1].Motor->GetLatchBuffer(LatchInputNo1 +1, &iGetAxisCounterNo1, LatchDataTable1);
        LatchDataCnt4 = MOT[MInShuttle1].Motor->GetLatchBuffer(LatchInputNo4 +1, &iGetAxisCounterNo2, LatchDataTable4);
        LatchDataCnt5 = MOT[MInShuttle1].Motor->GetLatchBuffer(LatchInputNo5 +1, &iGetAxisCounterNo3, LatchDataTable5);

        ABSLatchDataTable(LatchDataCnt0, LatchDataTable0);                      //Jimmychiu 20221208 abs shuttle1 LatchData
        ABSLatchDataTable(LatchDataCnt1, LatchDataTable1);
        ABSLatchDataTable(LatchDataCnt4, LatchDataTable4);
        ABSLatchDataTable(LatchDataCnt5, LatchDataTable5);
    }
    else if(iShuttle==2)                                                        //Steven 20240822 : For HT-9046AU
    {
        LatchDataCntSort0 = MOT[MInShuttle1].Motor->GetLatchBuffer(LatchInputNo0 +1, &iGetSortCounter0, LatchSortDataTable0);
        LatchDataCntSort1 = MOT[MInShuttle1].Motor->GetLatchBuffer(LatchInputNo1 +1, &iGetSortCounter1, LatchSortDataTable1);
        LatchDataCntSort2 = MOT[MInShuttle1].Motor->GetLatchBuffer(LatchInputNo2 +1, &iGetSortCounter2, LatchSortDataTable2);
        LatchDataCntSort3 = MOT[MInShuttle1].Motor->GetLatchBuffer(LatchInputNo3 +1, &iGetSortCounter3, LatchSortDataTable3);
    }    
    else
    {
        LatchDataCnt2 = MOT[MInShuttle2].Motor->GetLatchBuffer(LatchInputNo2 +1, &iGetAxisCounterNo4, LatchDataTable2);
        LatchDataCnt3 = MOT[MInShuttle2].Motor->GetLatchBuffer(LatchInputNo3 +1, &iGetAxisCounterNo5, LatchDataTable3);
        LatchDataCnt6 = MOT[MInShuttle2].Motor->GetLatchBuffer(LatchInputNo6 +1, &iGetAxisCounterNo6, LatchDataTable6);
        LatchDataCnt7 = MOT[MInShuttle2].Motor->GetLatchBuffer(LatchInputNo7 +1, &iGetAxisCounterNo7, LatchDataTable7);

        if(LatchDataCnt2<1024)                                                  //JerryYang 20210310 : 修正長度超過1024會記憶體溢位
        {
            for(int i=0; i<LatchDataCnt2; i++)
            {
                ltcTmp[i] = abs(LatchDataTable2[i]);
                LatchDataTable2[i] = ltcTmp[i];
            }
        }
        if(LatchDataCnt3<1024)
        {
            for(int i=0; i<LatchDataCnt3; i++)
            {
                ltcTmp[i] = abs(LatchDataTable3[i]);
                LatchDataTable3[i] = ltcTmp[i];
            }
        }
        if(LatchDataCnt6<1024)
        {
            for(int i=0; i<LatchDataCnt6; i++)
            {
                ltcTmp[i] = abs(LatchDataTable6[i]);
                LatchDataTable6[i] = ltcTmp[i];
            }
        }
        if(LatchDataCnt7<1024)
        {
            for(int i=0; i<LatchDataCnt7; i++)
            {
                ltcTmp[i] = abs(LatchDataTable7[i]);
                LatchDataTable7[i] = ltcTmp[i];
            }
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfLtcSensor::GetLtcSensor(int iShuttle)
{
    #ifdef SOFT_SIMULTE
        return ;
    #else
        if(SThreadPara.bExeShuttleThread==false)
        {
            if(MOTION_CARD_TYPE==0)
            {
                GetLtcSensor_SYNTEK(iShuttle);
            }
            else
            {
                GetLtcSensor_SMC(iShuttle);
            }
        }
    #endif
}
//------------------------------------------------------------------------------
void __fastcall TfLtcSensor::ClearLtcSensor(int iShuttle)                       //Sam 20221101 : Latch 清除都要確認是否清清乾淨
{
    #ifdef SOFT_SIMULTE
        return ;
    #else
        if(SThreadPara.bExeShuttleThread==false)
        {
            if(MOTION_CARD_TYPE==0)
            {
                GetLtcSensor_SYNTEK(iShuttle);
            }
            else
            {
                GetLtcSensor_SMC(iShuttle);

                for(int c=0; c<5; c++)
                {
                    if(GetLtcDataCnt_SMC(iShuttle)>0)
                        GetLtcSensor_SMC(iShuttle);
                }

                if(GetLtcDataCnt_SMC(iShuttle)>0)
                     ShowMessage("SMC motion card Latch clear error !!! "+IntToStr(iShuttle));
            }
        }
    #endif
}
//------------------------------------------------------------------------------
int TfLtcSensor::GetLtcDataCnt_SMC(int iShuttle)                                //Sam 20211115 : Latch 清除增加檢查
{
    if(iShuttle==0)
        return (LatchDataCnt0+LatchDataCnt1+LatchDataCnt4+LatchDataCnt5);
    else if(iShuttle==2)                                                        //Steven 20240822 : For HT-9046AU
        return (LatchDataCnt0+LatchDataCnt1+LatchDataCnt2+LatchDataCnt3);
    else
        return (LatchDataCnt2+LatchDataCnt3+LatchDataCnt6+LatchDataCnt7);
}
//------------------------------------------------------------------------------
int TfLtcSensor::GetLatchTotalLength_SMC()
{
    int len =0;

    len =MOT[MInShuttle1].Motor->GetLatchTotalLen();
    len+=MOT[MInShuttle2].Motor->GetLatchTotalLen();
    if(USE_OUT_SORT_ARM!=eartUninstall)                                         //Steven 20240822 : For HT-9046AU
        len+=MOT[MOutSortSht].Motor->GetLatchTotalLen();

    return len;
}
//------------------------------------------------------------------------------
int TfLtcSensor::GetLatchTotalLength_SYNTEK()
{
    int len=MOT[MInShuttle1].Motor->GetLatchTotalLen();

    return len;
}
//------------------------------------------------------------------------------
int __fastcall TfLtcSensor::GetLatchTotalLength()
{
    int ilength;

    if(MOTION_CARD_TYPE==0)
    {
        ilength=GetLatchTotalLength_SYNTEK();
    }
    else
    {
        ilength=GetLatchTotalLength_SMC();
    }

    return ilength;
}
//------------------------------------------------------------------------------
void __fastcall TfLtcSensor::btGetSh1LtcClick(TObject *Sender)
{
    GetLtcSensor(0);

    if(MOTION_CARD_TYPE==0)
    {
        labOut1_Z1_Axis->Caption=uGetAxisCounterNo0;
        labOut1_Z2_Axis->Caption=uGetAxisCounterNo1;
    }
    else
    {
        labOut1_Z1_Axis->Caption=iGetAxisCounterNo0;
        labOut1_Z2_Axis->Caption=iGetAxisCounterNo1;
        labOut1_Y_Axis->Caption =iGetAxisCounterNo2;
        labIn1_Y_Axis->Caption  =iGetAxisCounterNo3;
    }

    labOut1_Z1_Count->Caption=LatchDataCnt0;
    labOut1_Z2_Count->Caption=LatchDataCnt1;
    labIn1_Y_Count->Caption  =LatchDataCnt4;
    labOut1_Y_Count->Caption =LatchDataCnt5;

    Memo_Out1Z1->Lines->Clear();
    if(LatchDataCnt0<1024)                                                      //JerryYang 20210310 : 修正長度超過1024會記憶體溢位
    {
        for(int i=0; i<LatchDataCnt0; i++)
        {
            Memo_Out1Z1->Lines->Add(AnsiString(abs(LatchDataTable0[i]*MOT[MInShuttle1].Motor->GearRatio)));
        }
    }
    Memo_Out1Z2->Lines->Clear();
    if(LatchDataCnt1<1024)
    {
        for(int i=0; i<LatchDataCnt1; i++)
        {
            Memo_Out1Z2->Lines->Add(AnsiString(abs(LatchDataTable1[i]*MOT[MInShuttle1].Motor->GearRatio)));
        }
    }
    Memo_In1Y->Lines->Clear();
    if(LatchDataCnt4<1024)
    {
        for(int i=0; i<LatchDataCnt4; i++)
        {
            Memo_In1Y->Lines->Add(AnsiString(abs(LatchDataTable4[i]*MOT[MInShuttle1].Motor->GearRatio)));
        }
    }
    Memo_Out1Y->Lines->Clear();
    if(LatchDataCnt5<1024)
    {
        for(int i=0; i<LatchDataCnt5; i++)
        {
            Memo_Out1Y->Lines->Add(AnsiString(abs(LatchDataTable5[i]*MOT[MInShuttle1].Motor->GearRatio)));
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfLtcSensor::btGetSh2LtcClick(TObject *Sender)
{
    GetLtcSensor(1);

    if(MOTION_CARD_TYPE==0)
    {
        labOut2_Z1_Axis->Caption=uGetAxisCounterNo0;
        labOut2_Z2_Axis->Caption=uGetAxisCounterNo1;
    }
    else
    {
        labOut2_Z1_Axis->Caption=iGetAxisCounterNo4;
        labOut2_Z2_Axis->Caption=iGetAxisCounterNo5;
        labOut2_Y_Axis->Caption =iGetAxisCounterNo6;
        labIn2_Y_Axis->Caption  =iGetAxisCounterNo7;
    }

    labOut2_Z1_Count->Caption=LatchDataCnt2;
    labOut2_Z2_Count->Caption=LatchDataCnt3;
    labIn2_Y_Count->Caption  =LatchDataCnt6;
    labOut2_Y_Count->Caption =LatchDataCnt7;

    Memo_Out2Z1->Lines->Clear();
    if(LatchDataCnt2<1024)                                                      //JerryYang 20210310 : 修正長度超過1024會記憶體溢位
    {
        for(int i=0; i<LatchDataCnt2; i++)
        {
            Memo_Out2Z1->Lines->Add(AnsiString(abs(LatchDataTable2[i]*MOT[MInShuttle2].Motor->GearRatio)));
        }
    }
    Memo_Out2Z2->Lines->Clear();
    if(LatchDataCnt3<1024)
    {
        for(int i=0; i<LatchDataCnt3; i++)
        {
            Memo_Out2Z2->Lines->Add(AnsiString(abs(LatchDataTable3[i]*MOT[MInShuttle2].Motor->GearRatio)));
        }
    }
    Memo_In2Y->Lines->Clear();
    if(LatchDataCnt6<1024)
    {
        for(int i=0; i<LatchDataCnt6; i++)
        {
            Memo_In2Y->Lines->Add(AnsiString(abs(LatchDataTable6[i]*MOT[MInShuttle2].Motor->GearRatio)));
        }
    }
    Memo_Out2Y->Lines->Clear();
    if(LatchDataCnt7<1024)
    {
        for(int i=0; i<LatchDataCnt7; i++)
        {
            Memo_Out2Y->Lines->Add(AnsiString(abs(LatchDataTable7[i]*MOT[MInShuttle2].Motor->GearRatio)));
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfLtcSensor::btGetSh3LtcClick(TObject *Sender)
{
    GetLtcSensor(2);                                                            //Steven 20240822 : For HT-9046AU

    labOut3_Z1_Axis->Caption=iGetSortCounter0;
    labOut3_Z2_Axis->Caption=iGetSortCounter1;
    labOut3_Y_Axis->Caption =iGetSortCounter2;

    labOut3_Z1_Count->Caption=LatchDataCntSort0;
    labOut3_Z2_Count->Caption=LatchDataCntSort1;
    labOut3_Y_Count->Caption =LatchDataCntSort2;

    Memo_Out3Z1->Lines->Clear();
    if(LatchDataCntSort0<1024)                                      
    {
        for(int i=0; i<LatchDataCntSort0; i++)
        {
            Memo_Out3Z1->Lines->Add(AnsiString(abs(LatchSortDataTable0[i]*MOT[MOutSortSht].Motor->GearRatio)));
        }
    }
    Memo_Out3Z2->Lines->Clear();
    if(LatchDataCntSort1<1024)
    {
        for(int i=0; i<LatchDataCntSort1; i++)
        {
            Memo_Out3Z2->Lines->Add(AnsiString(abs(LatchSortDataTable1[i]*MOT[MOutSortSht].Motor->GearRatio)));
        }
    }
    Memo_Out3Y->Lines->Clear();
    if(LatchDataCntSort2<1024)
    {
        for(int i=0; i<LatchDataCntSort2; i++)
        {
            Memo_Out3Y->Lines->Add(AnsiString(abs(LatchSortDataTable2[i]*MOT[MOutSortSht].Motor->GearRatio)));
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfLtcSensor::btnCloseClick(TObject *Sender)
{
    Close();
}
//------------------------------------------------------------------------------
void __fastcall TfLtcSensor::BtnDellTestClick(TObject *Sender)
{
    DellTestP(true);
    DellTest->Enabled=!DellTest->Enabled;
}
//------------------------------------------------------------------------------
bool bLoopFinish =true;
AnsiString sDir;
void SaveAsCSV(TMemo *Memo, int icount, int uCnt, int DCnt)
{
    AnsiString tmp, sss, ppp;

    if(bLoopFinish)
    {
        sDir=FormatDateTime("MM_dd_hh", Now());
        bLoopFinish =false;
    }

    tmp.sprintf("D:\\DellShuttleLog\\%s\\", sDir);
    if(DirectoryExists(tmp)==false)
    {
        MyForceDirectories(tmp);
    }
    tmp+=Memo->Name+".csv";

    fLtcSensor->str->Clear();
    if(FileExists(tmp))
        fLtcSensor->str->LoadFromFile(tmp);
    ppp.sprintf("Speed %d, Count %d, UpCnt %03d, DownCnt %03d", fLtcSensor->ScrollBar1->Position, icount, uCnt, DCnt);
    sss=TimeToStr(Time()) +"," +ppp +",";
    for(int i=0; i<Memo->Lines->Count; i++)
    {
        sss+=Memo->Lines->Strings[i]+",";
    }
    fLtcSensor->str->Add(sss);
    fLtcSensor->str->SaveToFile(tmp);
}
//------------------------------------------------------------------------------
TQPF_Timer WaitTmr;
const int LoopLimit = 20;
bool bDTError =false;
int iSpeedCount =0;
void TfLtcSensor::DellTestP(bool Reset =false)
{
    static int DTask=0;
    static int iCount=0;
    bool b1=false, b2=false;

    if(Reset)
    {
        DTask=0;
        iCount=0;
        iSpeedCount=1;
        bDTError=false;
        str->Clear();
        return;
    }

    if(fAllMotorHome==false)
    {
        ShowMessage("fAllMotorHome = false");
        bDTError=true;
        return;
    }

    switch(DTask)
    {
        case 0:
            MOT[MInShuttle1].SetSpeed(ScrollBar1->Position);
            MOT[MInShuttle2].SetSpeed(ScrollBar1->Position);
            DTask++;
            break;
        case 1:
            b1=MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iLeft);
            b2=MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iLeft);

            if(b1 && b2)
            {
                WaitTmr.SetMSAndOn(500);
                DTask++;
            }
            break;
        case 2:
            if(WaitTmr.Off())
            {
                DTask++;
            }
            break;
        case 3:
            btGetSh1LtcClick(Owner);
            btGetSh2LtcClick(Owner);
            SaveAsCSV(Memo_Out1Z1 , atoi(labOut1_Z1_Count->Caption.c_str()) ,MOT[MInShuttle1].Motor->PulseEdgeCnt[0][0] ,MOT[MInShuttle1].Motor->PulseEdgeCnt[0][1]);
            SaveAsCSV(Memo_Out1Z2 , atoi(labOut1_Z2_Count->Caption.c_str()) ,MOT[MInShuttle1].Motor->PulseEdgeCnt[1][0] ,MOT[MInShuttle1].Motor->PulseEdgeCnt[1][1]);
            SaveAsCSV(Memo_Out2Z1 , atoi(labOut2_Z1_Count->Caption.c_str()) ,MOT[MInShuttle2].Motor->PulseEdgeCnt[2][0] ,MOT[MInShuttle2].Motor->PulseEdgeCnt[2][1]);
            SaveAsCSV(Memo_Out2Z2 , atoi(labOut2_Z2_Count->Caption.c_str()) ,MOT[MInShuttle2].Motor->PulseEdgeCnt[3][0] ,MOT[MInShuttle2].Motor->PulseEdgeCnt[3][1]);
            SaveAsCSV(Memo_In1Y   , atoi(labIn1_Y_Count->Caption.c_str())   ,MOT[MInShuttle1].Motor->PulseEdgeCnt[4][0] ,MOT[MInShuttle1].Motor->PulseEdgeCnt[4][1]);
            SaveAsCSV(Memo_Out1Y  , atoi(labOut1_Y_Count->Caption.c_str())  ,MOT[MInShuttle1].Motor->PulseEdgeCnt[5][0] ,MOT[MInShuttle1].Motor->PulseEdgeCnt[5][1]);
            SaveAsCSV(Memo_In2Y   , atoi(labIn2_Y_Count->Caption.c_str())   ,MOT[MInShuttle2].Motor->PulseEdgeCnt[6][0] ,MOT[MInShuttle2].Motor->PulseEdgeCnt[6][1]);
            SaveAsCSV(Memo_Out2Y  , atoi(labOut2_Y_Count->Caption.c_str())  ,MOT[MInShuttle2].Motor->PulseEdgeCnt[7][0] ,MOT[MInShuttle2].Motor->PulseEdgeCnt[7][1]);
            DTask++;
            break;
        case 4:
            b1=MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iRight);
            b2=MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iRight);

            if(b1 && b2)
            {
                WaitTmr.SetMSAndOn(500);
                WaitTmr.On();
                DTask++;
            }
            break;
        case 5:
            if(WaitTmr.Off())
            {
                DTask++;
            }
            break;
        case 6:
            iCount++;
            if(iCount>=LoopLimit)
            {
                iSpeedCount++;
                int percent=iSpeedCount *10;

                ScrollBar1->Position=percent;
                iCount=0;

                if(iSpeedCount>=10)
                {
                    iSpeedCount=0;
                    bLoopFinish=true;
                }
            }
            Label23->Caption=iCount;
            DTask++;
            break;
        case 7:
            DTask=0;
            break;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfLtcSensor::DellTestTimer(TObject *Sender)
{
    if(InitialOK==false)
        return;

    DellTest->Enabled=false;
    DellTestP();
    if(bDTError==false)
        DellTest->Enabled=true;
}
//------------------------------------------------------------------------------
void __fastcall TfLtcSensor::ScrollBar1Change(TObject *Sender)
{
    Label22->Caption=ScrollBar1->Position;
}
//------------------------------------------------------------------------------
