//---------------------------------------------------------------------------

#ifndef aRotateKIT_OutH
#define aRotateKIT_OutH
//---------------------------------------------------------------------------

void InitialOutArmRotateKIT();
bool DoOutArmRotateKIT();
bool MoveOutArmXY_ToRotateKIT(int iKit);
bool MoveOutArmZ_ToRotateKIT_Place(int iKit);
bool MoveOutArmZ_ToRotateKIT_Pick(int iKit);

//---------------------------------------------------------------------------
//2013-04-12    Dell :旋轉站;馬達版  底下都是馬達版的Code
//---------------------------------------------------------------------------
bool M_MoveOutArmXY_ToRotateKIT();
bool M_MoveOutArmZ_ToRotateKIT_Place(int iKit);
bool M_MoveOutArmZ_ToRotateKIT_Pick(int iKit, int PickAngle);
bool M_DoOutArmRotateKIT_Motor();
bool M_DoShakeOutArmRotateKIT();                                                //JerryYang 20160825 Rotate sensor偵測異常,要先試著旋轉三次再跳alarm
bool M_DoOutArmRotate_InOutArm();                                               //add One sucker with rotate

int DoRotate_Out(int &rCount, bool bReset=false);
int CheckOutRotateSensor(bool bReset=false);
int M1_DoOutRotateMove(int &rCount, bool bReset=false);
int M1_DoOutRotateMove1Mot(int &rCount, bool bReset=false);
int M2_DoOutRotateMove(int &rCount, bool bReset=false);                         //Steven 20170504 (wei) : For 4顆獨立旋轉馬達同時轉動
int M4_DoOutRotateMove(int &rCount, bool bReset=false);                         //Steven 20170504 (wei) : For 4顆獨立旋轉馬達同時轉動
int M8_DoOutRotateMove(int &rCount, bool bReset=false);                         //Steven 20170504 (wei) : For 8顆獨立旋轉馬達同時轉動
bool MoveOutRotateToDegreeAtSameTime(int iDegree, bool bInitial=false);         //Steven 20170329 (wei) : Add individual rotate motor
bool CheckRotateOutAnglePostion(int iRow, int iCol, int icount, int iMode);     //Ifor 20170412 add Check Angle 與各Site位置是否相同
bool CheckOutRotateAnglePostion(int iSuckR, int iSuckC, int iTrayY, int iTrayX);
bool CheckRotateOutNotFinish();

#endif
