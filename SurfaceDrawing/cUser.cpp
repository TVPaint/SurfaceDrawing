#include "cUser.h"

#include  "cPaperLogic.h"

#include <time.h>

#include <QDataStream>
#include <QDebug>

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


std::ostream&
operator<<( std::ostream& oStream, const cUser& iUser )
{
    oStream << "cUser (" << compute_hash("cUser") << "):" << std::endl;
    oStream << "\tIndex: " << iUser.mIndex << std::endl
            << "\tColor: " << QVariant(iUser.mColor).toString().toStdString() << std::endl
            << "\tPosition: " << iUser.mPosition.x() << "," << iUser.mPosition.y() << std::endl
            << "\tGUIPosition: " << iUser.mGUIPosition.x() << "," << iUser.mGUIPosition.y() << std::endl
            << "\tGUISize: " << iUser.mGUISize.x() << "," << iUser.mGUISize.y() << std::endl
            << "\tGUIMovementVector: " << iUser.mGUIMovementVector.x() << "," << iUser.mGUIMovementVector.y() << std::endl
            << "\tGUICenter: " << iUser.mGUICenter.x() << "," << iUser.mGUICenter.y() << std::endl
            << "\tGUICurrentMovementVector: " << iUser.mGUICurrentMovementVector.x() << "," << iUser.mGUICurrentMovementVector.y() << std::endl
            << "\tAskDirectionChange: " << iUser.mAskDirectionChange << std::endl
            << "\tIsOutOfGround: " << iUser.mIsOutOfGround << std::endl
            << "\tIsDead: " << iUser.mIsDead << std::endl
            << "\tTrailPoints: " << iUser.mTrailPoints.size() << std::endl;
}

QDataStream&
operator<<(QDataStream& oStream, const cUser& iUser )
{
    oStream << compute_hash("cUser");
    oStream << iUser.mIndex
            << iUser.mColor
            << iUser.mPosition
            << iUser.mGUIPosition
            << iUser.mGUISize
            << iUser.mGUIMovementVector
            << iUser.mGUICenter
            << iUser.mGUICurrentMovementVector
            << iUser.mAskDirectionChange
            << iUser.mIsOutOfGround
            << iUser.mIsDead
            << iUser.mTrailPoints;

    return  oStream;
}

QDataStream&
operator>>(QDataStream& iStream, cUser& oUser )
{
    uint32_t  id;
    iStream >> id;
    if( id != compute_hash("cUser") )
    {
        qDebug() << "Invalid user object!";
        return iStream;
    }

    iStream >> oUser.mIndex
            >> oUser.mColor
            >> oUser.mPosition
            >> oUser.mGUIPosition
            >> oUser.mGUISize
            >> oUser.mGUIMovementVector
            >> oUser.mGUICenter
            >> oUser.mGUICurrentMovementVector
            >> oUser.mAskDirectionChange
            >> oUser.mIsOutOfGround
            >> oUser.mIsDead;
    oUser.mTrailPoints.clear();
    iStream >> oUser.mTrailPoints;
    return  iStream;
}

QDataStream&
operator<<(QDataStream& oStream, const cUser* iUser )
{
    oStream << *iUser;
    return oStream;
}


QDataStream&
operator>>(QDataStream& iStream, cUser*& oUser )
{
    oUser = new cUser( -1 );
    iStream >> *oUser;
    return iStream;
}
