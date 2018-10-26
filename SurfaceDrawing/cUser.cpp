#include "cUser.h"

#include  "cPaperLogic.h"

#include <time.h>


cUser::~cUser()
{
}


cUser::cUser( int iIndex ) :
    mIndex( iIndex ),
    mColor( cPaperLogic::GetColorByIndex( iIndex ) ),
    mAskDirectionChange( false ),
    mIsOutOfGround( false )
{
    int randX = (rand() % (GRIDSIZE - 2)) + 1; // Between 1 and GRIDSIZE - 1
    int randY = (rand() % (GRIDSIZE - 2)) + 1; // Between 1 and GRIDSIZE - 1
                                               // Points are in grid coordinates
    setPosition( QPoint( randX, randY ) );
    setSize( QPoint( 1, 1 ) );
    setMovementVector( QPoint( 1, 0 ) );
    mGUICurrentMovementVector = mGUIMovementVector;
    mIsDead = false;
}




void
cUser::setPosition( QPoint iPosition )
{
    mPosition = iPosition;
    mGUIPosition = cPaperLogic::MapFromGrid( iPosition );
}


void
cUser::Update()
{
    mGUIPosition += mGUICurrentMovementVector;

    QPoint newCenter = mGUIPosition + mGUISize * 0.5;
    QPoint newCell   = cPaperLogic::MapToGrid( newCenter );
    mPosition = newCell;

    if( mAskDirectionChange )
    {
        QPoint centerNewCell = newCell * CELLSIZE + QPoint( CELLSIZE / 2, CELLSIZE / 2 );
        QPoint delta = centerNewCell - newCenter;
        if( std::abs( delta.x() ) < 2 && std::abs( delta.y() ) < 2 )
        {
            mGUICurrentMovementVector = mGUIMovementVector;
            mAskDirectionChange = false;
            mGUIPosition = cPaperLogic::MapFromGrid( mPosition );
        }
    }
}


void
cUser::setSize( QPoint iSize )
{
    mGUISize = iSize * CELLSIZE;
}


void
cUser::setMovementVector( QPoint iMovementVector )
{
    if( iMovementVector == mGUICurrentMovementVector )
        return;

    if( iMovementVector == -mGUICurrentMovementVector ) // Can't go at opposite direction
        return;

    mGUIMovementVector = iMovementVector;
    mAskDirectionChange = true;
}


