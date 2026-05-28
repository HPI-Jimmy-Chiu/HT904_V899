#ifndef MachineDefH
#define MachineDefH

//系統Header-------------------------
#include <vcl.h>
#include <winbase.h>
#include <stdio.h>
#include <FileCtrl.hpp>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <SysUtils.hpp>
//#include <Math.hpp>
#include <math.h>
#include <fcntl.h>
#include <io.h>
#include <algorithm>
#include <dos.h>
#include <dir.h>
#include <fstream>
#include <iostream.h>
#include <inifiles.hpp>
#include <winsock.h>
#include <ComCtrls.hpp>
#include <vector>
#include <utilcls.h>
#include <Mmsystem.h>
#include <jpeg.hpp>
#include <map.h>
#include <cstdio>
#include <string.h>
#include <mem.h>
#include <masks.hpp>
#include <cstdlib>
#include <functional>

using namespace std;

//Driver-----------------
#include "adamtcp.h"
#include "Dmccom.h"
#include "Type_def.h"
#include "CMNet.h"
#include "CMNETError.h"
#include "PCI_L112.h"
#include "PCI_L122.h"
#include "MN200.h"
#include "mdfunc.h"
#include "sqlite3.h"
//#include "bsapi.h"
#include "PComm.h"
#include "CSmc.h"
//#include "XLSfile.hpp"      //Steven 20140307 mark: 加快Compiler
#include "Hontech_M4.h"
#include "TasmInfo.h"
#include "DNM100API.h"

////公用程式-----------------
#include "iobyte.h"
#include "hsensor.h"
#include "myio.h"
#include "HTimer.h"
#include "halarm.h"
#include "RegExpr.hpp"
#include "myTimer.h"
//#include "HTray.h"
//#include "HTMotor.h"

//不會修改的程式碼-----------------
//#include "cMyDef.h"
//#include "mytray.h"
//#include "MyMotor.h"
//#include "mySYNTEKmotor.h"
//#include "mySMCmotor.h"
//#include "HThreadCtrl.h"
//#include "TCPData.h"
//#include "handlerlog.h"
#include "TextProcess.h"
#include "uruncontrol.h"
//#include "uHeaterThread.h"
#include "language.h"
#include "ScanBtnThread.h"
//#include "TasmInfo.h"
//#include "RunTime.h"
//#include "Fingerprint.h"
//#include "adam6024.h"
//#include "INPUT.h"
//#include "myQwertyKeyBoard.h"
//#include "Password.h"
//#include "cUnitConvert.h"
//#include "cBuilder.h"
//#include "DIOInterFaceCFG.h"
#include "LastSet.h"
#include "WinSocketErrorCode.h"
#include "MyStringList.h"
#include "cMyDB.h"
#include "uhome.h"
#include "cSetUp.h"
#include "uTemp_Set.h"
#include "cTesterIF.h"
#include "cBinSel.h"
#include "cLd_ULd.h"
#include "cTrayForm.h"
#include "cHotPlate.h"
#include "uYieldMonitoring.h"
#include "cTrayAssignment.h"
#include "cContact.h"
#include "cBinSel.h"
#include "cOffSet.h"
#include "cSpeed.h"
#include "cBuilder.h"
#include "MyCCLinkSensor.h"
#include "ContactForce.h"
#include "AutoTeach.h"                      //JimmyChiu 20220901 add AutoTeach.h
#include "AutoAlignment.h"      //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
#endif

