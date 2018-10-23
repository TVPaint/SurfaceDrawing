#include "cPaperLogic.h"


#include "cUser.h"


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
    return  QColor( (iIndex * 50)       % 256,
                    ((iIndex + 7) * 50) % 256,
                    (iIndex * 120)      % 256
    );
}


void
cPaperLogic::AddUser( cUser * iUser )
{
    mAllUsers.push_back( iUser );

    int x = iUser->mPosition.x();
    int y = iUser->mPosition.y();
    int userIndex = 1;

    SetPlayerValueAt( x,   y,       userIndex );
    SetGroundValueAt( x,   y,       userIndex );
    SetGroundValueAt( x-1, y,       userIndex );
    SetGroundValueAt( x+1, y,       userIndex );
    SetGroundValueAt( x,   y-1,     userIndex );
    SetGroundValueAt( x-1, y-1,     userIndex );
    SetGroundValueAt( x+1, y-1,     userIndex );
    SetGroundValueAt( x,   y+1,     userIndex );
    SetGroundValueAt( x-1, y+1,     userIndex );
    SetGroundValueAt( x+1, y+1,     userIndex );
}


void
cPaperLogic::Update()
{
    int index = 1;
    for( auto user : mAllUsers )
    {
        // PLAYER
        int oldX = user->mPosition.x();
        int oldY = user->mPosition.y();

        SetPlayerValueAt( oldX, oldY, 0 );

        // User movement
        user->setGUIPosition( user->mGUIPosition + user->mGUICurrentMovementVector );
        int newX = user->mPosition.x();
        int newY = user->mPosition.y();

        SetPlayerValueAt( newX, newY, index );



        // TRAILS
        if( mPaperGrid[ newX ][ newY ].mGround == index && user->mIsOutOfGround == true ) // if we land back on our ground
        {
            user->mIsOutOfGround = false;
            user->mColor = GetColorByIndex( index );
            _AddTrailAtIndex( oldX, oldY, index ); // Add the last trail to close the path
            FillZone( index );
        }
        else if( mPaperGrid[ newX ][ newY ].mGround != index && user->mIsOutOfGround == false ) // If we leave our land
        {
            user->mIsOutOfGround = true;
            user->mColor = Qt::red;
        }
        else if( mPaperGrid[ newX ][ newY ].mTrail == index ) // If we leave our land
        {
            //user->mIsOutOfGround = true;
            //user->mColor = Qt::red;
        }


        if( /*mPaperGrid[ oldX ][ oldY ].mPlayer != index &&*/ mPaperGrid[ oldX ][ oldY ].mGround != index )
        {
            _AddTrailAtIndex( oldX, oldY, index );
        }

        ++index;
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
cPaperLogic::SetPlayerValueAt( int x, int y, int value )
{
    mPaperGrid[ x ][ y ].mPlayer = value;
    _CallCB( x, y, value, kPlayer );
}


void
cPaperLogic::SetTrailValueAt( int x, int y, int value )
{
    mPaperGrid[ x ][ y ].mTrail = value;
    _CallCB( x, y, value, kTrail );
}


void
cPaperLogic::SetGroundValueAt( int x, int y, int value )
{
    mPaperGrid[ x ][ y ].mGround = value;
    _CallCB( x, y, value, kGround );
}





void
cPaperLogic::FillZone( int iIndex )
{
    for( auto& point : mTrailPoints )
    {
        SetTrailValueAt( point.x(), point.y(), 0 );
        SetGroundValueAt( point.x(), point.y(), iIndex );
    }

    mTrailPoints.clear();
    //TODO
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
cPaperLogic::_AddTrailAtIndex( int iX, int iY, int iIndex )
{
    SetTrailValueAt( iX, iY, iIndex );

    QPoint userPosAsQPoint( iX, iY );
    if( !mTrailPoints.contains( userPosAsQPoint ) )
        mTrailPoints.append( userPosAsQPoint );

}
