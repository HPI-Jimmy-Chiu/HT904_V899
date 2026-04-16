//---------------------------------------------------------------------------

#ifndef ainarm9045_All_1PickH
#define ainarm9045_All_1PickH
//---------------------------------------------------------------------------
void SetInOutArmParameter_All_1Pick();
extern int GetNowSiteKitMode_All_1Pick(int iSht, bool bPlace);
extern bool GetNowInShuttleRowCol_All_1Picker(int iSht, int *iUseRow, int *iUseCol, bool bPick);
extern void InArmZNeedDown_All_1Pick(int iSht, bool bPlace);
#endif
