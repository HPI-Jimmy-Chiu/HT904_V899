#include "MachineDefine.h"
#pragma hdrstop

#include "PMAlarmInterFace.h"
#include "ExternFunction.h"
#include "PMAlarmShowData.h"
#include "cmydef.h"
#include "common.h"
#include "mymessbox.h"
#include "TfFTP.h"
#include "myQwertyKeyBoard.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SHDocVw_OCX"
#pragma resource "*.dfm"
TfPMAlarmInterFace *fPMAlarmInterFace;
//---------------------------------------------------------------------------
__fastcall TfPMAlarmInterFace::TfPMAlarmInterFace(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
// 2015.03.25 , Joye , PM Alarm ---------------------------------------------->>
void __fastcall TfPMAlarmInterFace::sbPMAlarmMonth_AddClick(TObject *Sender)
{
    TSpeedButton * pBtn = (TSpeedButton *)Sender;

    if( pBtn == NULL || pBtn->Tag <= 0 || pBtn->Tag >=4 )
    {
        return;
    }

    static TStringGrid* pItemList[3] = { sgPMAlarmMonthItem , sgPMAlarmQuarterItem , sgPMAlarmYearItem };

    int iIndex = pBtn->Tag - 1;

    int iRow = pItemList[iIndex]->RowCount;
    StringGrid_Insert_Row( pItemList[iIndex] , iRow );
    pItemList[iIndex]->Cells[0][iRow] = AnsiString( iRow );
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sbPMAlarmMonth_DeleteClick(TObject *Sender)
{
    TSpeedButton * pBtn = (TSpeedButton *)Sender;

    if( pBtn == NULL || pBtn->Tag <= 0 || pBtn->Tag >=4 )
    {
        return;
    }

    static TStringGrid* pItemList[3] = { sgPMAlarmMonthItem , sgPMAlarmQuarterItem , sgPMAlarmYearItem };

    int iIndex = pBtn->Tag - 1;
    int iRow = pItemList[iIndex]->RowCount;
    if( iRow == 2 )
    {
        iPMAlarmSelectRow = 1;
    }

    if( iPMAlarmSelectRow <= 0 || iPMAlarmSelectRow >= iRow )
    {
        iPMAlarmSelectRow = -1;
        return;
    }

    StringGrid_Delete_Row( pItemList[iIndex] , iPMAlarmSelectRow );
    iPMAlarmSelectRow = -1;
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sbPMAlarm_SaveClick(TObject *Sender)
{
    AnsiString sItem;
    PMAlarm_SYS.PMItem_Month.ClearList();
    int iCount_Month = sgPMAlarmMonthItem->RowCount - 1;

    for( int i = 0 ; i < iCount_Month ; i++ )
    {
        sItem = sgPMAlarmMonthItem->Cells[1][i+1];
        if( sItem != "" )
        {
            PMAlarm_SYS.PMItem_Month.PushItem( sItem );
        }

        sItem = sgPMAlarmMonthItem->Cells[2][i+1];
        if( sItem != "" )
        {
            PMAlarm_SYS.PMItem_Month.PushLimitP( sItem );
        }

        sItem = sgPMAlarmMonthItem->Cells[3][i+1];
        if( sItem != "" )
        {
            PMAlarm_SYS.PMItem_Month.PushLimitN( sItem );
        }
    }

    PMAlarm_SYS.PMItem_Quarter.ClearList();
    int iCount_Quarter = sgPMAlarmQuarterItem->RowCount - 1;

    for( int i = 0 ; i < iCount_Quarter ; i++ )
    {
        sItem = sgPMAlarmQuarterItem->Cells[1][i+1];
        if( sItem != "" )
        {
            PMAlarm_SYS.PMItem_Quarter.PushItem( sItem );
        }

        sItem = sgPMAlarmQuarterItem->Cells[2][i+1];
        if( sItem != "" )
        {
            PMAlarm_SYS.PMItem_Quarter.PushLimitP( sItem );
        }

        sItem = sgPMAlarmQuarterItem->Cells[3][i+1];
        if( sItem != "" )
        {
            PMAlarm_SYS.PMItem_Quarter.PushLimitN( sItem );
        }
    }

    PMAlarm_SYS.PMItem_Year.ClearList();
    int iCount_Year = sgPMAlarmYearItem->RowCount - 1;

    for( int i = 0 ; i < iCount_Year ; i++ )
    {
        sItem = sgPMAlarmYearItem->Cells[1][i+1];
        if( sItem != "" )
        {
            PMAlarm_SYS.PMItem_Year.PushItem( sItem );
        }

        sItem = sgPMAlarmYearItem->Cells[2][i+1];
        if( sItem != "" )
        {
            PMAlarm_SYS.PMItem_Year.PushLimitP( sItem );
        }

        sItem = sgPMAlarmYearItem->Cells[3][i+1];
        if( sItem != "" )
        {
            PMAlarm_SYS.PMItem_Year.PushLimitN( sItem );
        }
    }

    PMAlarm_SYS.PMItem_Temperature.ClearList();
    int iCount_Temperature = sgPMAlarmTemperatureItem->RowCount - 1;

    for( int i = 0 ; i < iCount_Temperature ; i++ )
    {
        sItem = sgPMAlarmTemperatureItem->Cells[1][i+1];
        if( sItem != "" )
        {
            PMAlarm_SYS.PMItem_Temperature.PushItem( sItem );
        }

        sItem = sgPMAlarmTemperatureItem->Cells[2][i+1];
        if( sItem != "" )
        {
            PMAlarm_SYS.PMItem_Temperature.PushLimitP( sItem );
        }

        sItem = sgPMAlarmTemperatureItem->Cells[3][i+1];
        if( sItem != "" )
        {
            PMAlarm_SYS.PMItem_Temperature.PushLimitN( sItem );
        }
    }

    PMAlarm_SYS.PMItem_ESD.ClearList();
    int iCount_ESD = sgPMAlarmESDItem->RowCount - 1;

    for( int i = 0 ; i < iCount_ESD ; i++ )
    {
        sItem = sgPMAlarmESDItem->Cells[1][i+1];
        if( sItem != "" )
        {
            PMAlarm_SYS.PMItem_ESD.PushItem( sItem );
        }

        sItem = sgPMAlarmESDItem->Cells[2][i+1];
        if( sItem != "" )
        {
            PMAlarm_SYS.PMItem_ESD.PushLimitP( sItem );
        }

        sItem = sgPMAlarmESDItem->Cells[3][i+1];
        if( sItem != "" )
        {
            PMAlarm_SYS.PMItem_ESD.PushLimitN( sItem );
        }
    }

    PMAlarm_SYS.PMItem_IonFan.ClearList();
    int iCount_IonFan = sgPMAlarmIonFanItem->RowCount - 1;

    for( int i = 0 ; i < iCount_IonFan ; i++ )
    {
        sItem = sgPMAlarmIonFanItem->Cells[1][i+1];
        if( sItem != "" )
        {
            PMAlarm_SYS.PMItem_IonFan.PushItem( sItem );
        }

        sItem = sgPMAlarmIonFanItem->Cells[2][i+1];
        if( sItem != "" )
        {
            PMAlarm_SYS.PMItem_IonFan.PushLimitP( sItem );
        }

        sItem = sgPMAlarmIonFanItem->Cells[3][i+1];
        if( sItem != "" )
        {
            PMAlarm_SYS.PMItem_IonFan.PushLimitN( sItem );
        }
    }

    PMAlarm_SYS.SetPMDate( cbPMAlarmDate->ItemIndex + 1 );

    bool bSetOk = PMAlarm_SYS.SetPMAlarmDays( atoi( edtPMAlarmAlarmDays->Text.c_str() ) );
    if(bSetOk == false)
    {
        edtPMAlarmAlarmDays->Text = "0";
        PMAlarm_SYS.SetPMAlarmDays( atoi( edtPMAlarmAlarmDays->Text.c_str() ) );
    }

    //Always One Day
    PMAlarm_SYS.SetPMStopDays(1);

    //PMAlarm_SYS.SetNextPMDate( dtpPMAlarmNextPMDate->Date );
    PMAlarm_SYS.SavePMAlarm();
    UpdatePMAlarmToScreen();
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sbPMAlarmCheck_UpdateClick(TObject *Sender)
{
    bool bCheckOK = true;
    TDateTime dtDate = dtpPMAlarmNextPMDate->Date;
    int wMonth=GetMonth(dtDate);
    int wNowMonth=GetMonth(Now());
    int wNextMonth=0;
    if((wMonth != 1 && wNowMonth != wMonth && (wNowMonth != wMonth-1)) ||  //Sam 20190926 Modify PM Alarm PM Month
       (wMonth == 1 && wNowMonth != wMonth &&  wNowMonth != 12))
    {
        ShowMyMessage( "Not Yet Reached Machine Maintenance Month!!" );
        return;
    }
//    要把目前Check的資料記下寫入檔案，開啟時要更新
//    要在當月才可以Check
    if(PMAllAlarmCheckOK()==true)
    {
        ShowMyMessage( "Maintenance Item Check is Correct!!" );

        ed_GerneralExecutiveOwner->Text = "";
        ed_GerneralConformOwner->Text = "";

        ed_TemperatureExecutiveOwner->Text = "";
        ed_TemperatureConformOwner->Text = "";

        ed_ESDExecutiveOwner->Text = "";
        ed_ESDConformOwner->Text = "";

        ed_IonFanExecutiveOwner->Text = "";
        ed_IonFanConformOwner->Text = "";

        //_PM_DATE
        for( int i = 0 ; i < 31 ; i++ )
        {
            dtDate = dtDate + 1;
            wNextMonth=GetMonth(dtDate);
            if( wNextMonth != wMonth )
            {
                break;
            }
        }
        dtDate = dtDate + PMAlarm_SYS.GetPMDate() - 1;
        dtpPMAlarmNextPMDate->Date = dtDate;
        PMAlarm_SYS.SetNextPMDate( dtpPMAlarmNextPMDate->Date );
        PMAlarm_SYS.SavePMAlarm();
    }
    SaveGerneralPMAlarmToCSV();
    SaveTemperaturePMAlarmToCSV();
    SaveESDPMAlarmToCSV();
    SaveIonFanPMAlarmToCSV();

    UpdatePMAlarmToScreen(bCheckOK);

    LoadGerneralPMAlarmFromCSV();
    LoadTemperaturePMAlarmFromCSV();
    LoadESDPMAlarmFromCSV();
    LoadIonFanPMAlarmFromCSV();
}
//---------------------------------------------------------------------------
void TfPMAlarmInterFace::UpdatePMAlarmToScreen(bool bIsCheckOk)
{
    //要把目前Check的資料記下寫入檔案，開啟時要更新
    iPMAlarmSelectRow = -1;
    PMAlarm_SYS.LoadPMAlarm();
    ed_ExecutiveItem->Text      = PMAlarm_SYS.GetExecutiveItem();
    ed_ExecutiveOwner->Text     = PMAlarm_SYS.GetExecutiveOwner();
    ed_ConformItem->Text        = PMAlarm_SYS.GetConformItem();
    ed_ConformOwner->Text       = PMAlarm_SYS.GetConformOwner();
    ed_GerneralPMDocVer->Text   = PMAlarm_SYS.GetGerneralPMDocVer();
    ed_TemperaturePMDocVer->Text= PMAlarm_SYS.GetTemperaturePMDocVer();
    ed_ESDPMDocVer->Text        = PMAlarm_SYS.GetESDPMDocVer();
    ed_IonFanPMDocVer->Text     = PMAlarm_SYS.GetIonFanPMDocVer();
    ed_MachineNo->Text          = PMAlarm_SYS.GetMachineNo();
    ed_SaveFilePath->Text       = PMAlarm_SYS.GetSaveFilePath();
    cbPMStartMonth->Text        = PMAlarm_SYS.GetStartMonth();
    dtpPMAlarmNextPMDate->Date  = PMAlarm_SYS.GetNextPMDate();

    int wMonth=GetMonth(dtpPMAlarmNextPMDate->Date);
    pnlPMAlarmCheck_Title->Caption = IntToStr(wMonth) + "Month PM Item Check";

    bool bIsYear = false;
    bool bIsQuarter = false;

    AnsiString sStartMonth = PMAlarm_SYS.GetStartMonth();
    int iStartMonth = atoi(sStartMonth.c_str());

    if(iStartMonth <= 0 || iStartMonth > 12)
    {
        iStartMonth = 1; //1 4 7 10
    }

    //if( wMonth == iStartMonth )
    if(wMonth == iStartMonth || wMonth == iStartMonth - 6 || wMonth == iStartMonth + 6 ) //Sam 20190925 Modify PM Alarm Year to Half Year
    {
        bIsYear = true;
    }

    iStartMonth = iStartMonth % 3; //4->1 1 4 7 10
    if(iStartMonth == 0)           //5->2 2 5 8 11
    {                              //6->0 3 6 9 12
        iStartMonth = 3;
    }
    if(wMonth == 1 || wMonth == 4 || wMonth == 7 || wMonth == 10) //20150707
    {
        bIsQuarter = true;
    }

    int iCount_Month    = PMAlarm_SYS.PMItem_Month.GetCount();
    if( iCount_Month == 0 )
    {
        sgPMAlarmMonthItem->RowCount = 2;
        sgPMAlarmMonthItem->Cells[0][1] = AnsiString(1);
    }
    else
    {
        sgPMAlarmMonthItem->RowCount = 1 + iCount_Month;
    }

    sgPMAlarmMonthItem->Cells[0][0] = "No.";
    sgPMAlarmMonthItem->Cells[1][0] = "PM Item";
    sgPMAlarmMonthItem->Cells[2][0] = "Limit P";
    sgPMAlarmMonthItem->Cells[3][0] = "Limit N";

    for( int i = 0 ; i < iCount_Month ; i++ )
    {
        sgPMAlarmMonthItem->Cells[0][1+i] = AnsiString(1+i);
        sgPMAlarmMonthItem->Cells[1][1+i] = PMAlarm_SYS.PMItem_Month.GetItem(i);
        sgPMAlarmMonthItem->Cells[2][1+i] = PMAlarm_SYS.PMItem_Month.GetLimitP(i);
        sgPMAlarmMonthItem->Cells[3][1+i] = PMAlarm_SYS.PMItem_Month.GetLimitN(i);

        PMAlarm_SYS.PMItem_List.PushItem( PMAlarm_SYS.PMItem_Month.GetItem(i) );
        PMAlarm_SYS.PMItem_List.PushLimitP( PMAlarm_SYS.PMItem_Month.GetLimitP(i) );
        PMAlarm_SYS.PMItem_List.PushLimitN( PMAlarm_SYS.PMItem_Month.GetLimitN(i) );
    }

    int iCount_Quarter  = PMAlarm_SYS.PMItem_Quarter.GetCount();

    if( iCount_Quarter == 0 )
    {
        sgPMAlarmQuarterItem->RowCount = 2;
        sgPMAlarmQuarterItem->Cells[0][1] = AnsiString(1);
    }
    else
    {
        sgPMAlarmQuarterItem->RowCount = 1 + iCount_Quarter;
    }

    sgPMAlarmQuarterItem->Cells[0][0] = "No.";
    sgPMAlarmQuarterItem->Cells[1][0] = "PM Item";
    sgPMAlarmQuarterItem->Cells[2][0] = "Limit P";
    sgPMAlarmQuarterItem->Cells[3][0] = "Limit N";

    for( int i = 0 ; i < iCount_Quarter ; i++ )
    {
        sgPMAlarmQuarterItem->Cells[0][1+i] = AnsiString(1+i);
        sgPMAlarmQuarterItem->Cells[1][1+i] = PMAlarm_SYS.PMItem_Quarter.GetItem(i);
        sgPMAlarmQuarterItem->Cells[2][1+i] = PMAlarm_SYS.PMItem_Quarter.GetLimitP(i);
        sgPMAlarmQuarterItem->Cells[3][1+i] = PMAlarm_SYS.PMItem_Quarter.GetLimitN(i);

        if( bIsQuarter == true )
        {
            PMAlarm_SYS.PMItem_List.PushItem( PMAlarm_SYS.PMItem_Quarter.GetItem(i) );
            PMAlarm_SYS.PMItem_List.PushLimitP( PMAlarm_SYS.PMItem_Quarter.GetLimitP(i) );
            PMAlarm_SYS.PMItem_List.PushLimitN( PMAlarm_SYS.PMItem_Quarter.GetLimitN(i) );
        }
    }

    int iCount_Year     = PMAlarm_SYS.PMItem_Year.GetCount();
    if( iCount_Year == 0 )
    {
        sgPMAlarmYearItem->RowCount = 2;
        sgPMAlarmYearItem->Cells[0][1] = AnsiString(1);
    }
    else
    {
        sgPMAlarmYearItem->RowCount = 1 + iCount_Year;
    }

    sgPMAlarmYearItem->Cells[0][0] = "No.";
    sgPMAlarmYearItem->Cells[1][0] = "PM Item";
    sgPMAlarmYearItem->Cells[2][0] = "Limit P";
    sgPMAlarmYearItem->Cells[3][0] = "Limit N";

    for( int i = 0 ; i < iCount_Year ; i++ )
    {
        sgPMAlarmYearItem->Cells[0][1+i] = AnsiString(1+i);
        sgPMAlarmYearItem->Cells[1][1+i] = PMAlarm_SYS.PMItem_Year.GetItem(i);
        sgPMAlarmYearItem->Cells[2][1+i] = PMAlarm_SYS.PMItem_Year.GetLimitP(i);
        sgPMAlarmYearItem->Cells[3][1+i] = PMAlarm_SYS.PMItem_Year.GetLimitN(i);

        if( bIsYear == true )
        {
            PMAlarm_SYS.PMItem_List.PushItem( PMAlarm_SYS.PMItem_Year.GetItem(i) );
            PMAlarm_SYS.PMItem_List.PushLimitP( PMAlarm_SYS.PMItem_Year.GetLimitP(i) );
            PMAlarm_SYS.PMItem_List.PushLimitN( PMAlarm_SYS.PMItem_Year.GetLimitN(i) );
        }
    }

    int iCount_Temperature    = PMAlarm_SYS.PMItem_Temperature.GetCount();

    if( iCount_Temperature == 0 )
    {
        sgPMAlarmTemperatureItem->RowCount = 2;
        sgPMAlarmTemperatureItem->Cells[0][1] = AnsiString(1);
    }
    else
    {
        sgPMAlarmTemperatureItem->RowCount = 1 + iCount_Temperature;
    }

    sgPMAlarmTemperatureItem->Cells[0][0] = "No.";
    sgPMAlarmTemperatureItem->Cells[1][0] = "PM Item";
    sgPMAlarmTemperatureItem->Cells[2][0] = "Limit P";
    sgPMAlarmTemperatureItem->Cells[3][0] = "Limit N";

    for( int i = 0 ; i < iCount_Temperature ; i++ )
    {
        sgPMAlarmTemperatureItem->Cells[0][1+i] = AnsiString(1+i);
        sgPMAlarmTemperatureItem->Cells[1][1+i] = PMAlarm_SYS.PMItem_Temperature.GetItem(i);
        sgPMAlarmTemperatureItem->Cells[2][1+i] = PMAlarm_SYS.PMItem_Temperature.GetLimitP(i);
        sgPMAlarmTemperatureItem->Cells[3][1+i] = PMAlarm_SYS.PMItem_Temperature.GetLimitN(i);

        PMAlarm_SYS.PMItem_ListTemperature.PushItem( PMAlarm_SYS.PMItem_Temperature.GetItem(i) );
        PMAlarm_SYS.PMItem_ListTemperature.PushLimitP( PMAlarm_SYS.PMItem_Temperature.GetLimitP(i) );
        PMAlarm_SYS.PMItem_ListTemperature.PushLimitN( PMAlarm_SYS.PMItem_Temperature.GetLimitN(i) );
    }

    int iCount_ESD    = PMAlarm_SYS.PMItem_ESD.GetCount();
    if( iCount_ESD == 0 )
    {
        sgPMAlarmESDItem->RowCount = 2;
        sgPMAlarmESDItem->Cells[0][1] = AnsiString(1);
    }
    else
    {
        sgPMAlarmESDItem->RowCount = 1 + iCount_ESD;
    }

    sgPMAlarmESDItem->Cells[0][0] = "No.";
    sgPMAlarmESDItem->Cells[1][0] = "PM Item";
    sgPMAlarmESDItem->Cells[2][0] = "Limit P";
    sgPMAlarmESDItem->Cells[3][0] = "Limit N";

    for( int i = 0 ; i < iCount_ESD ; i++ )
    {
        sgPMAlarmESDItem->Cells[0][1+i] = AnsiString(1+i);
        sgPMAlarmESDItem->Cells[1][1+i] = PMAlarm_SYS.PMItem_ESD.GetItem(i);
        sgPMAlarmESDItem->Cells[2][1+i] = PMAlarm_SYS.PMItem_ESD.GetLimitP(i);
        sgPMAlarmESDItem->Cells[3][1+i] = PMAlarm_SYS.PMItem_ESD.GetLimitN(i);

        PMAlarm_SYS.PMItem_ListESD.PushItem( PMAlarm_SYS.PMItem_ESD.GetItem(i) );
        PMAlarm_SYS.PMItem_ListESD.PushLimitP( PMAlarm_SYS.PMItem_ESD.GetLimitP(i) );
        PMAlarm_SYS.PMItem_ListESD.PushLimitN( PMAlarm_SYS.PMItem_ESD.GetLimitN(i) );
    }

    int iCount_IonFan = PMAlarm_SYS.PMItem_IonFan.GetCount();
    if( iCount_IonFan == 0 )
    {
        sgPMAlarmIonFanItem->RowCount = 2;
        sgPMAlarmIonFanItem->Cells[0][1] = AnsiString(1);
    }
    else
    {
        sgPMAlarmIonFanItem->RowCount = 1 + iCount_IonFan;
    }

    sgPMAlarmIonFanItem->Cells[0][0] = "No.";
    sgPMAlarmIonFanItem->Cells[1][0] = "PM Item";
    sgPMAlarmIonFanItem->Cells[2][0] = "Limit P";
    sgPMAlarmIonFanItem->Cells[3][0] = "Limit N";

    for( int i = 0 ; i < iCount_IonFan ; i++ )
    {
        sgPMAlarmIonFanItem->Cells[0][1+i] = AnsiString(1+i);
        sgPMAlarmIonFanItem->Cells[1][1+i] = PMAlarm_SYS.PMItem_IonFan.GetItem(i);
        sgPMAlarmIonFanItem->Cells[2][1+i] = PMAlarm_SYS.PMItem_IonFan.GetLimitP(i);
        sgPMAlarmIonFanItem->Cells[3][1+i] = PMAlarm_SYS.PMItem_IonFan.GetLimitN(i);

        PMAlarm_SYS.PMItem_ListIonFan.PushItem( PMAlarm_SYS.PMItem_IonFan.GetItem(i) );
        PMAlarm_SYS.PMItem_ListIonFan.PushLimitP( PMAlarm_SYS.PMItem_IonFan.GetLimitP(i) );
        PMAlarm_SYS.PMItem_ListIonFan.PushLimitN( PMAlarm_SYS.PMItem_IonFan.GetLimitN(i) );
    }

    int iPM_Date = PMAlarm_SYS.GetPMDate();
    cbPMAlarmDate->ItemIndex = iPM_Date-1;
    edtPMAlarmAlarmDays->Text  = AnsiString( PMAlarm_SYS.GetPMAlarmDays() );
    edtPMAlarmStopDays->Text   = AnsiString( PMAlarm_SYS.GetPMStopDays() );
    int iCount_PMList = PMAlarm_SYS.PMItem_List.GetCount();

    if( iCount_PMList == 0 )
    {
        sgGerneralPMAlarmCheckItem->RowCount = 2;
        sgGerneralPMAlarmCheckItem->Cells[0][1] = AnsiString(1);
    }
    else
    {
        sgGerneralPMAlarmCheckItem->RowCount = 1 + iCount_PMList;
    }

    sgGerneralPMAlarmCheckItem->Cells[0][0] = "No.";
    sgGerneralPMAlarmCheckItem->Cells[1][0] = "PM Item";
    sgGerneralPMAlarmCheckItem->Cells[2][0] = "Check";

    for( int i = 0 ; i < iCount_PMList ; i++ )
    {
        sgGerneralPMAlarmCheckItem->Cells[0][1+i] = AnsiString(1+i);
        sgGerneralPMAlarmCheckItem->Cells[1][1+i] = PMAlarm_SYS.PMItem_List.GetItem(i);
        if(bIsCheckOk == true)
        {
            sgGerneralPMAlarmCheckItem->Cells[2][1+i] = "";

            PMAlarm_SYS.PMItem_Check.PushItem( "" );
        }
    }

    int iCount_TemperaturePMList = PMAlarm_SYS.PMItem_ListTemperature.GetCount();
    if( iCount_TemperaturePMList == 0 )
    {
        sgTemperaturePMAlarmCheckItem->RowCount = 2;
        sgTemperaturePMAlarmCheckItem->Cells[0][1] = AnsiString(1);
    }
    else
    {
        sgTemperaturePMAlarmCheckItem->RowCount = 1 + iCount_TemperaturePMList;
    }

    sgTemperaturePMAlarmCheckItem->Cells[0][0] = "No.";
    sgTemperaturePMAlarmCheckItem->Cells[1][0] = "PM Item";
    sgTemperaturePMAlarmCheckItem->Cells[2][0] = "LowBaset Machine Displayed Temperature)";
    sgTemperaturePMAlarmCheckItem->Cells[3][0] = "LowBaset Actual Measurement Value";
    sgTemperaturePMAlarmCheckItem->Cells[4][0] = "LowBaset Compensation Value";
    sgTemperaturePMAlarmCheckItem->Cells[5][0] = "HighBaset Machine Displayed Temperature";
    sgTemperaturePMAlarmCheckItem->Cells[6][0] = "HighBaset Actual Measurement Value";
    sgTemperaturePMAlarmCheckItem->Cells[7][0] = "HighBaset Compensation Value";

    for( int i = 0 ; i < iCount_TemperaturePMList ; i++ )
    {
        sgTemperaturePMAlarmCheckItem->Cells[0][1+i] = AnsiString(1+i);
        sgTemperaturePMAlarmCheckItem->Cells[1][1+i] = PMAlarm_SYS.PMItem_ListTemperature.GetItem(i);
        if(bIsCheckOk == true)
        {
            sgTemperaturePMAlarmCheckItem->Cells[2][1+i] = "";

            //PMAlarm_SYS.PMItem_CheckTemperature.PushItem( "" );
        }
    }

    int iCount_ESDPMList = PMAlarm_SYS.PMItem_ListESD.GetCount();

    if( iCount_ESDPMList == 0 )
    {
        sgESDPMAlarmCheckItem->RowCount = 2;
        sgESDPMAlarmCheckItem->Cells[0][1] = AnsiString(1);
    }
    else
    {
        sgESDPMAlarmCheckItem->RowCount = 1 + iCount_ESDPMList;
    }

    sgESDPMAlarmCheckItem->Cells[0][0] = "No.";
    sgESDPMAlarmCheckItem->Cells[1][0] = "PM Item";
    sgESDPMAlarmCheckItem->Cells[2][0] = "Value";

    for( int i = 0 ; i < iCount_ESDPMList ; i++ )
    {
        sgESDPMAlarmCheckItem->Cells[0][1+i] = AnsiString(1+i);
        sgESDPMAlarmCheckItem->Cells[1][1+i] = PMAlarm_SYS.PMItem_ListESD.GetItem(i);
        if(bIsCheckOk == true)
        {
            sgESDPMAlarmCheckItem->Cells[2][1+i] = "";

            PMAlarm_SYS.PMItem_CheckESD.PushItem( "" );
        }
    }

    int iCount_IonFanPMList = PMAlarm_SYS.PMItem_ListIonFan.GetCount();

    if( iCount_IonFanPMList == 0 )
    {
        sgIonFanPMAlarmCheckItem->RowCount = 2;
        sgIonFanPMAlarmCheckItem->Cells[0][1] = AnsiString(1);
    }
    else
    {
        sgIonFanPMAlarmCheckItem->RowCount = 1 + iCount_IonFanPMList;
    }

    sgIonFanPMAlarmCheckItem->Cells[0][0] = "No.";
    sgIonFanPMAlarmCheckItem->Cells[1][0] = "PM Item";
    sgIonFanPMAlarmCheckItem->Cells[2][0] = "Value";

    for( int i = 0 ; i < iCount_IonFanPMList ; i++ )
    {
        sgIonFanPMAlarmCheckItem->Cells[0][1+i] = AnsiString(1+i);
        sgIonFanPMAlarmCheckItem->Cells[1][1+i] = PMAlarm_SYS.PMItem_ListIonFan.GetItem(i);
        if(bIsCheckOk == true)
        {
            sgIonFanPMAlarmCheckItem->Cells[2][1+i] = "";

            PMAlarm_SYS.PMItem_CheckIonFan.PushItem( "" );
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sgPMAlarmMonthItemSelectCell(
      TObject *Sender, int ACol, int ARow, bool &CanSelect)
{
    iPMAlarmSelectRow = ARow;
}
//---------------------------------------------------------------------------
// 2015.03.25 , Joye , PM Alarm ----------------------------------------------<<
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::FormShow(TObject *Sender)
{
    fShow=true;
    fPMAlarmInterFace->Height = 760;
    fPMAlarmInterFace->Width = 1024;

    UpdatePMAlarmToScreen();        // 2015.03.26 , Joye , PM Alarm
//    #ifndef SOFT_SIMULTE
    LoadGerneralPMAlarmFromCSV();
    LoadTemperaturePMAlarmFromCSV();
    LoadESDPMAlarmFromCSV();
    LoadIonFanPMAlarmFromCSV();
    TemperaturePMAlarmLoadFromSetup();
//    #endif
    if(pc_PMAlarmFuction->ActivePage == ts_PMItemCheck
    && pc_PMItemCheck->ActivePage == ts_GerneralItemCheck)
    {
        iPMAlarmSelectRow = 1;
        iPMAlarmSelectCol = 2;
    }
    #ifndef SOFT_SIMULTE
    //Sam 20171213 (Steven) : PM SOP Show
    //==>
    if(CUSTOMER_CODE==CC_Greatek)
    {
        ts_SOPItem->Visible=true;
        if(FileExists(IniConfig.asA28_1ShowPmSopReadFilePath)==true)
        {
            cpwbrwsrPmSOP->Refresh();
            WideString URL(IniConfig.asA28_1ShowPmSopReadFilePath);
            cpwbrwsrPmSOP->Navigate(URL);
        }
        else
        {
            ShowMyMessage("PM SOP Path File Abnormality!");
        }
    }
    else
    {
        ts_SOPItem->Visible =false;
    }
    //<==
    //Sam 20171213 (Steven) : PM SOP Show
    #endif
    pn_SetPMDate->Visible = false;
    mc_SetPMDate->Date = Now();
    DateSeparator='/';  // Sam 20210225 : 增加保護
    ed_PMDate->Text = mc_SetPMDate->Date.FormatString("YYY/MM/DD");
    QuarterYear->Caption = "Semi-Annual Maintenance Items"; //Sam 20190925 Modify PM Alarm Year to Half Year
    pc_PMItemCheck->ActivePageIndex =4;
    gbUpdateByServer->Visible=(CosFunction.bUsePMAlarmFunction && CosFunction.bOEEFunction && IniConfig.bA55EnablePMAlarmUpdateFromServerbyFTP);
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::bt_ResetPMDateClick(TObject *Sender)
{
    SaveGerneralPMAlarmToCSV();
    SaveTemperaturePMAlarmToCSV();
    SaveESDPMAlarmToCSV();
    SaveIonFanPMAlarmToCSV();

    TDateTime dtNow = Now();
    int wNowMonth=GetMonth(dtNow);
    int wNextMonth=0;
    for(int i=0; i<31; i++)  //找上個月的最後一天
    {
        dtNow = dtNow - 1;
        wNextMonth=GetMonth(dtNow);
        if(wNextMonth!=wNowMonth)
        {
            break;
        }
    }

    dtNow = dtNow + PMAlarm_SYS.GetPMDate();
    dtpPMAlarmNextPMDate->Date = dtNow;
    PMAlarm_SYS.SetNextPMDate( dtpPMAlarmNextPMDate->Date );
    PMAlarm_SYS.SavePMAlarm();
    UpdatePMAlarmToScreen();
    LoadGerneralPMAlarmFromCSV();
    LoadTemperaturePMAlarmFromCSV();
    LoadESDPMAlarmFromCSV();
    LoadIonFanPMAlarmFromCSV();
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sgGerneralPMAlarmCheckItemDrawCell(
      TObject *Sender, int ACol, int ARow, TRect &Rect,
      TGridDrawState State)
{
    TStringGrid *sgPtr;
    sgPtr = (TStringGrid *)Sender;

    if(iPMAlarmSelectRow == ARow &&
       iPMAlarmSelectCol == ACol)
    {
        sgPtr->Canvas->Pen->Color = clBlack;
        sgPtr->Canvas->Brush->Color = clLime;
        sgPtr->Canvas->Rectangle(Rect);

        AnsiString sTemp = sgPtr->Cells[iPMAlarmSelectCol][iPMAlarmSelectRow];
        int Px=Rect.Left;
        int Py=Rect.Top+(Rect.Height()/2)-(-1*sgPtr->Canvas->Font->Height)/2;
        sgPtr->Canvas->Pen->Color = clWhite;
        sgPtr->Canvas->TextRect(Rect,Px,Py,sTemp);
    }
    else
    {
        sgPtr->Canvas->Pen->Color = clBlack;
        sgPtr->Canvas->Brush->Color = clSilver;
    }

    sgPtr->Canvas->Brush->Color=clBlack;
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sgGerneralPMAlarmCheckItemSelectCell(
      TObject *Sender, int ACol, int ARow, bool &CanSelect)
{
    iPMAlarmSelectRow = ARow;
    iPMAlarmSelectCol = ACol;
    sgGerneralPMAlarmCheckItem->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sbPMAlarmCheck_ACCClickClick(TObject *Sender)
{
    if(iPMAlarmSelectRow < 0 ||
       iPMAlarmSelectCol < 0)
    {
        return;
    }

    TSpeedButton *sbPtr;
    sbPtr = (TSpeedButton *)Sender;
    AnsiString sCheck = "";
    AnsiString sItem = sgGerneralPMAlarmCheckItem->Cells[1][iPMAlarmSelectRow];
    AnsiString sExecutiveOwner = ed_ExecutiveItem->Text;
    AnsiString sConformOwner = ed_ConformItem->Text;

    if(sbPtr->Name == "sbPMAlarmCheck_ACCClick")
    {
        if(sItem == sExecutiveOwner)
        {
            sCheck = ed_ExecutiveOwner->Text;
        }
        else if(sItem == sConformOwner)
        {
            sCheck = ed_ConformOwner->Text;
        }
        else
        sCheck = "V";
    }
    else if(sbPtr->Name == "sbPMAlarmCheck_REJClick")
    {
        sCheck = "";
    }

    if(iPMAlarmSelectRow > -1 &&
       iPMAlarmSelectCol > -1)
    {
        if(iPMAlarmSelectCol != 2)
        {
            return;
        }
        sgGerneralPMAlarmCheckItem->Cells[iPMAlarmSelectCol][iPMAlarmSelectRow] = sCheck;
            if(iPMAlarmSelectRow < sgGerneralPMAlarmCheckItem->RowCount - 1)
            {
                iPMAlarmSelectRow++;
            }
            else
            {
                iPMAlarmSelectRow = 1;
            }
        sgGerneralPMAlarmCheckItem->Refresh();
    }
    UpdateToGerneralPMAlarmToCSV();
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sb_GerneralExecutiveOwnerCheckClick(
      TObject *Sender)
{
    if(bIsGerneralItemCheckOK(true) == false)
    {
        ShowMyMessage("Preventive Maintenance Item Check Error!!");
        return;
    }

    TSpeedButton *sbPtr;
    sbPtr = (TSpeedButton *)Sender;

    if(sbPtr->Name == "sb_GerneralExecutiveOwnerCheck")
    {
        if(ed_GerneralExecutiveOwner->Text == "")
        {
            ed_GerneralExecutiveOwner->Text = ed_ExecutiveOwner->Text + " " + ed_PMDate->Text;
        }
        else
        {
            ed_GerneralExecutiveOwner->Text = "";
        }
    }
    else if(sbPtr->Name == "sb_GerneralConformOwnerCheck")
    {

        if(ed_GerneralExecutiveOwner->Text == "")
        {
            ShowMyMessage("Operator Not Yet Confirmed!!");
            return;
        }

        if(ed_GerneralConformOwner->Text == "")
        {
            ed_GerneralConformOwner->Text = ed_ConformOwner->Text + " " + ed_PMDate->Text;
        }
        else
        {
            ed_GerneralConformOwner->Text = "";
        }
    }
    UpdateToGerneralPMAlarmToCSV();
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::pc_PMItemCheckChange(TObject *Sender)
{
    if(pc_PMItemCheck->ActivePage == ts_GerneralItemCheck)
    {
        iPMAlarmSelectRow = 1;
        iPMAlarmSelectCol = 2;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::pc_PMAlarmFuctionChange(TObject *Sender)
{
    SaveSetting();
    PMAlarm_SYS.SavePMAlarm();

    if(pc_PMAlarmFuction->ActivePage == ts_PMItemCheck
    && pc_PMItemCheck->ActivePage == ts_GerneralItemCheck)
    {
        iPMAlarmSelectRow = 1;
        iPMAlarmSelectCol = 2;
    }
}
//---------------------------------------------------------------------------
bool TfPMAlarmInterFace::bIsGerneralItemCheckOK(bool bIsItemCheckOnly)
{
    AnsiString sItem;
    PMAlarm_SYS.PMItem_Check.ClearList();
    int iCount_List = sgGerneralPMAlarmCheckItem->RowCount - 1;

    for( int i = 0 ; i < iCount_List ; i++ )
    {
        sItem = sgGerneralPMAlarmCheckItem->Cells[2][i+1];

        PMAlarm_SYS.PMItem_Check.PushItem( sItem );
    }

    int iCount_PMCheck = PMAlarm_SYS.PMItem_Check.GetCount();
    bool bCheckOK = true;
    for( int i = 0 ; i < iCount_PMCheck ; i++ )
    {
        if( PMAlarm_SYS.PMItem_Check.GetItem(i) != "V" )
        {
            bCheckOK = false;
            break;
        }
    }

    if(bCheckOK == false)
    {
        ed_GerneralExecutiveOwner->Text = "";
        ed_GerneralConformOwner->Text = "";
    }

    if(bIsItemCheckOnly == false)
    {
        if(ed_GerneralExecutiveOwner->Text == "" ||
           ed_GerneralConformOwner->Text == "")
        {
            bCheckOK = false;
        }
    }

    return bCheckOK;
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sbPMAlarmCheck_CheckClick(
      TObject *Sender)
{
    if(PMAllAlarmCheckOK()==true)
    {
        ShowMyMessage("All Inspection and Verification Items Completed!");
    }
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sbPMAlarmCheck_IsAlarmClick(
      TObject *Sender)
{
    bool bAlarm = PMAlarm_SYS.IsAlarmDate(true);
    bool bStop = PMAlarm_SYS.IsStopDate(true);

    if(bAlarm == false &&
       bStop == false)
    {
        ShowMyMessage("No PM Time Reached");
    }
}
//---------------------------------------------------------------------------
void TfPMAlarmInterFace::LoadGerneralPMAlarmFromCSV()
{
    TStringList *slGerneralPMAlarm;
    AnsiString sTemp = "";
    int wNowYear=GetYear(Now());
    slGerneralPMAlarm = new TStringList;
    slGerneralPMAlarm->Clear();
    AnsiString sFullFileName=GetFullFileName(IntToStr(wNowYear), "GerneralPMAlarm.csv");
    bool bFileExists = false;
    if(FileExists(sFullFileName) == true)
    {
        slGerneralPMAlarm->LoadFromFile(sFullFileName);
        bFileExists = true;
    }

    int iTotalItemCount = PMAlarm_SYS.PMItem_Month.slItemList->Count   +
                          PMAlarm_SYS.PMItem_Quarter.slItemList->Count +
                          PMAlarm_SYS.PMItem_Year.slItemList->Count + 1 + 2;

    InitialGrid(sg_GerneralPMAlarmToCSV,iTotalItemCount);

    TDateTime dtDate = dtpPMAlarmNextPMDate->Date;
    int wMonth=GetMonth(dtDate);
    int iStartCount = 1;
    int iEndCount = PMAlarm_SYS.PMItem_Month.slItemList->Count+1;
    for(int iy=iStartCount;iy<iEndCount;iy++)
    {
        sg_GerneralPMAlarmToCSV->Cells[0][iy] = IntToStr(iy);
        sTemp = PMAlarm_SYS.PMItem_Month.slItemList->Strings[iy-iStartCount];
        sg_GerneralPMAlarmToCSV->Cells[1][iy] = sTemp;

        sg_GerneralPMAlarmToCSV->Cells[2][iy] = "Month";
    }
    iStartCount =PMAlarm_SYS.PMItem_Month.slItemList->Count+1;
    iEndCount   =PMAlarm_SYS.PMItem_Month.slItemList->Count  +
                 PMAlarm_SYS.PMItem_Quarter.slItemList->Count+1;

    for(int iy=iStartCount;iy<iEndCount;iy++)
    {
        sg_GerneralPMAlarmToCSV->Cells[0][iy] = IntToStr(iy);
        sTemp = PMAlarm_SYS.PMItem_Quarter.slItemList->Strings[iy-iStartCount];
        sg_GerneralPMAlarmToCSV->Cells[1][iy] = sTemp;

        sg_GerneralPMAlarmToCSV->Cells[2][iy] = "Quarte";
    }
    iStartCount=PMAlarm_SYS.PMItem_Month.slItemList->Count  +
                PMAlarm_SYS.PMItem_Quarter.slItemList->Count+1;

    iEndCount =PMAlarm_SYS.PMItem_Month.slItemList->Count   +
               PMAlarm_SYS.PMItem_Quarter.slItemList->Count +
               PMAlarm_SYS.PMItem_Year.slItemList->Count    +1;

    for(int iy=iStartCount;iy<iEndCount;iy++)
    {
        sg_GerneralPMAlarmToCSV->Cells[0][iy] = IntToStr(iy);
        sTemp = PMAlarm_SYS.PMItem_Year.slItemList->Strings[iy-iStartCount];
        sg_GerneralPMAlarmToCSV->Cells[1][iy] = sTemp;
        sg_GerneralPMAlarmToCSV->Cells[2][iy] = "Semi-Annual/Annual"; //Sam 20190925 Modify PM Alarm Year to Half Year
    }
    sg_GerneralPMAlarmToCSV->Cells[1][iEndCount] = ed_ExecutiveItem->Text;
    sg_GerneralPMAlarmToCSV->Cells[1][iEndCount + 1] = ed_ConformItem->Text;
    sg_GerneralPMAlarmToCSV->Cells[0][iEndCount + 2] = "Doc Version";
    sg_GerneralPMAlarmToCSV->Cells[1][iEndCount + 2] = ed_GerneralPMDocVer->Text;

    if(bFileExists == true)
    {
        for(int iy=0;iy<iTotalItemCount;iy++)
        {
            sTemp = slGerneralPMAlarm->Strings[iy];

            for(int ix=0;ix<16;ix++)
            {
                if((ix - 3) == wMonth)
                {

                }

                sg_GerneralPMAlarmToCSV->Cells[ix][iy] = GetCsvData(ix+1,sTemp); //取得CSV格式中的第幾筆資料;

                if((ix - 3) == wMonth)
                {
                    for(int i=0; i<sgGerneralPMAlarmCheckItem->RowCount; i++)
                    {
                        AnsiString s1 = sg_GerneralPMAlarmToCSV->Cells[1][iy];
                        AnsiString s2 = sgGerneralPMAlarmCheckItem->Cells[1][i];
                        //AnsiString s3 = GetCsvData(ix,sTemp);
                        if(s1 == s2)
                        {
                            sgGerneralPMAlarmCheckItem->Cells[2][i] = GetCsvData(ix,sTemp); //取得CSV格式中的第幾筆資料;
                             break;
                        }
                    }

                    AnsiString s3 = GetCsvData(ix,sTemp);
                    if(s3.Pos(ed_ExecutiveOwner->Text) == 1)
                    {
                        ed_GerneralExecutiveOwner->Text = GetCsvData(ix,sTemp);
                        //continue;
                    }
                    if(s3.Pos(ed_ConformOwner->Text) == 1)
                    {
                        ed_GerneralConformOwner->Text = GetCsvData(ix,sTemp);
                        //continue;
                    }
                }
            }
        }
    }
    else
    {
    }
    slGerneralPMAlarm->Clear(); //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete slGerneralPMAlarm;
}
//---------------------------------------------------------------------------
void TfPMAlarmInterFace::SaveGerneralPMAlarmToCSV()
{
    TStringList *slGerneralPMAlarm;
    AnsiString sTemp = "";
    int wNowYear=GetYear(Now());
    slGerneralPMAlarm = new TStringList;
    slGerneralPMAlarm->Clear();
    AnsiString sFullFileName=GetFullFileName(IntToStr(wNowYear),"GerneralPMAlarm.csv");
    int iTotalItemCount = 0;
    iTotalItemCount =PMAlarm_SYS.PMItem_Month.slItemList->Count  +
                     PMAlarm_SYS.PMItem_Quarter.slItemList->Count+
                     PMAlarm_SYS.PMItem_Year.slItemList->Count   +1 + 2 + 2;
    for(int iy=0;iy<iTotalItemCount;iy++)
    {
        sTemp = "";
        for(int ix=0;ix<16;ix++)
        {
            sTemp = sTemp + sg_GerneralPMAlarmToCSV->Cells[ix][iy];
            if(ix != 15)
            {
                sTemp = sTemp + ",";
            }
        }
        slGerneralPMAlarm->Add(sTemp);
    }
    slGerneralPMAlarm->SaveToFile(sFullFileName);
    slGerneralPMAlarm->Clear(); //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete slGerneralPMAlarm;
}
//------------------------------------------------------------------------------
void TfPMAlarmInterFace::UpdateToGerneralPMAlarmToCSV()
{
    AnsiString sTemp = "";

    int iTotalItemCount = 0;
    iTotalItemCount =PMAlarm_SYS.PMItem_Month.slItemList->Count  +
                     PMAlarm_SYS.PMItem_Quarter.slItemList->Count+
                     PMAlarm_SYS.PMItem_Year.slItemList->Count   + 1 + 2;

    TDateTime dtDate = dtpPMAlarmNextPMDate->Date;
    int wMonth=GetMonth(dtDate);
    for(int iy=0;iy<iTotalItemCount;iy++)
    {
        sTemp = "";
        for(int ix=0;ix<16;ix++)    // 2015.11.06 , Joye , PM Alarm Can't save
        {
            if(ix == wMonth + 2)
            {
                for(int i=0; i<sgGerneralPMAlarmCheckItem->RowCount; i++)
                {
                    AnsiString s1 = sgGerneralPMAlarmCheckItem->Cells[1][i];
                    AnsiString s2 = sg_GerneralPMAlarmToCSV->Cells[1][iy];
                    if(s1 == s2)
                    {
                        sg_GerneralPMAlarmToCSV->Cells[wMonth + 2][iy] = sgGerneralPMAlarmCheckItem->Cells[2][i];
                    }
                }
                if(iy == (iTotalItemCount-2))
                {
                    sg_GerneralPMAlarmToCSV->Cells[wMonth + 2][iy] = ed_GerneralExecutiveOwner->Text;
                }
                else if(iy == (iTotalItemCount-1))
                {
                    sg_GerneralPMAlarmToCSV->Cells[wMonth + 2][iy] = ed_GerneralConformOwner->Text;
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::SpeedButton9Click(TObject *Sender)
{
    SaveGerneralPMAlarmToCSV();
    SaveTemperaturePMAlarmToCSV();
    SaveESDPMAlarmToCSV();
    SaveIonFanPMAlarmToCSV();
}
//---------------------------------------------------------------------------
//取得CSV格式中的第幾筆資料
AnsiString TfPMAlarmInterFace::GetCsvData(int iDataNum,AnsiString sCSVStr) //取得CSV格式中的第幾筆資料
{
    //從1開始
    AnsiString sRes = "";
    for(int i=0; i<iDataNum - 1; i++)
    {
        if(sCSVStr.Pos(",") > 0)
        {
            sCSVStr = sCSVStr.SubString((sCSVStr.Pos(",") + 1),(sCSVStr.Length() - sCSVStr.Pos(",")));
        }
        else
        {
            sCSVStr = "";
        }
    }
    sRes = sCSVStr;
    if(sCSVStr.Pos(",") > 0)
    {
        sRes = sCSVStr.SubString(1,sCSVStr.Pos(",")-1);
    }
    return sRes;
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::SpeedButton10Click(TObject *Sender)
{
    LoadGerneralPMAlarmFromCSV();
    LoadTemperaturePMAlarmFromCSV();
    LoadESDPMAlarmFromCSV();
    LoadIonFanPMAlarmFromCSV();
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::FormCloseQuery(TObject *Sender,
      bool &CanClose)
{
    #ifndef SOFT_SIMULTE
    SaveGerneralPMAlarmToCSV();
    SaveTemperaturePMAlarmToCSV();
    SaveESDPMAlarmToCSV();
    SaveIonFanPMAlarmToCSV();
    SaveSetting();
    PMAlarm_SYS.SavePMAlarm();
    #endif
    fShow=false;
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sbPMAlarmTemperature_AddClick(
      TObject *Sender)
{
    int iRow = sgPMAlarmTemperatureItem->RowCount;
    StringGrid_Insert_Row( sgPMAlarmTemperatureItem , iRow );
    sgPMAlarmTemperatureItem->Cells[0][iRow] = AnsiString( iRow );
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sbPMAlarmTemperature_DeleteClick(
      TObject *Sender)
{
    int iRow = sgPMAlarmTemperatureItem->RowCount;
    if( iRow == 2 )
    {
        iPMAlarmTemperatureSelectRow = 1;
    }

    if( iPMAlarmTemperatureSelectRow <= 0 || iPMAlarmTemperatureSelectRow >= iRow )
    {
        iPMAlarmTemperatureSelectRow = -1;
        return;
    }

    StringGrid_Delete_Row( sgPMAlarmTemperatureItem , iPMAlarmTemperatureSelectRow );
    iPMAlarmTemperatureSelectRow = -1;
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sgPMAlarmTemperatureItemSelectCell(
      TObject *Sender, int ACol, int ARow, bool &CanSelect)
{
    iPMAlarmTemperatureSelectRow = ARow;
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sgTemperaturePMAlarmCheckItemSelectCell(
      TObject *Sender, int ACol, int ARow, bool &CanSelect)
{
    iPMAlarmTemperatureSelectRow = ARow;
    iPMAlarmTemperatureSelectCol = ACol;
    sgTemperaturePMAlarmCheckItem->Refresh();
}
//---------------------------------------------------------------------------
bool TfPMAlarmInterFace::bIsTemperatureItemCheckOK(bool bIsItemCheckOnly)
{
    TDateTime dtDate = dtpPMAlarmNextPMDate->Date;
    int wMonth=GetMonth(dtDate);
    if(wMonth == 1 || wMonth == 4 || wMonth == 7 || wMonth == 10)
    {
    }
    else
    {
        return true;
    }
    bool bCheckOK = true;
    int iRowCount = sgTemperaturePMAlarmCheckItem->RowCount;
    int iColCount = sgTemperaturePMAlarmCheckItem->ColCount;

    for(int ix=1;ix<iColCount;ix++)
    {
        for(int iy=1;iy<iRowCount;iy++)
        {
            if(sgTemperaturePMAlarmCheckItem->Cells[0][iy] != ""
            && sgTemperaturePMAlarmCheckItem->Cells[ix][iy] == "")
            {
                bCheckOK=false;
                //Jimmychiu 20230414 : PMAlarm溫度補償無填值導致異常修復
                //<==
                ed_TemperatureExecutiveOwner->Text = "";
                ed_TemperatureConformOwner->Text = "";
                return bCheckOK;
                //<==
                //Jimmychiu 20230414 : PMAlarm溫度補償無填值導致異常修復
            }
        }
    }

    if(bCheckOK == false)
    {
        ed_TemperatureExecutiveOwner->Text = "";
        ed_TemperatureConformOwner->Text = "";
    }

    if(bIsItemCheckOnly == false)
    {
        if(ed_TemperatureExecutiveOwner->Text == "" ||
           ed_TemperatureConformOwner->Text   == "")
        {
            bCheckOK = false;
        }
    }

    return bCheckOK;
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sb_TemperatureExecutiveOwnerCheckClick(
      TObject *Sender)
{
    if(bIsTemperatureItemCheckOK(true) == false)
    {
        ShowMyMessage("Temperature Calibration Record Item Error!!");
        return;
    }

    TSpeedButton *sbPtr;
    sbPtr = (TSpeedButton *)Sender;

    if(sbPtr->Name == "sb_TemperatureExecutiveOwnerCheck")
    {
        if(ed_TemperatureExecutiveOwner->Text == "")
        {
            ed_TemperatureExecutiveOwner->Text = ed_ExecutiveOwner->Text + " " + ed_PMDate->Text;
        }
        else
        {
            ed_TemperatureExecutiveOwner->Text = "";
        }
    }
    else if(sbPtr->Name == "sb_TemperatureConformOwnerCheck")
    {

        if(ed_TemperatureExecutiveOwner->Text == "")
        {
            ShowMyMessage("Operator Not Yet Confirmed!!");
            return;
        }

        if(ed_TemperatureConformOwner->Text == "")
        {
            ed_TemperatureConformOwner->Text = ed_ConformOwner->Text + " " + ed_PMDate->Text;
        }
        else
        {
            ed_TemperatureConformOwner->Text = "";
        }
    }

    UpdateToTemperaturePMAlarmToCSV();
}
//---------------------------------------------------------------------------
void TfPMAlarmInterFace::UpdateToTemperaturePMAlarmToCSV()
{
    AnsiString sTemp = "";

    int iTotalItemCount = 0;
    iTotalItemCount = PMAlarm_SYS.PMItem_Temperature.slItemList->Count + 1 + 2;

    TDateTime dtDate = dtpPMAlarmNextPMDate->Date;
    int wMonth=GetMonth(dtDate);
    int iaQuarter[] = {0,1,1,1,2,2,2,3,3,3,4,4,4};
    int iStartQuarter = iaQuarter[wMonth];

    for(int iq=0;iq<4;iq++)
    {
        for(int iy=0;iy<iTotalItemCount;iy++)
        {
            int iyToCSVStart = iTotalItemCount*iq + iy;
            sTemp = "";
            for(int ix=0;ix<8;ix++)
            {
                if(iy == 0 || ix == 0 || ix == 1)
                {
                    sg_TemperaturePMAlarmToCSV->Cells[ix][iyToCSVStart] = sgTemperaturePMAlarmCheckItem->Cells[ix][iy];
                }
                if(iy == 0 && ix == 0)
                {
                    sg_TemperaturePMAlarmToCSV->Cells[ix][iyToCSVStart] = "Quarter " + IntToStr(iq + 1) + " No.";
                }
            }
        }
        sg_TemperaturePMAlarmToCSV->Cells[1][iTotalItemCount*iq + iTotalItemCount - 2] = ed_ExecutiveItem->Text;
        sg_TemperaturePMAlarmToCSV->Cells[1][iTotalItemCount*iq + iTotalItemCount - 1] = ed_ConformItem->Text;
    }

    for(int iy=0;iy<iTotalItemCount;iy++)
    {
        int iyToCSVStart = iTotalItemCount*(iStartQuarter-1) + iy;
        sTemp = "";
        for(int ix=0;ix<8;ix++)
        {
            if(iy < (iTotalItemCount-2))
            {
                sg_TemperaturePMAlarmToCSV->Cells[ix][iyToCSVStart] = sgTemperaturePMAlarmCheckItem->Cells[ix][iy];
            }
        }
        if(iy == (iTotalItemCount-2))
        {
            sg_TemperaturePMAlarmToCSV->Cells[2][iyToCSVStart] = ed_TemperatureExecutiveOwner->Text;
        }
        else if(iy == (iTotalItemCount-1))
        {
            sg_TemperaturePMAlarmToCSV->Cells[2][iyToCSVStart] = ed_TemperatureConformOwner->Text;
        }
    }
}
//---------------------------------------------------------------------------
void TfPMAlarmInterFace::SaveTemperaturePMAlarmToCSV()
{
    TStringList *slTemperaturePMAlarm;
    AnsiString sTemp = "";
    int wNowYear=GetYear(Now());
    slTemperaturePMAlarm = new TStringList;
    slTemperaturePMAlarm->Clear();
    AnsiString sFullFileName=GetFullFileName(IntToStr(wNowYear),"TemperaturePMAlarm.csv");
    int iTotalItemCount = 0;
    iTotalItemCount = PMAlarm_SYS.PMItem_Temperature.slItemList->Count + 1 + 2 + 2;

    for(int iy=0;iy<iTotalItemCount*4;iy++)
    {
        sTemp = "";
        for(int ix=0;ix<16;ix++)
        {
            sTemp = sTemp + sg_TemperaturePMAlarmToCSV->Cells[ix][iy];

            if(ix != 15)
            {
                sTemp = sTemp + ",";
            }
        }
        slTemperaturePMAlarm->Add(sTemp);
    }

    slTemperaturePMAlarm->SaveToFile(sFullFileName);
    slTemperaturePMAlarm->Clear();  //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete slTemperaturePMAlarm;
}
//---------------------------------------------------------------------------
void TfPMAlarmInterFace::LoadTemperaturePMAlarmFromCSV()
{
    TStringList *slTemperaturePMAlarm;
    AnsiString sTemp = "";
    int wNowYear=GetYear(Now());
    slTemperaturePMAlarm = new TStringList;
    slTemperaturePMAlarm->Clear();
    AnsiString sFullFileName=GetFullFileName(IntToStr(wNowYear),"TemperaturePMAlarm.csv");
    bool bFileExists = false;
    if(FileExists(sFullFileName) == true)
    {
        slTemperaturePMAlarm->LoadFromFile(sFullFileName);
        bFileExists = true;
    }

    int iTotalItemCount = PMAlarm_SYS.PMItem_Temperature.slItemList->Count + 1 + 2;

    for(int ix=0;ix<16;ix++)
    {
        for(int iy=0;iy<iTotalItemCount*4;iy++)
        {
            sg_TemperaturePMAlarmToCSV->Cells[ix][iy] = "";
        }
    }

    TDateTime dtDate = dtpPMAlarmNextPMDate->Date;
    int wMonth=GetMonth(dtDate);
    if(wMonth == 1 || wMonth == 4 || wMonth == 7 || wMonth == 10)
    {
        ts_TemperatureCheck->TabVisible = true;
    }
    else
    {
        ts_TemperatureCheck->TabVisible = false;
    }

    int iaQuarter[] = {0,1,1,1,2,2,2,3,3,3,4,4,4};
    int iStartQuarter = iaQuarter[wMonth];

    //寫出標頭
    for(int iq=0;iq<4;iq++)
    {
        for(int iy=0;iy<iTotalItemCount;iy++)
        {
            int iyToCSVStart = iTotalItemCount*iq + iy;
            sTemp = "";
            for(int ix=0;ix<8;ix++)
            {
                if(iy == 0 || ix == 0 || ix == 1)
                {
                    sg_TemperaturePMAlarmToCSV->Cells[ix][iyToCSVStart] = sgTemperaturePMAlarmCheckItem->Cells[ix][iy];
                }
                if(iy == 0 && ix == 0)
                {
                    sg_TemperaturePMAlarmToCSV->Cells[ix][iyToCSVStart] = "Quarter " + IntToStr(iq + 1) + " No.";
                }
            }
        }
        sg_TemperaturePMAlarmToCSV->Cells[1][iTotalItemCount*iq + iTotalItemCount - 2] = ed_ExecutiveItem->Text;
        sg_TemperaturePMAlarmToCSV->Cells[1][iTotalItemCount*iq + iTotalItemCount - 1] = ed_ConformItem->Text;
    }
    sg_TemperaturePMAlarmToCSV->Cells[0][iTotalItemCount*4] = "Doc Version";
    sg_TemperaturePMAlarmToCSV->Cells[1][iTotalItemCount*4] = ed_TemperaturePMDocVer->Text;

    //***************************
    if(bFileExists == true)
    {
        for(int iq=0;iq<4;iq++)
        {
            for(int iy=0;iy<iTotalItemCount;iy++)
            {
                int iyToCSVStart = iTotalItemCount*iq + iy;

                sTemp = slTemperaturePMAlarm->Strings[iyToCSVStart];

                for(int ix=0;ix<8;ix++)
                {
                    sg_TemperaturePMAlarmToCSV->Cells[ix][iyToCSVStart] = GetCsvData(ix+1,sTemp); //取得CSV格式中的第幾筆資料;

                    if((iq + 1)  == iStartQuarter)
                    {
                        sgTemperaturePMAlarmCheckItem->Cells[ix][iy] = GetCsvData(ix+1,sTemp); //取得CSV格式中的第幾筆資料;

                        AnsiString s3 = GetCsvData(ix+1,sTemp);
                        if(s3.Pos(ed_ExecutiveOwner->Text) == 1)
                        {
                            ed_TemperatureExecutiveOwner->Text = GetCsvData(ix+1,sTemp);
                            //continue;
                        }
                        if(s3.Pos(ed_ConformOwner->Text) == 1)
                        {
                            ed_TemperatureConformOwner->Text = GetCsvData(ix+1,sTemp);
                            //continue;
                        }
                    }
                }
            }
        }
    }
    else
    {
    }
    slTemperaturePMAlarm->Clear();  //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete slTemperaturePMAlarm;
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sbPMAlarmESD_AddClick(TObject *Sender)
{
    int iRow = sgPMAlarmESDItem->RowCount;
    StringGrid_Insert_Row( sgPMAlarmESDItem , iRow );
    sgPMAlarmESDItem->Cells[0][iRow] = AnsiString( iRow );
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sbPMAlarmESD_DeleteClick(
      TObject *Sender)
{
    int iRow = sgPMAlarmESDItem->RowCount;

    if( iRow == 2 )
    {
        iPMAlarmESDSelectRow = 1;
    }

    if( iPMAlarmESDSelectRow <= 0 || iPMAlarmESDSelectRow >= iRow )
    {
        iPMAlarmESDSelectRow = -1;
        return;
    }
    StringGrid_Delete_Row( sgPMAlarmESDItem , iPMAlarmESDSelectRow );
    iPMAlarmESDSelectRow = -1;
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sbPMAlarmIonFan_AddClick(
      TObject *Sender)
{
    int iRow = sgPMAlarmIonFanItem->RowCount;
    StringGrid_Insert_Row( sgPMAlarmIonFanItem , iRow );
    sgPMAlarmIonFanItem->Cells[0][iRow] = AnsiString( iRow );
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sbPMAlarmIonFan_DeleteClick(
      TObject *Sender)
{
    int iRow = sgPMAlarmIonFanItem->RowCount;
    if( iRow == 2 )
    {
        iPMAlarmIonFanSelectRow = 1;
    }

    if( iPMAlarmIonFanSelectRow <= 0 || iPMAlarmIonFanSelectRow >= iRow )
    {
        iPMAlarmIonFanSelectRow = -1;
        return;
    }

    StringGrid_Delete_Row( sgPMAlarmIonFanItem , iPMAlarmIonFanSelectRow );
    iPMAlarmIonFanSelectRow = -1;
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sgPMAlarmESDItemSelectCell(
      TObject *Sender, int ACol, int ARow, bool &CanSelect)
{
    iPMAlarmESDSelectRow = ARow;
}
//---------------------------------------------------------------------------

void __fastcall TfPMAlarmInterFace::sgPMAlarmIonFanItemSelectCell(
      TObject *Sender, int ACol, int ARow, bool &CanSelect)
{
    iPMAlarmIonFanSelectRow = ARow;
}
//---------------------------------------------------------------------------
bool TfPMAlarmInterFace::bIsESDItemCheckOK(bool bIsItemCheckOnly)
{
    AnsiString sItem;
    PMAlarm_SYS.PMItem_CheckESD.ClearList();
    int iCount_List = sgESDPMAlarmCheckItem->RowCount - 1;
    for( int i = 0 ; i < iCount_List ; i++ )
    {
        sItem = sgESDPMAlarmCheckItem->Cells[2][i+1];
        PMAlarm_SYS.PMItem_CheckESD.PushItem( sItem );
    }

    int iCount_PMCheckESD = PMAlarm_SYS.PMItem_CheckESD.GetCount();

    bool bCheckOK = true;
    for( int i = 0 ; i < iCount_PMCheckESD ; i++ )
    {
        if( PMAlarm_SYS.PMItem_CheckESD.GetItem(i) == "" )
        {
            bCheckOK = false;
            break;
        }

        AnsiString sValue = PMAlarm_SYS.PMItem_CheckESD.GetItem(i);
        AnsiString sLimitP = PMAlarm_SYS.PMItem_ESD.GetLimitP(i);
        AnsiString sLimitN = PMAlarm_SYS.PMItem_ESD.GetLimitN(i);

        double fValue = atof(sValue.c_str());
        double fLimitP = atof(sLimitP.c_str());
        double fLimitN = atof(sLimitN.c_str());

        if(sLimitP == "" || sLimitN == "")
        {
            continue;
        }

        if(sLimitP != "0" && fLimitP == 0 ||
           sLimitN != "0" && fLimitN == 0 ||
           fLimitN >= fLimitP)
        {
            ShowMyMessage("E.S.D. Record Item Limit Setting Error");
            bCheckOK = false;
            break;
        }

        if(fValue >= fLimitP || fValue <= fLimitN)                              //Jimmychiu 20240226 : 政賢要求修改PM值判斷條件
        {
            ShowMyMessage("E.S.D. Record Check Out of Range Error");
            bCheckOK = false;
            break;
        }
    }

    if(bCheckOK == false)
    {
        ed_ESDExecutiveOwner->Text = "";
        ed_ESDConformOwner->Text = "";
    }

    if(bIsItemCheckOnly == false)
    {
        if(ed_ESDExecutiveOwner->Text == "" ||
           ed_ESDConformOwner->Text == "")
        {
            bCheckOK = false;
        }
    }

    return bCheckOK;
}
//---------------------------------------------------------------------------
bool TfPMAlarmInterFace::bIsIonFanItemCheckOK(bool bIsItemCheckOnly)
{
    AnsiString sItem;
    PMAlarm_SYS.PMItem_CheckIonFan.ClearList();
    int iCount_List = sgIonFanPMAlarmCheckItem->RowCount - 1;

    for( int i = 0 ; i < iCount_List ; i++ )
    {
        sItem = sgIonFanPMAlarmCheckItem->Cells[2][i+1];
        PMAlarm_SYS.PMItem_CheckIonFan.PushItem( sItem );
    }

    int iCount_PMCheckIonFan = PMAlarm_SYS.PMItem_CheckIonFan.GetCount();
    bool bCheckOK = true;
    for( int i = 0 ; i < iCount_PMCheckIonFan ; i++ )
    {
        if( PMAlarm_SYS.PMItem_CheckIonFan.GetItem(i) == "" )
        {
            bCheckOK = false;
            break;
        }

        AnsiString sValue = PMAlarm_SYS.PMItem_CheckIonFan.GetItem(i);
        AnsiString sLimitP = PMAlarm_SYS.PMItem_IonFan.GetLimitP(i);
        AnsiString sLimitN = PMAlarm_SYS.PMItem_IonFan.GetLimitN(i);

        double fValue = atof(sValue.c_str());
        double fLimitP = atof(sLimitP.c_str());
        double fLimitN = atof(sLimitN.c_str());

        if(sLimitP == "" || sLimitN == "")
        {
            continue;
        }

        if(sLimitP != "0" && fLimitP == 0 ||
           sLimitN != "0" && fLimitN == 0 ||
           fLimitN >= fLimitP)
        {
            ShowMyMessage("ION FAN Measurement Record Item Limit Setting Error");
            bCheckOK = false;
            break;
        }

        if(fValue >= fLimitP || fValue <= fLimitN)                              //Jimmychiu 20240226 : 政賢要求修改PM值判斷條件
        {
            ShowMyMessage("ION FAN Measurement Record Out of Range Error");
            bCheckOK = false;
            break;
        }
    }

    if(bCheckOK == false)
    {
        ed_IonFanExecutiveOwner->Text = "";
        ed_IonFanConformOwner->Text = "";
    }

    if(bIsItemCheckOnly == false)
    {
        if(ed_IonFanExecutiveOwner->Text == "" ||
           ed_IonFanConformOwner->Text == "")
        {
            bCheckOK = false;
        }
    }

    return bCheckOK;
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sb_ESDExecutiveOwnerCheckClick(
      TObject *Sender)
{
    if(bIsESDItemCheckOK(true) == false)
    {
        ShowMyMessage("ESD Record Item Check Error!!");
        return;
    }

    TSpeedButton *sbPtr;
    sbPtr = (TSpeedButton *)Sender;

    if(sbPtr->Name == "sb_ESDExecutiveOwnerCheck")
    {
        if(ed_ESDExecutiveOwner->Text == "")
        {
            ed_ESDExecutiveOwner->Text = ed_ExecutiveOwner->Text + " " + ed_PMDate->Text;
        }
        else
        {
            ed_ESDExecutiveOwner->Text = "";
        }
    }
    else if(sbPtr->Name == "sb_ESDConformOwnerCheck")
    {
        if(ed_ESDExecutiveOwner->Text == "")
        {
            ShowMyMessage("Operator Not Yet Confirmed!!");
            return;
        }

        if(ed_ESDConformOwner->Text == "")
        {
            ed_ESDConformOwner->Text = ed_ConformOwner->Text + " " + ed_PMDate->Text;
        }
        else
        {
            ed_ESDConformOwner->Text = "";
        }
    }
    UpdateToESDPMAlarmToCSV();
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sb_IonFanExecutiveOwnerCheckClick(
      TObject *Sender)
{
    if(bIsIonFanItemCheckOK(true) == false)
    {
        ShowMyMessage("Ion Fan Measurement Record Item Check Error!!");
        return;
    }

    TSpeedButton *sbPtr;
    sbPtr = (TSpeedButton *)Sender;

    if(sbPtr->Name == "sb_IonFanExecutiveOwnerCheck")
    {
        if(ed_IonFanExecutiveOwner->Text == "")
        {
            ed_IonFanExecutiveOwner->Text = ed_ExecutiveOwner->Text + " " + ed_PMDate->Text;
        }
        else
        {
            ed_IonFanExecutiveOwner->Text = "";
        }
    }
    else if(sbPtr->Name == "sb_IonFanConformOwnerCheck")
    {
        if(ed_IonFanExecutiveOwner->Text == "")
        {
            ShowMyMessage("Operator Not Yet Confirmed!!");
            return;
        }

        if(ed_IonFanConformOwner->Text == "")
        {
            ed_IonFanConformOwner->Text = ed_ConformOwner->Text + " " + ed_PMDate->Text;
        }
        else
        {
            ed_IonFanConformOwner->Text = "";
        }
    }

    UpdateToIonFanPMAlarmToCSV();
}
//---------------------------------------------------------------------------
void TfPMAlarmInterFace::UpdateToESDPMAlarmToCSV()
{
    AnsiString sTemp = "";

    int iTotalItemCount = 0;
    iTotalItemCount = PMAlarm_SYS.PMItem_ESD.slItemList->Count + 1 + 2;

    TDateTime dtDate = dtpPMAlarmNextPMDate->Date;
    int wMonth=GetMonth(dtDate);
    for(int iy=0;iy<iTotalItemCount;iy++)
    {
        sTemp = "";
        //for(int ix=0;ix<13;ix++)
        for(int ix=0;ix<16;ix++)    // 2015.11.06 , Joye , PM Alarm Can't saves
        {
            if(ix == wMonth + 2)
            {
                for(int i=0; i<sgESDPMAlarmCheckItem->RowCount; i++)
                {
                    AnsiString s1 = sgESDPMAlarmCheckItem->Cells[1][i];
                    AnsiString s2 = sg_ESDPMAlarmToCSV->Cells[1][iy];
                    if(s1 == s2)
                    {
                        sg_ESDPMAlarmToCSV->Cells[wMonth + 2][iy] = sgESDPMAlarmCheckItem->Cells[2][i];
                    }
                }
                if(iy == (iTotalItemCount-2))
                {
                    sg_ESDPMAlarmToCSV->Cells[wMonth + 2][iy] = ed_ESDExecutiveOwner->Text;
                }
                else if(iy == (iTotalItemCount-1))
                {
                    sg_ESDPMAlarmToCSV->Cells[wMonth + 2][iy] = ed_ESDConformOwner->Text;
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void TfPMAlarmInterFace::SaveESDPMAlarmToCSV()
{
    TStringList *slESDPMAlarm;
    AnsiString sTemp = "";
    int wNowYear=GetYear(Now());
    slESDPMAlarm = new TStringList;
    slESDPMAlarm->Clear();
    AnsiString sFullFileName=GetFullFileName(IntToStr(wNowYear),"ESDPMAlarm.csv");

    int iTotalItemCount = 0;
    iTotalItemCount = PMAlarm_SYS.PMItem_ESD.slItemList->Count
                    + 1 + 2 + 2;
    for(int iy=0;iy<iTotalItemCount*4;iy++)
    {
        sTemp = "";
        for(int ix=0;ix<16;ix++)
        {
            sTemp = sTemp + sg_ESDPMAlarmToCSV->Cells[ix][iy];

            if(ix != 15)
            {
                sTemp = sTemp + ",";
            }
        }
        slESDPMAlarm->Add(sTemp);
    }
    slESDPMAlarm->SaveToFile(sFullFileName);
    slESDPMAlarm->Clear();  //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete slESDPMAlarm;
}
//---------------------------------------------------------------------------
void TfPMAlarmInterFace::LoadESDPMAlarmFromCSV()
{
    TStringList *slESDPMAlarm;
    AnsiString sTemp = "";
    int wNowYear=GetYear(Now());
    slESDPMAlarm = new TStringList;
    slESDPMAlarm->Clear();
    AnsiString sFullFileName=GetFullFileName(IntToStr(wNowYear),"ESDPMAlarm.csv");
    bool bFileExists = false;
    if(FileExists(sFullFileName) == true)
    {
        slESDPMAlarm->LoadFromFile(sFullFileName);
        bFileExists = true;
    }

    int iTotalItemCount = PMAlarm_SYS.PMItem_ESD.slItemList->Count + 1 + 2;

    InitialGrid(sg_ESDPMAlarmToCSV,iTotalItemCount);
    TDateTime dtDate = dtpPMAlarmNextPMDate->Date;
    int wMonth=GetMonth(dtDate);
    int iStartCount = 1;
    int iEndCount = PMAlarm_SYS.PMItem_ESD.slItemList->Count+1;
    for(int iy=iStartCount;iy<iEndCount;iy++)
    {
        sg_ESDPMAlarmToCSV->Cells[0][iy] = IntToStr(iy);
        sTemp = PMAlarm_SYS.PMItem_ESD.slItemList->Strings[iy-iStartCount];
        sg_ESDPMAlarmToCSV->Cells[1][iy] = sTemp;

        sg_ESDPMAlarmToCSV->Cells[2][iy] = "Month";
    }

    sg_ESDPMAlarmToCSV->Cells[1][iEndCount] = ed_ExecutiveItem->Text;
    sg_ESDPMAlarmToCSV->Cells[1][iEndCount + 1] = ed_ConformItem->Text;
    sg_ESDPMAlarmToCSV->Cells[0][iEndCount + 2] = "Doc Version";
    sg_ESDPMAlarmToCSV->Cells[1][iEndCount + 2] = ed_TemperaturePMDocVer->Text;

    if(bFileExists == true)
    {
        for(int iy=0;iy<iTotalItemCount;iy++)
        {
            sTemp = slESDPMAlarm->Strings[iy];

            for(int ix=0;ix<16;ix++)
            {
                sg_ESDPMAlarmToCSV->Cells[ix][iy] = GetCsvData(ix+1,sTemp); //取得CSV格式中的第幾筆資料;

                if((ix - 3) == wMonth)
                {
                    for(int i=0; i<sgESDPMAlarmCheckItem->RowCount; i++)
                    {
                        AnsiString s1 = sg_ESDPMAlarmToCSV->Cells[1][iy];
                        AnsiString s2 = sgESDPMAlarmCheckItem->Cells[1][i];
                        //AnsiString s3 = GetCsvData(ix,sTemp);
                        if(s1 == s2)
                        {
                            sgESDPMAlarmCheckItem->Cells[2][i] = GetCsvData(ix,sTemp); //取得CSV格式中的第幾筆資料;
                             break;
                        }
                    }
                    AnsiString s3 = GetCsvData(ix,sTemp);
                    if(s3.Pos(ed_ExecutiveOwner->Text) == 1)
                    {
                        ed_ESDExecutiveOwner->Text = GetCsvData(ix,sTemp);
                    }
                    if(s3.Pos(ed_ConformOwner->Text) == 1)
                    {
                        ed_ESDConformOwner->Text = GetCsvData(ix,sTemp);
                    }
                }
            }
        }
    }
    else
    {
    }
    slESDPMAlarm->Clear();  //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete slESDPMAlarm;
}
//---------------------------------------------------------------------------
void TfPMAlarmInterFace::UpdateToIonFanPMAlarmToCSV()
{
    AnsiString sTemp = "";
    int iTotalItemCount = 0;
    iTotalItemCount = PMAlarm_SYS.PMItem_IonFan.slItemList->Count + 1 + 2;

    TDateTime dtDate = dtpPMAlarmNextPMDate->Date;
    int wMonth=GetMonth(dtDate);
    for(int iy=0;iy<iTotalItemCount;iy++)
    {
        sTemp = "";
        for(int ix=0;ix<16;ix++)    // 2015.11.06 , Joye , PM Alarm Can't save
        {
            if(ix == wMonth + 2)
            {
                for(int i=0; i<sgIonFanPMAlarmCheckItem->RowCount; i++)
                {
                    AnsiString s1 = sgIonFanPMAlarmCheckItem->Cells[1][i];
                    AnsiString s2 = sg_IonFanPMAlarmToCSV->Cells[1][iy];
                    if(s1 == s2)
                    {
                        sg_IonFanPMAlarmToCSV->Cells[wMonth + 2][iy] = sgIonFanPMAlarmCheckItem->Cells[2][i];
                    }
                }
                if(iy == (iTotalItemCount-2))
                {
                    sg_IonFanPMAlarmToCSV->Cells[wMonth + 2][iy] = ed_IonFanExecutiveOwner->Text;
                }
                else if(iy == (iTotalItemCount-1))
                {
                    sg_IonFanPMAlarmToCSV->Cells[wMonth + 2][iy] = ed_IonFanConformOwner->Text;
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void TfPMAlarmInterFace::SaveIonFanPMAlarmToCSV()
{
    TStringList *slIonFanPMAlarm;
    AnsiString sTemp = "";
    int wNowYear=GetYear(Now());
    slIonFanPMAlarm = new TStringList;
    slIonFanPMAlarm->Clear();
    AnsiString sFullFileName=GetFullFileName(IntToStr(wNowYear),"IonFanPMAlarm.csv");
    int iTotalItemCount = 0;
    iTotalItemCount = PMAlarm_SYS.PMItem_IonFan.slItemList->Count + 1 + 2 + 2;

    for(int iy=0;iy<iTotalItemCount*4;iy++)
    {
        sTemp = "";
        for(int ix=0;ix<16;ix++)
        {
            sTemp = sTemp + sg_IonFanPMAlarmToCSV->Cells[ix][iy];

            if(ix != 15)
            {
                sTemp = sTemp + ",";
            }
        }
        slIonFanPMAlarm->Add(sTemp);
    }
    slIonFanPMAlarm->SaveToFile(sFullFileName);
    slIonFanPMAlarm->Clear();   //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete slIonFanPMAlarm;
}
//---------------------------------------------------------------------------
void TfPMAlarmInterFace::LoadIonFanPMAlarmFromCSV()
{
    TStringList *slIonFanPMAlarm;
    AnsiString sTemp = "";
    int wNowYear=GetYear(Now());
    slIonFanPMAlarm = new TStringList;
    slIonFanPMAlarm->Clear();
    AnsiString sFullFileName=GetFullFileName(IntToStr(wNowYear),"IonFanPMAlarm.csv");
    bool bFileExists = false;
    if(FileExists(sFullFileName) == true)
    {
        slIonFanPMAlarm->LoadFromFile(sFullFileName);
        bFileExists = true;
    }

    int iTotalItemCount = PMAlarm_SYS.PMItem_IonFan.slItemList->Count + 1 + 2;

    InitialGrid(sg_IonFanPMAlarmToCSV,iTotalItemCount);
    TDateTime dtDate = dtpPMAlarmNextPMDate->Date;
    int wMonth=GetMonth(dtDate);
    int iStartCount = 1;
    int iEndCount = PMAlarm_SYS.PMItem_IonFan.slItemList->Count+1;
    for(int iy=iStartCount;iy<iEndCount;iy++)
    {
        sg_IonFanPMAlarmToCSV->Cells[0][iy] = IntToStr(iy);
        sTemp = PMAlarm_SYS.PMItem_IonFan.slItemList->Strings[iy-iStartCount];
        sg_IonFanPMAlarmToCSV->Cells[1][iy] = sTemp;

        sg_IonFanPMAlarmToCSV->Cells[2][iy] = "Month";
    }

    sg_IonFanPMAlarmToCSV->Cells[1][iEndCount] = ed_ExecutiveItem->Text;
    sg_IonFanPMAlarmToCSV->Cells[1][iEndCount + 1] = ed_ConformItem->Text;
    sg_IonFanPMAlarmToCSV->Cells[0][iEndCount + 2] = "Doc Version";
    sg_IonFanPMAlarmToCSV->Cells[1][iEndCount + 2] = ed_IonFanPMDocVer->Text;

    if(bFileExists == true)
    {
        for(int iy=0;iy<iTotalItemCount;iy++)
        {
            sTemp = slIonFanPMAlarm->Strings[iy];

            for(int ix=3;ix<16;ix++)   //Sam 20190927 : 修正更邊離子風扇數量資料錯亂問題。
            {
                sg_IonFanPMAlarmToCSV->Cells[ix][iy] = GetCsvData(ix+1,sTemp); //取得CSV格式中的第幾筆資料;
                if((ix - 3) == wMonth)
                {
                    for(int i=0; i<sgIonFanPMAlarmCheckItem->RowCount; i++)
                    {
                        AnsiString s1 = sg_IonFanPMAlarmToCSV->Cells[1][iy];
                        AnsiString s2 = sgIonFanPMAlarmCheckItem->Cells[1][i];
                        if(s1 == s2)
                        {
                            sgIonFanPMAlarmCheckItem->Cells[2][i] = GetCsvData(ix,sTemp); //取得CSV格式中的第幾筆資料;
                             break;
                        }
                    }

                    AnsiString s3 = GetCsvData(ix,sTemp);
                    if(s3.Pos(ed_ExecutiveOwner->Text) == 1)
                    {
                        ed_IonFanExecutiveOwner->Text = GetCsvData(ix,sTemp);
                    }
                    if(s3.Pos(ed_ConformOwner->Text) == 1)
                    {
                        ed_IonFanConformOwner->Text = GetCsvData(ix,sTemp);
                    }
                }
            }
        }
    }
    else
    {
    }
    slIonFanPMAlarm->Clear();   //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete slIonFanPMAlarm;
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sgESDPMAlarmCheckItemKeyUp(
      TObject *Sender, WORD &Key, TShiftState Shift)
{
    UpdateToESDPMAlarmToCSV();
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sgIonFanPMAlarmCheckItemKeyUp(
      TObject *Sender, WORD &Key, TShiftState Shift)
{
    UpdateToIonFanPMAlarmToCSV();
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sgTemperaturePMAlarmCheckItemKeyUp(
      TObject *Sender, WORD &Key, TShiftState Shift)
{
    UpdateToTemperaturePMAlarmToCSV();
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sgGerneralPMAlarmCheckItemKeyUp(
      TObject *Sender, WORD &Key, TShiftState Shift)
{
    UpdateToGerneralPMAlarmToCSV();
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sb_SaveSettingClick(TObject *Sender)
{
    SaveSetting();
    sbPMAlarm_SaveClick(this);
    PMAlarm_SYS.SavePMAlarm();
}
//---------------------------------------------------------------------------
void TfPMAlarmInterFace::SaveSetting()
{
    PMAlarm_SYS.SetExecutiveItem(ed_ExecutiveItem->Text);
    PMAlarm_SYS.SetExecutiveOwner(ed_ExecutiveOwner->Text);
    PMAlarm_SYS.SetConformItem(ed_ConformItem->Text);
    PMAlarm_SYS.SetConformOwner(ed_ConformOwner->Text);
    PMAlarm_SYS.SetGerneralPMDocVer(ed_GerneralPMDocVer->Text);
    PMAlarm_SYS.SetTemperaturePMDocVer(ed_TemperaturePMDocVer->Text);
    PMAlarm_SYS.SetESDPMDocVer(ed_ESDPMDocVer->Text);
    PMAlarm_SYS.SetIonFanPMDocVer(ed_IonFanPMDocVer->Text);
    PMAlarm_SYS.SetMachineNo(ed_MachineNo->Text);
    PMAlarm_SYS.SetSaveFilePath(ed_SaveFilePath->Text);
    PMAlarm_SYS.SetStartMonth(cbPMStartMonth->Text);
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::Panel28DblClick(TObject *Sender)
{
    ts_Others->TabVisible = !ts_Others->TabVisible;
}
//---------------------------------------------------------------------------
void TfPMAlarmInterFace::SetTemperatureValue(
        AnsiString sTmpBase,       double fLowBase,    double fHighBase,
        AnsiString sPLATE1,        double fPLATE1_L,   double fPLATE1_H,
        AnsiString sPLATE2,        double fPLATE2_L,   double fPLATE2_H,
        AnsiString sSHUTTLE1,      double fSHUTTLE1_L, double fSHUTTLE1_H,
        AnsiString sSHUTTLE2,      double fSHUTTLE2_L, double fSHUTTLE2_H,
        AnsiString sSocket,        double fSocket_L,   double fSocket_H,
        AnsiString sChamber,       double fChamber_L,  double fChamber_H,
        AnsiString sAa1,           double fAa1_L,      double fAa1_H,
        AnsiString sAb1,           double fAb1_L,      double fAb1_H,
        AnsiString sAc1,           double fAc1_L,      double fAc1_H,
        AnsiString sAd1,           double fAd1_L,      double fAd1_H,
        AnsiString sBa1,           double fBa1_L,      double fBa1_H,
        AnsiString sBb1,           double fBb1_L,      double fBb1_H,
        AnsiString sBc1,           double fBc1_L,      double fBc1_H,
        AnsiString sBd1,           double fBd1_L,      double fBd1_H,
        AnsiString sAa2,           double fAa2_L,      double fAa2_H,
        AnsiString sAb2,           double fAb2_L,      double fAb2_H,
        AnsiString sAc2,           double fAc2_L,      double fAc2_H,
        AnsiString sAd2,           double fAd2_L,      double fAd2_H,
        AnsiString sBa2,           double fBa2_L,      double fBa2_H,
        AnsiString sBb2,           double fBb2_L,      double fBb2_H,
        AnsiString sBc2,           double fBc2_L,      double fBc2_H,
        AnsiString sBd2,           double fBd2_L,      double fBd2_H)
{
    PMAlarm_SYS.Temprature_Value.sTempBase = sTmpBase;
    PMAlarm_SYS.Temprature_Value.sTempOffset[ 0] = sPLATE1;
    PMAlarm_SYS.Temprature_Value.sTempOffset[ 1] = sPLATE2;
    PMAlarm_SYS.Temprature_Value.sTempOffset[ 2] = sSHUTTLE1;
    PMAlarm_SYS.Temprature_Value.sTempOffset[ 3] = sSHUTTLE2;
    PMAlarm_SYS.Temprature_Value.sTempOffset[ 4] = sSocket;
    PMAlarm_SYS.Temprature_Value.sTempOffset[ 5] = sChamber;
    PMAlarm_SYS.Temprature_Value.sTempOffset[ 6] = sAa1;
    PMAlarm_SYS.Temprature_Value.sTempOffset[ 7] = sAb1;
    PMAlarm_SYS.Temprature_Value.sTempOffset[ 8] = sAc1;
    PMAlarm_SYS.Temprature_Value.sTempOffset[ 9] = sAd1;
    PMAlarm_SYS.Temprature_Value.sTempOffset[10] = sBa1;
    PMAlarm_SYS.Temprature_Value.sTempOffset[11] = sBb1;
    PMAlarm_SYS.Temprature_Value.sTempOffset[12] = sBc1;
    PMAlarm_SYS.Temprature_Value.sTempOffset[13] = sBd1;
    PMAlarm_SYS.Temprature_Value.sTempOffset[14] = sAa2;
    PMAlarm_SYS.Temprature_Value.sTempOffset[15] = sAb2;
    PMAlarm_SYS.Temprature_Value.sTempOffset[16] = sAc2;
    PMAlarm_SYS.Temprature_Value.sTempOffset[17] = sAd2;
    PMAlarm_SYS.Temprature_Value.sTempOffset[18] = sBa2;
    PMAlarm_SYS.Temprature_Value.sTempOffset[19] = sBb2;
    PMAlarm_SYS.Temprature_Value.sTempOffset[20] = sBc2;
    PMAlarm_SYS.Temprature_Value.sTempOffset[21] = sBd2;


    PMAlarm_SYS.Temprature_Value.fTempBase[0] = fLowBase;
    PMAlarm_SYS.Temprature_Value.fTempOffsetLow[ 0] = fPLATE1_L;
    PMAlarm_SYS.Temprature_Value.fTempOffsetLow[ 1] = fPLATE2_L;
    PMAlarm_SYS.Temprature_Value.fTempOffsetLow[ 2] = fSHUTTLE1_L;
    PMAlarm_SYS.Temprature_Value.fTempOffsetLow[ 3] = fSHUTTLE2_L;
    PMAlarm_SYS.Temprature_Value.fTempOffsetLow[ 4] = fSocket_L;
    PMAlarm_SYS.Temprature_Value.fTempOffsetLow[ 5] = fChamber_L;
    PMAlarm_SYS.Temprature_Value.fTempOffsetLow[ 6] = fAa1_L;
    PMAlarm_SYS.Temprature_Value.fTempOffsetLow[ 7] = fAb1_L;
    PMAlarm_SYS.Temprature_Value.fTempOffsetLow[ 8] = fAc1_L;
    PMAlarm_SYS.Temprature_Value.fTempOffsetLow[ 9] = fAd1_L;
    PMAlarm_SYS.Temprature_Value.fTempOffsetLow[10] = fBa1_L;
    PMAlarm_SYS.Temprature_Value.fTempOffsetLow[11] = fBb1_L;
    PMAlarm_SYS.Temprature_Value.fTempOffsetLow[12] = fBc1_L;
    PMAlarm_SYS.Temprature_Value.fTempOffsetLow[13] = fBd1_L;
    PMAlarm_SYS.Temprature_Value.fTempOffsetLow[14] = fAa2_L;
    PMAlarm_SYS.Temprature_Value.fTempOffsetLow[15] = fAb2_L;
    PMAlarm_SYS.Temprature_Value.fTempOffsetLow[16] = fAc2_L;
    PMAlarm_SYS.Temprature_Value.fTempOffsetLow[17] = fAd2_L;
    PMAlarm_SYS.Temprature_Value.fTempOffsetLow[18] = fBa2_L;
    PMAlarm_SYS.Temprature_Value.fTempOffsetLow[19] = fBb2_L;
    PMAlarm_SYS.Temprature_Value.fTempOffsetLow[20] = fBc2_L;
    PMAlarm_SYS.Temprature_Value.fTempOffsetLow[21] = fBd2_L;

    PMAlarm_SYS.Temprature_Value.fTempBase[1] = fHighBase;
    PMAlarm_SYS.Temprature_Value.fTempOffsetHigt[ 0] = fPLATE1_H;
    PMAlarm_SYS.Temprature_Value.fTempOffsetHigt[ 1] = fPLATE2_H;
    PMAlarm_SYS.Temprature_Value.fTempOffsetHigt[ 2] = fSHUTTLE1_H;
    PMAlarm_SYS.Temprature_Value.fTempOffsetHigt[ 3] = fSHUTTLE2_H;
    PMAlarm_SYS.Temprature_Value.fTempOffsetHigt[ 4] = fSocket_L;
    PMAlarm_SYS.Temprature_Value.fTempOffsetHigt[ 5] = fChamber_L;
    PMAlarm_SYS.Temprature_Value.fTempOffsetHigt[ 6] = fAa1_H;
    PMAlarm_SYS.Temprature_Value.fTempOffsetHigt[ 7] = fAb1_H;
    PMAlarm_SYS.Temprature_Value.fTempOffsetHigt[ 8] = fAc1_H;
    PMAlarm_SYS.Temprature_Value.fTempOffsetHigt[ 9] = fAd1_H;
    PMAlarm_SYS.Temprature_Value.fTempOffsetHigt[10] = fBa1_H;
    PMAlarm_SYS.Temprature_Value.fTempOffsetHigt[11] = fBb1_H;
    PMAlarm_SYS.Temprature_Value.fTempOffsetHigt[12] = fBc1_H;
    PMAlarm_SYS.Temprature_Value.fTempOffsetHigt[13] = fBd1_H;
    PMAlarm_SYS.Temprature_Value.fTempOffsetHigt[14] = fAa2_H;
    PMAlarm_SYS.Temprature_Value.fTempOffsetHigt[15] = fAb2_H;
    PMAlarm_SYS.Temprature_Value.fTempOffsetHigt[16] = fAc2_H;
    PMAlarm_SYS.Temprature_Value.fTempOffsetHigt[17] = fAd2_H;
    PMAlarm_SYS.Temprature_Value.fTempOffsetHigt[18] = fBa2_H;
    PMAlarm_SYS.Temprature_Value.fTempOffsetHigt[19] = fBb2_H;
    PMAlarm_SYS.Temprature_Value.fTempOffsetHigt[20] = fBc2_H;
    PMAlarm_SYS.Temprature_Value.fTempOffsetHigt[21] = fBd2_H;
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sb_LoadTempClick(TObject *Sender)
{
    TemperaturePMAlarmLoadFromSetup();
}
//---------------------------------------------------------------------------
void TfPMAlarmInterFace::TemperaturePMAlarmLoadFromSetup()
{
    int iCount_TemperaturePMList = PMAlarm_SYS.PMItem_ListTemperature.GetCount();

    if( iCount_TemperaturePMList == 0 )
    {
        sgTemperaturePMAlarmCheckItem->RowCount = 2;
        sgTemperaturePMAlarmCheckItem->Cells[0][1] = AnsiString(1);
    }
    else
    {
        sgTemperaturePMAlarmCheckItem->RowCount = 1 + iCount_TemperaturePMList;
    }
    for( int iy = 1 ; iy <= iCount_TemperaturePMList ; iy++ )
    {
        for(int it=0;it<22;it++)
        {
            if(sgTemperaturePMAlarmCheckItem->Cells[1][iy] == PMAlarm_SYS.Temprature_Value.sTempOffset[it])
            {
                double fBas = 0;
                double fOff = 0;

                fBas = PMAlarm_SYS.Temprature_Value.fTempBase[0];
                fOff = PMAlarm_SYS.Temprature_Value.fTempOffsetLow[it];
                sgTemperaturePMAlarmCheckItem->Cells[2][iy] = FloatToStr(fBas);
                sgTemperaturePMAlarmCheckItem->Cells[4][iy] = FloatToStr(fOff);
                sgTemperaturePMAlarmCheckItem->Cells[3][iy] = FloatToStr(fBas - fOff);

                fBas = PMAlarm_SYS.Temprature_Value.fTempBase[1];
                fOff = PMAlarm_SYS.Temprature_Value.fTempOffsetHigt[it];
                sgTemperaturePMAlarmCheckItem->Cells[5][iy] = FloatToStr(fBas);
                sgTemperaturePMAlarmCheckItem->Cells[7][iy] = FloatToStr(fOff);
                sgTemperaturePMAlarmCheckItem->Cells[6][iy] = FloatToStr(fBas - fOff);
                break;
            }
        }
    }
    UpdateToTemperaturePMAlarmToCSV();
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::pn_SetPMDateCloseClick(TObject *Sender)
{
    pn_SetPMDate->Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sb_SetPmDateClick(TObject *Sender)
{
    pn_SetPMDate->Visible = !pn_SetPMDate->Visible;
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::mc_SetPMDateClick(TObject *Sender)
{
    if(mc_SetPMDate->Date > (PMAlarm_SYS.GetNextPMDate() + PMAlarm_SYS.GetPMAlarmDays()))
    {
        mc_SetPMDate->Date = PMAlarm_SYS.GetNextPMDate() + PMAlarm_SYS.GetPMAlarmDays();
    }
    if(mc_SetPMDate->Date < (PMAlarm_SYS.GetNextPMDate() - PMAlarm_SYS.GetPMAlarmDays()))
    {
        mc_SetPMDate->Date = PMAlarm_SYS.GetNextPMDate() - PMAlarm_SYS.GetPMAlarmDays();
    }
    DateSeparator='/';  // Sam 20210225 : 增加保護
    ed_PMDate->Text = mc_SetPMDate->Date.FormatString("YYYY/MM/DD");
}
//---------------------------------------------------------------------------
AnsiString TfPMAlarmInterFace::GetWhenAlarmShowPMDate()
{
    bool bAlarm = PMAlarm_SYS.IsAlarmDate();
    bool bStop = PMAlarm_SYS.IsStopDate();

    TDateTime dtPMDate = PMAlarm_SYS.GetNextPMDate();

    if(bAlarm == true || bStop == true)
    {
        DateSeparator='/';  // Sam 20210225 : 增加保護
        return dtPMDate.FormatString("yyyy/mm/dd");
    }

    return "";
}
//---------------------------------------------------------------------------
void TfPMAlarmInterFace::SetVisibleLevel(bool bVisiblePMItemSetup,bool bVisibleSetup,bool bVisibleExecutive)
{
    ts_PMItemSetup->TabVisible = false;
    ts_PMItemCheck->TabVisible = false;
    ts_PMSetup->TabVisible = false;
    ts_Others->TabVisible = false;

    if(bVisibleExecutive == true)
    {
        ts_PMItemCheck->TabVisible = true;
    }

    if(bVisibleSetup == true)
    {
        ts_PMSetup->TabVisible = true;
    }

    if(bVisiblePMItemSetup == true)
    {
        ts_PMItemSetup->TabVisible = true;
        ts_Others->TabVisible = true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::sbPMAlarmCheck_ShowDataClick(
      TObject *Sender)
{
    fPMAlarm_ShowData->sDataFilePath = PMAlarm_SYS.GetSaveFilePath(); //20150707
    fPMAlarm_ShowData->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::btnDownloadClick(TObject *Sender) //JimmyChiu 20230315 : PM Alarm Update From Server by FTP
{
    int ret=ShowMyMessageBox_YES_NO("Sure Download From Server?", "確定要從伺服下載?");
    if(ret==1)
    {
       if(DownloadFromServer()==true)
        {
            PMAlarm_SYS.SavePMAlarmForGreatekServerUpdate();
            PMAlarm_SYS.LoadPMAlarm();      // 2015.03.26 , Joye , PM Alarm
            UpdatePMAlarmToScreen();
            ShowMyMessage("Download completed.");
        }
        else
        {
            ShowMyMessage("Please Check Server path : " + IniConfig.asA55PMAlarmUpdateFromServerbyFTP);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::btnUploadClick(TObject *Sender)      //JimmyChiu 20230315 : PM Alarm Update From Server by FTP
{
    int ret=ShowMyMessageBox_YES_NO("Sure Upload To Server?", "確定要上傳到伺服?");
    if(ret==1)
    {
        if(Upload2Server()==true)
        {
            PMAlarm_SYS.SavePMAlarm();
            ShowMyMessage("Update completed.");
        }
        else
        {
            ShowMyMessage("Please Check Server path : " + IniConfig.asA55PMAlarmUpdateFromServerbyFTP);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfPMAlarmInterFace::edtPMAlarmAlarmDaysMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 365, 1);
}
//---------------------------------------------------------------------------
bool TfPMAlarmInterFace::Upload2Server()      //JimmyChiu 20230315 : PM Alarm Update From Server by FTP
{
    bool bReturn=false;
    AnsiString sTempSource="D:\\HT9045\\PMAlarm\\";
    AnsiString sTempTarget=IniConfig.asA55PMAlarmUpdateFromServerbyFTP;
    AnsiString asUserID="HONPREC";
    AnsiString asPassword="27025312";
    AnsiString asHost="127.0.0.1";
    #ifndef SOFT_SIMULTE
    asUserID=IniConfig.asN14_3_OEEFTPUserName;
    asPassword=IniConfig.asN14_3_OEEFTPPassword;
    asHost=IniConfig.asN14_3_OEEFTPHost;
    #endif
    TfFTP fFTP(asUserID,asPassword,asHost);
    if(fFTP.Connect())
    {
        AnsiString asError=AnsiString("");
        fFTP.CheckLocalFilePath(sTempSource);
        //上傳路徑內所有檔案
        AnsiString sFindFile=sTempSource + "*.*";
        TSearchRec srFile;
        if(FindFirst(sFindFile, 0, srFile)==0)
        {
            do
            {
                AnsiString sUploadFile=srFile.Name;
                if(FileExists(sTempSource+sUploadFile))                     //Sam 20180802 (wei) : 增加保護
                    fFTP.Upload(sTempSource,sTempTarget,sUploadFile,asError);
                else
                    ShowMyMessage("Can not find : "+sTempSource+sUploadFile);
            }
            while(FindNext(srFile)==0);
        }
        FindClose(srFile);
        bReturn=true;
    }
    else
    {
        MyDBIProcess("Exception", "TfProductionInfo::FTP_OEEFile_Upload");
        ShowMyMessage("FTP_OEEFile_Upload Connect Failed");
        bReturn=false;
    }
    return bReturn;
}
//---------------------------------------------------------------------------
bool TfPMAlarmInterFace::DownloadFromServer()  //JimmyChiu 20230315 : PM Alarm Update From Server by FTP
{
    bool bReturn=false;
    AnsiString sTempULFuleName="";
    AnsiString sTempSource="D:\\HT9045\\PMAlarm\\";
    AnsiString sTempTarget=IniConfig.asA55PMAlarmUpdateFromServerbyFTP;
    AnsiString asUserID="HONPREC";
    AnsiString asPassword="27025312";
    AnsiString asHost="127.0.0.1";
    #ifndef SOFT_SIMULTE
    asUserID=IniConfig.asN14_3_OEEFTPUserName;
    asPassword=IniConfig.asN14_3_OEEFTPPassword;
    asHost=IniConfig.asN14_3_OEEFTPHost;
    #endif
    TfFTP fFTP(asUserID,asPassword,asHost);
    if(fFTP.Connect())
    {
        AnsiString asError=AnsiString("");
        fFTP.CheckLocalFilePath(sTempSource);
        if(fFTP.DownloadFilterFile(sTempTarget,sTempSource,AnsiString(".ini"),asError))
        {}
        bReturn=true;
    }
    else
    {
        bReturn=false;
    }
    return bReturn;
}
//---------------------------------------------------------------------------
bool TfPMAlarmInterFace::PMAllAlarmCheckOK()                                    //JimmyChiu 20231230 : PM All Alarm Check
{
    bool bBack=false;
    if(bIsGerneralItemCheckOK() == false)
    {
        ShowMyMessage("Preventive Maintenance Item Check Error!!" );
    }
    else if(bIsTemperatureItemCheckOK() == false)
    {
        ShowMyMessage("Temperature Calibration Record Item Error!!" );
    }
    else if(bIsESDItemCheckOK() == false)
    {
        ShowMyMessage("ESD Record Item Check Error!!" );
    }
    else if(bIsIonFanItemCheckOK() == false)
    {
        ShowMyMessage("Ion Fan Measurement Record Item Check Error!!" );
    }
    else
    {
        bBack=true;
    }
    return bBack;
}
//---------------------------------------------------------------------------
void TfPMAlarmInterFace::InitialGrid(TStringGrid *sg,int iTotalCount)           //JimmyChiu 20231230 : Initial PM Alarm Grid
{
    for(int ix=0;ix<16;ix++)
    {
        for(int iy=0;iy<iTotalCount;iy++)
        {
            sg->Cells[ix][iy]="";
        }
    }
    //寫出標頭
    sg->Cells[0][0] = "Item Number";
    sg->Cells[1][0] = "Maintenance Item";
    sg->Cells[2][0] = "Maintenance Cycle";
    for(int ix=3;ix<15;ix++)
    {
        sg->Cells[ix][0] = IntToStr(ix-2) + "Month";
    }
}
//---------------------------------------------------------------------------
AnsiString TfPMAlarmInterFace::GetFilePath(AnsiString sFilePath)                //JimmyChiu 20231230 : PM Alarm Get File Path
{
    if(sFilePath=="")
    {
        sFilePath=PMAlarm_SYS.GetSaveFilePath();
    }
    if(sFilePath.SubString(sFilePath.Length(),1) != "\\")
    {
        sFilePath=sFilePath+"\\";
    }
    MyForceDirectories(sFilePath);  //檢查是否有目錄
    return sFilePath;
}
//---------------------------------------------------------------------------
AnsiString TfPMAlarmInterFace::GetFileName(AnsiString sNowYear,AnsiString sFileName)//JimmyChiu 20231230 : PM Alarm Get File Name
{
    return ed_MachineNo->Text+"_"+sNowYear+"_"+sFileName;
}
//---------------------------------------------------------------------------
AnsiString TfPMAlarmInterFace::GetFullFileName(AnsiString sNowYear,AnsiString sFileName)//JimmyChiu 20231230 : PM Alarm Get Full File Name
{
    AnsiString sFilePath="";
    return GetFilePath(sFilePath)+GetFileName(sNowYear,sFileName);
}
//---------------------------------------------------------------------------
int TfPMAlarmInterFace::GetYear(TDateTime tdt)                                  //JimmyChiu 20231230 : PM Alarm Get Year
{
    Word wYear,wMonth,wDate;
    DecodeDate(tdt,wYear,wMonth,wDate);
    return wYear;
}
//---------------------------------------------------------------------------
int TfPMAlarmInterFace::GetMonth(TDateTime tdt)                                 //JimmyChiu 20231230 : PM Alarm Get Month
{
    Word wYear,wMonth,wDate;
    DecodeDate(tdt,wYear,wMonth,wDate);
    return wMonth;
}
//---------------------------------------------------------------------------

