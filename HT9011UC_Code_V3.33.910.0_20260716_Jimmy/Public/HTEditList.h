//---------------------------------------------------------------------------

#ifndef HTEditListH
#define HTEditListH
//---------------------------------------------------------------------------
#include <vcl.h>
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include "MyStringList.h"
#include "cJSON.h"
#include "HTEdit.h"

extern AnsiString sHPPickRec;
extern AnsiString sHPPickRecException;
extern AnsiString sHPPlaceRec;
extern AnsiString sCleanPlaceRec;

//---------------------------------------------------------------------------
class HTEditList
{
    public:
        TList *FEditList;
        int iDecimalPoint;
        int iBarcodeReadType;
        HTEditList();
        ~HTEditList();

        void Add(TControl  *SoureCtrl,                                          //Steven 20230224 : Edit List改支援多型態元件
                 void *Par,
                 TEditContent Content=ECText,
                 AnsiString GroupName="",
                 AnsiString KeyName="",
                 bool Visible=true,
                 bool Enable=true,
                 bool ReadFromFile=true,                                        //是否從檔案讀取
                 AnsiString DefValue="",
                 bool bDisableEventOverlap=false,
                 AnsiString Min="",
                 AnsiString Max="",
                 int iTransform=0);

        void AddInteger (TEdit *SoureEdit, int    *Par, int Min=-9999999,       int Max=9999999,      int DefValue=0,      AnsiString GroupName="", AnsiString KeyName="");
        void AddInteGerP(TEdit *SoureEdit, int    *Par, int Min=0,              int Max=9999999,      int DefValue=0,      AnsiString GroupName="", AnsiString KeyName="");
        void AddIntegerN(TEdit *SoureEdit, int    *Par, int Min=-9999999,       int Max=0,            int DefValue=0,      AnsiString GroupName="", AnsiString KeyName="");
        void AddDouble  (TEdit *SoureEdit, double *Par, double Min=-9999999.0,  double Max=9999999.0, double DefValue=0.0, AnsiString GroupName="", AnsiString KeyName="");
        void AddDoubleP (TEdit *SoureEdit, double *Par, double Min=0.0,         double Max=9999999.0, double DefValue=0.0, AnsiString GroupName="", AnsiString KeyName="");
        void AddDoubleN (TEdit *SoureEdit, double *Par, double Min=-9999999.0,  double Max=0.0,       double DefValue=0.0, AnsiString GroupName="", AnsiString KeyName="");
        void AddPercent (TEdit *SoureEdit, double *Par, double Min=0.0,         double Max=100.0,     double DefValue=0.0, AnsiString GroupName="", AnsiString KeyName="");

        void ClearALL();
        bool SaveEditTextToFile(AnsiString Path, AnsiString FileName);
        bool ReadEditTextFromFile(AnsiString Path, AnsiString FileName);
        void InitialDataToEdit();
        void SetBarcodeType(int iBarcodeType);

        void ExportCustomLimitToCSV(AnsiString CsvFileName);
        void ImportCustomLimitationFromCSV(AnsiString CSVFileName, AnsiString IniFileName);
        void SaveCustomLimitationToIni(AnsiString IniFileName);
        void ReadCustomLimitationFromIni(AnsiString IniFileName);

        void SaveDefaultLimitationToIni(AnsiString IniFileName);
        void ReadDefaultLimitationFromIni(AnsiString IniFileName);

        bool bAlarmLimitation;
        void SetFontBlue();                                                     //Sam 20220921 : config儲存跟隨recipe
        bool ReadAndCompareDataFromFile(AnsiString asNameWithPath1, AnsiString asNameWithPath2, TList *tlDiff, AnsiString &asError);//JimmyChiu 20230504 : 新增資料比對顯示function
    private:
        bool bFontBlue;                                                         //Sam 20220921 : config儲存跟隨recipe
        void AddDiffData(TList *tdiff, AnsiString asContent, AnsiString a1, AnsiString a2);     //JimmyChiu 20230504 : 新增資料比對顯示function
        void SetItemLimit(THTEdit *Item, const TEditContent &Content, const AnsiString &sMin, const AnsiString &sMax);
};
//---------------------------------------------------------------------------
//class HTCheckBoxList
//{
//    public:
//        TList *CheckBoxList;
//        HTCheckBoxList();
//        ~HTCheckBoxList();
//
//        void Add(TCheckBox *SoureCB,
//                 bool *Par,
//                 AnsiString GroupName,
//                 AnsiString KeyName,
//                 bool bVisible,
//                 bool bEnable,
//                 bool bReadFromFile,        //是否從檔案讀取
//                 bool bForceValue);         //如果不是從檔案讀取的預設值
//
//        void ClearALL();
//        bool SaveCheckBoxToFile(AnsiString Path, AnsiString FileName);
//        bool ReadCheckBoxFromFile(AnsiString Path, AnsiString FileName);
//        void InitialDataToCheckBox();
//        bool bAlarmLimitation;
//        void SetFontBlue();     //Sam 20220919 : 設定 Config 字型顏色
//    private:
//        bool bFontBlue;         //Sam 20220919 : 設定 Config 字型顏色
//};
//---------------------------------------------------------------------------
class uPlacePoint2D;
//---------------------------------------------------------------------------
class uBasicPickPlace                                                           //Jimmychiu 20230417 : Record the position after placing the IC
{
public:     // User declarations
    int    GetIntegerFromJSON(cJSON* root, AnsiString TagName);
    double GetDoubleFromJSON(cJSON* root, AnsiString TagName);
    void   DataForwardAndDelete(TList* list);
    AnsiString GetStringFromJSON(cJSON* root, AnsiString TagName);
};
//---------------------------------------------------------------------------
extern HTEditList *elConfig;
extern HTEditList *elUdUld;
extern HTEditList *elTrayForm;
extern HTEditList *elContact;
extern HTEditList *elLaser;
extern HTEditList *elConfig_byRecipe;                                           //Sam 20220921 : config儲存跟隨recipe
extern HTEditList *cbLastSet;
extern HTEditList *elVacuumUnit;                                                //Sam 20230210 : 新增 VacuumUnit 通訊模組
//extern HTEditList *cbTest;
extern HTEditList *cbConfig_byRecipe;                                       //Eastsun 20251216 : config 存讀 recipe
extern HTEditList *elTeach;
//extern uHPPickPos *HPTryPickPos;

//extern void EditLimitInit();
//extern void EditLimitFree();
//---------------------------------------------------------------------------
class uHPSuckTeam : public uBasicPickPlace                                      //大群組底下, 分團隊, 同一次吸放的為同一團
{
private:    // User declarations
    void SavePlateRow(cJSON* root);
    void SavePlateCol(cJSON* root);
    void SavePlateSite(cJSON* root);
    void SavePlateSuck(cJSON* root);
    void ReadPlateRowFromJSON(cJSON* root);
    void ReadPlateColFromJSON(cJSON* root);
    void ReadPlateSiteFromJSON(cJSON* root);
    void ReadPlateSuckFromJSON(cJSON* root);
public:     // User declarations
    uHPSuckTeam();
    uHPSuckTeam(int iP, int iR, int iC, int iSht, int iKit);
    ~uHPSuckTeam();
    //
    int iP;
    int iR;
    int iC;
    int iSht;
    int iKit;
    int iSite[MAX_ARM_Row][MAX_ARM_Col];
    int iPlateR[MAX_ARM_Row][MAX_ARM_Col];
    int iPlateC[MAX_ARM_Row][MAX_ARM_Col];
    bool bSuck[MAX_ARM_Row][MAX_ARM_Col];

    int  GetUseSuckCol();                                                       //取得目前是哪個Col要做事
    void ClearHPSuckTeam();
    //
    uHPSuckTeam* Clone();

    //To JSON
    void SaveJSONFile(cJSON* root);
    void LoadJSONFile(cJSON* root);
};
//---------------------------------------------------------------------------
class uHPSuckGroup:public uBasicPickPlace                                       //大群組, 一組就是 入料吸滿後要放HP 或 下次吸取加熱盤 的資料
{
private:    // User declarations
    TList *HPSuckTeamList;
public:     // User declarations
    uHPSuckGroup();
    ~uHPSuckGroup();

    void AddHPSuckTeam(uHPSuckTeam* info);
    void AddHPSuckTeam(int iP, int iR, int iC, int iSht, int iKit);

    void DelHPSuckTeam(int iIndex);

    void ClearTeamList();
    int  GetTeamCount();                                                        //分幾次吸放料

    void DataForward();

    uHPSuckGroup*   Clone();
    uHPSuckTeam* ExtractLastTeam();
    uHPSuckTeam* ExtractSuckTeam(int iIndex);

    //To JSON
    void SaveJSONFile(cJSON* root);
    void LoadJSONFile(cJSON* root);
};
//---------------------------------------------------------------------------
class uPlateInfo:public uBasicPickPlace                                         //Jimmychiu 20230417 : Record the position after placing the IC
{
private:
    TList *HPSuckGroupList;
    int iLimitcount;
    uHPSuckTeam* SearchHPSuckTeamByCoordinate(int &iSuckRow, int &iSuckCol, int _iP, int _iRow, int _iCol); //根據加熱盤座標, 尋找要使用的吸嘴
    uHPSuckTeam* TempSuckTeam;
public:
    uPlateInfo();
    ~uPlateInfo();

    void AddHPSuckGroup();
    void AddHPSuckGroup(uHPSuckGroup* info);
    void UpdateHPSuckGroup(int iP, int iR, int iC, int iSht, int iKit);

    void ClearGroupList();
    void DelHPSuckGroup(int iIndex);

    bool SetPlateSuck(int _iP, int _iRow, int _iCol, bool bSuck);               //編輯已儲存資料
    void SetArrPlateXY(int iSuckRow, int iSuckCol, int _iP, int iPlateR, int iPlateC, int _iSite=-1);

    int  GetHPSuckGroupCount();

    bool GetHPFirstTeam(int *iP, int (*PlateR)[MAX_ARM_Col], int (*PlateC)[MAX_ARM_Col], bool (*Suck)[MAX_ARM_Col]);    //取得第一筆資料的全部吸嘴資料
    bool GetHPFirstTeamPlate(int &iP, int &iR, int &iC, int &iSht, int &iKit);  //取得第一筆資料的加熱盤座標與Shuttle位置
    int  GetHPFirstTeamSuckCol();                                               //取得第一筆資料中, 最左側吸嘴的Col位置, 以便轉換成實體位置
    bool GetHPFirstTeamSuckUse(bool bSuck[MAX_ARM_Row][MAX_ARM_Col]);           //取得第一筆資料中, 吸嘴資料是否需要交換
    bool GetHPFirstTeamMotUse(bool bSuck[MAX_ARM_Row][MAX_ARM_Col]);            //取得第一筆資料中, 吸嘴Z軸使用與否的資料
    bool GetHPFirstTeamToList(int *iP, TList* lsPoint2D);                       //取得第一筆資料, 並轉換成陣列

    bool GetHPTeamSuck(int iGroupIndex, int iTeamIndex, int *iP, int (*PlateR)[MAX_ARM_Col], int (*PlateC)[MAX_ARM_Col], bool (*Suck)[MAX_ARM_Col]);

    bool DataForwardAndNextTeam();                                              //確認是否需要多次吸料, 並刪除舊資料

    void InsertHPGroup(int Index, void *Item);
    void TransferByuPlacePoint2DList(TList *Listpp2D);
    void AddDataByPlacePoint2D(uPlacePoint2D* pp2D);

    void SetTempSuckTeam(uHPSuckTeam* HPTeam);
    uHPSuckGroup*   ExtractLastGroup();                                         //群組, 同一群需要同時吸放
    uHPSuckGroup*   ExtractSuckGroup(int iIndex);
    uHPSuckTeam*    GetTempSuckTeam();
    uHPSuckTeam*    ExtractFirstTeam();                                         //團隊, 群組內可以分成幾團吸放
    uHPSuckTeam*    ExtractLastTeam();
    uHPSuckTeam*    ExtractLastTeamHasData();                                   //最後一個可能是空值, 所以取倒數第二個
    uHPSuckTeam*    ExtractTeamByIndex(int iGroupIndex, int iTeamIndex);

    //To File
    void LoadFile(AnsiString sFileName);
    void SaveFile(AnsiString sFileName);
    void ResetFile(AnsiString sFileName);
};
//---------------------------------------------------------------------------
extern uPlateInfo *PickFromHPList;
extern uPlateInfo *PlaceToHPList;

extern uPlateInfo *PlaceToCleanList;
//---------------------------------------------------------------------------
class uPoint2D                                                                  //Jimmychiu 20230417 : Record the position after placing the IC
{
private:
public:
    uPoint2D();
    uPoint2D(int x, int y);
    ~uPoint2D(){}
    int X;
    int Y;
    void Clear();
};
//---------------------------------------------------------------------------
class uPlacePoint2D                                                             //Jimmychiu 20230417 : Record the position after placing the IC
{
private:
public:
    uPlacePoint2D()
    {
        Clear();
    }
    ~uPlacePoint2D(){}
    int iPlate;
    int iPlateRow;                                                              //對應加熱盤位置
    int iPlateCol;
    int iSuckRow;                                                               //對應吸嘴位置
    int iSuckCol;
    int iSubStep;                                                               //吸嘴上IC到加熱盤吸放料的次數
    int iHPStep;                                                                //加熱盤放料陣列的Index
    void Clear()
    {
        iPlate      =0;
        iPlateRow   =0;
        iPlateCol   =0;
        iSuckRow    =0;
        iSuckCol    =0;
        iSubStep    =0;
        iHPStep     =0;
    }
};
//---------------------------------------------------------------------------
typedef struct                                                                  //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
{
    bool bZActByMode[MAX_ARM_Row][MAX_ARM_Col];
    bool bZActByOnOffSite[MAX_ARM_Row][MAX_ARM_Col];
    bool bZActByLimit[MAX_ARM_Row][MAX_ARM_Col];
    bool bZActive[MAX_ARM_Row][MAX_ARM_Col];
    int iYVariable;
    int iXVariable[X_PITCH_COUNT];
    int iZPosition[MAX_ARM_Row][MAX_ARM_Col];
    uPoint2D pArmXYPos;
    void Clear()
    {
        pArmXYPos.Clear();
        ZeroMemory(iXVariable , sizeof(iXVariable));
        iYVariable=0;
        ZeroMemory(bZActive   , sizeof(bZActive));
        ZeroMemory(iZPosition , sizeof(iZPosition));
    }
    bool GetZActive(int irow,int icol)
    {
        bZActive[irow][icol]=bZActByMode[irow][icol] && bZActByOnOffSite[irow][icol] && bZActByLimit[irow][icol];
        return bZActive[irow][icol];
    }
}ArmAndSuckInfo;
//---------------------------------------------------------------------------
typedef struct                                                                  //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
{
    int iWhichAuto;
    uPoint2D pTrayRowCol;
    uPoint2D pArmXYPos;
    int iArmXV1;
    int iArmXV2;
    int iArmYV1;
    bool bPlace;
    void Clear()
    {
        iWhichAuto=-1;
        pTrayRowCol.Clear();
        pArmXYPos.Clear();
        iArmXV1=0;
        iArmXV2=0;
        iArmYV1=0;
        bPlace=false;
    }
}SingleSuckPosOnTray;
//---------------------------------------------------------------------------
typedef struct                                                                  //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
{
    SingleSuckPosOnTray ssTarget;
    SingleSuckPosOnTray ssSource;
    uPoint2D pArmSuckActive;
    void Clear()
    {
        ssTarget.Clear();
        ssSource.Clear();
        pArmSuckActive.Clear();
    }
}MyFillTheTrayAfterOutArmPlace;
//---------------------------------------------------------------------------
typedef struct                                                                  //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
{
    ArmAndSuckInfo ssTarget;
    ArmAndSuckInfo ssSource;
    void Clear()
    {
        ssTarget.Clear();
        ssSource.Clear();
    }
}PickPlaceByArm;
//---------------------------------------------------------------------------
#endif
