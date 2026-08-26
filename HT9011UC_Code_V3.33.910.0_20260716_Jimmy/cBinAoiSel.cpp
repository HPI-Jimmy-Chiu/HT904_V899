//---------------------------------------------------------------------------
//#include <vcl.h>
#include "MachineDefine.h"
#pragma hdrstop

#include "cBinAoiSel.h"
#include "MachineType.h"
#include "cmydef.h"
//#include <SysUtils.hpp>
#include "cBinSel.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TfBinAOISel *fBinAOISel;

//---------------------------------------------------------------------------
__fastcall TfBinAOISel::TfBinAOISel(TComponent* Owner)
    : TForm(Owner)
{
    MaxBinSearch=15;
    //Refresh(-1);
}
//---------------------------------------------------------------------------
void TfBinAOISel::Save(int tag)
{
    if(tag!=iTestRunMode)
        return;
    if(tag < 0 || tag >= eBinTypeTotal) //Eastsun 20260515 F023: E17 AI risk fix
        return;
    AnsiString ss = "";
    int ComboListCC=0;                                                          //W8012 signed/unsigned (was: unsigned int)
    for(int i = 0; i < TEST_MAX_BIN ; i++)
    {
        int searchBin = -1;

        if(ComboListCC<(int)ComboList.size())                                   //Eastsun 20260521 Fix type
        {
            if(ComboList[ComboListCC].Bin==i)
            {
                searchBin=i;
                ComboListCC++;
            }
        }

        if(searchBin>=0)
        {
            AnsiString sss = "";
            if(ComboList[ComboListCC-1].Combo->Text=="")
                sss="0";
            else
                sss=ComboList[ComboListCC-1].Combo->Text;

            int iTrayIDAddOne=GetTrayIDAddOne(sss);                             //Ifor 20260715 : sync runtime bin data so AOI fail bin takes effect at next start
            BinSelect[tag].iAOICategData[i]=iTrayIDAddOne;
            if(i==0)
            {
                ss+=IntToStr(iTrayIDAddOne);
            }
            else
            {
                ss+=",";
                ss+=IntToStr(iTrayIDAddOne);
            }
        }
        else
        {
            BinSelect[tag].iAOICategData[i]=0;                                  //Ifor 20260715 : 0 = not use
            if(i==0)
                ss+="0";
            else
                ss+=",0";
        }
    }
    fBinSel->sAOIBinTraySetting[tag]->CommaText=ss;
}
//---------------------------------------------------------------------------
void TfBinAOISel::Refresh(int tag)
{
    if(tag!=iTestRunMode)
        return;
    if(tag < 0 || tag >= eBinTypeTotal) //Eastsun 20260515 F023: E17 AI risk fix
        return;

    AOI_ReadBin=fBinSel->AOI_SplitToInt(tag);

    ComboList.clear();
    RowCount = 0;

    // 遍歷 ScrollBox1 內的元件，刪除所有動態增加的子元件
    while(ScrollBox1->ControlCount > 0)
    {
        TControl* ctrl = ScrollBox1->Controls[0];
        //ctrl->Parent = nullptr;  // 先解除 Parent，避免 VCL 迴圈問題
        delete ctrl;             // 釋放元件
    }

    AddRow("BIN", "select",-1);

    for(int i = 0; i < MaxBinSearch && i < (int)AOI_ReadBin.size(); i++) //Eastsun 20260515 F023: E17 AI risk fix
    {
        if(AOI_ReadBin[i]>0)
        {
            AddRow(IntToStr(i), "",i);
            if(tag>=0)
            {
                //AnsiString ss = GetTrayName(BinSelect[tag].iAOICategData[i]-1);
                //ComboList[ComboList.size()-1]->Text=GetTrayName(AOI_ReadBin[i]-1);
                ComboList[ComboList.size()-1].Combo->Text=GetTrayName(BinSelect[tag].iAOICategData[i]-1);
            }
            else
            {
                //AnsiString ss = GetTrayName(BinSelect[iTestRunMode].iAOICategData[i]-1);
                //ComboList[ComboList.size()-1]->Text=GetTrayName(AOI_ReadBin[i]-1);
                ComboList[ComboList.size()-1].Combo->Text=GetTrayName(BinSelect[iTestRunMode].iAOICategData[i]-1);
            }
        }
    }
}
//---------------------------------------------------------------------------
AnsiString TfBinAOISel::GetTrayName(int trayID)
{
     if(trayID>=0 && trayID<eTrayCount)                                         //Eastsun 20260515 F023: E17 AI risk fix
     {
          return s6TrayName[trayID];
     }
     else
     {
          return "Not Use";
     }
}
//---------------------------------------------------------------------------
int TfBinAOISel::GetTrayIDAddOne(AnsiString Name)
{
    return GetTrayID(Name) + 1;
}
//---------------------------------------------------------------------------
int TfBinAOISel::GetTrayID(AnsiString Name)
{
    if(Name == "Not Use") return -1;
    if(Name == GetTrayName(eFix1))    return eFix1;
    if(Name == GetTrayName(eFix2))    return eFix2;
    if(Name == GetTrayName(eFix3))    return eFix3;
    if(Name == GetTrayName(eAuto1))   return eAuto1;
    if(Name == GetTrayName(eAuto2))   return eAuto2;

    if(Name == GetTrayName(eAuto3))   return eAuto3;

    if(Name.Pos("Magazine") == 1)  // "Mag1" ~ "Mag14"
    {
        int n = StrToIntDef(Name.SubString(9, Name.Length() - 8), -1); // 取得數字部分
        if(n < 1 || n > 14) return -1; //Eastsun 20260515 F023: E17 AI risk fix
        switch(n)
        {
            case 1: return eMag1;
            case 2: return eMag2;
            case 3: return eMag3;
            case 4: return eMag4;
            case 5: return eMag5;
            case 6: return eMag6;
            case 7: return eMag7;
            case 8: return eMag8;
            case 9: return eMag9;
            case 10: return eMag10;
            case 11: return eMag11;
            case 12: return eMag12;
            case 13: return eMag13;
            case 14: return eMag14;
        }
    }

    if(Name == "BulkBox") return eBulkBox;

    // 找不到對應
    return -1;
}
//---------------------------------------------------------------------------
void TfBinAOISel::AddRow(AnsiString labelText, AnsiString comboDefault,int bin)
{
    int rowHeight = 30;
    int marginTop = 10;
    int marginLeft = 10;
    //Panel1->Height=(RowCount+1) * rowHeight + RowCount * 2;
    // Row Panel
    TPanel *rowPanel = new TPanel(this);
    rowPanel->Parent = ScrollBox1;

    rowPanel->Left = marginLeft;
    rowPanel->Top = marginTop + RowCount * rowHeight + RowCount * 2;
    rowPanel->Width = ScrollBox1->Width - marginLeft * 2 - 15;
    rowPanel->Height = rowHeight;

    rowPanel->Color = clWhite;
    rowPanel->BevelOuter = bvNone;
    rowPanel->BorderStyle = bsSingle;

    // Label
    TLabel *lbl = new TLabel(this);
    lbl->Parent = rowPanel;
    lbl->Caption = labelText;
    lbl->Left = 10;
    lbl->Top = 0;
    lbl->AutoSize = false;
    lbl->Width = 120;              // 或你想要的寬度
    lbl->Height = rowPanel->Height;
    lbl->Font->Size = 14;
    lbl->Alignment = taCenter;     // 水平置中
    lbl->Layout = tlCenter;        // 垂直置中

    if(comboDefault == "")
    {
         // ComboBox
        TComboBox *cb = new TComboBox(this);
        cb->Parent = rowPanel;
        cb->Left = 150;
        cb->Top = 3;
        cb->Width = 120;

        cb->Items->Add("Not Use");
        cb->Items->Add(GetTrayName(eFix1));
        cb->Items->Add(GetTrayName(eAuto1));
        cb->Items->Add(GetTrayName(eAuto2));
        if(AUTO3_IS_MAGAZINE==1)
        {
            for(int i = eMag1; i <= eMag14;i++)
            {
                cb->Items->Add(GetTrayName(i));
            }
        }
        else
        {
            cb->Items->Add(GetTrayName(eAuto3));
        }

        cb->ItemIndex = 0; // 選擇預設文字

        TComboItem item;

        item.Combo = cb;
        item.Bin = bin;

        ComboList.push_back(item);
    }
    else
    {
        TLabel *lbl2 = new TLabel(this);
        lbl2->Parent = rowPanel;
        lbl2->Caption = comboDefault;
        lbl2->Left = 150;
        lbl2->Top = 3;
        lbl2->Width = 120;
        lbl2->Font->Size = 12;
        lbl2->Alignment = taCenter;
        lbl2->Layout = tlCenter;
        rowPanel->Color =(TColor)0x00C2B8A6;
    }

    RowCount++;
}
//---------------------------------------------------------------------------
void __fastcall TfBinAOISel::spbExitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfBinAOISel::FormShow(TObject *Sender)
{
    //MyBinPanel[iTestRunMode]->edBinSetting->Text
    //fBinSel->SaveFunctionData(iTestRunMode);
    Refresh(iTestRunMode);
}
//---------------------------------------------------------------------------
