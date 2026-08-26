//------------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop

#include "fVATMesFileSys.h"                                                     //jou 20200409 : VTest Mes system

#include "mymessbox.h"
#include "cprod.h"
#include "cmydef.h"
#include "cpublic.h"
#include "main.h"
#include "common.h"
#include "cConfiguration.h"
#include "cCounterClear.h"
#include "uYieldMonitoring.h"
#include "uLotInfo.h"
#include "cSocket.h"
#include "csystem.h"
#include "SCK_WebService.h"
#include "FTPClient.h"
#include "uLotInfo.h"
#include "main.h"
#include "cSortCT.h"
//------------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfMesSystem *fMesSystem;
//------------------------------------------------------------------------------
int ValidNoRTBinInfo(AnsiString str, TStringList* result)                       //RogerYang 20250626 偉測不可複測bin功能
{
    result->Clear();
    AnsiString trimmed = str.Trim();
    if(trimmed.IsEmpty())
        return 2;

    TStringList* temp = new TStringList();
    temp->Delimiter = ',';
    temp->DelimitedText=trimmed;

    for(int i=0; i <temp->Count; i++)
    {
        AnsiString token=temp->Strings[i].Trim();
        int num=token.ToIntDef(-1);

        if(num<1 || num>3)                                                      //其他bin別視為錯誤
        {
            return 1;
        }
        result->Add(IntToStr(num));
    }
    delete temp;
    return 0;
}
//---------------------------------------------------------------------------
cJSON* uReadDeviceNumer::SearchObject(cJSON *InNode, AnsiString sFilter)
{
    if(InNode==NULL)
    {
        return NULL;
    }

    if(InNode->type==cJSON_Object)
    {
        cJSON *sortAsNode=cJSON_GetObjectItem(InNode, sFilter.c_str());
        if(sortAsNode!=NULL)
        {
            return sortAsNode;
        }
    }
    cJSON *child=InNode->child;
    while(child!=NULL)
    {
        cJSON *result=SearchObject(child, sFilter);
        if(result!=NULL)
        {
            return result;
        }
        child=child->next;
    }
    return NULL;
}
//------------------------------------------------------------------------------
bool uReadDeviceNumer::Read2DFile(AnsiString sFileNameWithPath)
{
    List2DClear();
    //read file
    FILE* fp=fopen(sFileNameWithPath.c_str(), "r");
    fseek(fp, 0, SEEK_END);
    long file_size=ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* file_buf=(char*)malloc(file_size + 1);
    fread(file_buf, file_size, 1, fp);
    file_buf[file_size] = '\0';
    fclose(fp);
    //
    cJSON* loaded_root=cJSON_Parse(file_buf);
    free(file_buf);//    release
    if (!loaded_root)
    {
        return false;
    }
    cJSON *result=SearchObject(loaded_root,"Units");
    if(result!=NULL)
    {
        int n=cJSON_GetArraySize(result);
        for(int i=0; i<n; i++)
        {
            cJSON* UnitID =cJSON_GetArrayItem(result, i);
            cJSON* _obj   =cJSON_GetObjectItem(UnitID, "UnitID");
            List2DoAdd(AnsiString(_obj->valuestring));
        }
        return true;
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------
void uReadDeviceNumer::WriteFile(AnsiString asLocationFile,AnsiString asContent)
{
    FILE* fp = fopen(asLocationFile.c_str(), "w");
    fwrite(asContent.c_str(), strlen(asContent.c_str()), 1, fp);
    fclose(fp);
}
//------------------------------------------------------------------------------
__fastcall TfMesSystem::TfMesSystem(TComponent* Owner)
    : TForm(Owner)
{
    bDownloadLotInforFlag=false;
    bVATToChangeWorkFile-false;

    bCanShowMESReport=false;
    iRunSecondTemp=0;
    iUPHXCount=0;

    for(int i=0; i<eTrayCount; i++)
    {
        asFTUnloadBin[i]=new TStringList();
        asUnloadBin[i]  =new TStringList();
    }

    asPassUnloadBin     =new TStringList();
    asFailUnloadBin     =new TStringList();

    stOEEState          =new TStringList();
    stOEEState12hList   =new TStringList();
    dtOEETimeStart=Now();

    bFormShowJustInitial=false;
    stringlRecordReport =new TStringList;
    stringlRecordReport->Clear();
    LoadStringlRecordReport();

    iJamRateTotalForAlways=0;
    bCreateManualEOCAP_OK=false;                                                //jou 20221104 : VTest CreateManualEOCAP function;
    bFirstMaterialsQA=false;                                                    //jou 20230207 : VTEST 首盤請執行送檢，通知QA確認

    asGetRcsCheckingResult="PASS";
    dIndexTime=0.8;

    bShow=false;
    DoInitialLotInfor();
}
//------------------------------------------------------------------------------
void __fastcall TfMesSystem::buttonSaveClick(TObject *Sender)
{
    int ret;
    AnsiString sPath=AuthPath+"config.ini";

    ret=ShowMyMessageBox_YES_NO("Save file ?", "確定要存檔 ?");
    if(ret==2)
    {
        return;
    }
    IniConfig.asMesSyURLPath     =LabeledURL->Text;
    IniConfig.asMesSyACodePath   =LabeledACode->Text;
    IniConfig.asMesSyActionPath  =LabeledAction->Text;
    IniConfig.asSummaryReportPath=lbledtSummaryReportPath->Text;
    IniConfig.asByTimeOEEPath    =lbledtByTimeOEEpath->Text;
    IniConfig.asUPHReportPath    =lbledtUPHReportPath->Text;

    //jou 20221104 : VTest CreateManualEOCAP function;
    IniConfig.asCreateManualEOCAP_URL =lbledtCreateManualEOCAP_URL->Text;
    IniConfig.asQueryEocapStatusURL   =lbledtQueryEocapStatusURL->Text;

    //jou 20230621 : VTEST Handler即時監控 GetRcsCheckingResult
    IniConfig.asGetRcsCheckingResultUrl      =lbledtGetRcsCheckingResultUrl->Text;
    IniConfig.asGetRcsCheckingResultACode    =lbledtGetRcsCheckingResultACode->Text;
    IniConfig.asGetRcsCheckingResultAction   =lbledtGetRcsCheckingResultAction->Text;
    IniConfig.bGetRcsCheckingResult          =chkGetRcsCheckingResult->Checked;
    //----------------------------

    WriteIniData(sPath, "Server", "asMesSyURLPath"     , IniConfig.asMesSyURLPath);
    WriteIniData(sPath, "Server", "asMesSyACodePath"   , IniConfig.asMesSyACodePath);
    WriteIniData(sPath, "Server", "asMesSyActionPath"  , IniConfig.asMesSyActionPath);
    WriteIniData(sPath, "Server", "asSummaryReportPath", IniConfig.asSummaryReportPath);
    WriteIniData(sPath, "Server", "asByTimeOEEPath"    , IniConfig.asByTimeOEEPath);
    WriteIniData(sPath, "Server", "asUPHReportPath"    , IniConfig.asUPHReportPath);

    //jou 20221104 : VTest CreateManualEOCAP function;
    WriteIniData(sPath, "Server", "asCreateManualEOCAP_URL", IniConfig.asCreateManualEOCAP_URL);
    WriteIniData(sPath, "Server", "asQueryEocapStatusURL"  , IniConfig.asQueryEocapStatusURL);

    //jou 20230621 : VTEST Handler即時監控 GetRcsCheckingResult
    WriteIniData(sPath, "Server", "asGetRcsCheckingResultUrl",      IniConfig.asGetRcsCheckingResultUrl);
    WriteIniData(sPath, "Server", "asGetRcsCheckingResultACode",    IniConfig.asGetRcsCheckingResultACode);
    WriteIniData(sPath, "Server", "asGetRcsCheckingResultAction",   IniConfig.asGetRcsCheckingResultAction);
    WriteIniData(sPath, "Server", "bGetRcsCheckingResult",          IniConfig.bGetRcsCheckingResult);
}
//------------------------------------------------------------------------------
void __fastcall TfMesSystem::FormShow(TObject *Sender)
{
    bShow=true;

    LabeledURL->Text             =IniConfig.asMesSyURLPath;
    LabeledACode->Text           =IniConfig.asMesSyACodePath;
    LabeledAction->Text          =IniConfig.asMesSyActionPath;
    lbledtSummaryReportPath->Text=IniConfig.asSummaryReportPath;
    lbledtByTimeOEEpath->Text    =IniConfig.asByTimeOEEPath;
    lbledtUPHReportPath->Text    =IniConfig.asUPHReportPath;

    //jou 20221104 : VTest CreateManualEOCAP function;
    lbledtCreateManualEOCAP_URL->Text =IniConfig.asCreateManualEOCAP_URL;
    lbledtQueryEocapStatusURL->Text   =IniConfig.asQueryEocapStatusURL;

    if(RunInfo.bLotStart)
    {
        DoEnableLotInforForMesFile(false);
    }
    else
    {
        DoEnableLotInforForMesFile(true);
    }
    groupbMesFileTemp->Enabled=false;
    if(AccessLevel>=2)
    {
        groupbMesFileTemp->Enabled=true;
    }

    //jou 20230621 : VTEST Handler即時監控 GetRcsCheckingResult
    lbledtGetRcsCheckingResultUrl->Text     =IniConfig.asGetRcsCheckingResultUrl;
    lbledtGetRcsCheckingResultACode->Text   =IniConfig.asGetRcsCheckingResultACode;
    lbledtGetRcsCheckingResultAction->Text  =IniConfig.asGetRcsCheckingResultAction;
    chkGetRcsCheckingResult->Checked        =IniConfig.bGetRcsCheckingResult;
}
//------------------------------------------------------------------------------
void __fastcall TfMesSystem::DoInitialLotInfor()
{
    bVATToChangeWorkFile=false;
    asVATToChangeWorkFileName="";

    LabeledHandlerDevice->Text="";
    LabeledEditLotSize->Text="";
    LabeledSiteGap->Text="";
    LabeledEditContFail->Text="";
    LabeledTestTemp->Text="";
    LabeledSoakTime->Text="";
    lbledtLabeledFTAutoBin1->Text="";
    lbledtLabeledFTAutoBin2->Text="";
    LabeledAutoBin1->Text="";
    LabeledAutoBin2->Text="";
    LabeledAutoBin3->Text="";
    LabeledFixBin1->Text="";
    LabeledFixBin2->Text="";
    LabeledFixBin3->Text="";
    LabeledLeadCount->Text="";
    LabeledForcePerPin->Text="";
    lbledtHandlerDeviceLowYield->Text="";
    lbledtProcess->Text="";
    lbledtCustCode->Text="";
    lbledtCustPart->Text="";
    lbledtInternalLot->Text="Unknown";
    lbledtCustLotNum->Text="";
    lbledtDevicePassBins->Text="";
    lbledtHandlerDeviceOpenBins->Text="";
    lbledtDeviceRetestBins->Text="";
    lbledtHandlerDeviceATCcheck->Text="";
    lbledtHandlerDeviceSOTdelay->Text="";
    lbledtHandlerDeviceGPIBaddress->Text="";
    lbledtHandlerDeviceTestTempVar->Text="";
    lbledtHandlerDeviceSiteCheck->Text="";
    lbledtHandlerID->Text="";
    lbledtHandlerDeviceHotplateXDevision->Text="";
    lbledtHandlerDeviceHotplateYDevision->Text="";
    lbledtHandlerDeviceHotplateXStart->Text="";
    lbledtHandlerDeviceHotplateYStart->Text="";
    lbledtHandlerDeviceHotplateXPitch->Text="";
    lbledtHandlerDeviceHotplateYPitch->Text="";
    lbledtHandlerDeviceTrayXDevision->Text="";
    lbledtHandlerDeviceTrayYDevision->Text="";
    lbledtHandlerDeviceTrayXStart->Text="";
    lbledtHandlerDeviceTrayYStart->Text="";
    lbledtHandlerDeviceTrayXPitch->Text="";
    lbledtHandlerDeviceTrayYPitch->Text="";
    lbledtHandlerDeviceSiteLayouts->Text="";
    lbledtHandlerDeviceCleanPadLimit->Text="";
    lbledtHandlerDeviceInshuttleSensorCheck->Text="";
    lbledtTestTimePcs->Text="";
    lbledtC1->Text="Unknown";
    lbledtTestSites->Text="";
    lbledtIndexTime->Text="";
    lbledtHandlerDeviceICNum->Text="";                                          //RogerYang 20250821 : 新增良率中的數量比對
    lbledtHandlerDeviceNotRepeatableArea->Text="";                              //RogerYang 20250717 偉測更改欄位名稱，為了不跟bin別混淆 //RogerYang 20250604 偉測不可複測bin功能

    for(int i=0; i<eTrayCount; i++)
    {
        asFTUnloadBin[i]->Clear();
        asUnloadBin[i]->Clear();
    }

    asPassUnloadBin->Clear();
    asFailUnloadBin->Clear();

    for(int i=0; i<2; i++)
        for(int j=0; j<4; j++)
            iSiteLayouts[i][j]=-1;
}
//------------------------------------------------------------------------------
void __fastcall TfMesSystem::buttonDownloadLotInforClick(TObject *Sender)
{
    AnsiString asURL;
    AnsiString asACode;
    AnsiString asAction;
    AnsiString asItem, asString;
    AnsiString asData,asSavePath,asLotInforTemp="";
    AnsiString S="";
    AnsiString szDir="";
    int ret, iPos;

    bDownloadLotInforFlag=false;
    bNewBinCheck=false;

    if(IniConfig.bCheckFile==false ||
       RunInfo.bLotStart)
    {
        return;
    }

    if(IniConfig.bEnable_SECS_GEM==false)                                       //RogerYang 20260625 : 陳永恆說SECS不要跳
    {
        ret=ShowMyMessageBox_YES_NO("download lot mes ?","確定要下載開批訊息 ?");
        if(ret==2)
        {
            return;
        }
    }

    #ifndef SOFT_SIMULTE                                                        //Rogerynag 20250723 軟體模擬pass
    if(IniConfig.asMesSyURLPath.Length()<=0)
    {
         ShowMyMessage("請輸入URL開批下載資料.");
         return;
    }

    if(IniConfig.asMesSyActionPath.Length()<=0)
    {
         ShowMyMessage("請輸入Action開批下載資料.");
         return;
    }

    if(IniConfig.asMesSyACodePath.Length()<=0)
    {
         ShowMyMessage("請輸入ACode開批下載資料.");
         return;
    }

    if(LabeledEditLotNo->Text.Length()<=0)
    {
         ShowMyMessage("請輸入LotNo開批下載資料.");
         return;
    }

    if(IniConfig.sRmsPath.Length()<=0)
    {
         ShowMyMessage("請輸入N05工作檔路徑.");
         return;
    }

    if(IniConfig.asSummaryReportPath.Length()<=0)
    {
         ShowMyMessage("請輸入Summary report path 路徑.");
         return;
    }

    if(IniConfig.asByTimeOEEPath.Length()<=0)
    {
         ShowMyMessage("請輸入By time OEE path 路徑.");
         return;
    }

    if(IniConfig.asUPHReportPath.Length()<=0)
    {
         ShowMyMessage("請輸入UPH report path 路徑.");
         return;
    }
    #endif

    DoInitialLotInfor();

    listbMesFileLog->Items->Clear();
    asURL       =IniConfig.asMesSyURLPath;
    asACode     =IniConfig.asMesSyACodePath;
    asAction    =IniConfig.asMesSyActionPath;
    asItem      =LabeledEditLotNo->Text+":"+IniConfig.SocketHandlerID;
    asURL.cat_printf("?ACode=%s&Action=%s&ItemName=%s", asACode, asAction, asItem);
    #ifdef SOFT_SIMULTE
        if(FileExists("D:\\RMS\\MesFileLog.txt")==false)
        {
            ShowMyMessage("D:\\RMS\\找不到MesFileLog.txt Error!");
            return;
        }
        listbMesFileLog->Items->LoadFromFile("D:\\RMS\\MesFileLog.txt");
        if(listbMesFileLog->Items->Count>0)
        {
            asData="";
            asData=listbMesFileLog->Items->Strings[0];
        }
    #else
        try
        {
            asData=IdHTTPMESSystem->Get(asURL);
        }
        catch(...)
        {
            ShowMyMessage("mes file get error.");
            return;
        }
    #endif

    if(asData.Length()>0)
    {
        if(asData.AnsiPos("Error")==1)                                          //Steven 20221220 : for VTest show error messsage of MES
        {
            ShowMyMessage(asData);
            return;
        }
        else
        {
            listbMesFileLog->Items->Add(asData);
            TrayForm.bVTestNoRTBin=false;                                       //RogerYang 20250728 不可複測Bin相關flag搬到上面
            bNoRTBinFixFlag[0]=false;
            bNoRTBinFixFlag[1]=false;
            bNoRTBinFixFlag[2]=false;
            while(asData.Length()>0)
            {
                if(asData.Pos("|")>=1)
                {
                    if(asData.SubString(1, 1)=="|")
                    {
                        asData.Delete(1, 1);
                    }
                    else
                    {
                        asLotInforTemp=asData.SubString(1, asData.Pos("|")-1);
                        asData.Delete(1, asData.Pos("|"));
                        if(asLotInforTemp.Pos("HandlerDevice:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            LabeledHandlerDevice->Text  =asLotInforTemp;
                            asVATToChangeWorkFileName   =asLotInforTemp;
                            fLotInfo->edDeviceName->Text=asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceSiteSiteGapStop:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            LabeledSiteGap->Text        =asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceTestTemp:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            LabeledTestTemp->Text       =asLotInforTemp;
                            fLotInfo->edTemp->Text      =asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceSoakTime:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            LabeledSoakTime->Text       =asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceContFail:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            LabeledEditContFail->Text   =asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("PCS:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            LabeledEditLotSize->Text    =asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceLeadCount:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            LabeledLeadCount->Text      =asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("FTHandlerDeviceAutoBin1:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtLabeledFTAutoBin1->Text=asLotInforTemp;
                            asFTUnloadBin[0]->CommaText  =asLotInforTemp;
                            bNewBinCheck=true;
                        }
                        else if(asLotInforTemp.Pos("FTHandlerDeviceAutoBin2:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtLabeledFTAutoBin2->Text=asLotInforTemp;
                            asFTUnloadBin[1]->CommaText  =asLotInforTemp;
                            bNewBinCheck=true;
                        }
                        else if(asLotInforTemp.Pos("FTHandlerDeviceAutoBin3:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtLabeledFTAutoBin3->Text=asLotInforTemp;
                            asFTUnloadBin[2]->CommaText  =asLotInforTemp;
                            bNewBinCheck=true;
                        }
                        else if(asLotInforTemp.Pos("FTHandlerDeviceFixBin1:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            LabeledFTFixBin1->Text      =asLotInforTemp;
                            asFTUnloadBin[3]->CommaText =asLotInforTemp;
                            bNewBinCheck=true;
                        }
                        else if(asLotInforTemp.Pos("FTHandlerDeviceFixBin2:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            LabeledFTFixBin2->Text      =asLotInforTemp;
                            asFTUnloadBin[4]->CommaText =asLotInforTemp;
                            bNewBinCheck=true;
                        }
                        else if(asLotInforTemp.Pos("FTHandlerDeviceFixBin3:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            LabeledFTFixBin3->Text      =asLotInforTemp;
                            asFTUnloadBin[5]->CommaText =asLotInforTemp;
                            bNewBinCheck=true;
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceAutoBin1:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            LabeledAutoBin1->Text       =asLotInforTemp;
                            asUnloadBin[0]->CommaText   =asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceAutoBin2:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            LabeledAutoBin2->Text       =asLotInforTemp;
                            asUnloadBin[1]->CommaText   =asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceAutoBin3:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            LabeledAutoBin3->Text       =asLotInforTemp;
                            asUnloadBin[2]->CommaText   =asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceFixBin1:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            LabeledFixBin1->Text        =asLotInforTemp;
                            asUnloadBin[3]->CommaText   =asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceFixBin2:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            LabeledFixBin2->Text        =asLotInforTemp;
                            asUnloadBin[4]->CommaText   =asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceFixBin3:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            LabeledFixBin3->Text        =asLotInforTemp;
                            asUnloadBin[5]->CommaText   =asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceEQP_PassBins:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtDevicePassBins->Text  =asLotInforTemp;
                            asPassUnloadBin->CommaText  =asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceOpenBins:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtHandlerDeviceOpenBins->Text=asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceRetestBins:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtDeviceRetestBins->Text=asLotInforTemp;
                            asFailUnloadBin->CommaText  =asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceForcePerPin:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            LabeledForcePerPin->Text    =asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceLowYield:")==1)    //jou 20231101 : mes check StopYield -> HandlerDeviceLowYield
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtHandlerDeviceLowYield->Text=asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("WipStep:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtProcess->Text         =asLotInforTemp;
                            fLotInfo->cbProcess->Text   =asLotInforTemp;
                            fLotInfo->cbProcess->Enabled=false;
                        }
                        else if(asLotInforTemp.Pos("CustCode:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtCustCode->Text=asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("CustPart:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtCustPart->Text=asLotInforTemp;
                            fLotInfo->edtProcessName->Text=asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("CustLotNum:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtCustLotNum->Text=asLotInforTemp;
                            fLotInfo->edtProduct->Text=asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("LotNum:")==1)
                        {
                            asLotInforTemp.Delete(1,asLotInforTemp.Pos(":"));
                            lbledtInternalLot->Text=asLotInforTemp;
                            fLotInfo->sVTestInternalLot=asLotInforTemp;         //RogerYang 20250809 偉測Summary文件修改
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceATCcheck:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtHandlerDeviceATCcheck->Text=asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceSOTdelay:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtHandlerDeviceSOTdelay->Text=asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceGPIBaddress:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtHandlerDeviceGPIBaddress->Text=asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceTestTempVar:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtHandlerDeviceTestTempVar->Text=asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceSiteCheck:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtHandlerDeviceSiteCheck->Text=asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("HandlerID:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtHandlerID->Text=asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceHotplateXDevision:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtHandlerDeviceHotplateXDevision->Text=asLotInforTemp.SubString(1, asLotInforTemp.Pos(".")+2);
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceHotplateYDevision:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtHandlerDeviceHotplateYDevision->Text=asLotInforTemp.SubString(1, asLotInforTemp.Pos(".")+2);
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceHotplateXStart:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtHandlerDeviceHotplateXStart->Text=asLotInforTemp.SubString(1, asLotInforTemp.Pos(".")+2);
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceHotplateYStart:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtHandlerDeviceHotplateYStart->Text=asLotInforTemp.SubString(1, asLotInforTemp.Pos(".")+2);
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceHotplateXPitch:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtHandlerDeviceHotplateXPitch->Text=asLotInforTemp.SubString(1, asLotInforTemp.Pos(".")+2);
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceHotplateYPitch:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtHandlerDeviceHotplateYPitch->Text=asLotInforTemp.SubString(1, asLotInforTemp.Pos(".")+2);
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceTrayXDevision:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtHandlerDeviceTrayXDevision->Text=asLotInforTemp.SubString(1, asLotInforTemp.Pos(".")+2);
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceTrayYDevision:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtHandlerDeviceTrayYDevision->Text=asLotInforTemp.SubString(1, asLotInforTemp.Pos(".")+2);
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceTrayXStart:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtHandlerDeviceTrayXStart->Text=asLotInforTemp.SubString(1, asLotInforTemp.Pos(".")+2);
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceTrayYStart:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtHandlerDeviceTrayYStart->Text=asLotInforTemp.SubString(1, asLotInforTemp.Pos(".")+2);
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceTrayXPitch:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtHandlerDeviceTrayXPitch->Text=asLotInforTemp.SubString(1, asLotInforTemp.Pos(".")+2);
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceTrayYPitch:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtHandlerDeviceTrayYPitch->Text=asLotInforTemp.SubString(1, asLotInforTemp.Pos(".")+2);
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceSiteLayouts:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtHandlerDeviceSiteLayouts->Text=asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceCleanPadLimit:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtHandlerDeviceCleanPadLimit->Text=asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceInshuttleSensorCheck:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtHandlerDeviceInshuttleSensorCheck->Text=asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("TestTime/Pcs:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtTestTimePcs->Text=asLotInforTemp;

                            asString=lbledtProcess->Text.SubString(0, 3);
                            iPos=lbledtTestTimePcs->Text.Pos(asString);
                            if(iPos>0)
                            {
                                asString=lbledtTestTimePcs->Text.SubString(iPos+4, lbledtTestTimePcs->Text.Length());
                                iPos=asString.Pos("s;")-1;
                                asString=asString.SubString(0, iPos);
                                dTestTime=atof(asString.c_str());
                            }
                            else
                            {
                                if(asLotInforTemp.Pos("s")>0)
                                    asLotInforTemp.Delete(asLotInforTemp.Length(), asLotInforTemp.Pos("s"));
                                dTestTime=atof(asLotInforTemp.c_str());
                            }
                        }
                        else if(asLotInforTemp.Pos("C1:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtC1->Text=asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("TestSites:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtTestSites->Text=asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("IndexTime:")==1)
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtIndexTime->Text=asLotInforTemp;

                            asString=lbledtProcess->Text.SubString(0, 3);
                            iPos=lbledtIndexTime->Text.Pos(asString);
                            if(iPos>0)
                            {
                                asString=lbledtIndexTime->Text.SubString(iPos+4, lbledtIndexTime->Text.Length());
                                iPos=asString.Pos("s;")-1;
                                asString=asString.SubString(0,iPos);
                                dIndexTime=atof(asString.c_str());
                            }
                            else
                            {
                                if(asLotInforTemp.Pos("s")>0)
                                    asLotInforTemp.Delete(asLotInforTemp.Length(), asLotInforTemp.Pos("s"));
                                dIndexTime=atof(asLotInforTemp.c_str());
                            }
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceIcNum:")==1)   //RogerYang 20250821 : 新增良率中的數量比對
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtHandlerDeviceICNum->Text=asLotInforTemp;
                        }
                        else if(asLotInforTemp.Pos("HandlerDeviceNotRepeatableArea:")==1)  //RogerYang 20250717 偉測更改欄位名稱，為了不跟bin別混淆
                        {
                            asLotInforTemp.Delete(1, asLotInforTemp.Pos(":"));
                            lbledtHandlerDeviceNotRepeatableArea->Text=asLotInforTemp;     //RogerYang 20250717 偉測更改欄位名稱，為了不跟bin別混淆
                            TStringList* list = new TStringList();              //20250626 新增判斷資料是否正確，只能為1~3，逗號分隔

                            int iRlt=ValidNoRTBinInfo(asLotInforTemp, list);
                            int idx=0;
                            bool bReturnFlag=false;                             //RogerYang 20250723 Add return flag
                            AnsiString msg="";
                            AnsiString msg2="";
                            switch(iRlt)
                            {
                                case 0:
                                    for(int i=0; i<list->Count; i++)
                                    {
                                        idx=list->Strings[i].ToIntDef(1);       //RogerYang 20250723 修正idx錯誤
                                        bNoRTBinFixFlag[idx-1]=true;            //RogerYang 20250723 修正idx錯誤
                                        if(TrayForm.asNoRTBinFix[idx-1]=="")    //RogerYang 20250723 修正idx錯誤
                                        {
                                            msg.sprintf("Fix%d",idx);
                                            if(msg2=="")
                                                msg2=msg;
                                            else
                                                msg2+=" ,"+msg;
                                        }
                                        }

                                    if(msg2!="")
                                    {
                                        msg2+=" 未綁定Tray ID資料，請至料盤指定頁面設定!";
                                        ShowMyMessage(msg2);
                                        bReturnFlag=true;
                                    }

                                    if(bNoRTBinFixFlag[0]==true  ||
                                        bNoRTBinFixFlag[1]==true ||
                                        bNoRTBinFixFlag[2]==true)
                                    {
                                        TrayForm.bVTestNoRTBin=true;            //RogerYang 20250814 : 偉測不可複測bin功能
                                        S=GetLastOpenFN();
                                        szDir.sprintf("%s%s", DataPath, S);
                                        szDir+="\\Tray.Data";
                                        WriteIniData(szDir, "VTest", "NoRTBin",   TrayForm.bVTestNoRTBin);
                                    }
                                    break;
                                case 1:
                                    msg.sprintf("(MesFileLog)不可複測Bin別不合法!,(%s)", asLotInforTemp);
                                    ShowMyMessage(msg);
                                    bReturnFlag=true;
                                    break;
                                case 2:
                                    break;
                            }
                            list->Clear();
                            delete list;

                            if(bReturnFlag==true)
                                return;
                        }
                    }
                }
                else
                {
                    asData="";
                }
            }
        }
    }

    asSavePath.sprintf("D:\\HT9045_Log\\Mes_file\\%s", Now().FormatString("yyyy_mm_dd").c_str());
    if(b_Check_Dir_Exist_And_Creak_Dir(asSavePath)==false)
    {
        ShowMyMessage("Load Lot Information Create Mes Dir Error!");
        return;
    }

    if(listbMesFileLog->Items->Count>0)
    {
        asSavePath.cat_printf("\\MesFileLog_%s.txt", Now().FormatString("yyyymmdd_hhnnss"));
        listbMesFileLog->Items->SaveToFile(asSavePath.c_str());
    }

    if(IniConfig.bEnableFTP)
    {
        if(SystemStart || HasICUnderMachine())                                  //Steven 20110503
        {
            bVATToChangeWorkFile=false;

            if(LastSet.bHasDownloadFile==true)
                bVATToChangeWorkFile=true;
        }
        else
        {
            if(HasAutoICInMachine())                                            //jou 20211129 : 修正 VTEST Tray end 結束後重開程式會重置 Auto Tray盤造成疊料。
            {
                LastSet.bHasDownloadFile=true;
                bVATToChangeWorkFile    =true;
                fFTPClient->bError      =false;
            }
            else
            {
                fFTPClient->bControlBySECSGEM  =true;
                fFTPClient->aSetUpNameBySECSGEM=asVATToChangeWorkFileName;
                fFTPClient->ShowFTPModal(0);
            }

            if(fFTPClient->bError==false)
            {
                LastSet.bHasDownloadFile=true;
                bVATToChangeWorkFile    =true;
                fMain->ModifyTester(ON_LINE);                                   //Steven 20191218 : 整合修改LastSet.iTester
                NewRecordProcess("MES2157", "Change to On_Line", "by LotInfo Download Button"); //ChungHung 20140722 add add record
                fMain->UpdateMainOperateMode();
                fMain->CloseGpibProgram(__FUNC__);
                fMain->LoadTestModePicture();

                LastSet.iRealDummy=REALLY;

                if(fLotInfo->cbRunMode->Text.Pos("FT")>0 || fLotInfo->cbRunMode->Text.Pos("EQC")>0)
                    SetRunStartMode(rsmInitialStart);
                else
                    SetRunStartMode(rsmCInitialRetest);

                fMain->LoadRunModePicture();
                fMain->UpdateMainOperateMode();
            }
            else
            {
                ShowMyMessage("請檢查工作檔檔案或路徑設定.");
                return;
            }
        }
    }
    else if(IniConfig.bEnableRms)
    {
        fLotInfo->btDownload->Click();
        if(LastSet.bHasDownloadFile==true)
        {
            bVATToChangeWorkFile=true;
        }
        else
        {
            ShowMyMessage("請檢查工作檔檔案或路徑設定.");
            return;
        }
    }

    if(fMain->cbSetupFileName->Text==asVATToChangeWorkFileName &&
       bVATToChangeWorkFile==true)
    {
        bDownloadLotInforFlag=true;

        if(HasAutoICInMachine())
        {
            if(fLotInfo->cbRunMode->Text.Pos("RT")>0)
                SetRunStartMode(rsmContinuRetest);
            else
                SetRunStartMode(rsmContinuStart);
        }
        else if(LastSet.iRunStartMode==rsmAutoSiteMap)                          //jou 20211215 : 修正重啟程式後 Auto site mapping完 RT 會切換成 FT模式
        {
            if(fLotInfo->cbRunMode->Text.Pos("RT")>0)
                iAutoSiteMapRunStartMode=1;
            else
                iAutoSiteMapRunStartMode=0;
        }

        if(CheckLotInfor())
        {
            if(IniConfig.bEnable_SECS_GEM==false)                               //RogerYang 20260625 : 陳永恆說SECS不要跳
                ShowMyMessage("下載完成.");
        }
        else
        {
            bDownloadLotInforFlag=false;
        }
    }
    else
    {
        bDownloadLotInforFlag=false;
        ShowMyMessage("自動下載開批訊息切換工作檔失敗.", asVATToChangeWorkFileName);
    }
}
//------------------------------------------------------------------------------
void __fastcall TfMesSystem::DoEnableLotInforForMesFile(bool bFlag)
{
    LabeledEditLotNo->Enabled=bFlag;
    LabeledHandlerDevice->Enabled=bFlag;
    LabeledTestTemp->Enabled=bFlag;
    LabeledSoakTime->Enabled=bFlag;
    LabeledEditLotSize->Enabled=bFlag;
    LabeledEditOPID->Enabled=bFlag;
    buttonDownloadLotInfor->Enabled=bFlag;
    fConfiguration->cbN05_CheckFile->Enabled=bFlag;
    LabeledEditContFail->Enabled=bFlag;
    LabeledLeadCount->Enabled=bFlag;
    lbledtLabeledFTAutoBin1->Enabled=bFlag;
    lbledtLabeledFTAutoBin2->Enabled=bFlag;
    lbledtLabeledFTAutoBin3->Enabled=bFlag;
    LabeledFTFixBin1->Enabled=bFlag;
    LabeledFTFixBin2->Enabled=bFlag;
    LabeledFTFixBin3->Enabled=bFlag;
    LabeledAutoBin1->Enabled=bFlag;
    LabeledAutoBin2->Enabled=bFlag;
    LabeledAutoBin3->Enabled=bFlag;
    LabeledFixBin1->Enabled=bFlag;
    LabeledFixBin2->Enabled=bFlag;
    LabeledFixBin3->Enabled=bFlag;
    LabeledSiteGap->Enabled=bFlag;
    LabeledForcePerPin->Enabled=bFlag;
    lbledtHandlerDeviceLowYield->Enabled=bFlag;
    lbledtProcess->Enabled=bFlag;
    lbledtDevicePassBins->Enabled=bFlag;
    lbledtHandlerDeviceOpenBins->Enabled=bFlag;
    lbledtDeviceRetestBins->Enabled=bFlag;
    lbledtHandlerDeviceATCcheck->Enabled=bFlag;
    lbledtHandlerDeviceSOTdelay->Enabled=bFlag;
    lbledtHandlerDeviceGPIBaddress->Enabled=bFlag;
    lbledtHandlerDeviceTestTempVar->Enabled=bFlag;
    lbledtHandlerDeviceSiteCheck->Enabled=bFlag;
    lbledtHandlerID->Enabled=bFlag;
    lbledtHandlerDeviceHotplateXDevision->Enabled=bFlag;
    lbledtHandlerDeviceHotplateYDevision->Enabled=bFlag;
    lbledtHandlerDeviceHotplateXStart->Enabled=bFlag;
    lbledtHandlerDeviceHotplateYStart->Enabled=bFlag;
    lbledtHandlerDeviceHotplateXPitch->Enabled=bFlag;
    lbledtHandlerDeviceHotplateYPitch->Enabled=bFlag;
    lbledtHandlerDeviceTrayXDevision->Enabled=bFlag;
    lbledtHandlerDeviceTrayYDevision->Enabled=bFlag;
    lbledtHandlerDeviceTrayXStart->Enabled=bFlag;
    lbledtHandlerDeviceTrayYStart->Enabled=bFlag;
    lbledtHandlerDeviceTrayXPitch->Enabled=bFlag;
    lbledtHandlerDeviceTrayYPitch->Enabled=bFlag;
    lbledtHandlerDeviceSiteLayouts->Enabled=bFlag;
    lbledtHandlerDeviceCleanPadLimit->Enabled=bFlag;
    lbledtHandlerDeviceInshuttleSensorCheck->Enabled=bFlag;
    lbledtCustCode->Enabled=bFlag;
    lbledtCustPart->Enabled=bFlag;
    lbledtInternalLot->Enabled=bFlag;
    lbledtCustLotNum->Enabled=bFlag;
    lbledtTestTimePcs->Enabled=bFlag;
    lbledtC1->Enabled=bFlag;
    lbledtTestSites->Enabled=bFlag;
    lbledtIndexTime->Enabled=bFlag;
    lbledtHandlerDeviceICNum->Enabled=bFlag;                                    //RogerYang 20250821 : 新增良率中的數量比對
    lbledtHandlerDeviceNotRepeatableArea->Enabled=bFlag;                        //RogerYang 20250717 偉測不可複測Bin功能
}
//------------------------------------------------------------------------------
bool __fastcall TfMesSystem::CheckLotInfor()
{
    bool bPassFlag=false;
    int iUnloaderCT[TEST_MAX_BIN];
    bool bNeedCheck=false;
    double dA=-999.0, dB=-999.0, dC=-999.0;
    AnsiString asString;
    AnsiString sConfigPath=AuthPath+"Security_new.def";
    ZeroMemory(iUnloaderCT, sizeof(iUnloaderCT));
    int iTmp=-999;

    if(IniConfig.bVTESTFunction==true &&
       LastSet.iTester==ON_LINE &&
       IniConfig.bCheckFile)
    {
        if(bDownloadLotInforFlag==false)
        {
            ShowMyMessage("請下載開批資訊.");
            return false;
        }

        if(dIndexTime<=0.0)
        {
            dIndexTime=0.8;
        }

        if(CheckVTENGmode(fLotInfo->edtSysLotID->Text)==true)
        {
            return true;
        }

//        ShowMyMessage("In arm vaccum delay與開批資訊vaccum delay不同,請檢查.");
//        return false;

        if(fLotInfo->cbRunMode->Text.Pos("RT")>0)
        {
//            if(iRunStartMode!=RT)
//            {
//                ShowMyMessage("Run RT Start mode error!!");
//                return false;
//            }
        }
        else
        {
//            if(iRunStartMode!=FT)
//            {
//                ShowMyMessage("Run FT Start mode error!!");
//                return false;
//            }

            bNeedCheck=true;                                                    //RogerYang 20250314 偉測張冬冬要求 Stop Yield和Consecutive Fail功能修改為灰色,不可更改,默認開啟

            if(bNeedCheck==true)
            {
                if((unsigned int)LabeledEditContFail->Text.ToIntDef(-999)!=TestIF_File.iContsFailSocketAlarmCT ||
                   LabeledEditContFail->Text.ToIntDef(-999)==-999)
                {
                    ShowMyMessage("ContFail與開批資訊ContFail不同,請檢查.");
                    return false;
                }

                if((unsigned int)LabeledEditContFail->Text.ToIntDef(-999)>0)
                {
                    if(TestIF_File.bContsFailBySocket==false)
                    {
                        ShowMyMessage("ContFail On 未開啟,請檢查.");
                        return false;
                    }
                }
            }

            if(LabeledLeadCount->Text.ToIntDef(-999)!=DeviceForm_File.iPinCT ||
               LabeledLeadCount->Text.ToIntDef(-999)==-999)
            {
                ShowMyMessage("Lead Count與開批資訊Lead Count不同,請檢查.");
                return false;
            }

            dA=-999.0;                                                          //RogerYang 20250804 修正浮點數與整數比較
            if(LabeledSiteGap->Text.Length()>0)
                dA=atof(LabeledSiteGap->Text.c_str());
            dB=Prod.dFailAlarmSiteYieldCmp;
            dC=Prod.dFailAlarmSiteYield;                                        //RogerYang 20260125 : site gap 比對by arm和by site兩項，新增by arm
            if(cMathTool().IsDoubleEqual(dA,dB)==false ||
                cMathTool().IsDoubleEqual(dA,dC)==false)
            {
                asString.sprintf("開批資訊Site gap(%f)與\r\nSite gap(By Site)(%f) / Site gap(By Arm)(%f) 不同,請檢查良率設定.", dB, dA, dC);   //RogerYang 20250725 顯示數值
                ShowMyMessage(asString);
                return false;
            }

            iTmp=lbledtHandlerDeviceICNum->Text==""?0:lbledtHandlerDeviceICNum->Text.ToIntDef(-999);
            if(iTmp!=0 &&
               (iTmp!=Prod.iLowYieldCount                    ||                 //RogerYang 20250821 : 新增良率中的數量比對
                iTmp!=Prod.iFailAlarmSiteYieldDifferentCount ||
                iTmp!=Prod.iFailAlarmSiteYieldCmpCount))
            {
                asString.sprintf("HandlerDeviceICNum(%d) 與\r\nLow Yield(%d) / Site Diff Yield(%d) / Site Comp Yield(%d) 不同,\r\n請檢查良率設定."
                    , iTmp, Prod.iLowYieldCount, Prod.iFailAlarmSiteYieldDifferentCount, Prod.iFailAlarmSiteYieldCmpCount);
                ShowMyMessage(asString, "Please check yield count.");
                return false;
            }

            if(LabeledTestTemp->Text.ToIntDef(-999)>25)
            {
                if(LastSet.iTemperature==Tempture_Ambient)
                {
                    ShowMyMessage("開批資訊高溫模式錯誤,請檢查.");
                    return false;
                }
            }

            if(LastSet.iTemperature!=Tempture_Ambient)
            {
                if(iTempLimitation==tTemp150)
                {
                    if(LabeledTestTemp->Text.ToIntDef(-999)<25 ||
                       LabeledTestTemp->Text.ToIntDef(-999)>150)
                    {
                        ShowMyMessage(AnsiString().sprintf("開批資訊溫度錯誤,請檢查. Max Temp=%0.2f, Min Temp=%0.2f",150.0,25.0));
                        return false;
                    }
                }
                else if(iTempLimitation==tTemp155)
                {
                    if(LabeledTestTemp->Text.ToIntDef(-999)<25 ||
                       LabeledTestTemp->Text.ToIntDef(-999)>155)
                    {
                        ShowMyMessage(AnsiString().sprintf("開批資訊溫度錯誤,請檢查. Max Temp=%0.2f, Min Temp=%0.2f",155.0,25.0));
                        return false;
                    }
                }
                else if(iTempLimitation==tTemp175)
                {
                    if(LabeledTestTemp->Text.ToIntDef(-999)<25 ||
                       LabeledTestTemp->Text.ToIntDef(-999)>175)
                    {
                        ShowMyMessage(AnsiString().sprintf("開批資訊溫度錯誤,請檢查. Max Temp=%0.2f, Min Temp=%0.2f",175.0,25.0));
                        return false;
                    }
                }
                else if(iTempLimitation==tTemp200)
                {
                    if(LabeledTestTemp->Text.ToIntDef(-999)<25 ||
                       LabeledTestTemp->Text.ToIntDef(-999)>200)
                    {
                        ShowMyMessage(AnsiString().sprintf("開批資訊溫度錯誤,請檢查. Max Temp=%0.2f, Min Temp=%0.2f",200.0,25.0));
                        return false;
                    }
                }
                else
                {
                    if(LabeledTestTemp->Text.ToIntDef(-999)<25 ||
                       LabeledTestTemp->Text.ToIntDef(-999)>130)
                    {
                        ShowMyMessage(AnsiString().sprintf("開批資訊溫度錯誤,請檢查. Max Temp=%0.2f, Min Temp=%0.2f",130.0,25.0));
                        return false;
                    }
                }

                if(Temperature.fWorkTemperBase!=LabeledTestTemp->Text.ToIntDef(-999))
                {
                    ShowMyMessage("測試溫度與開批資訊溫度不同,請檢查.");
                    return false;
                }

                if(Temperature.fSoakTime!=LabeledSoakTime->Text.ToIntDef(-999))
                {
                    ShowMyMessage("Soak time與開批資訊Soak time不同,請檢查.");
                    return false;
                }
            }
        }

//        for(int i=0; i<iTestBinCount; i++)                                    //RogerYang 20251017 : 移到下面
//        {
//            if(BinSelect[iTestRunMode].iCatDataT3Pos[i]>0 &&
//               BinSelect[iTestRunMode].iCatDataT3Pos[i]<7)
//                iUnloaderCT[BinSelect[iTestRunMode].iCatDataT3Pos[i]-1]++;
//        }

        if(bNewBinCheck==true && CUSTOMER_CODE==CC_VTEST_Shanghai)
        {
            if(fLotInfo->cbRunMode->Text.Pos("RT")>0)
            {
                for(int i=0; i<iTestBinCount; i++)                              //RogerYang 20251017 : 修正上海FT/RT設定Bin別不同下載比對失敗
                {
                    if(BinSelect[RT].iCatDataT3Pos[i]>0 &&
                       BinSelect[RT].iCatDataT3Pos[i]<7)
                        iUnloaderCT[BinSelect[RT].iCatDataT3Pos[i]-1]++;
                }
                for(int i=0; i<eTrayCount; i++)
                {
                    for(int j=0; j<asUnloadBin[i]->Count; j++)
                    {
                        if((i+1)!=BinSelect[RT].iCatDataT3Pos[atoi(asUnloadBin[i]->Strings[j].c_str())])
                        {
                            ShowMyMessage("RT Bin與開批資訊Bin不同,請檢查.");
                            return false;
                        }
                    }

                    if(iUnloaderCT[i]!=asUnloadBin[i]->Count)
                    {
                        ShowMyMessage("RT Bin與開批資訊Bin不同,請檢查.");
                        return false;
                    }
                }
            }
            else
            {
                for(int i=0; i<iTestBinCount; i++)                              //RogerYang 20251017 : 修正上海FT/RT設定Bin別不同下載比對失敗
                {
                    if(BinSelect[FT].iCatDataT3Pos[i]>0 &&
                       BinSelect[FT].iCatDataT3Pos[i]<7)
                        iUnloaderCT[BinSelect[FT].iCatDataT3Pos[i]-1]++;
                }
                for(int i=0; i<eTrayCount; i++)
                {
                    for(int j=0; j<asFTUnloadBin[i]->Count; j++)
                    {
                        if((i+1)!=BinSelect[FT].iCatDataT3Pos[atoi(asFTUnloadBin[i]->Strings[j].c_str())])
                        {
                            ShowMyMessage("FT Bin與開批資訊Bin不同,請檢查.");
                            return false;
                        }
                    }

                    if(iUnloaderCT[i]!=asFTUnloadBin[i]->Count)
                    {
                        ShowMyMessage("FT Bin與開批資訊Bin不同,請檢查.");
                        return false;
                    }
                }
            }
        }
        else
        {
            for(int i=0; i<iTestBinCount; i++)                                    //RogerYang 20251017 : 移到下面
            {
                if(BinSelect[iTestRunMode].iCatDataT3Pos[i]>0 &&
                   BinSelect[iTestRunMode].iCatDataT3Pos[i]<7)
                    iUnloaderCT[BinSelect[iTestRunMode].iCatDataT3Pos[i]-1]++;
            }
            for(int i=0; i<eTrayCount; i++)
            {
                for(int j=0; j<asUnloadBin[i]->Count; j++)
                {
                    if((i+1)!=BinSelect[iTestRunMode].iCatDataT3Pos[atoi(asUnloadBin[i]->Strings[j].c_str())])
                    {
                        ShowMyMessage("Bin與開批資訊Bin不同,請檢查.");
                        return false;
                    }
                }

                if(iUnloaderCT[i]!=asUnloadBin[i]->Count)
                {
                    ShowMyMessage("Bin與開批資訊Bin不同,請檢查.");
                    return false;
                }
            }
        }

        for(int i=0; i<asPassUnloadBin->Count; i++)                             //jou 20230131 : 增加HandlerDevicePassBins & HandlerDeviceRetestBins 資訊比對
        {
            if(BinSelect[iTestRunMode].iStackDefFailCate[BinSelect[iTestRunMode].iCatDataT3Pos[atoi(asPassUnloadBin->Strings[i].c_str())]-1]!=0)
            {
                ShowMyMessage("Pass Bin與開批資訊HandlerDevicePassBins不同,請檢查.");
                return false;
            }
        }

        for(int i=0; i<asFailUnloadBin->Count; i++)
        {
            if(BinSelect[iTestRunMode].iStackDefFailCate[BinSelect[iTestRunMode].iCatDataT3Pos[atoi(asFailUnloadBin->Strings[i].c_str())]-1]!=1)
            {
                ShowMyMessage("Fail Bin與開批資訊HandlerDeviceRetestBins不同,請檢查.");
                return false;
            }
        }

        if(TestIF_File.iASMPassBin!=-1)
        {
            bPassFlag=false;
            for(int i=0; i<asPassUnloadBin->Count; i++)
            {
                if(asPassUnloadBin->Strings[i].ToIntDef(-999)==TestIF_File.iASMPassBin)
                {
                    bPassFlag=true;
                }
            }

            if(bPassFlag==false)
            {
                ShowMyMessage("Auto Site Map Pass Bin與開批資訊HandlerDevicePassBins不同,請檢查.");
                return false;
            }
        }

        if(lbledtHandlerDeviceOpenBins->Text.ToIntDef(-999)!=TestIF_File.iOpenBin ||
           lbledtHandlerDeviceOpenBins->Text.ToIntDef(-999)==-999)
        {
            ShowMyMessage("Auto Site Map Open Bin與開批資訊HandlerDeviceOpenBins不同,請檢查.");
            return false;
        }

//        if(LabeledForcePerPin->Text.ToIntDef(-999)!=DeviceForm_File.ForcePerPinG ||
//           LabeledForcePerPin->Text.ToIntDef(-999)==-999)
          dA=-999.0;                                                            //RogerYang 20250731 改支援浮點數
          if(LabeledForcePerPin->Text.Length()>0)
              dA=LabeledForcePerPin->Text.ToDouble();
          dB=DeviceForm_File.ForcePerPinG;
          if(dA!=dB)
        {
            ShowMyMessage("contact force per pin與開批資訊force per pin不同,請檢查.");
            return false;
        }

        bNeedCheck=true;                                                        //RogerYang 20250314 偉測張冬冬要求 Stop Yield和Consecutive Fail功能修改為灰色,不可更改,默認開啟

        if(bNeedCheck)
        {
            if(atof(lbledtHandlerDeviceLowYield->Text.c_str())!=TestIF_File.dLowYieldLimit)
            {
                ShowMyMessage("Low Yields與開批資訊HandlerDeviceLowYield不同,請檢查.");
                return false;
            }

            if(atof(lbledtHandlerDeviceLowYield->Text.c_str())>0)
            {
                if(TestIF_File.bFailAlarmLowYield==false)
                {
                    ShowMyMessage("Low Yields Enable 未開啟,請檢查.");
                    return false;
                }
            }
        }

        if(lbledtHandlerDeviceATCcheck->Text.Length()>0)
        {
            if(lbledtHandlerDeviceATCcheck->Text.Pos("On"))
            {
                if(Temperature.bATCActiveCooling==false)
                {
                    ShowMyMessage("Temp.頁面 ATC 功能未開啟,請檢查.");
                    return false;
                }
            }
            else if(lbledtHandlerDeviceATCcheck->Text.Pos("Off"))
            {
                if(Temperature.bATCActiveCooling==true)
                {
                    ShowMyMessage("Temp.頁面 ATC 功能未關閉,請檢查.");
                    return false;
                }
            }
        }

        if(lbledtHandlerDeviceSiteCheck->Text.Length()>0)
        {
            if(lbledtHandlerDeviceSiteCheck->Text.Pos("On"))
            {
                if(IniConfig.bI21EnableASM==false)
                {
                    ShowMyMessage("config頁面 [I21]Auto Site Mapping 功能未開啟,請檢查.");
                    return false;
                }

                if(TestIF_File.bAutoSiteMappingOpenSite==false)
                {
                    ShowMyMessage("Setup頁面 Auto Site Mapping Open Site 功能未開啟,請檢查.");
                    return false;
                }

                if(TestIF_File.bAutoSiteMappingOneCycle==false)
                {
                    ShowMyMessage("Setup頁面 Auto Site Mapping One cycle 功能未開啟,請檢查.");
                    return false;
                }
            }
        }

        if(atof(lbledtHandlerDeviceSOTdelay->Text.c_str())>0.0)
        {
            if(atof(lbledtHandlerDeviceSOTdelay->Text.c_str())!=TestIF_File.dStartDelayTime)
            {
                ShowMyMessage("Start delay time 與開批資訊 HandlerDeviceSOTdelay 不同,請檢查.");
                return false;
            }
        }

        if(lbledtHandlerDeviceGPIBaddress->Text.Length()>0)
        {
            if(lbledtHandlerDeviceGPIBaddress->Text.ToIntDef(-999)!=TestIF_File.iGpibAddress)
            {
                ShowMyMessage("GPIB address 與開批資訊 HandlerDeviceGPIBaddress 不同,請檢查.");
                return false;
            }
        }

        if(lbledtHandlerDeviceTestTempVar->Text.ToIntDef(-999)!=IniConfig.iL04TemptureRange ||
           lbledtHandlerDeviceTestTempVar->Text.ToIntDef(-999)==-999)
        {
            ShowMyMessage("[L04]Temperature range 與開批資訊 HandlerDeviceTestTempVar 不同,請檢查.");
            return false;
        }

        if(fLotInfo->cbRunMode->Text.Pos("EQC")==0)
        {
            if(lbledtHandlerID->Text!=IniConfig.SocketHandlerID)
            {
                ShowMyMessage("config [N04]Machine ID 與開批資訊 HandlerID 不同,請檢查.");
                return false;
            }
        }

        if(LastSet.iTemperature==Tempture_Hot &&
           HotPlateForm_File.bTrayHotplateCheck==true)
        {
            //Hotplate
            if(atoi(lbledtHandlerDeviceHotplateXDevision->Text.c_str())!=HotPlateForm_File.XDivision)
            {
                ShowMyMessage("Hotplate X division 與開批資訊 HandlerDeviceHotplateXDevision 不同,請檢查.");
                return false;
            }

            if(atoi(lbledtHandlerDeviceHotplateYDevision->Text.c_str())!=HotPlateForm_File.YDivision)
            {
                ShowMyMessage("Hotplate Y division 與開批資訊 HandlerDeviceHotplateYDevision 不同,請檢查.");
                return false;
            }

            dA=-999.0;
            if(lbledtHandlerDeviceHotplateXStart->Text.Length()>0)
                dA=lbledtHandlerDeviceHotplateXStart->Text.ToDouble();
            dB=HotPlateForm_File.XStart;
            if(dA!=dB)
            {
                asString.sprintf("Hotplate X start:%f 與開批資訊 HandlerDeviceHotplateXStart:%f 不同,請檢查.", dB, dA);
                ShowMyMessage(asString);
                return false;
            }

            dA=-999.0;
            if(lbledtHandlerDeviceHotplateYStart->Text.Length()>0)
                dA=lbledtHandlerDeviceHotplateYStart->Text.ToDouble();
            dB=HotPlateForm_File.YStart;
            if(dA!=dB)
            {
                asString.sprintf("Hotplate Y start:%f 與開批資訊 HandlerDeviceHotplateYStart:%f 不同,請檢查.", dB, dA);
                ShowMyMessage(asString);
                return false;
            }

            dA=-999.0;
            if(lbledtHandlerDeviceHotplateXPitch->Text.Length()>0)
                dA=lbledtHandlerDeviceHotplateXPitch->Text.ToDouble();
            dB=HotPlateForm_File.XPitch;
            if(dA!=dB)
            {
                asString.sprintf("Hotplate X pitch:%f 與開批資訊 HandlerDeviceHotplateXPitch:%f 不同,請檢查.", dB, dA);
                ShowMyMessage(asString);
                return false;
            }

            dA=-999.0;
            if(lbledtHandlerDeviceHotplateYPitch->Text.Length()>0)
                dA=lbledtHandlerDeviceHotplateYPitch->Text.ToDouble();
            dB=HotPlateForm_File.YPitch;
            if(dA!=dB)
            {
                asString.sprintf("Hotplate Y pitch:%f 與開批資訊 HandlerDeviceHotplateYPitch:%f 不同,請檢查.", dB, dA);
                ShowMyMessage(asString);
                return false;
            }
        }

        if(HotPlateForm_File.bTrayHotplateCheck==true)
        {
            //Tray
            if(atoi(lbledtHandlerDeviceTrayXDevision->Text.c_str())!=TrayForm.Loader.XDivision)
            {
                ShowMyMessage("Tray X division 與開批資訊 HandlerDeviceTrayXDevision 不同,請檢查.");
                return false;
            }

            if(atoi(lbledtHandlerDeviceTrayYDevision->Text.c_str())!=TrayForm.Loader.YDivision)
            {
                ShowMyMessage("Tray Y division 與開批資訊 HandlerDeviceTrayYDevision 不同,請檢查.");
                return false;
            }

            dA=-999.9;
            if(lbledtHandlerDeviceTrayXStart->Text.Length()>0)
                dA=lbledtHandlerDeviceTrayXStart->Text.ToDouble();
            dB=UserDefForm_File[0].XStart;
            if(dA!=dB)
            {
                asString.sprintf("Tray X start:%f 與開批資訊 HandlerDeviceTrayXStart:%f 不同,請檢查.", dB, dA);
                ShowMyMessage(asString);
                return false;
            }

            dA=-999.9;
            if(lbledtHandlerDeviceTrayYStart->Text.Length()>0)
                dA=lbledtHandlerDeviceTrayYStart->Text.ToDouble();
            dB=UserDefForm_File[0].YStart;
            if(dA!=dB)
            {
                asString.sprintf("Tray Y start:%f 與開批資訊 HandlerDeviceTrayYStart:%f 不同,請檢查.", dB, dA);
                ShowMyMessage(asString);
                return false;
            }

            dA=-999.9;
            if(lbledtHandlerDeviceTrayXPitch->Text.Length()>0)
                dA=lbledtHandlerDeviceTrayXPitch->Text.ToDouble();
            dB=UserDefForm_File[0].XPitch;
            if(dA!=dB)
            {
                asString.sprintf("Tray X pitch:%f 與開批資訊 HandlerDeviceTrayXPitch:%f 不同,請檢查.", dB, dA);
                ShowMyMessage(asString);
                return false;
            }

            dA=-999.9;
            if(lbledtHandlerDeviceTrayYPitch->Text.Length()>0)
                dA=lbledtHandlerDeviceTrayYPitch->Text.ToDouble();
            dB=UserDefForm_File[0].YPitch;
            if(dA!=dB)
            {
                asString.sprintf("Tray Y pitch:%f 與開批資訊 HandlerDeviceTrayYPitch:%f 不同,請檢查.", dB, dA);
                ShowMyMessage(asString);
                return false;
            }
        }

        if(lbledtHandlerDeviceSiteLayouts->Text.Length()>0)
        {
            if(TestSocket.iShtRow>2 || TestSocket.iShtCol>4)                    //SL1:8;SL2:7;SL3:6;SL4:5;SL5:4;SL6:3;SL7:2;SL8:1;
            {
                ShowMyMessage("HandlerDeviceSiteLayouts 不支援此模式.");
                return false;
            }

            for(int i=0; i<TestSocket.iShtRow; i++)
            {
                for(int j=0; j<TestSocket.iShtCol; j++)
                {
                    asString.sprintf("SL%d:", i*4+j+1);
                    iSiteLayouts[i][j]=atoi(lbledtHandlerDeviceSiteLayouts->Text.SubString(lbledtHandlerDeviceSiteLayouts->Text.Pos(asString)+4, 1).c_str());

                    if(iSiteLayouts[i][j]!=TestIF_File.iSiteMap[i][j])
                    {
                        ShowMyMessage("Site Layouts 與開批資訊 HandlerDeviceSiteLayouts 不同,請檢查.");
                        return false;
                    }
                }
            }
        }

#ifdef HiSilicon
        if(lbledtHandlerDeviceCleanPadLimit->Text.Length()>0)
        {
            if(lbledtHandlerDeviceCleanPadLimit->Text.ToIntDef(-999)!=TestIF_File.iAutoClean_AlarmCount)
            {
                ShowMyMessage("Auto clean alarm count 與開批資訊 HandlerDeviceCleanPadLimit 不同,請檢查.");
                return false;
            }
        }
#endif

        if(lbledtHandlerDeviceInshuttleSensorCheck->Text.Length()>0)
        {
            if(lbledtHandlerDeviceInshuttleSensorCheck->Text.Pos("True"))
            {
                if(IniConfig.bF18InshuttleDetect==false)
                {
                    ShowMyMessage("config頁面 [F18] In shuttle product detect 功能未開啟,請檢查.");
                    return false;
                }
            }
        }

        if(IniConfig.bI21EnableASM==true)
        {
            if(TestIF_File.iOpenBin==-1)
            {
                ShowMyMessage("分類設定頁面 auto site mapping Open bin 未設定錯誤");
                return false;
            }
        }

        if(bFormShowJustInitial==false &&                                       //marvin 20200424 (Kirin) Added always record report by time.
           LastSet.SendCT[0]!=0)
        {
            ShowMyMessage("請先將報表數量清除." );
            return true;
        }
    }
    return true;
}
//------------------------------------------------------------------------------
void __fastcall TfMesSystem::VTestSummaryReport()
{
    AnsiString asPassFail[2]={"pass", "fail"};

    unsigned long ulAllTotalSelBinCT            =0;
    unsigned long ulGetSelBinCT[TEST_MAX_BIN]   ={0};
    unsigned long ulTotalSelBinCT[TEST_MAX_BIN] ={0};
    unsigned long ulAllGetSelBinCT[TEST_MAX_BIN]={0};

    AnsiString S;
    if(b_Check_Dir_Exist_And_Creak_Dir(IniConfig.asSummaryReportPath)==false)
    {
        S.sprintf("目錄不存在 %s",IniConfig.asSummaryReportPath);
        ShowMyMessage(S);
        return;
    }

    TStringList *slReport = new TStringList();
    AnsiString aFileName, aLocalFile;
    int iSites=0;

    slReport->Clear();
    //HHT-234_ZX211221LM_TA408E115_ANFQI13F01.AJ_FT5T0_RP0_2024_04_03_11_32_45
//    aFileName.sprintf("%s_%s_%s_%s_%s_%s_%04d_%02d_%02d_%02d_%02d_%02d.SUM", IniConfig.SocketHandlerID,
//                                                                             fLotInfo->edtProcessName->Text,
//                                                                             fLotInfo->edtProduct->Text,
//                                                                             fLotInfo->edtSysLotID->Text,
//                                                                             fLotInfo->cbProcess->Text,
//                                                                             fLotInfo->cbRunMode->Text,
//                                                                             SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);

    aFileName.sprintf("%s_%s_%s_%s_%s_%s_%04d_%02d_%02d_%02d_%02d_%02d.SUM",    //RogerYang 20250809 偉測Summary文件修改
                                                                             IniConfig.SocketHandlerID,
                                                                             fLotInfo->edtProcessName->Text,
                                                                             fLotInfo->edtProduct->Text,
                                                                             fLotInfo->sVTestInternalLot,
                                                                             fLotInfo->cbProcess->Text,
                                                                             fLotInfo->cbTestTimes->Text,
                                                                             SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);

    aLocalFile = IniConfig.asSummaryReportPath+"\\"+aFileName;

    slReport->Add(" ");

    S.sprintf("Operator ID: %s", fLotInfo->edtSysOperatorID->Text);
    slReport->Add(S);

    S.sprintf("Lot ID: %s", fLotInfo->edtSysLotID->Text);
    slReport->Add(S);

    S.sprintf("Device: %s", fLotInfo->edtProcessName->Text);
    slReport->Add(S);

    S.sprintf("Cust lot: %s", fLotInfo->edtProduct->Text);
    slReport->Add(S);

    S.sprintf("Test Stage: %s", fLotInfo->cbRunMode->Text);
    slReport->Add(S);

    if(LastSet.iRunStartMode==rsmContinuStart ||
       LastSet.iRunStartMode==rsmInitialStart)
        slReport->Add("Test mode: FT");
    else
        slReport->Add("Test mode: RT");

    S.sprintf("TFME NO: %s","null");
    slReport->Add(S);

    if(LastSet.iTemperature==Tempture_Ambient)
        S.sprintf("Temperature: %d", 25);
    else
        S.sprintf("Temperature: %s", FormatFloat("0.0", Temperature.fWorkTemperBase));
    slReport->Add(S);

    S.sprintf("Station: %s", "null");
    slReport->Add(S);

    S.sprintf("ReportAlias: %s", "null");
    slReport->Add(S);

    S.sprintf("Date Code: %s", "null");
    slReport->Add(S);

    S.sprintf("Product ID: %s", "null");
    slReport->Add(S);

    S.sprintf("CP Lot NO: %s", "null");
    slReport->Add(S);

    S.sprintf("ProgramName: %s", "null");
    slReport->Add(S);

    S.sprintf("Lot Start Date: %s", RunInfo.LotStartTime);
    slReport->Add(S);

    S.sprintf("MUBA=%d/%d", LastSet.iJamCount[1], LastSet.SendCT_ART[1]);
    slReport->Add(S);

    S.sprintf("Time OEE:%d/%d", LastSet.SystemAccSecond[0][stStartTime]/1000, LastSet.SystemAccSecond[0][stPowerOn]/1000);
    slReport->Add(S);

    S.sprintf("OEE:%d", 0);
    slReport->Add(S);

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(LastSet.bUseTestSocket[0][i][j]==true ||
               LastSet.bUseTestSocket[1][i][j]==true)                           //jou 20220922 : 修正關arm記數site數量異常
            {
                iSites++;
            }
        }
    }

    S.sprintf("Site Q'ty:%d", iSites);
    slReport->Add(S);

    slReport->Add(" ");
    slReport->Add(" ");
    slReport->Add(" ");
    slReport->Add(" ");
    slReport->Add("********Hard Bin Summary******************");
    slReport->Add("Hard  Bin             Count           Percentage(%)");

    for(int i=eAuto1; i<=iFixRightHalf; i++)
    {
        if(Prod.iTrayType[i]!=tNotUse)
        {
            S.sprintf("H/W %s:            %6d               %s", s6TrayName[i], LastSet.BinCT_ART[0][iTo3Unload[i]], RunInfo.sT6AutoYield_ART[i]);
            slReport->Add(S);
        }
    }

    S.sprintf("Total Q'ty: %d", RunInfo.iUnloadCount_ART);
    slReport->Add(S);
    S.sprintf("Total pass : %d", iSECSGEMPass);
    slReport->Add(S);
    S.sprintf("Total fail : %d", iSECSGEMFail);
    slReport->Add(S);

    if(float(RunInfo.iUnloadCount_ART)<=0)
        S.sprintf("Yield : 0.0%");
    else
        S.sprintf("Yield : %02.2f%",(float(iSECSGEMPass)/float(RunInfo.iUnloadCount_ART))*100.0);
    slReport->Add(S);
    slReport->Add(" ");

    slReport->Add("****************************Summary*********************");

    for(int i=0; i<TEST_MAX_BIN; i++)
    {
        ulGetSelBinCT[i]=0;
        ulTotalSelBinCT[i]=0;
        ulAllGetSelBinCT[i]=0;
    }

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            S.sprintf("*****Site%s Summary  *************************", IndexSuckName[i][j]);
            slReport->Add(S);
            slReport->Add("Software Hardware COUNT   Per(%)  Code        Bin DESCRIP");
            slReport->Add("Category   BIN");

            for(int k=0; k<iTestBinCount; k++)
            {
                ulGetSelBinCT[k]  =ArmData[2]->ArmSKET[i][j]->GetSelBinCT(k);
                ulTotalSelBinCT[k]=ArmData[2]->ArmSKET[i][j]->GetTotal();

                if(ulGetSelBinCT[k]>0)
                {
                    ulAllGetSelBinCT[k]+=ulGetSelBinCT[k];
                    ulAllTotalSelBinCT+=ulGetSelBinCT[k];

                    S.sprintf("BIN%02d      %s  %s    %6d    %s", k,
                                                                  s3TrayName[BinSelect[iTestRunMode].iCatDataT3Pos[k]-1],
                                                                  asPassFail[BinSelect[iTestRunMode].iStackDefFailCate[BinSelect[iTestRunMode].iCatDataT3Pos[k]-1]],
                                                                  ulGetSelBinCT[k],
                                                                  ChangeToPercentage(ulGetSelBinCT[k], ulTotalSelBinCT[k]));
                    slReport->Add(S);
                }
            }
            slReport->Add(" ");
            slReport->Add(" ");
        }
    }

    slReport->Add("****************************Summary*********************");
    slReport->Add("*****Total Site Summary  *************************");
    slReport->Add("Software   Hardware COUNT   Per(%)    Code        Bin DESCRIP");
    slReport->Add("Category   BIN");

    for(int k=0; k<iTestBinCount; k++)
    {
        if(ulAllGetSelBinCT[k]>0)
        {
            S.sprintf("BIN%02d      %s  %s    %6d    %s", k,
                                                          s3TrayName[BinSelect[iTestRunMode].iCatDataT3Pos[k]-1],
                                                          asPassFail[BinSelect[iTestRunMode].iStackDefFailCate[BinSelect[iTestRunMode].iCatDataT3Pos[k]-1]],
                                                          ulAllGetSelBinCT[k],
                                                          ChangeToPercentage(ulAllGetSelBinCT[k], ulAllTotalSelBinCT));
            slReport->Add(S);
        }
    }

    slReport->Add("**************** End ******************");

    slReport->SaveToFile(aLocalFile);
    delete slReport;
}
//===========================================================================
// 函式說明: 替代原本的GetTickCount
// v1.0 HT3000: Kirin 20200205 替代原本的GetTickCount
//===========================================================================
DWORD64 MyGetTickCount64()
{
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return ((double)(now.QuadPart)/(double)(freq.QuadPart))*1000;               //Kirin 20200414 修改支援後三位. return now.QuadPart / freq.QuadPart * 1000;
}

//============================================================================//
// 函式說明:Initail record report by time.                                    //
// v1.00d:marvin 20200424 (Kirin) Added always record report by time.         //
//============================================================================//
//------------------------------------------------------------------------------
void __fastcall TfMesSystem::DoInitailRecordReportByTime()
{
    strcpy(LastSet.cLotStartTimeForAlways, Now().FormatString("yyyy-mm-dd hh:nn:ss").c_str());
    iJamRateTotalForAlways=0;
    for(int i=0; i<4; i++)
    {
        for(int j=eAuto1; j<=iFixRightHalf; j++)
        {
            if(i==2 ||i==3)
                continue;

            LastSet.iBinCTForAlways[i][j]=0;
        }
    }

    for(int i=0; i<MAX_SOCKET_ROW; i++)
    {
        for(int j=0; j<MAX_SOCKET_COL; j++)
        {
            LastSet.iSiteTotalCTForAlways[i][j]=0;

            for(int k=0; k<TEST_MAX_BIN; k++)
                LastSet.iSiteBinCTForAlways[i][j][k]=0;
        }
    }
}
//============================================================================//
// 函式說明:Record report by time.                                            //
// v1.00d:marvin 20200424 (Kirin) Added always record report by time.         //
//============================================================================//
//------------------------------------------------------------------------------
void __fastcall TfMesSystem::DoRecordReportByTime(bool bRecrodOEE)
{
    bool bOpenSite=false;
    AnsiString asReprotData, asReprotRecordTime, asReprotDataBuf, S, S1, asOEE;
    int iTotalTestCT=0, iSiteCount=0, iTotalPassCT=0;
    AnsiString asPassFail[2]={"pass", "fail"};

    if(IniConfig.bCheckFile==false ||
       IniConfig.bVTESTFunction==false)
    {
#ifdef SOFT_SIMULTE
        return ;
#else
        if(LastSet.iTester==OFF_LINE)
            return;
#endif
    }

    for(int i=eAuto1; i<=iFixRightHalf; i++)
    {
        iTotalTestCT+=LastSet.iBinCTForAlways[0][i];                            //[0] : 12 hour
    }

    for(int i=eAuto1; i<=iFixRightHalf; i++)
    {
        if(Prod.iIsPassT6[i]==1)
        {
            iTotalPassCT+=LastSet.iBinCTForAlways[0][i];                        //[0] : 12 hour
        }
    }

    if(iTotalTestCT<=0)
    {
        return;
    }

    asReprotData.sprintf("Operator ID:%s", fLotInfo->edtSysOperatorID->Text);
    stringlRecordReport->Add(asReprotData);

    asReprotData.sprintf("Lot ID:%s", fLotInfo->edtSysLotID->Text);
    stringlRecordReport->Add(asReprotData);

    if(LastSet.iTemperature==Tempture_Ambient)
        S.sprintf("Temperature: %d", 25);
    else
        S.sprintf("Temperature: %s", FormatFloat("0.0", Temperature.fWorkTemperBase));
    stringlRecordReport->Add(S);

    asReprotData.sprintf("Process: %s", fLotInfo->cbProcess->Text);
    stringlRecordReport->Add(asReprotData);

    asReprotData.sprintf("Run Mode: %s", fLotInfo->cbRunMode->Text);
    stringlRecordReport->Add(asReprotData);

    asReprotData.sprintf("Lot Start Date:%s", AnsiString(LastSet.cLotStartTimeForAlways));
    stringlRecordReport->Add(asReprotData);

    asReprotData.sprintf("End Lot Date:%s", Now().FormatString("yyyy-mm-dd hh:nn:ss").c_str());
    stringlRecordReport->Add(asReprotData);

    if(iTotalTestCT>0)
    {
        asReprotData.sprintf("MUBA=%d/%d", iJamRateTotalForAlways, iTotalTestCT);
    }
    else
    {
        asReprotData.sprintf("MUBA=%d/0", iJamRateTotalForAlways);
    }
    stringlRecordReport->Add(asReprotData);                                     //MUBA

    for(int i=0; i<TestSocket.iShtRow; i++)                                     //jou 20221024 : 修正關arm計數site數量異常
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            bOpenSite=false;
            for(int k=0; k<iTestBinCount; k++)
            {
                if(LastSet.iSiteBinCTForAlways[i][j][k]>0)
                {
                    bOpenSite=true;
                }
            }

            if(bOpenSite==true)
                iSiteCount++;
        }
    }

    asReprotData.sprintf("Site Q'ty:%d", iSiteCount);
    stringlRecordReport->Add(asReprotData);
    stringlRecordReport->Add("********Hard Bin Summary******************");
    stringlRecordReport->Add("Hard  Bin             Count           Percentage(%)");

    for(int i=eAuto1; i<=iFixRightHalf; i++)
    {
        if(iTotalTestCT==0 || LastSet.iBinCTForAlways[0][i]==0)
        {
            asReprotData.sprintf("H/W %s:               0               --.--%%", s6TrayName[i],i+1);
        }
        else
        {
            asReprotData.sprintf("H/W %s:           %5d                %5.2f%%",
                                 s6TrayName[i],
                                 LastSet.iBinCTForAlways[0][i],
                                 ChangeToFloat((double)LastSet.iBinCTForAlways[0][i], (double)iTotalTestCT));
        }
        stringlRecordReport->Add(asReprotData);
    }

    asReprotData.sprintf("Total Q'ty:%d", iTotalTestCT);
    stringlRecordReport->Add(asReprotData);

    asReprotData.sprintf("Total pass :%d", iTotalPassCT);
    stringlRecordReport->Add(asReprotData);

    asReprotData.sprintf("Total fail :%d", iTotalTestCT-iTotalPassCT);
    stringlRecordReport->Add(asReprotData);

    asReprotData.sprintf("Yield :%5.2f%%", ChangeToFloat((double)iTotalPassCT, (double)iTotalTestCT));
    stringlRecordReport->Add(asReprotData);

    stringlRecordReport->Add(" ");
    stringlRecordReport->Add("****************************Summary*********************");

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            S.sprintf("*****Site%s Summary  *************************", IndexSuckName[i][j]);
            stringlRecordReport->Add(S);
            stringlRecordReport->Add("Software Hardware COUNT   Per(%)  Code        Bin DESCRIP");
            stringlRecordReport->Add("Category   BIN");

            for(int k=0; k<iTestBinCount; k++)
            {
                if(LastSet.iSiteBinCTForAlways[i][j][k]>0)
                {
                    S.sprintf("BIN%02d      %s  %s    %6d    %s", k, s3TrayName[BinSelect[iTestRunMode].iCatDataT3Pos[k]-1],
                        asPassFail[BinSelect[iTestRunMode].iStackDefFailCate[BinSelect[iTestRunMode].iCatDataT3Pos[k]-1]],
                        LastSet.iSiteBinCTForAlways[i][j][k], ChangeToPercentage( LastSet.iSiteBinCTForAlways[i][j][k], LastSet.iSiteTotalCTForAlways[i][j]));
                    stringlRecordReport->Add(S);
                }
            }
            stringlRecordReport->Add(" ");
            stringlRecordReport->Add(" ");
        }
    }

    asReprotData.sprintf("******************************************************************************************************");
    stringlRecordReport->Add(asReprotData);

    //jou 20211213 : VTEST 8點報表中最後一欄添加一項綠燈率，即12小時內的綠燈時間，百分比呈現
    if(bRecrodOEE==true)
    {
        if(LastSet.SystemAccSecond[1][stPowerOn]==0)
            asReprotData.sprintf("Time OEE:%3.2f%", 0.0);
        else
            asReprotData.sprintf("Time OEE:%3.2f%", double(LastSet.SystemAccSecond[1][stStartTime]/1000)/double(LastSet.SystemAccSecond[1][stPowerOn]/1000)*100.0);
        stringlRecordReport->Add(asReprotData);
        stringlRecordReport->Add(" ");
        LastSet.SystemAccSecond[1][stStartTime]=0;
        LastSet.SystemAccSecond[1][stPowerOn]=0;

        asReprotData.sprintf("Total Pass:%d",iOEETotalPass);
        stringlRecordReport->Add(asReprotData);
        stringlRecordReport->Add(" ");
        asReprotData.sprintf("Total Plan output:%d", iPlanOut12h);
        stringlRecordReport->Add(asReprotData);
        stringlRecordReport->Add(" ");

        if(iPlanOut12h==0)
            asReprotData.sprintf("COEE:%3.2f%", 0.0);
        else
            asReprotData.sprintf("COEE:%3.2f%", (double(iOEETotalPass)/double(iPlanOut12h))*100.0);
        stringlRecordReport->Add(asReprotData);
        stringlRecordReport->Add(" ");
    }

    DoInitailRecordReportByTime();
}
//============================================================================//
// 函式說明:Save report by time.                                            //
// v1.00d:marvin 20200424 (Kirin) Added always record report by time.         //
//============================================================================//
//------------------------------------------------------------------------------
void __fastcall TfMesSystem::DoSaveReportByTime()
{
    AnsiString asSavePath="D:\\HT9045_Log\\ProductReport";

    b_Check_Dir_Exist_And_Creak_Dir(asSavePath);
    DoRecordReportByTime(true);
    if(stringlRecordReport->Count>0)
    {
        if(IniConfig.SocketHandlerID.Length()<=0)
        {
           asSavePath.cat_printf("\\null_%s.sum",Now().FormatString("yyyy_mm_dd_hh_nn_ss").c_str());
        }
        else
        {
            asSavePath.cat_printf("\\%s_%s.sum",IniConfig.SocketHandlerID,Now().FormatString("yyyy_mm_dd_hh_nn_ss").c_str());
        }
        stringlRecordReport->SaveToFile(asSavePath.c_str());
        stringlRecordReport->Clear();
        DeleteStringlRecordReport();
    }
}
//============================================================================//
// 函式說明:解構子.                                                           //
// v1.00c:marvin 20200210 (Kirin) Added mes system.                           //
//============================================================================//
void __fastcall TfMesSystem::SaveStringlRecordReport()
{
    AnsiString sPathName="D:\\HT9045_Log\\DataBackup";                          //RogerYang 20250512 修正不存在資料夾無法正常關閉程式
    MyForceDirectories(sPathName);                                              //RogerYang 20250512 修正不存在資料夾無法正常關閉程式

    if(stringlRecordReport->Count>0)
    {
        stringlRecordReport->Add("******************************************************************************************************");
        stringlRecordReport->Add("*Close program");
        stringlRecordReport->Add("******************************************************************************************************");
        stringlRecordReport->Add("  ");
        stringlRecordReport->SaveToFile(sPathName+"\\stringlRecordReport.sum"); //RogerYang 20250512 修正不存在資料夾無法正常關閉程式
    }

    if(stOEEState12hList->Count>0)
    {
        stOEEState12hList->SaveToFile(sPathName+"\\stOEEState12hList.csv");     //RogerYang 20250812 修正存檔錯誤
    }

    stringlRecordReport->Clear();
    stOEEState12hList->Clear();
}
//------------------------------------------------------------------------------
__fastcall TfMesSystem::~TfMesSystem()
{
    SaveStringlRecordReport();

    if(stringlRecordReport!=NULL)
    {
        delete stringlRecordReport;
    }

    for(int i=0; i<eTrayCount; i++)
    {
        asFTUnloadBin[i]->Clear();
        delete asFTUnloadBin[i];

        asUnloadBin[i]->Clear();
        delete asUnloadBin[i];
    }

    asPassUnloadBin->Clear();
    delete asPassUnloadBin;
    asFailUnloadBin->Clear();
    delete asFailUnloadBin;

    stOEEState->Clear();
    delete stOEEState;
    stOEEState12hList->Clear();
    delete stOEEState12hList;
}
//------------------------------------------------------------------------------
//jou 20210823 : lot start增加保護避免重複執行run mode
int __fastcall TfMesSystem::RunModeRW(bool bRead, AnsiString asLot, AnsiString asRunMode)
{
    int iRet=0;

    if(CheckVTENGmode(fLotInfo->edtSysLotID->Text)==false)
    {
        if(b_Check_Dir_Exist_And_Creak_Dir("D:\\HT9045_Log\\DataBackup\\"))
        {
            TIniFile *INIFile1 = new TIniFile("D:\\HT9045_Log\\DataBackup\\"+asLot+".ini");

            if(bRead==true)
                iRet=INIFile1->ReadInteger("Run mode", asRunMode, 0);
            else
                INIFile1->WriteInteger("Run mode", asRunMode, 1);

            delete INIFile1;
        }
    }
    return iRet;
}
//------------------------------------------------------------------------------
void __fastcall TfMesSystem::LoadStringlRecordReport()
{
    if(FileExists("D:\\HT9045_Log\\DataBackup\\stringlRecordReport.sum"))
    {
        stringlRecordReport->LoadFromFile("D:\\HT9045_Log\\DataBackup\\stringlRecordReport.sum");
        DeleteFile("D:\\HT9045_Log\\DataBackup\\stringlRecordReport.sum");
    }

    if(FileExists("D:\\HT9045_Log\\DataBackup\\stOEEState12hList.csv"))
    {
        stOEEState12hList->LoadFromFile("D:\\HT9045_Log\\DataBackup\\stOEEState12hList.csv");
        DeleteFile("D:\\HT9045_Log\\DataBackup\\stOEEState12hList.csv");
    }
}
//------------------------------------------------------------------------------
void __fastcall TfMesSystem::DeleteStringlRecordReport()
{
    if(FileExists("D:\\HT9045_Log\\DataBackup\\stringlRecordReport.sum"))
    {
        DeleteFile("D:\\HT9045_Log\\DataBackup\\stringlRecordReport.sum");
    }

    if(FileExists("D:\\HT9045_Log\\DataBackup\\stOEEState12hList.csv"))
    {
        DeleteFile("D:\\HT9045_Log\\DataBackup\\stOEEState12hList.csv");
    }
}
//------------------------------------------------------------------------------
void TfMesSystem::CleaOEEState12hList()
{
    stOEEState12hList->Clear();
}
//------------------------------------------------------------------------------
struct MES_JAM_COUNT
{
    int iCount;
    int iStopTime;
    AnsiString JamCode;
    AnsiString Message;
};
//------------------------------------------------------------------------------
bool mes_cmp_by_count(MES_JAM_COUNT a, MES_JAM_COUNT b)
{
    return a.iCount>b.iCount;
}
vector<MES_JAM_COUNT> myMesJamCount;
vector<MES_JAM_COUNT> myMesWarCount;
//------------------------------------------------------------------------------
int __fastcall TfMesSystem::TimeToSec(AnsiString asString)
{
    int iSec=0;
    AnsiString asStr;
    asStr=asString.SubString(0, 2);
    iSec=asStr.ToIntDef(0)*3600;
    asStr=asString.SubString(4, 2);
    iSec+=asStr.ToIntDef(0)*60;
    asStr=asString.SubString(7, 2);
    iSec+=asStr.ToIntDef(0);
    return iSec;
}
//------------------------------------------------------------------------------
//config N10 每小時紀錄一次OEE & 生產時間
void __fastcall TfMesSystem::RecordByTimeOEE(int iHour)
{
    bool bMatch=false;
    int iLastHour=0, iTotalTestCT=0;
    int iJamCount=0, iWarCount=0, iStopTime=0;
    AnsiString str, asFileName, asLocalFile, asDate, asLastDateTime, asDateTime;

//Date         : 2022/03/01
//Machine ID   : HHT-06
//Total Alarm  : 0
//Start Time   : 00:11:42.279
//Home Time    : 00:00:19.492
//Contact Test : 00:00:00.000
//Pause Time   : 00:00:00.000
//Product Time : 00:11:22.787
//Jam Time     : 00:00:00.000
//Power On     : 00:14:47.169
//Time OEE     : 79.14%

    if(IniConfig.bN10_UploadSummaryToFTP==false)
        return;

    if(b_Check_Dir_Exist_And_Creak_Dir(IniConfig.asByTimeOEEPath)==false)
    {
        ShowMyMessage("By time OEE save path error");
        return;
    }

    TRegExpr *regexDate=new TRegExpr;
    TRegExpr *regexTime=new TRegExpr;
    regexDate->Expression="(\\d+-\\d+-\\d+)";
    regexTime->Expression="(\\d+:\\d+:\\d+)";
    TDateTime LotStart;
    TDateTime LotEnd;

    DateSeparator='-';
    ShortDateFormat="yyyy-mm-dd";

    str.sprintf("By time OEE record : %s %d", Now().FormatString("yyyy/mm/dd hh:nn"), LastSet.SystemAccSecond[3][stPowerOn]);
    RecordProcess(str);

    iLastHour=iHour-1;
    if(iLastHour<0)
    {
        iLastHour=23;
        time_t today;
        struct tm *p;

        time(&today);
        today-=24*60*60;
        p=localtime(&today);

        AnsiString year,month,day;
        year=p->tm_year+1900;
        month=p->tm_mon+1;
        day=p->tm_mday;
        asDate.sprintf("%s%02d%02d", year, atoi(month.c_str()), atoi(day.c_str()));
        asLastDateTime.sprintf("%s/%02d/%02d %02d:00:00", year, atoi(month.c_str()), atoi(day.c_str()), iLastHour);
    }
    else
    {
        asDate=Now().FormatString("yyyymmdd");
        asLastDateTime.sprintf("%s %02d:00:00", Now().FormatString("yyyy/mm/dd"), iLastHour);
    }

    asDateTime.sprintf("%s", Now().FormatString("yyyy/mm/dd hh:nn:ss"));

    try
    {
        regexDate->InputString=asLastDateTime;
        if(regexDate->Exec())
        {
            LotStart=asLastDateTime;
        }
        else
        {
            LotStart=TDateTime("2023-01-01 00:00:00");
        }
    }
    catch(...)
    {
        LotStart=TDateTime("2023-01-01 00:00:00");
    }

    try
    {
        regexDate->InputString=asDateTime;
        if(regexDate->Exec())
        {
            LotEnd=asDateTime;
        }
        else
        {
            LotEnd=Now();
        }
    }
    catch(...)
    {
        LotEnd=Now();
    }

    for(int i=0; i<RunInfo.slEventLogFile->Count; i++)
    {
        TStringList *slEvent=new TStringList();
        TStringList *slLog  =new TStringList();
        if(FileExists(RunInfo.slEventLogFile->Strings[i])==false)
            continue;
        slEvent->LoadFromFile(RunInfo.slEventLogFile->Strings[i]);
        for(int j=1; j<slEvent->Count; j++)
        {
            slLog->CommaText=slEvent->Strings[j];
            if(slLog->Count>8)
            {
                TDateTime CurrentDateTime;
                regexDate->InputString=slLog->Strings[0];
                regexTime->InputString=slLog->Strings[1];
                if(regexDate->Exec() && regexTime->Exec())
                {
                    CurrentDateTime=StrToDate(slLog->Strings[0])+StrToTime(slLog->Strings[1]);
                }
                else
                {
                    continue;
                }

                if(CurrentDateTime>=LotStart && CurrentDateTime<=LotEnd)
                {
                    if(slLog->Strings[3].AnsiPos("JAM")!=0 && slLog->Strings[6]!="1")
                    {
                        iJamCount++;
                        iStopTime+=atoi(slLog->Strings[5].c_str());

                        if(myMesJamCount.size()==0)
                        {
                            MES_JAM_COUNT Temp;
                            Temp.JamCode=slLog->Strings[3];
                            Temp.Message=slLog->Strings[2]+" _ "+slLog->Strings[7];
                            Temp.iStopTime=atoi(slLog->Strings[5].c_str());
                            Temp.iCount=1;
                            myMesJamCount.push_back(Temp);
                        }
                        else
                        {
                            bMatch=false;
                            for(unsigned int k=0; k<myMesJamCount.size(); k++)
                            {
                                if(myMesJamCount[k].JamCode==slLog->Strings[3])
                                {
                                    bMatch=true;
                                    myMesJamCount[k].iCount++;
                                    myMesJamCount[k].iStopTime+=atoi(slLog->Strings[5].c_str());
                                }
                            }

                            if(bMatch==false)
                            {
                                MES_JAM_COUNT Temp;
                                Temp.JamCode=slLog->Strings[3];
                                Temp.Message=slLog->Strings[2]+" _ "+slLog->Strings[7];
                                Temp.iStopTime=atoi(slLog->Strings[5].c_str());
                                Temp.iCount=1;
                                myMesJamCount.push_back(Temp);
                            }
                        }
                    }
                    else if(slLog->Strings[3].AnsiPos("WAR")!=0 &&
                            slLog->Strings[6]!="1" &&
                            slLog->Strings[2].AnsiPos("Motion")==0)
                    {
                        iWarCount++;

                        if(myMesWarCount.size()==0)
                        {
                            MES_JAM_COUNT Temp;
                            Temp.JamCode=slLog->Strings[3];
                            Temp.Message=slLog->Strings[2]+" _ "+slLog->Strings[7];
                            Temp.iStopTime=atoi(slLog->Strings[5].c_str());
                            Temp.iCount=1;
                            myMesWarCount.push_back(Temp);
                        }
                        else
                        {
                            bMatch=false;
                            for(unsigned int k=0; k<myMesWarCount.size(); k++)
                            {
                                if(myMesWarCount[k].JamCode==slLog->Strings[3])
                                {
                                    bMatch=true;
                                    myMesWarCount[k].iCount++;
                                    myMesWarCount[k].iStopTime+=atoi(slLog->Strings[5].c_str());
                                }
                            }

                            if(bMatch==false)
                            {
                                MES_JAM_COUNT Temp;
                                Temp.JamCode=slLog->Strings[3];
                                Temp.Message=slLog->Strings[2]+" _ "+slLog->Strings[7];
                                Temp.iStopTime=atoi(slLog->Strings[5].c_str());
                                Temp.iCount=1;
                                myMesWarCount.push_back(Temp);
                            }
                        }
                    }
                }
            }
        }

        slLog->Clear();
        delete slLog;
        slEvent->Clear();
        delete slEvent;
    }

    TStringList *SL;
    SL=new TStringList();

    str.sprintf("Date         : %s", asDate);
    SL->Add(str);

    str.sprintf("Machine ID   : %s", IniConfig.SocketHandlerID);
    SL->Add(str);

    str.sprintf("Total Alarm  : %d", iJamCount+iWarCount);
    SL->Add(str);

    str.sprintf("Start Time   : %s", ConvertMSecToSPC(LastSet.SystemAccSecond[3][stStartTime]));
    SL->Add(str);

    str.sprintf("Home Time    : %s", ConvertMSecToSPC(LastSet.SystemAccSecond[3][stHomeTime]));
    SL->Add(str);

    str.sprintf("Contact Test : %s", ConvertMSecToSPC(LastSet.SystemAccSecond[3][stContactTest]));
    SL->Add(str);

    str.sprintf("Pause Time   : %s", ConvertMSecToSPC(LastSet.SystemAccSecond[3][stPauseTime]));
    SL->Add(str);

    str.sprintf("Product Time : %s", ConvertMSecToSPC(LastSet.SystemAccSecond[3][stProductTime]));
    SL->Add(str);

    str.sprintf("Jam Time     : %s", ConvertMSecToSPC(LastSet.SystemAccSecond[3][stJamTime]));
    SL->Add(str);

    str.sprintf("Power On     : %s", ConvertMSecToSPC(LastSet.SystemAccSecond[3][stPowerOn]));
    SL->Add(str);

    if(LastSet.SystemAccSecond[3][stPowerOn]==0)
        str.sprintf("Time OEE     : %3.2f%", 0.0);
    else
        str.sprintf("Time OEE     : %3.2f%", double(LastSet.SystemAccSecond[3][stStartTime]/1000)/double(LastSet.SystemAccSecond[3][stPowerOn]/1000)*100.0);
    SL->Add(str);

    SL->Add("");

    for(int i=eAuto1; i<=iFixRightHalf; i++)
    {
        iTotalTestCT+=LastSet.iBinCTForAlways[2][i];                            //[2] : 1 hour
    }

    for(int i=eAuto1; i<=iFixRightHalf; i++)
    {
        if(iTotalTestCT==0 || LastSet.iBinCTForAlways[2][i]==0)                 //[2] : 1 hour
        {
            str.sprintf("H/W %s    : %5d", s6TrayName[i], 0);
        }
        else
        {
            str.sprintf("H/W %s    : %5d",
                                 s6TrayName[i],
                                 LastSet.iBinCTForAlways[2][i]);                //[2] : 1 hour
        }
        SL->Add(str);
    }

    str.sprintf("Total Q'ty   : %d", iTotalTestCT);
    SL->Add(str);

    str.sprintf("Start time   %s", asLastDateTime);
    SL->Add(str);
    str.sprintf("End time     %s", asDateTime);
    SL->Add(str);

    SL->Add("");

    SL->Add("Jam Statistics");
    SL->Add("------------------------------------");
    str.sprintf("Jam rate: %d / %d", iJamCount, iTotalTestCT);
    SL->Add(str);
    str.sprintf("Stoppage(s): %d", iStopTime);
    SL->Add(str);

    SL->Add("");
    SL->Add("Brief :");
    str.sprintf("Alarm : %d", iJamCount);
    SL->Add(str);
    SL->Add("");
    SL->Add("Details Data:");
    str.sprintf("Item AlarmCode Count Stoppage(s) UnitName_AlarmMessage");
    SL->Add(str);

    sort(myMesJamCount.begin(), myMesJamCount.end(), mes_cmp_by_count);
    for(unsigned int i=0; i<myMesJamCount.size(); i++)
    {
        str.sprintf("%3d   %s  %3d     %5d     %s", i+1, myMesJamCount[i].JamCode,
                                                         myMesJamCount[i].iCount,
                                                         myMesJamCount[i].iStopTime,
                                                         myMesJamCount[i].Message);
        SL->Add(str);
    }

    SL->Add("");

    SL->Add("Alarm(NoJam) Stoppage Statistics");
    SL->Add("------------------------------------");
    SL->Add("Brief :");
    str.sprintf("Alarm : %d",iWarCount);
    SL->Add(str);
    SL->Add("");
    SL->Add("Details Data:");
    str.sprintf("Item AlarmCode Count Stoppage(s) UnitName_AlarmMessage");
    SL->Add(str);

    sort(myMesWarCount.begin(), myMesWarCount.end(), mes_cmp_by_count);
    for(unsigned int i=0; i<myMesWarCount.size(); i++)
    {
        str.sprintf("%3d   %s  %3d     %5d     %s", i+1, myMesWarCount[i].JamCode,
                                                         myMesWarCount[i].iCount,
                                                         myMesWarCount[i].iStopTime,
                                                         myMesWarCount[i].Message);
        SL->Add(str);
    }

    SL->Add("");
    str.sprintf("Last Device Recipe:%s", fMain->cbSetupFileName->Text);
    SL->Add(str);
    str.sprintf("Package Size: %s x %s mm",FormatFloat("0.00", DeviceForm_File.XDimension), FormatFloat("0.00", DeviceForm_File.YDimension));
    SL->Add(str);
    str.sprintf("Report is created in: %s",asDateTime);
    SL->Add(str);
    str.sprintf("Statistic from: %s",asLastDateTime);
    SL->Add(str);

    //(HHJ-01)_SPCSCH_2022022501.spcrpt
    asFileName.sprintf("(%s)_SPCSCH_%s%02d.spcrpt", IniConfig.SocketHandlerID,asDate,iLastHour);
    asLocalFile=IniConfig.asByTimeOEEPath+"\\"+asFileName;
    SL->SaveToFile(asLocalFile);
    SL->Clear();

    AddOEEState(fLotInfo->labOEEState->Caption);

    //------------------
    int iOEESites=0, iCount[2]={0, 0}, iTime=0, iMaxPlanOut=0;
    int iOEETime[9]={0, 0, 0, 0, 0, 0, 0, 0, 0};
    int iOEETimeH[2]={0, 0};
    int iPlanOutH[2]={0, 0};
    double dOEE=0.0,dOEETestTime=0.0,dOEEIndexTime=0.0;
    TStringList *slOEE=new TStringList();

    try
    {
        iPlanOut12h=0;
        for(int j=0; j<2; j++)
        {
            iOEETotalPass=0;

            if(j==0)                                                            //1 hour
                iCount[0]=stOEEState->Count;
            else                                                                //12 hour
                iCount[1]=stOEEState12hList->Count;

            for(int i=1; i<iCount[j]; i++)
            {
                if(j==0)                                                        //1 hour
                    slOEE->CommaText=stOEEState->Strings[i];
                else                                                            //12 hour
                    slOEE->CommaText=stOEEState12hList->Strings[i];

                if(slOEE->Count<13 || slOEE->Strings[0]=="Code")
                    continue;

                if(slOEE->Strings[0]!="Stop")
                    iOEETotalPass+=atoi(slOEE->Strings[4].c_str());

                iOEESites    =atoi(slOEE->Strings[10].c_str());
                dOEETestTime =atof(slOEE->Strings[11].c_str());
                dOEEIndexTime=atof(slOEE->Strings[12].c_str());

                iTime=TimeToSec(slOEE->Strings[5]);

                if(j==1)
                {
                    if(slOEE->Strings[0]=="Jam")
                        iOEETime[0]+=iTime;
                    else if(slOEE->Strings[0]=="Retest")
                        iOEETime[1]+=iTime;
                    else if(slOEE->Strings[0]=="Down")
                        iOEETime[2]+=iTime;
                    else if(slOEE->Strings[0]=="Setup")
                        iOEETime[3]+=iTime;
                    else if(slOEE->Strings[0]=="Idle")
                        iOEETime[4]+=iTime;
                    else if(slOEE->Strings[0]=="ENG")
                        iOEETime[5]+=iTime;
                    else if(slOEE->Strings[0]=="PM")
                        iOEETime[6]+=iTime;
                    else if(slOEE->Strings[0]=="Stop")
                        iOEETime[7]+=iTime;
                    else if(slOEE->Strings[0]=="Running")
                        iOEETime[8]+=iTime;
                }

                iOEETimeH[j]+=iTime;

                if(dOEEIndexTime>0 && iOEESites>0)
                {
                    iMaxPlanOut=(iTime/(dOEETestTime+dOEEIndexTime))*iOEESites;
                    if(iMaxPlanOut>6500)
                        iMaxPlanOut=6500;

                    iPlanOutH[j]+=iMaxPlanOut;
                }
            }

            if(iOEESites>0 && iPlanOutH[j]>0 && iOEETimeH[j]>0)
            {
                dOEE=(double(iOEETotalPass)/double(iPlanOutH[j]))*100.0;
                if(dOEE>100.0)
                    dOEE=100.0;
                str.sprintf("%3.2f%", dOEE);

                if(j==0)
                {
                    stOEEState->Add("**********************************************************************************");
                    stOEEState->Add("OEE:"+str);
                    stOEEState->Add("**********************************************************************************");
                }
                else if(j==1)
                {
                    iPlanOut12h=iPlanOutH[1];

                    fLotInfo->sgOEEState->Cells[1][0]=str;
                    str.sprintf("%3.2f%", (double(iOEETime[1]+iOEETime[8]) /double(iOEETimeH[1]))*100.0 );
                    fLotInfo->sgOEEState->Cells[1][1]=str;
                    for(int i=0;i<8;i++)
                    {
                        str.sprintf("%3.2f%", (double(iOEETime[i]) /double(iOEETimeH[1]))*100.0 );
                        fLotInfo->sgOEEState->Cells[1][i+2]=str;
                    }
                }
            }
        }

        asFileName.sprintf("(%s)_OEE_%s%02d.csv", IniConfig.SocketHandlerID, asDate,iLastHour);
        asLocalFile=IniConfig.asByTimeOEEPath+"\\"+asFileName;
        stOEEState->SaveToFile(asLocalFile);

        if(iHour==8 || iHour==20)
        {
            stOEEState12hList->Insert(0,"**********************************************************************************");
            stOEEState12hList->Insert(0,"");
            stOEEState12hList->Insert(0,"Handler version : "+asHandlerVersion);

            for(int i=9; i>=0; i--)
            {
                str.sprintf("%s:%s", fLotInfo->sgOEEState->Cells[0][i], fLotInfo->sgOEEState->Cells[1][i]);
                stOEEState12hList->Insert(0,str);
            }
            stOEEState12hList->Insert(0,"Time Rate:");

            asFileName.sprintf("(%s)_TwelveHours_OEE_%s_%02d.csv", IniConfig.SocketHandlerID,asDate,iHour);
            asLocalFile = "D:\\HT9045_Log\\ProductReport\\"+asFileName;
            stOEEState12hList->SaveToFile(asLocalFile);
            stOEEState12hList->Clear();
        }
    }
    catch(...)
    {
        ShowMyMessage("stOEEState save file fail");
    }

    slOEE->Clear();
    delete slOEE;
    stOEEState->Clear();

    //------------------
    myMesJamCount.clear();
    myMesWarCount.clear();
    delete SL;
    delete regexDate;
    delete regexTime;

    for(int i=0; i<8; i++)
    {
        LastSet.SystemAccSecond[2][i]=0;
        LastSet.SystemAccSecond[3][i]=0;
    }

    for(int j=eAuto1; j<=iFixRightHalf; j++)
    {
        LastSet.iBinCTForAlways[2][j]=0;                                        //[2] : 1 hour
    }

    RunInfo.slEventLogFile->Clear();
}
//------------------------------------------------------------------------------
void __fastcall TfMesSystem::VTestUPHReport()                                   //jou 20220525 : lot end 時輸出 UPH report
{
    AnsiString S;
    if(b_Check_Dir_Exist_And_Creak_Dir(IniConfig.asUPHReportPath)==false)
    {
        S.sprintf("目錄不存在 %s",IniConfig.asUPHReportPath);
        ShowMyMessage(S);
        return;
    }

    TStringList *slReport = new TStringList();
    AnsiString aFileName,aLocalFile;
    int iTotalTestCT=0;
    int iMUBA=0;

    slReport->Clear();

    aFileName.sprintf("%s-%s-%s-%04d%02d%02d%02d%02d%02d.sum", fLotInfo->edtSysLotID->Text, fLotInfo->cbRunMode->Text,
                        fLotInfo->cbProcess->Text, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);

    aLocalFile = IniConfig.asUPHReportPath+"\\"+aFileName;

    S.sprintf("Operator ID: %s", fLotInfo->edtSysOperatorID->Text);
    slReport->Add(S);

    S.sprintf("Lot ID: %s", fLotInfo->edtSysLotID->Text);
    slReport->Add(S);

    for(int i=eAuto1; i<=iFixRightHalf; i++)
        iTotalTestCT+=LastSet.BinCT_ART[0][iTo3Unload[i]];

    if(iTotalTestCT>0 && LastSet.SystemAccSecond[0][stProductTime]>0)
        iMUBA=iTotalTestCT/double(LastSet.SystemAccSecond[0][stProductTime]/3600000.0);

    S.sprintf("MUBA=%d",iMUBA);
    slReport->Add(S);

    slReport->SaveToFile(aLocalFile);
    slReport->Clear();
    delete slReport;
}
//------------------------------------------------------------------------------
void __fastcall TfMesSystem::SendJamCode(AnsiString asJamCode)                  //jou 20221104 : VTest CreateManualEOCAP function;
{
    AnsiString asURL;
    AnsiString asData;

    bCreateManualEOCAP_OK=false;
    lbledtCreateManualEOCAP_JamCode->Text=StringReplace(asJamCode, " ", "", TReplaceFlags()<<rfReplaceAll);
//    lbledtCreateManualEOCAP_JamCode->Text=StringReplace(lbledtCreateManualEOCAP_JamCode->Text, ",", "", TReplaceFlags()<<rfReplaceAll);
//    lbledtCreateManualEOCAP_JamCode->Text=StringReplace(lbledtCreateManualEOCAP_JamCode->Text, "!", "", TReplaceFlags()<<rfReplaceAll);
//    lbledtCreateManualEOCAP_JamCode->Text=StringReplace(lbledtCreateManualEOCAP_JamCode->Text, ":", "", TReplaceFlags()<<rfReplaceAll);
    if(bDownloadLotInforFlag==false)
    {
        ShowMyMessage("請下載開批資料.");
        return;
    }

    if(IniConfig.asCreateManualEOCAP_URL.Length()<=0)
    {
         ShowMyMessage("請輸入URL CreateManualEOCAP資料.");
         return;
    }

    if(lbledtCustCode->Text.Length()<=0)
    {
         ShowMyMessage("請輸入CustCode CreateManualEOCAP資料.");
         return;
    }

    if(lbledtCustPart->Text.Length()<=0)
    {
         ShowMyMessage("請輸入CustPart CreateManualEOCAP資料.");
         return;
    }

    if(lbledtCustLotNum->Text.Length()<=0)
    {
         ShowMyMessage("請輸入CustLot CreateManualEOCAP資料.");
         return;
    }

    if(lbledtInternalLot->Text.Length()<=0)
    {
         ShowMyMessage("請輸入InternalLot CreateManualEOCAP資料.");
         return;
    }

    if(lbledtProcess->Text.Length()<=0)
    {
         ShowMyMessage("請輸入StepCode CreateManualEOCAP資料.");
         return;
    }

    if(lbledtCreateManualEOCAP_JamCode->Text.Length()<=0)
    {
         ShowMyMessage("請輸入JamCode CreateManualEOCAP資料.");
         return;
    }

    if(IniConfig.SocketHandlerID.Length()<=0)
    {
         ShowMyMessage("請輸入Machine ID CreateManualEOCAP資料.");
         return;
    }

    lstCreateManualEOCAP->Items->Clear();

    #ifdef SOFT_SIMULTE
//        asData="ok";
    #else
        try
        {
            asData=fWebService->SIPServiceSoap_VTEST->CreateManualEOCAP(lbledtCustCode->Text,
                                                                 lbledtCustPart->Text,
                                                                 lbledtCustLotNum->Text,
                                                                 lbledtInternalLot->Text,
                                                                 lbledtProcess->Text,
                                                                 AnsiString(""),
                                                                 lbledtCreateManualEOCAP_JamCode->Text,
                                                                 IniConfig.SocketHandlerID,
                                                                 AnsiString("SYS"));
        }
        catch(...)
        {
            ShowMyMessage("CreateManualEOCAP send error.");
            return;
        }
    #endif

    if(asData.Length()>0)
    {
        lstCreateManualEOCAP->Items->Add(asData);
        bCreateManualEOCAP_OK=false;
    }
    else
    {
        bCreateManualEOCAP_OK=true;
    }

    asData.sprintf("CustCode:%s,CustPart:%s,CustLotNum:%s,InternalLot:%s,Process:%s,JamCode:%s,ID:%s",
                                                                                lbledtCustCode->Text,
                                                                                lbledtCustPart->Text,
                                                                                lbledtCustLotNum->Text,
                                                                                lbledtInternalLot->Text,
                                                                                lbledtProcess->Text,
                                                                                lbledtCreateManualEOCAP_JamCode->Text,
                                                                                IniConfig.SocketHandlerID);
    lstCreateManualEOCAP->Items->Add(asData);

    lstCreateManualEOCAP->Items->SaveToFile("D:\\HT9045\\system\\CreateManualEOCAP.txt");
}
//------------------------------------------------------------------------------
void __fastcall TfMesSystem::btnSendJamCodeClick(TObject *Sender)               //jou 20221104 : VTest CreateManualEOCAP function;
{
    SendJamCode(lbledtCreateManualEOCAP_JamCode->Text);
}
//------------------------------------------------------------------------------
bool __fastcall TfMesSystem::CheckEocapStatus(AnsiString asString)              //jou 20221104 : VTest CreateManualEOCAP function;
{
    int iStart=0;
    bool bResult=false;
    AnsiString asResult;
    iStart=asString.Pos("eocapStatus");
    asResult=asString.SubString(iStart+13, 1);
    if(asResult=="1")
    {
        labEocapStatus->Caption="eocapStatus : 1 ( 已結案 )";
        bResult=true;
    }
    else if(asResult=="0")
    {
        labEocapStatus->Caption="eocapStatus : 0 ( 處理中 )";
    }
    else
    {
        labEocapStatus->Caption="eocapStatus : null";
    }

    if(bShow==true)
        RecordProcess(labEocapStatus->Caption);
    return bResult;
}
//------------------------------------------------------------------------------
bool __fastcall TfMesSystem::QueryEocapStatus(bool bManualTest)                 //jou 20221104 : VTest CreateManualEOCAP function;
{
    if(bManualTest==false && bCreateManualEOCAP_OK==false)
        return false;

    // BCB Client
    AnsiString asURL;
    AnsiString result;
    AnsiString eqpCode     = StringReplace(lbledtCreateManualEOCAP_JamCode->Text, " ", "%20", TReplaceFlags()<<rfReplaceAll);
    AnsiString eqpAlarmsId = IniConfig.SocketHandlerID;

    TStringStream *response= new TStringStream("");

    lstQueryEocapStatus->Clear();

/*
    //POST
    TStringStream *request = new TStringStream("");

#ifdef SOFT_SIMULTE
    asURL = "http://192.168.100.59/webapitest/api/mes/queryEocapStatus";   //honprec
//    IdHTTPqueryEocapStatus->Port = 80;
#else
    asURL = IniConfig.asQueryEocapStatusURL;      //VTEST
//    IdHTTPqueryEocapStatus->Port = 9014;
#endif

    // 產生 POST 參數
    request->WriteString(_T("?eqpCode="+eqpCode));
    request->WriteString(_T("&"));
    request->WriteString(_T("eqpAlarmsId="+eqpAlarmsId));

//    IdHTTPqueryEocapStatus->Request->ContentType = "application/x-www-form-urlencoded";
    IdHTTPqueryEocapStatus->Request->Accept="application/json";
    IdHTTPqueryEocapStatus->Response->Accept="application/json";
    IdHTTPqueryEocapStatus->Request->ContentType  = "application/json";
    IdHTTPqueryEocapStatus->Response->ContentType = "application/json";

    // 傳遞資料
    try
    {
        IdHTTPqueryEocapStatus->Post(url, request, response);
        // 接收 Respose 資料
        result = response->DataString;
    }
    catch(...)
    {
        ShowMyMessage("QueryEocapStatus get error.");
    }

    delete request;
    delete response;
*/
    #ifdef SOFT_SIMULTE
        asURL="http://192.168.100.59/webapitest/api/mes/queryEocapStatus";
        lstQueryEocapStatus->Items->LoadFromFile("D:\\HT9045\\system\\QueryEocapStatus.txt");
    #else
        asURL=IniConfig.asQueryEocapStatusURL;
    #endif

    asURL.cat_printf("?eqpCode=%s&eqpAlarmsId=%s", eqpCode, eqpAlarmsId);

    IdHTTPqueryEocapStatus->Request->Accept         ="application/json";
    IdHTTPqueryEocapStatus->Response->Accept        ="application/json";
    IdHTTPqueryEocapStatus->Request->ContentType    ="application/json";
    IdHTTPqueryEocapStatus->Response->ContentType   ="application/json";

    try
    {
        IdHTTPqueryEocapStatus->Get(asURL,response);
    }
    catch(...)
    {
        ShowMyMessage("QueryEocapStatus get error.");
    }

/*
    #ifdef SOFT_SIMULTE
        asURL="http://192.168.100.59/webapitest/api/mes/queryEocapStatus";
    #else
        asURL=IniConfig.asQueryEocapStatusURL;
    #endif
    asURL.cat_printf("?eqpCode=%s&eqpAlarmsId=%s",eqpCode,eqpAlarmsId);

    try
    {
        result=IdHTTPqueryEocapStatus->Get(asURL);
    }
    catch(...)
    {
        ShowMyMessage("QueryEocapStatus get error.");
    }
*/
//--------------------
#ifdef SOFT_SIMULTE
    result=lstQueryEocapStatus->Items->Text;
#else
    try
    {
        // 接收 Respose 資料
        result = response->DataString;
    }
    catch(...)
    {
        ShowMyMessage("QueryEocapStatus data error.");
    }
    lstQueryEocapStatus->Items->Add(result);
    lstQueryEocapStatus->Items->SaveToFile("D:\\HT9045\\system\\QueryEocapStatus.txt");
#endif
    delete response;

    if(CheckEocapStatus(result))  //確認結果
    {
        bCreateManualEOCAP_OK=false;
    }

    return bCreateManualEOCAP_OK;
}
//------------------------------------------------------------------------------
void __fastcall TfMesSystem::btnQueryEocapStatusClick(TObject *Sender)
{
    QueryEocapStatus(true);
}
//------------------------------------------------------------------------------
void __fastcall TfMesSystem::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    bShow=false;
}
//------------------------------------------------------------------------------
bool __fastcall TfMesSystem::CheckVTENGmode(AnsiString asLotId)
{
    if(asLotId=="VTENGrecipe" || asLotId=="VTEErecipe")
    {
        return true;
    }
    else
        return false;
}
//------------------------------------------------------------------------------
//jou 20230621 : VTEST Handler即時監控 GetRcsCheckingResult
bool __fastcall TfMesSystem::GetRcsCheckingResult(bool bShowAlarm)
{
    if(bShow==false && (IniConfig.bGetRcsCheckingResult==false ||
                        LastSet.iRunStartMode==rsmAutoSiteMap))
    {
        return true;
    }

    AnsiString asURL="";
    AnsiString asACode="";
    AnsiString asAction="";
    AnsiString asItemName="";
    AnsiString asData="";

    lstGetRcsCheckingResult->Items->Clear();
    asURL     =(bShow==true)?lbledtGetRcsCheckingResultUrl->Text:IniConfig.asGetRcsCheckingResultUrl;
    asACode   =(bShow==true)?lbledtGetRcsCheckingResultACode->Text:IniConfig.asGetRcsCheckingResultACode;
    asAction  =(bShow==true)?lbledtGetRcsCheckingResultAction->Text:IniConfig.asGetRcsCheckingResultAction;

    if(bShow==true)
    {
        if(lbledtGetRcsCheckingResultItemName->Text=="")
        {
            ShowMyMessage("請輸入ItemName，格式 LotID:Process");
            return false;
        }
        asItemName=lbledtGetRcsCheckingResultItemName->Text;
    }
    else
    {
        asItemName.sprintf("%s:%s", fLotInfo->edtSysLotID->Text, fLotInfo->cbProcess->Text.SubString(1, 3));
        lbledtGetRcsCheckingResultItemName->Text=asItemName;
    }

    asURL.cat_printf("?ACode=%s&Action=%s&ItemName=%s", asACode, asAction, asItemName);
    asGetRcsCheckingResult="PASS";
#ifdef SOFT_SIMULTE
//    asData="PASS";
    asData="FAIL";
#else
    try
    {
        asData=IdHTTPMESSystem->Get(asURL);
    }
    catch(...)
    {
        ShowMyMessage("GetRcsCheckingResult get error.");
        return false;
    }
#endif

    if(asData.Length()>0)
    {
        lstGetRcsCheckingResult->Items->Add(asData);
        if(asData.Pos("FAIL")>=1)
        {
            asGetRcsCheckingResult=asData;
            if(bShowAlarm==true)
            {
                SendJamCode(asData);
                ShowMyMessage(asData);
                return false;
            }
        }
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
void __fastcall TfMesSystem::btnGetRcsCheckingResultClick(TObject *Sender)
{
    GetRcsCheckingResult(true);
}
//------------------------------------------------------------------------------
bool TfMesSystem::Get2DIDFromServer(AnsiString asServerIP)
{
    AnsiString asSaveDirectory="D:\\HT9045\\system\\FromServer";
    AnsiString asSaveFileName="2DID_DL.json";
    AnsiString asSaveLocation=asSaveDirectory+AnsiString("\\")+asSaveFileName;

    bool bBack=false;
    //
    MyForceDirectories(asSaveDirectory);
    AnsiString asGet=GetServerWebAPI(asServerIP);
    uRDN.WriteFile(asSaveLocation,asGet);
    //
    if(uRDN.Read2DFile(asSaveLocation)==true)
    {
        map2DIDFromServer=uRDN.GetList2D();
        bBack=true;
    }
    else
    {
        bBack=false;
    }
    //
    return bBack;
}
//------------------------------------------------------------------------------
bool TfMesSystem::IsMatchServerData(AnsiString as2DID)
{
    for(unsigned int i=0; i<map2DIDFromServer.size(); i++)
    {
        if(map2DIDFromServer[i].Pos(as2DID)>0)
        {
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
void TfMesSystem::RecordMsg(AnsiString asMsg)
{
    if(listbMesFileLog->Items->Count>10000)
    {
        listbMesFileLog->Items->Clear();
    }
    listbMesFileLog->Items->Add(asMsg);
}
//------------------------------------------------------------------------------
AnsiString TfMesSystem::GetServerWebAPI(AnsiString asURL)
{
    AnsiString asBack="";
    TIdHTTP *http = new TIdHTTP(NULL);
    try
    {
        asBack=http->Get(asURL);
    }
    catch (Exception &e)
    {
    }
    delete http;
    return asBack;
}
//------------------------------------------------------------------------------
void TfMesSystem::WriteOEEState(AnsiString asString)
{
    if(asString!=fLotInfo->labOEEState->Caption)
    {
        AddOEEState(fLotInfo->labOEEState->Caption);
        fLotInfo->labOEEState->Caption=asString;
    }
}
//------------------------------------------------------------------------------
void TfMesSystem::SetOEEState(int iState)
{
    if(IniConfig.bVTESTFunction==false)
        return;

    switch(iState)
    {
        case stStartTime:
            if(fLotInfo->rgOEEState->ItemIndex>1)
                return;

            if(iRunStartMode==FT)
                WriteOEEState("State : Running");
            else
                WriteOEEState("State : Retest");
            fLotInfo->rgOEEState->Enabled=false;
            break;
        case stPauseTime:
            if(fLotInfo->rgOEEState->ItemIndex>=1)
                return;

            WriteOEEState("State : Stop");
            fLotInfo->rgOEEState->Enabled=true;
            break;
        case stJamTime:
            if(fLotInfo->rgOEEState->ItemIndex>=1)
                return;

            WriteOEEState("State : Jam");
            fLotInfo->rgOEEState->Enabled=false;
            break;
    }

    iOEEStateChangeCnt++;                                                       //RogerYang 20250529 加入保護，曾發生狀態莫名其妙一直切換導致LOG暴增
    fLotInfo->rgOEEState->ItemIndex=0;
    if(iOEEStateChangeCnt>0)
        iOEEStateChangeCnt--;
}
//------------------------------------------------------------------------------
void TfMesSystem::AddOEEState(AnsiString asOEECode)
{
    if(IniConfig.bN10_UploadSummaryToFTP==false)
        return;

    if(b_Check_Dir_Exist_And_Creak_Dir(IniConfig.asByTimeOEEPath)==false)
    {
        ShowMyMessage("By time OEE save path error");
        return;
    }

    AnsiString asString,asLotNo;
    AnsiString asTitle="Code,Start,End,Pass_Bin_Qty,TTL_Q'ty,TTL_Time,Device,Lot_NO,Flow,Mode,Site,TestTime,IndexTime";
    dtOEETimeEnd=Now();

    try
    {
        dtOEETimeDiff=dtOEETimeEnd-dtOEETimeStart;
    //    fLotInfo->grpOEEState->Caption=dtOEETimeDiff.FormatString("hhnnss");
        if(stOEEState->Count==0)
            stOEEState->Add(asTitle);

        if(stOEEState12hList->Count==0)
            stOEEState12hList->Add(asTitle);

        int iTotalTestCT=0, iPassTestCT=0;
        int iSites=0;

        if(atoi(lbledtTestSites->Text.c_str())<=0)
        {
            for(int i=0; i<TestSocket.iShtRow; i++)
                for(int j=0; j<TestSocket.iShtCol; j++)
                    if(LastSet.bUseTestSocket[0][i][j]==true ||
                       LastSet.bUseTestSocket[1][i][j]==true)                   //jou 20220922 : 修正關arm記數site數量異常
                        iSites++;
        }
        else
        {
            iSites=atoi(lbledtTestSites->Text.c_str());
        }

        if(CheckVTENGmode(fLotInfo->edtSysLotID->Text)==false)
        {
            for(int i=eAuto1; i<=iFixRightHalf; i++)
            {
                if(Prod.iIsPassT6[i]==1)
                    iPassTestCT+=LastSet.iBinCTForAlways[3][i];                 //[3] : OEE state

                iTotalTestCT+=LastSet.iBinCTForAlways[3][i];                    //[3] : OEE state
            }

            asLotNo=lbledtInternalLot->Text;
        }
        else
        {
            for(int i=eAuto1; i<=iFixRightHalf; i++)
                LastSet.iBinCTForAlways[3][i]=0;

            asLotNo=fLotInfo->edtSysLotID->Text;
        }

        asOEECode=asOEECode.SubString(8, asOEECode.Length());
        asString.sprintf("%s,%s,%s,%5d,%5d,%s,%s,%s,%s,%s,%2d,%3.2f,%3.2f",
                               asOEECode,                                       //Code
                               dtOEETimeStart.FormatString("hh:nn:ss"),         //Start
                               dtOEETimeEnd.FormatString("hh:nn:ss"),           //End
                               iPassTestCT,                                     //Pass_Bin_Qty
                               iTotalTestCT,                                    //TTL_Q'ty
                               dtOEETimeDiff.FormatString("hh:nn:ss"),          //TTL_Time
                               lbledtC1->Text,                                  //Device
                               asLotNo,                                         //Lot_NO
                               fLotInfo->cbProcess->Text,                       //Flow
                               fLotInfo->cbRunMode->Text,                       //Mode
                               iSites,                                          //Site
                               dTestTime,
                               dIndexTime
                               );

        stOEEState->Add(asString);
        stOEEState12hList->Add(asString);
        for(int j=eAuto1; j<=iFixRightHalf; j++)
        {
            LastSet.iBinCTForAlways[3][j]=0;                                    //[3] : OEE state
        }
        dtOEETimeStart=dtOEETimeEnd;
    }
    catch(...)
    {
        ShowMyMessage("AddOEEState error.");
    }
}
//------------------------------------------------------------------------------
bool TfMesSystem::AutoSiteMapPass(int iBin)
{
    if(TestIF_File.iASMPassBin==-1)
    {
        if(BinSelect[iTestRunMode].iStackDefFailCate[BinSelect[iTestRunMode].iCatDataT3Pos[iBin]-1]==0)
            return true;
    }
    else
    {
        if(iBin==TestIF_File.iASMPassBin)
            return true;
    }
    return false;
}
//---------------------------------------------------------------------------
bool TfMesSystem::NeedNoRTBinID(int iFix)                                       //RogerYang 20250626 偉測不可複測bin功能
{
    if(TrayForm.bVTestNoRTBin==false)
    {
        return false;
    }
    else
    {
        if(bNoRTBinFlag[iFix]==false && bNoRTBinFixFlag[iFix]==true)            //ID還沒比對過 && 該Fix需要比對
            return true;
        else
            return false;
    }
}
//---------------------------------------------------------------------------
void TfMesSystem::ClearMesData()
{
    LabeledEditLotNo->Text="";
    LabeledEditOPID->Text="";
    lbledtHandlerID->Text="";
    LabeledHandlerDevice->Text="";
    LabeledEditLotSize->Text="";
    LabeledSiteGap->Text="";
    LabeledEditContFail->Text="";
    LabeledTestTemp->Text="";
    LabeledSoakTime->Text="";
    lbledtLabeledFTAutoBin1->Text="";
    lbledtLabeledFTAutoBin2->Text="";
    lbledtLabeledFTAutoBin3->Text="";
    LabeledFTFixBin1->Text="";
    LabeledFTFixBin2->Text="";
    LabeledFTFixBin3->Text="";
    LabeledAutoBin1->Text="";
    LabeledAutoBin2->Text="";
    LabeledAutoBin3->Text="";
    LabeledFixBin1->Text="";
    LabeledFixBin2->Text="";
    LabeledFixBin3->Text="";
    lbledtDevicePassBins->Text="";
    lbledtDeviceRetestBins->Text="";
    LabeledLeadCount->Text="";
    LabeledForcePerPin->Text="";
    lbledtHandlerDeviceLowYield->Text="";
    lbledtProcess->Text="";
    lbledtCustCode->Text="";
    lbledtCustPart->Text="";
    lbledtInternalLot->Text="";
    lbledtCustLotNum->Text="";
    lbledtHandlerDeviceATCcheck->Text="";
    lbledtHandlerDeviceSOTdelay->Text="";
    lbledtHandlerDeviceGPIBaddress->Text="";
    lbledtHandlerDeviceTestTempVar->Text="";
    lbledtHandlerDeviceSiteCheck->Text="";
    lbledtHandlerDeviceOpenBins->Text="";
    lbledtHandlerDeviceHotplateXDevision->Text="";
    lbledtHandlerDeviceHotplateYDevision->Text="";
    lbledtHandlerDeviceHotplateXStart->Text="";
    lbledtHandlerDeviceHotplateYStart->Text="";
    lbledtHandlerDeviceHotplateXPitch->Text="";
    lbledtHandlerDeviceHotplateYPitch->Text="";
    lbledtHandlerDeviceTrayXDevision->Text="";
    lbledtHandlerDeviceTrayYDevision->Text="";
    lbledtHandlerDeviceTrayXStart->Text="";
    lbledtHandlerDeviceTrayYStart->Text="";
    lbledtHandlerDeviceTrayXPitch->Text="";
    lbledtHandlerDeviceTrayYPitch->Text="";
    lbledtHandlerDeviceSiteLayouts->Text="";
    lbledtHandlerDeviceCleanPadLimit->Text="";
    lbledtHandlerDeviceInshuttleSensorCheck->Text="";
    lbledtTestTimePcs->Text="";
    lbledtC1->Text="";
    lbledtTestSites->Text="";
    lbledtIndexTime->Text="";
    lbledtHandlerDeviceNotRepeatableArea->Text="";                              //RogerYang 20250717 偉測更改欄位名稱，為了不跟bin別混淆     //RogerYang 20250604 偉測不可複測bin功能
    fSortCT->btnClearCountClick(fSortCT);
}
//---------------------------------------------------------------------------
void __fastcall ShowInputDialog()                                               //RogerYang 20250723 偉測不可複測bin功能(未完成)
{
    TForm *dlg = new TForm(Application);
    dlg->Caption = "Input NoRTBin Data";
    dlg->Width = 300;
    dlg->Height = 220;
    dlg->Position = poScreenCenter;
    dlg->BorderStyle = bsDialog;
}
//---------------------------------------------------------------------------
