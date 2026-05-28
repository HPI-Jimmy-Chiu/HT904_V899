#include "MachineDefine.h"
#pragma hdrstop

#include "OCR.h"

#include "mymessbox.h"
#include "cmydef.h"
#include "note.h"
#include "uLotInfo.h"
#include "cprod.h"
#include "main.h"
#include "uLotInfo.h"
#include "cMyDB.h"
#include "common.h"
#include "mymotor.h"
#include "myQwertyKeyBoard.h"
#include "InputForm.h"
#include "MessageDef.h"
#include "TextProcess.h"
#include "OCRInsp.h"
#include "mycylin.h"
#include "CosFunction.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfOCR *fOCR;
AnsiString _STX_;
AnsiString _ETX_;
//AI(ht9045-v899) 20260505: keep OCR simulation state local to OCR form and restore production tray data after test
static bool bOCRSimRunning=false;
static bool bOCRSimBackupReady=false;
static int  iOCRSimLoopNow=0;
static int  iOCRSimLoopTotal=1;
static int  iOCRSimBackupMMOCRData[_MAX_COL_ITEM][_MAX_ROW_ITEM];
static AnsiString asOCRSimBackupDeviceInf[_MAX_COL_ITEM][_MAX_ROW_ITEM];
static int  iOCRSimBackupNewOCRCount[10];
static int  iOCRSimBackupXItem=1;
static int  iOCRSimBackupYItem=1;
static int  iOCRSimBackupFlow=1;
static int  iOCRSimBackupPosC=0;
static int  iOCRSimBackupPosR=0;
static int  iOCRSimBackupBarCodeErrorC=0;
static int  iOCRSimBackupBarCodeErrorR=0;
static int  iOCRSimBackupOCRCheck=0;
static int  iOCRSimBackupOCRLotTrayCount=0;
static bool bOCRSimBackupMMOCRHasTray=false;
static bool bOCRSimBackupBarCodeError=false;
static bool bOCRSimBackupOCRNoIC=false;
static bool bOCRSimBackupInspectError=false;
static bool bOCRSimBackupOCRReadOK=false;
static bool bOCRSimBackupOCRStartPosition=false;
static bool bOCRSimBackupRunOcrInsp=false;
static bool bOCRSimBackupOCRCheckFlag=false;
static bool bOCRSimBackupOCRCheckNG=false;
static AnsiString asOCRSimBackupBarCode="";
bool bOCRSimBypassAOI=false;                                                    //AI(ht9045-v899) 20260511: runtime bypass flag read by OCRInsp.cpp DoOCRFlow1; non-static so OCRInsp can link to it. Default off; only set true while OCR Area Simulation runs with the Bypass AOI checkbox ticked
//AI(ht9045-v899) 20260522: simulation-only flow log state; production OCR keeps this disabled
bool bOCRSimLogEnable=false;
static int iOCRSimLogLoopNow=0;
static int iOCRSimLogLoopTotal=0;
static AnsiString asOCRSimLogFile="";
//AI(ht9045-v899) 20260522: define OCR capture trigger modes without changing existing switch-trigger default
const int OCR_TRIGGER_MODE_SWITCH=0;
const int OCR_TRIGGER_MODE_COMMAND=1;
//---------------------------------------------------------------------------
//const unsigned char T_HEX2ASCII[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
//unsigned char T_HEX2ASCII_Mac(unsigned char hex2ascii)
//{
//    return(T_HEX2ASCII[(hex2ascii)&0x0f]);
//}
//---------------------------------------------------------------------------
__fastcall TfOCR::TfOCR(TComponent* Owner)
    : TForm(Owner)
{
    int i;
    _STX_.sprintf("%c", 0x02);
    _ETX_.sprintf("%c", 0x03);
    sTesterLotId="";
    TesterReceiveBuffer="";
    bHaveLotID=false;
    bSendOCRGetLotID=true;
    for(i=0; i<TESTER_MAX_CMD; i++)
    {
        sTester_Send[i]="";
        sTester_Recv[i]="";
        bTester_ReceiveOK[i]=false;
    }

    OCRReceiveBuffer        = "";
    _OcrCommType            = eocrUseMessage;
    _ImgPath                = "D:\\HontechOCR\\ShareImage\\";
    _iOcrChangeFileTask     = 1;
    bSendOcrSendStart       = true;
    for(i=0; i<OCR_MAX_CMD; i++)
    {
        sOCR_Send[i]=""; //ChungHung 20120830 add OCR Function
        sOCR_Recv[i]="";
        bOcr_ReceiveOK[i]=false;
    }

    //送資料---------------------------------------
    sTester_Send[testerLotID      ].sprintf("%c%s%c\r\n", 0x02, "LOTID?", 0x03);
    sTester_Send[testerLotIDOK    ].sprintf("%c%s%c\r\n", 0x02, "OK", 0x03);
    sTester_Send[testerLotIDNG    ].sprintf("%c%s%c\r\n", 0x02, "NG", 0x03);
    sTester_Send[testerLotEnd     ].sprintf("%c%s%c\r\n", 0x02, "LOTEND", 0x03);
    sTester_Send[testerProgram    ].sprintf("%c%s%c\r\n", 0x02, "PGM:", 0x03);
    sTester_Send[testerProgramOK  ].sprintf("%c%s%c", 0x02, "OK", 0x03);
    sTester_Send[testerProgramNG  ].sprintf("%c%s%c", 0x02, "NG", 0x03);

    //收資料---------------------------------------
    sTester_Recv[testerLotID      ].sprintf("%c%s", 0x02, "LOTID:");
    sTester_Recv[testerLotIDOK    ].sprintf("%c%s%c\r\n", 0x02, "OK", 0x03);
    sTester_Recv[testerLotIDNG    ].sprintf("%c%s%c\r\n", 0x02, "NG", 0x03);
    sTester_Recv[testerLotEnd     ].sprintf("%c%s%c\r\n", 0x02, "LOTEND", 0x03);
    sTester_Recv[testerProgram    ].sprintf("%c%s", 0x02, "PGM:");
    sTester_Recv[testerProgramOK  ].sprintf("%c%s%c", 0x02, "OK", 0x03);
    sTester_Recv[testerProgramNG  ].sprintf("%c%s%c", 0x02, "NG", 0x03);

    //送資料---------------------------------------
    sOCR_Send[ocrVisionOn           ]="#VISIONON+";                             //詢問系統上線
    sOCR_Send[ocrVisionOff          ]="#VISIONOFF+";
    sOCR_Send[ocrStartOk            ]="#START+";                                //Handler通知啟動檢測
    //sOCR_Send[ocrStartNoGolden      ]="#STARTNOGORDEN+";                      //ChungHung 20120830 add OCR Function delete
    //sOCR_Send[ocrStartNoRoi         ]="#STARTNOROI+";                         //ChungHung 20120830 add OCR Function delete
    //sOCR_Send[ocrStartNoMatch       ]="#STARTNOMATCH+";                       //ChungHung 20120830 add OCR Function delete
    //sOCR_Send[ocrStartRepeat        ]="#STARTREPEAT+";                        //ChungHung 20120830 add OCR Function delete
    sOCR_Send[ocrStartNoReady       ]="#STARTNOREADY+";

    sOCR_Send[ocrRecipeOk           ]="#RECIPE+";                               //ChungHung 20120830 add OCR Function add
    sOCR_Send[ocrRecipeNoModel      ]="#RECIPENOMODEL+";                        //ChungHung 20120830 add OCR Function add
    sOCR_Send[ocrRecipeNoROI        ]="#RECIPENOROI+";                          //ChungHung 20120830 add OCR Function add
    sOCR_Send[ocrRecipeNoFont       ]="#RECIPENOFONT+";                         //ChungHung 20120830 add OCR Function add
    sOCR_Send[ocrRecipeNoLotID      ]="#RECIPENOLOTID+";

    sOCR_Send[ocrEnd                ]="#END+";                                  //Handler通知關閉檢測
    sOCR_Send[ocrFileOk             ]="#FILE+";                                 //Handler通知換料檔名
    sOCR_Send[ocrClear              ]="#CLEAR+";                                //Handler通知結批完成
    //sOCR_Send[ocrRelease            ]="#RELEASE+";                            //Handler通知狀況排除, Vision關閉ErrorMessage      //ChungHung 20120830 add OCR Function delete
    sOCR_Send[ocrInspOk             ]="#INSP+";                                 //Handler送出比對命令與字串
    sOCR_Send[ocrInspNg             ]="#INSPNG+";
    //sOCR_Send[ocrInspErr            ]="#INSPERR+";                            //ChungHung 20120830 add OCR Function delete
    //sOCR_Send[ocrGrabTimeOut        ]="#GRABTIMEOUT+";                        //ChungHung 20120830 add OCR Function delete
    //sOCR_Send[ocrReadErr            ]="#READERR+";                            //ChungHung 20120830 add OCR Function delete
    sOCR_Send[ocrInspErr2001        ]="#INSPERR2001+";                          //ChungHung 20120830 add OCR Function add
    sOCR_Send[ocrInspErr2002        ]="#INSPERR2002+";                          //ChungHung 20120830 add OCR Function add
    sOCR_Send[ocrInspErr2003        ]="#INSPERR2003+";                          //ChungHung 20120830 add OCR Function add
    sOCR_Send[ocrInspErr2004        ]="#INSPERR2004+";                          //ChungHung 20120830 add OCR Function add
    sOCR_Send[ocrInspErr3001        ]="#INSPERR3001+";                          //ChungHung 20120830 add OCR Function add
    sOCR_Send[ocrInspErr3002        ]="#INSPERR3002+";                          //ChungHung 20120830 add OCR Function add

    sOCR_Send[ocrTrainOK            ]="#TRAIN+";                                //ChungHung 20120830 add OCR Function add
    sOCR_Send[ocrTrainNG            ]="#TRAIN+";                                //ChungHung 20120830 add OCR Function add

    sOCR_Send[ocrLot                ]="#LOT+";                                  //ChungHung 20120830 add OCR Function add

    //收資料---------------------------------------
    sOCR_Recv[ocrVisionOn           ]="#VISIONON+";                             //系統上線回應
    sOCR_Recv[ocrVisionOff          ]="#VISIONOFF+";                            //系統下線通知
    sOCR_Recv[ocrStartOk            ]="#STARTOK+";                              //回應可以正常啟動
    //sOCR_Recv[ocrStartNoGolden      ]="#STARTNOGORDEN+";                      //回應未建立Golden Sample    //ChungHung 20120830 add OCR Function delete
    //sOCR_Recv[ocrStartNoRoi         ]="#STARTNOROI+";                         //回應未建立ROI              //ChungHung 20120830 add OCR Function delete
    //sOCR_Recv[ocrStartNoMatch       ]="#STARTNOMATCH+";                       //回應未建立Match Sample     //ChungHung 20120830 add OCR Function delete
    //sOCR_Recv[ocrStartRepeat        ]="#STARTREPEAT+";                        //Start訊號連續送來不及反應  //ChungHung 20120830 add OCR Function delete
    sOCR_Recv[ocrStartNoReady       ]="#STARTNOREADY+";                         //Vision處於無法工作的狀態

    sOCR_Recv[ocrRecipeOk           ]="#RECIPEOK+";                             //ChungHung 20120830 add OCR Function add
    sOCR_Recv[ocrRecipeNoModel      ]="#RECIPENOMODEL+";                        //ChungHung 20120830 add OCR Function add
    sOCR_Recv[ocrRecipeNoROI        ]="#RECIPENOROI+";                          //ChungHung 20120830 add OCR Function add
    sOCR_Recv[ocrRecipeNoFont       ]="#RECIPENOFONT+";                         //ChungHung 20120830 add OCR Function add
    sOCR_Recv[ocrRecipeNoLotID      ]="#RECIPENOLOTID+";

    sOCR_Recv[ocrEnd                ]="#END+";                                  //Vision進入IDLE狀態
    sOCR_Recv[ocrFileOk             ]="#FILEOK+";                               //Vision建立料號完成回傳
    sOCR_Recv[ocrClear              ]="#CLEAR+";                                //Vision製作檢測報表
    //sOCR_Recv[ocrRelease            ]="#RELEASE+";                            //解除Error Message, 進入關閉檢測功能狀態    //ChungHung 20120830 add OCR Function delete
    sOCR_Recv[ocrInspOk             ]="#INSPOK+";                               //Vision回傳比對結果OK
    sOCR_Recv[ocrInspNg             ]="#INSPNG+";                               //Vision回傳比對結果異常
    //sOCR_Recv[ocrInspErr            ]="#INSPERR+";                            //分析異常        //ChungHung 20120830 add OCR Function delete
    //sOCR_Recv[ocrGrabTimeOut        ]="#GRABTIMEOUT+";                        //取相延遲        //ChungHung 20120830 add OCR Function delete
    //sOCR_Recv[ocrReadErr            ]="#READERR+";                            //讀取檔案錯誤    //ChungHung 20120830 add OCR Function delete

    sOCR_Recv[ocrInspErr2001        ]="#INSPERR2001+";                          //No source image  //ChungHung 20120830 add OCR Function add
    sOCR_Recv[ocrInspErr2002        ]="#INSPERR2002+";                          //No model found   //ChungHung 20120830 add OCR Function add
    sOCR_Recv[ocrInspErr2003        ]="#INSPERR2003+";                          //Char database can not use //ChungHung 20120830 add OCR Function add
    sOCR_Recv[ocrInspErr2004        ]="#INSPERR2004+";                          //Alignment error  //ChungHung 20120830 add OCR Function add
    sOCR_Recv[ocrInspErr3001        ]="#INSPERR3001+";                          //Grab timeout     //ChungHung 20120830 add OCR Function add
    sOCR_Recv[ocrInspErr3002        ]="#INSPERR3002+";                          //Readimage error  //ChungHung 20120830 add OCR Function add

    sOCR_Recv[ocrTrainOK            ]="#TRAINOK+";                              //ChungHung 20120830 add OCR Function add
    sOCR_Recv[ocrTrainNG            ]="#TRAINNG";                               //ChungHung 20120830 add OCR Function add

    sOCR_Recv[ocrLot                ]="#LOTOK+";                                //ChungHung 20120830 add OCR Function add

    ListOCRByLot=new TStringList;
    ListOCRDataLog=new TStringList;                                             //KenHsieh 20230406 : 新增OCR Data + Bin Log功能

    tsOCRSim=NULL;
    gbOCRSim=NULL;
    lbOCRSimInfo=NULL;
    lbOCRSimLoopCaption=NULL;
    lbOCRSimStatus=NULL;
    edOCRSimLoopTimes=NULL;
    btnOCRSimRun=NULL;
    btnOCRSimStop=NULL;
    tmrOCRSim=NULL;
    cbOCRSimBypassAOI=NULL;                                                     //AI(ht9045-v899) 20260511: init bypass-AOI checkbox handle
    bOCRSimUiCreated=false;
    bShow=false;
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::FormShow(TObject *Sender)
{
    bShow=true;
    CreateOCRSimulationControls();                                             //AI(ht9045-v899) 20260505: add OCR simulation UI after dfm controls are loaded
    PageControl1->ActivePageIndex=2;                                            //tsOCR_Cognex_Setting
    fOCR_ReadFile();
    fOCR_DoIniDataToForm();
    ShowOCRImg();
    ShowMatchImg();
    ckDisabledKeyin->Visible  =(CUSTOMER_CODE==CC_KYEC_LEE || IniConfig.bSPILFunction);
    cbCheckBarCodeMap->Visible=(CUSTOMER_CODE==CC_KYEC_LEE);
    btSend->BevelInner=bvRaised;
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::FormClose(TObject *Sender, TCloseAction &Action)
{
    StopOCRSimulation("Stopped (Form Close)");                                //AI(ht9045-v899) 20260505: restore tray data if simulation is closed mid-flow
    bShow=false;
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::CreateOCRSimulationControls()
{
    if(bOCRSimUiCreated)
        return;

    //AI(ht9045-v899) 20260505: put simulation UI on its own tab so it never overlaps existing Light Setup / OCR Setup controls
    tsOCRSim=new TTabSheet(this);
    tsOCRSim->PageControl=PageControl1;
    tsOCRSim->Caption="OCR Simulation";

    gbOCRSim=new TGroupBox(this);
    gbOCRSim->Parent=tsOCRSim;
    gbOCRSim->Left=12;
    gbOCRSim->Top=12;
    gbOCRSim->Width=460;
    gbOCRSim->Height=160;
    gbOCRSim->Caption="OCR Area Simulation";

    lbOCRSimInfo=new TLabel(this);
    lbOCRSimInfo->Parent=gbOCRSim;
    lbOCRSimInfo->Left=12;
    lbOCRSimInfo->Top=24;
    lbOCRSimInfo->Width=380;
    lbOCRSimInfo->Caption="Source: Tray Form full area";

    lbOCRSimLoopCaption=new TLabel(this);
    lbOCRSimLoopCaption->Parent=gbOCRSim;
    lbOCRSimLoopCaption->Left=12;
    lbOCRSimLoopCaption->Top=54;
    lbOCRSimLoopCaption->Width=80;
    lbOCRSimLoopCaption->Caption="Loop Times";

    edOCRSimLoopTimes=new TEdit(this);
    edOCRSimLoopTimes->Parent=gbOCRSim;
    edOCRSimLoopTimes->Left=100;
    edOCRSimLoopTimes->Top=50;
    edOCRSimLoopTimes->Width=45;
    edOCRSimLoopTimes->Height=24;
    edOCRSimLoopTimes->Text="1";

    btnOCRSimRun=new TButton(this);
    btnOCRSimRun->Parent=gbOCRSim;
    btnOCRSimRun->Left=160;
    btnOCRSimRun->Top=48;
    btnOCRSimRun->Width=110;
    btnOCRSimRun->Height=28;
    btnOCRSimRun->Caption="Run OCR Sim";
    btnOCRSimRun->OnClick=btnOCRSimRunClick;

    btnOCRSimStop=new TButton(this);
    btnOCRSimStop->Parent=gbOCRSim;
    btnOCRSimStop->Left=282;
    btnOCRSimStop->Top=48;
    btnOCRSimStop->Width=110;
    btnOCRSimStop->Height=28;
    btnOCRSimStop->Caption="Stop";
    btnOCRSimStop->Enabled=false;
    btnOCRSimStop->OnClick=btnOCRSimStopClick;

    lbOCRSimStatus=new TLabel(this);
    lbOCRSimStatus->Parent=gbOCRSim;
    lbOCRSimStatus->Left=12;
    lbOCRSimStatus->Top=92;
    lbOCRSimStatus->Width=380;
    lbOCRSimStatus->Caption="Idle";

    tmrOCRSim=new TTimer(this);
    tmrOCRSim->Enabled=false;
    tmrOCRSim->Interval=20;
    tmrOCRSim->OnTimer=tmrOCRSimTimer;

    //AI(ht9045-v899) 20260511: "Bypass AOI" checkbox lets simulation drive only handler motion when the vision PC is not online (skips bOCRConnect / WAR09113 and fakes barcode result locally)
    cbOCRSimBypassAOI=new TCheckBox(this);
    cbOCRSimBypassAOI->Parent=gbOCRSim;
    cbOCRSimBypassAOI->Left=12;
    cbOCRSimBypassAOI->Top=120;
    cbOCRSimBypassAOI->Width=200;
    cbOCRSimBypassAOI->Caption="Bypass AOI (no vision PC)";
    cbOCRSimBypassAOI->Checked=false;

    //AI(ht9045-v899) 20260506: stray ckWordCount on form root bleeds across every PageControl tab; reparent it back to OCR Setup tab
    if(ckWordCount!=NULL && ckWordCount->Parent!=tsOCR_Cognex_Setting)
        ckWordCount->Parent=tsOCR_Cognex_Setting;

    bOCRSimUiCreated=true;
}
//---------------------------------------------------------------------------
static void BackupOCRSimulationData()
{
    iOCRSimBackupXItem=MOT[MMOCR].Tray.XItem;
    iOCRSimBackupYItem=MOT[MMOCR].Tray.YItem;
    bOCRSimBackupMMOCRHasTray=MOT[MMOCR].fHasTray;
    bOCRSimBackupBarCodeError=bBarCodeError;
    bOCRSimBackupOCRNoIC=bOCRNoIC;
    bOCRSimBackupInspectError=bInspectError;
    bOCRSimBackupOCRReadOK=bOCRReadOK;
    bOCRSimBackupOCRStartPosition=bOCRStartPosition;
    bOCRSimBackupRunOcrInsp=bRunOcrInsp;
    iOCRSimBackupFlow=iOCRFlow;
    iOCRSimBackupPosC=iOCRPosC;
    iOCRSimBackupPosR=iOCRPosR;
    iOCRSimBackupBarCodeErrorC=iBarCodeErrorC;
    iOCRSimBackupBarCodeErrorR=iBarCodeErrorR;
    iOCRSimBackupOCRCheck=iOCRCheck;
    iOCRSimBackupOCRLotTrayCount=iOCRLotTrayCount;
    bOCRSimBackupOCRCheckFlag=bOCRCheck;
    bOCRSimBackupOCRCheckNG=bOCRCheckNG;
    asOCRSimBackupBarCode=asBarCode;

    for(int i=0; i<10; i++)
        iOCRSimBackupNewOCRCount[i]=iNewOCRCount[i];

    for(int iC=0; iC<_MAX_COL_ITEM; iC++)
    {
        for(int iR=0; iR<_MAX_ROW_ITEM; iR++)
        {
            iOCRSimBackupMMOCRData[iC][iR]=MOT[MMOCR].Tray.Data[iC][iR];
            asOCRSimBackupDeviceInf[iC][iR]=MOT[MMTrayY].Tray.cDeviceInf[iC][iR];
        }
    }
    bOCRSimBackupReady=true;
}
//---------------------------------------------------------------------------
static void RestoreOCRSimulationData()
{
    if(bOCRSimBackupReady==false)
        return;

    MOT[MMOCR].Tray.SetXYItem(iOCRSimBackupXItem, iOCRSimBackupYItem);
    MOT[MMOCR].fHasTray=bOCRSimBackupMMOCRHasTray;

    for(int iC=0; iC<_MAX_COL_ITEM; iC++)
    {
        for(int iR=0; iR<_MAX_ROW_ITEM; iR++)
        {
            MOT[MMOCR].Tray.Data[iC][iR]=iOCRSimBackupMMOCRData[iC][iR];
            MOT[MMTrayY].Tray.cDeviceInf[iC][iR]=asOCRSimBackupDeviceInf[iC][iR];
        }
    }

    for(int i=0; i<10; i++)
        iNewOCRCount[i]=iOCRSimBackupNewOCRCount[i];

    bBarCodeError=bOCRSimBackupBarCodeError;
    bOCRNoIC=bOCRSimBackupOCRNoIC;
    bInspectError=bOCRSimBackupInspectError;
    bOCRReadOK=bOCRSimBackupOCRReadOK;
    bOCRStartPosition=bOCRSimBackupOCRStartPosition;
    bRunOcrInsp=bOCRSimBackupRunOcrInsp;
    iOCRFlow=iOCRSimBackupFlow;
    iOCRPosC=iOCRSimBackupPosC;
    iOCRPosR=iOCRSimBackupPosR;
    iBarCodeErrorC=iOCRSimBackupBarCodeErrorC;
    iBarCodeErrorR=iOCRSimBackupBarCodeErrorR;
    iOCRCheck=iOCRSimBackupOCRCheck;
    iOCRLotTrayCount=iOCRSimBackupOCRLotTrayCount;
    bOCRCheck=bOCRSimBackupOCRCheckFlag;
    bOCRCheckNG=bOCRSimBackupOCRCheckNG;
    asBarCode=asOCRSimBackupBarCode;
    MOT[MMOCR].Refresh();
}
//---------------------------------------------------------------------------
static bool PrepareOCRSimulationTrayFromTrayForm()
{
    int iXItem=MOT[MMTrayY].Tray.XItem;
    int iYItem=MOT[MMTrayY].Tray.YItem;

    if(iXItem<=0 || iYItem<=0 || iXItem>_MAX_COL_ITEM || iYItem>_MAX_ROW_ITEM)
        return false;

    MOT[MMOCR].Tray.SetXYItem(iXItem, iYItem);
    MOT[MMOCR].fHasTray=true;

    for(int iC=0; iC<iXItem; iC++)
    {
        for(int iR=0; iR<iYItem; iR++)
        {
            MOT[MMOCR].SetTraySingleData(iC, iR, HAS_IC);
        }
    }

    MOT[MMOCR].Refresh();
    return true;
}
//---------------------------------------------------------------------------
void TfOCR::UpdateOCRSimulationStatus(AnsiString asStatus)
{
    if(lbOCRSimStatus!=NULL)
        lbOCRSimStatus->Caption=asStatus;
}
//---------------------------------------------------------------------------
//AI(ht9045-v899) 20260522: build one fixed current-loop log path so every simulation loop starts from a clean file
static AnsiString BuildOCRSimulationLogFileName()
{
    AnsiString asDir, asFile;
    asDir.sprintf("D:\\HT9045_Log\\OCRSimulation\\%04d_%02d_%02d", SystemYear, SystemMonth, SystemDate);
    MyForceDirectories(asDir, "BuildOCRSimulationLogFileName");
    asFile.sprintf("%s\\CurrentLoop.txt", asDir.c_str());
    return asFile;
}
//---------------------------------------------------------------------------
void OCRSimLogClear(int iLoopNow, int iLoopTotal)
{
    AnsiString asLine, asTime;

    if(bOCRSimLogEnable==false)
        return;

    iOCRSimLogLoopNow=iLoopNow;
    iOCRSimLogLoopTotal=iLoopTotal;
    asOCRSimLogFile=BuildOCRSimulationLogFileName();

    asTime=Now().FormatString("yyyymmddhhmmss");
    asLine.sprintf("%s Loop %d/%d Start", asTime.c_str(), iOCRSimLogLoopNow, iOCRSimLogLoopTotal);
    WriteDataToFile(asOCRSimLogFile, asLine, true);
}
//---------------------------------------------------------------------------
void OCRSimLogWrite(const char* cMessage)
{
    AnsiString asLine, asTime;

    if(bOCRSimLogEnable==false || asOCRSimLogFile=="" || cMessage==NULL)
        return;

    asTime=Now().FormatString("yyyymmddhhmmss");
    asLine.sprintf("%s Loop %d/%d %s", asTime.c_str(), iOCRSimLogLoopNow, iOCRSimLogLoopTotal, cMessage);
    WriteDataToFile(asOCRSimLogFile, asLine);
}
//---------------------------------------------------------------------------
void TfOCR::StopOCRSimulation(AnsiString asStatus, bool bRestoreData)
{
    if(tmrOCRSim!=NULL)
        tmrOCRSim->Enabled=false;

    //AI(ht9045-v899) 20260522: write final simulation status before disabling simulation-only logging
    if(bOCRSimLogEnable)
    {
        AnsiString asLog;
        asLog.sprintf("Simulation stop: %s", asStatus.c_str());
        OCRSimLogWrite(asLog.c_str());
    }

    if(bOCRSimRunning || bOCRSimBackupReady)
    {
        SW[SwOCRTigger].Off();
        Cylinder[C_OCRLight_Up].Off();
        if(bRestoreData)
            RestoreOCRSimulationData();
    }

    bOCRSimRunning=false;
    bOCRSimBackupReady=false;
    bOCRSimBypassAOI=false;                                                     //AI(ht9045-v899) 20260511: clear runtime bypass flag whenever simulation stops, so production OCR flow always uses real AOI socket
    bOCRSimLogEnable=false;

    if(btnOCRSimRun!=NULL)
    {
        btnOCRSimRun->Enabled=true;
        btnOCRSimRun->Caption="Run OCR Sim";
    }
    if(btnOCRSimStop!=NULL)
        btnOCRSimStop->Enabled=false;

    UpdateOCRSimulationStatus(asStatus);
}
//---------------------------------------------------------------------------
void TfOCR::WriteOcrInfoToMemo(AnsiString asInfo)                               //工作訊息寫入memo
{
    TDateTime myDateTime=Now();
    if(memoOcrCommand->Lines->Count>4096)
        memoOcrCommand->Clear();

    memoOcrCommand->Lines->Add(myDateTime.TimeString()+" "+asInfo);
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::btSendClick(TObject *Sender)
{   //手動測試送訊號
    switch(rgCommadn->ItemIndex)
    {
        case 0:
            SendOCR(ocrVisionOn);
            break;
        case 1:
            SendOCR(ocrStartOk);
            break;
        case 2:
            SendOCR(ocrEnd);
            break;
        case 3:
            sOCR_Send[ocrFileOk]="#FILE"+edFileName->Text+"+";
            SendOCR(ocrFileOk);
            break;
        case 4:
            SendOCR(ocrClear);
            break;
        case 5:
            //SetOcrInspText(edInsp->Text);
            SendOCR(ocrInspOk);
            break;
    }
}
//---------------------------------------------------------------------------
void TfOCR::SendOCR(OCRComIndex OcrIndex, bool bSetToFalse)
{

    WriteOcrInfoToMemo("SEND: "+sOCR_Send[OcrIndex]);
    #ifdef HT9045
        MyDBIProcess("Process", "SEND: "+sOCR_Send[OcrIndex]);
    #endif

    if(bSetToFalse)
        bOcr_ReceiveOK[OcrIndex]=false;

    if(_OcrCommType==eocrUseMessage)
    {

    }
    else if(_OcrCommType==eocrUseRS23)
    {
        CommOcr->WriteCommData(sOCR_Send[OcrIndex].c_str(), sOCR_Send[OcrIndex].Length());
    }
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::InitialOCRCom(AnsiString ComPort, TParity Parity, unsigned Rate, TByteSize ByteSize, bool ParityCheck, TStopBits StopBits)
{
    CommOcr->CommName       = "\\\\.\\"+ComPort;
    CommOcr->Parity         = Parity;
    CommOcr->BaudRate       = Rate;
    CommOcr->ByteSize       = ByteSize;
    CommOcr->ParityCheck    = ParityCheck;
    CommOcr->StopBits       = StopBits;
    if(GetCOMPortStatus(ComPort))
    {
        try
        {
            CommOcr->StartComm();  //僅能啟動一次
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TfOCR::InitialOCRCom");
            #ifdef HT9045
                ShowMyMessage("OCR : "+ComPort+" port error", "");
            #else
                ShowMyMessage("OCR : "+ComPort+" port error");
            #endif
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::InitialOCRCwithTesterCom(AnsiString ComPort, TParity Parity, unsigned Rate, TByteSize ByteSize, bool ParityCheck, TStopBits StopBits)
{
    CommOcr_Tester->CommName       = "\\\\.\\"+ComPort;
    CommOcr_Tester->Parity         = Parity;
    CommOcr_Tester->BaudRate       = Rate;
    CommOcr_Tester->ByteSize       = ByteSize;
    CommOcr_Tester->ParityCheck    = ParityCheck;
    CommOcr_Tester->StopBits       = StopBits;
    if(GetCOMPortStatus(ComPort))
    {
        try
        {
            CommOcr_Tester->StartComm();  //僅能啟動一次
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TfOCR::InitialOCRCwithTesterCom");
            #ifdef HT9045
                ShowMyMessage("OCRwithTester : "+ComPort+" port error", "");
            #else
                ShowMyMessage("OCRwithTester : "+ComPort+" port error");
            #endif
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::CommOcrReceiveData(TObject *Sender, Pointer Buffer,
      WORD BufferLength)
{
    if(InitialOK==false)    //Steven 20120202 : 加入Thread保護
        return;

    if(BufferLength==0)
        return;

    char temp[1024]={'\0'};
    //JimmyChiu 20210922 Avoid overflow
    //==>
    if(BufferLength>1024)
    {
        LogMsg.sprintf("Receive (%d): %s", BufferLength, Buffer);
        fMain->AddTorqueLog(LogMsg);
        fMain->slTorqueLog->AddTextWithDateTime(LogMsg);
        return;
    }
    //<==
    //JimmyChiu 20210922 Avoid overflow
    memcpy(temp, Buffer, BufferLength);
    OCRReceiveBuffer = OCRReceiveBuffer + AnsiString(temp);
    WriteOcrInfoToMemo("DEBG: "+OCRReceiveBuffer);
    ProcessOcrCommand();
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::ProcessOcrCommand()
{
    int iStart,iiStart,iEnd;
//    int i, j, ret;
    OCRComIndex OcrIndex;

    for(OcrIndex=ocrVisionOn; OcrIndex<OCR_MAX_CMD; OcrIndex=(OCRComIndex)(OcrIndex+1))
    {
        iStart=OCRReceiveBuffer.Pos(sOCR_Recv[OcrIndex]);

        if(iStart==0)
        {
            iStart=OCRReceiveBuffer.Pos("#INSPOK");
            if(iStart!=0)
                OcrIndex=ocrInspOk;
            else if(iStart==0)
            {
                iStart=OCRReceiveBuffer.Pos("#INSPNG");
                if(iStart!=0)
                    OcrIndex=ocrInspNg;
            }
        }
        if(iStart!=0)
        {
            switch(OcrIndex)
            {
                case ocrVisionOn:
                    bOcr_ReceiveOK[ocrVisionOn]=true;
                    sOCR_Send[ocrFileOk]="#FILE"+edFileName->Text+"+";          //ChungHung 20130624 add 收到VisionOn 先送File Name
                    SendOCR(ocrFileOk);
                    //SetOcrFileName();
                    #ifdef HT9045
                        MyDBIProcess("Process", "RECV:OCR Vision On.");
                    #endif
                    break;
                case ocrVisionOff:
                    bOcr_ReceiveOK[ocrVisionOff]=true;
                    ShowMyMessage("OCR Program Close,Please Check", "OCR 程式關閉請確認");
                    InitialOCRPara();

                    #ifdef HT9045
                        MyDBIProcess("Process", "RECV:OCR Vision Off.");
                    #endif
                    break;
                case ocrStartOk:
//                    bSendOcrSendStart=true;
                    bOcr_ReceiveOK[ocrStartOk]=true;
                    #ifdef HT9045
                        MyDBIProcess("Process", "RECV:OCR Start OK.");
                    #endif
                    break;
                //case ocrStartNoGolden:   //ChungHung 20120830 add OCR Function delete
                //    bOcr_ReceiveOK[ocrStartNoGolden]=true;
                //    #ifdef HT9045
                //        ShowErrorMessage("WAR0931", 0, MMTrayY);   //"OCR no golsen sample error!!"
                //    #endif
                //    bSendOcrSendStart=true;
                //    break;
                //case ocrStartNoRoi:
                //    bOcr_ReceiveOK[ocrStartNoRoi]=true;
                //    #ifdef HT9045
                //        ShowErrorMessage("WAR0932", 0, MMTrayY);   //"OCR no ROI data error!!"
                //    #endif
                //    bSendOcrSendStart=true;
                //    break;
                //case ocrStartNoMatch:
                //    bOcr_ReceiveOK[ocrStartNoMatch]=true;
                //    #ifdef HT9045
                //        ShowErrorMessage("WAR0933", 0, MMTrayY);   //"OCR no Match data error!!"
                //    #endif
                //    bSendOcrSendStart=true;
                //    break;
                //case ocrStartRepeat:
                //    bOcr_ReceiveOK[ocrStartRepeat]=true;
                //    #ifdef HT9045
                //        MyDBIProcess("Process", "OCR Start Repeat");
                //    #endif
                //    bSendOcrSendStart=false;
                //    break;
                case ocrStartNoReady:
                    bOcr_ReceiveOK[ocrStartNoReady]=true;
                    #ifdef HT9045
                        ShowErrorMessage("WAR0935", 0, MMTrayY);   //"OCR start not ready error!!"
                    #endif
                    #ifdef HT9045
                        MyDBIProcess("Process", "RECV:OCR Start No Ready.");
                    #endif
                    bSendOcrSendStart=true;
                    break;
                case ocrRecipeOk:      //ChungHung 20120830 add OCR Function add
                    bOcr_ReceiveOK[ocrRecipeOk]=true;
                    #ifdef HT9045
                        MyDBIProcess("Process", "RECV:OCR Recipe OK.");
                    #endif
                    break;
                case ocrRecipeNoModel:
                    bOcr_ReceiveOK[ocrRecipeNoModel]=true;
                    #ifdef HT9045
                        MyDBIProcess("Process", "RECV:OCR Recipe No Model.");
                    #endif
                    //#ifdef HT9045
                    //    ShowErrorMessage("WAR0933", 0, MMTrayY);   //"OCR no Model error!!"
                    //#endif
                    break;
                case ocrRecipeNoROI:
                    bOcr_ReceiveOK[ocrRecipeNoROI]=true;
                    #ifdef HT9045
                        MyDBIProcess("Process", "RECV:OCR Recipe No ROI.");
                    #endif
                    //#ifdef HT9045
                    //    ShowErrorMessage("WAR0932", 0, MMTrayY);   //"OCR no ROI data error!!"
                    //#endif
                    break;
                case ocrRecipeNoFont:
                    bOcr_ReceiveOK[ocrRecipeNoFont]=true;
                    #ifdef HT9045
                        MyDBIProcess("Process", "RECV:OCR Recipe No Font.");
                    #endif
                    //#ifdef HT9045
                    //    ShowErrorMessage("WAR0931", 0, MMTrayY);   //"OCR no ROI data error!!"
                    //#endif
                    break;             //ChungHung 20120830 add OCR Function end
                case ocrRecipeNoLotID:
                    bOcr_ReceiveOK[ocrRecipeNoLotID]=true;
                    #ifdef HT9045
                        MyDBIProcess("Process", "RECV:OCR Recipe No LOTID.");
                    #endif
                    break;
                case ocrEnd:
                    bOcr_ReceiveOK[ocrEnd]=true;
                    #ifdef HT9045
                        MyDBIProcess("Process", "RECV:OCR End.");
                    #endif
                    bSendOcrSendStart=true;
                    break;
                case ocrFileOk:
                    bOcr_ReceiveOK[ocrFileOk]=true;
                    #ifdef HT9045
                        MyDBIProcess("Process", "RECV:OCR File OK.");
                    #endif
                    break;
                case ocrClear:
                    bSendOcrSendStart=true;
                    bOcr_ReceiveOK[ocrClear]=true;
                    #ifdef HT9045
                        MyDBIProcess("Process", "RECV:OCR Clear.");
                    #endif
                    break;
                //case ocrRelease:                      //ChungHung 20120830 add OCR Function delete
                //    bOcr_ReceiveOK[ocrRelease]=true;
                //    break;
                case ocrInspOk:
                    bOcr_ReceiveOK[ocrInspOk]=true;
                    iiStart=iStart+7;
                    iEnd=OCRReceiveBuffer.Pos("+");
                    fLotInfo->lblOCR_LotID->Caption=OCRReceiveBuffer.SubString(iiStart, iEnd-iiStart);
                    fLotInfo->lblOCR_LotID->Refresh();
                    OCRReceiveBuffer.Delete(iiStart, iEnd-iiStart);
                    fLotInfo->Image1->Picture->LoadFromFile(OCRINSPNG);
                    #ifdef HT9045
                        MyDBIProcess("Process", "RECV:OCR Insp OK.");
                    #endif
                    break;
                case ocrInspNg:
                    bOcr_ReceiveOK[ocrInspNg]=true;
                    iiStart=iStart+7;
                    iEnd=OCRReceiveBuffer.Pos("+");
                    fLotInfo->lblOCR_LotID->Caption=OCRReceiveBuffer.SubString(iiStart, iEnd-iiStart);
                    fLotInfo->lblOCR_LotID->Refresh();
                    OCRReceiveBuffer.Delete(iiStart, iEnd-iiStart);
                    fLotInfo->Image1->Picture->LoadFromFile(OCRINSPNG);
                    #ifdef HT9045
                        MyDBIProcess("Process", "RECV:OCR Insp NG.");
                    #endif
//                    #ifdef HT9045
//                        ShowErrorMessage("WAR0940", 0, MMTrayY);   //"OCR inspection NG!!"
//                    #endif
                    break;
                //case ocrInspErr:                         //ChungHung 20120830 add OCR Function delete
                //    bOcr_ReceiveOK[ocrInspErr]=true;
                //    bSendOcrSendStart=true;
                //    break;
                //case ocrGrabTimeOut:                     //ChungHung 20120830 add OCR Function delete
                //    bOcr_ReceiveOK[ocrGrabTimeOut]=true;
                //    bSendOcrSendStart=true;
                //    break;
                //case ocrReadErr:                         //ChungHung 20120830 add OCR Function delete
                //    bOcr_ReceiveOK[ocrReadErr]=true;
                //    bSendOcrSendStart=true;
                //    break;
                case ocrInspErr2001:                       //ChungHung 20120830 add OCR Function add
                    bOcr_ReceiveOK[ocrInspErr2001]=true;
                    #ifdef HT9045
                        MyDBIProcess("Process", "RECV:OCR Insp Err:No source image.");
                    #endif
                    break;
                case ocrInspErr2002:
                    bOcr_ReceiveOK[ocrInspErr2002]=true;
                    #ifdef HT9045
                        MyDBIProcess("Process", "RECV:OCR Insp Err:No model found.");
                    #endif
                    break;
                case ocrInspErr2003:
                    bOcr_ReceiveOK[ocrInspErr2003]=true;
                    #ifdef HT9045
                        MyDBIProcess("Process", "RECV:OCR Insp Err:Char database can not use.");
                    #endif
                    break;
                case ocrInspErr2004:
                     bOcr_ReceiveOK[ocrInspErr2004]=true;
                     #ifdef HT9045
                        MyDBIProcess("Process", "RECV:OCR Insp Err:Alignment error.");
                    #endif
                     break;
                case ocrInspErr3001:
                    bOcr_ReceiveOK[ocrInspErr3001]=true;
                    #ifdef HT9045
                        MyDBIProcess("Process", "RECV:OCR Insp Grab timeout.");
                    #endif
                    break;
                case ocrInspErr3002:
                    bOcr_ReceiveOK[ocrInspErr3002]=true;
                    #ifdef HT9045
                        MyDBIProcess("Process", "RECV:OCR Insp Read image error.");
                    #endif
                    break;
                case ocrTrainOK:
                    bOcr_ReceiveOK[ocrTrainOK]=true;
                    #ifdef HT9045
                        MyDBIProcess("Process", "RECV:OCR Train OK.");
                    #endif
                    break;
                case ocrTrainNG:
                    bOcr_ReceiveOK[ocrTrainNG]=true;
                    #ifdef HT9045
                        MyDBIProcess("Process", "RECV:OCR Train NG.");
                    #endif
                    break;
                case ocrLot:
                    bOcr_ReceiveOK[ocrLot]=true;
                    #ifdef HT9045
                        MyDBIProcess("Process", "RECV:OCR LOT OK.");
                    #endif
                    break;
                    //ChungHung 20120830 add OCR Function end
            }
            WriteOcrInfoToMemo("RECV: "+sOCR_Recv[OcrIndex]);
            OCRReceiveBuffer.Delete(iStart, iStart+sOCR_Recv[OcrIndex].Length());
        }
    }
}
//---------------------------------------------------------------------------
void TfOCR::ShowOCRImg()
{
    if(FileExists(_ImgPath+"OCR.bmp"))
    {
        imgOCR->Picture->LoadFromFile(_ImgPath+"OCR.bmp");
    }
    else
    {
        imgOCR->Canvas->Font->Color=clRed;
        imgOCR->Canvas->Font->Size=20;
        imgOCR->Canvas->Font->Name="Arial";
        imgOCR->Canvas->TextOutA(10, 10, "No Image!!");
    }
}
//---------------------------------------------------------------------------
void TfOCR::ShowMatchImg()
{
    if(FileExists(_ImgPath+"Match.bmp"))
    {
        imgMatch->Picture->LoadFromFile(_ImgPath+"Match.bmp");
    }
    else
    {
        imgMatch->Canvas->Font->Color=clRed;
        imgMatch->Canvas->Font->Size=20;
        imgOCR->Canvas->Font->Name="Arial";
        imgMatch->Canvas->TextOutA(10, 10, "No Image!!");
    }
}
//---------------------------------------------------------------------------
TQPF_Timer hVisionOnDelay;
void __fastcall TfOCR::TimerOcrOnTimer(TObject *Sender)
{
    if(InitialOK==false)
        return;

    /*
    static int Task=1;
    if(cbEnableOcr->Checked)
    {
        switch(Task)
        {
            case 1:
                SendOCR(ocrVisionOn);
                hVisionOnDelay.Set(10);
                hVisionOnDelay.On();
                break;
            case 10:
                if(bOcr_ReceiveOK[ocrVisionOn]==true)
                {
                    Task=1;
                }
                else
                {
                    if(hVisionOnDelay.Off())
                    {
                         #ifdef HT9045
                            ShowErrorMessage("WAR0930", 0, MMTrayY);   //"OCR Vision program off error!!"
                         #else
                            ShowMyMessage("OCR Vision program off error!!");
                         #endif
                         Task=1;
                    }
                }
                break;

        }
    }
    */

    static int count=0;   //只有開啟時確認
    if(cbEnableOcr->Checked)
    {
        /*if(bOcr_ReceiveOK[ocrVisionOn]==false)
        {
            count++;
            if(count>10)
            {
                #ifdef HT9045
                    ShowErrorMessage("WAR0930", 0, MMTrayY);   //"OCR Vision program off error!!"
                #else
                    ShowMyMessage("OCR Vision program off error!!");
                #endif
                count=0;
                TimerOcrOn->Enabled=false;
            }
        }
        else
        {
            count=0;
            TimerOcrOn->Enabled=false;
        }                               */
    }
    else
    {
        count=0;
        TimerOcrOn->Enabled=false;
    }
}
//---------------------------------------------------------------------------
void TfOCR::SetOcrFileName(AnsiString Name)
{
    if(cbEnableOcr->Checked && InitialOK==true)
    {
        if(TimerOcrChangeFile->Enabled==false)
        {
            edFileName->Text            = Name;
            sOCR_Send[ocrFileOk]        = "#FILE"+edFileName->Text+"+";
            _iOcrChangeFileTask         = 1;
            TimerOcrChangeFile->Enabled = true;
        }
    }
}
//---------------------------------------------------------------------------
void TfOCR::SetOcrInspText(AnsiString Name)
{
    edInsp->Text=Name;
    sOCR_Send[ocrInspOk]="#INSP"+edInsp->Text+"+";
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::InitialOCRPara(bool bSendRealCCDStart)
{
    int index;
    bSendOcrSendStart=bSendRealCCDStart;

    for(index=ocrVisionOn; index<OCR_MAX_CMD; index++)
    {
        if(bSendRealCCDStart==false)
            continue;
        bOcr_ReceiveOK[index]=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::DoOCRReleaseAndInspEnd()
{
    //SendOCR(ocrRelease);
    //MySleep(100);
    SendOCR(ocrEnd);
    InitialOCRPara();
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::btSendMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    btSend->BevelInner=bvLowered;
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::btSendMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    btSend->BevelInner=bvRaised;
}
//---------------------------------------------------------------------------
enum eOcrChangeFileTask
{
    wodInitial=1,
    wodEndTask
};
//---------------------------------------------------------------------------
void __fastcall TfOCR::TimerOcrChangeFileTimer(TObject *Sender)
{
    if(InitialOK==false)
        return;

    AnsiString str;
    int &Task=_iOcrChangeFileTask;
    switch(Task)
    {
        case wodInitial:
            SendOCR(ocrFileOk);
            Task=wodEndTask;
            break;
        case wodEndTask:
            SendOCR(ocrEnd);
            TimerOcrChangeFile->Enabled=false;
            break;
    }
}
//---------------------------------------------------------------------------
void TfOCR::InitInsp()
{
    bOcr_ReceiveOK[ocrInspOk        ]=false;
    bOcr_ReceiveOK[ocrInspNg        ]=false;
    bOcr_ReceiveOK[ocrInspErr2001        ]=false;
    bOcr_ReceiveOK[ocrInspErr2002        ]=false;
    bOcr_ReceiveOK[ocrInspErr2003        ]=false;
    bOcr_ReceiveOK[ocrInspErr2004        ]=false;
    bOcr_ReceiveOK[ocrInspErr3001        ]=false;
    bOcr_ReceiveOK[ocrInspErr3002        ]=false;

    //bOcr_ReceiveOK[ocrInspErr       ]=false;         //ChungHung 20120830 add OCR Function delete
    //bOcr_ReceiveOK[ocrGrabTimeOut   ]=false;         //ChungHung 20120830 add OCR Function delete
    //bOcr_ReceiveOK[ocrReadErr       ]=false;         //ChungHung 20120830 add OCR Function delete
}
//---------------------------------------------------------------------------
// 檢測有無錯誤處理
// -1: 處理中, 0 : 處理完成, K_RETRY, K_SKIP
//---------------------------------------------------------------------------
int __fastcall TfOCR::CheckInspectionResult()
{
    int ret=-1; //預設還沒完成檢知

    if(bOcr_ReceiveOK[fOCR->ocrInspOk])
    {
        ret=0;
    }
    else if(bOcr_ReceiveOK[ocrInspNg]==true)
    {
        #ifdef HT9045
            ret=ShowErrorMessage("WAR0940", K_RETRY|K_SKIP, MMTrayY);   //"OCR inspection NG!!"
        #else
            ShowMyMessage("OCR inspection NG!!");
            ret=K_RETRY;
        #endif
    }
    else if(bOcr_ReceiveOK[ocrInspErr2001])
    {
        #ifdef HT9045
            ret=ShowErrorMessage("WAR0940", K_RETRY|K_SKIP, MMTrayY);   //"OCR inspection NG!!"
        #else
            ShowMyMessage("OCR inspection no Source image!!");
            ret=K_RETRY;
        #endif
    }else if(bOcr_ReceiveOK[ocrInspErr2002])
    {
        #ifdef HT9045
            ret=ShowErrorMessage("WAR0940", K_RETRY|K_SKIP, MMTrayY);   //"OCR inspection NG!!"
        #else
            ShowMyMessage("OCR inspection no model found!!");
            ret=K_RETRY;
        #endif
    }else if(bOcr_ReceiveOK[ocrInspErr2003])
    {
        #ifdef HT9045
            ret=ShowErrorMessage("WAR0940", K_RETRY|K_SKIP, MMTrayY);   //"OCR inspection NG!!"
        #else
            ShowMyMessage("OCR inspection char database can not use!!");
            ret=K_RETRY;
        #endif
    }else if(bOcr_ReceiveOK[ocrInspErr3001])
    {
        #ifdef HT9045
            ret=ShowErrorMessage("WAR0940", K_RETRY|K_SKIP, MMTrayY);   //"OCR inspection NG!!"
        #else
            ShowMyMessage("OCR inspection grab time out!!");
            ret=K_RETRY;
        #endif
    }else if(bOcr_ReceiveOK[ocrInspErr3002])
    {
        #ifdef HT9045
            ret=ShowErrorMessage("WAR0940", K_RETRY|K_SKIP, MMTrayY);   //"OCR inspection NG!!"
        #else
            ShowMyMessage("OCR inspection read image error!!");
            ret=K_RETRY;
        #endif
    }

    //else if(bOcr_ReceiveOK[ocrInspErr]==true)        //ChungHung 20120830 add OCR Function delete
    //{
    //    #ifdef HT9045
    //        ret=ShowErrorMessage("WAR0941", K_RETRY|K_SKIP, MMTrayY);   //"OCR inspection analysis Error!!"
    //    #else
    //        ShowMyMessage("OCR inspection analysis Error!!");
    //        ret=K_RETRY;
    //    #endif
    //}
    //else if(bOcr_ReceiveOK[ocrGrabTimeOut]==true)    //ChungHung 20120830 add OCR Function delete
    //{
    //    #ifdef HT9045
    //        ret=ShowErrorMessage("WAR0942", K_RETRY|K_SKIP, MMTrayY);   //"OCR inspection grab time out!!"
    //    #else
    //        ShowMyMessage("OCR inspection grab time out!!");
    //        ret=K_RETRY;
    //    #endif
    //}
    //else if(bOcr_ReceiveOK[ocrReadErr]==true)        //ChungHung 20120830 add OCR Function delete
    //{
    //    #ifdef HT9045
    //        ret=ShowErrorMessage("WAR0943", K_RETRY|K_SKIP, MMTrayY);   //"OCR inspection read image error!!"
    //    #else
    //        ShowMyMessage("OCR inspection read image error!!");
    //        ret=K_RETRY;
    //    #endif
    //}
    //if(ret==K_RETRY || ret==K_SKIP)
    //    DoOCRReleaseAndInspEnd();

    return ret;
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::SendOcrStart()
{
    bOcr_ReceiveOK[ocrStartOk]          =false;
    //bOcr_ReceiveOK[ocrStartNoGolden]    =false;   //ChungHung 20120830 add OCR Function delete
    //bOcr_ReceiveOK[ocrStartNoRoi]       =false;   //ChungHung 20120830 add OCR Function delete
    //bOcr_ReceiveOK[ocrStartNoMatch]     =false;   //ChungHung 20120830 add OCR Function delete
    //bOcr_ReceiveOK[ocrStartRepeat]      =false;   //ChungHung 20120830 add OCR Function delete
    bOcr_ReceiveOK[ocrStartNoReady]     =false;
    bOcr_ReceiveOK[ocrEnd]=false;
    SendOCR(ocrStartOk);
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::FormDestroy(TObject *Sender)
{
    try
    {
        TimerOcrOn->Enabled=false;
        TimerOcrChangeFile->Enabled=false;
        CommOcr->StopComm();
        delete ListOCRByLot;
        delete ListOCRDataLog;                    //KenHsieh 20230406 : 新增OCR Data + Bin Log功能
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfOCR::FormDestroy");
    }
    LogSoftwareOffTime("TfOCR, FormDestroy"); //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::SendTester(int iTesterIndex)
{
    WriteOcrInfoToMemo("SEND Tester: "+sTester_Send[iTesterIndex]);
    #ifdef HT9045
        MyDBIProcess("Process", "SEND Tester: "+sTester_Send[iTesterIndex]);
    #endif
    fLotInfo->mmoHandlerWithTester->Lines->Add("SEND Tester: "+sTester_Send[iTesterIndex]);
    CommOcr_Tester->WriteCommData(sTester_Send[iTesterIndex].c_str(), sTester_Send[iTesterIndex].Length());
}
//---------------------------------------------------------------------------
int iOcrWithTesterTask=1;
void __fastcall TfOCR::InitOcrWithTester()
{
    _STX_.sprintf("%c", 0x02);
    _ETX_.sprintf("%c", 0x03);
    sTesterLotId="";
    TesterReceiveBuffer="";
    bSendOCRGetLotID=false; //ChungHung 20121009 add
    bHaveLotID=false;
    for(int i=0; i<TESTER_MAX_CMD; i++)
    {
        bTester_ReceiveOK[i]=false;
    }

    iOcrWithTesterTask=1;
    Timer1->Enabled=true;
}
//---------------------------------------------------------------------------
TQPF_Timer hOcrWithTester;
const int iOCRWithTesterDelay=50;
int __fastcall TfOCR::OcrWithTester() //0:no finish 1:OK 2:NG 3:PGM NG
{
    int &Task=iOcrWithTesterTask;
    AnsiString SendRECVLOTID;
    AnsiString SendRECVPGM;
    switch(Task)
    {
        case 1: //Send LOTID?
            fLotInfo->mmoHandlerWithTester->Clear();
            SendTester( testerLotID);
            hOcrWithTester.Set0_1SecAndOn(iOCRWithTesterDelay);
            //fLotInfo->mmoHandlerWithTester->Clear();

            #ifdef SOFT_SIMULTE
            bTester_ReceiveOK[testerLotID]=true;          //wei 20150713 模擬
            #endif

            Task=50;
        case 50: //Revice LOTID
            if(bTester_ReceiveOK[testerLotID])
            {
                //sTesterLotId;
                Task=100;
                return 0;
            }
            if(hOcrWithTester.Off())
            {
                //ShowAlarm
                ShowErrorMessage("WAR0950", K_RETRY|K_SKIP, MMTrayY);   //"OCR LOTID Time Out!!"
                //InitOcrWithTester();
                return 2;
            }
            break;
        case 100: //Send LOTID to Tester
            bTester_ReceiveOK[testerLotIDNG]=false;
            bTester_ReceiveOK[testerLotIDOK]=false;
            bTester_ReceiveOK[testerProgramOK]=false;
            bTester_ReceiveOK[testerProgramNG]=false;
            bTester_ReceiveOK[testerProgram]=false;

            #ifdef SOFT_SIMULTE
            bTester_ReceiveOK[testerProgram]=true;                 //wei 20150713 模擬
            #endif

            SendRECVLOTID.sprintf("%c%s%s%c\r\n", 0x02, "LOTID:", sTesterLotId, 0x03);
            WriteOcrInfoToMemo("SEND Tester: "+SendRECVLOTID);
            //#ifdef HT9045
                MyDBIProcess("Process", "SEND Tester: "+SendRECVLOTID);
            //#endif

            CommOcr_Tester->WriteCommData(SendRECVLOTID.c_str(), SendRECVLOTID.Length());
            fLotInfo->mmoHandlerWithTester->Lines->Add("SEND Tester: "+SendRECVLOTID);

            hOcrWithTester.Set0_1SecAndOn(iOCRWithTesterDelay);
            Task=200;
        case 200: // OK or NG
            if(bTester_ReceiveOK[testerProgram]) //Wait Receive Test Program then Send Test Program To Tester
            {
                edInsp->Text=sTesterLotId;
                bTester_ReceiveOK[testerLotIDNG]=false;
                bTester_ReceiveOK[testerLotIDOK]=false;
                bTester_ReceiveOK[testerProgramOK]=false;
                bTester_ReceiveOK[testerProgramNG]=false;

                #ifdef SOFT_SIMULTE
                bTester_ReceiveOK[testerLotIDOK]=true;                 //wei 20150713 模擬
                bTester_ReceiveOK[testerProgramOK]=true;
                #endif

                SendRECVPGM.sprintf("%c%s%s%c\r\n", 0x02, "PGM:", sTesterPGM, 0x03);
                WriteOcrInfoToMemo("SEND Tester: "+ SendRECVPGM);
                //#ifdef HT9045
                    MyDBIProcess("Process", "SEND Tester: "+SendRECVPGM);
                //#endif

                CommOcr_Tester->WriteCommData(SendRECVPGM.c_str(), SendRECVPGM.Length());
                fLotInfo->mmoHandlerWithTester->Lines->Add("SEND Tester PGM: "+SendRECVPGM);

                hOcrWithTester.Set0_1SecAndOn(iOCRWithTesterDelay);
                Task=400;
            }
            else if(bTester_ReceiveOK[testerLotIDNG] || bTester_ReceiveOK[testerProgramNG])
            {
                return 2;
            }
            if(hOcrWithTester.Off())
            {
                ShowErrorMessage("WAR0951", K_RETRY, MMTrayY);   //"OCR LOTID OK or NG Time Out!!"
                //InitOcrWithTester();
                Task=1;
                return 2;
            }
            break;
        case 400: //
            if(bTester_ReceiveOK[testerProgramOK] || bTester_ReceiveOK[testerLotIDOK])
            {
                return 1;
            }
            else if(bTester_ReceiveOK[testerProgramNG] || bTester_ReceiveOK[testerLotIDNG])
            {
                ShowErrorMessage("WAR0958", K_RETRY, MMTrayY);   //"OCR LOTID OK or NG Time Out!!"
                return 3;
            }
            if(hOcrWithTester.Off())
            {
                ShowErrorMessage("WAR0959", K_RETRY, MMTrayY);   //"OCR LOTID OK or NG Time Out!!"
                return 3;
            }
            break;
    }
    return 0;
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::CommOcr_TesterReceiveData(TObject *Sender,
      Pointer Buffer, WORD BufferLength)
{
    if(InitialOK==false)    //Steven 20120202 : 加入Thread保護
        return;

    if(BufferLength==0)
        return;

    char temp[1024]={'\0'};
    //JimmyChiu 20210922 Avoid overflow
    //==>
    if(BufferLength>1024)
    {
        LogMsg.sprintf("Receive (%d): %s", BufferLength, Buffer);
        fMain->AddTorqueLog(LogMsg);
        fMain->slTorqueLog->AddTextWithDateTime(LogMsg);
        return;
    }
    //<==
    //JimmyChiu 20210922 Avoid overflow
    memcpy(temp, Buffer, BufferLength);
    TesterReceiveBuffer = TesterReceiveBuffer + AnsiString(temp);
    //WriteOcrInfoToMemo("DEBG Tester: "+TesterReceiveBuffer);
    ProcessTesterCommand();
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::ProcessTesterCommand()
{
    int iStart;
    int iEnd;
//    int i, j, ret;
    TesterComIndex TesterIndex;

    for(TesterIndex=testerLotIDOK; TesterIndex<TESTER_MAX_CMD; TesterIndex=TesterComIndex(TesterIndex+1))
    {
        iStart=TesterReceiveBuffer.Pos(sTester_Recv[TesterIndex]);
        iEnd=TesterReceiveBuffer.Pos(_ETX_);

        if(iStart!=0 && iEnd!=0)
        {
            switch(TesterIndex)
            {
                case testerLotID:
                    bTester_ReceiveOK[testerLotID]=true;
                    iEnd = TesterReceiveBuffer.Pos(_ETX_);
                    sTesterLotId=TesterReceiveBuffer.SubString(iStart+sTester_Recv[testerLotID].Length(), iEnd-(iStart+sTester_Recv[testerLotID].Length()));
                    iEnd=iEnd+2;
                    //#ifdef HT9045
                        MyDBIProcess("Process", "RECV:Tester LOTID:"+sTesterLotId);
                    //#endif
                    break;
                case testerLotIDOK:
                    iEnd=iEnd+2;//TesterReceiveBuffer.Length();
                    bTester_ReceiveOK[testerLotIDOK]=true;
                    //#ifdef HT9045
                        MyDBIProcess("Process", "RECV:Tester LOTID:OK");
                    //#endif
                    break;
                case testerLotIDNG:
                    iEnd=iEnd+2;
                    //iEnd=TesterReceiveBuffer.Length();
                    bTester_ReceiveOK[testerLotIDNG]=true;
                    //#ifdef HT9045
                        MyDBIProcess("Process", "RECV:Tester LOTID:NG");
                    //#endif
                    break;
                case testerLotEnd:
                    iEnd=iEnd+2;
                    //iEnd=TesterReceiveBuffer.Length();
                    bTester_ReceiveOK[testerLotEnd]=true;
                    SendTester(testerLotEnd);
                    sTesterLotId="";
                    bHaveLotID=false;
                    fOCR->bSendOCRGetLotID=true;
                    //#ifdef HT9045
                        MyDBIProcess("Process", "RECV:Tester LOTID:END");
                    //#endif
                    break;
                case testerProgram:
                    bTester_ReceiveOK[testerProgram]=true;
                    iEnd = TesterReceiveBuffer.Pos(_ETX_);
                    sTesterPGM=TesterReceiveBuffer.SubString(iStart+sTester_Recv[testerProgram].Length(), iEnd-(iStart+sTester_Recv[testerProgram].Length()));
                    iEnd=iEnd+2;
                    //#ifdef HT9045
                        MyDBIProcess("Process", "RECV:Tester PGM:"+sTesterPGM);
                    //#endif
                    break;
                case testerProgramOK:
                    iEnd=iEnd+2;
                    //iEnd=TesterReceiveBuffer.Length();
                    bTester_ReceiveOK[testerProgramOK]=true;
                    //#ifdef HT9045
                        MyDBIProcess("Process", "RECV:Tester PGM:OK");
                    //#endif
                    break;
                case testerProgramNG:
                    iEnd=iEnd+2;
                    //iEnd=TesterReceiveBuffer.Length();
                    bTester_ReceiveOK[testerProgramNG]=true;
                    //#ifdef HT9045
                        MyDBIProcess("Process", "RECV:Tester PGM:NG");
                    //#endif
                    break;
            }
            WriteOcrInfoToMemo("RECV Tester: "+TesterReceiveBuffer.SubString(iStart, iEnd));
            fLotInfo->mmoHandlerWithTester->Lines->Add("RECV Tester: "+TesterReceiveBuffer.SubString(iStart, iEnd));

            TesterReceiveBuffer.Delete(iStart, iEnd);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::Button1Click(TObject *Sender)
{
    InitOcrWithTester();
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::Timer1Timer(TObject *Sender)
{
    int ret=0;
    static bool bEnter=false;

    if(InitialOK==false)
        return;

    if(bEnter==true)
        return;

    bEnter=true;

    ret=OcrWithTester();
    if(ret==0)
        edInsp->Text="";
    else if(ret==1)
    {
        Timer1->Enabled=false;
        edInsp->Text=sTesterLotId;
    }
    else if(ret==2)
    {
        Timer1->Enabled=false;
        edInsp->Text="NG";
    }

    bEnter=false;
}
//---------------------------------------------------------------------------
void TfOCR::DoConnect()
{
    if(ClientSocket1->Active==false)
    {
        ClientSocket1->Address=IPAddress1->Text;
        ClientSocket1->Port=atoi(IPPort1->Text.c_str());
        ClientSocket1->Open();
    }

    if(ClientSocket2->Active==false)
    {
        ClientSocket2->Address=IPAddress2->Text;
        ClientSocket2->Port=IniConfig.iOCRPort; //wei 20181225 ocr port
        ClientSocket2->Open();
    }
}
//---------------------------------------------------------------------------
void TfOCR::DoDisConnect()
{
    try
    {
        ClientSocket1->Close();
        ClientSocket2->Close();
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfOCR::DoDisConnect");
    }
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::ClientSocket1Read(TObject *Sender,
      TCustomWinSocket *Socket)
{
    int length=Socket->ReceiveLength();
    AnsiString s,Result;
    char temp[1024]={'\0'};
    int Pos;

    if(bOCRConnectTest)     //wei 20160613 ocr連線測試
    {
        BYTE* Buff;
        Buff=new BYTE[length];
        Socket->ReceiveBuf(Buff, length);

        memcpy(temp, Buff, length);
        s=temp;

        if(s!="")
        {
            bOCRConnectOK=true;
        }
        delete[] Buff;  //Ifor 20170603 (wei) 刪除Buffer
        Buff=NULL;  //kevin 20180517
    }
    else
    {
        if(length>0)
        {
            BYTE* Buff;
            Buff=new BYTE[length];
            Socket->ReceiveBuf(Buff, length);

            memcpy(temp, Buff, length);
            s=temp;

            if(TestIF_File.bEnableOCRTrayIDDown)    //wei 20170629
            {
                Pos=s.Pos("\r");
                s=s.SubString(1, Pos-1);
                asBarCode=s;
                if(asBarCode!="Error")
                    bOCRReadOK=true;
            }
            else
            {
                if(IniConfig.bEnabledOCRCheckIC)       //wei 20161228 確認Tray是否有IC
                {
                    Pos=s.Pos("\r");
                    s=s.SubString(1, Pos-1);
                    asBarCode=s;

                    if(asBarCode.Pos("T")==1 || asBarCode.Pos("F")==1 || asBarCode.Pos("N")==1)
                    {
                        bOCRReadOK=true;
                    }
                }
                else
                {
                    Pos=s.Pos("\r");
                    s=s.SubString(1, Pos-1);
                    asBarCode=s;

                    if(asBarCode.Pos("T")==1 || asBarCode.Pos("F")==1 || asBarCode.Pos("N")==1)
                    {
                        bOCRReadOK=true;
                    }
                }
            }
            Memo2->Lines ->Add(s.c_str());
            //AI(ht9045-v899) 20260522: capture raw OCR socket result only while OCR simulation logging is enabled
            if(bOCRSimLogEnable)
            {
                Result.sprintf("Socket result: %s", s.c_str());
                OCRSimLogWrite(Result.c_str());
            }
            delete[] Buff;  //Ifor 20170603 (wei) 刪除Buffer
            Buff=NULL;  //kevin 20180517
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::ClientSocket1Connect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AnsiString str="";
    str.printf("%s Connect Port 3000 OK", Now().FormatString("yyyymmddhhmmss"));
    Memo2->Lines->Add(str.c_str());
    RecordProcess("OCR Connect 3000 OK");
    bOCRConnect=true;
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::ClientSocket1Disconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AnsiString str="";
    str.printf("%s DisConnect Port 3000 OK", Now().FormatString("yyyymmddhhmmss"));
    Memo2->Lines->Add(str.c_str());
    RecordProcess("OCR Connect 3000 NG");
    bOCRConnect=false;
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::spInternetConnectClick(TObject *Sender)
{
    DoConnect();        //wei 20150720 OCR連線
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::spInternetDisConnecClick(TObject *Sender)
{
    DoDisConnect();     //wei 20150720 OCR連線
    bSignIn=false;
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::Button2Click(TObject *Sender)
{
    if(StartOCRCaptureTrigger(false))                                           //AI(ht9045-v899) 20260522: manual reader trigger follows selected switch/SE8 mode
    {
        MySleep(100);
        StopOCRCaptureTrigger();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::Button3Click(TObject *Sender)
{
    StopOCRCaptureTrigger();                                                    //AI(ht9045-v899) 20260522: keep manual trigger-off routed through the shared OCR trigger helper
}
//---------------------------------------------------------------------------
bool TfOCR::StartOCRCaptureTrigger(bool bBypassVision)
{
    AnsiString str;

    if(IsOCRCommandTrigger())                                                   //AI(ht9045-v899) 20260522: command trigger mode is exclusive and never toggles SwOCRTigger
    {
        if(bBypassVision)
        {
            Memo1->Lines->Add("SE8 bypassed by OCR simulation");
            return true;
        }

        if(ClientSocket2->Active==false)
        {
            DoConnect();
            return false;
        }

        try
        {
            str.sprintf("SE8\r\n");
            ClientSocket2->Socket->SendBuf(str.c_str(), str.Length());
            Memo1->Lines->Add(str.c_str());
            RecordProcess("OCR Capture Trigger SE8");
            return true;
        }
        catch(...)
        {
            RecordProcess("OCR Capture Trigger SE8 Send Fail");
            DoConnect();
            return false;
        }
    }

    SW[SwOCRTigger].On();
    return true;
}
//---------------------------------------------------------------------------
bool TfOCR::IsOCRCommandTrigger()
{
    return (IniConfig.iOCRTriggerMode==OCR_TRIGGER_MODE_COMMAND);
}
//---------------------------------------------------------------------------
void TfOCR::StopOCRCaptureTrigger()
{
    if(IsOCRCommandTrigger())                                                   //AI(ht9045-v899) 20260522: SE8 mode must not touch the switch output, including trigger-off timing
        return;

    SW[SwOCRTigger].Off();
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::SpeedButton2Click(TObject *Sender)
{
    AnsiString str;
    str.sprintf("%s\r\n", fOCR->edtSendCmd->Text);
    ClientSocket2->Socket->SendBuf(str.c_str(), str.Length());
    Memo1->Lines->Add(str.c_str());
}
//---------------------------------------------------------------------------
//AI(ht9045-v899) 20260505: run OCR simulation through Tray OCR auto-flow instead of command-only trigger loop
void __fastcall TfOCR::btnOCRSimRunClick(TObject *Sender)
{
    int iLoop=atoi(edOCRSimLoopTimes->Text.c_str());
    AnsiString asStatus;

    if(bOCRSimRunning)
        return;

    if(SystemStart)
    {
        ShowMyMessage("OCR simulation can only run while Auto Run is stopped");
        return;
    }

    if(fAllMotorHome==false)
    {
        ShowMyMessage("Please Home All before OCR simulation");
        return;
    }

    if(INSTALL_OCR==eocrUninstal || TestIF.bOcrFunction==false || CosFunction.bTrayOCR==false)
    {
        ShowMyMessage("Tray OCR function is not enabled");
        return;
    }

    //AI(ht9045-v899) 20260506: simulation must use new TrayArm-CCD + MLoaderY architecture (Frank 20250214);
    //if INSTALL_OCR_YMot is not installed, DoOCRFlow1 falls back to legacy InArm-CCD path which would move InArm and ask to remove tray
    if(INSTALL_OCR_YMot!=eocrYMotInstal)
    {
        ShowMyMessage("OCR simulation requires INSTALL_OCR_YMot = Installed.\r\nLegacy InArm CCD flow is not supported by this simulation.");
        return;
    }

    if(bRunOcrInsp)
    {
        ShowMyMessage("OCR flow is already running");
        return;
    }

    //AI(ht9045-v899) 20260506: only allow OCR simulation outside of production (Lot must not be started)
    if(RunInfo.bLotStart)
    {
        ShowMyMessage("OCR simulation is not allowed during production. Please run Lot End first.");
        return;
    }

    if(IniConfig.bCompareOCRData)
    {
        ShowMyMessage("Disable Compare OCR Data before simulation to avoid consuming barcode files");
        return;
    }

    if(iLoop<1)
        iLoop=1;
    if(iLoop>100)
        iLoop=100;
    edOCRSimLoopTimes->Text=AnsiString(iLoop);

    BackupOCRSimulationData();
    if(PrepareOCRSimulationTrayFromTrayForm()==false)
    {
        StopOCRSimulation("Stopped (Invalid Tray Form)");
        ShowMyMessage("Tray Form data is invalid for OCR simulation");
        return;
    }

    bOCRSimBypassAOI=(cbOCRSimBypassAOI!=NULL && cbOCRSimBypassAOI->Checked);   //AI(ht9045-v899) 20260511: latch bypass flag at simulation start; cleared again in StopOCRSimulation

    InitOCRFlow(false);
    bOCRStartPosition=false;
    bBarCodeError=false;
    bOCRNoIC=false;
    bInspectError=false;
    bOCRReadOK=false;
    bRunOcrInsp=false;
    iOCRSimLoopNow=1;
    iOCRSimLoopTotal=iLoop;
    bOCRSimRunning=true;
    //AI(ht9045-v899) 20260522: enable and clear per-loop OCR simulation log at the first loop start
    bOCRSimLogEnable=true;
    OCRSimLogClear(iOCRSimLoopNow, iOCRSimLoopTotal);
    asStatus.sprintf("Simulation start: Tray=%d x %d, BypassAOI=%d", MOT[MMTrayY].Tray.XItem, MOT[MMTrayY].Tray.YItem, bOCRSimBypassAOI);
    OCRSimLogWrite(asStatus.c_str());

    if(btnOCRSimRun!=NULL)
    {
        btnOCRSimRun->Enabled=false;
        btnOCRSimRun->Caption="Running";
    }
    if(btnOCRSimStop!=NULL)
        btnOCRSimStop->Enabled=true;

    asStatus.sprintf("Running loop %d/%d, Tray %d x %d", iOCRSimLoopNow, iOCRSimLoopTotal, MOT[MMTrayY].Tray.XItem, MOT[MMTrayY].Tray.YItem);
    UpdateOCRSimulationStatus(asStatus);
    Memo2->Lines->Add(Now().FormatString("yyyymmddhhmmss")+" OCR Simulation Start");
    tmrOCRSim->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::btnOCRSimStopClick(TObject *Sender)
{
    StopOCRSimulation("Stopped (User)");
    Memo2->Lines->Add(Now().FormatString("yyyymmddhhmmss")+" OCR Simulation Stop");
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::tmrOCRSimTimer(TObject *Sender)
{
    static bool bEnter=false;
    int ret=0;
    AnsiString asStatus;

    if(bEnter)
        return;
    bEnter=true;

    if(bOCRSimRunning==false)
    {
        bEnter=false;
        return;
    }

    if(SystemStart)
    {
        StopOCRSimulation("Stopped (Auto Run Start)");
        bEnter=false;
        return;
    }

    ret=DoOCRFlow1();

    if(bBarCodeError)
    {
        asStatus.sprintf("Stopped (Error R%d C%d)", iBarCodeErrorR+1, iBarCodeErrorC+1);
        //AI(ht9045-v899) 20260522: record the simulation stop reason before StopOCRSimulation disables logging
        OCRSimLogWrite(asStatus.c_str());
        StopOCRSimulation(asStatus);
        Memo2->Lines->Add(Now().FormatString("yyyymmddhhmmss")+" OCR Simulation Error");
        bEnter=false;
        return;
    }

    if(ret==0)
    {
        asStatus.sprintf("Running loop %d/%d, R%d C%d", iOCRSimLoopNow, iOCRSimLoopTotal, iOCRPosR+1, iOCRPosC+1);
        UpdateOCRSimulationStatus(asStatus);
        bEnter=false;
        return;
    }

    if(ret==1)
    {
        asStatus.sprintf("OCR Simulation Finish Loop %d/%d", iOCRSimLoopNow, iOCRSimLoopTotal);
        //AI(ht9045-v899) 20260522: keep each simulation loop result in the current-loop log before preparing the next loop
        OCRSimLogWrite(asStatus.c_str());
        Memo2->Lines->Add(Now().FormatString("yyyymmddhhmmss")+" "+asStatus);

        if(iOCRSimLoopNow<iOCRSimLoopTotal)
        {
            RestoreOCRSimulationData();
            PrepareOCRSimulationTrayFromTrayForm();
            InitOCRFlow(false);
            bOCRStartPosition=false;
            bBarCodeError=false;
            bOCRNoIC=false;
            bInspectError=false;
            bOCRReadOK=false;
            iOCRSimLoopNow++;
            //AI(ht9045-v899) 20260522: overwrite the log file when a new simulation loop begins to avoid cross-loop contamination
            OCRSimLogClear(iOCRSimLoopNow, iOCRSimLoopTotal);
            asStatus.sprintf("Running loop %d/%d, Tray %d x %d", iOCRSimLoopNow, iOCRSimLoopTotal, MOT[MMTrayY].Tray.XItem, MOT[MMTrayY].Tray.YItem);
            OCRSimLogWrite(asStatus.c_str());
            UpdateOCRSimulationStatus(asStatus);
        }
        else
        {
            StopOCRSimulation("Finished");
        }
        bEnter=false;
        return;
    }

    if(ret==OCR_SIM_RET_TRIGGER_FAIL)                                           //AI(ht9045-v899) 20260526: convert simulation trigger alarm return into a clean timer stop
    {
        asStatus="Stopped (OCR Trigger Fail)";
        StopOCRSimulation(asStatus);
        Memo2->Lines->Add(Now().FormatString("yyyymmddhhmmss")+" OCR Simulation Trigger Fail");
        bEnter=false;
        return;
    }

    if(ret==OCR_SIM_RET_CAPTURE_TIMEOUT)                                        //AI(ht9045-v899) 20260526: stop simulation after capture timeout alarm instead of retrying silently
    {
        asStatus="Stopped (OCR Capture Timeout)";
        StopOCRSimulation(asStatus);
        Memo2->Lines->Add(Now().FormatString("yyyymmddhhmmss")+" OCR Simulation Capture Timeout");
        bEnter=false;
        return;
    }

    asStatus.sprintf("Stopped (Result %d)", ret);
    //AI(ht9045-v899) 20260522: record unexpected OCR simulation return code before shutdown clears the log enable flag
    OCRSimLogWrite(asStatus.c_str());
    StopOCRSimulation(asStatus);
    Memo2->Lines->Add(Now().FormatString("yyyymmddhhmmss")+" OCR Simulation Stop Result "+AnsiString(ret));
    bEnter=false;
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::RadioGroup1Click(TObject *Sender)
{
    if(RadioGroup1->ItemIndex==0)
    {
        edtSendCmd->Text="admin";
    }
    else if(RadioGroup1->ItemIndex==1)
    {
        edtSendCmd->Text="\r\n";
    }
    else if(RadioGroup1->ItemIndex==2)
    {
        edtSendCmd->Text="SO0";
    }
    else if(RadioGroup1->ItemIndex==3)
    {
        edtSendCmd->Text="LF1008";
    }
    else if(RadioGroup1->ItemIndex==4)
    {
        edtSendCmd->Text="SO1";
    }
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::ClientSocket2Connect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AnsiString str="";
    str.printf("Connect Port %d OK", IniConfig.iOCRPort);
    Memo1->Lines->Add(str.c_str());
    RecordProcess(str.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::ClientSocket2Disconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AnsiString str="";
    str.printf("DisConnect Port %d OK", IniConfig.iOCRPort);
    Memo1->Lines->Add(str.c_str());
    RecordProcess(str.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::ClientSocket2Read(TObject *Sender,
      TCustomWinSocket *Socket)
{
    int length=Socket->ReceiveLength();
    AnsiString s, Result;
    char temp[1024]={'\0'};
    int Pos=0;

    if(length>0)
    {
        BYTE* Buff;
        Buff=new BYTE[length];
        Socket->ReceiveBuf(Buff, length);

        memcpy(temp, Buff, length);
        s=temp;

        if(s.Pos("1\r\n"))
        {
            if(bOCROK[0])
            {
                bOCROK[1]=true;
                bOCROK[0]=false;
            }
            else if(bOCROK[1])
            {
                bOCROK[2]=true;
                bOCROK[1]=false;
            }
            else if(bOCROK[9])
            {
                bOCROK[3]=true;
                bOCROK[9]=false;
            }
            else if(bOCROK[3])
            {
                bOCROK[4]=true;
                bOCROK[3]=false;
            }
            else if(bOCROK[4] || bOCROK[10])
            {
               Pos=s.Pos(".job");
               asCheckFileName=s.Delete(Pos, s.Length());
               asCheckFileName=asCheckFileName.SubString(4, Pos-1);
               bOCROK[4]=false;
               bOCROK[5]=true;
               bOCROK[10]=false;
            }
            else if(bOCROK[7])
            {
                bOCROK[7]=false;
            }
        }
        else if(s.Pos("User:"))
        {
            bOCRUser=true;
        }
        else if(s.Pos("Password: "))
        {
            bOCROK[8]=true;
            bOCRPassWord=true;
        }
        else if(s.Pos("User Logged In"))
        {
            bSignIn=true;
            bOCROK[8]=false;
            bOCROK[10]=true;
            bOCRNewFile=true;
            RecordProcess("OCR User Logged In");
        }
        else
        {
            bOCROK[7]=true;
        }
        Memo1->Lines->Add(s.c_str());
        delete[] Buff;  //Ifor 20170603 (wei) 刪除Buffer
        Buff=NULL;  //kevin 20180517
    }
}
//---------------------------------------------------------------------------
bool TfOCR::OCRIn()
{
    AnsiString str;
    if(bOCRUser)
    {
        str.sprintf("admin\r\n");
        fOCR->ClientSocket2->Socket->SendBuf(str.c_str(), str.Length());
        Memo1->Lines->Add(str.c_str());
        bOCRUser=false;
    }
    else if(bOCROK[8])
    {
        str.sprintf("\r\n\r\n");
        fOCR->ClientSocket2->Socket->SendBuf(str.c_str(), str.Length());
        Memo1->Lines->Add(str.c_str());
        bOCRPassWord=false;
        return true;
    }
    else if(bOCROK[10])      //wei 20161028回傳OCR檔名
    {
        str.sprintf("GF\r\n");
        fOCR->ClientSocket2->Socket->SendBuf(str.c_str(), str.Length());
        Memo1->Lines->Add(str.c_str());
        bOCRNewFile=false;
    }
    return false;
}
//---------------------------------------------------------------------------
int TfOCR::OCRChangeFile(bool bReset)
{
    AnsiString str;
    if(bOCROK[7])
    {
        str.sprintf("SO1\r\n");
        bOCROK[7]=false;
    }
    else if(bOCROK[0])
    {
        str.sprintf("SO1\r\n");
    }
    else if(bOCROK[1])
    {
        str.sprintf("SO0\r\n");
    }
    else if(bOCROK[2])
    {
        str.sprintf("LF%s\r\n", fLotInfo->lbOCRUseFile->Caption);
        bOCROK[9]=true;
        bOCROK[2]=false;
    }
    else if(bOCROK[3])
    {
        str.sprintf("SO1\r\n");
    }
    else if(bOCROK[4])
    {
        str.sprintf("GF\r\n");
    }
    else if(bOCROK[5])
    {
        if(asCheckFileName==fLotInfo->lbOCRUseFile->Caption)
        {
            RecordProcess("Change OCR File OK");
            ShowMyMessage("Change OCR File OK");
        }
        else
        {
            RecordProcess("Change OCR File NG");
            ShowMyMessage("Change OCR File NG");
        }
        bOCROK[5]=false;
        bCheckFile=false;
        bOCROK[6]=true;
    }
    else if(bOCROK[6])
    {
        DoConnect();
        bOCROK[6]=false;
        return 1;
    }

    fOCR->ClientSocket2->Socket->SendBuf(str.c_str(), str.Length());
    Memo1->Lines->Add(str.c_str());
    return 0;
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::SpeedButton4Click(TObject *Sender)
{
    if(fLotInfo->lbOCRUseFile->Caption!="")
    {
        bOCROK[0]=true;
        fLotInfo->Timer3->Enabled=true;
    }
    else
    {
        ShowMyMessage("Please Enter Change OCR File Name");
    }
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::spbSaveClick(TObject *Sender)
{
    if(IniConfig.bA02DisableSaveParsWhenSwitchToOp==true &&                     //RogerYang 20260305 張寧說時間到都要登出，且不可被更改任何設定(A01_2)
        AccessLevel==0)
    {
        ShowMyMessage("[A01_2]目前已切換到Operator權限，\r\n請重新登入再做設定!");
        Close();
    }

    AnsiString S="";

    S=GetLastOpenFN();
    AnsiString szDir="";
    szDir.sprintf("%s%s\\AOI.Data", DataPath, S);

    WriteIniData(szDir, "OCR SETTING", "iOCRSkip",        edOCRSkip->Text);
    WriteIniData(szDir, "OCR SETTING", "iOCRRetry",       edOCRRetry->Text);
    WriteIniData(szDir, "OCR SETTING", "iWordCount",      edOCRWordCount->Text);
    WriteIniData(szDir, "OCR SETTING", "asOCRWordType",   edOCRWordType->Text);          //wei 20161128 確認字串各自Type是否正確
    WriteIniData(szDir, "OCR SETTING", "iBlueLight",        edSetBlueLight->Text);
    WriteIniData(szDir, "OCR SETTING", "iRedLight",         edSetRedLight->Text);
    WriteIniData(szDir, "OCR SETTING", "bDisabledKeyin",    ckDisabledKeyin->Checked);      //wei 20161004 No IC 不能Keyin
    WriteIniData(szDir, "OCR SETTING", "bCheckBarCodeMap",  cbCheckBarCodeMap->Checked);    //Frank 20161025 確認四個角落的OCR Code
    WriteIniData(szDir, "OCR SETTING", "bEnabledOCRCheckIC",ckOCRCheckHasIC->Checked);      //wei 20161228 確認Tray是否有IC
    WriteIniData(szDir, "OCR SETTING", "bEnabledOCRMoveSRead",ckOCRMoveSRead->Checked);     //wei 20161118 OCR S型讀取
    WriteIniData(szDir, "OCR SETTING", "bEnabledStartposshift",ckStartposshift->Checked);   //wei 20181225 初始點位
    WriteIniData(szDir, "OCR SETTING", "iStartposshift",    edStartposshift->Text);         //wei 20181225 初始點位
    WriteIniData(szDir, "OCR SETTING", "OCRLightChange",    ckOCRLightChange->Checked);     //wei 20181225 光源auto change
    WriteIniData(szDir, "OCR SETTING", "OCRLightNoDown",    ckOCRLightNoDown->Checked);     //wei 20181225 光源不下降
    WriteIniData(szDir, "OCR SETTING", "iOCRTriggerMode",    rgOCRTriggerMode->ItemIndex);   //AI(ht9045-v899) 20260522: persist selected switch/SE8 OCR trigger mode per setup file
    WriteIniData(szDir, "OCR SETTING", "CompareOCRData",    ckCompareOCRData->Checked);     //KenHsieh 20220825 : 新增OCR比對功能
    WriteIniData(szDir, "OCR SETTING", "bEnabledOCRCheckWordCount",  ckWordCount->Checked); //wei 20220413 開關字數判斷功能
    WriteIniData(szDir, "OCR SETTING", "OCRAndBinLog",      ckOCRAndBinLog->Checked);       //KenHsieh 20230406 : 新增OCR Data + Bin Log功能
    WriteIniData(szDir, "OCR SETTING", "OCRBinLogAddMark",  ckOCRBinLogAddMark->Checked);   //KenHsieh 20230412 : 新增OCR Data + Bin Log功能 + Mark

    fOCR_ReadFile();
    fMain->BackupSetupFile();    //Ifor 20170620 (wei) add Auto BackUp Setup File & Last Data
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::sbtExitClick(TObject *Sender)
{
    sbtExit->Down =false;
    Close();
}
//---------------------------------------------------------------------------
void TfOCR::fOCR_ReadFile()
{
    AnsiString S="";
    S=GetLastOpenFN();
    AnsiString szDir="";

    AnsiString tmpS ="";
    szDir.sprintf("%s%s\\AOI.Data", DataPath, S);

    IniConfig.bDisabledKeyin    = ReadIniData(szDir, "OCR SETTING", "bDisabledKeyin",    false);        //Frank 20161012 add
    IniConfig.bCheckBarCodeMap  = ReadIniData(szDir, "OCR SETTING", "bCheckBarCodeMap",  false);        //Frank 20161025 確認四個角落的OCR Code
    IniConfig.bEnabledOCRCheckIC= ReadIniData(szDir, "OCR SETTING", "bEnabledOCRCheckIC",false);        //wei 20161228 確認Tray是否有IC
    IniConfig.bEnableOCRMoveSRead= ReadIniData(szDir, "OCR SETTING", "bEnabledOCRMoveSRead",false);     //wei 20161118 OCR S型讀取
    IniConfig.bEnableStartposshift= ReadIniData(szDir, "OCR SETTING", "bEnabledStartposshift",false);   //wei 20181225 初始點位
    IniConfig.iStartposshift    = ReadIniData(szDir, "OCR SETTING", "iStartposshift",0);                //wei 20181225 初始點位
    IniConfig.iOCRPort          = CheckAndReadIniData(asGeneralPath, "OCR SETTING", "OCR Port", 24);    //wei 20181225 ocr port
    IniConfig.iOCRTriggerMode   = ReadIniData(szDir, "OCR SETTING", "iOCRTriggerMode", OCR_TRIGGER_MODE_SWITCH); //AI(ht9045-v899) 20260522: default missing OCR trigger mode to switch for old machine compatibility
    if(IniConfig.iOCRTriggerMode<OCR_TRIGGER_MODE_SWITCH ||
       IniConfig.iOCRTriggerMode>OCR_TRIGGER_MODE_COMMAND)
        IniConfig.iOCRTriggerMode=OCR_TRIGGER_MODE_SWITCH;

    IniConfig.iOCRSkip        = ReadIniData(szDir, "OCR SETTING", "iOCRSkip",         3);
    IniConfig.iOCRRetry       = ReadIniData(szDir, "OCR SETTING", "iOCRRetry",       10);
    IniConfig.iOCRWordCount   = ReadIniData(szDir, "OCR SETTING", "iWordCount",      10);
    IniConfig.asOCRWordType   = ReadIniData(szDir, "OCR SETTING", "asOCRWordType",   AnsiString(""));   //wei 20161128 確認字串各自Type是否正確

    IniConfig.iBlueLight      = ReadIniData(szDir, "OCR SETTING", "iBlueLight",       0);
    IniConfig.iRedLight       = ReadIniData(szDir, "OCR SETTING", "iRedLight",        0);
    IniConfig.OCRLightChange  = ReadIniData(szDir, "OCR SETTING", "OCRLightChange",false);              //wei 20181225 光源auto change
    IniConfig.OCRLightNoDown  = ReadIniData(szDir, "OCR SETTING", "OCRLightNoDown",false);              //wei 20181225 光源不下降
    IniConfig.bCompareOCRData = ReadIniData(szDir, "OCR SETTING", "CompareOCRData",false);              //KenHsieh 20220825 : 新增OCR比對功能
    IniConfig.bOCRAndBinLog   = ReadIniData(szDir, "OCR SETTING", "OCRAndBinLog"  ,false);              //KenHsieh 20230406 : 新增OCR Data + Bin Log功能
    IniConfig.bOCRBinLogAddMark=ReadIniData(szDir, "OCR SETTING", "OCRBinLogAddMark" ,false);           //KenHsieh 20230412 : 新增OCR Data + Bin Log功能 + Mark

    IniConfig.bEnabledOCRCheckWordCount= ReadIniData(szDir, "OCR SETTING", "bEnabledOCRCheckWordCount",false);//wei 20220413 開關字數判斷功能

    if(INSTALL_OCR!=eocrUninstal)   //jou 20211029 : 需判斷是否有安裝OCR
    {
        if(TestIF_File.bOcrFunction)            //Steven 20150713 : GPIB update to V2.01 for 2D Code   //wei 20151120
            fMain->SendMSG_CMD(MSG_CMD_EnableBarCode);
        else
            fMain->SendMSG_CMD(MSG_CMD_DisableBarCode);
    }
}
//---------------------------------------------------------------------------
void TfOCR::fOCR_DoIniDataToForm()
{
    edOCRSkip->Text        = IniConfig.iOCRSkip;
    edOCRRetry->Text       = IniConfig.iOCRRetry;
    edOCRWordCount->Text   = IniConfig.iOCRWordCount;
    edOCRWordType->Text    = IniConfig.asOCRWordType;                           //wei 20161128 確認字串各自Type是否正確

    if(INSTALL_OCR!=eocrUninstal && CosFunction.bTrayOCR)
    {
        TabSheet1->TabVisible=false;
        tsOCR_Cognex->TabVisible=true;
        tsOCR_Cognex_Setting->TabVisible=true;
    }
    else
    {
        TabSheet1->TabVisible=true;
        tsOCR_Cognex->TabVisible=false;
        tsOCR_Cognex_Setting->TabVisible=false;
    }

    edSetBlueLight->Text= IniConfig.iBlueLight;
    edSetRedLight->Text = IniConfig.iRedLight;
    ckDisabledKeyin  ->Checked =IniConfig.bDisabledKeyin;      //wei 20161004 No IC 不能Keyin
    cbCheckBarCodeMap->Checked =IniConfig.bCheckBarCodeMap;    //Frank 20161025 確認四個角落的OCR Code
    ckOCRCheckHasIC  ->Checked =IniConfig.bEnabledOCRCheckIC;  //wei 20161228 確認Tray是否有IC
    ckOCRMoveSRead   ->Checked =IniConfig.bEnableOCRMoveSRead; //wei 20161118 OCR S型讀取
    ckStartposshift  ->Checked =IniConfig.bEnableStartposshift;//wei 20181225 初始點位
    edStartposshift->Text       = IniConfig.iStartposshift;    //wei 20181225 初始點位
    ckOCRLightChange->Checked   = IniConfig.OCRLightChange;    //wei 20181225 光源auto change
    ckOCRLightNoDown->Checked   = IniConfig.OCRLightNoDown;    //wei 20181225 光源不下降
    IPPort2->Text               = IniConfig.iOCRPort;          //wei 20181225 ocr port
    rgOCRTriggerMode->ItemIndex  = IniConfig.iOCRTriggerMode;   //AI(ht9045-v899) 20260522: show saved switch/SE8 OCR trigger mode, defaulting to switch
    ckCompareOCRData->Checked   = IniConfig.bCompareOCRData;   //KenHsieh 20220825 : 新增OCR比對功能
    ckOCRAndBinLog->Checked     = IniConfig.bOCRAndBinLog;     //KenHsieh 20230406 : 新增OCR Data + Bin Log功能
    ckOCRBinLogAddMark->Checked = IniConfig.bOCRBinLogAddMark; //KenHsieh 20230412 : 新增OCR Data + Bin Log功能 + Mark
    ckWordCount->Checked        =IniConfig.bEnabledOCRCheckWordCount;  //wei 20220413 開關字數判斷功能
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::edOCRRetryClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1, 300);
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::spbResetComClick(TObject *Sender)
{
    StopAllCom();
    RS232Init();
}
//---------------------------------------------------------------------------
void TfOCR::StopAllCom()
{
    CommOcr_Light->StopComm();
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::RS232Init()
{
    bool flag;
    AnsiString Str;

    flag=GetCOMPortStatus(cbCommReader1->Text);
    if(flag==false)
    {
        Str.sprintf("OCR Light : %s port status error!", cbCommReader1->Text);
        ShowMyMessage(Str);
    }
    else
    {
        CommOcr_Light->CommName="\\\\.\\"+cbCommReader1->Text;
        try
        {
            CommOcr_Light->Parity=None;
            CommOcr_Light->BaudRate=19200;
            CommOcr_Light->ByteSize=_8;
            CommOcr_Light->ParityCheck=false;
            CommOcr_Light->StopBits=_1;
            CommOcr_Light->StopComm();
            CommOcr_Light->StartComm();  //僅能啟動一次
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TfOCR::RS232Init");
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::btSetBlueLightClick(TObject *Sender)
{
    ChangeLightValue(1, atoi(edSetBlueLight->Text.c_str()));
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::btSetRedLightClick(TObject *Sender)
{
    ChangeLightValue(2, atoi(edSetRedLight->Text.c_str()));
}
//---------------------------------------------------------------------------
void TfOCR::ChangeLightValue(int Channel, int Value)
{
    unsigned char Btmp1;
    AnsiString tmp1;
    char Str[1024];
    char Str1[1024];
    AnsiString tmp2;
//    int Value=atoi(edSetRedLight->Text.c_str());
    CommOcr_Light->StopComm();
    Sleep(100);
    RS232Init();
    Sleep(100);
    tmp1 = IntToHex(Value,4);
    ::sprintf(Str,":%02X%02X%04X%4s",1,6,Channel,tmp1);
    Btmp1=A_Create_LCR(&Str[1], 12);
    Str[13]=T_HEX2ASCII_Mac(Btmp1>>4);
    Str[14]=T_HEX2ASCII_Mac(Btmp1);

    ::sprintf(Str1,":%02X%02X%04X%4s%c%c\r\n",1,6,Channel,tmp1,Str[13],Str[14]);

    CommOcr_Light->WriteCommData(Str1,::strlen(Str1));
    tmp2.sprintf("%s",Str1);
    Memo3->Lines->Add(tmp2.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::sbSetBlueLightChange(TObject *Sender)
{
    AnsiString Str;
    edSetBlueLight->Text=AnsiString(sbSetBlueLight->Position);
//    IniConfig.iBlueLight=atoi(edSetBlueLight->Text.c_str());
//    Str.sprintf("%c%c%c%c", 0x66, 0x01, IniConfig.iBlueLight, 0x66);
//    CommOcr_Light->WriteCommData(Str.c_str(), Str.Length());
//    Memo3->Lines->Add(Str.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::sbSetRedLightChange(TObject *Sender)
{
    AnsiString Str;
    edSetRedLight->Text=AnsiString(sbSetRedLight->Position);
//    IniConfig.iRedLight=atoi(edSetRedLight->Text.c_str());
//    Str.sprintf("%c%c%c%c", 0x66, 0x02, IniConfig.iRedLight, 0x66);
//    CommOcr_Light->WriteCommData(Str.c_str(), Str.Length());
//    Memo3->Lines->Add(Str.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::edSetBlueLightChange(TObject *Sender)
{
    sbSetBlueLight->Position=atoi(edSetBlueLight->Text.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::edSetRedLightChange(TObject *Sender)
{
    sbSetRedLight->Position=atoi(edSetRedLight->Text.c_str());
}
//---------------------------------------------------------------------------
void TfOCR::SaveOCRImage()
{
    AnsiString S="";
    S=GetLastOpenFN();
//    AnsiString OrgPath="D:\\HT9045_Log\\OCR\\Alarm\\Bad";
    AnsiString SPath="D:\\HT9045_Log\\OCR\\";
    AnsiString str1, str2, str3="", str4;

    DirectoryListBox1->Directory=asOCRSaveMane; //wei 20170518 建立OCR存圖路徑
    DirectoryListBox1->Update();
    DirectoryListBox1->Refresh();
    FileListBox1->Directory=asOCRSaveMane;      //wei 20170518 建立OCR存圖路徑
    FileListBox1->Update();
    FileListBox1->Refresh();

    if(FileListBox1->Items->Count!=0)
    {
        asOCRImageName=FileListBox1->Items->Strings[0];
        str3=asOCRImageName;                                                //wei 20161122 OCR 存圖檔名加入輸入值
        str3.Delete(asOCRImageName.Length()-3, asOCRImageName.Length());    //wei 20161122 OCR 存圖檔名加入輸入值
    }
    else
        asOCRImageName="";

    SPath+=S;

    if(CUSTOMER_CODE==CC_KYEC_LEE)
        str4.sprintf("%s\\%s", SPath , fLotInfo->edtSysLotID->Text);
    else
        str4.sprintf("%s", SPath);

    MyForceDirectories(str4);

    if(asOCRImageName!="")
    {
        str2.sprintf("%s\\%s", asOCRSaveMane, asOCRImageName);  //wei 20170518 建立OCR存圖路徑
        if(FileExists(str2))
        {
            if(CUSTOMER_CODE!=CC_KYEC_LEE)                                      //Alick 20170104 (Steven) add 增加判斷客戶(京元和非京元)
            {
                str1.sprintf("%s\\%s_%s,%s_%s.bmp", str4, AnsiString(iBarCodeErrorC+1), AnsiString(iBarCodeErrorC+1), str3, asOCRSaveName);      //wei 20161122 OCR 存圖檔名加入輸入值    //Alick 20161227 mark 存檔失敗&沒有asOCRSaveName
            }
            else
            {
//                if(asOCRSaveType=="_NO IC")
//                {
//                    str1.sprintf("%s\\%s_%s,%s_%s.bmp", str4, fLotInfo->edtSysLotID->Text+asOCRSaveType, AnsiString(iBarCodeErrorR+1), AnsiString(iBarCodeErrorC+1), Now().FormatString("yyyymmddhhmmss"));      //Alick 20161227 add 修正存檔格式(LOTID+時間) //Alick 20170104 add 京元增加紀錄IC位置
//                }
//                else
                {
                    str1.sprintf("%s\\%s_%s,%s_%s.bmp", str4, fLotInfo->edtSysLotID->Text, AnsiString(iBarCodeErrorC+1), AnsiString(iBarCodeErrorC+1), Now().FormatString("yyyymmddhhmmss"));      //Alick 20161227 add 修正存檔格式(LOTID+時間) //Alick 20170104 add 京元增加紀錄IC位置
                }
            }
            CopyFile(str2.c_str(), str1.c_str(), false);
            MySleep(50);
        }
    }
}
//---------------------------------------------------------------------------
void TfOCR::ReadOCRImage()    //wei 20161122 OCR 存圖檔名加入輸入值
{
    AnsiString S="";
    S=GetLastOpenFN();
//    AnsiString OrgPath="D:\\HT9045_Log\\OCR\\Alarm\\Bad";
    AnsiString SPath="D:\\HT9045_Log\\OCR\\";
    AnsiString str1, str2;

    DirectoryListBox1->Directory=asOCRSaveMane; //wei 20170518 建立OCR存圖路徑
    DirectoryListBox1->Update();
    DirectoryListBox1->Refresh();
    FileListBox1->Directory=asOCRSaveMane;      //wei 20170518 建立OCR存圖路徑
    FileListBox1->Update();
    FileListBox1->Refresh();

    if(FileListBox1->Items->Count!=0)
        asOCRImageName=FileListBox1->Items->Strings[0];
    else
        asOCRImageName="";
}
//---------------------------------------------------------------------------
void TfOCR::DeleteOCRImage()
{
//    AnsiString OrgPath="D:\\HT9045_Log\\OCR\\Alarm\\Bad";
    AnsiString str1, str2;

    DirectoryListBox1->Directory=asOCRSaveMane; //wei 20170518 建立OCR存圖路徑
    DirectoryListBox1->Update();
    DirectoryListBox1->Refresh();
    FileListBox1->Directory=asOCRSaveMane;      //wei 20170518 建立OCR存圖路徑
    FileListBox1->Update();
    FileListBox1->Refresh();

    //wei 20161122 修改OCR刪圖方式
    for(int i=0; i<=FileListBox1->Items->Count-1; i++)
    {
        asOCRImageName=FileListBox1->Items->Strings[i];
        str1.sprintf("%s\\%s", asOCRSaveMane, asOCRImageName);  //wei 20170518 建立OCR存圖路徑
        DeleteFile(str1.c_str());
    }

    MyForceDirectories(asOCRSaveMane);
}
//---------------------------------------------------------------------------
bool TfOCR::CheckOCRWordType(AnsiString asOCR)    //wei 20161128 確認字串各自Type是否正確
{
    char str[300]="", str1[300]="";

    if(IniConfig.asOCRWordType!="")
    {
        sprintf(str, "%s",  IniConfig.asOCRWordType);
        sprintf(str1, "%s", asOCR);
        for(int i=0; i<=IniConfig.iOCRWordCount; i++)
        {
            if(str[i]=='A')
            {
                if(isalpha(str1[i])==false)
                {
                    return false;
                }
            }
            else if(str[i]=='N')
            {
                if(isdigit(str1[i])==false)
                {
                    return false;
                }
            }
        }
    }
    return true;
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::IPPort1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_PORT);
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::edSetBlueLightClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 255, 0);
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::edStartposshiftClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, -50000, 50000);
}
//---------------------------------------------------------------------------
void __fastcall TfOCR::Button4Click(TObject *Sender)
{
    TDateTime tt= Now();
    Word SystemYear,SystemMonth,SystemDate;
    DecodeDate(tt, SystemYear, SystemMonth, SystemDate);
    AnsiString str1,str2,sLastFile,sFolder;
    char cAlarmTime[64];

    str1.sprintf("%04d%s%02d%s",SystemYear,"_",SystemMonth,"\\");
    GetTimeInfo();
    sprintf(cAlarmTime,"%04d-%02d-%02d %02d %02d %02d",
                SystemYear, SystemMonth, SystemDate,SystemHour,SystemMin,SystemSec);

    AnsiString fp="",s_cFolder;
    sFolder.sprintf("D:\\HT9045_Log\\OCR");

    if(sFolder.SubString(sFolder.Length(),1)!="\\")
        sFolder=sFolder+"\\";

    s_cFolder= sFolder + str1;
    fp= s_cFolder + str2;

    MyForceDirectories(s_cFolder);

    sLastFile=s_cFolder+AnsiString(cAlarmTime)+".txt";

    fOCR->Memo2->Lines->SaveToFile(sLastFile);
}
//---------------------------------------------------------------------------
void TfOCR::SaveOCRAndBinData(AnsiString asOCRCode, AnsiString iBinData, int iBinFailCate)      //KenHsieh 20230406 : 新增OCR Data + Bin Log功能
{
    AnsiString asPath="", asFileName="";//, asUploadPath="", astest="";
    AnsiString asData="", asBin="", asMark="";
    int iIndex=-1;

    asPath.sprintf("%s\\", "D:\\HT9045_Log\\OCR\\BarcodeAndBin");
    MyForceDirectories(asPath);
    asFileName.sprintf("%s.txt", fLotInfo->edtSysLotID->Text);
    asPath+=asFileName;

    if(FileExists(asPath))                          //KenHsieh 20230620 : 客戶要求FT與RT相同code直接刪除更新
    {
        ListOCRDataLog->Clear();
        ListOCRDataLog->LoadFromFile(asPath);

        for(int j=0; j<ListOCRDataLog->Count; j++)
        {
            if(ListOCRDataLog->Strings[j].AnsiPos(asOCRCode)!=0)
            {
                iIndex=j;
                ListOCRDataLog->Delete(iIndex);
                ListOCRDataLog->SaveToFile(asPath);
            }

            if(iIndex!=-1)
                break;
        }
    }
    ListOCRDataLog->Clear();

    if(iBinFailCate==1)                             //判斷丟位置是否為fail Bin或是Error
        asBin="";
    else
        asBin="BIN"+iBinData;

    if(IniConfig.bOCRBinLogAddMark)                 //KenHsieh 20230412 : 新增OCR Data + Bin Log功能 + Mark
    {
        if(iBinData=="8")
            asMark="N0";
        else if(iBinData=="11")
            asMark="N1";
        else if(iBinData=="12")
            asMark="N2";
        else if(iBinData=="13")
            asMark="N3";
        else if(iBinData=="14")
            asMark="N4";
        else
            asMark="";
    }
    else
    {
        asMark="";
    }

    asData.sprintf("%s,%s,%s", asOCRCode, asBin.c_str(), asMark.c_str());

    WriteDataToFile(asPath, asData);
}
//------------------------------------------------------------------------------
void TfOCR::UpLoadOCRAndBinLog()                                                //KenHsieh 20230502 : 利揚要求上傳OCR + BIN Log上傳至Host
{
    AnsiString asUploadPath="", asSourcePath="", asFileName="";
    TStringList *ListUploadOCRDataLog=new TStringList;

    asFileName.sprintf("%s.txt", fLotInfo->edtSysLotID->Text);
    asSourcePath="D:\\HT9045_Log\\OCR\\BarcodeAndBin\\"+asFileName;
    asUploadPath=IniConfig.asN33_UploadLogPath;

    if(MyForceDirectories(asUploadPath, "[N32] Upload OCR and Bin log")!=1)
    {
        delete ListUploadOCRDataLog;
        return;
    }

    if(asUploadPath.SubString(asUploadPath.Length(), 2)!="\\")
        asUploadPath=asUploadPath+"\\"+asFileName;
    else
        asUploadPath=asUploadPath+asFileName;

    if(DirectoryExists(IniConfig.asN33_UploadLogPath))      //再次確認資料夾是否存在或網路是否連線
    {
        if(FileExists(asUploadPath)==false)
        {
            ListUploadOCRDataLog->Clear();
            ListUploadOCRDataLog->SaveToFile(asUploadPath);
        }

        if(FileExists(asSourcePath))
        {
            ListUploadOCRDataLog->Clear();
            ListUploadOCRDataLog->LoadFromFile(asSourcePath);
            ListUploadOCRDataLog->SaveToFile(asUploadPath);
        }
        else
        {
            ShowMyMessage("Source Path is not exists!", "原始資料不存在");
        }
    }
    else
    {
        ShowMyMessage("Network path is not exists or Network disconnected!", "網路路徑不存在或網路未連線");
    }

    delete ListUploadOCRDataLog;
}
//------------------------------------------------------------------------------
