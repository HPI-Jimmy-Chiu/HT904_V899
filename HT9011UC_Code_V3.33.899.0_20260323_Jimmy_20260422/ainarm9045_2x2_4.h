//---------------------------------------------------------------------------
#ifndef ainarm9045_2x2_4H
#define ainarm9045_2x2_4H
//---------------------------------------------------------------------------

void SetInOutArmParameter_2x2_4();
int GetShuttleState_2x2_4(int iSht, bool bPick);
extern const int XPHSuckToSht_2x2_13[3][8];
void SetInArmHasDropToShuttle_2x2_4(int SHT, int Row, int Col);
#endif
