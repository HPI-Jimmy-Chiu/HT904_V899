// =============================================================================
//  forms/fOffSet.cpp  --  definitions for the fOffSet facade
//
//  AI(W906-W7-F0) 20260728: split out of FormsFacade.cpp by the W7-F0 refactor
//  (docs/W7_UI_ARCHITECTURE_PLAN.md SS6-F0-d).  Bodies moved VERBATIM.
// =============================================================================
#include "forms/fOffSet.h"

// --- W6.2: TfOffSet --------------------------------------------------------
bool TfOffSet::UseAutoOffsetFunction(AnsiString /*sName*/) { return false; }  // no auto-offset offline
bool TfOffSet::UseInArmSetupTeach(int /*iWhich*/)          { return false; }  // no setup-teach offline
TfOffSet *fOffSet = new TfOffSet();

// =============================================================================
//  AI(W906-FW-NOTE-W33) 20260826: cOffSet Wave A -- 22 支唯讀方向 methods。
//  上面的 W7-F0 內容一行未動（append-only）。
//
//  波次範圍、分母重量方式、取批標準、DEVIATION 與完整 GATE REGISTER 都在
//  forms/fOffSet.h 的檔頭 banner，這裡不複製第二份（避免兩份漂移）。
//
//  INCLUDES -- 為什麼不需要動 CMakeLists：
//    cmydef.cpp / cprod.cpp / Config 相關全域都在 ht9045_globals；
//    common.cpp（OnlyNumberAndDotInPut / ReadIniData）在 ht9045_core；
//    forms/fQwertyKey.cpp 與 forms/fMain.cpp 與本檔同屬 ht9045_forms
//    （CMakeLists.txt:655/:699 vs 本檔 :661）。
//    ht9045_forms 已經宣告 PUBLIC vclcompat + ht9045_globals + ht9045_core
//    （CMakeLists.txt:714-721），所以本波沒有新的 link edge，
//    也沒有新的 translation unit。
//    這與 forms/fSetup.cpp 的既有作法完全一致。
// =============================================================================
#include "cmydef.h"          // CUSTOMER_CODE / USE_PICKER_COUNT / iSortUnloadT6 / Tempture_Hot
#include "cprod.h"           // InputLimit / Temperature / TrayForm / UserDefForm_File / DeviceForm
#include "Config.h"          // IniConfig
#include "LastSet.h"         // LastSet
#include "common.h"          // OnlyNumberAndDotInPut / ReadIniData
#include "MachineType.h"     // CC_KYEC_LEE / CC_ASE_CL / ep16Picker / eBtnAOI_*
#include "vclcompat/SysUtils.h"  // FloatToStr
#include "forms/fQwertyKey.h"    // fQwertyKey (:406) + ShowQwertyKey (:370)
#include "forms/fMain.h"         // fMain->cbSetupFileName

// ---------------------------------------------------------------------------
//  golden cOffSet.cpp:3219 `bool bhasKeyDown=false;` -- golden 的 file-scope
//  global，見 forms/fOffSet.h 的宣告註解。這裡照 golden 原字寫 =false。
// ---------------------------------------------------------------------------
bool bhasKeyDown = false;

// ---------------------------------------------------------------------------
//  golden cOffSet.cpp:856-882 -- SetXYPitchVCLVisible
//  純 widget Visible 串接 + 一個 USE_PICKER_COUNT 分支。golden 原文逐字。
//  ⚠ golden :858 的 `if(bVisible)` 只包住 edPitchX1 一行（Steven 20151103
//  的 32Site YPitch 修正），其餘六顆是無條件指派 —— 看起來像縮排錯誤，
//  但那是 golden 自己的行為，照翻不修。
//  ⚠ offline 語意：本樹沒有視窗，Visible 只是存值，不會有任何東西顯示或
//  隱藏。USE_PICKER_COUNT 的初值是 1（cmydef.cpp:3154），ep16Picker==3
//  （MachineType.h:1312），所以 offline 走的是 else 臂 —— 四顆 PitchX3/X4
//  一律 false，與 golden 在非 16-picker 機台上的行為相同。
// ---------------------------------------------------------------------------
void TfOffSet::SetXYPitchVCLVisible(bool bVisible)                              //Steven 20140510 : XY變距
{
    if(bVisible)                                                                //Steven 20151103 : Fixed for 32Site YPitch
    {
        edPitchX1->Visible  =bVisible;
    }
    lblPitchX2->Visible     =bVisible;
    lblPitchY->Visible      =bVisible;
    edPitchX2->Visible      =bVisible;
    edPitchY->Visible       =bVisible;
    lblPitchX2Range->Visible=bVisible;
    lblPitchYRange->Visible =bVisible;
    if(USE_PICKER_COUNT==ep16Picker)
    {
        lblPitchX3->Visible     =bVisible;
        lblPitchX4->Visible     =bVisible;
        edPitchX3->Visible      =bVisible;
        edPitchX4->Visible      =bVisible;
    }
    else
    {
        lblPitchX3->Visible     =false;
        lblPitchX4->Visible     =false;
        edPitchX3->Visible      =false;
        edPitchX4->Visible      =false;
    }
}

// golden cOffSet.cpp:2792-2795 -- `TObject *Sender` dropped, never read (D-1).
// `Key=NULL` 是 golden 原字，同 forms/fSetup.cpp:74 的既有先例；
// target 帶 -Wno-conversion-null（CMakeLists.txt:728）所以不會有警告。
void TfOffSet::edArmXKeyPress(char &Key)
{
    if(OnlyNumberAndDotInPut(Key)==false)   Key=NULL;
}

// golden cOffSet.cpp:2797-2801 -- `TObject *Sender` dropped, never read (D-1).
// Close() 是 offline no-op（D-2）：淨效果只有 sbtExit->Down=false。
void TfOffSet::sbtExitClick()
{
    sbtExit->Down=false;
    Close();
}

// golden cOffSet.cpp:2881-2886 -- Sender 直接宣告成 TEdit*（golden 自己就是
// cast 目標），`TMouseButton Button, TShiftState Shift, int X, int Y` 全部
// dropped（本體從未讀取）。D-1。
// ⚠ ShowQwertyKey 的 offline 語意見 forms/fQwertyKey.h BEHAVIOUR NOTE：
// ShowModal() 是 no-op，等於「使用者開了鍵盤立刻送出」，所以這裡的淨效果是
// 把 Sender->Text 過一次 CheckRange 再寫回（bCheckRange=true 且 N_DOUBLE）。
// 那是 golden 自己的 submit 路徑，不是本移植的發明。
// ⚠ fQwertyKey 是裸全域指標，全樹唯一建立點是 Public/HTEdit.cpp 的 lazy
// new，所以平時是 NULL；本支今天全樹 0 個 caller（不接線，facade 規則 3），
// 所以到不了。安全是因為沒接線，不是因為有守衛 —— 與 forms/fSetup.h
// GATE (WA-1) 已開閘的兄弟站點同一曝露。
void TfOffSet::IndexArmOffSet3MouseDown(TEdit *Sender)
{
    fQwertyKey->ShowQwertyKey(Sender, N_DOUBLE, 2, true, InputLimit.dContactHigh, InputLimit.dContactLow); //Steven 20140123 : Contact Height的Offset限制
    iIndexChange=3;                                                             //kevin 20211211 index socket value change
}

// golden cOffSet.cpp:3049-3053 -- 同上 D-1 / ShowQwertyKey 說明。
// golden 的兩個 `(double)` cast 保留原字（dShuttleHigh/Low 在 cprod.h 已是
// double，cast 是 golden 自己的冗字，照翻不清理）。
void TfOffSet::IndexArmOffSet5MouseDown(TEdit *Sender)
{
    fQwertyKey->ShowQwertyKey(Sender, N_DOUBLE, 2, true, (double)InputLimit.dShuttleHigh, (double)InputLimit.dShuttleLow);  //ChungHung 20150115 add for ATK +/-2 mm
}

// golden cOffSet.cpp:3055-3086 -- 同上 D-1。
// ⚠ GOLDEN ODDITY，照翻並記錄：:3071-3082 的三分支在 dTrayXPitch 與
// dTrayYPitch 相等時走 else，指派的仍是 dTrayXPitch，與第一個 if 臂同值 ——
// 也就是說 else if 那支只在 Y>X 時有作用，整段等價於 max(X,Y)。
// golden 這樣寫，本移植不「寫得更好」。
// ⚠ 最後一行把 max 當上限、-(max) 當下限傳給 ShowQwertyKey；注意 golden 的
// 參數順序是 (…, min, max)（見 forms/fQwertyKey.h:370），所以 golden 這裡
// 傳的是 min=dMaxDimension、max=-(dMaxDimension)，上下限是反的。
// 這同樣是 golden 自己的行為 —— 兄弟站點 IndexArmOffSet5MouseDown 傳的是
// (High, Low) 也是同一個反向 —— 照翻，不修。
void TfOffSet::edShtFor2DMouseDown(TEdit *Sender)
{
    double dMaxDimension;                                                       //Frank 20171030 (Steven) add Floating Shuttle調整Offset
    double dTrayXPitch=0.0;
    double dTrayYPitch=0.0;

    dTrayXPitch=UserDefForm_File[TrayForm.Loader.iTrayType].XPitch;
    dTrayYPitch=UserDefForm_File[TrayForm.Loader.iTrayType].YPitch;

    if(UserDefForm_File[TrayForm.Loader.iTrayType].XDivision==1)                //Ifor 20251017 add:2D Offset改用IC大小判斷
    {
        dMaxDimension=120.0;
    }
    else
    {
        if(dTrayXPitch>dTrayYPitch)
        {
            dMaxDimension=dTrayXPitch;
        }
        else if(dTrayYPitch>dTrayXPitch)
        {
            dMaxDimension=dTrayYPitch;
        }
        else
        {
            dMaxDimension=dTrayXPitch;
        }
    }

    fQwertyKey->ShowQwertyKey(Sender, N_DOUBLE, 2, true, dMaxDimension, -(dMaxDimension));
}

// golden cOffSet.cpp:3088-3092 -- 同上 D-1。
// ⚠ GOLDEN ODDITY：第三個實參 golden 寫 `0.0`，但 ShowQwertyKey 的第三個
// 形參是 `int iDP`（forms/fQwertyKey.h:370）。double 0.0 隱式轉成 int 0。
// 保留 golden 原字 `0.0` 而不「順手」改成 0 —— 值相同，字面忠實。
void TfOffSet::edPreciserOpenMouseDown(TEdit *Sender)
{
    fQwertyKey->ShowQwertyKey(Sender, N_DOUBLE, 0.0, true, 0.5, 0.0);  //Frank 20180410 (Steven) : InArm Preciser Station
}

// golden cOffSet.cpp:3220-3227 -- `TObject *Sender, WORD &Key, TShiftState
// Shift` 全部 dropped（本體從未讀取）。D-1。
// ⚠ CUSTOMER_CODE 的 offline 值決定這支等不等於 no-op：非 KYEC 機台上
// 整段不執行，bhasKeyDown 永遠留在 false。
void TfOffSet::IndexArmOffSet1KeyDown()
{
    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //Ifor 20190930 add : KYEC Index Offset 讀檔不卡Range，有修改才卡Range
    {
        bhasKeyDown=true;
    }
}

// golden cOffSet.cpp:3229-3237 -- 同上 D-1。
// 注意 iIndexChange=1 在 if 之外，非 KYEC 機台也會執行（golden 就是這樣）。
void TfOffSet::IndexArmOffSet3KeyDown()
{
    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //Ifor 20190930 add : KYEC Index Offset 讀檔不卡Range，有修改才卡Range
    {
        bhasKeyDown=true;
    }
    iIndexChange=1;                                                             //kevin 20211211 index socket value change
}

// golden cOffSet.cpp:3239-3247 -- 同上 D-1。
void TfOffSet::IndexArmOffSet1KeyUp()
{
    if(CUSTOMER_CODE==CC_KYEC_LEE && bhasKeyDown==true)                         //Ifor 20190930 add : KYEC Index Offset 讀檔不卡Range，有修改才卡Range
    {
        IndexArmOffSet1->Text="0";
        bhasKeyDown=false;
    }
}

// golden cOffSet.cpp:3249-3257 -- 同上 D-1。
void TfOffSet::IndexArmOffSet3KeyUp()
{
    if(CUSTOMER_CODE==CC_KYEC_LEE && bhasKeyDown==true)                         //Ifor 20190930 add : KYEC Index Offset 讀檔不卡Range，有修改才卡Range
    {
        IndexArmOffSet3->Text="0";
        bhasKeyDown=false;
    }
}

// golden cOffSet.cpp:3259-3262 -- `TObject *Sender` dropped, never read (D-1).
void TfOffSet::IndexArmOffSet3Change()
{
    iIndexChange=2;                                                             //kevin 20211211 index socket value change
}

// golden cOffSet.cpp:3264-3278 -- `TObject *Sender` dropped, never read (D-1).
// ⚠ offline：USE_Scanner_AOI_Inspection 由 database.cpp:1431 從 ini 讀入，
// 預設 eBtnAOI_UniInstall(0)，兩個比較都 false，所以整支是 no-op ——
// 這與 golden 在沒裝 Scanner AOI 的機台上完全相同，不是移植的降級。
// Height 只存值不排版（D-4）。
void TfOffSet::ck_ScanAOIClick()
{
    if((USE_Scanner_AOI_Inspection==(int)eBtnAOI_BottomInstall) ||
        (USE_Scanner_AOI_Inspection==(int)eBtnAOI_TopBottomInstall))                   //Jimmychiu 20240322 : Top & Bottom Inspect
    {
        if(ck_ScanAOI->Checked)
        {
            Pnl_ScanAOI->Height=81;
        }
        else
        {
            Pnl_ScanAOI->Height=33;
        }
    }
}

// golden cOffSet.cpp:3341-3345 -- 同上 D-1 / ShowQwertyKey 說明。
void TfOffSet::edOffsetContactForceMouseDown(TEdit *Sender)
{
    fQwertyKey->ShowQwertyKey(Sender, N_DOUBLE, 3, true, 0.0, 10.0);   //JimmyChiu 20220114 : Index 總壓力 Offset，加總後數值不顯示於Contact Form
}

// golden cOffSet.cpp:3347-3350 -- 純顯示。DeviceForm.fAireForce 是 double
// （cprod.h:1191），FloatToStr 取 double（vclcompat/SysUtils.h:40）。
void TfOffSet::ShowFinalAirForce()                                              //JimmyChiu 20220114 : Index 總壓力 Offset，加總後數值不顯示於Contact Form
{
    lbFinalAirForce->Caption=FloatToStr(DeviceForm.fAireForce);
}

// golden cOffSet.cpp:3363-3367 -- `TObject *Sender` dropped, never read (D-1).
// ActivePage 只存值不切頁（D-3）。
void TfOffSet::btnBackClick()
{
    btnBack->Down=false;
    PageControl1->ActivePage=tsInOutArmOffset;
}

// golden cOffSet.cpp:3376-3380 -- 同上 D-1 / D-3。
void TfOffSet::btnToIndexOffsetClick()
{
    btnToIndexOffset->Down=false;
    PageControl1->ActivePage=tsIndexOffset;
}

// golden cOffSet.cpp:3382-3386 -- 同上 D-1 / D-3。
void TfOffSet::btnToArmOffsetClick()
{
    btnToArmOffset->Down=false;
    PageControl1->ActivePage=tsInOutArmOffset;
}

// ---------------------------------------------------------------------------
//  golden cOffSet.cpp:3526-3543 -- Tri_Position_Offset
//  純讀 Temperature.fWorkTemperBase（cprod.h:1646 SYSTEM_TEMPERATURE）與
//  LastSet.iTemperature（LastSet.h:587），回傳一段檔名尾巴。不開檔、不寫檔。
//  ⚠ GOLDEN ODDITY，照翻並記錄：兩個臂的條件在 fWorkTemperBase==25 時
//  同時成立，第一個 if 先贏，所以 25 度整會拿到 "cool"。golden 這樣寫。
//  ⚠ 回傳字串前面的 "\\" 是 golden 原字（單一反斜線），呼叫端負責接在
//  目錄後面。本波沒有呼叫端 —— 這支今天全樹 0 個 caller。
// ---------------------------------------------------------------------------
AnsiString TfOffSet::Tri_Position_Offset()                                      //Ztex 2024.03.29 Tri Temp Position Offset
{
    AnsiString sResult="";

    if(Temperature.fWorkTemperBase<=25 && LastSet.iTemperature==Tempture_Hot)
    {
        sResult="\\Position Offset cool.Data";
    }
    else if(Temperature.fWorkTemperBase>=25 && LastSet.iTemperature==Tempture_Hot)
    {
        sResult="\\Position Offset Hot.Data";
    }
    else
    {
        sResult="\\Position Offset.Data";
    }
    return sResult;
}

// golden cOffSet.cpp:3545-3549 -- 同上 D-1 / D-3。
void TfOffSet::sbZcalibrationClick()
{
    sbZcalibration->Down=false;
    PageControl1->ActivePage=tsZCalibration;
}

// ---------------------------------------------------------------------------
//  golden cOffSet.cpp:3956-3994 -- LoadASECLOffsetIndexValues
//  純讀：ReadIniData 是 common.cpp 的 **純讀** 家族（回傳預設值時不回寫），
//  與 CheckAndReadIniData 不同 —— 後者在 key 不存在時會 WriteInteger 種回
//  檔案（common.cpp:603），那是寫入路徑，本波一律不用。這支用的是前者。
//  ⚠ 讀的檔是 D:\HT9045\Data\<setup>.ini，不是 system\Gerneral.ini，
//  所以與 --dry 保護的那條路無關；而且是唯讀，不會改動客戶檔案。
//  ⚠ offline：CUSTOMER_CODE 不是 CC_ASE_CL(933) 時 :3963-3964 直接 return 0，
//  連檔案都不會開。也就是說非日月光昆山機台上這支不碰磁碟。
//  ⚠ golden :3959 的 `AnsiString(...)` 是把 TComboBox->Text 這個 VCL
//  property 轉成 AnsiString 的 BCB6 慣用寫法；本樹 fMain->cbSetupFileName
//  ->Text 已經是 AnsiString，這個 cast 因此是恆等的，保留原字。
//  golden :3965-3967 的三行註解掉的 iGearRatio 一併保留（沿革）。
// ---------------------------------------------------------------------------
int TfOffSet::LoadASECLOffsetIndexValues(int iType, int iPos)
{
    AnsiString aPath, S;
    S=AnsiString(fMain->cbSetupFileName->Text);
    aPath.sprintf("D:\\HT9045\\Data\\%s.ini",S);
    int iOffsetPos=0;

    if(CUSTOMER_CODE!=CC_ASE_CL)
        return 0;
//    double iGearRatio=0.5;
//    if(IndexGearRatio==1)
//        iGearRatio=1.0;

    switch(iType)
    {
        case 14:
            if(iPos==0)
            {
                iOffsetPos = ReadIniData(aPath,"Index1","ShuttleLOffset",0);
            }
            else
            {
                iOffsetPos = ReadIniData(aPath,"Index1","ShuttleROffset",0);
            }
            break;

        case 15:
            if( iPos==0 )
            {
                iOffsetPos = ReadIniData(aPath,"Index2","ShuttleLOffset",0);
            }
            else
            {
                iOffsetPos = ReadIniData(aPath,"Index2","ShuttleROffset",0);
            }
            break;
    }
    return iOffsetPos;
}

// golden cOffSet.cpp:3997-4000 -- 同上 D-1 / ShowQwertyKey 說明。
void TfOffSet::edLodXClick(TEdit *Sender)
{
    fQwertyKey->ShowQwertyKey(Sender, N_DOUBLE, 6, true, 0.95, 1.05);
}

// =============================================================================
//  GATED -- 以下 12 支只在 forms/fOffSet.h 宣告，本檔**刻意不定義**，
//  由 linker 當互鎖（forms/fMotorTest.h 先例）：任何未來波次一旦接線呼叫
//  它們，會在 link 階段炸出 undefined reference，而不是靜默跑一個空殼。
//    CheckBox1Click / btnOffsetListClick        -- (O-1)(O-2) 呼叫未翻的大支
//    FormDestroy / GetOffsetPath                -- (O-3)(O-4) 相依在 ht9045_sm
//    sb_AutoOffset{Up,Down,Right,Left}Click     -- (O-5) 會寫檔並啟動機台
//    ClearIndexOffset                           -- (O-6) 寫教導／偏移值
//    edOffsetContactForceChange                 -- (O-7) fProductionInfo 缺 API
//    UseOutArmSetupTeach                        -- (O-8) 與既有 in-arm stub 不對稱
//    btnSortAuto1Click                          -- (O-9) Tag 值在本樹永遠讀 0
//  理由逐支見 forms/fOffSet.h 的 GATE REGISTER。
// =============================================================================
