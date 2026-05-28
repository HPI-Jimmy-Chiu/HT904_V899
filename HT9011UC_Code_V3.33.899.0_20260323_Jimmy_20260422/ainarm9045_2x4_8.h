//---------------------------------------------------------------------------
#ifndef ainarm9045_2x4_8H
#define ainarm9045_2x4_8H
//---------------------------------------------------------------------------

void SetInOutArmParameter_2x4_8();
int GetShuttleState_2x4_8(int iSht, bool bPick);
extern const int XPHSuckToSht_2x4_8[3][8];
void SetInArmHasDropToShuttle_2x4_8(int SHT, int Row, int Col);
#endif
