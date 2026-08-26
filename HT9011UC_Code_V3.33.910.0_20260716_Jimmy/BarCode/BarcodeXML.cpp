#include "MachineDefine.h"
#pragma hdrstop

#include "BarcodeXML.h"

#include "cprod.h"
#include "MyKitSuck.h"
#include "BarCode.h"
#include "uLotInfo.h"
#include "common.h"
#include "SCK_ART.h"
#include "csystem.h"
#include "cObserver.h"
#include "main.h"
#include "cShowBinSelect.h"
#include "cConfiguration.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmXml *frmXml;

//---------------------------------------------------------------------------
__fastcall TfrmXml::TfrmXml(TComponent* Owner)
    : TForm(Owner)
{
}
//------------------------------------------------------------------------------
int TfrmXml::SendHTTPRequest(int iSht)
{
    int iHasErr=1;
    AnsiString asURL, asData="", S1, S2;
    AnsiString FileNamePath="D:\\HT9045_Log\\HTTP_Log", FileName;

    FileName.sprintf("%s\\%04d%02d%02d\\", FileNamePath, SystemYear, SystemMonth, SystemDate);
    MyForceDirectories(FileName);

    if(iSht==0)
        ptr2DIDSHT=&FLCarryKit;
    else
        ptr2DIDSHT=&BLCarryKit;

    if(ptr2DIDSHT->HasRealIC())
    {
        for(int i=0; i<InArmSuck.iShtRow; i++)
        {
            for(int j=0; j<InArmSuck.iShtCol; j++)
            {
                if(ptr2DIDSHT->Item[i][j]!=NULL_IC && ptr2DIDSHT->Item[i][j]!=HAS_NULL_IC)
                {
                    if(ptr2DIDSHT->cDeviceInf[i][j]!=asBarCodeErrorSend && ptr2DIDSHT->cDeviceInf[i][j]!="")
                    {
                        if(asData=="")
                            asData=ptr2DIDSHT->cDeviceInf[i][j];
                        else
                            asData=asData+","+ptr2DIDSHT->cDeviceInf[i][j];
                    }
                }
            }
        }

        if(asData!="")
        {
            asURL.sprintf("%s&CDC0010=%s&DHC0015=%s&DHC0013=%s&NOC0027=%s&CDC1826=%s&CDC2627=%s&NON0083=%s",
                          IniConfig.sN23_1_URL,
                          IniConfig.SocketHandlerID,
                          IniConfig.sN23_2_Line,
                          IniConfig.sN23_2_Process,
                          fLotInfo->edtSysLotID->Text,
                          asData,
                          IniConfig.sN23_2_Product,
                          fLotInfo->edPage->Text
                          );

            fBarCode->lblURL->Caption=asURL;
            fBarCode->mmoXML->Clear();
            try
            {
//                if(IdHTTPMESSystem->Connected()==false) //Steven 20200908 : 加上有無連線的判斷
//                    IdHTTPMESSystem->Connect();
                fBarCode->mmoXML->Text=IdHTTPMESSystem->Get(asURL);
//                IdHTTPMESSystem->Disconnect();
            }
            catch(Exception &E) //Steven 20201014 : 增加XML上傳的例外紀錄
            {
                S1.sprintf("Shuttle %d SendHTTPRequest fail exception", iSht+1);
                S2.sprintf("%s - %s (%s)", AnsiString(E.ClassName()), E.Message, asURL);
                RecordProcess(S1, S2);
//                IdHTTPMESSystem->Disconnect();
                iHasErr=-1;
            }
            catch(...)
            {
                S1.sprintf("Shuttle %d SendHTTPRequest fail exception", iSht+1);
                S2.sprintf("(%s)", asURL);
                RecordProcess(S1, S2);
//                IdHTTPMESSystem->Disconnect();
                iHasErr=-1;
            }

            if(iHasErr!=-1)
            {
                PeocessXML(iSht);
            }

            if(iHasErr!=-1)
                fBarCode->mmoXML->Lines->Add("Process FAIL!!");
            else
                fBarCode->mmoXML->Lines->Add("Process done!!");

            fBarCode->mmoXML->Lines->Add("");
            fBarCode->mmoXML->Lines->Add("//XML Process Result========================================");

            fBarCode->mmoXML->Lines->Insert(0, "");
            fBarCode->mmoXML->Lines->Insert(0, asURL);

            FileName.sprintf("%s\\%04d%02d%02d\\%04d-%02d-%02d_%02d-%02d-%02d.%03d_%s_%s_%s_%s.xml",
                              FileNamePath,
                              SystemYear, SystemMonth, SystemDate,
                              SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec,
                              IniConfig.SocketHandlerID, IniConfig.sN23_2_Line, IniConfig.sN23_2_Process, fLotInfo->edtSysLotID->Text);
            fBarCode->mmoXML->Lines->SaveToFile(FileName);
        }
    }
    return iHasErr;
}
//------------------------------------------------------------------------------
void TfrmXml::PeocessXML(int iSht)
{
    bool bHasError=false;                                                       //Steven 20200818 : 增加Murata 2DID狀態的例外處理
    AnsiString Str, S1, S2;
    AnsiString Text, ResultNG, Result, ErrorStr;
    if(iSht==0)
        ptr2DIDSHT=&FLCarryKit;
    else
        ptr2DIDSHT=&BLCarryKit;

    try
    {
        XMLDocument1->LoadFromXML(fBarCode->mmoXML->Text);
        int iCount;

        if(iSht==0)
        {
            for(int i=0; i<fBarCode->strngrdXMLResult1->RowCount; i++)
            {
                for(int j=0; j<fBarCode->strngrdXMLResult1->ColCount; j++)
                {
                    fBarCode->strngrdXMLResult1->Cells[j][i]="";
                }
            }
            fBarCode->strngrdXMLResult1->Cells[1][0]=AnsiString("2DID for SHT1");
            fBarCode->strngrdXMLResult1->Cells[2][0]=AnsiString("Result");
        }
        else
        {
            for(int i=0; i<fBarCode->strngrdXMLResult2->RowCount; i++)
            {
                for(int j=0; j<fBarCode->strngrdXMLResult2->ColCount; j++)
                {
                    fBarCode->strngrdXMLResult2->Cells[j][i]="";
                }
            }
            fBarCode->strngrdXMLResult2->Cells[1][0]=AnsiString("2DID for SHT1");
            fBarCode->strngrdXMLResult2->Cells[2][0]=AnsiString("Result");
        }

        _di_IXMLNode NodePattern=XMLDocument1->DocumentElement->ChildNodes->FindNode("PATTERN");
        _di_IXMLNode NodesIndex0, NodeRow, Node2DID;
        _di_IXMLNodeList lNodePattern, lNodesIndex0, lNodeRow;

        if(NodePattern!=NULL)
        {
            lNodePattern=NodePattern->ChildNodes;
            if(lNodePattern!=NULL && lNodePattern->Count>0)
            {
                NodesIndex0=lNodePattern->FindNode("INDEX0");
                if(NodesIndex0!=NULL)
                {
                    lNodesIndex0=NodesIndex0->ChildNodes;
                    if(lNodesIndex0!=NULL && lNodesIndex0->Count>0)
                    {
                        for(int j=0; j<lNodesIndex0->Count; j++)
                        {
                            NodeRow=lNodesIndex0->Nodes[j];
                            if(NodeRow!=NULL)
                            {
                                lNodeRow=NodeRow->ChildNodes;
                                if(iSht==0)
                                    fBarCode->strngrdXMLResult1->Cells[0][j+1]=j+1;
                                else
                                    fBarCode->strngrdXMLResult2->Cells[0][j+1]=j+1;
                                if(lNodeRow!=NULL && lNodeRow>0)
                                {
                                    iCount=lNodeRow->Count;
                                    ResultNG="";
                                    if(iCount>0)
                                    {
                                        Node2DID=lNodeRow->Nodes[0];
                                        Text=Node2DID->GetText();
                                        if(iCount>=3)
                                        {
                                            ResultNG=lNodeRow->Nodes[2]->GetText();
                                            Result  =lNodeRow->Nodes[1]->GetText();
                                            ErrorStr.sprintf("%s_%s_%s", Text, Result, ResultNG);  //Steven 20200610 : 修改2DID比對XML的Error Log
                                        }

                                        if(ResultNG=="")
                                        {
                                            ResultNG="Good";
                                        }

                                        if(iSht==0)
                                        {
                                            fBarCode->strngrdXMLResult1->Cells[1][j+1]=Text;
                                            fBarCode->strngrdXMLResult1->Cells[2][j+1]=ResultNG;
                                        }
                                        else
                                        {
                                            fBarCode->strngrdXMLResult2->Cells[1][j+1]=Text;
                                            fBarCode->strngrdXMLResult2->Cells[2][j+1]=ResultNG;
                                        }
                                        Str.sprintf("%s = %s", Text, ResultNG);
                                        fBarCode->mmoXML->Lines->Add(Str);

                                        if(ResultNG!="Good")
                                        {
                                            for(int y=0; y<InArmSuck.iShtRow; y++)
                                            {
                                                for(int x=0; x<InArmSuck.iShtCol; x++)
                                                {
                                                    if(ptr2DIDSHT->Item[y][x]!=NULL_IC && ptr2DIDSHT->Item[y][x]!=HAS_NULL_IC &&
                                                       ptr2DIDSHT->cDeviceInf[y][x]!=asBarCodeErrorSend && ptr2DIDSHT->cDeviceInf[y][x]!="")
                                                    {
                                                        if(ptr2DIDSHT->cDeviceInf[y][x]==Text)
                                                        {
                                                            ptr2DIDSHT->PordRec[y][x].AddTestResultRecord(iTestBinCount, ptr2DIDSHT->cSBin[y][x], ErrorStr);
                                                            ptr2DIDSHT->b2DIDNG[y][x]=true;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                bHasError=true;
                            }
                        }
                    }
                    else
                    {
                        bHasError=true;
                    }
                }
                else
                {
                    bHasError=true;
                }
            }
            else
            {
                bHasError=true;
            }
        }
        else
        {
            bHasError=true;
        }
    }
    catch(Exception &E)                                                         //Steven 20201014 : 增加XML上傳的例外紀錄
    {
        S1.sprintf("Shuttle %d PeocessXML fail exception", iSht+1);
        S2.sprintf("%s - %s (%s)", AnsiString(E.ClassName()), E.Message, fBarCode->mmoXML->Text);
        RecordProcess(S1, S2);
        bHasError=true;
    }
    catch(...)
    {
        S1.sprintf("Shuttle %d PeocessXML fail exception", iSht+1);
        S2.sprintf("(%s)", fBarCode->mmoXML->Text);
        RecordProcess(S1, S2);
        bHasError=true;
    }

    if(bHasError)
    {
        for(int y=0; y<InArmSuck.iShtRow; y++)
        {
            for(int x=0; x<InArmSuck.iShtCol; x++)
            {
                if(ptr2DIDSHT->Item[y][x]!=NULL_IC && ptr2DIDSHT->Item[y][x]!=HAS_NULL_IC)
                {
                    ErrorStr.sprintf("%s_NGXML", ptr2DIDSHT->cDeviceInf[y][x]);
                    fBarCode->mmoXML->Lines->Add(ErrorStr);
                    ptr2DIDSHT->PordRec[y][x].AddTestResultRecord(iTestBinCount, ptr2DIDSHT->cSBin[y][x], ErrorStr);
                    ptr2DIDSHT->b2DIDNG[y][x]=true;
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
int TfrmXml::SendTestResultToHttp()
{
    int iHasErr=1;
    AnsiString asURL, result="", S1, S2;
    AnsiString FileNamePath="D:\\HT9045_Log\\HTTP_Log", FileName;

    FileName.sprintf("%s\\%04d%02d\\", FileNamePath, SystemYear, SystemMonth);
    MyForceDirectories(FileName);

    FileName.sprintf("%s\\%04d%02d\\%02d_%s_%s_%s_%s.txt",
                      FileNamePath, SystemYear, SystemMonth, SystemDate,
                      IniConfig.SocketHandlerID, IniConfig.sN23_2_Line, IniConfig.sN23_2_Process, fLotInfo->edtSysLotID->Text);

    if(TestSocket.HasRealIC())
    {
        for(int i=0; i<MAX_SOCKET_ROW; i++)
        {
            for(int j=0; j<MAX_SOCKET_COL; j++)
            {
                if(TestSocket.Item[i][j]!=NULL_IC && TestSocket.Item[i][j]!=HAS_NULL_IC)
                {
                    if(TestSocket.b2DIDNG[i][j]==true)
                        result="ERROR";
                    else if(TestSocket.Item[i][j]>=TEST_PASS+iTestBinCount || TestSocket.Item[i][j]<0)
                        result="NG";
                    else if(Prod.bIsPassBin[(TestSocket.Item[i][j]-TEST_PASS)]==false)     //Steven 20200721 : 修正for Murata
                        result="NG";
                    else
                        result="PASS";

                    asURL.sprintf("%s&CDC2627=%s&RESULT=%s&NGCD=%d&PROCESS=%s&LINE=%s&JIG=%d&MACHINE_NO=%s&NOC0027=%s&CDC1826=%s&RETRY=%s&PRASS_MSG=%s&TESTSTART=%s&TESTEND=%s&TESTTIME=%s",
                                  IniConfig.sN23_3_URL,
                                  IniConfig.sN23_2_Product,                     //Product Name
                                  result,                                       //result
                                  TestSocket.iBinData[i][j],                    //Bin No
                                  IniConfig.sN23_2_Process,                     //Procese Name
                                  IniConfig.sN23_2_Line,                        //Line ID
                                  TestSocket.PordRec[i][j].GetSiteNo(),         //site no
                                  IniConfig.SocketHandlerID,                    //MACHINE_NO
                                  fLotInfo->edtSysLotID->Text,                  //LOT ID
                                  TestSocket.cDeviceInf[i][j],                  //IC 2D
                                  fLotInfo->edPage->Text,                       //Retry
                                  TestSocket.PordRec[i][j].GetErrorLog(),       //PRASS_Msg
                                  TestSocket.PordRec[i][j].GetSOTTime(),        //TestStartTime
                                  TestSocket.PordRec[i][j].GetEOTTime(),        //TestEndTime
                                  TestSocket.PordRec[i][j].GetTestTime()        //TestTime
                                  );
                    asURL=StringReplace(asURL, "(null)", "", TReplaceFlags()<<rfReplaceAll);
                    #ifndef SOFT_SIMULTE
                    try
                    {
//                        if(IdHTTPMESSystem->Connected()==false) //Steven 20200908 : 加上有無連線的判斷
//                            IdHTTPMESSystem->Connect();
                        IdHTTPMESSystem->Get(asURL);
//                        IdHTTPMESSystem->Disconnect();
                    }
                    catch(Exception &E) //Steven 20201014 : 增加XML上傳的例外紀錄
                    {
                        S1.sprintf("SendTestResultToHttp fail exception");
                        S2.sprintf("%s - %s (%s)", AnsiString(E.ClassName()), E.Message, asURL);
                        RecordProcess(S1, S2);
//                        IdHTTPMESSystem->Disconnect();
                        iHasErr=-1;
                    }
                    catch(...)
                    {
                        S1.sprintf("SendTestResultToHttp fail exception");
                        S2.sprintf("(%s)", asURL);
                        RecordProcess(S1, S2);
//                        IdHTTPMESSystem->Disconnect();
                        iHasErr=-1;
                    }
                    #endif
                    WriteDataToFile(FileName, asURL);
                }
            }
        }
    }
    return iHasErr;
}
//------------------------------------------------------------------------------
int SendHTTPRequest(int iSht)
{
    if(bResetMode==true)                                                        //Steven 20201130 : RESET模式下, 不要傳送XML
        return 1;

    if(CUSTOMER_CODE==CC_Murata && IniConfig.bN23_1_Enable2DIDCompare &&        //Steven 20200409 : Murata 2DID比對功能
       (fLotInfo->iXMLOnLineStatus==0 || fLotInfo->iXMLOnLineStatus==1))        //Steven 20200629 : Murata要求可以按按鈕後停止Server功能
    {
        return frmXml->SendHTTPRequest(iSht);
    }
    return 1;
}
//------------------------------------------------------------------------------
int SendTestResultToHttp()
{
    if(bResetMode==true)                                                        //Steven 20201130 : RESET模式下, 不要傳送XML
        return 1;

    if(CUSTOMER_CODE==CC_Murata && IniConfig.bN23_3_UploadTestResult &&         //Steven 20200617 : Murata上傳測試結果
       (fLotInfo->iXMLOnLineStatus==0 || fLotInfo->iXMLOnLineStatus==1))        //Steven 20200629 : Murata要求可以按按鈕後停止Server功能
    {
        return frmXml->SendTestResultToHttp();
    }
    return 1;
}
//------------------------------------------------------------------------------
bool ProcessLotInfo(AnsiString S2)                                              //JerryYang 20220923 : 修改SPIL LOT INFO
{
    AnsiString strLotID, sLotCnt, str1, sLOTCount,
               sCUS, sInnLotID, sCustID, sCustDevGup, sDevName, sStage, sStep, sReportCnt, sTemper, sTestID, sProName, sTestBin, sHandlerID, sOPID, sBinSet="",
               sCUSArr[5],strLotIDArr[5],sCustIDArr[5],sCustDevGupArr[5], sDevNameArr[5],sStageArr[5], sStepArr[5],sReportCntArr[5], sBinSetArr[5],
               sProNameArr[5], sTestBinArr[5], sTestIDArr[5], sHandlerIDArr[5], sTemperArr[5], sLotCntArr[5], sOPIDArr[5];

    int iIndex=-1, iLotCount=0;
    frmXml->XMLDocument1->LoadFromXML(S2);
//    frmXml->XMLDocument1->LoadFromFile("D:\\test.xml");     //test
    _di_IXMLNode RNode=frmXml->XMLDocument1->DocumentElement;                   //Get access violations on this line
    _di_IXMLNode NodesIndex0;

    iSecsGemSwitchFTRT=0;                                                       //JerryYang 20250905 : add for remote switch FT/RT

    if(RNode!=NULL)
    {
        str1=RNode->GetNodeName();
        if(str1=="LOT_INFOS" && RNode->HasAttribute("LOT_COUNT"))               //Multi lot information
        {
            sLOTCount=RNode->GetAttribute("LOT_COUNT");
            iIndex=RNode->ChildNodes->IndexOf("LOT_INFO");
            iLotCount=RNode->ChildNodes->GetCount();

            for(int i=0; i<iLotCount; i++)
            {
                iIndex=RNode->ChildNodes->Nodes[i]->ChildNodes->IndexOf("CUSTOMER");
                if(iIndex>=0)
                {
                    sCUSArr[i]=RNode->ChildNodes->Nodes[i]->ChildNodes->Nodes[iIndex]->Text;
                }

                iIndex=RNode->ChildNodes->Nodes[i]->ChildNodes->IndexOf("INNER_LOT_ID");  //****Lot ID*****
                if(iIndex>=0)
                {
                    strLotIDArr[i]=RNode->ChildNodes->Nodes[i]->ChildNodes->Nodes[iIndex]->Text;
                }

                iIndex=RNode->ChildNodes->Nodes[i]->ChildNodes->IndexOf("CUST_LOT_ID");
                if(iIndex>=0)
                {
                    sCustIDArr[i]=RNode->ChildNodes->Nodes[i]->ChildNodes->Nodes[iIndex]->Text;
                }

                iIndex=RNode->ChildNodes->Nodes[i]->ChildNodes->IndexOf("CUSTOMER_DEVICE_GROUP");
                if(iIndex>=0)
                {
                    sCustDevGupArr[i]=RNode->ChildNodes->Nodes[i]->ChildNodes->Nodes[iIndex]->Text;
                }

                iIndex=RNode->ChildNodes->Nodes[i]->ChildNodes->IndexOf("DEVICE_NAME");
                if(iIndex>=0)
                {
                    sDevNameArr[i]=RNode->ChildNodes->Nodes[i]->ChildNodes->Nodes[iIndex]->Text;
                }

                iIndex=RNode->ChildNodes->Nodes[i]->ChildNodes->IndexOf("BIN_SETTING");    //Jerryyang 20250529 : add
                if(iIndex>=0)
                {
                    sBinSetArr[i]=RNode->ChildNodes->Nodes[i]->ChildNodes->Nodes[iIndex]->Text;
                }

                iIndex=RNode->ChildNodes->Nodes[i]->ChildNodes->IndexOf("STAGE");
                if(iIndex>=0)
                {
                    sStageArr[i]=RNode->ChildNodes->Nodes[i]->ChildNodes->Nodes[iIndex]->Text;
                }

                iIndex=RNode->ChildNodes->Nodes[i]->ChildNodes->IndexOf("STEP");
                if(iIndex>=0)
                {
                    sStepArr[i]=RNode->ChildNodes->Nodes[i]->ChildNodes->Nodes[iIndex]->Text;
                }

                iIndex=RNode->ChildNodes->Nodes[i]->ChildNodes->IndexOf("REPORTCOUNT");
                if(iIndex>=0)
                {
                    sReportCntArr[i]=RNode->ChildNodes->Nodes[i]->ChildNodes->Nodes[iIndex]->Text;
                }

                iIndex=RNode->ChildNodes->Nodes[i]->ChildNodes->IndexOf("TEMPERATURE");
                if(iIndex>=0)
                {
                    sTemperArr[i]=RNode->ChildNodes->Nodes[i]->ChildNodes->Nodes[iIndex]->Text;
                }

                iIndex=RNode->ChildNodes->Nodes[i]->ChildNodes->IndexOf("TESTER_ID");
                if(iIndex>=0)
                {
                    sTestIDArr[i]=RNode->ChildNodes->Nodes[i]->ChildNodes->Nodes[iIndex]->Text;
                }

                iIndex=RNode->ChildNodes->Nodes[i]->ChildNodes->IndexOf("PROGRAM_NAME");
                if(iIndex>=0)
                {
                    sProNameArr[i]=RNode->ChildNodes->Nodes[i]->ChildNodes->Nodes[iIndex]->Text;
                }

                iIndex=RNode->ChildNodes->Nodes[i]->ChildNodes->IndexOf("TEST_BIN_NO");
                if(iIndex>=0)
                {
                    sTestBinArr[i]=RNode->ChildNodes->Nodes[i]->ChildNodes->Nodes[iIndex]->Text;
                }

                iIndex=RNode->ChildNodes->Nodes[i]->ChildNodes->IndexOf("HANDLER_ID");
                if(iIndex>=0)
                {
                    sHandlerIDArr[i]=RNode->ChildNodes->Nodes[i]->ChildNodes->Nodes[iIndex]->Text;
                }

                iIndex=RNode->ChildNodes->Nodes[i]->ChildNodes->IndexOf("CURR_QTY");   //****Lot QTY*****
                if(iIndex>=0)
                {
                    sLotCntArr[i]=RNode->ChildNodes->Nodes[i]->ChildNodes->Nodes[iIndex]->Text;
//                    fSCKART->sInfo_CurrQty=sLotCnt;
                }

                iIndex=RNode->ChildNodes->Nodes[i]->ChildNodes->IndexOf("OPERATOR_ID");
                if(iIndex>=0)
                {
                    sOPIDArr[i]=RNode->ChildNodes->Nodes[i]->ChildNodes->Nodes[iIndex]->Text;
                }
            }
        }
        else
        {
            iIndex=RNode->ChildNodes->IndexOf("CUSTOMER");
            if(iIndex>=0)
            {
                sCUS=RNode->ChildNodes->Nodes[iIndex]->Text;
            }

            iIndex=RNode->ChildNodes->IndexOf("INNER_LOT_ID");                  //****Lot ID*****
            if(iIndex>=0)
            {
                strLotID=RNode->ChildNodes->Nodes[iIndex]->Text;
            }

            iIndex=RNode->ChildNodes->IndexOf("CUST_LOT_ID");
            if(iIndex>=0)
            {
                sCustID=RNode->ChildNodes->Nodes[iIndex]->Text;
            }

            iIndex=RNode->ChildNodes->IndexOf("CUSTOMER_DEVICE_GROUP");
            if(iIndex>=0)
            {
                sCustDevGup=RNode->ChildNodes->Nodes[iIndex]->Text;
            }

            iIndex=RNode->ChildNodes->IndexOf("DEVICE_NAME");
            if(iIndex>=0)
            {
                sDevName=RNode->ChildNodes->Nodes[iIndex]->Text;
            }

            iIndex=RNode->ChildNodes->IndexOf("BIN_SETTING");                   //JerryYang 20250529
            if(iIndex>=0)
            {
                sBinSet=RNode->ChildNodes->Nodes[iIndex]->Text;
            }

            iIndex=RNode->ChildNodes->IndexOf("STAGE");
            if(iIndex>=0)
            {
                sStage=RNode->ChildNodes->Nodes[iIndex]->Text;
            }

            iIndex=RNode->ChildNodes->IndexOf("STEP");
            if(iIndex>=0)
            {
                sStep=RNode->ChildNodes->Nodes[iIndex]->Text;
            }

            iIndex=RNode->ChildNodes->IndexOf("REPORTCOUNT");
            if(iIndex>=0)
            {
                sReportCnt=RNode->ChildNodes->Nodes[iIndex]->Text;
            }

            iIndex=RNode->ChildNodes->IndexOf("TEMPERATURE");
            if(iIndex>=0)
            {
                sTemper=RNode->ChildNodes->Nodes[iIndex]->Text;
            }

            iIndex=RNode->ChildNodes->IndexOf("TESTER_ID");
            if(iIndex>=0)
            {
                sTestID=RNode->ChildNodes->Nodes[iIndex]->Text;
            }

            iIndex=RNode->ChildNodes->IndexOf("PROGRAM_NAME");
            if(iIndex>=0)
            {
                sProName=RNode->ChildNodes->Nodes[iIndex]->Text;
            }

            iIndex=RNode->ChildNodes->IndexOf("TEST_BIN_NO");
            if(iIndex>=0)
            {
                sTestBin=RNode->ChildNodes->Nodes[iIndex]->Text;
            }

            iIndex=RNode->ChildNodes->IndexOf("HANDLER_ID");
            if(iIndex>=0)
            {
                sHandlerID=RNode->ChildNodes->Nodes[iIndex]->Text;
            }

            iIndex=RNode->ChildNodes->IndexOf("CURR_QTY");                      //****Lot QTY*****
            if(iIndex>=0)
            {
                sLotCnt=RNode->ChildNodes->Nodes[iIndex]->Text;
                fSCKART->sInfo_CurrQty=sLotCnt;
            }

            iIndex=RNode->ChildNodes->IndexOf("OPERATOR_ID");
            if(iIndex>=0)
            {
                sOPID=RNode->ChildNodes->Nodes[iIndex]->Text;
            }
        }

        bool bChangeLotID=false;
        if(HasICUnderMachine()==false && HasAnyICInMachine()==false)
        {
            bChangeLotID=true;
        }
        else
        {
            if(strLotID==fSCKART->sLotID && (strLotID!="" && strLotID!=" "))
            {
                bChangeLotID=false;
            }
            else
            {
                bChangeLotID=true;
            }
        }

        if(LastSet.iTester==_2D_SORT)
        {
        }
        else
        {
            if(IniConfig.bSPILFunction==true &&                                 //Steven 20250825 : for SPIL
               IniConfig.bA37LotStartLotEnd==false)
            {
                AnsiString sPath=AuthPath+"config.ini";
                IniConfig.bA37LotStartLotEnd=true;
                IniConfig.bA37LotStartLotEnd=ReadWriteIni(sPath, "Function", "bA32LotStartLotEnd", IniConfig.bA37LotStartLotEnd, false, false);
            }
        }

        fObserver->memoLotSummary->Clear();
        if(bChangeLotID==true)
        {
            fSCKART->iCurrent93KARTStep=1;
            fSCKART->iCurrentFlexARTStep=4;
            fMain->Clarn_Data(1, "ART_INPUTQTY");
            fSCKART->ClearLotInfo();
            RecordProcess("ART INPUTQTY.");
            fLotInfo->btClearBarcodeList->Click();                              //Steven 20190214 : 統一清除2DID方式

            slDupBundlID->Clear();
            slDupBundlID->SaveToFile(asDupBundleID);

            slDupUnloadBundlID->Clear();
            slDupUnloadBundlID->SaveToFile(aslDupUnloadBundlID);

            if(IniConfig.bA68_AutoLoadUnload)                                   //Jerryyang 20250529 : add
            {
                if(sBinSet.Pos("FT")==1)                                        //JerryYang 20240821 : 矽品佑騰要求切FT/RT
                {
                    iSecsGemSwitchFTRT=1;                                       //JerryYang 20250905 : add for remote switch FT/RT
                    fMain->DoFTRTClick(false, false);                           //RogerYang 20260410 : 整合並區分手動按下還是程式按下
                }
                else if(sBinSet.Pos("RT")==1)
                {
                    iSecsGemSwitchFTRT=1;                                       //JerryYang 20250905 : add for remote switch FT/RT
                    fMain->DoFTRTClick(true, false);                                    //RogerYang 20260410 : 整合並區分手動按下還是程式按下
                }
            }
            else
            {
                if(sStep.Pos("FT")==1)                                          //JerryYang 20240821 : 矽品佑騰要求切FT/RT
                {
                    iSecsGemSwitchFTRT=1;                                       //JerryYang 20250905 : add for remote switch FT/RT
                    fMain->DoFTRTClick(false, false);                           //RogerYang 20260410 : 整合並區分手動按下還是程式按下
                }
                else if(sStep.Pos("RC")==1)
                {
                    iSecsGemSwitchFTRT=1;                                       //JerryYang 20250905 : add for remote switch FT/RT
                    fMain->DoFTRTClick(true, false);                                    //RogerYang 20260410 : 整合並區分手動按下還是程式按下
                }
            }

            fSCKART->iInfo_MultiLotCnt=iLotCount;
            fSCKART->SetLotStatus(fSCKART->iLOTSTATUS_W);
            fSCKART->sLotID.sprintf("%s", strLotID);
            fSCKART->sLotStartTime      =Now().FormatString("yyyymmddhhnnss");
            fSCKART->sInfo_Customer     =sCUS;
            fSCKART->sInfo_CustLotID    =sCustID;
            fSCKART->sInfo_CustDevGup   =sCustDevGup;
            fSCKART->sInfo_DeviceName   =sDevName;
            fSCKART->sInfo_Stage        =sStage;
            fSCKART->sInfo_Step         =sStep;
            fSCKART->sInfo_ReportCnt    =sReportCnt;
            fSCKART->sInfo_ProgramName  =sProName;
            fSCKART->sInfo_TestBinNo    =sTestBin;
            fSCKART->sInfo_TesterID     =sTestID;
            fSCKART->sInfo_HandlerID    =sHandlerID;
            fSCKART->sInfo_Temperauture =sTemper;
            fSCKART->sInfo_CurrQty      =sLotCnt;
            fSCKART->sInfo_OperatorID   =sOPID;
            fSCKART->sInfo_InnerLotID   =strLotID;
            fSCKART->sInfo_BinSet       =sBinSet;                               //Jerryyang 20250529 : add

            if(fSCKART->iInfo_MultiLotCnt>0)
            {
                for(int i=0; i<fSCKART->iInfo_MultiLotCnt; i++)
                {
                    fSCKART->sInfoArr_Customer[i]       =sCUSArr[i];
                    fSCKART->sInfoArr_InnerLotID[i]     =strLotIDArr[i];
                    fSCKART->sInfoArr_CustLotID[i]      =sCustIDArr[i];
                    fSCKART->sInfoArr_CustDevGup[i]     =sCustDevGupArr[i];
                    fSCKART->sInfoArr_DeviceName[i]     =sDevNameArr[i];
                    fSCKART->sInfoArr_Stage[i]          =sStageArr[i];
                    fSCKART->sInfoArr_Step[i]           =sStepArr[i];
                    fSCKART->sInfoArr_ReportCnt[i]      =sReportCntArr[i];
                    fSCKART->sInfoArr_ProgramName[i]    =sProNameArr[i];
                    fSCKART->sInfoArr_TestBinNo[i]      =sTestBinArr[i];
                    fSCKART->sInfoArr_TesterID[i]       =sTestIDArr[i];
                    fSCKART->sInfoArr_HandlerID[i]      =sHandlerIDArr[i];
                    fSCKART->sInfoArr_Temperauture[i]   =sTemperArr[i];
                    fSCKART->sInfoArr_CurrQty[i]        =sLotCntArr[i];
                    fSCKART->sInfoArr_OperatorID[i]     =sOPIDArr[i];
                    fSCKART->sInfoArr_BinSet[i]=        sBinSetArr[i];          //Jerryyang 20250529 : add
                }
            }

            if(fSCKART->sInfo_Stage.Pos("QC")==1)
            {
                fMain->palEQC->Color=clYellow;
                fMain->palFT->Color=clBtnFace;
            }
            else
            {
//                fMain->palEQC->Color=clBtnFace;                               //JerryYang 20240821 : modify
//                fMain->palFT->Color=clLime;
            }

            if(bQAModeFlag==true)                                               //Steven 20170830 (wei) : QA mode for ATK ART
            {
                fSCKART->iLotCount  =TestIF_File.iQAModeCount;
                fSCKART->iInputCount=TestIF_File.iQAModeCount;
                fSCKART->AccessFile(false, 1);                                  //必須在轉換模式前先存檔一次, 不然轉模式的裡面會讀取到舊的資料
                SetRunStartMode(rsmInitial_ART);
                bQAModeFlag=false;
            }
            else
            {
                fSCKART->iLotCount  =atoi(sLotCnt.c_str());
                fSCKART->iInputCount=atoi(sLotCnt.c_str());
                fSCKART->AccessFile(false, 1);
            }

            fLotInfo->lbledtStarTime    ->Text=fSCKART->sLotStartTime;          //Steven 20250825 : for SJSM
            fLotInfo->lbledtEndTime     ->Text="";
            fLotInfo->lbledtTesterOsVer ->Text="";
            fLotInfo->lbledtTesterID    ->Text=fSCKART->sInfo_TesterID;
            fLotInfo->edtSysOperatorID  ->Text=fSCKART->sInfo_OperatorID;
            fLotInfo->lbledtCustomer    ->Text=fSCKART->sInfo_Customer;
            fLotInfo->lbledtTestProg    ->Text=fSCKART->sInfo_ProgramName;
            fLotInfo->lbledtDeviceName  ->Text=fSCKART->sInfo_DeviceName;
            fLotInfo->lbledtSubLotNo    ->Text="";
            fLotInfo->lbledtModeCode    ->Text="";
            fLotInfo->lbledtTestCode    ->Text="";
            fLotInfo->lbledtTestBinNo   ->Text=fSCKART->sInfo_TestBinNo;
            if(CUSTOMER_CODE==CC_SJ_Semiconductor)                              //Steven 20251104 : for SJSM
                fLotInfo->cbRunMode     ->Text=fSCKART->sInfo_Step;
            else
                fLotInfo->cbRunMode     ->Text=fSCKART->sInfo_BinSet;
            fLotInfo->edCustomerLotId   ->Text=fSCKART->sInfo_CustLotID;
            fLotInfo->coStation         ->Text="";
            fLotInfo->edStationNum      ->Text="";
            fLotInfo->edtCusLotID       ->Text=fSCKART->sInfo_CustLotID;
            fLotInfo->edtCusDevGrp      ->Text=fSCKART->sInfo_CustDevGup;
            fLotInfo->edPage            ->Text="";
            fLotInfo->cbProcess         ->Text=fSCKART->sInfo_Stage;
            fLotInfo->edtStage          ->Text=fSCKART->sInfo_Stage;
            fLotInfo->edtStep           ->Text=fSCKART->sInfo_Step;

            fLotInfo->SetLotID(fSCKART->sLotID, false);
            fLotInfo->SetLotStart(__FUNC__);

            str1.sprintf("Lot start: %s, %d", fSCKART->sLotID, fSCKART->iLotCount);
            RecordProcess(str1);                                                //Steven 20190722 : add TSV log

            if(fSCKART->iTesterType==1 || IniConfig.bA37LotStartLotEnd)         //Steven 20170309 (wei) : Fixed for ART
            {
                fSCKART->iNeedRT=0;
                LastSet.bEndLotAutoRetestGPIB=false;
                LastSet.bWaitStartLotAutoRetestGPIB=false;
                LastSet.bFirstTestAutoRetestGPIB=true;
                if(LastSet.iRunStartMode==rsmInitial_ART || LastSet.iRunStartMode==rsmContinuStart_ART ||
                   LastSet.iRunStartMode==rsmContinuRetest_ART || LastSet.iRunStartMode==rsmAutoRetest)
                {
                    fMain->SetLotState(2);                                      //XML ART FT Start
                }
                else
                {
                    if(fSCKART->sInfo_Step==FT)
                        fMain->SetLotState(2);                                  //XML ART FT Start
                    else
                        fMain->SetLotState(4);                                  //XML ART RT Start
                }
            }
        }
        else
        {
            fSCKART->sLotID.sprintf("%s", strLotID);
            fSCKART->sLotStartTime=Now().FormatString("yyyymmddhhnnss");
            fSCKART->sInfo_Customer=sCUS;
            fSCKART->sInfo_CustLotID=sCustID;
            fSCKART->sInfo_CustDevGup=sCustDevGup;
            fSCKART->sInfo_DeviceName=sDevName;
            fSCKART->sInfo_Stage=sStage;
            fSCKART->sInfo_Step=sStep;
            fSCKART->sInfo_ReportCnt=sReportCnt;
            fSCKART->sInfo_ProgramName=sProName;
            fSCKART->sInfo_TestBinNo=sTestBin;
            fSCKART->sInfo_TesterID=sTestID;
            fSCKART->sInfo_HandlerID=sHandlerID;
            fSCKART->sInfo_Temperauture=sTemper;
            fSCKART->sInfo_CurrQty=sLotCnt;
            fSCKART->sInfo_OperatorID=sOPID;
            fSCKART->sInfo_BinSet=sBinSet;                                          //Jerryyang 20250529 : add

            if(fSCKART->sInfo_Stage.Pos("QC")==1)
            {
                fMain->palEQC->Color=clYellow;
                fMain->palFT->Color=clBtnFace;
            }
            else
            {
                fMain->palEQC->Color=clBtnFace;
                fMain->palFT->Color=clLime;
            }

            fSCKART->iInfo_MultiLotCnt=iLotCount;

            if(fSCKART->iInfo_MultiLotCnt>0)
            {
                for(int i=0; i<fSCKART->iInfo_MultiLotCnt; i++)
                {
                    fSCKART->sInfoArr_Customer[i]=sCUSArr[i];
                    fSCKART->sInfoArr_InnerLotID[i]=strLotIDArr[i];
                    fSCKART->sInfoArr_CustLotID[i]=sCustIDArr[i];
                    fSCKART->sInfoArr_CustDevGup[i]=sCustDevGupArr[i];
                    fSCKART->sInfoArr_DeviceName[i]=sDevNameArr[i];
                    fSCKART->sInfoArr_Stage[i]=sStageArr[i];
                    fSCKART->sInfoArr_Step[i]=sStepArr[i];
                    fSCKART->sInfoArr_ReportCnt[i]=sReportCntArr[i];
                    fSCKART->sInfoArr_ProgramName[i]=sProNameArr[i];
                    fSCKART->sInfoArr_TestBinNo[i]=sTestBinArr[i];
                    fSCKART->sInfoArr_TesterID[i]=sTestIDArr[i];
                    fSCKART->sInfoArr_HandlerID[i]=sHandlerIDArr[i];
                    fSCKART->sInfoArr_Temperauture[i]=sTemperArr[i];
                    fSCKART->sInfoArr_CurrQty[i]=sLotCntArr[i];
                    fSCKART->sInfoArr_OperatorID[i]=sOPIDArr[i];
                    fSCKART->sInfoArr_BinSet[i]=sBinSetArr[i];                    //Jerryyang 20250529 : add
                }
            }
            fSCKART->AccessFile(false, 1);
        }
//        #ifndef FOR_NVIDIA_2D_SORT
        if(IniConfig.bSPILFunction && IniConfig.bRCMDStart==false && TestIF_File.bSortingBy2DIDList==false)  //JerryYang 20230222 : 中科佳毅要求切換工作檔時開啟run check
        {
            AnsiString sPath=AuthPath+"config.ini";
            IniConfig.bRCMDStart=true;
            IniConfig.bRCMDStart=ReadWriteIni(sPath, "SECS GEM", "Enable RCMD START", IniConfig.bRCMDStart, false, false);
            fConfiguration->cbN07_EnableHostStart->Checked=true;                //JerryYang 20250826 : 元件也要強制打勾避免進溫度頁面按存檔又被關閉
        }

        if(iSecsGemSwitchFTRT==1)                                               //JerryYang 20250905 : add for remote switch FT/RT
            return false;
        iSecsGemSwitchFTRT=0;

//        #endif
        return true;
    }
//    }
    return false;
}
//------------------------------------------------------------------------------
int Process_2DID_BinCode(AnsiString s2DID_Source)                               //Steven 20231111 : 調整XML讀取
{
    int HCACK=0, temp=0;
    AnsiString sLotNo, S2, sDeviceID="", sBin="", s3, str2, str1, str4, str5, strSort, str;
    frmXml->XMLDocument1->LoadFromXML(s2DID_Source);
    _di_IXMLNode RNode;                                                         //Get access violations on this line
    _di_IXMLNodeList nodelst;                                                   //nodelst結點列表指向<enable>,<ladder>…這//一層結點
    _di_IXMLNode BInodelst;                                                     //nodelst結點列表指向<enable>,<ladder>…這//一層結點
    _di_IXMLNodeList BInodelst1;                                                //nodelst結點列表指向<enable>,<ladder>…這//一層結點

    RNode=frmXml->XMLDocument1->DocumentElement;                                //Get access violations on this line

    TStringList *l2DSorting;
    l2DSorting=new TStringList;
    l2DSorting->Clear();

    sl2DMappingLog->Clear();
    sl2DMappingLog->Add("2D Code, Binning, ECID, Device, Lot No., VS-shuttle, VS-unload, RC1-shuttle, RC1-unload, RC2-shuttle, RC2-unload");

    if(RNode!=NULL)
    {
        int iIndex=-1;
        iIndex=RNode->ChildNodes->IndexOf("SubstrateMap");                      //取結點列表的第一個結點也就是<SubstrateMap>

        if(iIndex==0)
        {
            nodelst=RNode->ChildNodes;
            RNode=nodelst->operator [](0);                                      //取結點列表的第二個結點也就是<Overlay>
            iIndex=RNode->ChildNodes->IndexOf("Overlay");

            if(iIndex==0 && nodelst->Count==1)
            {
                nodelst=RNode->ChildNodes;
                RNode=nodelst->operator [](0);                                  //取結點列表的第三個結點也就是<DeviceIdMap>
                iIndex=RNode->ChildNodes->IndexOf("DeviceIdMap");
                if(iIndex!=0)
                    iIndex=RNode->ChildNodes->IndexOf("DeviceIDMap");           //Steven 20240919 : 增加名稱判斷

                if(iIndex==0 && nodelst->Count==1)
                {
                    nodelst=RNode->ChildNodes;
                    RNode=nodelst->operator [](0);                              //取結點列表的第四個結點也就是<Id>
                    iIndex=RNode->ChildNodes->IndexOf("Id");
                    nodelst=RNode->ChildNodes;                                  //取結點列表的第四個結點也就是<Id>

                    if(TestIF_File.b2DIDAllowList)                              //JerryYang 20250320 : 2DID白名單功能
                    {
                        fBarCode->list2DWhitle->Clear();
                        fBarCode->list2DWhitleResult->Clear();
                    }

                    for(int i=0; i<nodelst->Count; i++)
                    {
                        sDeviceID=RNode->ChildNodes->Nodes[i]->Text;            //取得deviceID
                        sBin=RNode->ChildNodes->Nodes[i]->GetAttribute("Bin");  //取得BIN

                        if(sDeviceID!="" && sBin!="")
                        {
                            l2DSorting->Add(sDeviceID+","+sBin);
                            sl2DMappingLog->Add(sDeviceID+","+sBin+",,"+fLotInfo->edtDevice->Text+","+fLotInfo->edtCusLotID->Text+",,,,,,");
                            if(TestIF_File.b2DIDAllowList && IniConfig.iN23DownloadMethod==2)   //JerryYang 20250320 : 2DID白名單功能
                            {
                                fBarCode->list2DWhitle->Add(sDeviceID);
                                fBarCode->list2DWhitleResult->Add(sDeviceID);
                            }
                        }
                        else if(sDeviceID!="")                                  //JerryYang 20250320 : 2DID白名單功能
                        {
                            if(TestIF_File.b2DIDAllowList && IniConfig.iN23DownloadMethod==2) //SECS
                            {
                                fBarCode->list2DWhitle->Add(sDeviceID);
                                fBarCode->list2DWhitleResult->Add(sDeviceID);
                            }
                        }
                    }

                    MyForceDirectories("D:\\HT9045_Log\\2D_SortList\\");
                    MyForceDirectories("D:\\HT9045_Log\\2D_MappingResult\\");

                    str2.sprintf("D:\\HT9045_Log\\2D_MappingResult\\%s_%s_%s_VS_Result.csv", fLotInfo->edtSysLotID->Text, fLotInfo->edtCusLotID->Text, fLotInfo->edtCusDevGrp->Text);
                    if(FileExists(str2)==false)                                 //Steven 20160505 : 加上保護, 不然開程式會跳Error
                        sl2DMappingLog->SaveToFile(str2);

                    str2.sprintf("%s\\SortBy2DID_%s.csv", "D:\\HT9045_Log\\2D_SortList", fLotInfo->edtSysLotID->Text);
                    l2DSorting->SaveToFile(str2);

                    if(FileExists(str2))                                        //Steven 20160505 : 加上保護, 不然開程式會跳Error
                    {
                        fSCKART->sLotID=fLotInfo->edtSysLotID->Text;
                        fBarCode->list2DSorting->Clear();
                        fBarCode->list2DSorting->LoadFromFile(str2);
//                        fBarCode->list2DSorting->Sort();

                        for(int i=0; i<fBarCode->list2DSorting->Count; i++)
                        {
                            str=fBarCode->list2DSorting->Strings[i];
                            str4=StringReplace(str, '"', "", TReplaceFlags()<<rfReplaceAll);

                            str5=StringReplace(str4, " ", "_", TReplaceFlags()<<rfReplaceAll);
                            fBarCode->s2DSorting->CommaText=str5;

                            str1=StringReplace(str, ",", "=", TReplaceFlags()<<rfReplaceAll);
                            strSort=StringReplace(str1, '"', "", TReplaceFlags()<<rfReplaceAll);
                            fBarCode->list2DSorting->Strings[i]=strSort;

                            int iBin=StrToIntDef(fBarCode->s2DSorting->Strings[1], iTestBinCount);
                            if(iBin>=0 && iBin<iTestBinCount)
                            {
                                iByBinCnt[iBin]++;
                            }
                        }

                        for(int i=0; i<6; i++)
                        {
                            for(int j=0; j<iTestBinCount; j++)
                            {
                                temp=Prod.iT6CatData[j];
                                if(temp<=0)
                                    continue;

                                if(i==temp-1)
                                {
                                    iExceptAutoCnt[i]+=iByBinCnt[j];
                                }
                            }
                        }

                        fBarCode->TransformListToMap();                         //Steven 20240515 : modified for 2D sort
                        int sum=0, Sum_ART=0;
                        double f=0.0, f1=0.0;
                        for(int i=0; i<6; i++)
                        {
                            sum+=LastSet.BinCT[0][i];
                            Sum_ART+=LastSet.BinCT_ART[0][i];                   //wei 20150923 add ART計數
                        }
                        int iTemp=0;

                        for(int i=0; i<iTestBinCount+1; i++)                    //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
                        {
                            if(LastSet.iTester==_2D_SORT)
                            {
                                fShowBinSelect->StrGrdCategory->Cells[0][1+i]="";
                                fShowBinSelect->StrGrdCategory->Cells[1][1+i]="";
                                fShowBinSelect->StrGrdCategory->Cells[2][1+i]="";
                                fShowBinSelect->StrGrdCategory->Cells[3][1+i]="";
                                if(iByBinCnt[i]>0 || i==iTestBinCount)
                                {
                                    if(i==iTestBinCount)
                                    {
                                        fShowBinSelect->StrGrdCategory->Cells[0][1+iTemp]="Error Bin";
                                    }
                                    else
                                    {
                                        fShowBinSelect->StrGrdCategory->Cells[0][1+iTemp]="BIN "+AnsiString(i);
                                    }
                                    fShowBinSelect->StrGrdCategory->Cells[2][1+iTemp]=LastSet.iBinData32[0][i];
                                    fShowBinSelect->StrGrdCategoryART->Cells[1][1+iTemp]=LastSet.iBinData32_ART[0][i];      //wei 20150923 add ART計數
                                    fShowBinSelect->StrGrdCategory->Cells[1][1+iTemp]=iByBinCnt[i];
                                    if(sum>0)
                                    {
                                        f=ChangeToFloat((double)LastSet.iBinData32[0][i], (double)sum);
                                        fShowBinSelect->StrGrdCategory->Cells[3][1+iTemp]=(AnsiString)GetFloatFormatString(f, 5, 2)+(AnsiString)("%");
                                    }
                                    else
                                    {
                                        fShowBinSelect->StrGrdCategory->Cells[3][1+iTemp]="0.00%";
                                    }

                                    if((USE_AUTO_RETEST==eartInstall && IniConfig.bA10_AutoReTest) || CosFunction.bUseARTSortCount)    //wei 20150923 add ART計數  //Ifor 20170316 (wei) add MRT Mode
                                    {
                                        if(Sum_ART>0)
                                        {
                                            f1=ChangeToFloat((double)LastSet.iBinData32_ART[0][i], (double)Sum_ART);
                                            fShowBinSelect->StrGrdCategoryART->Cells[2][1+iTemp]=(AnsiString)GetFloatFormatString(f1, 5, 2)+(AnsiString)("%");
                                        }
                                        else
                                        {
                                            fShowBinSelect->StrGrdCategoryART->Cells[2][1+iTemp]="0.00%";
                                        }
                                    }
                                    iTemp++;
                                }
                            }
                        }
                        HCACK=0;
                    }
                }
                else
                {
                    HCACK=3;
                }
            }
            else
            {
                HCACK=3;
            }
        }
        else
        {
            HCACK=3;
        }
    }
    else
    {
        HCACK=3;
    }
    l2DSorting->Clear();
    delete l2DSorting;
    return HCACK;
}
//------------------------------------------------------------------------------
int S2F42_SET_2DID_BIN_CODE(AnsiString s2DID_Source)                            //Steven 20241008 : S2F42的XML換位置
{
    int HCACK=0;
    AnsiString sDeviceID, sBin, sXY, sECID, sTempID, sTempBin, strSort;
    AnsiString str1, str2, str4, str5, str8, sLotNo, sLOTCount;
    int iEnd;

    _di_IXMLNode RNode;
    _di_IXMLNodeList nodelst, nodelst2;

    frmXml->XMLDocument1->LoadFromXML(s2DID_Source);
    RNode=frmXml->XMLDocument1->DocumentElement;                                //Get access violations on this line

    TStringList *l2DSorting;
    l2DSorting=new TStringList;
    l2DSorting->Clear();
    int iLotCnt=0;

    fBarCode->list2DWhitle->Clear();                                            //JerryYang 20250320 : 2DID白名單功能
    sl2DMappingLog->Clear();
    sl2DMappingLog->Add("2D Code, Binning, ECID, Device, Lot No., VS-shuttle, VS-unload, RC1-shuttle, RC1-unload, RC2-shuttle, RC2-unload, VS-Error log, RC1-Error log, RC2-Error log");

    str1=RNode->GetNodeName();
    if(str1=="SUBSTRATEMAPS" &&
       RNode->HasAttribute("LOT_COUNT"))                                        //Multi lot information
    {
        sLOTCount=RNode->GetAttribute("LOT_COUNT");
        iLotCnt=atoi(sLOTCount.c_str());
        fSCKART->iInfo_MultiLotCnt=iLotCnt;
    }

    if(RNode!=NULL)
    {
        int iIndex=-1;
        iIndex=RNode->ChildNodes->IndexOf("SUBSTRATEMAP");                      //取結點列表的第一個結點也就是<SubstrateMap>

        if(iIndex==0)
        {
            iLotCnt=RNode->ChildNodes->GetCount();

            nodelst=RNode->ChildNodes;
            for(int k=0; k<nodelst->Count; k++)
            {
                RNode=nodelst->operator [](k);

                if(RNode->HasAttribute("LOT_NO"))                               //Multi lot information
                {
                    sLotNo=RNode->GetAttribute("LOT_NO");
                    sTotalLotID=sTotalLotID+sLotNo+";";
                }

                iIndex=RNode->ChildNodes->IndexOf("ID");
                if(iIndex==0)
                {
                    nodelst2=RNode->ChildNodes;                                 //取結點列表的第四個結點也就是<Id>
                    for(int i=0; i<nodelst2->Count; i++)
                    {
                        sDeviceID=RNode->ChildNodes->Nodes[i]->Text;            //取得deviceID
                        sBin="";

                        if(RNode->ChildNodes->Nodes[i]->HasAttribute("BIN"))
                        {
                            sBin=RNode->ChildNodes->Nodes[i]->GetAttribute("BIN");  //取得BIN
                        }

                        if(sDeviceID!="" && sBin!="")
                        {
                            l2DSorting->Add(sDeviceID+","+sBin+","+sXY+","+ fLotInfo->edtCusDevGrp->Text+","+ fLotInfo->edtCusLotID->Text+","+sLotNo);
                            sl2DMappingLog->Add(sDeviceID+","+sBin+","+sECID+","+","+sLotNo+",,,,,,,,,");
                            if(TestIF_File.b2DIDAllowList && IniConfig.iN23DownloadMethod==2)  //JerryYang 20250320 : 2DID白名單功能
                            {
                                fBarCode->list2DWhitle->Add(sDeviceID);
                                fBarCode->list2DWhitleResult->Add(sDeviceID);
                            }
                        }
                        else if(sDeviceID!="")                                  //JerryYang 20250320 : 2DID白名單功能
                        {
                            if(TestIF_File.b2DIDAllowList && IniConfig.iN23DownloadMethod==2)
                            {
                                fBarCode->list2DWhitle->Add(sDeviceID);
                                fBarCode->list2DWhitleResult->Add(sDeviceID);
                            }
                        }
                    }
                }
            }

            MyForceDirectories("D:\\HT9045_Log\\2D_SortList\\", "S2F42_SET_2DID_BIN_CODE");
            MyForceDirectories("D:\\HT9045_Log\\2D_MappingResult\\", "S2F42_SET_2DID_BIN_CODE");
            str2.sprintf("D:\\HT9045_Log\\2D_MappingResult\\MultiLot_VS_Result_%s.csv", sTotalLotID);

            if(FileExists(str2))
                DeleteFile(str2);

            if(FileExists(str2)==false)                                         //Steven 20160505 : 加上保護, 不然開程式會跳Error
                sl2DMappingLog->SaveToFile(str2);

            str2.sprintf("%s\\SortBy2DID_%s.csv", "D:\\HT9045_Log\\2D_SortList", sTotalLotID);
            l2DSorting->SaveToFile(str2);

            if(FileExists(str2))                                                //Steven 20160505 : 加上保護, 不然開程式會跳Error
            {
                fBarCode->list2DSorting->Clear();
                fBarCode->list2DSorting->LoadFromFile(str2);

                for(int i=0; i<fBarCode->list2DSorting->Count; i++)
                {
                    str8=fBarCode->list2DSorting->Strings[i];
                    str4=StringReplace(str8, '"', "", TReplaceFlags()<<rfReplaceAll);    //刪除雙引號

                    str5=StringReplace(str4, " ", "_", TReplaceFlags()<<rfReplaceAll);
                    fBarCode->s2DSorting->CommaText=str5;

                    iEnd=str4.Pos(",");
                    if(iEnd>1)
                    {
                        sTempID=str4.SubString(1, iEnd-1);
                        sTempBin=str4.SubString(iEnd+1, str4.Length()-iEnd);
                        iEnd=sTempBin.Pos(",");
                        sTempBin=sTempBin.SubString(1, iEnd-1);
                    }

                    str1=sTempID+"="+sTempBin;
                    strSort=StringReplace(str1, '"', "", TReplaceFlags()<<rfReplaceAll);
                    fBarCode->list2DSorting->Strings[i]=strSort;

                    int iBin=StrToIntDef(fBarCode->s2DSorting->Strings[1], iTestBinCount);
                    if(iBin>=0 && iBin<iTestBinCount)
                    {
                        iByBinCnt[iBin]++;
                    }
                }
                fBarCode->TransformListToMap();                                 //Steven 20240515 : modified for 2D sort

                int sum=0, Sum_ART=0;
                double f=0.0, f1=0.0;
                for(int i=0; i<6; i++)
                {
                    sum+=LastSet.BinCT[0][i];
                    Sum_ART+=LastSet.BinCT_ART[0][i];                           //wei 20150923 add ART計數
                }
                int iTemp=0;

                for(int i=0; i<iTestBinCount+1; i++)                            //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
                {
                    if(LastSet.iTester==_2D_SORT)
                    {
                        fShowBinSelect->StrGrdCategory->Cells[0][1+i]="";
                        fShowBinSelect->StrGrdCategory->Cells[1][1+i]="";
                        fShowBinSelect->StrGrdCategory->Cells[2][1+i]="";
                        fShowBinSelect->StrGrdCategory->Cells[3][1+i]="";
                        if(iByBinCnt[i]>0 || i==iTestBinCount)
                        {
                            if(i==iTestBinCount)
                            {
                                fShowBinSelect->StrGrdCategory->Cells[0][1+iTemp]="Error Bin";
                            }
                            else
                            {
                                fShowBinSelect->StrGrdCategory->Cells[0][1+iTemp]="BIN "+AnsiString(i);
                            }
                            fShowBinSelect->StrGrdCategory->Cells[2][1+iTemp]=LastSet.iBinData32[0][i];
                            fShowBinSelect->StrGrdCategoryART->Cells[1][1+iTemp]=LastSet.iBinData32_ART[0][i];      //wei 20150923 add ART計數
                            fShowBinSelect->StrGrdCategory->Cells[1][1+iTemp]=iByBinCnt[i];
                            if(sum>0)
                            {
                                f=ChangeToFloat((double)LastSet.iBinData32[0][i], (double)sum);                     //Steven 20250820 : 針對除以0加上保護
                                fShowBinSelect->StrGrdCategory->Cells[3][1+iTemp]=(AnsiString)GetFloatFormatString(f, 5, 2)+(AnsiString)("%");
                            }
                            else
                            {
                                fShowBinSelect->StrGrdCategory->Cells[3][1+iTemp]="0.00%";
                            }

                            if((USE_AUTO_RETEST==eartInstall &&
                                IniConfig.bA10_AutoReTest) ||                   //Ifor 20170316 (wei) add MRT Mode
                               CosFunction.bUseARTSortCount)                    //wei 20150923 add ART計數
                            {
                                if(Sum_ART>0)
                                {
                                    f1=ChangeToFloat((double)LastSet.iBinData32_ART[0][i], (double)Sum_ART);
                                    fShowBinSelect->StrGrdCategoryART->Cells[2][1+iTemp]=(AnsiString)GetFloatFormatString(f1, 5, 2)+(AnsiString)("%");
                                }
                                else
                                {
                                    fShowBinSelect->StrGrdCategoryART->Cells[2][1+iTemp]="0.00%";
                                }
                            }
                            iTemp++;
                        }
                    }
                }
                fSCKART->AccessFile(false, 1);
                HCACK=0;
            }
            else
            {
                HCACK=8;
            }
        }
        else
        {
            HCACK=7;
        }
    }
    else
    {
        HCACK=3;
    }

    delete l2DSorting;

    return HCACK;
}
//------------------------------------------------------------------------------
int ASECL_LotStart(AnsiString Source)                                           //Steven 20260408 : ASECL_LotStart的XML換位置
{
    _di_IXMLNode RNode;                                                         //Get access violations on this line
    _di_IXMLNodeList nodelst;                                                   //nodelst結點列表指向<enable>,<ladder>…這//一層結點
    _di_IXMLNode BInodelst;                                                     //nodelst結點列表指向<enable>,<ladder>…這//一層結點
    _di_IXMLNodeList BInodelst1;                                                //nodelst結點列表指向<enable>,<ladder>…這//一層結點
    AnsiString sDeviceID;

    frmXml->XMLDocument1->LoadFromFile(Source);
    RNode=frmXml->XMLDocument1->DocumentElement;                                //Get access violations on this line

    if(RNode!=NULL)
    {
        int iIndex=-1;
        iIndex=RNode->ChildNodes->IndexOf("PickDeviceId");                      //取結點列表的第一個結點也就是<SubstrateMap>

        if(iIndex==0)
        {
            nodelst=RNode->ChildNodes;
            RNode=nodelst->operator [](0);                                      //取結點列表的第四個結點也就是<Id>
            iIndex=RNode->ChildNodes->IndexOf("Id");
            nodelst=RNode->ChildNodes;                                          //取結點列表的第四個結點也就是<Id>

            fBarCode->list2DWhitle->Clear();

            for(int i=0; i<nodelst->Count; i++)
            {
                sDeviceID=RNode->ChildNodes->Nodes[i]->Text;                    //取得deviceID

                fBarCode->list2DWhitle->Add(sDeviceID);
            }
            fBarCode->list2DWhitle->Sort();
        }
    }
    return 0;
}
//------------------------------------------------------------------------------



