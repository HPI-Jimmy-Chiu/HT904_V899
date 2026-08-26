// =============================================================================
//  forms/fTrayMapping.cpp  --  the READ-ONLY first batch of golden's
//  TfTrayMapping (cTrayMapping.cpp).  42 of golden's 150 method definitions
//  (355 of its 6,040 method-body lines -- QUOTE BOTH, see the header banner).
//
//  AI(W906-FW-TRAYMAP-W32) 20260826: new file, wave FW-TRAYMAP-W32.
//  Golden source of truth:
//    D:\HT9045\HT9011UC_Code_V3.33.906.0_20260618\cTrayMapping.cpp (7,268 lines,
//    cp950, decoded this wave, written out UTF-8, zero U+FFFD).
//  The full banner -- why the class is NOT called TfTrayMapping, the measured
//  denominator, the batch criterion, all 108 gates, the deviations, and the
//  symbol/archive analysis -- lives in forms/fTrayMapping.h.  This file carries
//  per-body pointer-back comments only.
//
//  ==========================================================================
//  *** HOW THE GATES ARE ENFORCED HERE -- READ BEFORE EDITING ***
//  ==========================================================================
//  Every gated method is DECLARED in forms/fTrayMapping.h and DELIBERATELY NOT
//  DEFINED here.  The LINKER is the interlock: calling one fails the build with
//  `undefined reference to TfTrayMappingForm::<method>`, naming the exact entry
//  point.  An empty stub would link and read as "already translated"; an
//  `#if 0` body is one keystroke from moving MLoaderY or writing an ini key.
//  TfTrayMappingForm has no virtual member, so an undefined member function
//  costs nothing until somebody calls it.
//
//  ⚠ COROLLARY THAT COST THIS WAVE TWO METHODS -- an ACTIVE body may not CALL
//  a gated one.  The call is emitted into this object file, so the undefined
//  reference becomes unconditional and every link that pulls this TU in fails.
//  `AddAOIMemeber` (golden :5419-5424) installs RecordMsgLaser /
//  RecordErrorMsgLaser as the AOI objects' log callbacks, and both of those are
//  GATE (G-2) file writers; `InitialAOIGroup` (golden :5398-5412) calls
//  AddAOIMemeber.  Both therefore moved from the delivered list to GATE (G-8)
//  DURING this wave, after the bodies were already written.  Recorded because
//  the plan and the output diverged, and because the same trap will catch the
//  next batch.
// =============================================================================
#include "forms/fTrayMapping.h"

#include "cmydef.h"             // USE_TRAY_MAPPING, DUMMY, N_INTEGER, N_PORT,
                                // MLoaderY / MTrayX / MMTrayY, INSTALL_OCR_YMot,
                                // C_LoaderUpPress / C_LoaderEdgePush / C_TrayY_Fixer
#include "MachineType.h"        // etmDeviceRemain (eTrayMap, :910), eocrYMotInstal (:514)
#include "CosFunction.h"        // CosFunction.bTrayDeviceCheckFromLoader (:366) /
                                //             bTrayMapFromLoader        (:367)
#include "LastSet.h"            // LastSet.iRealDummy (:127)
#include "cprod.h"              // Prod.iMLoaderYSurePos/CarPos/OCRPos (:1112-1114),
                                // TestIF_File.bCheckTrayIDBylot
#include "common.h"             // asTrayIDByLot (:68), OnlyNumberInPut (:403)
#include "canary_support.h"     // ShowMyMessage (:80)
#include "Motor/mymotor.h"      // MOT[] (:385) -- ReadPos/GetSpeed/HomeFlag/Tray.XItem
#include "mycylin.h"            // Cylinder[] (:176) -- OnStatus/OffStatus/OnSensor/OffSensor
#include "vclcompat/SysUtils.h" // FileExists
#include "vclcompat/LedCore.h"  // vclcompat's guarded TColor / cl* block
#include "forms/fQwertyKey.h"   // fQwertyKey->ShowQwertyKey

// golden's colour spellings are unqualified; vclcompat deliberately does not
// publish the colour constants globally (forms/fMotorTest.cpp:62-65 idiom).
using vclcompat::TColor;
using vclcompat::clRed;
using vclcompat::clGreen;

// ---------------------------------------------------------------------------
//  golden cTrayMapping.cpp:46 `int iCCDConntectionOkTask;` -- a file-scope
//  global with NO extern declaration anywhere in golden (grepped golden's own
//  cTrayMapping.cpp/.h and the whole 906 BCB6 tree this wave: 3 hits, all in
//  cTrayMapping.cpp -- the definition :46, ChangeTraySetupFile's write :970,
//  and TimerDownCCDTrayConnectTimer's `int &Task=iCCDConntectionOkTask;` :982).
//
//  DEVIATION: given internal linkage here (anonymous namespace) rather than
//  golden's external linkage.  Observationally identical -- no other TU in
//  golden or in this port can name it -- and it removes a duplicate-symbol
//  hazard if some future wave lands the rest of cTrayMapping.cpp in a second
//  TU.  Its ONLY reader (TimerDownCCDTrayConnectTimer) is GATE (G-1), so the
//  delivered ChangeTraySetupFile below currently writes a value nothing reads;
//  that is golden's structure, not a translation gap.
// ---------------------------------------------------------------------------
namespace {
int iCCDConntectionOkTask;      // golden :46
}   // namespace

// ===========================================================================
//  PORT CTOR -- FIELDS ONLY.  DEVIATION (D-2); golden's ctor body
//  (:59-195, 137 lines) is Init(), which is GATE (G-7).
//
//  Why the split is load-bearing for THIS form: golden's ctor reads
//  CUSTOMER_CODE / USE_MR_SYSTEM / USE_TRAY_MAPPING / USE_KEYENCE_EMPTY /
//  CosFunction / USE_LD_Rot_Arm, calls TrayMapSetDefaultView() -- which reads
//  MOT[MMTrayY].Tray.XItem, i.e. live motor config -- then InitialAOIGroup(),
//  InitialcbLaserScanMode(), SetSimulationState(), and `new TfRFID(...)`.
//  A translation unit's ctor runs BEFORE main(); forms/fLaserSensor already
//  cost this tree 88 SEGFAULTing ctest binaries for exactly that shape.
// ===========================================================================
TfTrayMappingForm::TfTrayMappingForm()
    : iLoaderYCWSpeedPerc(33)       // golden :166
    , bSimulate(false)              // golden :179 SetSimulationState(false) sets this
    , iSettingCount(0)              // golden :185
    , PageControl1(new TfTrayMappingPageControl())
    , sbtExit(new TSpeedButton())
    , sgTraymap(new TStringGrid())
    , sgSuckmap(new TStringGrid())
    , sgTrayDeviceCnt1(new TStringGrid())
    , sgTrayDeviceCnt2(new TStringGrid())
    , sgTrayDeviceCnt3(new TStringGrid())
    , edLoaderDC_Address(new TEdit())
    , edLoaderDC_Port(new TEdit())
    , BtLoaderDC_Connect(new TfTrayMappingBitBtn())
    , cbLaserScanMode(new TComboBox())
    , btnSimulateRemainIC(new TButton())
    , tsCommand(new TTabSheet())
    , cbPassAOI(new TCheckBox())
    , cbPassActive(new TCheckBox())
    , mtTrayMap(0)                  // TTMyTray is INCOMPLETE here -- cannot be new'd.
                                    // golden's own value comes from the .dfm; see
                                    // forms/fTrayMapping.h GATE notes.
    , lbResult(new TLabel())
    , btnAddRandom(new TButton())
    , sgTrayDeviceYield(new TStringGrid())
    , cbEnableTrayMapLaser(new TCheckBox())
    , cbEnableDeviceRemainLaser(new TCheckBox())
    , bDoTrayMapTune(false)         // golden h:607 (ctor leaves it default-初始; golden
                                    // never writes it in the ctor -- see GOLDEN NOTE 1)
    , listTrayIDByLot(new TStringList())   // golden :76-77
    , fShow(false)                  // golden :89
    , iAutoRetryCount(0)            // golden :186
{
    // No further statements ON PURPOSE.  Everything else golden's ctor does is
    // Init() -- DEVIATION (D-2).
}

// ===========================================================================
//  PORT DTOR -- EMPTY.  DEVIATION (D-6).
//  golden `__fastcall TfTrayMapping::~TfTrayMapping()` (:197-201) is
//      if(USE_LD_Rot_Arm==true) delete ldRFID;
//  and `ldRFID` (golden cTrayMapping.h:745, type TfRFID) cannot be declared in
//  this port -- TfRFID has ZERO port, GATE (G-5c).  The widgets new'd above are
//  deliberately NOT deleted here either: golden never deletes them (the VCL
//  form owns them), and this facade has no owner to hand them to.  Nothing
//  constructs a TfTrayMappingForm today, so nothing leaks today.
// ===========================================================================
TfTrayMappingForm::~TfTrayMappingForm()
{
}

// ===========================================================================
//  GOLDEN NOTE 1 -- `bDoTrayMapTune` (golden cTrayMapping.h:607) is read by
//  sbtExitClick (:1140) but is NEVER written by golden's ctor.  In BCB6 a
//  TForm's members are zero-filled by the VCL allocator, so golden's read is
//  well-defined-by-accident at false.  The port ctor initialises it explicitly
//  to false -- same observable value, but no longer dependent on allocator
//  behaviour.  Recorded rather than silently "fixed".
// ===========================================================================

//---------------------------------------------------------------------------
//  golden :287-299
void TfTrayMappingForm::TrayMapSetDefaultView()
{
    //JimmyChiu 20220219 : 整理程式碼
    //<=
    TrayMapSetDefault(sgTraymap);
    TrayMapSetDefault(sgSuckmap);
    TrayMapSetDefault(sgTrayDeviceCnt1);
    TrayMapSetDefault(sgTrayDeviceCnt2);
    TrayMapSetDefault(sgTrayDeviceCnt3);
    InitialTrayDeviceYield();
    //<=
    //JimmyChiu 20220219 : 整理程式碼
}
//---------------------------------------------------------------------------
//  golden :301-317.  Reads MOT[MMTrayY].Tray.XItem/YItem -- a PURE READ of the
//  live tray geometry; it commands nothing.  This is also the statement that
//  makes calling this from a static-initialisation-time ctor unsafe, hence
//  DEVIATION (D-2).
void TfTrayMappingForm::TrayMapSetDefault(TStringGrid *tsGrid)
{
    AnsiString str="";
    tsGrid->ColCount=MOT[MMTrayY].Tray.XItem+1;
    tsGrid->RowCount=MOT[MMTrayY].Tray.YItem+1;
    tsGrid->Cells[0][0]="X/Y";
    for(int i=1; i<=MOT[MMTrayY].Tray.XItem; i++)
    {
        str.sprintf("%d", i);
        tsGrid->Cells[i][0]=str.c_str();
    }
    for(int i=1; i<=MOT[MMTrayY].Tray.YItem; i++)
    {
        str.sprintf("%d", i);
        tsGrid->Cells[0][i]=str.c_str();
    }
}
//---------------------------------------------------------------------------
//  golden :968-971.  Writes the file-scope task cursor declared above; its only
//  reader is TimerDownCCDTrayConnectTimer, GATE (G-1).
void TfTrayMappingForm::ChangeTraySetupFile()
{
    iCCDConntectionOkTask=1;
}
//---------------------------------------------------------------------------
//  golden :1138-1148.  `Close()` is the DEVIATION (D-4) offline no-op.
void TfTrayMappingForm::sbtExitClick(TObject *Sender)
{
    if(bDoTrayMapTune)
    {
        ShowMyMessage("Tray Map Auto Tune need Finish !!", "需要完成 Tray Map Auto Tune!!");
        return;
    }
    sbtExit->Down=false;
    Close();
    fShow=false;
}
//---------------------------------------------------------------------------
//  golden :2814-2817
void TfTrayMappingForm::PageControl1Change(TObject *Sender)
{
    TrayMapSetDefaultView();
}
//---------------------------------------------------------------------------
//  golden :3274-3277
void TfTrayMappingForm::edTrayStartDelayClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 100, 30000);
}
//---------------------------------------------------------------------------
//  golden :3279-3282
void TfTrayMappingForm::edTrayAutoRetryClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1, 30);
}
//---------------------------------------------------------------------------
//  golden :3336-3339.  The `max` argument is a PURE READ of the tray geometry
//  (MOT[MMTrayY].Tray.XItem), same as TrayMapSetDefault above.
void TfTrayMappingForm::edtTrayMapCatchClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 1, true, 30, MOT[MMTrayY].Tray.XItem);
}
//---------------------------------------------------------------------------
//  golden :3341-3344
void TfTrayMappingForm::edtTrayMapShiftClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1000, 30000);
}
//---------------------------------------------------------------------------
//  golden :3406-3409
void TfTrayMappingForm::edtTrayIDReadShiftClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, -3000, 3000);
}
//---------------------------------------------------------------------------
//  golden :3719-3722.  GOLDEN NOTE 2: the range is `min=-10000, max=-50000`,
//  i.e. min > max.  Translated VERBATIM.  It looks like a transposed pair
//  (every sibling handler above passes min<max), and CheckRange with an
//  inverted window rejects every value -- but changing it is a behaviour
//  change on an operator-facing field, so it is reported, not fixed.
void TfTrayMappingForm::edtTrayDeciveCntStartClick(TObject *Sender)
{
  fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, -10000, -50000);
}
//---------------------------------------------------------------------------
//  golden :4764-4768
void TfTrayMappingForm::edTray_ID_PortMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_PORT);
}
//---------------------------------------------------------------------------
//  golden :4823-4848.  A DIAGNOSTIC SNAPSHOT: every term is a READ
//  (MOT[].ReadPos / GetSpeed / HomeFlag, Cylinder[].OnStatus / OffStatus /
//  OnSensor / OffSensor, Prod teach positions, INSTALL_OCR_YMot).  It commands
//  no motion and touches no cylinder solenoid -- reading Cylinder[].OnStatus()
//  is not the same thing as calling Cylinder[].On().
AnsiString TfTrayMappingForm::DumpLaserScanState()                              //AI(ht9045-v899) 20260505: 一次抓 MLoaderY/Cylinder/Sensor 狀態，方便客戶 log 還原當下情境
{
    AnsiString s;
    //AI(ht9045-v899) 20260505: 加入 MLoaderY HomeFlag 與 C_LoaderUpPress (固定機台壓桿) 狀態，協助診斷 auto-home timeout / 壓桿未退
    s.sprintf("MLY[Pos=%d Spd=%d Home=%d Tgt=Sure%d Car%d OCR%d] Cyl[UpPress On=%d/Off=%d Sen(On=%d Off=%d) | EdgePush On=%d/Off=%d Sen(On=%d Off=%d) | TrayY_Fixer On=%d/Off=%d Sen(On=%d Off=%d)] OCRMode=%d",
              MOT[MLoaderY].ReadPos(),
              MOT[MLoaderY].GetSpeed(),
              (int)MOT[MLoaderY].HomeFlag,
              Prod.iMLoaderYSurePos,
              Prod.iMLoaderYCarPos,
              Prod.iMLoaderYOCRPos,
              (int)Cylinder[C_LoaderUpPress].OnStatus(),
              (int)Cylinder[C_LoaderUpPress].OffStatus(),
              (int)Cylinder[C_LoaderUpPress].OnSensor(),
              (int)Cylinder[C_LoaderUpPress].OffSensor(),
              (int)Cylinder[C_LoaderEdgePush].OnStatus(),
              (int)Cylinder[C_LoaderEdgePush].OffStatus(),
              (int)Cylinder[C_LoaderEdgePush].OnSensor(),
              (int)Cylinder[C_LoaderEdgePush].OffSensor(),
              (int)Cylinder[C_TrayY_Fixer].OnStatus(),
              (int)Cylinder[C_TrayY_Fixer].OffStatus(),
              (int)Cylinder[C_TrayY_Fixer].OnSensor(),
              (int)Cylinder[C_TrayY_Fixer].OffSensor(),
              (int)(INSTALL_OCR_YMot==eocrYMotInstal));
    return s;
}
//---------------------------------------------------------------------------
//  golden :4850-4856.
//  ⚠ GOLDEN NOTE 3 -- TWO REAL DEFECTS, BOTH PRESERVED VERBATIM:
//    (a) `delete GetAOI(i)` deletes through `TfAOI*`, and TfAOI has virtual
//        members but a NON-virtual destructor (TfAOILaserScan.h:161, faithful
//        to golden TfAOILaserScan.h:150).  Every object in mapAOI is really a
//        TfTrayDeviceCheckFromLoader or TfTrayMapFromLoader, so this is
//        undefined behaviour in golden AND here.  On MinGW with -Wall this
//        body emits `-Wdelete-non-virtual-dtor`.  THAT WARNING IS THE POINT:
//        do NOT silence it by adding `virtual` to ~TfAOI -- that changes the
//        vtable layout of a class this port has already translated, and the
//        defect belongs to golden.  Fixing it is a decision for the user.
//    (b) mapAOI is never cleared, so after this runs every entry dangles and
//        the very next GetAOI(i) returns a freed pointer.  golden's only
//        caller is FormDestroy (:335-405), where the form is going away
//        anyway, which is why it has never bitten.
void TfTrayMappingForm::ClearAllAOI()
{
    for(int i=0;i<GetAOISize();i++)
    {
        delete GetAOI(i);
    }
}
//---------------------------------------------------------------------------
//  golden :4858-4862.  Out-of-range indices are CLAMPED TO 0, not rejected --
//  including negative ones.  Verbatim.
TfAOI* TfTrayMappingForm::GetAOI(int iIndex)
{
    iIndex=(iIndex>=GetAOISize() || iIndex<0)?0:iIndex;
    return mapAOI[iIndex];
}
//---------------------------------------------------------------------------
//  golden :4864-4872.  Note the match is `asName.Pos(asName)>0`, i.e. a
//  SUBSTRING match on the stored name, not equality.  Verbatim.
TfAOI* TfTrayMappingForm::GetAOI(AnsiString asName)
{
    for(int i=0;i<GetAOISize();i++)
    {
        if(GetAOI(i)->asName.Pos(asName)>0)
            return GetAOI(i);
    }
    return NULL;
}
//---------------------------------------------------------------------------
//  golden :4883-4896.  Read-only: it asks the socket whether it is connected
//  and paints the button.  DEVIATION (D-5) on the parameter type.
void TfTrayMappingForm::UpdateConnectionState(TfTrayMappingBitBtn * btn)
{
    if(GetNowAOI()==NULL)return;
    if(GetNowAOI()->socketClient->IsConnected())
    {
        btn->Caption="Disconnect";
        btn->Font->Color=clRed;
    }
    else
    {
        btn->Caption="Connect";
        btn->Font->Color=clGreen;
    }
}
//---------------------------------------------------------------------------
//  golden :4954-4965
void TfTrayMappingForm::SetSimulationState(bool bswitch)
{
    bSimulate=bswitch;
    if(bswitch)
    {
        btnSimulateRemainIC->Caption="Running...";
    }
    else
    {
        btnSimulateRemainIC->Caption="Run";
    }
}
//---------------------------------------------------------------------------
//  golden :5298-5309.  GOLDEN NOTE 4: an EMPTY string returns TRUE (the loop
//  body never runs).  Its callers -- btnSimulateRemainICClick (:4922, GATE
//  G-3) -- then atoi("") == 0.  Verbatim.
bool TfTrayMappingForm::IsNumber(AnsiString& asValue)
{
    asValue=asValue.Trim();
    for(int i=0;i<asValue.Length();i++)
    {
        if (asValue.c_str()[i] >= '0' && asValue.c_str()[i] <= '9')
        {/*pass*/}
        else
            return false;
    }
    return true;
}
//---------------------------------------------------------------------------
//  golden :5311-5316.  `Key=NULL` is golden's own spelling; the build already
//  carries -Wno-conversion-null (build/CMakeFiles/ht9045_sm.dir/flags.make),
//  so it is kept verbatim rather than rewritten to `Key=0`.
void TfTrayMappingForm::edLoopTimesKeyPress(TObject *Sender,
      char &Key)
{
    if(OnlyNumberInPut(Key)==false)
        Key=NULL;
}
//---------------------------------------------------------------------------
//  golden :5318-5321
void TfTrayMappingForm::cbLaserScanModeChange(TObject *Sender)
{
    ResetcbLaserScanMode();
}
//---------------------------------------------------------------------------
//  golden :5323-5337.  GOLDEN NOTE 5: the "Check Device" arm additionally
//  requires CosFunction.bTrayDeviceCheckFromLoader; when that flag is false the
//  else-arm returns the TRAY-MAP AOI even though the combo still reads
//  "Check Device".  Verbatim -- this asymmetry decides which AOI object a
//  dozen gated call sites talk to.
TfAOI* TfTrayMappingForm::GetNowAOI()
{
    if(USE_TRAY_MAPPING==etmDeviceRemain)
    {
        if(CosFunction.bTrayDeviceCheckFromLoader && cbLaserScanMode->Text==AnsiString("Check Device"))
        {
            return GetAOI(eAOIType_TrayDeviceCheckLaser);
        }
        else                                                                    //Tray Map
        {
            return GetAOI(eAOIType_TrayMapLaser);
        }
    }
    return NULL;
}
//---------------------------------------------------------------------------
//  golden :5345-5355.  Note this is a PREDICATE, not an action: it answers
//  "may the laser-scan state machine command the Loader-Y motor?".  It moves
//  nothing itself.  `cbPassActive` only counts while the form is on screen
//  (fShow); off-screen the answer comes from LastSet.iRealDummy.
bool TfTrayMappingForm::IsRunMotor()
{
    if(fShow)
    {
        if(cbPassActive->Checked)
            return false;
    }
    else if(LastSet.iRealDummy==DUMMY)
        return false;
    return true;
}
//---------------------------------------------------------------------------
//  golden :5357-5360
bool TfTrayMappingForm::IsCommAOI()
{
    return (cbPassAOI->Checked && fShow)==false;
}
//---------------------------------------------------------------------------
//  golden :5362-5368.  The MASTER GATE of the whole Tray-Map-laser branch
//  family (acatchtray_shims.h:284-289 lists its 6 golden call sites in
//  asendic_Loader.cpp).  Note the DUMMY term: in dummy mode the laser scan is
//  reported as not running at all.
bool TfTrayMappingForm::IsRunTrayMapLaser()
{
    return (USE_TRAY_MAPPING==etmDeviceRemain &&
            CosFunction.bTrayMapFromLoader &&
            cbEnableTrayMapLaser->Checked &&
            (LastSet.iRealDummy!=DUMMY));
}
//---------------------------------------------------------------------------
//  golden :5370-5376
bool TfTrayMappingForm::IsRunDeviceRemainLaser()
{
    return (USE_TRAY_MAPPING==etmDeviceRemain &&
            CosFunction.bTrayDeviceCheckFromLoader &&
            cbEnableDeviceRemainLaser->Checked  &&
            (LastSet.iRealDummy!=DUMMY));
}
//---------------------------------------------------------------------------
//  golden :5378-5386
void TfTrayMappingForm::InitialcbLaserScanMode()
{
    cbLaserScanMode->Items->Clear();
    for(int i=0;i<GetAOISize();i++)
    {
        cbLaserScanMode->Items->Add(GetAOI(i)->asName);
    }
    cbLaserScanMode->Text=(GetAOISize()>0)?(GetAOI(0)->asName):AnsiString("");
}
//---------------------------------------------------------------------------
//  golden :5388-5396.  Reads the AOI's stored address/port back onto the form.
//  `GetSocketAddress()`/`GetSocketPort()` are plain member reads
//  (EJ1N/uSocketServerClient.h:147/:149) -- no socket call.
void TfTrayMappingForm::ResetcbLaserScanMode()
{
    TfAOI* aoi=GetAOI(cbLaserScanMode->Text);
    if(aoi==NULL)
        return;

    edLoaderDC_Address->Text=aoi->socketClient->GetSocketAddress();
    edLoaderDC_Port->Text=aoi->socketClient->GetSocketPort();
}
//---------------------------------------------------------------------------
//  golden :5414-5417
void TfTrayMappingForm::ShowLaserScanSheet(TTabSheet *tsShow)
{
    PageControl1->ActivePage=tsShow;
}
//---------------------------------------------------------------------------
//  golden :5457-5460.  A pure read of the AOI object's cached GO/NOGO result
//  (TfTrayDeviceCheckFromLoader::IsRemainIC returns bIsRemainIC,
//  TfAOILaserScan.h).  It triggers no scan.
bool TfTrayMappingForm::GetTrayDeviceCheckIsRemainIC()
{
    return GetAOI(eAOIType_TrayDeviceCheckLaser)->IsRemainIC();
}
//---------------------------------------------------------------------------
//  golden :5467-5470.  `mtTrayMap` is a TTMyTray* and TTMyTray is an INCOMPLETE
//  type in this port (TfAOILaserScan.h GATE (1) -- no compiled definition
//  anywhere; deferred tree-wide to W7-C4).  Passing the pointer through
//  compiles; the dereference happens inside TfTrayMapFromLoader::HasNullIC,
//  which is that unit's own gate, not this one's.  Offline mtTrayMap is NULL
//  (the port ctor cannot new an incomplete type and golden's value comes from
//  the .dfm), so this is only meaningful once W7-C4 lands a real TTMyTray.
bool TfTrayMappingForm::GetTrayMapHasNullIC()
{
    return GetAOI(eAOIType_TrayMapLaser)->HasNullIC(mtTrayMap);
}
//---------------------------------------------------------------------------
//  golden :5581-5593
void TfTrayMappingForm::InitialTrayDeviceYield()
{
    sgTrayDeviceYield->ColCount=3;
    sgTrayDeviceYield->ColWidths[0]=50;                                         //Yield
    sgTrayDeviceYield->ColWidths[1]=150;                                        //MO
    sgTrayDeviceYield->ColWidths[2]=60;                                         //%
    sgTrayDeviceYield->RowCount=2;
    //title
    int colPos=0;
    SetTtileTrayDeviceYield(colPos++,0,"Yield");
    SetTtileTrayDeviceYield(colPos++,0,"MO");
    SetTtileTrayDeviceYield(colPos++,0,"%");
}
//---------------------------------------------------------------------------
//  golden :5595-5599.  GOLDEN NOTE 6: the `iscol`/`isrow` origin locals are
//  the ONLY difference between this and SetValueTrayDeviceYield (0,0 here vs
//  1,1 there) -- i.e. titles go in row 0 / col 0, values in row 1 / col 1.
void TfTrayMappingForm::SetTtileTrayDeviceYield(int icol,int irow,AnsiString sValue)
{
    int iscol=0,isrow=0;
    sgTrayDeviceYield->Cells[iscol+icol][isrow+irow]=sValue;
}
//---------------------------------------------------------------------------
//  golden :5601-5605
void TfTrayMappingForm::SetValueTrayDeviceYield(int icol,int irow,AnsiString sValue)
{
    int iscol=1,isrow=1;
    sgTrayDeviceYield->Cells[iscol+icol][isrow+irow]=sValue;
}
//---------------------------------------------------------------------------
//  golden :5607-5619.  GOLDEN NOTE 7: the retry allowance is gated on
//  IsRunDeviceRemainLaser() ONLY -- there is no IsRunTrayMapLaser() term -- so
//  a tray-map laser scan never auto-retries even when the AOI object's own
//  bEnableRetryCount is set.  Verbatim.
//
//  DEVIATION (D-8), the ONLY token changed in this body: golden writes
//  `==True`.  `True` is BCB6's Delphi-compatibility constant (System.hpp,
//  `static const Shortint True = 1`-style); it has ZERO port -- grepped the
//  whole tree 20260826 for `#define True` / `const bool True` / `True = true`
//  and for bare `True` inside vclcompat: no definition, and this file was the
//  only `== True` in any .cpp.  Translated to `true`, which is what BCB6's
//  `True` IS.  Recorded because it is a token substitution, not a copy.
bool TfTrayMappingForm::CheckRunAutoRetry(int iAOIType)
{
    if(IsRunDeviceRemainLaser() &&
       GetAOI(iAOIType)->bEnableRetryCount==true &&
       iAutoRetryCount<GetAOI(iAOIType)->iRemainICLaserRetryCount)
    {
        return true;
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
//  golden :5544-5562.  A hidden-menu unlock: right-click twice then left-click
//  reveals the raw-AOI-command tab.  Pure UI state.  Signature keeps golden's
//  full (TMouseButton, TShiftState) parameter list per vclcompat/ShiftState.h's
//  stated convention for NEWLY translated handlers.
void TfTrayMappingForm::Label18MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Button==mbRight)
        iSettingCount++;
    else if(Button==mbLeft)
    {
        if(iSettingCount==2)
        {
            tsCommand->TabVisible=!tsCommand->TabVisible;
            btnAddRandom->Visible=!btnAddRandom->Visible;
        }
        iSettingCount=999;
    }
    else
        iSettingCount=999;
    if(iSettingCount>2)
        iSettingCount=0;
}
//---------------------------------------------------------------------------
//  golden :5752-5755
bool TfTrayMappingForm::DoAutoSkip(int iAOIType)
{
    return GetAOI(iAOIType)->bRemainICLaserAutoSkip;
}
//---------------------------------------------------------------------------
//  golden :5757-5767.  GOLDEN NOTE 8: the NULL guard here is dead in practice --
//  GetAOI(int) clamps out-of-range indices to 0 and `mapAOI[0]` on an EMPTY map
//  default-inserts a null TfAOI*, so the guard DOES fire before any AOI is
//  registered.  Kept verbatim; note that its siblings (DoAutoSkip,
//  SetRealRunInitialStart, GetRealRunInitialStart) have NO such guard and would
//  dereference that same null.  Golden asymmetry, preserved.
bool TfTrayMappingForm::DoRunAtInitialStart(int iAOIType)
{
    if(GetAOI(iAOIType)==NULL)
    {
        return false;
    }
    else
    {
        return GetAOI(iAOIType)->bNeedCheckInitialStart;
    }
}
//---------------------------------------------------------------------------
//  golden :5769-5772
void TfTrayMappingForm::SetRealRunInitialStart(int iAOIType,bool bSwitch)
{
    GetAOI(iAOIType)->bRealRunCheckInitialStart=bSwitch;
}
//---------------------------------------------------------------------------
//  golden :5774-5777
bool TfTrayMappingForm::GetRealRunInitialStart(int iAOIType)
{
    return GetAOI(iAOIType)->bRealRunCheckInitialStart;
}
//---------------------------------------------------------------------------
//  golden :5799-5802
void TfTrayMappingForm::ShowCheckDeviceResult(AnsiString sValue)
{
    lbResult->Caption=sValue;
}
//---------------------------------------------------------------------------
//  golden :7241-7266.  READ-ONLY file access: FileExists + LoadFromFile, then
//  an in-memory Sort/Find.  The WRITER of the same file is ClearTrayIDByLot
//  (:7104-7109, `listTrayIDByLot->SaveToFile(asTrayIDByLot)`) and it is
//  GATE (G-2).
//
//  ⚠ SEMANTIC NOTE, and it disagrees with the shim.  Returning FALSE here means
//  "this tray ID is a DUPLICATE"; the two Keyence arms in golden
//  asendic_Color.cpp (:747, :1073) test `DoTrayIDCheck(...)==false` and route
//  the operator to remove the tray.  acatchtray_shims.h:296-306 hard-codes the
//  shim's offline answer to TRUE ("no duplicate") as a DELIBERATE decision.
//  This body is the real thing: with TestIF_File.bCheckTrayIDBylot off it also
//  returns true, so the two agree in the default configuration -- but they
//  DIVERGE the moment that recipe flag is on and the file has a match.  Anyone
//  wiring this facade in must decide which answer the engine gets.
bool TfTrayMappingForm::DoTrayIDCheck(AnsiString Str)                           //JerryYang 20250120 : add
{
    int iIndex=0,iPos=0;
    AnsiString Str2, Str1, Str3;
    if(TestIF_File.bCheckTrayIDBylot)
    {
        if(FileExists(asTrayIDByLot))                                           //Steven 20160505 : 加上保護, 不然開程式會跳Error
        {
            listTrayIDByLot->Clear();
            listTrayIDByLot->LoadFromFile(asTrayIDByLot);
        }

        listTrayIDByLot->Sort();
        iPos=Str.Pos("\r\n");
        if(iPos>0)
            Str1=Str.SubString(1, iPos-1);
        else
            Str1=Str;
        if(listTrayIDByLot->Find(Str1, iIndex))
        {
            return false;
        }
    }

    return true;
}
//---------------------------------------------------------------------------
//  END OF DELIVERED BODIES.
//
//  Everything else golden defines in cTrayMapping.cpp is GATED: declared in
//  forms/fTrayMapping.h, deliberately absent here, so the linker names the
//  exact entry point if anybody calls it.  See that header's GATE REGISTER for
//  all 108 with their evidence.
//---------------------------------------------------------------------------
