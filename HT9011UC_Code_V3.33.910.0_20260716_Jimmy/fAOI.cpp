//------------------------------------------------------------------------------

#include "MachineDefine.h"
#pragma hdrstop

#include "fAOI.h"
#include "main.h"
#include "cprod.h"
#include "MyKitSuck.h"
#include "cmydef.h"
#include "aoutarm.h"
#include "aoutarm9045.h"
#include "MyMotor.h"
#include "note.h"
#include "common.h"
//#include "INPUT.h"
#include "myswitch.h"
#include "mysensor.h"
#include "cOffSet.h"
#include "mymessbox.h"
#include "csystem.h"
#include "atester.h"
#include "rs232.h"
#include "myQwertyKeyBoard.h"
//#include "aScannerAOI.h"
#include "uTrayEditForm.h"
#include "MachineType.h"
#include "uTimeTool.h"
#include "MyMotor.h"
#include "cmydef.h"
#include "uLotInfo.h"
#include "acarry.h"
#include "cinitial.h"
#include "cTrayMapping.h"
#include "mycylin.h"
#include "iosetview.h"
#include "cBinAoiSel.h"
//------------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "HTray"
#pragma link "SPComm"
#pragma resource "*.dfm"
TFrmAOI *FrmAOI;

int iTopViewTask=1;
int iTopViewInspectionTask=1;
int iPADViewTask=1;
int iPADViewInspectionTask=1;
int iBGAViewTask=1;
int iBGAViewInspectionTask=1;

int iScanAOITask=1;
int iScannerAOIInspectionTask=1;

int iAOITask=1;
const int eAOI_Top=1;
const int eAOI_PAD=2;
const int eAOI_BGA=3;
const int eAOI_SCANAOI=4;
bool bActiveSuck[MAX_ARM_Row][MAX_ARM_Col];

bool bVitroxBGAViewMap[MAX_ARM_Row][MAX_ARM_Col];                                 //Alick 20160713
bool bVitroxBGAViewUse[MAX_ARM_Row][MAX_ARM_Col];                                 //Alick 20160713
bool bBGAViewBySiteRes[MAX_ARM_Row][MAX_ARM_Col];                                 //Alick 20160713
bool bBGAViewBySiteResOld[MAX_ARM_Row][MAX_ARM_Col];                              //Alick 20160713
int  iBGAViewBySiteTotal[MAX_ARM_Row][MAX_ARM_Col];                               //Alick 20160713
bool bBGAViewByArmRes[2];                                                         //Alick 20160714
bool bBGAViewByArmResOld[2];                                                      //Alick 20160714
int  iBGAViewByArmTotal[2];                                                       //Alick 20160714
bool bBGAViewByArmDouChk[2]={false};                                              //Alick 20160715

bool bVitroxPADViewMap[MAX_ARM_Row][MAX_ARM_Col];                                 //Alick 20160713
bool bVitroxPADViewUse[MAX_ARM_Row][MAX_ARM_Col];                                 //Alick 20160713
bool bPADViewBySiteRes[MAX_ARM_Row][MAX_ARM_Col];                                 //Alick 20160713
bool bPADViewBySiteResOld[MAX_ARM_Row][MAX_ARM_Col];                              //Alick 20160713
int  iPADViewBySiteTotal[MAX_ARM_Row][MAX_ARM_Col];                               //Alick 20160713
bool bPADViewByArmRes[2];                                                         //Alick 20160714
bool bPADViewByArmResOld[2];                                                      //Alick 20160714
int  iPADViewByArmTotal[2];                                                       //Alick 20160714
bool bPADByArmDouChk[2]={false};                                                  //Alick 20160715

//bool bPickSH1Flag=false;                                                          //Alick 20160714    //Ifor 20191226 移至全域
//bool bPickSH2Flag=false;                                                          //Alick 20160714    //Ifor 20191226 移至全域
bool bBGABySiteFailCont=false;                                                    //Alick 20160714
bool bBGAByArmFailCont=false;                                                     //Alick 20160714
bool bPADBySiteFailCont=false;                                                    //Alick 20160714
bool bPADByArmFailCont=false;                                                     //Alick 20160714

TQPF_Timer hStartDelay;
TQPF_Timer hTimeOut;
bool bAOI_Fail_Unit=false;
int iAOI_Fail_Count=0;
int iAOIResult[2];
bool bTriggerAOI=false;
int iScanAOIIntervalCounter=9999;
bool bScanAOIBySiteFailCont=false;
bool bScanAOIByArmFailCont=false;
int  iScanAOIBySiteTotal[MAX_ARM_Row][MAX_ARM_Col];
bool bScanAOIByArmDouChk[2]={false};
int  iScanAOIByArmTotal[2];
int  iScanAOI_BackupBin[MAX_ARM_Row][MAX_ARM_Col];                              //AI(fAOI) 20260527 (RogerYang) : backup bin for retry
AnsiString asSavedRunModeForAOIReport="";                                     //Eastsun 20260515 F023: E1 save RunMode
TQPF_Timer hLGA_DelayTime;  //Ifor 20211022 add: AOI LGA Mode
int iScanLGAStep=0;         //Ifor 20211022 add: AOI LGA Mode
//Richard 20220817:Add Scanner_AOI add check Big Size IC Function
//==>
bool bplus =true;                        //判斷加or減
int irowX=0;
int icolumnY=0;
//<==
//Richard 20220817:Add Scanner_AOI add check Big Size IC Function

//Ifor 20200902 add: TFAMD Top AOI
//==>
int iTopScanAOITask=1;
int iTopScannerAOIInspectionTask=1;
int iTopScanAOIIntervalCounter=9999;
bool bTopScanAOIBySiteFailCont=false;
bool bTopScanAOIByArmFailCont=false;
int  iTopScanAOIBySiteTotal[MAX_ARM_Row][MAX_ARM_Col];
bool bTopScanAOIByArmDouChk[2]={false};
int  iTopScanAOIByArmTotal[2];
//<==
//Ifor 20200902 add: TFAMD Top AOI
int iLGAPitchXCnt,iLGAPitchYCnt,iLGAPitchX,iLGAPitchY;
TQPF_Timer hGetResultDelayTime;
//------------------------------------------------------------------------------
__fastcall TFrmAOI::TFrmAOI(TComponent* Owner)
    : TForm(Owner)
{
    mtDutCol_Vitrox->SetColorMap(0, clBtnFace);
    mtDutCol_Vitrox->SetColorMap(1, clGreen);
    mtDutRow_Vitrox->SetColorMap(0, clBtnFace);
    mtDutRow_Vitrox->SetColorMap(1, clGreen);
    mtDutOnOff_Vitrox->SetColorMap(0, clLime);
    mtDutOnOff_Vitrox->SetColorMap(1, clAqua);
    mtDutOnOff_Vitrox->SetColorMap(2, clSilver);
    mtDutCol_Vitrox1->SetColorMap(0, clBtnFace);
    mtDutCol_Vitrox1->SetColorMap(1, clGreen);
    mtDutRow_Vitrox1->SetColorMap(0, clBtnFace);
    mtDutRow_Vitrox1->SetColorMap(1, clGreen);
    mtDutOnOff_Vitrox1->SetColorMap(0, clLime);
    mtDutOnOff_Vitrox1->SetColorMap(1, clAqua);
    mtDutOnOff_Vitrox1->SetColorMap(2, clSilver);
    iQuotient=0;            //KenHsieh 20220830 : 修改add:AOI Tray Edit
    iRemainder=0;           //KenHsieh 20220830 : 修改add:AOI Tray Edit
    elParameter=NULL;       //AI(general) 20260411 (Rogeryang) : init NULL, 避免非TopBottom模式野指標

    if(USE_Scanner_AOI_Inspection==(int)eBtnAOI_TopBottomInstall)                    //Jimmychiu 20240322 : Top & Bottom Inspect
    {
        ttbInsp=new TTopBottomInspect();
        bSimulate=false;
        ttbInsp->SetRecordMsg(ShowMsgTopBtmCommLog);
        InitialTopBtmSG(sgICSmall);
        InitialTopBtmSG(sgICLarge);
        InitialTopBtmSGTitle(sgICSmall);
        InitialTopBtmSGTitle(sgICLarge);
        InitialEdList(edlsSamll,2,6);
        InitialEdList(edlsLarge,4,6);
        IntialParameter();
    }
    scroAOIFailCountLinkLotRunMode->HorzScrollBar->Visible = true;              //Eastsun 20260515
    fShow=false;
}
//------------------------------------------------------------------------------
void TFrmAOI::IntialParameter()
{
    iConFailCount=0;                                                            //Eastsun 20260515 F023: E4 init
    iConFailCountPicture[0]=0;                                                  //Eastsun 20260515 F023: E4 init
    iConFailCountPicture[1]=0;                                                  //Eastsun 20260515 F023: E4 init
    iConFailCountPicture[2]=0;                                                  //Eastsun 20260515 F023: E4 init
    iConFailCountPicture[3]=0;                                                  //Eastsun 20260515 F023: E4 init
    iAccumulatedCount=0;                                                        //Eastsun 20260515 F023: E4 init
    ttbInsp->iAOIYieldTotalCount=0;
    ttbInsp->iAOIYieldPassCount=0;
    elParameter=new HTEditList;
    bool bShow=true, bNoShow=false;
    bool bEnable=true;//, bDisable=false;
    bool bReadFromFile=true;//, bFixedValue=false;
    if(USE_Scanner_AOI_Inspection==(int)eBtnAOI_TopBottomInstall)                    //Jimmychiu 20240322 : Top & Bottom Inspect
    {
        elParameter->Add(rgTopBtmScannEnable,   &ttbInsp->iEnable,         ECInteger,  "TopBottomInspect",    "iEnable",            bShow,      bEnable, bReadFromFile, 0);
        elParameter->Add(rgActionMode,          &ttbInsp->iAction,         ECInteger,  "TopBottomInspect",    "iAction",            bShow,      bEnable, bReadFromFile, 0);

        elParameter->Add(edTopBtm_Address,      &ttbInsp->sSocketAddress,  ECText,     "TopBottomInspect",    "SocketAddress",      bShow,      bEnable, bReadFromFile, "172.16.8.200");
        elParameter->Add(edTopBtm_Port,         &ttbInsp->sSocketPort,     ECText,     "TopBottomInspect",    "SocketPort",         bShow,      bEnable, bReadFromFile, "5109");
        elParameter->Add(edCamaName,            &ttbInsp->sCamaName,       ECText,     "TopBottomInspect",    "sCamaName",          bShow,      bEnable, bReadFromFile, "CM1");
        elParameter->Add(edStartDelayTimeTBAOI, &ttbInsp->iStartDelayTime, ECInteger,  "TopBottomInspect",    "iStartDelayTime",    bShow,      bEnable, bReadFromFile, 5);
        elParameter->Add(edGetResultDelay,      &ttbInsp->iGetResultDelay, ECInteger,  "TopBottomInspect",    "iGetResultDelay",    bShow,      bEnable, bReadFromFile, 200);
        elParameter->Add(edTBTimeout,           &ttbInsp->iTimeout,        ECInteger,  "TopBottomInspect",    "iTimeout",           bShow,      bEnable, bReadFromFile, 10);
        elParameter->Add(edRotate0,             &ttbInsp->iRotate0Pos,     ECInteger,  "TopBottomInspect",    "iRotate0Pos",        bShow,      bEnable, bReadFromFile, -20);
        elParameter->Add(edRotate180,           &ttbInsp->iRotate180Pos,   ECInteger,  "TopBottomInspect",    "iRotate180Pos",      bShow,      bEnable, bReadFromFile, 3230);
        elParameter->Add(edTopBtnCenterX,       &ttbInsp->tbCenter.ppPosition.X,   ECInteger,  "TopBottomInspect",    "iTopBtnCenterX",  bShow,      bEnable, bReadFromFile, 977);
        elParameter->Add(edTopBtnCenterY,       &ttbInsp->tbCenter.ppPosition.Y,   ECInteger,  "TopBottomInspect",    "iTopBtnCenterY",  bShow,      bEnable, bReadFromFile, 80);
        elParameter->Add(edLightZTop,           &ttbInsp->iLight_Up_Z_Top,   ECInteger,  "TopBottomInspect",    "iLight_Up_Z_Top",  bShow,      bEnable, bReadFromFile, 0);
        elParameter->Add(edLightZBtm,           &ttbInsp->iLight_Up_Z_Btm,   ECInteger,  "TopBottomInspect",    "iLight_Up_Z_Btm",  bShow,      bEnable, bReadFromFile, 0);
        elParameter->Add(edCCDZTop,             &ttbInsp->iCCD_Up_Z_Top,   ECInteger,  "TopBottomInspect",    "iCCD_Up_Z_Top",      bShow,      bEnable, bReadFromFile, 0);
        elParameter->Add(edCCDZBtm,             &ttbInsp->iCCD_Up_Z_Btm,   ECInteger,  "TopBottomInspect",    "iCCD_Up_Z_Btm",      bShow,      bEnable, bReadFromFile, 0);

        elParameter->Add(edRotateKitAngOffset_In, &ttbInsp->iRotateKitAngOffset_In,    ECInteger,  "TopBottomInspect",    "iRotateKitAngOffset_In",       bShow,      bEnable, bReadFromFile, 0);//Eastsun 20260305 : RoataeKit Offset
        elParameter->Add(edRotateKitAngOffset_Out,&ttbInsp->iRotateKitAngOffset_Out,   ECInteger,  "TopBottomInspect",    "iRotateKitAngOffset_Out",      bShow,      bEnable, bReadFromFile, 0);//Eastsun 20260305 : RoataeKit Offset

        elParameter->Add(edZoffset,             &ttbInsp->iZPickOffset,    ECInteger,  "ZPickOffset",         "iZPickOffset",       bShow,      bEnable, bReadFromFile, 0);
        //action
        for(int i=0;i<2;i++)
        {
            elParameter->Add(edlsSamll[i][0],       &ttbInsp->tbpiLessThanOrEqual65mm[i].ppPosition.X,   ECInteger,     "TopBottomInspect", AnsiString().sprintf("Small_%d_PosX",i),     bShow,      bEnable, bReadFromFile, 0);
            elParameter->Add(edlsSamll[i][1],       &ttbInsp->tbpiLessThanOrEqual65mm[i].ppPosition.Y,   ECInteger,     "TopBottomInspect", AnsiString().sprintf("Small_%d_PosY",i),     bShow,      bEnable, bReadFromFile, 0);
            elParameter->Add(edlsSamll[i][2],       &ttbInsp->tbpiLessThanOrEqual65mm[i].bClampLT,       ECBool,        "TopBottomInspect", AnsiString().sprintf("Small_%d_LT",i),       bShow,      bEnable, bReadFromFile, "");
            elParameter->Add(edlsSamll[i][3],       &ttbInsp->tbpiLessThanOrEqual65mm[i].bClampLB,       ECBool,        "TopBottomInspect", AnsiString().sprintf("Small_%d_LB",i),       bShow,      bEnable, bReadFromFile, "");
            elParameter->Add(edlsSamll[i][4],       &ttbInsp->tbpiLessThanOrEqual65mm[i].bClampRT,       ECBool,        "TopBottomInspect", AnsiString().sprintf("Small_%d_RT",i),       bShow,      bEnable, bReadFromFile, "");
            elParameter->Add(edlsSamll[i][5],       &ttbInsp->tbpiLessThanOrEqual65mm[i].bClampRB,       ECBool,        "TopBottomInspect", AnsiString().sprintf("Small_%d_RB",i),       bShow,      bEnable, bReadFromFile, "");
        }
        for(int i=0;i<4;i++)
        {
            elParameter->Add(edlsLarge[i][0],       &ttbInsp->tbpiMoreThan65mm[i].ppPosition.X,   ECInteger,     "TopBottomInspect", AnsiString().sprintf("Large_%d_PosX",i),     bShow,      bEnable, bReadFromFile, 0);
            elParameter->Add(edlsLarge[i][1],       &ttbInsp->tbpiMoreThan65mm[i].ppPosition.Y,   ECInteger,     "TopBottomInspect", AnsiString().sprintf("Large_%d_PosY",i),     bShow,      bEnable, bReadFromFile, 0);
            elParameter->Add(edlsLarge[i][2],       &ttbInsp->tbpiMoreThan65mm[i].bClampLT,       ECBool,        "TopBottomInspect", AnsiString().sprintf("Large_%d_LT",i),       bShow,      bEnable, bReadFromFile, "");
            elParameter->Add(edlsLarge[i][3],       &ttbInsp->tbpiMoreThan65mm[i].bClampLB,       ECBool,        "TopBottomInspect", AnsiString().sprintf("Large_%d_LB",i),       bShow,      bEnable, bReadFromFile, "");
            elParameter->Add(edlsLarge[i][4],       &ttbInsp->tbpiMoreThan65mm[i].bClampRT,       ECBool,        "TopBottomInspect", AnsiString().sprintf("Large_%d_RT",i),       bShow,      bEnable, bReadFromFile, "");
            elParameter->Add(edlsLarge[i][5],       &ttbInsp->tbpiMoreThan65mm[i].bClampRB,       ECBool,        "TopBottomInspect", AnsiString().sprintf("Large_%d_RB",i),       bShow,      bEnable, bReadFromFile, "");
        }

        elParameter->Add(cbConsecutiveFail,         &ttbInsp->bConsecutiveFailCheck,    ECBool,         "Function Setting",     "Enable Consecutive Fail Check",    bShow,      bEnable, bReadFromFile, "");
        elParameter->Add(edConsecutiveFailCount,    &ttbInsp->iConsecutiveFailCount,    ECInteger,      "Function Setting",     "Consecutive Fail Count",           bShow,      bEnable, bReadFromFile, 0);

        elParameter->Add(cbConsecutiveFailPicture,      &ttbInsp->bConsecutiveFailPictureCheck,    ECBool,       "Function Setting",     "Enable Consecutive Fail Check(Picture)",    bShow,      bEnable, bReadFromFile, "");
        elParameter->Add(edConsecutiveFailPictureCount, &ttbInsp->iConsecutiveFailPictureCount,    ECInteger,    "Function Setting",     "Consecutive Fail Count(Picture)",           bShow,      bEnable, bReadFromFile, 0);

        elParameter->Add(cbAccumulatedFail,      &ttbInsp->bAccumulatedFailCheck,    ECBool,       "Function Setting",     "Enable Accumulated Fail Check",    bShow,      bEnable, bReadFromFile, "");
        elParameter->Add(edAccumulatedFailCount, &ttbInsp->iAccumulatedFailCount,    ECInteger,    "Function Setting",     "Accumulated Fail Count",           bShow,      bEnable, bReadFromFile, 0);

        elParameter->Add(cbIntervalCheck,           &ttbInsp->bIntervalCheck,           ECBool,         "Function Setting",     "Enable Interval Check",            bNoShow,      bEnable, bReadFromFile, "");
        elParameter->Add(edIntervalCount,           &ttbInsp->iIntervalCount,           ECInteger,      "Function Setting",     "Interval Count",                   bNoShow,      bEnable, bReadFromFile, 0);

        elParameter->Add(edAOIFailSetBin,           &ttbInsp->iAOIFailSetBin,           ECInteger,      "Function Setting",     "AOI Fail Set Bin",                 bNoShow,    bEnable, bReadFromFile, 15);
        elParameter->Add(cbAOIBin,                  &ttbInsp->bAOIFailBin,    ECBool,         "Function Setting",     "Enable AOI Fail Bin",    bShow,      bEnable, bReadFromFile, "");

        AOIFailList.clear();
        for(int i=0;i<iAOIFailCheckMax;i++)                                     //Eastsun 20260515
        {
            TAOIFailCountComboItem item;
            item.cbConsecutiveFail             = new TCheckBox(this);
            item.edConsecutiveFailCount             = new TEdit(this);
            item.cbIntervalCheck             = new TCheckBox(this);
            item.edIntervalCount             = new TEdit(this);
            item.cbConsecutiveFailPicture             = new TCheckBox(this);
            item.edConsecutiveFailPictureCount             = new TEdit(this);
            item.cbAccumulatedFail             = new TCheckBox(this);
            item.edAccumulatedFailCount             = new TEdit(this);
            AOIFailList.push_back(item);

            elParameter->Add(AOIFailList[i].cbConsecutiveFail,       &ttbInsp->bConsecutiveFailCheck_List[i],   ECBool,     "Function Setting", AnsiString().sprintf("Enable Consecutive Fail Check_%d",i),     bShow,      bEnable, bReadFromFile, "");
            elParameter->Add(AOIFailList[i].edConsecutiveFailCount,       &ttbInsp->iConsecutiveFailCount_List[i],   ECInteger,     "Function Setting", AnsiString().sprintf("Consecutive Fail Count_%d",i),     bShow,      bEnable, bReadFromFile, 0);

            elParameter->Add(AOIFailList[i].cbConsecutiveFailPicture,       &ttbInsp->bConsecutiveFailPictureCheck_List[i],   ECBool,     "Function Setting", AnsiString().sprintf("Enable Consecutive Fail Check(Picture)_%d",i),     bShow,      bEnable, bReadFromFile, "");
            elParameter->Add(AOIFailList[i].edConsecutiveFailPictureCount,       &ttbInsp->iConsecutiveFailPictureCount_List[i],   ECInteger,     "Function Setting", AnsiString().sprintf("Consecutive Fail Count(Picture)_%d",i),     bShow,      bEnable, bReadFromFile, 0);

            elParameter->Add(AOIFailList[i].cbAccumulatedFail,       &ttbInsp->bAccumulatedFailCheck_List[i],   ECBool,     "Function Setting", AnsiString().sprintf("Enable Accumulated Fail Check_%d",i),     bShow,      bEnable, bReadFromFile, "");
            elParameter->Add(AOIFailList[i].edAccumulatedFailCount,       &ttbInsp->iAccumulatedFailCount_List[i],   ECInteger,     "Function Setting", AnsiString().sprintf("Accumulated Fail Count_%d",i),     bShow,      bEnable, bReadFromFile, 0);

            elParameter->Add(AOIFailList[i].cbIntervalCheck,       &ttbInsp->bIntervalCheck_List[i],   ECBool,     "Function Setting", AnsiString().sprintf("Enable Interval Check_%d",i),     bNoShow,      bEnable, bReadFromFile, "");
            elParameter->Add(AOIFailList[i].edIntervalCount,       &ttbInsp->iIntervalCount_List[i],   ECInteger,     "Function Setting", AnsiString().sprintf("Interval Count_%d",i),     bNoShow,      bEnable, bReadFromFile, 0);
        }
    }
    elParameter->InitialDataToEdit();
}
//------------------------------------------------------------------------------
int DoViewBinFunction()
{
    int iResult=0;

    if(FrmAOI->ComboBox1->Text=="Auto1")
        iResult=0;
    else if(FrmAOI->ComboBox1->Text=="Auto2")
        iResult=1;
    else if(FrmAOI->ComboBox1->Text=="Auto3")
        iResult=2;
    else if(FrmAOI->ComboBox1->Text=="Fix1")
        iResult=3;
    else if(FrmAOI->ComboBox1->Text=="Fix3")
        iResult=5;

    return iResult;
}
//------------------------------------------------------------------------------
bool DoMoveXY_TopViewSafePos()
{
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iXVariable[X_PITCH_COUNT];
    int iXPos=0, iYPos=0;
    int iYVariable  =0;
    int iMovePitchX =8000;
    int iMovePitchY =TestIF.iARM_Y_PITCH;
    int iOffsetPos  =-1;
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    if(XPitchIsStand())                                                         //X Pitch is 4000mm
        iMovePitchX=TestIF.dSiteXPitch*3;
    else
        iMovePitchX=TestIF.dSiteXPitch+TestIF.dSiteXPitch/2;

    for(int i=0; i<X_PITCH_COUNT; i++)
        iXVariable[i]=GetOutArmPitchX_9045(iMovePitchX, i, iOffsetPos);
    iYVariable=GetOutArmPitchY_9045(iMovePitchY, iOffsetPos);

    iXPos=Prod.iTopViewSafePos_X;
    iYPos=Prod.iTopViewSafePos_Y;

    if(OutArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, false))
        return true;
    return false;
}
//------------------------------------------------------------------------------
void SetActiveSuck()
{
    bool bflag=false;

    ZeroMemory(bActiveSuck ,sizeof(bActiveSuck));

    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            if((OutArmSuck.Item[i][j]!=NULL_IC && OutArmSuck.Item[i][j]!=HAS_NULL_IC)
                && OutArmSuck.iAOIStation[i][j]!=eAOI_Top)
            {
                bActiveSuck[i][j]=true;
                bflag=true;
                break;
            }
        }

        if(bflag)
            break;
    }
}
//------------------------------------------------------------------------------
bool DoMoveXY_TopView()
{
    int iRow=0,  iCol=0;
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iXVariable[X_PITCH_COUNT];
    int iXPos=0, iYPos=0;
    int iYVariable  =TestIF.dSiteXPitch*3;
    int iMovePitchX =8000;
    int iMovePitchY =TestIF.iARM_Y_PITCH;
    int iOffsetPos  =-1;
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            if(bActiveSuck[i][j])
            {
                iRow=i;
                iCol=j;
            }
        }
    }

    if(XPitchIsStand())                                                         //X Pitch is 4000mm
        iMovePitchX=TestIF.dSiteXPitch*3;
    else
        iMovePitchX=TestIF.dSiteXPitch+TestIF.dSiteXPitch/2;

    for(int i=0; i<X_PITCH_COUNT; i++)
        iXVariable[i]=GetOutArmPitchX_9045(iMovePitchX, i, iOffsetPos);
    iYVariable=GetOutArmPitchY_9045(iMovePitchY, iOffsetPos);

    iXPos=Prod.iTopView_X+(iMovePitchX/3)*2-(iMovePitchX/3)*iCol;
    iYPos=Prod.iTopView_Y+iMovePitchY*iRow;

    if(IsCheckOutArmDestroyActiveFinish(iXPos, iYPos)==false)
        return false;

    if(OutArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, false))
        return true;
    return false;
}
//------------------------------------------------------------------------------
bool DoMoveZ_TopView(int zPos)
{
    bool flag[2][4]={{true, true, true, true}, {true, true, true, true}};

    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            if(bActiveSuck[i][j])
            {
                flag[i][j]=MOT[OutArmZIndex[i][j]].MotorMove(zPos);
            }
        }
    }
    for(int i=0; i<MAX_ARM_Row; i++)
        for(int j=0; j<MAX_ARM_Col; j++)
            if(flag[i][j]==false)
                return false;
    return true;
}
//------------------------------------------------------------------------------
bool DoPlace_TopView()
{
    bool bflag[MAX_ARM_Row][MAX_ARM_Col]={false};
    bool bHasDuplicateErr=false;
    bool bHasErr=false;
    AnsiString ErrPart=" ";

    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            if(bActiveSuck[i][j])
            {
                bflag[i][j]=OutArmSuck.Suck[i][j].Destroy();
                if(bflag[i][j])
                {
                    AOIKit.MoveSuckData(OutArmSuck, i, j);
                    OutArmSuck.SetItemData(i, j, NULL_IC);

                    bOutArmSuckActive[i][j]=false;
                    bOutArmCheckDestroyACT[i][j]=true;                          //jou 981130 確認device確實destroy完成
                }

                if(OutArmSuck.Suck[i][j].Error)
                {
                    ErrPart+=InArmSuck.Suck[i][j].sName;
                    bHasErr=true;
                }
            }
            else
            {
                bflag[i][j]=true;
            }
        }
    }

    if(bHasErr)
    {
        ShowErrorMessage("JAM0203", K_RETRY, MOutArmX, bHasDuplicateErr, ErrPart);
    }
    for(int i=0; i<MAX_ARM_Row; i++)
        for(int j=0; j<MAX_ARM_Col; j++)
            if(bflag[i][j]==false)
                return false;
    return true;
}
//------------------------------------------------------------------------------
bool DoPick_TopView(bool &SuckError)
{
    bool bflag[MAX_ARM_Row][MAX_ARM_Col]={false};
    bool bHasErr=false;

    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            if(bActiveSuck[i][j])
            {
                bflag[i][j]=OutArmSuck.Suck[i][j].Suck();
                if(bflag[i][j])
                {
                    OutArmSuck.MoveSuckData(AOIKit, i, j);
                }

                if(OutArmSuck.Suck[i][j].Error)
                {
                    bHasErr=true;
                }
            }
            else
            {
                bflag[i][j]=true;
            }
        }
    }

    if(bHasErr)
    {
        SuckError=true;
    }

    for(int i=0; i<MAX_ARM_Row; i++)
        for(int j=0; j<MAX_ARM_Col; j++)
            if(bflag[i][j]==false)
                return false;
    return true;
}
//------------------------------------------------------------------------------
bool DoTopViewMoveKit(bool Down)
{
    int iPos;

    if(Down)
    {
        iPos=Prod.iTopViewKit_Z;
    }
    else
    {
        iPos=Prod.iTopViewKit_Zup;
    }

    if(MOT[MAOIKit].MotorMove(iPos))
        return true;
    return false;
}
//------------------------------------------------------------------------------
void InitTopViewInspection()
{
    iTopViewInspectionTask=1;
}
//------------------------------------------------------------------------------
bool DoTopViewFunction_Inspection()
{
    bool bResult=false;
    int &Task=iTopViewInspectionTask;
    int ret;
    int iBin;

    switch(Task)
    {
        case 1:
            bAOI_Fail_Unit=false;
            Task=1000;
            break;
        case 1000:
            if(DoTopViewMoveKit(true))
            {
                hStartDelay.SetMSAndOn(tAOISetup.tTopView.iStartDelayTime);
                Task=2000;
            }
            break;
        case 2000:
            if(hStartDelay.Off())
            {
                Task=3000;
            }
            break;
        case 3000:
            hTimeOut.SetMSAndOn(tAOISetup.tTopView.iTimeOut);
            SW[SwStartTest1].On();
            Task=3500;
        case 3500:
            if(Sen[SnEOF1].IsOff())
            {
                Task=4000;
            }
            else if(hTimeOut.Off())
            {
                Task=4500;
            }
            break;
        case 4000:
            if(Sen[SnEOF1].IsOn())
            {
                SW[SwStartTest1].Off();

                if(Sen[SnAOIResult1].IsOff())
                {
                    bAOI_Fail_Unit=true;
                    iAOI_Fail_Count++;
                }

                if(iAOI_Fail_Count>=tAOISetup.iAlarmCount)
                {
                    Task=4600;
                }
                else
                {
                    Task=5000;
                }
            }
            else if(hTimeOut.Off())
            {
                Task=4500;
            }
            break;
        case 4500:
            SW[SwStartTest1].Off();
            ret=ShowErrorMessage("WAR16300", K_RETRY|K_SKIP, MMSystem);

            if(ret==K_RETRY)
            {
                Task=3000;
            }
            else
            {
                bAOI_Fail_Unit=true;
                Task=5000;
            }
            break;
        case 4600:
            if(DoTopViewMoveKit(false))
            {
                Task=4610;
            }
            break;
        case 4610:
            iAOI_Fail_Count=0;
            ret=ShowErrorMessage("WAR16303", K_RETRY|K_SKIP, MMSystem);

            if(ret==K_RETRY)
            {
                bAOI_Fail_Unit=false;
                Task=1000;
            }
            else
            {
                Task=5000;
            }
            break;
        case 5000:
            if(DoTopViewMoveKit(false))
            {
                Task=6000;
            }
            break;
        case 6000:
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(bActiveSuck[i][j])
                    {
                        OutArmSuck.iAOIStation[i][j]=eAOI_Top;
                        if(bAOI_Fail_Unit)
                        {
                            iBin=tAOISetup.tTopView.iFailBin;
                            CheckBin(iBin, 1); //kevin 20160826 add shuttle 2
                            AOIKit.iWhichAuto[i][j]=iBin;
                        }
                    }
                }
            }
            Task=7000;
            break;
        case 7000:
            bResult=true;
            break;
        default:
            break;
    }

    return bResult;
}
//------------------------------------------------------------------------------
void InitDoTopViewFunction()
{
    iTopViewTask=1;
}
//------------------------------------------------------------------------------
bool DoTopViewFunction()
{
    int &Task=iTopViewTask;
    bool bResult=false;
    bool bflag=false;
    bool bError=false;
    int ret;

    switch(Task)
    {
        case 1:
            SetActiveSuck();
            Task=1000;
            break;
        case 1000:
            if(DoMoveXY_TopView())
            {
                Task=1100;
            }
            break;
        case 1100:
            if(DoMoveZ_TopView(Prod.iTopView_Place))
            {
                if((AccessLevel>=1 || CosFunction.bOPCanPressStepAndTStart) &&  //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
                (Sen[SnRKManualStep].IsOn() || fOffSet->UseAutoOffsetFunction("Top View")))    //jou 2013-08-29 Use Auto Offset Funtion
                {
                    bEnterOffset=false;
                    fMain->Pause("DoTopViewFunction 1100");
                    Task=1200;
                }
                else
                {
                    Task=1500;
                }
            }
            break;
        case 1200:
            if(MoveOutArmToAutoSafe())
            {
                Task=1000;
            }
            break;
        case 1500:
            if(DoPlace_TopView())
            {
                Task=1300;
            }
            break;
        case 1300:
            if(MoveOutArmToAutoSafe())
            {
                Task=1400;
            }
            break;
        case 1400:
            if(DoMoveXY_TopViewSafePos())
            {
                Task=2000;
            }
            break;
        case 2000:
            InitTopViewInspection();
            Task=2100;
            break;
        case 2100:
            if(DoTopViewFunction_Inspection())
            {
                Task=3000;
            }
            break;
        case 3000:
            if(MoveOutArmToAutoSafe())
            {
                Task=3100;
            }
            break;
        case 3100:
            if(DoMoveXY_TopView())
            {
                Task=3200;
            }
            break;
        case 3200:
            if(DoMoveZ_TopView(Prod.iTopView_Pick))
            {
                if((AccessLevel>=1 || CosFunction.bOPCanPressStepAndTStart) &&  //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
                (Sen[SnRKManualStep].IsOn() || fOffSet->UseAutoOffsetFunction("Top View")))    //jou 2013-08-29 Use Auto Offset Funtion
                {
                    bEnterOffset=false;
                    fMain->Pause("DoTopViewFunction 3200");
                    Task=3250;
                }
                else
                {
                    Task=3300;
                }
            }
            break;
        case 3250:
            if(MoveOutArmToAutoSafe())
            {
                Task=3100;
            }
            break;
        case 3300:
            if(DoPick_TopView(bError))
            {
                Task=3400;
            }
            else if(bError)
            {
                Task=3310;
            }
            break;
        case 3310:
            if(MoveOutArmToAutoSafe())
            {
                Task=3320;
            }
            break;
        case 3320:
            ret=ShowErrorMessage("JAM0201", K_RETRY|K_SKIP, MOutArmX, false, NULL);

            if(ret==K_SKIP)
            {
                for(int i=0; i<MAX_ARM_Row; i++)
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                    {
                        if(OutArmSuck.Suck[i][j].Error)
                        {
                            OutArmSuck.SetItemData(i, j, NULL_IC);
                            AOIKit.SetItemData(i, j, NULL_IC);
                            Task=3400;
                        }
                    }
                }
            }
            else if(ret==K_RETRY)
            {
                Task=3200;
            }
            break;
        case 3400:
            if(MoveOutArmToAutoSafe())
            {
                Task=4000;
            }
            break;
        case 4000:
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if((OutArmSuck.Item[i][j]!=NULL_IC) &&
                       (OutArmSuck.iAOIStation[i][j]!=eAOI_Top))
                    {
                        bflag=true;
                        break;
                    }
                }
            }

            if(bflag)
            {
                Task=1000;
            }
            else
            {
                Task=5000;
            }
            break;
        case 5000:
            bResult=true;
            break;
        default:
            break;
    }

    return bResult;
}
//------------------------------------------------------------------------------
bool DoMoveXY_PADView(int &iRow, int &iCol)
{
    int iXPos=0, iYPos=0;
    int iXVariable[X_PITCH_COUNT];
    int iYVariable =0;
    int iMovePitchX=TestIF.dSiteXPitch*3;
    int iMovePitchY=TestIF.iARM_Y_PITCH;
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iOffsetPos=-1;
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    iZPos[iRow][iCol] =Prod.OutArm_Pad_Place[iRow][iCol];
    bZFlag[iRow][iCol]=true;

    if(XPitchIsStand())                                                         //X Pitch is 4000mm
        iMovePitchX=TestIF.dSiteXPitch/2*3;
    else
        iMovePitchX=TestIF.dSiteXPitch+TestIF.dSiteXPitch/2;

    for(int i=0; i<X_PITCH_COUNT; i++)
        iXVariable[i]=GetOutArmPitchX_9045(iMovePitchX, i, iOffsetPos);
    iYVariable=GetOutArmPitchY_9045(iMovePitchY, iOffsetPos);

    iXPos=Prod.iPADView_X+(iMovePitchX/3)*2-(iMovePitchX/3)*iCol;
    iYPos=Prod.iPADView_Y+iMovePitchY*iRow;

    if(IsCheckOutArmDestroyActiveFinish(iXPos, iYPos)==false)
        return false;

    if(OutArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, true))
        return true;
    return false;
}
//------------------------------------------------------------------------------
void InitPADViewInspection()
{
    iPADViewInspectionTask=1;
}
//------------------------------------------------------------------------------
void DoContinuousFailBySitePADView(bool Res, int iRow, int iCol)                //Alick 20160712 add By Site 連續Fail  PADView
{
    if(Res==true && bVitroxPADViewUse[iRow][iCol])
    {
        iPADViewBySiteTotal[iRow][iCol]++;
    }
    else
    {
        iPADViewBySiteTotal[iRow][iCol]=0;
    }

    if(iPADViewBySiteTotal[iRow][iCol]>=tAOISetup.iContAlarmCountBySite)
        bPADBySiteFailCont=true;
}
//------------------------------------------------------------------------------
void DoContinuousFailByArmPADView(bool Res, int ArmSide)
{
    if(Res==true)
    {
        iPADViewByArmTotal[ArmSide]++;
    }
    else
    {
        iPADViewByArmTotal[ArmSide]=0;
    }

    if(iPADViewByArmTotal[ArmSide]>=tAOISetup.iContAlarmCountByArm)
    {
        bPADByArmFailCont=true;
        bPADByArmDouChk[ArmSide]=true;
    }
}
//------------------------------------------------------------------------------
void DoContinuousFailBySiteBGAView(bool Res, int iRow, int iCol)                //Alick 20160712 add By Site 連續Fail  BGAView
{
    if(Res==true && bVitroxBGAViewUse[iRow][iCol])
    {
        iBGAViewBySiteTotal[iRow][iCol]++;
    }
    else
    {
        iBGAViewBySiteTotal[iRow][iCol]=0;
    }

    if(iBGAViewBySiteTotal[iRow][iCol]>=tAOISetup.iContAlarmCountBySite)
        bBGABySiteFailCont=true;
}
//------------------------------------------------------------------------------
void DoContinuousFailByArmBGAView(bool Res, int ArmSide)
{
    if(Res==true)
    {
        iBGAViewByArmTotal[ArmSide]++;
    }
    else
    {
        iBGAViewByArmTotal[ArmSide]=0;
    }

    if(iBGAViewByArmTotal[ArmSide]>=tAOISetup.iContAlarmCountByArm)
    {
        bBGAByArmFailCont=true;
        bBGAViewByArmDouChk[ArmSide]=true;
    }
}
//------------------------------------------------------------------------------
bool DoPADViewFunction_Inspection(int &iRow, int &iCol)
{
    bool bResult=false;
    int &Task=iPADViewInspectionTask;
    int ret;
    int iBin;
    int iGetBin=0;
    switch(Task)
    {
        case 1:
            hStartDelay.SetMSAndOn(tAOISetup.tPADView.iStartDelayTime);
            bAOI_Fail_Unit=false;
            Task=1000;
            break;
        case 1000:
            if( hStartDelay.Off())
            {
                Task=2000;
            }
            break;
        case 2000:
            hTimeOut.SetMSAndOn(tAOISetup.tPADView.iTimeOut);
            SW[SwStartTest2].On();
            Task=2500;
        case 2500:
            if(Sen[SnEOF2].IsOff())
            {
                Task=3000;
            }
            else if(hTimeOut.Off())
            {
                Task=3500;
            }
            break;
        case 3000:
            if(Sen[SnEOF2].IsOn())
            {
                SW[SwStartTest2].Off();

                if(Sen[SnAOIResult2].IsOff())
                {
                    bAOI_Fail_Unit=true;
                    iAOI_Fail_Count++;
                }

                if(iAOI_Fail_Count>=tAOISetup.iAlarmCount)
                {
                    Task=3600;
                }
                else
                {
                    Task=4000;
                }

                if(fMain->cb1->Checked==true)
                {
                    if(rand()%2 ==1)
                    {
                        bAOI_Fail_Unit=false;
                        Task = 4000;
                    }
                    else
                    {
                        bAOI_Fail_Unit=true;
                        Task =4000;
                    }
                }
            }
            else if(hTimeOut.Off())
            {
                Task=3500;
            }
            break;
        case 3500:
            SW[SwStartTest2].Off();
            if(tAOISetup.tAutoSkipPadPkg.bEnabled)
                ret=K_SKIP;
            else
                ret=ShowErrorMessage("WAR16301", K_RETRY|K_SKIP, MMSystem);

            if(ret==K_RETRY)
            {
                bAOI_Fail_Unit=false;
                Task=3700;
            }
            else
            {
                bAOI_Fail_Unit=true;
                Task=4000;
            }
            break;
        case 3600:
            iAOI_Fail_Count=0;
            if(tAOISetup.tAutoSkipPadPkg.bEnabled)
                ret=K_SKIP;
            else
                ret=ShowErrorMessage("WAR16311", K_RETRY|K_SKIP, MMSystem);

            if(ret==K_RETRY)
            {
                bAOI_Fail_Unit=false;
                Task=3700;
            }
            else
            {
                Task=4000;
            }
            break;
        case 3700:
            if(MoveOutArmToAutoSafe())
            {
                Task=3800;
            }
            break;
        case 3800:
            if(DoMoveXY_PADView(iRow, iCol))
            {
                Task=2000;
            }
            break;
        case 4000:
            if((OutArmSuck.Item[iRow][iCol]!=NULL_IC &&
                OutArmSuck.Item[iRow][iCol]!=HAS_NULL_IC) &&
               OutArmSuck.iAOIStation[iRow][iCol]!=eAOI_PAD)
            {
                if(tAOISetup.tAOINoSort.bEnabled==false)                        //wei 20161207
                {
                    if(bAOI_Fail_Unit)
                    {
                        if(tAOISetup.tTesterFailBin.bEnabled)
                        {
                            iGetBin=OutArmSuck.Item[iRow][iCol]-TEST_PASS;
                            if(Prod.bConsFail[iGetBin]==true)
                            {
                                iBin=tAOISetup.tBGAPADView.iTestFailFailBin;
                            }
                            else
                            {
                                iBin=tAOISetup.tBGAPADView.iTestPassFailBin;
                            }
                        }
                        else
                        {
                            iBin=tAOISetup.tPADView.iFailBin;
                        }
                        CheckBin(iBin, 1);
                        OutArmSuck.iWhichAuto[iRow][iCol]=iBin;
                    }
                }
                OutArmSuck.iAOIStation[iRow][iCol]=eAOI_PAD;
            }
            Task=5000;
            break;
        case 5000:
            if(tAOISetup.bEnabledContinueAlarm==true)                                                    //Alick 20160715
            {
                if(tAOISetup.bEnabledBySitePADView==true)
                {
                    DoContinuousFailBySitePADView(bAOI_Fail_Unit, iRow, iCol);
                }

                if(tAOISetup.bEnabledByArmPADView==true)
                {
                    if(bPickSH1Flag==true && bPickSH2Flag==false && bPADByArmDouChk[0]==false)
                        DoContinuousFailByArmPADView(bAOI_Fail_Unit, 0);
                    else if (bPickSH1Flag==false && bPickSH2Flag==true && bPADByArmDouChk[1]==false)
                        DoContinuousFailByArmPADView(bAOI_Fail_Unit, 1);
                }
            }
            bResult=true;
            break;
        default:
            break;
    }

    return bResult;
}
//------------------------------------------------------------------------------
void InitDoPADViewFunction()
{
    iPADViewTask=1;
}
//------------------------------------------------------------------------------
bool DoPADViewFunction()
{
    static int iSuckRow=0, iSuckCol=0;
    int &Task=iPADViewTask;
    int ret;
    int icount=0;
    bool bResult=false;
    bool bflag=false;
    AnsiString ErrPart="";

    switch(Task)
    {
        case 1:
            bRunAOI=false;
            Task=100;
            break;
        case 100:
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(OutArmSuck.Item[i][j]!=NULL_IC &&
                       OutArmSuck.iAOIStation[i][j]!=eAOI_PAD &&
                       bVitroxPADViewUse[i][j] &&
                       ((tAOISetup.tFailRunFunction.bEnabled &&
                         bAOIPassFail[0][i][j]==true) ||
                         tAOISetup.tFailRunFunction.bEnabled==false) &&
                       bRunAOI==false)
                    {
                        iSuckRow=i;
                        iSuckCol=j;
                        bRunAOI=true;
                        Task=1000;
                        break;
                    }
                }
            }

            if(bRunAOI==false)
            {
                for(int i=0; i<MAX_ARM_Row; i++)
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                    {
                        if(bAOIPassFail[0][i][j]==false)
                            icount++;
                        if(icount>=TestSocket.iShtCnt)
                            Task=5000;
                    }
                }
            }
            break;
        case 1000:
            if(DoMoveXY_PADView(iSuckRow, iSuckCol))
            {
                if((AccessLevel>=1 ||
                    CosFunction.bOPCanPressStepAndTStart) &&                    //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
                   (Sen[SnRKManualStep].IsOn() ||
                    fOffSet->UseAutoOffsetFunction("PAD View")))                //jou 2013-08-29 Use Auto Offset Funtion
                {
                    bEnterOffset=false;
                    fMain->Pause("DoPADViewFunction 1000");
                    Task=1100;
                }
                else
                {
                    InitPADViewInspection();
                    Task=2000;
                }
            }
            break;
        case 1100:
            if(MoveOutArmToAutoSafe())
            {
                bRunAOI=false;
                Task=1000;
            }
            break;
        case 2000:
            if(DoPADViewFunction_Inspection(iSuckRow, iSuckCol))
            {
                Task=3000;
            }
            break;
        case 3000:
            if(MoveOutArmToAutoSafe())
            {
                Task=4000;
            }
            break;
        case 4000:
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(OutArmSuck.Item[i][j]!=NULL_IC &&
                       OutArmSuck.iAOIStation[i][j]!=eAOI_PAD &&
                       bVitroxPADViewUse[i][j])
                    {
                        bflag=true;
                        break;
                    }
                }
            }

            if(bflag)
            {
                bRunAOI=false;
                Task=100;
            }
            else
            {
                if(tAOISetup.bEnabledContinueAlarm==true)
                {
                    if(tAOISetup.bEnabledBySitePADView==true && bPADBySiteFailCont==true)                       //Alick 20160718 修改做完FUNCTION判斷連續FAIL結果
                        Task=4100;
                    else if(tAOISetup.bEnabledByArmPADView==true && bPADByArmFailCont==true)
                        Task=4200;
                    else
                        Task=5000;
                }
                else
                {
                    Task=5000;
                }
            }
            break;
        case 4100:                                                              //Alick 20160718 add PAD Continuous Fail by Site
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(iPADViewBySiteTotal[i][j]>=tAOISetup.iContAlarmCountBySite)
                    {
                        ErrPart+=InArmSuck.Suck[i][j].sName;
                    }
                }
            }

            ret=ShowErrorMessage("WAR16308", K_SKIP, MMSystem, false, ErrPart);
            if(ret==K_SKIP)
            {
                bPADBySiteFailCont=false;
                for(int i=0; i<MAX_ARM_Row; i++)
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                    {
                        iPADViewBySiteTotal[i][j]=0;
                    }
                }
            }

            if(tAOISetup.bEnabledByArmPADView==true && bPADByArmFailCont==true)
                Task=4200;
            else
                Task=5000;
            break;
        case 4200:                                                              //Alick 20160718 add PAD Continuous Fail by Arm
            if(bPADByArmDouChk[0])
                ret=ShowErrorMessage("WAR16309", K_SKIP, MMSystem);
            else
                ret=ShowErrorMessage("WAR163010", K_SKIP, MMSystem);

            if(ret==K_SKIP)
            {
                bPADByArmFailCont=false;
                if(bPADByArmDouChk[0])
                {
                    iPADViewByArmTotal[0]=0;
                    bPADByArmDouChk[0]=false;
                }
                else
                {
                    iPADViewByArmTotal[1]=0;
                    bPADByArmDouChk[1]=false;
                }
            }
            Task=5000;
            break;
        case 5000:
            bResult=true;
            break;
        default:
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool DoMoveXY_BGAView(int &iRow, int &iCol)
{
    int iXPos=0, iYPos=0;
    int iXVariable[X_PITCH_COUNT];
    int iYVariable =0;
    int iMovePitchX=TestIF.dSiteXPitch*3;
    int iMovePitchY=TestIF.iARM_Y_PITCH;
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iOffsetPos=-1;
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    iZPos[iRow][iCol] =Prod.OutArm_BGA_Place[iRow][iCol];
    bZFlag[iRow][iCol]=true;

    if(XPitchIsStand())                                                         //X Pitch is 4000mm
        iMovePitchX=TestIF.dSiteXPitch/2*3;
    else
        iMovePitchX=TestIF.dSiteXPitch+TestIF.dSiteXPitch/2;

    for(int i=0; i<X_PITCH_COUNT; i++)
        iXVariable[i]=GetOutArmPitchX_9045(iMovePitchX, i, iOffsetPos);
    iYVariable=GetOutArmPitchY_9045(iMovePitchY, iOffsetPos);

    iXPos=Prod.iBGAView_X+(iMovePitchX/3)*2-(iMovePitchX/3)*iCol;
    iYPos=Prod.iBGAView_Y+TestIF.iARM_Y_PITCH*iRow;

    if(IsCheckOutArmDestroyActiveFinish(iXPos, iYPos)==false)
        return false;

    if(OutArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, true))
        return true;
    return false;
}
//------------------------------------------------------------------------------
void InitBGAViewInspection()
{
    iBGAViewInspectionTask=1;
}
//------------------------------------------------------------------------------
bool DoBGAViewFunction_Inspection(int &x, int &y)
{
    bool bResult=false;
//    bResult = true;

    int &Task=iBGAViewInspectionTask;
    int ret;
    int iBin;
    int iGetBin=0;
    switch(Task)
    {
        case 1:
            hStartDelay.SetMSAndOn(tAOISetup.tBGAView.iStartDelayTime);
            bAOI_Fail_Unit=false;
            Task=1000;
            break;
        case 1000:
            if(hStartDelay.Off())
            {
                Task=2000;
            }
            break;
        case 2000:
            hTimeOut.SetMSAndOn(tAOISetup.tBGAView.iTimeOut);
            SW[SwStartTest3].On();
            Task=2500;
        case 2500:
            if(Sen[SnEOF3].IsOff())
            {
                Task=3000;
            }
            else if(hTimeOut.Off())
            {
                Task=3500;
            }
            #ifdef SOFT_SIMULTE
            Task=3000;
            #endif
            break;
        case 3000:
            #ifndef SOFT_SIMULTE
            if(Sen[SnEOF3].IsOn())
            #endif
            {
                SW[SwStartTest3].Off();

                if(Sen[SnAOIResult3].IsOff())
                {
                    bAOI_Fail_Unit=true;
                    iAOI_Fail_Count++;
                }

                if(iAOI_Fail_Count>=tAOISetup.iAlarmCount)
                {
                    Task=3600;
                }
                else
                {
                    Task=4000;
                }

                if(fMain->cb1->Checked==true)
                {
                    if(rand()%2 ==1)
                    {
                        bAOI_Fail_Unit=false;
                        Task = 4000;
                    }
                    else
                    {
                        bAOI_Fail_Unit = true;
                        Task =4000;
                    }
                }
            }
            #ifndef SOFT_SIMULTE
            else if(hTimeOut.Off())
            {
                Task=3500;
            }
            #endif
            break;
        case 3500:
            SW[SwStartTest3].Off();
            if(tAOISetup.tAutoSkipBGA.bEnabled)
                ret=K_SKIP;
            else
                ret=ShowErrorMessage("WAR16302", K_RETRY|K_SKIP, MMSystem);

            if(ret==K_RETRY)
            {
                bAOI_Fail_Unit=false;
                Task=3700;
            }
            else
            {
                bAOI_Fail_Unit=true;
                Task=4000;
            }
            break;
        case 3600:
            iAOI_Fail_Count=0;
            if(tAOISetup.tAutoSkipBGA.bEnabled)
                ret=K_SKIP;
            else
                ret=ShowErrorMessage("WAR16312", K_RETRY|K_SKIP, MMSystem);

            if(ret==K_RETRY)
            {
                bAOI_Fail_Unit=false;
                Task=3700;
            }
            else
            {
                Task=4000;
            }
            break;
        case 3700:
            if(MoveOutArmToAutoSafe())
            {
                Task=3800;
            }
            break;
        case 3800:
            if(DoMoveXY_BGAView(x, y))
            {
                Task=2000;
            }
            break;
        case 4000:
            if((OutArmSuck.Item[x][y]!=NULL_IC &&
                OutArmSuck.Item[x][y]!=HAS_NULL_IC) &&
                OutArmSuck.iAOIStation[x][y]!=eAOI_BGA)
            {
                if(tAOISetup.tAOINoSort.bEnabled==false)                        //wei 20161207
                {
                    if(bAOI_Fail_Unit)
                    {
                        if(tAOISetup.tTesterFailBin.bEnabled)
                        {
                            iGetBin=OutArmSuck.Item[x][y]-TEST_PASS;
                            if(Prod.bConsFail[iGetBin]==true)
                            {
                                iBin=tAOISetup.tBGAPADView.iTestFailFailBin;
                            }
                            else
                            {
                                iBin=tAOISetup.tBGAPADView.iTestPassFailBin;
                            }
                        }
                        else
                        {
                            iBin=tAOISetup.tBGAView.iFailBin;
                        }
                        CheckBin(iBin, 1);
                        OutArmSuck.iWhichAuto[x][y]=iBin;
                        bAOIPassFail[0][x][y]=false;            //wei 20160616
                    }
                    else
                    {
                        bAOIPassFail[0][x][y]=true;             //wei 20160616
                    }
                }
                OutArmSuck.iAOIStation[x][y]=eAOI_BGA;
            }
            Task=5000;
            break;
        case 5000:
            if(tAOISetup.bEnabledContinueAlarm==true)                                                    //Alick 20160715
            {
                if(tAOISetup.bEnabledBySiteBGAView==true)                 // bAOI_Fail_Unit==true &&
                {
                    DoContinuousFailBySiteBGAView(bAOI_Fail_Unit, x, y);
                }

                if(tAOISetup.bEnabledByArmBGAView==true)
                {
                    if(bPickSH1Flag==true && bPickSH2Flag==false && bBGAViewByArmDouChk[0]==false)
                        DoContinuousFailByArmBGAView(bAOI_Fail_Unit, 0);
                    else if (bPickSH1Flag==false && bPickSH2Flag==true && bBGAViewByArmDouChk[1]==false)
                        DoContinuousFailByArmBGAView(bAOI_Fail_Unit, 1);
                }
            }
            bResult=true;
            break;
        default:
            break;
    }

    return bResult;
}
//------------------------------------------------------------------------------
void InitBGAViewFunction()
{
    iBGAViewTask=1;
}
//------------------------------------------------------------------------------
bool DoBGAViewFunction()
{
    int &Task=iBGAViewTask;
    bool bResult=false;
    bool bflag=false;
    int i, j;
    int ret;
    AnsiString ErrPart="";
    static int x=0, y=0;

    switch(Task)
    {
        case 1:
            bRunAOI=false;
            Task=100;
            break;
        case 100:
            for(i=0; i<MAX_ARM_Row; i++)
            {
                for(j=0; j<MAX_ARM_Col; j++)
                {
                    if(OutArmSuck.Item[i][j]!=NULL_IC && OutArmSuck.iAOIStation[i][j]!=eAOI_BGA && bRunAOI==false && bVitroxBGAViewUse[i][j])
                    {
                        x=i;
                        y=j;
                        bRunAOI=true;
                        Task=1000;
                        break;
                    }
                }
            }
            break;
        case 1000:
            if(DoMoveXY_BGAView(x, y))
            {
                if((AccessLevel>=1 || CosFunction.bOPCanPressStepAndTStart) &&  //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
                (Sen[SnRKManualStep].IsOn() || fOffSet->UseAutoOffsetFunction("BGA View")))    //jou 2013-08-29 Use Auto Offset Funtion
                {
                    bEnterOffset=false;
                    fMain->Pause("DoBGAViewFunction 1000");
                    Task=1100;
                }
                else
                {
                    InitBGAViewInspection();
                    Task=2000;
                }
            }
            break;
        case 1100:
            if(MoveOutArmToAutoSafe())
            {
                bRunAOI=false;
                Task=1000;
            }
            break;
        case 2000:
            if(DoBGAViewFunction_Inspection(x, y))
            {
                Task=3000;
            }
            break;
        case 3000:
            if(MoveOutArmToAutoSafe())
            {
                Task=4000;
            }
            break;
        case 4000:
            for(i=0; i<MAX_ARM_Row; i++)
            {
                for(j=0; j<MAX_ARM_Col; j++)
                {
                    if(OutArmSuck.Item[i][j]!=NULL_IC && OutArmSuck.iAOIStation[i][j]!=eAOI_BGA && bVitroxBGAViewUse[i][j])
                    {
                        bflag=true;
                        break;
                    }
                }
            }

            if(bflag)
            {
                bRunAOI=false;
                Task=100;
            }
            else
            {
                if(tAOISetup.bEnabledContinueAlarm==true)
                {
                    if(tAOISetup.bEnabledBySiteBGAView==true && bBGABySiteFailCont==true)                       //Alick 20160718 修改做完FUNCTION判斷連續FAIL結果
                        Task=4100;
                    else if(tAOISetup.bEnabledByArmBGAView==true && bBGAByArmFailCont==true)
                        Task=4200;
                    else
                        Task=5000;
                }
                else
                    Task=5000;
            }
            break;
        case 4100:                                                              //Alick 20160718 add BGA Continuous Fail by Site
            for(i=0; i<MAX_ARM_Row; i++)
            {
                for(j=0; j<MAX_ARM_Col; j++)
                {
                    if(iBGAViewBySiteTotal[i][j]>=tAOISetup.iContAlarmCountBySite)
                    {
                        ErrPart+=InArmSuck.Suck[i][j].sName;
                    }
                }
            }
            ret=ShowErrorMessage("WAR16305", K_SKIP, MMSystem, false, ErrPart);
            if(ret==K_SKIP)
            {
                bBGABySiteFailCont=false;
                for(i=0; i<MAX_ARM_Row; i++)
                {
                    for(j=0; j<MAX_ARM_Col; j++)
                    {
                        iBGAViewBySiteTotal[i][j]=0;
                    }
                }
            }

            if(tAOISetup.bEnabledByArmBGAView==true && bBGAByArmFailCont==true)
                Task=4200;
            else
                Task=5000;
            break;
        case 4200:                                                              //Alick 20160718 add BGA Continuous Fail by Arm
            if(bBGAViewByArmDouChk[0])
                ret=ShowErrorMessage("WAR16306", K_SKIP, MMSystem);
            else
                ret=ShowErrorMessage("WAR16307", K_SKIP, MMSystem);

            if(ret==K_SKIP)
            {
                bBGAByArmFailCont=false;
                if(bBGAViewByArmDouChk[0])
                {
                    iBGAViewByArmTotal[0]=0;
                    bBGAViewByArmDouChk[0]=false;
                }
                else
                {
                    iBGAViewByArmTotal[1]=0;
                    bBGAViewByArmDouChk[1]=false;
                }
            }
            Task=5000;
            break;
        case 5000:
            bResult=true;
            break;
        default:
            break;
    }

    return bResult;
}
//------------------------------------------------------------------------------
bool DoMoveXY_ScannerAOI(int &iRow, int &iCol)
{
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iXVariable[X_PITCH_COUNT];
    int iXPos=0, iYPos=0;
    int iYVariable  =0;
    int iMovePitchX =8000;
    int iMovePitchY =TestIF.iARM_Y_PITCH;
    int iOffsetPos  =-1;
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    AnsiString sRecord="";
    static int iCountX=0;
    static int iCountY=0;
    int iCntX=0, iCntY=0, iPitchX=0, iPitchY=0;                                 //KenHsieh 20230202 : Fix AOI LGA Mode Direction

    iZPos[iRow][iCol] =Prod.OutArm_ScannerAOI_Place[iRow][iCol];
    bZFlag[iRow][iCol]=true;

    if(TestIF_File.iTestMode==SingleSite)
    {
        iMovePitchX=8000;
    }
    else
    {
        if(XPitchIsStand())                                                     //X Pitch is 4000mm
            iMovePitchX=TestIF.dSiteXPitch/2*3;
        else
            iMovePitchX=TestIF.dSiteXPitch+TestIF.dSiteXPitch/2;
    }

    for(int i=0; i<X_PITCH_COUNT; i++)
        iXVariable[i]=GetOutArmPitchX_9045(iMovePitchX, i, iOffsetPos);
    iYVariable=GetOutArmPitchY_9045(iMovePitchY, iOffsetPos);

    iXPos=Prod.iScannerAOI_X+(iMovePitchX/3)*2-(iMovePitchX/3)*iCol;            //移到第一顆
    iYPos=Prod.iScannerAOI_Y+iMovePitchY*iRow;

    if(ScannerAOIIF.bLGAModeDirection==true)                                    //KenHsieh 20230202 : Fix AOI LGA Mode Direction
    {
        iCntX   =iLGAPitchXCnt;
        iCntY   =iLGAPitchYCnt;
        iPitchX =iLGAPitchX;
        iPitchY =iLGAPitchY;
    }
    else
    {
        iCntY   =iLGAPitchXCnt;
        iCntX   =iLGAPitchYCnt;
        iPitchY =iLGAPitchX;
        iPitchX =iLGAPitchY;
    }

    if(ScannerAOIIF.bEnabledPositionByAOI==true)                                //Ifor 20251031 add: Move Position Provided By AOI
    {
        if(iScanLGAStep!=0)
        {
            iXPos=iXPos+iPitchX;
            iYPos=iYPos+iPitchY;
        }
    }
    else if(ScannerAOIIF.bScanAOIUseLGAMode==true)
    {
        if(iCntX==0 && iCntY==0)
        {
        }
        else if(iCntX==0 && iCntY!=0)
        {
            iXPos=iXPos+((DeviceForm_File.YDimension*100/(iCntY+1)/2)*((iCntY+1)*2/2-1)-(iPitchY*iScanLGAStep));
        }
        else if(iCntY==0 && iCntX!=0)
        {
            iXPos=iXPos-(DeviceForm_File.XDimension*100/(iCntX+1)/2)*((iCntX+1)*2/2-1)+(iPitchX*iScanLGAStep);
        }
        else
        {                                                                       //Richard 20220817:Add Scanner_AOI add check Big Size IC Function
            iXPos=iXPos-(DeviceForm_File.XDimension*100/(iCntX+1)/2)*((iCntX+1)*2/2-1);   //第一點拍攝位置在pin1對角位置  iX-(IC分割後每個分割之一半(ICx) * 要走幾個ICx) 第一點位置
            iYPos=iYPos+(DeviceForm_File.YDimension*100/(iCntY+1)/2)*((iCntY+1)*2/2-1);

            if(iScanLGAStep==0)
            {
                iCountX=0;
                iCountY=0;
            }

            iXPos=iXPos+iCountX*iPitchX;

            if(iCountX%2==0)
            {
                iYPos=iYPos-(iCountY)*iPitchY;
            }
            else
            {
                iYPos=iYPos-iCntY*iPitchY+(iCountY)*iPitchY;
            }
        }
    }
    sRecord.sprintf("iX:%d,iY:%d", iXPos, iYPos);

    if(IsCheckOutArmDestroyActiveFinish(iXPos, iYPos)==false)
        return false;

    if(OutArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, true))
    {
        iCountY++;
        if(iCountY==iCntY+1)
        {
            iCountY=0;
            iCountX++;
        }
        FrmAOI->ShowCommData( "[Position]", sRecord);
        return true;
    }

    return false;
}
//------------------------------------------------------------------------------
void InitScannerAOIInspection()
{
    iScannerAOIInspectionTask=1;
    iScanLGAStep=0;                                                             //Ifor 20211022 add: AOI LGA Mode
    bplus   =true;                                                              //判斷加or減
    irowX   =0;                                                                 //Richard 20220817:Add Scanner_AOI add check Big Size IC Function
    icolumnY=0;
}
//------------------------------------------------------------------------------
bool DoScanAOIFunction_Inspection(int &iRow, int &iCol)
{
    bool bResult=false;

    int &Task=iScannerAOIInspectionTask;
    int ret;
    int iBin;
    static int iOldCount=0;                                                     //Ifor 20251031 add: Move Position Provided By AOI
    switch(Task)
    {
        case 1:
            hStartDelay.SetMSAndOn(ScannerAOIIF.iStartDelayTime);
            bAOI_Fail_Unit=false;
            iOldCount=iScanLGAStep;                                             //Ifor 20251031 add: Move Position Provided By AOI
            Task=1000;
            break;
        case 1000:
            if(hStartDelay.Off())
            {
                Task=2000;
            }
            break;
        case 2000:                                                              //Richard 20220817:Add Scanner_AOI add check Big Size IC Function
            if(ScannerAOIIF.bEnabledPositionByAOI==true)                        //Ifor 20251031 add: Move Position Provided By AOI
            {
                FrmAOI->TriggerAOISystem(true, false);
                Task=2200;
                hLGA_DelayTime.SetSecAndOn(ScannerAOIIF.fLGAWaitTime);
            }
            else
            {
                if(iScanLGAStep==0)
                {
                    FrmAOI->TriggerAOISystem(true, true);                           //第一點位觸發改為"11BDAOI"
                }
                else if(iScanLGAStep<=((iLGAPitchXCnt+1)*(iLGAPitchYCnt+1))-1)
                {
                    FrmAOI->TriggerAOISystem(true, false);                           //其他點位觸發改為"10BDAOI"
                }

                if(ScannerAOIIF.bScanAOIUseLGAMode==true && iScanLGAStep<((iLGAPitchXCnt+1)*(iLGAPitchYCnt+1))-1) //更改拍攝次數
                {
                    Task=2100;
                    hLGA_DelayTime.SetSecAndOn(ScannerAOIIF.fLGAWaitTime);
                }
                else
                {
                    hTimeOut.SetMSAndOn(ScannerAOIIF.iTimeOut);
                    Task=2500;
                }
            }
            break;
        case 2200:                                                              //Ifor 20251031 add: Move Position Provided By AOI
            if(hLGA_DelayTime.Off())
            {
                ShowMyMessage("Wait NEXT POSITION TimeOut!!");
                Task=2000;
            }
            if(iOldCount!=iScanLGAStep)
            {
                iOldCount=iScanLGAStep;
                Task=3700;
            }
            else
            {
                if(bTriggerAOI==false)
                    Task=3000;
            }
            break;
        case 2100:
            if(hLGA_DelayTime.Off())
            {
                iScanLGAStep++;                                                 //Richard 20220817:Add Scanner_AOI add check Big Size IC Function 更改拍攝次數
                Task=3700;
            }
            break;
        case 2500:
            if(bTriggerAOI==false)                                              //收到 AOI 回覆
            {
                Task=3000;
            }
            else if(hTimeOut.Off())
            {
                Task=3500;
                FrmAOI->ShowCommData( "[Decode]" , " TimeOut");
            }
            break;
        case 3000:
            if(bTriggerAOI==false)                                              //收到 AOI 回覆
            {
                if(iAOIResult[0]!=1)
                {
                    bAOI_Fail_Unit=true;
                    iAOI_Fail_Count++;
                }
                Task=4000;
            }
            break;
        case 3500:
            ret=ShowErrorMessage("WAR0883", K_RETRY|K_SKIP, MMSystem);

            if(ret==K_RETRY)
            {
                bAOI_Fail_Unit=false;
                bplus   =true;                                                  //判斷加or減
                irowX   =0;                                                     //Richard 20220817:Add Scanner_AOI add check Big Size IC Function
                icolumnY=0;
                Task    =3700;
                iScanLGAStep=0;
            }
            else
            {
                bAOI_Fail_Unit=true;
                Task=4000;
            }
            break;
        case 3700:
            if(MoveOutArmToAutoSafe())
            {
                Task=3800;
            }
            break;
        case 3800:
            if(DoMoveXY_ScannerAOI(iRow, iCol))
            {
                Task=2000;
            }
            break;
        case 4000:
            if((OutArmSuck.Item[iRow][iCol]!=NULL_IC &&
                OutArmSuck.Item[iRow][iCol]!=HAS_NULL_IC) &&
                OutArmSuck.iAOIStation[iRow][iCol]!=eAOI_SCANAOI)
            {
                if(bAOI_Fail_Unit &&
                   tAOISetup.bEnabledScanAOIUnUseFailBin==false)                //Ifor 20200825 add:TF-AMD AOI Error 不丟至Fail Bin
                {
                    iBin=GetAOIFailBin(iRow, iCol);                             //RogerYang 20251120 : 新增AOI測試後分bin也考慮測試結果的選項
                    OutArmSuck.iWhichAuto[iRow][iCol]=iBin;
                    bAOIPassFail[0][iRow][iCol]=false;                          //wei 20160616
                }
                else
                {
                    bAOIPassFail[0][iRow][iCol]=true;                           //wei 20160616
                    OutArmSuck.iWhichAuto[iRow][iCol]=iScanAOI_BackupBin[iRow][iCol];       //AI(fAOI) 20260527 (RogerYang) : restore bin on retry pass
                }
                OutArmSuck.iAOIStation[iRow][iCol]=eAOI_SCANAOI;
            }
            else if(OutArmSuck.Item[iRow][iCol]==HAS_NULL_IC)
            {
                OutArmSuck.iAOIStation[iRow][iCol]=eAOI_SCANAOI;
            }
            Task=5000;
            break;
        case 5000:
            if(tAOISetup.bEnabledScanAOIBySiteAlarm==true)                      // bAOI_Fail_Unit==true &&
            {
                FrmAOI->DoContinuousFailBySiteScanAOI(bAOI_Fail_Unit, iRow, iCol);
            }

            if(tAOISetup.bEnabledScanAOIByArmAlarm==true)
            {
                if(bPickSH1Flag==true && bPickSH2Flag==false && bScanAOIByArmDouChk[0]==false)
                    FrmAOI->DoContinuousFailByArmScanAOI(bAOI_Fail_Unit, 0);
                else if (bPickSH1Flag==false && bPickSH2Flag==true && bScanAOIByArmDouChk[1]==false)
                    FrmAOI->DoContinuousFailByArmScanAOI(bAOI_Fail_Unit, 1);
            }
            bResult=true;
            break;
        default:
            break;
    }

    return bResult;
}
//------------------------------------------------------------------------------
void InitTopScannerAOIInspection()
{
    iTopScannerAOIInspectionTask=1;
}
//------------------------------------------------------------------------------
bool DoTopScanAOIFunction_Inspection(int &iRow, int &iCol)
{
    bool bResult=false;

    int &Task=iTopScannerAOIInspectionTask;
    int ret;
    int iBin;
    switch(Task)
    {
        case 1:
            hStartDelay.SetMSAndOn(ScannerAOIIF.iTopStartDelayTime);
            bAOI_Fail_Unit=false;
            Task=1000;
            break;
        case 1000:
            if(hStartDelay.Off())
            {
                Task=2000;
            }
            break;
        case 2000:
            hTimeOut.SetMSAndOn(ScannerAOIIF.iTopTimeOut);
            FrmAOI->TriggerAOISystem(true, false, 1);
            Task=2500;
        case 2500:
            if(bTriggerAOI==false)                                              //收到 AOI 回覆
            {
                Task=3000;
            }
            else if(hTimeOut.Off())
            {
                Task=3500;
            }
            break;
        case 3000:
            if(bTriggerAOI==false)                                              //收到 AOI 回覆
            {
                if(iAOIResult[0]!=1)
                {
                    bAOI_Fail_Unit=true;
                }
                Task=4000;
            }
            break;
        case 3500:
            ret=ShowErrorMessage("WAR0887", K_RETRY|K_SKIP, MMSystem);

            if(ret==K_RETRY)
            {
                bAOI_Fail_Unit=false;
                Task=2000;
            }
            else
            {
                bAOI_Fail_Unit=true;
                Task=4000;
            }
            break;
        case 4000:
            if((OutArmSuck.Item[iRow][iCol]!=NULL_IC &&
                OutArmSuck.Item[iRow][iCol]!=HAS_NULL_IC) &&
                OutArmSuck.iAOIStation[iRow][iCol]!=eAOI_Top)
            {
                if(bAOI_Fail_Unit &&                                            //Ifor 20200825 add:TF-AMD AOI Error 不丟至Fail Bin
                   tAOISetup.bEnabledTopScanAOIUnUseFailBin==false)             //0: Auto1  1: Auto2    2: Auto3
                {                                                               //cbAOIFial: 0=Fix2  1=Fix3  2=Fix5  3=Fix6
                    iBin=ScannerAOIIF.TopScannerIfError+eFix2;                  //AI(general) 20260416 (RogerYang) : fix enum offset, was +4 (old eTrayName), now +eFix2
                    if(ScannerAOIIF.TopScannerIfError>=2) iBin++;               //AI(general) 20260416 (RogerYang) : cbAOIFial skips Fix4, index>=2 needs +1
                    if(ScannerAOIIF.TopScannerIfError==2)
                    {
                        if(TrayForm.bTrayUpDownSet[eFix2]==true)
                        {
                            iBin=eFix5;                                         //AI(general) 20260416 (RogerYang) : fix enum offset, was 7
                        }
                        else
                        {
                            iBin=eFix2;                                         //AI(general) 20260416 (RogerYang) : fix enum offset, was 4
                        }
                    }
                    else if(ScannerAOIIF.TopScannerIfError==3)
                    {
                        if(TrayForm.bTrayUpDownSet[eFix3]==true)
                        {
                            iBin=eFix6;                                         //AI(general) 20260416 (RogerYang) : fix enum offset, was 8
                        }
                        else
                        {
                            iBin=eFix3;                                         //AI(general) 20260416 (RogerYang) : fix enum offset, was 5
                        }
                    }
                    OutArmSuck.iWhichAuto[iRow][iCol]=iBin;
                    bAOIPassFail[0][iRow][iCol]=false;                          //wei 20160616
                }
                else
                {
                    bAOIPassFail[0][iRow][iCol]=true;                           //wei 20160616
                }
                OutArmSuck.iAOIStation[iRow][iCol]=eAOI_Top;
            }
            else if(OutArmSuck.Item[iRow][iCol]==HAS_NULL_IC)
            {
                OutArmSuck.iAOIStation[iRow][iCol]=eAOI_Top;
            }
            Task=5000;
            break;
        case 5000:
            if(tAOISetup.bEnabledScanAOIBySiteAlarm==true)                      // bAOI_Fail_Unit==true &&
            {
                FrmAOI->DoContinuousFailBySiteScanAOI(bAOI_Fail_Unit, iRow, iCol, 1);
            }

            if(tAOISetup.bEnabledScanAOIByArmAlarm==true)
            {
                if(bPickSH1Flag==true && bPickSH2Flag==false && bScanAOIByArmDouChk[0]==false)
                    FrmAOI->DoContinuousFailByArmScanAOI(bAOI_Fail_Unit, 0);
                else if (bPickSH1Flag==false && bPickSH2Flag==true && bScanAOIByArmDouChk[1]==false)
                    FrmAOI->DoContinuousFailByArmScanAOI(bAOI_Fail_Unit, 1);
            }
            bResult=true;
            break;
        default:
            break;
    }

    return bResult;
}
//------------------------------------------------------------------------------
void InitialScanAOITask()
{
    iScanAOITask=1;
}
//------------------------------------------------------------------------------
bool DoScanAOIFunction()                                                        //Ifor 20191210 : add Scanner AOI
{
    int &Task=iScanAOITask;
    bool bResult=false;
    bool bflag=false;
    int ret;
    AnsiString ErrPart="";
    static int iRow=0, iCol=0;
    static int iScanSite=-1;
    int iCheckSite=0;
    static bool bScanStep=false;

    switch(Task)
    {
        case 1:
            bRunAOI=false;
            Task=100;
            if(ScannerAOIIF.iIntervalCounter==0)                                //JerryYang 20210224 : 修正沒有執行AOI的問題
            {
                iScanSite=-1;
            }
            break;
        case 100:
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    iCheckSite=(i*4)+j;
                    if(OutArmSuck.Item[i][j]!=NULL_IC &&
                       OutArmSuck.iAOIStation[i][j]!=eAOI_SCANAOI &&
                       bRunAOI==false)
                    {
                        iScanAOIIntervalCounter++;
                        if(TestIF_File.iTestMode==SingleSite &&
                           iScanAOIIntervalCounter>=ScannerAOIIF.iIntervalCounter+1)
                        {
                            iScanAOIIntervalCounter=0;
                            iScanSite=iCheckSite;
                            iRow=i;
                            iCol=j;
                            bRunAOI=true;
                            break;
                        }
                        else if(TestIF_File.iTestMode==DualSite &&
                                ScannerAOIIF.iIntervalCounter==1 &&
                                TestIF_File.iShuttleMode==0)                    //Ifor 20200218 50%抽檢
                        {
                            if(iScanAOIIntervalCounter>=9999)                   //Ifor 20200218 add 為了讓第一顆IC先測試AOI初始值9999
                            {
                                iScanAOIIntervalCounter=1;
                            }

                            if(iScanAOIIntervalCounter==1 ||
                               iScanAOIIntervalCounter==3 ||
                               iScanAOIIntervalCounter==6 ||
                               iScanAOIIntervalCounter==8 )                     //Ifor 20200218 50%抽檢
                            {
                                if(iScanAOIIntervalCounter==8)
                                {
                                    iScanAOIIntervalCounter=0;
                                }
                                iScanSite=iCheckSite;
                                iRow=i;
                                iCol=j;
                                bRunAOI=true;
                                break;
                            }
                            else
                            {
                                OutArmSuck.iAOIStation[i][j]=eAOI_SCANAOI;
                            }
                        }
                        else if(iScanAOIIntervalCounter>=ScannerAOIIF.iIntervalCounter+1 && iScanSite!=iCheckSite)
                        {
                            iScanAOIIntervalCounter=0;
                            iScanSite=iCheckSite;
                            iRow=i;
                            iCol=j;
                            bRunAOI=true;
                            break;
                        }
                        else
                        {
                            OutArmSuck.iAOIStation[i][j]=eAOI_SCANAOI;
                        }
                    }
                    else if(LastSet.bUseTestSocket[0][i][j]==false ||
                            LastSet.bUseTestSocket[1][i][j]==false)             //Ifor 20210923 fix:關Site 計數異常導致AOI檢測條件不成立
                    {
                        if(iScanSite==iCheckSite-1 && bScanStep==false)
                        {
                            bScanStep=true;
                            iScanSite++;
                        }
                    }
                }
            }

            if(bRunAOI==true)
            {
                Task=1000;
                iScanLGAStep=0;                                                 //Ifor 20211022 add: AOI LGA Mode
                iScanAOI_BackupBin[iRow][iCol]=OutArmSuck.iWhichAuto[iRow][iCol];           //AI(fAOI) 20260527 (RogerYang) : backup bin before AOI
            }
            else
            {
                return true;
            }
            break;
        case 1000:
            if(DoMoveXY_ScannerAOI(iRow, iCol))                                 //first移到IC中心
            {
                InitScannerAOIInspection();
                bScanStep=false;
                Task=2000;
                if(LastSet.iRealDummy!=REALLY)                                  //Ifor 20200825 add:Scan AOI Dummy 不檢測
                {
                    Task=3000;
                    bAOIPassFail[0][iRow][iCol]=true;
                    OutArmSuck.iAOIStation[iRow][iCol]=eAOI_SCANAOI;
                }
            }
            break;
        case 2000:
            if(DoScanAOIFunction_Inspection(iRow, iCol))
            {
                Task=3000;
            }
            break;
        case 3000:
            if(MoveOutArmToAutoSafe())
            {
                Task=4000;
            }
            break;
        case 4000:
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(OutArmSuck.Item[i][j]!=NULL_IC &&
                       OutArmSuck.iAOIStation[i][j]!=eAOI_SCANAOI)
                    {
                        bflag=true;
                        break;
                    }
                }
            }

            if(bflag)
            {
                bRunAOI=false;
                Task=100;
            }
            else
            {
                if(tAOISetup.bEnabledScanAOIBySiteAlarm==true && bScanAOIBySiteFailCont==true)
                {
                    Task=4100;
                }
                else if(tAOISetup.bEnabledScanAOIByArmAlarm==true && bScanAOIByArmFailCont==true)
                {
                    Task=4200;
                }
                else
                {
                    Task=5000;
                }
            }
            break;
        case 4100:
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(iScanAOIBySiteTotal[i][j]>=tAOISetup.iScanAOIAlarmCountBySite)
                    {
                        ErrPart+=InArmSuck.Suck[i][j].sName;
                    }
                }
            }

            ret=ShowErrorMessage("WAR0880", K_RETRY|K_SKIP, MMSystem, false, ErrPart);  //Ifor 20200909 add:AOI Fail Retry
            if(ret==K_SKIP)
            {
                bScanAOIBySiteFailCont=false;
                for(int i=0; i<MAX_ARM_Row; i++)
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                    {
                        iScanAOIBySiteTotal[i][j]=0;
                    }
                }

                if(tAOISetup.bEnabledScanAOIByArmAlarm==true && bScanAOIByArmFailCont==true)
                    Task=4200;
                else
                    Task=5000;
            }
            else if(ret==K_RETRY)
            {
                Task=6000;
            }
            break;
        case 4200:
            if(bScanAOIByArmDouChk[0])
                ret=ShowErrorMessage("WAR0881", K_RETRY|K_SKIP, MMSystem);      //Ifor 20200909 add:AOI Fail Retry
            else
                ret=ShowErrorMessage("WAR0882", K_RETRY|K_SKIP, MMSystem);      //Ifor 20200909 add:AOI Fail Retry

            if(ret==K_SKIP)
            {
                bScanAOIByArmFailCont=false;
                if(bScanAOIByArmDouChk[0])
                {
                    iScanAOIByArmTotal[0]=0;
                    bScanAOIByArmDouChk[0]=false;
                }
                else
                {
                    iScanAOIByArmTotal[1]=0;
                    bScanAOIByArmDouChk[1]=false;
                }
                Task=5000;
            }
            else if(ret==K_RETRY)
            {
                Task=6000;
            }

            break;
        case 5000:
            bResult=true;
            break;
        case 6000:
            if(MoveOutArmToAutoSafe())
            {
                bRunAOI=true;
                OutArmSuck.iAOIStation[iRow][iCol]=0;
                bScanAOIBySiteFailCont=false;                                   //AI(fAOI) 20260527 (RogerYang) : clear site alarm on retry
                iScanAOIBySiteTotal[iRow][iCol]=0;                              //reset site counter
                bScanAOIByArmFailCont=false;                                    //clear arm alarm on retry
                bScanAOIByArmDouChk[0]=false;                                   //clear arm DouChk
                bScanAOIByArmDouChk[1]=false;                                   //clear arm DouChk
                iScanAOIByArmTotal[0]=0;
                iScanAOIByArmTotal[1]=0;
                iScanLGAStep=0;                                                 //Ifor 20211022 add: AOI LGA Mode
                Task=1000;
            }
            break;
        default:
            break;
    }

    return bResult;
}
//------------------------------------------------------------------------------
void InitialTopScanAOITask()
{
    iTopScanAOITask=1;
}
//------------------------------------------------------------------------------
bool DoTopScanAOIFunction()                                                     //Ifor 20191210 : add Scanner AOI
{
    int &Task=iTopScanAOITask;
    bool bResult=false;
    bool bflag=false;
    int ret;
    AnsiString ErrPart="";
    static int iRow=0, iCol=0;
    static int iScanSite=-1;
    int iCheckSite=0;
    bool bError=false;

    switch(Task)
    {
        case 1:
            bRunAOI=false;
            Task=100;
            break;
        case 100:
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(OutArmSuck.Item[i][j]!=NULL_IC &&
                       OutArmSuck.iAOIStation[i][j]!=eAOI_Top &&
                       bRunAOI==false)
                    {
                        iTopScanAOIIntervalCounter++;
                        iCheckSite=(i*4)+j;
                        if(TestIF_File.iTestMode==SingleSite && iTopScanAOIIntervalCounter>=ScannerAOIIF.iTopIntervalCounter+1)
                        {
                            iTopScanAOIIntervalCounter=0;
                            iScanSite=iCheckSite;
                            iRow=i;
                            iCol=j;
                            bRunAOI=true;
                            break;
                        }
                        else if(TestIF_File.iTestMode==DualSite && ScannerAOIIF.iTopIntervalCounter==1 && TestIF_File.iShuttleMode==0)   //Ifor 20200218 50%抽檢
                        {
                            if(iTopScanAOIIntervalCounter>=9999)                //Ifor 20200218 add 為了讓第一顆IC先測試AOI初始值9999
                            {
                                iTopScanAOIIntervalCounter=1;
                            }

                            if(iTopScanAOIIntervalCounter == 1 ||
                               iTopScanAOIIntervalCounter == 3 ||
                               iTopScanAOIIntervalCounter == 6 ||
                               iTopScanAOIIntervalCounter == 8 )                //Ifor 20200218 50%抽檢
                            {
                                if(iTopScanAOIIntervalCounter==8)
                                {
                                    iTopScanAOIIntervalCounter=0;
                                }
                                iScanSite=iCheckSite;
                                iRow=i;
                                iCol=j;
                                bRunAOI=true;
                                break;
                            }
                            else
                            {
                                OutArmSuck.iAOIStation[i][j]=eAOI_Top;
                            }
                        }
                        else if(iTopScanAOIIntervalCounter>=ScannerAOIIF.iTopIntervalCounter+1 && iScanSite!=iCheckSite)
                        {
                            iTopScanAOIIntervalCounter=0;
                            iScanSite=iCheckSite;
                            iRow=i;
                            iCol=j;
                            bRunAOI=true;
                            break;
                        }
                        else
                        {
                            OutArmSuck.iAOIStation[i][j]=eAOI_Top;
                        }
                    }
                }
            }

            if(bRunAOI==true)
            {
                SetActiveSuck();
                Task=1000;
            }
            else
            {
                return true;
            }
            break;
        case 1000:
            if(DoMoveXY_TopView())
            {
                Task=1100;
            }
            break;
        case 1100:
            if(DoMoveZ_TopView(Prod.iTopView_Place))
            {
                if((AccessLevel>=1 ||
                    CosFunction.bOPCanPressStepAndTStart) &&                    //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
                   (Sen[SnRKManualStep].IsOn() ||
                    fOffSet->UseAutoOffsetFunction("Top View")))                //jou 2013-08-29 Use Auto Offset Funtion
                {
                    bEnterOffset=false;
                    fMain->Pause("DoTopScanAOIFunction 1100");
                    Task=1200;
                }
                else
                {
                    Task=1500;
                }
            }
            break;
        case 1200:
            if(MoveOutArmToAutoSafe())
            {
                Task=1000;
            }
            break;
        case 1500:
            if(DoPlace_TopView())
            {
                Task=1300;
            }
            break;
        case 1300:
            if(MoveOutArmToAutoSafe())
            {
                Task=1400;
            }
            break;
        case 1400:
            if(DoMoveXY_TopViewSafePos())
            {
                InitScannerAOIInspection();
                Task=2000;
                if(LastSet.iRealDummy!=REALLY)                                  //Ifor 20200825 add:Scan AOI Dummy 不檢測
                {
                    Task=3000;
                    bAOIPassFail[0][iRow][iCol]=true;
                    OutArmSuck.iAOIStation[iRow][iCol]=eAOI_Top;
                }
            }
            break;
        case 2000:
            if(DoTopScanAOIFunction_Inspection(iRow, iCol))
            {
                Task=3000;
            }
            break;
        case 3000:
            if(MoveOutArmToAutoSafe())
            {
                Task=3100;
            }
            break;
        case 3100:
            if(DoMoveXY_TopView())
            {
                Task=3200;
            }
            break;
        case 3200:
            if(DoMoveZ_TopView(Prod.iTopView_Pick))
            {
                if((AccessLevel>=1 ||
                    CosFunction.bOPCanPressStepAndTStart) &&                    //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
                   (Sen[SnRKManualStep].IsOn() ||
                    fOffSet->UseAutoOffsetFunction("Top View")))                //jou 2013-08-29 Use Auto Offset Funtion
                {
                    bEnterOffset=false;
                    fMain->Pause("DoTopScanAOIFunction 3200");
                    Task=3250;
                }
                else
                {
                    Task=3300;
                }
            }
            break;
        case 3250:
            if(MoveOutArmToAutoSafe())
            {
                Task=3100;
            }
            break;
        case 3300:
            if(DoPick_TopView(bError))
            {
                Task=3400;
            }
            else if(bError)
            {
                Task=3310;
            }
            break;
        case 3310:
            if(MoveOutArmToAutoSafe())
            {
                Task=3320;
            }
            break;
        case 3320:
            ret=ShowErrorMessage("JAM0201", K_RETRY|K_SKIP, MOutArmX, false, NULL);

            if(ret==K_SKIP)
            {
                for(int i=0; i<MAX_ARM_Row; i++)
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                    {
                        if(OutArmSuck.Suck[i][j].Error)
                        {
                            OutArmSuck.SetItemData(i, j, NULL_IC);
                            AOIKit.SetItemData(i, j, NULL_IC);
                            Task=3400;
                        }
                    }
                }
            }
            else if(ret==K_RETRY)
            {
                Task=3200;
            }
            break;
        case 3400:
            if(MoveOutArmToAutoSafe())
            {
                Task=4000;
            }
            break;
        case 4000:
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(OutArmSuck.Item[i][j]!=NULL_IC &&
                       OutArmSuck.iAOIStation[i][j]!=eAOI_Top)
                    {
                        bflag=true;
                        break;
                    }
                }
            }

            if(bflag)
            {
                bRunAOI=false;
                Task=100;
            }
            else
            {
                if(tAOISetup.bEnabledTopScanAOIBySiteAlarm==true && bTopScanAOIBySiteFailCont==true)
                {
                    Task=4100;
                }
                else if(tAOISetup.bEnabledTopScanAOIByArmAlarm==true && bTopScanAOIByArmFailCont==true)
                {
                    Task=4200;
                }
                else
                {
                    Task=5000;
                }
            }
            break;
        case 4100:
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(iTopScanAOIBySiteTotal[i][j]>=tAOISetup.iTopScanAOIAlarmCountBySite)
                    {
                        ErrPart+=InArmSuck.Suck[i][j].sName;
                    }
                }
            }

            ret=ShowErrorMessage("WAR0880", K_RETRY|K_SKIP, MMSystem, false, ErrPart);
            if(ret==K_SKIP)
            {
                bScanAOIBySiteFailCont=false;
                for(int i=0; i<MAX_ARM_Row; i++)
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                    {
                        iTopScanAOIBySiteTotal[i][j]=0;
                    }
                }

                if(tAOISetup.bEnabledTopScanAOIByArmAlarm==true && bTopScanAOIByArmFailCont==true)
                    Task=4200;
                else
                    Task=5000;
            }
            else
            {
                Task=6000;
            }
            break;
        case 4200:
            if(bTopScanAOIByArmDouChk[0])
                ret=ShowErrorMessage("WAR0885", K_RETRY|K_SKIP, MMSystem);
            else
                ret=ShowErrorMessage("WAR0886", K_RETRY|K_SKIP, MMSystem);

            if(ret==K_SKIP)
            {
                bTopScanAOIByArmFailCont=false;
                if(bTopScanAOIByArmDouChk[0])
                {
                    iTopScanAOIByArmTotal[0]=0;
                    bTopScanAOIByArmDouChk[0]=false;
                }
                else
                {
                    iTopScanAOIByArmTotal[1]=0;
                    bTopScanAOIByArmDouChk[1]=false;
                }
                Task=5000;
            }
            else
            {
                Task=6000;
            }
            break;
        case 5000:
            bResult=true;
            break;
        case 6000:
            if(MoveOutArmToAutoSafe())
            {
                bRunAOI=true;
                OutArmSuck.iAOIStation[iRow][iCol]=0;
                Task=6010;
            }
            break;
        case 6010:

            break;
        default:
            break;
    }

    return bResult;
}
//------------------------------------------------------------------------------
void InitAOIFunction()
{
    iAOITask=1;
    bAOI_Fail_Unit=false;

    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            OutArmSuck.iAOIStation[i][j]=0;
        }
    }
}
//------------------------------------------------------------------------------
bool DoAOIFunction()
{
    int &Task=iAOITask;
    bool bResult=false;
    static int iIntervalCnt=0;

    switch(Task)
    {
        case 1:
            if(tAOISetup.tBGAView.bEnabled)
            {
                for(int i=0; i<MAX_ARM_Row; i++)
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                    {
                        bAOIPassFail[0][i][j]=false;
                    }
                }
                Task=1000;
            }
            else if(tAOISetup.tPADView.bEnabled)
            {
                Task=2000;
            }
            else if(USE_Scanner_AOI_Inspection==(int)eBtnAOI_BottomInstall && ScannerAOIIF.iEnableScannerMode!=0)
            {
                Task=6000;
            }
            else if(USE_Top_Scanner_AOI_Inspection==true && ScannerAOIIF.iEnableTopScannerMode!=0)  //Ifor 20200902 add: TFAMD Top AOI
            {
                Task=6500;
            }
            else if(FrmAOI->RunTopBottomInspect())                          //Jimmychiu 20240322 : Top & Bottom Inspect
            {
                Task = 7000;
            }
            else
            {
                Task=4000;
            }
            break;
        case 1000:
            InitBGAViewFunction();
            Task=1100;
            break;
        case 1100:
            if(DoBGAViewFunction())
            {
                if(tAOISetup.tPADView.bEnabled)
                {
                    Task=2000;
                }
                else
                {
                    Task=4000;
                }
            }
            break;
        case 2000:
            InitDoPADViewFunction();
            Task=2100;
            break;
        case 2100:
            if(DoPADViewFunction())
            {
                Task=4000;
            }
            break;
        case 3000:
            InitDoTopViewFunction();
            Task=3100;
            break;
        case 3100:
            if(DoTopViewFunction())
            {
                Task=4000;
            }
            break;
        case 4000:
            bResult=true;
            bPickSH1Flag=false;
            bPickSH2Flag=false;
            break;
        case 6000:
            InitialScanAOITask();
            Task=6100;
            break;
        case 6100:
            if(DoScanAOIFunction())
            {
                if(USE_Top_Scanner_AOI_Inspection==true && ScannerAOIIF.iEnableTopScannerMode!=0)  //Ifor 20200902 add: TFAMD Top AOI
                    Task=6500;
                else
                    Task=4000;
            }
            break;
        case 6500:
            InitialTopScanAOITask();
            Task=6600;
            break;
        case 6600:
            if(DoTopScanAOIFunction())
            {
                Task=4000;
            }
            break;
        case 7000:
            if(FrmAOI->ttbInsp->bIntervalCheck)
            {
                iIntervalCnt++;
                if(iIntervalCnt>FrmAOI->ttbInsp->iIntervalCount)
                {
                    FrmAOI->ttbInsp->DoTopBtmInspFunc(true);
                    iIntervalCnt=0;
                    Task=7100;
                }
                else
                {
                    Task=4000;
                }
            }
            else
            {
                FrmAOI->ttbInsp->DoTopBtmInspFunc(true);
                Task=7100;
            }
            break;
        case 7100:
            if(FrmAOI->ttbInsp->DoTopBtmInspFunc(false))
            {
                Task=4000;
            }
            break;
        default:
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
int GetAOIFailBin(int iRow, int iCol)                                           //RogerYang 20251120 : 新增AOI測試後分bin也考慮測試結果的選項
{
    //cbAOIFial ItemIndex: 0=Fix2  1=Fix3  2=Fix5  3=Fix6
    //e6TrayName: eFix2=7, eFix3=8, eFix5=10, eFix6=11

    int iWitchScannerIfError;
    int iBin;
    if(ScannerAOIIF.iAOIFailBinType==0)                                         //RogerYang 20251120 : 新增AOI測試後分bin也考慮測試結果的選項
    {
        //舊規則
        iWitchScannerIfError=ScannerAOIIF.ScannerIfError;
        iBin=iWitchScannerIfError+eFix2;                                        //AI(general) 20260416 (RogerYang) : fix enum offset, was +4 (old eTrayName), now +eFix2
        if(iWitchScannerIfError>=2) iBin++;                                     //AI(general) 20260416 (RogerYang) : cbAOIFial skips Fix4, index>=2 needs +1
    }
    else //新規則                                                               //RogerYang 20251120 : 新增AOI測試後分bin也考慮測試結果的選項
    {
        //判斷outarm上的IC在測試後是pass/fail
        if(Prod.bIsPassBin[OutArmSuck.iBinData[iRow][iCol]]==true)
        {
            iWitchScannerIfError=ScannerAOIIF.ScannerIfErrorAndTestPass;
            iBin=iWitchScannerIfError+eFix2;                                    //AI(general) 20260416 (RogerYang) : fix enum offset
            if(iWitchScannerIfError>=2) iBin++;                                 //AI(general) 20260416 (RogerYang) : cbAOIFial skips Fix4, index>=2 needs +1
        }
        else
        {
            iWitchScannerIfError=ScannerAOIIF.ScannerIfError;
            iBin=iWitchScannerIfError+eFix2;                                    //AI(general) 20260416 (RogerYang) : fix enum offset
            if(iWitchScannerIfError>=2) iBin++;                                 //AI(general) 20260416 (RogerYang) : cbAOIFial skips Fix4, index>=2 needs +1
        }
    }

    if(iWitchScannerIfError==2)
    {
        if(TrayForm.bTrayUpDownSet[eFix2]==true)
        {
            iBin=eFix5;                                                         //AI(general) 20260416 (RogerYang) : fix enum offset, was 7
        }
        else
        {
            iBin=eFix2;                                                         //AI(general) 20260416 (RogerYang) : fix enum offset, was 4
        }
    }
    else if(iWitchScannerIfError==3)
    {
        if(TrayForm.bTrayUpDownSet[eFix3]==true)
        {
            iBin=eFix6;                                                         //AI(general) 20260416 (RogerYang) : fix enum offset, was 8
        }
        else
        {
            iBin=eFix3;                                                         //AI(general) 20260416 (RogerYang) : fix enum offset, was 5
        }
    }
    return iBin;
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::spbSaveClick(TObject *Sender)
{
    if(IniConfig.bA02DisableSaveParsWhenSwitchToOp==true &&                     //RogerYang 20260305 張寧說時間到都要登出，且不可被更改任何設定(A01_2)
        AccessLevel==0)
    {
        ShowMyMessage("[A01_2]目前已切換到Operator權限，\r\n請重新登入再做設定!");
        Close();
    }

    AnsiString S="";

    if(CheckFailBin()==false)
    {
        ShowMyMessage("AOI Fail Bin設定與Bin Sel Pass Bin別相同");
    }

    S=GetLastOpenFN();
    AnsiString szDir="";
    szDir.sprintf("%s%s\\AOI.Data", DataPath, S);

    WriteIniData(szDir, "SETTING", "EnabledTopView", cbEnabledTopView->Checked);
    WriteIniData(szDir, "SETTING", "FailBinTopView", edt_FailTopView->Text);
    WriteIniData(szDir, "SETTING", "TimeOutTopView", edt_TimeoutTopView->Text);
    WriteIniData(szDir, "SETTING", "StartDelayTimeTopView", edt_SDelayTopView->Text);

    WriteIniData(szDir, "SETTING", "EnabledPADView", cbEnabledPADView->Checked);
    WriteIniData(szDir, "SETTING", "FailBinPADView", edt_FailPADView->Text);
    WriteIniData(szDir, "SETTING", "TimeOutPADView", edt_TimeoutPADView->Text);
    WriteIniData(szDir, "SETTING", "StartDelayTimePADView", edt_SDelayPADView->Text);

    WriteIniData(szDir, "SETTING", "EnabledBGAView", cbEnabledBGAView->Checked);
    WriteIniData(szDir, "SETTING", "FailBinBGAView", edt_FailBGAView->Text);
    WriteIniData(szDir, "SETTING", "TimeOutBGAView", edt_TimeoutBGAView->Text);
    WriteIniData(szDir, "SETTING", "StartDelayTimeBGAView", edt_SDelayBGAView->Text);

    WriteIniData(szDir, "SETTING", "AlarmCount", edt_AlarmCount->Text);

    WriteIniData(szDir, "SETTING", "EnabledailRunFunction", cbBGAFailRun->Checked);
    WriteIniData(szDir, "SETTING", "EnabledAutoSkipBGA",    cbAutoSkipBGA->Checked);
    WriteIniData(szDir, "SETTING", "EnabledAutoSkipPadPkg", cbAutoSkipPadPkg->Checked);

    WriteIniData(szDir, "SETTING", "EnabledAOINoSort",      cbAOINoSort->Checked);   //wei 20161207

    WriteIniData(szDir, "SETTING", "TestPassFailBinBGAPADView", edt_PassFailBGAPADView->Text);    //wei  20160706
    WriteIniData(szDir, "SETTING", "TestFailFailBinBGAPADView", edt_FailFailBGAPADView->Text);    //wei  20160706

    WriteIniData(szDir, "SETTING", "EnabledTesterFailBin",   cbTesterFailBin->Checked);     //wei  20160706
    WriteIniData(szDir, "SETTING", "EnabledContinueAlarm",   cb_VitContiuneAlarm->Checked); //Alick 20160712
    WriteIniData(szDir, "SETTING", "EnabledBySiteBGAView",   cb_AlarmBySite->Checked);      //Alick 20160712
    WriteIniData(szDir, "SETTING", "EnabledByArmBGAView",    cb_AlarmByArm->Checked);       //Alick 20160712
    WriteIniData(szDir, "SETTING", "ContAlarmCountBySite",   edt_ConAlarmConut->Text);      //Alick 20160712
    WriteIniData(szDir, "SETTING", "ContAlarmCountByArm",    edt_ConAlarmConut1->Text);     //Alick 20160718
    WriteIniData(szDir, "SETTING", "EnabledBySitePADView",   cb_AlarmBySite1->Checked);     //Alick 20160712
    WriteIniData(szDir, "SETTING", "EnabledByArmPADView",    cb_AlarmByArm1->Checked);      //Alick 20160712

    //Ifor 20191225 : add Scan AOI Alarm
    //==>
    WriteIniData(szDir, "SETTING", "bEnabledScanAOIBySiteAlarm",    cb_ScanAOIAlarmBySite->Checked);
    WriteIniData(szDir, "SETTING", "bEnabledScanAOIByArmAlarm",     cb_ScanAOIAlarmByArm->Checked);
    WriteIniData(szDir, "SETTING", "iScanAOIAlarmCountBySite",      edt_ScanAOIAlarmCountBySite->Text);
    WriteIniData(szDir, "SETTING", "iScanAOIAlarmCountByArm",       edt_ScanAOIAlarmCountByArm->Text);

    WriteIniData(szDir, "SETTING", "bEnabledScanAOIUnUseFailBin",   cb_ScanAOIUnUseFailBin->Checked);   //Ifor 20200825 add:TF-AMD AOI Error 不丟至Fail Bin
    //<==
    //Ifor 20191225 : add Scan AOI Alarm

    for(int i=0; i<MAX_ARM_Row; i++)                                                              //Alick 20160713
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            S.sprintf("Dut %s", IndexSuckName[i][j]);
            WriteIniData(szDir, "DutOnOff_BGAView", S, bVitroxBGAViewUse[i][j]);
            bVitroxBGAViewMap[i][j]=bVitroxBGAViewUse[i][j];
        }
    }

    for(int i=0; i<MAX_ARM_Row; i++)                                                              //Alick 20160714
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            S.sprintf("Dut %s", IndexSuckName[i][j]);
            WriteIniData(szDir, "DutOnOff_PADView", S, bVitroxPADViewUse[i][j]);
            bVitroxPADViewMap[i][j]=bVitroxPADViewUse[i][j];
        }
    }

    //Ifor 20190724 add AMD AOI
    //==>
    WriteIniData(szDir, "SETTING", "iEnableScannerMode",        rgScannerMode->ItemIndex);
    WriteIniData(szDir, "SETTING", "ScannerReadTimeout",        edtScannerReadTimeout->Text);

    WriteIniData(szDir, "SETTING", "iIntervalCounter",          edtAOIIntervalCounter->Text);
    WriteIniData(szDir, "SETTING", "iRetryCounter",             edtAOIRetryCounter->Text);
    WriteIniData(szDir, "SETTING", "iAOIFailBinType",           rgAOIFailBinType->ItemIndex);       //RogerYang 20251120 : 新增AOI測試後分bin也考慮測試結果的選項
    WriteIniData(szDir, "SETTING", "iScanAOIFialBin",           cbAOIFial->ItemIndex);
    WriteIniData(szDir, "SETTING", "iAOIFialAndTestPass",       cbAOIFialAndTestPass->ItemIndex);   //RogerYang 20251120 : 新增AOI測試後分bin也考慮測試結果的選項

    WriteIniData(szDir, "SETTING", "bBDTotalFunction",          cbBDByTotal->Checked);
    WriteIniData(szDir, "SETTING", "bBDTotalContiFunction",     cbBDByTotalConti->Checked);
    WriteIniData(szDir, "SETTING", "bBDSiteFunction",           cbBDBySite->Checked);
    WriteIniData(szDir, "SETTING", "bBDSiteContiFunction",      cbBDBySiteConti->Checked);
    WriteIniData(szDir, "SETTING", "bDBAlramAutoResetCount",    cbDBAlarmAutoReset->Checked);

    WriteIniData(szDir, "SETTING", "iBDTotalCounter",           edtAOIBDCounterTotal->Text);
    WriteIniData(szDir, "SETTING", "iBDTotalContiCounter",      edtAOIBDCounterTotalConti->Text);
    WriteIniData(szDir, "SETTING", "iBDSiteCounter",            edtAOIBDCounterSite->Text);
    WriteIniData(szDir, "SETTING", "iBDSiteContiCounter",       edtAOIBDCounterSiteConti->Text);

    WriteIniData(szDir, "SETTING", "iBDTotalCounterLast",       pnlAOIBDTotalLast->Caption);
    WriteIniData(szDir, "SETTING", "iBDTotalContiCounterLast",  pnlAOIBDTotalContiLast->Caption);
    WriteIniData(szDir, "SETTING", "iBDSite1CounterLast",       pnlAOIBDSite1Last->Caption);
    WriteIniData(szDir, "SETTING", "iBDSite2CounterLast",       pnlAOIBDSite2Last->Caption);
    WriteIniData(szDir, "SETTING", "iBDSite1ContiCounterLast",  pnlAOIBDSite1ContiLast->Caption);
    WriteIniData(szDir, "SETTING", "iBDSite2ContiCounterLast",  pnlAOIBDSite2ContiLast->Caption);

    WriteIniData(szDir, "SETTING", "StartDelayTimeScanAOIView", edt_SDelayScanAOI->Text);
    WriteIniData(szDir, "SETTING", "TimeOutScanAOIView", edt_TimeoutScanAOI->Text);

    WriteIniData(szDir, "RS232",   "Device",                    cbDevice->Text);
    WriteIniData(szDir, "RS232",   "Baud Rate",                 cbBaudRate->Text);
    WriteIniData(szDir, "RS232",   "Byte Size",                 cbByteSize->Text);
    WriteIniData(szDir, "RS232",   "Stop Bit",                  cbStopBit->Text);
    WriteIniData(szDir, "RS232",   "Parity",                    cbParity->Text);

    WriteIniData(szDir, "SETTING", "ScannerICGain",             edtScannerICGain->Text);        //Ifor 20200525 add:依據Tray Y Pitch Gain 決定不使用範圍

    WriteIniData(szDir, "SETTING", "bEnabledScanAOIUseLGAMode", cb_ScanAOIUseLGAMode->Checked); //Ifor 20211022 add: AOI LGA Mode
    WriteIniData(szDir, "SETTING", "ScannerLGAWaitTime",        edtScannerLGAWaitTime->Text);   //Ifor 20211022 add: AOI LGA Mode

    WriteIniData(szDir, "SETTING", "bLGAModeDirection", cb_LGAModeDirection->Checked);          //Ifor 20230202 add: AOI LGA Mode Direction
    WriteIniData(szDir, "SETTING", "bEnabledPositionByAOI", cb_EnabledPositionByAOI->Checked);              //Ifor 20251031 add: Move Position Provided By AOI
    //<==
    //Ifor 20190724 add AMD AOI

    //Ifor 20211027 add:AOI Tray Edit
    //==>
    AnsiString AsTrayData="";

    for(int k=0; k<iQuotient; k++)                                              //KenHsieh 20220830 : 修改add:AOI Tray Edit
    {
        AsTrayData="";
        if(iRemainder==0 || k!=iQuotient-1)
        {
            for(int i=0; i<10; i++)
            {
                for(int j=0; j<_MAX_ROW_ITEM; j++)
                {
                    AsTrayData=AsTrayData+MOT[MMScanAOI].Tray.Data[i+(k*10)][j]+",";
                }
            }
        }
        else
        {
            for(int i=0; i<iRemainder; i++)
            {
                for(int j=0; j<_MAX_ROW_ITEM; j++)
                {
                    AsTrayData=AsTrayData+MOT[MMScanAOI].Tray.Data[i+(k*10)][j]+",";
                }
            }
        }

        S.sprintf("TrayDisable_%d", k+1);
        WriteIniData(szDir, "AOITRAY", S, AsTrayData);                          //Ifor 20211022 add: AOI LGA Mode
    }
    //<==
    //Ifor 20211027 add:AOI Tray Edit

    //Ifor 20200902 add: TFAMD Top AOI
    //==>
    WriteIniData(szDir, "SETTING", "iEnableTopScannerMode",     rgTopScannerMode->ItemIndex);
    WriteIniData(szDir, "SETTING", "iTopIntervalCounter",       edtTopAOIIntervalCounter->Text);
    WriteIniData(szDir, "SETTING", "iTopRetryCounter",          edtTopAOIRetryCounter->Text);
    WriteIniData(szDir, "SETTING", "iTopScanAOIFialBin",        cbTopAOIFial->ItemIndex);

    WriteIniData(szDir, "SETTING", "bEnabledTopScanAOIBySiteAlarm", cb_TopScanAOIAlarmBySite->Checked);
    WriteIniData(szDir, "SETTING", "iTopScanAOIAlarmCountBySite",   edt_TopScanAOIAlarmCountBySite->Text);
    WriteIniData(szDir, "SETTING", "bEnabledTopScanAOIByArmAlarm",  cb_TopScanAOIAlarmByArm->Checked);
    WriteIniData(szDir, "SETTING", "iTopScanAOIAlarmCountByArm",    edt_TopScanAOIAlarmCountByArm->Text);
    WriteIniData(szDir, "SETTING", "StartDelayTimeScanAOIView",     edt_SDelayTopScanAOI->Text);
    WriteIniData(szDir, "SETTING", "TimeOutTopScanAOIView",         edt_TimeoutTopScanAOI->Text);
    WriteIniData(szDir, "SETTING", "TopScannerReadTimeout",         edtTopScannerReadTimeout->Text);
    WriteIniData(szDir, "SETTING", "bEnabledTopScanAOIUnUseFailBin",cb_TopScanAOIUnUseFailBin->Checked);

    WriteIniData(szDir, "RS232",   "Top Device",                    cbTopDevice->Text);
    WriteIniData(szDir, "RS232",   "Top Baud Rate",                 cbTopBaudRate->Text);
    WriteIniData(szDir, "RS232",   "Top Byte Size",                 cbTopByteSize->Text);
    WriteIniData(szDir, "RS232",   "Top Stop Bit",                  cbTopStopBit->Text);
    WriteIniData(szDir, "RS232",   "Top Parity",                    cbTopParity->Text);
    //<==
    //Ifor 20200902 add: TFAMD Top AOI

    if(USE_Scanner_AOI_Inspection==(int)eBtnAOI_TopBottomInstall)                    //Jimmychiu 20240322 : Top & Bottom Inspect
    {
        for(int i=0;i<2;i++)
        {
            for(int j=0;j<6;j++)
            {
                edlsSamll[i][j]->Text=sgICSmall->Cells[j+1][i+1];
            }
        }
        for(int i=0;i<4;i++)
        {
            for(int j=0;j<6;j++)
            {
                edlsLarge[i][j]->Text=sgICLarge->Cells[j+1][i+1];
            }
        }
    }

    if(elParameter)                                                             //AI(general) 20260411 (Rogeryang) : fix path缺Recipe子資料夾 + NULL guard
    {
        AnsiString sSavePath;
        sSavePath.sprintf("%s%s\\", DataPath, GetLastOpenFN());
        elParameter->SaveEditTextToFile(sSavePath, "AOI.Data");
    }

    fAOI_ReadFile();
    fMain->BackupSetupFile();                                                   //Ifor 20170620 (wei) add Auto BackUp Setup File & Last Data

    if(USE_Scanner_AOI_Inspection==true)
    {
        if(ScannerAOIIF.iEnableScannerMode==0)
        {
            FrmAOI->spbStopCom->Click();
        }
        else
        {
            FrmAOI->spbStartCom->Click();
        }
    }

    if(USE_Top_Scanner_AOI_Inspection==true)                                    //Ifor 20200902 add: TFAMD Top AOI
    {
        if(ScannerAOIIF.iEnableTopScannerMode==0)
        {
            FrmAOI->spbStopCom->Click();
        }
        else
        {
            FrmAOI->spbStartCom->Click();
        }
    }

    if(FrmAOI->RunTopBottomInspect())
        FrmAOI->ttbInsp->DoCCDLightDown(false, true);
}
//------------------------------------------------------------------------------
void TFrmAOI::fAOI_ReadFile()
{
    AnsiString S="";
    S=GetLastOpenFN();
    AnsiString szDir="";

    AnsiString tmpS="";
    szDir.sprintf("%s%s\\AOI.Data", DataPath, S);

    tAOISetup.tTopView.bEnabled         =ReadIniData(szDir, "SETTING", "EnabledTopView", false);
    tAOISetup.tTopView.iFailBin         =ReadIniData(szDir, "SETTING", "FailBinTopView", 15);
    tAOISetup.tTopView.iTimeOut         =ReadIniData(szDir, "SETTING", "TimeOutTopView", 3);
    tAOISetup.tTopView.iStartDelayTime  =ReadIniData(szDir, "SETTING", "StartDelayTimeTopView", 10);

    tAOISetup.tPADView.bEnabled         =ReadIniData(szDir, "SETTING", "EnabledPADView", false);
    tAOISetup.tPADView.iFailBin         =ReadIniData(szDir, "SETTING", "FailBinPADView", 15);
    tAOISetup.tPADView.iTimeOut         =ReadIniData(szDir, "SETTING", "TimeOutPADView", 3);
    tAOISetup.tPADView.iStartDelayTime  =ReadIniData(szDir, "SETTING", "StartDelayTimePADView", 10);

    tAOISetup.tBGAView.bEnabled         =ReadIniData(szDir, "SETTING", "EnabledBGAView", false);
    tAOISetup.tBGAView.iFailBin         =ReadIniData(szDir, "SETTING", "FailBinBGAView", 15);
    tAOISetup.tBGAView.iTimeOut         =ReadIniData(szDir, "SETTING", "TimeOutBGAView", 3);
    tAOISetup.tBGAView.iStartDelayTime  =ReadIniData(szDir, "SETTING", "StartDelayTimeBGAView", 10);

    tAOISetup.tBGAPADView.iTestPassFailBin =ReadIniData(szDir, "SETTING", "TestPassFailBinBGAPADView", 15);         //wei  20160706
    tAOISetup.tBGAPADView.iTestFailFailBin =ReadIniData(szDir, "SETTING", "TestFailFailBinBGAPADView", 15);         //wei  20160706
    tAOISetup.tTesterFailBin.bEnabled   =ReadIniData(szDir, "SETTING", "EnabledTesterFailBin",   false);            //wei  20160706

    tAOISetup.iAlarmCount               =ReadIniData(szDir, "SETTING", "AlarmCount", 1);
    tAOISetup.bEnabledContinueAlarm     =ReadIniData(szDir, "SETTING", "EnabledContinueAlarm", false);              //Alick 20160712 是否使用連續Fail
    tAOISetup.bEnabledBySiteBGAView     =ReadIniData(szDir, "SETTING", "EnabledBySiteBGAView", false);              //Alick 20160712 BGA Fail by Site
    tAOISetup.bEnabledByArmBGAView      =ReadIniData(szDir, "SETTING", "EnabledByArmBGAView",  false);              //Alick 20160712 BGA Fail by Arm
    tAOISetup.iContAlarmCountBySite     =ReadIniData(szDir, "SETTING", "ContAlarmCountBySite", 3);                  //Alick 20160712 Fail by Site Count
    tAOISetup.iContAlarmCountByArm      =ReadIniData(szDir, "SETTING", "ContAlarmCountByArm",  3);                  //Alick 20160718 Fail by Arm Count
    tAOISetup.bEnabledBySitePADView     =ReadIniData(szDir, "SETTING", "EnabledBySitePADView", false);              //Alick 20160712 PAD Fail by Site
    tAOISetup.bEnabledByArmPADView      =ReadIniData(szDir, "SETTING", "EnabledByArmPADView",  false);              //Alick 20160712 PAD Fail bu Arm

    //Ifor 20191225 : add Scan AOI Alarm
    tAOISetup.bEnabledScanAOIBySiteAlarm    =ReadIniData(szDir, "SETTING", "bEnabledScanAOIBySiteAlarm", false);
    tAOISetup.bEnabledScanAOIByArmAlarm     =ReadIniData(szDir, "SETTING", "bEnabledScanAOIByArmAlarm",  false);
    tAOISetup.iScanAOIAlarmCountBySite      =ReadIniData(szDir, "SETTING", "iScanAOIAlarmCountBySite", 3);
    tAOISetup.iScanAOIAlarmCountByArm       =ReadIniData(szDir, "SETTING", "iScanAOIAlarmCountByArm",  3);

    tAOISetup.bEnabledScanAOIUnUseFailBin   =ReadIniData(szDir, "SETTING", "bEnabledScanAOIUnUseFailBin", false);   //Ifor 20200825 add:TF-AMD AOI Error 不丟至Fail Bin
    //Ifor 20191225 : add Scan AOI Alarm

    for(int i=0; i<MAX_ARM_Row; i++)                                                                                //Alick 20160713
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            S.sprintf("Dut %s", IndexSuckName[i][j]);
            bVitroxBGAViewUse[i][j]=ReadIniData(szDir, "DutOnOff_BGAView", S, bVitroxBGAViewMap[i][j]);
            bVitroxBGAViewMap[i][j]=bVitroxBGAViewUse[i][j];
        }
    }

    for(int i=0; i<MAX_ARM_Row; i++)                                                                            //Alick 20160714
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            S.sprintf("Dut %s", IndexSuckName[i][j]);
            bVitroxPADViewUse[i][j]=ReadIniData(szDir, "DutOnOff_PADView", S, bVitroxPADViewMap[i][j]);
            bVitroxPADViewMap[i][j]=bVitroxPADViewUse[i][j];
        }
    }

    if(USE_AOI_Inspection)
        tAOISetup.bEnabledAOI = tAOISetup.tTopView.bEnabled || tAOISetup.tPADView.bEnabled || tAOISetup.tBGAView.bEnabled;
    else
        tAOISetup.bEnabledAOI = false;

    tAOISetup.tFailRunFunction.bEnabled = ReadIniData(szDir, "SETTING", "EnabledailRunFunction", false);
    tAOISetup.tAutoSkipBGA.bEnabled     = ReadIniData(szDir, "SETTING", "EnabledAutoSkipBGA",    false);
    tAOISetup.tAutoSkipPadPkg.bEnabled  = ReadIniData(szDir, "SETTING", "EnabledAutoSkipPadPkg", false);

    tAOISetup.tAOINoSort.bEnabled       = ReadIniData(szDir, "SETTING", "EnabledAOINoSort", false);   //wei 20161207

    ScannerAOIIF.iEnableScannerMode      =ReadIniData(szDir, "SETTING", "iEnableScannerMode", 0);                               //Ifor 20190724 add AMD AOI
    ScannerAOIIF.fScannerReadTimeout     =ReadIniData(szDir, "SETTING", "ScannerReadTimeout", 100);
    ScannerAOIIF.iIntervalCounter        =ReadIniData(szDir, "SETTING", "iIntervalCounter", 0);
    ScannerAOIIF.iRetryCounter           =ReadIniData(szDir, "SETTING", "iRetryCounter", 0);
    ScannerAOIIF.iAOIFailBinType         =ReadIniData(szDir, "SETTING", "iAOIFailBinType", 0);                              //RogerYang 20251120 : 新增AOI測試後分bin也考慮測試結果的選項
    ScannerAOIIF.ScannerIfError          =ReadIniData(szDir, "SETTING", "iScanAOIFialBin", 0);
    ScannerAOIIF.ScannerIfErrorAndTestPass=ReadIniData(szDir,"SETTING", "iAOIFialAndTestPass", 0);                          //RogerYang 20251120 : 新增AOI測試後分bin也考慮測試結果的選項
    ScannerAOIIF.iBallDamageType         =ReadIniData(szDir, "SETTING", "iBallDamageType", 0);
    ScannerAOIIF.bBDTotalFunction        =ReadIniData(szDir, "SETTING", "bBDTotalFunction", false);
    ScannerAOIIF.bBDTotalContiFunction   =ReadIniData(szDir, "SETTING", "bBDTotalContiFunction", false);
    ScannerAOIIF.bBDSiteFunction         =ReadIniData(szDir, "SETTING", "bBDSiteFunction", false);
    ScannerAOIIF.bBDSiteContiFunction    =ReadIniData(szDir, "SETTING", "bBDSiteContiFunction", false);
    ScannerAOIIF.bDBAlramAutoResetCount  =ReadIniData(szDir, "SETTING", "bDBAlramAutoResetCount", false);

    ScannerAOIIF.iBDTotalCounter         =ReadIniData(szDir, "SETTING", "iBDTotalCounter", 0);
    ScannerAOIIF.iBDTotalContiCounter    =ReadIniData(szDir, "SETTING", "iBDTotalContiCounter", 0);
    ScannerAOIIF.iBDSiteCounter          =ReadIniData(szDir, "SETTING", "iBDSiteCounter", 0);
    ScannerAOIIF.iBDSiteContiCounter     =ReadIniData(szDir, "SETTING", "iBDSiteContiCounter", 0);
    ScannerAOIIF.iBDTotalCounterLast     =ReadIniData(szDir, "SETTING", "iBDTotalCounterLast", 0);
    ScannerAOIIF.iBDTotalContiCounterLast=ReadIniData(szDir, "SETTING", "iBDTotalContiCounterLast", 0);
    ScannerAOIIF.iBDSite1CounterLast     =ReadIniData(szDir, "SETTING", "iBDSite1CounterLast", 0);
    ScannerAOIIF.iBDSite2CounterLast     =ReadIniData(szDir, "SETTING", "iBDSite2CounterLast", 0);
    ScannerAOIIF.iBDSite1ContiCounterLast=ReadIniData(szDir, "SETTING", "iBDSite1ContiCounterLast", 0);
    ScannerAOIIF.iBDSite2ContiCounterLast=ReadIniData(szDir, "SETTING", "iBDSite2ContiCounterLast", 0);

    ScannerAOIIF.iStartDelayTime        =ReadIniData(szDir, "SETTING", "StartDelayTimeScanAOIView", 10);
    ScannerAOIIF.iTimeOut               =ReadIniData(szDir, "SETTING", "TimeOutScanAOIView", 3);

    ScannerAOIIF.sSCANNER_ComPort       =ReadIniData(szDir, "RS232", "Device", AnsiString(""));
    ScannerAOIIF.sSCANNER_BaudRate      =ReadIniData(szDir, "RS232", "Baud Rate", AnsiString("57600"));
    ScannerAOIIF.sSCANNER_ByteSize      =ReadIniData(szDir, "RS232", "Byte Size", AnsiString("8"));
    ScannerAOIIF.sSCANNER_StopBit       =ReadIniData(szDir, "RS232", "Stop Bit", AnsiString("1"));
    ScannerAOIIF.sSCANNER_Parity        =ReadIniData(szDir, "RS232", "Parity", AnsiString("None"));

    ScannerAOIIF.fScannerICGain         =CheckRange(ReadIniData(szDir, "SETTING", "ScannerICGain", 1.0),1.5 ,0.5);          //Ifor 20200525 add:依據Tray Y Pitch Gain 決定不使用範圍
    edtScannerICGain->Text              =FormatFloat("0.00", ScannerAOIIF.fScannerICGain);

    ScannerAOIIF.bScanAOIUseLGAMode     =ReadIniData(szDir, "SETTING", "bEnabledScanAOIUseLGAMode", false);                 //Ifor 20211022 add: AOI LGA Mode
    ScannerAOIIF.fLGAWaitTime           =CheckRange(ReadIniData(szDir, "SETTING", "ScannerLGAWaitTime", 2.0),20.0 ,1.0);    //Ifor 20211022 add: AOI LGA Mode
    ScannerAOIIF.bLGAModeDirection      =ReadIniData(szDir, "SETTING", "bLGAModeDirection", true);                          //Ifor 20230202 add: AOI LGA Mode Direction
    ScannerAOIIF.bEnabledPositionByAOI  =ReadIniData(szDir, "SETTING", "bEnabledPositionByAOI", false);                     //Ifor 20251031 add: Move Position Provided By AOI

    ScannerAOIIF.iEnableTopScannerMode      =ReadIniData(szDir, "SETTING", "iEnableTopScannerMode", 0);                     //Ifor 20200902 add: TFAMD Top AOI
    ScannerAOIIF.fTopScannerReadTimeout     =ReadIniData(szDir, "SETTING", "TopScannerReadTimeout", 100);
    ScannerAOIIF.iTopIntervalCounter        =ReadIniData(szDir, "SETTING", "iTopIntervalCounter", 0);
    ScannerAOIIF.iTopRetryCounter           =ReadIniData(szDir, "SETTING", "iTopRetryCounter", 0);
    ScannerAOIIF.TopScannerIfError          =ReadIniData(szDir, "SETTING", "iTopScanAOIFialBin", 0);

    ScannerAOIIF.iTopStartDelayTime        =ReadIniData(szDir, "SETTING", "StartDelayTimeTopScanAOIView", 10);
    ScannerAOIIF.iTopTimeOut               =ReadIniData(szDir, "SETTING", "TimeOutScanTopAOIView", 3);

    ScannerAOIIF.sTopSCANNER_ComPort       =ReadIniData(szDir, "RS232", "Top Device", AnsiString(""));
    ScannerAOIIF.sTopSCANNER_BaudRate      =ReadIniData(szDir, "RS232", "Top Baud Rate", AnsiString("57600"));
    ScannerAOIIF.sTopSCANNER_ByteSize      =ReadIniData(szDir, "RS232", "Top Byte Size", AnsiString("8"));
    ScannerAOIIF.sTopSCANNER_StopBit       =ReadIniData(szDir, "RS232", "Top Stop Bit", AnsiString("1"));
    ScannerAOIIF.sTopSCANNER_Parity        =ReadIniData(szDir, "RS232", "Top Parity", AnsiString("None"));

    ScannerAOIIF.fScannerICGain         =CheckRange(ReadIniData(szDir, "SETTING", "ScannerICGain", 1.0), 1.5 ,0.5);   //Ifor 20200525 add:依據Tray Y Pitch Gain 決定不使用範圍
    edtScannerICGain->Text              =FormatFloat("0.00", ScannerAOIIF.fScannerICGain);

    AnsiString AsTrayData="";
    AnsiString asBuffer="";
    bool bHasFile=true;
    if(iQuotient==0)                                                            //KenHsieh 20220830 : 修改add:AOI Tray Edit
    {
        iQuotient=_MAX_COL_ITEM/10;
        iRemainder=_MAX_COL_ITEM%10;
        if(iRemainder!=0)
            iQuotient+=1;
    }

    for(int k=0; k<iQuotient; k++)                                              //KenHsieh 20220830 : 修改add:AOI Tray Edit
    {
        S.sprintf("TrayDisable_%d", k+1);
        AsTrayData                      =ReadIniData(szDir, "AOITRAY", S, AnsiString("NA"));
        asBuffer=asBuffer+AsTrayData;
        if(AsTrayData=="NA")
            bHasFile=false;
    }
    TStringList *sList;
    sList=new TStringList();
    sList->CommaText=asBuffer;
    if(asBuffer.Length()<_MAX_COL_ITEM*_MAX_ROW_ITEM*2)                         //KenHsieh 20220830 : 修改add:AOI Tray Edit
        bHasFile=false;

    if(bHasFile==true)
    {
        for(int i=0; i<_MAX_COL_ITEM; i++)
        {
            for(int j=0; j<_MAX_ROW_ITEM; j++)
            {
                MOT[MMScanAOI].Tray.Data[i][j]=atoi(sList->Strings[i*_MAX_ROW_ITEM+j].c_str());   //KenHsieh 20220830 : 修改add:AOI Tray Edit
            }
        }
    }
    sList->Clear();
    delete sList;
    fAOI_DoIniDataToForm();                                                     //Ifor 20200813 fix:移至檔案讀取後
    UpDataTrayData();

    if(elParameter)                                                             //AI(general) 20260411 (Rogeryang) : fix path缺Recipe子資料夾 + NULL guard
    {
        tmpS.sprintf("%s%s\\", DataPath, GetLastOpenFN());
        elParameter->ReadEditTextFromFile(tmpS, "AOI.Data");
    }

    if(USE_Scanner_AOI_Inspection==(int)eBtnAOI_TopBottomInstall && ttbInsp)
    {
        ttbInsp->SetCommParameter(ttbInsp->sSocketAddress,ttbInsp->sSocketPort);
        for(int i=0;i<2;i++)
        {
            sgICSmall->Cells[1][i+1]=IntToStr(ttbInsp->tbpiLessThanOrEqual65mm[i].ppPosition.X);
            sgICSmall->Cells[2][i+1]=IntToStr(ttbInsp->tbpiLessThanOrEqual65mm[i].ppPosition.Y);
            sgICSmall->Cells[3][i+1]=(ttbInsp->tbpiLessThanOrEqual65mm[i].bClampLT)?"1":"0";
            sgICSmall->Cells[4][i+1]=(ttbInsp->tbpiLessThanOrEqual65mm[i].bClampLB)?"1":"0";
            sgICSmall->Cells[5][i+1]=(ttbInsp->tbpiLessThanOrEqual65mm[i].bClampRT)?"1":"0";
            sgICSmall->Cells[6][i+1]=(ttbInsp->tbpiLessThanOrEqual65mm[i].bClampRB)?"1":"0";
        }
        for(int i=0;i<4;i++)
        {
            sgICLarge->Cells[1][i+1]=IntToStr(ttbInsp->tbpiMoreThan65mm[i].ppPosition.X);
            sgICLarge->Cells[2][i+1]=IntToStr(ttbInsp->tbpiMoreThan65mm[i].ppPosition.Y);
            sgICLarge->Cells[3][i+1]=(ttbInsp->tbpiMoreThan65mm[i].bClampLT)?"1":"0";
            sgICLarge->Cells[4][i+1]=(ttbInsp->tbpiMoreThan65mm[i].bClampLB)?"1":"0";
            sgICLarge->Cells[5][i+1]=(ttbInsp->tbpiMoreThan65mm[i].bClampRT)?"1":"0";
            sgICLarge->Cells[6][i+1]=(ttbInsp->tbpiMoreThan65mm[i].bClampRB)?"1":"0";
        }
    }
    //Update Main Form UI
    if(USE_Scanner_AOI_Inspection>(int)eBtnAOI_Uninstall && fMain)              //Jimmychiu 20240322 : Top & Bottom Inspect
    {
        if((USE_Scanner_AOI_Inspection==(int)eBtnAOI_BottomInstall && ScannerAOIIF.iEnableScannerMode==0) ||
           (FrmAOI->RunTopBottomInspect()==false))
        {
            fMain->labScanAOI->Caption="Scan AOI OFF";
            fMain->labScanAOI->Font->Color=clRed;
        }
        else
        {
            fMain->labScanAOI->Caption="Scan AOI ON";
            fMain->labScanAOI->Font->Color=clBlue;
        }
    }

    if(USE_Scanner_AOI_Inspection==(int)eBtnAOI_TopBottomInstall)               //Eastsun 20260519 KYEC 要求鎖定
    {
        if(CUSTOMER_CODE==CC_KYEC_LEE && ttbInsp != NULL)
        {
            if(IniConfig.bA72_FixAOIButton==1)
            {
                rgActionMode->Controls[0]->Enabled=false;
                rgActionMode->Controls[1]->Enabled=false;
                ttbInsp->iAction=1;
                rgActionMode->ItemIndex=1;
            }
            else
            {
                rgActionMode->Controls[0]->Enabled=true;
                rgActionMode->Controls[1]->Enabled=true;
            }
        }
    }
}
//------------------------------------------------------------------------------
void TFrmAOI::fAOI_DoIniDataToForm()
{
    cbEnabledTopView->Checked = tAOISetup.tTopView.bEnabled;
    edt_FailTopView->Text     = tAOISetup.tTopView.iFailBin;
    edt_TimeoutTopView->Text  = tAOISetup.tTopView.iTimeOut;
    edt_SDelayTopView->Text   = tAOISetup.tTopView.iStartDelayTime;

    cbEnabledPADView->Checked = tAOISetup.tPADView.bEnabled;
    edt_FailPADView->Text     = tAOISetup.tPADView.iFailBin;
    edt_TimeoutPADView->Text  = tAOISetup.tPADView.iTimeOut;
    edt_SDelayPADView->Text   = tAOISetup.tPADView.iStartDelayTime;

    cbEnabledBGAView->Checked = tAOISetup.tBGAView.bEnabled;
    edt_FailBGAView->Text     = tAOISetup.tBGAView.iFailBin;
    edt_TimeoutBGAView->Text  = tAOISetup.tBGAView.iTimeOut;
    edt_SDelayBGAView->Text   = tAOISetup.tBGAView.iStartDelayTime;

    edt_AlarmCount->Text         = tAOISetup.iAlarmCount;
    cbBGAFailRun->Checked        = tAOISetup.tFailRunFunction.bEnabled;   //wei 20160617 Vitrox

    cbAutoSkipBGA->Checked       = tAOISetup.tAutoSkipBGA.bEnabled;       //wei 20160617 Vitrox
    cbAutoSkipPadPkg->Checked    = tAOISetup.tAutoSkipPadPkg.bEnabled;    //wei 20160617 Vitrox
    cbAOINoSort->Checked         = tAOISetup.tAOINoSort.bEnabled;         //wei 20161207

    edt_PassFailBGAPADView->Text = tAOISetup.tBGAPADView.iTestPassFailBin;   //wei  20160706
    edt_FailFailBGAPADView->Text = tAOISetup.tBGAPADView.iTestFailFailBin;   //wei  20160706
    cbTesterFailBin->Checked     = tAOISetup.tTesterFailBin.bEnabled;     //wei  20160706
    cb_VitContiuneAlarm->Checked = tAOISetup.bEnabledContinueAlarm;       //Alick 20160712
    edt_ConAlarmConut->Text      = tAOISetup.iContAlarmCountBySite;       //Alick 20160712
    edt_ConAlarmConut1->Text     = tAOISetup.iContAlarmCountByArm;        //Alick 20160718
    cb_AlarmBySite->Checked      = tAOISetup.bEnabledBySiteBGAView;       //Alick 20160712
    cb_AlarmByArm->Checked       = tAOISetup.bEnabledByArmBGAView;        //Alick 20160712
    cb_AlarmBySite1->Checked     = tAOISetup.bEnabledBySitePADView;       //Alick 20160712
    cb_AlarmByArm1->Checked      = tAOISetup.bEnabledByArmPADView;        //Alick 20160712

    //Ifor 20191225 : add Scan AOI Alarm
    //==>
    cb_ScanAOIAlarmBySite->Checked      = tAOISetup.bEnabledScanAOIBySiteAlarm;
    cb_ScanAOIAlarmByArm->Checked       = tAOISetup.bEnabledScanAOIByArmAlarm;
    edt_ScanAOIAlarmCountBySite->Text   = tAOISetup.iScanAOIAlarmCountBySite;
    edt_ScanAOIAlarmCountByArm->Text    = tAOISetup.iScanAOIAlarmCountByArm;
    cb_ScanAOIUnUseFailBin->Checked     = tAOISetup.bEnabledScanAOIUnUseFailBin;    //Ifor 20200825 add:TF-AMD AOI Error 不丟至Fail Bin
    //<==
    //Ifor 20191225 : add Scan AOI Alarm

    //Ifor 20190724 add AMD AOI
    //==>
    rgScannerMode->ItemIndex        = ScannerAOIIF.iEnableScannerMode;
    edtScannerReadTimeout->Text     = ((int)(ScannerAOIIF.fScannerReadTimeout*100))/100;

    edtAOIIntervalCounter->Text     = ScannerAOIIF.iIntervalCounter;
    edtAOIRetryCounter->Text        = ScannerAOIIF.iRetryCounter;

    rgBallDamageType->ItemIndex     = ScannerAOIIF.iBallDamageType;

    cbBDByTotal->Checked            = ScannerAOIIF.bBDTotalFunction;
    cbBDByTotalConti->Checked       = ScannerAOIIF.bBDTotalContiFunction;
    cbBDBySite->Checked             = ScannerAOIIF.bBDSiteFunction;
    cbBDBySiteConti->Checked        = ScannerAOIIF.bBDSiteContiFunction;
    cbDBAlarmAutoReset->Checked     = ScannerAOIIF.bDBAlramAutoResetCount;

    edtAOIBDCounterTotal->Text      = ScannerAOIIF.iBDTotalCounter;
    edtAOIBDCounterTotalConti->Text = ScannerAOIIF.iBDTotalContiCounter;
    edtAOIBDCounterSite->Text       = ScannerAOIIF.iBDSiteCounter;
    edtAOIBDCounterSiteConti->Text  = ScannerAOIIF.iBDSiteContiCounter;

    pnlAOIBDTotalLast->Caption      = ScannerAOIIF.iBDTotalCounterLast;
    pnlAOIBDTotalContiLast->Caption = ScannerAOIIF.iBDTotalContiCounterLast;
    pnlAOIBDSite1Last->Caption      = ScannerAOIIF.iBDSite1CounterLast;
    pnlAOIBDSite2Last->Caption      = ScannerAOIIF.iBDSite2CounterLast;
    pnlAOIBDSite1ContiLast->Caption = ScannerAOIIF.iBDSite1ContiCounterLast;
    pnlAOIBDSite2ContiLast->Caption = ScannerAOIIF.iBDSite2ContiCounterLast;

    edt_SDelayScanAOI->Text         = ScannerAOIIF.iStartDelayTime;
    edt_TimeoutScanAOI->Text        = ScannerAOIIF.iTimeOut;

    cbDevice->Text                  = ScannerAOIIF.sSCANNER_ComPort;
    cbBaudRate->Text                = ScannerAOIIF.sSCANNER_BaudRate;

    edtScannerICGain->Text          =FormatFloat("0.00", ScannerAOIIF.fScannerICGain);  //Ifor 20200525 add:依據Tray Y Pitch Gain 決定不使用範圍

    if(ScannerAOIIF.sSCANNER_ByteSize == "5")
        cbByteSize->ItemIndex = 0;
    else if(ScannerAOIIF.sSCANNER_ByteSize == "6")
        cbByteSize->ItemIndex = 1;
    else if(ScannerAOIIF.sSCANNER_ByteSize == "7")
        cbByteSize->ItemIndex = 2;
    else if(ScannerAOIIF.sSCANNER_ByteSize == "8")
        cbByteSize->ItemIndex = 3;

    if(ScannerAOIIF.sSCANNER_StopBit == "1" )
        cbStopBit->ItemIndex = 0;
    else if(ScannerAOIIF.sSCANNER_StopBit == "1.5" )
        cbStopBit->ItemIndex = 1;
    else if(ScannerAOIIF.sSCANNER_StopBit == "2" )
        cbStopBit->ItemIndex = 2;

    if(ScannerAOIIF.sSCANNER_Parity == "None" )
        cbParity->ItemIndex = 0;
    else if(ScannerAOIIF.sSCANNER_Parity == "Odd" )
        cbParity->ItemIndex = 1;
    else if(ScannerAOIIF.sSCANNER_Parity == "Even" )
        cbParity->ItemIndex = 2;

    rgAOIFailBinType->ItemIndex     =ScannerAOIIF.iAOIFailBinType;                  //RogerYang 20251120 : 新增AOI測試後分bin也考慮測試結果的選項
    UpdateAOIFailBinTypetoForm();                                                   //RogerYang 20251120 : 新增AOI測試後分bin也考慮測試結果的選項
    cbAOIFial->ItemIndex            =ScannerAOIIF.ScannerIfError;
    cbAOIFialAndTestPass->ItemIndex =ScannerAOIIF.ScannerIfErrorAndTestPass;        //RogerYang 20251120 : 新增AOI測試後分bin也考慮測試結果的選項
    //<==
    //Ifor 20190724 add AMD AOI
    cb_ScanAOIUseLGAMode->Checked   = ScannerAOIIF.bScanAOIUseLGAMode;              //Ifor 20211022 add: AOI LGA Mode
    cb_LGAModeDirection->Checked    = ScannerAOIIF.bLGAModeDirection;               //Ifor 20230202 add: AOI LGA Mode Direction
    cb_EnabledPositionByAOI->Checked= ScannerAOIIF.bEnabledPositionByAOI;           //Ifor 20251031 add: Move Position Provided By AOI
    edtScannerLGAWaitTime->Text     = ((int)(ScannerAOIIF.fLGAWaitTime*100))/100;   //Ifor 20211022 add: AOI LGA Mode
    //Ifor 20200902 add: TFAMD Top AOI
    //==>
    rgTopScannerMode->ItemIndex         = ScannerAOIIF.iEnableTopScannerMode;
    edtTopAOIIntervalCounter->Text      = ScannerAOIIF.iTopIntervalCounter;
    edtTopAOIRetryCounter->Text         = ScannerAOIIF.iTopRetryCounter;
    cbTopAOIFial->ItemIndex             = ScannerAOIIF.TopScannerIfError;
    cb_TopScanAOIAlarmBySite->Checked   = tAOISetup.bEnabledTopScanAOIBySiteAlarm;
    edt_TopScanAOIAlarmCountBySite->Text= tAOISetup.iTopScanAOIAlarmCountBySite;
    cb_TopScanAOIAlarmByArm->Checked    = tAOISetup.bEnabledTopScanAOIByArmAlarm;
    edt_TopScanAOIAlarmCountByArm->Text = tAOISetup.iTopScanAOIAlarmCountByArm;
    edt_SDelayTopScanAOI->Text          = ScannerAOIIF.iTopStartDelayTime;
    edt_TimeoutTopScanAOI->Text         = ScannerAOIIF.iTopTimeOut;
    edtTopScannerReadTimeout->Text      = ((int)(ScannerAOIIF.fTopScannerReadTimeout*100))/100;
    cb_TopScanAOIUnUseFailBin->Checked  = tAOISetup.bEnabledTopScanAOIUnUseFailBin;

    cbTopDevice->Text                  = ScannerAOIIF.sTopSCANNER_ComPort;
    cbTopBaudRate->Text                = ScannerAOIIF.sTopSCANNER_BaudRate;

    if(ScannerAOIIF.sTopSCANNER_ByteSize == "5")
        cbTopByteSize->ItemIndex = 0;
    else if(ScannerAOIIF.sTopSCANNER_ByteSize == "6")
        cbTopByteSize->ItemIndex = 1;
    else if(ScannerAOIIF.sTopSCANNER_ByteSize == "7")
        cbTopByteSize->ItemIndex = 2;
    else if(ScannerAOIIF.sTopSCANNER_ByteSize == "8")
        cbTopByteSize->ItemIndex = 3;

    if(ScannerAOIIF.sTopSCANNER_StopBit == "1" )
        cbTopStopBit->ItemIndex = 0;
    else if(ScannerAOIIF.sTopSCANNER_StopBit == "1.5" )
        cbTopStopBit->ItemIndex = 1;
    else if(ScannerAOIIF.sTopSCANNER_StopBit == "2" )
        cbTopStopBit->ItemIndex = 2;

    if(ScannerAOIIF.sTopSCANNER_Parity == "None" )
        cbTopParity->ItemIndex = 0;
    else if(ScannerAOIIF.sTopSCANNER_Parity == "Odd" )
        cbTopParity->ItemIndex = 1;
    else if(ScannerAOIIF.sTopSCANNER_Parity == "Even" )
        cbTopParity->ItemIndex = 2;
    //<==
    //Ifor 20200902 add: TFAMD Top AOI
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::FormShow(TObject *Sender)
{
    FrmAOI->Height=660;                                                         //RogerYang 20260622 : add

    if(IniConfig.bA74AOIFailCountLinkLotRunMode)
        AOIFailCountRefresh();

    fAOI_ReadFile();
    if(USE_Scanner_AOI_Inspection==(int)eBtnAOI_BottomInstall)
    {
        PageControl1->ActivePageIndex=2;
        TabSheet1->TabVisible=false;
        TabSheet2->TabVisible=false;
        if(USE_Top_Scanner_AOI_Inspection==false)                               //Ifor 20200902 add: TFAMD Top AOI
        {
            TabSheet5->TabVisible=false;
            TabSheet6->TabVisible=false;
        }
    }
    else if(USE_Top_Scanner_AOI_Inspection==true)                               //Ifor 20200902 add: TFAMD Top AOI
    {
        PageControl1->ActivePageIndex=4;
        TabSheet1->TabVisible=false;
        TabSheet2->TabVisible=false;
        TabSheet3->TabVisible=false;
        TabSheet4->TabVisible=false;
    }
    else if(USE_Scanner_AOI_Inspection==(int)eBtnAOI_TopBottomInstall)          //Jimmychiu 20240322 : Top & Bottom Inspect
    {
        CloseAllPage();
        tsTopBtnModSet->TabVisible=true;
        tsTopBtnCommSet->TabVisible=true;
        #ifndef SOFT_SIMULTE
        btnStart->Visible=false;
        btnPause->Visible=false;
        #endif
    }
    else
    {
        PageControl1->ActivePageIndex=0;
        DrawSitePanelVitrox1();
        DrawSitePanelVitrox2();
        TabSheet3->TabVisible=false;
        TabSheet4->TabVisible=false;
        TabSheet5->TabVisible=false;                                            //Ifor 20200902 add: TFAMD Top AOI
        TabSheet6->TabVisible=false;                                            //Ifor 20200902 add: TFAMD Top AOI
        tsTopBtnModSet->TabVisible=false;                                       //Jimmychiu 20240322 : Top & Bottom Inspect
        tsTopBtnCommSet->TabVisible=false;                                      //Jimmychiu 20240322 : Top & Bottom Inspect
        if(cbTesterFailBin->Checked)
        {
            Label11->Visible=false;
            edt_FailBGAView->Visible=false;
            Label6->Visible=false;
            edt_FailPADView->Visible=false;
            Panel3->Visible=true;
        }
        else
        {
            Label11->Visible=true;
            edt_FailBGAView->Visible=true;
            Label6->Visible=true;
            edt_FailPADView->Visible=true;
            Panel3->Visible=false;
        }
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE && ttbInsp != NULL)                           //Eastsun 20260519 整合
    {
        if(IniConfig.bA72_FixAOIButton==1)
        {
            rgActionMode->Controls[0]->Enabled=false;
            rgActionMode->Controls[1]->Enabled=false;
            ttbInsp->iAction=1;
        }
        else
        {
            rgActionMode->Controls[0]->Enabled=true;
            rgActionMode->Controls[1]->Enabled=true;
        }
    }

    if(IniConfig.bA74AOIFailCountLinkLotRunMode)
    {
        scroAOIFailCountLinkLotRunMode->Visible = true;                                  //Eastsun 20260401
    }
    else
    {
        scroAOIFailCountLinkLotRunMode->Visible = false;                                  //Eastsun 20260401
    }

    fShow=true;
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::sbtExitClick(TObject *Sender)
{
    sbtExit->Down =false;
    rbSetFocus->SetFocus();                                                     //Ifor 20200818 add:避免按下離開Scanner Mode無法更新問題
    Close();
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::edt_FailTopViewClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 1, 15);
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::edtAOIRetryCounterClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 0, 15);
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::edt_SDelayTopViewClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 0, 5000);
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::edt_AlarmCountClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 1, 10000);
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::edtScannerReadTimeoutClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 0, true, 1, 120);
}
//------------------------------------------------------------------------------
bool TFrmAOI::CheckFailBin()
{
    int iBin[5];

    iBin[1]=atoi(edt_FailPADView->Text.c_str());
    iBin[2]=atoi(edt_FailBGAView->Text.c_str());

    iBin[3]=atoi(edt_PassFailBGAPADView->Text.c_str());
    iBin[4]=atoi(edt_FailFailBGAPADView->Text.c_str());

    if(cbEnabledBGAView->Checked || cbEnabledPADView->Checked || cbEnabledTopView->Checked)
    {
        for(int i=1; i<sizeof(iBin)/sizeof(int); i++)
        {
            if(Prod.bIsPassBin[iBin[i]])
            {
                return false;
            }
        }
    }
    return true;
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::DrawSitePanelVitrox1()                                  //Alick 20160712 add
{
    mtDutOnOff_Vitrox->XItem=TestSocket.iShtCol;
    mtDutOnOff_Vitrox->YItem=TestSocket.iShtRow;
    mtDutCol_Vitrox->XItem=TestSocket.iShtCol;
    mtDutRow_Vitrox->YItem=TestSocket.iShtRow;

    for(int i=0; i<TestSocket.iShtCol; i++)
    {
        mtDutCol_Vitrox->SetCellNumber(i, 0, AnsiString(char('a'+i)).c_str());
        mtDutCol_Vitrox->SetCellColorIndex(i, 0, 0);
    }

    for(int j=0; j<TestSocket.iShtRow; j++)
    {
        mtDutRow_Vitrox->SetCellNumber(0, j, AnsiString(char('A'+j)).c_str());
        mtDutRow_Vitrox->SetCellColorIndex(0, j, 0);
    }

    int z=1;
    for(int i=0; i<TestSocket.iShtCol; i++)
    {
        for(int j=0; j<TestSocket.iShtRow; j++)
        {
            mtDutOnOff_Vitrox->SetCellNumber(i, j, z);
            z+=1;
            bVitroxBGAViewUse[i][j]=bVitroxBGAViewMap[i][j];
            if(bVitroxBGAViewUse[j][i])
                mtDutOnOff_Vitrox->SetCellColorIndex(i, j, 0);
            else
                mtDutOnOff_Vitrox->SetCellColorIndex(i, j, 2);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::DrawSitePanelVitrox2()                                  //Alick 20160714 add
{
    mtDutOnOff_Vitrox1->XItem=TestSocket.iShtCol;
    mtDutOnOff_Vitrox1->YItem=TestSocket.iShtRow;
    mtDutCol_Vitrox1->XItem=TestSocket.iShtCol;
    mtDutRow_Vitrox1->YItem=TestSocket.iShtRow;

    for(int i=0; i<TestSocket.iShtCol; i++)
    {
        mtDutCol_Vitrox1->SetCellNumber(i, 0, AnsiString(char('a'+i)).c_str());
        mtDutCol_Vitrox1->SetCellColorIndex(i, 0, 0);
    }

    for(int j=0; j<TestSocket.iShtRow; j++)
    {
        mtDutRow_Vitrox1->SetCellNumber(0, j, AnsiString(char('A'+j)).c_str());
        mtDutRow_Vitrox1->SetCellColorIndex(0, j, 0);
    }

    int z=1;
    for(int i=0; i<TestSocket.iShtCol; i++)
    {
        for(int j=0; j<TestSocket.iShtRow; j++)
        {
            mtDutOnOff_Vitrox1->SetCellNumber(i, j, z);
            z+=1;
            bVitroxPADViewUse[i][j]=bVitroxPADViewMap[i][j];
            if(bVitroxPADViewUse[j][i])
                mtDutOnOff_Vitrox1->SetCellColorIndex(i, j, 0);
            else
                mtDutOnOff_Vitrox1->SetCellColorIndex(i, j, 2);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::mtDutOnOff_Vitrox1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(SystemStart)
        return;

    if(mtDutOnOff_Vitrox1->ConvertIndexCells(X,Y)==-1)
        return;
     bVitroxPADViewUse[Y][X]=!bVitroxPADViewUse[Y][X];
     if(bVitroxPADViewUse[Y][X])
        mtDutOnOff_Vitrox1->SetCellColorIndex(X, Y, 0);
     else
        mtDutOnOff_Vitrox1->SetCellColorIndex(X, Y, 2);
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::mtDutOnOff_VitroxMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(SystemStart)
        return;

    if(mtDutOnOff_Vitrox->ConvertIndexCells(X,Y)==-1)
        return;
     bVitroxBGAViewUse[Y][X]=!bVitroxBGAViewUse[Y][X];
     if(bVitroxBGAViewUse[Y][X])
        mtDutOnOff_Vitrox->SetCellColorIndex(X, Y, 0);
     else
        mtDutOnOff_Vitrox->SetCellColorIndex(X, Y, 2);
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::cbTesterFailBinMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(cbTesterFailBin->Checked)
    {
        Label11->Visible=false;
        edt_FailBGAView->Visible=false;
        Label6->Visible=false;
        edt_FailPADView->Visible=false;
        Panel3->Visible=true;
    }
    else
    {
        Label11->Visible=true;
        edt_FailBGAView->Visible=true;
        Label6->Visible=true;
        edt_FailPADView->Visible=true;
        Panel3->Visible=false;
    }
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::FormDestroy(TObject *Sender)
{
    try
    {
        if(ttbInsp)
            delete ttbInsp;
        if(elParameter)
        delete elParameter;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TFrmAOI::FormDestroy");
    }
    LogSoftwareOffTime("TFrmAOI, FormDestroy");                                 //Steven 20210526 : 紀錄軟體執行時間
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::spbStopComClick(TObject *Sender)
{
    TSpeedButton* pBtn=(TSpeedButton*)Sender;
    if(pBtn->Tag==1)
        TopAOIComm->StopComm();
    else
        AOIComm->StopComm();
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::spbStartComClick(TObject *Sender)
{
    TSpeedButton* pBtn=(TSpeedButton*)Sender;
    if(pBtn->Tag==1)
        TopAOIComm->StopComm();
    else
        AOIComm->StopComm();
    RS232Init();
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::RS232Init()
{
    #ifndef SOFT_SIMULTE
    bool flag;
    AnsiString Str;
    flag=GetCOMPortStatus(ScannerAOIIF.sSCANNER_ComPort);
    if(flag==false)
    {
        Str.sprintf("Scanner AOI : %s port status error!", ScannerAOIIF.sSCANNER_ComPort);
        ShowMyMessage(Str);
    }
    else
    {
        AOIComm->CommName="\\\\.\\"+ScannerAOIIF.sSCANNER_ComPort;
        try
        {
            AOIComm->BaudRate=atoi(ScannerAOIIF.sSCANNER_BaudRate.c_str());

            if(ScannerAOIIF.sSCANNER_ByteSize == "5")
                AOIComm->ByteSize = _5;
            else if(ScannerAOIIF.sSCANNER_ByteSize == "6")
                AOIComm->ByteSize = _6;
            else if(ScannerAOIIF.sSCANNER_ByteSize == "7")
                AOIComm->ByteSize = _7;
            else if(ScannerAOIIF.sSCANNER_ByteSize == "8")
                AOIComm->ByteSize = _8;

            if(ScannerAOIIF.sSCANNER_StopBit == "1" )
                AOIComm->StopBits = _1;
            else if(ScannerAOIIF.sSCANNER_StopBit == "1.5" )
                AOIComm->StopBits = _1_5;
            else if(ScannerAOIIF.sSCANNER_StopBit == "2" )
                AOIComm->StopBits = _2;

            if(ScannerAOIIF.sSCANNER_Parity == "None" )
                AOIComm->Parity = None;
            else if(ScannerAOIIF.sSCANNER_Parity == "Odd" )
                AOIComm->Parity = Odd;
            else if(ScannerAOIIF.sSCANNER_Parity == "Even" )
                AOIComm->Parity = Even;

            AOIComm->StopComm();
            AOIComm->StartComm();                                               //僅能啟動一次
        }
        catch(...)
        {
        }
    }
    #endif
}
//------------------------------------------------------------------------------
void TFrmAOI::ShowCommData(AnsiString sType, AnsiString sData, int iNum)        //Ifor 20200908 add:可使用多台Scan AOI 裝置
{
    AnsiString sLog     =sType+sData;
    AnsiString sHexLog  =sType+"";

    if(iNum==1)
    {
        if(TopMemoLogString->Lines->Count>300)
        {
            TopMemoLogString->Clear();
        }

        TopMemoLogString->Lines->Append(sLog);

        if(TopMemoLogHex->Lines->Count>300)
        {
            TopMemoLogHex->Clear();
        }
    }
    else
    {
        if(MemoLogString->Lines->Count>300)
        {
            MemoLogString->Clear();
        }

        MemoLogString->Lines->Append(sLog);

        if(MemoLogHex->Lines->Count>300)
        {
            MemoLogHex->Clear();
        }
    }

    for(int i=0; i<sData.Length(); i++)
    {
        Byte dData=(Byte)sData[i+1];                                            // 2013.05.31 , Joye , AMD-SG UBS
        sHexLog+=IntToHex((int)dData, 2)+" ";                                   // 2013.05.31 , Joye , AMD-SG UBS
    }

    if(iNum==1)
        TopMemoLogHex->Lines->Append(sHexLog);
    else
        MemoLogHex->Lines->Append(sHexLog);
}
//------------------------------------------------------------------------------
void TFrmAOI::SendCommand(AnsiString sData, int iNum)                           //Ifor 20200908 add:可使用多台Scan AOI 裝置
{
    int iSize=sData.Length();
    if(iSize<=0)
    {
        ShowCommData("[Send]", sData, iNum);                                    //Ifor 20200908 add:可使用多台Scan AOI 裝置
        ShowCommData("[Send Data Fail]", " Size = 0", iNum);                    //Ifor 20200908 add:可使用多台Scan AOI 裝置
        return;
    }

    char* Buff;
    Buff=new char[iSize+1];

    for(int i=0; i<iSize; i++)
    {
        Buff[i]=sData[i+1];
    }

    Buff[iSize]='\0';

    if(iNum==0)
        AOIComm->WriteCommData(Buff, strlen(Buff));
    else
        TopAOIComm->WriteCommData(Buff, strlen(Buff));

    ShowCommData("[Send]", sData, iNum);                                        //Ifor 20200908 add:可使用多台Scan AOI 裝置
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::sbSendClick(TObject *Sender)
{
    TSpeedButton* pBtn=(TSpeedButton*)Sender;
    int iNum=pBtn->Tag;

    SendCommand(edtManual->Text, iNum);                                         //Ifor 20200908 add:可使用多台Scan AOI 裝置
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::sbClearClick(TObject *Sender)
{
    TSpeedButton* pBtn=(TSpeedButton*)Sender;
    if(pBtn->Tag==0)
    {
        MemoLogString->Clear();
        MemoLogHex->Clear();
    }
    else
    {
        TopMemoLogString->Clear();
        TopMemoLogHex->Clear();
    }
}
//------------------------------------------------------------------------------
void TFrmAOI::GetScannerResult(AnsiString sData)
{
    AnsiString sResult;

    int iPos1=sData.Pos("site");

    if(iPos1==0)
    {
        iAOIResult[0]   =-1;
    }
    else
    {
        sResult         =sData.SubString(iPos1+6 , 1);
        iAOIResult[0]   =atoi(sResult.c_str());
    }

    if(iAOIResult[0]==-1)
    {
        FrmAOI->ShowCommData("[Decode] Fail ", "Abnormal data");
    }
    else if(iAOIResult[0]!=1)
    {
        FrmAOI->ShowCommData("[Decode] Fail ", sResult);
    }
    else
    {
        FrmAOI->ShowCommData("[Decode] Pass ", sResult);
    }
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::AOICommReceiveData(TObject *Sender,
      Pointer Buffer, WORD BufferLength)
{
    char *data;
    data=(char*)Buffer;
    AnsiString sRead;
    AnsiString sReadChk;
    int iDataCount=0;
    int j=strlen(data);
    AnsiString LogMsg="";

    sRead="";
    for(int i=0; i<j; i++)
    {
        sRead+=data[i];
    }
    ShowCommData("[Read]", sRead);

    iDataCount= BufferLength-2;

    if(j<1+iDataCount+1)
    {
        ShowCommData("[Error]", " Read Length Error!!");
        return;
    }

    sReadChk=sRead.SubString(2,iDataCount);
    try                                                                         //Richard 20220817:Add Scanner_AOI add check Big Size IC Function
    {
        TStringList *sList=new TStringList();
        sList->CommaText=sReadChk;
        if(sList->Strings[0] == "GetGrabPos")
        {
            if(sList->Count==5)
            {
                iLGAPitchXCnt=atoi(sList->Strings[1].c_str());
                iLGAPitchYCnt=atoi(sList->Strings[2].c_str());
                iLGAPitchX   =atoi(sList->Strings[3].c_str());
                iLGAPitchY   =atoi(sList->Strings[4].c_str());
            }
        }
        delete sList;
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    try                                                                         //Ifor 20251031 add:TFAMD  AOI    2~5
    {
        if(ScannerAOIIF.bEnabledPositionByAOI==true)
        {
            if(sRead.Pos("NEXTPOS")>0)                                          //Ifor 20260518 add: AOI
            {
                TStringList *sList = new TStringList;
                AnsiString asPointScan=sRead.SubString(11, sRead.Length()-11);
                sList->CommaText=asPointScan;
                if(sList->Count>=2)
                {
                    iLGAPitchX=sList->Strings[0].ToIntDef(0)/10;                //Ifor 20260519 add:     10
                    iLGAPitchY=sList->Strings[1].ToIntDef(0)/10;                //Ifor 20260519 add:     10
                }
                iScanLGAStep++;
                sList->Clear();
                delete sList;
                return;
            }
        }
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }

    char *chk=sReadChk.c_str();
    Byte k=0;
    for(int i=0; i<iDataCount; i++)
    {
        k+=Byte(chk[i]);
    }
    k=k%0x100;
    sReadChk.sprintf("%c",k);
    char *chk1=sReadChk.c_str();

    if(data[j-1]!=chk1[0])
    {
        ShowCommData("[Error]", " Checksum Error!!");
        return;
    }
    bTriggerAOI=false;
    GetScannerResult(sRead);
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::sbBDTotalResetClick(TObject *Sender)
{
    TSpeedButton* pBtn=(TSpeedButton*)Sender;

    if(pBtn->Tag==1)
    {
        pnlAOIBDTotalLast->Caption              ="0";
        ScannerAOIIF.iBDTotalCounterLast        =0;
    }
    else if(pBtn->Tag==2)
    {
        pnlAOIBDTotalContiLast->Caption         ="0";
        ScannerAOIIF.iBDTotalContiCounterLast   =0;
    }
    else if(pBtn->Tag==3)
    {
        pnlAOIBDSite1Last->Caption              ="0";
        pnlAOIBDSite2Last->Caption              ="0";

        ScannerAOIIF.iBDSite1CounterLast        =0;
        ScannerAOIIF.iBDSite2CounterLast        =0;
    }
    else if(pBtn->Tag==4)
    {
        pnlAOIBDSite1ContiLast->Caption         ="0";
        pnlAOIBDSite2ContiLast->Caption         ="0";

        ScannerAOIIF.iBDSite1ContiCounterLast   =0;
        ScannerAOIIF.iBDSite2ContiCounterLast   =0;
    }
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::TriggerAOISystem(bool bSite1, bool bSite2, int iNum)   //Ifor 20200908 add:可使用多台Scan AOI 裝置
{
    Byte k=0;
    Byte Blength=0x07;
    AnsiString asR="";
    AnsiString asChk="";

    if(ScannerAOIIF.bEnabledPositionByAOI==true)                                //Ifor 20251031 add: Move Position Provided By AOI
    {
        if(iScanLGAStep==0)
        {
            asChk.sprintf("%d%dBDAOI",bSite1,bSite2);
        }
        else
        {
            Blength=0x05;
            asChk.sprintf("%d%dTRG",bSite1,bSite2);
        }
    }
    else
    {
        asChk.sprintf("%d%dBDAOI", bSite1, bSite2);
    }

    unsigned char *chk=asChk.c_str();
    int j=asChk.Length();
    for(int i=0; i<j; i++)
    {
        k+=Byte(chk[i]);
    }
    k=k%0x100;
    asR.sprintf("%c%s%c", Blength, asChk, k);
    bTriggerAOI=true;
    SendCommand(asR, iNum);                                                     //Ifor 20200908 add:可使用多台Scan AOI 裝置
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::sbAOISendClick(TObject *Sender)
{
    TSpeedButton* pBtn = (TSpeedButton*)Sender;
    int iNum=pBtn->Tag;

    if(pBtn->Tag==1)
    {
        if(rgTopAOISiteMode->ItemIndex==0)
        {
            FrmAOI->TriggerAOISystem(true, false, iNum);
        }
        else if(rgTopAOISiteMode->ItemIndex==1)
        {
            FrmAOI->TriggerAOISystem(false, true, iNum);
        }
        else if(rgTopAOISiteMode->ItemIndex==2)
        {
            FrmAOI->TriggerAOISystem(true, true, iNum);
        }
    }
    else
    {
        if(rgAOISiteMode->ItemIndex==0)
        {
            FrmAOI->TriggerAOISystem(true, false, iNum);
        }
        else if(rgAOISiteMode->ItemIndex==1)
        {
            FrmAOI->TriggerAOISystem(false, true, iNum);
        }
        else if(rgAOISiteMode->ItemIndex==2)
        {
            FrmAOI->TriggerAOISystem(true, true, iNum);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::CheckBallDamageCounter()
{
    if(bAOIBallDamageCounterFailAlarm==true)                                    // 2012.12.17 , Joye , AMD HT-7046M
        return;

    if(ScannerAOIIF.bBDTotalFunction==true)
    {
        if(ScannerAOIIF.iBDTotalCounterLast>=ScannerAOIIF.iBDTotalCounter)
        {                                                                       // Ball Damage Fail Alarm
            ShowErrorMessage("WAR0870", K_SKIP, MMSystem, false);               //Ifor 20191205 : add Ball AOI
            bAOIBallDamageCounterFailAlarm=true;
        }
    }
    else
    {
        ScannerAOIIF.iBDTotalCounterLast=0;
    }

    if(ScannerAOIIF.bBDTotalContiFunction==true)
    {
        if(ScannerAOIIF.iBDTotalContiCounterLast>=ScannerAOIIF.iBDTotalContiCounter)
        {                                                                       // Ball Damage Fail Alarm
            ShowErrorMessage("WAR0873", K_SKIP, MMSystem, false);               //Ifor 20191205 : add Ball AOI
            bAOIBallDamageCounterFailAlarm=true;
        }
    }
    else
    {
        ScannerAOIIF.iBDTotalContiCounterLast=0;
    }

    if(ScannerAOIIF.bBDSiteFunction==true)
    {
        if(ScannerAOIIF.iBDSite1CounterLast>=ScannerAOIIF.iBDSiteCounter)
        {                                                                       // Ball Damage Fail Alarm
            ShowErrorMessage("WAR0871", K_SKIP, MMSystem, false);               //Ifor 20191205 : add Ball AOI
            bAOIBallDamageCounterFailAlarm=true;
        }

        if(ScannerAOIIF.iBDSite2CounterLast>=ScannerAOIIF.iBDSiteCounter)
        {                                                                       // Ball Damage Fail Alarm
            ShowErrorMessage("WAR0872", K_SKIP, MMSystem, false);               //Ifor 20191205 : add Ball AOI
            bAOIBallDamageCounterFailAlarm=true;
        }
    }
    else
    {
        ScannerAOIIF.iBDSite1CounterLast=0;
        ScannerAOIIF.iBDSite2CounterLast=0;
    }

    if(ScannerAOIIF.bBDSiteContiFunction==true)
    {
        if(ScannerAOIIF.iBDSite1ContiCounterLast>=ScannerAOIIF.iBDSiteContiCounter)
        {                                                                       // Ball Damage Fail Alarm
            ShowErrorMessage("WAR0874", K_SKIP, MMSystem, false);               //Ifor 20191205 : add Ball AOI
            bAOIBallDamageCounterFailAlarm=true;
        }

        if(ScannerAOIIF.iBDSite2ContiCounterLast>=ScannerAOIIF.iBDSiteContiCounter)
        {                                                                       // Ball Damage Fail Alarm
            ShowErrorMessage("WAR0875", K_SKIP, MMSystem, false);               //Ifor 20191205 : add Ball AOI
            bAOIBallDamageCounterFailAlarm=true;
        }
    }
    else
    {
        ScannerAOIIF.iBDSite1ContiCounterLast=0;
        ScannerAOIIF.iBDSite2ContiCounterLast=0;
    }

    if(bAOIBallDamageCounterFailAlarm==true)
    {
        iOneCycle=1;
    }
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::DoContinuousFailBySiteScanAOI(bool Res, int iRow, int iCol, int iNum)
{
    if(iNum==0)
    {
        if(Res==true)
        {
            iScanAOIBySiteTotal[iRow][iCol]++;
        }
        else
        {
            iScanAOIBySiteTotal[iRow][iCol]=0;
        }

        if(iScanAOIBySiteTotal[iRow][iCol]>=tAOISetup.iScanAOIAlarmCountBySite)
        {
            bScanAOIBySiteFailCont=true;
        }
    }
    else
    {
        if(Res==true)
        {
            iTopScanAOIBySiteTotal[iRow][iCol]++;
        }
        else
        {
            iTopScanAOIBySiteTotal[iRow][iCol]=0;
        }

        if(iTopScanAOIBySiteTotal[iRow][iCol]>=tAOISetup.iTopScanAOIAlarmCountBySite)
        {
            bTopScanAOIBySiteFailCont=true;
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::DoContinuousFailByArmScanAOI(bool Res, int ArmSide, int iNum)
{
    if(iNum==0)
    {
        if(Res==true)
        {
            iScanAOIByArmTotal[ArmSide]++;
        }
        else
        {
            iScanAOIByArmTotal[ArmSide]=0;
        }

        if(iScanAOIByArmTotal[ArmSide]>=tAOISetup.iScanAOIAlarmCountByArm)
        {
            bScanAOIByArmFailCont=true;
            bScanAOIByArmDouChk[ArmSide]=true;
        }
    }
    else
    {
        if(Res==true)
        {
            iTopScanAOIByArmTotal[ArmSide]++;
        }
        else
        {
            iTopScanAOIByArmTotal[ArmSide]=0;
        }

        if(iTopScanAOIByArmTotal[ArmSide]>=tAOISetup.iTopScanAOIAlarmCountByArm)
        {
            bTopScanAOIByArmFailCont=true;
            bTopScanAOIByArmDouChk[ArmSide]=true;
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::edtScannerICGainMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 1.50, 0.50);
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::edtScannerLGAWaitTimeMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 20.0, 1.0);
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::mtAOIBufferMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(HasICUnderMachine()==false)
    {
        EditTray(MMScanAOI);
        UpDataTrayData();
    }
    else
    {
        ShowErrorMessage("MES1646", 0, MMSystem, false, "AOI Tray Edit");       //Must finish [Clean out]!!
    }
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::UpDataTrayData()
{
    if(InitialOK==false)
        return;

    mtAOIBuffer->XItem=MOT[MManualTray2].Tray.XItem;
    mtAOIBuffer->YItem=MOT[MManualTray2].Tray.YItem;
    MOT[MMScanAOI].Tray.XItem=MOT[MManualTray2].Tray.XItem;
    MOT[MMScanAOI].Tray.YItem=MOT[MManualTray2].Tray.YItem;
    for(int i=0; i<MOT[MMScanAOI].Tray.XItem; i++)
    {
        for(int j=0; j<MOT[MMScanAOI].Tray.YItem; j++)
        {
            mtAOIBuffer->SetCellColorIndex(i, j, MOT[MMScanAOI].Tray.Data[i][j]);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TFrmAOI::rgAOIFailBinTypeClick(TObject *Sender)                 //RogerYang 20251120 : 新增AOI測試後分bin也考慮測試結果的選項
{
    UpdateAOIFailBinTypetoForm();
}
//---------------------------------------------------------------------------
void __fastcall TFrmAOI::UpdateAOIFailBinTypetoForm()                           //RogerYang 20251120 : 新增AOI測試後分bin也考慮測試結果的選項
{
    if(rgAOIFailBinType->ItemIndex==0)
    {
        lblAOIBinSel1->Caption          ="AOI Fail :";
        lblAOIBinSel2->Visible          =false;
        cbAOIFialAndTestPass->Visible   =false;
    }
    else
    {
        lblAOIBinSel1->Caption          ="AOI Fail + Test Fail :";
        lblAOIBinSel2->Visible          =true;
        cbAOIFialAndTestPass->Visible   =true;
    }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TFrmAOI::btnTopBtm_ConnectClick(TObject *Sender)
{
    if(ttbInsp->IsCommActive())
        ttbInsp->CommClose();
    else
        ttbInsp->DoCommuncation();
    UpdateConnectionState(btnTopBtm_Connect);
}
//---------------------------------------------------------------------------
void TFrmAOI::UpdateConnectionState(TBitBtn * btn)
{
    if(USE_Scanner_AOI_Inspection==(int)eBtnAOI_TopBottomInstall)
    {
        if(ttbInsp->IsCommActive())
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
}
//---------------------------------------------------------------------------
void TFrmAOI::SetSimulationState(bool bswitch)
{
    bSimulate=bswitch;
    if(bswitch)
    {
        btnSimulateTopBtm->Caption="Running...";
    }
    else
    {
        btnSimulateTopBtm->Caption="Run";
    }
}
//---------------------------------------------------------------------------
bool TFrmAOI::RunTopBottomInspect()
{
    if(ttbInsp)
        return USE_Scanner_AOI_Inspection==(int)eBtnAOI_TopBottomInstall && ttbInsp->iEnable==1;
    else
        return false;
}
//---------------------------------------------------------------------------
void TFrmAOI::CloseAllPage()
{
    for(int i=0;i<PageControl1->PageCount;i++)
    {
        TTabSheet *page=PageControl1->Pages[i];
        page->TabVisible=false;
    }
}
//---------------------------------------------------------------------------
void TFrmAOI::ShowMsgTopBtmCommLog(AnsiString asStr)
{
    if(mTopBtmCommLog->Lines->Count>50)
        mTopBtmCommLog->Lines->Clear();
    mTopBtmCommLog->Lines->Add(asStr);
}
//---------------------------------------------------------------------------
void TFrmAOI::RecordMsgTopBtmComm(AnsiString asMsg)
{
    asMsg=FormatDateTime("yyyy-mm-dd HH:MM:ss ", Now())+asMsg;
    ShowMsgTopBtmCommLog(asMsg);
    ShowMyMessage(asMsg);
}
//---------------------------------------------------------------------------
void TFrmAOI::ShowTopBtnTask(AnsiString sMsg)
{
    blTopBtmTask->Caption="Task:"+sMsg;
}
//---------------------------------------------------------------------------
void TFrmAOI::DoTopBottomInspectSpin()
{
    if(ttbInsp && RunTopBottomInspect() && DoSpinDelay.Off())
    {
        DoSpinDelay.SetSecAndOn(0.01);
        ttbInsp->bSimuAOICommand=cbSkipAOI->Checked;
        ttbInsp->bSimuMot=cbSkipMotor->Checked;
        ttbInsp->bCycleRun=cbCycleRun->Checked;
        int icsize=(cbICBig->Checked)?1:0;

        if(DeviceForm_File.XDimension>=65 || DeviceForm_File.YDimension>=65)
            icsize=1;
        ttbInsp->SetIC_Size(icsize);
        if(GetSimulationState())
        {
            ShowTopBtnTask(ttbInsp->GetTaskInfo());
            if(ttbInsp->DoSimulationTopBtmInspFunc(false,sTopBtmInspErrMsg))
            {
                SetSimulationState(false);
                RecordMsgTopBtmComm("SimulateRemainIC Finish");
            }
        }
        else if(bSimuSingleComm) //single command
        {
            if(DoSingleComm(false,iDoSingleComm))
            {
                bSimuSingleComm=false;
                iDoSingleComm=0;
            }
        }
    }
}
//---------------------------------------------------------------------------
void TFrmAOI::InitialTopBtmSG(TStringGrid* sg)
{
    int iCol=sg->ColCount;
    if(iCol<=0) return;                                                  //Steven 20260421 : add zero-guard for ColCount
    int iAvgColW=(int)(sg->Width/iCol)-1;
    for(int i=0;i<iCol;i++)
    {
        sg->ColWidths[i]=iAvgColW;
    }
    sg->OnDblClick=sgICSmallDblClick;
}
//---------------------------------------------------------------------------
void TFrmAOI::InitialTopBtmSGTitle(TStringGrid* sg)
{
    //col title
    for(int i=0;i<6;i++)
    {
        sg->Cells[i+1][0]=GetStringGridTitle(i);
    }
    //row title
    int iRow=sg->RowCount;
    for(int i=1;i<iRow;i++)
    {
        sg->Cells[0][i]=IntToStr(i);
    }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TTopBottomInspect::TTopBottomInspect()
{
    socketAOI=new TAOISocket();
    socketAOI->SetRecordMsg(RecordMsg);
    tsAnalysis=new TStringList;
    bSimuAOICommand=false;
    bSimuMot=false;
    bCycleRun=false;
    sErrorMsg="";
    asSendMsg="";
    asRecMsg="";
    sTemp="";
    asErrorMsg="";
    sCamaName="CM1";
    iTakePhotoTask=0;
    iTopBtmInspFuncTask=0;
    iTopBtmInspFuncInspTask=0;
    iPhotosCenterposTask=0;
    iPhotos4posTask=0;
    iMoveFixedSeatAndCCD2ReadyTask=0;
    iSimulationTopBtmInspTask=0;
    iIC_Size=0;
    iEnable=0;
    sSocketAddress="";
    sSocketPort="";
    SetSimuICInfo();
    iCCD_Up_Z=0;
    iStartDelayTime=0;
    iTimeout=0;
    tbCenter.Clear();
    iRotate0Pos=0;
    iRotate180Pos=0;
    iRetryCount=0;
}
//---------------------------------------------------------------------------
TTopBottomInspect::~TTopBottomInspect()
{
    try
    {
        delete tsAnalysis;
        delete socketAOI;
        socketAOI=NULL;
    }
    catch(...)
    {
    }
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoStartInspect(bool bIsFirst)
{
    if(bIsFirst)
    {
        DoSendCommand(true,"");
        if(bSimuAOICommand)
        {
            if(t_ti.iPosName==(int)ePC_Top)
            {
                asSendMsg="~TOPINSP,CM1,20200601100857,L2OOXXGG_FT,1_0,1,8692,NA,1/4@";
            }
            else
            {
                asSendMsg="~BOTINSP,CM2,20200601100857,L2OOXXGG_FT,1_0,1,8692,NA,1/4@";
            }
        }
        else
        {
            asSendMsg=GetInspectCommand(t_ti);
        }
        return true;
    }
    else
    {
        if(bSimuAOICommand)
        {
            if(t_ti.iPosName==(int)ePC_Top)
            {
                asRecMsg="~TOPINSP_ACK,OK@";
            }
            else
            {
                asRecMsg="~BOTINSP_ACK,OK@";
            }
            RecordMsg("[Receive]"+asRecMsg);
        }
        else
        {
            asRecMsg=DoSendCommand(false,asSendMsg);
        }

        if(asRecMsg!="")
        {
            tsAnalysis->CommaText=asRecMsg;
            if(tsAnalysis->Count==2)//~TOPINSP_ACK,OK@
            {
                if(tsAnalysis->Strings[1].Pos("OK@")!=1)
                    RecordErrorMsg(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asRecMsg));
                return true;
            }
            else if(tsAnalysis->Count==3)//~TOPINSP_ACK,NG,ErrorMessage@
            {
                if(tsAnalysis->Strings[1].Pos("NG")!=1)
                    RecordErrorMsg(GetErrorMsg(__FUNC__,GetAOIReturnError(),asRecMsg));
                else
                    RecordErrorMsg(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asRecMsg));
                return true;
            }
            else//Error
                RecordErrorMsg(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asRecMsg));
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoImagingCompleted(bool bIsFirst)
{
    if(bIsFirst)
    {
        DoSendCommand(true,asSendMsg);
        return true;
    }
    else
    {
        if(bSimuAOICommand)
        {
            asRecMsg="~GRAB_FINISH,"+t_ti.CamName+"@";
            RecordMsg("[Receive]"+asRecMsg);
        }
        else
        {
            asRecMsg=socketAOI->asReceiveMsg;
        }

        if(asRecMsg!="")
        {
            if(asRecMsg!="~GRAB_FINISH,"+t_ti.CamName+"@")
            {
                RecordErrorMsg(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asRecMsg));
            }
            else
            {
                asSendMsg=AnsiString().sprintf("~GRAB_FINISH_ACK,%s@",t_ti.CamName);
                if(bSimuAOICommand)
                {
                    RecordMsg("[Send]"+asSendMsg);
                }
                else
                {
                    DoSendCommand(false,asSendMsg);
                }
            }
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoRecordResult()
{
    static int iConFailCount=0;
    static int iConFailCountPicture[4]={0, 0, 0, 0};
    static int iAccumulatedCount=0;

    AnsiString Str, asFailCode, asFailPos;
//    int iRow, iCol, iBin, ct, iOldBin;
    int iRow, iCol, ct, iOldBin;

    AnsiString asStr[4], S;
//    int iLen, iPos1;
    int iPos1;

    GetNeedActSuck(iRow,iCol);
    iAOIYieldTotalCount++;
    iAOILotCount++;
    for(int i=0; i<iTestBinCount; i++)
    {
        if(Prod.iT6CatData[i]==OutArmSuck.iWhichAuto[iRow][iCol])
        {
            iOldBin=i;
        }
    }

    AnsiString s4="";
    AnsiString s5="";
    if(bSimuAOICommand)
    {
        s4="PASS";
        s5="00000";
    }
    else
    {
        s4=tsAnalysis->Strings[4];
        s5=tsAnalysis->Strings[5];
    }

    if(s4=="PASS")
    {
        iConFailCount=0;
        iConFailCountPicture[0]=0;
        iConFailCountPicture[1]=0;
        iConFailCountPicture[2]=0;
        iConFailCountPicture[3]=0;
        iAOIYieldPassCount++;
        AddAOIRecord("Pass", "", iOldBin, "", iAOILotCount);
    }
    else
    {
        Str=s4;
        asFailCode=Str.SubString(5, Str.Length()-4);
        Str=s5;
        asFailPos=Str.SubString(1, Str.Length()-1);

        AddAOIRecord("Fail", asFailPos, iOldBin, asFailCode, iAOILotCount);

//        ct=FrmAOI->ttbInsp->iAOIFailSetBin;                                     //Frank 20260317
//        iBin=(ct>iTestBinCount-1)?Prod.IfError:Prod.CatData[ct];                //Frank 20260317
//        OutArmSuck.iWhichAuto[iRow][iCol]=iBin;                                 //Frank 20260317
        //==>
        //ct=-1;
        if(FrmAOI->bCheckAOIFailBinUse())
        {
            if(CUSTOMER_CODE==CC_KYEC_LEE)                                        //Eastsun 20260710 京元後面又改成 AOI Fail BIN 是後面20BIN
            {
                int ct = OutArmSuck.iBinData[iRow][iCol]+20;                      //Eastsun 20260710整合
                if(ct >= 0 && ct < iTestBinCount)
                {
                    if(Prod.iT6CatData[ct] >= 0)
                    {
                        OutArmSuck.iWhichAuto[iRow][iCol] = Prod.iT6CatData[ct];
                    }
                }
            }
            else
            {
                vector<int> veSearchBin;
                for(int i = 0 ; i < iTestBinCount;i++)                                  //Eastsun 20260317 : 搜尋原本Bin編號
                  {
                    if(Prod.iT6CatData[i]==OutArmSuck.iWhichAuto[iRow][iCol])
                      {
                        //ct=i;
                        veSearchBin.push_back(i);
                        //break;
                      }
                  }

                for(int i = 0; i < (int)veSearchBin.size();i++)                      //W8012 signed/unsigned
                {
                    ct=veSearchBin[i];
                    if(ct>=0 && ct<fBinAOISel->MaxBinSearch)                           //Eastsun 20260317 : AOI分Bin功能
                    {
                        if(Prod.AOICatData[ct]>=0)
                        {
                            OutArmSuck.iWhichAuto[iRow][iCol]=Prod.AOICatData[ct];
                        }
                    }
                }
                veSearchBin.clear();
            }
        }

        if(IniConfig.bA74AOIFailCountLinkLotRunMode)                            //Eastsun 20260515
        {
            int iLotRunModeIndex=-1;
            for(int i=0; i<(int)FrmAOI->AOIFailList.size();i++)
            {
                if(fLotInfo->cbRunMode->Text==FrmAOI->AOIFailList[i].asRunMode)
                {
                    iLotRunModeIndex=i;
                    break;
                }
            }

            if(iLotRunModeIndex>=0 && iLotRunModeIndex<iAOIFailCheckMax)            //Eastsun 20260402
            {
                if(FrmAOI->ttbInsp->bConsecutiveFailCheck_List[iLotRunModeIndex])
                {
                    FrmAOI->iConFailCount++;

                    if(FrmAOI->iConFailCount>=FrmAOI->ttbInsp->iConsecutiveFailCount_List[iLotRunModeIndex])
                    {
                        FrmAOI->iConFailCount=0;
                        bAOIConsecutiveFailFlag=true;
                    }
                }
                else
                {
                    FrmAOI->iConFailCount=0;
                }

                if(FrmAOI->ttbInsp->bAccumulatedFailCheck_List[iLotRunModeIndex])
                {
                    FrmAOI->iAccumulatedCount++;

                    if(FrmAOI->iAccumulatedCount>=FrmAOI->ttbInsp->iAccumulatedFailCount_List[iLotRunModeIndex])
                    {
                        FrmAOI->iAccumulatedCount=0;
                        bAOIFailCountFlag=true;
                    }
                }
                else
                {
                    FrmAOI->iAccumulatedCount=0;
                }

                if(FrmAOI->ttbInsp->bConsecutiveFailPictureCheck_List[iLotRunModeIndex])
                {
                    S=asFailCode.SubString(2, asFailCode.Length()-2);

                    iPos1=S.Pos("_");
                    asStr[0]=S.SubString(1, iPos1-1);
                    S=S.Delete(1, iPos1);

                    iPos1=S.Pos("_");
                    asStr[1]=S.SubString(1, iPos1-1);
                    S=S.Delete(1, iPos1);

                    iPos1=S.Pos("_");
                    asStr[2]=S.SubString(1, iPos1-1);
                    S=S.Delete(1, iPos1);

                    asStr[3]=S;

                    for(int i=0; i<4; i++)
                    {
                        if(asStr[i].Length()>4)
                        {
                            FrmAOI->iConFailCountPicture[i]++;
                        }

                        if(FrmAOI->iConFailCountPicture[i]>=FrmAOI->ttbInsp->iConsecutiveFailPictureCount_List[iLotRunModeIndex])
                        {
                            FrmAOI->iConFailCountPicture[i]=0;
                            bAOIConsecutiveFailPictureFlag=true;
                        }
                    }
                }
                else
                {
                    FrmAOI->iConFailCountPicture[0]=0;
                    FrmAOI->iConFailCountPicture[1]=0;
                    FrmAOI->iConFailCountPicture[2]=0;
                    FrmAOI->iConFailCountPicture[3]=0;
                }
            }
        }
        else
        {
        if(FrmAOI->ttbInsp->bConsecutiveFailCheck)
        {
            iConFailCount++;

            if(iConFailCount>=FrmAOI->ttbInsp->iConsecutiveFailCount)
            {
                iConFailCount=0;
                bAOIConsecutiveFailFlag=true;
            }
        }
        else
        {
            iConFailCount=0;
        }

        if(FrmAOI->ttbInsp->bAccumulatedFailCheck)
        {
            iAccumulatedCount++;

            if(iAccumulatedCount>=FrmAOI->ttbInsp->iAccumulatedFailCount)
            {
                iAccumulatedCount=0;
                bAOIFailCountFlag=true;
            }
        }
        else
        {
            iAccumulatedCount=0;
        }

        if(FrmAOI->ttbInsp->bConsecutiveFailPictureCheck)
        {
            S=asFailCode.SubString(2, asFailCode.Length()-2);

            iPos1=S.Pos("_");
            asStr[0]=S.SubString(1, iPos1-1);
            S=S.Delete(1, iPos1);

            iPos1=S.Pos("_");
            asStr[1]=S.SubString(1, iPos1-1);
            S=S.Delete(1, iPos1);

            iPos1=S.Pos("_");
            asStr[2]=S.SubString(1, iPos1-1);
            S=S.Delete(1, iPos1);

            asStr[3]=S;

            for(int i=0; i<4; i++)
            {
                if(asStr[i].Length()>4)
                {
                    iConFailCountPicture[i]++;
                }

                if(iConFailCountPicture[i]>=FrmAOI->ttbInsp->iConsecutiveFailPictureCount)
                {
                    iConFailCountPicture[i]=0;
                    bAOIConsecutiveFailPictureFlag=true;
                }
            }
        }
        else
        {
            iConFailCountPicture[0]=0;
            iConFailCountPicture[1]=0;
            iConFailCountPicture[2]=0;
            iConFailCountPicture[3]=0;
        }
    }
    }

    return true;
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoInquireResult(bool bIsFirst, int iStep, int iRetry)
{
//    static int iConFailCount=0;
//    static int iConFailCountPicture[4]={0, 0, 0, 0};
//    static int iAccumulatedCount=0;

    AnsiString Str, asFailCode, asFailPos;
//    int iRow, iCol, iBin, ct, iOldBin;

    AnsiString asStr[4], S;
//    int iLen, iPos1;

    if(bIsFirst)
    {
        DoSendCommand(true,"");
        if(t_ti.iPosName==(int)ePC_Top)
        {//~TOPINSP_ISFINISH,CamName@
            asSendMsg="~TOPINSP_ISFINISH,"+t_ti.CamName+"@";
        }
        else
        {//~BOTINSP_ISFINISH,CamName@
            asSendMsg="~BOTINSP_ISFINISH,"+t_ti.CamName+"@";
        }
        return true;
    }
    else if(iStep==10)
    {
        DoSendCommand(false,asSendMsg);
        return true;
    }
    else
    {
        if(bSimuAOICommand)
        {
            if(t_ti.iPosName==(int)ePC_Top)
            {//~TOPINSP_ISFINISH_ACK,CamName,InspStatus,Timestamp,Pass/Fail(fail code),Fail area@
                asRecMsg="~TOPINSP_FINISH_ACK,"+t_ti.CamName+",FIN,20200601100857,PASS,0_0_0@";
            }
            else
            {//BOTINSP_ISFINISH_ACK,CamName,InspStatusTimestamp,Pass/Fail(fail code),Fail area@
                asRecMsg="~BOTINSP_FINISH_ACK,"+t_ti.CamName+",FIN,20200601100857,PASS,0_0_0@";
            }
            RecordMsg("[Receive]"+asRecMsg);
        }
        else
        {
            asRecMsg=DoSendCommand(false,asSendMsg);
        }

        if(asRecMsg!="")
        {
            tsAnalysis->CommaText=asRecMsg;
            if(tsAnalysis->Count>=3)//Wait//~TOPINSP_FINISH_ACK,CM1,WAIT@
            {
//                if(tsAnalysis->Strings[2]=="FIN")
                if(tsAnalysis->Strings[2].Pos("FIN")==1)
                {
                    return true;
                }
                else if(tsAnalysis->Strings[2].Pos("WAIT")==1)
                {
//                    DoInquireResult(true);
                    return false;
                }
                else if(tsAnalysis->Strings[2].Pos("NG")==1)
                {
                    return false;
//                    RecordErrorMsg(GetErrorMsg(__FUNC__,GetAOIReturnError(),asRecMsg));
                }
                else
                {
                    if(iRetry>3)
                        RecordErrorMsg(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asRecMsg));
                    return false;
                }
            }
            else
            {
                if(iRetry>3)
                    RecordErrorMsg(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asRecMsg));
                return false;
            }
//            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoBinOutInfo(bool bIsFirst)
{
    if(bIsFirst)
    {
        DoSendCommand(true,"");
        //~BINOUT,Timestamp,TrayID,Bin_out_location@
        asSendMsg=GetBinOutInfoCommand(t_bi);
        return true;
    }
    else
    {
        if(bSimuAOICommand)
        {
            asRecMsg="~BINOUT_ACK@";
            RecordMsg("[Receive]"+asRecMsg);
        }
        else
        {
            asRecMsg=DoSendCommand(false,asSendMsg);
        }

        if(asRecMsg!="")
        {
            if(asRecMsg!="~BINOUT_ACK@")
            {
                RecordErrorMsg(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asRecMsg));
            }
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoStartLot(bool bIsFirst,AnsiString sLotID)
{
    if(bIsFirst)
    {
        DoSendCommand(true,"");
        asSendMsg="~LOTST,"+sLotID+"@";
        return true;
    }
    else
    {
        if(bSimuAOICommand)
        {
            asRecMsg="~LOTST_ACK@";
            RecordMsg("[Receive]"+asRecMsg);
        }
        else
        {
            asRecMsg=DoSendCommand(false,asSendMsg);
        }

        if(asRecMsg!="")
        {
            if(asRecMsg!="~LOTST_ACK@")
            {
                RecordErrorMsg(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asRecMsg));
            }
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoEndLot(bool bIsFirst)
{
    if(bIsFirst)
    {
        DoSendCommand(true,"");
        asSendMsg="~LOTED@";
        return true;
    }
    else
    {
        if(bSimuAOICommand)
        {
            asRecMsg="~LOTST_ACK@";
            RecordMsg("[Receive]"+asRecMsg);
        }
        else
        {
            asRecMsg=DoSendCommand(false,asSendMsg);
        }

        if(asRecMsg!="")
        {
            if(asRecMsg!="~LOTST_ACK@")
            {
                RecordErrorMsg(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asRecMsg));
            }
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoTestLighting(bool bIsFirst,int iRetryCount)
{
    if(bIsFirst)
    {
        //~TESTLIGHT,CamName@
        if(t_ti.iPosName==(int)ePC_Top)
        {
            sTemp="S1";
        }
        else
        {
            if(FrmAOI->ttbInsp->iAction==eBottom)
                sTemp="S1";
            else
            sTemp="S2";
        }
        DoSendCommand(true,"");
        asSendMsg="~TESTLIGHT,"+t_ti.CamName+","+sTemp+"@";
        return true;
    }
    else
    {
        if(bSimuAOICommand)
        {
            asRecMsg="~TESTLIGHT_ACK,CM1,OK@";
            RecordMsg("[Receive]"+asRecMsg);
        }
        else
        {
            asRecMsg=DoSendCommand(false,asSendMsg);
        }

        if(asRecMsg!="")
        {
            tsAnalysis->CommaText=asRecMsg;
            //if(tsAnalysis->Count==3)
            //==>
            if(tsAnalysis->Count>=3)                                            //Eastsun20260305 : Someimes the error code gigger than three length
            {
                //result : OK光源正常，NG光源錯誤。( 產品表面亮度有問題 )。
//                if(tsAnalysis->Strings[2]=="OK@")//~TESTLIGHT_ACK,CM1,OK@
//                {
//                }
//                else if(tsAnalysis->Strings[2]=="NG@")//~TESTLIGHT_ACK,CM1,NG@
//                {
//                    if(iRetryCount>3)
//                        RecordErrorMsg(GetErrorMsg(__FUNC__,GetAOIReturnError(),asRecMsg));
//                    return false;
//                }
//                else
//                {
//                    if(iRetryCount>3)
//                        RecordErrorMsg(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asRecMsg));
//                    return false;
//                }
                //==>
                if(tsAnalysis->Strings[2].Pos("OK"))//~TESTLIGHT_ACK,CM1,OK@       //Eastsun20260305 :
                {
                }
                else if(tsAnalysis->Strings[2].Pos("NG"))//~TESTLIGHT_ACK,CM1,NG@
                {
                    //if(iRetryCount>3)
                    RecordErrorMsg(GetErrorMsg(__FUNC__,GetAOIReturnError(),asRecMsg));
                    ShowMyMessage(AnsiString().sprintf("func:%s ,Msg:%s",__FUNC__,asErrorMsg));
                    asRecMsg="";
                    return false;
                }
                else
                {
                    //if(iRetryCount>3)
                        RecordErrorMsg(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asRecMsg));
                    return false;
                }
            }
            else
            {
                if(iRetryCount>3)
                    RecordErrorMsg(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asRecMsg));
                return false;
            }
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoCamaChange(bool bIsFirst)
{
    static bool bOK=false;
    if(bIsFirst)
    {
        //~STATIONCHG,CamName,station name@
        //~STATIONCHG,CM1,S1@
        DoSendCommand(true,"");

        if(t_ti.iPosName==(int)ePC_Top)
        {
            sTemp="S1";
        }
        else
        {
            if(FrmAOI->ttbInsp->iAction==eBottom)
                sTemp="S1";
            else
            sTemp="S2";
        }
        asSendMsg="~STATIONCHG,"+t_ti.CamName+","+sTemp+"@";
        bOK=false;
        return true;
    }
    else
    {
        if(bOK)
        {
            return true;
        }

        if(bSimuAOICommand)
        {
            asRecMsg="~STATIONCHG_ACK,OK@";
            RecordMsg("[Receive]"+asRecMsg);
        }
        else
        {
            asRecMsg=DoSendCommand(false,asSendMsg);
        }

        if(asRecMsg!="")
        {
            tsAnalysis->CommaText=asRecMsg;
            if(tsAnalysis->Count==3)
            {
                if(tsAnalysis->Strings[1]=="NG")//~STATIONCHG_ACK,NG,ErrorMessage@
                {
                    RecordErrorMsg(GetErrorMsg(__FUNC__,GetAOIReturnError(),asRecMsg));
                }
                else
                {
                    RecordErrorMsg(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asRecMsg));
                }
            }
            else if(tsAnalysis->Count==2)
            {
                if(tsAnalysis->Strings[1]=="OK@")//~STATIONCHG_ACK,OK@
                {
                    bOK=true;
                }
                else
                {
                    RecordErrorMsg(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asRecMsg));
                }
            }
            else
            {
                RecordErrorMsg(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asRecMsg));
            }
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoAutoRun(bool bIsFirst)
{
    if(bIsFirst)
    {
        //~AUTORUN@
        DoSendCommand(true,"");
        asSendMsg="~AUTORUN@";
        return true;
    }
    else
    {
        if(bSimuAOICommand)
        {
            asRecMsg="~AUTORUN_ACK,OK@";
            RecordMsg("[Receive]"+asRecMsg);
        }
        else
        {
            asRecMsg=DoSendCommand(false,asSendMsg);
        }

        if(asRecMsg!="")
        {
            tsAnalysis->CommaText=asRecMsg;
            if(tsAnalysis->Count==2)
            {
                if(tsAnalysis->Strings[1]=="OK@")//~AUTORUN_ACK,OK@
                {
                }
                else
                {
                    RecordErrorMsg(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asRecMsg));
                }
            }
            else
            {
                RecordErrorMsg(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asRecMsg));
            }
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoPause(bool bIsFirst)
{
    if(bIsFirst)
    {
        //~AUTORUN@
        DoSendCommand(true,"");
        asSendMsg="~PAUSE@";
        return true;
    }
    else
    {
        if(bSimuAOICommand)
        {
            asRecMsg="~PAUSE_ACK,OK@";
            RecordMsg("[Receive]"+asRecMsg);
        }
        else
        {
            asRecMsg=DoSendCommand(false,asSendMsg);
        }

        if(asRecMsg!="")
        {
            tsAnalysis->CommaText=asRecMsg;
            if(tsAnalysis->Count==2)
            {
                if(tsAnalysis->Strings[1]=="OK@")//~PAUSE_ACK,OK@
                {
                }
                else
                {
                    RecordErrorMsg(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asRecMsg));
                }
            }
            else
            {
                RecordErrorMsg(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asRecMsg));
            }
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoRecipeChange(bool bIsFirst)
{
    if(bIsFirst)
    {
        //~RECIPE_CHANGE,RecipeName@
        DoSendCommand(true,"");
        asSendMsg="~RECIPE_CHANGE,"+TfAOI().GetLoadFileName()+"@";
        return true;
    }
    else
    {
        if(bSimuAOICommand)
        {
            asRecMsg="~RECIPE_CHANGE_ACK,OK@";
            RecordMsg("[Receive]"+asRecMsg);
        }
        else
        {
            asRecMsg=DoSendCommand(false,asSendMsg);
        }

        if(asRecMsg!="")
        {
            tsAnalysis->CommaText=asRecMsg;
            if(tsAnalysis->Count==3)
            {
                if(tsAnalysis->Strings[1]=="NG")//~RECIPE_CHANGE_ACK,NG,ErrorMessage@
                {
                    RecordErrorMsg(GetErrorMsg(__FUNC__,GetAOIReturnError(),asRecMsg));
                }
                else
                {
                    RecordErrorMsg(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asRecMsg));
                }
            }
            else if(tsAnalysis->Count==2)
            {
                if(tsAnalysis->Strings[1]=="OK@")//~RECIPE_CHANGE_ACK,OK@
                {
                }
                else
                {
                    RecordErrorMsg(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asRecMsg));
                }
            }
            else
            {
                RecordErrorMsg(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asRecMsg));
            }
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoTimeSynchronization(bool bIsFirst)
{
    if(bIsFirst)
    {
        //~ALIGNTIME,20240409145535156@
        DoSendCommand(true,"");
        asSendMsg="~ALIGNTIME,"+GetTimestamp()+"000@";
        return true;
    }
    else
    {
        if(bSimuAOICommand)
        {
            asRecMsg="~ALIGNTIME_ACK,OK@";
            RecordMsg("[Receive]"+asRecMsg);
        }
        else
        {
            asRecMsg=DoSendCommand(false,asSendMsg);
        }

        if(asRecMsg!="")
        {
            tsAnalysis->CommaText=asRecMsg;
            if(tsAnalysis->Count==3)
            {
                if(tsAnalysis->Strings[1]=="NG")//~ALIGNTIME_ACK,NG,ErrorMessage@
                {
                    RecordErrorMsg(GetErrorMsg(__FUNC__,GetAOIReturnError(),asRecMsg));
                }
                else
                {
                    RecordErrorMsg(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asRecMsg));
                }
            }
            else if(tsAnalysis->Count==2)
            {
                if(tsAnalysis->Strings[1]=="OK@")//~ALIGNTIME_ACK,OK@
                {
                }
                else
                {
                    RecordErrorMsg(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asRecMsg));
                }
            }
            else
            {
                RecordErrorMsg(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asRecMsg));
            }
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoResetFlow(bool bIsFirst,int iRetry)
{
    if(bIsFirst)
    {
        //~INSP_INTERRUPT@
        DoSendCommand(true,"");
        asSendMsg="~INSP_INTERRUPT@";
        return true;
    }
    else
    {
        if(bSimuAOICommand)
        {
            asRecMsg="~INSP_INTERRUPT_ACK,OK@";
            RecordMsg("[Receive]"+asRecMsg);
        }
        else
        {
            asRecMsg=DoSendCommand(false,asSendMsg);
        }

        if(asRecMsg!="")
        {
            tsAnalysis->CommaText=asRecMsg;
            if(tsAnalysis->Count==3)
            {
                if(tsAnalysis->Strings[1]=="NG")//~INSP_INTERRUPT_ACK,NG,ErrorMessage@
                {
                    if(iRetry>3)
                        RecordErrorMsg(GetErrorMsg(__FUNC__,GetAOIReturnError(),asRecMsg));
                    return false;
                }
                else
                {
                    if(iRetry>3)
                        RecordErrorMsg(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asRecMsg));
                    return false;
                }
            }
            else if(tsAnalysis->Count==2)
            {
                if(tsAnalysis->Strings[1]=="OK@")//~INSP_INTERRUPT_ACK,OK@
                {
                }
                else
                {
                    if(iRetry>3)
                        RecordErrorMsg(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asRecMsg));
                    return false;
                }
            }
            else
            {
                if(iRetry>3)
                    RecordErrorMsg(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asRecMsg));
                return false;
            }
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
AnsiString TTopBottomInspect::GetTimestamp()
{
    return uTimeTool().TDatetmeToSavestring(Now());
}
//---------------------------------------------------------------------------
AnsiString TTopBottomInspect::GetInspectCommand(t_TopBtnInspect t)
{
    if(t.iPosName==(int)ePC_Top)
    {//~TOPINSP,CamName,Timestamp,LotID,ArmSite,BinResult,TouchDown,DeviceInfo,GrabIndex@
        sTemp="TOPINSP";
    }
    else
    {//~BOTINSP,CamName,Timestamp,LotID,ArmSite,BinResult,TouchDown,DeviceInfo,GrabIndex@
        sTemp="BOTINSP";
    }
    t.Timestamp=GetTimestamp();
    AnsiString s="~"+sTemp;
    s+=","+t.CamName;
    s+=","+t.Timestamp;
    s+=","+t.LotID;
    s+=","+t.ArmSite;
    s+=","+t.BinResult;
    s+=","+t.TouchDown;
    s+=","+t.DeviceInfo;
    s+=","+GetGrabIndex(t.iGrabIndex);
    s+="@";
    return s;
}
//---------------------------------------------------------------------------
AnsiString TTopBottomInspect::GetBinOutInfoCommand(t_BinOutInfo t)
{//~BINOUT,Timestamp,TrayID,Bin_out_location@
    AnsiString s="~BINOUT";
    s+=","+t.Timestamp;
    s+=","+t.TrayID;
    s+=","+t.Bin_out_location;
    s+="@";
    return s;
}
//---------------------------------------------------------------------------
void TTopBottomInspect::RecordMsg(AnsiString str)
{
    fTrayMapping->AddCCDTrayCommunicationLog(91, str);
    if(RecMsg)
        RecMsg(str);
}
//---------------------------------------------------------------------------
void TTopBottomInspect::RecordErrorMsg(AnsiString str)
{
    asErrorMsg=str;
    str="[Error]"+str;
    RecordMsg(str);
}
//---------------------------------------------------------------------------
AnsiString TTopBottomInspect::GetErrorMsg(AnsiString asFunName,AnsiString asErrorType,AnsiString asContent)
{
    return AnsiString().sprintf("[Error][%s][%s]%s",asFunName,asErrorType,asContent);
}
//---------------------------------------------------------------------------
int TTopBottomInspect::GetIC_Size()
{
    return iIC_Size;
}
//---------------------------------------------------------------------------
void TTopBottomInspect::SetIC_Size(int is)//0:<=65mm 1:>65mm
{
    if(is<(int)eICS_Total)
        iIC_Size=is;
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoTakePhoto(bool bIsFirst)
{
    bool bResult=false;
    if(bIsFirst)
    {
        asErrorMsg="";
        iTakePhotoTask=1;
        bResult=true;
    }
    else
    {
        int &Task=iTakePhotoTask;
        switch(Task)
        {
            case 1://Initial
                Task=100;
                break;
            case 100:
                DoStartInspect(true);
                Task=101;
            case 101:
                if(DoStartInspect(false)==true)
                {
                    Task=200;
                }
                break;
            case 200:
                DoImagingCompleted(true);
                Task=201;
//                break;
            case 201:
                if(DoImagingCompleted(false)==true)
                {
                    Task=9999;
                }
                break;
            case 9999:// finish
                bResult=true;
                break;
            default:
                RecordErrorMsg(GetErrorMsg(__FUNC__,"Wrong Task",IntToStr(Task)));
                Task=9999;
                break;
        }
    }
    return bResult;
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoTopBtmInspFunc(bool bIsFirst)
{
    static int iDoTopBtmInspFuncRetry=0;
    bool bResult=false;
    if(bIsFirst)
    {
        asErrorMsg="";
        iTopBtmInspFuncTask=1;
        iDoTopBtmInspFuncRetry=0;
        bResult=true;
    }
    else
    {
        int &Task=iTopBtmInspFuncTask;
        int iSuckRow,iSuckCol;
        bool bError=false;
        int icsize;
        int ret=0;
        if(asErrorMsg!="")
        {
            if(iDoTopBtmInspFuncRetry<3)                                        //Eastsun 20260304 : 新增Retry功能
            {
                iDoTopBtmInspFuncRetry++;
                asErrorMsg="";
                Task=1;
            }
            else
            {
                ShowMyMessage(AnsiString().sprintf("func:%s ,Msg:%s",__FUNC__,asErrorMsg));
                asErrorMsg="";
                Task=3500;
            }
        }
        #ifdef SOFT_SIMULTE
        bSimuAOICommand=true;
        bSimuMot=true;
        #endif
        switch(Task)
        {
            case 1://Initial
                CommClose();
                if(DeviceForm_File.XDimension>=65 || DeviceForm_File.YDimension>=65)
                    icsize=1;
                else
                    icsize=0;
                FrmAOI->ttbInsp->SetIC_Size(icsize);
                DoMoveFixedSeatAndCCD2Ready(true,bError);
                SetMotorScaleSpeed(MTopAOIArmX,30);
                SetMotorScaleSpeed(MTopAOIArmY,30);
                SetMotorScaleSpeed(MTopAOICCDZ,50);
                SetMotorScaleSpeed(MTopAOIArmR,50);
                Task=100;
                break;
            case 100:
                if(DoMoveXY2SafePos() && DoMoveFixedSeatAndCCD2Ready(false,bError))
                {
                    if(bError==true)
                    {
                        ShowMyMessage(AnsiString().sprintf("%s DoMoveFixedSeatAndCCD2Ready error!",__FUNC__));
                        Task=9900;
                    }
                    else
                    {
                        tTimeOut.SetSecAndOn(GetTimeOutSec());
                        Task=200;
                    }
                }
                break;
            case 200:
                if(bSimuAOICommand)
                {
                    Task=2000;
                    break;
                }

                if(bSimuAOICommand==false)
                {
                    if(DoCommuncation()==false)
                    {
                        if(tTimeOut.Off())
                        {
                            ShowMyMessage(AnsiString().sprintf("%s AOI connect timeout error!",__FUNC__));
                            tTimeOut.SetSecAndOn(GetTimeOutSec());
                        }
                        break;
                    }
                }

                if(SetSuckActive((int)eAOI_SCANAOI)==true)
                {
                    Task=500;
                }
                else
                {
                    Task=9900;
                }
                break;
            case 500:

                if(GetNeedActSuck(iSuckRow,iSuckCol))
                {
                    bool bPlace=true;
                    if(FrmAOI->ttbInsp->iAction==eBottom)
                    {
                        if(DoMoveFixedSeatXYandClamp((int)eICS_LessThanOrEqual65mm,10))
                        {
                            ret=ess_Finish;
                        }
                    }
                    else
                    {
                        ret=DoOutarm2AOI_PickPlaceIC(iSuckRow,iSuckCol,bPlace);
                    }

                    if(ret==ess_Running)
                    {
                    }
                    else if(ret==ess_Finish)
                    {
                        Task=1900;
                    }
                    else if(ret==ess_Skip)
                    {
                        Task=3500;
                    }
                    else  //retry
                    {
                        Task=500;
                    }
                }
                else
                {
                    Task=3500;//finish
                }
                break;
            case 1900:// Out Arm Z Down to AOI Height before Inspection
                if(DoMoveZ_TopView(Prod.iScannerAOI_Z))
                {
                    Task=2000;
                }
                break;
            case 2000:
                DoTopBtmInspFunc_Inspection(true);
                Task=2100;
//                break;
            case 2100:
                if(FrmAOI->ttbInsp->iAction==eBottom)
                {
                    if(DoTopBtmInspFunc_Inspection(false)==true)
                    {
                        GetNeedActSuck(iSuckRow,iSuckCol);
                        if(!bSimuAOICommand)
                        {
                            OutArmSuck.iAOIStation[iSuckRow][iSuckCol]=eAOI_SCANAOI;
                        }
                        Task=3500;
                    }
                }
                else if(DoMoveXY2SafePos()==true &&
                   DoTopBtmInspFunc_Inspection(false)==true)
                {
                    Task=3000;
                }
                break;
            case 3000:
                if(DoMoveFixedSeatXY2RotatePos() &&
                   GetNeedActSuck(iSuckRow,iSuckCol))
                {
                    bool bPlace=false;
                    ret=DoOutarm2AOI_PickPlaceIC(iSuckRow,iSuckCol,bPlace);

                    if(ret==ess_Running)
                    {
                    }
                    else if(ret==ess_Finish)
                    {
                        Task=3500;
                    }
                    else if(ret==ess_Skip)
                    {
                        Task=3500;
                    }
                    else  //retry
                    {
                        Task=3000;
                    }
                }
                else
                    Task=3500;//finish
                break;
            case 3500://all suck up
                if(MoveOutArmToAutoSafe())
                {
                    Task=200;//find other suck status
                    ZeroMemory(bActiveSuck ,sizeof(bActiveSuck));
                }
                break;
            case 9900://Finish
                if(MoveOutArmToAutoSafe())
                {
                    Task=9999;
                }
                break;
            case 9999:
                bResult=true;
                break;
            default:
                Task=1;//find other suck status
                break;
        }
    }
    return bResult;
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoTopBtmInspFunc_Inspection(bool bIsFirst)
{
    bool bResult=false;
    static int iRetryCount=0;
    static int iRetryCount1=0;
    static int iRetryCount2=0;
    static int iAOIPhotosTimeOutRetry=0;
    static int iAOIDisconnectErrorRetry=0;
    static int iDoResetFlowRetry=0;
    static int iDoTopBtmInspFunc_InspectionRetry=0;

    if(bIsFirst)
    {
        asErrorMsg="";
        iTopBtmInspFuncInspTask=1;
        iRetryCount1=0;
        iRetryCount2=0;
        iAOIPhotosTimeOutRetry=0;
        iAOIDisconnectErrorRetry=0;
        iDoTopBtmInspFunc_InspectionRetry=0;
        bResult=true;
    }
    else
    {
        int &Task=iTopBtmInspFuncInspTask;
        if(asErrorMsg!="")                                                      //Eastsun 20260223 : 備註:這邊沒用 break後外層還有 會先show外層
        {
            if(iDoTopBtmInspFunc_InspectionRetry<3)                             //Eastsun 20260304 : 加入Retry功能
            {
                asErrorMsg="";
                iDoTopBtmInspFunc_InspectionRetry++;
                Task=1;
            }
            else
            {
                ShowMyMessage(AnsiString().sprintf("func:%s ,Msg:%s",__FUNC__,asErrorMsg));
                asErrorMsg="";
                return true;
            }
        }
        switch(Task)
        {
            case 1://Initial
//                if(bSimuAOICommand)
//                {
//                    Task=303;
//                    break;
//                }
                #ifdef SOFT_SIMULTE
                return true;
                #endif
                Task=50;
                //bottom info
                t_ti.CamName=sCamaName;
                t_ti.iPosName=(int)ePC_Bottom;
                DoAutoRun(true);
                break;
            case 50:
                if(DoAutoRun(false))
                {
                    DoCamaChange(true);//change cama
                    Task=100;
                }
                break;
            case 100:
                {
                    bool b1=DoCamaDown(false);             //Camara up
                    bool b2=DoClamp((int)eCN_Total,true);  //Clamp on
                    bool b3=DoCamaChange(false);  //change cama
                    if(b1 && b2 && b3)
                    {
                        Task=140;
                        iDoResetFlowRetry=0;
                    }
                }
                break;
            case 140: //Reset
                DoResetFlow(true,0);
                hGetResultDelayTime.SetSecAndOn(2);                             //Eastsun 20260223 : 設定異常時間
                Task=141;
//                break;
            case 141: //Reset
                if(DoResetFlow(false,iDoResetFlowRetry))
                {
                    Task=142;
                    hGetResultDelayTime.SetMSAndOn(200);
                }
                else if(hGetResultDelayTime.Off())
                {
                    iDoResetFlowRetry++;
                    Task=140;
                }
                break;
            case 142:
                if(hGetResultDelayTime.Off())                                   //Eastsun 20260223 : 訊號延遲一下 不要馬上再發
                {
                    Task=150;
                }
                break;
            case 150: //try light
                DoTestLighting(true,0);
                hGetResultDelayTime.SetSecAndOn(5);
                Task=160;
//                break;
            case 160: //try light
                if(DoTestLighting(false, iRetryCount1))
                {
                    iRetryCount1=0;
                    Task=200;
                }
                else if(hGetResultDelayTime.Off())
                {
                    iRetryCount1++;
                    Task=140;
                }
                break;
            case 200:
                if(GetIC_Size()==(int)eICS_LessThanOrEqual65mm)//<=65mm size
                {
                    DoPhotosCenterpos(true);
                }
                else//>65mm size
                {
                    DoPhotos4pos(true);
                }
                hGetResultDelayTime.SetSecAndOn(20);
                iRetryCount=0;
                Task=201;
            case 201:
                {
                    bool b5=false;
                    if(GetIC_Size()==(int)eICS_LessThanOrEqual65mm)//<=65mm size
                    {
                        b5=DoPhotosCenterpos(false);
                    }
                    else//>65mm size
                    {
                        b5=DoPhotos4pos(false);
                    }

                    if(b5==true)
                    {
                        Task=250;
                    }
                    else if(hGetResultDelayTime.Off())
                    {
                        if(iAOIPhotosTimeOutRetry<2)                            //Eastsun 20260302 : AOI Photos Time Out Error Retry
                        {
                            iAOIPhotosTimeOutRetry++;
                        }
                        else
                        {
                            asErrorMsg="AOI Photos Time Out Error";
                        }
                        Task=140;
                    }
                    break;
                }
            case 250:
                DoInquireResult(true);
                Task=301;
                hGetResultDelayTime.SetMSAndOn(FrmAOI->ttbInsp->iGetResultDelay);
                break;
            case 301:
                if(hGetResultDelayTime.Off())
                {
                    DoInquireResult(false, 10);
                    hGetResultDelayTime.SetMSAndOn(FrmAOI->ttbInsp->iGetResultDelay);
                    Task=302;
                }
                break;
            case 302:
                if(DoInquireResult(false,0,iAOIDisconnectErrorRetry))
                {
                    //Ifor 20260624 add : AOI Fail 可Retry/Skip
                    AnsiString asAOIFailRet="";
                    if(bSimuAOICommand==false && tsAnalysis->Count>=5)
                        asAOIFailRet=tsAnalysis->Strings[4];
                    if(asAOIFailRet!="" && asAOIFailRet!="PASS")
                    {
                        int iAOIFailKey=ShowErrorMessage("WAR0880", K_RETRY|K_SKIP, MMScanner, false, "AOI Fail:"+asAOIFailRet);
                        if(iAOIFailKey==K_RETRY)
                        {
                            iAOIPhotosTimeOutRetry=0;
                            iRetryCount=0;
                            Task=140;
                            break;
                        }
                    }
                    Task=303;
                }
                else if(hGetResultDelayTime.Off())
                {
                    if(iRetryCount<3)
                    {
                        iRetryCount++;
                        Task=250;
                    }
                    else
                    {
                        if(iAOIDisconnectErrorRetry<2)
                        {
                            Task=1;
                        }
                        else
                        {
                            //asErrorMsg="AOI Disconnect Error";
                            //ShowMyMessage(AnsiString().sprintf("func:%s ,Msg:%s",__FUNC__,asErrorMsg));
                            int ret=ShowErrorMessage("WAR09114", K_RETRY|K_SKIP, MMScanner, false, "");
                            //asErrorMsg="";
                            if(ret==K_SKIP)
                            {
                                return true;
                            }
                            else//retry
                            {
                                Task=1;
                            }
                        }
                        iAOIDisconnectErrorRetry++;
                    }
                }

                break;
            case 303:
                if(DoRecordResult())
                {
                    if(bSimuAOICommand)
                    {
                        Task=9999;
                        break;
                    }

                    if(FrmAOI->ttbInsp->iAction==eBottom)
                        Task=9999;
                    else
                        Task=350;
                }
                break;
            case 350:
                {
                    bool b1=DoCamaDown(true);             //Camara down
                    bool b2=DoClamp((int)eCN_Total,true);  //Clamp on
                    if(b1 && b2)
                    {
                        Task=400;
                    }
                }
                break;
            case 400:
                DoCamaChange(true);//change cama
                t_ti.iPosName=(int)ePC_Top;
                DoRotate(180, true);
                Task=410;
//                break;
            case 410:
                if(DoMoveFixedSeatXY2RotatePos()==true)
                {
                    bool b1=DoCamaChange(false); //change cama
                    bool b2=DoRotate(180);  //Rotate to 180°
                    if(b1 && b2)
                    {
                        Task=500;
                    }
                }
                break;
            case 500:
                {
                    bool b1=DoCamaDown(false);             //Camara up
                    bool b2=DoClamp((int)eCN_Total,true);  //Clamp on
                    if(b1 && b2)
                    {
                        Task=600;
                    }
                }
                break;
            case 600:
                if(iIC_Size==(int)eICS_LessThanOrEqual65mm)//<=65mm size
                {
                    DoPhotosCenterpos(true);
                }
                else//>65mm size
                {
                    DoPhotos4pos(true);
                }
                Task=601;
//                break;
            case 601:
                {
                    bool b1=false;
                    if(iIC_Size==(int)eICS_LessThanOrEqual65mm)//<=65mm size
                    {
                        b1=DoPhotosCenterpos(false);
                    }
                    else//>65mm size
                    {
                        b1=DoPhotos4pos(false);
                    }

                    if(b1==true)
                    {
                        Task=650;
                    }
                }
                break;
            case 650:
                DoInquireResult(true);
                Task=651;
//                break;
            case 651:
                if(DoInquireResult(false))
                {
                    Task=700;
                }
                break;
            case 700:
                {
                    bool b1=DoClamp((int)eCN_Total,true);   //Clamp on
                    bool b2=DoMoveFixedSeatXY2RotatePos(); //To Center
                    bool b3=DoCamaDown(true);               //Camara down
                    if(b1 && b2 && b3)
                    {
                        DoRotate(0, true);
                        Task=800;
                    }
                }
                break;
            case 800:
                if(DoRotate(0)==true)//Rotate to 0°
                {
                    Task=900;
                }
                break;
            case 900:
                {
                    bool b1=DoCamaDown(false);             //Camara up
                    bool b2=DoClamp((int)eCN_Total,true); //Clamp on   //wait outarm suck
                    if(b1 && b2)
                    {
                        Task=9999;
                    }
                }
                break;
            case 9999:// finish
                bResult=true;
                break;
            default:
                RecordErrorMsg(GetErrorMsg(__FUNC__,"Wrong Task",IntToStr(Task)));
                Task=9999;
                break;
        }
    }
    return bResult;
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoClamp(int iPos,bool bOn)
{
    if(bSimuMot==true)
        return true;
    bool b=false;

    if(FrmAOI->ttbInsp->iAction==eBottom)
        return true;

    if(iPos==eCN_TopLeft)
    {
//        SW[SwFixedSeatTLOn].OnOff(bOn);
//        SW[SwFixedSeatTLOff].OnOff(!bOn);
//        if(bOn)
//        {
//            return Sen[SnFixedSeatTLOn].Status();
//        }
//        else
//        {
//            return Sen[SnFixedSeatTLOff].Status();
//        }
        SW[SwFixedSeatTLOff].OnOff(!bOn);
        return DoCylinder(C_FixedSeatTL,bOn);
    }
    else if(iPos==eCN_TopRight)
    {
        SW[SwFixedSeatTROff].OnOff(!bOn);
        return DoCylinder(C_FixedSeatTR,bOn);
    }
    else if(iPos==eCN_BottomLeft)
    {
        SW[SwFixedSeatBLOff].OnOff(!bOn);
        return DoCylinder(C_FixedSeatBL,bOn);
    }
    else if(iPos==eCN_BottomRight)
    {
        SW[SwFixedSeatBROff].OnOff(!bOn);
        return DoCylinder(C_FixedSeatBR,bOn);
    }
    else//eCN_Total
    {
        bool b1=DoClamp(eCN_TopLeft,bOn);
        bool b2=DoClamp(eCN_TopRight,bOn);
        bool b3=DoClamp(eCN_BottomLeft,bOn);
        bool b4=DoClamp(eCN_BottomRight,bOn);
        b=b1 & b2 & b3 & b4;
    }
    return b;
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoCylinder(int iCylin,bool bOn)
{
    if(bOn)
    {
        if(Cylinder[iCylin].OnStatus()==true)
        {
            return true;
        }
        else
            return Cylinder[iCylin].Push();
    }
    else
    {
        if(Cylinder[iCylin].OffStatus()==true)
        {
            return true;
        }
        else
            return Cylinder[iCylin].Pop();
    }
}
//---------------------------------------------------------------------------
TQPF_Timer hActionTimer;
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoRotate(int iDeg, bool bReset)
{
    if(bSimuMot==true)
        return true;
    int iPos=(iDeg==0)?iRotate0Pos:iRotate180Pos;
    static int iDegOld=-999;
    static int Task=0;

    if(bReset)
    {
        Task=1;
        return false;
    }

    switch(Task)
    {
        case 1:
            hActionTimer.SetSecAndOn(5);
            Task=100;
            break;
        case 100:
            if(DoLockRotate(false)) //Rotate Unlock
                Task=200;
            else if(hActionTimer.Off())
                ShowMyMessage("Please check AOI rotate,Action time out");
            break;
        case 200:
            if(MOT[MTopAOIArmR].MotorMove(iPos))
                Task=300;
            break;
        case 300:
            if(iDeg==0 && GetRotateIs0()==false)
            {
                ShowMyMessage("Please manually rotate the platform to the origin.");
                return false;
            }
            else if(iDeg==180 && GetRotateIs180()==false)
            {
                ShowMyMessage("Please manually rotate the platform to the 180.");
                return false;
            }

            if(DoLockRotate(true)) //Rotate Lock
            {
                if(iDeg!=iDegOld)
                {
                    iDegOld=iDeg;
                    RecordMsg(AnsiString().sprintf("Rotate Degree:%d MotPos:%d",iDeg,iPos));
                }
                return true;
            }
            break;
    }

    return false;
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoCamaDown(bool bDowm)
{
    if(bSimuMot==true)
        return true;
//    if(DoCCDLightDown(bDowm)==false)
//        return false;
    int iPos=0,iTarPos;
    bool bBack=false;
    if(t_ti.iPosName==(int)ePC_Top)
    {
        iTarPos=iCCD_Up_Z_Top;
    }
    else
    {
        iTarPos=iCCD_Up_Z_Btm;
    }
    iPos=(bDowm)?0:iTarPos;
    bBack=MOT[MTopAOICCDZ].MotorMove(iPos);
    return bBack;
}
//---------------------------------------------------------------------------
TQPF_Timer hLightActionTimer;
//---------------------------------------------------------------------------
int iCCDLightDownTask=1;
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoCCDLightDown(bool bDowm, bool bReset)
{
//    static bool bflag=false;
    int SwPRG[7]={SwPRGSEL0, SwPRGSEL1, SwPRGSEL2, SwPRGSEL3, SwPRGSEL4, SwPRGSEL5, SwPRGSEL6};

    if(bSimuMot==true || USE_Scanner_AOI_Inspection_FixLight_Z_Axis==1) //Eastsun 20260410 : AOI Fix Light Z Axis bypass
        return true;

    int iPos=0,iTarPos;

    if(bReset)
    {
        iCCDLightDownTask=1;
        return true;
    }
    else
    {
        if(t_ti.iPosName==(int)ePC_Top)
        {
            iTarPos=iLight_Up_Z_Top;
        }
        else
        {
            iTarPos=iLight_Up_Z_Btm;
        }

        int &Task=iCCDLightDownTask;

        switch(Task)
        {
            case 1:
                SW[SwLightStart].OnOff(true);
                Task=20;
                break;
            case 20:
                if(Sen[SnLightZINP].IsOff())
                {
                    iPos=(bDowm)?0:iTarPos;
                    fiosetview->SetLightZI0(iPos);

                    for(int i=0;i<7;i++)
                        SW[SwPRG[i]].On();
                    Task=30;
                }
                break;
            case 30:
                SW[SwLightStart].OnOff(false);
                Task=40;
                break;
            case 40:
                if(Sen[SnLightZINP].IsOff())
                {
                    SW[SwLightStart].OnOff(true);
                    Task=40;
                    return true;
                }
                break;
        }
        return false;
    }
//    static bool bflag=false;
//    if(bSimuMot==true)
//        return true;
//
//    int iPos=0,iTarPos;
//    if(t_ti.iPosName==(int)ePC_Top)
//    {
//        iTarPos=iLight_Up_Z_Top;
//    }
//    else
//    {
//        iTarPos=iLight_Up_Z_Btm;
//    }
//
//    iPos=(bDowm)?0:iTarPos;
//
//    fiosetview->SetLightZI0(iPos);
//
//    if(SW[SwLightStart].Status()==true)
//        hLightActionTimer.SetMSAndOn(100);
//
//    SW[SwLightStart].OnOff(false);
//    if(hLightActionTimer.Off() && Sen[SnLightZINP].IsOff())
//    {
//        SW[SwLightStart].OnOff(true);
//        return true;
//    }
//
//    return false;
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoMoveFixedSeatXY(int iX,int iY)
{
    if(bSimuMot==true)
        return true;
    bool b1=MOT[MTopAOIArmX].MotorMove(iX);
    bool b2=MOT[MTopAOIArmY].MotorMove(iY);
    static int iOldX=0,iOldY=0;

    if(b1 && b2)
    {
        if(iX!=iOldX || iY!=iOldY)
        {
            iOldX=iX;
            iOldY=iY;
            RecordMsg(AnsiString().sprintf("Move Fixed Seat X:%d Y:%d",iX,iY));
        }
        return true;
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoMoveOutArmSeatXY(int iX,int iY)
{
    if(bSimuMot==true)
        return true;
    bool b1=MOT[MOutArmX].MotorMove(iX);
    bool b2=MOT[MOutArmY].MotorMove(iY);
    static int iOldX=0,iOldY=0;
    if(b1 && b2)
    {
        if(iX!=iOldX || iY!=iOldY)
        {
            iOldX=iX;
            iOldY=iY;
            RecordMsg(AnsiString().sprintf("Move OutArm Seat X:%d Y:%d",iX,iY));
        }
        return true;
    }
    else
        return false;
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoMoveFixedSeatXY2RotatePos()
{
    return DoMoveFixedSeatXY(tbCenter.ppPosition.X,tbCenter.ppPosition.Y);
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoPhotosCenterpos(bool bIsFirst)
{
    bool bResult=false;
    if(bIsFirst)
    {
        asErrorMsg="";
        iPhotosCenterposTask=1;
        bResult=true;
    }
    else
    {
        if(asErrorMsg!="")
            return true;
        int &Task=iPhotosCenterposTask;
        switch(Task)
        {
            case 1://Initial
                if(DoClamp((int)eCN_Total,true)==true)
                    Task=100;
                break;
            case 100:
                {
                    bool b1=DoCamaDown(false);             //Camara up
                    bool b2=DoClamp((int)eCN_Total,true); //Clamp ALL
                    if(b1 && b2)
                    {
                        Task=110;
                    }
                }
                break;
            case 110:// Out Arm Z Up
                if(MoveOutArmToAutoSafe())
                {
                    Task=150;
                }
                break;
            case 150:
                //Clamp on   Clamp Type 1
                if(DoMoveFixedSeatXYandClamp((int)eICS_LessThanOrEqual65mm,0))
                {
                    Task=155;
                }
                break;
            case 155:// Out Arm Z Down to AOI Height
                if(DoMoveZ_TopView(Prod.iScannerAOI_Z))
                {
                    Task=200;
                }
                break;
            case 200:
                t_ti.iGrabIndex=(int)eGI_1_2;
                DoTakePhoto(true);
                Task=201;
//                break;
            case 201:
                if(DoTakePhoto(false)==true)
                {
                    Task=205;
                }
                break;
            case 205:// Out Arm Z Up
                if(MoveOutArmToAutoSafe())
                {
                    if(DoClamp((int)eCN_Total,true)==true)  //All lock
                    {
                        if(FrmAOI->ttbInsp->iAction==eBottom)
                            Task=9999;
                        else
                            Task=300;
                    }
                }
                break;
            case 300:
                if(MoveOutArmToAutoSafe())
                {
                    Task=350;
                }
                break;
            case 350:
                if(DoMoveFixedSeatXYandClamp((int)eICS_LessThanOrEqual65mm,1))
                {
                    Task=355;
                }
                break;
            case 355:// Out Arm Z Down to AOI Height
                if(DoMoveZ_TopView(Prod.iScannerAOI_Z))
                {
                    Task=400;
                }
                break;
            case 400:
                t_ti.iGrabIndex=(int)eGI_2_2;
                DoTakePhoto(true);
                Task=401;
//                break;
            case 401:
                if(DoTakePhoto(false)==true)
                {
                    Task=405;
                }
                break;
            case 405:// Out Arm Z Up
                if(MoveOutArmToAutoSafe())
                {
                    if(DoClamp((int)eCN_Total,true)==true)
                    {
                        Task=9999;
                    }
                }
                break;
            case 9999:// finish
                bResult=true;
                break;
            default:
                break;
        }
    }
    return bResult;
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoPhotos4pos(bool bIsFirst)
{
    bool bResult=false;
    if(bIsFirst)
    {
        asErrorMsg="";
        iPhotos4posTask=1;
        bResult=true;
    }
    else
    {
        int &Task=iPhotos4posTask;
        switch(Task)
        {
            case 1://Initial
                Task=100;
                break;
            case 100:
                {
                    bool b1=DoCamaDown(false);             //Camara up
                    bool b2=DoClamp((int)eCN_Total,true); //Clamp ALL
                    if(b1 && b2)
                    {
                        Task=110;
                    }
                }
                break;
            case 110:// Out Arm Z Up
                if(MoveOutArmToAutoSafe())
                {
                    Task=150;
                }
                break;
            case 150:
                if(DoMoveFixedSeatXYandClamp((int)eICS_MoreThan65mm,0))
                {
                    Task=155;
                }
                break;
            case 155:// Out Arm Z Down to AOI Height
                if(DoMoveZ_TopView(Prod.iScannerAOI_Z))
                {
                    Task=200;
                }
                break;
            case 200:
                t_ti.iGrabIndex=(int)eGI_1_4;
                DoTakePhoto(true);
                Task=201;
//                break;
            case 201:
                if(DoTakePhoto(false)==true)
                {
                    Task=205;
                }
                break;
            case 205:// Out Arm Z Up
                if(MoveOutArmToAutoSafe())
                {
                    Task=300;
                }
                break;
            case 300:
                if(DoClamp((int)eCN_Total,true)) //Clamp ALL
                {
                    Task=310;
                }
                break;
            case 310:// Out Arm Z Up
                if(MoveOutArmToAutoSafe())
                {
                    Task=350;
                }
                break;
            case 350:
                if(DoMoveFixedSeatXYandClamp((int)eICS_MoreThan65mm,1))
                {
                    Task=355;
                }
                break;
            case 355:// Out Arm Z Down to AOI Height
                if(DoMoveZ_TopView(Prod.iScannerAOI_Z))
                {
                    Task=400;
                }
                break;
            case 400:
                t_ti.iGrabIndex=(int)eGI_2_4;
                DoTakePhoto(true);
                Task=401;
//                break;
            case 401:
                if(DoTakePhoto(false)==true)
                {
                    Task=405;
                }
                break;
            case 405:// Out Arm Z Up
                if(MoveOutArmToAutoSafe())
                {
                    Task=500;
                }
                break;
            case 500:
                if(DoClamp((int)eCN_Total,true))   //Clamp ALL
                {
                    Task=510;
                }
                break;
            case 510:// Out Arm Z Up
                if(MoveOutArmToAutoSafe())
                {
                    Task=550;
                }
                break;
            case 550:
                if(DoMoveFixedSeatXYandClamp((int)eICS_MoreThan65mm,2))
                {
                    Task=555;
                }
                break;
            case 555:// Out Arm Z Down to AOI Height
                if(DoMoveZ_TopView(Prod.iScannerAOI_Z))
                {
                    Task=600;
                }
                break;
            case 600:
                t_ti.iGrabIndex=(int)eGI_3_4;
                DoTakePhoto(true);
                Task=601;
//                break;
            case 601:
                if(DoTakePhoto(false)==true)
                {
                    Task=605;
                }
                break;
            case 605:// Out Arm Z Up
                if(MoveOutArmToAutoSafe())
                {
                    Task=700;
                }
                break;
            case 700:
                if(DoClamp((int)eCN_Total,true)) //Clamp ALL
                {
                    Task=710;
                }
                break;
            case 710:// Out Arm Z Up
                if(MoveOutArmToAutoSafe())
                {
                    Task=750;
                }
                break;
            case 750:
                if(DoMoveFixedSeatXYandClamp((int)eICS_MoreThan65mm,3))
                {
                    Task=755;
                }
                break;
            case 755:// Out Arm Z Down to AOI Height
                if(DoMoveZ_TopView(Prod.iScannerAOI_Z))
                {
                    Task=800;
                }
                break;
            case 800:
                t_ti.iGrabIndex=(int)eGI_4_4;
                DoTakePhoto(true);
                Task=801;
//                break;
            case 801:
                if(DoTakePhoto(false)==true)
                {
                    Task=805;
                }
                break;
            case 805:// Out Arm Z Up
                if(MoveOutArmToAutoSafe())
                {
                    Task=9999;
                }
                break;
            case 9999:// finish
                bResult=true;
                break;
            default:
                break;
        }
    }
    return bResult;
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoMoveXYZ2AOI(int iSuckRow,int iSuckCol,bool bPlace)
{
    if(bSimuMot==true)
        return true;

    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iXPos=0, iYPos=0;
    int iXVariable[X_PITCH_COUNT];
    int iYVariable  =0;
    int iMovePitchX =8000;
    int iMovePitchY =TestIF.iARM_Y_PITCH;
    int iOffsetPos  =-1;
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    if(XPitchIsStand())                                                         //X Pitch is 4000mm
        iMovePitchX=TestIF.dSiteXPitch*3;
    else
        iMovePitchX=TestIF.dSiteXPitch+TestIF.dSiteXPitch/2;

    for(int i=0; i<X_PITCH_COUNT; i++)
        iXVariable[i]=GetOutArmPitchX_9045(iMovePitchX, i, iOffsetPos);
    iYVariable=GetOutArmPitchY_9045(iMovePitchY, iOffsetPos);

    iXPos=Prod.iScannerAOI_X;
    iYPos=Prod.iScannerAOI_Y;

    //suck xy to cell(iy,ix)
    iXPos-=(iSuckCol-iOutArmXBase)*(iMovePitchX/3);
    iYPos+=(iSuckRow-iOutArmYBase)*iMovePitchY;

    static bool bXarrival=false;
//    int iSuck=iSuckRow+iSuckCol*MAX_ARM_Row;
    bool bDoSuck=IsOutArmArrival(uPoint2D(iXPos,iYPos));
    if(bDoSuck==true)
    {
        bZFlag[iSuckRow][iSuckCol]=true;
        if(bPlace==false)//need down more at pick
        {
            iZPos[iSuckRow][iSuckCol]=Prod.OutArm_ScannerAOI_Pick[iSuckRow][iSuckCol];
        }
        else
        {
            iZPos[iSuckRow][iSuckCol]=Prod.OutArm_ScannerAOI_Place[iSuckRow][iSuckCol];
        }
    }
    else if(bDoSuck==false && MoveOutArmToAutoSafe()==false)
    {
        bXarrival=false;
        return false;
    }

    if(bDoSuck==true && IsOutArmSuckArrival(iSuckRow,iSuckCol,iZPos[iSuckRow][iSuckCol]))
    {
        return true;
    }
    else if(bXarrival==false)
    {
        if(IsMotorArrival(MOutArmX,iXPos)==false)
        {
            MOT[MOutArmX].MotorMove(iXPos);
        }
        else
        {
            bXarrival=true;
        }
        return false;
    }
    else if(OutArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, false))
    {
        return true;
    }
    else
        return false;
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoMoveXYZ2Sht2(int iSuckRow,int iSuckCol,bool bPlace)
{
    if(bSimuMot==true)
        return true;

    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iXVariable[X_PITCH_COUNT];
    int iYVariable  =0;
    int iMovePitchX =iXpitchMaxX3;
    int iMovePitchY =TestIF.iARM_Y_PITCH;
    int iOffsetPos  =-1;
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    for(int i=0; i<X_PITCH_COUNT; i++)
        iXVariable[i]=GetOutArmPitchX_9045(iMovePitchX, i, iOffsetPos);
    iYVariable=GetOutArmPitchY_9045(iMovePitchY, iOffsetPos);

    int iXPos=Prod.XOutArm_Shuttle2_Pick[iOutArmYBase][iOutArmXBase];
    int iYPos=Prod.YOutArm_Shuttle2_Pick[iOutArmYBase][iOutArmXBase];
    bool bZDown=true;

    //suck xy to cell(iy,ix)
    if(USE_PICKER_COUNT!=ep1Picker)
        iXPos-=(iSuckCol-iOutArmXBase)*(iMovePitchX/3);
    iYPos+=(iSuckRow-iOutArmYBase)*iMovePitchY;

    iXPos+=iOutArmShtXCenterPos;
    iYPos+=iOutArmShtYCenterPos;//wrong coding

    static bool bYarrival=false;
//    int iSuck=iSuckRow+iSuckCol*MAX_ARM_Row;
    bool bDoSuck=IsOutArmArrival(uPoint2D(iXPos,iYPos));
    if(bDoSuck==true)
    {
        iZPos[iSuckRow][iSuckCol]=Prod.ZOutArm_Shuttle2_Pick[iSuckRow][iSuckCol]+iZPickOffset;
        bZFlag[iSuckRow][iSuckCol]=true;
    }
    else if(bDoSuck==false && MoveOutArmToAutoSafe()==false)
    {
        bYarrival=false;
        return false;
    }

    if(bDoSuck==true && IsOutArmSuckArrival(iSuckRow,iSuckCol,iZPos[iSuckRow][iSuckCol]))
    {
        return true;
    }
    else if(bYarrival==false)
    {
        if(IsMotorArrival(MOutArmY,iYPos)==false)
        {
            MOT[MOutArmY].MotorMove(iYPos);
        }
        else
        {
            bYarrival=true;
        }
        return false;
    }
    else if(OutArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, bZDown))
    {
        return true;
    }
    else
        return false;
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoMoveXY2SafePos()
{
    if(bSimuMot==true)
        return true;

    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iMovePitchX=iXpitchMaxX3;
    int iMovePitchY=6000;
    int iXVariable[X_PITCH_COUNT];
    int iYVariable;
    int iXPos=Prod.iTopViewSafePos_X;
    int iYPos=Prod.iTopViewSafePos_Y;
//    bool bZDown=false;
    static bool bYarrival=false;
    int iOffsetPos  =-1;
    bool bArrival=IsOutArmArrival(uPoint2D(iXPos,iYPos));

    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    for(int i=0; i<X_PITCH_COUNT; i++)
        iXVariable[i]=GetOutArmPitchX_9045(iMovePitchX, i, iOffsetPos);
    iYVariable=GetOutArmPitchY_9045(iMovePitchY, iOffsetPos);

    if(bArrival==false)
    {
        bYarrival=false;
    }

    if(bArrival==true)
    {
        return true;
    }
    else if(bArrival==false && MoveOutArmToAutoSafe()==false)
    {
        return false;
    }
    else if(IsMotorArrival(MOutArmY,iYPos)==false)
    {
        MOT[MOutArmY].MotorMove(iYPos);
        return false;
    }
    else if(OutArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, false))
    {
        return true;
    }
    else
        return false;
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::GetNeedActSuck(int &iSuckRow,int &iSuckCol)
{
    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            if(bActiveSuck[i][j]==true)
            {
                iSuckRow=i;
                iSuckCol=j;
                return true;
            }
        }
    }
    return false;
}
//---------------------------------------------------------------------------
int TTopBottomInspect::DoSuckDestroyVacuum(int iSuckRow,int iSuckCol)  //0:running 1:finish 2:skip 3:retry
{
    if(bSimuMot==true)
        return true;
    bool SuckError=false;
    if(OutArmSuck.Suck[iSuckRow][iSuckCol].Destroy())
    {
        bOutArmCheckDestroyACT[iSuckRow][iSuckCol]=true; //jou 981130 確認device確實destroy完成
        OutArmSuck.ResetAll();       //Steven 20160323 : 避免未開啟真空
        return ess_Finish;
    }

    if(OutArmSuck.Suck[iSuckRow][iSuckCol].Error)
    {
        SuckError=true;
    }

    if(SuckError)
    {
        AnsiString ErrPart="";ErrPart+=OutArmSuck.Suck[iSuckRow][iSuckCol].sName;
        OutArmSuck.ResetAll();       //Steven 20160323 : 避免未開啟真空
        int ret=ShowErrorMessage("JAM0203", K_RETRY|K_SKIP, MOutArmX, false, ErrPart);
        if(ret==K_SKIP)
        {
            return ess_Skip;
        }
        else       //K_RETRY
        {
            return ess_Retry;
        }
    }
    return ess_Running;
}
//---------------------------------------------------------------------------
int TTopBottomInspect::DoSuckCreateVacuum(int iSuckRow,int iSuckCol)     //0:running 1:finish 2:skip 3:retry
{
    if(bSimuMot==true)
        return true;
    bool SuckError=false;
    if(OutArmSuck.Suck[iSuckRow][iSuckCol].Suck())
    {
        OutArmSuck.ResetAll();       //Steven 20160323 : 避免未開啟真空
        return ess_Finish;
    }

    if(OutArmSuck.Suck[iSuckRow][iSuckCol].Error)
    {
        SuckError=true;
    }

    if(SuckError)
    {
        int ret=ShowErrorMessage("JAM0201", K_RETRY|K_SKIP, MOutArmX, false, NULL);
        if(ret==K_SKIP)
        {
            return ess_Skip;
        }
        else //K_RETRY
        {
            return ess_Retry;
        }
    }
    return ess_Running;
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoMoveFixedSeatAndCCD2Ready(bool bIsFirst,bool &bError)
{
    if(bSimuMot==true)
        return true;
    bool bResult=false;
    if(bIsFirst)
    {
        iMoveFixedSeatAndCCD2ReadyTask=1;
        bResult=true;
        bError=false;
    }
    else
    {
        int &Task=iMoveFixedSeatAndCCD2ReadyTask;
        switch(Task)
        {
            case 1: //Initial  //move out arm to safe
                if(GetRotateIs0())
                {
                    Task=500;
                }
                else
                {
                    DoRotate(0, true);
                    Task=100;
                }
                break;
            case 100:
                if(DoMoveFixedSeatXY2RotatePos()==true && DoRotate(0)==true)
                {
                    if(GetRotateIs0())
                    {
                        Task=500;
                    }
                    else
                    {
                        ShowMyMessage("Rotary table motor skipped steps.");
                        bError=true;
                        Task=9999;
                    }
                }
                break;
            case 500:
                if(DoMoveXY2SafePos()==true)
                {
                    bool b1=DoMoveFixedSeatXY2RotatePos();
                    bool b2=DoCamaDown(false);  //ccd up
                    bool b3=DoClamp((int)eCN_Total,false);
                    if(b1 && b2 && b3)
                    {
                        Task=9999;
                    }
                }
                break;
            case 9999:
                bResult=true;
                break;
        }
    }
    return bResult;
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoSimulationTopBtmInspFunc(bool bIsFirst,AnsiString &sErr)
{
    bool bResult=false;
    int iSiteMapForArmZHome[8]={0, 2, 4, 6, 1, 3, 5, 7};
    int &Task=iSimulationTopBtmInspTask;
    int iKitRow=0,iKitCol=0;
    if(bIsFirst)
    {
        sErr="";
        Task=1;
        bResult=true;
    }
    else
    {
        bool bError=false;
        int iSuckRow=0,iSuckCol=2;
        if(sErr!="")
            return true;
        switch(Task)
        {
            case 1://Initial
                {
                    CommClose();
                    DoMoveFixedSeatAndCCD2Ready(true,bError);
                    Task=500;
                }
                break;
            case 500:
                if(DoMoveFixedSeatAndCCD2Ready(false,bError))
                {
                    if(bError==true)
                    {
                        sErr=AnsiString().sprintf("%s error!",__FUNC__);
                        Task=9900;
                    }
                    else
                    {
                        OutArmSuck.ResetAll();       //Steven 20160323 : 避免未開啟真空
                        Task=1000;
                    }
                }
                break;
            case 1000://OutArm suck from fixed seat
                {
                    BRCarryKit.SetItemData(iKitRow, iKitCol, HAS_IC);
                    OutArmSuck.ClearAll();
                    Task=1001;
                }
                break;
            case 1001://OutArm suck from fixed seat
                {
                    bool bPlace=false;
                    int ret=DoOutarm2Sht2PickPlaceIC(iSuckRow,iSuckCol,bPlace);  //0:running 1:finish 2:skip 3:retry
                    if(ret==ess_Running)
                    {
                    }
                    else if(ret==ess_Finish)
                    {
                        SetMotorScaleSpeed(MOutArmZA+iSiteMapForArmZHome[iSuckRow*MAX_ARM_Row+iSuckCol], 10);
                        Task=2000;
                    }
                    else if(ret==ess_Skip)
                    {
                        Task=9900;//leave
                    }
                    else
                    {
                        //retry
                    }
                }
                break;
            case 2000://OutArm suck up
                if(MoveOutArmToAutoSafe()==true)
                {
                    DoTopBtmInspFunc(true);
                    Task=3000;
                }
                break;
            case 3000:
                if(DoTopBtmInspFunc(false)==true)
                {
                    Task=3500;
                }
                break;
            case 3500:    //add for back to sht2 IC Fall down
                if(MoveOutArmToAutoSafe()==true && MOT[MOutArmY].MotorMove(0)==true)
                    Task=4000;
                break;
            case 4000:    //place ic to sht2
                {
                    bool bPlace=true;
                    int ret=DoOutarm2Sht2PickPlaceIC(iSuckRow,iSuckCol,bPlace);  //0:running 1:finish 2:skip 3:retry
                    if(ret==0)
                    {
                        //wait
                    }
                    else if(ret==1)
                    {
                        Task=9900;
                    }
                    else if(ret==2) //leave
                    {
                        Task=9900;//leave
                    }
                    else
                    {
                        //retry
                    }
                }
                break;
            case 9900:  //Out Arm Move to safe area
                if(DoMoveXY2SafePos()==true)
                {
                    Task=9999;
                }
                break;
            case 9999:  //Finish
                if(bCycleRun)
                {
                    SetMotorScaleSpeed(MOutArmZA+iSiteMapForArmZHome[iSuckRow*MAX_ARM_Row+iSuckCol], ArmSpeed[OutArm].iZSP);
                    sErr="";
                    Task=1000;
                }
                else
                {
                    BRCarryKit.SetItemData(iKitRow, iKitCol, NULL_IC);
                    bResult=true;
                }
                break;
        }
    }
    return bResult;
}
//---------------------------------------------------------------------------
void TTopBottomInspect::SetSimuICInfo()
{
    t_ti.iPosName=(int)ePC_Top;
    t_ti.CamName=sCamaName;
    t_ti.LotID="L2OOXX.GG_FT";
    t_ti.ArmSite="1_0";
    t_ti.BinResult="1";
    t_ti.TouchDown="8692";
    t_ti.DeviceInfo="NA";
    t_ti.iGrabIndex=(int)eGI_1_2;
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::SetSuckActive(int iStation)
{
    if(bSimuMot==true)
    {
        if(bActiveSuck[0][0]==true)
            return true;
        else
            return false;
    }
    ZeroMemory(bActiveSuck ,sizeof(bActiveSuck));
    for(int irow=0; irow<MAX_ARM_Row; irow++)
    {
        for(int icol=0; icol<MAX_ARM_Col; icol++)
        {
            if((OutArmSuck.Item[irow][icol]!=NULL_IC &&
                OutArmSuck.Item[irow][icol]!=HAS_NULL_IC) &&
                OutArmSuck.iAOIStation[irow][icol]!=iStation)
            {
                bActiveSuck[irow][icol]=true;
                t_ti.LotID=fLotInfo->edtSysLotID->Text;
                t_ti.ArmSite=OutArmSuck.iWhichSht;
                t_ti.BinResult=OutArmSuck.iBinData[irow][icol];
                t_ti.TouchDown=OutArmSuck.PordRec[irow][icol].asBuffer->Strings[eOrderTest];
                if(CosFunction.b2DUsePinInspection && TestIF_File.b2DUsePinInspection)
                {
                    t_ti.DeviceInfo=iAOILotCount+1;
////                    if(OutArmSuck.cDeviceInf[irow][icol]=="@SEOK+")
////                        t_ti.DeviceInfo="PIN1_PASS";
////                    else
////                        t_ti.DeviceInfo="PIN1_FAIL";
                }
                else
                {
                    t_ti.DeviceInfo=OutArmSuck.cDeviceInf[irow][icol];
                }
                return true;
            }
            else
            {
                OutArmSuck.iAOIStation[irow][icol]=eAOI_SCANAOI;
            }
        }
    }
    return false;
}
//---------------------------------------------------------------------------
AnsiString TTopBottomInspect::DoSendCommand(bool bIsFirst,AnsiString asCommand)
{
    if(bIsFirst)
    {
        asRecMsg="";
        socketAOI->asReceiveMsg="";
    }
    return socketAOI->DoSendCommand(bIsFirst,asCommand);
}
//---------------------------------------------------------------------------
void TTopBottomInspect::ReceiveData(char* cGet,int iLen)
{
    return socketAOI->ReceiveData(cGet,iLen);
}
//---------------------------------------------------------------------------
void TTopBottomInspect::SendCommandSocket(AnsiString asCommand)
{
    return socketAOI->SendCommandSocket(asCommand);
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoCommuncation()
{
    return socketAOI->DoCommuncation();
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::IsCommActive()
{
    return socketAOI->IsCommActive();
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::GetRotateIs0()
{
    return MOT[MTopAOIArmR].Led[iHomeLed];
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::GetRotateIs180()
{
    return Sen[SnTopBtmAOIR180].IsOn();
}
//---------------------------------------------------------------------------
AnsiString TTopBottomInspect::GetGrabIndex(int iGrpIdx)
{
    if(iGrpIdx==(int)eGI_1_4)
    {
        return "1/4";
    }
    else if(iGrpIdx==(int)eGI_2_4)
    {
        return "2/4";
    }
    else if(iGrpIdx==(int)eGI_3_4)
    {
        return "3/4";
    }
    else if(iGrpIdx==(int)eGI_4_4)
    {
        return "4/4";
    }
    else if(iGrpIdx==(int)eGI_2_2)
    {
        return "2/2";
    }
    else//eGI_1_2
    {
        return "1/2";
    }
}
//---------------------------------------------------------------------------
AnsiString TTopBottomInspect::GetTaskInfo()
{
    return AnsiString().sprintf("%d,%d,%d,%d,%d,%d,%d"
                                ,iSimulationTopBtmInspTask
                                ,iTopBtmInspFuncTask
                                ,iTopBtmInspFuncInspTask
                                ,iPhotosCenterposTask
                                ,iPhotos4posTask
                                ,iTakePhotoTask
                                ,iMoveFixedSeatAndCCD2ReadyTask
                                );
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoMoveFixedSeatXYandClamp(int iIC_Size,int iStep)
{
    t_TopBtnPhotoInfo* tInfo=(iIC_Size==(int)eICS_LessThanOrEqual65mm)?tbpiLessThanOrEqual65mm:tbpiMoreThan65mm;
    bool b1=false,b2=false,b3=false,b4=false,b5=false;
    int ix, iy;
//    int iPos;
//    int iUseSuck;
    int iSuckRow, iSuckCol;
    GetNeedActSuck(iSuckRow,iSuckCol);

    if(FrmAOI->ttbInsp->iAction==eBottom)
    {
        if(GetIC_Size()==(int)eICS_LessThanOrEqual65mm)
        {
            if(iStep==0)
            {
                ix=Prod.iScannerAOI_X+((DeviceForm_File.XDimension*100)/4);
                iy=Prod.iScannerAOI_Y-((DeviceForm_File.YDimension*100)/4);
            }
            else
            {
                ix=Prod.iScannerAOI_X-((DeviceForm_File.XDimension*100)/4);
                iy=Prod.iScannerAOI_Y+((DeviceForm_File.YDimension*100)/4);
            }
        }
        else
        {
            if(iStep==10)
            {
                ix=Prod.iScannerAOI_X;
                iy=Prod.iScannerAOI_Y;
            }
            else
            {
                if(iStep%2==0)
                    ix=Prod.iScannerAOI_X+((DeviceForm_File.XDimension*100)/4);
                else
                    ix=Prod.iScannerAOI_X-((DeviceForm_File.XDimension*100)/4);

                if(iStep>=2)
                    iy=Prod.iScannerAOI_Y-((DeviceForm_File.YDimension*100)/4);
                else
                    iy=Prod.iScannerAOI_Y+((DeviceForm_File.YDimension*100)/4);
            }
        }

//        iMovePitchX=iXpitchMaxX3;
//        iMovePitchY=6000;
//        iXVariable =GetOutArmPitch_9045(iMovePitchX);
//        iXVariable2=GetOutArmPitch_9045(iMovePitchX);
//        iYVariable=GetOutArmPitchY_9045(iMovePitchY);       //Steven 20160627 : Add for Y-Pitch

        //suck xy to cell(iy,ix)
//        if(TestIF_File.bSingleUseOtherSuck)
//            iUseSuck=2;
//        else if(TestIF_File.bSingleUseDSuck)
//            iUseSuck=1;
//        else
//            iUseSuck=3;
        if(USE_PICKER_COUNT!=ep1Picker)
            ix-=(iSuckCol-iOutArmXBase)*(iXpitchMaxX3/3);
        iy+=(iSuckRow-iOutArmYBase)*6000;

        b1=DoMoveOutArmSeatXY(ix, iy);
        b2=true;
        b3=true;
        b4=true;
        b5=true;
    }
    else
    {
        b1=DoMoveFixedSeatXY(tInfo[iStep].ppPosition.X,tInfo[iStep].ppPosition.Y);
        if(t_ti.iPosName==(int)ePC_Top)       //Cuz rotate 180
        {
            b2=DoClamp((int)eCN_TopLeft,       tInfo[iStep].bClampLB);
            b3=DoClamp((int)eCN_BottomLeft,    tInfo[iStep].bClampLT);
            b4=DoClamp((int)eCN_TopRight,      tInfo[iStep].bClampRB);
            b5=DoClamp((int)eCN_BottomRight,   tInfo[iStep].bClampRT);
        }
        else
        {
            b2=DoClamp((int)eCN_TopLeft,       tInfo[iStep].bClampLT);
            b3=DoClamp((int)eCN_BottomLeft,    tInfo[iStep].bClampLB);
            b4=DoClamp((int)eCN_TopRight,      tInfo[iStep].bClampRT);
            b5=DoClamp((int)eCN_BottomRight,   tInfo[iStep].bClampRB);
        }
    }
    return (b1 && b2 && b3 && b4 && b5);
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoLockRotate(bool bOn)
{
    SW[SwTopBtmRotateLockOn].OnOff(bOn);
    SW[SwTopBtmRotateLockOff].OnOff(!bOn);

    if(bOn)
    {
        if(Sen[SnTopBtmRotateLockOn1].IsOn() && Sen[SnTopBtmRotateLockOn2].IsOn())
            return true;
        else
            return false;
    }
    else
    {
        if(Sen[SnTopBtmRotateLockOn1].IsOff() && Sen[SnTopBtmRotateLockOn2].IsOff())
            return true;
        else
            return false;
    }

//    SW[SwTopBtmRotateLockOff].OnOff(!bOn);
//    return DoCylinder(C_TopBtmRotateLock,bOn);
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::DoSht2ToRight()
{
    if(IsTestZ2NotSafeShuttle2CanNotMove(iRetryCount))
    {
        ShowMyMessage("Check Index 2 Home");
        return false;
    }
    else
    {
        if(MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iRight))
        {
            return true;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
int TTopBottomInspect::DoOutarm2Sht2PickPlaceIC(int iSuckRow,int iSuckCol,bool bPlace)    //0:running 1:finish 2:skip 3:retry
{
    int iKitRow=0,iKitCol=0;    //need add to func parameter
    if(DoSht2ToRight())
    {
        if(OutArmSuck.Suck[iSuckRow][iSuckCol].GetStatus()==false)
            OutArmSuck.Suck[iSuckRow][iSuckCol].On();
        if(DoMoveXYZ2Sht2(iSuckRow,iSuckCol,bPlace))
        {
            if(bPlace)
            {
                int ret=DoSuckDestroyVacuum(iSuckRow,iSuckCol);
                if(ret==ess_Running)
                {
                    //wait
                }
                else   //active
                {
                    if(ret==ess_Finish)
                    {
                        BRCarryKit.MoveSuckDataDiff(OutArmSuck,iKitRow,iKitCol,iSuckRow,iSuckCol);
                    }
                    else if(ret==ess_Skip)
                    {
                        OutArmSuck.SetItemData(iSuckRow, iSuckCol, NULL_IC);
                        BRCarryKit.SetItemData(iKitRow, iKitCol, NULL_IC);
                    }
                    return ret;
                }
            }
            else     //pick
            {
                int ret=DoSuckCreateVacuum(iSuckRow,iSuckCol);
                if(ret==ess_Running)
                {
                    //wait
                }
                else   //active
                {
                    if(ret==ess_Finish)
                    {
                        OutArmSuck.MoveSuckDataDiff(BRCarryKit,iKitRow,iKitCol, iSuckRow, iSuckCol);
                    }
                    else if(ret==ess_Skip)
                    {
                        OutArmSuck.SetItemData(iSuckRow, iSuckCol, NULL_IC);
                        BRCarryKit.SetItemData(iKitRow, iKitCol, NULL_IC);
                    }
                    return ret;
                }
            }
        }
    }
    return ess_Running;
}
//---------------------------------------------------------------------------
int TTopBottomInspect::DoOutarm2AOI_PickPlaceIC(int iSuckRow,int iSuckCol,bool bPlace)    //0:running 1:finish 2:skip 3:retry
{
    int iKitRow=0,iKitCol=0;    //need add to func parameter
    OutArmSuck.Suck[iSuckRow][iSuckCol].On();
    if(DoMoveXYZ2AOI(iSuckRow,iSuckCol,bPlace))
    {
        if(bPlace)
        {
            if(DoClamp((int)eCN_Total,true)==true)
            {
                int ret=DoSuckDestroyVacuum(iSuckRow,iSuckCol);
                if(ret==ess_Running)
                {
                    //wait
                }
                else   //active
                {
                    if(ret==ess_Skip)
                    {
                        OutArmSuck.SetItemData(iSuckRow, iSuckCol, NULL_IC);
                        AOIKit.SetItemData(iKitRow, iKitCol, NULL_IC);
                    }
                    else  if(ret==ess_Finish)//吸取狀態下夾IC，夾完後放氣
                    {
                        AOIKit.MoveSuckDataDiff(OutArmSuck,iSuckRow,iSuckCol,iKitRow,iKitCol);
                    }
                    return ret;
                }
            }
        }
        else     //pick
        {
            int ret=DoSuckCreateVacuum(iSuckRow,iSuckCol);
            if(ret==ess_Running)
            {
                //wait
            }
            else if(ret==ess_Finish)//suck and clamp off
            {
                if(DoClamp((int)eCN_Total,false)==true)
                {
                    OutArmSuck.MoveSuckDataDiff(AOIKit,iKitRow,iKitCol, iSuckRow, iSuckCol);
                    OutArmSuck.iAOIStation[iSuckRow][iSuckCol]=eAOI_SCANAOI;
                    return ret;
                }
            }
            else   //active
            {
                if(ret==ess_Skip)
                {
                    OutArmSuck.SetItemData(iSuckRow, iSuckCol, NULL_IC);
                    AOIKit.SetItemData(iKitRow, iKitCol, NULL_IC);
                }
                return ret;
            }
        }
    }
    return ess_Running;
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::IsOutArmSuckArrival(int iSuckRow,int iSuckCol,int iPlace)
{
    int iSuck=iSuckRow+iSuckCol*MAX_ARM_Row;
    int iMot=MOutArmZA+iSuck;
    return TMyMotor().CheckArmPosArrival(iPlace, MOT[iMot].ReadPos(), 10);
}
//---------------------------------------------------------------------------
bool TTopBottomInspect::CheckAirPressureIsEnough()
{
    if(FrmAOI->ttbInsp->iAction==eBottom)
        return true;
    if(Sen[SnTopBtmAirMaxAlarm].Status()==true)
    {
        ShowMyMessage("The clamp air pressure is over the maximum limit.");
    }
    else if(Sen[SnTopBtmAirMinAlarm].Status()==true)
    {
        ShowMyMessage("The clamp air pressure is below the minimum limit.");
    }
    else
        return true;
    return false;
}
//---------------------------------------------------------------------------
bool TFrmAOI::SendCommandManual(int iCmd,bool bfirst)
{
    switch(iCmd)
    {
        case 0:  //Top Inspect
            ttbInsp->t_ti.iPosName=0;
            return ttbInsp->DoStartInspect(bfirst);
        case 1:  //Bottom Inspect
            ttbInsp->t_ti.iPosName=1;
            return ttbInsp->DoStartInspect(bfirst);
        case 2:  //Grab Finish
            return ttbInsp->DoImagingCompleted(bfirst);
        case 3:  //Top Is Finish
            ttbInsp->t_ti.iPosName=0;
            return ttbInsp->DoInquireResult(bfirst);
        case 4:  //Bottom Is Finish
            ttbInsp->t_ti.iPosName=1;
            return ttbInsp->DoInquireResult(bfirst);
        case 5:  //Bin Out
            return ttbInsp->DoBinOutInfo(bfirst);
        case 6:  //Start Lot
            return ttbInsp->DoStartLot(bfirst,"TestLot");
        case 7:  //End Lot
            return ttbInsp->DoEndLot(bfirst);
        case 8:  //Test Lighting
            return ttbInsp->DoTestLighting(bfirst,3);
        case 9:  //Station Change
            return ttbInsp->DoCamaChange(bfirst);
        case 10:  //Auto Run
            return ttbInsp->DoAutoRun(bfirst);
        case 11:  //Pause
            return ttbInsp->DoPause(bfirst);
        case 12:  //Recipe Change
            return ttbInsp->DoRecipeChange(bfirst);
        case 13:  //Align Time
            return ttbInsp->DoTimeSynchronization(bfirst);
        case 14:  //Reset
            return ttbInsp->DoResetFlow(bfirst,3);
    }
    return true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmAOI::FormClose(TObject *Sender, TCloseAction &Action)
{
    if(USE_Scanner_AOI_Inspection==(int)eBtnAOI_TopBottomInstall)                    //Jimmychiu 20240322 : Top & Bottom Inspect
    {
        if(ttbInsp)
        {
            ttbInsp->bSimuAOICommand=false;
            ttbInsp->bSimuMot=false;
        }
        //SpinTimer->Enabled=false;
    }
    fShow=false;
}
//---------------------------------------------------------------------------
void __fastcall TFrmAOI::btnSimulateTopBtmClick(TObject *Sender)
{
    if((!ttbInsp) || SystemStart)
    {
        ShowMessage("設備運作中無法模擬");
        return;
    }
    ttbInsp->DoSimulationTopBtmInspFunc(true,sTopBtmInspErrMsg);
    if(GetSimulationState())
    {
        SetSimulationState(false);
    }
    else
    {
        SetSimulationState(true);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmAOI::sgICSmallDblClick(TObject *Sender)
{
    TStringGrid *grid=static_cast<TStringGrid*>(Sender);
    int iCol=grid->Col;
    int iRow=grid->Row;
    int iColTitle=grid->ColCount;
    int iRowTitle=grid->RowCount;
    if(iCol>=0 && iCol<iColTitle &&
       iRow>=0 && iRow<iRowTitle)
    {
        if(grid->Cells[iCol][0].AnsiPos(AnsiString("LT"))!=0 ||
           grid->Cells[iCol][0].AnsiPos(AnsiString("LB"))!=0 ||
           grid->Cells[iCol][0].AnsiPos(AnsiString("RT"))!=0 ||
           grid->Cells[iCol][0].AnsiPos(AnsiString("RB"))!=0)
        {
            if(grid->Cells[iCol][iRow].AnsiPos(AnsiString("1"))!=0)
            {
                grid->Cells[iCol][iRow]="0";
            }
            else
            {
                grid->Cells[iCol][iRow]="1";
            }
        }
        else if(grid->Cells[iCol][0].AnsiPos(AnsiString("X"))!=0 ||
                grid->Cells[iCol][0].AnsiPos(AnsiString("Y"))!=0)
        {
            AnsiString sMsg="Select input method"+GetNewLineStr();
            sMsg+=("YES:Manual Input")+GetNewLineStr();
            sMsg+=("NO :Auto Read Motor")+GetNewLineStr();
            int ret=ShowMyMessageBox_YES_NO(sMsg,"");
            if(ret==2)//NO
            {
                if((MOT[MTopAOIArmX].HomeFlag==0) ||
                   (MOT[MTopAOIArmY].HomeFlag==0))
                {
                    ShowMyMessage("motor need home", "馬達需要歸零", "Teach");
                    return;
                }
                int iXpos=MOT[MTopAOIArmX].ReadPos();
                int iYpos=MOT[MTopAOIArmY].ReadPos();
                grid->Cells[1][iRow]=IntToStr(iXpos);
                grid->Cells[2][iRow]=IntToStr(iYpos);
                return;
            }
            else//yes
            {
                TEdit *edtTemp=new TEdit(this);
                edtTemp->Text=grid->Cells[iCol][iRow];
                fQwertyKey->ShowQwertyKey(edtTemp, N_INTEGER);
                grid->Cells[iCol][iRow]=AnsiString(edtTemp->Text);
                delete edtTemp;
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmAOI::btnCmmStartLotClick(TObject *Sender)
{
    iDoSingleComm=(int)eSC_StartLot;
    DoSingleComm(true,iDoSingleComm);
}
//---------------------------------------------------------------------------
bool TFrmAOI::DoSingleComm(bool bFirst,int iType)
{
    if(!ttbInsp)return true;
    bSimuSingleComm=true;
    if(iType==(int)eSC_StartLot)
    {
        return ttbInsp->DoStartLot(bFirst,"LotID");
    }
    else if(iType==(int)eSC_EndLot)
    {
        return ttbInsp->DoEndLot(bFirst);
    }
    else
    {
        bSimuSingleComm=false;
        return true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmAOI::btnCmmEndLotClick(TObject *Sender)
{
    iDoSingleComm=(int)eSC_EndLot;
    DoSingleComm(true,iDoSingleComm);
}
//---------------------------------------------------------------------------
void TFrmAOI::InitialEdList(TEdit* (*edl)[6],int iRow,int iCow)
{
    for(int i=0; i<iRow; i++)
    {
        for(int j=0; j<iCow; j++)
        {
            edl[i][j]=new TEdit(this);
        }
    }
}
//---------------------------------------------------------------------------
AnsiString TFrmAOI::GetStringGridTitle(int iCol)
{
    AnsiString aslist[]={"X","Y","LT","LB","RT","RB"};
    if(iCol<6)
        return aslist[iCol];
    else
        return "";
}
//---------------------------------------------------------------------------
bool TFrmAOI::GetRotateIs0()
{
    if(ttbInsp)
        return ttbInsp->GetRotateIs0();
    else
        return false;
}
//---------------------------------------------------------------------------
void TFrmAOI::SetICSmallGrid(int iRow,int iCol,int iValue)
{
    sgICSmall->Cells[iRow+1][iCol+1]=IntToStr(iValue);
}
//---------------------------------------------------------------------------
void TFrmAOI::SetICLargeGrid(int iRow,int iCol,int iValue)
{
    sgICLarge->Cells[iRow+1][iCol+1]=IntToStr(iValue);
}
//---------------------------------------------------------------------------
void __fastcall TFrmAOI::btnStartClick(TObject *Sender)
{
    fMain->Start("AOI");
}
//---------------------------------------------------------------------------
void __fastcall TFrmAOI::btnPauseClick(TObject *Sender)
{
    fMain->Pause("AOI");
}
//---------------------------------------------------------------------------
bool TFrmAOI::CheckAirPressureIsEnough()
{
    if(ttbInsp)
    {
        ttbInsp->CheckAirPressureIsEnough();
    }
    return true;
}
//---------------------------------------------------------------------------
bool SaveAOISummaryReport()
{
//    bool bflag;
    TStringList *slReport = new TStringList();
    AnsiString S,aFileName,aLocalFile, asAOIReport;
//    int i, j, iBinCT, iType, iLastTestMode=0;
    int iPass=0, iFail=0, iSum=0;
    AnsiString s1,s2,s3,s4,s5,sStartTime;
//    int iPos1, iPos2;
    int iPos1;
//    double dYield=0.0, dSum=0.0;
    AnsiString SYeild;
    AnsiString ansAOI_Path="D:\\HT9045_Log\\AOI";
    slReport->Clear();

    TStringList *File;
    File=new TStringList();
    File->Clear();
    aFileName.sprintf("AOI_%s.csv",fLotInfo->edtSysLotID->Text);
    asAOIReport=ansAOI_Path+"\\"+aFileName;

    if(!DirectoryExists(ansAOI_Path))                                            //Eastsun 20260309 沒有資料夾 會導致執行緒崩潰問題
    {
        CreateDir(ansAOI_Path);
    }

    if(FileExists(asAOIReport))
    {
        File->LoadFromFile(asAOIReport);
        iPass=0;
        iFail=0;
        sStartTime=File->Strings[0];
        for(int i=1; i<File->Count; i++)
        {
            S=File->Strings[i];
            iPos1=S.Pos(",");

            s1=S.SubString(1, iPos1-1);

            if(s1=="Pass")
                iPass++;
            else
                iFail++;
        }
        iSum=iPass+iFail;
    }

    if(iTestRunMode==FT)                                                        //Eastsun 20260319 : 依照各模式 輸出名稱
    {
        aFileName.sprintf( "FT_AOI_Inspection_Summary_%04d%02d%02d%02d%02d%02d.txt",
                            SystemYear, SystemMonth, SystemDate,SystemHour, SystemMin,SystemSec);
    }
    else if(iTestRunMode==RT)
    {
        aFileName.sprintf( "RT_AOI_Inspection_Summary_%04d%02d%02d%02d%02d%02d.txt",
                            SystemYear, SystemMonth, SystemDate,SystemHour, SystemMin,SystemSec);
    }
    else if(iTestRunMode==OffT) //ChungHung 20141002 add for KYEC AutoRetest  //ChungHung 20111110 add
    {
        aFileName.sprintf( "OffT_AOI_Inspection_Summary_%04d%02d%02d%02d%02d%02d.txt",
                            SystemYear, SystemMonth, SystemDate,SystemHour, SystemMin,SystemSec);
    }
    else if(iTestRunMode==FT_ART) //ChungHung 20141002 add for KYEC AutoRetest
    {
        aFileName.sprintf( "FT_ART_ART_AOI_Inspection_Summary_%04d%02d%02d%02d%02d%02d.txt",
                            SystemYear, SystemMonth, SystemDate,SystemHour, SystemMin,SystemSec); //ChungHung 20141002 add for KYEC AutoRetest
    }
    else if(iTestRunMode==RT_ART) //ChungHung 20141002 add for KYEC AutoRetest
    {
        aFileName.sprintf( "RT_AOI_Inspection_Summary_%04d%02d%02d%02d%02d%02d.txt",
                            SystemYear, SystemMonth, SystemDate,SystemHour, SystemMin,SystemSec);
    }
    else if(iTestRunMode==FT_MRT) //Ifor 20170316 (wei) add MRT Mode
    {
        aFileName.sprintf( "FT_MRT_AOI_Inspection_Summary_%04d%02d%02d%02d%02d%02d.txt",
                            SystemYear, SystemMonth, SystemDate,SystemHour, SystemMin,SystemSec);
    }
    else if(iTestRunMode==RT_MRT) //Ifor 20170316 (wei) add MRT Mode
    {
        aFileName.sprintf( "RT_MRT_AOI_Inspection_Summary_%04d%02d%02d%02d%02d%02d.txt",
                            SystemYear, SystemMonth, SystemDate,SystemHour, SystemMin,SystemSec);
    }
    else
    {
        aFileName.sprintf( "AOI_Inspection_Summary_%04d%02d%02d%02d%02d%02d.txt",
                            SystemYear, SystemMonth, SystemDate,SystemHour, SystemMin,SystemSec);
    }

    aLocalFile = "D:\\HT9045_Log\\AOI\\"+aFileName;

    slReport->Add("                           AOI Inspection System");
    slReport->Add("                            Lot Summary Report");
    slReport->Add("");
    slReport->Add("");
    slReport->Add("");
    S.sprintf("Lot ID       : %s",fLotInfo->edtSysLotID->Text);
    slReport->Add(S);
    if(asSavedRunModeForAOIReport!="")                                          //Eastsun 20260515 F023: E1 use saved RunMode
        S.sprintf("RunMode      : %s",asSavedRunModeForAOIReport);
    else
        S.sprintf("RunMode      : %s",fLotInfo->cbRunMode->Text);
    slReport->Add(S);
    S.sprintf("Operator ID  : %s",fLotInfo->edtSysOperatorID->Text);
    slReport->Add(S);
    S.sprintf("Machine ID   : %s",IniConfig.SocketHandlerID);
    slReport->Add(S);
    S.sprintf("Start Time   : %s",sStartTime);
    slReport->Add(S);
    RunInfo.LotEndTime.sprintf("%04d-%02d-%02d %02d:%02d:%02d",
                                SystemYear, SystemMonth, SystemDate,SystemHour,SystemMin,SystemSec);
    S.sprintf("End Time     : %s",RunInfo.LotEndTime);
    slReport->Add(S);

    S.sprintf("======================================================================================================");
    slReport->Add(S);
    slReport->Add("");
    slReport->Add("Summary");
    slReport->Add("------------------------");
    S.sprintf("Pass                               :%d",iPass);
    slReport->Add(S);
    S.sprintf("Fail                               :%d",iFail);
    slReport->Add(S);
    S.sprintf("Total                              :%d",iSum);
    slReport->Add(S);

    if(iSum>0 && iPass>0)
    {
        SYeild=FloatToStr((double)iPass/(double)iSum*100.0);
        SYeild.sprintf("%0.2f%", atof(SYeild.c_str()));
        S.sprintf("Pass Yield                           : %s", SYeild);
    }
    else
    {
        S.sprintf("Pass Yield                           :0.00%");
    }
    slReport->Add(S);
    slReport->Add("");

    S.sprintf("======================================================================================================");
    slReport->Add(S);
    S.sprintf("  No.   AOI Pass/Fail                        Fail Details                     Test Bin");
    slReport->Add(S);
    File->Clear();
    if(FileExists(asAOIReport))
    {
        File->LoadFromFile(asAOIReport);
        for(int i=1; i<File->Count; i++)
        {
            S=File->Strings[i];

            iPos1=S.Pos(",");
            s1=S.SubString(1, iPos1-1);

            S=S.Delete(1, iPos1);

            iPos1=S.Pos(",");
            s2=S.SubString(1, iPos1-1);

            S=S.Delete(1, iPos1);

            iPos1=S.Pos(",");
            s3=S.SubString(1, iPos1-1);

            S=S.Delete(1, iPos1);

            iPos1=S.Pos(",");
            s4=S.SubString(1, iPos1-1);

            S=S.Delete(1, iPos1);
            s5=S.SubString(1, S.Length());

            S.sprintf("%04d%12s%55s%12s",i, s1, s4, s3);
            slReport->Add(S);
        }
    }

    slReport->SaveToFile(aLocalFile);
    delete slReport;
    delete File;
    return true;                                                                //W8070 function should return a value
}
//---------------------------------------------------------------------------

void __fastcall TFrmAOI::btSaveReportClick(TObject *Sender)
{
    SaveAOISummaryReport();
    iAOILotCount=0;
}
//---------------------------------------------------------------------------
void TTopBottomInspect::AddAOIRecord(AnsiString asPass, AnsiString asArea, int iwhichAuto, AnsiString asErrorCode, int iNo)
{
    bool bHasFile=false;

    AnsiString S, aFileName, aLocalFile;
    TStringList *SList;
    FILE *WriteAoiLOG;
    aFileName.sprintf("AOI_%s.csv",fLotInfo->edtSysLotID->Text);

    aLocalFile="D:\\HT9045_Log\\AOI";
    if(!(DirectoryExists(aLocalFile)))
        ForceDirectories(aLocalFile);

    aLocalFile="D:\\HT9045_Log\\AOI\\"+aFileName;
    asSavedRunModeForAOIReport=fLotInfo->cbRunMode->Text; //Eastsun 20260515 F023: E1 save RunMode before TrayEnd

    bHasFile=FileExists(aLocalFile);

    if(bHasFile==false)
    {
        S.sprintf("%s", RunInfo.LotStartTime);
        SList=new TStringList();
        SList->Clear();
        SList->Add(S);
        SList->SaveToFile(aLocalFile);
        SList->Clear();
        delete SList;
    }

    S.sprintf("%s,%s,%d,%s,%d", asPass, asArea, iwhichAuto, asErrorCode, iNo);

    WriteAoiLOG=fopen(aLocalFile.c_str(), "a+");
    if(WriteAoiLOG!=NULL)
    {
        try
        {
            fprintf(WriteAoiLOG, "%s \n", S);
        }
        catch(...)
        {
            MyDBIProcess("Exception", "AOI Log");
        }
        fclose(WriteAoiLOG);
    }
    WriteAoiLOG=NULL;
}
//---------------------------------------------------------------------------
bool TFrmAOI::bCheckAOIFailBinUse()
{
    if(FrmAOI->ttbInsp==NULL) return false;                                     //AI(general) 20260411 (RogerYang) : fix null pointer when TopBottom not installed
    return FrmAOI->ttbInsp->bAOIFailBin &&
       FrmAOI->RunTopBottomInspect()==true && FrmAOI->ttbInsp->iAction==1;
}
//---------------------------------------------------------------------------
#pragma warn -8080
void TFrmAOI::AOIFailCountAddRow(AnsiString runMode, int RowCount)              //Eastsun 20260515
{
    int marginLeft = 10;
    int itemWidth  = 300;

    const int pWidth       = itemWidth - 10;                        // 340
    const int cbWidth      = itemWidth - 20;                        // 330
    const int cbLeft       = (pWidth - cbWidth) / 2;               // 5

    const int lblCountW    = 50;
    const int edWidth      = 60;
    const int lblPcsW      = 30;
    const int rowGap       = 5;
    const int totalRowW    = lblCountW + rowGap + edWidth + rowGap + lblPcsW;  // 150
    const int rowStartX    = ((pWidth - totalRowW) / 2) +40;             // 95
    const int edTop        = 26;
    const int lblCountLeft = rowStartX;
    const int edLeft       = rowStartX + lblCountW + rowGap;
    const int lblPcsLeft   = edLeft + edWidth + rowGap;
    const int lblRowTop    = edTop + 4;

    // ===== struct =====
    //TAOIFailCountComboItem item = AOIFailList[RowCount];
    AOIFailList[RowCount].asRunMode = runMode;

    // ===== Item Panel =====
    //item.paItem          = new TPanel(this);
    TPanel *itemPanel      = new TPanel(this);
    itemPanel->Parent      = scroAOIFailCountLinkLotRunMode;
    itemPanel->Left        = marginLeft + RowCount * (itemWidth + 10);
    itemPanel->Top         = 10;
    itemPanel->Width       = itemWidth;
    itemPanel->Height      = 280;
    itemPanel->BevelOuter  = bvNone;
    itemPanel->BorderStyle = bsSingle;
    itemPanel->Color       = clWhite;

    if(runMode=="CORR")
        itemPanel->Visible = false;

    // ========================= macro 輔助 =========================
    // 每個 sub-panel 設 Align=alTop 後會依序往下疊，不需手動算 Top

    // ========================= 4. 累計NG =========================
    TPanel *p4     = new TPanel(this);
    p4->Height     = 55;
    p4->BevelOuter = bvNone;
    p4->Align      = alTop;
    p4->Parent     = itemPanel;

    //item.cbAccumulatedFail             = new TCheckBox(this);
    AOIFailList[RowCount].cbAccumulatedFail->Caption    = "Enable Accumulated Fail Check(Total)";
    AOIFailList[RowCount].cbAccumulatedFail->Left       = cbLeft;
    AOIFailList[RowCount].cbAccumulatedFail->Top        = 4;
    AOIFailList[RowCount].cbAccumulatedFail->Width      = cbWidth;
    AOIFailList[RowCount].cbAccumulatedFail->Font->Size = 11;
    AOIFailList[RowCount].cbAccumulatedFail->Parent     = p4;

    TLabel *lbCnt4    = new TLabel(this);
    lbCnt4->Caption   = "Count";
    lbCnt4->Left      = lblCountLeft;
    lbCnt4->Top       = lblRowTop;
    lbCnt4->Width     = lblCountW;
    lbCnt4->Alignment = taRightJustify;
    lbCnt4->Parent    = p4;

    //item.edAccumulatedFailCount         = new TEdit(this);
    AOIFailList[RowCount].edAccumulatedFailCount->Left   = edLeft;
    AOIFailList[RowCount].edAccumulatedFailCount->Top    = edTop;
    AOIFailList[RowCount].edAccumulatedFailCount->Width  = edWidth;
    AOIFailList[RowCount].edAccumulatedFailCount->Text   = "0";
    AOIFailList[RowCount].edAccumulatedFailCount->Parent = p4;
    SetWindowLong(AOIFailList[RowCount].edAccumulatedFailCount->Handle, GWL_STYLE,
        GetWindowLong(AOIFailList[RowCount].edAccumulatedFailCount->Handle, GWL_STYLE) | ES_CENTER);

    TLabel *lbPcs4  = new TLabel(this);
    lbPcs4->Caption = "Pcs";
    lbPcs4->Left    = lblPcsLeft;
    lbPcs4->Top     = lblRowTop;
    lbPcs4->Width   = lblPcsW;
    lbPcs4->Parent  = p4;

    TPanel *pp4     = new TPanel(this);
    pp4->Height     = 2;
    pp4->BevelOuter = bvNone;
    pp4->Align      = alTop;
    pp4->Parent     = itemPanel;
    pp4->Color       = clWhite;
    // ========================= 3. 連續拍照 =========================
    TPanel *p3     = new TPanel(this);
    p3->Height     = 65;   // ? 稍微加高，讓兩行文字不擠
    p3->BevelOuter = bvNone;
    p3->Align      = alTop;
    p3->Parent     = itemPanel;
    // ===== CheckBox（只保留勾）=====
    //item.cbConsecutiveFailPicture          = new TCheckBox(this);
    AOIFailList[RowCount].cbConsecutiveFailPicture->Caption = "";   // ? 不用文字
    AOIFailList[RowCount].cbConsecutiveFailPicture->Left    = cbLeft;
    AOIFailList[RowCount].cbConsecutiveFailPicture->Top     = 10;
    AOIFailList[RowCount].cbConsecutiveFailPicture->Width   = 15;
    AOIFailList[RowCount].cbConsecutiveFailPicture->Parent  = p3;

    // ===== Label（負責顯示文字 + 換行）=====
    TLabel *lbl3        = new TLabel(this);
    lbl3->Parent        = p3;
    lbl3->Caption       = "Enable Consecutive Fail Check\r\n(The Same Picture)";
    lbl3->Left          = cbLeft + 20;   // ? 接在 checkbox 右邊
    lbl3->Top           = 10;
    lbl3->Width         = cbWidth+150;
    lbl3->Height = 40;
    lbl3->WordWrap      = true;
    lbl3->AutoSize      = true;
    lbl3->Font->Size    = 11;
    lbl3->Font->Color   = clBlack;    // 黑
    lbl3->Tag = (int)AOIFailList[RowCount].cbConsecutiveFailPicture;
    lbl3->OnClick = AOIConsecutiveFailPictureLabel_Click;
    // ===== Count Label =====
    TLabel *lbCnt3    = new TLabel(this);
    lbCnt3->Caption   = "Count";
    lbCnt3->Left      = lblCountLeft;
    lbCnt3->Top       = lblRowTop;
    lbCnt3->Width     = lblCountW;
    lbCnt3->Alignment = taRightJustify;
    lbCnt3->Parent    = p3;
    // ===== Edit =====
    //item.edConsecutiveFailPictureCount         = new TEdit(this);
    AOIFailList[RowCount].edConsecutiveFailPictureCount->Left   = edLeft;
    AOIFailList[RowCount].edConsecutiveFailPictureCount->Top    = edTop;
    AOIFailList[RowCount].edConsecutiveFailPictureCount->Width  = edWidth;
    AOIFailList[RowCount].edConsecutiveFailPictureCount->Text   = "0";
    AOIFailList[RowCount].edConsecutiveFailPictureCount->Parent = p3;

    SetWindowLong(AOIFailList[RowCount].edConsecutiveFailPictureCount->Handle, GWL_STYLE,
        GetWindowLong(AOIFailList[RowCount].edConsecutiveFailPictureCount->Handle, GWL_STYLE) | ES_CENTER);

    // ===== Pcs Label =====
    TLabel *lbPcs3  = new TLabel(this);
    lbPcs3->Caption = "Pcs";
    lbPcs3->Left    = lblPcsLeft;
    lbPcs3->Top     = lblRowTop;
    lbPcs3->Width   = lblPcsW;
    lbPcs3->Parent  = p3;
    // ===== 分隔線 =====
    TPanel *pp3     = new TPanel(this);
    pp3->Height     = 2;
    pp3->BevelOuter = bvNone;
    pp3->Align      = alTop;
    pp3->Parent     = itemPanel;
    pp3->Color      = clWhite;
    // ========================= 2. 間隔檢查 =========================
    TPanel *p2     = new TPanel(this);
    p2->Height     = 55;
    p2->BevelOuter = bvNone;
    p2->Align      = alTop;
    p2->Parent     = itemPanel;

    //item.cbIntervalCheck             = new TCheckBox(this);
    AOIFailList[RowCount].cbIntervalCheck->Caption    = "Enable Interval Check";
    AOIFailList[RowCount].cbIntervalCheck->Left       = cbLeft;
    AOIFailList[RowCount].cbIntervalCheck->Top        = 4;
    AOIFailList[RowCount].cbIntervalCheck->Width      = cbWidth;
    AOIFailList[RowCount].cbIntervalCheck->Font->Size = 11;
    AOIFailList[RowCount].cbIntervalCheck->Parent     = p2;

//    TLabel *lbCnt2    = new TLabel(this);
//    lbCnt2->Caption   = "Count";
//    lbCnt2->Left      = lblCountLeft;
//    lbCnt2->Top       = lblRowTop;
//    lbCnt2->Width     = lblCountW;
//    lbCnt2->Alignment = taRightJustify;
//    lbCnt2->Parent    = p2;

    //item.edIntervalCount         = new TEdit(this);
    AOIFailList[RowCount].edIntervalCount->Left   = edLeft;
    AOIFailList[RowCount].edIntervalCount->Top    = edTop;
    AOIFailList[RowCount].edIntervalCount->Width  = edWidth;
    AOIFailList[RowCount].edIntervalCount->Text   = "0";
    AOIFailList[RowCount].edIntervalCount->Parent = p2;
    SetWindowLong(AOIFailList[RowCount].edIntervalCount->Handle, GWL_STYLE,
        GetWindowLong(AOIFailList[RowCount].edIntervalCount->Handle, GWL_STYLE) | ES_CENTER);

//    TLabel *lbPcs2  = new TLabel(this);
//    lbPcs2->Caption = "Pcs";
//    lbPcs2->Left    = lblPcsLeft;
//    lbPcs2->Top     = lblRowTop;
//    lbPcs2->Width   = lblPcsW;
//    lbPcs2->Parent  = p2;

    TPanel *pp2     = new TPanel(this);
    pp2->Height     = 2;
    pp2->BevelOuter = bvNone;
    pp2->Align      = alTop;
    pp2->Parent     = itemPanel;
    pp2->Color       = clWhite;
    // ========================= 1. 連續NG =========================
    TPanel *p1     = new TPanel(this);
    p1->Height     = 55;
    p1->BevelOuter = bvNone;
    p1->Align      = alTop;
    p1->Parent     = itemPanel;

    //item.cbConsecutiveFail             = new TCheckBox(this);
    AOIFailList[RowCount].cbConsecutiveFail->Caption    = "Enable Consecutive Fail Check";
    AOIFailList[RowCount].cbConsecutiveFail->Left       = cbLeft;
    AOIFailList[RowCount].cbConsecutiveFail->Top        = 4;
    AOIFailList[RowCount].cbConsecutiveFail->Width      = cbWidth;
    AOIFailList[RowCount].cbConsecutiveFail->Font->Size = 11;
    AOIFailList[RowCount].cbConsecutiveFail->Parent     = p1;

    TLabel *lbCnt1    = new TLabel(this);
    lbCnt1->Caption   = "Count";
    lbCnt1->Left      = lblCountLeft;
    lbCnt1->Top       = lblRowTop;
    lbCnt1->Width     = lblCountW;
    lbCnt1->Alignment = taRightJustify;
    lbCnt1->Parent    = p1;

    //item.edConsecutiveFailCount         = new TEdit(this);
    AOIFailList[RowCount].edConsecutiveFailCount->Left   = edLeft;
    AOIFailList[RowCount].edConsecutiveFailCount->Top    = edTop;
    AOIFailList[RowCount].edConsecutiveFailCount->Width  = edWidth;
    AOIFailList[RowCount].edConsecutiveFailCount->Text   = "0";
    AOIFailList[RowCount].edConsecutiveFailCount->Parent = p1;
    SetWindowLong(AOIFailList[RowCount].edConsecutiveFailCount->Handle, GWL_STYLE,
        GetWindowLong(AOIFailList[RowCount].edConsecutiveFailCount->Handle, GWL_STYLE) | ES_CENTER);

    TLabel *lbPcs1  = new TLabel(this);
    lbPcs1->Caption = "Pcs";
    lbPcs1->Left    = lblPcsLeft;
    lbPcs1->Top     = lblRowTop;
    lbPcs1->Width   = lblPcsW;
    lbPcs1->Parent  = p1;

    TPanel *pp1     = new TPanel(this);
    pp1->Height     = 2;
    pp1->BevelOuter = bvNone;
    pp1->Align      = alTop;
    pp1->Parent     = itemPanel;
    pp1->Color       = clWhite;
    // ========================= runMode 大標題 =========================
    TLabel *lblRunMode    = new TLabel(this);
    lblRunMode->Parent    = itemPanel;
    lblRunMode->Caption   = runMode;
    lblRunMode->Left      = 0;
    lblRunMode->Top       = 0;
    lblRunMode->Width     = itemWidth;
    lblRunMode->Height    = 30;
    lblRunMode->Align     = alTop;          // 固定在最頂
    lblRunMode->Alignment = taCenter;
    lblRunMode->Layout    = tlCenter;
    lblRunMode->Font->Size  = 14;
    lblRunMode->Font->Style = TFontStyles() << fsBold;
}
#pragma warn .8080
//---------------------------------------------------------------------------
void TFrmAOI::AOIFailCountRefresh()                                             //Eastsun 20260402 : AOI check fail 功能Refresh
{
    for(int i = 0; i < (int)FrmAOI->AOIFailList.size(); i++)                    //W8012 signed/unsigned
    {
        FrmAOI->AOIFailList[i].cbConsecutiveFail->Parent = NULL;
        FrmAOI->AOIFailList[i].edConsecutiveFailCount->Parent = NULL;
        FrmAOI->AOIFailList[i].cbIntervalCheck->Parent = NULL;
        FrmAOI->AOIFailList[i].edIntervalCount->Parent = NULL;
        FrmAOI->AOIFailList[i].cbConsecutiveFailPicture->Parent = NULL;
        FrmAOI->AOIFailList[i].edConsecutiveFailPictureCount->Parent = NULL;
        FrmAOI->AOIFailList[i].cbAccumulatedFail->Parent = NULL;
        FrmAOI->AOIFailList[i].edAccumulatedFailCount->Parent = NULL;
    }
    for(int i = scroAOIFailCountLinkLotRunMode->ControlCount - 1; i >= 0; i--)
    {
        delete scroAOIFailCountLinkLotRunMode->Controls[i];
    }
    for(int i = 0; i < fLotInfo->cbRunMode->Items->Count && i<iAOIFailCheckMax; i++)
    {
        AnsiString text = fLotInfo->cbRunMode->Items->Strings[i];
        AOIFailCountAddRow(text,i);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmAOI::AOIConsecutiveFailPictureLabel_Click(TObject *Sender)  //Eastsun 20260402 : AOI check fail
{
    TLabel *lbl = (TLabel*)Sender;
    TWinControl *parent = lbl->Parent;   // 就是 p3

    // 掃 p3 的子元件，找第一個 TCheckBox
    for(int i = 0; i < parent->ControlCount; i++)
    {
        TCheckBox *cb = dynamic_cast<TCheckBox*>(parent->Controls[i]);
        if(cb != NULL)
        {
            cb->Checked = !cb->Checked;
            break;
        }
    }
}

