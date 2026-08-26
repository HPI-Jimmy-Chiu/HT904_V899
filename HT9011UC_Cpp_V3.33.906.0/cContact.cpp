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

// ---------------------------------------------------------------------------
// ComputeTotalAirForce
//   BCB6 source: cContact.cpp:18675-18891 (TfContact::CalculateTotalAirForce)
//                //JimmyChiu 20220119 獨立計算氣壓壓力
//
//   AI(W906-FW-CONTACT-W28) 20260826.  See cContact.h for the parameter
//   rationale, for the two defaultless switches that are preserved on purpose,
//   and above all for the ONE piece of golden that is deliberately NOT here:
//   the fContactForce->SLKClass walk at :18710-18727, whose two results
//   (dKitDiameter, and which SLK entry's contact offsets to use) are inputs.
// ---------------------------------------------------------------------------
double ComputeTotalAirForce(double dBallCount,
                            double dSingleGf,
                            const TotalAirForceIn& in,
                            const bool bUseTestSocket[][MAX_SOCKET_ROW][MAX_SOCKET_COL],
                            double& dDutCount,
                            TotalAirForceOut& out)
{
    // BCB6 :18677-18683 -- golden's locals with golden's initialisers.  AnsiString Str /
    // sBuffer (:18677-18678) belong to the caption assembly and the untranslated SLK walk;
    // this file stays free of a string dependency, so the caption CHOICE is returned as
    // out.bMinForceCaptionPerIC instead (see cContact.h).
    double dTotalForce     = 0.0;                    // :18679
    double fComplianceUnit = 1.0;                    // :18680
    double dKitDiameter    = in.dKitDiameter;        // :18680 golden inits 30.0, then :18723-18725
                                                     //   overwrites it inside the SLK walk.
    double dMinKgPerHead   = 1.0;                    // :18681  Ifor 20191114 : add Heat 最小力道保護
    double dNowKgPerHead   = 0.0;                    // :18682
    double dHeadMaxForce   = 0.0;                    // :18682
    // iTag (:18683, init -1) belongs to the untranslated SLK walk -- see cContact.h.

    // BCB6 :18684 -- device 要壓的重量   //JimmyChiu 20220119 /1000=>*0.001
    double dDeviceGf = dBallCount * dSingleGf * 0.001;

    // BCB6 :18686-18689   //kevin 20170513 (wei) add
    // NOTE: this WRITES golden's member dDutCount, which is why it is an in/out parameter.
    if (in.iTestMode == DualSite &&
        in.bQualSite2X2Shift &&
        in.bNS7000kit)
        dDutCount = 4;

    // BCB6 :18691-18692 -- golden's own commented-out
    //   `if(CosFunction.bUseDynamicKitDiameter)` wrapper
    //   (Steven 20170605 (wei) 可以自定義Kit口徑 / Steven 20230901 統一只取這個變數).
    //   Dead in golden; nothing to translate.

    // BCB6 :18693-18705 -- golden has NO default here; Position outside 2..6 leaves
    // fComplianceUnit at its 1.0 initialiser.  Preserved (see cContact.h).
    switch (in.iSlkPosition)
    {
        case 2:                                      // :18695  1 Device with 1 Compliance Unit
            fComplianceUnit = 1.0;   break;          // :18696
        case 3:                                      // :18697  2 Device with 1 Compliance Unit
            fComplianceUnit = 0.5;   break;          // :18698
        case 4:                                      // :18699  4 Device with 1 Compliance Unit
            fComplianceUnit = 0.25;  break;          // :18700
        case 5:                                      // :18701  2 Device with 4 Compliance Unit
            fComplianceUnit = 2.0;   break;          // :18702
        case 6:                                      // :18703  8 Device with 1 Compliance Unit
            fComplianceUnit = 0.125; break;          // :18704
    }

    // BCB6 :18707-18728 -- NOT TRANSLATED.  golden guards on
    //   `rgKitDiameter->ItemIndex>=0 && <Items->Count` (Ifor 20191114 : Contact最小值保護的上限)
    //   and then walks fContactForce->SLKClass matching SLKClass[i]->dDiameter against
    //   atof(Items->Strings[ItemIndex]), with the "40x2" -> 402 -> 40 special case, to produce
    //   iTag and dKitDiameter.  The port has no SLK container.  Caller supplies both results.

    // BCB6 :18730-18734   //Steven 20230901 : 要先重置顏色
    out.eValueFieldColor       = efcWhite;   // edAirKPA/edSetKg/edForcePerDeviceKG/edForcePerDeviceN
    out.bAirForceRed           = false;      // edAirForce  (:18730)
    out.bDeviceGfClamped       = false;
    out.fMaxForcePerCompliance = 0.0f;

    // BCB6 :18736-18757   //Steven 20200813 : 用缸徑計算最大壓力
    if (in.bD28MaxForceLimitByDiameter)
    {
        out.bMaxForcePerICVisible = true;                                     // :18738
        // :18739  Jimmychiu 20211021 : #P211018-ATK-H9-01 , V3.21.701.1 ,
        //         It does not work [D28] Maxium Force by Kit Diameter.
        double dCoefficient = (in.dEpMaxKpa <= 500) ? 5.0 : 6.0;
        // :18740  4 = 2*2 ;  2 = diameter to radius
        dHeadMaxForce = (((dKitDiameter * dKitDiameter * 3.14) / 4.0) * dCoefficient * 0.0101972) * fComplianceUnit;
        // :18741-18742  Str.sprintf("Max force per compliance: %0.2fkg", ...) -> lblMaxForcePerIC
        //   ChangeToFloatNonPcnt (MachineType.h:1601) returns float; kept float, not widened.
        out.fMaxForcePerCompliance = ChangeToFloatNonPcnt((double)(dHeadMaxForce), (double)(fComplianceUnit));
        if (dDeviceGf > dHeadMaxForce)                                        // :18743
        {
            dDeviceGf            = dHeadMaxForce;                             // :18745
            out.eValueFieldColor = efcRed;                                    // :18746-18749
            // :18750-18751 -- caller rewrites edForcePerDeviceKG->Text / edForcePerDeviceN->Text
            //   from out.dDeviceGf.  Golden's N field really is dDeviceGf*9.8 here.
            out.bDeviceGfClamped = true;
        }
    }
    else
    {
        out.bMaxForcePerICVisible = false;                                    // :18756
    }

    // BCB6 :18759   //JimmyChiu 20220119 模組化計算AirForce
    dTotalForce = dDeviceGf * dDutCount;

    // BCB6 :18761-18762 -- `if(iTag==-1 || iTag>SLKClass.size()) iTag=0;` belongs to the
    //   untranslated SLK walk; the caller has already applied it when choosing which entry's
    //   offsets to pass in.
    // BCB6 :18763   //JimmyChiu 20220119 上限檢查最小壓力
    //   golden: dMinForce=GetMinForce(dKitDiameter, iTag);  -- WRITES the member dMinForce,
    //   returned here via out.dMinForce.
    double dMinForce = ComputeMinForce(dKitDiameter,
                                       in.dTagContactOffset,
                                       in.dTagContactOffsetNS,
                                       in.bNSKitPress,
                                       in.bD04MinForceByFile,
                                       in.dD04MinForceByFile,
                                       in.dD04MinForceByFile_20mm,
                                       in.dD04MinForceByFile_30mm,
                                       in.dD04MinForceByFile_40mm,
                                       in.dD04MinForceByFile_60mm,
                                       in.dD04MinForceByFile_80mm);

    // BCB6 :18765-18855 -- golden has NO default.  An unlisted iTestMode leaves
    // dNowKgPerHead at 0.0 and dMinKgPerHead at 1.0, and 0.0 then satisfies the :18860
    // test, i.e. it silently takes the minimum-force branch.  Preserved (see cContact.h).
    switch (in.iTestMode)
    {
        case SingleSite:                                                      // :18767
            // :18768-18769  Steven 20231110 : Fix for min force of 1x1
            dNowKgPerHead = dTotalForce / (1.0 * fComplianceUnit);
            dMinKgPerHead = dMinForce   * (1.0 * fComplianceUnit);
            break;
        case DualSite:                                                        // :18771  1x2
        case QualSite2X2N:                                                    // :18772  Frank 20200520 2X2NN Mode
            // :18773-18775  Steven 20110915 : 1x2兩Site,單Dut要可以壓到85KG
            //               2012-01-03 Dell 在1X2模式下,單Site壓到85kg
            if (((bUseTestSocket[0][0][1] == false && bUseTestSocket[1][0][1] == false) ||
                 (bUseTestSocket[0][0][0] == false && bUseTestSocket[1][0][0] == false)) &&
                in.bD27UseSingleSite85kg)
            {
                dNowKgPerHead = dTotalForce / (1.0 * fComplianceUnit);         // :18777
                dMinKgPerHead = dMinForce   * (1.0 * fComplianceUnit);         // :18778
            }
            else
            {
                dNowKgPerHead = dTotalForce / (2.0 * fComplianceUnit);         // :18782
                dMinKgPerHead = dMinForce   * (2.0 * fComplianceUnit);         // :18783
            }
            break;
        case DualSite2x1:                                                     // :18786
            // :18787-18789 -- note the DIFFERENT socket cells from the DualSite arm above.
            if (((bUseTestSocket[0][1][0] == false && bUseTestSocket[1][1][0] == false) ||
                 (bUseTestSocket[0][0][0] == false && bUseTestSocket[1][0][0] == false)) &&
                in.bD27UseSingleSite85kg)
            {
                dNowKgPerHead = dTotalForce / (1.0 * fComplianceUnit);         // :18791
                dMinKgPerHead = dMinForce   * (1.0 * fComplianceUnit);         // :18792
            }
            else
            {
                dNowKgPerHead = dTotalForce / (2.0 * fComplianceUnit);         // :18796
                dMinKgPerHead = dMinForce   * (2.0 * fComplianceUnit);         // :18797
            }
            break;
        case _6Site2X3N:                                                      // :18800  Steven 20220425 : 2X3NN Mode
        case TriSite1X3:                                                      // :18801
            dNowKgPerHead = dTotalForce / (3.0 * fComplianceUnit);             // :18802
            dMinKgPerHead = dMinForce   * (3.0 * fComplianceUnit);             // :18803
            break;
        case QualSite1X4:                                                     // :18805  1x4
        case QualSite2X2:                                                     // :18806  2x2
        //  golden :18807 has `//                case QualSite2X2BS: //2x1 BusyShuttle`
        //  commented out -- dead in golden, nothing to translate.
        case _8Site1X4:                                                       // :18808  ChungHung 20150528 add for 海思 _8Site1x4
        case _8Site2X4N:                                                      // :18809  Wei 20231211 : 2X4NN Mode
            dNowKgPerHead = dTotalForce / (4.0 * fComplianceUnit);             // :18810
            dMinKgPerHead = dMinForce   * (4.0 * fComplianceUnit);             // :18811
            break;
        case _6Site2X3:                                                       // :18813  ChungHung 20140115 add for 2x3_6
            dNowKgPerHead = dTotalForce / (6.0 * fComplianceUnit);             // :18814
            dMinKgPerHead = dMinForce   * (6.0 * fComplianceUnit);             // :18815
            break;
        case _16Site4X4:                                                      // :18817  Sam 20190226 : 16Site4X4
        case _8Site2X4:                                                       // :18818  2x4
            if (in.bOctal_12Kit)                                              // :18819  ChungHung 20140508 add for SCK
            {
                dNowKgPerHead = dTotalForce / (12.0 * fComplianceUnit);        // :18821
                dMinKgPerHead = dMinForce   * (12.0 * fComplianceUnit);        // :18822
            }
            else
            {
                dNowKgPerHead = dTotalForce / (8.0 * fComplianceUnit);         // :18826
                dMinKgPerHead = dMinForce   * (8.0 * fComplianceUnit);         // :18827
            }
            break;
        case _10Site2X5:                                                      // :18830  wei 20190614 10 site
            dNowKgPerHead = dTotalForce / (10.0 * fComplianceUnit);            // :18831
            dMinKgPerHead = dMinForce   * (10.0 * fComplianceUnit);            // :18832
            break;
        case _12Site2X6:                                                      // :18834
            dNowKgPerHead = dTotalForce / (12.0 * fComplianceUnit);            // :18835
            dMinKgPerHead = dMinForce   * (12.0 * fComplianceUnit);            // :18836
            break;
        case _16Site2X8:                                                      // :18838  2x8
            if (in.iCloseSiteModeFor2x8 > e2x8Standard)                       // :18839  Steven 20260420 : != --> >
            {
                dNowKgPerHead = dTotalForce / (8.0 * fComplianceUnit);         // :18841
                dMinKgPerHead = dMinForce   * (8.0 * fComplianceUnit);         // :18842
            }
            else
            {
                dNowKgPerHead = dTotalForce / (16.0 * fComplianceUnit);        // :18846
                dMinKgPerHead = dMinForce   * (16.0 * fComplianceUnit);        // :18847
            }
            break;
        case _32Site4X8N:                                                     // :18850  Steven 20140619 : for 32Site
        case _32Site4X8M:                                                     // :18851
            // :18852-18853  KenHsieh 20230313 : NN Mode Dutcount 32 -> 16
            dNowKgPerHead = dTotalForce / (16.0 * fComplianceUnit);
            dMinKgPerHead = dMinForce   * (16.0 * fComplianceUnit);
            break;
    }

    // BCB6 :18857  `IniConfig.iEP_Min_KG=dMinKgPerHead;`
    //   Config.h:79 declares iEP_Min_KG as `int`, so golden narrows here implicitly.
    //   Reproduced with an explicit cast (same value, no -Wconversion noise); the
    //   untruncated double is also returned so a future caller can tell them apart.
    out.dMinKgPerHead = dMinKgPerHead;
    out.iEP_Min_KG    = (int)dMinKgPerHead;

    // BCB6 :18858  Str.sprintf("Min force per compliance: %0.2fkg", dMinForce);
    out.bMinForceCaptionPerIC = false;

    // BCB6 :18860  1個arm要壓的重量
    if (dNowKgPerHead <= dMinForce)
    {
        // :18862  Steven 20170705 (wei) : 修正最小Contact Force保護 for 連續contact模式
        //         JimmyChiu 20220119 模組化計算AirForce
        dTotalForce = dMinKgPerHead;
        if (in.iCustomerCode == CC_ASE_KaohSiung && in.iTestMode == QualSite2X2)   // :18863  kevin 20191204 ASE KH Telix 2x2
        {
            dTotalForce = dDeviceGf * dDutCount;                              // :18865
            if (dKitDiameter == 30)                                           // :18866
            {
                dMinForce = 1;                                                // :18868
            }
            out.bMinForceCaptionPerIC = true;                                 // :18870
            // NOTE: this arm sets NO field colour, so a red from :18746 survives it.
        }
        else
        {
            out.eValueFieldColor      = efcYellow;                            // :18874-18877
            out.bMinForceCaptionPerIC = false;                                // :18878
        }
    }
    // :18881  lblMinForce->Caption=Str;  -- caller applies, from out.bMinForceCaptionPerIC
    //   and out.dMinForce.

    // BCB6 :18883
    double dMaxLimit = ComputeMaxIndexForceLimit(in.indexPressType,
                                                 dDutCount,
                                                 in.iTestMode,
                                                 in.bD27UseSingleSite85kg,
                                                 in.iKitDiameterItemIndex);
    if (dMaxLimit > 0 && dTotalForce > dMaxLimit)                             // :18884
    {
        out.bAirForceRed = true;                                              // :18886
        // :18887 -- golden's own commented-out
        //   `if(IniConfig.bD28MaxForceLimitByDiameter==false)` guard
        //   (Steven 20200813 : 用缸徑計算最大壓力).  Dead in golden; the clamp below is
        //   therefore UNCONDITIONAL, which is what golden actually executes.
        dTotalForce = dMaxLimit;                                              // :18888
    }

    out.dMinForce       = dMinForce;
    out.dNowKgPerHead   = dNowKgPerHead;
    out.fComplianceUnit = fComplianceUnit;
    out.dDeviceGf       = dDeviceGf;
    out.dHeadMaxForce   = dHeadMaxForce;
    out.dMaxLimit       = dMaxLimit;

    return dTotalForce;                                                       // :18890
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
