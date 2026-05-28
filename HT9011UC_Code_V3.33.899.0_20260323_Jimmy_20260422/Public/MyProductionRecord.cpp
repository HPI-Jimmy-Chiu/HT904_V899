#include "MachineDefine.h"
#pragma hdrstop

#include "MyProductionRecord.h"

//---------------------------------------------------------------------------
#include "cprod.h"
#include "cmydef.h"
#include "common.h"
#include "aseTest.h"
#include "MachineType.h"
#include "cpublic.h"
#include "ProductionInfo.h"
#include "atester.h"
#include "MyKitSuck.h"
#include "VacuumUnit.h"                                                         //Sam 20230210 : 新增 VacuumUnit 通訊模組
#include "uLotInfo.h"
#include "FileInfo.h"
#include "BarCode.h"                                                            //RogerYang 20251210 : JCET 2D FT1白名單/FT2比對功能

#pragma package(smart_init)
//------------------------------------------------------------------------------
//Steven 20160113 : 記錄每一顆IC 狀態
//------------------------------------------------------------------------------
TMyProductionRecord::TMyProductionRecord()
{
    asBuffer = new TStringList();
    for(int i=eScheduleName; i<eDataTotal; i++)
    {
        //Ifor 20170209 (Steven) add 避免記憶體變大(多240Mb)
        //==>
        /*
        #ifdef ASE_KaohSiung
            asBuffer->Add("na");
        #else
            asBuffer->Add("");
        #endif
        */
        asBuffer->Add("");                                                      //kevin 20180129 add
        //<==
        //Ifor 20170209 (Steven) add 避免記憶體變大(多240Mb)
    }
    InitialRecord();

    asDataTitleGreatek=AnsiString("Schedule name, ")                 +   //eScheduleName         =0         //Sam 20200707 : ProductionRecord for Greatek
                       AnsiString("Start time, ")                    +   //eStartTime            =1
                       AnsiString("Input tray, ")                    +   //eLoadNum              =2
                       AnsiString("Tray Form, ")                     +   //eTrayForm             =41     //Sam 20210907 : Add record
                       AnsiString("In X, ")                          +   //eLoadX                =3
                       AnsiString("In Y, ")                          +   //eLoadY                =4
                       AnsiString("In Time, ")                       +   //eLoadTime             =5
                       AnsiString("In Arm Loader Pick, ")            +   //eInArmLoaderPick      =30
                       AnsiString("Hotplate No,")                    +   //eHotplateNo           =35     //Sam 20200716 : Add record
                       AnsiString("Hotplate Form, ")                 +   //eHotPlateForm         =42     //Sam 20210907 : Add record
                       AnsiString("Hot X, ")                         +   //eHotX                 =6
                       AnsiString("Hot Y, ")                         +   //eHotY                 =7
                       AnsiString("Hot Time, ")                      +   //eHotTime              =8
                       AnsiString("In Arm Hotplate Pick, ")          +   //eInArmHotplatePick    =36     //Sam 20200716 : Add record
                       AnsiString("2DCode, ")                        +   //e2DCode               =9
                       AnsiString("In Arm Place Shuttle Site No, ")  +   //eSiteNO               =10    //1-32 //Site No -> In Arm Place Shuttle Site No
                       AnsiString("Arm No, ")                        +   //eArmNO                =11    //0: arm1  1: arm2
                       AnsiString("Index Pick Shuttle Site No, ")    +   //eIndexPickSHTSiteNo   =31
                       AnsiString("Arm Time, ")                      +   //eArmTime              =12
                       AnsiString("Order of testing, ")              +   //eOrderTest            =13    //touch-down
                       AnsiString("SOT time stamp, ")                +   //eSOTTime              =14
                       AnsiString("Index cycle time, ")              +   //eIndexCycleTime       =15    //第一次測試 是 0 (1st device)
                       AnsiString("Test category, ")                 +   //eTestCate             =16    //測試 BIN別
                       AnsiString("Test Mode, ")                     +   //eTestMode             =37
                       AnsiString("Index Place Shuttle Site No,")    +   //eIndexPlaceSHTSiteNo  =32
                       AnsiString("iWhichAuto, ")                    +   //eWhichAuto            =17
                       AnsiString("Out Shuttle Detect Site No,")     +   //eOutSHTDetectSiteNo   =33
                       AnsiString("Out Arm Shuttle Pick, ")          +   //eOutArmShuttlePick    =34
                       AnsiString("Output tray, ")                   +   //eOutTrayNum           =18    //Auto1 :O1-1  Auto2 :2-1  Auto3 :3-1  Fix 1:4-1 Fix 2:5-1 Fix 3:6-1
                       AnsiString("Out X, ")                         +   //eUnloadX              =19
                       AnsiString("Out Y, ")                         +   //eUnloadY              =20
                       AnsiString("Out Time, ")                      +   //eUnloadTime           =21
                       AnsiString("Out Arm X Pos, ")                 +   //eUnloadXPos           =22
                       AnsiString("Out Arm Y Pos, ")                 +   //eUnloadYPos           =23
                       AnsiString("Out Arm Time, ")                  +   //eUnloadYPos           =24
                       AnsiString("Error log, ")                     +   //eErrorCode            =25
                       AnsiString("OCR Code , ")                     +   //eOCRCode              =26
                       AnsiString("Test Time, ")                     +   //eTestTime             =27
                       AnsiString("TSD Time,   ")                    +   //eTSDTime              =28
                       AnsiString("EOT time stamp,")                 +   //eEOTTime              =29
                       AnsiString("Out Arm X pitch Pos, ")           + //eUnloadXpPos            =38  //JerryYang 20210119 : add x,y pitch位置
                       AnsiString("Out Arm X pitch2 Pos, ")          + //eUnloadXp2Pos           =39
                       AnsiString("Out Arm Y pitch Pos, ")           ; //eUnloadYpPos            =40

    //Ifor 20180510 : add 時間顯示
    //==>
    asDataTitle=AnsiString("Schedule name, ")   +       //eScheduleName   =0
                AnsiString("Start time, ")      +       //eStartTime      =1
                AnsiString("Input tray, ")      +       //eLoadNum        =2
                AnsiString("In X, ")            +       //eLoadX          =3
                AnsiString("In Y, ")            +       //eLoadY          =4
                AnsiString("In Time, ")         +       //eLoadTime       =5
                AnsiString("Hot X, ")           +       //eHotX           =6
                AnsiString("Hot Y, ")           +       //eHotY           =7
                AnsiString("Hot Time, ")        +       //eHotTime        =8
                AnsiString("2DCode, ")          +       //e2DCode         =9
                AnsiString("Site No, ")         +       //eSiteNO         =10    //1-32
                AnsiString("Arm No, ")          +       //eArmNO          =11    //0: arm1  1: arm2
                AnsiString("Arm Time, ")        +       //eArmTime        =12
                AnsiString("Order of testing, ")+       //eOrderTest      =13    //touch-down
                AnsiString("SOT time stamp, ")  +       //eSOTTime        =14    //kevin 20140918  yyyymmdd_hhmmss
                AnsiString("Index cycle time, ")+       //eIndexCycleTime =15    //第一次測試 是 0 (1st device)
                AnsiString("Test category, ")   +       //eTestCate       =16    //測試 BIN別
                AnsiString("iWhichAuto, ")      +       //eWhichAuto      =17
                AnsiString("Output tray, ")     +       //eOutTrayNum     =18    //Auto1 :O1-1  Auto2 :2-1  Auto3 :3-1  Fix 1:4-1 Fix 2:5-1 Fix 3:6-1
                AnsiString("Out X, ")           +       //eUnloadX        =19
                AnsiString("Out Y, ")           +       //eUnloadY        =20
                AnsiString("Out Time, ")        +       //eUnloadTime     =21
                AnsiString("Out Arm X Pos, ")   +       //eUnloadXPos     =22
                AnsiString("Out Arm Y Pos, ")   +       //eUnloadYPos     =23
                AnsiString("Out Arm Time, ")    +       //eUnloadYPos     =24
                AnsiString("Error log, ")       +       //eErrorCode      =25
                AnsiString("OCR Code , ")       +       //eOCRCode        =26
                AnsiString("Test Time, ")       +       //eTestTime       =27
                AnsiString("TSD Time,   ")      +       //eTSDTime        =28
                AnsiString("EOT time stamp, ")  +       //eEOTTime        =29     //wei 20181211 更換位置SOT
                AnsiString("In Arm Loader Pick, ")+     //eInArmLoaderPick=30     //Sam 20200623 : Add Suck record
                AnsiString("Index Pick Shuttle Site No, ")+ //eIndexPickSHTSiteNo  =31     //Sam 20200623 : Add record
                AnsiString("Index Place Shuttle Site No,")+ //eIndexPlaceSHTSiteNo =32     //Sam 20200623 : Add record
                AnsiString("Out Shuttle Detect Site No,") + //eOutSHTDetectSiteNo  =33     //Sam 20200623 : Add record
                AnsiString("Out Arm Shuttle Pick, ")      + //eOutArmShuttlePick   =34     //Sam 20200623 : Add record
                AnsiString("Hotplate No,")                + //eHotplateNo          =35     //Sam 20200716 : Add record
                AnsiString("In Arm Hotplate Pick, ")      + //eInArmHotplatePick   =36     //Sam 20200716 : Add record
                AnsiString("Test Mode, ")                 + //eTestMode            =37     //Sam 20201216 : Add record
                AnsiString("Out Arm X pitch Pos, ")       + //eUnloadXpPos         =38  //JerryYang 20210119 : add x,y pitch位置
                AnsiString("Out Arm X pitch2 Pos, ")      + //eUnloadXp2Pos        =39
                AnsiString("Out Arm Y pitch Pos, ")       + //eUnloadYpPos         =40
                AnsiString("Tray Form, ")                 + //eTrayForm            =41     //Sam 20210907 : Add record
                AnsiString("Hotplate Form, ")             + //eHotPlateForm        =42     //Sam 20210907 : Add record
                AnsiString("Tray ID, ")                   + //eTrayID              =43     //Steven 20220120 : Amlogic需要收SBIN
                AnsiString("SBin, ")                      + //eSBin                =44     //Steven 20220120 : Amlogic需要收SBIN
                AnsiString("Ground, ")                    + //eGround              =45     //Sam 20211223 : 每顆 IC 測試完畢都要記錄當時的 Ground & ESD 數值。
                AnsiString("ESD, ")                       + //eESD                 =46     //Sam 20211223 : 每顆 IC 測試完畢都要記錄當時的 Ground & ESD 數值。
                AnsiString("In Rotation Angle, ")         + //eInRotationAngle     =47     //Sam 20221103 : 新增In旋轉角度
                AnsiString("Pick cleanpad data, ")              +       //ePickCleanPadData         =48 //Sam 20230616 : Add Auto Clean Record
                AnsiString("Pick cleanpad time, ")              +       //ePickCleanPadTime         =49
                AnsiString("Pick cleanpad suck, ")              +       //ePickCleanPadSuck         =50
                AnsiString("Pick cleanpad row, ")               +       //ePickCleanPadRow          =51
                AnsiString("Pick cleanpad col, ")               +       //ePickCleanPadCol          =52
                AnsiString("Place cleanpad to shuttle time, ")  +       //ePlaceCleanPadToShtTime   =53
                AnsiString("Place cleanpad to shuttle suck, ")  +       //ePlaceCleanPadToShtSuck   =54
                AnsiString("Place cleanpad to shuttle row, ")   +       //ePlaceCleanPadToShtRow    =55
                AnsiString("Place cleanpad to shuttle col, ")   +       //ePlaceCleanPadToShtCol    =56
                AnsiString("Pick cleanpad form shuttle time, ") +       //ePickCleanPadFormShtTime  =57
                AnsiString("Pick cleanpad form shuttle suck, ") +       //ePickCleanPadFormShtSuck  =58
                AnsiString("Pick cleanpad form shuttle row, ")  +       //ePickCleanPadFormShtRow   =59
                AnsiString("Pick cleanpad form shuttle col, ")  +       //ePickCleanPadFormShtCol   =60
                AnsiString("Place cleanpad time, ")             +       //ePlaceCleanPadTime        =61
                AnsiString("Place cleanpad suck, ")             +       //ePlaceCleanPadSuck        =62
                AnsiString("Place cleanpad row, ")              +       //ePlaceCleanPadRow         =63
                AnsiString("Place cleanpad col, ")              +       //ePlaceCleanPadCol         =64
                AnsiString("InArm Pick From Loader Vacuum, ")   +       //eLoaderPickVacuum         =65     //RogerYang 20251225 : 補上
                AnsiString("InArm Pick From Hotplate Vacuum, ") +       //eHPPickVacuum             =66     //RogerYang 20251225 : 補上
                AnsiString("OutArm Pick From Shuttle Vacuum, ") +       //eOSHTPickVacuum           =67     //RogerYang 20251225 : 補上
                AnsiString("Index Pick Vacuum, ")               +       //eIndexPickVacuum          =68     //RogerYang 20251225 : 補上
                AnsiString("In Arm X Pos, ")                    +       //eLoadXPos                 =69     //Sam 20250211 : Add Load Pos
                AnsiString("In Arm Y Pos, ")                    ;       //eLoadYPos                 =70
    //<==
    //Ifor 20180510 : add 時間顯示

    //Sam 20211223 : 每顆 IC 測試完畢都要記錄當時的 Ground & ESD 數值。
    //==>
    asDataTitle_SG=AnsiString("EOT time stamp, ")   +       //eEOTTime     =29
                   AnsiString("2DCode, ")           +       //e2DCode      =9
                   AnsiString("Test category, ")    +       //eTestCate    =16    //測試 BIN別
                   AnsiString("Test result, ")      +       //Pass=P , Fail=F
                   AnsiString("Ground, ")           +       //eGround      =45
                   AnsiString("ESD ")               ;       //eESD         =46
    //<==
    //Sam 20211223 : 每顆 IC 測試完畢都要記錄當時的 Ground & ESD 數值。

    //Sam 20230616 : Add Auto Clean Record
    //==>
    asDataTitleCleanPad=AnsiString("Pick clean pad data, ")              +       //ePickCleanPadData         =48
                        AnsiString("Pick clean pad time, ")              +       //ePickCleanPadTime         =49
                        AnsiString("Pick clean pad suck, ")              +       //ePickCleanPadSuck         =50
                        AnsiString("Pick clean pad row, ")               +       //ePickCleanPadRow          =51
                        AnsiString("Pick clean pad col, ")               +       //ePickCleanPadCol          =52
                        AnsiString("Place clean pad to shttle time, ")   +       //ePlaceCleanPadToShtTime   =53
                        AnsiString("Place clean pad to shttle suck, ")   +       //ePlaceCleanPadToShtSuck   =54
                        AnsiString("Place clean pad to shttle row, ")    +       //ePlaceCleanPadToShtRow    =55
                        AnsiString("Place clean pad to shttle col, ")    +       //ePlaceCleanPadToShtCol    =56
                        AnsiString("Pick clean pad form shttle time, ")  +       //ePickCleanPadFormShtTime  =57
                        AnsiString("Pick clean pad form shttle suck, ")  +       //ePickCleanPadFormShtSuck  =58
                        AnsiString("Pick clean pad form shttle row, ")   +       //ePickCleanPadFormShtRow   =59
                        AnsiString("Pick clean pad form shttle col, ")   +       //ePickCleanPadFormShtCol   =60
                        AnsiString("Place clean pad time, ")             +       //ePlaceCleanPadTime        =61
                        AnsiString("Place clean pad suck, ")             +       //ePlaceCleanPadSuck        =62
                        AnsiString("Place clean pad row, ")              +       //ePlaceCleanPadRow         =63
                        AnsiString("Place clean pad col, ")              +       //ePlaceCleanPadCol         =64
                        AnsiString("Error log ")                         ;       //eErrorCode            =25
    //<==
    //Sam 20230616 : Add Auto Clean Record

    //RogerYang 20251208 : JCET 2D FT1白名單/FT2比對功能
    asJCET2DIDList= AnsiString("2DID, ")                            +           //e2DCode           =9
                    AnsiString("SOFTBIN, ")                         +           //eSBin             =44
                    AnsiString("HARDBIN, ")                         +           //eTestCate         =16
                    AnsiString("SubLOTID, ")                        +
                    AnsiString("CustLOTID, ")                       +
                    AnsiString("Modecode, ")                        +
                    AnsiString("StartTime, ")                       +           //eSOTTime          =14
                    AnsiString("EndTime, ")                         +           //eEOTTime          =29
                    AnsiString("SITE, ")                            +           //eSiteNO           =10
                    AnsiString("In X, ")                            +           //eLoadX            =3
                    AnsiString("In Y, ")                            +           //eLoadY            =4
                    AnsiString("Out X, ")                           +           //eUnloadX          =19
                    AnsiString("Out Y, ")                           +           //eUnloadY          =20
                    AnsiString("iWhichAuto, ")                      +           //eWhichAuto        =17
                    AnsiString("PartID, ")                          ;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::DeleteProductionRecord()
{
    if(asBuffer!=0 && asBuffer!=NULL)
    {
        asBuffer->Clear();                                                      //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete asBuffer;
        asBuffer=NULL;                                                          //kevin 20170306
    }
}
//------------------------------------------------------------------------------
void TMyProductionRecord::InitialRecord()
{
    if(asBuffer->Count!=eDataTotal)
    {
        DeleteProductionRecord();
        TMyProductionRecord();
    }

    for(int i=eLoadNum; i<eDataTotal; i++)
    {
        //Ifor 20170209 (Steven) add 避免記憶體變大(多240Mb)
        //==>
//        #ifdef ASE_KaohSiung
//            asBuffer->Strings[i]="na";
//        #else
            asBuffer->Strings[i]="";
//        #endif
        //<==
        //Ifor 20170209 (Steven) add 避免記憶體變大(多240Mb)
    }
    bUse=false;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddLoadRecord(int iNum, int iR, int iC, int iXPos, int iYPos, AnsiString Name, AnsiString Time)
{
    AnsiString sTestMode="", asTrayForm="";

    if(CUSTOMER_CODE==CC_KYEC_LEE || CUSTOMER_CODE==CC_Greatek)                 //Sam 20200704 : Add Greatek
    {
        iC=iC+1;
        iR=iR+1;
    }
    AnsiString sTime=GetDateInfoByString("/")+" "+GetOnlyTimeInfoByString(":"); //Ifor 20180510 : add 時間顯示

    sTestMode.sprintf("%dx%d", TestSocket.iShtRow, TestSocket.iShtCol);
    asTrayForm.sprintf("%dx%d", LoadForm->XDivision, LoadForm->YDivision);      //Sam 20210907 : Add record

    asBuffer->Strings[eLoadNum      ]   ="I1-"+AnsiString(iNum);
    asBuffer->Strings[eLoadX        ]   =AnsiString(iC);
    asBuffer->Strings[eLoadY        ]   =AnsiString(iR);
    asBuffer->Strings[eLoadXPos     ]   =AnsiString(iXPos);                     //Sam 20250211 : Add Load Pos
    asBuffer->Strings[eLoadYPos     ]   =AnsiString(iYPos);
    asBuffer->Strings[eLoadTime     ]   =sTime;                                 //Ifor 20180510 : add 時間顯示
    asBuffer->Strings[eScheduleName ]   =Name;
    asBuffer->Strings[eStartTime    ]   =Time;
    asBuffer->Strings[eTestMode     ]   =sTestMode;                             //Sam 20201216 : Add record
    asBuffer->Strings[eTrayForm     ]   =asTrayForm;                            //Sam 20210907 : Add record
    bUse                                =true;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddHPRecord(int iPlate, int iR, int iC)
{
    AnsiString sHPNo="",asHotPlateForm="";

    if(iPlate==0)
        sHPNo="2";
    else if(iPlate==1)
        sHPNo="1";
    else
        sHPNo="na";

    asHotPlateForm.sprintf("%dx%d",HotPlateForm.XDivision,HotPlateForm.YDivision);  //Sam 20210907 : Add record

    if(CUSTOMER_CODE==CC_KYEC_LEE || CUSTOMER_CODE==CC_Greatek)                 //Sam 20200704 : Add Greatek
    {
        iC=iC+1;
        iR=iR+1;
    }

    AnsiString sTime=GetDateInfoByString("/")+" "+GetOnlyTimeInfoByString(":"); //Ifor 20180510 : add 時間顯示
    asBuffer->Strings[eHotX         ]   =AnsiString(iC);
    asBuffer->Strings[eHotY         ]   =AnsiString(iR);
    asBuffer->Strings[eHotTime      ]   =sTime;                                 //Ifor 20180510 : add 時間顯示
    asBuffer->Strings[eHotplateNo   ]   =sHPNo;                                 //Sam 20210907 : Add record
    asBuffer->Strings[eHotPlateForm ]   =asHotPlateForm;                        //Sam 20210907 : Add record
}
//------------------------------------------------------------------------------
//void TMyProductionRecord::AddTestRecord()
//{
//    AnsiString sTime=GetDateInfoByString("/")+" "+GetOnlyTimeInfoByString(":"); //Ifor 20180510 : add 時間顯示
//    asBuffer->Strings[eArmTime]         =sTime;                                 //Ifor 20180510 : add 時間顯示
//}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddArmSiteRecord(int iArm, int iSite)                 //Steven 20201204 : 修正Site No在非標準Pitch對應不上的問題
{
    asBuffer->Strings[eArmNO]           =AnsiString(iArm);
    asBuffer->Strings[eSiteNO]          =AnsiString(iSite);
    AnsiString sTime=GetDateInfoByString("/")+" "+GetOnlyTimeInfoByString(":"); //Ifor 20180510 : add 時間顯示
    asBuffer->Strings[eArmTime]         =sTime;                                 //Ifor 20180510 : add 時間顯示
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddTestResultRecord(int iBin, AnsiString SBin, AnsiString ErrorLog)
{
    if(TestIF_File.iGpibMode==1)                                                //256 bin    kevin 20141031
    {
        if(iBin>=255)                                                           //JerryYang 20230322 : 最大只到255BIN
            asBuffer->Strings[eTestCate]="E";
        else
            asBuffer->Strings[eTestCate]=AnsiString(iBin);
    }
    else
    {
        if(iBin>=iTestBinCount)                                                 //JerryYang 20200812 : error bin統一記錄"E"
        {
            asBuffer->Strings[eTestCate]="E";
        }
        else
        {
            asBuffer->Strings[eTestCate]=AnsiString(iBin);
        }
    }
    asBuffer->Strings[eSBin]            =SBin;                                  //Steven 20220120 : Amlogic需要收SBIN
    AddErrorLog(ErrorLog);
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddErrorLog(AnsiString ErrorLog)                      //Steven 20200518 : 把多個ErrorLog接起來
{
    if(ErrorLog!="")
    {
        if(asBuffer->Strings[eErrorCode]=="" || asBuffer->Strings[eErrorCode]=="na")
        {
            asBuffer->Strings[eErrorCode]=ErrorLog;
        }
        else
        {
            if(ErrorLog=="" || ErrorLog=="na")
            {

            }
            else if(asBuffer->Strings[eErrorCode].AnsiPos(ErrorLog)==0)         //不寫入重複的Error log
            {
                asBuffer->Strings[eErrorCode]=asBuffer->Strings[eErrorCode]+AnsiString("_")+ErrorLog;
            }
        }
    }
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddIndexCycleTimeRecord(double fIndexCycleTime)
{
    AnsiString IndexCycleTime;
    IndexCycleTime.sprintf("%6.3f", (double)fIndexCycleTime);
    asBuffer->Strings[eIndexCycleTime]=IndexCycleTime;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddOrderTestRecord(int iNum)
{
    asBuffer->Strings[eOrderTest]=AnsiString(iNum);
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddDataTimeRecord(AnsiString asStr)
{
    asBuffer->Strings[eSOTTime]=asStr;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddDataTimeEOTRecord(AnsiString asStr)                //wei 20181211 更換位置SOT
{
    asBuffer->Strings[eEOTTime]=asStr;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::Add2DIDRecord(AnsiString s2DID)
{
    asBuffer->Strings[e2DCode]=s2DID;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddUnloadRecord(int iWhichAuto, int iNum, int iTrayRow, int iTrayCol, int iXPos, int iYPos, AnsiString ascode, int iXPitch, int iXPitch2, int iYPitch, AnsiString TrayID)   //Frank 20160727 add iXpos iYpos
{
    AnsiString str;
    if(CUSTOMER_CODE==CC_KYEC_LEE ||
       CUSTOMER_CODE==CC_Greatek ||                                             //Sam 20200704 : Add Greatek
       CUSTOMER_CODE==CC_HANA_MICRON)
    {
        iTrayRow=iTrayRow+1;
        iTrayCol=iTrayCol+1;
    }
    AnsiString sTime=GetDateInfoByString("/")+" "+GetOnlyTimeInfoByString(":"); //Ifor 20180510 : add 時間顯示
    asBuffer->Strings[eWhichAuto]       =AnsiString(iWhichAuto+1);
    asBuffer->Strings[eOutTrayNum]      ="O"+AnsiString(iWhichAuto+1)+"-"+AnsiString(iNum);        //Auto1 :O1-1  Auto2 :2-1  Auto3 :3-1  Fix 1:4-1 Fix 2:5-1 Fix 3:6-1
    asBuffer->Strings[eUnloadX]         =AnsiString(iTrayCol);
    asBuffer->Strings[eUnloadY]         =AnsiString(iTrayRow);
    asBuffer->Strings[eUnloadTime]      =sTime;
    asBuffer->Strings[eUnloadXPos]      =AnsiString(iXPos);                     //Frank 20160727 add iXpos iYpos
    asBuffer->Strings[eUnloadYPos]      =AnsiString(iYPos);                     //Frank 20160727 add iXpos iYpos
    asBuffer->Strings[eUnloadPosTime]   =sTime;                                 //Ifor 20180510 : add 時間顯示
    asBuffer->Strings[eOCRCode]         =AnsiString(ascode);                    //wei 20161005
    asBuffer->Strings[eUnloadXpPos]     =AnsiString(iXPitch);                   //JerryYang 20210119 : add x,y pitch位置
    asBuffer->Strings[eUnloadXp2Pos]    =AnsiString(iXPitch2);
    asBuffer->Strings[eUnloadYpPos]     =AnsiString(iYPitch);
    asBuffer->Strings[eTrayID]          =AnsiString(TrayID);                    //Steven 20220120 : Amlogic需要收SBIN
    iOneTrayPickCount[1+iWhichAuto]++;                                          //kevin 20210623 Count 每盤數量
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddTestTime(AnsiString asStr)                         //Steven 20161219 : Add Test Time in Production log
{
    asBuffer->Strings[eTestTime]=asStr;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddTSDTime(AnsiString asStr)                          //Steven 20161219 (jou) : 修改TSD Log方式
{
    asBuffer->Strings[eTSDTime]=asStr;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddInArmLoaderPickRecord(int iSuckRow, int iSuckCol)  //Sam 20200623 : Add record
{
    asBuffer->Strings[eInArmLoaderPick]=InArmSuck.Suck[iSuckRow][iSuckCol].sName;
    AnsiString s="";                                                            //Sam 20230210 : 新增 VacuumUnit 通訊模組
    s.sprintf("%2.1f", InArmSuck.Suck[iSuckRow][iSuckCol].GetIOValue());
    if(s=="999.9")
        asBuffer->Strings[eLoaderPickVacuum]="NA";
    else
         asBuffer->Strings[eLoaderPickVacuum]=s;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddOutArmShuttlePickRecord(int iSuckRow, int iSuckCol)//Sam 20200623 : Add record
{
    asBuffer->Strings[eOutArmShuttlePick]=OutArmSuck.Suck[iSuckRow][iSuckCol].sName;
    AnsiString s="";                                                            //Sam 20230210 : 新增 VacuumUnit 通訊模組
    s.sprintf("%2.1f", OutArmSuck.Suck[iSuckRow][iSuckCol].GetIOValue());
    if(s=="999.9")
        asBuffer->Strings[eOSHTPickVacuum]="NA";
    else
        asBuffer->Strings[eOSHTPickVacuum]=s;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddIndexPickShuttleRecord(int iSite, int iSiteRow, int iSiteCol)    //Sam 20200623 : Add record
{
    AnsiString asSite="";
    asSite.sprintf("%s-%d", IndexSuckName[iSiteRow][iSiteCol], iSite);
    asBuffer->Strings[eIndexPickSHTSiteNo]=asSite;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddIndexPlaceShuttleRecord(int iSite, int iSiteRow, int iSiteCol)    //Sam 20200623 : Add record
{
    AnsiString asSite="";
    asSite.sprintf("%s-%d", IndexSuckName[iSiteRow][iSiteCol], iSite);
    asBuffer->Strings[eIndexPlaceSHTSiteNo]=asSite;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddOutShuttleDetectRecord(int iSite, int iSiteRow, int iSiteCol)    //Sam 20200623 : Add record
{
    AnsiString asSite="";
    asSite.sprintf("%s-%d", IndexSuckName[iSiteRow][iSiteCol], iSite);
    asBuffer->Strings[eOutSHTDetectSiteNo]=asSite;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddInArmHotplatePickRecord(int iSuckRow, int iSuckCol)//Sam 20200716 : Add record
{
    asBuffer->Strings[eInArmHotplatePick]=InArmSuck.Suck[iSuckRow][iSuckCol].sName;
    AnsiString s="";                                                            //Sam 20230210 : 新增 VacuumUnit 通訊模組
    s.sprintf("%2.1f", OutArmSuck.Suck[iSuckRow][iSuckCol].GetIOValue());
    if(s=="999.9")
        asBuffer->Strings[eHPPickVacuum]="NA";
    else
        asBuffer->Strings[eHPPickVacuum]=s;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddTestModeRecord(int iTestMode)                      //Sam 20201216 : Add record
{
    asBuffer->Strings[eTestMode]=iTestMode;
}
//------------------------------------------------------------------------------
int TMyProductionRecord::GetAutoX()
{
    return atoi(asBuffer->Strings[eUnloadX].c_str());
}
//------------------------------------------------------------------------------
int TMyProductionRecord::GetAutoY()
{
    return atoi(asBuffer->Strings[eUnloadY].c_str());
}
//------------------------------------------------------------------------------
int TMyProductionRecord::GetWhcihAuto()
{
    return atoi(asBuffer->Strings[eWhichAuto].c_str())-1;
}
//------------------------------------------------------------------------------
int TMyProductionRecord::GetLoaderX()
{
    return atoi(asBuffer->Strings[eLoadX].c_str());
}
//------------------------------------------------------------------------------
int TMyProductionRecord::GetLoaderY()
{
    return atoi(asBuffer->Strings[eLoadY].c_str());
}
//------------------------------------------------------------------------------
int TMyProductionRecord::GetLoaderNum()                                         //Frank 20181121 add iLoaderNum
{
    AnsiString Str;
    Str=asBuffer->Strings[eLoadNum].SubString(4, asBuffer->Strings[eLoadNum].Length());
    return atoi(Str.c_str());
}
//------------------------------------------------------------------------------
int TMyProductionRecord::GetSiteNo()
{
    return atoi(asBuffer->Strings[eSiteNO].c_str());
}
//------------------------------------------------------------------------------
int TMyProductionRecord::GetOrderOfContact()
{
    return atoi(asBuffer->Strings[eOrderTest].c_str());
}
//------------------------------------------------------------------------------
AnsiString TMyProductionRecord::GetSOTTime()
{
    if(asBuffer->Strings[eSOTTime]==NULL)
        return "";
    else
        return asBuffer->Strings[eSOTTime];
}
//------------------------------------------------------------------------------
AnsiString TMyProductionRecord::GetEOTTime()
{
    if(asBuffer->Strings[eEOTTime]==NULL)
        return "";
    else
        return asBuffer->Strings[eEOTTime];
}
//------------------------------------------------------------------------------
AnsiString TMyProductionRecord::GetTestTime()
{
    if(asBuffer->Strings[eTestTime]==NULL)
        return "";
    else
        return asBuffer->Strings[eTestTime];
}
//------------------------------------------------------------------------------
AnsiString TMyProductionRecord::GetErrorLog()
{
    if(asBuffer->Strings[eErrorCode]==NULL)
        return "";
    else
        return asBuffer->Strings[eErrorCode];
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddErrorRecord(AnsiString ErrorLog, bool bAddPosition, int iXPos, int iYPos, int iXPitch, int iXPitch2, int iYPitch)
{
    if(bAddPosition)                                                            //Steven 20190110 : Error Log Add Motor Position.
    {
        asBuffer->Strings[eUnloadXPos]  =AnsiString(iXPos);
        asBuffer->Strings[eUnloadYPos]  =AnsiString(iYPos);

        asBuffer->Strings[eUnloadXpPos]  =AnsiString(iXPitch);                  //JerryYang 20210119 : add x,y pitch位置
        asBuffer->Strings[eUnloadXp2Pos] =AnsiString(iXPitch2);
        asBuffer->Strings[eUnloadYpPos]  =AnsiString(iYPitch);
    }
    SaveRecord(ErrorLog);
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddErrorRecordNoSave(AnsiString ErrorLog, bool bAddPosition, int iXPos, int iYPos, int iXPitch, int iXPitch2, int iYPitch)
{
    if(bAddPosition)                                                            //Steven 20190110 : Error Log Add Motor Position.
    {
        asBuffer->Strings[eUnloadXPos]  =AnsiString(iXPos);
        asBuffer->Strings[eUnloadYPos]  =AnsiString(iYPos);

        asBuffer->Strings[eUnloadXpPos]  =AnsiString(iXPitch);                  //JerryYang 20210119 : add x,y pitch位置
        asBuffer->Strings[eUnloadXp2Pos]  =AnsiString(iXPitch2);
        asBuffer->Strings[eUnloadYpPos]  =AnsiString(iYPitch);
    }
    AddErrorLog(ErrorLog);
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddGroundRecord(AnsiString sGround)                   //Sam 20211223 : 每顆 IC 測試完畢都要記錄當時的 Ground & ESD 數值。
{
     asBuffer->Strings[eGround]=(CosFunction.bRecordGroundESDByTestIC==true)?sGround.c_str():"NA";
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddESDRecord(AnsiString sESD)                         //Sam 20211223 : 每顆 IC 測試完畢都要記錄當時的 Ground & ESD 數值。
{
    asBuffer->Strings[eESD]=(CosFunction.bRecordGroundESDByTestIC==true)?sESD.c_str():"NA";
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddInRotationAngRecord(int iAngle)                    //Sam 20221103 : 新增In旋轉角度
{
    asBuffer->Strings[eInRotationAngle]=AnsiString(iAngle);
}
//------------------------------------------------------------------------------
AnsiString TMyProductionRecord::GetInRotationAngRecord()                        //Sam 20221103 : 新增In旋轉角度
{
    return asBuffer->Strings[eInRotationAngle];
}
//------------------------------------------------------------------------------
//Sam 20230616 : Add Auto Clean Record
//==>
void TMyProductionRecord::AddPickCleanPad(int iSuckRow, int iSuckCol, int iKitRow, int iKitCol, int iData)
{
     AnsiString sTime="";
     GetTimeInfo();
     sTime.sprintf("%s %02d:%02d:%02d.%03d", Now().FormatString("yyyy/mm/dd"), SystemHour, SystemMin, SystemSec, SystemMSec);
     asBuffer->Strings[ePickCleanPadData]=iData;
     asBuffer->Strings[ePickCleanPadTime]=sTime;
     asBuffer->Strings[ePickCleanPadSuck]=InArmSuck.Suck[iSuckRow][iSuckCol].sName;
     asBuffer->Strings[ePickCleanPadRow]=AnsiString(iKitRow);
     asBuffer->Strings[ePickCleanPadCol]=AnsiString(iKitCol);
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddPlaceCleanPadToShuttle(int iSuckRow, int iSuckCol, int iShtRow, int iShtCol)
{
     AnsiString sTime="";
     GetTimeInfo();
     sTime.sprintf("%s %02d:%02d:%02d.%03d", Now().FormatString("yyyy/mm/dd"), SystemHour, SystemMin, SystemSec, SystemMSec);
     asBuffer->Strings[ePlaceCleanPadToShtTime]=sTime;
     asBuffer->Strings[ePlaceCleanPadToShtSuck]=InArmSuck.Suck[iSuckRow][iSuckCol].sName;
     asBuffer->Strings[ePlaceCleanPadToShtRow]=AnsiString(iShtRow);
     asBuffer->Strings[ePlaceCleanPadToShtCol]=AnsiString(iShtCol);
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddPickCleanPadFormShuttle(int iSuckRow, int iSuckCol, int iShtRow, int iShtCol)
{
     AnsiString sTime="";
     GetTimeInfo();
     sTime.sprintf("%s %02d:%02d:%02d.%03d", Now().FormatString("yyyy/mm/dd"), SystemHour, SystemMin, SystemSec, SystemMSec);
     asBuffer->Strings[ePickCleanPadFormShtTime]=sTime;
     asBuffer->Strings[ePickCleanPadFormShtSuck]=InArmSuck.Suck[iSuckRow][iSuckCol].sName;
     asBuffer->Strings[ePickCleanPadFormShtRow]=AnsiString(iShtRow);
     asBuffer->Strings[ePickCleanPadFormShtCol]=AnsiString(iShtCol);
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddPlaceCleanPad(int iSuckRow, int iSuckCol, int iKitRow, int iKitCol)
{
     AnsiString sTime="";
     GetTimeInfo();
     sTime.sprintf("%s %02d:%02d:%02d.%03d", Now().FormatString("yyyy/mm/dd"), SystemHour, SystemMin, SystemSec, SystemMSec);
     asBuffer->Strings[ePlaceCleanPadTime]=sTime;
     asBuffer->Strings[ePlaceCleanPadSuck]=InArmSuck.Suck[iSuckRow][iSuckCol].sName;
     asBuffer->Strings[ePlaceCleanPadRow]=AnsiString(iKitRow);
     asBuffer->Strings[ePlaceCleanPadCol]=AnsiString(iKitCol);
}
//<==
//Sam 20230616 : Add Auto Clean Record
//------------------------------------------------------------------------------
AnsiString oldStartTime="";
FILE *WriteProdLOG;
void TMyProductionRecord::SaveRecord(AnsiString ErrorLog)
{
    AnsiString asStr, asPath, asFileName="";
    TStringList *SList;
    bool bHasFile=false;

    AnsiString sFilePath="", sFileName="", sFullFileName="";

    if(CUSTOMER_CODE==CC_LEADYO &&
       IniConfig.bN33_1_NetChangeFileAndData)                                   //KenHsieh 20230727 : 更改工作檔與資料 By NetFile
    {
        SaveRecordForLeadYo();
    }
    else if(fBarCode->JCETUseMakeWhite2DIDList()==true)                         //RogerYang 20251210 : JCET 2D FT1白名單/FT2比對功能
    {
        Save2DIDForJCET(fLotInfo->edtSysLotID->Text, fLotInfo->edtCusLotID->Text, fLotInfo->cbRunMode->Text);
    }

    if(bUse==false)                                                             //一開始就沒吸取ic 就不紀錄
    {                                                                           //目前 tray資料被清為0
        return;
    }
    AddErrorLog(ErrorLog);
    if(IniConfig.bASE_Report)                                                   //Steven 20161227 (jou) : 修改成新的方式存ASELog
    {
        return;                                                                 //kevin 20210914 mark
    }

    asPath=GetProdLogFilePath();
    asFileName=GetProdLogFileName(false);
    MyForceDirectories(asPath, "TMyProductionRecord::SaveRecord_1");
    sFullFileName=FileInfo().PathCombin(asPath,asFileName);
    if(fProductionInfo->_bOEEStartLotSuccess &&
       IniConfig.bN14_3_OEEFTPUpload &&
       IniConfig.bN14_19_TrayMappingToFTP)
    {
        sFilePath="D:\\HT9045_Log\\ProductionInfo\\TrayMapping";
        sFileName="TrayMappingTemp.csv";
        MyForceDirectories(sFilePath, "TMyProductionRecord::SaveRecord_2");
        sFullFileName=FileInfo().PathCombin(sFilePath,sFileName);
        SaveDataForGreatek(sFullFileName);
    }

    if(CUSTOMER_CODE==CC_Greatek)                                               //Sam 20200707 : ProductionRecord for Greatek
    {
        SaveDataForGreatek(sFullFileName);
        InitialRecord();
        return;
    }

    bHasFile=FileExists(sFullFileName);
    if(bHasFile==false)
    {
        SList=new TStringList();                                                //Steven 20170518 (jou) : 換位置改善UPH
        SList->Clear();
        SList->Add(asDataTitle);                                                //Steven 20200416 : 整合Eventlog的title
        SList->SaveToFile(sFullFileName);
        SList->Clear();                                                         //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete SList;                                                           //Steven 20170518 (jou) : 換位置改善UPH
    }

    WriteProdLOG=fopen(sFullFileName.c_str(), "a+");
    if(WriteProdLOG!=NULL)
    {
        try
        {
            fprintf(WriteProdLOG, "%s \n", asBuffer->CommaText);                //kevin 20151001
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TMyProductionRecord::SaveRecord - 1", sFullFileName);
            asPath=GetProdLogFilePath();                                        //Steven 20200811 : 存檔失敗的時候,改個名字再存一次看看
            asFileName=GetProdLogFileName(true);
            sFullFileName=FileInfo().PathCombin(asPath,asFileName);
            fclose(WriteProdLOG);
            WriteProdLOG=fopen(sFullFileName.c_str(), "a+");
            if(WriteProdLOG!=NULL)
            {
                try
                {
                    fprintf(WriteProdLOG, "%s \n", asBuffer->CommaText); //kevin 20151001
                }
                catch(...)
                {
                    MyDBIProcess("Exception", "TMyProductionRecord::SaveRecord - 2", sFullFileName);
                }
            }
        }
    }
    else
    {
        MyDBIProcess("Exception", "TMyProductionRecord::SaveRecord - WriteProdLOG is NULL", sFullFileName);
        asPath=GetProdLogFilePath();                                            //Steven 20200811 : 存檔失敗的時候,改個名字再存一次看看
        asFileName=GetProdLogFileName(true);
        sFullFileName=FileInfo().PathCombin(asPath,asFileName);
        WriteProdLOG=NULL;
        WriteProdLOG=fopen(sFullFileName.c_str(), "a+");
        if(WriteProdLOG!=NULL)
        {
            try
            {
                fprintf(WriteProdLOG, "%s \n", asBuffer->CommaText);            //kevin 20151001
            }
            catch(...)
            {
                MyDBIProcess("Exception", "TMyProductionRecord::SaveRecord - 3", sFullFileName);
            }
        }
    }

    if(WriteProdLOG!=NULL)
        fclose(WriteProdLOG);
    InitialRecord();
}
//------------------------------------------------------------------------------
//kevin 20140918 儲存 每一顆IC 資料
//Steven 20161227 (jou) : 修改成新的方式存ASELog
//------------------------------------------------------------------------------
void TMyProductionRecord::SaveDataForASE(AnsiString sPcName)
{
    AnsiString asStr, asPatch, asFileName="";
    AnsiString asSendToASE1="@e02001";
    bool bHasFile=false;
    TStringList *SList;

    asPatch.sprintf("%s\\%04d%02d", asASELogPath, SystemYear, SystemMonth);
    MyForceDirectories(asPatch, "TMyProductionRecord::SaveDataForASE");
    asFileName.sprintf("%s_%s_%s.csv", ASET_ScheduleNAME, sPcName, ASET_StartTimeNAME);
    AseSaveFilePath.sprintf("%s\\%s", asPatch, asFileName);
    bHasFile=FileExists(AseSaveFilePath);

    if(bHasFile==false)
    {
        SList=new TStringList();                                                //Steven 20170519 (wei) : 換位置改善UPH
        //kevin 20170607 (wei) SList->LoadFromFile(AseSaveFilePath);
        asStr=AnsiString("Input tray, ")      +       //eLoadNum        =2
              AnsiString("X, ")               +       //eLoadX          =3
              AnsiString("Y, ")               +       //eLoadY          =4
              AnsiString("Output tray, ")     +       //eOutTrayNum     =15            //Auto1 :O1-1  Auto2 :2-1  Auto3 :3-1  Fix 1:4-1 Fix 2:5-1 Fix 3:6-1
              AnsiString("Out X, ")           +       //eUnloadX        =16
              AnsiString("Out Y, ")           +       //eUnloadY        =17
              AnsiString("Site No, ")         +       //eSiteNO         =8             //1-32
              AnsiString("Arm No, ")          +       //eArmNO          =9             //0: arm1  1: arm2
              AnsiString("Order of testing, ")+       //eOrderTest      =10            //touch-down
              AnsiString("Test category, ")   +       //eTestCate       =13            //測試 BIN別
              AnsiString("Index cycle time, ")+       //eIndexCycleTime =12            //第一次測試 是 0 (1st device)
              AnsiString("Time stamp, ")      +       //eSOTTime        =11            //kevin 20140918  yyyymmdd_hhmmss
              AnsiString("Error log")         ;       //eErrorCode      =20

        SList->Add(asStr);
        SList->SaveToFile(AseSaveFilePath);
        SList->Clear();                                                         //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete SList;                                                           //Steven 20170519 (wei) : 換位置改善UPH
    }

    asStr=asBuffer->Strings[eLoadNum        ]+","+
          asBuffer->Strings[eLoadX          ]+","+
          asBuffer->Strings[eLoadY          ]+","+
          asBuffer->Strings[eOutTrayNum     ]+","+
          asBuffer->Strings[eUnloadX        ]+","+
          asBuffer->Strings[eUnloadY        ]+","+
          asBuffer->Strings[eSiteNO         ]+","+
          asBuffer->Strings[eArmNO          ]+","+
          asBuffer->Strings[eOrderTest      ]+","+
          asBuffer->Strings[eTestCate       ]+","+
          asBuffer->Strings[eIndexCycleTime ].Trim()+","+
          asBuffer->Strings[eSOTTime        ].TrimLeft()+","+
          asBuffer->Strings[eErrorCode      ].TrimLeft();

    AseIcRecord+=asStr+":";
    if(IniConfig.bASE_Report && IniConfig.bG11ASEReport==false)       //kevin 20170306
    {
        return;
    }

    WriteProdLOG=fopen(AseSaveFilePath.c_str(), "a+");
    if(WriteProdLOG!=NULL)
    {
        try
        {
            fprintf(WriteProdLOG, "%s \n", asStr);                              //kevin 20151001
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TMyProductionRecord::SaveDataForASE");
        }
        fclose(WriteProdLOG);
    }
    WriteProdLOG=NULL;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::SaveDataForGreatek(AnsiString asPath)                 //Sam 20200707 : ProductionRecord for Greatek
{
    AnsiString asStr="";
    bool bHasFile=false;
    TStringList *SList;

    bHasFile=FileExists(asPath);
    int iSortData[eDataTotal]={0,1,2,41,3,4,5,30,35,42,6,7,8,36,9,10,11,31,12,13,14,15,16,37,32,17,33,34,18,19,20,21,22,23,24,25,26,27,28,29,38,39,40};
    if(bHasFile==false)
    {
        SList=new TStringList();
        SList->Clear();

        SList->Add(asDataTitleGreatek);
        SList->SaveToFile(asPath);
        SList->Clear();
        delete SList;
    }

    asStr="";
    for(int i=0; i<eDataTotal; i++)
    {
        asStr+=asBuffer->Strings[iSortData[i]]+",";
    }

    WriteProdLOG=fopen(asPath.c_str(), "a+");
    if(WriteProdLOG!=NULL)
    {
        try
        {
            fprintf(WriteProdLOG, "%s \n", asStr);                              //kevin 20151001
        }
        catch(...){}
        fclose(WriteProdLOG);
    }
    WriteProdLOG=NULL;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::SaveGroundESDData(AnsiString sLot)                    //Sam 20211223 : 每顆 IC 測試完畢都要記錄當時的 Ground & ESD 數值。
{
    if(CosFunction.bRecordGroundESDByTestIC==false)
        return;
    int iBin=0;
    AnsiString sPassFail="F";
    AnsiString aSaveFilePath="",asStr="";
    bool bHasFile=false;
    TStringList *SList;
    MyForceDirectories(sGroundESDLogPath, "TMyProductionRecord::SaveGroundESDData");
    aSaveFilePath.sprintf("%s\\%s.csv", sGroundESDLogPath, sLot);
    bHasFile=FileExists(aSaveFilePath);
    if(bHasFile==false)
    {
        SList=new TStringList();
        SList->Add(asDataTitle_SG);
        SList->SaveToFile(aSaveFilePath);
        SList->Clear();
        delete SList;
    }

    iBin=atoi(asBuffer->Strings[eTestCate].c_str());
    if(iBin>=0 && iBin<TEST_MAX_BIN)
        sPassFail=(Prod.bIsPassBin[iBin])?"P":"F";

    asStr=asBuffer->Strings[eEOTTime ]+","+
          asBuffer->Strings[e2DCode  ]+","+
          asBuffer->Strings[eTestCate]+","+
          sPassFail                   +","+                                     //Pass=0 Fail=1
          asBuffer->Strings[eGround  ]+","+
          asBuffer->Strings[eESD     ];

    WriteProdLOG=fopen(aSaveFilePath.c_str(), "a+");
    if(WriteProdLOG!=NULL)
    {
        try
        {
            fprintf(WriteProdLOG, "%s \n", asStr);                              //kevin 20151001
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TMyProductionRecord::SaveGroundESDData");
        }
        fclose(WriteProdLOG);
    }
    WriteProdLOG=NULL;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::SaveRecordCleanPad(AnsiString ErrorLog)               //Sam 20230616 : Add Auto Clean Record
{
    AnsiString asStr, asPath, asFileName="";
    TStringList *SList;
    bool bHasFile=false;

    AnsiString sFilePath="",sFileName="",sFullFileName="";

    asPath.sprintf("%s\\%04d%02d\\", asCleanPadLogPath, SystemYear, SystemMonth);
    AddErrorLog(ErrorLog);

    MyForceDirectories(asPath, "TMyProductionRecord::SaveRecordCleanPad");
    asFileName.sprintf("%s_%04d%02d%02d.csv", PC_NAME, SystemYear, SystemMonth, SystemDate);
    asPath+=asFileName;

    bHasFile=FileExists(asPath);
    if(bHasFile==false)
    {
        SList=new TStringList();
        SList->Clear();
        SList->Add(asDataTitleCleanPad);
        SList->SaveToFile(asPath);
        SList->Clear();
        delete SList;
    }
    asStr=asBuffer->Strings[ePickCleanPadData]          +","+
          asBuffer->Strings[ePickCleanPadTime]          +","+
          asBuffer->Strings[ePickCleanPadSuck]          +","+
          asBuffer->Strings[ePickCleanPadRow]           +","+
          asBuffer->Strings[ePickCleanPadCol]           +","+
          asBuffer->Strings[ePlaceCleanPadToShtTime]    +","+
          asBuffer->Strings[ePlaceCleanPadToShtSuck]    +","+
          asBuffer->Strings[ePlaceCleanPadToShtRow]     +","+
          asBuffer->Strings[ePlaceCleanPadToShtCol]     +","+
          asBuffer->Strings[ePickCleanPadFormShtTime]   +","+
          asBuffer->Strings[ePickCleanPadFormShtSuck]   +","+
          asBuffer->Strings[ePickCleanPadFormShtRow]    +","+
          asBuffer->Strings[ePickCleanPadFormShtCol]    +","+
          asBuffer->Strings[ePlaceCleanPadTime]         +","+
          asBuffer->Strings[ePlaceCleanPadSuck]         +","+
          asBuffer->Strings[ePlaceCleanPadRow]          +","+
          asBuffer->Strings[ePlaceCleanPadCol]          +","+
          asBuffer->Strings[eErrorCode]                   ;

    WriteProdLOG=fopen(asPath.c_str(), "a+");
    if(WriteProdLOG!=NULL)
    {
        try
        {
            fprintf(WriteProdLOG, "%s \n", asStr);
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TMyProductionRecord::SaveRecordCleanPad - 1", asPath);

            asPath.sprintf("%s\\%04d%02d\\", asCleanPadLogPath, SystemYear, SystemMonth);
            asFileName.sprintf("%s_%04d%02d%02d_1.csv", PC_NAME, SystemYear, SystemMonth, SystemDate);     //Steven 20170123 (Jou) : 修改檔案命名格式
            asPath+=asFileName;
            fclose(WriteProdLOG);
            WriteProdLOG=fopen(asPath.c_str(), "a+");
            if(WriteProdLOG!=NULL)
            {
                try
                {
                    fprintf(WriteProdLOG, "%s \n", asBuffer->CommaText); //kevin 20151001
                }
                catch(...)
                {
                    MyDBIProcess("Exception", "TMyProductionRecord::SaveRecordCleanPad - 2", asPath);
                }
            }
        }
    }
    else
    {
        MyDBIProcess("Exception", "TMyProductionRecord::SaveRecordCleanPad - WriteProdLOG is NULL", asPath);

        asPath.sprintf("%s\\%04d%02d\\", asCleanPadLogPath, SystemYear, SystemMonth);
        asFileName.sprintf("%s_%04d%02d%02d_1.csv", PC_NAME, SystemYear, SystemMonth, SystemDate);     //Steven 20170123 (Jou) : 修改檔案命名格式
        asPath+=asFileName;
//        fclose(WriteProdLOG);
        WriteProdLOG=NULL;
        WriteProdLOG=fopen(asPath.c_str(), "a+");
        if(WriteProdLOG!=NULL)
        {
            try
            {
                fprintf(WriteProdLOG, "%s \n", asBuffer->CommaText); //kevin 20151001
            }
            catch(...)
            {
                MyDBIProcess("Exception", "TMyProductionRecord::SaveRecordCleanPad - 3", asPath);
            }
        }
    }

    if(WriteProdLOG!=NULL)
        fclose(WriteProdLOG);
    InitialRecord();
}
//------------------------------------------------------------------------------
void TMyProductionRecord::SaveRecordForLeadYo()                                 //KenHsieh 20230727 : 更改工作檔與資料 By NetFile
{
    AnsiString asFileNameByFile="";
    bool bHasFile=false;
    TStringList *SList;

    bHasFile=FileExists(asProductionByFileNamePath);
    if(bHasFile==false)
    {
        SList=new TStringList();
        SList->Clear();
        SList->Add(asDataTitle);
        SList->SaveToFile(asProductionByFileNamePath);
        SList->Clear();
        delete SList;
    }

    WriteProdLOG=fopen(asProductionByFileNamePath.c_str(), "a+");
    if(WriteProdLOG!=NULL)
    {
        try
        {
            fprintf(WriteProdLOG, "%s \n", asBuffer->CommaText);                //kevin 20151001
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TMyProductionRecord::SaveRecordForLeadYo - 1", asProductionByFileNamePath);
            asProductionByFileNamePath.sprintf("%s\\%s\\%s\\", AnsiString(asProductionByFilePath), asStation_ByFile, asType_ByFile);         //Steven 20200811 : 存檔失敗的時候,改個名字再存一次看看
            asFileNameByFile.sprintf("%s_%s_%s_1.csv", asLotID_ByFile, asStation_ByFile, asState_ByFile);     //Steven 20170123 (Jou) : 修改檔案命名格式
            asProductionByFileNamePath+=asFileNameByFile;
            fclose(WriteProdLOG);
            WriteProdLOG=fopen(asProductionByFileNamePath.c_str(), "a+");
            if(WriteProdLOG!=NULL)
            {
                try
                {
                    fprintf(WriteProdLOG, "%s \n", asBuffer->CommaText);        //kevin 20151001
                }
                catch(...)
                {
                    MyDBIProcess("Exception", "TMyProductionRecord::SaveRecordForLeadYo - 2", asProductionByFileNamePath);
                }
            }
        }
    }
    else
    {
        MyDBIProcess("Exception", "TMyProductionRecord::SaveRecordForLeadYo - WriteProdLOG is NULL", asProductionByFileNamePath);
        asProductionByFileNamePath.sprintf("%s\\%s\\%s\\", AnsiString(asProductionByFilePath), asStation_ByFile, asType_ByFile);             //Steven 20200811 : 存檔失敗的時候,改個名字再存一次看看
        asFileNameByFile.sprintf("%s_%s_%s_1.csv", asLotID_ByFile, asStation_ByFile, asState_ByFile);     //Steven 20170123 (Jou) : 修改檔案命名格式
        asProductionByFileNamePath+=asFileNameByFile;
        fclose(WriteProdLOG);
        WriteProdLOG=fopen(asProductionByFileNamePath.c_str(), "a+");
        if(WriteProdLOG!=NULL)
        {
            try
            {
                fprintf(WriteProdLOG, "%s \n", asBuffer->CommaText);            //kevin 20151001
            }
            catch(...)
            {
                MyDBIProcess("Exception", "TMyProductionRecord::SaveRecordForLeadYo - 3", asProductionByFileNamePath);
            }
        }
    }

    fclose(WriteProdLOG);
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddIndexPickVacuum(double dKpa)                       //Sam 20230210 : 新增 VacuumUnit 通訊模組
{
    AnsiString s="";
    s.sprintf("%2.1f", dKpa);
    if(s=="999.0")
        asBuffer->Strings[eIndexPickVacuum]="NA";
    else
        asBuffer->Strings[eIndexPickVacuum]=s;
}
//------------------------------------------------------------------------------
AnsiString TMyProductionRecord::GetProdLogFileName(bool bTryCatch)
{
    AnsiString asFileName="";
    if(bTryCatch)
    {
        if(CUSTOMER_CODE==CC_SJ_Semiconductor)                                      //Steven 20250807 : HandlerID_InnerLotID_Step_Stage_TimeStamp
            asFileName.sprintf("%s_%s_%s_%s_%s_1.csv", IniConfig.SocketHandlerID, fLotInfo->edtSysLotID->Text, fLotInfo->edtStep->Text, fLotInfo->edtStage->Text, fLotInfo->lbledtStarTime->Text);
        else if(IniConfig.bO24_ProductionLogByLot)                                  //Steven 20250519 : Production Log By Lot
            asFileName.sprintf("%s_%s_%s_%s_1.csv", IniConfig.SocketHandlerID, fLotInfo->edtSysLotID->Text, fLotInfo->cbRunMode->Text, fLotInfo->lbledtStarTime->Text);     //Steven 20170123 (Jou) : 修改檔案命名格式
        else
            asFileName.sprintf("%s_%04d%02d%02d_1.csv", IniConfig.SocketHandlerID, SystemYear, SystemMonth, SystemDate);     //Steven 20170123 (Jou) : 修改檔案命名格式
    }
    else
    {
        if(CUSTOMER_CODE==CC_SJ_Semiconductor)                                      //Steven 20250807 : HandlerID_InnerLotID_Step_Stage_TimeStamp
            asFileName.sprintf("%s_%s_%s_%s_%s.csv", IniConfig.SocketHandlerID, fLotInfo->edtSysLotID->Text, fLotInfo->edtStep->Text, fLotInfo->edtStage->Text, fLotInfo->lbledtStarTime->Text);
        else if(IniConfig.bO24_ProductionLogByLot)                                  //Steven 20250519 : Production Log By Lot
            asFileName.sprintf("%s_%s_%s_%s.csv", IniConfig.SocketHandlerID, fLotInfo->edtSysLotID->Text, fLotInfo->cbRunMode->Text, fLotInfo->lbledtStarTime->Text);     //Steven 20170123 (Jou) : 修改檔案命名格式
        else
            asFileName.sprintf("%s_%04d%02d%02d.csv", IniConfig.SocketHandlerID, SystemYear, SystemMonth, SystemDate);     //Steven 20170123 (Jou) : 修改檔案命名格式
    }
    return asFileName;
}
//------------------------------------------------------------------------------
AnsiString TMyProductionRecord::GetProdLogFilePath()
{
    AnsiString asPath="";
    asPath.sprintf("%s\\%04d%02d\\", asTravelingLogPath, SystemYear, SystemMonth);
    return asPath;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::Save2DIDForJCET(AnsiString SubLOTID, AnsiString CustLOTID, AnsiString Modecode) //RogerYang 20251210 : JCET 2D FT1白名單/FT2比對功能
{

    AnsiString asJCET2DIDListPath, asStr;
    AnsiString asFileNameByFile="";
    bool bHasFile=false, bPass=false;
    TStringList *SList;
    int iBin;
    AnsiString sCust=SubLOTID.SubString(0,3);                                   //RogerYang 20260225 : 修改白名單路徑 \\客戶代碼(批次前3碼)\\批次_FTx_csv
    asJCET2DIDListPath.sprintf("%s%s\\%s\\%s_%s.csv", sWhite2DIDListLoc, sCust, SubLOTID, SubLOTID, Modecode);

    bHasFile=FileExists(asJCET2DIDListPath);

    if(bHasFile==false)
    {
        SList=new TStringList();
        SList->Clear();
        SList->Add(asJCET2DIDList);
        SList->SaveToFile(asJCET2DIDListPath);
        SList->Clear();
        delete SList;
        fBarCode->iJCETWhitelistSN=0; //序號從0開始
    }

    iBin=atoi(asBuffer->Strings[eTestCate].c_str());
    if(iBin>=0 && iBin<TEST_MAX_BIN &&
        asBuffer->Strings[eTestCate]!="E")                                      //RogerYang 20260302 : 避免Errbin被寫入
        bPass=Prod.bIsPassBin[iBin];
    else
        bPass=false;

    if(asBuffer->Strings[e2DCode].Pos("ERROR")>0)  //Error不紀錄
        return;


    if(bPass==false)  //不是pass bin不列入白名單
        return;

    fBarCode->iJCETWhitelistSN++;

    asStr=asBuffer->Strings[e2DCode]         +","+
      asBuffer->Strings[eSBin]               +","+
      asBuffer->Strings[eTestCate]           +","+
      SubLOTID                               +","+
      CustLOTID                              +","+
      Modecode                               +","+
      asBuffer->Strings[eSOTTime].TrimLeft() +","+
      asBuffer->Strings[eEOTTime].TrimLeft() +","+
      asBuffer->Strings[eSiteNO]             +","+
      asBuffer->Strings[eLoadX]              +","+
      asBuffer->Strings[eLoadY]              +","+
      asBuffer->Strings[eUnloadX]            +","+
      asBuffer->Strings[eUnloadY]            +","+
      asBuffer->Strings[eWhichAuto]          +","+
      IntToStr(fBarCode->iJCETWhitelistSN)   ;

    WriteProdLOG=fopen(asJCET2DIDListPath.c_str(), "a+");
    if(WriteProdLOG!=NULL)
    {
        try
        {
            fprintf(WriteProdLOG, "%s \n", asStr);
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TMyProductionRecord::Save2DIDForJCET - 1", asJCET2DIDListPath);

            //存檔失敗的時候,改個名字再存一次看看
            asJCET2DIDListPath.sprintf("%s%s\\%s\\%s_%s_Retry.csv", sWhite2DIDListLoc, sCust, SubLOTID, SubLOTID, Modecode);  //RogerYang 20260225 : 修改白名單路徑 \\客戶代碼(批次前3碼)\\批次_FTx_csv
            fclose(WriteProdLOG);
            WriteProdLOG=fopen(asJCET2DIDListPath.c_str(), "a+");
            if(WriteProdLOG!=NULL)
            {
                try
                {
                    fprintf(WriteProdLOG, "%s \n", asStr);
                }
                catch(...)
                {
                    MyDBIProcess("Exception", "TMyProductionRecord::Save2DIDForJCET - 2", asJCET2DIDListPath);
                }
            }
        }
    }
    else
    {
        MyDBIProcess("Exception", "TMyProductionRecord::Save2DIDForJCET - WriteProdLOG is NULL", asJCET2DIDListPath);

        //存檔失敗的時候,改個名字再存一次看看
        asJCET2DIDListPath.sprintf("%s%s\\%s\\%s_%s_Retry.csv", sWhite2DIDListLoc, sCust, SubLOTID, SubLOTID, Modecode);   //RogerYang 20260225 : 修改白名單路徑 \\客戶代碼(批次前3碼)\\批次_FTx_csv
        fclose(WriteProdLOG);
        WriteProdLOG=fopen(asJCET2DIDListPath.c_str(), "a+");
        if(WriteProdLOG!=NULL)
        {
            try
            {
                fprintf(WriteProdLOG, "%s \n", asBuffer->CommaText); //kevin 20151001
            }
            catch(...)
            {
                MyDBIProcess("Exception", "TMyProductionRecord::SaveRecordForLeadYo - 3", asProductionByFileNamePath);
            }
        }
    }

    fclose(WriteProdLOG);
}
//------------------------------------------------------------------------------
