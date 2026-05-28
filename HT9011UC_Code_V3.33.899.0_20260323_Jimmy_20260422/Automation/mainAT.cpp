//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "mainAT.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfMainAT *fMainAT;
int MAX_Index_Col=0;
int MAX_Index_Row=0;
//---------------------------------------------------------------------------
__fastcall TfMainAT::TfMainAT(TComponent* Owner)
    : TForm(Owner)
{
    int i, j;
    AnsiString asCategory[10]={"NoUse", "Auto1", "Auto2", "Auto3", "Fix1", "Fix2", "Fix3", "Fix4", "Fix5", "Fix6"};
    AnsiString asSiteMapping[16]={"CH 1", "CH 2", "CH 3", "CH 4", "CH 5", "CH 6", "CH 7", "CH 8", "CH 9", "CH 10", "CH 11", "CH 12", "CH 13", "CH 14", "CH 15", "CH 16"};
    AnsiString asDutOnOff[2]={"Close", "Open"};

    edTrayForm[0]=Edit1, edTrayForm[1]=Edit2, edTrayForm[2]=Edit3;
    edTrayForm[3]=Edit4, edTrayForm[4]=Edit5, edTrayForm[5]=Edit6;

    edPlateForm[0]=Edit7,  edPlateForm[1]=Edit8,  edPlateForm[2]=Edit9;
    edPlateForm[3]=Edit10, edPlateForm[4]=Edit11, edPlateForm[5]=Edit12;

    for(i=0; i<16; i++)
    {
        lbCategory[i]           =new TLabel(this);
        cbCategory[i]           =new TComboBox(this);
        lbDutOnOff[i]           =new TLabel(this);
        cbDutOnOff[i]           =new TComboBox(this);
        cbSiteMapping[i]        =new TComboBox(this);
        lbSiteMapping[i]        =new TLabel(this);

        lbCategory[i]->Parent   =gbCategory;
        cbCategory[i]->Parent   =gbCategory;
        lbDutOnOff[i]->Parent   =gbDutOn;
        cbDutOnOff[i]->Parent   =gbDutOn;
        lbSiteMapping[i]->Parent=gbMapping;
        cbSiteMapping[i]->Parent=gbMapping;

        lbCategory[i]->Left     =5;
        cbCategory[i]->Left     =60;
        lbDutOnOff[i]->Left     =5;
        cbDutOnOff[i]->Left     =60;
        lbSiteMapping[i]->Left  =5;
        cbSiteMapping[i]->Left  =60;

        lbCategory[i]->Top      =22+25*i;
        cbCategory[i]->Top      =20+25*i;
        lbDutOnOff[i]->Top      =22+25*i;
        cbDutOnOff[i]->Top      =20+25*i;
        lbSiteMapping[i]->Top   =22+25*i;
        cbSiteMapping[i]->Top   =20+25*i;

        lbCategory[i]->Width    =50;
        cbCategory[i]->Width    =70;
        lbDutOnOff[i]->Width    =50;
        cbDutOnOff[i]->Width    =70;
        lbSiteMapping[i]->Width =50;
        cbSiteMapping[i]->Width =70;

        lbCategory[i]->Height   =17;
        cbCategory[i]->Height   =17;
        lbDutOnOff[i]->Height   =17;
        cbDutOnOff[i]->Height   =17;
        lbSiteMapping[i]->Height=17;
        cbSiteMapping[i]->Height=17;

        lbCategory[i]->Caption  =(i==0)?AnsiString("IF Error"):AnsiString("BIN"+AnsiString(i));
        if(i==0)
            for(j=1; j<10; j++)
                cbCategory[i]->Items->Add(asCategory[j]);
        else
            for(j=0; j<10; j++)
                cbCategory[i]->Items->Add(asCategory[j]);

        lbDutOnOff[i]->Caption      ="Dut"+AnsiString(i+1);
        for(j=0; j<2; j++)
            cbDutOnOff[i]->Items->Add(asDutOnOff[j]);

        lbSiteMapping[i]->Caption   ="Dut"+AnsiString(i+1);
        for(j=0; j<16; j++)
            cbSiteMapping[i]->Items->Add(asSiteMapping[j]);

        cbCategory[i]   ->ItemIndex=0;
        cbDutOnOff[i]   ->ItemIndex=1;
        cbSiteMapping[i]->ItemIndex=i;
    }
}
//---------------------------------------------------------------------------
void TfMainAT::ShowOLPState(int iState)
{
    AnsiString Name="";
    Name="Automation ";

    switch(iState)
    {
        case 0:
            lbSystemStatus->Caption=Name+"Off-Line";
            lbSystemStatus->Font->Color=clTeal;
            break;
        case 1:
            lbSystemStatus->Caption=Name+"On-Line";
            lbSystemStatus->Font->Color=clBlue;
            break;
        case 2:
            lbSystemStatus->Caption=Name+"Client Error";
            lbSystemStatus->Font->Color=clMaroon;
            break;
    }
    lbSystemStatus->Alignment=2;
}
//---------------------------------------------------------------------------
int SetCategory(AnsiString *Data) //OK
{
    //AnsiString KeyWord[16]={"I//F Error=","Bin1=","Bin2=","Bin3=","Bin4=","Bin5=","Bin6=","Bin7=","Bin8=","Bin9=","Bin10=","Bin11=","Bin12=","Bin13=","Bin14=","Bin15="};
    //AnsiString Position[10]={"No Use","Auto1", "Auto2", "Auto3", "Fix1", "Fix2", "Fix3", "Fix4", "Fix5", "Fix6"};

    if(CheckSystemStart())
        return 2;

    if(CheckNeedCleanOut())
        return 2;

    int PosIndex[16];

    for(int i=0; i<16; i++)
    {
        PosIndex[i] = atoi(Data[i].c_str());

        if(PosIndex[i]<0 || PosIndex[i]>=10)
            return 1;
    }

    fMainAT->cbCategory[0]->ItemIndex=PosIndex[0];
    for(int i=1; i<16; i++)
    {
        fMainAT->cbCategory[i]->ItemIndex=PosIndex[i];
    }
    return 0;
}
//---------------------------------------------------------------------------
int SetDutOnOff(AnsiString *Data) //OK
{
    if(CheckSystemStart())
        return 2;

    if(CheckNeedCleanOut())
        return 2;

    //根據模式選擇
    //TestIF.iTestMode
 /*   int MaxI=0;
    int MaxJ=0;
    switch(TestIF.iTestMode)
    {
      case 0://1x2
           MaxI=1;
           MaxJ=2;
           break;
      case 1://1x1
           MaxI=1;
           MaxJ=1;
           break;
      case 2://1x4
           MaxI=1;
           MaxJ=4;
           break;
      case 3://2x2
           MaxI=2;
           MaxJ=2;
           break;
      case 5://2x4
           MaxI=2;
           MaxJ=4;
           break;
      case 6://2x8
           MaxI=2;
           MaxJ=8;
           break;
    };

    if(fMain->CanChangeSite()==false)
    {
        return 2;
    }

    //AnsiString KeyWord[16]={ "Dut1=" ,"Dut2=" ,"Dut3=" ,"Dut4=" ,"Dut5=" ,"Dut6=" ,"Dut7=" ,"Dut8=" ,
    //                         "Dut9=" ,"Dut10=" ,"Dut11=" ,"Dut12=" ,"Dut13=" ,"Dut14=" ,"Dut15=" ,"Dut16=" };
    //AnsiString Result[2]={ "Close","Open"};

    int index;
    int value;
    bool bDutOnOff[16]={false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};

    for(int i=0; i<MaxI; i++)
    {
        for(int j=0; j<MaxJ; j++)
        {
            index = i*MaxJ+j;
            if(index<8)
            {
                if(TestIF.iSiteMap[index/4][index%4]==0)
                {
                    bTestSiteUse[index/4][index%4]=false;
                    LastSet.bUseTestSocket[index/4][index%4]=false;
                }
                else
                {
                    bTestSiteUse[index/4][index%4]=!bTestSiteUse[index/4][index%4];
                    LastSet.bUseTestSocket[index/4][index%4]=!LastSet.bUseTestSocket[index/4][index%4];
                }
            }
            else
            {
                if(TestIF.iSiteMap[index/4-2][index%4+4]==0)
                {
                    bTestSiteUse[index/4-2][index%4+4]=false;
                    LastSet.bUseTestSocket[index/4-2][index%4+4]=false;
                }
                else
                {
                    bTestSiteUse[index/4-2][index%4+4]=!bTestSiteUse[index/4-2][index%4+4];
                    LastSet.bUseTestSocket[index/4-2][index%4+4]=!LastSet.bUseTestSocket[index/4-2][index%4+4];
                }
            }

            value =  atoi(Data[index].c_str());
            bDutOnOff[index] = (value==1)? true:false;
        }
    }

    for(int i=0; i<MaxI; i++)
    {
        for(int j=0; j<MaxJ; j++)
        {
          index = i*MaxJ+j;
          LastSet.bUseTestSocket[i][j] = bDutOnOff[index];
          bTestSiteUse[i][j] = bDutOnOff[index];
        }
    }

    fMain->ShowTestHeadComp();      */
    return 0;
}
//---------------------------------------------------------------------------
int SetMapping(AnsiString *Data) //ok
{
    if(CheckSystemStart())
      return 2;

    if(CheckNeedCleanOut())
      return 2;

    //根據模式選擇
    //TestIF.iTestMode
/*    int MaxI=0;
    int MaxJ=0;
    switch(TestIF.iTestMode)
    {
      case 0://1x2
           MaxI=1;
           MaxJ=2;
           break;
      case 1://1x1
           MaxI=1;
           MaxJ=1;
           break;
      case 2://1x4
           MaxI=1;
           MaxJ=4;
           break;
      case 3://2x2
           MaxI=2;
           MaxJ=2;
           break;
      case 5://2x4
           MaxI=2;
           MaxJ=4;
           break;
      case 6://2x8
           MaxI=2;
           MaxJ=8;
           break;
    };

    //AnsiString KeyWord[16]={ "Dut1=" ,"Dut2=" ,"Dut3=" ,"Dut4=" ,"Dut5=" ,"Dut6=" ,"Dut7=" ,"Dut8=" ,
    //                         "Dut9=" ,"Dut10=" ,"Dut11=" ,"Dut12=" ,"Dut13=" ,"Dut14=" ,"Dut15=" ,"Dut16=" };
    //AnsiString Position[16]={ "Ch1" ,"Ch2" , "Ch3", "Ch4", "Ch5", "Ch6", "Ch7", "Ch8",
    //                          "Ch9", "Ch10", "Ch11", "Ch12", "Ch13", "Ch14", "Ch15", "Ch16" };

    AnsiString value;
    int index=0;
    int PosIndex[16]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
                      -1,-1,-1,-1,-1,-1};
    bool bSet[17]={false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};

    for(int i=0; i<MaxI; i++)
    {
        for(int j=0; j<MaxJ; j++)
        {
            index = i*MaxJ+j;
            PosIndex[index] = atoi(Data[index].c_str());

            if(PosIndex[index]<1 || PosIndex[index]>MaxI*MaxJ )
              return 1;

            if(bSet[PosIndex[index]] == false)     bSet[PosIndex[index]]=true;
            else                                     return 1;
        }
    }

    for(int i=0; i<MaxI; i++)
    {
        for(int j=0; j<MaxJ; j++)
        {
            index = i*MaxJ+j;
            TestIF_File.iSiteMap[i][j] = PosIndex[index];
        }
    }

    AnsiString S=GetLastOpenFN();
    AnsiString szDir="";
    AnsiString str;

    szDir.sprintf("D:\\HT9045\\IniData\\Data\\%s", S);
    if(!DirectoryExists(szDir))
    {
        ForceDirectories(szDir);
    }
    szDir+="\\HandlerCondition.Data";

    for(int i=0; i<2; i++)
    {
        for(int j=0; j<8; j++)
        {
            str.sprintf("Site %c%c", i+'A', j+'A');
            WriteIniData(szDir, "Configuration", str, TestIF_File.iSiteMap[i][j]);
        }
    }

    ChangeSite();

    DoStructUnitConvert();
    fMain->ShowTestHeadComp();
    fTestCategory->AdjFormData();
    fContactCT->ShowFormComp();    */

    return 0;
}
//---------------------------------------------------------------------------
int SetSetUpFileName(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;
    if(CheckNeedCleanOut())
        return 2;

    fMainAT->cbSetUpFileName->Text=Data[0];
    return 1;
}
//---------------------------------------------------------------------------
int SetSoakTime(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;
    if(CheckNeedCleanOut())
        return 2;

    fMainAT->edtSoakTime->Text=Data[0];
    return 0;
}
//---------------------------------------------------------------------------
int SetTemperature(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;
    if(CheckNeedCleanOut())
        return 2;

    fMainAT->edtTemperature->Text=Data[0];

    return 0;
}
//---------------------------------------------------------------------------
int SetTempMode(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;
    if(CheckNeedCleanOut())
        return 2;

    fMainAT->cTempmode->Checked=(Data[0]==1)?true:false;
    return 0;
}
//---------------------------------------------------------------------------
int SetTesterConnect(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;
    if(CheckNeedCleanOut())
        return 2;
    fMainAT->cConnectionTester->Checked=(Data[0]==1)?true:false;
}
//---------------------------------------------------------------------------
int SetTesterMode(AnsiString *Data)
{
    fMainAT->rgTesterMode->ItemIndex=atoi(Data[0].c_str());
}
//---------------------------------------------------------------------------
bool CheckInitialOK()
{
    return true;
}
//---------------------------------------------------------------------------
bool CheckNeedCleanOut()
{
    if(fMainAT->cbHandlerStatus=="RUNNING")
        return true;
    else
        return false;
}
//---------------------------------------------------------------------------
bool CheckSystemStart()
{
    if(fMainAT->cbHandlerStatus=="RUNNING")
        return true;
    else
        return false;
}
//---------------------------------------------------------------------------
int ShowOLPState(int iState)
{
    fMainAT->ShowOLPState(iState);
    return 0;
}
//---------------------------------------------------------------------------
AnsiString GetMainStatus()
{
    AnsiString S=fMainAT->cbHandlerStatus->Text;
    if(S=="RUNNING" ||
       S=="PAUSE" ||
       S=="HALT"  )
    {
        if(S=="RUNNING")
        {
            S="RUN";
        }
        return S;
    }
}
//---------------------------------------------------------------------------
AnsiString GetDataPath()
{
    return "d:\\HT9045\\IniData\\Data\\";
}
//---------------------------------------------------------------------------
AnsiString GetSoftwareVersion()
{
    return fMainAT->enVersion->Text;
}
//---------------------------------------------------------------------------
AnsiString GetActTemp(int i)
{
    TEdit *tmpEdit[10]={fMainAT->edTemp1, fMainAT->edTemp2, fMainAT->edTemp3, fMainAT->edTemp4, fMainAT->edTemp5,
                        fMainAT->edTemp6, fMainAT->edTemp7, fMainAT->edTemp8, fMainAT->edTemp9, fMainAT->edTemp10};
    return tmpEdit[i]->Text;
}
//---------------------------------------------------------------------------
AnsiString GetHandlerType()
{
    return fMainAT->cbMachineModel->Text;
}
//---------------------------------------------------------------------------
AnsiString GetSetUpFileName()
{
    return fMainAT->cbSetUpFileName->Text;
}
//---------------------------------------------------------------------------
AnsiString GetJamCount()
{
//    return AnsiString(LastSet.iJamCount[1]);
    return 0;
}
//---------------------------------------------------------------------------
AnsiString GetSoakTime()
{
    return fMainAT->edtSoakTime->Text;
}
//---------------------------------------------------------------------------
AnsiString GetTemperature()
{
    return fMainAT->edtTemperature->Text;
}
//---------------------------------------------------------------------------
AnsiString GetTempMode()
{
    return (fMainAT->cTempmode->Checked)?1:0;
}
//---------------------------------------------------------------------------
AnsiString GetTesterConnect()
{
    return (fMainAT->cConnectionTester->Checked)?1:0;
}
//---------------------------------------------------------------------------
AnsiString GetTesterMode()
{
    return fMainAT->rgTesterMode->ItemIndex;
}
//---------------------------------------------------------------------------
AnsiString GetLoadCount()
{
    //return LastSet.SendCT[1];    // 2009.09.04 , Joye
    return 0;
}
//---------------------------------------------------------------------------
bool CheckCanChangeRealDummy()
{
    //return fMain->CheckCanChangeRealDummy();
    return true;
}
//---------------------------------------------------------------------------
void DoClearReportRequest(int iSwitchCase)
{
    //fCounterClear->ClearCount(iSwitchCase);
    return;
}
//---------------------------------------------------------------------------
int GetCategory(AnsiString *Data)
{
    //AnsiString KeyWord[16]={"I//F Error=","Bin1=","Bin2=","Bin3=","Bin4=","Bin5=","Bin6=","Bin7=","Bin8=","Bin9=","Bin10=","Bin11=","Bin12=","Bin13=","Bin14=","Bin15="};
    //AnsiString Position[10]={"No Use","Auto1", "Auto2", "Auto3", "Fix1", "Fix2", "Fix3", "Fix4", "Fix5", "Fix6"};

    for(int i=0; i<16; i++)
    {
        Data[i]=AnsiString(fMainAT->cbCategory[i]->ItemIndex);
    }
    return 0;
}
//---------------------------------------------------------------------------
int GetMapping(AnsiString *Data)
{
    //根據模式選擇
    //TestIF.iTestMode
    /*int MaxI=0;
    int MaxJ=0;
    switch(TestIF.iTestMode)
    {
      case 0://1x2
           MaxI=1;
           MaxJ=2;
           break;
      case 1://1x1
           MaxI=1;
           MaxJ=1;
           break;
      case 2://1x4
           MaxI=1;
           MaxJ=4;
           break;
      case 3://2x2
           MaxI=2;
           MaxJ=2;
           break;
      case 5://2x4
           MaxI=2;
           MaxJ=4;
           break;
      case 6://2x8
           MaxI=2;
           MaxJ=8;
           break;
    };

    //AnsiString KeyWord[16]={ "Dut1=" ,"Dut2=" ,"Dut3=" ,"Dut4=" ,"Dut5=" ,"Dut6=" ,"Dut7=" ,"Dut8=" ,
    //                         "Dut9=" ,"Dut10=" ,"Dut11=" ,"Dut12=" ,"Dut13=" ,"Dut14=" ,"Dut15=" ,"Dut16=" };
    //AnsiString Position[16]={ "Ch1" ,"Ch2" , "Ch3", "Ch4", "Ch5", "Ch6", "Ch7", "Ch8",
    //                          "Ch9", "Ch10", "Ch11", "Ch12", "Ch13", "Ch14", "Ch15", "Ch16" };

    for(int i=0; i<MaxI; i++)
    {
        for(int j=0; j<MaxJ; j++)
        {
            Data[i*MaxJ+j] = TestIF.iSiteMap[i][j];
        }
    }
    return MaxI*MaxJ; */
    return 0;
}
//---------------------------------------------------------------------------
int GetDutOnOff(AnsiString *Data)
{
    //根據模式選擇
    //TestIF.iTestMode
    int MaxI=0;
    int MaxJ=0;
    /*switch(TestIF.iTestMode)
    {
      case 0://1x2
           MaxI=1;
           MaxJ=2;
           break;
      case 1://1x1
           MaxI=1;
           MaxJ=1;
           break;
      case 2://1x4
           MaxI=1;
           MaxJ=4;
           break;
      case 3://2x2
           MaxI=2;
           MaxJ=2;
           break;
      case 5://2x4
           MaxI=2;
           MaxJ=4;
           break;
      case 6://2x8
           MaxI=2;
           MaxJ=8;
           break;

    };

    //AnsiString KeyWord[16]={ "Dut1=" ,"Dut2=" ,"Dut3=" ,"Dut4=" ,"Dut5=" ,"Dut6=" ,"Dut7=" ,"Dut8=" ,
    //                         "Dut9=" ,"Dut10=" ,"Dut11=" ,"Dut12=" ,"Dut13=" ,"Dut14=" ,"Dut15=" ,"Dut16=" };
    //AnsiString Result[2]={ "Close","Open"};
    int num;
    for(int i=0; i<MaxI; i++)
    {
        for(int j=0; j<MaxJ; j++)
        {
            Data[i*MaxJ+j] = (LastSet.bUseTestSocket[i][j]==true)?1:0;
        }
    }    */
    return MaxI*MaxJ;
}
//---------------------------------------------------------------------------
int GetProductivity(AnsiString *Data)
{
    Data[0]="0";                                    //Down Time Rate
    Data[1]="0 /1 unit";       //MUBF
    Data[2]="0 /1 hr";       //MTBF
    Data[3]="0";
    return 0;                                    //MTTR
}
//---------------------------------------------------------------------------
int GetSortingCount(AnsiString *Data)
{
    /*Data[0]=AnsiString(LastSet.BinCT[0][0]);
    Data[1]=AnsiString(LastSet.BinCT[0][1]);
    Data[2]=AnsiString(LastSet.BinCT[0][2]);
    Data[3]=AnsiString(LastSet.BinCT[0][3]);
    Data[4]=AnsiString(LastSet.BinCT[0][4]);
    Data[5]=AnsiString(LastSet.BinCT[0][5]);
    if(TrayForm.iFixTrayMode==0)
    {
        Data[6]=0;
        Data[7]=0;
        Data[8]=0;
    }
    else
    {
        Data[6]=AnsiString(LastSet.BinCT[0][6]);
        Data[7]=AnsiString(LastSet.BinCT[0][7]);
        Data[8]=AnsiString(LastSet.BinCT[0][8]);
    }          */
    return 0;
}
//---------------------------------------------------------------------------
int GetSocketCount(AnsiString *Data)
{
    //HT9045,最多8個Socket
    // Data Format : Bin0,Bin1,Bin2,Bin3,Bin4,Bin5,Bin6,Bin7,Bin8,Bin9,Bin10,Bin11,Bin12,Bin13,Bin14,Bin15,SKIP,TOTAL,PASS
    // 順序: Aa, Ab, Ac, Ad, Ba, Bb, Bc, Bd
    int iIFErrTotal=0, iGategoryTotal[15]={0}, i_Total=0, i_Pass_Total=0;
    int DutCount=MAX_Index_Col*MAX_Index_Row;
   /* for(int i=0; i<DutCount; i++)
    {
        int j=i/MAX_Index_Col, k=i%MAX_Index_Col;

        // Bin 0 Data
        Data[i]="0,";

        // Bin 1 ~ Bin15
        for(int l=0; l<15; l++)
        {
            Data[i]+=AnsiString(ArmData[0]->ArmSKET[j][k]->GetSelBinCT(l)+ArmData[1]->ArmSKET[j][k]->GetSelBinCT(l))+",";
            iGategoryTotal[l]+=(ArmData[0]->ArmSKET[j][k]->GetSelBinCT(l)+ArmData[1]->ArmSKET[j][k]->GetSelBinCT(l));
        }

        //I/F Error
        Data[i]+=AnsiString(ArmData[0]->ArmSKET[j][k]->GetIFError()+ArmData[1]->ArmSKET[j][k]->GetIFError())+",";
        //Total
        Data[i]+=AnsiString(ArmData[0]->ArmSKET[j][k]->GetTotal()+ArmData[1]->ArmSKET[j][k]->GetTotal())+",";
        //Pass
        Data[i]+=AnsiString(ArmData[0]->ArmSKET[j][k]->GetPassCT()+ArmData[0]->ArmSKET[j][k]->GetPassCT());
        i_Pass_Total+=(ArmData[0]->ArmSKET[j][k]->GetPassCT()+ArmData[1]->ArmSKET[j][k]->GetPassCT());
        iIFErrTotal+=(ArmData[0]->ArmSKET[j][k]->GetIFError()+ArmData[1]->ArmSKET[j][k]->GetIFError());
    }
    Data[DutCount]="0";
    for(int i=0; i<15; i++)
    {
        Data[DutCount]+=","+AnsiString(iGategoryTotal[i]);
        i_Total+=iGategoryTotal[i];
    }
    Data[DutCount]+=","+AnsiString(iIFErrTotal)+","+AnsiString(i_Total)+","+AnsiString(i_Pass_Total);
            */
    return 0;
}
//---------------------------------------------------------------------------
int GetHeadCount(AnsiString *Data)
{
    int iIFErrTotal=0, iGategoryTotal[15]={0}, i_Total=0, i_Pass_Total=0;
    // Data Format : Bin0,Bin1,Bin2,Bin3,Bin4,Bin5,Bin6,Bin7,Bin8,Bin9,Bin10,Bin11,Bin12,Bin13,Bin14,Bin15,SKIP,TOTAL,PASS
    // 順序: Aa, Ab, Ac, Ad, Ba, Bb, Bc, Bd 先Arm1再Arm2
   /* for(int x=0; x<2; x++)
    {
        for(int i=0; i<MAX_Index_Col; i++)
        {
            int j=i/MAX_Index_Col, k=i%MAX_Index_Col, y=x*8+i;

            // Bin 0 Data
            Data[y]="0,";
            iIFErrTotal+=ArmData[x]->ArmSKET[j][k]->GetIFError();//category total->IFErr

            // Bin 1 ~ Bin15
            for(int l=0; l<15; l++)
            {
                Data[y]+=AnsiString(ArmData[x]->ArmSKET[j][k]->GetSelBinCT(l))+",";
                iGategoryTotal[l]+=ArmData[x]->ArmSKET[j][k]->GetSelBinCT(l);
            }

            //I/F Error
            Data[y]+=AnsiString(ArmData[x]->ArmSKET[j][k]->GetIFError())+",";
            //Total
            Data[y]+=AnsiString(ArmData[x]->ArmSKET[j][k]->GetTotal())+",";
            //Pass
            Data[y]+=AnsiString(ArmData[x]->ArmSKET[j][k]->GetPassCT());
            i_Pass_Total+=ArmData[x]->ArmSKET[j][k]->GetPassCT();
        }
    }
    Data[16]="0";
    for(int i=0; i<15; i++)
    {
        Data[16]+=","+AnsiString(iGategoryTotal[i]);
        i_Total+=iGategoryTotal[i];
    }
    Data[16]+=","+AnsiString(iIFErrTotal)+","+AnsiString(i_Total)+","+AnsiString(i_Pass_Total);
       */
    return 0;
}
//---------------------------------------------------------------------------
int GetTestResult(AnsiString *Data)
{
   /* for(int i=0; i<MAX_Index_Row; i++)
    {
        for(int j=0; j<MAX_Index_Col; j++)
        {
            if(TestSocket.Item[i][j]!=NULL_IC && TestSocket.Item[i][j]!=HAS_NULL_IC)
                Data[i*MAX_Index_Col+j].sprintf("Dut%d=Bin%d", TestIF.iSiteMap[i][j], (TestSocket.Item[i][j]-TEST_PASS+1));
            else
                Data[i*MAX_Index_Col+j].sprintf("Dut%d=Bin%d", TestIF.iSiteMap[i][j], 0);
        }
    }      */
    return MAX_Index_Row*MAX_Index_Col;
}
//---------------------------------------------------------------------------
int GetTrayForm(AnsiString *Data)
{
    for(int i=0; i<6; i++)
        Data[i]=fMainAT->edTrayForm[i]->Text;
    return 6;
}
//---------------------------------------------------------------------------
int GetPlateForm(AnsiString *Data)
{
    for(int i=0; i<6; i++)
        Data[i]=fMainAT->edPlateForm[i]->Text;
    return 6;
}
//---------------------------------------------------------------------------
void DoOnLineReply(AnsiString *Data)
{
    Data[0]=fMainAT->cbMachineModel->Text;
    Data[1]=fMainAT->edMachineID->Text;
    Data[2]=fMainAT->enVersion->Text;
}
//---------------------------------------------------------------------------
void AlarmMessage(AnsiString enMess, AnsiString chMess)
{
    //ShowMyMessage("Wait client connect.", "等待連線...");
}
//---------------------------------------------------------------------------
void DoULRequest(AnsiString *Data)  //上傳檔案到Host
{
/*    AnsiString str1;
    AnsiString sULFileName=fMain->ComboBox3->Text;
    char ddd[10];
    FILE *fs;

    //先刪除舊的壓縮檔
    str1.sprintf("del d:\\HT9045\\IniData\\Data\\%s.zip", sULFileName);
    system(str1.c_str());
    Sleep(50);

    //把7Zip準備好
    if(FileExists( "d:\\HT9045\\7z.exe" ) == false)
    {
        CopyFile( "C:\\Program Files\\7-Zip\\7z.exe" , "d:\\HT9045\\7z.exe" , FALSE);
    }

    //把工作檔壓縮
    str1.sprintf("d:\\HT9045\\7z.exe a -tzip \"d:\\HT9045\\IniData\\Data\\%s.zip\" \"d:\\HT9045\\IniData\\Data\\%s\\*.*\"", sULFileName, sULFileName);
    system(str1.c_str());

    str1="";
    int j=0;
    int s,size=0;
    char ccc;
    str1.sprintf("d:\\HT9045\\IniData\\Data\\%s.zip", sULFileName);
    fs=fopen(str1.c_str(), "rb");
    //記錄檔案大小
    if(fs != NULL )    // 2009.07.30 , Joye
    {
        fseek(fs, 0, SEEK_END);
        size = ftell(fs);
    }
    fclose(fs);

    //將工作檔的壓縮檔進行轉換成要傳輸的格式
    fs=fopen(str1.c_str(), "rb");
    str1="";
    while(j<=size)
    {
        j++;
        if(fscanf(fs,"%1c",&ccc))
        {
            sprintf(ddd,"%02X",ccc);
            if(ddd[0]=='F' && ddd[1]=='F' && ddd[2]=='F')
            {
                ddd[0]=ddd[6];
                ddd[1]=ddd[7];
                ddd[2]='\0';
            }
            str1=str1+ddd;
        }
        //str1=str1+s;
    }
    fclose(fs); // 2009.08.04

    str1.Delete(str1.Length()-2,2);
    Data[0]=sULFileName+".zip";
    Data[1]=str1.Length();
    Data[2]=str1;
    Data[3]="0";        */
}
//---------------------------------------------------------------------------
int DoDLRequest(AnsiString sDLFileName)     //下載檔案,並切換工作檔
{
 /*   AnsiString str, str2;
    AnsiString SPath[3]={"D:\\HT9045\\IniData\\Data\\", "D:\\HT9045\\IniData\\Offset\\", "D:\\HT9045\\IniData\\GoleSample\\"};
    int ret=0;
    bool bHasSetUpFile=false;

    if(!DirectoryExists(SPath[0]+sDLFileName))  //首先判斷工作檔有沒有
    {
        ForceDirectories(SPath[0]+sDLFileName);
        bHasSetUpFile=false;
    }
    else
    {
        bHasSetUpFile=true;
    }

    if(bHasSetUpFile==false)    //沒有的話,就進行解壓縮
    {
        for(int i=1; i<3; i++)
        {
            if(!DirectoryExists(SPath[i]+sDLFileName))
            {
                ForceDirectories(SPath[i]+sDLFileName);
            }
        }

        //檢查Offset檔
        str.sprintf("D:\\HT9045\\IniData\\Offset\\%s\\Position Offset.Data", sDLFileName);
        if(!FileExists(str))    //如果沒有就從當下的工作檔複製過來
        {
            str2.sprintf("D:\\HT9045\\IniData\\Offset\\%s\\Position Offset.Data", fMain->ComboBox3->Text);
            CopyFile(str2.c_str(), str.c_str(), false);
        }

        str.sprintf("D:\\HT9045\\IniData\\Offset\\%s\\Position Offset Hot.Data", sDLFileName);
        if(!FileExists(str))
        {
            str2.sprintf("D:\\HT9045\\IniData\\Offset\\%s\\Position Offset Hot.Data", fMain->ComboBox3->Text);
            CopyFile(str2.c_str(), str.c_str(), false);
        }

        str.sprintf("Del /f/s/q \"%s\"", SPath[0]+sDLFileName);
        system(str.c_str());

        Sleep(50);
        if(FileExists("d:\\HT9045\\7z.exe")==false)
        {
            CopyFile("C:\\Program Files\\7-Zip\\7z.exe", "d:\\HT9045\\7z.exe", false);
        }

        str="d:\\HT9045\\7z.exe e \"d:\\HT9045\\IniData\\Data\\"+sDLFileName+".zip\" -o\"d:\\HT9045\\IniData\\Data\\"+sDLFileName+"\\\"";
        ret=system(str.c_str());
    }
    else
        ret=0;

    if(ret==0)  //解壓縮成功的話
    {
        if(bHasSetUpFile==false)    //新的工作檔的話，就要加一筆進去。
            fMain->ComboBox3->Items->Add(sDLFileName);
        fMain->ComboBox3->Text=sDLFileName;
        fMain->ComboBox3Change(fMain);
    }
    return ret;    */
    return 0;
}
