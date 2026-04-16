#include "MachineDefine.h"
#pragma hdrstop

#include "cStartCondition.h"
#include "cprod.h"
#include "common.h"
#include "cSocket.h"
//#include "INPUT.h"
#include "cpublic.h"
#include "cAuthority.h"
#include "cmydef.h"
#include "cSetUp.h"
#include "atester.h"
#include "myQwertyKeyBoard.h"
#include "uLotInfo.h"
#include "cSecurity.h"
#include "csystem.h"
#include "SmartDiagnostic.h"
#include "MyKitSuck.h"
#include "mymessbox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfStartCondition *fStartCondition;
//JerryYang 20170214 (wei) add for socket ID and contact count
//==>
TEdit *SocketID[MAX_SOCKET_ROW][MAX_SOCKET_COL];
TPanel *SocketContactCnt[MAX_SOCKET_ROW][MAX_SOCKET_COL];
TLabel *TestLabelCol[MAX_SOCKET_COL];
TLabel *TestLabelRow[MAX_SOCKET_ROW];
TLabel *TestIDLabelRow[MAX_SOCKET_ROW];
TLabel *TestCountLabelRow[MAX_SOCKET_ROW];
//<==
//JerryYang 20170214 (wei) add for socket ID and contact count
//---------------------------------------------------------------------------
__fastcall TfStartCondition::TfStartCondition(TComponent* Owner)
    : TForm(Owner)
{
    //JerryYang 20170214 (wei) add for socket ID and contact count
    //==>
    TEdit *TempSocketID[MAX_SOCKET_ROW][MAX_SOCKET_COL]=
    {
        {editSocketAa, editSocketAb, editSocketAc, editSocketAd, editSocketAe, editSocketAf, editSocketAg, editSocketAh},
        {editSocketBa, editSocketBb, editSocketBc, editSocketBd, editSocketBe, editSocketBf, editSocketBg, editSocketBh},
        {editSocketCa, editSocketCb, editSocketCc, editSocketCd, editSocketCe, editSocketCf, editSocketCg, editSocketCh},
        {editSocketDa, editSocketDb, editSocketDc, editSocketDd, editSocketDe, editSocketDf, editSocketDg, editSocketDh}
    };
    TPanel *TempSocketContactCnt[MAX_SOCKET_ROW][MAX_SOCKET_COL]=
    {
        {PanelAa, PanelAb, PanelAc, PanelAd, PanelAe, PanelAf, PanelAg, PanelAh},
        {PanelBa, PanelBb, PanelBc, PanelBd, PanelBe, PanelBf, PanelBg, PanelBh},
        {PanelCa, PanelCb, PanelCc, PanelCd, PanelCe, PanelCf, PanelCg, PanelCh},
        {PanelDa, PanelDb, PanelDc, PanelDd, PanelDe, PanelDf, PanelDg, PanelDh}
    };
    TLabel *tempTestLabelCol[MAX_SOCKET_COL]=
    {
        labTestColA, labTestColB, labTestColC, labTestColD, labTestColE, labTestColF, labTestColG, labTestColH
    };
    TLabel *tempTestLabelRow[MAX_SOCKET_ROW]={labTestRowA, labTestRowB, labTestRowC, labTestRowD};
    TLabel *tempLabelIDRow[MAX_SOCKET_ROW]={LabelID_RowA, LabelID_RowB, LabelID_RowC, LabelID_RowD};
    TLabel *tempLabelCountRow[MAX_SOCKET_ROW]={LabelCnt_RowA, LabelCnt_RowB, LabelCnt_RowC, LabelCnt_RowD};
    int iCol, iRow;
    for(int i=0; i<MAX_SOCKET_TOTAL; i++)
    {
        iRow=i/MAX_SOCKET_COL;
        iCol=i%MAX_SOCKET_COL;
        if(i<MAX_SOCKET_ROW)
        {
            TestLabelRow[i]=tempTestLabelRow[i];
            TestLabelRow[i]->Visible=false;
            TestIDLabelRow[i]=tempLabelIDRow[i];
            TestIDLabelRow[i]->Visible=false;
            TestCountLabelRow[i]=tempLabelCountRow[i];
            TestCountLabelRow[i]->Visible=false;
        }
        if(i<MAX_SOCKET_COL)
        {
            TestLabelCol[i]=tempTestLabelCol[i];
            TestLabelCol[i]->Visible=false;
        }
        SocketID[iRow][iCol]=TempSocketID[iRow][iCol];
        SocketID[iRow][iCol]->Visible=false;
        SocketContactCnt[iRow][iCol]=TempSocketContactCnt[iRow][iCol];
        SocketContactCnt[iRow][iCol]->Visible=false;
    }
    //<==
    //JerryYang 20170214 (wei) add for socket ID and contact count
}
//---------------------------------------------------------------------------
void __fastcall TfStartCondition::FormShow(TObject *Sender)
{
    GetCountClrAuth();
    GetLimitAuth();                                                             //JerryYang 20200504 銦片計數下限
    cbLoadingCount_FT->Enabled=authCounterClr[3];
    cbTestCategory_FT->Enabled=authCounterClr[1];
    cbContactCount_FT->Enabled=authCounterClr[4];
    cbSortingCount_FT->Enabled=authCounterClr[6];
    cbScanner_FT     ->Enabled=authCounterClr[2];
    cbAlarmData_FT   ->Enabled=authCounterClr[0];
    cbTimeData_FT    ->Enabled=authCounterClr[7];
    cbLoadingCount_RT->Enabled=authCounterClr[3];
    cbTestCategory_RT->Enabled=authCounterClr[1];
    cbContactCount_RT->Enabled=authCounterClr[4];
    cbSortingCount_RT->Enabled=authCounterClr[6];
    cbScanner_RT     ->Enabled=authCounterClr[2];
    cbAlarmData_RT   ->Enabled=authCounterClr[0];
    cbTimeData_RT    ->Enabled=authCounterClr[7];

    cbLoadingCount_FT->Checked=LastSet.bCTClear[0][0];
    cbTestCategory_FT->Checked=LastSet.bCTClear[0][1];
    cbContactCount_FT->Checked=LastSet.bCTClear[0][2];
    cbSortingCount_FT->Checked=LastSet.bCTClear[0][3];
    cbScanner_FT     ->Checked=LastSet.bCTClear[0][4];
    cbAlarmData_FT   ->Checked=LastSet.bCTClear[0][5];
    cbTimeData_FT    ->Checked=LastSet.bCTClear[0][6];
    cbLoadingCount_RT->Checked=LastSet.bCTClear[1][0];
    cbTestCategory_RT->Checked=LastSet.bCTClear[1][1];
    cbContactCount_RT->Checked=LastSet.bCTClear[1][2];
    cbSortingCount_RT->Checked=LastSet.bCTClear[1][3];
    cbScanner_RT     ->Checked=LastSet.bCTClear[1][4];
    cbAlarmData_RT   ->Checked=LastSet.bCTClear[1][5];
    cbTimeData_RT    ->Checked=LastSet.bCTClear[1][6];

    AnsiString str;

    if(IniConfig.bShowFTandRTButtonCanClick==true &&                            //Steven 20131224 : FT & RT Buttion 可以按
       CUSTOMER_CODE!=CC_SCC)                                                   //Steven 20200309 : SCC要使用
    {
        LastSet.iStartMode=2;
        gbStartMode->Visible=false;
    }
    TEdit *edtInArmPickLifeCnt[2][4]={                //JerryYang 20220331 : add吸嘴作動次數計數
                                     {edtInArmA, edtInArmC, edtInArmE, edtInArmG},
                                     {edtInArmB, edtInArmD, edtInArmF, edtInArmH},
                                     };

    TEdit *edtOutArmPickLifeCnt[2][4]={
                                     {edtOutArmA, edtOutArmC, edtOutArmE, edtOutArmG},
                                     {edtOutArmB, edtOutArmD, edtOutArmF, edtOutArmH},
                                     };

    TEdit *edtArm1PickLifeCnt[2][8]={
                                     {edtArm1Aa, edtArm1Ab, edtArm1Ac, edtArm1Ad, edtArm1Ae, edtArm1Af, edtArm1Ag, edtArm1Ah},
                                     {edtArm1Ba, edtArm1Bb, edtArm1Bc, edtArm1Bd, edtArm1Be, edtArm1Bf, edtArm1Bg, edtArm1Bh},
                                     };

    TEdit *edtArm2PickLifeCnt[2][8]={
                                     {edtArm2Aa, edtArm2Ab, edtArm2Ac, edtArm2Ad, edtArm2Ae, edtArm2Af, edtArm2Ag, edtArm2Ah},
                                     {edtArm2Ba, edtArm2Bb, edtArm2Bc, edtArm2Bd, edtArm2Be, edtArm2Bf, edtArm2Bg, edtArm2Bh},
                                     };
    fSetup->ReadFile();

     if(cbStartModeOnlyFT->Checked==true)                                       //KaiChen 20181206 ：Start Run Mode Only FT
    {
        LastSet.iStartMode=1;
    }

    if(LastSet.iStartMode==0)
        rbStartMode1->Checked=true;
    else if(LastSet.iStartMode==1)
        rbStartMode2->Checked=true;
    else
        rbStartMode3->Checked=true;

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20140918 ASET 不能使用ReTest 功能
    {
        rbStartMode3->Enabled=false;
        rbStartMode2->Checked=true;
    }
    if(CosFunction.bContactAlmNeedOneCycle)                                     //Sam 20241226 : Contact Alarm 需要先做 OneCycle
    {
        sgContactCount->ColCount=4;
        sgContactCount->Width=657;
        if(LastSet.iLanguageCountry==1)
        {
            sgContactCount->Cells[1][0]=" 接觸幾次要發出提示";
            sgContactCount->Cells[2][0]=" 接觸幾次要發出警告";
            sgContactCount->Cells[3][0]=" 目前接觸次數";
        }
        else
        {
            sgContactCount->Cells[1][0]=" Contact times for warning";
            sgContactCount->Cells[2][0]=" Contact times for alarm";
            sgContactCount->Cells[3][0]=" Current Status";
        }
        for(int i=0; i<2; i++)
        {
            if(LastSet.iLanguageCountry==1)
                str.sprintf(" %d 號測試頭接觸次數", i+1);
            else
                str.sprintf(" Head %d Contact ", i+1);
            sgContactCount->Cells[0][1+i]=str;
            sgContactCount->Cells[1][1+i]=" "+AnsiString(TestIF_File.iContactWarningCount[i]);
            sgContactCount->Cells[2][1+i]=" "+AnsiString(LastSet.ContactSet[i]);
            sgContactCount->Cells[3][1+i]=" "+AnsiString(LastSet.iContactCT[i]);   //jou 2012-06-05
        }
    }
    else
    {
        sgContactCount->ColCount=3;
        sgContactCount->Width=513;
        if(LastSet.iLanguageCountry==1)
        {
            sgContactCount->Cells[1][0]=" 接觸幾次要發出警告";
            sgContactCount->Cells[2][0]=" 目前接觸次數";
        }
        else
        {
            sgContactCount->Cells[1][0]=" Contact times for alarm";
            sgContactCount->Cells[2][0]=" Current Status";
        }

        for(int i=0; i<2; i++)
        {
            if(LastSet.iLanguageCountry==1)
                str.sprintf(" %d 號測試頭接觸次數", i+1);
            else
                str.sprintf(" Head %d Contact ", i+1);
            sgContactCount->Cells[0][1+i]=str;
            sgContactCount->Cells[1][1+i]=" "+AnsiString(LastSet.ContactSet[i]);
            sgContactCount->Cells[2][1+i]=" "+AnsiString(LastSet.iContactCT[i]);    //jou 2012-06-05
        }
    }
    //ChungHung 20160420 add for Head Count Start
    sgHeadCondition1->Cells[1][0]=" Contact times for alarm";
    sgHeadCondition1->Cells[2][0]=" Current Status";
    sgHeadCondition1->Cells[3][0]=" Total Contact Count";

    sgHeadCondition2->Cells[1][0]=" Contact times for alarm";
    sgHeadCondition2->Cells[2][0]=" Current Status";
    sgHeadCondition2->Cells[3][0]=" Total Contact Count";

    sgHeadCondition3->Cells[1][0]=" Contact times for alarm";
    sgHeadCondition3->Cells[2][0]=" Current Status";
    sgHeadCondition3->Cells[3][0]=" Total Contact Count";
    //ChungHung 20160420 add for Head Count End

    for(int i=0; i<FTestSuck.iShtRow; i++)                                                      //ChungHung 20160420 add for Head Count
    {
        for(int j=0; j<FTestSuck.iShtCol; j++)
        {
            str.sprintf("Arm%d Head %d Contact ", i+1, j+1);

            if(CUSTOMER_CODE==CC_KYEC_LEE)// || CosFunction.bHiSiliconFunction==true)
            {
                sgHeadCondition1->Cells[1][1+(i*8+j)]=" "+AnsiString(TestIF_File.iContactAlarmCount[0]);
                sgHeadCondition2->Cells[1][1+(i*8+j)]=" "+AnsiString(TestIF_File.iContactAlarmCount[1]);
                sgHeadCondition3->Cells[1][1+(i*8+j)]=" "+AnsiString(TestIF_File.iContactAlarmCount[2]);
            }
            else
            {
                sgHeadCondition1->Cells[1][1+(i*8+j)]=" "+AnsiString(IniConfig.ContactSet[0][i][j]);
                sgHeadCondition2->Cells[1][1+(i*8+j)]=" "+AnsiString(IniConfig.ContactSet[1][i][j]);
                sgHeadCondition3->Cells[1][1+(i*8+j)]=" "+AnsiString(IniConfig.ContactSet[2][i][j]);
            }

            sgHeadCondition1->Cells[0][1+(i*8+j)]=str;

            sgHeadCondition1->Cells[2][1+(i*8+j)]=" "+AnsiString(IniConfig.HeadContactCount[0][i][j]);
            sgHeadCondition1->Cells[3][1+(i*8+j)]=" "+AnsiString(IniConfig.HeadContactCountHistory[0][i][j]);

            sgHeadCondition2->Cells[0][1+(i*8+j)]=str;

            sgHeadCondition2->Cells[2][1+(i*8+j)]=" "+AnsiString(IniConfig.HeadContactCount[1][i][j]);
            sgHeadCondition2->Cells[3][1+(i*8+j)]=" "+AnsiString(IniConfig.HeadContactCountHistory[1][i][j]);

            sgHeadCondition3->Cells[0][1+(i*8+j)]=str;
            sgHeadCondition3->Cells[2][1+(i*8+j)]=" "+AnsiString(IniConfig.HeadContactCount[2][i][j]);
            sgHeadCondition3->Cells[3][1+(i*8+j)]=" "+AnsiString(IniConfig.HeadContactCountHistory[2][i][j]);
        }
    }

    if(CosFunction.bUseHeadContactCount)                                        //Ifor 20160516 京元要求銦片 Life Time 功能
    {
        tsCondition01->TabVisible=IniConfig.bLifeTimeCount[0];                  //Ifor 20190920 : add Config 開啟時才顯是對應的Lift Time Count
        tsCondition02->TabVisible=IniConfig.bLifeTimeCount[1];                  //Ifor 20190920 : add Config 開啟時才顯是對應的Lift Time Count
        tsCondition03->TabVisible=IniConfig.bLifeTimeCount[2];                  //Ifor 20190920 : add Config 開啟時才顯是對應的Lift Time Count
        if(CUSTOMER_CODE==CC_AMD_M)
            TabsSocketID->TabVisible=true;
    }
    else if(CUSTOMER_CODE==CC_ASE_CL ||                                         //JerryYang 20170214 (wei) ASE中壢才顯示Socket contact life time
            CUSTOMER_CODE==CC_PTI ||                                            //Sam 20170526 (jou) PTI要求socket contact計數
            CUSTOMER_CODE==CC_TERAPOWER ||                                      //Sam 20190916 : 晶兆成也記錄測頭 Count
            CUSTOMER_CODE==CC_HANA_MICRON ||                                    //JimmyChiu 20211008 R211005-Hana-H9-01
            CUSTOMER_CODE==CC_ASE_JP)                                           //Sam 20220706 : ASE_JP 也記錄測頭 Count
    {
        tsCondition01->TabVisible=false;
        tsCondition02->TabVisible=false;
        tsCondition03->TabVisible=false;
    }
    else
    {
        tsCondition01->TabVisible=false;
        tsCondition02->TabVisible=false;
        tsCondition03->TabVisible=false;
    }

    TabsSocketID->TabVisible=CosFunction.bUseSocketContactCount;                //Steven 20221216 : Modify for Socket ID
    tsSocketCount->TabVisible=CosFunction.bUseSocketContactCount;               //Sam 20220720 : 新增一組 Socket Count
    tsVibration->TabVisible =(TRAY_VIBRATION==VibrationMotor);                  //Steven 20240904 : 修正Start condition畫面顯示

    if(CUSTOMER_CODE==CC_KYEC_LEE ||
       CosFunction.bHiSiliconFunction==true)
    {
        if(pgLifeTime->ActivePage==tsCondition01)                               //Active Page 在第1頁
        {
            if(IniConfig.bLifeTimeCount[0]==false)                              //O12 功能沒開啟 切換至別頁面顯示
            {
                if(IniConfig.bLifeTimeCount[1]==true)                           //O13 功能開啟 切換Active Page 至第2頁
                {
                    pgLifeTime->ActivePage=tsCondition02;
                }
                else if(IniConfig.bLifeTimeCount[2]==true)                      //O14 功能開啟 切換Active Page 至第3頁
                {
                    pgLifeTime->ActivePage=tsCondition03;
                }
            }
        }
        else if(pgLifeTime->ActivePage==tsCondition02)                          //Active Page 在第2頁
        {
            if(IniConfig.bLifeTimeCount[1]==false)                              //O13 功能沒開啟 切換至別頁面顯示
            {
                if(IniConfig.bLifeTimeCount[0]==true)                           //O12 功能開啟 切換Active Page 至第1頁
                {
                    pgLifeTime->ActivePage=tsCondition01;
                }
                else if(IniConfig.bLifeTimeCount[2]==true)                      //O14 功能開啟 切換Active Page 至第3頁
                {
                    pgLifeTime->ActivePage=tsCondition03;
                }
            }
        }
        else if(pgLifeTime->ActivePage==tsCondition03)                          //Active Page 在第3頁
        {
            if(IniConfig.bLifeTimeCount[2]==false)                              //O14 功能沒開啟 切換至別頁面顯示
            {
                if(IniConfig.bLifeTimeCount[0]==true)                           //O12 功能開啟 切換Active Page 至第1頁
                {
                    pgLifeTime->ActivePage=tsCondition01;
                }
                else if(IniConfig.bLifeTimeCount[1]==true)                      //O13 功能開啟 切換Active Page 至第2頁
                {
                    pgLifeTime->ActivePage=tsCondition02;
                }
            }
        }

        if(pgLifeTime->ActivePage==tsCondition01)
        {
            editContactCountAlarm->Text=AnsiString(TestIF_File.iContactAlarmCount[0]);
        }
        else if(pgLifeTime->ActivePage==tsCondition02)
        {
            editContactCountAlarm->Text=AnsiString(TestIF_File.iContactAlarmCount[1]);
        }
        else if(pgLifeTime->ActivePage==tsCondition03)
        {
            editContactCountAlarm->Text=AnsiString(TestIF_File.iContactAlarmCount[2]);
        }
        else if(pgLifeTime->ActivePage==TabsSocketID)                           //Steven 20200211 : Add socket count for ASE-CL
        {
            editContactCountAlarm->Text=AnsiString(TestIF_File.iContactAlarmCount[3]);
        }

        if(AccessLevel<iDefHonPrecLevel)
        {
            editContactCountAlarm->Enabled=false;
        }
        else
        {
            editContactCountAlarm->Enabled=true;
        }
    }
    else
    {
        labContactCountAlarm->Visible=(CUSTOMER_CODE==CC_ASE_CL || CUSTOMER_CODE==CC_PTI);
        editContactCountAlarm->Visible=(CUSTOMER_CODE==CC_ASE_CL || CUSTOMER_CODE==CC_PTI);

        if(CUSTOMER_CODE==CC_ASE_CL || CUSTOMER_CODE==CC_PTI)    //Steven 20200211 : Add socket count for ASE-CL
        {
            editContactCountAlarm->Text=AnsiString(TestIF_File.iContactAlarmCount[3]);
        }
    }

    tsCondition01->Caption=IniConfig.ContactConditionName[0];
    tsCondition02->Caption=IniConfig.ContactConditionName[1];
    tsCondition03->Caption=IniConfig.ContactConditionName[2];

    Left=75;                                                                    //Steven 20091103
    Top=10;                                                                     //Steven 20091103

    for(int i=0; i<MAX_SOCKET_ROW; i++)                                         //JerryYang 20170214 (wei) add for socket ID and contact count
    {
        TestLabelRow[i]->Visible=false;
        TestIDLabelRow[i]->Visible=false;
        TestCountLabelRow[i]->Visible=false;
        for(int j=0; j<MAX_SOCKET_COL; j++)
        {
            SocketID[i][j]->Text="";
            SocketID[i][j]->Visible=false;
            TestLabelCol[j]->Visible=false;
            SocketContactCnt[i][j]->Visible=false;
        }
    }

    for(int j=0; j<TestSocket.iShtCol; j++)                                     //JerryYang 20170206
    {
        TestLabelCol[j]->Visible=true;
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            SocketID[i][j]->Visible=true;
            SocketID[i][j]->Enabled=true;
            SocketID[i][j]->Text=AnsiString(LastSet.strSocketID[i][j]);
            SocketContactCnt[i][j]->Visible=true;
            SocketContactCnt[i][j]->Enabled=true;
            SocketContactCnt[i][j]->Caption=AnsiString(LastSet.iSocketContactCount[i][j]);
            TestLabelRow[i]->Visible=true;
            TestIDLabelRow[i]->Visible=true;
            TestCountLabelRow[i]->Visible=true;
        }
    }

    palVibratorHP1->Caption=IniConfig.iVibratorHP1;                             //JerryYang 20200612 振動馬達作動時間累計
    palVibratorSht1->Caption=IniConfig.iVibratorSht1;
    palVibratorSht2->Caption=IniConfig.iVibratorSht2;
    palVibratorUnloader->Caption=IniConfig.iVibratorUnloader;

    edtInOutArmPickerAlmCnt->Text=TestIF_File.InOutArmLifeCntSet;               //JerryYang 20220331 : add吸嘴作動次數計數
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<4; j++)
        {
            edtInArmPickLifeCnt[i][j]->Text=TestIF_File.InArmPickerLifeCnt[i][j];
            edtOutArmPickLifeCnt[i][j]->Text=TestIF_File.OutArmPickerLifeCnt[i][j];
        }
    }

    for(int i=0; i<2; i++)
    {
        for(int j=0; j<8; j++)
        {
            edtArm1PickLifeCnt[i][j]->Text=TestIF_File.Arm1PickerLifeCnt[i][j];
            edtArm2PickLifeCnt[i][j]->Text=TestIF_File.Arm2PickerLifeCnt[i][j];
        }
    }

    if(CUSTOMER_CODE==CC_SIGURD_HUKOU)                                          //KaiChen 20181206 ：Start Run Mode Only FT
    {
        if(LastSet.iRunStartMode==rsmInitialStart ||
           LastSet.iRunStartMode==rsmCInitialRetest)                            //KaiChen 20181211 ：增加卡關 RunStartMode
        {
            tsFunction->TabVisible=true;
        }
        else
        {
            tsFunction->TabVisible=false;
        }
    }
    else
    {
        tsFunction->TabVisible=false;
    }

    if(CosFunction.bCylinderOnOffTimeLog)                                       //JerryYang 20220215 : cylinder作動次數計數
    {
        tsSmartDiagnostic->TabVisible=true;
    }
    else
    {
        tsSmartDiagnostic->TabVisible=false;
    }

    if(CosFunction.bUseSocketContactCount)                                      //Sam 20220720 : 新增一組 Socket Count
    {
        sgSocketCount->Cells[1][0]=" Contact times for alarm";
        sgSocketCount->Cells[2][0]=" Current Status";
        sgSocketCount->Cells[3][0]=" Total Contact Count";

        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                str.sprintf("Socket %s Contact ", IndexSuckName[i][j]);
                sgSocketCount->Cells[0][1+(i*TestSocket.iShtCol+j)]=str;
                sgSocketCount->Cells[1][1+(i*TestSocket.iShtCol+j)]=" "+AnsiString(IniConfig.SocketContactSet[i][j]);
                sgSocketCount->Cells[2][1+(i*TestSocket.iShtCol+j)]=" "+AnsiString(IniConfig.SocketContactCount[i][j]);
                sgSocketCount->Cells[3][1+(i*TestSocket.iShtCol+j)]=" "+AnsiString(IniConfig.SocketContactCountHistory[i][j]);
            }
        }
    }

    fShow=true;
    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20100629
}
//---------------------------------------------------------------------------
void __fastcall TfStartCondition::FormClose(TObject *Sender, TCloseAction &Action)
{
    sbSaveClick(this);
    if(HasICUnderMachine()==false)                                              //Steven 20210309 : 避免生產中被修改模式
    {
        if(rbStartMode1->Checked)
            LastSet.iStartMode=0;
        else if(rbStartMode2->Checked)
            LastSet.iStartMode=1;
        else
            LastSet.iStartMode=2;
    }
    fShow=false;

    for(int i=0; i<TestSocket.iMaxRow; i++)
    {
        for(int j=0; j<TestSocket.iMaxCol; j++)
        {
            fLotInfo->edSocket[i][j]->Text=AnsiString(LastSet.strSocketID[i][j]);
        }
    }
    //Sam 20210615 : 修正 Count Alarm 寫入 ini 需要讀取回來。
    //==>
    AnsiString S;
    S=GetLastOpenFN();
    AnsiString szDir="";

    if(IniConfig.bD05_1SaveSocketCntByHandler)                                  //Steven 20250807 : By handler save contact count
    {
        szDir="D:\\HT9045\\IniData\\SocketCount.ini";
    }
    else
    {
        szDir=GetRecipeFileName("HandlerCondition.Data");
    }

    TestIF_File.iContactAlarmCount[0]=CheckAndReadIniData(szDir, "HeadCondition",       "HeadContactSet[1]",   6000);   //Ifor 20200306 : add 讀不到檔案需寫入預設值
    TestIF_File.iContactAlarmCount[1]=CheckAndReadIniData(szDir, "HeadCondition",       "HeadContactSet[2]",   6000);   //Ifor 20200306 : add 讀不到檔案需寫入預設值
    TestIF_File.iContactAlarmCount[2]=CheckAndReadIniData(szDir, "HeadCondition",       "HeadContactSet[3]",   6000);   //Ifor 20200306 : add 讀不到檔案需寫入預設值
    TestIF_File.iContactAlarmCount[3]=CheckAndReadIniData(szDir, "HeadCondition",       "HeadContactSet[4]",   0);      //Ifor 20200306 : add 讀不到檔案需寫入預設值
    //<==
    //Sam 20210615 : 修正 Count Alarm 寫入 ini 需要讀取回來。

    DoIniDataToForm();                                                          //JerryYang 20250411 : 離開頁面要刷新一次, 避免誤存檔
    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);    //Steven 20100629
}
//---------------------------------------------------------------------------
void __fastcall TfStartCondition::sbSameAsHead1Click(TObject *Sender)
{
    if(CosFunction.bContactAlmNeedOneCycle) //Sam 20241226 : Contact Alarm 需要先做 OneCycle
    {
        for(int i=1; i<2; i++)
        {
            sgContactCount->Cells[1][i+1]=sgContactCount->Cells[1][1];
            sgContactCount->Cells[2][i+1]=sgContactCount->Cells[2][1];
        }
    }
    else
    {
        for(int i=1; i<2; i++)
            sgContactCount->Cells[1][i+1]=sgContactCount->Cells[1][1];
    }
}
//---------------------------------------------------------------------------
void __fastcall TfStartCondition::sbClearCountClick(TObject *Sender)
{
   if(CosFunction.bContactAlmNeedOneCycle)                                      //Sam 20241226 : Contact Alarm 需要先做 OneCycle
    {
        for(int i=0; i<2; i++)
        {
            sgContactCount->Cells[3][1+i]="0";
            LastSet.iContactCT[i]=0;
        }
    }
    else
    {
        for(int i=0; i<2; i++)
        {
            sgContactCount->Cells[2][1+i]="0";
            LastSet.iContactCT[i]=0;                                            //jou 2012-06-05
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfStartCondition::sbSaveClick(TObject *Sender)
{
    AnsiString szDir;
    if(CosFunction.bContactAlmNeedOneCycle)                                     //Sam 20241226 : Contact Alarm 需要先做 OneCycle
    {
        if(IniConfig.bD05_1SaveSocketCntByHandler)                              //Steven 20250807 : By handler save contact count
        {
            szDir="D:\\HT9045\\IniData\\SocketCount.ini";
        }
        else
        {
            szDir=GetRecipeFileName("HandlerCondition.Data");
        }

        WriteIniData(szDir, "HeadCondition",       "HandContactWarningSet[0]",   sgContactCount->Cells[1][1]);
        WriteIniData(szDir, "HeadCondition",       "HandContactWarningSet[1]",   sgContactCount->Cells[1][2]);
        for(int i=0; i<2; i++)                                                  //jou 2012-06-05
        {
            LastSet.ContactSet[i]=atoi(sgContactCount->Cells[2][i+1].c_str());
            LastSet.iContactCT[i]=atoi(sgContactCount->Cells[3][i+1].c_str());
        }
    }
    else
    {
        for(int i=0; i<2; i++)                                                  //jou 2012-06-05
        {
            LastSet.ContactSet[i]=atoi(sgContactCount->Cells[1][i+1].c_str());
            LastSet.iContactCT[i]=atoi(sgContactCount->Cells[2][i+1].c_str());
        }
    }
    WriteLastDataFile();                                                        //Steven 20231017 : add
}
//---------------------------------------------------------------------------
void __fastcall TfStartCondition::sgContactCountSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
    if(ACol==0 || ARow==0)
        return;
    edTemp->Text=sgContactCount->Cells[ACol][ARow];
    fQwertyKey->ShowQwertyKey(edTemp, N_INTEGER, 0, true, 2000000, 0);          //Jimmychiu 20231006 : Fixed for Contact count quantity not set to 0
    sgContactCount->Cells[ACol][ARow]=edTemp->Text;
}
//---------------------------------------------------------------------------
void __fastcall TfStartCondition::spbExitClick(TObject *Sender)
{
    LastSet.bCTClear[0][0]=cbLoadingCount_FT->Checked;
    LastSet.bCTClear[0][1]=cbTestCategory_FT->Checked;
    LastSet.bCTClear[0][2]=cbContactCount_FT->Checked;
    LastSet.bCTClear[0][3]=cbSortingCount_FT->Checked;
    LastSet.bCTClear[0][4]=cbScanner_FT->Checked;
    LastSet.bCTClear[0][5]=cbAlarmData_FT->Checked;
    LastSet.bCTClear[0][6]=cbTimeData_FT->Checked;
    LastSet.bCTClear[1][0]=cbLoadingCount_RT->Checked;
    LastSet.bCTClear[1][1]=cbTestCategory_RT->Checked;
    LastSet.bCTClear[1][2]=cbContactCount_RT->Checked;
    LastSet.bCTClear[1][3]=cbSortingCount_RT->Checked;
    LastSet.bCTClear[1][4]=cbScanner_RT->Checked;
    LastSet.bCTClear[1][5]=cbAlarmData_RT->Checked;
    LastSet.bCTClear[1][6]=cbTimeData_RT->Checked;

    Close();
    WriteLastDataFile();
}
//---------------------------------------------------------------------------
void __fastcall TfStartCondition::sbHeadCondition1ClearClick(
      TObject *Sender)
{
    TSpeedButton *PSpeedButton=dynamic_cast <TSpeedButton *>(Sender);
    int iTag=PSpeedButton->Tag;

    if(pgLifeTime->ActivePage==tsCondition01)
        iTag=0;
    else if(pgLifeTime->ActivePage==tsCondition02)
        iTag=1;
    else if(pgLifeTime->ActivePage==tsCondition03)
        iTag=2;
//JerryYang 20170214 (wei) 清除socket ID及socket contact count
//==>
    else if(pgLifeTime->ActivePage==TabsSocketID)
        iTag=3;
    else if(pgLifeTime->ActivePage==tsVibration)                                //JerryYang 20200612 振動馬達作動時間累計
        iTag=4;
    else if(pgLifeTime->ActivePage==tsSocketCount)                              //Sam 20220720 : 新增一組 Socket Count
        iTag=5;
    else if(pgLifeTime->ActivePage==tsPickerLifeTime)                           //Steven 20230110 : 清除Picker Count
        iTag=6;

    if(iTag>=0 && iTag<=2)                                                      //JerryYang 20170214 (wei) HeadCondition1~3
    {
        TStringGrid *StrGrid[]={sgHeadCondition1, sgHeadCondition2, sgHeadCondition3};

        for(int i=0; i<16; i++)
            StrGrid[iTag]->Cells[2][1+i]="0";
//        sbHeadCondition1SaveClick(this);                                      //JerryYang 20200214 Mark掉, 按clear時先不要存檔
    }
    else if(iTag==3)
    {
        for(int i=0; i<TestSocket.iMaxRow; i++)
        {
            for(int j=0; j<TestSocket.iMaxCol; j++)
            {
                SocketID[i][j]->Text="";
                LastSet.iSocketContactCount[i][j]=0;
                SocketContactCnt[i][j]->Caption="0";
            }
        }
    }
    else if(iTag==4)                                                            //JerryYang 20200612 振動馬達作動時間累計
    {
        palVibratorHP1->Caption="0";
        palVibratorSht1->Caption="0";
        palVibratorSht2->Caption="0";
        palVibratorUnloader->Caption="0";

        IniConfig.iVibratorHP1=0;
        IniConfig.iVibratorSht1=0;
        IniConfig.iVibratorSht2=0;
        IniConfig.iVibratorUnloader=0;
        RecordProcess("Clear all vibrator time");
    }
    else if(iTag==5)                                                            //Sam 20220720 : 新增一組 Socket Count
    {
        for(int i=0; i<TestSocket.iShtCnt; i++)
            sgSocketCount->Cells[2][1+i]="0";
    }
    else if(iTag==6)                                                            //Steven 20230110 : 清除Picker Count
    {
        ClearPickerCount();
    }
//<==
//JerryYang 20170214 (wei) 清除socket ID及socket contact count
}
//------------------------------------------------------------------------------
void TfStartCondition::ReadWriteStartCondition(bool bRead)                      //Steven 20250822 : Socket ID存檔讀檔
{
    AnsiString str1, str2, str3;
    AnsiString szDir="";
    int iPage=0;
    int iSetCount=0;

    TStringGrid *StrGrid[]={sgHeadCondition1, sgHeadCondition2, sgHeadCondition3};

    TEdit *edtInArmPickLifeCnt[2][4]={                                          //JerryYang 20220331 : add吸嘴作動次數計數
                                     {edtInArmA, edtInArmC, edtInArmE, edtInArmG},
                                     {edtInArmB, edtInArmD, edtInArmF, edtInArmH},
                                     };

    TEdit *edtOutArmPickLifeCnt[2][4]={
                                     {edtOutArmA, edtOutArmC, edtOutArmE, edtOutArmG},
                                     {edtOutArmB, edtOutArmD, edtOutArmF, edtOutArmH},
                                     };

    TEdit *edtArm1PickLifeCnt[2][8]={
                                     {edtArm1Aa, edtArm1Ab, edtArm1Ac, edtArm1Ad, edtArm1Ae, edtArm1Af, edtArm1Ag, edtArm1Ah},
                                     {edtArm1Ba, edtArm1Bb, edtArm1Bc, edtArm1Bd, edtArm1Be, edtArm1Bf, edtArm1Bg, edtArm1Bh},
                                     };

    TEdit *edtArm2PickLifeCnt[2][8]={
                                     {edtArm2Aa, edtArm2Ab, edtArm2Ac, edtArm2Ad, edtArm2Ae, edtArm2Af, edtArm2Ag, edtArm2Ah},
                                     {edtArm2Ba, edtArm2Bb, edtArm2Bc, edtArm2Bd, edtArm2Be, edtArm2Bf, edtArm2Bg, edtArm2Bh},
                                     };

    if(IniConfig.bD05_1SaveSocketCntByHandler)                                  //Steven 20250807 : By handler save contact count
    {
        szDir="D:\\HT9045\\IniData\\SocketCount.ini";
    }
    else
    {
        szDir=GetRecipeFileName("HandlerCondition.Data");
    }

    if(bRead)
    {
        TestIF_File.iContactAlarmCount[0]=CheckAndReadIniData(szDir, "HeadCondition",       "HeadContactSet[1]",   6000);   //Ifor 20200306 : add 讀不到檔案需寫入預設值
        TestIF_File.iContactAlarmCount[1]=CheckAndReadIniData(szDir, "HeadCondition",       "HeadContactSet[2]",   6000);
        TestIF_File.iContactAlarmCount[2]=CheckAndReadIniData(szDir, "HeadCondition",       "HeadContactSet[3]",   6000);
        TestIF_File.iContactAlarmCount[3]=CheckAndReadIniData(szDir, "HeadCondition",       "HeadContactSet[4]",   0);

        edtKitNo1->Text=CheckAndReadIniData(szDir, "HeadCondition", "Kit No 1", AnsiString(""));
        edtKitNo2->Text=CheckAndReadIniData(szDir, "HeadCondition", "Kit No 2", AnsiString(""));
        edtKitNo3->Text=CheckAndReadIniData(szDir, "HeadCondition", "Kit No 3", AnsiString(""));

        if(CosFunction.bContactAlmNeedOneCycle)                                 //Sam 20241226 : Contact Alarm 需要先做 OneCycle
        {
            TestIF_File.iContactWarningCount[0]=CheckAndReadIniData(szDir, "HeadCondition",       "HandContactWarningSet[0]",   0);
            TestIF_File.iContactWarningCount[1]=CheckAndReadIniData(szDir, "HeadCondition",       "HandContactWarningSet[1]",   0);
        }
    }
    else
    {
        if(CUSTOMER_CODE==CC_KYEC_LEE ||
           CosFunction.bHiSiliconFunction==true ||                              //Ifor 20190927 : add
           CosFunction.bUseHeadContactCount==true)
        {
            if(pgLifeTime->ActivePage==tsCondition01)
                iPage=0;
            else if(pgLifeTime->ActivePage==tsCondition02)
                iPage=1;
            else if(pgLifeTime->ActivePage==tsCondition03)
                iPage=2;
            str1.sprintf("HeadContactSet[%d]", iPage+1);
            iSetCount=atoi(editContactCountAlarm->Text.c_str());
            if(iSetCount>InputLimit.iContactCntAlm)                             //JerryYang 20200504 銦片計數限制
                iSetCount=InputLimit.iContactCntAlm;
            else if(iSetCount<0)
                iSetCount=0;
            editContactCountAlarm->Text=iSetCount;

            TestIF_File.iContactAlarmCount[iPage]=iSetCount;
            WriteIniData(szDir, "HeadCondition", str1, iSetCount);
        }

        if(CUSTOMER_CODE==CC_ASE_CL || CUSTOMER_CODE==CC_PTI)                   //Steven 20200211 : Add socket count for ASE-CL
        {
            WriteIniData(szDir, "HeadCondition", "HeadContactSet[4]", atoi(editContactCountAlarm->Text.c_str()));
        }

        WriteIniData(szDir, "HeadCondition", "Kit No 1", edtKitNo1->Text);
        WriteIniData(szDir, "HeadCondition", "Kit No 2", edtKitNo2->Text);
        WriteIniData(szDir, "HeadCondition", "Kit No 3", edtKitNo3->Text);
    }

    for(int i=0; i<2; i++)
    {
        for(int j=0; j<8; j++)
        {
            if(CUSTOMER_CODE==CC_KYEC_LEE ||
               CosFunction.bHiSiliconFunction==true ||
               CosFunction.bUseHeadContactCount==true)
            {
                sgHeadCondition1->Cells[1][1+(i*8+j)]=" "+AnsiString(TestIF_File.iContactAlarmCount[0]);
                sgHeadCondition2->Cells[1][1+(i*8+j)]=" "+AnsiString(TestIF_File.iContactAlarmCount[1]);
                sgHeadCondition3->Cells[1][1+(i*8+j)]=" "+AnsiString(TestIF_File.iContactAlarmCount[2]);
            }
            else
            {
                sgHeadCondition1->Cells[1][1+(i*8+j)]=" "+AnsiString(IniConfig.ContactSet[0][i][j]);
                sgHeadCondition2->Cells[1][1+(i*8+j)]=" "+AnsiString(IniConfig.ContactSet[1][i][j]);
                sgHeadCondition3->Cells[1][1+(i*8+j)]=" "+AnsiString(IniConfig.ContactSet[2][i][j]);
            }
        }
    }

    for(int iTag=0; iTag<3; iTag++)
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<8; j++)
            {
                IniConfig.ContactSet[iTag][i][j]      =atoi(StrGrid[iTag]->Cells[1][i*8+j+1].c_str());
                if(IniConfig.HeadContactCount[iTag][i][j]!=atoi(StrGrid[iTag]->Cells[2][i*8+j+1].c_str()))  //JerryYang 20200214 add change log
                {
                    str2.sprintf("%s Arm%d Head %d Contact",  IniConfig.ContactConditionName[iTag],i+1, j+1);
                    str3.sprintf("%d -> %s", IniConfig.HeadContactCount[iTag][i][j], StrGrid[iTag]->Cells[2][i*8+j+1]);
                    RecordChangeLogProcess(str2, str3);
                }

                if(bRead==false)                                                //RogerYang 20260203 : 修正formshow裡的fSetup->ReadFile()又調用ReadWriteFile()會造成銦片計數異常
                {
                    IniConfig.HeadContactCount[iTag][i][j]=atoi(StrGrid[iTag]->Cells[2][i*8+j+1].c_str());
                }
            }
        }
    }

//    if(bRead==false)
//    {
//        TestIF_File.InOutArmLifeCntSet=atoi(edtInOutArmPickerAlmCnt->Text.c_str());
//        WriteIniData(szDir, "O_Count", "O_20InOutArmLifeCntSet", TestIF_File.InOutArmLifeCntSet);
//    }
    TestIF_File.InOutArmLifeCntSet=ReadWriteIni(szDir, "O_Count", "O_20InOutArmLifeCntSet", atoi(edtInOutArmPickerAlmCnt->Text.c_str()), 100000, bRead);  //RogerYang 20260205 : 修正設定值異常

    for(int i=0; i<2; i++)                                                      //JerryYang 20220331 : add 吸真空次數計數
    {
        for(int j=0; j<4; j++ )
        {
            str1.sprintf("InArmSuckCnt%d_%d", i, j);
            str2.sprintf("OutArmSuckCnt%d_%d", i, j);
            TestIF_File.InArmPickerLifeCnt[i][j] =ReadWriteIni(szDir, "O_Count", str1, atoi(edtInArmPickLifeCnt[i][j]->Text.c_str()),  0, bRead);
            TestIF_File.OutArmPickerLifeCnt[i][j]=ReadWriteIni(szDir, "O_Count", str2, atoi(edtOutArmPickLifeCnt[i][j]->Text.c_str()), 0, bRead);
        }
    }

    for(int i=0; i<2; i++)                                                      //JerryYang 20220331 : add 吸真空次數計數
    {
        for(int j=0; j<8; j++ )
        {
            str1.sprintf("Arm1SuckCnt%d_%d", i, j);
            str2.sprintf("Arm2SuckCnt%d_%d", i, j);
            TestIF_File.Arm1PickerLifeCnt[i][j]=ReadWriteIni(szDir, "O_Count", str1, atoi(edtArm1PickLifeCnt[i][j]->Text.c_str()), 0, bRead);
            TestIF_File.Arm2PickerLifeCnt[i][j]=ReadWriteIni(szDir, "O_Count", str2, atoi(edtArm2PickLifeCnt[i][j]->Text.c_str()), 0, bRead);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfStartCondition::sbHeadCondition1SaveClick(
      TObject *Sender)
{
    if(IniConfig.bA02DisableSaveParsWhenSwitchToOp==true &&                     //RogerYang 20260305 張寧說時間到都要登出，且不可被更改任何設定(A01_2)
        AccessLevel==0)
    {
        ShowMyMessage("[A01_2]目前已切換到Operator權限，\r\n請重新登入再做設定!");
        Close();
    }

    ReadWriteStartCondition(false);

    for(int i=0; i<4; i++)                                                      //JerryYang 20170214 (wei) 存socket ID及socket contact count
    {
        for(int j=0; j<8; j++)
        {
            strncpy(LastSet.strSocketID[i][j], SocketID[i][j]->Text.c_str(), sizeof(LastSet.strSocketID[i][j]));
            LastSet.iSocketContactCount[i][j]=atoi(SocketContactCnt[i][j]->Caption.c_str());
        }
    }

    DoIniDataToForm();

    for(int i=0; i<4; i++)                                                      //Sam 20220720 : 新增一組 Socket Count
    {
        for(int j=0; j<8; j++)
        {
            IniConfig.SocketContactSet[i][j]        =atoi(sgSocketCount->Cells[1][i*8+j+1].c_str());
            IniConfig.SocketContactCount[i][j]      =atoi(sgSocketCount->Cells[2][i*8+j+1].c_str());
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfStartCondition::sgHeadCondition1SelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
    int ret=0;
    TStringGrid *PStrGrid = dynamic_cast <TStringGrid *>(Sender);
    int iTag=PStrGrid->Tag;
    TStringGrid *StrGrid[]={sgHeadCondition1, sgHeadCondition2, sgHeadCondition3, sgSocketCount};   //Sam 20220720 : 新增一組 Socket Count

    if(ACol<=1 || ARow==0 || ACol>2)                                            //Steven 20210623 : ACol==0 --> ACol<=1
        return;

    if(CUSTOMER_CODE==CC_AMD_M && ACol==2 && ARow>=1)
    {
        ret=ShowMyMessageBox_YES_NO("Config data reset to zero?", "確定資料要重置為零？");
        if(ret==1)
        {
            edTemp->Text=0;
            StrGrid[iTag]->Cells[ACol][ARow]=edTemp->Text;
            return;
        }
        else
        {
            edTemp->Text=StrGrid[iTag]->Cells[ACol][ARow];
        }
    }
    else
    {
        edTemp->Text=StrGrid[iTag]->Cells[ACol][ARow];
    }
    fQwertyKey->ShowQwertyKey(edTemp, N_INTEGER, 0, true, 0, 999999);
    StrGrid[iTag]->Cells[ACol][ARow]=edTemp->Text;
}
//---------------------------------------------------------------------------
void __fastcall TfStartCondition::DoIniDataToForm()                             //JerryYang 20170214 (wei) DoIniDataToForm
{
    for(int i=0; i<TestSocket.iShtRow; i++)                                     //JerryYang 20170206
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            SocketID[i][j]->Text=AnsiString(LastSet.strSocketID[i][j]);
            SocketContactCnt[i][j]->Caption=AnsiString(LastSet.iSocketContactCount[i][j]);
        }
    }

    for(int i=0; i<TestSocket.iMaxRow; i++)
    {
        for(int j=0; j<TestSocket.iMaxCol; j++)
        {
            fLotInfo->edSocket[i][j]->Text=AnsiString(LastSet.strSocketID[i][j]);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfStartCondition::pgLifeTimeChange(TObject *Sender)
{
    if(pgLifeTime->ActivePage==tsCondition01)
    {
        editContactCountAlarm->Text=AnsiString(TestIF_File.iContactAlarmCount[0]);
    }
    else if(pgLifeTime->ActivePage==tsCondition02)
    {
        editContactCountAlarm->Text=AnsiString(TestIF_File.iContactAlarmCount[1]);
    }
    else if(pgLifeTime->ActivePage==tsCondition03)
    {
        editContactCountAlarm->Text=AnsiString(TestIF_File.iContactAlarmCount[2]);
    }
    else if(pgLifeTime->ActivePage==TabsSocketID)                               //Steven 20200211 : Add socket count for ASE-CL
    {
        editContactCountAlarm->Text=AnsiString(TestIF_File.iContactAlarmCount[3]);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfStartCondition::editContactCountAlarmMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, InputLimit.iContactCntAlm, 0); //kevin 20170922 (wei) 小鍵盤上下限  //JerryYang 20200504 銦片計數限制
}
//---------------------------------------------------------------------------
void __fastcall TfStartCondition::PanelAaDblClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 2000000, 0);    //Steven 20200211 : Add socket count for ASE-CL
}
//---------------------------------------------------------------------------
void __fastcall TfStartCondition::cbStartModeOnlyFTClick(TObject *Sender)
{
    if(cbStartModeOnlyFT->Checked==true)
    {
        LastSet.iStartMode=1;
    }
    else if(IniConfig.bShowFTandRTButtonCanClick==true)
    {
        LastSet.iStartMode=2;
    }

    if(LastSet.iStartMode==0)
        rbStartMode1->Checked=true;
    else if(LastSet.iStartMode==1)
        rbStartMode2->Checked=true;
    else
        rbStartMode3->Checked=true;
}
//---------------------------------------------------------------------------
void __fastcall TfStartCondition::sb_Maintenance_SmartDiagnosticFunctionClick(
      TObject *Sender)
{
    fSmartDiagnostic->ShowModal();                                              //JerryYang 20220215 : cylinder作動次數計數
}
//---------------------------------------------------------------------------
void TfStartCondition::ClearPickerCount(int iArea, int iTag)                    //Steven 20230110 : 清除Picker Count
{                                                                               //iArea -1: all area, 0: in arm, 1: out arm, 2: Index arm 1, 3: Index arm 2
    TEdit *edtInArmPickLifeCnt[8]={edtInArmA, edtInArmC, edtInArmE, edtInArmG,
                                   edtInArmB, edtInArmD, edtInArmF, edtInArmH};

    TEdit *edtOutArmPickLifeCnt[8]={edtOutArmA, edtOutArmC, edtOutArmE, edtOutArmG,
                                    edtOutArmB, edtOutArmD, edtOutArmF, edtOutArmH};

    TEdit *edtArm1PickLifeCnt[16]={edtArm1Aa, edtArm1Ab, edtArm1Ac, edtArm1Ad, edtArm1Ae, edtArm1Af, edtArm1Ag, edtArm1Ah,
                                   edtArm1Ba, edtArm1Bb, edtArm1Bc, edtArm1Bd, edtArm1Be, edtArm1Bf, edtArm1Bg, edtArm1Bh};

    TEdit *edtArm2PickLifeCnt[16]={edtArm2Aa, edtArm2Ab, edtArm2Ac, edtArm2Ad, edtArm2Ae, edtArm2Af, edtArm2Ag, edtArm2Ah,
                                   edtArm2Ba, edtArm2Bb, edtArm2Bc, edtArm2Bd, edtArm2Be, edtArm2Bf, edtArm2Bg, edtArm2Bh};

    if(iArea==0)        //0: in arm
    {
        edtInArmPickLifeCnt[iTag]->Text="0";
    }
    else if(iArea==1)   //1: out arm
    {
        edtOutArmPickLifeCnt[iTag]->Text="0";
    }
    else if(iArea==2)   //2: Index arm 1
    {
        edtArm1PickLifeCnt[iTag]->Text="0";
    }
    else if(iArea==3)   //3: Index arm 2
    {
        edtArm2PickLifeCnt[iTag]->Text="0";
    }
    else
    {
        for(int i=0; i<8; i++)
        {
            edtInArmPickLifeCnt[i]->Text="0";
            edtOutArmPickLifeCnt[i]->Text="0";
        }

        for(int i=0; i<16; i++)
        {
            edtArm1PickLifeCnt[i]->Text="0";
            edtArm2PickLifeCnt[i]->Text="0";
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfStartCondition::btnInArmAClick(TObject *Sender)               //JerryYang 20220331 : add吸嘴作動次數計數
{
    TButton *Ptr;
    Ptr=(TButton *) Sender;
    ClearPickerCount(0, Ptr->Tag);
}
//---------------------------------------------------------------------------
void __fastcall TfStartCondition::btnOutArmAClick(TObject *Sender)              //JerryYang 20220331 : add吸嘴作動次數計數
{
    TButton *Ptr;
    Ptr=(TButton *) Sender;
    ClearPickerCount(1, Ptr->Tag);
}
//---------------------------------------------------------------------------
void __fastcall TfStartCondition::btnArm2AaClick(TObject *Sender)               //JerryYang 20220331 : add吸嘴作動次數計數
{
    TButton *Ptr;
    Ptr=(TButton *) Sender;
    ClearPickerCount(3, Ptr->Tag);
}
//---------------------------------------------------------------------------
void __fastcall TfStartCondition::btnArm1AaClick(TObject *Sender)               //JerryYang 20220331 : add吸嘴作動次數計數
{
    TButton *Ptr;
    Ptr=(TButton *) Sender;
    ClearPickerCount(2, Ptr->Tag);
}
//---------------------------------------------------------------------------
void __fastcall TfStartCondition::WritePickerCount()                            //JerryYang 20220331 : add吸嘴作動次數計數
{
    if(IniConfig.bO20InOutArmPickerLifeTimeCount==false)
        return;

    AnsiString S="";
    AnsiString str1, str2, str3;
    S=GetLastOpenFN();
    AnsiString szDir="";

    if(IniConfig.bD05_1SaveSocketCntByHandler)                                  //Steven 20250807 : By handler save contact count
    {
        szDir="D:\\HT9045\\IniData\\SocketCount.ini";
    }
    else
    {
        szDir=GetRecipeFileName("HandlerCondition.Data");
    }

//    TestIF_File.InOutArmLifeCntSet=atoi(edtInOutArmPickerAlmCnt->Text.c_str()); //RogerYang 20260205 : 設定值不需要一直寫入
//    WriteIniData(szDir, "O_Count", "O_20InOutArmLifeCntSet", TestIF_File.InOutArmLifeCntSet);

    for(int i=0; i<2; i++)
    {
        for(int j=0; j<4; j++ )
        {
            str1.sprintf("InArmSuckCnt%d_%d", i, j);
            str2.sprintf("OutArmSuckCnt%d_%d", i, j);
            WriteIniData(szDir, "O_Count", str1, TestIF_File.InArmPickerLifeCnt[i][j]);
            WriteIniData(szDir, "O_Count", str2, TestIF_File.OutArmPickerLifeCnt[i][j]);
        }
    }

    for(int i=0; i<2; i++)
    {
        for(int j=0; j<8; j++ )
        {
            str1.sprintf("Arm1SuckCnt%d_%d", i, j);
            str2.sprintf("Arm2SuckCnt%d_%d", i, j);
            WriteIniData(szDir, "O_Count", str1, TestIF_File.Arm1PickerLifeCnt[i][j]);
            WriteIniData(szDir, "O_Count", str2, TestIF_File.Arm2PickerLifeCnt[i][j]);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfStartCondition::edtInOutArmPickerAlmCntClick(
      TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 100000, 0);
}
//---------------------------------------------------------------------------
void __fastcall TfStartCondition::edtInArmAClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 10000000, 0);
}
//---------------------------------------------------------------------------
