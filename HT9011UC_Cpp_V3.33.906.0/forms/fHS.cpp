// =============================================================================
//  forms/fHS.cpp  --  ACTIVE bodies for forms/fHS.h's TFormHS facade.
//  See forms/fHS.h for the full safety posture / gate register / deviation
//  ledger.  This file defines ONLY the 13 methods marked ACTIVE there; every
//  GATED method declared in the header is deliberately left undefined here.
//
//  AI(W906-FW3-HS1) 20260827: new file, FW wave FW3-HS1.
// =============================================================================
#include "forms/fHS.h"

#include <cmath>              // std::fabs -- DEVIATION D-9, see forms/fHS.h
#include "MachineType.h"     // enum constants: CC_*/ATC_SYSTEM values/eTempControll/
                              // eHeaterType/eSocketTempControll/BAR_CODE_INSTALL values/
                              // Type_HT1032 -- header-only, no link
#include "cmydef.h"           // CUSTOMER_CODE/ATC_SYSTEM/USE_16_HEATER/iSocketBaseTempCount/
                              // BAR_CODE_INSTALL/iATC_Use_Heat_Count/asHandlerVersion/
                              // asTempCtrl/UN150Read/bUT150Install/iATCTempWaitTimer(_Start)/
                              // bCheckATCTemp/bEnable_KLT_Function/Tri_Temp_Machine/
                              // Tri_Temperature_Max|MinDegree/TempFuseLimitType/
                              // MachineTypeChoice/MMATC_* (cmydef.cpp, ht9045_globals)
#include "cprod.h"            // Temperature (cprod.cpp, ht9045_globals)
#include "Config.h"           // IniConfig (Config.h:1499 extern HT9045_CONFIG IniConfig;
                              // definition in cprod.cpp, ht9045_globals)
#include "CosFunction.h"      // CosFunction (CosFunction.h:488 extern
                              // HT9045_COUSTOMER_FUNCTION CosFunction;
                              // definition in CosFunction.cpp, ht9045_globals)
#include "LastSet.h"          // LastSet.iTemperature/Tempture_* (LastSet.cpp, ht9045_globals)
#include "cpublic.h"          // GetTimeInfo/GetOnlyTimeInfoByString/GetDateInfoByString
                              // (cpublic.cpp, ht9045_globals)
#include "common.h"           // MyTickCount (common.cpp, ht9045_core)
#include "canary_support.h"   // ShowMyMessage/ShowErrorMessage/RecordProcess declarations
                              // (definitions: canary_support.cpp, ht9045_sm -- established
                              // pre-existing edge, see forms/fHS.h BATCH CRITERION 4)
#include "myTimer.h"          // TQPF_Timer (myTimer.cpp, ht9045_globals)
#include "Public/HTKeyProShim.h"  // KeyPro_GetLevel (DEVIATION D-8)
#include "vclcompat/TStringList.h" // TStringList (tHiSiRecord)

// AI(W906-FW3-HS1) 20260827: MyDBIProcess's 2-arg overload has no shared
// header declaration anywhere in the tree (every consumer, including the
// already-merged forms/fLotInfo.cpp:79, redeclares it locally the same way).
// Definition: aHotPlateSubstrate.cpp, ht9045_sm -- same established
// pre-existing edge as the canary_support.h trio above.
extern void MyDBIProcess(AnsiString S1, AnsiString S2);

// ---------------------------------------------------------------------------
//  TFormHS ctor -- golden :52-60, PORT SPLIT (DEVIATION D-2): fields only.
//  golden's `TimerAutoBackup->Enabled=true;` / `TimerRTMMsg->Enabled=true;`
//  are NOT translated -- no TTimer member on this facade this wave.
// ---------------------------------------------------------------------------
TFormHS::TFormHS()
    : bFTPError(false), bNeedUpload_N10(false), bN10_3_1_Flag(false),
      bHaltHandler(false), tHiSiRecord(nullptr)
{
}

// ---------------------------------------------------------------------------
//  FormCreate -- golden :62-66
// ---------------------------------------------------------------------------
void TFormHS::FormCreate(TObject * /*Sender*/)
{
    tHiSiRecord = new TStringList();
    //    ClearLastFileLogName_HS(-1);                                          //Steven 20260504 : 換到別的地方
}

// ---------------------------------------------------------------------------
//  GetTempUseName -- golden :2171-2426.  Pure string builder over
//  config/enum state; NAME RESERVED by the gated-16 list
//  (csystem.cpp:29440,:29461) -- signature matches golden exactly.
// ---------------------------------------------------------------------------
AnsiString TFormHS::GetTempUseName(int source)                                  //Ifor 20160303 依據溫控器開啟狀況變更Log顯示名稱
{
    AnsiString NowTemperature="";
    NowTemperature ="Ver:"+asHandlerVersion+"\n";                               //kevin 20191212 add
    AnsiString sTempName[tcTotalCount]={"Plate1OFS",    "Plate2OFS",    "Shut1OFS",     "Shut2OFS",     "Head1OFS",     //溫控器要一起改
                                        "Head2OFS",     "Head3OFS",     "Head4OFS",     "DutOFS",       "ChamberOFS",
                                        "CCDOFS",       "Aa1OFS",       "Ab1OFS",       "Ac1OFS",       "Ad1OFS",
                                        "Ba1OFS",       "Bb1OFS",       "Bc1OFS",       "Bd1OFS",       "Aa2OFS",
                                        "Ab2OFS",       "Ac2OFS",       "Ad2OFS",       "Ba2OFS",       "Bb2OFS",
                                        "Bc2OFS",       "Bd2OFS",       "HeatGun1OFS",  "HeatGun2OFS",  "DUT1OFS",
                                        "DUT2OFS",      "DUT3OFS",      "DUT4OFS",      "Ae1OFS",       "Af1OFS",
                                        "Ag1OFS",       "Ah1OFS",       "Be1OFS",       "Bf1OFS",       "Bg1OFS",
                                        "Bh1OFS",       "Ae2OFS",       "Af2OFS",       "Ag2OFS",       "Ah2OFS",
                                        "Be2OFS",       "Bf2OFS",       "Bg2OFS",       "Bh2OFS",       "2DOFS",
                                        "LBOFS",        "IndexESDOFS",  "CCD_2OFS",     "ATCAir1OFS",   "ATCAir2OFS",
                                        "OutSht1OFS",   "OutSht2OFS",   "Base1OFS",     "Base2OFS",     "Base3OFS",
                                        "Base4OFS",     "Base5OFS",     "Base6OFS",     "HotPlate3OFS", "HotPlate4OFS",
                                        "Shuttle3OFS",  "Shuttle4OFS",  "Door1OFS",     "Door2OFS",     "LBUpOFS",
                                        "LBDownOFS"
                                       };

    if(source==1)
    {
        NowTemperature=NowTemperature+
        "Time"                                  +","+
        "Plate1"                                +","+
        "Plate2"                                +","+
        "Shut1"                                 +","+
        "Shut2"                                 ;
    }
    else
    {
        if(USE_16_HEATER==eht4Heater)
        {
            if(iSocketBaseTempCount==eDut2ea)
            {
                NowTemperature=NowTemperature+                                  //Richard 20230224 : 修正溫度顯示內容
                "Time"                              +","+
                "TempType"                          +","+
                "Plate1"                            +","+
                "Plate2"                            +","+
                "Shut1"                             +","+
                "Shut2"                             +","+
                "Chamber"                           +","+
                "CCD"                               +","+
                "Dut1"                              +","+
                "Dut2"                              +","+
                "HeatGun1"                          +","+
                "HeatGun2"                          +","+
                "Head1"                             +","+
                "Head2"                             +","+
                "Head3"                             +","+
                "Head4"                             ;
            }
            else if(iSocketBaseTempCount==eDut1ea)
            {
                NowTemperature=NowTemperature+
                "Time"                              +","+
                "TempType"                          +","+
                "Plate1"                            +","+
                "Plate2"                            +","+
                "Shut1"                             +","+
                "Shut2"                             +","+
                "Chamber"                           +","+
                "CCD"                               +","+
                "Dut"                               +","+
                "HeatGun1"                          +","+
                "HeatGun2"                          +","+
                "Head1"                             +","+
                "Head2"                             +","+
                "Head3"                             +","+
                "Head4"                             ;
            }
        }
        else
        {
            NowTemperature=NowTemperature+
            "Time"                              +","+
            "TempType"                          +","+
            "Plate1"                            +","+
            "Plate2"                            +","+
            "Shut1"                             +","+
            "Shut2"                             +","+
            "Chamber"                           +","+
            "CCD"                               ;

            if(CUSTOMER_CODE==CC_ASE_KaohSiung)
            {
                  //kevin 20200520 log 4 dut
                  NowTemperature=NowTemperature            +","+
                  "DUT1"                          +","+
                  "DUT2"                          +","+
                  "DUT3"                          +","+
                  "DUT4"                          ;
            }
            else
            {
            //JerryYang 20180417 (jou) : 補上Dut溫度log
            //==>
                if(iSocketBaseTempCount==eDut4ea)
                {
                    NowTemperature=NowTemperature            +","+
                    "DUT1"                          +","+
                    "DUT2"                          +","+
                    "DUT3"                          +","+
                    "DUT4"                          ;
                }
                else if(iSocketBaseTempCount==eDut2ea)
                {
                    NowTemperature=NowTemperature            +","+
                    "DUT1"                                   +","+
                    "DUT2"                                   ;
                }
                else
                {
                    NowTemperature=NowTemperature   +","+
                    "Socket"                        ;
                }
            }
            //<==
            //JerryYang 20180417 (jou) : 補上Dut溫度log

            NowTemperature=NowTemperature       +","+
            "Aa1"                               +","+
            "Ab1"                               +","+
            "Ac1"                               +","+
            "Ad1"                               +","+
            "Aa2"                               +","+
            "Ab2"                               +","+
            "Ac2"                               +","+
            "Ad2"                               +","+
            "Ba1"                               +","+
            "Bb1"                               +","+
            "Bc1"                               +","+
            "Bd1"                               +","+
            "Ba2"                               +","+
            "Bb2"                               +","+
            "Bc2"                               +","+
            "Bd2"                               ;
            if(USE_16_HEATER>2)
            {
                NowTemperature=NowTemperature   +","+
                "Ae1"                           +","+
                "Af1"                           +","+
                "Ag1"                           +","+
                "Ah1"                           +","+
                "Ae2"                           +","+
                "Af2"                           +","+
                "Ag2"                           +","+
                "Ah2"                           +","+
                "Be1"                           +","+
                "Bf1"                           +","+
                "Bg1"                           +","+
                "Bh1"                           +","+
                "Be2"                           +","+
                "Bf2"                           +","+
                "Bg2"                           +","+
                "Bh2"                           ;
            }
        }
    }

    if(ATC_SYSTEM==eATCHonPrecType)                                             //Ifor 20151119 新增海思ATC2.0 溫度Log
    {
        NowTemperature=NowTemperature           +","+
        "ATC First CH1"                         +","+
        "ATC Second CH1"                        +","+
        "ATC First CH2"                         +","+
        "ATC Second CH2"                        +","+
        "ATC First CH3"                         +","+
        "ATC Second CH3"                        +","+
        "ATC First CH4"                         +","+
        "ATC Second CH4"                        +","+
        "ATC Working Temp"                      +","+
        "ATC Chiller SV "                       ;
    }
    else if(ATC_SYSTEM==eNewATCSystem)                                          //Ifor 20160303 新增 NewATC 溫度Log
    {
        if(iATC_Use_Heat_Count<=8)                                              //Ifor 20190906 : 海思尚未使用 ATC 8Site機台
        {
            NowTemperature=NowTemperature           +","+
            "ATC First CH1"                         +","+
            "ATC Second CH1"                        +","+
            "ATC First CH2"                         +","+
            "ATC Second CH2"                        +","+
            "ATC First CH3"                         +","+
            "ATC Second CH3"                        +","+
            "ATC First CH4"                         +","+
            "ATC Second CH4"                        ;

            if(iATC_Use_Heat_Count>4)
            {
                NowTemperature=NowTemperature       +","+
                "ATC First CH5"                     +","+
                "ATC Second CH5"                    +","+
                "ATC First CH6"                     +","+
                "ATC Second CH6"                    +","+
                "ATC First CH7"                     +","+
                "ATC Second CH7"                    +","+
                "ATC First CH8"                     +","+
                "ATC Second CH8"                    ;
            }
            NowTemperature=NowTemperature+","+"ATC Working Temp";
        }
        else                                                                    //Ifor 20190909 :預留 32Site ATC Log
        {
        }
    }

    if(BAR_CODE_INSTALL==ebctInShtIntel)
    {
         NowTemperature=NowTemperature           +","+
         "2D,"                                   ;
    }

    if(source==1)
    {
        if(ATC_SYSTEM==eATCHonPrecType || ATC_SYSTEM==eNewATCSystem)
        {
            NowTemperature=NowTemperature           +","+
            "RunState"                              +","+
            "Run ATC State"                         +","+
            "Monitor ATC State"                     +","+
            "Set Range"                             ;
        }
    }

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20190615 tester on line offline
    {
        NowTemperature=NowTemperature           +","+
        "Tester"                                ;
    }

    NowTemperature=NowTemperature +","+                                         //kevin 20191226 change
    "Hot Gun 1"                   +","+
    "Hot Gun 2";

    //Ifor 20190905 :add 海思 V02.30 版  上傳Server資料增加Lot ID欄位
    /*
    if(CosFunction.bHiSiliconFunction==true || CUSTOMER_CODE==CC_KYEC_LEE)
    {
        NowTemperature=NowTemperature           +","+"Lot ID";
    } */

//    if(CosFunction.bHiSiliconFunction==true ||                                //Steven 20240925 : mark for 上傳Server資料增加Lot ID欄位
//       CUSTOMER_CODE==CC_KYEC_LEE  ||
//       CUSTOMER_CODE==CC_ASE_KaohSiung)                                       //kevin 20191210 add
    {
        NowTemperature=NowTemperature+",Lot ID , Temp Target  , Low Limit , High Limit , OSAT Lot number," ;            //kevin 20191210 add
        for(int i=0; i<tcTotalCount; i++)                                       //kevin 20200305 change
        {
            NowTemperature=NowTemperature+sTempName[i]+",";
        }
    }
    return NowTemperature;
}

// ---------------------------------------------------------------------------
//  ShowATCAlarmPosition -- golden :3567-3657.  Pure iCode -> position
//  classifier; touches only MMATC_* extern-const-int constants.
// ---------------------------------------------------------------------------
int TFormHS::ShowATCAlarmPosition(int iCode)                                    //Ifor 20160830 add ATC Alarm 位置顯示
{
    int iATCPos=0;
    //Ifor 20170330 (wei) add ATC Alarm Code 相關說明
    //======MMATC_TCPIP======//
    //WAR15230 ATC system alarm: Network disconnect!!
    //======MMATC_WaterValve======//
    //WAR15223 ATC system alarm: <ALM06>Water valve motor error
    //======MMATC_RS232======//
    //WAR15227 ATC system alarm: ATC3.1 module connect fail!!
    //WAR15228 ATC system alarm: ATC Command time out!!
    //WAR15229 ATC system alarm: Receive #SETFAIL!!
    //======MMATC_NI======//
    //WAR15209 ATC system alarm: Ts Thermocouple Continuous Error
    //WAR15224 ATC system alarm: NI module initial fail!!
    //WAR15226 ATC system alarm: DA module connect fail!!
    //======MMATC_Head======//
    //WAR15205 ATC system alarm: Water temperature too high!!
    //WAR15206 ATC system alarm: Temperature over setting !!
    //WAR15207 ATC system alarm: Tc Thermocouple Continuous Error
    //WAR15208 ATC system alarm: Ts thermocouple over setting
    //WAR15242 ATC system alarm: Temperature over setting !!
    //======MMATC_PowerSupply======//
    //WAR15218 ATC system alarm: <ALM01>GPUTJ board alarm/INT thermal couple error
    //WAR15219 ATC system alarm: <ALM02>PV over max temp error.
    //WAR15220 ATC system alarm: <ALM03>GPUTJ board sensor mode error (TJ/TC)
    //WAR15221 ATC system alarm: <ALM04>GPUTJ board communication error
    //WAR15238 ATC system alarm: ATC power supply off error

    //WAR15246 ATC system alarm: Undefined Serial number!!
    //WAR15247 ATC system alarm: Serial number does not match record!!
    //WAR15248 ATC system alarm: The serial number conflicts with other channels!!
    //WAR15319 ATC alarm: ATC temperature Index always same error
    //======MMATC_Chiller======//
    //WAR15200 ATC system alarm: Water tank leaking!!
    //WAR15201 ATC system alarm: Water tank over high limit!!
    //WAR15202 ATC system alarm: Water tank over low limit!!
    //WAR15203 ATC system alarm: Water flow error!!
    //WAR15204 ATC system alarm: [ARM] Water flow error!!
    //WAR15210 ATC system alarm: Chiller circulating fluid pressure too high!!
    //WAR15211 ATC system alarm: Chiller no power!!
    //WAR15212 ATC system alarm: Low level in tank!!
    //WAR15213 ATC system alarm: Over high temperature in chiller!!
    //WAR15214 ATC system alarm: Over Low temperature in chiller!!
    //WAR15215 ATC system alarm: Circulating fluid discharge pressure rise!!
    //WAR15216 ATC system alarm: Circulating fluid discharge pressure drop!!
    //WAR15217 ATC system alarm: <Chiller can't running!!>
    //WAR15234 ATC system alarm: Chiller has not started error
    //WAR15235 ATC system alarm: Chiller connect error
    //WAR15236 ATC system alarm: Water flow error
    //WAR15241 ATC system alarm: Leak Water
    //WAR15245 ATC system alarm: Chiller cannot communication!!
    //======MMATC_ATC======//
    //WAR15222 ATC system alarm: <ALM05>ATC IPC is idle
    //WAR15225 ATC system alarm: Read recipe file error!!
    //WAR15231 ATC system alarm: Unknown alarm code!!
    //WAR15232 ATC system alarm: Unknown Command code!!
    //WAR15233 ATC system alarm: Write Five same Temperature
    //WAR15237 ATC system alarm: Water temperature sensor over high
    //WAR15239 ATC system alarm: ATC Disable Site
    //WAR15240 ATC system alarm: Temperature below error

    if(iCode==15230)
        iATCPos=MMATC_TCPIP;
    else if(iCode==15223)
        iATCPos=MMATC_WaterValve;
    else if(iCode>=15227 && iCode<=15229)
        iATCPos=MMATC_RS232;
    else if(iCode==15209 || iCode==15224 || iCode==15226)
        iATCPos=MMATC_NI;
    else if((iCode>=15205 && iCode<=15208) || iCode==15242)
        iATCPos=MMATC_Head;
    else if(iCode>=15218 && iCode<=15221)
        iATCPos=MMATC_PowerSupply;
    else if(iCode==15238 || iCode==15319)
        iATCPos=MMATC_PowerSupply;
    else if(iCode>=15244 && iCode<=15246)                                       //Ifor 20170330 (wei) add ATC3.1 Serial number Alarm
        iATCPos=MMATC_PowerSupply;
    else if(iCode>=15200 && iCode<=15204)
        iATCPos=MMATC_Chiller;
    else if(iCode>=15210 && iCode<=15217)
        iATCPos=MMATC_Chiller;
    else if(iCode>=15234 && iCode<=15236)
        iATCPos=MMATC_Chiller;
    else if(iCode==15241 || iCode==15243)
        iATCPos=MMATC_Chiller;
    else
        iATCPos=MMATC_ATC;

    return iATCPos;
}

// ---------------------------------------------------------------------------
//  CheckClockTrigger -- golden :4234-4417.  Pure edge-detector; all state is
//  its own function-local statics.  GOLDEN NOTE G-1: the 30/90/120-min
//  branches' magic offsets (+3000/+1300) are not re-derived, translated
//  verbatim.
// ---------------------------------------------------------------------------
bool TFormHS::CheckClockTrigger(int iClock)                                     //Sam 20230504 : 整合 Clock 觸發執行動作
{
    static bool b0000AM=false, b0100AM=false, b0800AM=false, b0800PM=false, bPerHours=false;
    static bool b30_0min=false,b30_1min=false, b90_0min=false, b90_1min=false, b120min=false;

    bool bResult=false;
    int iNowTime=0,iTime=0;
    int iInterVal=0;

    iNowTime=StrToInt(GetOnlyTimeInfoByString());
    if(iClock==0)                                                               //00:00AM
    {
        iTime=0;
        if(b0000AM)
        {
            if(iNowTime>iTime+3)
            {
                b0000AM=false;
                bResult=true;
            }
        }
        else
        {
            if(iNowTime>iTime && iNowTime<iTime+3)
                b0000AM=true;
        }
    }
    else if(iClock==1)                                                          //01:00AM
    {
        iTime=10000;
        if(b0100AM)
        {
            if(iNowTime>iTime+3)
            {
                b0100AM=false;
                bResult=true;
            }
        }
        else
        {
            if(iNowTime>iTime && iNowTime<iTime+3)
                b0100AM=true;
        }
    }
    else if(iClock==8)                                                          //08:00AM
    {
        iTime=80000;
        if(b0800AM)
        {
            if(iNowTime>iTime+3)
            {
                b0800AM=false;
                bResult=true;
            }
        }
        else
        {
            if(iNowTime>iTime && iNowTime<iTime+3)
                b0800AM=true;
        }
    }
    else if(iClock==20)                                                         //08:00PM
    {
        iTime=200000;
        if(b0800PM)
        {
            if(iNowTime>iTime+3)
            {
                b0800PM=false;
                bResult=true;
            }
        }
        else
        {
            if(iNowTime>iTime && iNowTime<iTime+3)
                b0800PM=true;
        }
    }
    else if(iClock==60)                                                         //整點
    {
        iTime=10000;
        if(bPerHours)
        {
            if(iNowTime%iTime>3)
            {
                bPerHours=false;
                bResult=true;
            }
        }
        else
        {
            if(iNowTime%iTime<3)
                bPerHours=true;
        }
    }
    else if(iClock==30)                                                         //30min     00:00 00:30 01:00 01:30 ......
    {
        iTime=10000;
        iInterVal=iNowTime/iTime;
        if(b30_0min)
        {
            if(iNowTime>(iInterVal*iTime)+3)
            {
                b30_0min=false;
                bResult=true;
            }
        }
        else
        {
            if((iNowTime>(iInterVal*iTime) &&
                iNowTime<(iInterVal*iTime)+3))
                b30_0min=true;
        }

        if(b30_1min)
        {
            if(iNowTime>(iInterVal*iTime)+3000+3)
            {
                b30_1min=false;
                bResult=true;
            }
        }
        else
        {
            if(iNowTime>(iInterVal*iTime)+3000 &&
               iNowTime<(iInterVal*iTime)+3000+3)
                b30_1min=true;
        }
    }
    else if(iClock==90)                                                         //90min     00:00 01:30 03:00 04:30 ..........
    {
        iTime=30000;
        iInterVal=iNowTime/iTime;
        if(b90_0min)
        {
            if(iNowTime>(iInterVal*iTime)+3)
            {
                b90_0min=false;
                bResult=true;
            }
        }
        else
        {
            if(iNowTime>(iInterVal*iTime) &&
               iNowTime<(iInterVal*iTime)+3)
                b90_0min=true;
        }

        if(b90_1min)
        {
            if(iNowTime>(iInterVal*iTime)+1300+3)
            {
                b90_1min=false;
                bResult=true;
            }
        }
        else
        {
            if(iNowTime>(iInterVal*iTime)+1300 &&
               iNowTime<(iInterVal*iTime)+1300+3)
                b90_1min=true;
        }
    }
    else if(iClock==120)                                                        //120min
    {
        iTime=20000;
        iInterVal=iNowTime/iTime;
        if(b120min)
        {
            if(iNowTime>(iInterVal*iTime)+3)
            {
                b120min=false;
                bResult=true;
            }
        }
        else
        {
            if(iNowTime>(iInterVal*iTime) &&
               iNowTime<(iInterVal*iTime)+3)
                b120min=true;
        }
    }
    return bResult;
}

// ---------------------------------------------------------------------------
//  GetTitleName -- golden :4635-4743.  Pure string builder; calls this
//  class's own GetTempUseName for the Temp case.
// ---------------------------------------------------------------------------
void TFormHS::GetTitleName(AnsiString &S, eRecordType iType)                    //Ifor 20221024 add: KLT Auto Motive Function
{
    switch (iType)
    {
        case eReocrd_EP:
            S ="Ver:"+asHandlerVersion+"\n";                                    //kevin 20191212 add ver
            S=S+"Time,Index,Setting,Kpa,Kg,Alarm,Status";

            S=S+",Dual Setting,Dual Kpa,Dual Kg,Dual Alarm";                    //Ifor 20221228 add:新增Dual Ep Log

//            if(CosFunction.bHiSiliconFunction==true ||                        //Steven 20240925 : mark for 上傳Server資料增加Lot ID欄位
//               CUSTOMER_CODE==CC_KYEC_LEE)                                    //Ifor 20190905 :add 海思 V02.30 版  上傳Server資料增加Lot ID欄位
            {
                S=S+","+"Lot ID, OSAT Lot number ";
            }
            break;
        case eReocrd_Temp:
            if(CUSTOMER_CODE==CC_KYEC_LEE)
                S=GetTempUseName(1);
            else
                S=GetTempUseName(0);
            break;
        case eReocrd_ESD:
            S ="Ver:"+asHandlerVersion+"\n";                                    //kevin 20191212 add ver
            S = S+
                "Time"                  +","+
                "EnableAlarm"           +","+
                "AlarmContinuousTimes"  +","+
                "AlarmOccurrence"       +","+
                "ScanInterval"          +","+
                "Station1_Status"       +","+
                "Station1_1_Status"     +","+
                "Station1_1_Setting"    +","+
                "Station1_1_Value"      +","+
                "Station1_2_Status"     +","+
                "Station1_2_Setting"    +","+
                "Station1_2_Value"      +","+
                "Station1_3_Status"     +","+
                "Station1_3_Setting"    +","+
                "Station1_3_Value"      +","+
                "Station2_Status"       +","+
                "Station2_1_Status"     +","+
                "Station2_1_Setting"    +","+
                "Station2_1_Value"      +","+
                "Station2_2_Status"     +","+
                "Station2_2_Setting"    +","+
                "Station2_2_Value"      +","+
                "Station2_3_Status"     +","+
                "Station2_3_Setting"    +","+
                "Station2_3_Value"      +","+
                "Station3_Status"       +","+
                "Station3_1_Status"     +","+
                "Station3_1_Setting"    +","+
                "Station3_1_Value"      +","+
                "Station3_2_Status"     +","+
                "Station3_2_Setting"    +","+
                "Station3_2_Value"      +","+
                "Station3_3_Status"     +","+
                "Station3_3_Setting"    +","+
                "Station3_3_Value"      +","+
                "Station4_Status"       +","+
                "Station4_1_Status"     +","+
                "Station4_1_Setting"    +","+
                "Station4_1_Value"      +","+
                "Station4_2_Status"     +","+
                "Station4_2_Setting"    +","+
                "Station4_2_Value"      +","+
                "Station4_3_Status"     +","+
                "Station4_3_Setting"    +","+
                "Station4_3_Value"      +","+
                "Station5_Status"       +","+
                "Station5_1_Status"     +","+
                "Station5_1_Setting"    +","+
                "Station5_1_Value"      +","+
                "Station5_2_Status"     +","+
                "Station5_2_Setting"    +","+
                "Station5_2_Value"      +","+
                "Station5_3_Status"     +","+
                "Station5_3_Setting"    +","+
                "Station5_3_Value"      +","+
                "Station6_Status"       +","+
                "Station6_1_Status"     +","+
                "Station6_1_Setting"    +","+
                "Station6_1_Value"      +","+
                "Station6_2_Status"     +","+
                "Station6_2_Setting"    +","+
                "Station6_2_Value"      +","+
                "Station6_3_Status"     +","+
                "Station6_3_Setting"    +","+
                "Station6_3_Value"      ;

//            if(CosFunction.bHiSiliconFunction==true ||                        //Ifor 20190905 :add 海思 V02.30 版  上傳Server資料增加Lot ID欄位
//               CUSTOMER_CODE==CC_KYEC_LEE ||                                  //Steven 20240925 : mark for 上傳Server資料增加Lot ID欄位
//               CUSTOMER_CODE==CC_ASE_KaohSiung)                               //kevin 20191212 add
            {
                S=S+",Lot ID, OSAT Lot number " ;                               //kevin 20191210 add
            }
            break;
        case eReocrd_ATCEven:
            break;
        case eReocrd_Space:
            break;
        case eReocrd_TestArm:
            S="Time,TestArm,Torque(%),Auto Height(mm),Contact Offset(mm),Lot ID";
            break;
        case eReocrd_ChangEven:
            break;
        default:
            break;
    }
}

// ---------------------------------------------------------------------------
//  NMFTP1Failure -- golden :3897-3921.  Pure TCmdType switch -> own
//  sFTPErrMsg/bFTPError fields + MyDBIProcess; no FTP connect in the body.
// ---------------------------------------------------------------------------
void TFormHS::NMFTP1Failure(bool & /*Handled*/, TCmdType Trans_Type)
{
    sFTPErrMsg="";
    switch(Trans_Type)
    {
        case cmdChangeDir   : sFTPErrMsg="FTP Failure -- ChangeDir failed"      ; break;
        case cmdMakeDir     : sFTPErrMsg="FTP Failure -- MakeDir failed"        ; break;
        case cmdDelete      : sFTPErrMsg="FTP Failure -- Delete failed"         ; break;
        case cmdRemoveDir   : sFTPErrMsg="FTP Failure -- RemoveDir failed"      ; break;
        case cmdList        : sFTPErrMsg="FTP Failure -- List failed"           ; break;
        case cmdRename      : sFTPErrMsg="FTP Failure -- Rename failed"         ; break;
        case cmdUpRestore   : sFTPErrMsg="FTP Failure -- UploadRestore failed"  ; break;
        case cmdDownRestore : sFTPErrMsg="FTP Failure -- DownloadRestore failed"; break;
        case cmdDownload    : sFTPErrMsg="FTP Failure -- Download failed"       ; break;
        case cmdUpload      : sFTPErrMsg="FTP Failure -- Upload failed"         ; break;
        case cmdAppend      : sFTPErrMsg="FTP Failure -- UploadAppend failed"   ; break;
        case cmdReInit      : sFTPErrMsg="FTP Failure -- Reinitialize failed"   ; break;
        case cmdAllocate    : sFTPErrMsg="FTP Failure -- Allocate failed"       ; break;
        case cmdNList       : sFTPErrMsg="FTP Failure -- NList failed"          ; break;
        case cmdDoCommand   : sFTPErrMsg="FTP Failure -- DoCommand failed"      ; break;
        case cmdCurrentDir  : sFTPErrMsg="FTP Failure -- CurrentDir failed"     ; break;
    }
    MyDBIProcess("Process", sFTPErrMsg);
    bFTPError=true;
}

// ---------------------------------------------------------------------------
//  NMFTP1UnSupportedFunction -- golden :3923-3947.
// ---------------------------------------------------------------------------
void TFormHS::NMFTP1UnSupportedFunction(TCmdType Trans_Type)
{
    sFTPErrMsg="";
    switch(Trans_Type)
    {
        case cmdChangeDir   : sFTPErrMsg="FTP UnSupported -- ChangeDir not supported by this server"       ; break;
        case cmdMakeDir     : sFTPErrMsg="FTP UnSupported -- MakeDir not supported by this server"         ; break;
        case cmdDelete      : sFTPErrMsg="FTP UnSupported -- Delete not supported by this server"          ; break;
        case cmdRemoveDir   : sFTPErrMsg="FTP UnSupported -- RemoveDir not supported by this server"       ; break;
        case cmdList        : sFTPErrMsg="FTP UnSupported -- List not supported by this server"            ; break;
        case cmdRename      : sFTPErrMsg="FTP UnSupported -- Rename not supported by this server"          ; break;
        case cmdUpRestore   : sFTPErrMsg="FTP UnSupported -- UploadRestore not supported by this server"   ; break;
        case cmdDownRestore : sFTPErrMsg="FTP UnSupported -- DownloadRestore not supported by this server" ; break;
        case cmdDownload    : sFTPErrMsg="FTP UnSupported -- Download not supported by this server"        ; break;
        case cmdUpload      : sFTPErrMsg="FTP UnSupported -- Upload not supported by this server"          ; break;
        case cmdAppend      : sFTPErrMsg="FTP UnSupported -- UploadAppend not supported by this server"    ; break;
        case cmdReInit      : sFTPErrMsg="FTP UnSupported -- Reinitialize not supported by this server"    ; break;
        case cmdAllocate    : sFTPErrMsg="FTP UnSupported -- Allocate not supported by this server"        ; break;
        case cmdNList       : sFTPErrMsg="FTP UnSupported -- NList not supported by this server"           ; break;
        case cmdDoCommand   : sFTPErrMsg="FTP UnSupported -- DoCommand not supported by this server"       ; break;
        case cmdCurrentDir  : sFTPErrMsg="FTP UnSupported -- CurrentDir not supported by this server"      ; break;
    }
    MyDBIProcess("Process", sFTPErrMsg);
    bFTPError=true;
}

// ---------------------------------------------------------------------------
//  NMFTP1Error -- golden :3949-3953.
// ---------------------------------------------------------------------------
void TFormHS::NMFTP1Error(TComponent * /*Sender*/, WORD /*Errno*/, AnsiString Errmsg)
{
    ShowMyMessage("TFormHS:NMFTP1Error:"+Errmsg, "");          //Eastsun 20260518 : 新增錯誤位置代碼
}

// ---------------------------------------------------------------------------
//  CheckTempOffset -- golden :3958-4049.  NAME RESERVED by the gated-16
//  list (bthermo.cpp:2628,:2632,:3941...).  GOLDEN NOTE G-3: the
//  Tri_Temp_Machine==1 && MachineTypeChoice==Type_HT1032 arm is unreachable
//  dead code (an earlier arm already claims every Tri_Temp_Machine==1 case)
//  -- translated verbatim, not fixed.
// ---------------------------------------------------------------------------
bool TFormHS::CheckTempOffset(int Addr, double SetTemp, bool bSendAlarm)
{
    bool bOverSet=false;
    static AnsiString asErrorAddr="";
    static bool bHasErr=false;
    static double dAlarmTime=0;                                                 //Ifor 20180828 : add 記錄溫度異常時間
    double dTime=0;                                                             //Ifor 20180828 : add 記錄溫度異常時間

    if(Addr==tcHeatGun1 || Addr==tcHeatGun2)                                    //kevin 20190620
    {
        if(SetTemp>250)
        {
            bOverSet=true;
        }
    }
    else if(Addr==tcChamber)
    {
        if(SetTemp>165)
        {
            bOverSet=true;
        }
    }
    else if(Tri_Temp_Machine==1)                                                //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    {
        if(SetTemp>=Tri_Temperature_MaxDegree)
        {
            bOverSet=true;
        }

        if(SetTemp<0)
        {
            if(SetTemp<Tri_Temperature_MinDegree)
                bOverSet=true;
        }
    }
    else if(CUSTOMER_CODE==CC_KYEC_LEE ||                                       //Ifor 20220610 add:KYEC要求取消150溫度限制
            CUSTOMER_CODE==CC_PTI)
    {
        if(SetTemp>=175)
        {
            bOverSet=true;
        }
    }
    else if(Tri_Temp_Machine==1 &&
            MachineTypeChoice==Type_HT1032)                                     //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    {
        if(SetTemp>Tri_Temperature_MaxDegree)
        {
            bOverSet=true;
        }
    }
    else
    {
        if(SetTemp>TempFuseLimitType)                                           //Steven 20250701 : 整合機台溫度設定
        {
            bOverSet=true;
        }
    }

    if(bOverSet==true)
    {
        if(bHasErr==true)
        {
            if(asErrorAddr.Pos(asTempCtrl[Addr])==0)                            //Ifor 20180828 :add 避免溫度Alarm 位置重複顯示
                asErrorAddr=asErrorAddr+ ", "+ asTempCtrl[Addr];
        }
        else
        {
            bHasErr=true;
            asErrorAddr=asTempCtrl[Addr];
            dAlarmTime = MyTickCount();                                         //取得Alarm 時間
        }
    }

    if(asErrorAddr!="")                                                         //字串有資料時判斷是否超過Alarm時間
    {
        dTime = MyTickCount();                                                  //取得現在時間
        if(std::fabs(dTime-dAlarmTime)>=60000)                                  //超過一分鐘無Alarm清除位置與旗標 -- DEVIATION D-9: golden `abs()`, ambiguous under MinGW libstdc++ for a double argument (int/long/long long overloads all viable, none exact) -- std::fabs is the unambiguous double overload, identical semantics
        {
            bHasErr=false;                                                      //避免Alarm後條件成立無更新時間導致無位置顯示問題
            asErrorAddr="";                                                     //清除溫控器位置避免誤報Alarm
        }
    }

    if(bSendAlarm==true)
    {
        ShowErrorMessage("WAR15194", 0, MMSystem, false, asErrorAddr);
        asErrorAddr="";
        bHasErr=false;
    }
    return bOverSet;
}

// ---------------------------------------------------------------------------
//  CheckATCTempWait -- golden :4745-4827.  Pure UN150Read[] threshold
//  comparison + own statics; RecordProcess only.
// ---------------------------------------------------------------------------
bool TFormHS::CheckATCTempWait()                                                //Ifor 20230608 add:KYEC 要求新增ATC 溫度等待功能
{
    bool bHasTempWait=false;
    static bool bNeedWait=false;
    int i=0;
    double dTempWorkBase;

    if(CUSTOMER_CODE==CC_KYEC_LEE &&
       bEnable_KLT_Function==false &&
       bCheckATCTemp==true &&
       Temperature.bATCActiveCooling==true)                                     //Ifor 20230608 add:
    {
        if(LastSet.iTemperature==Tempture_Hot ||
           LastSet.iTemperature==Tempture_AmbientHot)
            dTempWorkBase=Temperature.fWorkTemperBase;
        else
            dTempWorkBase=IniConfig.dATCAmbientTemperature;

        for(i=0; i<tcTotalCount; i++)
        {
            if(bUT150Install[i]==true)
            {
                if((i>=tcAa1 && i<=tcBd1) ||
                   (i>=tcAe1 && i<=tcBh1) ||
                   (i>=tcAa2 && i<=tcBd2) ||
                   (i>=tcAe2 && i<=tcBh2))
                {
                    if(IniConfig.bL07UseSingleTenmpertureLimit)
                    {
                        if(UN150Read[i]>(dTempWorkBase+IniConfig.dSingleTempLimit[i]))
                        {
                            bHasTempWait=true;
                        }
                        else if(UN150Read[i]<(dTempWorkBase-IniConfig.dSingleTempLimit[i]))
                        {
                            bHasTempWait=true;
                        }
                    }
                    else
                    {
                        if(UN150Read[i]>(dTempWorkBase+IniConfig.iATCTemperatureRange))
                        {
                            bHasTempWait=true;
                        }
                        else if(UN150Read[i]<(dTempWorkBase-IniConfig.iATCTemperatureRange))
                        {
                            bHasTempWait=true;
                        }
                    }
                }
            }
        }

        if(bNeedWait==false)                                                    //Ifor 20230615 fix:避免倒數等待中溫度剛好到達設定取消等待
        {
            if(bHasTempWait==true)
            {
                bNeedWait=true;
                iATCTempWaitTimer=180;
                iATCTempWaitTimer_Start = MyTickCount();
                RecordProcess("ATC Temp Wait Start!");
            }
            else
            {
                bCheckATCTemp=false;
            }
        }

        if(iATCTempWaitTimer<=0 && bNeedWait==true)
        {
            bCheckATCTemp=false;
            bNeedWait=false;
            RecordProcess("ATC Temp Wait Finish!");
        }
    }
    else
    {
        bCheckATCTemp=false;
        bNeedWait=false;
    }

    return bNeedWait;
}

// ---------------------------------------------------------------------------
//  CheckKeyPro -- golden :4898-4947.  DEVIATION D-8:
//  `#include "HTKeyPro.h"` / `KEYPRO_GET_LEVEL(3)` (Borland dllimport driver
//  header, not portable to MinGW) ADAPTED to
//  `#include "Public/HTKeyProShim.h"` / `KeyPro_GetLevel(3)` per that shim's
//  own migration note (Public/HTKeyProShim.h:33-36), prepared by an earlier
//  wave in anticipation of this exact translation.  GOLDEN NOTE G-4:
//  `bStart` is read only in the SystemStart==false branch -- verbatim.
// ---------------------------------------------------------------------------
TQPF_Timer KeyProCheckTime;                                                     //Ifor 20180227 (Steven) add SECS GEM Confirm the Employee ID
bool TFormHS::CheckKeyPro(bool bStart)                                          //Ifor 20230721 : add Check Keypro
{
    int iLevel=0;
    bool bNeedCheck=false;
    static bool bCheckKeyPro=false;                                             //Ifor 20230721 add:SECS GEM 使用KeyPro

    if(CosFunction.bSECSGEM_UseKeyPro &&
       IniConfig.bEnable_SECS_GEM==true)
    {
        if(SystemStart)
        {
            if(bCheckKeyPro==false)
            {
                KeyProCheckTime.SetSecAndOn(60);
                bCheckKeyPro=true;
            }
            else
            {
                if(KeyProCheckTime.Off())
                {
                  bNeedCheck=true;
                }
            }
        }
        else
        {
            if(bStart==true)
                bNeedCheck=true;
            else
                bNeedCheck=false;
        }

        if(bNeedCheck==true)
        {
            bCheckKeyPro=false;
            iLevel=KeyPro_GetLevel(3);
            if(iLevel!=1)
            {
                ShowMyMessage("For enabling SECS GEM function, please insert KeyPro.","開啟SECS GEM 功能,請插入KeyPro.");
                return true;
            }
        }
    }
    else
    {
        bCheckKeyPro=false;
    }

    return false;
}

// ---------------------------------------------------------------------------
//  GetGPIBLogFilePath -- golden :5027-5031.  Pure sprintf over
//  SystemYear/SystemMonth.
// ---------------------------------------------------------------------------
AnsiString TFormHS::GetGPIBLogFilePath()
{
    AnsiString asPath="";
    asPath.sprintf("%s\\%04d_%02d", "D:\\GPIBLOG\\Log", SystemYear, SystemMonth);
    return asPath;
}
