//---------------------------------------------------------------------------

#ifndef aRotateKIT_InH
#define aRotateKIT_InH
//---------------------------------------------------------------------------

void InitialInArmRotateKIT();
bool DoInArmRotateKIT();
bool MoveInArmXY_ToRotateKIT(int iKit);
bool MoveInArmZ_ToRotateKIT_Place(int iKit);
bool MoveInArmZ_ToRotateKIT_Pick(int iKit);

//---------------------------------------------------------------------------
//2013-04-12    Dell :旋轉站;馬達版  底下都是馬達版的Code
//---------------------------------------------------------------------------
bool M_MoveInArmXY_ToRotateKIT();
bool M_MoveInArmZ_ToRotateKIT_Place(int iKit);
bool M_MoveInArmZ_ToRotateKIT_Pick(int iKit, int PickAngle);
bool M_DoInArmRotateKIT_Motor();
bool M_DoShakeInArmRotateKIT();                                                 //JerryYang 20160825 Rotate sensor偵測異常,要先試著旋轉三次再跳alarm
bool M_DoInArmRotate_InOutArm();                                                //add One sucker with rotate

int DoRotate_In(int &rCount, bool bReset=false);
int CheckInRotateSensor(bool bReset=false);                                     //Steven 20170504 (wei) : 針對入料旋轉站的Sensor檢查
bool CheckInRotateAnglePostion(int iSuckR, int iSuckC, int iTrayY, int iTrayX);
int M1_DoInRotateMove(int &rCount, bool bReset=false);
int M1_DoInRotateMove1Mot(int &rCount, bool bReset=false);
int M2_DoInRotateMove(int &rCount, bool bReset=false);
int M4_DoInRotateMove(int &rCount, bool bReset=false);                          //Steven 20170504 (wei) : For 4顆獨立旋轉馬達同時轉動
int M8_DoInRotateMove(int &rCount, bool bReset=false);                          //Steven 20170504 (wei) : For 8顆獨立旋轉馬達同時轉動
bool MoveInRotateToDegreeAtSameTime(int iDegree, bool bInitial=false);          //Steven 20170329 (wei) : Add individual rotate motor
void RecordRotateAngle(int iSuckR, int iSuckC, int iTrayR, int iTrayC);         //Sam 20221103 : 新增In旋轉角度
#endif
