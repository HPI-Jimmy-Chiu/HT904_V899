#include "MachineDefine.h"
#pragma hdrstop

#include "cTestCategory.h"
#include "cmydef.h"
#include "cprod.h"
#include "common.h"
//#include "MachineType.h"
#include "MyKitSuck.h"
#include "cinitial.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfTestCategory *fTestCategory;
AnsiString ArmStr[2]={"Arm1","Arm2"};
//---------------------------------------------------------------------------
__fastcall TfTestCategory::TfTestCategory(TComponent* Owner)
    : TForm(Owner)
{
    TStringGrid *TempPtr[]={sgArm1, sgArm2};
    for(int i=0; i<2; i++)
    {
        MyStringGD[i]=TempPtr[i];
        MyStringGD[i]->Tag=i;
    }
    EdgeWidth=80;
    EdgeHeight=24;
    bCateByArm=false;
    bShow=false;
    PtrDC[0]=GetDC(sgArm1->Handle);
    PtrDC[1]=GetDC(sgArm2->Handle);
    pCanvas=new TCanvas;
}
//---------------------------------------------------------------------------
void __fastcall TfTestCategory::FormShow(TObject *Sender)
{
    SetShowCateMode();
    AdjFormData();
    bShow=true;
}
//---------------------------------------------------------------------------
void __fastcall TfTestCategory::AdjFormData()
{
    fTestCategory->Width=269;
    sgArm1->Height =80;
    sgArm2->Height =80;

    for(int i=0; i<2; i++)
    {
        MyStringGD[i]->RowHeights[0]=EdgeHeight;
        MyStringGD[i]->ColWidths[0] =EdgeWidth;
        if((TestIF.iTestMode<=QualSite1X4   ||
            TestIF.iTestMode==_8Site1X4     ||                                  //ChungHung 20150528 add for 海思 _8Site1x4
            IsNNMode()==NN_1Row) &&
           TestIF.iTestMode!=DualSite2x1)
        {
            MyStringGD[i]->RowCount=2;
            MyStringGD[i]->RowHeights[1]=24;
        }
        else if(TestIF.iTestMode==_32Site4X8M)
        {
            MyStringGD[i]->RowCount=3;
            MyStringGD[i]->RowHeights[1]=24;
            MyStringGD[i]->RowHeights[2]=24;
        }
        else
        {
            MyStringGD[i]->RowCount=3;
            MyStringGD[i]->RowHeights[1]=24;
            MyStringGD[i]->RowHeights[2]=24;
        }

        if(TestIF.iTestMode==SingleSite)
        {
            MyStringGD[i]->ColCount=2;
            MyStringGD[i]->ColWidths[1]=120;
        }
        else if(TestIF.iTestMode==TriSite1X3 ||                                 //Frank 20160329 add for 1x3_4
                TestIF.iTestMode==_6Site2X3N ||                                 //Steven 20220425 : 2X3NN Mode
                TestIF.iTestMode==_6Site2X3)                                    //ChungHung 20140115 add for 2x3_6
        {
            MyStringGD[i]->ColCount=4;
            MyStringGD[i]->ColWidths[1]=40;
            MyStringGD[i]->ColWidths[2]=40;
            MyStringGD[i]->ColWidths[3]=40;
        }
        else if(TestIF.iTestMode==QualSite1X4 ||
                TestIF.iTestMode==_8Site2X4   ||
                TestIF.iTestMode==_8Site1X4   ||                                //ChungHung 20150528 add for 海思 _8Site1x4
                TestIF.iTestMode==_16Site4X4  ||                                //Sam 20190226 : 16Site4X4
                TestIF.iTestMode==_8Site2X4N)                                   //Wei 20231211 : 2X4NN Mode
        {
            MyStringGD[i]->ColCount=5;
            MyStringGD[i]->ColWidths[1]=40;
            MyStringGD[i]->ColWidths[2]=40;
            MyStringGD[i]->ColWidths[3]=40;
            MyStringGD[i]->ColWidths[4]=40;
        }
        else if(TestIF.iTestMode==_10Site2X5)                                   //wei 20190614 10 site
        {
            fTestCategory->Width=309;
            MyStringGD[i]->ColCount=6;
            MyStringGD[i]->ColWidths[1]=40;
            MyStringGD[i]->ColWidths[2]=40;
            MyStringGD[i]->ColWidths[3]=40;
            MyStringGD[i]->ColWidths[4]=40;
            MyStringGD[i]->ColWidths[5]=40;
        }
        else if(TestIF.iTestMode==_12Site2X6)                                   //Eliot 2009_12_24
        {
            fTestCategory->Width=429;
            fTestCategory->Width=349;
            MyStringGD[i]->ColCount=7;
            MyStringGD[i]->ColWidths[1]=40;
            MyStringGD[i]->ColWidths[2]=40;
            MyStringGD[i]->ColWidths[3]=40;
            MyStringGD[i]->ColWidths[4]=40;
            MyStringGD[i]->ColWidths[5]=40;
            MyStringGD[i]->ColWidths[6]=40;
        }
        else if(TestIF.iTestMode==_16Site2X8 ||
                TestIF.iTestMode==_32Site4X8N ||                                //Steven 20140512 : For HT-9047
                TestIF.iTestMode==_32Site4X8M)                                  //Eliot 2009_12_24
        {
            fTestCategory->Width=429;
            MyStringGD[i]->ColCount=9;
            MyStringGD[i]->ColWidths[1]=40;
            MyStringGD[i]->ColWidths[2]=40;
            MyStringGD[i]->ColWidths[3]=40;
            MyStringGD[i]->ColWidths[4]=40;
            MyStringGD[i]->ColWidths[5]=40;
            MyStringGD[i]->ColWidths[6]=40;
            MyStringGD[i]->ColWidths[7]=40;
            MyStringGD[i]->ColWidths[8]=40;
        }
        else
        {
            MyStringGD[i]->ColCount=3;
            MyStringGD[i]->ColWidths[1]=80;
            MyStringGD[i]->ColWidths[2]=80;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTestCategory::sgArm1DrawCell(TObject *Sender,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
    if(InitialOK==false)
        return;

    TStringGrid *Ptr=(TStringGrid *)Sender;
    int iArm32, iRow32, iCol32;
    AnsiString str;
    pCanvas->Handle=PtrDC[Ptr->Tag];
    if(ACol==0)
    {
        if(ARow==0)
        {
            if(bCateByArm==false)
                str.sprintf("Socket %d", Ptr->Tag);
            else
                str=ArmStr[Ptr->Tag];
            pCanvas->Font->Style=TFontStyles();
            MyDrawText(pCanvas, Rect, str.c_str(), clBtnFace);
        }
        else if(IsNNMode()==NN_2Row)
        {
            if(ARow<MAX_SOCKET_ROW+1)
            {
                if(Ptr->Tag==0)                                                 //Arm 1
                    str.sprintf("%c", 'C'+ARow-1);
                else
                    str.sprintf("%c", 'A'+ARow-1);
                pCanvas->Font->Style=TFontStyles()<<fsBold;
                MyDrawText(pCanvas, Rect, str.c_str(), clBtnFace);
            }
        }
        else if(IsNNMode()==NN_1Row)
        {
            if(ARow<MAX_SOCKET_ROW+1)
            {
                if(Ptr->Tag==0)                                                 //Arm 1
                    str.sprintf("%c", 'B'+ARow-1);
                else
                    str.sprintf("%c", 'A'+ARow-1);
                pCanvas->Font->Style=TFontStyles()<<fsBold;
                MyDrawText(pCanvas, Rect, str.c_str(), clBtnFace);
            }
        }
        else
        {
            if(ARow==1 || ARow==2)
            {
                str.sprintf("%c", 'A'+ARow-1);
                pCanvas->Font->Style=TFontStyles()<<fsBold;
                MyDrawText(pCanvas, Rect, str.c_str(), clBtnFace);
            }
        }
    }

    if(ARow==0)
    {
        if(ACol>=1 && ACol<=8)
        {
            str.sprintf("%c", 'a'+ACol-1);
            pCanvas->Brush->Color=clBtnFace;
            pCanvas->Font->Style=TFontStyles()<<fsBold;
            MyDrawText(pCanvas, Rect, str.c_str(), clBtnFace);
        }
    }
    else if(IsNNMode()==NN_2Row)
    {
        if(ACol>=1 && ACol<=8)
        {
            iRow32=ARow-1;
            iCol32=ACol-1;
            if(Ptr->Tag==0)
            {
                iArm32=0;
            }
            else
            {
                iArm32=1;
            }

            if(TestResult[iArm32][iRow32][iCol32]<0)                            //JerryYang 20230721 : 修正bin 0不會顯示的問題
            {
                str="";
                if(IniConfig.bA09_ByArmCloseSite)                               //ChungHung 20130910 alter for SCK can close site by Index
                {
                    if((TestIF.iShuttleMode && TestIF.iShuttle_Sel!=Ptr->Tag) ||
                       (Ptr->Tag==0 && LastSet.bUseTestSocket[0][2+iRow32][iCol32]==false) ||
                       (Ptr->Tag==1 && LastSet.bUseTestSocket[0][0+iRow32][iCol32]==false))
                        str="X";                                                //Steven 20240326 : 修正Test Cate顯示
                }
            }
            else
            {
                if(TestResult[iArm32][iRow32][iCol32]>=iTestBinCount)           //Steven 20190628 : > --> >=
                {
                    switch(IniConfig.iI20ErrorBinAlphabet)
                    {
                        case 0: str="0";                            break;
                        case 1: str=AnsiString(iTestBinCount);      break;
                        case 2: str="E";                            break;
                        case 3: str="Err";                          break;
                        case 4: str="Error";                        break;
                        default: str.sprintf("%d", TestResult[iArm32][iRow32][iCol32]);
                    }
                }
                else
                {
                    str.sprintf("%d", TestResult[iArm32][iRow32][iCol32]);
                }
            }
            MyDrawText(pCanvas, Rect, str.c_str(), ColorPtr[iArm32][iRow32][iCol32]);
        }
    }
    else if(IsNNMode()==NN_1Row)
    {
        if(ACol>=1 && ACol<=4)
        {
            iRow32=0;
            iCol32=ACol-1;
            if(Ptr->Tag==0)
            {
                iRow32=1;
            }
            else
            {
                iRow32=0;
            }

            if(TestResult[0][iRow32][iCol32]<0)                                 //JerryYang 20230721 : 修正bin 0不會顯示的問題
            {
                str="";
                if(IniConfig.bA09_ByArmCloseSite)                               //ChungHung 20130910 alter for SCK can close site by Index
                {
                    if((TestIF.iShuttleMode && TestIF.iShuttle_Sel!=Ptr->Tag) ||
                        LastSet.bUseTestSocket[Ptr->Tag][ARow-1][iCol32]==false)
                        str="X";                                                //Steven 20240326 : 修正Test Cate顯示
                }
            }
            else
            {
                if(TestResult[0][iRow32][iCol32]>=iTestBinCount)                //Steven 20190628 : > --> >=
                {
                    switch(IniConfig.iI20ErrorBinAlphabet)
                    {
                        case 0: str="0";        break;
                        case 1: str=AnsiString(iTestBinCount);       break;
                        case 2: str="E";        break;
                        case 3: str="Err";      break;
                        case 4: str="Error";    break;
                        default: str.sprintf("%d", TestResult[0][iRow32][iCol32]);
                    }
                }
                else
                {
                    str.sprintf("%d", TestResult[0][iRow32][iCol32]);
                }
            }
            MyDrawText(pCanvas, Rect, str.c_str(), ColorPtr[0][iRow32][iCol32]);
        }
    }
    else if(ARow==1 || ARow==2)
    {
        if(bCateByArm==false)
        {
            iArm32=0;
        }
        else
        {
            iArm32=Ptr->Tag;
        }

        if(ACol>=1 && ACol<=8)
        {
            if(TestResult[iArm32][ARow-1][ACol-1]<0)                            //ChungHung 20140611 modify 要加上括弧
            {
                str="";
                if(IniConfig.bA09_ByArmCloseSite)                               //ChungHung 20130910 alter for SCK can close site by Index
                {
                    if((TestIF.iShuttleMode && TestIF.iShuttle_Sel!=Ptr->Tag) ||
                        LastSet.bUseTestSocket[Ptr->Tag][ARow-1][ACol-1]==false)
                        str="X";
                }
            }
            else
            {
                if(TestResult[iArm32][ARow-1][ACol-1]>=iTestBinCount)
                {
                    switch(IniConfig.iI20ErrorBinAlphabet)
                    {
                        case 0: str="0";        break;
                        case 1: str=AnsiString(iTestBinCount);    break;        //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
                        case 2: str="E";        break;
                        case 3: str="Err";      break;
                        case 4: str="Error";    break;
                        default: str.sprintf("%d", TestResult[iArm32][ARow-1][ACol-1]);
                    }

                    if(CosFunction.bBarcodeErrNoTestAndShowH==true &&           //jou 20191007 : Barcode Error No Test & Show "H"
                       TestIF_File.bEnableBarCode==true &&
                       (TestSocket.cDeviceInf[ARow-1][ACol-1]==asBarCodeErrorSend ||
                        TestSocket.cDeviceInf[ARow-1][ACol-1]==""))
                    {
                        str="H";
                    }
                }
                else
                {
                    str.sprintf("%d", TestResult[iArm32][ARow-1][ACol-1]);
                }
            }
            MyDrawText(pCanvas, Rect, str.c_str(), ColorPtr[iArm32][ARow-1][ACol-1]);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTestCategory::SetTestCateCellINT(int Arm, int X, int Y, int Bin, TColor Color)
{
    if(IsNNMode()==NN_2Row)
    {
        if(X>=2)
        {
            X-=2;
            Arm=0;
        }
        else
        {
            Arm=1;
        }
    }
    else if(IsNNMode()==NN_1Row)
    {
        Arm=0;
    }

    if(bCateByArm)
    {
        ColorPtr[Arm][X][Y]=Color;
        TestResult[Arm][X][Y]=Bin;
    }
    else
    {
        ColorPtr[0][X][Y]=Color;
        TestResult[0][X][Y]=Bin;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTestCategory::SetTestCateCellAS(int Arm, int X, int Y, AnsiString Bin, TColor Color)
{
    if(IsNNMode()==NN_2Row)
    {
        if(X>=2)
        {
            X-=2;
            Arm=0;
        }
        else
        {
            Arm=1;
        }
    }
    else if(IsNNMode()==NN_1Row)
    {
        Arm=0;
    }

    if(bCateByArm)
    {
        ColorPtr[Arm][X][Y]=Color;
        TestResult[Arm][X][Y]=atoi(Bin.c_str());
    }
    else
    {
        ColorPtr[0][X][Y]=Color;
        TestResult[0][X][Y]=atoi(Bin.c_str());
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTestCategory::SetTestingCateCell(int Arm, int X, int Y, TColor Color)
{
    if(IsNNMode()==NN_2Row)
    {
        if(X>=2)
        {
            X-=2;
            Arm=0;
        }
        else
        {
            Arm=1;
        }
    }
    else if(IsNNMode()==NN_1Row)
    {
        Arm=0;
    }

    if(bCateByArm)
        ColorPtr[Arm][X][Y]=Color;
    else
        ColorPtr[0][X][Y]=Color;
}
//---------------------------------------------------------------------------
void __fastcall TfTestCategory::ShowTestCategory(int iIndex)
{
    SetShowCateMode();
    if(bUseTwoArm32Site==true)
    {
        sgArm1->Refresh();
        sgArm2->Refresh();
    }
    else
    {
        if(iIndex==0 || bCateByArm==false)
            sgArm1->Refresh();
        else
            sgArm2->Refresh();
    }
}
//---------------------------------------------------------------------------
int __fastcall TfTestCategory::GetTestResult(int Arm, int Row, int Col, TColor *CellColor)
{
    if(IsNNMode()==NN_2Row)
    {
        if(Row>=2)
        {
            Row-=2;
            Arm=0;
        }
        else
        {
            Arm=1;
        }
    }
    else if(IsNNMode()==NN_1Row)
    {
        Arm=0;
    }

    if(bCateByArm)
    {
        *CellColor=ColorPtr[Arm][Row][Col];
        return TestResult[Arm][Row][Col];
    }
    else
    {
        *CellColor=ColorPtr[0][Row][Col];
        return TestResult[0][Row][Col];
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTestCategory::InitCateCell()
{
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<4; j++)
        {
            for(int k=0; k<8; k++)
            {
                ColorPtr[i][j][k]=clWhite;
                TestResult[i][j][k]=-1;
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTestCategory::SetShowCateMode()
{
    bCateByArm=(IniConfig.iShowCateByArm!=0);
    #ifdef DEBUG_WIN7_FULL_HD
        if(bCateByArm)
            fTestCategory->Height=247;
        else
            fTestCategory->Height=143;
    #else
        if(bCateByArm)
            fTestCategory->Height=192;
        else
            fTestCategory->Height=105;
    #endif
}
//---------------------------------------------------------------------------
void __fastcall TfTestCategory::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    bShow=false;
}
//---------------------------------------------------------------------------
void __fastcall TfTestCategory::FormDestroy(TObject *Sender)
{
    try
    {
        for(int i=0; i<2; i++)
            ReleaseDC(0, PtrDC[i]);
        delete pCanvas;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfTestCategory::FormDestroy");
    }
    LogSoftwareOffTime("TfTestCategory, FormDestroy");                          //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
