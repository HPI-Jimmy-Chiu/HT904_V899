// =============================================================================
//  forms/fIoSetView.cpp  --  ACTIVE bodies for Tfiosetview (see forms/
//  fIoSetView.h for the full wave banner: scope, safety posture, gate
//  register, deviations).  AI(W906-FW3-IOSV1) 20260827: new file.
//
//  Only the 6 methods the header marks ACTIVE are DEFINED here.  Every other
//  declaration in forms/fIoSetView.h has NO body -- calling one is a link
//  error by design.
// =============================================================================
#include "forms/fIoSetView.h"

#include "MachineType.h"    // eTestMode (SingleSite/DualSite/.../_32Site4X8M)
#include "cmydef.h"          // _8Bit/_10Bit/_10BitPE/_10BitPO
#include "cprod.h"            // TTLCfg / TestIF_File

Tfiosetview::Tfiosetview()
{
    // golden iosetview.cpp:49-91 (ctor).  PORT SPLIT (DEVIATION D-2): this
    // body stuffs no logic at all -- every field above already carries its
    // faithful default via an in-class initialiser.  Golden's ACTUAL ctor
    // body (Index_8/16/InArm_8/16/OutArm_8/16 widget-array wiring,
    // btnSwHeatGun/btnSwCDAGun visibility, the LoadIoTable(0,0,0) call) is
    // NOT translated this wave -- see forms/fIoSetView.h DEVIATION D-2 for
    // why (its only consumer, ShowSuckMode, is gated on the ht9045_sm
    // boundary; LoadIoTable is gated on the ht9045_db boundary).
}

Tfiosetview::~Tfiosetview()
{
    // Port-only addition (DEVIATION D-3) -- golden has no explicit
    // destructor.  Frees the 16 owning TLabel pointers this wave declared.
    delete lblTTL_Aa;
    delete lblTTL_Ab;
    delete lblTTL_Ac;
    delete lblTTL_Ad;
    delete lblTTL_Ba;
    delete lblTTL_Bb;
    delete lblTTL_Bc;
    delete lblTTL_Bd;
    delete lblTTLInAa;
    delete lblTTLInAb;
    delete lblTTLInAc;
    delete lblTTLInAd;
    delete lblTTLInBa;
    delete lblTTLInBb;
    delete lblTTLInBc;
    delete lblTTLInBd;
}

// golden iosetview.cpp:1861-1869
void Tfiosetview::ResetIndexSuck()
{
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<4; j++)
            for(int k=0; k<8; k++)
                bIndexSuck[i][j][k]=false;
    }
}

// golden iosetview.cpp:1871-1879
void Tfiosetview::ResetIndexDestroy()
{
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<4; j++)
            for(int k=0; k<8; k++)
                bIndexDestroy[i][j][k]=false;
    }
}

// golden iosetview.cpp:2861-2983
void Tfiosetview::LabSiteMap()
{
    int iCH[2][4];
    AnsiString asString[9] ={"Aa", "Ab", "Ac", "Ad", "Ba", "Bb", "Bc", "Bd", "X"};
    AnsiString asString1[9]={"Aa", "Ab", "Ac", "Ba", "Bb", "Bc", "X", "X", "X"};

    //待Frank新增
    if((TTLCfg.iCateBitLength==_8Bit || TTLCfg.iCateBitLength==_10Bit ||
        TTLCfg.iCateBitLength==_10BitPE || TTLCfg.iCateBitLength==_10BitPO))
    {
        lblTTL_Aa->Visible=false;
        lblTTL_Ab->Visible=false;
        lblTTL_Ac->Visible=false;
        lblTTL_Ad->Visible=false;
        lblTTL_Ba->Visible=false;
        lblTTL_Bb->Visible=false;
        lblTTL_Bc->Visible=false;
        lblTTL_Bd->Visible=false;
        //===============================================
        lblTTLInAa->Visible=false;
        lblTTLInAb->Visible=false;
        lblTTLInAc->Visible=false;
        lblTTLInAd->Visible=false;
        lblTTLInBa->Visible=false;
        lblTTLInBb->Visible=false;
        lblTTLInBc->Visible=false;
        lblTTLInBd->Visible=false;
    }
    else
    {
        switch(TestIF_File.iTestMode)
        {
            case SingleSite:
            case DualSite:
            case TriSite1X3:
            case QualSite1X4:
            case DualSite2x1:
            case QualSite2X2:
            case _8Site1X4:

                for(int i=0; i<2; i++)
                {
                    for(int j=0; j<4; j++)
                    {
                        if(TestIF_File.iSiteMap[i][j]>0)
                            iCH[i][j]=TestIF_File.iSiteMap[i][j]-1;
                        else
                            iCH[i][j]=8;
                    }
                }
                lblTTL_Aa->Caption=asString[iCH[0][0]];
                lblTTL_Ab->Caption=asString[iCH[0][1]];
                lblTTL_Ac->Caption=asString[iCH[0][2]];
                lblTTL_Ad->Caption=asString[iCH[0][3]];
                lblTTL_Ba->Caption=asString[iCH[1][0]];
                lblTTL_Bb->Caption=asString[iCH[1][1]];
                lblTTL_Bc->Caption=asString[iCH[1][2]];
                lblTTL_Bd->Caption=asString[iCH[1][3]];
                //===============================================
                lblTTLInAa->Caption=asString[iCH[0][0]];
                lblTTLInAb->Caption=asString[iCH[0][1]];
                lblTTLInAc->Caption=asString[iCH[0][2]];
                lblTTLInAd->Caption=asString[iCH[0][3]];
                lblTTLInBa->Caption=asString[iCH[1][0]];
                lblTTLInBb->Caption=asString[iCH[1][1]];
                lblTTLInBc->Caption=asString[iCH[1][2]];
                lblTTLInBd->Caption=asString[iCH[1][3]];
                break;
            case _6Site2X3:
                for(int i=0; i<2; i++)
                {
                    for(int j=0; j<4; j++)
                    {
                        if(TestIF_File.iSiteMap[i][j]>0)
                            iCH[i][j]=TestIF_File.iSiteMap[i][j]-1;
                        else
                            iCH[i][j]=8;
                    }
                }

                lblTTL_Aa->Caption=asString1[iCH[0][0]];
                lblTTL_Ab->Caption=asString1[iCH[1][0]];
                lblTTL_Ac->Caption=asString1[iCH[0][1]];
                lblTTL_Ad->Caption=asString1[iCH[0][3]];
                lblTTL_Ba->Caption=asString1[iCH[0][2]];
                lblTTL_Bb->Caption=asString1[iCH[1][2]];
                lblTTL_Bc->Caption=asString1[iCH[1][1]];
                lblTTL_Bd->Caption=asString1[iCH[1][3]];
                //===============================================
                lblTTLInAa->Caption=asString1[iCH[0][0]];
                lblTTLInAb->Caption=asString1[iCH[1][0]];
                lblTTLInAc->Caption=asString1[iCH[0][1]];
                lblTTLInAd->Caption=asString1[iCH[0][3]];
                lblTTLInBa->Caption=asString1[iCH[0][2]];
                lblTTLInBb->Caption=asString1[iCH[1][2]];
                lblTTLInBc->Caption=asString1[iCH[1][1]];
                lblTTLInBd->Caption=asString1[iCH[1][3]];
                break;
            case _32Site4X8N:
                lblTTL_Aa->Caption="Aa";
                lblTTL_Ab->Caption="Aa";
                lblTTL_Ac->Caption="Aa";
                lblTTL_Ad->Caption="Aa";
                lblTTL_Ba->Caption="Aa";
                lblTTL_Bb->Caption="Aa";
                lblTTL_Bc->Caption="Aa";
                lblTTL_Bd->Caption="Aa";
                break;
            case _32Site4X8M:
                lblTTL_Aa->Caption="Aa";
                lblTTL_Ab->Caption="Aa";
                lblTTL_Ac->Caption="Aa";
                lblTTL_Ad->Caption="Aa";
                lblTTL_Ba->Caption="Aa";
                lblTTL_Bb->Caption="Aa";
                lblTTL_Bc->Caption="Aa";
                lblTTL_Bd->Caption="Aa";
                break;
        }
    }
}

// golden iosetview.cpp:3143-3148
void Tfiosetview::strngrdIoTableSelectCell(TObject *Sender, int ACol,
                                            int ARow, bool &CanSelect)
{
    (void)Sender;
    (void)CanSelect;
    iSelectRow=ARow;
    iSelectCol=ACol;
}
