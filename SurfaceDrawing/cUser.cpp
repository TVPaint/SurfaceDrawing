#include "cUser.h"

#include  "cPaperLogic.h"

#include <time.h>

#include <QDataStream>
#include <QDebug>

cUser::~cUser()
{
}


cUser::cUser() :
    mIndex( -5 ),
    mAskDirectionChange( false ),
    mIsOutOfGround( false )
{
    mColor = Qt::transparent;
    mIsDead = false;
    setSize( QPoint( 1, 1 ) ); // Grid coordinates
}


cUser::cUser( int iIndex, const QColor& iColor ) :
    mIndex( iIndex ),
    mAskDirectionChange( false ),
    mIsOutOfGround( false )
{
    mColor = iColor;

    setSize( QPoint( 1, 1 ) ); // Grid coordinates
    setMovementVector( QPoint( 1, 0 ) );
    mGUICurrentMovementVector = mGUIMovementVector;
    mIsDead = true;
}


void
cUser::copyFromUser( const cUser * iUser )
{
    mPosition = iUser->mPosition;
    mColor = iUser->mColor;

    mGUICurrentMovementVector = iUser->mGUICurrentMovementVector;
    mGUIMovementVector = iUser->mGUIMovementVector;
    mGUIPosition = iUser->mGUIPosition;
    mGUISize = iUser->mGUISize;

    mAskDirectionChange = iUser->mAskDirectionChange;
    mIsOutOfGround = iUser->mIsOutOfGround;
    mIsDead = iUser->mIsDead;

    if( mTrailPoints.size() != iUser->mTrailPoints.size() )
        mTrailPoints = iUser->mTrailPoints;
}




void
cUser::setPosition( QPoint iPosition )
{
    mPosition = iPosition;
    mGUIPosition = cPaperLogic::MapFromGrid( iPosition );
}


void
cUser::Update( int iTickCount )
{
    mGUIPosition += mGUICurrentMovementVector * iTickCount;

    QPoint newCenter = mGUIPosition + mGUISize * 0.5;
    QPoint newCell   = cPaperLogic::MapToGrid( newCenter );
    mPosition = newCell;

    if( iTickCount > 0 && mAskDirectionChange )
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

    //*mDEBUGStream   << "====================USER "
    //                << mIndex
    //                << " POS -----> "
    //                << mGUIPosition.x() << "-" << mGUIPosition.y() << "\n";
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
            << "\tGUICurrentMovementVector: " << iUser.mGUICurrentMovementVector.x() << "," << iUser.mGUICurrentMovementVector.y() << std::endl
            << "\tAskDirectionChange: " << iUser.mAskDirectionChange << std::endl
            << "\tIsOutOfGround: " << iUser.mIsOutOfGround << std::endl
            << "\tIsDead: " << iUser.mIsDead << std::endl
            << "\tTrailPoints: " << iUser.mTrailPoints.size() << std::endl;
    return oStream;
}


QDebug&
operator<<( QDebug& oStream, const cUser& iUser )
{
    oStream << "cUser (" << compute_hash("cUser") << "):";
    oStream << "\tIndex: " << iUser.mIndex
        << "\tColor: " << QVariant(iUser.mColor).toString()
        << "\tPosition: " << iUser.mPosition.x() << "," << iUser.mPosition.y()
        << "\tGUIPosition: " << iUser.mGUIPosition.x() << "," << iUser.mGUIPosition.y()
        << "\tGUISize: " << iUser.mGUISize.x() << "," << iUser.mGUISize.y()
        << "\tGUIMovementVector: " << iUser.mGUIMovementVector.x() << "," << iUser.mGUIMovementVector.y()
        << "\tGUICurrentMovementVector: " << iUser.mGUICurrentMovementVector.x() << "," << iUser.mGUICurrentMovementVector.y()
        << "\tAskDirectionChange: " << iUser.mAskDirectionChange
        << "\tIsOutOfGround: " << iUser.mIsOutOfGround
        << "\tIsDead: " << iUser.mIsDead
        << "\tTrailPoints: " << iUser.mTrailPoints.size();
    return oStream;
}


QDataStream&
operator<<(QDataStream& oStream, const cUser& iUser )
{
    oStream << compute_hash("cUser");
    oStream << iUser.mIndex
            << iUser.mColor
            << iUser.mPosition
            << iUser.mGUIPosition
            //<< iUser.mGUISize
            << iUser.mGUIMovementVector
            << iUser.mGUICurrentMovementVector
            << iUser.mAskDirectionChange
            //<< iUser.mIsOutOfGround
            << iUser.mIsDead;
            //<< iUser.mTrailPoints;

    return  oStream;
}

QDataStream&
operator>>(QDataStream& iStream, cUser& oUser )
{
    uint32_t  id;
    iStream >> id;
    if( id != compute_hash("cUser") )
    {
        //qDebug() << "Invalid user object!";
        return iStream;
    }

    iStream >> oUser.mIndex
            >> oUser.mColor
            >> oUser.mPosition
            >> oUser.mGUIPosition
            //>> oUser.mGUISize
            >> oUser.mGUIMovementVector
            >> oUser.mGUICurrentMovementVector
            >> oUser.mAskDirectionChange
            //>> oUser.mIsOutOfGround
            >> oUser.mIsDead;

    //oUser.mTrailPoints.clear();
    //iStream >> oUser.mTrailPoints;

    //oUser.mPosition = cPaperLogic::MapToGrid( oUser.mGUIPosition );


//
//    if( oUser.mIndex >= 0 && !oUser.mDEBUGFile )
//    {
//        QString wher = "CLIENT";
//#ifdef SERVERSIDE
//        wher = "SERVER";
//#endif
//        oUser.mDEBUGFile = new QFile( "./DEBUGLOGS/deb" + QString::number( oUser.mIndex ) + wher + ".txt" );
//        oUser.mDEBUGFile->open( QIODevice::WriteOnly );
//        oUser.mDEBUGStream = new QTextStream( oUser.mDEBUGFile );
//    }

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
    oUser = new cUser( -1, Qt::transparent );
    iStream >> *oUser;
    return iStream;
}
