// cContact.cpp
// Standard C++ translation of BCB6 cContact.cpp (V3.33.906.0_20260618) -- CALC CORE ONLY.
// Source of truth: HT9011UC_Code_V3.33.906.0_20260618/cContact.cpp
//
// AI(W64b-ForceCalc-Translate) 20260710: new file. See cContact.h for full scope rationale
// (extract-calc-core; CalculateTotalAirForce and the rest of cContact.cpp/.h/.dfm are OUT OF
// SCOPE this wave).
//
// Toolchain: MinGW g++ 6.3+, C++14 or later.

#include "cContact.h"

// ---------------------------------------------------------------------------
// ComputeMaxIndexForceLimit
//   BCB6 source: cContact.cpp:18893-18955 (TfContact::GetMaxIndexForceLimit)
// ---------------------------------------------------------------------------
double ComputeMaxIndexForceLimit(eIndexPressType indexPressType,
                                  double dDutCount,
                                  int iTestMode,
                                  bool bD27UseSingleSite85kg,
                                  int iKitDiameterItemIndex)
{
    // BCB6 :18895
    double dMaxLimit = 0.0;

    // BCB6 :18896-18931: fixed-kg if/else-if ladder over INDEX_PRESS_TYPE.
    if (indexPressType == e240KG)                 // Steven 20110310 : 240KG
    {
        dMaxLimit = 240;
    }
    else if (indexPressType == e120KG)             // Steven 20121124 : 120KG
    {
        dMaxLimit = 120;
    }
    else if (indexPressType == e500KG)             // Steven 20121124 : 500KG
    {
        dMaxLimit = 500;
    }
    else if (indexPressType == e400KG)             // Steven 20131007 : Index 1.5KW, 400KG
    {
        dMaxLimit = 400;
    }
    else if (indexPressType == e360KG)             // Steven 20140312 : For HT9045WA 360KG
    {
        dMaxLimit = 360;
    }
    else if (indexPressType == e260KG)             // Steven 20131007 : Index 1.5KW, 400KG
    {
        dMaxLimit = 260;
    }
    else if (indexPressType == e160KG)             // ifor 20170215 (wei) add 160KG
    {
        dMaxLimit = 160;
    }
    else if (indexPressType == e640KG)             // Sam 20210628 : add 640KG
    {
        dMaxLimit = 640;
    }
    else if (indexPressType == e800KG)             // Ifor 20230830 : add 800KG
    {
        dMaxLimit = 800;
    }
    else
    {
        // BCB6 :18934-18938: dDutCount==1 && iTestMode in {DualSite,SingleSite,QualSite2X2N}
        // && bD27UseSingleSite85kg==true
        if (dDutCount == 1 &&
            (iTestMode == DualSite   ||
             iTestMode == SingleSite ||
             iTestMode == QualSite2X2N) &&           // Steven 20210908 : add 2x2 nn mode
            bD27UseSingleSite85kg == true)
        {
            // AI(W64b-ForceCalc-Translate) 20260710: faithful golden gap -- this inner
            // if/else-if has NO trailing else (golden :18940-18948).  Any ItemIndex other
            // than 1 or 2 leaves dMaxLimit at its prior value (0.0, the initializer above),
            // NOT 85.  Do not add a default branch here; it would diverge from the BCB6 binary.
            if (iKitDiameterItemIndex == 1)          // Steven 20110915 : 40mm浮動頭,最多只能55kgf
            {
                dMaxLimit = 55;
            }
            else if (iKitDiameterItemIndex == 2)     // Steven 20110915: 60mm浮動頭
            {
                dMaxLimit = 85;
            }
        }
        else
        {
            dMaxLimit = 85;                          // Steven 20110914 : 將9045/46都改成85kgf
        }
    }
    return dMaxLimit;
}

// ---------------------------------------------------------------------------
// ComputeMinForce
//   BCB6 source: cContact.cpp:18957-19022 (TfContact::GetMinForce)
//
//   NOTE: the golden function mutates+returns the class member dMinForce; this free function
//   uses a local variable instead and returns it.  See cContact.h's DEFERRED note for why that
//   is faithful for the sole real call site (CalculateTotalAirForce, golden :18763) and what
//   remains deferred (the member persistence read later at cContact.cpp:5650/8414).
// ---------------------------------------------------------------------------
double ComputeMinForce(double dKitDiameter,
                        double dTagContactOffset,
                        double dTagContactOffsetNS,
                        bool bNSKitPress,
                        bool bD04MinForceByFile,
                        double dD04MinForceByFile,
                        double dD04MinForceByFile_20mm,
                        double dD04MinForceByFile_30mm,
                        double dD04MinForceByFile_40mm,
                        double dD04MinForceByFile_60mm,
                        double dD04MinForceByFile_80mm)
{
    double dMinForce;   // local stand-in for the golden TfContact member (see note above)

    // BCB6 :18959-18967
    if (dKitDiameter == 20)
    {
        dMinForce = 0.5;
        if (bD04MinForceByFile == true)              // Steven 20220822 : Min force for different SLK
        {
            if (dMinForce < dD04MinForceByFile_20mm)
                dMinForce = dD04MinForceByFile_20mm;
        }
    }
    // BCB6 :18968-18976: Ifor 20191106 : add KYEC 缸徑 28 40 58 56
    else if (dKitDiameter == 30 || dKitDiameter == 28)
    {
        dMinForce = 1.5;
        if (bD04MinForceByFile == true)
        {
            if (dMinForce < dD04MinForceByFile_30mm)
                dMinForce = dD04MinForceByFile_30mm;
        }
    }
    // BCB6 :18977-18985
    else if (dKitDiameter == 40)
    {
        dMinForce = 4.0;
        if (bD04MinForceByFile == true)
        {
            if (dMinForce < dD04MinForceByFile_40mm)
                dMinForce = dD04MinForceByFile_40mm;
        }
    }
    // BCB6 :18986-18994: Ifor 20191106 : add KYEC 缸徑 28 40 58 56
    else if (dKitDiameter == 60 || dKitDiameter == 58)
    {
        dMinForce = 8.0;
        if (bD04MinForceByFile == true)
        {
            if (dMinForce < dD04MinForceByFile_60mm)
                dMinForce = dD04MinForceByFile_60mm;
        }
    }
    // BCB6 :18995-19003: Ifor 20240620 : add缸徑 80
    else if (dKitDiameter == 80)
    {
        dMinForce = 15.0;
        if (bD04MinForceByFile == true)
        {
            if (dMinForce < dD04MinForceByFile_80mm)
                dMinForce = dD04MinForceByFile_80mm;
        }
    }
    // BCB6 :19004-19020: else branch -- SLK-tag contact-offset selection + file override.
    else
    {
        if (bNSKitPress)                             // kevin 20170807 (Steven) add NS offset
        {
            dMinForce = dTagContactOffsetNS;
        }
        else
        {
            dMinForce = dTagContactOffset;
        }

        if (bD04MinForceByFile == true)               // Steven 20190314 : Min force is read from file
        {
            if (dMinForce < dD04MinForceByFile)
                dMinForce = dD04MinForceByFile;
        }
    }
    return dMinForce;
}

// =============================================================================
// AI(W906-cContactLeaf) 20260721: W906 cContact leaf-function wave (Step 1 +
// Step 2). See cContact.h for the full per-function parameter rationale; this
// .cpp only carries the translated bodies + inline BCB6 line citations.
// =============================================================================

// ---------------------------------------------------------------------------
// ComputeIsRun2DCheck
//   BCB6 source: cContact.cpp:21203-21219 (TfContact::IsRun2DCheck)
// ---------------------------------------------------------------------------
bool ComputeIsRun2DCheck(bool bCheck2DHardware,
                          int  iBarCodeInstall,
                          bool bEnableBarCode,
                          int  iInArmShtRow,
                          int  iContactMode,
                          bool bRun2DCheck)
{
    // BCB6 :21205
    bool ret = false;

    // BCB6 :21206-21214
    if (bCheck2DHardware &&
        (iBarCodeInstall == ebctUseCCDMode ||
         iBarCodeInstall == ebctInShtIntel ||
         iBarCodeInstall == ebctEtherNetCCD) &&
        bEnableBarCode &&
        iInArmShtRow == 2 &&
        iContactMode == CONTACT_NORMAL &&      // JerryYang 20250428 : fix contact mode有alarm
        // golden :21213 carries a commented-out 8th term here
        // (`TestIF_File.bEnableBottom2D==false`) -- dead in golden itself, nothing to translate.
        bRun2DCheck == true)
    {
        ret = true;
    }
    return ret;
}

// ---------------------------------------------------------------------------
// kgTranToMitsubishikg
//   BCB6 source: cContact.cpp:5261-5271 (free function in golden, not a TfContact member)
// ---------------------------------------------------------------------------
int kgTranToMitsubishikg(int kg)
{
    int vkg = 0;
    vkg = kg / 3;
    if (vkg < 15)
        vkg = 15;
    else if (vkg > 30)
        vkg = 30;

    return vkg;
}

// ---------------------------------------------------------------------------
// ComputeAutoHeightMaxKGTorque
//   BCB6 source: cContact.cpp:5273-5382 (TfContact::GetAutoHeightMaxKGTorque)
// ---------------------------------------------------------------------------
int ComputeAutoHeightMaxKGTorque(int iContactMode,
                                  bool bD10ManualHeightComptibleWithNS,
                                  bool bD14_AutoHeightUseSetTorque,
                                  int iD14_AutoHeightUseSetTorque,
                                  double dKitDiameter,
                                  eIndexPressType indexPressType,
                                  int iTestMode,
                                  int iTestSocketShtRow,
                                  int iTestSocketShtCol,
                                  const int iSiteMap[][MAX_SOCKET_COL],
                                  int iNNMode)
{
    // BCB6 :5275
    int kg = 40, iOpenSiteCT = 0;

    // BCB6 :5277-5281
    if (iContactMode == CONTACT_MANUAL_GET_HEIGHT &&
        bD10ManualHeightComptibleWithNS)
    {
        kg = 80;
    }
    // BCB6 :5282-5285
    else if (bD14_AutoHeightUseSetTorque)          // Ifor 20251017 Fix:無法正常取得高度
    {
        kg = iD14_AutoHeightUseSetTorque;
    }
    // BCB6 :5286-5301
    else if (dKitDiameter < 2.5)                    // JerryYang 20180413 (jou) : 增加為四個判斷是否為low contact force SLK
    {
        if (indexPressType == e85KG)                // Frank 20250214 add
        {
            kg = 22;
        }
        else
        {
            if (iTestMode == SingleSite)
                kg = 10;
            else if (iTestMode == DualSite)
                kg = 10;
            else
                kg = 15;                            // JerryYang 20161124 85kg機台改成,原本是10,load cell量測是22kg
        }
    }
    // BCB6 :5302-5323 (240kg)
    else if (indexPressType == e240KG || indexPressType == e260KG)
    {
        for (int i = 0; i < iTestSocketShtRow; i++)   // jou 2014-07-08 修正16 sites只安裝部分workplace時,在240kg機台狀況下會過力
            for (int j = 0; j < iTestSocketShtCol; j++)
                if (iSiteMap[i][j] > 0)
                    iOpenSiteCT++;

        if (iTestMode == DualSite && dKitDiameter == 3.0)                                  // Steven 20120330 : 1x2, 30mm浮動頭,實際力量最大30KG, 所以Torque比例改成15
            kg = 15;
        else if (iTestMode == _16Site2X8 && dKitDiameter == 3.0 && iOpenSiteCT <= 4)         // jou 2014-07-08 修正16 sites只安裝部分workplace時,在240kg機台狀況下會過力
            kg = 20;
        else if (iTestMode == DualSite && dKitDiameter == 4.0 && iOpenSiteCT <= 1)           // jou 2014-12-18 修正dual sites只安裝一個workplace時,在240kg機台狀況下會過力
            kg = 15;
        else if (iTestMode <= DualSite && dKitDiameter == 4.0)                              // wei 20160329 DualSite以下使用25%
            kg = 25;
        else if ((iTestMode <= _6Site2X3N ||
                  iNNMode == NN_1Row) &&
                 dKitDiameter == 3.0)
            kg = 20;                                // jou 2013-04-23
        else
            kg = 30;
    }
    // BCB6 :5324-5335
    else if (indexPressType == e400KG || indexPressType == e360KG)      // Steven 20131007 : Index 1.5KW, 400KG
    {
        if (iTestMode <= _6Site2X3N ||               // 2013-08-30    Dell
            iNNMode == NN_1Row)
        {
            kg = 12;                                 // 跟Jason討論認為設定12%就好了,12% = 60kg
        }
        else
        {
            kg = 12;                                 // Steven 20131121 : 8 Site只吸一個吸嘴力量
        }
    }
    // BCB6 :5336-5348
    else if (indexPressType == e500KG)                // 2013-08-15 Dell   for TSMC 無ATC 因為SingleSite
    {
        if (iTestMode <= _6Site2X3N ||                // 2013-08-30    Dell
            iNNMode == NN_1Row ||
            (iTestMode >= SingleSite && dKitDiameter < 8.0))                                // Ifor 20200624 add:避免小於80的12%
        {
            kg = 12;                                  // 跟Jason討論認為設定12%就好了,12% = 60kg
        }
        else
        {
            kg = 20;
        }
    }
    // BCB6 :5349-5360
    else if (indexPressType == e640KG)                 // Sam 20210628 : add 640KG
    {
        if (iTestMode <= QualSite2X2N ||                // 2013-08-30    Dell
            iNNMode == NN_1Row)
        {
            kg = 12;                                    // 跟Jason討論認為設定12%就好了,12% = 60kg
        }
        else
        {
            kg = 20;
        }
    }
    // BCB6 :5361-5372
    else if (indexPressType == e800KG)
    {
        if (iTestMode <= QualSite2X2N ||                // 2013-08-30    Dell
            iNNMode == NN_1Row)
        {
            kg = 12;                                    // 跟Jason討論認為設定12%就好了,12% = 60kg
        }
        else
        {
            kg = 20;
        }
    }
    // BCB6 :5373-5379
    else
    {
        if (iTestMode == DualSite && dKitDiameter == 3.0)          // Steven 20120330 : 1x2, 30mm浮動頭,實際力量最大30KG, 所以Torque比例改成30
            kg = 20;
        else if (iTestMode == DualSite && dKitDiameter == 4.0)     // Steven 20161019 : 1x2, 40mm浮動頭,實際力量最大60KG, 所以Torque比例改成30
            kg = 30;
    }

    return kg;
}

// ---------------------------------------------------------------------------
// ComputeTestZCompensationHight
//   BCB6 source: cContact.cpp:18453-18540 (TfContact::TestZ_CompensationHight)
// ---------------------------------------------------------------------------
void ComputeTestZCompensationHight(double dKitDiameter,
                                    double dPress,
                                    const double dIndexZOffset[][15],
                                    double& iTotalOffset_1,
                                    double& iTotalOffset_2)
{
    // BCB6 :18455-18539 -- GOLDEN QUIRK: when the outer diameter check is false, this whole body
    // is a no-op -- iTotalOffset_1/iTotalOffset_2 are NOT touched at all (see cContact.h note).
    if (dKitDiameter == 8 ||
        dKitDiameter == 40.2)
    {
        int iLowLevel = 0;
        int iHightLevel = 0;              // golden spelling "Hight" preserved
        iTotalOffset_1 = 0;
        iTotalOffset_2 = 0;

        if (dPress > 120)                                          // 超過120才需補償
        {
            if (dPress > dIndexZOffset[2][13])                     // 760~800
            {
                iLowLevel = 13;
                iHightLevel = 14;
            }
            else if (dPress > dIndexZOffset[2][12])                // 720~760
            {
                iLowLevel = 12;
                iHightLevel = 13;
            }
            else if (dPress > dIndexZOffset[2][11])                // 680~720
            {
                iLowLevel = 11;
                iHightLevel = 12;
            }
            else if (dPress > dIndexZOffset[2][10])                // 640~680
            {
                iLowLevel = 10;
                iHightLevel = 11;
            }
            else if (dPress > dIndexZOffset[2][9])                 // 600~640
            {
                iLowLevel = 9;
                iHightLevel = 10;
            }
            else if (dPress > dIndexZOffset[2][8])                 // 560~600
            {
                iLowLevel = 8;
                iHightLevel = 9;
            }
            else if (dPress > dIndexZOffset[2][7])                 // 520~560
            {
                iLowLevel = 7;
                iHightLevel = 8;
            }
            else if (dPress > dIndexZOffset[2][6])                 // 480~520
            {
                iLowLevel = 6;
                iHightLevel = 7;
            }
            else if (dPress > dIndexZOffset[2][5])                 // 420~480
            {
                iLowLevel = 5;
                iHightLevel = 6;
            }
            else if (dPress > dIndexZOffset[2][4])                 // 360~420
            {
                iLowLevel = 4;
                iHightLevel = 5;
            }
            else if (dPress > dIndexZOffset[2][3])                 // 300~360
            {
                iLowLevel = 3;
                iHightLevel = 4;
            }
            else if (dPress > dIndexZOffset[2][2])                 // 240~300
            {
                iLowLevel = 2;
                iHightLevel = 3;
            }
            else if (dPress > dIndexZOffset[2][1])                 // 180~240
            {
                iLowLevel = 1;
                iHightLevel = 2;
            }
            else                                                    // 120~180
            {
                iLowLevel = 0;
                iHightLevel = 1;
            }

            iTotalOffset_1 = (dIndexZOffset[0][iLowLevel] + (((dIndexZOffset[0][iHightLevel] - dIndexZOffset[0][iLowLevel]) / 60) * (dPress - dIndexZOffset[2][iLowLevel]))) * 100;
            iTotalOffset_2 = (dIndexZOffset[1][iLowLevel] + (((dIndexZOffset[1][iHightLevel] - dIndexZOffset[1][iLowLevel]) / 60) * (dPress - dIndexZOffset[2][iLowLevel]))) * 100;
        }
    }
}

// ---------------------------------------------------------------------------
// ComputeIndexDownPos
//   BCB6 source: cContact.cpp:18628-18661 (TfContact::SetIndexDownPos)
// ---------------------------------------------------------------------------
double ComputeIndexDownPos(bool bUserDefMaxContactHeight,
                            double dUserDefMaxContactHeight,
                            int iMachineTypeChoice,
                            int iCustomerCode,
                            int iATCMixMode,
                            int iATCSystem)
{
    double fIndexDownPos;   // local stand-in for the golden global (see cContact.h note)

    // BCB6 :18630-18633
    if (bUserDefMaxContactHeight)                              // Steven 20180412 : For new HT-9045高度限制
    {
        fIndexDownPos = dUserDefMaxContactHeight;
    }
    // BCB6 :18634-18641
    else if (iMachineTypeChoice == Type_HT9046_LS)
    {
        fIndexDownPos = fIndexDownPos_for9046LS;
        if (iCustomerCode == CC_QUALCOMM && iATCMixMode == eMixATC60_ATC33)     // Ifor 20260121 add
        {
            fIndexDownPos = fIndexDownPos - 20;
        }
    }
    // BCB6 :18642-18645
    else if (iATCSystem != eATCUninstall)                       // wei 20160303 HT-9045HA -146
    {
        fIndexDownPos = fIndexDownPos_forATC;
    }
    // BCB6 :18646-18660
    else
    {
        if (iCustomerCode == CC_KYEC_LEE ||
            iCustomerCode == CC_KYEC_CHEN ||
            iCustomerCode == CC_KYEC_JCTHIU ||
            iCustomerCode == CC_DL_TEK ||
            iCustomerCode == CC_KYEC_XILINX)                    // Ifor 20161117 Kyec Z軸的位置上移5mm Z軸多下移5mm  -135-5 =-140
        {
            fIndexDownPos = fIndexDownPos_for9045 - 5;
        }
        else
        {
            fIndexDownPos = fIndexDownPos_for9045;
        }
    }

    return fIndexDownPos;
}

// ---------------------------------------------------------------------------
// ComputeIndexYSpeed / ComputeIndexZSpeed
//   BCB6 source: cContact.cpp:18663-18667 / 18669-18673
//   (TfContact::GotIndexYSpeed / TfContact::GotIndexZSpeed)
// ---------------------------------------------------------------------------
int ComputeIndexYSpeed(int iFixValue, int iContactMode, int iMotGailSpeed)
{
    int itemp;   // local stand-in for the golden TfContact member (see cContact.h note)
    itemp = (iContactMode == CONTACT_DEVICE_MAP_CHECK) ? iMotGailSpeed : iFixValue;
    return itemp;
}

int ComputeIndexZSpeed(int iFixValue, int iContactMode, int iMotGailSpeed)
{
    int itemp;   // local stand-in for the golden TfContact member (see cContact.h note)
    itemp = (iContactMode == CONTACT_DEVICE_MAP_CHECK) ? iMotGailSpeed : iFixValue;
    return itemp;
}
