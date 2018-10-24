#include "cPaperLogic.h"


#include "cUser.h"


#define  CELLAT( point ) mPaperGrid[ point.x()][point.y()]



cPaperLogic::~cPaperLogic()
{
}


cPaperLogic::cPaperLogic()
{
}


void
cPaperLogic::Init()
{
    for( int i = 0; i < GRIDSIZE; ++i )
        mPaperGrid.push_back( QVector< eDataCell >( GRIDSIZE, eDataCell() ) );

    Q_ASSERT( SanityChecks() );
}


// static
QPoint
cPaperLogic::MapToGrid( const QPoint & iPoint )
{
    return  QPoint( iPoint.x() / CELLSIZE, iPoint.y() / CELLSIZE );
}


// static
QPoint
cPaperLogic::MapFromGrid( const QPoint & iPoint )
{
    return  QPoint( iPoint.x() * CELLSIZE, iPoint.y() * CELLSIZE );
}


QColor
cPaperLogic::GetColorByIndex( int iIndex )
{
    if( iIndex < 0 )
        return  Qt::transparent;


    if( iIndex == 0 )
        return  Qt::red;
    else
        return  Qt::blue;

    return  QColor( (iIndex * 50)       % 256,
                    ((iIndex + 7) * 50) % 256,
                    (iIndex * 120)      % 256
    );
}


void
cPaperLogic::AddUser( cUser * iUser )
{
    mAllUsers.insert( iUser->mIndex, iUser );

    SetPlayerValueAt( iUser->mPosition,                     iUser->mIndex );

    SetGroundValueAt( iUser->mPosition + QPoint( -1, -1 ),  iUser->mIndex );
    SetGroundValueAt( iUser->mPosition + QPoint( -1, 0 ),   iUser->mIndex );
    SetGroundValueAt( iUser->mPosition + QPoint( -1, 1 ),   iUser->mIndex );
    SetGroundValueAt( iUser->mPosition + QPoint( 0, -1 ),   iUser->mIndex );
    SetGroundValueAt( iUser->mPosition + QPoint( 0, 0 ),    iUser->mIndex );
    SetGroundValueAt( iUser->mPosition + QPoint( 0, 1 ),    iUser->mIndex );
    SetGroundValueAt( iUser->mPosition + QPoint( 1, -1 ),   iUser->mIndex );
    SetGroundValueAt( iUser->mPosition + QPoint( 1, 0 ),    iUser->mIndex );
    SetGroundValueAt( iUser->mPosition + QPoint( 1, 1 ),    iUser->mIndex );
}


void
cPaperLogic::Update()
{
    for( auto user : mAllUsers )
    {
        if( !user || user->mIsDead )
            continue;

        // USER
        QPoint oldPosition = user->mPosition;

        SetPlayerValueAt( oldPosition, -1 );

        // User movement
        user->Update();
        QPoint newPosition = user->mPosition;

        SetPlayerValueAt( newPosition, user->mIndex );


        if( oldPosition != newPosition && CELLAT(oldPosition).mGround != user->mIndex )
        {
            _AddTrailAtIndex( oldPosition, user->mIndex );
        }

        // TRAILS
        if( CELLAT(newPosition).mGround == user->mIndex && user->mIsOutOfGround == true ) // if we land back on our ground
        {
            user->mIsOutOfGround = false;
            FillZone( user->mIndex );
        }
        else if( CELLAT(newPosition).mGround != user->mIndex && user->mIsOutOfGround == false ) // If we leave our land
        {
            user->mIsOutOfGround = true;
        }
        else if( CELLAT( newPosition ).mTrail != -1 )
        {
            if( CELLAT(newPosition).mTrail >= 0 )
            {
                KillUser( mAllUsers[ CELLAT(newPosition).mTrail ] );
            }
        }
    }
}


void
cPaperLogic::AddGridChangedCB( std::function<void( int, int, int, eDataType )> iCB )
{
    mCBList.push_back( iCB );
}





// ______________ //
// ______________ //
// ______________ //


void
cPaperLogic::SetPlayerValueAt( const QPoint& iPoint, int value )
{
    mPaperGrid[ iPoint.x() ][ iPoint.y() ].mPlayer = value;
    _CallCB( iPoint.x(), iPoint.y(), value, kPlayer );
}


void
cPaperLogic::SetTrailValueAt( const QPoint& iPoint, int value )
{
    mPaperGrid[ iPoint.x() ][ iPoint.y() ].mTrail = value;
    _CallCB( iPoint.x(), iPoint.y(), value, kTrail );
}


void
cPaperLogic::SetGroundValueAt( const QPoint& iPoint, int value )
{
    mPaperGrid[ iPoint.x() ][ iPoint.y() ].mGround = value;
    _CallCB( iPoint.x(), iPoint.y(), value, kGround );
}





void
cPaperLogic::FillZone( int iIndex )
{
    for( auto& point : mAllUsers[ iIndex ]->mTrailPoints )
    {
        SetTrailValueAt( point, -1 );
        SetGroundValueAt( point, iIndex );
    }

    mAllUsers[ iIndex ]->mTrailPoints.clear();
    //TODO
}


void
cPaperLogic::KillUser( cUser* iUser )
{
    iUser->mColor = Qt::transparent;
    iUser->mIsDead = true;
    iUser->mGUICurrentMovementVector = QPoint( 0, 0 );
    iUser->mGUIMovementVector = QPoint( 0, 0 );

    for( auto row = 0; row < GRIDSIZE; ++row )
    {
        for( auto col = 0; col < GRIDSIZE; ++col )
        {
            QPoint pos( row, col );
            if( CELLAT( pos ).mGround == iUser->mIndex )
            {
                SetGroundValueAt( pos, -2 );
                SetGroundValueAt( pos, -1 );
            }
            if( CELLAT( pos ).mPlayer == iUser->mIndex )
            {
                SetPlayerValueAt( pos, -2 );
                SetPlayerValueAt( pos, -1 );
            }
            if( CELLAT( pos ).mTrail == iUser->mIndex )
            {
                SetTrailValueAt( pos, -2 );
                SetTrailValueAt( pos, -1 );
            }
        }
    }
}












bool
cPaperLogic::SanityChecks() const
{
    QPoint test( 1, 10 );
    auto map = MapFromGrid( test );
    auto map2 = MapToGrid( map );

    Q_ASSERT( map  == QPoint( CELLSIZE, CELLSIZE*10 ) );
    Q_ASSERT( map2 == QPoint( 1, 10 ) );

    return  true;
}


void
cPaperLogic::_CallCB( int x, int y, int newValue, eDataType iDataType )
{
    for( auto cb : mCBList )
        cb( x, y, newValue, iDataType );
}


void
cPaperLogic::_AddTrailAtIndex( const QPoint& iPoint, int iIndex )
{
    SetTrailValueAt( iPoint, iIndex );

    if( !mAllUsers[ iIndex ]->mTrailPoints.contains( iPoint ) )
        mAllUsers[ iIndex ]->mTrailPoints.append( iPoint );
}

