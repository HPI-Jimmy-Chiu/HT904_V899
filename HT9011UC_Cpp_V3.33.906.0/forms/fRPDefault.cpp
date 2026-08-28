// =============================================================================
//  forms/fRPDefault.cpp  --  definitions for the fRPDefault facade
//
//  AI(W906-FW3-BTQ1) 20260828: new file, FW wave FW3-BTQ1 (5 of 5 facades).
//  GOLDEN SOURCE: HT9011UC_Code_V3.33.906.0_20260618/RPDefault.cpp (416 lines)
//  + RPDefault.h (70 lines), read with `io.open(p, encoding='cp950')`.
//  SPAN: 11 golden `TfRPDefault::` member bodies, 386 span lines
//  (tools/census/wave_preflight.py, 20260828).
//
//  THIS WAVE, against the 11-member denominator:
//     4 ACTIVE           ctor / spbExitClick / InitialComponentHead / FormShow
//                        -- 30 of 386 golden span lines (7.8%)
//     7 GATED-WITH-BODY  356 golden span lines carried as `#if 0` transcript
//  ⚠ ONE body dominates: CompareRPDefaultAndValue is 228 lines = 59.1% of the
//  file, and it is gated three times over.
//
//  See forms/fRPDefault.h for the full GATE REGISTER (R-1)..(R-7), the ACTIVE
//  evidence, DEVIATIONS D-1..D-7, the FIELD LIST, the seven RESERVED-NAME sites
//  in forms/fTesterIF.cpp, and the GOLDEN-SIDE FINDING that
//  `SetReciepeParameterDefault` is declared in golden's own header and defined
//  nowhere in golden's own tree.
//
//  ⚠ TWO GOLDEN BUGS PRESERVED VERBATIM in GATE (R-6), both flagged in place:
//    1. golden :268 declares a LOCAL `bool bReceipeDefaultDiffShow=false;`
//       that SHADOWS the class member of the same name (RPDefault.h:61).  The
//       member is therefore never written -- in golden either.  Not "fixed".
//    2. golden :161 `static AnsiString asOldValue[256]` is indexed by `i` over
//       `aName->Count`, which nothing bounds to 256.
//
//  ⚠ EVERY `#if 0` BLOCK BELOW HAS NEVER BEEN COMPILED.  The text is golden's
//  own (the only edit is dropping `__fastcall`), so it is a faithful
//  TRANSCRIPT -- not verified code.  Most of the identifiers it names
//  (fRecipeDefault and its 68 members, fSpeed, fCleaning, FTestIF,
//  fYieldMonitoring, fSecurity, fPassword->edUserName, fMain->BackupSetupFile,
//  RECEIPE_DEFAULT, eRPVisible*, spbSave, ...) are NOT members of this facade
//  and/or have no reachable definition; un-gating requires supplying them
//  first.
//
//  BACKSLASH-COMMENT SCAN (the -Wcomment line-splice trap): RPDefault.cpp was
//  scanned 20260828 for a `//` comment whose line ends in a backslash.  ZERO
//  hits -- so no comment delimiter was changed anywhere in this file.
// =============================================================================
#include "forms/fRPDefault.h"
#include "cmydef.h"              // AccessLevel (cmydef.h:3503) / iDefHonPrecLevel (cmydef.h:3588) -- both cmydef.cpp, ht9045_globals

// AI(W906-FW3-BTQ1) 20260828: TfRPDefault/fRPDefault were FREE tree-wide --
// same idiom as forms/fCounterSel.cpp:38 / forms/fLd_ULd.cpp:43.
// ⚠ THIS IS THE ONLY CTOR IN THIS WAVE THAT ALLOCATES.  Golden's body
// (:23-25) is three `new TStringList` into this object's OWN fields;
// vclcompat::TStringList is a leaf container that calls nothing translated and
// dereferences no global, so the static-init `new` below is still SIOF-safe
// (docs/KNOWLEDGE.md "static-init ctor 不可碰 NULL 全域"; the fLaserSensor
// incident that rule comes from turned 88 of 134 ctest binaries into
// SEGFAULTs).  The two widget field initialisers are likewise leaf vclcompat
// objects.
TfRPDefault *fRPDefault = new TfRPDefault();

TfRPDefault::TfRPDefault()   // golden :20-26
{
    RP_Name     = new TStringList;
    RP_Default  = new TStringList;
    RP_Value    = new TStringList;
}

// ---------------------------------------------------------------------------
void TfRPDefault::spbExitClick(TObject *Sender)   // golden :48-51
{
    (void)Sender;
    Close();   // DEVIATION D-3 -- port-only no-op
}

// ---------------------------------------------------------------------------
//  golden :118-123.  ACTIVE.  vclcompat::TStringGrid exposes Cells[col][row]
//  as an l-value (vclcompat/StringGrid.h:182-196) and defaults to 5x5, so
//  columns 0..2 of row 0 are in range with no resize.
//  ⚠ golden declares this one under `__published` (RPDefault.h:53) even though
//  it is not an event handler.  Kept as an ordinary public method.
// ---------------------------------------------------------------------------
void TfRPDefault::InitialComponentHead()
{
    strngrdReciepeParameterDefault->Cells[0][0]="Parameter";
    strngrdReciepeParameterDefault->Cells[1][0]="Default";
    strngrdReciepeParameterDefault->Cells[2][0]="Value";
}

// ---------------------------------------------------------------------------
//  golden :382-394.  ACTIVE -- a FULL translation, every line live.
//  `AccessLevel` and `iDefHonPrecLevel` are both `extern int` in cmydef.h
//  (:3503 / :3588), defined cmydef.cpp = ht9045_globals, which ht9045_forms
//  links.  Left/Top are DEVIATION D-5 (value-only, no layout).
// ---------------------------------------------------------------------------
void TfRPDefault::FormShow(TObject *Sender)  //Isaac 20170718 (wei) defalut值比較功能:按鈕權限管控
{
    (void)Sender;
    if(AccessLevel>=iDefHonPrecLevel)
    {
        spbSetRPDefault->Enabled=true;
    }
    else
    {
        spbSetRPDefault->Enabled=false;
    }
    Left=75;
    Top=75;
}

// ===========================================================================
//  GATE REGISTER -- translated golden bodies, deliberately NOT COMPILED.
//  See forms/fRPDefault.h for the per-entry reasoning.
// ===========================================================================

#if 0 // GATE (R-1) FormDestroy -- golden :28-41.  LINK BOUNDARY, ONE LINE
      // DEEP: LogSoftwareOffTime(AnsiString) at :40 is declared cmydef.h:5032
      // but its ONLY definition is acarry_shims.cpp:255 (a `{}` no-op) in
      // **ht9045_sm**, and it is not one of the four sanctioned forms->sm
      // exceptions.  ⚠ MyDBIProcess at :38 IS reachable (2-arg body
      // aHotPlateSubstrate.cpp:1099) and the three deletes are fine -- an
      // sm-reaching target opens this one for free.  Same shape as
      // forms/fTestCategory.h GATE (T-8)(a), same wave.
void TfRPDefault::FormDestroy(TObject *Sender)
{
    try
    {
        delete RP_Name;
        delete RP_Default;
        delete RP_Value;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfRPDefault::FormDestroy");
    }
    LogSoftwareOffTime("TfRPDefault, FormDestroy"); //Steven 20210526 : 紀錄軟體執行時間
}
#endif // GATE (R-1)

#if 0 // GATE (R-2) spbDefaulteViewShowClick -- golden :43-46.  MISSING
      // COLLABORATOR: `fRecipeDefault` has NO port anywhere in this tree --
      // the only two hits are .dfm DATA inside generated
      // tools/dfm2rc/layout_out/uRecipeDefault_layout.gen.cpp and its twin,
      // neither of which is in any add_library (20260828).  The whole body is
      // that one call.
void TfRPDefault::spbDefaulteViewShowClick(TObject *Sender)
{
    fRecipeDefault->Show();
}
#endif // GATE (R-2)

#if 0 // GATE (R-3) spbSetRPDefaultClick -- golden :53-78.  LINK BOUNDARY,
      // several times over: fSecurity->Insufficient (cSecurity.cpp =
      // ht9045_sm), fSpeed (cSpeed.cpp = ht9045_sm; forms/fSpeed.cpp is in NO
      // add_library at all), and DoSetRPDefault on fCleaning / FTestIF /
      // fYieldMonitoring -- no facade here carries that method.
void TfRPDefault::spbSetRPDefaultClick(TObject *Sender)
{
    if(fSecurity->Insufficient(168, false)==false)   //Sam 20210414 : Recipe Parameter Default 功能新增權限控制
        return;

    fSpeed->Show();
    fSpeed->DoSetRPDefault(fSpeed->tsAllSpeed, -1); //Isaac 20170718 (wei) defalut值比較功能:add trackbar default
    fSpeed->DoSetRPDefault(fSpeed->tsIndex,     0);
    fSpeed->DoSetRPDefault(fSpeed->tsInArm,     1);
    fSpeed->DoSetRPDefault(fSpeed->tsOutArm,    2);
    fSpeed->DoSetRPDefault(fSpeed->tsTrayArm,   3);
    fSpeed->DoSetRPDefault(fSpeed->tsShuttle,   4);
    fSpeed->Close();

    fCleaning->Show();
    fCleaning->DoSetRPDefault(fCleaning, 0);
    fCleaning->Close();

    FTestIF->Show();
    FTestIF->DoSetRPDefault(FTestIF, 0);
    FTestIF->Close();

    fYieldMonitoring->Show();
    fYieldMonitoring->DoSetRPDefault(fYieldMonitoring, 0);
    fYieldMonitoring->Close();
}
#endif // GATE (R-3)

#if 0 // GATE (R-4) spbReplyRPDefaultClick -- golden :80-116.  LINK BOUNDARY as
      // (R-3), PLUS it drives four other forms' SAVE paths
      // (fSpeed->spbSaveClick, fCleaning->sbCleanSaveClick,
      // FTestIF->spbSaveClick, fYieldMonitoring->btnApplyClick), i.e. it
      // REWRITES FOUR RECIPE FILES.  Safety and reach, independently.
void TfRPDefault::spbReplyRPDefaultClick(TObject *Sender)
{
    AnsiString szDir="D:\\HT9045\\IniData\\RPDefault.ini";  //Isaac 20170718 (wei) defalut值比較功能:add trackbar default

    if(fSecurity->Insufficient(169, false)==false)   //Sam 20210414 : Recipe Parameter Default 功能新增權限控制
        return;

    fSpeed->Show();

    fSpeed->tbAllSpeed->Position      =ReadIniData(szDir, "All", "Speed", 1);//Isaac 20170718 (wei) defalut值比較功能:add trackbar default
    fSpeed->tbAccSpeed->Position      =ReadIniData(szDir, "All", "Accel", 1);
    fSpeed->edAllSpeed->Text          =fSpeed->tbAllSpeed->Position;
    fSpeed->edAllAccSpeed->Text       =fSpeed->tbAccSpeed->Position;

    fSpeed->DoReplyDefaultToForm(fSpeed->tsIndex, 0);
    fSpeed->DoReplyDefaultToForm(fSpeed->tsInArm, 1);
    fSpeed->DoReplyDefaultToForm(fSpeed->tsOutArm, 2);
    fSpeed->DoReplyDefaultToForm(fSpeed->tsTrayArm, 3);
    fSpeed->DoReplyDefaultToForm(fSpeed->tsShuttle, 4);
    fSpeed->spbSaveClick(this);
    fSpeed->Close();

    fCleaning->Show();
    fCleaning->DoReplyDefaultToForm(fCleaning, 0);
    fCleaning->sbCleanSaveClick(this);
    fCleaning->Close();

    FTestIF->Show();
    FTestIF->DoReplyDefaultToForm(FTestIF, 0);
    FTestIF->spbSaveClick(this);
    FTestIF->Close();

    fYieldMonitoring->Show();
    fYieldMonitoring->DoReplyDefaultToForm(fYieldMonitoring, 0);
    fYieldMonitoring->btnApplyClick(this);
    fYieldMonitoring->Close();
}
#endif // GATE (R-4)

#if 0 // GATE (R-5) spbSaveClick -- golden :125-151.  LINK BOUNDARY as (R-3)
      // (eight SearchRecipeParameter walks across four unreachable forms) plus
      // fMain->BackupSetupFile() at :150, which copies the whole recipe.
      // ⚠ NOT gated for :131-133 -- RP_Name/RP_Default/RP_Value->Clear() are
      // this facade's own fields and would work today.
void TfRPDefault::spbSaveClick(TObject *Sender)
{
    fSpeed->Show();
    fCleaning->Show();
    FTestIF->Show();
    fYieldMonitoring->Show();
    RP_Name->Clear();
    RP_Default->Clear();
    RP_Value->Clear();
    fSpeed->SearchRecipeParameter(fSpeed->tsIndex, 0);
    fSpeed->SearchRecipeParameter(fSpeed->tsInArm, 1);
    fSpeed->SearchRecipeParameter(fSpeed->tsOutArm, 2);
    fSpeed->SearchRecipeParameter(fSpeed->tsTrayArm, 3);
    fSpeed->SearchRecipeParameter(fSpeed->tsShuttle, 4);
    fCleaning->SearchRecipeParameter(fCleaning, 0);
    FTestIF->SearchRecipeParameter(FTestIF, 0);
    fYieldMonitoring->SearchRecipeParameter(fYieldMonitoring, 0);

    CompareRPDefaultAndValue(RP_Name, RP_Default, RP_Value);

    fSpeed->Close();
    fCleaning->Close();
    FTestIF->Close();
    fYieldMonitoring->Close();

    fMain->BackupSetupFile();               //Ifor 20170620 (wei) add Auto BackUp Setup File & Last Data
}
#endif // GATE (R-5)

#if 0 // GATE (R-6) CompareRPDefaultAndValue -- golden :153-380 (228L = 59.1% of
      // the file).  RESERVED NAME (forms/fTesterIF.cpp:1600).  THREE
      // INDEPENDENT GATES:
      // (a) MISSING COLLABORATOR -- 68 distinct `fRecipeDefault->` members
      //     (8 TTabSheet* at :173-180, 60 TLabel*/TPanel* pairs at :186-258)
      //     and fRecipeDefault has no port at all (see R-2).
      // (b) WRITES DISK -- MyForceDirectories(asDefaultRecipeChangeLogPath) at
      //     :354 CREATES A DIRECTORY, then SaveToFile at :359 and again at
      //     :363 (the second into IniConfig.asN14_20_ChangeLogPath, a
      //     CUSTOMER-CONFIGURED path that may be a network share).
      // (c) fPassword->edUserName->Text at :275 -- forms/fPassword.cpp exists
      //     in ht9045_forms but edUserName was NOT confirmed to be a member
      //     (see the header's DISCLOSURE).  Listed third; (a) and (b) already
      //     decide this body.
      // ⚠ GOLDEN BUG PRESERVED VERBATIM #1: :268 declares a LOCAL
      // `bool bReceipeDefaultDiffShow=false;` that SHADOWS the class member of
      // the same name (RPDefault.h:61).  Every write at :307 lands on the
      // local; the member is never updated -- in golden either.  NOT "fixed".
      // ⚠ GOLDEN BUG PRESERVED VERBATIM #2: :161 `static AnsiString
      // asOldValue[256]` is indexed by `i` over `aName->Count`, which nothing
      // bounds to 256.
void TfRPDefault::CompareRPDefaultAndValue(TStringList *aName, TStringList *aDefault, TStringList *aValue)
{
    int iRPCount;
    AnsiString aSection="";
    AnsiString aPath="";
    AnsiString asMsg;
    TStringList *slDefaultRecipeChangeLog = new TStringList;
    slDefaultRecipeChangeLog->Clear();
    static AnsiString asOldValue[256]={""};
    bool bHasChange=false;
    int a;
    int iVisibleCnt[eRPVisibleCnt]={0};

    for(int i=0;i<eRPVisibleCnt;i++)
    {
        iVisibleCnt[i]=0;
    }

    TTabSheet *tsVisible[eRPVisibleCnt]=
    {
        fRecipeDefault->tsIndex,
        fRecipeDefault->tsInputArm,
        fRecipeDefault->tsOutputArm,
        fRecipeDefault->tsShuttle,
        fRecipeDefault->tsTrayArm,
        fRecipeDefault->tsAutoClean,
        fRecipeDefault->tsYield,
        fRecipeDefault->tsTester
    };

    RECEIPE_DEFAULT ReceipeDefault_PARA[]=
    {
        //Index
        { fRecipeDefault->lbIndexunit_Speed                                       , fRecipeDefault->pnlIndexunit_Speed },
        { fRecipeDefault->lbIndexunit_VaccumCheckTime                             , fRecipeDefault->pnlIndexunit_VaccumCheckTime },
        { fRecipeDefault->lbIndexunit_CounterAirOnTime                            , fRecipeDefault->pnlIndexunit_CounterAirOnTime },
        { fRecipeDefault->lbIndexunit_DestoryAgainTime                            , fRecipeDefault->pnlIndexunit_DestoryAgainTime },
        { fRecipeDefault->lbIndexunit_DestoryAgainCount                           , fRecipeDefault->pnlIndexunit_DestoryAgainCount },
        { fRecipeDefault->lblInitialMaxTest                                       , fRecipeDefault->pnlInitialMaxTest },
        { fRecipeDefault->lblMaxTestTime                                          , fRecipeDefault->pnlMaxTestTime },
        { fRecipeDefault->lblStartDelayTime                                       , fRecipeDefault->pnlStartDelayTime },

        //Input
        { fRecipeDefault->lbInput_XYSpeed                                         , fRecipeDefault->pnlInput_XYSpeed },
        { fRecipeDefault->lbInput_ZSpeed                                          , fRecipeDefault->pnlInput_ZSpeed  },
        { fRecipeDefault->lbInput_PitchSpeed                                      , fRecipeDefault->pnlInput_PitchSpeed },
        { fRecipeDefault->lbInput_XYAcc                                           , fRecipeDefault->pnlInput_XYAcc },
        { fRecipeDefault->lbInput_ZAcc                                            , fRecipeDefault->pnlInput_ZAcc },
        { fRecipeDefault->lbInput_PitchAcc                                        , fRecipeDefault->pnlInput_PitchAcc },
        { fRecipeDefault->lbInput_VacumCheckTime                                  , fRecipeDefault->pnlInput_VacumCheckTime },
        { fRecipeDefault->lbInput_ArmAirOnTime                                    , fRecipeDefault->pnlInput_ArmAirOnTime },
        { fRecipeDefault->lbInput_ArmShuttleWaitTime                              , fRecipeDefault->pnlInput_ArmShuttleWaitTime },
        { fRecipeDefault->lbInput_ArmDestroyAgainTime                             , fRecipeDefault->pnlInput_ArmDestroyAgainTime },
        { fRecipeDefault->lbInput_ArmDestroyAgainCount                            , fRecipeDefault->pnlInput_ArmDestroyAgainCount },

        //Output
        { fRecipeDefault->lbOutput_XYSpeed                                        , fRecipeDefault->pnlOutput_XYSpeed },
        { fRecipeDefault->lbOutput_ZSpeed                                         , fRecipeDefault->pnlOutput_ZSpeed  },
        { fRecipeDefault->lbOutput_PitchSpeed                                     , fRecipeDefault->pnlOutput_PitchSpeed },
        { fRecipeDefault->lbOutput_OutXYAcc                                       , fRecipeDefault->pnlOutput_OutXYAcc },
        { fRecipeDefault->lbOutput_ZAcc                                           , fRecipeDefault->pnlOutput_ZAcc },
        { fRecipeDefault->lbOutput_PitchAcc                                       , fRecipeDefault->pnlOutput_PitchAcc },
        { fRecipeDefault->lbAutoClean_InArmSpeed                                  , fRecipeDefault->pnlOutput_VacumCheckTime  },
        { fRecipeDefault->lbOutput_ArmAirOnTime                                   , fRecipeDefault->pnlOutput_ArmAirOnTime },
        { fRecipeDefault->lbOutput_ArmDestroyAgainTime                            , fRecipeDefault->pnlOutput_ArmDestroyAgainTime },
        { fRecipeDefault->lbOutput_ArmDestroyAgainCount                           , fRecipeDefault->pnlOutput_ArmDestroyAgainCount },

        //Shuttle
        { fRecipeDefault->lbShuttle_Shuttle1Speed                                 , fRecipeDefault->pnlShuttle_Shuttle1Speed  },
        { fRecipeDefault->lbShuttle_Shuttle2Speed                                 , fRecipeDefault->pnlShuttle_Shuttle2Speed },
        { fRecipeDefault->lbShuttle_Shuttle1Accel                                 , fRecipeDefault->pnlShuttle_Shuttle1Accel },
        { fRecipeDefault->lbShuttle_Shuttle2Accel                                 , fRecipeDefault->pnlShuttle_Shuttle2Accel },

        //TrayArm
        { fRecipeDefault->lbTrayArm_XSpeed                                        , fRecipeDefault->pnlTrayArm_XSpeed },
        { fRecipeDefault->lbTrayArm_XAcc                                          , fRecipeDefault->pnlTrayArm_XAcc  },
        { fRecipeDefault->lbTrayArm_RetryCount                                    , fRecipeDefault->pnlTrayArm_RetryCount },
        { fRecipeDefault->lbTrayArm_VacuumCheckTime                               , fRecipeDefault->pnlTrayArm_VacuumCheckTime },
        { fRecipeDefault->lbTrayArm_CounterAirONTime                              , fRecipeDefault->pnlTrayArm_CounterAirONTime },
        { fRecipeDefault->lbTrayArm_HandDownTime                                  , fRecipeDefault->pnlTrayArm_HandDownTime },

        //AutoClean
        { fRecipeDefault->lbAutoClean_AlarmCount                                  , fRecipeDefault->pnlAutoClean_AlarmCount },
        { fRecipeDefault->lbAutoClean_OutArmSpeed                                 , fRecipeDefault->pnlAutoClean_OutArmSpeed  },
        { fRecipeDefault->lbAutoClean_ShuttleSpeed                                , fRecipeDefault->pnlAutoClean_ShuttleSpeed },
        { fRecipeDefault->lbAutoClean_RotationStageSpeed                          , fRecipeDefault->pnlAutoClean_RotationStageSpeed },
        { fRecipeDefault->lbAutoClean_ContactTime                                 , fRecipeDefault->pnlAutoClean_ContactTime },
        { fRecipeDefault->lbAutoClean_ContactCount                                , fRecipeDefault->pnlAutoClean_ContactCount },
        { fRecipeDefault->lblAutoCleaning_ModeIntervalContactCount                , fRecipeDefault->pnlAutoCleaning_ModeIntervalContactCount },
        { fRecipeDefault->lblAutoCleaning_ModeSocketAlarmCount                    , fRecipeDefault->pnlAutoCleaning_ModeSocketAlarmCount },

        //Yield
        { fRecipeDefault->lblLowYieldEnable_FT                                    , fRecipeDefault->pnlLowYieldEnable_FT },
        { fRecipeDefault->lblLowYield_FT                                          , fRecipeDefault->pnlLowYield_FT  },
        { fRecipeDefault->lblLowYieldIg                                           , fRecipeDefault->pnlLowYieldIg },
        { fRecipeDefault->lblContsFailSocketAlarmCT_FT                            , fRecipeDefault->pnlContsFailSocketAlarmCT_FT },
        { fRecipeDefault->lblContsFailHeadAlarmCT_FT                              , fRecipeDefault->pnlContsFailHeadAlarmCT_FT },
        { fRecipeDefault->lblAlarm4ContinueType                                   , fRecipeDefault->pnlAlarm4ContinueType },
        { fRecipeDefault->lblAlarm4IntervalCount                                  , fRecipeDefault->pnlAlarm4IntervalCount },
        { fRecipeDefault->lblAlarm4ContinueCount                                  , fRecipeDefault->pnlAlarm4ContinueCount },
        { fRecipeDefault->lblAlarm4EnableIntervalYield                            , fRecipeDefault->pnlAlarm4EnableIntervalYield },
        { fRecipeDefault->lblAlarm4IntervalYieldYield                             , fRecipeDefault->pnlAlarm4IntervalYieldYield },
        { fRecipeDefault->lblAlarm4IntervalYieldIntervalCount                     , fRecipeDefault->pnlAlarm4IntervalYieldIntervalCount },
        { fRecipeDefault->lblAlarm4IntervalYieldContinueCount                     , fRecipeDefault->pnlAlarm4IntervalYieldContinueCount },
        { fRecipeDefault->lblSiteToSiteYield                                      , fRecipeDefault->pnlSiteToSiteYield },
        { fRecipeDefault->lblHeadToHeadYield                                      , fRecipeDefault->pnlHeadToHeadYield },
    };
    strngrdReciepeParameterDefault->RowCount=aName->Count+1;
    InitialComponentHead();
    for(int i=0; i<aName->Count; i++)
    {
        strngrdReciepeParameterDefault->Cells[0][i+1]=aName->Strings[i];
        strngrdReciepeParameterDefault->Cells[1][i+1]=aDefault->Strings[i];
        strngrdReciepeParameterDefault->Cells[2][i+1]=aValue->Strings[i];
    }
    bool bReceipeDefaultDiffShow=false;
    iRPCount=sizeof(ReceipeDefault_PARA)/sizeof(RECEIPE_DEFAULT);

    //Sam 20201209 : Default Recipe ChangeLog
    //==>
    if(CosFunction.bRecipeParameterDefaultLog)
    {
        asMsg.sprintf("USER ID:%s",fPassword->edUserName->Text);
        slDefaultRecipeChangeLog->Add(asMsg);
        for(a=0; a<iRPCount; a++)
        {
            ReceipeDefault_PARA[a].PLabel->Visible=false;
            ReceipeDefault_PARA[a].PPanel->Visible=false;
        }
    }
    //<==
    //Sam 20201209 : Default Recipe ChangeLog

    for(int i=0; i<aName->Count; i++)
    {
        for(int j=0; j<iRPCount; j++)
        {
            if(aName->Strings[i]==ReceipeDefault_PARA[j].PLabel->Caption)
            {
                ReceipeDefault_PARA[j].PPanel->Caption=aValue->Strings[i];

                if(asOldValue[i]!=aValue->Strings[i])
                {
                    asOldValue[i]=aValue->Strings[i];
                    bHasChange=true;
                }

                if(aDefault->Strings[i]==aValue->Strings[i])
                {
                    ReceipeDefault_PARA[j].PPanel->Color=clGreen;
                }
                else
                {
                    ReceipeDefault_PARA[j].PPanel->Color=clRed;
                    bReceipeDefaultDiffShow=true;
                }
                //Sam 20201209 : Default Recipe ChangeLog
                //==>
                if(CosFunction.bRecipeParameterDefaultLog)
                {
                    if(aName->Strings[i].Pos("Index_")==1)
                        iVisibleCnt[eRPVisibleIndex]++;
                    else if(aName->Strings[i].Pos("Input_")==1)
                        iVisibleCnt[eRPVisibleInput]++;
                    else if(aName->Strings[i].Pos("Output_")==1)
                        iVisibleCnt[eRPVisibleOutput]++;
                    else if(aName->Strings[i].Pos("Shuttle_")==1)
                        iVisibleCnt[eRPVisibleShuttle]++;
                    else if(aName->Strings[i].Pos("TrayArm_")==1)
                        iVisibleCnt[eRPVisibleTrayArm]++;
                    else if(aName->Strings[i].Pos("AutoClean_")==1)
                        iVisibleCnt[eRPVisibleAutoClean]++;
                    else if(aName->Strings[i].Pos("Yield_")==1)
                        iVisibleCnt[eRPVisibleYield]++;
                    else if(aName->Strings[i].Pos("Tester_")==1)
                        iVisibleCnt[eRPVisibleTester]++;

                    ReceipeDefault_PARA[j].PLabel->Visible=true;
                    ReceipeDefault_PARA[j].PPanel->Visible=true;
                    asMsg.sprintf("%s,%s,%s",aName->Strings[i],aDefault->Strings[i],aValue->Strings[i]);
                    slDefaultRecipeChangeLog->Add(asMsg);
                }
                //<==
                //Sam 20201209 : Default Recipe ChangeLog
            }
        }
    }
    //Sam 20201209 : Default Recipe ChangeLog
    //==>
    if(CosFunction.bRecipeParameterDefaultLog)
    {
        for(int i=0;i<eRPVisibleCnt;i++)
        {
            if(iVisibleCnt[i]>0)
                tsVisible[i]->TabVisible=true;
            else
                tsVisible[i]->TabVisible=false;
        }

        try
        {
            MyForceDirectories(asDefaultRecipeChangeLogPath);
            aPath.sprintf("%s\\%s_%s.txt", asDefaultRecipeChangeLogPath, IniConfig.SocketHandlerID, Now().FormatString("yyyymmddhhnnss"));

            if(bHasChange || RPLogRecord)  //有改變才需要保存
            {
                slDefaultRecipeChangeLog->SaveToFile(aPath);
                if(IniConfig.bN14_20_DefaultRecipeChangeLog)
                {
                    aPath.sprintf("%s\\%s_%s.txt", IniConfig.asN14_20_ChangeLogPath, IniConfig.SocketHandlerID, Now().FormatString("yyyymmddhhnnss"));
                    slDefaultRecipeChangeLog->SaveToFile(aPath);
                }
            }
        }
        catch(...)
        {
        }
    }
    slDefaultRecipeChangeLog->Clear();
    delete slDefaultRecipeChangeLog;
    //<==
    //Sam 20201209 : Default Recipe ChangeLog

    if(bReceipeDefaultDiffShow)
        fRecipeDefault->Show();
    else if(bReceipeDefaultDiffShow==false)
        fRecipeDefault->Close();
}
#endif // GATE (R-6)

#if 0 // GATE (R-7) ShowMonitoredParameter -- golden :396-415.  WRITES A SHARED
      // RUNTIME FILE: five WriteDataToFile(...) calls into the HARD-CODED
      // literal "D:\\HT9045\\CurrentSetupData.txt" (golden :398), the first
      // with the truncate flag `true`.  That file is on the project's
      // shared-runtime-parameter read-only list (AGENTS.md / CLAUDE.md).
      // ⚠ A HARD-CODED path cannot be redirected by `--dry` -- same hazard
      // class as the ReadLastDataFile / WriteLastDataFile literals.
      // Everything else it needs IS reachable (WriteDataToFile, GetLastOpenFN,
      // TestIF_File, LastSet), so this is a PURE SAFETY GATE.
void TfRPDefault::ShowMonitoredParameter()   //Isaac 20210222 : 全智要求把參數拉出來看，方便監控
{
    AnsiString Path=AnsiString().sprintf("D:\\HT9045\\CurrentSetupData.txt");
    AnsiString s="";

    s=AnsiString().sprintf("AutoCleanEnable=%d",TestIF_File.iAutoClean_Function);
    WriteDataToFile(Path, s, true);

    s=AnsiString().sprintf("Setup File Name=%s",GetLastOpenFN());
    WriteDataToFile(Path, s);

    s=AnsiString().sprintf("Socket sensor on/off=%d",TestIF_File.bEnSocketSensor);
    WriteDataToFile(Path, s);

    s=AnsiString().sprintf("Start Mode=%d",LastSet.iRunStartMode);
    WriteDataToFile(Path, s);

    s=AnsiString().sprintf("Shuttle Mode=%d",TestIF_File.iShuttleMode);         //Jimmychiu 20260223 : Shuttle Mode加在CurrentSetupData
    WriteDataToFile(Path, s);
}
#endif // GATE (R-7)
