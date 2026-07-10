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
