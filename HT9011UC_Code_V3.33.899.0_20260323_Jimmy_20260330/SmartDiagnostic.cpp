#include "MachineDefine.h"
#pragma hdrstop
#include "myQwertyKeyBoard.h"
#include "SmartDiagnostic.h"
#include "cmydef.h"
#include "mycylin.h"
#include "HTimer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfSmartDiagnostic *fSmartDiagnostic;
//---------------------------------------------------------------------------
__fastcall TfSmartDiagnostic::TfSmartDiagnostic(TComponent* Owner)
    : TForm(Owner)
{
    Height          = 660;
    Width           = 810;
    InitialVariable();
    LoadSDSummaryData();
    LoadSDResetSummaryData();
    SearchChangePageButton(fSmartDiagnostic);
    LoadSmartDiagnosticParameter();
    SmartDiagnosticTimer->Enabled   = bStartRecord;
}
//==============================================================================
//                        設定元件大小、位置
//==============================================================================
void TfSmartDiagnostic::SetVCLSizePosition(TWinControl *PCtrl , int iTop, int iLeft, int iWidth, int iHeight)
{
    PCtrl->Top      = iTop;
    PCtrl->Left     = iLeft;
    PCtrl->Width    = iWidth;
    PCtrl->Height   = iHeight;
}
//==============================================================================
//                          設定元件到變數並存檔
//==============================================================================
void TfSmartDiagnostic::SetVCLToParameter(TIniFile *IniF, AnsiString Section, AnsiString Name, TObject *PCtrl, int &iValue)
{
    TEdit     *PEdit         = dynamic_cast <TEdit *>(PCtrl);
    TLabeledEdit *LabEditPtr = dynamic_cast <TLabeledEdit *> (PCtrl);

    if(PEdit!=NULL)
    {
        iValue=atoi(PEdit->Text.c_str());
        IniF->WriteInteger(Section, Name, iValue);
    }
    else if(LabEditPtr!=NULL)
    {
        iValue=atoi(LabEditPtr->Text.c_str());
        IniF->WriteInteger(Section, Name, iValue);
    }
}
//==============================================================================
//                              設定參數到元件
//==============================================================================
void TfSmartDiagnostic::SetParameterToShow(TIniFile *IniF, AnsiString Section, AnsiString Name, TObject *PCtrl, int &iValue)
{
    TEdit     *PEdit         = dynamic_cast <TEdit *>(PCtrl);
    TLabeledEdit *LabEditPtr = dynamic_cast <TLabeledEdit *> (PCtrl);

    if(PEdit!=NULL)
    {
        iValue      = IniF->ReadInteger(Section, Name, 0);
        PEdit->Text = iValue;
    }
    else if(LabEditPtr!=NULL)
    {
        iValue          = IniF->ReadInteger(Section, Name, 0);
        LabEditPtr->Text= iValue;
    }
}
//==============================================================================
//                                  初始化變數
//==============================================================================
void TfSmartDiagnostic::InitialVariable()
{
    aSmartDiagnosticFilePath        = "d:\\HT9045\\system\\SmartDiagnosticRecord.txt";
    aSmartDiagnosticResetFilePath   = "d:\\HT9045\\system\\SmartDiagnosticRecordReset.txt";
    aSmartDiagnosticParaPath        = "d:\\HT9045\\system\\SmartDiagnosticPara.ini";

    MyPageChangeList = new TList;
    MyPageChangeList->Clear();

    sg_SmartDiagnostic_Summary->ColWidths[0] = 50;
    sg_SmartDiagnostic_Summary->ColWidths[1] = 200;
    sg_SmartDiagnostic_Summary->ColWidths[2] = 100;
    sg_SmartDiagnostic_Summary->ColWidths[3] = 100;
    sg_SmartDiagnostic_Summary->ColWidths[4] = 100;
    sg_SmartDiagnostic_Summary->ColWidths[5] = 100;
    sg_SmartDiagnostic_Summary->Cells[0][0]  = "Item";
    sg_SmartDiagnostic_Summary->Cells[1][0]  = "Cylider_Name";
    sg_SmartDiagnostic_Summary->Cells[2][0]  = "ON_Count";
    sg_SmartDiagnostic_Summary->Cells[3][0]  = "OFF_Count";
    sg_SmartDiagnostic_Summary->Cells[4][0]  = "StartTime";
    sg_SmartDiagnostic_Summary->Cells[5][0]  = "EndTime";

    sg_SmartDiagnostic_CyliderManagement->ColWidths[0]  = 130;
    sg_SmartDiagnostic_CyliderManagement->ColWidths[1]  = 95;
    sg_SmartDiagnostic_CyliderManagement->Cells[0][0]   = "CyliderName";
    sg_SmartDiagnostic_CyliderManagement->Cells[1][0]   = "ResetCount";

    BackupChangePage(pc_SmartDiagnostic,sb_SmartDiagnostic_Summary);
    BackupChangePage(pc_SmartDiagnostic,sb_SmartDiagnostic_Setup);
    BackupChangePage(pc_SmartDiagnostic,sb_SmartDiagnostic_Save);
    BackupChangePage(pc_SmartDiagnostic,sb_SmartDiagnostic_Exit);

    for(int i=0; i<MaxCylinderItem; i++)
        bRecordCyStatus[i] = false;

    iLimitCountValue    = 0;
    bStartRecord        = false;
}
//==============================================================================
//                             Backup Change Page
//==============================================================================
void TfSmartDiagnostic::BackupChangePage(TPageControl *pc, TSpeedButton *sb)
{
    MySDPageChange *P;
    P=new MySDPageChange;
    P->PCtrl    = pc;
    P->sbPtr    = sb;
    P->iTag     = sb->Tag;
    MyPageChangeList->Add((MySDPageChange*)P);
}
//==============================================================================
//                             Search Change Page
//==============================================================================
void TfSmartDiagnostic::SearchChangePageButton(TWinControl *PCtrl)
{
    MySDPageChange *MyP;

    for(int iP=0; iP<PCtrl->ControlCount; iP++)
    {
        TControl *P=PCtrl->Controls[iP];
        if(dynamic_cast <TWinControl *>(P)!=NULL)
        {
            SearchChangePageButton((TWinControl *) P);                          // 找該物件附屬的物件(遞迴)
        }

        TSpeedButton *SpeedBtnPtr=dynamic_cast <TSpeedButton  *>(P);

        if(SpeedBtnPtr!=NULL)
        {
            for(int i=0; i<MyPageChangeList->Count; i++)
            {
                MyP=(MySDPageChange *)MyPageChangeList->Items[i];
                if(MyP->sbPtr==SpeedBtnPtr)
                    SpeedBtnPtr->OnClick=FormButtonClick;
            }
        }
    }
}
//==============================================================================
//                                 畫面按鈕
//==============================================================================
void __fastcall TfSmartDiagnostic::FormButtonClick(TObject *Sender)
{
    int ret;
    TSpeedButton *P=(TSpeedButton *)Sender;
    MySDPageChange *MyP;
//    TPageControl *MyBackPC;

    if(P->Tag==10)                                                          //Exit
    {
        sb_SmartDiagnostic_Exit->Down=false;
        Close();
    }
    else if(P->Tag==9)                                                      //Save
    {
        ret=MessageDlg("Sure To Save Report?", mtConfirmation, TMsgDlgButtons()<<mbYes<<mbNo, 0);
        if(ret==mrNo)
            return;
        SaveSDSummaryData();
        SaveSDResetSummaryData();
        SaveSmartDiagnosticParameter();
    }
    else
    {
        for(int i=0; i<MyPageChangeList->Count; i++)
        {
            MyP=(MySDPageChange *)MyPageChangeList->Items[i];
            if(MyP->sbPtr==P)
            {
                MyP->PCtrl->ActivePageIndex=MyP->iTag;
//                MyBackPC=MyP->PCtrl;
                break;
            }
        }
    }
}
//==============================================================================
//                         初始化 StringGrid color
//==============================================================================
void __fastcall TfSmartDiagnostic::InitialStringGridColor(TStringGrid *sg, int iRow, int iCol)
{
    int i, j;

    GridColor=new TColor*[iCol];

    for(i=0; i<iCol; i++)
    {
        GridColor[i]=new TColor[iRow];
    }

    for(i=0; i<iCol; i++)
    {
        for(j=0; j<iRow; j++)
        {
            if(j==0)
            {
                GridColor[i][j]=TColor(0x00917B51);
            }
            else
            {
                GridColor[i][j]=TColor(0x00DFD9CC);
            }
            SetCellColor(sg, sg->Cells[i][j], i, j, GridColor[i][j]);
        }
    }
    sg->DefaultDrawing=false;
}
//==============================================================================
//
//==============================================================================
void __fastcall TfSmartDiagnostic::SetCellColor(TStringGrid *sg, AnsiString aTitle, int iCol, int iRow, TColor Color)
{
    if(iCol>=0 && iRow>=0)
    {
        GridColor[iCol][iRow]=Color;
        sg->Refresh();
        sg->Cells[iCol][iRow]=aTitle;
    }
}
//==============================================================================
//
//==============================================================================
void __fastcall TfSmartDiagnostic::FormCreate(TObject *Sender)
{
    InitialStringGridColor(sg_SmartDiagnostic_Summary, sg_SmartDiagnostic_Summary->RowCount, 6);
}
//==============================================================================
void __fastcall TfSmartDiagnostic::FormShow(TObject *Sender)
{
    TDateTime TimeNow=Now();
    AnsiString S;

    Left=(1280-Width)/2;
    Top =(1024-Height)/2;

    SetVCLSizePosition(pn_SmartDiagnosticTitle, 0, 0, 800, 50);
    SetVCLSizePosition(pc_SmartDiagnostic, 25, 0, 800, 600);

    pc_SmartDiagnostic->ActivePageIndex=0;

    S.sprintf("%s年-%s月-%s日-%s時", TimeNow.FormatString("YYYY"), TimeNow.FormatString("MM"), TimeNow.FormatString("DD"), TimeNow.FormatString("HH"));
    pn_SmartDiagnostic_Time->Caption   =S;
}
//---------------------------------------------------------------------------
void __fastcall TfSmartDiagnostic::FormDestroy(TObject *Sender)
{
    delete MyPageChangeList;
}
//==============================================================================
//                            讓StringGrid字型置中
//==============================================================================
void __fastcall TfSmartDiagnostic::sg_SmartDiagnostic_SummaryDrawCell(
      TObject *Sender, int ACol, int ARow, TRect &Rect,
      TGridDrawState State)
{
    TStringGrid *P=(TStringGrid *)Sender;

    sg_SmartDiagnostic_Summary->Canvas->Brush->Color=GridColor[ACol][ARow];
    sg_SmartDiagnostic_Summary->Canvas->FillRect(Rect);
    sg_SmartDiagnostic_Summary->Canvas->TextOut(Rect.Left+2, Rect.Top, sg_SmartDiagnostic_Summary->Cells[ACol][ARow]);
    if(ARow==0)
    {
        P->Canvas->FillRect(Rect);
        DrawText(P->Canvas->Handle,
                 P->Cells[ACol][ARow].c_str(),
                 P->Cells[ACol][ARow].Length(),
                 &Rect,
                 DT_VCENTER | DT_SINGLELINE | DT_CENTER);
    }

}
//==============================================================================
bool SplitStrByDotSpaceOnly(char *str, char *dest, int Max)
{
    char Buffer[10240];
    int ct1=0, ct2=0;

    while(1)         // find first character
    {
        if(str[ct1]=='\x0')
            return false;
        if((str[ct1]!=',' && str[ct1]!='\t' && str[ct1]!='\r' && str[ct1]!=':'))
            break;
        ct1++;
    }
    while(1)
    {
        dest[ct2]=str[ct1];
        ct2++;
        ct1++;
        dest[ct2]='\x0';
        if((ct2+1)>=Max)
            break;

        if(str[ct1]!=',' && str[ct1]!='\t' && str[ct1]!='\0' && str[ct1]!='\r' && str[ct1]!=':')
        {
        }
        else
        {
            break;
        }
    }
    ct2=0;
    while(1)
    {
        Buffer[ct2]=str[ct1];
        if(str[ct1]=='\x0')
            break;
        ct1++;
        ct2++;

        if(ct2>=10240)
            break;
        Buffer[ct2]='\x0';
    }
    strcpy(str, Buffer);
    return true;
}
//==============================================================================
//                               轉檔案到StringGrid
//==============================================================================
void TfSmartDiagnostic::PasteStringGridAsTabFormat(int XSTART, TStringGrid *strGrd, TStringList *memoPtr)
{
    int x, y, StartX, StartY;
    char str[8192], str2[256];
    bool flag;

    for(y=0; y<strGrd->RowCount; y++)
        for(x=XSTART; x<strGrd->ColCount; x++)
            strGrd->Cells[x][y]="";

    for(y=0; y<memoPtr->Count; y++)
    {
        if(y>=strGrd->RowCount)
            break;
        strcpy(str, memoPtr->Strings[y].c_str());
        x=XSTART;
        do
        {
            flag=SplitStrByDotSpaceOnly(str, str2, 256);
            if(flag==false)
                break;

            StartX=x;
            StartY=y;

            strGrd->Cells[StartX][StartY]=str2;
            x++;
            if(x>=strGrd->ColCount)
                break;
        }while(1);
    }
}
//==============================================================================
//                                  存檔
//==============================================================================
void __fastcall TfSmartDiagnostic::SaveSmartDiagnosticParameter()
{
    TIniFile* IniFile=new TIniFile(aSmartDiagnosticParaPath);

    SetVCLToParameter(IniFile, "Setup", "LimitCount"                , ed_SmartDiagnostic_LimitCountValue                , iLimitCountValue);
    SetVCLToParameter(IniFile, "Setup", "LimitCheckTime"            , ed_SmartDiagnostic_LimitCheckTime                 , iLimitCheckTime);
    LoadSmartDiagnosticParameter();

    delete IniFile;
}
//==============================================================================
//                                  載檔
//==============================================================================
void __fastcall TfSmartDiagnostic::LoadSmartDiagnosticParameter()
{
    TIniFile* IniFile=new TIniFile(aSmartDiagnosticParaPath);

    SetParameterToShow(IniFile, "Setup", "LimitCount"                , ed_SmartDiagnostic_LimitCountValue                , iLimitCountValue);
    SetParameterToShow(IniFile, "Setup", "LimitCheckTime"            , ed_SmartDiagnostic_LimitCheckTime                 , iLimitCheckTime);

    delete IniFile;
}
//==============================================================================
//                              載入Summary資料
//==============================================================================
void TfSmartDiagnostic::LoadSDSummaryData()
{
    if(FileExists(aSmartDiagnosticFilePath.c_str())==false)
    {
        bStartRecord=false;
        return;
    }

    TStringList *slSummary;

    slSummary=new TStringList;

    slSummary->LoadFromFile(aSmartDiagnosticFilePath);
    sg_SmartDiagnostic_Summary->RowCount=slSummary->Count;
    PasteStringGridAsTabFormat(0, sg_SmartDiagnostic_Summary, slSummary);
    cob_SmartDiagnostic_CyliderName->Items->Clear();
    for(int i=0; i<sg_SmartDiagnostic_Summary->RowCount; i++)
    {
        if(sg_SmartDiagnostic_Summary->Cells[1][i+1]!="")
        {
            iRecordCyliderOnCount[i]  = atoi(sg_SmartDiagnostic_Summary->Cells[2][i+1].c_str());
            iRecordCyliderOffCount[i] = atoi(sg_SmartDiagnostic_Summary->Cells[3][i+1].c_str());
            cob_SmartDiagnostic_CyliderName->Items->Add(sg_SmartDiagnostic_Summary->Cells[1][i+1]);
        }
    }
    bStartRecord=true;
    delete slSummary;
}
//==============================================================================
//                              載入Reset Summary資料
//==============================================================================
void TfSmartDiagnostic::LoadSDResetSummaryData()
{
    if(FileExists(aSmartDiagnosticResetFilePath.c_str())==false)
    {
        return;
    }

    TStringList *slSummary;

    slSummary=new TStringList;

    slSummary->LoadFromFile(aSmartDiagnosticResetFilePath);
    sg_SmartDiagnostic_CyliderManagement->RowCount = slSummary->Count;
    PasteStringGridAsTabFormat(0, sg_SmartDiagnostic_CyliderManagement, slSummary);

    delete slSummary;
}
//==============================================================================
//                              儲存Summary資料
//==============================================================================
void TfSmartDiagnostic::SaveSDSummaryData()
{
    AnsiString S;
    int i, j;

    TStringList *P;
    P=new TStringList;
    P->Clear();

    for(i=0; i<sg_SmartDiagnostic_Summary->RowCount; i++)
    {
        S="";
        for(j=0; j<sg_SmartDiagnostic_Summary->ColCount; j++)
        {
            S+=sg_SmartDiagnostic_Summary->Cells[j][i];
            S+=",";
        }
        P->Add(S);
    }
    P->SaveToFile(aSmartDiagnosticFilePath.c_str());
    delete P;
}
//==============================================================================
//                              儲存sSummary Summary資料
//==============================================================================
void TfSmartDiagnostic::SaveSDResetSummaryData()
{
    AnsiString S;
    int i, j;

    TStringList *P;
    P=new TStringList;
    P->Clear();

    for(i=0; i<sg_SmartDiagnostic_CyliderManagement->RowCount; i++)
    {
        S="";
        for(j=0; j<sg_SmartDiagnostic_CyliderManagement->ColCount; j++)
        {
            S+=sg_SmartDiagnostic_CyliderManagement->Cells[j][i];
            S+=",";
        }
        P->Add(S);
    }
    P->SaveToFile(aSmartDiagnosticResetFilePath.c_str());
    delete P;
}
//==============================================================================
void __fastcall TfSmartDiagnostic::sb_SmarDiagnostic_CreateCyliderNameClick(
      TObject *Sender)
{
    int ret=MessageDlg("Sure To Create New Report?", mtConfirmation, TMsgDlgButtons()<<mbYes<<mbNo, 0);
    TDateTime TimeNow=Now();

    if(ret==mrNo)
        return;

    sg_SmartDiagnostic_Summary->RowCount=MaxCylinderItem+1;
    sg_SmartDiagnostic_CyliderManagement->RowCount=MaxCylinderItem+1;

    for(int i=0; i<MaxCylinderItem; i++)
    {
        sg_SmartDiagnostic_Summary->Cells[0][i+1]       = i+1;
        if(Cylinder[i].CylinderName!="")
        {
            sg_SmartDiagnostic_Summary->Cells[1][i+1]   = Cylinder[i].CylinderName;
            sg_SmartDiagnostic_Summary->Cells[2][i+1]   = "0";
            sg_SmartDiagnostic_Summary->Cells[3][i+1]   = "0";
            sg_SmartDiagnostic_Summary->Cells[4][i+1]   = TimeNow.FormatString("YYYY-MM-DD-HH");
            sg_SmartDiagnostic_Summary->Cells[5][i+1]   = TimeNow.FormatString("YYYY-MM-DD-HH");
            iRecordCyliderOnCount[i]                    = 0;
            iRecordCyliderOffCount[i]                   = 0;
            cob_SmartDiagnostic_CyliderName->Items->Add(Cylinder[i].CylinderName);
            sg_SmartDiagnostic_CyliderManagement->Cells[0][i+1]   =Cylinder[i].CylinderName;
            sg_SmartDiagnostic_CyliderManagement->Cells[1][i+1]   ="0";
        }
    }
}
//==============================================================================
//                              找出汽缸On並增加
//==============================================================================
void TfSmartDiagnostic::GetCyliderOnCount(AnsiString CyName)
{
    if(CosFunction.bCylinderOnOffTimeLog==false)
    {
        return;
    }

    if(FileExists(aSmartDiagnosticFilePath.c_str())==false)
    {
        return;
    }

    TDateTime TimeNow=Now();

    for(int i=0; i<MaxCylinderItem; i++)
    {
        if(CyName==sg_SmartDiagnostic_Summary->Cells[1][i+1])
        {
            if(bRecordCyStatus[i]==true)
                break;

            iRecordCyliderOnCount[i]=atoi(sg_SmartDiagnostic_Summary->Cells[2][i+1].c_str());
            iRecordCyliderOnCount[i]++;
            sg_SmartDiagnostic_Summary->Cells[2][i+1]   = iRecordCyliderOnCount[i];
            sg_SmartDiagnostic_Summary->Cells[5][i+1]   = TimeNow.FormatString("YYYY-MM-DD-HH");
            bRecordCyStatus[i]=true;
            break;
        }
    }
    SaveSDSummaryData();
}
//==============================================================================
//                              找出汽缸Off並增加
//==============================================================================
void TfSmartDiagnostic::GetCyliderOffCount(AnsiString CyName)
{

    if(CosFunction.bCylinderOnOffTimeLog==false)
    {
        return;
    }

    if(FileExists(aSmartDiagnosticFilePath.c_str())==false)
    {
        return;
    }
    TDateTime TimeNow=Now();

    for(int i=0; i<MaxCylinderItem; i++)
    {
        if(CyName==sg_SmartDiagnostic_Summary->Cells[1][i+1])
        {
            if(bRecordCyStatus[i]==false)
                break;

            iRecordCyliderOffCount[i]=atoi(sg_SmartDiagnostic_Summary->Cells[3][i+1].c_str());
            iRecordCyliderOffCount[i]++;
            sg_SmartDiagnostic_Summary->Cells[3][i+1]   = iRecordCyliderOffCount[i];
            sg_SmartDiagnostic_Summary->Cells[5][i+1]   = TimeNow.FormatString("YYYY-MM-DD-HH");
            bRecordCyStatus[i] = false;
            break;
        }
    }
    SaveSDSummaryData();
}
//==============================================================================
void __fastcall TfSmartDiagnostic::sb_SmartDiagnostic_ResetRecordCountClick(
      TObject *Sender)
{
    TDateTime TimeNow=Now();
    int iCT=0, iIndex=0;

    int ret=MessageDlg("Sure To Reset Report?", mtConfirmation, TMsgDlgButtons()<<mbYes<<mbNo, 0);
    if(ret==mrNo)
        return;

    for(int i=0; i<MaxCylinderItem; i++)
    {
        if(sg_SmartDiagnostic_Summary->Cells[1][i+1]==cob_SmartDiagnostic_CyliderName->Items->Strings[i])
        {
            sg_SmartDiagnostic_Summary->Cells[2][i+1]   = "0";
            sg_SmartDiagnostic_Summary->Cells[3][i+1]   = "0";
            sg_SmartDiagnostic_Summary->Cells[4][i+1]   = TimeNow.FormatString("YYYY-MM-DD-HH");
            sg_SmartDiagnostic_Summary->Cells[5][i+1]   = TimeNow.FormatString("YYYY-MM-DD-HH");
            iRecordCyliderOnCount[i]                    = 0;
            iRecordCyliderOffCount[i]                   = 0;
            iCT     = atoi(sg_SmartDiagnostic_CyliderManagement->Cells[1][i+1].c_str());
            iCT++;
            sg_SmartDiagnostic_CyliderManagement->Cells[1][i+1]=iCT;
            break;
        }
    }
    iIndex=cob_SmartDiagnostic_CyliderName->ItemIndex+1;
    for(int i=0; i<6; i++)
    {
        SetCellColor(sg_SmartDiagnostic_Summary, sg_SmartDiagnostic_Summary->Cells[i][iIndex], i, iIndex, clWindow);
    }
    cob_SmartDiagnostic_CyliderName->ItemIndex=-1;
}
//==============================================================================
//
TQPF_Timer hDelay;
//==============================================================================
void __fastcall TfSmartDiagnostic::SmartDiagnosticTimerTimer(
      TObject *Sender)
{
    static int Task=1, iAlarmOnIndex=0, iAlarmOffIndex=0;
//    static HTimer Delay;
    int i, iOnCount, iOffCount;
    static bool bAlarm=false;

    switch(Task)
    {
        case 1:
            bAlarm          = false;
            iAlarmOnIndex   = 0;
            iAlarmOffIndex  = 0;
            for(i=1; i<sg_SmartDiagnostic_Summary->RowCount; i++)
            {
                iOnCount    = atoi(sg_SmartDiagnostic_Summary->Cells[2][i].c_str());
                iOffCount   = atoi(sg_SmartDiagnostic_Summary->Cells[3][i].c_str());

                if(iOnCount>=iLimitCountValue)
                {
                    bAlarm=true;
                    iAlarmOnIndex=i;
                    break;
                }
                if(iOffCount>=iLimitCountValue)
                {
                    bAlarm=true;
                    iAlarmOffIndex=i;
                    break;
                }
            }
            if(iAlarmOffIndex==0 && iAlarmOnIndex==0)
            {

            }
            hDelay.SetMSAndOn(iLimitCheckTime*10);
            Task=10;
            break;
        case 10:
            if(hDelay.Off())
            {
                for(i=0; i<6; i++)
                {
                    if(iAlarmOnIndex>0)
                        SetCellColor(sg_SmartDiagnostic_Summary, sg_SmartDiagnostic_Summary->Cells[i][iAlarmOnIndex], i, iAlarmOnIndex,clRed);
                    if(iAlarmOffIndex>0)
                        SetCellColor(sg_SmartDiagnostic_Summary, sg_SmartDiagnostic_Summary->Cells[i][iAlarmOffIndex], i, iAlarmOffIndex,clRed);
                }
                Task=1;
            }
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfSmartDiagnostic::ed_SmartDiagnostic_LimitCountValueMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 999999, 1);
}
//---------------------------------------------------------------------------
void __fastcall TfSmartDiagnostic::ed_SmartDiagnostic_LimitCheckTimeMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 999999, 1);
}
//---------------------------------------------------------------------------