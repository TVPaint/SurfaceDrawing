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


    mComps.push_back( cRollBack() );
    mComps.push_back( cSpeed() );
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


    mComps.push_back( cRollBack() );
    mComps.push_back( cSpeed() );
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
    mAskedRespawn = iUser->mAskedRespawn;

    if( mTrailPoints.size() != iUser->mTrailPoints.size() )
        mTrailPoints = iUser->mTrailPoints;


    for( int i = 0; i < iUser->mComps.size(); ++i )
        mComps[ i ] = iUser->mComps[ i ];

    mCompPower = iUser->mCompPower;
}




void
cUser::setPosition( QPoint iPosition )
{
    mPosition = iPosition;
    mGUIPosition = cPaperLogic::MapFromGrid( iPosition );
}


void
cUser::setGUIPosition( QPoint iGUIPosition )
{
    mGUIPosition = iGUIPosition;
    QPoint newCenter = mGUIPosition + mGUISize * 0.5;
    mPosition = cPaperLogic::MapToGrid( newCenter );
}


void
cUser::Update( int iTickCount )
{
    setGUIPosition( mGUIPosition + mGUICurrentMovementVector * iTickCount * mSpeedMultiplicator );
    QPoint newCenter = mGUIPosition + mGUISize * 0.5;

    if( iTickCount > 0 && mAskDirectionChange )
    {
        QPoint centerNewCell = mPosition * CELLSIZE + QPoint( CELLSIZE / 2, CELLSIZE / 2 );
        QPoint delta = centerNewCell - newCenter;
        if( std::abs( delta.x() ) < 2 && std::abs( delta.y() ) < 2 )
        {
            mGUICurrentMovementVector = mGUIMovementVector;
            mAskDirectionChange = false;
            mGUIPosition = cPaperLogic::MapFromGrid( mPosition );
        }
    }

    UpdateComps( iTickCount );
}


void
cUser::UpdateComps( int iTickCount )
{
    for( auto& comp : mComps )
    {
        if( comp.mCooldown > 0 )
            comp.mCooldown -= iTickCount;

        if( comp.mCompDuration > 0 )
            comp.mCompDuration -= iTickCount;
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


void
cUser::activateComp( int iCompNumber )
{
    if( mComps[ iCompNumber ].mCooldown <= 0 && !mComps[ iCompNumber ].mActive && mComps[ iCompNumber ].mCompDuration <= 0 )
    {
        mComps[ iCompNumber ].mActive = true;
        mComps[ iCompNumber ].mCompDuration = mComps[ iCompNumber ].mCompDurationBase;
    }
}


void
cUser::deactivateComp( int iCompNumber )
{
    mComps[ iCompNumber ].mActive = false;
    mComps[ iCompNumber ].mCompDuration = 0;
    mCompPower = 1;
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
            << "\tAskedRespawn: " << iUser.mAskedRespawn << std::endl
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
        << "\tAskedRespawn: " << iUser.mAskedRespawn
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
            << iUser.mIsDead
            << iUser.mAskedRespawn
            << iUser.mSpeedMultiplicator
            << iUser.mComps
            << iUser.mCompPower;
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
            >> oUser.mIsDead
            >> oUser.mAskedRespawn
            >> oUser.mSpeedMultiplicator
            >> oUser.mComps
            >> oUser.mCompPower;

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
