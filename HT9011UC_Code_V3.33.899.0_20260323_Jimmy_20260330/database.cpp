#include "MachineDefine.h"
#pragma hdrstop

#include "database.h"

#include "MachineType.h"
#include "common.h"
#include "cmydef.h"
#include "cprod.h"
#include "mymessbox.h"

#include "uHGemHT9045.h"
#include "uHGemClass.h"
#include "uHGemEquipment.h"
#include "OCR.h"
#include "HandlerSys.h"                                                         //Alick 20160822 add
#include "MessageDef.h"                                                         //Isaac 20210511 : TTLRS232板子版本檢查
#include "ATC_Handler_Side.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma resource "*.dfm"

TDataModule1 *DataModule1;                                                      //"Shuttle 1", "Shuttle 2",

//---------------------------------------------------------------------------
SYSTEM_MODULAR HSys;                                                            // 電控元件及共用參數集合
TMyStringList *tsSoftwareExeTime;                                               //Steven 20210526 : 紀錄軟體執行時間
TQPF_Timer  SoftwareExeTimer;
TQPF_Timer  SoftwareExeTimerSum;
int iSoftwareExeTag=0;
//---------------------------------------------------------------------------
__fastcall TDataModule1::TDataModule1(TComponent* Owner)
    : TDataModule(Owner)
{
    CylinderTable->DatabaseName=ALIAS;
    SwitchTable->DatabaseName=ALIAS;
    MotorTable->DatabaseName=ALIAS;
    SensorTable->DatabaseName=ALIAS;
    SuckerTable->DatabaseName=ALIAS;
}
//---------------------------------------------------------------------------
_fastcall SYSTEM_MODULAR::SYSTEM_MODULAR()
{
    LogSoftwareOnTime("SYSTEM_MODULAR, Start");
    SysTimer.LatchCycleTime(true);                                              //Steven 20190714 : 計算系統時間
    InitialMemory();                                                            //Steven 20160319 : 初始化數值
    InitCommonString();                                                         //jou 2016-08-24 Initial Common String
    OpenGeneralIniFile();                                                       //Steven 20141120 : Add Read/Write IniFile Speed
    BinDisCtrl=NULL;
    ATKRecipeInfo=new ATK_RECIPE_INFO();                                        //Steven 20170901 (wei) : For ATK要新增工作檔比對用的檔案

    ReadGeneralIni();                                                           //Steven 20190320 : Handler Sys讀檔獨立成Function
    SystemModularInitial();
    AddSpace(asGeneralPath);

    mapATCAlarmCode.clear();                                                    //Steven 20160706 : Add ATC alarm code
    mapATCAlarmCode["ALM001"]="WAR15200";
    mapATCAlarmCode["ALM002"]="WAR15201";
    mapATCAlarmCode["ALM003"]="WAR15202";
    mapATCAlarmCode["ALM004"]="WAR15203";
    mapATCAlarmCode["ALM005"]="WAR15204";
    mapATCAlarmCode["ALM006"]="WAR15205";
    mapATCAlarmCode["ALM010"]="WAR15206";
    mapATCAlarmCode["ALM011"]="WAR15207";
    mapATCAlarmCode["ALM012"]="WAR15208";
    mapATCAlarmCode["ALM013"]="WAR15209";
    mapATCAlarmCode["ALM020"]="WAR15210";
    mapATCAlarmCode["ALM021"]="WAR15211";
    mapATCAlarmCode["ALM022"]="WAR15212";
    mapATCAlarmCode["ALM023"]="WAR15213";
    mapATCAlarmCode["ALM024"]="WAR15214";
    mapATCAlarmCode["ALM025"]="WAR15215";
    mapATCAlarmCode["ALM026"]="WAR15216";
    mapATCAlarmCode["ALM027"]="WAR15217";
    mapATCAlarmCode["ALM031"]="WAR15218";
    mapATCAlarmCode["ALM032"]="WAR15219";
    mapATCAlarmCode["ALM033"]="WAR15220";
    mapATCAlarmCode["ALM034"]="WAR15221";
    mapATCAlarmCode["ALM035"]="WAR15222";
    mapATCAlarmCode["ALM036"]="WAR15223";

    mapATCAlarmCode["ALM037"]="WAR15251";                                       //JerryYang 20220728 : add ATC alarm code
    mapATCAlarmCode["ALM038"]="WAR15252";
    mapATCAlarmCode["ALM039"]="WAR15253";
    mapATCAlarmCode["ALM040"]="WAR15254";
    mapATCAlarmCode["ALM041"]="WAR15255";
    mapATCAlarmCode["ALM042"]="WAR15256";
    mapATCAlarmCode["ALM043"]="WAR15257";
    mapATCAlarmCode["ALM044"]="WAR15258";
    mapATCAlarmCode["ALM045"]="WAR15259";
    mapATCAlarmCode["ALM046"]="WAR15260";
    mapATCAlarmCode["ALM047"]="WAR15261";
    mapATCAlarmCode["ALM048"]="WAR15262";
    mapATCAlarmCode["ALM049"]="WAR15263";
    mapATCAlarmCode["ALM050"]="WAR15264";
    mapATCAlarmCode["ALM051"]="WAR15265";
    mapATCAlarmCode["ALM052"]="WAR15266";
    mapATCAlarmCode["ALM053"]="WAR15267";
    mapATCAlarmCode["ALM060"]="WAR15268";
    mapATCAlarmCode["ALM061"]="WAR15269";
    mapATCAlarmCode["ALM062"]="WAR15270";
    mapATCAlarmCode["ALM063"]="WAR15271";
    mapATCAlarmCode["ALM064"]="WAR15272";
    mapATCAlarmCode["ALM065"]="WAR15273";
    mapATCAlarmCode["ALM066"]="WAR15274";
    mapATCAlarmCode["ALM067"]="WAR15275";
    mapATCAlarmCode["ALM068"]="WAR15276";
    mapATCAlarmCode["ALM069"]="WAR15277";
    mapATCAlarmCode["ALM070"]="WAR15278";
    mapATCAlarmCode["ALM071"]="WAR15279";
    mapATCAlarmCode["ALM072"]="WAR15280";
    mapATCAlarmCode["ALM073"]="WAR15281";
    mapATCAlarmCode["ALM074"]="WAR15282";
    mapATCAlarmCode["ALM075"]="WAR15283";
    mapATCAlarmCode["ALM076"]="WAR15284";
    mapATCAlarmCode["ALM077"]="WAR15285";
    mapATCAlarmCode["ALM078"]="WAR15286";
    mapATCAlarmCode["ALM079"]="WAR15287";
    mapATCAlarmCode["ALM080"]="WAR15288";
    mapATCAlarmCode["ALM081"]="WAR15289";
    mapATCAlarmCode["ALM082"]="WAR15290";
    mapATCAlarmCode["ALM083"]="WAR15291";
    mapATCAlarmCode["ALM084"]="WAR15292";
    mapATCAlarmCode["ALM085"]="WAR15293";
    mapATCAlarmCode["ALM086"]="WAR15294";
    mapATCAlarmCode["ALM087"]="WAR15295";
    mapATCAlarmCode["ALM088"]="WAR15296";
    mapATCAlarmCode["ALM089"]="WAR15297";

    mapATCAlarmCode["MES001"]="WAR15224";
    mapATCAlarmCode["MES002"]="WAR15225";
    mapATCAlarmCode["MES003"]="WAR15226";
    mapATCAlarmCode["MES010"]="WAR15227";
    mapATCAlarmCode["MES011"]="WAR15228";
    mapATCAlarmCode["MES012"]="WAR15229";
    mapATCAlarmCode["MES101"]="WAR15230";
    mapATCAlarmCode["MES901"]="WAR15231";
    mapATCAlarmCode["MES902"]="WAR15232";
    mapATCAlarmCode["ALM028"]="WAR15245";
    //Ifor 20170327 (wei) add ATC3.1 Serial number 相關Alarm Code
    //==>
    mapATCAlarmCode["MES903"]="WAR15246";
    mapATCAlarmCode["MES904"]="WAR15247";
    mapATCAlarmCode["MES905"]="WAR15248";
    //<==
    //Ifor 20170327 (wei) add ATC3.1 Serial number 相關Alarm Code
    mapATCAlarmCode["WAR15313"]="WAR15313";
    mapATCAlarmCode["WAR15314"]="WAR15314";
    mapATCAlarmCode["WAR15315"]="WAR15315";
    mapATCAlarmCode["WAR15316"]="WAR15316";
    mapATCAlarmCode["WAR15317"]="WAR15317";
    mapATCAlarmCode["WAR15318"]="WAR15318";
    mapATCAlarmCode["ALM499"]="WAR15319";

    /*              //JerryYang 20251124 : Mark掉, fix ATC alarm code對應錯誤
    if(ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_61)
    {
        mapATCAlarmCode["ALM007"]="WAR15323";                                       //Ztex 2024.04.15 Add ATC 6.1 New Alarm Code ==>
        mapATCAlarmCode["ALM008"]="WAR15324";
        mapATCAlarmCode["ALM009"]="WAR15325";

        mapATCAlarmCode["ALM014"]="WAR15326";
        mapATCAlarmCode["ALM015"]="WAR15327";
        mapATCAlarmCode["ALM016"]="WAR15328";
        mapATCAlarmCode["ALM017"]="WAR15329";
        mapATCAlarmCode["ALM018"]="WAR15330";
        mapATCAlarmCode["ALM019"]="WAR15331";

        mapATCAlarmCode["ALM054"]="WAR15332";
        mapATCAlarmCode["ALM055"]="WAR15333";
        mapATCAlarmCode["ALM056"]="WAR15334";
        mapATCAlarmCode["ALM057"]="WAR15335";
        mapATCAlarmCode["ALM058"]="WAR15336";

        mapATCAlarmCode["ALM100"]="WAR15337";
        mapATCAlarmCode["ALM101"]="WAR15338";
        mapATCAlarmCode["ALM102"]="WAR15339";
        mapATCAlarmCode["ALM200"]="WAR15340";
        mapATCAlarmCode["ALM201"]="WAR15341";
        mapATCAlarmCode["ALM400"]="WAR15342";
        mapATCAlarmCode["ALM402"]="WAR15343";

        mapATCAlarmCode["MES004"]="WAR15344";
        mapATCAlarmCode["MES005"]="WAR15345";
        mapATCAlarmCode["MES006"]="WAR15346";
        mapATCAlarmCode["MES007"]="WAR15347";
        mapATCAlarmCode["MES008"]="WAR15348";
        mapATCAlarmCode["MES009"]="WAR15349";

        mapATCAlarmCode["MES013"]="WAR15350";
        mapATCAlarmCode["MES014"]="WAR15351";
        mapATCAlarmCode["MES015"]="WAR15352";
        mapATCAlarmCode["MES016"]="WAR15353";
        mapATCAlarmCode["MES017"]="WAR15354";
        mapATCAlarmCode["MES018"]="WAR15355";
        mapATCAlarmCode["MES019"]="WAR15356";
        mapATCAlarmCode["MES020"]="WAR15357";
    //    mapATCAlarmCode["MES021"]="WAR15358";
        mapATCAlarmCode["MES022"]="WAR15358";
        mapATCAlarmCode["MES023"]="WAR15359";
        mapATCAlarmCode["MES024"]="WAR15360";
        mapATCAlarmCode["MES025"]="WAR15361";
        mapATCAlarmCode["MES026"]="WAR15362";
        mapATCAlarmCode["MES028"]="WAR15363";

        mapATCAlarmCode["MES102"]="WAR15364";
        mapATCAlarmCode["MES103"]="WAR15365";
        mapATCAlarmCode["MES104"]="WAR15366";
        mapATCAlarmCode["MES200"]="WAR15367";
        mapATCAlarmCode["MES201"]="WAR15368";
        mapATCAlarmCode["MES202"]="WAR15369";
        mapATCAlarmCode["MES203"]="WAR15370";
        mapATCAlarmCode["MES220"]="WAR15371";

        mapATCAlarmCode["WAR001"]="WAR15372";
        mapATCAlarmCode["WAR002"]="WAR15373";
        mapATCAlarmCode["WAR006"]="WAR15374";

        mapATCAlarmCode["WAR053"]="WAR15375";
        mapATCAlarmCode["WAR058"]="WAR15376";

        mapATCAlarmCode["WAR140"]="WAR15377";
        mapATCAlarmCode["WAR141"]="WAR15378";
        mapATCAlarmCode["WAR148"]="WAR15379";

        mapATCAlarmCode["WAR156"]="WAR15380";

        mapATCAlarmCode["WAR209"]="WAR15381";
        mapATCAlarmCode["WAR210"]="WAR15382";
        mapATCAlarmCode["WAR211"]="WAR15383";
        mapATCAlarmCode["WAR212"]="WAR15384";
        mapATCAlarmCode["WAR213"]="WAR15385";

        mapATCAlarmCode["WAR300"]="WAR15386";
        mapATCAlarmCode["WAR301"]="WAR15387";

        mapATCAlarmCode["WAR400"]="WAR15388";
        mapATCAlarmCode["WAR402"]="WAR15389";
        mapATCAlarmCode["WAR404"]="WAR15390";

        mapATCAlarmCode["WAR500"]="WAR15391";
        mapATCAlarmCode["WAR501"]="WAR15392";                                       //Ztex 2024.04.15 Add ATC 6.1 New Alarm Code <==

        mapATCAlarmCode["WAR502"]="WAR15393";                                       //Ztex 2024.10.01 Add ATC AirStream New Alarm Code
        mapATCAlarmCode["WAR503"]="WAR15394";                                       //Ztex 2024.10.01 Add ATC AirStream New Alarm Code
    }
    */

//Steven 20170425 (wei) : Add rotate motor
//==>
    MInRotate[0][0] =MInRotateKit;
    MInRotate[0][1] =MInRotateC;
    MInRotate[0][2] =MInRotateE;
    MInRotate[0][3] =MInRotateG;
    MInRotate[1][0] =MInRotateB;
    MInRotate[1][1] =MInRotateD;
    MInRotate[1][2] =MInRotateF;
    MInRotate[1][3] =MInRotateH;

    if(USE_ROTATE_KIT && (iRotate_Type==e4MotRotate || iRotate_Type==e2MotRotate2Dut))
    {
        MOutRotate[0][0]=MOutRotateC;
        MOutRotate[0][1]=MOutRotateKit;
        MOutRotate[0][2]=MOutRotateG;
        MOutRotate[0][3]=MOutRotateE;
        MOutRotate[1][0]=MOutRotateD;
        MOutRotate[1][1]=MOutRotateB;
        MOutRotate[1][2]=MOutRotateH;
        MOutRotate[1][3]=MOutRotateF;
    }
    else
    {
        MOutRotate[0][0]=MOutRotateKit;
        MOutRotate[0][1]=MOutRotateC;
        MOutRotate[0][2]=MOutRotateE;
        MOutRotate[0][3]=MOutRotateG;
        MOutRotate[1][0]=MOutRotateB;
        MOutRotate[1][1]=MOutRotateD;
        MOutRotate[1][2]=MOutRotateF;
        MOutRotate[1][3]=MOutRotateH;
    }
//<==
//Steven 20170425 (wei) : Add rotate motor

    LogSoftwareOnTime("SYSTEM_MODULAR, End");
}
//---------------------------------------------------------------------------
void __fastcall TDataModule1::DataModuleDestroy(TObject *Sender)
{
    try
    {

    }
    catch(...)
    {
        MyDBIProcess("Exception", "TDataModule1::DataModuleDestroy");
    }
    LogSoftwareOffTime("TDataModule1, FormDestroy");
}
//---------------------------------------------------------------------------
void SYSTEM_MODULAR::ReadGeneralIni()                                           //Steven 20190320 : Handler Sys讀檔獨立成Function
{
    AnsiString str="", MachName, sIndEPCnt;
    MyForceDirectories("D:\\GPIB9045\\system");
    str="D:\\GPIB9045\\system\\general.ini";
    MachName=CheckAndReadIniData(str, "Version", "Model", AnsiString("ModelNG"));   //jou 20200601 : GPIB 型號讀取失敗需Alarm,不應該回寫型號
    if(MachName=="9045GPIB" ||
       MachName=="9046GPIB" ||
       MachName=="9046_32GPIB" ||                                               //Steven 20120505 : 如果不是9045或46, 就改成預設的9045
       MachName=="9045GPIB_12Site" ||
       MachName=="502GPIB" ||                                                   //ChungHung 20130507 add HT9045 updata for 12site 517
       MachName=="1032GPIB" ||
       MachName=="7080GPIB")
    {
        bHandlerModel=true;
    }
    else
    {
//        WriteIniData(str, "Version", "Model", "9046_32GPIB");                 //jou 20200601 : GPIB 型號讀取失敗需Alarm,不應該回寫型號
        bHandlerModel=false;
        return;
    }
    #ifdef ASE_KaohSiung                                                        //kevin 20141222
        WriteIniDataGeneral("System", "CUSTOMER_CODE", CC_ASE_KaohSiung);
    #endif

    CUSTOMER_CODE=CheckAndReadIniDataGeneral("System", "CUSTOMER_CODE", 0);
    #ifdef ASE_KaohSiung                                                        //Steven 20110520 為了長茅哥的告來告去
        if(CUSTOMER_CODE!=CC_HONPREC_QC && CUSTOMER_CODE!=CC_ASE_KaohSiung_K12) //Steven 20131101 : Add ASE-K12
            CUSTOMER_CODE=CC_ASE_KaohSiung;
    #else
        if(CUSTOMER_CODE==CC_ASE_KaohSiung)
            CUSTOMER_CODE=CC_HONPREC_QC;
    #endif
    CustomerFunctionSelect();                                                   //客戶功能選擇區
    ReadLastSetIni();
    ReadEventLogAutoSaveInfo();                                                 //Steven 20110603
    //溫度相關-----------------------------
    USE_NEW_TEMPCTRL_FUNCTION   =false;                                         //Steven 20100707 暫時先強制用舊的架構

    TEMPCTRL_NEED_UNDER_20A     =CheckAndReadIniDataGeneral("TempCtrl", "TEMPCTRL_NEED_UNDER_20A", false); //預設是採用HotPlate分段
    bUseHotGunCheck             =CheckAndReadIniDataGeneral("TempCtrl", "UseHotGunCheck", false);           //kevin 20190621 hot gun 流量 不足偵測
    bUseHotGunFlowCheck         =CheckAndReadIniDataGeneral("TempCtrl", "UseHotGunFlowCheck", false);       //KaiChen 20190729 ：Hot Gun Flow

    //Caption=MachName;
    if(MachName=="9045GPIB")
    {
        TEMPCTRL_HOTPLATE_TOGTHER=CheckAndReadIniDataGeneral("TempCtrl", "TEMPCTRL_HOTPLATE_TOGTHER", false);  //jou 2010-08-16 start : SCC Hotplate不分段加熱
        MachineTypeChoice=Type_HT9045;
        NEW_MAX_Index_Col =8;

        WriteIniDataGeneral("TempCtrl", "bTEMPCTRL_Shuttle_TOGTHER", true);     //Steven 20150909 : 取消蝦頭分段加熱功能
        bTEMPCTRL_Shuttle_TOGTHER=true;
    }
    else if(MachName=="9045GPIB_12Site")                                        //ChungHung 20130507 add HT9045 updata for 12site 517 define Index Col
    {
        TEMPCTRL_HOTPLATE_TOGTHER=false;
        MachineTypeChoice=Type_HT9045_12Site;
        NEW_MAX_Index_Col =6;

        WriteIniDataGeneral("TempCtrl", "bTEMPCTRL_Shuttle_TOGTHER", true);     //Steven 20150909 : 取消蝦頭分段加熱功能
        bTEMPCTRL_Shuttle_TOGTHER=true;
    }
    else if(MachName=="9046GPIB")
    {
        //TEMPCTRL_HOTPLATE_TOGTHER=false;
        TEMPCTRL_HOTPLATE_TOGTHER=CheckAndReadIniDataGeneral("TempCtrl", "TEMPCTRL_HOTPLATE_TOGTHER", false);   //kevin 20150508 add //jou 2010-08-16 start : SCC Hotplate不分段加熱
        MachineTypeChoice=Type_HT9046;
        NEW_MAX_Index_Col=8;

        WriteIniDataGeneral("TempCtrl", "bTEMPCTRL_Shuttle_TOGTHER", true);     //Steven 20150909 : 取消蝦頭分段加熱功能
        bTEMPCTRL_Shuttle_TOGTHER=true;
    }
    else if(MachName=="9046_32GPIB")
    {
        bTEMPCTRL_Shuttle_TOGTHER=CheckAndReadIniDataGeneral("TempCtrl", "bTEMPCTRL_Shuttle_TOGTHER", true);        //kevin 20150508不分段加熱      //Steven 20150909 : 取消蝦頭分段加熱功能
        TEMPCTRL_HOTPLATE_TOGTHER=false;
        MachineTypeChoice=Type_HT9046_LS;
        NEW_MAX_Index_Col=8;
    }
    else if(MachName=="502GPIB")
    {
        //TEMPCTRL_HOTPLATE_TOGTHER=false;
        TEMPCTRL_HOTPLATE_TOGTHER=CheckAndReadIniDataGeneral("TempCtrl", "TEMPCTRL_HOTPLATE_TOGTHER", false);   //kevin 20150508 add //jou 2010-08-16 start : SCC Hotplate不分段加熱
        MachineTypeChoice=Type_HT502;
        NEW_MAX_Index_Col=4;

        WriteIniDataGeneral("TempCtrl", "bTEMPCTRL_Shuttle_TOGTHER", true);     //Steven 20150909 : 取消蝦頭分段加熱功能
        bTEMPCTRL_Shuttle_TOGTHER=true;
    }
    else if(MachName=="7080GPIB")
    {
        //TEMPCTRL_HOTPLATE_TOGTHER=false;
        TEMPCTRL_HOTPLATE_TOGTHER=CheckAndReadIniDataGeneral("TempCtrl", "TEMPCTRL_HOTPLATE_TOGTHER", false);   //kevin 20150508 add //jou 2010-08-16 start : SCC Hotplate不分段加熱
        MachineTypeChoice=Type_HT7080;
        NEW_MAX_Index_Col=4;

        WriteIniDataGeneral("TempCtrl", "bTEMPCTRL_Shuttle_TOGTHER", true);     //Steven 20150909 : 取消蝦頭分段加熱功能
        bTEMPCTRL_Shuttle_TOGTHER=true;
    }
    else if(MachName=="1032GPIB")
    {
        //TEMPCTRL_HOTPLATE_TOGTHER=false;
        TEMPCTRL_HOTPLATE_TOGTHER=CheckAndReadIniDataGeneral("TempCtrl", "TEMPCTRL_HOTPLATE_TOGTHER", false);   //kevin 20150508 add //jou 2010-08-16 start : SCC Hotplate不分段加熱
        MachineTypeChoice=Type_HT1032;
        NEW_MAX_Index_Col=8;

        WriteIniDataGeneral("TempCtrl", "bTEMPCTRL_Shuttle_TOGTHER", true);     //Steven 20150909 : 取消蝦頭分段加熱功能
        bTEMPCTRL_Shuttle_TOGTHER=true;
    }

    HotGunFlowEnable            =CheckAndReadIniDataGeneral("System",       "HotGunFlowEnable", 0);             //KaiChen 20190729 ：Hot Gun Flow
    HotGunFlow_LineNo           =CheckAndReadIniDataGeneral("System",       "HotGunFlow_LineNo", 0);            //KaiChen 20190729 ：Hot Gun Flow
    HotGunFlow_DevNo            =CheckAndReadIniDataGeneral("System",       "HotGunFlow_DevNo", 0);             //KaiChen 20190729 ：Hot Gun Flow
    HotGunFlow_Gun1_ChannelNo   =CheckAndReadIniDataGeneral("System",       "HotGunFlow_Gun1_ChannelNo", 0);    //KaiChen 20190729 ：Hot Gun Flow
    HotGunFlow_Gun2_ChannelNo   =CheckAndReadIniDataGeneral("System",       "HotGunFlow_Gun2_ChannelNo", 0);    //KaiChen 20190729 ：Hot Gun Flow

    if(MachineTypeChoice==Type_HT9045 ||
       MachineTypeChoice==Type_HT502)                                           //Steven 20190307 : Add for HT-9046LA
        SubMachineType=Type_None;
    else
        SubMachineType=CheckAndReadIniDataGeneral("Version", "SubModel", Type_None);

    SHUTTLE_COOLING     =CheckAndReadIniDataGeneral("TempCtrl", "SHUTTLE_COOLING", 0.0);        //冷風渦流管的Offset
    VORTEX_COOLING      =CheckAndReadIniDataGeneral("TempCtrl", "VORTEX_COOLING",  0.5);        //冷風渦流管的啟動Offset
    SOCKET_OFFSET       =CheckAndReadIniDataGeneral("TempCtrl", "SOCKET_OFFSET",  10.0);        //Socket的Offset
    bUseSocketTemp      =CheckAndReadIniDataGeneral("TempCtrl", "Socket",            0);        //第九軸加熱開啟
    bUseAuto2Empty      =CheckAndReadIniDataGeneral("System",   "bUseAuto2Empty",    0);        //kevin 20120718 Auto2 有分離氣缸可以送料進去
    SHUTTLE_COOLING     =CheckRange(SHUTTLE_COOLING, 10.0, 0.0);
    VORTEX_COOLING      =CheckRange(VORTEX_COOLING,  10.0, 0.0);
    SOCKET_OFFSET       =CheckRange(SOCKET_OFFSET,   30.0, 0.0);
    TC401HeaterControl  =CheckAndReadIniDataGeneral("TempCtrl", "HEATER_CTRL_TYPE", KT4H); //Steven 20141030 : 新增OMRON E5DC溫控器
    OFFLINE_ALARM       =CheckAndReadIniDataGeneral("System",   "OFFLINE_ALARM",    1);        //Steven 20140805

    USE_AUTO_RETEST     =CheckAndReadIniDataGeneral("System", "USE_AUTO_RETEST",     (int)eartUninstall);        //ChungHung 20140317 add Auto Retest

    //RogerYang 20250823 : 這裡會先執行，搬到這邊
    SUPPORT_2_EMPTY_EMPTY  =CheckAndReadIniDataGeneral("System", "SUPPORT_2_EMPTY_EMPTY", 0);   //Steven 20230907 : For HT-9011UC
    int iTemp;
    if(CheckIniData(asGeneralPath, "System", "AUTO_EMPTY_COLOR")==false)
    {
        if(SUPPORT_2_EMPTY_EMPTY==true)                                         //Steven 20230907 : For HT-9011UC
        {
            iTemp=2;
        }
        else
        {
            iTemp=1;
            if(Application->MessageBox("Machine Empty load and Color load is Auto?", NULL, MB_YESNO | MB_TOPMOST) != IDYES)
                iTemp=0;
        }
        WriteIniDataGeneral("System","AUTO_EMPTY_COLOR", iTemp);
        AUTO_EMPTY_COLOR=iTemp;
    }
    else
    {
        AUTO_EMPTY_COLOR=CheckAndReadIniDataGeneral("System", "AUTO_EMPTY_COLOR", 0);
    }

    if(USE_AUTO_RETEST==eartInstall)
    {
        UNLOADER_ART[eAuto1] =CheckAndReadIniDataGeneral("System", "UNLOADER_AUTO1_ART",     (int)eartInstall);   //Steven 20161221 (jou) : for SCK only Auto 2 has ART
        UNLOADER_ART[eAuto2] =CheckAndReadIniDataGeneral("System", "UNLOADER_AUTO2_ART",     (int)eartInstall);   //Steven 20161221 (jou) : for SCK only Auto 2 has ART
        UNLOADER_ART[eAuto3] =CheckAndReadIniDataGeneral("System", "UNLOADER_AUTO3_ART",     (int)eartInstall);   //Steven 20161221 (jou) : for SCK only Auto 2 has ART

        if(AUTO_EMPTY_COLOR>=3)                                                 //Steven 20230907 : For HT-9011UC
        {
            UNLOADER_ART[eAuto4]    =CheckAndReadIniDataGeneral("System", "UNLOADER_AUTO4_ART",     (int)eartInstall);
            UNLOADER_ART[eAuto5]    =CheckAndReadIniDataGeneral("System", "UNLOADER_AUTO5_ART",     (int)eartInstall);
            if(AUTO_EMPTY_COLOR>=4)
                UNLOADER_ART[eAuto6]=CheckAndReadIniDataGeneral("System", "UNLOADER_AUTO6_ART",     (int)eartInstall);
            else
                UNLOADER_ART[eAuto6]=(int)eartUninstall;
        }
        else
        {
            UNLOADER_ART[eAuto4]    =(int)eartUninstall;
            UNLOADER_ART[eAuto5]    =(int)eartUninstall;
            UNLOADER_ART[eAuto6]    =(int)eartUninstall;
        }
    }
    else
    {
        UNLOADER_ART[eAuto1] =(int)eartUninstall;
        UNLOADER_ART[eAuto2] =(int)eartUninstall;
        UNLOADER_ART[eAuto3] =(int)eartUninstall;
        UNLOADER_ART[eAuto4] =(int)eartUninstall;
        UNLOADER_ART[eAuto5] =(int)eartUninstall;
        UNLOADER_ART[eAuto6] =(int)eartUninstall;
    }

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
        bNewCatchTrayblock = CheckAndReadIniDataGeneral("System" , "UseCatchTrayBlock"   , 1);  //kevin 20200512 夾tray遮版削短
    else
        bNewCatchTrayblock = CheckAndReadIniDataGeneral("System" , "UseCatchTrayBlock"   , 0);  //kevin 20200512 夾tray遮版削短

    MOTOR_DRIVER_TYPE   =CheckAndReadIniDataGeneral("MotorDriver", "Type", Panasonic_DRIVER);   //Steven 20110321 : 分辨其他軸使用的馬達廠牌
    USE_COLOR_TRAY_SENSOR=CheckAndReadIniDataGeneral("System",   "USE_COLOR_TRAY_SENSOR", 0);   //20140903 wei colcr Tray
    USE_SOCKET_SENSOR=CheckAndReadIniDataGeneral("System",   "USE_SOCKET_SENSOR", 999);         //JerryYang 20200327 Socket sensor改用硬體選項判斷

    CANBUS_METHOD=CheckAndReadIniDataGeneral("System",   "Canbus_Method", 0);   //Sam 20210518 : 新增 CanBus 軟體配置

    for(int i=0; i<tcTotalCount; i++)                                           //Steven 20100831 : 常溫檢查的位置
    {
        str.sprintf("AMBIENT_TEMP_CHECK%02d", i+1);
        if(i<2)                                                                 //加熱盤預設不檢查
            AMBIENT_TEMP_CHECK[i]=CheckAndReadIniDataGeneral("TempCtrl", str, false);
        else
            AMBIENT_TEMP_CHECK[i]=CheckAndReadIniDataGeneral("TempCtrl", str, true);
    }

    //數字顯示器----------------------------------------------
    NUMBER_PANEL_TYPE   =CheckAndReadIniDataGeneral("System",          "NUMBER_PANEL_TYPE",    2);
    dNumberPanelDelay   =CheckAndReadIniDataGeneral("NUMBER_PANEL",    "NUMBER_PANEL_DELAY",   1.0);    //Sam 20240604 : 顯示器輪巡時間改為 double
    //Steven 20120217 : Com Port改成可定義
    sNumberPanelComPort =CheckAndReadIniDataGeneral("NUMBER_PANEL",    "COM_PORT",         AnsiString("COM4"));
    sNumberPanelComPort2=CheckAndReadIniDataGeneral("NUMBER_PANEL2",    "COM_PORT",        AnsiString("COM4"));

    sTorqueComPort      =CheckAndReadIniDataGeneral("IndexDriver",     "COM_PORT",         AnsiString("COM1"));
    sTempComPort        =CheckAndReadIniDataGeneral("TempCtrl",        "COM_PORT",         AnsiString("COM2"));
    sTempOmronComPort   =CheckAndReadIniDataGeneral("TempCtrl",        "COM_PORT_OMRON",   AnsiString("COM7"));
    TrayStepMotor_ComPort=CheckAndReadIniDataGeneral("TrayY", "COM PORT",  AnsiString("COM18"));
    sTempDynamicComPort =CheckAndReadIniDataGeneral("TempCtrl",        "COM_PORT_DYNAMIC", AnsiString("COM6"));
    sRTCComPort         =CheckAndReadIniDataGeneral("RealTimeCCD",     "Port",             AnsiString("COM3"));

    asLASER_COM[0]      =CheckAndReadIniDataGeneral("Laser",        "COM_Laser_1",     AnsiString("COM16"));//Steven 20140228 : 雷射測距功能
    asLASER_COM[1]      =CheckAndReadIniDataGeneral("Laser",        "COM_Laser_2",     AnsiString("COM17"));//Steven 20140228 : 雷射測距功能
    asLASER_COM[2]      =CheckAndReadIniDataGeneral("Laser",        "COM_Laser_InArm", AnsiString("COM18"));//Steven 20140228 : 雷射測距功能

    sFinePitchComPort   =CheckAndReadIniDataGeneral("FinePitch",       "COM_PORT",           AnsiString("COM3"));
    sFinePitchAdjustmentComPort =CheckAndReadIniDataGeneral("FinePitch",       "COM_PORT_Adjustment",   AnsiString("COM3"));

    if(sNumberPanelComPort!="" && sNumberPanelComPort.Pos("COM")!=1)
    {
        sNumberPanelComPort="COM"+sNumberPanelComPort;
    }
    if(sNumberPanelComPort2!="" && sNumberPanelComPort2.Pos("COM")!=1)
    {
        sNumberPanelComPort2="COM"+sNumberPanelComPort2;
    }
    if(sTorqueComPort!="" && sTorqueComPort.Pos("COM")!=1)
    {
        sTorqueComPort="COM"+sTorqueComPort;
    }
    if(sTempComPort!="" && sTempComPort.Pos("COM")!=1)
    {
        sTempComPort="COM"+sTempComPort;
    }
    if(sTempOmronComPort!="" && sTempOmronComPort.Pos("COM")!=1)
    {
        sTempOmronComPort="COM"+sTempOmronComPort;
    }
    if(sRTCComPort!="" && sRTCComPort.Pos("COM")!=1)
    {
        sRTCComPort="COM"+sRTCComPort;
    }
    if(sTempDynamicComPort!="" && sTempDynamicComPort.Pos("COM")!=1)
    {
        sTempDynamicComPort="COM"+sTempDynamicComPort;
    }

    WEIGHT_CALIBRATION  =CheckAndReadIniDataGeneral("System",    "WEIGHT_CALIBRATION",      0);     //Steven 20111108
    //Shuttle Sensor-----------------------------
    SHUTTLE_SENSOR_TYPE=CheckAndReadIniDataGeneral("System", "SHUTTLE_SENSOR_TYPE", 0);
    NUEC_TYPE=CheckAndReadIniDataGeneral("System", "NUEC_TYPE", 0);             //Sam 20230707 : EtherCAT Shuttle sensor
    ENABLE_OUT_SHUTTLE_SENEOR=CheckAndReadIniDataGeneral("System", "ENABLE_OUT_SHUTTLE_SENEOR", true);      //Steven 20120531 : by pass out shuttle sensor
    ENABLE_OUT_SHUTTLEY_LATCH=CheckAndReadIniDataGeneral("System", "ENABLE_OUT_SHUTTLEY_LATCH", false);     //Steven 20170518 (jou) : Out shuttle Y sensor 使用 LATCH
    Use_AxisY_Sensor_2x3mode =CheckAndReadIniDataGeneral("System", "Use_AxisY_Sensor_2x3mode", false);      //Isaac 20180402 (Steven) 2x3 mode 用Y sensor 偵測Outshuttle殘料
    Bias_Mode_Use_Y_Sensor   =CheckAndReadIniDataGeneral("System", "Bias_Mode_Use_Y_Sensor", false);        //Steven 20240805 : 1x2 Bias mode 用Y sensor 偵測Outshuttle殘料

    if(MachineTypeChoice==Type_HT9046 ||
       MachineTypeChoice==Type_HT9046_LS ||
       MachineTypeChoice==Type_HT1032)
    {
        ENABLE_OUT_SHUTTLE_SENEOR=false;
    }

    if(MachineTypeChoice==Type_HT9046_LS ||                                     //Steven 20170518 (jou) : Out shuttle Y sensor 使用 LATCH
       MachineTypeChoice==Type_HT1032)
    {
        ENABLE_OUT_SHUTTLEY_LATCH=true;
    }
    USE_RFID_READER=CheckAndReadIniDataGeneral("System", "USE_RFID_READER", 0); //Steven 20220713 : RFID Reader for SJSEMI

    //安全門----------------------------------
    SAFE_DOOR_AMOUNT=CheckAndReadIniDataGeneral("System", "SAFE_DOOR_AMOUNT", 2);

    //敲敲Tray--------------------------------
    TRAY_VIBRATION=CheckAndReadIniDataGeneral("System", "TRAY_VIBRATION", 0);   //JerryYang 20170531 (wei) bool->int
    TRAY_ARM_MODE =CheckAndReadIniDataGeneral("System", "TRAY_ARM_MODE", 0);    //Frank 20230419

    USE_LOADER_VIBRATION=CheckAndReadIniDataGeneral("System", "LOADER_VIBRATION", false);  //JerryYang 20191001 loader震動馬達

    USE_TRAY_ROBOT=CheckAndReadIniDataGeneral("System", "USE_TRAY_ROBOT", 0);           //Steven 20170330 (Wei) : For HT-9046LM
    USE_LOADER_HINGE=CheckAndReadIniDataGeneral("System", "USE_LOADER_HINGE",   0);     //Steven 20170330 (Wei) : For TSMC

    //Ifor 20150708 : CKD FCM Clean Air--------------------------------
    USE_CKD_FCM_CleanAir=CheckAndReadIniDataGeneral("System", "USE_CKD_FCM_CleanAir", false);

    if(USE_AUTO_RETEST==eartInstall)                                            //wei 20150807 Catch Tray Model
    {
        USE_CATCH_TRAY_MODEL=CheckAndReadIniDataGeneral("System", "USE_CATCH_TRAY_MODEL", 2);
    }
    else
    {
        USE_CATCH_TRAY_MODEL=CheckAndReadIniDataGeneral("System", "USE_CATCH_TRAY_MODEL", 0);
    }
    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
        bNewCatchTrayblock = CheckAndReadIniDataGeneral("System" , "UseCatchTrayBlock"   , 1);         //kevin 20200512 夾tray遮版削短
    else
        bNewCatchTrayblock = CheckAndReadIniDataGeneral("System" , "UseCatchTrayBlock"   , 0);         //kevin 20200512 夾tray遮版削短

    USE_16_HEATER=CheckAndReadIniDataGeneral("System", "USE_16_HEATER", eht4Heater); //Steven 20111011 : 使用16個Heater
    if(USE_16_HEATER==eht16HeaterEJ1N)                                          //Steven 20200318 : 修正EJ1N讀取異常的問題
        EJ1N_Count=4;
    else if(USE_16_HEATER==eht32HeaterEJ1N)
        EJ1N_Count=8;
    else
        EJ1N_Count=0;

    //Real Time CCD--------------------------------
    REAL_TIME_CCD   =CheckAndReadIniDataGeneral("System", "REAL_TIME_CCD", false);
    RTC_TemperNumber=CheckAndReadIniDataGeneral("System", "RTC_TemperNumber", 1);       //Isaac 20201217 : RTC CCD增加第二組感溫
    CCD2_TEMPER     =CheckAndReadIniDataGeneral("System", "CCD2_TEMPER", false);
    LB_TEMP         =CheckAndReadIniDataGeneral("System", "LB_TEMP", false);        //Steven 20181023 : LB溫度
    LB_TEMP_UpDown  =CheckAndReadIniDataGeneral("System", "LB_TEMP_UpDown", false); //Frank 20241231 : add
    Index_ESDAir    =CheckAndReadIniDataGeneral("System", "Index_ESDAir", false);   //kevin 20200207 add index ESD temp

    //OCR------------------------------------------
    INSTALL_OCR=CheckAndReadIniDataGeneral("System", "INSTALL_OCR",  eocrUninstal); //Steven 20120716 : OCR
    asOCRComPort=CheckAndReadIniDataGeneral("OCR",   "OCR_COM_PORT", AnsiString("COM18"));
    asOCRwithTesterComPort=CheckAndReadIniDataGeneral("OCR",   "OCRwithTester_COM_PORT", AnsiString("COM19"));

    INSTALL_OCR_YMot=CheckAndReadIniDataGeneral("System", "INSTALL_OCR_YMot",  eocrYMotUninstal);   //Frank 20250214 add

    SAFE_DOOR_LOCK  =CheckAndReadIniDataGeneral("System", "SAFE_DOOR_LOCK", false);
    CLEAN_AIR       =CheckAndReadIniDataGeneral("IndexDriver", "CLEAN_AIR", false); //ChungHung 20120821 : Clean Air
//    HighTemperatureSet150=CheckAndReadIniDataGeneral("System",  "HighTemperatureSet150", false); //wei 20150617 改機最高溫150度
//    HighTemperatureSet155=CheckAndReadIniDataGeneral("System",  "HighTemperatureSet155", false); //Sam 20240118 新增 155度 模式
//    HighTemperatureSet175=CheckAndReadIniDataGeneral("System",  "HighTemperatureSet175", false); //JerryYang 20160810 改機最高溫175度

    int iTemp175=CheckAndReadIniDataGeneral("System", "HighTemperatureSet175", 0);          //Frank 20160705 add 改機最高溫175度
    int iTemp155=CheckAndReadIniDataGeneral("System", "HighTemperatureSet155", 0);          //Sam 20240118 新增 155度 模式
    int iTemp150=CheckAndReadIniDataGeneral("System", "HighTemperatureSet150", 0);          //wei 20150617 改機最高溫150度

    if(iTemp175!=0)                                                             //Steven 20250701 : 整合機台溫度設定
    {
        iTempLimitation=CheckAndReadIniDataGeneral("System", "HighTempLimit", tTemp175);
        iTempLimitation=tTemp175;
    }
    else if(iTemp155!=0)
    {
        iTempLimitation=CheckAndReadIniDataGeneral("System", "HighTempLimit", tTemp155);
        iTempLimitation=tTemp155;
    }
    else if(iTemp150!=0)
    {
        iTempLimitation=CheckAndReadIniDataGeneral("System", "HighTempLimit", tTemp150);
        iTempLimitation=tTemp150;
    }
    else
    {
        iTempLimitation=CheckAndReadIniDataGeneral("System", "HighTempLimit", tTemp130);
    }

    DewPoint_Hardware_Install=CheckAndReadIniDataGeneral("System", "DewPoint_Hardware_Install", 0);    //Steven 20191017 : 露點計
//    USE_FINGER_PRINT     =CheckAndReadIniDataGeneral("System",  "USE_FINGER_PRINT", 0);//Steven 20190503 : 指紋辨識權限
    USE_FINGER_PRINT=0;                                                         //Steven 20240920 : 移除指紋辨識
    MAGAZINE_BIN_DISP_TYPE=CheckAndReadIniDataGeneral("System", "MAGAZINE_BIN_DISP_TYPE", 0);   //JerryYang 20220909 : add magazine

    //ATC
    ATC_SYSTEM                  =CheckAndReadIniDataGeneral("ATC", "USE_ATC_MODE",  eATCUninstall);   //jou 2012-03-08 Enable ATC mode;
    asATC1ComPort               =CheckAndReadIniDataGeneral("ATC", "ATC1_COM_PORT", AnsiString("COM15"));
    asATC2ComPort               =CheckAndReadIniDataGeneral("ATC", "ATC2_COM_PORT", AnsiString("COM16"));
    asATC3ComPort               =CheckAndReadIniDataGeneral("ATC", "ATC3_COM_PORT", AnsiString("COM17"));
    asATC4ComPort               =CheckAndReadIniDataGeneral("ATC", "ATC4_COM_PORT", AnsiString("COM18"));
    asATCSYSTEMIP               =CheckAndReadIniDataGeneral("ATC", "ATC_SYSTEM_IP", AnsiString("172.16.8.90"));
    asATCSYSTEMPORT             =CheckAndReadIniDataGeneral("ATC", "ATC_SYSTEM_PORT", 1234);
    asATCSYSTEMUSEHEAT          =CheckAndReadIniDataGeneral("ATC", "ATC_SYSTEM_USEHEAT", 4);    //Ifor 20160506 :add New ATC Interface Use Heat Count

    if(ATC_SYSTEM==eATCHonPrecType)
        iATC_Use_Heat_Count     =4;
    else
        iATC_Use_Heat_Count     =asATCSYSTEMUSEHEAT;                            //Ifor 20160506 add ATC Heat Count
    bUseATC_SelfTestFunction    =CheckAndReadIniDataGeneral("ATC", "USE_ATC_SELFTEST", 0);           //Ifor 20170120 (Steven) : Add ATC Self Test Function Switch (僅由General.ini 修改)
    INDEX_PRESS_TYPE            =(eIndexPressType)CheckAndReadIniDataGeneral("System",       "INDEX_PRESS_TYPE",         0);    //Steven 20110310 : 240KG
    USE_IN_OUT_ARM_Y_PITCH      =CheckAndReadIniDataGeneral("System", "USE_IN_OUT_ARM_Y_PITCH", iXPitch60);                     //jou 2012-05-15 : 選擇 Y Pitch 機構模式
    USE_OUT_ARM_Y_PITCH         =CheckAndReadIniDataGeneral("System", "USE_OUT_ARM_Y_PITCH", USE_IN_OUT_ARM_Y_PITCH);  //JerryYang 20251218 : IN/OUT ARM支援不同模組

    IN_OUT_ARM_Y_PITCH_MIN      =CheckAndReadIniDataGeneral("System", "IN_OUT_ARM_Y_PITCH_MIN", 1500);
    IN_OUT_ARM_Y_PITCH_MAX      =CheckAndReadIniDataGeneral("System", "IN_OUT_ARM_Y_PITCH_MAX", 7500);
    BASE_X_TO_HP                =CheckAndReadIniDataGeneral("System", "BASE_X_TO_HP",           6800);                          //Steven 20230826 : 基準軸X到加熱盤邊緣的距離
    bUserDefMaxContactHeight    =CheckAndReadIniDataGeneral("Index", "EnableUserDefMaxContactHeight",   false);                 //Steven 20180412 : For new HT-9045高度較高
    dUserDefMaxContactHeight    =CheckAndReadIniDataGeneral("Index", "UserDefMaxContactHeight",   -135.0);                      //Steven 20180412 : For new HT-9045高度較高

    USE_OUT_SORT_ARM            =CheckAndReadIniDataGeneral("OutSortArm", "USE_OUT_SORT_ARM", eartUninstall);                   //Steven 20240822 : For HT-9046AU
    USE_OUT_SORT_X_PITCH_MIN    =CheckAndReadIniDataGeneral("OutSortArm", "USE_OUT_SORT_X_PITCH_MIN", 1500);
    USE_OUT_SORT_X_PITCH_MAX    =CheckAndReadIniDataGeneral("OutSortArm", "USE_OUT_SORT_X_PITCH_MAX", 4000);

    USE_IN_Y_IS_AUTO_PITCH      =(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||   //Steven 20230602 : 統一Y可變距的Flag  //JerryYang 20251218 : IN/OUT ARM支援不同模組
                                  USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||
                                  USE_IN_OUT_ARM_Y_PITCH==iXYPitchBb       ||
                                  USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be  ||   //Steven for HT7080 //Ztex 2023.12.06 Add HT-1032
                                  USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc);//Ztex 2024.02.24 Add HT-1132

    USE_OUT_Y_IS_AUTO_PITCH     =(USE_OUT_ARM_Y_PITCH==iXYPitchVariable ||   //Steven 20230602 : 統一Y可變距的Flag  //JerryYang 20251218 : IN/OUT ARM支援不同模組
                                  USE_OUT_ARM_Y_PITCH==iXYPitch16Picker ||
                                  USE_OUT_ARM_Y_PITCH==iXYPitchBb       ||
                                  USE_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be  ||   //Steven for HT7080 //Ztex 2023.12.06 Add HT-1032
                                  USE_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc);//Ztex 2024.02.24 Add HT-1132

    USE_16PICKER_TYPE           =(USE_PICKER_COUNT==ep16Picker ||
                                  USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)?1:0; //Ztex 2023.12.06 Add HT-1032

    #ifdef FOR_QLE
        if(CUSTOMER_CODE==CC_SPIL_CHINA_SUZHOU)
        {
            SPIL_FOR_QLE=1;
            WriteIniDataGeneral("System", "SPIL_FOR_QLE",  SPIL_FOR_QLE);       //Steven 20230110 : For渠梁
        }
    #else
        if(CUSTOMER_CODE==CC_SPIL_CHINA_SUZHOU)
            SPIL_FOR_QLE        =CheckAndReadIniDataGeneral("System", "SPIL_FOR_QLE", 0);
        else
            SPIL_FOR_QLE        =0;
    #endif

    if(SPIL_FOR_QLE==1 && ATC_SYSTEM==eATCUninstall && MachName=="9045GPIB")    //Steven 20230110 : index安全高度針對渠梁有Chamber的9045做預設開啟
    {
        bUser_Define_IndexZ_SafePos =CheckAndReadIniDataGeneral("Index", "EnableUser_Define_IndexZ_SafePos",   1);                  //Richard 20230107 : SPIL客戶自定義機台安裝高度(200~800間)
        dUserDefineIndexZSafePos    =CheckAndReadIniDataGeneral("Index", "UserDefineIndexZSafePos",   800);                         //Richard 20230107 : SPIL客戶自定義機台安裝高度(200~800間)
    }
    else
    {
        bUser_Define_IndexZ_SafePos =CheckAndReadIniDataGeneral("Index", "EnableUser_Define_IndexZ_SafePos",   false);              //Richard 20230107 : SPIL客戶自定義機台安裝高度(200~800間)
        dUserDefineIndexZSafePos    =CheckAndReadIniDataGeneral("Index", "UserDefineIndexZSafePos",   200);                         //Richard 20230107 : SPIL客戶自定義機台安裝高度(200~800間)
    }

    if(dUserDefineIndexZSafePos<200)
        dUserDefineIndexZSafePos=200;
    else if(dUserDefineIndexZSafePos>800)
        dUserDefineIndexZSafePos=800;

    USE_PICKER_COUNT            =CheckAndReadIniDataGeneral("System",       "USE_PICKER_COUNT",     1); //Steven 20161117 : for HT-9045S

    LOAD_Z_USE_MOTOR[0]=CheckAndReadIniDataGeneral("TrayZ", "LOAD_Z_USE_MOTOR", false);                 //Steven 20190813 : 入Tray改用步進馬達
    LOAD_Z_USE_MOTOR[1]=CheckAndReadIniDataGeneral("TrayZ", "EMPTY_Z_USE_MOTOR", false);
    LOAD_Z_USE_MOTOR[2]=CheckAndReadIniDataGeneral("TrayZ", "COLOR_Z_USE_MOTOR", false);
    LOAD_Z_USE_MOTOR[3]=CheckAndReadIniDataGeneral("TrayZ", "AUTO1_Z_USE_MOTOR", false);
    LOAD_Z_USE_MOTOR[4]=CheckAndReadIniDataGeneral("TrayZ", "AUTO2_Z_USE_MOTOR", false);
    LOAD_Z_USE_MOTOR[5]=CheckAndReadIniDataGeneral("TrayZ", "AUTO3_Z_USE_MOTOR", false);
    LOAD_Z_USE_MOTOR[6]=CheckAndReadIniDataGeneral("TrayZ", "AUTO4_Z_USE_MOTOR", false);                //Steven 20230907 : For HT-9011UC
    LOAD_Z_USE_MOTOR[7]=CheckAndReadIniDataGeneral("TrayZ", "AUTO5_Z_USE_MOTOR", false);
    LOAD_Z_USE_MOTOR[8]=CheckAndReadIniDataGeneral("TrayZ", "AUTO6_Z_USE_MOTOR", false);
    LOAD_Y_USE_MOTOR[0]=(CheckAndReadIniDataGeneral("TrayY", "LOAD_Y_USE_MOTOR", false) || (INSTALL_OCR>eocrUninstal));         //Jimmychiu 20240307 : Loader Tray改用步進馬達
    LOAD_Y_USE_MOTOR[1]=false;
    LOAD_Y_USE_MOTOR[2]=false;
    LOAD_Y_USE_MOTOR[3]=false;
    LOAD_Y_USE_MOTOR[4]=false;
    LOAD_Y_USE_MOTOR[5]=false;

    LOADUNLOAD_USE_CASSETTE[0]=CheckAndReadIniDataGeneral("TrayCassette", "LOAD_USE_Cassette",  false);
    LOADUNLOAD_USE_CASSETTE[1]=CheckAndReadIniDataGeneral("TrayCassette", "EMPTY_USE_Cassette", false);
    LOADUNLOAD_USE_CASSETTE[2]=CheckAndReadIniDataGeneral("TrayCassette", "COLOR_USE_Cassette", false);
    LOADUNLOAD_USE_CASSETTE[3]=CheckAndReadIniDataGeneral("TrayCassette", "AUTO1_USE_Cassette", false);
    LOADUNLOAD_USE_CASSETTE[4]=CheckAndReadIniDataGeneral("TrayCassette", "AUTO2_USE_Cassette", false);
    LOADUNLOAD_USE_CASSETTE[5]=CheckAndReadIniDataGeneral("TrayCassette", "AUTO3_USE_Cassette", false);
    LOADUNLOAD_USE_CASSETTE[6]=CheckAndReadIniDataGeneral("TrayCassette", "AUTO4_USE_Cassette", false); //Steven 20230907 : For HT-9011UC
    LOADUNLOAD_USE_CASSETTE[7]=CheckAndReadIniDataGeneral("TrayCassette", "AUTO5_USE_Cassette", false);
    LOADUNLOAD_USE_CASSETTE[8]=CheckAndReadIniDataGeneral("TrayCassette", "AUTO6_USE_Cassette", false);

    sIndEPCnt=CheckAndReadIniDataGeneral("System", "Individual_EP_COUNT", AnsiString("16"));

    if(sIndEPCnt=="16")
    {
        iIndEPCnt=16;
    }
    else
    {
        iIndEPCnt=4;                                                            //JerryYang 20210413 : Add 4組獨立EP版本
    }

    if(USE_PICKER_COUNT==ep1Picker)
    {
        iInArmXBase=2;
        iInArmYBase=0;
        iOutArmXBase=2;
        iOutArmYBase=0;

        IN_OUT_ARM_Y_PITCH_MIN=1500;
        IN_OUT_ARM_Y_PITCH_MAX=7500;

        iPitchY_Max_minus_Min=IN_OUT_ARM_Y_PITCH_MAX-IN_OUT_ARM_Y_PITCH_MIN;

        iInArmShtXCenterPos=-2000;
        iInArmShtYCenterPos=-3000;
        iOutArmShtXCenterPos=-2000;
        iOutArmShtYCenterPos=-3000;
    }
    else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)                           //Steven 20141029 : XY-Pitch
    {
        iInArmXBase=2;
        iInArmYBase=1;
        iOutArmXBase=1;
        iOutArmYBase=1;
        if(IN_OUT_ARM_Y_PITCH_MIN>IN_OUT_ARM_Y_PITCH_MAX ||
           IN_OUT_ARM_Y_PITCH_MIN<1500 ||
           IN_OUT_ARM_Y_PITCH_MIN>5000)
            IN_OUT_ARM_Y_PITCH_MIN=1500;

        if(IN_OUT_ARM_Y_PITCH_MAX<IN_OUT_ARM_Y_PITCH_MIN ||
           IN_OUT_ARM_Y_PITCH_MAX>8000 ||
           IN_OUT_ARM_Y_PITCH_MAX<5000)
            IN_OUT_ARM_Y_PITCH_MAX=8000;                                        //kevin 20190322 change
        iPitchY_Max_minus_Min=IN_OUT_ARM_Y_PITCH_MAX-IN_OUT_ARM_Y_PITCH_MIN;    //Jimmychiu 20221205 add y pitch minus
        iInArmShtXCenterPos=-2000;
        iInArmShtYCenterPos=3000;
        iOutArmShtXCenterPos=2000;
        iOutArmShtYCenterPos=3000;
    }
    else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchRowA)                               //Steven 20170424 (wei) : new XY變距
    {
        iInArmXBase=2;
        iInArmYBase=0;
        iOutArmXBase=1;
        iOutArmYBase=0;

        iInArmShtXCenterPos=-2000;
        iInArmShtYCenterPos=3000;
        iOutArmShtXCenterPos=2000;
        iOutArmShtYCenterPos=3000;
    }
    else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker)
    {
        if(IN_OUT_ARM_Y_PITCH_MIN>IN_OUT_ARM_Y_PITCH_MAX ||
           IN_OUT_ARM_Y_PITCH_MIN<2500 ||
           IN_OUT_ARM_Y_PITCH_MIN>5000)
            IN_OUT_ARM_Y_PITCH_MIN=2500;

        if(IN_OUT_ARM_Y_PITCH_MAX<IN_OUT_ARM_Y_PITCH_MIN ||
           IN_OUT_ARM_Y_PITCH_MAX>6500 ||
           IN_OUT_ARM_Y_PITCH_MAX<5000)
            IN_OUT_ARM_Y_PITCH_MAX=6500;

        iPitchY_Max_minus_Min=IN_OUT_ARM_Y_PITCH_MAX-IN_OUT_ARM_Y_PITCH_MIN;    //Jimmychiu 20221205 add y pitch minus

        iInArmXBase=3;
        iInArmYBase=1;
        iOutArmXBase=3;
        iOutArmYBase=1;
        iInArmShtXCenterPos =1100;
        iInArmShtYCenterPos =3000;
        iOutArmShtXCenterPos=1100;
        iOutArmShtYCenterPos=3000;
    }
    else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchBb)                                 //Steven for HT7080
    {
        if(IN_OUT_ARM_Y_PITCH_MIN>IN_OUT_ARM_Y_PITCH_MAX ||
           IN_OUT_ARM_Y_PITCH_MIN<1900 ||
           IN_OUT_ARM_Y_PITCH_MIN>5000)
            IN_OUT_ARM_Y_PITCH_MIN=2000;

        if(IN_OUT_ARM_Y_PITCH_MAX<IN_OUT_ARM_Y_PITCH_MIN ||
           IN_OUT_ARM_Y_PITCH_MAX>6500 ||
           IN_OUT_ARM_Y_PITCH_MAX<5000)
            IN_OUT_ARM_Y_PITCH_MAX=6000;

        iPitchY_Max_minus_Min=IN_OUT_ARM_Y_PITCH_MAX-IN_OUT_ARM_Y_PITCH_MIN;    //Jimmychiu 20221205 add y pitch minus

        iInArmXBase=1;
        iInArmYBase=1;
        iOutArmXBase=2;
        iOutArmYBase=1;
        iInArmShtXCenterPos =2000;
        iInArmShtYCenterPos =3000;
        iOutArmShtXCenterPos=-2000;
        iOutArmShtYCenterPos=3000;
    }
    else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                            //Ztex 2023.12.06 Add HT-1032
    {
        if(IN_OUT_ARM_Y_PITCH_MIN>IN_OUT_ARM_Y_PITCH_MAX ||
           IN_OUT_ARM_Y_PITCH_MIN<2500 ||
           IN_OUT_ARM_Y_PITCH_MIN>5000)
            IN_OUT_ARM_Y_PITCH_MIN=2500;

        if(IN_OUT_ARM_Y_PITCH_MAX<IN_OUT_ARM_Y_PITCH_MIN ||
           IN_OUT_ARM_Y_PITCH_MAX>6500 ||
           IN_OUT_ARM_Y_PITCH_MAX<5000)
            IN_OUT_ARM_Y_PITCH_MAX=6500;

        iPitchY_Max_minus_Min=IN_OUT_ARM_Y_PITCH_MAX-IN_OUT_ARM_Y_PITCH_MIN;    //Jimmychiu 20221205 add y pitch minus

        iInArmXBase=3;
        iInArmYBase=1;
        iOutArmXBase=3;
        iOutArmYBase=1;
        iInArmShtXCenterPos =1100;
        iInArmShtYCenterPos =3000;
        iOutArmShtXCenterPos=1100;
        iOutArmShtYCenterPos=3000;
    }
    else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                       //Ztex 2024.02.24 Add HT-1132
    {
        if(IN_OUT_ARM_Y_PITCH_MIN>IN_OUT_ARM_Y_PITCH_MAX ||
           IN_OUT_ARM_Y_PITCH_MIN<2500 ||
           IN_OUT_ARM_Y_PITCH_MIN>5000)
            IN_OUT_ARM_Y_PITCH_MIN=2500;

        if(IN_OUT_ARM_Y_PITCH_MAX<IN_OUT_ARM_Y_PITCH_MIN ||
           IN_OUT_ARM_Y_PITCH_MAX>6500 ||
           IN_OUT_ARM_Y_PITCH_MAX<5000)
            IN_OUT_ARM_Y_PITCH_MAX=6500;

        iPitchY_Max_minus_Min=IN_OUT_ARM_Y_PITCH_MAX-IN_OUT_ARM_Y_PITCH_MIN;    //Jimmychiu 20221205 add y pitch minus

        iInArmXBase=1;
        iInArmYBase=1;
        iOutArmXBase=2;
        iOutArmYBase=1;
        iInArmShtXCenterPos =1100;
        iInArmShtYCenterPos =3250;
        iOutArmShtXCenterPos=1100;
        iOutArmShtYCenterPos=3250;
    }
    else
    {
        IN_OUT_ARM_Y_PITCH_MIN=0;
    }

    if(USE_OUT_ARM_Y_PITCH==iXPitch60 || USE_OUT_ARM_Y_PITCH==iXPitchManual635)     //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        iOutArmXBase=2;
        iOutArmYBase=0;

        iOutArmShtXCenterPos=-2000;
        iOutArmShtYCenterPos=-3000;
    }

    if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||
       USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be  ||                              //Ztex 2023.12.06 Add HT-1032
       iInArmXBase==2)
    {
        if(BASE_X_TO_HP<6800)                                                   //Steven 20230826 : 基準軸X到加熱盤邊緣的距離
            BASE_X_TO_HP=6800;                                                  // >7000 : HT-9xxx, HT-1032AT
    }
    else
    {
        if(BASE_X_TO_HP>4000)                                                   // <0    : HT1040, HT-7080B
            BASE_X_TO_HP=4000;                                                  // <4000 : HT1028
    }

    //Isaac 20171204 (Steven) : Xpitch40->50mm 可選擇X Pitch機構模式
    //==>
    USE_IN_OUT_ARM_X_PITCH      =CheckAndReadIniDataGeneral("System", "USE_IN_OUT_ARM_X_PITCH", iXPitch40mm);
    if(USE_IN_OUT_ARM_X_PITCH==iXPitch40mm)
    {
        IN_OUT_ARM_X_PITCH_MIN=CheckAndReadIniDataGeneral("System", "IN_OUT_ARM_X_PITCH_MIN", 4000);
        IN_OUT_ARM_X_PITCH_MAX=CheckAndReadIniDataGeneral("System", "IN_OUT_ARM_X_PITCH_MAX", 12000);
        iXpitchMin          =1333;                                              //Steven 20180522 : X-Pitch最小值
        iXpitchMinX2        =2666;
        iXpitchMinX3        =4000;
        iXpitchMax          =4000;
        iXpitchMaxX2        =8000;
        iXpitchMaxX3        =12000;
        iPitch_Max_minus_Min=iXpitchMaxX3-iXpitchMinX3;
    }
    else if(USE_IN_OUT_ARM_X_PITCH==iXPitch50mm)
    {
        IN_OUT_ARM_X_PITCH_MIN=CheckAndReadIniDataGeneral("System", "IN_OUT_ARM_X_PITCH_MIN", 4000);
        IN_OUT_ARM_X_PITCH_MAX=CheckAndReadIniDataGeneral("System", "IN_OUT_ARM_X_PITCH_MAX", 15000);
        iXpitchMin          =1333;                                              //Steven 20180522 : X-Pitch最小值
        iXpitchMinX2        =2666;
        iXpitchMinX3        =4000;
        iXpitchMax          =5000;
        iXpitchMaxX2        =10000;
        iXpitchMaxX3        =15000;
        iPitch_Max_minus_Min=iXpitchMaxX3-iXpitchMinX3;
    }
    else if(USE_IN_OUT_ARM_X_PITCH==iXPitch16Pick)
    {
        IN_OUT_ARM_X_PITCH_MIN=CheckAndReadIniDataGeneral("System", "IN_OUT_ARM_X_PITCH_MIN", 7700);
        IN_OUT_ARM_X_PITCH_MAX=CheckAndReadIniDataGeneral("System", "IN_OUT_ARM_X_PITCH_MAX", 15400);
        iXpitchMin          =1100;                                              //Steven 20180522 : X-Pitch最小值
        iXpitchMinX2        =2200;
        iXpitchMinX3        =3300;
        iXpitchMinX4        =4400;
        iXpitchMinX5        =5500;
        iXpitchMinX6        =6600;
        iXpitchMinX7        =7700;
        iXpitchMax          =2200;
        iXpitchMaxX2        =4400;
        iXpitchMaxX3        =6600;
        iXpitchMaxX4        =8800;
        iXpitchMaxX5        =11000;
        iXpitchMaxX6        =13200;
        iXpitchMaxX7        =15400;
        iPitch_Max_minus_Min=iXpitchMaxX7-iXpitchMinX7;
    }
    else   //JimmyChiu 20220708 : add Auto X pitch
    {
        IN_OUT_ARM_X_PITCH_MIN=CheckAndReadIniDataGeneral("System", "IN_OUT_ARM_X_PITCH_MIN", 6000); //JimmyChiu 20220708 : add Auto X pitch
        IN_OUT_ARM_X_PITCH_MAX=CheckAndReadIniDataGeneral("System", "IN_OUT_ARM_X_PITCH_MAX", 12000);//JimmyChiu 20220708 : add Auto X pitch
        if(IN_OUT_ARM_X_PITCH_MIN>IN_OUT_ARM_X_PITCH_MAX)
        {
            int itemp=IN_OUT_ARM_X_PITCH_MIN;
            IN_OUT_ARM_X_PITCH_MIN=IN_OUT_ARM_X_PITCH_MAX;
            IN_OUT_ARM_X_PITCH_MAX=itemp;
        }
        iXpitchMin          =IN_OUT_ARM_X_PITCH_MIN/3;                          //Steven 20180522 : X-Pitch最小值
        iXpitchMinX2        =double(IN_OUT_ARM_X_PITCH_MIN)/3.0*2.0;
        iXpitchMinX3        =IN_OUT_ARM_X_PITCH_MIN;
        iXpitchMax          =IN_OUT_ARM_X_PITCH_MAX/3;
        iXpitchMaxX2        =double(IN_OUT_ARM_X_PITCH_MAX)/3.0*2.0;
        iXpitchMaxX3        =IN_OUT_ARM_X_PITCH_MAX;
        iPitch_Max_minus_Min=iXpitchMaxX3-iXpitchMinX3;
    }

    iXpitchMaxX1_MM=iXpitchMax/100.0;
    iXpitchMaxX2_MM=iXpitchMaxX2/100.0;
    iXpitchMaxX3_MM=iXpitchMaxX3/100.0;
    iXpitchMaxX4_MM=iXpitchMaxX4/100.0;
    iXpitchMaxX5_MM=iXpitchMaxX5/100.0;
    iXpitchMaxX6_MM=iXpitchMaxX6/100.0;
    iXpitchMaxX7_MM=iXpitchMaxX7/100.0;
    iXpitchMinX1_MM=iXpitchMin/100.0;
    iXpitchMinX2_MM=iXpitchMinX2/100.0;
    iXpitchMinX3_MM=iXpitchMinX3/100.0;
    iXpitchMinX4_MM=iXpitchMinX4/100.0;
    iXpitchMinX5_MM=iXpitchMinX5/100.0;
    iXpitchMinX6_MM=iXpitchMinX6/100.0;
    iXpitchMinX7_MM=iXpitchMinX7/100.0;
    //<==
    //Isaac 20171204 (Steven) : Xpitch40->50mm 可選擇X Pitch機構模式

    AnsiString sVer=CheckAndReadIniDataGeneral("Version", "Ver", AnsiString(""));

    #ifdef ASE_KaohSiung
    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20191225 (wei) 版本記錄提供給客戶遠端抓取
    {
        #ifdef HiSilicon
            if(MACHINE_HAS_AUTO_ALIGNMENT_CCD &&
               TestIF.bEnableAutoAlignment==true)                               //KenHsieh 20211015 : ASE要求版號新增AOA
                sVer=AnsiString(HISI_VERSION)+AnsiString(ASE_AOA);
            else
                sVer=AnsiString(HISI_VERSION)+AnsiString(HISI_ASE);             //kevin 20180514
        #else                                                                   //kevin 20190820 取消HT9046 版號
             if(MACHINE_HAS_AUTO_ALIGNMENT_CCD &&
                TestIF.bEnableAutoAlignment==true)                              //KenHsieh 20211015 : ASE要求版號新增AOA
                sVer=AnsiString(MainVersion)+AnsiString(ASE_AOA);
            else
                sVer=AnsiString(MainVersion)+AnsiString(HISI_ASE);
        #endif
        WriteIniDataGeneral("Version", "Ver",       sVer);                      //2014-05-30    Dell    for ATC6.0
        WriteIniDataGeneral("Version", "DefineVer", AnsiString(Record_Ver));    //2014-05-30    Dell    for ATC6.0
    }
    #endif

    //------------------------------------------
    INSTALL_HEAT_GUN        =CheckAndReadIniDataGeneral("System",       "INSTALL_HEAT_GUN",     0); //kevin 20120523 : 選擇 AirGUN 機構模式
    INSTALL_ATC_HEAT_GUN    =CheckAndReadIniDataGeneral("System",       "INSTALL_ATC_HEAT_GUN", 0); //JerryYang 20220408 : add for ATC3.5
    USE_HOTPLATE_TYPE       =CheckAndReadIniDataGeneral("System",       "USE_HOTPLATE_TYPE",    0); //jou 2012-05-15 : 選擇 Hotplate Type
    USE_MAGNETIC_SCALE      =CheckAndReadIniDataGeneral("System",       "USE_MAGNETIC_SCALE",   0); //Steven 20160426 : 磁性尺
    USE_IO_CHANGE_TOQUE     =CheckAndReadIniDataGeneral("IndexDriver",  "USE_IO_CHANGE_TOQUE",  0); //jou 2012-06-21 Enable index I/O Change Toque
    MOTION_CARD_TYPE        =CheckAndReadIniDataGeneral("System",       "MOTION_CARD_TYPE",     0); //Brian 20121015 : 選擇Motion Card 模式
    MOTIONNET_SPEED         =CheckAndReadIniDataGeneral("System",       "MOTIONNET_SPEED",      COMMSPEED_20M); //Steven 20181122 : MNet速度要可以修改
    IO_CARD_TYPE            =CheckAndReadIniDataGeneral("System",       "IO_CARD_TYPE",         0); //Brian 20121015 : 選擇I/O Card 模式
    USE_ReadIndex_TOQUE     =CheckAndReadIniDataGeneral("IndexDriver",  "USE_ReadIndex_TOQUE",  0); //kevin 20201027
    USE_INDEX_ARM_AXES      =CheckAndReadIniDataGeneral("IndexDriver",  "USE_INDEX_ARM_AXES",   0); //JimmyChiu 20220708 : add Index Arm Axis

    if(IO_CARD_TYPE==NewIO_MN200 ||
       IO_CARD_TYPE==PCI_P64C64 )                                               //Steven 20231218 HT7080B
    {
        LoadIoData();
    }

    TTL_CARD_TYPE           =CheckAndReadIniDataGeneral("System",       "TTL_CARD_TYPE",        0); //Steven 20121122 : 選擇TTL Card 模式
    if(TTL_CARD_TYPE==2)        //Isaac 20210511 : TTLRS232板子版本檢查，1塊板子
    {
        TTLRS232VerCheck=7071601;      //07071601
    }
    TTL_CARD_USE_ADDRESS    =CheckAndReadIniDataGeneral("System",       "TTL_CARD_USE_ADDRESS",        0);  //Isaac 20210922 : 選擇TTL板子是否帶站別

    INSTALL_SOCKET_CLAMP    =CheckAndReadIniDataGeneral("System",       "INSTALL_SOCKET_CLAMP", 0);         //JerryYang 20160523 是否改機為分離式
    INSTALL_DOUBLE_EP       =CheckAndReadIniDataGeneral("System",       "INSTALL_DOUBLE_EP", 0);            //Ifor 20190104 : Add Double EP Control
    CHECK_EP_SETTING        =CheckAndReadIniDataGeneral("System",       "CHECK_EP_SETTING", 1);             //Steven 20240701 : EP檢查功能加上開關
    bAutoTrackCanGoRear     =CheckAndReadIniDataGeneral("System",       "bAutoTrackCanGoRear",   0);        //Isaac 20180109 (Steven) : auto123可前進後退
    bNoAutoZSelect          =CheckAndReadIniDataGeneral("System",       "bAutoZNoUseART",   0);             //kevin 20181023 add ART Auto 1 2 3  使用一段氣缸 k21 false use 2段氣缸
    IN_SHT_LAST_SENSOR      =CheckAndReadIniDataGeneral("System",       "IN_SHT_LAST_SENSOR",   0);         //Steven 20181203 : In Shuttle最後一個Sensor定義
    USE_PRECISER            =CheckAndReadIniDataGeneral("System",       "USE_PRECISER",         0);         //Frank 20180410 (Steven) : InArm Preciser Station
    USE_InPlacement         =CheckAndReadIniDataGeneral("System",       "USE_InPlacement", eartUninstall);  //JimmyChiu 20220908 add Pickup Error Placement
    iPreciserInstallArea    =CheckAndReadIniDataGeneral("System", "iPreciserInstallArea", 0);               //Ifor 20191008 : add Preciser Install Area
    //Barcode_2D
    BOTTOM_2DID             =CheckAndReadIniDataGeneral("2D_BarCode", "BOTTOM_2DID",       ebctUninstall);  //Steven 20190308 : Bottom 2D
    if(BOTTOM_2DID==1)
        BAR_CODE_INSTALL    =ebctUseCCDMode;
    else
        BAR_CODE_INSTALL    =CheckRange(CheckAndReadIniDataGeneral("2D_BarCode", "BAR_CODE_INSTALL", ebctUninstall), (int)ebctUninstall, (int)ebcUseOCR);       //Steven 20120921 : Barcode_2D //Ifor 20190129 : add Cognex EtherNet 通訊
    SHT_FLOATING_CHK        =CheckAndReadIniDataGeneral("2D_BarCode", "SHT_FLOATING_CHK",  ebctUninstall);  //Steven 20160920 : IC置偏檢查
    BOTTOM_2DID_CCD         =CheckAndReadIniDataGeneral("2D_BarCode", "BOTTOM_2DID_CCD",   ebctUninstall);  //KaiChen 20200513 : Bottom 2D 8CCD
    asBarCodeComPort[0]     =CheckAndReadIniDataGeneral("2D_BarCode",   "BarCode1_COM_PORT"     , AnsiString("COM15"));
    asBarCodeComPort[1]     =CheckAndReadIniDataGeneral("2D_BarCode",   "BarCode2_COM_PORT"     , AnsiString("COM16"));
    asBarCodeComPort[2]     =CheckAndReadIniDataGeneral("2D_BarCode",   "BarCode3_COM_PORT"     , AnsiString("COM17"));
    asBarCodeComPort[3]     =CheckAndReadIniDataGeneral("2D_BarCode",   "BarCode4_COM_PORT"     , AnsiString("COM18"));

//----------------------------------
    MACHINE_HAS_AUTO_ALIGNMENT_CCD  =CheckAndReadIniDataGeneral("System", "MACHINE_HAS_AUTO_ALIGNMENT_CCD", 0);  //ChungHung 20210113 add for Alignment CCD     //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
//----------------------------------

    BAR_CODE_USECOUNT       =CheckRange(CheckAndReadIniDataGeneral("2D_BarCode", "BAR_CODE_USECOUNT", 4), 2, 4);  //Ifor 20190129 :add COGNEX SYSTEM CCD Count //Steven 20190904 : 預設值改成4
    if(BAR_CODE_INSTALL==ebctEtherNetCCD)                                       //Ifor 20180104 : add Cognex EtherNet 通訊
    {
        asCCDBarCodeIP[0]       =CheckAndReadIniDataGeneral("2D_BarCode",   "Use_CCDShuttle_1A_IP"     , AnsiString("172.16.8.210"));   //Ifor 20151224 CCD 2D
        asCCDBarCodeIP[1]       =CheckAndReadIniDataGeneral("2D_BarCode",   "Use_CCDShuttle_1B_IP"     , AnsiString("172.16.8.211"));   //Ifor 20151224 CCD 2D
        asCCDBarCodeIP[2]       =CheckAndReadIniDataGeneral("2D_BarCode",   "Use_CCDShuttle_2A_IP"     , AnsiString("172.16.8.212"));   //Ifor 20151224 CCD 2D
        asCCDBarCodeIP[3]       =CheckAndReadIniDataGeneral("2D_BarCode",   "Use_CCDShuttle_2B_IP"     , AnsiString("172.16.8.213"));   //Ifor 20151224 CCD 2D

        asCCDBarCodePort[0]     =CheckAndReadIniDataGeneral("2D_BarCode",   "Use_CCDShuttle_1A_Port"   , AnsiString("5001"));           //Ifor 20151224 CCD 2D
        asCCDBarCodePort[1]     =CheckAndReadIniDataGeneral("2D_BarCode",   "Use_CCDShuttle_1B_Port"   , AnsiString("5001"));           //Ifor 20151224 CCD 2D
        asCCDBarCodePort[2]     =CheckAndReadIniDataGeneral("2D_BarCode",   "Use_CCDShuttle_2A_Port"   , AnsiString("5001"));           //Ifor 20151224 CCD 2D
        asCCDBarCodePort[3]     =CheckAndReadIniDataGeneral("2D_BarCode",   "Use_CCDShuttle_2B_Port"   , AnsiString("5001"));           //Ifor 20151224 CCD 2D
    }
    else
    {
        if(BAR_CODE_INSTALL==ebcUseOCR)                                         //Ifor 20210407 add: 自製OCR
        {
            asCCDBarCodeIP[0]   =CheckAndReadIniDataGeneral("2D_BarCode",   "Use_CCDShuttle_1A_IP"     , AnsiString("172.16.8.70"));    //Ifor 20210407 add: 自製OCR
            asCCDBarCodeIP[1]   =CheckAndReadIniDataGeneral("2D_BarCode",   "Use_CCDShuttle_1B_IP"     , AnsiString("172.16.8.70"));    //Ifor 20210407 add: 自製OCR
            asCCDBarCodeIP[2]   =CheckAndReadIniDataGeneral("2D_BarCode",   "Use_CCDShuttle_2A_IP"     , AnsiString("172.16.8.70"));    //Ifor 20210407 add: 自製OCR
            asCCDBarCodeIP[3]   =CheckAndReadIniDataGeneral("2D_BarCode",   "Use_CCDShuttle_2B_IP"     , AnsiString("172.16.8.70"));    //Ifor 20210407 add: 自製OCR
        }
        else
        {
            asCCDBarCodeIP[0]   =CheckAndReadIniDataGeneral("2D_BarCode",   "Use_CCDShuttle_1A_IP"     , AnsiString("172.16.8.200"));   //Ifor 20151224 CCD 2D
            asCCDBarCodeIP[1]   =CheckAndReadIniDataGeneral("2D_BarCode",   "Use_CCDShuttle_1B_IP"     , AnsiString("172.16.8.200"));   //Ifor 20151224 CCD 2D
            asCCDBarCodeIP[2]   =CheckAndReadIniDataGeneral("2D_BarCode",   "Use_CCDShuttle_2A_IP"     , AnsiString("172.16.8.200"));   //Ifor 20151224 CCD 2D
            asCCDBarCodeIP[3]   =CheckAndReadIniDataGeneral("2D_BarCode",   "Use_CCDShuttle_2B_IP"     , AnsiString("172.16.8.200"));   //Ifor 20151224 CCD 2D
        }
        asCCDBarCodePort[0]     =CheckAndReadIniDataGeneral("2D_BarCode",   "Use_CCDShuttle_1A_Port"   , AnsiString("5001"));           //Ifor 20151224 CCD 2D
        asCCDBarCodePort[1]     =CheckAndReadIniDataGeneral("2D_BarCode",   "Use_CCDShuttle_1B_Port"   , AnsiString("5002"));           //Ifor 20151224 CCD 2D
        asCCDBarCodePort[2]     =CheckAndReadIniDataGeneral("2D_BarCode",   "Use_CCDShuttle_2A_Port"   , AnsiString("5003"));           //Ifor 20151224 CCD 2D
        asCCDBarCodePort[3]     =CheckAndReadIniDataGeneral("2D_BarCode",   "Use_CCDShuttle_2B_Port"   , AnsiString("5004"));           //Ifor 20151224 CCD 2D
    }
    //RogerYang 20180901 add 矽格湖口Demo AI CCD Function
    //==>
    asFix2BGAAICCDIP[0]     =CheckAndReadIniDataGeneral("Fix_AI_CCD",    "Fix_BGA_AI_CCD_IP"           , AnsiString("172.16.8.210"));
    asFix2BGAAICCDPort[0]   =CheckAndReadIniDataGeneral("Fix_AI_CCD",    "Fix_BGA_AI_CCD_Port"         , AnsiString("8000"));
    asFix2BGAAICCDIP[1]     =CheckAndReadIniDataGeneral("Fix_AI_CCD",    "Fix_BGA_AI_CCD_IP2"          , AnsiString("172.16.8.210"));
    asFix2BGAAICCDPort[1]   =CheckAndReadIniDataGeneral("Fix_AI_CCD",    "Fix_BGA_AI_CCD_Port2"        , AnsiString("8001"));
    asVisionLightPort       =CheckAndReadIniDataGeneral("Fix_AI_CCD",    "Vision_Light_PORT"           , AnsiString("COM8"));
    //<==
    //RogerYang 20180901 add 矽格湖口Demo AI CCD Function

    BarcodeBaudRate         =CheckAndReadIniDataGeneral("2D_BarCode",   "BaudRate"              , 9600);
    InBarcodeBaudRate       =CheckAndReadIniDataGeneral("2D_BarCode",   "InBaudRate"            , 9600);
    BarcodeByteSize         =CheckAndReadIniDataGeneral("2D_BarCode",   "ByteSize"              , 8);
    BarcodeStopBit          =CheckAndReadIniDataGeneral("2D_BarCode",   "StopBit"               , 1);
    BarcodeParity           =CheckAndReadIniDataGeneral("2D_BarCode",   "Parity"                , AnsiString("None"));

    asRFIDCom               =CheckAndReadIniDataGeneral("RFID",   "RFIDReader_PORT"       , AnsiString("COM15"));
    iRFIDBaudRate           =CheckAndReadIniDataGeneral("RFID",   "BaudRate"              , 9600);          //Steven 20220713 : RFID Reader for SJSEMI
    iRFIDByteSize           =CheckAndReadIniDataGeneral("RFID",   "ByteSize"              , 8);
    iRFIDStopBit            =CheckAndReadIniDataGeneral("RFID",   "StopBit"               , 1);
    sRFIDParity             =CheckAndReadIniDataGeneral("RFID",   "Parity"                , AnsiString("None"));

    asCCDTrayIP[0]          =CheckAndReadIniDataGeneral("Tray_Mapping",    "Use_Tray_ID_IP"           , AnsiString("172.16.8.200"));    //wei 20161219 Tray Mapping
    asCCDTrayIP[1]          =CheckAndReadIniDataGeneral("Tray_Mapping",    "Use_Tray_Map_IP"          , AnsiString("172.16.8.200"));    //wei 20161219 Tray Mapping
    asCCDTrayIP[4]          =CheckAndReadIniDataGeneral("Tray_Mapping",    "Use_Tray_DeciveCnt1_IP"   , AnsiString("172.16.8.201"));    //Sam 20190405 : Tray Decive Count
    asCCDTrayIP[5]          =CheckAndReadIniDataGeneral("Tray_Mapping",    "Use_Tray_DeciveCnt2_IP"   , AnsiString("172.16.8.201"));    //Sam 20190405 : Tray Decive Count
    asCCDTrayIP[6]          =CheckAndReadIniDataGeneral("Tray_Mapping",    "Use_Tray_DeciveCnt3_IP"   , AnsiString("172.16.8.201"));    //Sam 20190405 : Tray Decive Count

    asCCDTrayIP[7]          =CheckAndReadIniDataGeneral("Tray_Mapping",    "Use_CoverTray_ID_LoaderCarIP"   , AnsiString("172.16.8.151"));      //JerryYang 20250220 : 修改IP預設值
    asCCDTrayIP[8]          =CheckAndReadIniDataGeneral("Tray_Mapping",    "Use_CoverTray_ID_Loader"        , AnsiString("172.16.8.150"));
    asCCDTrayIP[9]          =CheckAndReadIniDataGeneral("Tray_Mapping",    "Use_CoverTray_ID_AUTO1Car_IP"   , AnsiString("172.16.8.152"));
    asCCDTrayIP[10]         =CheckAndReadIniDataGeneral("Tray_Mapping",    "Use_CoverTray_ID_AUTO2Car_IP"   , AnsiString("172.16.8.153"));
    asCCDTrayIP[11]         =CheckAndReadIniDataGeneral("Tray_Mapping",    "Use_CoverTray_ID_AUTO3Car_IP"   , AnsiString("172.16.8.154"));

    asCCDTrayPort[0]        =CheckAndReadIniDataGeneral("Tray_Mapping",    "Use_Tray_ID_Port"         , AnsiString("5101"));            //wei 20161219 Tray Mapping
    asCCDTrayPort[1]        =CheckAndReadIniDataGeneral("Tray_Mapping",    "Use_Tray_Map_Port"        , AnsiString("5102"));            //wei 20161219 Tray Mapping
    asCCDTrayPort[4]        =CheckAndReadIniDataGeneral("Tray_Mapping",    "Use_Tray_DeciveCnt1_Port" , AnsiString("5104"));            //Sam 20190405 : Tray Decive Count
    asCCDTrayPort[5]        =CheckAndReadIniDataGeneral("Tray_Mapping",    "Use_Tray_DeciveCnt2_Port" , AnsiString("5105"));            //Sam 20190405 : Tray Decive Count
    asCCDTrayPort[6]        =CheckAndReadIniDataGeneral("Tray_Mapping",    "Use_Tray_DeciveCnt3_Port" , AnsiString("5106"));            //Sam 20190405 : Tray Decive Count
    asCCDTrayPort[7]        =CheckAndReadIniDataGeneral("Tray_Mapping",    "Use_CoverTray_ID_LoaderCar_Port"  , AnsiString("23"));
    asCCDTrayPort[8]        =CheckAndReadIniDataGeneral("Tray_Mapping",    "Use_CoverTray_ID_Loader_Port"     , AnsiString("23"));
    asCCDTrayPort[9]        =CheckAndReadIniDataGeneral("Tray_Mapping",    "Use_CoverTray_ID_AUTO1Car_Port"   , AnsiString("23"));
    asCCDTrayPort[10]       =CheckAndReadIniDataGeneral("Tray_Mapping",    "Use_CoverTray_ID_AUTO2Car_Port"   , AnsiString("23"));
    asCCDTrayPort[11]       =CheckAndReadIniDataGeneral("Tray_Mapping",    "Use_CoverTray_ID_AUTO3Car_Port"   , AnsiString("23"));

    //Sam 20181201 : AutoAlignment
    //==>
    asCCDAlignIP[0]          =CheckAndReadIniDataGeneral("Auto_Alignment",    "Use_AutoAlign_InTop_IP"              , AnsiString("172.16.110.201"));
    asCCDAlignIP[1]          =CheckAndReadIniDataGeneral("Auto_Alignment",    "Use_AutoAlign_InBottom_IP"           , AnsiString("172.16.110.201"));
    asCCDAlignIP[2]          =CheckAndReadIniDataGeneral("Auto_Alignment",    "Use_AutoAlign_OutTop_IP"             , AnsiString("172.16.110.201"));
    asCCDAlignIP[3]          =CheckAndReadIniDataGeneral("Auto_Alignment",    "Use_AutoAlign_OutBottom_IP"          , AnsiString("172.16.110.201"));
    asCCDAlignPort[0]        =CheckAndReadIniDataGeneral("Auto_Alignment",    "Use_AutoAlign_InTop_Port"            , AnsiString("5110"));
    asCCDAlignPort[1]        =CheckAndReadIniDataGeneral("Auto_Alignment",    "Use_AutoAlign_InBottom_Port"         , AnsiString("5111"));
    asCCDAlignPort[2]        =CheckAndReadIniDataGeneral("Auto_Alignment",    "Use_AutoAlign_OutTop_Port"           , AnsiString("5112"));
    asCCDAlignPort[3]        =CheckAndReadIniDataGeneral("Auto_Alignment",    "Use_AutoAlign_OutBottom_Port"        , AnsiString("5113"));
    //<==
    //Sam 20181201 : AutoAlignment

    USE_TRAY_MAPPING        =CheckRange(CheckAndReadIniDataGeneral("System",   "USE_TRAY_MAPPING",  etmUninstall), (int)etmUninstall, (int)etmDeviceRemain);         //RogerYang 20250906 range max 1->2   //wei 20161219 Tray Mapping
    USE_DIE_CLEAN           =CheckRange(CheckAndReadIniDataGeneral("System",   "USE_DIE_CLEAN",  0), 0, 1);            //wei 20170418

    USE_LOADER_HINGE        =CheckRange(CheckAndReadIniDataGeneral("System",   "USE_LOADER_HINGE",  0), 0, 1);         //wei 20170418
    USE_MR_SYSTEM           =CheckRange(CheckAndReadIniDataGeneral("System",   "USE_MR_SYSTEM",  0), 0, 2);            ////Sam 20190112 LM //wei 20180702 MR
    USE_RFID_SYSTEM         =CheckRange(CheckAndReadIniDataGeneral("System",   "USE_RFID_SYSTEM",  0), 0, 1);          //wei 20180702 MR
    USE_AUTO_ALIGNMENT      =CheckAndReadIniDataGeneral("System", "USE_AUTO_ALIGNMENT",    0);                          //Sam 20181201 : AutoAlignment
    USE_COLORSENSOR_MUN     =CheckAndReadIniDataGeneral("System", "USE_COLORSENSOR_MUN",   eCSMUN_Uninstall);           //Jimmychiu 20230630 : add color sensor MU-N in Loader
    LoaderUnload_StepMotor  =CheckAndReadIniDataGeneral("TrayY", "LoaderUnload_StepMotor", 0);                          //Steven 20200529 : Loader入Tray改步進

    asRFIDComPort[0]        =CheckAndReadIniDataGeneral("RFID",   "RFID1_COM_PORT"        , AnsiString("COM15"));       //wei 20180726 RFID
    asRFIDComPort[1]        =CheckAndReadIniDataGeneral("RFID",   "RFID2_COM_PORT"        , AnsiString("COM16"));       //wei 20180726 RFID
    RFIDBaudRate            =CheckAndReadIniDataGeneral("RFID",   "BaudRate"              , 9600);                      //wei 20180726 RFID
    RFIDByteSize            =CheckAndReadIniDataGeneral("RFID",   "ByteSize"              , 8);                         //wei 20180726 RFID
    RFIDStopBit             =CheckAndReadIniDataGeneral("RFID",   "StopBit"               , 1);                         //wei 20180726 RFID
    RFIDParity              =CheckAndReadIniDataGeneral("RFID",   "Parity"                , AnsiString("None"));        //wei 20180726 RFID

    HOT_PLATE_POSITION      =CheckAndReadIniDataGeneral("System",       "HOT_PLATE_POSITION"    , 0);       //Steven 20140222 : Hot Plate Pin的位置
    HOT_PLATE_LIMITATION    =CheckAndReadIniDataGeneral("System",       "HOT_PLATE_LIMITATION"  , 0);       //Steven 20151117 : Hot Plate Pin的極限位置, 可以跑2x2 8吸嘴
    USE_LASER_DISTANCE      =CheckAndReadIniDataGeneral("System",       "USE_LASER_DISTANCE"    , 0);       //Steven 20140228 : 雷射測距功能
    USE_DEVICE_FLIPPER      =CheckAndReadIniDataGeneral("System",       "USE_DEVICE_FLIPPER"    , 0);       //Frank 20210612 : Flipper Function

    ION_PULSE_COUNT         =CheckRange(CheckAndReadIniDataGeneral("System",       "ION_PULSE_COUNT", 3000), 3000, 100000);            //Steven 20220107 : 張寧要求ESD要快點Alarm     //Steven 20230322 : ION_PULSE_COUNT 最小值改成3000
    FIX3_FULL_PLACE         =CheckRange(CheckAndReadIniDataGeneral("System",       "FIX3_FULL_PLACE",  0), 0, 5);                      //Steven 20130126 : Fix3滿盤功能  //ChungHung 20140722 add for HT9046LA 2--->3 //JimmyChiu 20220927 4->5
    USE_ROTATE_KIT          =CheckRange(CheckAndReadIniDataGeneral("ROTATE_KIT",   "USE_ROTATE_KIT",           0), 0, 1);              //Steven 20121001 : 旋轉Kit
    iRotate_In_Index        =CheckRange(CheckAndReadIniDataGeneral("ROTATE_KIT",   "iRotate_In_Index",         0), -1, 1);             //Steven 20121001 : 旋轉Kit
    iRotate_Out_Tray6       =CheckRange(CheckAndReadIniDataGeneral("ROTATE_KIT",   "iRotate_Out_Index",        2), 0, 2);              //Fix盤不能放!
    iRotate_Type            =CheckRange(CheckAndReadIniDataGeneral("ROTATE_KIT",   "RotateKit_Type",           e1MotRotate), 0, eRotateTypeTotal-1);    //2013-04-12    Dell :旋轉站;馬達版     //Frank 20180512 (Steven) : modify 1 Dut 1 Motor 3 -> 5
    iSpecialSequence        =CheckRange(CheckAndReadIniDataGeneral("ROTATE_KIT",   "SpecialSequence",          0), 0, 1);              //2013-04-12    Dell :旋轉站;馬達版
    FIX3_INSTALL            =CheckRange(CheckAndReadIniDataGeneral("System",   "FIX3_INSTALL",  1), 0, 1);             //2013-07-16 Dell    9046LS第一台沒Fix3;第二台以上Fix3標配
    CROSS_SENSOR_INSTALL    =CheckRange(CheckAndReadIniDataGeneral("System",   "CROSS_SENSOR_INSTALL",  0), 0, 1);     //2013-07-16    Dell    Shuttle cross sensor
    AUTO_SENSOR_INSTALL     =CheckRange(CheckAndReadIniDataGeneral("System",   "AUTO_SENSOR_INSTALL",  0), 0, 1);      //wei 20160914 Auto Shuttle Sensor
    ShuttleVibration_INSTALL=CheckRange(CheckAndReadIniDataGeneral("System",   "ShuttleVibration",  0), 0, 1);         //JerryYang 20171006 (wei) Shuttle 震動馬達
    //上面已經載了  USE_TRAY_MAPPING        =CheckRange(CheckAndReadIniDataGeneral("System",   "USE_TRAY_MAPPING",  0), 0, 2);         //wei 20161219 Tray Mapping
    ESD_Monitor             =CheckRange(CheckAndReadIniDataGeneral("System",   "USE_ESD_Monior",  0), 0, 1);           //2013-07-16    Dell    使用ESD monitor
    USE_NOVX3360            =CheckRange(CheckAndReadIniDataGeneral("System",   "USE_NOVX3360",    0), 0, 1);           //Steven 20131127 : 使用Simco ION風扇
    USE_AutoCleanIonFan     =CheckRange(CheckAndReadIniDataGeneral("System",   "USE_AutoCleanIonFan",    0), 0, 1);    //Isaac 20210609 : IO觸發IonFan清針
    USE_KASUGA              =CheckRange(CheckAndReadIniDataGeneral("System",   "USE_KASUGA",    0), 0, 1);             //Ifor 20150731 : 使用Kasuga ION風扇
    USE_OTD                 =CheckRange(CheckAndReadIniDataGeneral("System",   "USE_OTD",  0), 0, 2);                  //Steven 20140312 : OTD  //ChungHung 20140709 add 1--->2 區分軟體控制及外部控制
    USE_PULSE_TYPE          =CheckRange(CheckAndReadIniDataGeneral("System",   "USE_PULSE_TYPE",  0), 0, 1);           //Ifor 20180316 : add Simco Use Pulse Type
    bHT9045S_USE2x4         =CheckRange(CheckAndReadIniDataGeneral("System",   "bHT9045S_USE2x4",    0), 0, 1);        //Ifor 20170308 (wei) add HT9045S 2x4模式鎖定
    bEnable_KLT_Function    =CheckRange(CheckAndReadIniDataGeneral("System",   "bBarCodeRules",    0), 0, 1);          //Ifor 20180517 : add BarCode 編碼規則 0:民國 1: 西元
    i24V_PULSE_COUNT        =CheckRange(CheckAndReadIniDataGeneral("System",   "I24V_PULSE_COUNT", 500), 500, 1000);   //JerryYang 20190813 count最小值保護避免誤發alarm //Ifor 20190311 : add 24V Monitor Use Pulse Type
    CHAMBER_USE_PULSE_TYPE  =CheckRange(CheckAndReadIniDataGeneral("System",   "CHAMBER_USE_PULSE_TYPE",  0), 0, 1);   //Ifor 20190422 : add Chamber Use Pulse Type

    USE_DIE_CLEAN           =CheckRange(CheckAndReadIniDataGeneral("System",   "USE_DIE_CLEAN",  0), 0, 1);            //wei 20170418
    if(CUSTOMER_CODE==CC_AMD_M)
    {
        #ifdef TFAMD_SUZHOU
            iAMD_Function       =CheckRange(CheckAndReadIniDataGeneral("System",   "iEnable_Function",    1), 0, 2);       //Ifor 20231222 add TFAMD Function 0:AMD(新加坡、美國) 1:TFAMD(蘇州) 2:TFAMD(馬來西亞)
        #else
            #ifdef TFAMD_MALAYSIA
            iAMD_Function       =CheckRange(CheckAndReadIniDataGeneral("System",   "iEnable_Function",    2), 0, 2);       //Ifor 20231222 add TFAMD Function 0:AMD(新加坡、美國) 1:TFAMD(蘇州) 2:TFAMD(馬來西亞)
            #else
            iAMD_Function       =CheckRange(CheckAndReadIniDataGeneral("System",   "iEnable_Function",    0), 0, 2);       //Ifor 20231222 add TFAMD Function 0:AMD(新加坡、美國) 1:TFAMD(蘇州) 2:TFAMD(馬來西亞)
            #endif
        #endif
    }
    else
    {
        iAMD_Function=-1;
    }

    if(USE_SOCKET_SENSOR==0)                                                    //JerryYang 20200327 重新整理第三個通訊模組
    {
        SOCKET_AMP_QTY=0;
    }
    else
    {
        if(USE_COLOR_TRAY_SENSOR)
        {
            SOCKET_AMP_QTY=CheckRange(CheckAndReadIniDataGeneral("System",   "SocketSenAmpQty",  4), 0, iSnSocketCnt);  //裝Color sensor預設4 ea
        }
        else
        {
            if(CUSTOMER_CODE==CC_ASE_KaohSiung)
                SOCKET_AMP_QTY=CheckRange(CheckAndReadIniDataGeneral("System",   "SocketSenAmpQty",  4), 0, iSnSocketCnt);   //kevin 20210907 default: Socket sensor 改成顆
            else
                SOCKET_AMP_QTY=CheckRange(CheckAndReadIniDataGeneral("System",   "SocketSenAmpQty",  8), 0, iSnSocketCnt);   //Steven 20200610 : Socket sensor 改成16顆
        }
    }

    if(USE_ROTATE_KIT)
    {
        ROTATE_AMP_QTY=CheckRange(CheckAndReadIniDataGeneral("System",   "RotateSenAmpQty",  4), 0, 4);
    }
    else
    {
        ROTATE_AMP_QTY=0;
    }

    SOCKET_AMP_QTY_2nd=CheckRange(CheckAndReadIniDataGeneral("System",   "SocketSenAmpQty2nd",  0), 0, 16);

    VibrationMotorCount=CheckRange(CheckAndReadIniDataGeneral("System",   "VibrationCardQty",  2), 0, 2);  //JerryYang 20230814 : add震動馬達通訊調速版本

    COLOR_AMP_QTY=CheckRange(CheckAndReadIniDataGeneral("System",   "ColorSenAmpQty",  6), 0, 8);          //JerryYang 20200812 : color sensor 最大數量改8顆
    //Sam 20210518 : 新增 CanBus 軟體配置
    //==>>
    NUDN1_QTY             =CheckRange(CheckAndReadIniDataGeneral("System",   "CanBusNudn1Qty",    2), 0, 4);
    NUDN1_MACID11_AMP_QTY =CheckRange(CheckAndReadIniDataGeneral("System",   "Nudn1Macid11Qty",  16), 0, 16);
    NUDN1_MACID12_AMP_QTY =CheckRange(CheckAndReadIniDataGeneral("System",   "Nudn1Macid12Qty",   4), 0, 16);
    NUDN1_MACID13_AMP_QTY =CheckRange(CheckAndReadIniDataGeneral("System",   "Nudn1Macid13Qty",   0), 0, 16);
    NUDN1_MACID14_AMP_QTY =CheckRange(CheckAndReadIniDataGeneral("System",   "Nudn1Macid14Qty",   0), 0, 16);
    //<==
    //Sam 20210518 : 新增 CanBus 軟體配置

    USE_OHT_SYSTEM          =CheckRange(CheckAndReadIniDataGeneral("System",   "USE_OHT_SYSTEM",  0), 0, 1);           //KaiChen 20200716 ：OHT
    USE_Multile_Empty       =CheckRange(CheckAndReadIniDataGeneral("System",   "USE_Multile_Empty",  0), 0, 1);        //KaiChen 20200716 ：OHT

    USE_KEYENCE_LOADER      =CheckRange(CheckAndReadIniDataGeneral("System",   "USE_KEYENCE_LOADER",  0), 0, 1);       //wei 20161219 Tray Mapping
    USE_KEYENCE_EMPTY       =CheckRange(CheckAndReadIniDataGeneral("System",   "USE_KEYENCE_EMPTY",   0), 0, 2);       //wei 20161219 Tray Mapping
    USE_MultileEmptyTrayID_Keyence=CheckRange(CheckAndReadIniDataGeneral("System",   "USE_MultileEmptyTrayID_Keyence",   0), 0, 1);

    if(CheckIniData(asGeneralPath, "System", "OTDRecord")==false)               //kevin 20150827 OTD ON_OFF  RECORD
    {
        WriteIniDataGeneral("System", "OTDRecord", 0);
        bOTDRecord=false;
    }
    else
    {
        bOTDRecord            =CheckAndReadIniDataGeneral("System",   "OTDRecord",  0);             //kevin 20150827 OTD ON_OFF  RECORD
    }
    USE_BARCODE_AS_KEYBOARD =CheckAndReadIniDataGeneral("System",  "USE_BARCODE_AS_KEYBOARD" , 0);  //Steven 20140108 : 只能用Bar Code Reader輸入
    USE_46_SUCKER_DB        =CheckAndReadIniDataGeneral("System",  "USE_46_SUCKER_DB",      0);     //Steven 20131008 : HT9045使用46配氣
    USE_46_SENSOR_DB        =CheckAndReadIniDataGeneral("System",  "USE_46_SENSOR_DB",      0);     //Steven 20131008 : HT9045使用46配電
    INDEX_MOTION_CARD       =CheckAndReadIniDataGeneral("System",  "INDEX_MOTION_CARD",     0);     //Steven 20210623 : Index使用Galil
    USE_FINE_PITCH          =CheckAndReadIniDataGeneral("System",  "USE_FINE_PITCH",        0);     //Steven 20210624 : Fine Pitch
    USE_OUT_SHT_MOT         =CheckAndReadIniDataGeneral("System",  "USE_OUT_SHT_MOT",       0);     //Steven 20210624 : Out shuttle獨立馬達
    USE_AIR_CONDITIONER     =CheckAndReadIniDataGeneral("AIR_CON", "USE_AIR_CONDITIONER",   0);     //Steven 20131011 : 冷氣機
    asAirConPort            =CheckAndReadIniDataGeneral("AIR_CON", "AIR_CON_PORT",  AnsiString("COM15"));               //Steven 20131011 : 冷氣機
    USE_AOI_Inspection      =CheckRange(CheckAndReadIniDataGeneral("System", "AOI", 0), 0, 1);                         //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    USE_VibrationCommunication      =CheckRange(CheckAndReadIniDataGeneral("Vibration",   "VibrationCommunication",  0), 0, 1);     //JerryYang 20230814 : add震動馬達通訊調速版本

    USE_Scanner_AOI_Inspection =CheckRange(CheckAndReadIniDataGeneral("System",   "Scanner_AOI",  0), 0, 1);           //Ifor 20190725 : add Scanner AOI
    USE_Fix_AI_CCD          =CheckRange(CheckAndReadIniDataGeneral("System",   "Fix_AI_CCD",  0), 0, 1);               //RogerYang 20180901 add 矽格湖口Demo AI CCD Function
    USE_Top_Scanner_AOI_Inspection =CheckRange(CheckAndReadIniDataGeneral("System",   "Top_Scanner_AOI",  0), 0, 1);   //Ifor 20200902 add: TFAMD Top AOI
    if(CUSTOMER_CODE==CC_KYEC_LEE)                                               //Ifor 20180305 (wei) : add 京元喬智要求強制打開
    {
        USE_BARCODE_AS_KEYBOARD=1;
    }

    if(iRotate_Type!=eCynRotate)                                                //kevin 20130528   Rotate 使用馬達版
    {
        iRotate_In_Index=-1;                                                    //需搶
        iRotate_Out_Tray6=-1;
    }

    if(USE_ROTATE_KIT==1)
    {
        if(iRotate_Type==eCynRotate)
            USE_ROTATE=eCynRot;                                                 //Steven 20230930 : 整合氣缸版旋轉Kit的Flag
        else
            USE_ROTATE=eMotRot;
    }
    else
    {
        USE_ROTATE=eNonRot;
    }

    T_MODE_SPEED            =CheckRange(CheckAndReadIniDataGeneral("System", "T_MODE_SPEED", 0.9), 0.5, 1.0);  //jou 2012-05-30 T Mode Speed
    SHUTTLE_Z_TYPE          =CheckAndReadIniDataGeneral("System", "SHUTTLE_Z_TYPE", 0); //jou 2013-07-26 : 選擇 Shuttle Z Sensor Type
    if(CUSTOMER_CODE==CC_ASE_KaohSiung &&
       MachineTypeChoice==Type_HT9046_LS)                                       //kevin 20210616
    {
        SHUTTLE_Z_TYPE=1;
    }

    dFixTrayDataCleanTime   =CheckRange(CheckAndReadIniDataGeneral("System", "FixTrayDataCleanTime", 1.5), 0.5, 3.0);  //Ifor 20170525 (wei) add Fix Tray Data Clean Time
    iSocketBaseTempCount    =CheckAndReadIniDataGeneral("System", "SocketBasedAdd4Temp", 0);
    bHasEnteredPEModel      =CheckAndReadIniDataGeneral("System", "bHasEnteredPEModel", false);           //Ifor 20160825 add 進入過 PE 模式
    if(MachineTypeChoice==Type_HT9046_LS &&
       (SubMachineType==Type_None ||                                            //Steven 20190307 : Add for HT-9046LA
        SubMachineType==Type_HT9046AU ||                                        //Steven 20240822 : For HT-9046AU
        SubMachineType==Type_HT9046CR))                                         //Steven 20241001 : For HT-9046CR
    {
        iSocketBaseTempCount=eDut4ea;
    }

    if(MachineTypeChoice==Type_HT9046 ||
       MachineTypeChoice==Type_HT9046_LS ||
       MachineTypeChoice==Type_HT1032)
    {
        USE_46_SUCKER_DB=0;
        WriteIniDataGeneral("System", "USE_46_SUCKER_DB",      0);
        USE_46_SENSOR_DB=0;
        WriteIniDataGeneral("System", "USE_46_SENSOR_DB",      0);
    }

    iInArmAutoYTeachOffset   =CheckAndReadIniDataGeneral("AutoTeach",    "iInArmAutoYTeachOffset",      0);    //kevin 20161124 AutoTeach offset Y
    iOutArmAutoYTeachOffset  =CheckAndReadIniDataGeneral("AutoTeach",    "iOutArmAutoYTeachOffset",     0);    //kevin 20161124 AutoTeach offset Y

    iInArmTeachZ   =CheckAndReadIniDataGeneral("AutoTeach",    "iInArmTeachZ",      -1500);   //kevin 20170502 (wei) auto teach Z check pos
    ioutArmTeachZ  =CheckAndReadIniDataGeneral("AutoTeach",    "ioutArmTeachZ",     -1500);   //kevin 20170502 (wei) auto teach  Z check pos

    USE_GROUND_MAN          =CheckRange(CheckAndReadIniDataGeneral("Ground_Man",   "USE_GROUND_MAN",  0), 0, 1);       //Steven 20190828 : 通訊式GroundMan
    asGroundManComPort      =CheckAndReadIniDataGeneral("Ground_Man", "Ground_Man_COM_PORT", AnsiString("COM18"));
    iGroundManScanPoint     =CheckRange(CheckAndReadIniDataGeneral("Ground_Man",   "Ground_Man_ScanPoint",  0), 0, 2); //KaiChen 20191005 ：通訊式GroundMan  //JerryYang 20210428 : Add 28點接地偵側
    iGroundManAlarmOhm      =CheckRange(CheckAndReadIniDataGeneral("Ground_Man",   "Ground_Man_AlarmOhm",  0), 10, 1); //KaiChen 20191005 ：通訊式GroundMan
    USE_KASUGA_Fan          =CheckRange(CheckAndReadIniDataGeneral("System",   "USE_KASUGA_Fan",  0), 0, 1);       //KaiChen 20191225 ：KASUGA Fan 通訊
    iUseHTIonBarFunction    =CheckRange(CheckAndReadIniDataGeneral("System",   "HTIonBarFunction", 0), 0, 3);      //RogerYang 20250825 : Unloader新增3支IonBar，取代4 5 8 ion fan
    iDBQueryDays            =CheckAndReadIniDataGeneral("System", "iDBQueryDays", 3);   //Ifor 20200324 : add KYEC 要求新增DB Qery 天數

    AUTO3_IS_MAGAZINE        =CheckRange(CheckAndReadIniDataGeneral("System",   "AUTO3_IS_MAGAZINE",  0), 0, 1);         //JerryYang 20220909 : add magazine
    iMagazineCheckZPos       =CheckRange(CheckAndReadIniDataGeneral("System",   "iMagazineCheckZPos",    400), 100, 1000);
    #ifdef FOR_EVAN                                                             //Steven 20210304 : 長電給如春的版本
        if(CUSTOMER_CODE==CC_JCET)
        {
            JCET_FOR_EVAN           =1;
            USE_BARCODE_AS_KEYBOARD =1;
            WriteIniDataGeneral("System", "USE_BARCODE_AS_KEYBOARD",  USE_BARCODE_AS_KEYBOARD);
            WriteIniDataGeneral("System", "JCET_FOR_EVAN",  JCET_FOR_EVAN);
        }
        else
        {
            JCET_FOR_EVAN           =0;
        }
    #else
        if(CUSTOMER_CODE==CC_JCET)
            JCET_FOR_EVAN           =CheckAndReadIniDataGeneral("System", "JCET_FOR_EVAN", 0);            //Steven 20210304 : 長電給如春的版本
        else
            JCET_FOR_EVAN           =0;
    #endif

    //KenHsieh 20210830 : add CCD AUTO ALIGNMENT for Offset
    //==>
    iAOA_InArm_Loader_X         =CheckAndReadIniDataGeneral("System", "AOA_InArm_Loader_X", 0);
    iAOA_InArm_Loader_Y         =CheckAndReadIniDataGeneral("System", "AOA_InArm_Loader_Y", 0);
    iAOA_InArm_Shuttle1_X       =CheckAndReadIniDataGeneral("System", "AOA_InArm_Shuttle1_X", 0);
    iAOA_InArm_Shuttle1_Y       =CheckAndReadIniDataGeneral("System", "AOA_InArm_Shuttle1_Y", 0);
    iAOA_InArm_Shuttle2_X       =CheckAndReadIniDataGeneral("System", "AOA_InArm_Shuttle2_X", 0);
    iAOA_InArm_Shuttle2_Y       =CheckAndReadIniDataGeneral("System", "AOA_InArm_Shuttle2_Y", 0);
    iAOA_InArm_Hotplate1_X      =CheckAndReadIniDataGeneral("System", "AOA_InArm_Hotplate1_X", 0);
    iAOA_InArm_Hotplate1_Y      =CheckAndReadIniDataGeneral("System", "AOA_InArm_Hotplate1_Y", 0);
    iAOA_InArm_Hotplate2_X      =CheckAndReadIniDataGeneral("System", "AOA_InArm_Hotplate2_X", 0);
    iAOA_InArm_Hotplate2_Y      =CheckAndReadIniDataGeneral("System", "AOA_InArm_Hotplate2_Y", 0);
    //<==
    //KenHsieh 20210830 : add CCD AUTO ALIGNMENT for Offset
    //KenHsieh 20210918 : CCD AUTO ALIGNMENT offset新增
    //==>
    iAOA_OutArm_Auto1_X         =CheckAndReadIniDataGeneral("System", "AOA_OutArm_Auto1_X", 0);
    iAOA_OutArm_Auto1_Y         =CheckAndReadIniDataGeneral("System", "AOA_OutArm_Auto1_Y", 0);
    iAOA_OutArm_Auto2_X         =CheckAndReadIniDataGeneral("System", "AOA_OutArm_Auto2_X", 0);
    iAOA_OutArm_Auto2_Y         =CheckAndReadIniDataGeneral("System", "AOA_OutArm_Auto2_Y", 0);
    iAOA_OutArm_Auto3_X         =CheckAndReadIniDataGeneral("System", "AOA_OutArm_Auto3_X", 0);
    iAOA_OutArm_Auto3_Y         =CheckAndReadIniDataGeneral("System", "AOA_OutArm_Auto3_Y", 0);
    if(AUTO_EMPTY_COLOR>=3)                                                     //Steven 20240428 : Add for HT9011 AOA
    {
        iAOA_OutArm_Auto4_X     =CheckAndReadIniDataGeneral("System", "AOA_OutArm_Auto4_X", 0);
        iAOA_OutArm_Auto4_Y     =CheckAndReadIniDataGeneral("System", "AOA_OutArm_Auto4_Y", 0);
        iAOA_OutArm_Auto5_X     =CheckAndReadIniDataGeneral("System", "AOA_OutArm_Auto5_X", 0);
        iAOA_OutArm_Auto5_Y     =CheckAndReadIniDataGeneral("System", "AOA_OutArm_Auto5_Y", 0);
        iAOA_OutArm_Auto6_X     =CheckAndReadIniDataGeneral("System", "AOA_OutArm_Auto6_X", 0);
        iAOA_OutArm_Auto6_Y     =CheckAndReadIniDataGeneral("System", "AOA_OutArm_Auto6_Y", 0);
    }
    iAOA_OutArm_Fix1_X          =CheckAndReadIniDataGeneral("System", "AOA_OutArm_Fix1_X", 0);
    iAOA_OutArm_Fix1_Y          =CheckAndReadIniDataGeneral("System", "AOA_OutArm_Fix1_Y", 0);
    iAOA_OutArm_Fix2_X          =CheckAndReadIniDataGeneral("System", "AOA_OutArm_Fix2_X", 0);
    iAOA_OutArm_Fix2_Y          =CheckAndReadIniDataGeneral("System", "AOA_OutArm_Fix2_Y", 0);
    iAOA_OutArm_Fix3_X          =CheckAndReadIniDataGeneral("System", "AOA_OutArm_Fix3_X", 0);
    iAOA_OutArm_Fix3_Y          =CheckAndReadIniDataGeneral("System", "AOA_OutArm_Fix3_Y", 0);
    if(AUTO_EMPTY_COLOR>=3)                                                     //Steven 20240428 : Add for HT9011 AOA
    {
        iAOA_OutArm_Fix4_X      =CheckAndReadIniDataGeneral("System", "AOA_OutArm_Fix4_X", 0);
        iAOA_OutArm_Fix4_Y      =CheckAndReadIniDataGeneral("System", "AOA_OutArm_Fix4_Y", 0);
        iAOA_OutArm_Fix5_X      =CheckAndReadIniDataGeneral("System", "AOA_OutArm_Fix5_X", 0);
        iAOA_OutArm_Fix5_Y      =CheckAndReadIniDataGeneral("System", "AOA_OutArm_Fix5_Y", 0);
        iAOA_OutArm_Fix6_X      =CheckAndReadIniDataGeneral("System", "AOA_OutArm_Fix6_X", 0);
        iAOA_OutArm_Fix6_Y      =CheckAndReadIniDataGeneral("System", "AOA_OutArm_Fix6_Y", 0);
    }
    iAOA_OutArm_Shuttle1_X      =CheckAndReadIniDataGeneral("System", "AOA_OutArm_Shuttle1_X", 0);
    iAOA_OutArm_Shuttle1_Y      =CheckAndReadIniDataGeneral("System", "AOA_OutArm_Shuttle1_Y", 0);
    iAOA_OutArm_Shuttle2_X      =CheckAndReadIniDataGeneral("System", "AOA_OutArm_Shuttle2_X", 0);
    iAOA_OutArm_Shuttle2_Y      =CheckAndReadIniDataGeneral("System", "AOA_OutArm_Shuttle2_Y", 0);
    //<==
    //KenHsieh 20210918 : CCD AUTO ALIGNMENT offset新增

    iControlPanelMode           =CheckAndReadIniDataGeneral("System", "ControlPanelMode", 0);           //Ken 20210702 AddPadInterface
    VCCU_UNIT_TYPE              =CheckAndReadIniDataGeneral("System", "VacuUnitType", 0);               //Sam 20230210 : 新增 VacuumUnit 通訊模組
    USE_ATC_RS232_Check         =CheckRange(CheckAndReadIniDataGeneral("System", "USE_ATC_RS232_Check", 1), 0, 1);     //Ifor 20211116 add: ATC RS232 Check Function
    TRAY_MAPPING_GRAB           =CheckAndReadIniDataGeneral("System", "TRAY_MAPPING_GRAB", 0);          //KenHsieh 20220923 : add Tray Map Throw IC Function
    if(CUSTOMER_CODE ==CC_ASE_KaohSiung || CUSTOMER_CODE ==CC_AMD_M)    //kevin 20220901 ASE_KH close   //Ifor 20240603 add
        USE_ARM_PROTECTION      =CheckAndReadIniDataGeneral("System", "USE_ARM_PROTECTION", false);     //Steven 20220314 : In Our Arm Z Sensor保護加上開關
    else
        USE_ARM_PROTECTION      =CheckAndReadIniDataGeneral("System", "USE_ARM_PROTECTION", true);      //Steven 20220314 : In Our Arm Z Sensor保護加上開關

    //Ztex 2023.04.19 Add HT-1032 TriTemp Function ==>
    BASE_HEATER                 =CheckAndReadIniDataGeneral("System", "BASE_HEATER_COUNT",              0);
    SHUTTLE_FLOODGATE           =CheckAndReadIniDataGeneral("System", "SHUTTLE_FLOODGATE",              0);
    Tri_Temp_Machine            =CheckAndReadIniDataGeneral("System", "Tri_Temp_Machine",               0);
    AirStream_Select            =CheckAndReadIniDataGeneral("System", "AirStream_Select",               0);
    TriTemperature_TotalChannel =CheckAndReadIniDataGeneral("System", "TriTemperature_TotalChannel",    0);
    Tri_Temperature_MaxDegree   =CheckAndReadIniDataGeneral("System", "Tri_Temperature_MaxDegree",      175);
    Tri_Temperature_MinDegree   =CheckAndReadIniDataGeneral("System", "Tri_Temperature_MinDegree",      -55);
    SetHeaterTemp_MaxOutSht     =CheckAndReadIniDataGeneral("System", "SetHeaterTemp_MaxOutSht",        60);
    SetHeaterTemp_MaxIndex      =CheckAndReadIniDataGeneral("System", "SetHeaterTemp_MaxIndex",         60);
    SetHeaterTemp_MaxBase       =CheckAndReadIniDataGeneral("System", "SetHeaterTemp_MaxBase",          60);
    Total_Compressor            =CheckAndReadIniDataGeneral("System", "Total_Compressor",                3);
    INDEXDOORHEATER             =CheckAndReadIniDataGeneral("System", "IndexDoorHeater",                0);
    //Ztex 2023.04.19 Add HT-1032 TriTemp Function <==
    iAutoFormSize               =CheckAndReadIniDataGeneral("System", "iAutoFormSize",      0);         //Jimmychiu 20231130 : 可開關Auto form size
    Enable_PLCSafety_IO         =CheckAndReadIniDataGeneral("System",  "SafePlcIO",         0);         //Jason 20230619 增加PLC_IO模組及安全相關IO.
    DOUBLE_BELT_MODE            =CheckAndReadIniDataGeneral("System",  "DOUBLE_BELT_MODE",  0);         //Ztex 2023.12.13 Add Double Belt Mode
    USE_COVER_TRAYID            =CheckAndReadIniDataGeneral("System",  "USE_COVER_TRAYID",  tCIDNotUse);
    USE_LD_Rot_Arm              =CheckAndReadIniDataGeneral("System", "USE_LD_Rot_Arm",     0);         //RogerYang 20250828 : add for Loader Rotate Arm
    USE_E84_Sensor              =CheckAndReadIniDataGeneral("System", "AGVModal",     0);               //Frank 20251204 add E84

    USE_LdUldCassetteMode       =CheckAndReadIniDataGeneral("System", "USE_LdUldCassetteMode",     0);  //Frank 20251217 add
    if(Tri_Temp_Machine==1)                                                                             //Ztex 2024.09.14 Add Tri_Temp_Machine Do't Check
    {
        for(int i=0; i<tcTotalCount; i++)
        {
            AMBIENT_TEMP_CHECK[i]=false;
        }
    }
    In_Shuttle_Auto_Latch      =CheckRange(CheckAndReadIniDataGeneral("System",   "In_Shuttle_Auto_Latch",  0), 0, 1);    //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
}
//---------------------------------------------------------------------------
void SYSTEM_MODULAR::SystemModularInitial()
{
    MyGem=new HT9045Gem("HT9045", HGem);                                        //20140213  wei   KYEC SECS/GEM

    if(NUMBER_PANEL_TYPE==3 ||
       NUMBER_PANEL_TYPE==4)                                                    //Sam 20240604 : 新增 BinDisplay TFT
        InstallColorBinDisplay(NUMBER_PANEL_TYPE);
}
//---------------------------------------------------------------------------
void SYSTEM_MODULAR::LoadIoData()
{
    int iResult;
    AnsiString Str;
    IoTablePath="D:\\HT9045\\System\\IO_Table.csv";
    if(FileExists(IoTablePath))
    {
        TStringList *StrList=new TStringList();

        mapIOTable.clear();
        IOTable.clear();
        try
        {
            StrList->LoadFromFile(IoTablePath);

            if(StrList->Count<=1)
            {
                Str.sprintf("File %s data is lose!", IoTablePath);
                ShowMyMessage(Str, "注意！IO檔案的資料錯誤!");
            }
            else
            {
                iResult=HSys.IoNo.SetIOTableNo(StrList->Strings[0]);
                if(iResult==HSys.IoNo.eioTotal)
                {
                    for(int i=1; i<StrList->Count; i++)
                    {
                        IOTable.push_back(new TIODATA(StrList->Strings[i]));
                        mapIOTableIter=mapIOTable.find(IOTable[i-1]->Alias);
                        Str=IOTable[i-1]->Alias;
                        if(Str!="")
                        {
                            if(mapIOTableIter==mapIOTable.end())
                            {
                                mapIOTable[IOTable[i-1]->Alias]=i-1;
                            }
                            else
                            {
                                Str.sprintf("IO %s alias is duplicated!", IOTable[i-1]->Alias);
                                ShowMyMessage(Str, "注意！IO名稱重複!");
                            }
                        }
                    }
                }
                else
                {
                    Str.sprintf("File %s data is mistake! (%d)", IoTablePath, iResult);
                    ShowMyMessage(Str, "注意！IO檔案的資料錯誤!");
                }
            }
        }
        catch(...)
        {
            MyDBIProcess("Exception", "SYSTEM_MODULAR::LoadIoData");
            Str.sprintf("File %s is opened by other software!", IoTablePath);
            ShowMyMessage(Str, "注意！IO檔案被其他軟體開啟!");
        }
        delete StrList;

    }
    else
    {
        Str.sprintf("File %s is not exist!", IoTablePath);
        ShowMyMessage(Str, "注意！IO檔案遺失!");
    }
}
//---------------------------------------------------------------------------
void SYSTEM_MODULAR::LoadMotData()
{
    AnsiString Str;
    int iResult;
    MotTablePath="D:\\HT9045\\System\\Mot_Table.csv";
    if(FileExists(MotTablePath))
    {
        TStringList *StrList=new TStringList();

        mapMotTable.clear();
        MotTable.clear();
        try
        {
            StrList->LoadFromFile(MotTablePath);

            if(StrList->Count<=1)
            {
                Str.sprintf("File %s data is lose!", MotTablePath);
                ShowMyMessage(Str, "注意！Motor檔案的資料錯誤!");
            }
            else
            {
                iResult=HSys.MotNo.SetMOTTableNo(StrList->Strings[0]);
                if(iResult>=HSys.MotNo.emotTotal-1)                             //Steven 20231218 HT7080B
                {
                    for(int i=1; i<StrList->Count; i++)
                    {
                        MotTable.push_back(new TMOTDATA(StrList->Strings[i]));
                        mapMotTableIter=mapMotTable.find(MotTable[i-1]->No);
                        Str=MotTable[i-1]->No;
                        if(Str!="")
                        {
                            if(mapMotTableIter==mapMotTable.end())
                            {
                                mapMotTable[MotTable[i-1]->No]=i-1;
                            }
                            else
                            {
                                Str.sprintf("Motor %s alias is duplicated!", MotTable[i-1]->No);
                                ShowMyMessage(Str, "注意！Motor名稱重複!");
                            }
                        }
                    }
                }
                else
                {
                    Str.sprintf("File %s data is mistake! (%d)", MotTablePath, iResult);
                    ShowMyMessage(Str, "注意！Motor檔案的資料錯誤!");
                }
            }
        }
        catch(...)
        {
            MyDBIProcess("Exception", "SYSTEM_MODULAR::LoadIoData");
            Str.sprintf("File %s is opened by other software!", MotTablePath);
            ShowMyMessage(Str, "注意！Motor檔案被其他軟體開啟!");
        }
        delete StrList;

    }
    else
    {
        Str.sprintf("File %s is not exist!", MotTablePath);
        ShowMyMessage(Str, "注意！Motor檔案遺失!");
    }
}
//---------------------------------------------------------------------------
void SYSTEM_MODULAR::InstallColorBinDisplay(int iType)
{
    BinDisCtrl=new TMyBinDispHT9046;

    if(BinDisCtrl==NULL)                                                        //Sam 20240604 : 新增 BinDisplay TFT
        return;

    if(iType==3 || iType==4)                                                    //Sam 20240604 : 新增 BinDisplay TFT
    {

    }
    else
    {
        return;
    }
    BinDisCtrl->SetComPort(sNumberPanelComPort);
    BinDisCtrl->SetComPort2(sNumberPanelComPort2);

    for(int i=0; i<eBinDispTotal; i++)                                          //JerryYang 20220909 : 12->eBinDispTotal  //GG Steven
    {
        if(AUTO3_IS_MAGAZINE==0 && i>=eBinDispMag1 && i<=eBinDispMag14)         //JerryYang 20230515 : 沒裝Magazine就不要顯示
        {
        }
        else if(AUTO_EMPTY_COLOR<3 && i>=eBinDispAuto4 && i<=eBinDispFix12)
        {

        }
        else if(i==eBinDispBulkBox)
        {
        }
        else
        {
            BinDisCtrl->InstalledUnit(i);
        }
        BinDisCtrl->Alias[i]=asTrayForBinDisp[i];
    }

    int autoempty =CheckAndReadIniDataGeneral("System", "AUTO_EMPTY_COLOR", 1);
    int EmptyEmpty=CheckAndReadIniDataGeneral("System", "SUPPORT_2_EMPTY_EMPTY", 0);

    if(autoempty==0 && EmptyEmpty==0)
    {
        BinDisCtrl->CloseUnit(1);
        BinDisCtrl->CloseUnit(2);
    }

    BinDisCtrl->SetDelayTime(dNumberPanelDelay);                                //Sam 20240604 : 顯示器輪巡時間改為 double
}
//---------------------------------------------------------------------------
_fastcall SYSTEM_MODULAR::~SYSTEM_MODULAR()
{
    try
    {
        if(NUMBER_PANEL_TYPE==3 ||                                              //2013-04-12    Dell    debug 在不是使用彩色版七段時,關程式發生記憶體錯誤
           NUMBER_PANEL_TYPE==4)                                                //Sam 20240604 : 新增 BinDisplay TFT
        {
            if(BinDisCtrl!=NULL)
            {
                delete BinDisCtrl;
                BinDisCtrl=NULL;
            }
        }

        delete ATKRecipeInfo;                                                   //Steven 20170901 (wei) : For ATK要新增工作檔比對用的檔案
        CloseGeneralIniFile();                                                  //Steven 20141120 : Add Read/Write IniFile Speed
        CloseIniFile();                                                         //Steven 20141120 : Add Read/Write IniFile Speed
        mapATCAlarmCode.clear();                                                //Steven 20160706 : Add ATC alarm code
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~SYSTEM_MODULAR");
    }
}
//---------------------------------------------------------------------------
TIODATA::TIODATA(AnsiString Str)
{
    bool bHasNullData=false;
    TStringList *SL=new TStringList();
    SL->CommaText=Str;

    if(SL->Count>=14)
    {
        _CommaText=Str;
        Type=SL->Strings[HSys.IoNo.eioType];

        if(Type.AnsiPos(AnsiString("Cylinder_On"))!=0)
            Alias=SL->Strings[HSys.IoNo.eioAlias];
        else if(Type.AnsiPos(AnsiString("Cylinder_Off"))!=0)
            Alias=SL->Strings[HSys.IoNo.eioAlias];
        else if(Type.AnsiPos(AnsiString("Sucker_On"))!=0)
            Alias=SL->Strings[HSys.IoNo.eioAlias];
        else if(Type.AnsiPos(AnsiString("Sucker_Off"))!=0)
            Alias=SL->Strings[HSys.IoNo.eioAlias];
        else
            Alias=SL->Strings[HSys.IoNo.eioAlias];

        if(SL->Strings[HSys.IoNo.eioLane]==AnsiString(""))
        {
            iLane=-1;
            if(iISABase==eMotionNet)                                            //Steven 20231218 HT7080B
                bHasNullData=true;
        }
        else
        {
            iLane=atoi(SL->Strings[HSys.IoNo.eioLane].c_str());
        }

        if(SL->Strings[HSys.IoNo.eioModuleType]==AnsiString(""))
        {
            iModuleType=-1;
        }
        else
        {
            iModuleType=atoi(SL->Strings[HSys.IoNo.eioModuleType].c_str());
        }

        if(SL->Strings[HSys.IoNo.eioIP]==AnsiString(""))
        {
            iIP=-1;
            if(iISABase==eMotionNet)                                            //Steven 20231218 HT7080B
                bHasNullData=true;
        }
        else
        {
            iIP=atoi(SL->Strings[HSys.IoNo.eioIP].c_str());
        }

        if(SL->Strings[HSys.IoNo.eioISABase]==AnsiString(""))
        {
            iISABase=eMotionNet;
        }
        else
        {
            iISABase=atoi(SL->Strings[HSys.IoNo.eioISABase].c_str());
        }

        if(SL->Strings[HSys.IoNo.eioPort]==AnsiString(""))
        {
            iPort=-1;
            bHasNullData=true;
        }
        else
        {
            if(iISABase==eISABase ||                                            //Nickliu 20230310 add Suck ISABase Type
               iISABase==ePCI1735U ||
               iISABase==ePLCbase)
                iPort=HexStrToInt(SL->Strings[HSys.IoNo.eioPort].c_str());
            else
                iPort=atoi(SL->Strings[HSys.IoNo.eioPort].c_str());
        }

        if(SL->Strings[HSys.IoNo.eioBit]==AnsiString(""))
        {
            iBit=-1;
            bHasNullData=true;
        }
        else
        {
            iBit=atoi(SL->Strings[HSys.IoNo.eioBit].c_str());
        }

        if(SL->Strings[HSys.IoNo.eioInType]==AnsiString(""))
        {
            iInType=0;
        }
        else
        {
            iInType=atoi(SL->Strings[HSys.IoNo.eioInType].c_str());
        }

        if(SL->Strings[HSys.IoNo.eioEnable]==AnsiString("") || bHasNullData)
        {
            iEnable=0;
        }
        else
        {
            iEnable=atoi(SL->Strings[HSys.IoNo.eioEnable].c_str());
        }

        if(SL->Strings[HSys.IoNo.eioOnAlarmTime]==AnsiString(""))
            iOnAlarmTime=-1;
        else
            iOnAlarmTime=atoi(SL->Strings[HSys.IoNo.eioOnAlarmTime].c_str());

        if(SL->Strings[HSys.IoNo.eioOffAlarmTime]==AnsiString(""))
            iOffAlarmTime=-1;
        else
            iOffAlarmTime=atoi(SL->Strings[HSys.IoNo.eioOffAlarmTime].c_str());

        if(SL->Strings[HSys.IoNo.eioOnDelayTime]==AnsiString(""))

            iOnDelayTime=-1;
        else
            iOnDelayTime=atoi(SL->Strings[HSys.IoNo.eioOnDelayTime].c_str());

        if(SL->Strings[HSys.IoNo.eioOffDelayTime]==AnsiString(""))
            iOffDelayTime=-1;
        else
            iOffDelayTime=atoi(SL->Strings[HSys.IoNo.eioOffDelayTime].c_str());
    }
    else
    {
        _CommaText      ="";
        Type            ="";
        Alias           ="";
        iLane           =-1;
        iModuleType     =-1;
        iIP             =-1;
        iPort           =-1;
        iBit            =-1;
        iInType         =-1;
        iISABase        =-1;
        iEnable         =-1;
        iOnAlarmTime    =-1;
        iOffAlarmTime   =-1;
        iOnDelayTime    =-1;
        iOffDelayTime   =-1;
    }
    Tag=HSys.IOTable.size();
    bDelete=false;
    delete SL;
}
//---------------------------------------------------------------------------
TIOTABLENO::TIOTABLENO()
{
    eioType             =0;
    eioAlias            =1;
    eioLane             =2;
    eioModuleType       =3;
    eioIP               =4;
    eioPort             =5;
    eioBit              =6;
    eioInType           =7;
    eioISABase          =8;
    eioEnable           =9;
    eioOnAlarmTime      =10;
    eioOffAlarmTime     =11;
    eioOnDelayTime      =12;
    eioOffDelayTime     =13;
    eioNote             =14;
    eioTotal            =15;
}
//---------------------------------------------------------------------------
int TIOTABLENO::SetIOTableNo(AnsiString Str)
{
    int bResult=eioTotal;
    TStringList *SL=new TStringList();
    SL->CommaText=Str;

    if(SL->Count!=eioTotal)
    {
        bResult=false;
    }
    else
    {
        eioType=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("IOType"))!=0)
                eioType=i;
        if(eioType==-1)
            bResult=0;

        eioAlias=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("Alias"))!=0)
                eioAlias=i;
        if(eioAlias==-1)
            bResult=1;

        eioLane=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("Lane"))!=0)
                eioLane=i;
        if(eioLane==-1)
            bResult=2;

        eioModuleType=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("ModuleType"))!=0)
                eioModuleType=i;
        if(eioModuleType==-1)
            bResult=3;

        eioIP=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("IP"))!=0)
                eioIP=i;
        if(eioIP==-1)
            bResult=4;

        eioPort=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("Port"))!=0)
                eioPort=i;
        if(eioPort==-1)
            bResult=5;

        eioBit=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("Bit"))!=0)
                eioBit=i;
        if(eioBit==-1)
            bResult=6;

        eioInType=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("InType"))!=0)
                eioInType=i;
        if(eioInType==-1)
            bResult=7;

        eioISABase=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("ISABase"))!=0)
                eioISABase=i;
        if(eioISABase==-1)
            bResult=8;

        eioEnable=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("Enable"))!=0)
                eioEnable=i;
        if(eioEnable==-1)
            bResult=9;

        eioOnAlarmTime=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("OnAlarmTime"))!=0)
                eioOnAlarmTime=i;
        if(eioOnAlarmTime==-1)
            bResult=10;

        eioOffAlarmTime=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("OffAlarmTime"))!=0)
                eioOffAlarmTime=i;
        if(eioOffAlarmTime==-1)
            bResult=11;

        eioOnDelayTime=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("OnDelayTime"))!=0)
                eioOnDelayTime=i;
        if(eioOnDelayTime==-1)
            bResult=12;

        eioOffDelayTime=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("OffDelayTime"))!=0)
                eioOffDelayTime=i;
        if(eioOffDelayTime==-1)
            bResult=13;

        eioNote=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("Note"))!=0)
                eioNote=i;
        if(eioNote==-1)
            bResult=14;
    }

    delete SL;
    return bResult;
}
//---------------------------------------------------------------------------
TMOTNO::TMOTNO()
{
    emotNo              =0;
    emotAlias           =1;
    emotEnable          =2;
    emotBoardID         =3;
    emotPort            =4;
    emotIP              =5;
    emotGearRatio       =6;
    emotSoftLimitN      =7;
    emotSoftLimitP      =8;
    emotCardModel       =9;
    emotDirection       =10;
    emotHomeDirectior   =11;
    emotAcc             =12;
    emotDec             =13;
    emotInitSpeed       =14;
    emotHomeHighSpeed   =15;
    emotHomeLowSpeed    =16;
    emotJogHighSpeed    =17;
    emotJogLowSpeed     =18;
    emotRange           =19;
    emotRate            =20;
    emotServoAlarmOn    =21;
    emot1P2P            =22;
    emotSensorType      =23;
    emotEncodeType      =24;
    emotPickLimit       =25;
    emotLimitLogic      =26;
    emotIn1Logic        =27;
    emotSimulateSpeed   =28;
    emotTotal           =29;
}
//---------------------------------------------------------------------------
int TMOTNO::SetMOTTableNo(AnsiString Str)
{
    int bResult=emotTotal;
    TStringList *SL=new TStringList();
    SL->CommaText=Str;

    if(SL->Count!=HSys.MotNo.emotTotal)
    {
        bResult=-2;
    }
//    else
    {
        emotNo=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("Motorname"))!=0)
                emotNo=i;
        if(emotNo==-1)
            bResult=0;

        emotAlias=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("Alias"))!=0)
                emotAlias=i;
        if(emotAlias==-1)
            bResult=1;

        emotEnable=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("Enable"))!=0)
                emotEnable=i;
        if(emotEnable==-1)
            bResult=2;

        emotBoardID=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("BoardID"))!=0)
                emotBoardID=i;
        if(emotBoardID==-1)
            bResult=3;

        emotPort=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("Port"))!=0)
                emotPort=i;
        if(emotPort==-1)
            bResult=4;

        emotIP=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("IP"))!=0)
                emotIP=i;
        if(emotIP==-1)
            bResult=5;

        emotGearRatio=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("GearRatio"))!=0)
                emotGearRatio=i;
        if(emotGearRatio==-1)
            bResult=6;

        emotSoftLimitN=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("SoftLimitN"))!=0)
                emotSoftLimitN=i;
        if(emotSoftLimitN==-1)
            bResult=7;

        emotSoftLimitP=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("SoftLimitP"))!=0)
                emotSoftLimitP=i;
        if(emotSoftLimitP==-1)
            bResult=8;

        emotCardModel=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("CardModel"))!=0)
                emotCardModel=i;
        if(emotCardModel==-1)
            bResult=9;

        emotDirection=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("Direction"))!=0)
                emotDirection=i;
        if(emotDirection==-1)
            bResult=10;

        emotHomeDirectior=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("HomeDirectior"))!=0)
                emotHomeDirectior=i;
        if(emotHomeDirectior==-1)
            bResult=11;

        emotAcc=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("Acc"))!=0)
                emotAcc=i;
        if(emotAcc==-1)
            bResult=12;

        emotDec=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("Dec"))!=0)
                emotDec=i;
        if(emotDec==-1)
            bResult=13;

        emotInitSpeed=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("InitSpeed"))!=0)
                emotInitSpeed=i;
        if(emotInitSpeed==-1)
            bResult=14;

        emotHomeHighSpeed=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("HomeHighSpeed"))!=0)
                emotHomeHighSpeed=i;
        if(emotHomeHighSpeed==-1)
            bResult=15;

        emotHomeLowSpeed=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("HomeLowSpeed"))!=0)
                emotHomeLowSpeed=i;
        if(emotHomeLowSpeed==-1)
            bResult=16;

        emotJogHighSpeed=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("JogHighSpeed"))!=0)
                emotJogHighSpeed=i;
        if(emotJogHighSpeed==-1)
            bResult=17;

        emotJogLowSpeed=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("JogLowSpeed"))!=0)
                emotJogLowSpeed=i;
        if(emotJogLowSpeed==-1)
            bResult=18;

        emotRange=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("Range"))!=0)
                emotRange=i;
        if(emotRange==-1)
            bResult=19;

        emotRate=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("Rate"))!=0)
                emotRate=i;
        if(emotRate==-1)
            bResult=20;

        emotServoAlarmOn=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("ServoAlarmOn"))!=0)
                emotServoAlarmOn=i;
        if(emotServoAlarmOn==-1)
            bResult=21;

        emot1P2P=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("1P2P"))!=0)
                emot1P2P=i;
        if(emot1P2P==-1)
            bResult=22;

        emotSensorType=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("SensorType"))!=0)
                emotSensorType=i;
        if(emotSensorType==-1)
            bResult=23;

        emotEncodeType=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("EncodeType"))!=0)
                emotEncodeType=i;
        if(emotEncodeType==-1)
            bResult=24;

        emotPickLimit=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("PickLimit"))!=0)
                emotPickLimit=i;
        if(emotPickLimit==-1)
            bResult=25;

        emotLimitLogic=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("LimitLogic"))!=0)
                emotLimitLogic=i;
        if(emotLimitLogic==-1)
            bResult=26;

        emotIn1Logic=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("In1Logic"))!=0)
                emotIn1Logic=i;
        if(emotIn1Logic==-1)
            bResult=27;

        emotSimulateSpeed=-1;
        for(int i=0; i<SL->Count; i++)
            if(SL->Strings[i].AnsiPos(AnsiString("SimulateSpeed"))!=0)
                emotSimulateSpeed=i;
        if(emotSimulateSpeed==-1)
            bResult=28;
    }
    delete SL;
    return bResult;
}
//---------------------------------------------------------------------------
TMOTDATA::TMOTDATA(AnsiString Str)
{
    AnsiString Msg;
    bool bHasNullData=false;
    TStringList *SL=new TStringList();
    SL->CommaText=Str;

    if(SL->Count>=HSys.MotNo.emotTotal)
    {
        _CommaText=Str;

        if(HSys.MotNo.emotEnable<0 ||                                       //Steven 20240103 : 加入保護
           SL->Strings[HSys.MotNo.emotEnable]==AnsiString(""))
        {
            iEnable=0;
        }
        else
        {
            iEnable=atoi(SL->Strings[HSys.MotNo.emotEnable].c_str());
        }

        if(HSys.MotNo.emotNo<0 ||                                           //Steven 20240103 : 加入保護
           SL->Strings[HSys.MotNo.emotNo]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("MotorName is NULL in %s", MotTablePath);
                ShowMyMessage(Msg, "注意！MotorName為空!");
            }
            bHasNullData=true;
            No="";
        }
        else
        {
            No=SL->Strings[HSys.MotNo.emotNo];
        }

        if(HSys.MotNo.emotAlias<0 ||                                        //Steven 20240103 : 加入保護
           SL->Strings[HSys.MotNo.emotAlias]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("Alias of %s is NULL in %s", No, MotTablePath);
                ShowMyMessage(Msg, "注意！馬達Alias為空!");
            }
            bHasNullData=true;
            Alias="";
        }
        else
        {
            Alias=SL->Strings[HSys.MotNo.emotAlias];
        }

        if(INDEX_MOTION_CARD==0 && (Alias=="MTestY1" || Alias=="MTestZ1" || Alias=="MTestZ2" || Alias=="MTestY2"))
        {
            CardModel="SMC";
            iBoardID=-1;
            iPort=-1;
            iIP=-1;
            dAcc=1.0;
            dDec=1.0;
            iLimitLogic=0;
            iIn1Logic=0;
        }
        else
        {
            if(HSys.MotNo.emotCardModel<0 ||                                    //Steven 20231218 HT7080B
               SL->Strings[HSys.MotNo.emotCardModel]==AnsiString(""))
            {
                if(iEnable)
                {
                    Msg.sprintf("CardModel of %s is NULL in %s", Alias, MotTablePath);
                    ShowMyMessage(Msg, "注意！馬達CardModel為空!");
                }
                bHasNullData=true;
                CardModel="";
            }
            else
            {
                CardModel=SL->Strings[HSys.MotNo.emotCardModel];
            }

            if(CardModel=="MC88X1")                                             //Steven 20231218 HT7080B
            {
                iBoardID=0;
            }
            else
            {
                if(HSys.MotNo.emotBoardID<0 ||
                   SL->Strings[HSys.MotNo.emotBoardID]==AnsiString(""))
                {
                    if(iEnable)
                    {
                        Msg.sprintf("Alias of %s is NULL in %s", Alias, MotTablePath);
                        ShowMyMessage(Msg, "注意！馬達Alias為空!");
                    }
                    bHasNullData=true;
                    iBoardID=-1;
                }
                else
                {
                    iBoardID=atoi(SL->Strings[HSys.MotNo.emotBoardID].c_str());
                }
            }

            if(HSys.MotNo.emotPort<0 ||                                   //Steven 20231218 HT7080B
               SL->Strings[HSys.MotNo.emotPort]==AnsiString(""))
            {
                if(iEnable)
                {
                    Msg.sprintf("Port of %s is NULL in %s", Alias, MotTablePath);
                    ShowMyMessage(Msg, "注意！馬達Port為空!");
                }
                bHasNullData=true;
                iPort=-1;
            }
            else
            {
                if(CardModel=="MC88X1")                                     //20231218 Steven HT7080B
                    iPort=HexStrToInt(SL->Strings[HSys.MotNo.emotPort]);
                else
                    iPort=atoi(SL->Strings[HSys.MotNo.emotPort].c_str());
            }

            if(CardModel=="SYNTEK")
            {
                if(HSys.MotNo.emotIP<0 ||                                   //Steven 20240103 : 加入保護
                   SL->Strings[HSys.MotNo.emotIP]==AnsiString(""))
                {
                    if(iEnable)
                    {
                        Msg.sprintf("IP of %s is NULL in %s", Alias, MotTablePath);
                        ShowMyMessage(Msg, "注意！馬達IP為空!");
                    }
                    bHasNullData=true;
                    iIP=-1;
                }
                else
                {
                    iIP=atoi(SL->Strings[HSys.MotNo.emotIP].c_str());
                }
            }
            else
            {
                iIP=-1;
            }

            if(CardModel=="MC88X1")                                         //20231218 Steven HT7080B
            {
                dAcc=1.0;
                dDec=1.0;
                iLimitLogic=0;
                iIn1Logic=0;
            }
            else
            {
                if(HSys.MotNo.emotAcc<0 ||
                   SL->Strings[HSys.MotNo.emotAcc]==AnsiString(""))
                {
                    if(iEnable)
                    {
                        Msg.sprintf("ACC of %s is NULL in %s", Alias, MotTablePath);
                        ShowMyMessage(Msg, "注意！馬達ACC為空!");
                    }
                    bHasNullData=true;
                    dAcc=1.0;
                }
                else
                {
                    dAcc=atof(SL->Strings[HSys.MotNo.emotAcc].c_str());
                }

                if(HSys.MotNo.emotDec<0 ||
                   SL->Strings[HSys.MotNo.emotDec]==AnsiString(""))
                {
                    if(iEnable)
                    {
                        Msg.sprintf("DEC of %s is NULL in %s", Alias, MotTablePath);
                        ShowMyMessage(Msg, "注意！馬達DEC為空!");
                    }
                    bHasNullData=true;
                    dDec=1.0;
                }
                else
                {
                    dDec=atof(SL->Strings[HSys.MotNo.emotDec].c_str());
                }

                if(HSys.MotNo.emotLimitLogic<0 ||
                   SL->Strings[HSys.MotNo.emotLimitLogic]==AnsiString(""))
                {
                    if(iEnable)
                    {
                        Msg.sprintf("LimitLogic of %s is NULL in %s", Alias, MotTablePath);
                        ShowMyMessage(Msg, "注意！馬達LimitLogic為空!");
                    }
                    bHasNullData=true;
                    iLimitLogic=0;
                }
                else
                {
                    iLimitLogic=atoi(SL->Strings[HSys.MotNo.emotLimitLogic].c_str());
                }

                if(HSys.MotNo.emotIn1Logic<0 ||
                   SL->Strings[HSys.MotNo.emotIn1Logic]==AnsiString(""))
                {
                    if(iEnable)
                    {
                        Msg.sprintf("In1Logic of %s is NULL in %s", Alias, MotTablePath);
                        ShowMyMessage(Msg, "注意！馬達In1Logic為空!");
                    }
                    bHasNullData=true;
                    iIn1Logic=0;
                }
                else
                {
                    iIn1Logic=atoi(SL->Strings[HSys.MotNo.emotIn1Logic].c_str());
                }
            }
        }

        if(HSys.MotNo.emotGearRatio<0 ||                                          //Steven 20231218 HT7080B
           SL->Strings[HSys.MotNo.emotGearRatio]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("GearRatio of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "注意！馬達GearRatio為空!");
            }
            bHasNullData=true;
            dGearRatio=1.0;
        }
        else
        {
            dGearRatio=atof(SL->Strings[HSys.MotNo.emotGearRatio].c_str());
        }

        if(HSys.MotNo.emotDirection<0 ||                                        //Steven 20231218 HT7080B
           SL->Strings[HSys.MotNo.emotDirection]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("Direction of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "注意！馬達Direction為空!");
            }
            bHasNullData=true;
            iDirection=0;
        }
        else
        {
            iDirection=atoi(SL->Strings[HSys.MotNo.emotDirection].c_str());
        }

        if(HSys.MotNo.emotHomeDirectior<0 ||                                    //Steven 20231218 HT7080B
           SL->Strings[HSys.MotNo.emotHomeDirectior]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("HomeDirectior of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "注意！馬達HomeDirectior為空!");
            }
            bHasNullData=true;
            iHomeDirectior=0;
        }
        else
        {
            iHomeDirectior=atoi(SL->Strings[HSys.MotNo.emotHomeDirectior].c_str());
        }

        if(HSys.MotNo.emotSoftLimitN<0 ||                                       //Steven 20231218 HT7080B
           SL->Strings[HSys.MotNo.emotSoftLimitN]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("SoftLimitN of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "注意！馬達SoftLimitN為空!");
            }
            bHasNullData=true;
            iSoftLimitN=-999999;
        }
        else
        {
            iSoftLimitN=atoi(SL->Strings[HSys.MotNo.emotSoftLimitN].c_str());
        }

        if(HSys.MotNo.emotSoftLimitP<0 ||                                      //Steven 20231218 HT7080B
           SL->Strings[HSys.MotNo.emotSoftLimitP]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("SoftLimitP of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "注意！馬達SoftLimitP為空!");
            }
            bHasNullData=true;
            iSoftLimitP=999999;
        }
        else
        {
            iSoftLimitP=atoi(SL->Strings[HSys.MotNo.emotSoftLimitP].c_str());
        }

        if(HSys.MotNo.emotInitSpeed<0 ||                                        //Steven 20231218 HT7080B
           SL->Strings[HSys.MotNo.emotInitSpeed]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("InitSpeed of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "注意！馬達InitSpeed為空!");
            }
            bHasNullData=true;
            iInitSpeed=100;
        }
        else
        {
            iInitSpeed=atoi(SL->Strings[HSys.MotNo.emotInitSpeed].c_str());
        }

        if(HSys.MotNo.emotHomeHighSpeed<0 ||                                    //Steven 20231218 HT7080B
           SL->Strings[HSys.MotNo.emotHomeHighSpeed]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("HomeHighSpeed of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "注意！馬達HomeHighSpeed為空!");
            }
            bHasNullData=true;
            iHomeHighSpeed=100;
        }
        else
        {
            iHomeHighSpeed=atoi(SL->Strings[HSys.MotNo.emotHomeHighSpeed].c_str());
        }

        if(HSys.MotNo.emotHomeLowSpeed<0 ||                                     //Steven 20231218 HT7080B
           SL->Strings[HSys.MotNo.emotHomeLowSpeed]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("HomeLowSpeed of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "注意！馬達HomeLowSpeed為空!");
            }
            bHasNullData=true;
            iHomeLowSpeed=100;
        }
        else
        {
            iHomeLowSpeed=atoi(SL->Strings[HSys.MotNo.emotHomeLowSpeed].c_str());
        }

        if(HSys.MotNo.emotJogHighSpeed<0 ||                                     //Steven 20231218 HT7080B
           SL->Strings[HSys.MotNo.emotJogHighSpeed]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("JogHighSpeed of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "注意！馬達JogHighSpeed為空!");
            }
            bHasNullData=true;
            iJogHighSpeed=100;
        }
        else
        {
            iJogHighSpeed=atoi(SL->Strings[HSys.MotNo.emotJogHighSpeed].c_str());
        }

        if(HSys.MotNo.emotJogLowSpeed<0 ||                                      //Steven 20231218 HT7080B
           SL->Strings[HSys.MotNo.emotJogLowSpeed]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("JogLowSpeed of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "注意！馬達JogLowSpeed為空!");
            }
            bHasNullData=true;
            iJogLowSpeed=100;
        }
        else
        {
            iJogLowSpeed=atoi(SL->Strings[HSys.MotNo.emotJogLowSpeed].c_str());
        }

        if(CardModel=="MC88X1")                                                 //Steven 20231218 HT7080B
        {
            iRange=10;
        }
        else
        {
            if(HSys.MotNo.emotRange<0 ||
               SL->Strings[HSys.MotNo.emotRange]==AnsiString(""))
            {
                if(iEnable)
                {
                    Msg.sprintf("Range of %s is NULL in %s", Alias, MotTablePath);
                    ShowMyMessage(Msg, "注意！馬達Range為空!");
                }
                bHasNullData=true;
                iRange=1;
            }
            else
            {
                iRange=atoi(SL->Strings[HSys.MotNo.emotRange].c_str());
            }
        }

        if(HSys.MotNo.emotRate<0 ||                                              //Steven 20231218 HT7080B
           SL->Strings[HSys.MotNo.emotRate]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("Rate of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "注意！馬達Rate為空!");
            }
            bHasNullData=true;
            iRate=1;
        }
        else
        {
            iRate=atoi(SL->Strings[HSys.MotNo.emotRate].c_str());
        }

        if(HSys.MotNo.emotServoAlarmOn<0 ||                                     //Steven 20231218 HT7080B
           SL->Strings[HSys.MotNo.emotServoAlarmOn]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("ServoAlarmOn of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "注意！馬達ServoAlarmOn為空!");
            }
            bHasNullData=true;
            iServoAlarmOn=0;
        }
        else
        {
            iServoAlarmOn=atoi(SL->Strings[HSys.MotNo.emotServoAlarmOn].c_str());
        }

        if(HSys.MotNo.emot1P2P<0 ||                                             //Steven 20231218 HT7080B
           SL->Strings[HSys.MotNo.emot1P2P]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("1P2P of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "注意！馬達1P2P為空!");
            }
            bHasNullData=true;
            i1P2P=0;
        }
        else
        {
            i1P2P=atoi(SL->Strings[HSys.MotNo.emot1P2P].c_str());
        }

        if(HSys.MotNo.emotSensorType<0 ||                                       //20231218 Steven HT7080B
           SL->Strings[HSys.MotNo.emotSensorType]==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("SensorType of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "注意！馬達SensorType為空!");
            }
            bHasNullData=true;
            iSensorType=0;
        }
        else
        {
            iSensorType=atoi(SL->Strings[HSys.MotNo.emotSensorType].c_str());
        }

        if(CardModel=="MC88X1")                                                 //Steven 20231218 HT7080B
        {
            iEncodeType=0;
        }
        else
        {
            if(HSys.MotNo.emotEncodeType<0 ||                                   //Steven 20231218 HT7080B
               SL->Strings[HSys.MotNo.emotEncodeType]==AnsiString(""))
            {
                if(iEnable)
                {
                    Msg.sprintf("EncodeType of %s is NULL in %s", Alias, MotTablePath);
                    ShowMyMessage(Msg, "注意！馬達EncodeType為空!");
                }
                bHasNullData=true;
                iEncodeType=0;
            }
            else
            {
                iEncodeType=atoi(SL->Strings[HSys.MotNo.emotEncodeType].c_str());
            }
        }

        if(Alias=="MTestZ1" || Alias=="MTestZ2")
        {
            if(HSys.MotNo.emotPickLimit<0 ||                                    //Steven 20240103 : 加入保護
               SL->Strings[HSys.MotNo.emotPickLimit]==AnsiString(""))
            {
                Msg.sprintf("Motor PickLimit of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "注意！Motor PickLimit為空!");
                iPickLimit=0;
                bHasNullData=true;
            }
            else
            {
                iPickLimit=atoi(SL->Strings[HSys.MotNo.emotPickLimit].c_str());
            }
        }
        else
        {
            iPickLimit=0;
        }

        if(CardModel=="MC88X1")                                                 //Steven 20231218 HT7080B
        {
            iSimulateSpeed=1000;
        }
        else
        {
            if(HSys.MotNo.emotSimulateSpeed<0 ||                                //20231218 Steven HT7080B
               SL->Strings[HSys.MotNo.emotSimulateSpeed]==AnsiString(""))
            {
                if(iEnable)
                {
                    Msg.sprintf("SimulateSpeed of %s is NULL in %s", Alias, MotTablePath);
                    ShowMyMessage(Msg, "注意！馬達SimulateSpeed為空!");
                }
                bHasNullData=true;
                iSimulateSpeed=10000;
            }
            else
            {
                iSimulateSpeed=atoi(SL->Strings[HSys.MotNo.emotSimulateSpeed].c_str());
            }
        }

        if(bHasNullData)
        {
            iEnable=0;
        }
    }
    else
    {
        _CommaText      ="";
        No              ="";
        Alias           ="";
        iEnable         =-1;
        iBoardID        =-1;
        iPort           =-1;
        iIP             =-1;
        dGearRatio      =1.0;
        iSoftLimitN     =-999999;
        iSoftLimitP     =999999;
        CardModel       ="";
        iDirection      =0;
        iHomeDirectior  =0;
        dAcc            =1.0;                               //Steven 20231218 HT7080B
        dDec            =1.0;                               //Steven 20231218 HT7080B
        iInitSpeed      =100;
        iHomeHighSpeed  =100;
        iHomeLowSpeed   =100;
        iJogHighSpeed   =100;
        iJogLowSpeed    =100;
        iRange          =1;
        iRate           =1;
        iServoAlarmOn   =0;
        i1P2P           =0;
        iSensorType     =0;
        iEncodeType     =0;
        iPickLimit      =0;
        iLimitLogic     =0;
        iIn1Logic       =0;
        iSimulateSpeed  =10000;
    }
    delete SL;
}
//---------------------------------------------------------------------------
void LogSoftwareOnTime(AnsiString Flag)                                         //Steven 20210526 : 紀錄軟體執行時間
{
    #ifdef DEBUG_SOFTWARE_EXEC_TIME
    AnsiString str;
    if(tsSoftwareExeTime==NULL)
    {
        SoftwareExeTimer.LatchCycleTime(true);
        SoftwareExeTimerSum.LatchCycleTime(true);
        tsSoftwareExeTime=new TMyStringList("D:\\HT9045_Log\\SoftwareExecTime",            //KaiHuang 20200513 Add : For ASE 高雄 扭力值存Log
                                      "SoftwareExeTime",
                                      "Date, Time, Index, Sum, Value, Form, Description");

        tsSoftwareExeTime->SaveType=TByDay;
        tsSoftwareExeTime->AddTextWithDateTime("-1, 0.000, 0.000, Software On, //-------------------------------------------------------");
        iSoftwareExeTag=0;
    }

    str.sprintf("%d, %0.3f, %0.3f, %s", iSoftwareExeTag++, (double)SoftwareExeTimerSum.LatchCycleTime()/1000.0, (double)SoftwareExeTimer.LatchCycleTime()/1000.0, Flag);
    tsSoftwareExeTime->AddTextWithDateTime(str);
    SoftwareExeTimer.LatchCycleTime(true);
    tsSoftwareExeTime->MySaveToFile();
    #endif
}
//---------------------------------------------------------------------------
void LogSoftwareOffTime(AnsiString Flag)    //Steven 20210526 : 紀錄軟體執行時間
{
    #ifdef DEBUG_SOFTWARE_EXEC_TIME
    AnsiString str;
    if(iSoftwareExeTag==-1)
    {
        tsSoftwareExeTime->AddTextWithDateTime("-1, 0.000, 0.000, Software Off, //-------------------------------------------------------");
        SoftwareExeTimer.LatchCycleTime(true);
        SoftwareExeTimerSum.LatchCycleTime(true);
        iSoftwareExeTag++;
    }

    str.sprintf("%d, %0.3f, %0.3f, %s", iSoftwareExeTag++, (double)SoftwareExeTimerSum.LatchCycleTime()/1000.0, (double)SoftwareExeTimer.LatchCycleTime()/1000.0, Flag);
    tsSoftwareExeTime->AddTextWithDateTime(str);
    SoftwareExeTimer.LatchCycleTime(true);  //Steven 20210526 : 紀錄軟體執行時間
    tsSoftwareExeTime->MySaveToFile();
    #endif
}
