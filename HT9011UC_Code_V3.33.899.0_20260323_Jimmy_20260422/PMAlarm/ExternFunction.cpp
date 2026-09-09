#include "IncludeAllHeader.H"
#pragma hdrstop
//---------------------------------------------------------------------------

#include "ExternFunction.h"

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
int StringGrid_Insert_Row( TStringGrid* pSG , int iRow )
{
    int iMaxRow = pSG->RowCount;

    if( iRow >= iMaxRow - 1 )
    {
        //iRow++;
        pSG->RowCount++;
    }
    else
    {
        iRow++;

        TStringGrid *TempGrid;
        //TempGrid = new TStringGrid( pParent );
        TempGrid = new TStringGrid( (TForm*)pSG->ParentWindow );
        TempGrid->RowCount =  pSG->RowCount - iRow;

        for( int r = 0 ; r < TempGrid->RowCount ; r++ )
        {
            for( int c = 0 ; c < TempGrid->ColCount - TempGrid->FixedCols ; c++ )
            {
                TempGrid->Cells[TempGrid->FixedCols+c][r] = pSG->Cells[TempGrid->FixedCols+c][r+iRow];
            }
        }

        pSG->RowCount++;

        // Clear Insert Row Data {
        for( int c = 0 ; c < TempGrid->ColCount - TempGrid->FixedCols ; c++ )
        {
            pSG->Cells[TempGrid->FixedCols+c][iRow] = "";
        }
        // }

        for( int r = 0 ; r < TempGrid->RowCount ; r++ )
        {
            for( int c = 0 ; c < TempGrid->ColCount - TempGrid->FixedCols ; c++ )
            {
                pSG->Cells[TempGrid->FixedCols+c][r+iRow+1] = TempGrid->Cells[TempGrid->FixedCols+c][r];
            }
        }

        delete TempGrid;

    }



    return pSG->RowCount;
}

int StringGrid_Delete_Row( TStringGrid* pSG , int iRow , bool bKeepLastData )
{

    int iMaxRow = pSG->RowCount;

    if( iRow < iMaxRow - 1 )
    {
        TStringGrid *TempGrid;
        //TempGrid = new TStringGrid( pParent );
        TempGrid = new TStringGrid( (TForm*)pSG->ParentWindow );
        TempGrid->RowCount =  pSG->RowCount - iRow;

        for( int r = 0 ; r < TempGrid->RowCount ; r++ )
        {
            for( int c = 0 ; c < TempGrid->ColCount - TempGrid->FixedCols ; c++ )
            {
                TempGrid->Cells[TempGrid->FixedCols+c][r] = pSG->Cells[TempGrid->FixedCols+c][r+iRow+1];
            }
        }

        for( int r = 0 ; r < TempGrid->RowCount ; r++ )
        {
            for( int c = 0 ; c < TempGrid->ColCount - TempGrid->FixedCols ; c++ )
            {
                pSG->Cells[TempGrid->FixedCols+c][r+iRow] = TempGrid->Cells[TempGrid->FixedCols+c][r];
            }
        }

        // Clear Last Row Data {
        for( int c = 0 ; c < pSG->ColCount ; c++ )
        {
            pSG->Cells[c][pSG->RowCount-1] = "";
        }
        // }

        pSG->RowCount--;

        delete TempGrid;

        iMaxRow = pSG->RowCount;
    }
    else if( bKeepLastData && iRow == pSG->FixedRows )
    {
        // Clear Last Row Data {
        for( int c = 0 ; c < pSG->ColCount ; c++ )
        {
            pSG->Cells[c][pSG->RowCount-1] = "";
        }
        // }

        // Last Row Data is keep , but Data Size must subtract 1.
        iMaxRow = pSG->RowCount - 1;
    }
    else
    {
        // Clear Last Row Data {
        for( int c = 0 ; c < pSG->ColCount ; c++ )
        {
            pSG->Cells[c][pSG->RowCount-1] = "";
        }
        // }

        pSG->RowCount--;

        iMaxRow = pSG->RowCount;
    }

    return iMaxRow;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
bool StatusBar_ItemText( TStatusBar* pSB , int iIndex , AnsiString sText )
{
    if( iIndex >= pSB->Panels->Count || iIndex < 0 )  return false;

    pSB->Panels->Items[iIndex]->Text = sText;

    return true;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
double Math_AdjustAngle( double fAngle , bool bSign )
{
    double adj_angle = fAngle;

    if( bSign == true && adj_angle < 0 )
    {
        while( adj_angle <= -360 )
        {
            adj_angle += 360;
        }
    }
    else
    {
        while( adj_angle < 0 )
        {
            adj_angle += 360;
        }

        while( adj_angle >= 360 )
        {
            adj_angle -= 360;
        }
    }

    return adj_angle;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
AnsiString String_JustNumber( AnsiString sStr )
{
    AnsiString AdjustStr("");

    String_EraseLSpace( sStr );

    int length = sStr.Length();

    bool first_dot = false;
    for( int i = 1 ; i <= length ; i++ )
    {
        bool is_num     = ( sStr[i] >= '0' && sStr[i] <= '9' );
        bool is_dot     = ( first_dot == false && sStr[i] == '.' );
        bool is_sign    = ( i == 1 && ( sStr[i] == '-' || sStr[i] == '+' ) );

        if( is_num == false && is_dot == false && is_sign == false )
        {
            break;
        }

        if( is_dot == true )
        {
            if( i == 1 )
            {
                AdjustStr = "0";
            }
            else if( AdjustStr[ AdjustStr.Length() ] == '-' || AdjustStr[ AdjustStr.Length() ] == '+' )
            {
                AdjustStr += '0';
            }

            first_dot = true;
        }

        AdjustStr += sStr[i];

    }

    return AdjustStr;

}
//---------------------------------------------------------------------------
void String_EraseLSpace( AnsiString &sStr )
{
    while( sStr.Length() >= 1 && sStr[1] == ' ' )
    {
        sStr.Delete( 1 , 1 );
    }

}
//---------------------------------------------------------------------------
void String_EraseRSpace( AnsiString &sStr )
{
    int length = sStr.Length();
    while( length >= 1 && sStr[length] == ' ' )
    {
        sStr.Delete( length , 1 );

        length = sStr.Length();
    }

}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
int RecordTime( bool bIsStart )
{
    static clock_t tStartTime = clock();

    if( bIsStart == true )
    {
        tStartTime = clock();
        return 0;
    }
    else
    {
        clock_t tEndTime = clock();

        return tEndTime-tStartTime;
    }

}
//---------------------------------------------------------------------------
void ShowRecordTime( TEdit* edt , int iType )
{
    int iMSec = RecordTime( false );

    if( edt != NULL )
    {
        if( iType == 1 )   // Min.
        {
            int iMin = atoi( edt->Text.c_str() );
            if( iMSec < iMin )
            {
                iMSec = iMin;
            }
        }
        else if( iType == 2 )   // Max.
        {
            int iMax = atoi( edt->Text.c_str() );
            if( iMSec > iMax )
            {
                iMSec = iMax;
            }
        }

        edt->Text = AnsiString( iMSec );
    }

}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// 2014.06.12 , Joye , ASE-SG
bool DeleteDirectory( AnsiString sDir )
{

    if( DirectoryExists( sDir ) == false )
    {
        return true;
    }

    AnsiString sDirFile =  sDir + "\\*.*" ;
    TSearchRec sr;
    if( FindFirst( sDirFile , faAnyFile , sr ) == 0 )
    {
        do
        {
            if( !(sr.Attr & faDirectory) )
            {
                AnsiString sFilePath = sDir + "\\" + sr.Name;
                DeleteFile( sFilePath );
            }
            else
            {
                if( sr.Name != "." && sr.Name != ".." )
                {
                    AnsiString sSubDir = sDir + "\\" + sr.Name;
                    if( DeleteDirectory( sSubDir ) == false )
                        break;
                }
            }

        } while (FindNext(sr) == 0);
        FindClose(sr);
    }

    return RemoveDir( sDir );

}
//---------------------------------------------------------------------------
