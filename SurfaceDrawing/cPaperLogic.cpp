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
        mPaperGrid.push_back( QVector< int >( GRIDSIZE, 0 ) );

    Q_ASSERT( SanityChecks() );
}


// static
QPoint
cPaperLogic::MapToGrid( const QPoint & iPoint )
{
    return  iPoint / CELLSIZE;
}


// static
QPoint
cPaperLogic::MapFromGrid( const QPoint & iPoint )
{
    return  iPoint * CELLSIZE;
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

    mPaperGrid[ x ][ y ] = userIndex;

    ChangeGridValueAt( x,   y,      userIndex + 20 );
    ChangeGridValueAt( x-1, y,      userIndex + 20 );
    ChangeGridValueAt( x+1, y,      userIndex + 20 );
    ChangeGridValueAt( x,   y-1,    userIndex + 20 );
    ChangeGridValueAt( x-1, y-1,    userIndex + 20 );
    ChangeGridValueAt( x+1, y-1,    userIndex + 20 );
    ChangeGridValueAt( x,   y+1,    userIndex + 20 );
    ChangeGridValueAt( x-1, y+1,    userIndex + 20 );
    ChangeGridValueAt( x+1, y+1,    userIndex + 20 );
}


void
cPaperLogic::Update()
{
    int index = 1; // starts at player 1 => 1
    for( auto user : mAllUsers )
    {
        int x = user->mPosition.x();
        int y = user->mPosition.y();

        if( mPaperGrid[ x ][ y ] != index + 20 ) // 21 - 29 = players ground
            ChangeGridValueAt( x, y, 11 ); // 11 == papertrail
        else
            FillZone( index );


        // User movement
        user->setGUIPosition( user->mGUIPosition + user->mGUICurrentMovementVector );
        ChangeGridValueAt( x, y, 1 ); // 1 == userPosiiton

        ++index;
    }
}


void
cPaperLogic::AddGridChangedCB( std::function<void( int, int, int )> iCB )
{
    mCBList.push_back( iCB );
}





// ______________ //
// ______________ //
// ______________ //


void
cPaperLogic::ChangeGridValueAt( int x, int y, int value )
{
    mPaperGrid[ x ][ y ] = value;
    _CallCB( x, y, value );
}


void
cPaperLogic::FillZone( int iIndex )
{

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
cPaperLogic::_CallCB( int x, int y, int newValue )
{
    for( auto cb : mCBList )
        cb( x, y, newValue );
}
