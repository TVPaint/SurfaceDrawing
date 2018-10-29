#include "cPaperLogic.h"


#include "cUser.h"

#include <stack>

#include <QDebug>
#include <QDataStream>

#define  CELLAT( point ) mPaperGrid[ point.x()][point.y()]
#define  SPAWNINGAREAREQUIRED 5 // 5x5 ( spawns are 3x3, here we let a little room )


cPaperLogic::~cPaperLogic()
{
}


cPaperLogic::cPaperLogic()
{
}


void
cPaperLogic::CopyFromPaper( const cPaperLogic& iPaper )
{
    for( auto user : mAllUsers )
    {
        int index = mAllUsers.key( user );
        user->copyFromUser( iPaper.mAllUsers[ index ] );
    }

    for( int x = 0; x < GRIDSIZE; ++x )
        for( int y = 0; y < GRIDSIZE; ++y )
        {
            auto cell = CELLAT( QPoint( x, y ) );
            auto externCell = iPaper.mPaperGrid[ x ][ y ];
            if( cell == externCell )
                continue;


            //if( externCell.mPlayer != cell.mPlayer )
            //{
            //    cell.mPlayer = externCell.mPlayer;
            //    _CallCB( x, y, externCell.mPlayer, kPlayer ); // Only x and y matters here as canvas CB only uses them if player changed
            //}

            //if( externCell.mGround != cell.mGround )
            //{
            //    cell.mGround = externCell.mGround;
            //    _CallCB( x, y, externCell.mGround, kGround ); // Only x and y matters here as canvas CB only uses them if player changed
            //}

            //if( externCell.mTrail != cell.mTrail )
            //{
            //    cell.mTrail = externCell.mTrail;
            //    _CallCB( x, y, externCell.mTrail, kTrail ); // Only x and y matters here as canvas CB only uses them if player changed
            //}



            CELLAT( QPoint( x, y ) ) = iPaper.mPaperGrid[ x ][ y ];
            _CallCB( x, y, -5, kTrail ); // Only x and y matters here as canvas CB only uses them if player changed
        }
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
    if( iIndex < 0 || mAllUsers[ iIndex ]->mIsDead )
        return  Qt::transparent;

    return  mAllUsers[ iIndex ]->mColor;
}


void
cPaperLogic::AddUser( cUser * iUser )
{
    mAllUsers.insert( iUser->mIndex, iUser );

    SpawnUserAtPoint( iUser, iUser->mPosition );
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

        if( newPosition.x() < 0 || newPosition.x() >= GRIDSIZE
            || newPosition.y() < 0 || newPosition.y() >= GRIDSIZE )
        {
            KillUser( user );
            return;
        }

        SetPlayerValueAt( newPosition, user->mIndex );

        if( oldPosition != newPosition && CELLAT(oldPosition).mGround != user->mIndex )
        {
            _AddTrailAtIndex( oldPosition, user );
        }

        // TRAILS
        if( CELLAT(newPosition).mGround == user->mIndex && user->mIsOutOfGround == true ) // if we land back on our ground
        {
            user->mIsOutOfGround = false;
            FillZone( user );
        }
        else if( CELLAT(newPosition).mGround != user->mIndex && user->mIsOutOfGround == false ) // If we leave our land
        {
            user->mIsOutOfGround = true;
        }
        else if( CELLAT( newPosition).mTrail >= 0 ) // If user encounters a trail
        {
            KillUser( mAllUsers[ CELLAT(newPosition).mTrail ] );
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
cPaperLogic::FillZone( cUser*  iUser )
{
    for( auto& point : iUser->mTrailPoints )
    {
        SetTrailValueAt( point, -1 );
        SetGroundValueAt( point, iUser->mIndex );
    }
    iUser->mTrailPoints.clear();

    // add 1 row to be sure to be outside of user ground
    const int MASKWIDTH = GRIDSIZE;
    const int MASKHEIGHT = GRIDSIZE + 1;

    std::vector< bool >  mask( MASKWIDTH * MASKHEIGHT, false );

    #define POINT2MASK( point ) point.x() + (point.y() + 1) * MASKWIDTH
    #define IS_MASK_VALID( index ) index >= 0 && index < mask.size()

    for( int y = 0; y < GRIDSIZE; ++y )
    {
        for( int x = 0; x < GRIDSIZE; ++x )
        {
            QPoint point(x, y);
            mask[POINT2MASK(point)] = CELLAT(point).mGround == iUser->mIndex;
        }
    }

    // we need to find the first point outside of player area : 0 -1 should be ok!
    std::stack< QPoint >  stack;
    QPoint empty(0, -1);
    if( !mask[POINT2MASK(empty)] )
        stack.push( empty );

    while( !stack.empty() )
    {
        auto p = stack.top();
        stack.pop();

        mask[POINT2MASK(p)] = true;
        auto NEIGHBORHOOD = {
            p + QPoint( -1,  0 ),
            p + QPoint( +1,  0 ),
            p + QPoint(  0, +1 ),
            p + QPoint(  0, -1 ),
        };
        for( auto n: NEIGHBORHOOD )
        {
            if( IS_MASK_VALID(POINT2MASK(n)) && !mask[POINT2MASK(n)] )
                stack.push(n);
        }
    }

    for( int y = 0; y < GRIDSIZE; ++y )
    {
        for( int x = 0; x < GRIDSIZE; ++x )
        {
            QPoint point(x, y);
            if( !mask[POINT2MASK(point)] )
            {
                SetGroundValueAt( point, iUser->mIndex );
            }
        }
    }
}


void
cPaperLogic::KillUser( cUser* iUser )
{
    iUser->mIsDead = true;
    iUser->mGUICurrentMovementVector = QPoint( 0, 0 );
    iUser->mGUIMovementVector = QPoint( 0, 0 );
    iUser->mTrailPoints.clear();
    iUser->setPosition( QPoint( 1, 1 ) );

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

            CELLAT( pos ).mSpawnIsImpossibleHere = false;
        }
    }
}


void
cPaperLogic::TryRespawningPlayer( cUser*  iUser )
{
    if( !iUser->mIsDead )
        return;

    for( int x = SPAWNINGAREAREQUIRED / 2; x < GRIDSIZE - SPAWNINGAREAREQUIRED / 2; ++x )
    {
        for( int y = SPAWNINGAREAREQUIRED / 2; y < GRIDSIZE - SPAWNINGAREAREQUIRED / 2; ++y )
        {
            QPoint cellPos( x, y );

            if( CELLAT( cellPos ).mSpawnIsImpossibleHere )
                continue;

            if( _IsAvailableSpaceAtPoint( cellPos ) )
            {
                SpawnUserAtPoint( iUser, cellPos );
                return;
            }
        }
    }
}


void
cPaperLogic::SpawnUserAtPoint( cUser*  iUser, const QPoint& iPoint )
{
    iUser->setPosition( iPoint );
    iUser->mIsDead = false;
    iUser->mGUIMovementVector = QPoint( 1, 0 );
    iUser->mGUICurrentMovementVector = QPoint( 1, 0 );
    iUser->mIsOutOfGround = false;

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
cPaperLogic::_AddTrailAtIndex( const QPoint& iPoint, cUser*  iUser )
{
    SetTrailValueAt( iPoint, iUser->mIndex );

    if( !iUser->mTrailPoints.contains( iPoint ) )
        iUser->mTrailPoints.append( iPoint );
}


bool
cPaperLogic::_IsAvailableSpaceAtPoint( const QPoint & iPoint ) const
{
    bool result = true;
    for( int x = iPoint.x() - SPAWNINGAREAREQUIRED / 2; x < iPoint.x() + SPAWNINGAREAREQUIRED / 2; ++x )
    {
        for( int y = iPoint.y() - SPAWNINGAREAREQUIRED / 2; y < iPoint.y() + SPAWNINGAREAREQUIRED / 2; ++y )
        {
            eDataCell cell = mPaperGrid[ x ][ y ];
            if( cell.mSpawnIsImpossibleHere )
                continue;

            if( result == false )
            {
                cell.mSpawnIsImpossibleHere = true;
            }
            else if( cell.mGround >= 0 || cell.mTrail >= 0 ) // If there is a player's ground or trail, we can't spawn here
            {
                result = false;
                // We reset the loop, to set all cells at impossible spawn
                x = iPoint.x() - SPAWNINGAREAREQUIRED / 2;
                y = iPoint.y() - SPAWNINGAREAREQUIRED / 2;
            }
        }
    }

    return  result;
}


QDataStream&
operator<<(QDataStream& oStream, const cPaperLogic::eDataCell& iDataCell )
{
    oStream << compute_hash( "cPaperGrid::eDataCell" );
    oStream << iDataCell.mPlayer
            << iDataCell.mTrail
            << iDataCell.mGround;
            //<< iDataCell.mSpawnIsImpossibleHere;

    return  oStream;
}


QDataStream&
operator>>(QDataStream& iStream, cPaperLogic::eDataCell& oDataCell )
{
    uint32_t  id;
    iStream >> id;
    if( id != compute_hash( "cPaperGrid::eDataCell" ) )
    {
        //qDebug() << "Invalid cPaperLogic::eDataCell object!";
        return  iStream;
    }

    iStream >> oDataCell.mPlayer
            >> oDataCell.mTrail
            >> oDataCell.mGround;
            //>> oDataCell.mSpawnIsImpossibleHere;

    return  iStream;
}


std::ostream&
operator<<( std::ostream& oStream, const cPaperLogic& iPaperLogic )
{
    oStream << "cPaperGrid (" << compute_hash( "cPaperGrid" ) << "):" << std::endl;
    oStream << "User number: " << iPaperLogic.mAllUsers.size() << std::endl;
    for( auto user: iPaperLogic.mAllUsers )
        oStream << *user << std::endl;
    return  oStream;
}

QDebug&
operator<<( QDebug& oStream, const cPaperLogic& iPaperLogic )
{
    oStream << "cPaperGrid (" << compute_hash( "cPaperGrid" ) << "):";
    oStream << "User number: " << iPaperLogic.mAllUsers.size();
    for( auto user: iPaperLogic.mAllUsers )
        oStream << *user;
    return  oStream;
}



QDataStream&
operator<<(QDataStream& oStream, const cPaperLogic& iPaperLogic )
{
    oStream << compute_hash( "cPaperGrid" );
    oStream << iPaperLogic.mPaperGrid
            << iPaperLogic.mAllUsers;

    return  oStream;
}


QDataStream&
operator>>(QDataStream& iStream, cPaperLogic& oPaperLogic )
{
    uint32_t  id;
    iStream >> id;
    if( id != compute_hash( "cPaperGrid" ) )
    {
        //qDebug() << "Invalid cPaperLogic object!";
        return  iStream;
    }

    iStream >> oPaperLogic.mPaperGrid
            >> oPaperLogic.mAllUsers;


    return  iStream;
}
