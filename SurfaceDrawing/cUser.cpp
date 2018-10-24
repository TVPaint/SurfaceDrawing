#include "cUser.h"

#include  "cPaperLogic.h"

cUser::~cUser()
{
}


cUser::cUser( int iIndex ) :
    mIndex( iIndex ),
    mColor( Qt::transparent ),
    mAskDirectionChange( false ),
    mIsOutOfGround( false )
{
    // Points are in grid coordinates
    setPosition( QPoint( 10, 10 ) );
    setSize( QPoint( 1, 1 ) );
    setMovementVector( QPoint( 0, 0 ) );
    mGUICurrentMovementVector = mGUIMovementVector;
    mIsDead = true;
}



cUser::cUser( int iIndex, const QColor& iColor ) :
    mIndex( iIndex ),
    mColor( iColor ),
    mAskDirectionChange( false ),
    mIsOutOfGround( false )
{
    // Points are in grid coordinates
    setPosition( QPoint( 5 * (iIndex+1), 5 * (iIndex + 1) ) );
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

    mGUIMovementVector = iMovementVector;
    mAskDirectionChange = true;
}


void
cUser::Kill()
{
}


