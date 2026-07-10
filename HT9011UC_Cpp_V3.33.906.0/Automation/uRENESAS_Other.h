// =============================================================================
//  Automation/uRENESAS_Other.h  --  RENESAS host-protocol data-shape records
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/Automation/uRENESAS_Other.h
//                    (376 lines) + Automation/uRENESAS_Other.cpp (12 lines, globals only)
//  Translation wave: W5 Automation (whole-file, trivial POD data-record unit)
//  Translator: AI(W5-RENESAS_Other-Translate) 20260710
//
//  SCOPE: ~22 plain-old-data record / reply classes used by the RENESAS host
//  protocol server.  This is a PREREQUISITE for a FUTURE (not this wave's)
//  Automation/uRENESAS_Server.cpp translation (see MIGRATION_ROADMAP.md --
//  that file, 3186 golden lines, is not yet scheduled).  Every class here is
//  data-only: AnsiString/int fields (+ 3 tiny constant-table classes below),
//  no hardware, no VCL widgets, no global Prod/IniConfig coupling.  This unit
//  exists purely so a future wave has the data shapes + the 3 global command/
//  status/error-code constant-table instances (RENESAS_CMD/RENESAS_Status/
//  RENESAS_Err_Index, defined in uRENESAS_Other.cpp) ready to link against.
//
//  Key changes vs. BCB6 original:
//    - `#include <Classes.hpp>` -> `#include "vclcompat/vcl_compat.h"` (AnsiString).
//    - __published/__property (12 read-only properties, spread across
//      TRENESAS_CMD / TRENESAS_Status_Code / TRENESAS_Err_Index) -> plain
//      public getter METHODS with the SAME NAME as the golden property
//      identifier (established project convention -- see CanBus/cMyNUDN1.h,
//      EtherCAT/MyNUEC1.h "__property -> getter" precedent).  Golden backs
//      each property with a private `FGetXxx()` method that does nothing but
//      `return <literal>;`; that indirection is COLLAPSED here (no behavior
//      lost -- the getter method's body IS the literal return).
//        CROSS-UNIT / FUTURE-WAVE NOTE: golden call sites (in the NOT-YET-
//      translated Automation/uRENESAS_Server.cpp, 130+ occurrences) read the
//      BARE property name with NO parens, e.g.:
//          if (asTempCMD == RENESAS_CMD.OnLineCMD)                 // golden :176
//          iErrInt = RENESAS_Err_Index.DisiableFTCT;                // golden :426
//      Whoever translates uRENESAS_Server.cpp next MUST add `()` at every one
//      of those call sites: `RENESAS_CMD.OnLineCMD()`, `RENESAS_Err_Index.
//      DisiableFTCT()`, etc.  Flagged explicitly in this unit's translate
//      report (see docs/DEVLOG.md / integrate-agent handoff) as a cross-unit
//      conflict risk for that future wave, not something this unit can fix
//      pre-emptively (the consumer file does not exist in the translated tree
//      yet).
//    - TRENESAS_EndFileData: the ONE class with real logic (InitData() + a
//      ctor that calls it).  FAITHFUL GOLDEN GAP preserved (documented at the
//      class below, not "fixed"): InitData() only zeroes the ORIGINAL 8
//      fields (HdPass/HdFail/HdRejectATPass/HdRejectATFail/HdRejectBT/
//      TesterMatch/TesterUnmatch/TesterIgnore -- golden :218-227, part of the
//      "Kirin 20220218 TRENESAS_EndFileData InitData" edit).  The 5 fields
//      added by LATER edits -- HdPickUp (no dated marker) and the "Kirin
//      20221115 Add TwoPassBin" HdPass1/HdPass2/HdRejectATPass1/
//      HdRejectATPass2 -- are declared AFTER InitData()/the ctor in golden and
//      are NEVER zeroed by either.  A default-constructed TRENESAS_EndFileData
//      therefore leaves those 5 members holding INDETERMINATE values in BOTH
//      golden BCB6 and this translation (a plain `int` class member with no
//      initializer has indeterminate value until explicitly assigned -- the
//      same ISO/ANSI C++ rule applies to Borland C++ and to this translation;
//      this is not a translation-introduced bug, it is a golden authoring gap
//      carried over faithfully).
//    - All other classes (TRENESAS_OnLine/_ProdInfo/_ModeStart/_HandlerStart/
//      _ModeEnd/_LotEnd/_TestEnd/_SendAlarm/_SendCleanOut + their _Reply
//      counterparts + the two aggregate containers TRENESAS_RECV_CMD_DATA /
//      TRENESAS_Send_CMD_DATA) are translated verbatim field-for-field --
//      pure data, no logic, no gaps.
//    - Golden source is 100% ASCII (verified: zero bytes >= 0x80 in the
//      golden .h); there is no Big5/cp950 comment-transcoding concern in this
//      file (unlike most of the tree -- see docs/KNOWLEDGE.md mojibake note).
// =============================================================================
#ifndef uRENESAS_OtherH
#define uRENESAS_OtherH

#include "vclcompat/vcl_compat.h"   // AnsiString
//---------------------------------------------------------------------------
class TRENESAS_OnLine
{
    public:
    AnsiString CMD;
    int iRecv;
};
//---------------------------------------------------------------------------
class TRENESAS_ProdInfo
{
    public:
    AnsiString CMD;
    AnsiString LotNo;
    AnsiString LotID;
    AnsiString HdParameter;
    AnsiString Product;
    AnsiString StepCode;
    AnsiString Qty;
    AnsiString Date;
    int iRecv;
};
//---------------------------------------------------------------------------
class TRENESAS_ModeStart
{
    public:
    AnsiString CMD;
    AnsiString Status;
    AnsiString Date;
    int iRecv;
};
//---------------------------------------------------------------------------
class TRENESAS_HandlerStart
{
    public:
    AnsiString CMD;
    AnsiString Date;
    int iRecv;
};
//---------------------------------------------------------------------------
class TRENESAS_ModeEnd
{
    public:
    AnsiString CMD;
    AnsiString Status;
    AnsiString Date;
    int iRecv;
};
//---------------------------------------------------------------------------
class TRENESAS_LotEnd
{
    public:
    AnsiString CMD;
    AnsiString LECD;
    AnsiString Date;
    int iRecv;
};
//---------------------------------------------------------------------------
//Kirin 20220118 TestEnd
//==>
class TRENESAS_TestEnd
{
    public:
    AnsiString CMD;
    AnsiString Date;
    int iRecv;
};
//<==
//Kirin 20220118 TestEnd
//---------------------------------------------------------------------------
//Kirin 20211020 SendAlarm
//==>
class TRENESAS_SendAlarm
{
    public:
    AnsiString CMD;
    int iRecv;
};
//<==
//Kirin 20211020 SendAlarm
//---------------------------------------------------------------------------
//Kirin 20221115 SendCleanOut
//==>
class TRENESAS_SendCleanOut
{
    public:
    AnsiString CMD;
    AnsiString Date;
    int iRecv;
};
//<==
//Kirin 20221115 SendCleanOut
//---------------------------------------------------------------------------
class TRENESAS_RECV_CMD_DATA
{
    public:
    TRENESAS_OnLine         OnLine;
    TRENESAS_ProdInfo       ProdInfo;
    TRENESAS_ModeStart      ModeStart;
    TRENESAS_HandlerStart   HandlerStart;
    TRENESAS_ModeEnd        ModeEnd;
    TRENESAS_LotEnd         LotEnd;
    TRENESAS_TestEnd        TestEnd;  //Kirin 20220118 TestEnd
    TRENESAS_SendAlarm      SendAlarm;//Kirin 20211020 SendAlarm
    TRENESAS_SendCleanOut   SendCleanOut;//Kirin 20221115 SendCleanOut
};
//---------------------------------------------------------------------------
class TRENESAS_OnLine_Reply
{
    public:
    AnsiString CMD;
    AnsiString Result;
    AnsiString Comment;
};
//---------------------------------------------------------------------------
class TRENESAS_ProdInfo_Reply
{
    public:
    AnsiString CMD;
    AnsiString Result;
    AnsiString Comment;
    AnsiString HdParameter;
    AnsiString Product;
};
//---------------------------------------------------------------------------
class TRENESAS_ModeStart_Reply
{
    public:
    AnsiString CMD;
    AnsiString Status;
    AnsiString Result;
    AnsiString Comment;
    AnsiString Product;
};
//---------------------------------------------------------------------------
class TRENESAS_HandlerStart_Reply
{
    public:
    AnsiString CMD;
    AnsiString Result;
    AnsiString Comment;
};
//---------------------------------------------------------------------------
class TRENESAS_ModeEnd_Reply
{
    public:
    AnsiString CMD;
    AnsiString Status;
    AnsiString Result;
    AnsiString Comment;
    AnsiString TotalQty;
    AnsiString HdParameter;
};
//---------------------------------------------------------------------------
class TRENESAS_LotEnd_Reply
{
    public:
    AnsiString CMD;
    AnsiString Result;
    AnsiString Comment;
};
//---------------------------------------------------------------------------
//Kirin 20220118 TestEndReply
//==>
class TRENESAS_TestEnd_Reply
{
    public:
    AnsiString CMD;
    AnsiString Result;
    AnsiString Comment;
};
//<==
//Kirin 20220118 TestEndReply
//---------------------------------------------------------------------------
class TRENESAS_Send_CMD_DATA
{
    public:
    TRENESAS_OnLine_Reply       OnLine_Reply;
    TRENESAS_ProdInfo_Reply     ProdInfo_Reply;
    TRENESAS_ModeStart_Reply    ModeStart_Reply;
    TRENESAS_HandlerStart_Reply HandlerStart_Reply;
    TRENESAS_ModeEnd_Reply      ModeEnd_Reply;
    TRENESAS_LotEnd_Reply       LotEnd_Reply;
    TRENESAS_TestEnd_Reply      TestEnd_Reply;//Kirin 20220118 TestEndReply
};
//---------------------------------------------------------------------------
// AI(W5-RENESAS_Other-Translate) 20260710: golden __property (BCB6, read-only,
// backed by a private `FGetXxx()` that only `return`s a literal) -> plain
// public getter methods with the SAME NAME as the golden property identifier
// (the FGetXxx indirection is collapsed away -- see file-header note above).
// FUTURE-WAVE CALL-SITE NOTE: golden reads these as bare properties, e.g.
// `RENESAS_CMD.OnLineCMD` (no parens); the future uRENESAS_Server.cpp
// translation must change every such read to `RENESAS_CMD.OnLineCMD()`.
class TRENESAS_CMD
{
    public:
    AnsiString OnLineCMD()             const { return "00"; }
    AnsiString OnLineReplyCMD()        const { return "01"; }

    AnsiString ProductInfoCMD()        const { return "10"; }
    AnsiString ProductInfoReplyCMD()   const { return "11"; }

    AnsiString StartCMD()              const { return "20"; }
    AnsiString StartReplyCMD()         const { return "21"; }

    AnsiString HandlerStartCMD()       const { return "50"; }
    AnsiString HandlerStartReplyCMD()  const { return "51"; }

    AnsiString EndCMD()                const { return "30"; }
    AnsiString EndReplyCMD()           const { return "31"; }

    AnsiString LotEndCMD()             const { return "40"; }
    AnsiString LotEndReplyCMD()        const { return "41"; }

    //Kirin 20220118 TestEnd
    //==>
    AnsiString TestEndCMD()             const { return "70"; }
    AnsiString TestEndReplyCMD()        const { return "71"; }
    //<==
    //Kirin 20220118 TestEnd

    //Kirin 20211020 SendAlarm
    //==>
    AnsiString SendAlarmCMD()          const { return "90"; }
    AnsiString SendAlarmReplyCMD()     const { return "91"; }
    //<==
    //Kirin 20211020 SendAlarm

    //Kirin 20221115 SendCleanOut
    //==>
    AnsiString SendCleanOutCMD()          const { return "B0"; }
    AnsiString SendCleanOutReplyCMD()     const { return "B1"; }
    //<==
    //Kirin 20221115 SendCleanOut
};
//---------------------------------------------------------------------------
// AI(W5-RENESAS_Other-Translate) 20260710: same __property -> getter-method
// collapse as TRENESAS_CMD above.
class TRENESAS_Status_Code
{
    public:
    AnsiString TestStart()            const { return "00"; }
    AnsiString ReTestStart()          const { return "10"; }
    AnsiString ReReTestStart()        const { return "20"; }

    AnsiString TestEnd()              const { return "01"; }
    AnsiString ReTestEnd()            const { return "11"; }
    AnsiString ReReTestEnd()          const { return "21"; }
};
//---------------------------------------------------------------------------
class TRENESAS_EndFileData
{
    public:
    //Kirin 20220218 TRENESAS_EndFileData InitData
    //==>
    // AI(W5-RENESAS_Other-Translate) 20260710: FAITHFUL GOLDEN GAP -- this
    // only zeroes the 8 fields that existed when InitData() was written
    // (golden :218-227).  The 5 fields declared BELOW (HdPickUp and the
    // "TwoPassBin" quartet) are NOT touched here and are NOT given in-class
    // initializers either -- see file-header note.  Do not "fix" by adding
    // them; that would silently change behavior vs. the golden binary.
    void InitData()
    {
        HdPass=0;
        HdFail=0;
        HdRejectATPass=0;
        HdRejectATFail=0;
        HdRejectBT=0;
        TesterMatch=0;
        TesterUnmatch=0;
        TesterIgnore=0;
    };
    //<==
    //Kirin 20220218 TRENESAS_EndFileData InitData
    TRENESAS_EndFileData()
    {
        InitData();
    };
    int HdPass;
    int HdFail;
    int HdRejectATPass;
    int HdRejectATFail;
    int HdRejectBT;
    int TesterMatch;
    int TesterUnmatch;
    int TesterIgnore;
    int HdPickUp;
    //Kirin 20221115 Add TwoPassBin
    //==>
    int HdPass1;
    int HdPass2;
    int HdRejectATPass1;
    int HdRejectATPass2;
    //<==
    //Kirin 20221115 Add TwoPassBin
};
//---------------------------------------------------------------------------
//Kirin 20220127 TRENESAS_Err_Index
//==>
// AI(W5-RENESAS_Other-Translate) 20260710: same __property -> getter-method
// collapse as TRENESAS_CMD/TRENESAS_Status_Code above (int-valued this time).
class TRENESAS_Err_Index
{
    public:
    int OK()                    const { return 0; }
    int HandlerNoHome()         const { return 1; }
    int HandlerHaveDeviceData() const { return 2; }
    int HandlerHaveDevice()     const { return 3; }
    int CanNotLoadWorkFile()    const { return 4; }
    int DisiableFTCT()          const { return 5; }
    int OutOfOrder()            const { return 6; }
    int ARTIsDisable()          const { return 7; }
    int RecvTestEnd()           const { return 8; }
    int NoInputQty()            const { return 9; }  //Kirin 20220128
    int NoRemoveTray()          const { return 10; } //Kirin 20220714
    int CleanOutFail()          const { return 11; } //Kirin 20221123 CleanOutFail
    int CleanCountFail()        const { return 12; } //Kirin 20230814
};
//<==
//Kirin 20220127 TRENESAS_Err_Index
//---------------------------------------------------------------------------
extern TRENESAS_CMD         RENESAS_CMD;
extern TRENESAS_Status_Code RENESAS_Status;
extern TRENESAS_Err_Index   RENESAS_Err_Index; //Kirin 20220127 TRENESAS_Err_Index
#endif
