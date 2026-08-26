//---------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop
#include <vcl.h>
#pragma hdrstop
#include "mymessbox.h"
#include "LiftCount.h"
#include "cprod.h"
#include "common.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfLiftCount *fLiftCount;
//static TPanel *SocketPtr[4];
//static TPanel *Arm1TIMPtr[4];
//static TPanel *Arm2TIMPtr[4];

//---------------------------------------------------------------------------
__fastcall TfLiftCount::TfLiftCount(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfLiftCount::FormShow(TObject *Sender)
{
    TPanel *SocketPtr[]  ={pl_Socket_1, pl_Socket_2, pl_Socket_3, pl_Socket_4};
    TPanel *Arm1TIMPtr[] ={pl_Arm1_TIM1, pl_Arm1_TIM2, pl_Arm1_TIM3, pl_Arm1_TIM4};
    TPanel *Arm2TIMPtr[] ={pl_Arm2_TIM1, pl_Arm2_TIM2, pl_Arm2_TIM3, pl_Arm2_TIM4};

    TLabel *SocketLab[]  ={Lab_Socket1, Lab_Socket2, Lab_Socket3, Lab_Socket4};
    TLabel *Arm1TIMLab[] ={Lab_Arm1_1, Lab_Arm1_2, Lab_Arm1_3, Lab_Arm1_4};
    TLabel *Arm2TIMLab[] ={Lab_Arm2_1, Lab_Arm2_2, Lab_Arm2_3, Lab_Arm2_4};

    int iSiteCount=0;

    for(int i=0; i<4; i++)
    {
        SocketPtr[i] ->Color = clSilver;
        Arm1TIMPtr[i]->Color = clSilver;
        Arm2TIMPtr[i]->Color = clSilver;
        SocketPtr[i]->Visible=false;
        Arm1TIMPtr[i]->Visible=false;
        Arm2TIMPtr[i]->Visible=false;
        SocketLab[i]->Visible=false;
        Arm1TIMLab[i]->Visible=false;
        Arm2TIMLab[i]->Visible=false;
    }

    if(TestIF_File.iTestMode==SingleSite)
    {
        iSiteCount=1;
    }
    else if(TestIF_File.iTestMode==DualSite || TestIF_File.iTestMode==DualSite2x1)
    {
        iSiteCount=2;
    }
    else if(TestIF_File.iTestMode==TriSite1X3)
    {
        iSiteCount=3;
    }
    else if(TestIF_File.iTestMode==QualSite1X4 || TestIF_File.iTestMode==QualSite2X2)
    {
        iSiteCount=4;
    }

    if(TestIF_File.iTestMode==DualSite2x1)
    {
        SocketLab[0]->Caption=AnsiString(LastSet.iSocketContactCount[0][0]);
        Arm1TIMLab[0]->Caption=AnsiString(IniConfig.HeadContactCount[0][0][0]);
        Arm2TIMLab[0]->Caption=AnsiString(IniConfig.HeadContactCount[0][1][0]);

        SocketLab[1]->Caption=AnsiString(LastSet.iSocketContactCount[1][0]);
        Arm1TIMLab[1]->Caption=AnsiString(IniConfig.HeadContactCount[0][0][1]);
        Arm2TIMLab[1]->Caption=AnsiString(IniConfig.HeadContactCount[0][1][1]);
    }
    else if(TestIF_File.iTestMode==QualSite2X2)
    {
        SocketLab[0]->Caption=AnsiString(LastSet.iSocketContactCount[0][0]);
        Arm1TIMLab[0]->Caption=AnsiString(IniConfig.HeadContactCount[0][0][0]);
        Arm2TIMLab[0]->Caption=AnsiString(IniConfig.HeadContactCount[0][1][0]);

        SocketLab[1]->Caption=AnsiString(LastSet.iSocketContactCount[0][1]);
        Arm1TIMLab[1]->Caption=AnsiString(IniConfig.HeadContactCount[0][0][2]);
        Arm2TIMLab[1]->Caption=AnsiString(IniConfig.HeadContactCount[0][1][2]);

        SocketLab[2]->Caption=AnsiString(LastSet.iSocketContactCount[1][0]);
        Arm1TIMLab[2]->Caption=AnsiString(IniConfig.HeadContactCount[0][0][1]);
        Arm2TIMLab[2]->Caption=AnsiString(IniConfig.HeadContactCount[0][1][1]);

        SocketLab[3]->Caption=AnsiString(LastSet.iSocketContactCount[1][1]);
        Arm1TIMLab[3]->Caption=AnsiString(IniConfig.HeadContactCount[0][0][3]);
        Arm2TIMLab[3]->Caption=AnsiString(IniConfig.HeadContactCount[0][1][3]);
    }
    else
    {
        SocketLab[0]->Caption=AnsiString(LastSet.iSocketContactCount[0][0]);
        Arm1TIMLab[0]->Caption=AnsiString(IniConfig.HeadContactCount[0][0][0]);
        Arm2TIMLab[0]->Caption=AnsiString(IniConfig.HeadContactCount[0][1][0]);

        SocketLab[1]->Caption=AnsiString(LastSet.iSocketContactCount[0][1]);
        Arm1TIMLab[1]->Caption=AnsiString(IniConfig.HeadContactCount[0][0][2]);
        Arm2TIMLab[1]->Caption=AnsiString(IniConfig.HeadContactCount[0][1][2]);

        SocketLab[2]->Caption=AnsiString(LastSet.iSocketContactCount[0][2]);
        Arm1TIMLab[2]->Caption=AnsiString(IniConfig.HeadContactCount[0][0][4]);
        Arm2TIMLab[2]->Caption=AnsiString(IniConfig.HeadContactCount[0][1][4]);

        SocketLab[3]->Caption=AnsiString(LastSet.iSocketContactCount[0][3]);
        Arm1TIMLab[3]->Caption=AnsiString(IniConfig.HeadContactCount[0][0][6]);
        Arm2TIMLab[3]->Caption=AnsiString(IniConfig.HeadContactCount[0][1][6]);
    }
    
    for(int j=0; j<iSiteCount; j++)
    {
        SocketPtr[j]->Visible=true;
        Arm1TIMPtr[j]->Visible=true;
        Arm2TIMPtr[j]->Visible=true;
        SocketLab[j]->Visible=true;
        Arm1TIMLab[j]->Visible=true;
        Arm2TIMLab[j]->Visible=true;
    }
    LiftCountTimer->Enabled=true;

//    fContact->SendToBack();
    fLiftCount->BringToFront();
//    fLiftCount->BringToFront();
//    ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfLiftCount::pl_Socket_1Click(TObject *Sender)
{
    TPanel *panel = dynamic_cast<TPanel*>(Sender);
    if(!panel)
        return;

    if(panel->Color == clSilver)
    {
        panel->Color = clLime;
    }
    else
    {
        panel->Color = clSilver;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLiftCount::btRecoverClick(TObject *Sender)
{
    TPanel *SocketPtr[]  ={pl_Socket_1, pl_Socket_2, pl_Socket_3, pl_Socket_4};
    TPanel *Arm1TIMPtr[] ={pl_Arm1_TIM1, pl_Arm1_TIM2, pl_Arm1_TIM3, pl_Arm1_TIM4};
    TPanel *Arm2TIMPtr[] ={pl_Arm2_TIM1, pl_Arm2_TIM2, pl_Arm2_TIM3, pl_Arm2_TIM4};

    for(int i=0; i<4; i++)
    {
        SocketPtr[i]    ->Color = clSilver;
        Arm1TIMPtr[i]   ->Color = clSilver;
        Arm2TIMPtr[i]   ->Color = clSilver;
    }

}
//---------------------------------------------------------------------------
void __fastcall TfLiftCount::btSaveClick(TObject *Sender)
{
    int iMap[]   ={0,2,1,3};
    int ret=0;
    TPanel *SocketPtr[]  ={pl_Socket_1, pl_Socket_2, pl_Socket_3, pl_Socket_4};
    TPanel *Arm1TIMPtr[] ={pl_Arm1_TIM1, pl_Arm1_TIM2, pl_Arm1_TIM3, pl_Arm1_TIM4};
    TPanel *Arm2TIMPtr[] ={pl_Arm2_TIM1, pl_Arm2_TIM2, pl_Arm2_TIM3, pl_Arm2_TIM4};

    TLabel *SocketLab[]  ={Lab_Socket1, Lab_Socket2, Lab_Socket3, Lab_Socket4};
    TLabel *Arm1TIMLab[] ={Lab_Arm1_1, Lab_Arm1_2, Lab_Arm1_3, Lab_Arm1_4};
    TLabel *Arm2TIMLab[] ={Lab_Arm2_1, Lab_Arm2_2, Lab_Arm2_3, Lab_Arm2_4};

//    ret=ShowMyMessageBox_YES_NO("Confirm data deletion?", "確認資料刪除？");
    ret=ShowMyMessageBox_YES_NO("Config data reset to zero?", "確定資料要重置為零？");
    if(ret==2)
    {
        return;
    }

    for(int i=0; i<4; i++)
    {
        if(SocketPtr[i]->Color==clLime)
        {
            if(TestIF_File.iTestMode==DualSite2x1)
            {
                if(i==0)
                    LastSet.iSocketContactCount[0][0]=0;
                else if(i==1)
                    LastSet.iSocketContactCount[1][0]=0;
            }
            else if(TestIF_File.iTestMode==QualSite2X2)
            {
                if(i<2)
                {
                    LastSet.iSocketContactCount[0][i]=0;
                }
                else
                {
                    if(i==2)
                        LastSet.iSocketContactCount[1][0]=0;
                    else
                        LastSet.iSocketContactCount[1][1]=0;
                }
                SocketLab[i]->Caption="0";
            }
            else
            {
                LastSet.iSocketContactCount[0][i]=0;
                SocketLab[i]->Caption="0";                
            }
//            LastSet.iSocketContactCount[0][2];
//
//            IniConfig.HeadContactCount[0][0][0]=0;
//            SocketLab[i]->Caption="0";
        }

        if(Arm1TIMPtr[i]->Color==clLime)
        {
            if(TestIF_File.iTestMode==QualSite2X2)
            {
                IniConfig.HeadContactCount[0][0][iMap[i]]=0;
            }
            else
            {
                IniConfig.HeadContactCount[0][0][i*2]=0;
            }
            Arm1TIMLab[i]->Caption="0";
        }

        if(Arm2TIMPtr[i]->Color==clLime)
        {
            if(TestIF_File.iTestMode==QualSite2X2)
            {
                IniConfig.HeadContactCount[0][1][iMap[i]]=0;
            }
            else
            {
                IniConfig.HeadContactCount[0][1][i*2]=0;
            }
            Arm2TIMLab[i]->Caption="0";
        }
    }
    btRecover->Click();
}
//---------------------------------------------------------------------------
void __fastcall TfLiftCount::btEXITClick(TObject *Sender)
{
    int ret=0;

    ret=ShowMyMessageBox_YES_NO("Are you sure you want to exit?", "您確定要退出嗎?");
    if(ret==2)
    {
        return;
    }
    AnsiString str1;
    AnsiString str2;
    AnsiString str3;
    AnsiString sPath=AuthPath+"config.ini";
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<8; j++)
        {
            str1.sprintf("ContactSet%d_%d", i, j);
            str2.sprintf("HeadContactCount%d_%d", i, j);
            str3.sprintf("HeadContactCountHistory%d_%d", i, j);

            //Steven 20211215 : 改成不存Change Log
            WriteIniDataNoLog(sPath, "O_Count", "O_14"+AnsiString(str1), IniConfig.ContactSet[0][i][j]              );
            WriteIniDataNoLog(sPath, "O_Count", "O_14"+AnsiString(str2), IniConfig.HeadContactCount[0][i][j]        );
            WriteIniDataNoLog(sPath, "O_Count", "O_14"+AnsiString(str3), IniConfig.HeadContactCountHistory[0][i][j] );
            WriteIniDataNoLog(sPath, "O_Count", "O_15"+AnsiString(str1), IniConfig.ContactSet[1][i][j]              );
            WriteIniDataNoLog(sPath, "O_Count", "O_15"+AnsiString(str2), IniConfig.HeadContactCount[1][i][j]        );
            WriteIniDataNoLog(sPath, "O_Count", "O_15"+AnsiString(str3), IniConfig.HeadContactCountHistory[1][i][j] );
            WriteIniDataNoLog(sPath, "O_Count", "O_16"+AnsiString(str1), IniConfig.ContactSet[2][i][j]              );
            WriteIniDataNoLog(sPath, "O_Count", "O_16"+AnsiString(str2), IniConfig.HeadContactCount[2][i][j]        );
            WriteIniDataNoLog(sPath, "O_Count", "O_16"+AnsiString(str3), IniConfig.HeadContactCountHistory[2][i][j] );
        }
    }
    fLiftCount->Close();
    LiftCountTimer->Enabled=false;
}
//---------------------------------------------------------------------------
void __fastcall TfLiftCount::LiftCountTimerTimer(TObject *Sender)
{
    static bool bRunTimer=false;

    if(bRunTimer==true)
    {
        return;
    }

    bRunTimer=true;
    if(fLiftCount->Visible==true)
    {
//        fContact->SendToBack();
//        fLiftCount->BringToFront();
    }
    
    bRunTimer=false;
}
//---------------------------------------------------------------------------
