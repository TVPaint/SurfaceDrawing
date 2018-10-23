#include "cUser.h"

#include  "cPaperLogic.h"

cUser::~cUser()
{
}


cUser::cUser( const QColor& iColor ) :
    mColor( iColor ),
    mAskDirectionChange( false ),
    mIsOutOfGround( false )
{
    // Points are in grid coordinates
    setPosition( QPoint( 5, 5 ) );
    setSize( QSize( 1, 1 ) );
    setMovementVector( QPoint( 1, 0 ) );
    mGUICurrentMovementVector = mGUIMovementVector;
}





void
cUser::setPosition( QPoint iPosition )
{
    mPosition = iPosition;
    mGUIPosition = cPaperLogic::MapFromGrid( iPosition );
}


void
cUser::setGUIPosition( QPoint iPosition )
{
    auto debug = iPosition.x();
    if( debug == 104 )
        int bp = 8;
    mGUIPosition = iPosition;

    if( mPosition != cPaperLogic::MapToGrid( iPosition ) && mAskDirectionChange )
    {
        mPosition = cPaperLogic::MapToGrid( iPosition );
        mGUICurrentMovementVector = mGUIMovementVector;
        mAskDirectionChange = false;
        mGUIPosition = cPaperLogic::MapFromGrid( mPosition );
    }
    else
    {
        mPosition = cPaperLogic::MapToGrid( iPosition );
    }
}


void
cUser::setSize( QSize iSize )
{
    mSize = iSize;
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


