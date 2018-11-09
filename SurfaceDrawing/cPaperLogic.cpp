#include "cPaperLogic.h"


#include "cUser.h"

#include <stack>
#include <random>

#include <QDebug>
#include <QDataStream>

#define  CELLAT( point ) mPaperGrid[ point.x()][point.y()]
#define  SPAWNINGAREAREQUIRED 5 // 5x5 ( spawns are 3x3, here we let a little room )


cPaperLogic::~cPaperLogic()
{
}


cPaperLogic::cPaperLogic():
    mTick( 0 ),
    mSnapShots( 100 )
{
    mSnapShots.Write( new cSnapShot( 0 ) ); // Initial SS
}


void
cPaperLogic::CopyFromPaper( const cPaperLogic& iPaper, quint16 iMissingUpdates, eRollBackType iType )
{
    if( iType != kKeepOwnTick )
        mTick = iPaper.mTick;

    mSnapShots.Write( new cSnapShot( mTick ) ); // SnapShot for the copy step

    for( auto user : mAllUsers )
    {
        if( !user )
            continue;

        int index = mAllUsers.key( user );
        user->copyFromUser( iPaper.mAllUsers[ index ] );
        mSnapShots.Back()->AddUserDiff( user );
    }

    for( int x = 0; x < GRIDSIZE; ++x )
        for( int y = 0; y < GRIDSIZE; ++y )
        {
            QPoint cellPoint( x, y );
            auto externCell = iPaper.mPaperGrid[ x ][ y ];
            if( CELLAT( cellPoint ) == externCell )
                continue;

            CELLAT( cellPoint ) = externCell;

            // Clear tile
            _CallCB( x, y, -2, kTrail ); // Only x and y matters here as canvas CB only uses them if player changed

            mSnapShots.Back()->AddCellDiff( cellPoint, CELLAT( cellPoint ) );

            // Setup tile
            _CallCB( x, y, CELLAT( cellPoint ).mPlayer, kPlayer );
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
    if( iIndex < 0 || !mAllUsers[ iIndex ] || mAllUsers[ iIndex ]->mIsDead )
        return  Qt::transparent;

    return  mAllUsers[ iIndex ]->mColor;
}


void
cPaperLogic::AddUser( cUser * iUser )
{
    mAllUsers.insert( iUser->mIndex, iUser );
    TryRespawningPlayer( iUser );
}


void
cPaperLogic::RemoveUser( cUser * iUser )
{
    KillUser( iUser );
    mAllUsers.erase( mAllUsers.find( mAllUsers.key( iUser ) ) );
}


void
cPaperLogic::Update( quint64 iCurrentTimeRemaining )
{
    qint64 deltaTimeMs  = mPreviousTime - iCurrentTimeRemaining;
    mPreviousTime       = iCurrentTimeRemaining;

    if( deltaTimeMs < 0 )
        return;

    mTimeBuffer += deltaTimeMs;

    int tickCount = mTimeBuffer / SPEED; // Because we want 1 pixel per SPEEDms
    mTimeBuffer = mTimeBuffer % SPEED; // This is remaining ms that doesn't make a full tick, so we add them next round


    GoToTick( mTick + tickCount ); //TODO: This is annoying, it denies the call of TickUpdate, maybe do something like AdvanceByTick( deltaTick ), and do something like
    /*

    int oldTick = mTick
    UpdateTick();
    AdvanceBy( mTick - oldTick );
    */


    mTick += tickCount;
}


void
cPaperLogic::TickUpdate( quint64 iCurrentTimeRemaining )
{
    qint64 deltaTimeMs  = mPreviousTime - iCurrentTimeRemaining;
    mPreviousTime       = iCurrentTimeRemaining;

    if( deltaTimeMs < 0 )
        return;

    mTimeBuffer += deltaTimeMs;

    int tickCount = mTimeBuffer / SPEED; // Because we want 1 pixel per SPEEDms
    mTimeBuffer = mTimeBuffer % SPEED; // This is remaining ms that doesn't make a full tick, so we add them next round

    mTick += tickCount;
}


void
cPaperLogic::GoToTick( quint64 iTick )
{
    int deltaTick = int(iTick - mTick);

    if( deltaTick == 0 )
    {
        //qDebug() << "*********************************Nothing";
        return;
    }
    else if( deltaTick > 0 ) // Advance in ticks
    {
        mSnapShots.Write( new cSnapShot( mTick + deltaTick ) );

        //qDebug() << "*********************************FWD : " << deltaTick;
        for( auto user : mAllUsers )
        {
            if( !user || user->mIsDead )
                continue;

            // USER
            QPoint oldPosition = user->mPosition;

            SetPlayerValueAt( oldPosition, -1 );

            // User movement
            user->Update( deltaTick );
            mSnapShots.Back()->AddUserDiff( user );

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
    else if( deltaTick < 0 ) // Go back in time
    {
        ApplySnapShotHistoryUpToTick( iTick, kKeepOwnTick );
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
    CELLAT( iPoint ).mPlayer = value;
    mSnapShots.Back()->AddCellDiff( iPoint, CELLAT( iPoint ) );
    _CallCB( iPoint.x(), iPoint.y(), value, kPlayer );
}


void
cPaperLogic::SetTrailValueAt( const QPoint& iPoint, int value )
{
    CELLAT( iPoint ).mTrail = value;
    mSnapShots.Back()->AddCellDiff( iPoint, CELLAT( iPoint ) );
    _CallCB( iPoint.x(), iPoint.y(), value, kTrail );
}


void
cPaperLogic::SetGroundValueAt( const QPoint& iPoint, int value )
{
    CELLAT( iPoint ).mGround = value;
    mSnapShots.Back()->AddCellDiff( iPoint, CELLAT( iPoint ) );
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
    qDebug() << "KILL=================================================================";
    iUser->mIsDead = true;
    iUser->mGUICurrentMovementVector = QPoint( 0, 0 );
    iUser->mGUIMovementVector = QPoint( 0, 0 );
    iUser->mTrailPoints.clear();
    iUser->setPosition( QPoint( 1, 1 ) );
    mSnapShots.Back()->AddUserDiff( iUser );

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

    const int WIDTH = GRIDSIZE - (2 * SPAWNINGAREAREQUIRED);
    const int HEIGHT = WIDTH;
    QVector< bool >  spawnIsPossible( WIDTH * HEIGHT, true );

    auto _SetNotSpawnable = [&spawnIsPossible, WIDTH, HEIGHT] (const QPoint&  iPoint )
    {
        auto point = iPoint - QPoint( SPAWNINGAREAREQUIRED, SPAWNINGAREAREQUIRED );

        for( int dy = -SPAWNINGAREAREQUIRED / 2; dy <= SPAWNINGAREAREQUIRED / 2; ++dy )
        {
            for( int dx = -SPAWNINGAREAREQUIRED / 2; dx <= SPAWNINGAREAREQUIRED / 2; ++dx )
            {
                auto p = point + QPoint( dx, dy );
                if( p.y() < 0 || p.y() >= HEIGHT )
                    break;
                if( p.x() < 0 || p.x() >= WIDTH )
                    continue;
                spawnIsPossible[p.x() + p.y() * WIDTH] = false;
            }
        }
    };

    for( int y = 0; y < GRIDSIZE; ++y )
    {
        for( int x = 0; x < GRIDSIZE; ++x )
        {
            QPoint  point( x, y ); 
            eDataCell  cell = CELLAT( point );
            if( !cell.empty() )
                _SetNotSpawnable( point );
        }    
    }

    std::vector< QPoint >  elligibleSpawnPoint;
    for( int i = 0; i < spawnIsPossible.size(); ++i )
    {    
        if( spawnIsPossible[i] )
        {
            elligibleSpawnPoint.push_back( QPoint( i % WIDTH, i / WIDTH ) + QPoint( SPAWNINGAREAREQUIRED, SPAWNINGAREAREQUIRED ) );
        }
    }

    if( elligibleSpawnPoint.empty() )
        return;

    std::random_device  generator;
    std::uniform_int_distribution< int >  distribution( 0, elligibleSpawnPoint.size() - 1 );

    QPoint newStart = elligibleSpawnPoint[distribution( generator )];

    SpawnUserAtPoint( iUser, newStart );
    return;
}


void
cPaperLogic::SpawnUserAtPoint( cUser*  iUser, const QPoint& iPoint )
{
    iUser->setPosition( iPoint );
    iUser->mIsDead = false;
    iUser->mGUIMovementVector = QPoint( 1, 0 );
    iUser->mGUICurrentMovementVector = QPoint( 1, 0 );
    iUser->mIsOutOfGround = false;
    mSnapShots.Back()->AddUserDiff( iUser );

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


cSnapShot*
cPaperLogic::FindSnapShotByTick( quint64 iTick )
{
    for( int i = 0; i < mSnapShots.Count(); ++i )
    {
        if( mSnapShots[i]->Tick() == iTick ) // The one exactly equal
            return  mSnapShots[i];
    }

    return  nullptr;
}


int
cPaperLogic::GetSnapShotIndexByTick( quint64 iTick )
{
    for( int i = 0; i < mSnapShots.Count(); ++i )
    {
        if( mSnapShots[ i ]->mTick == iTick )
            return  i;
    }

    return  -1;
}


void
cPaperLogic::ApplySnapShot( cSnapShot * iSnap )
{
    auto& diffMap = iSnap->DiffMap();
    auto& diffUsers = iSnap->DiffUsers();

    for( auto& user : diffUsers )
    {
        if( mAllUsers[ user.mIndex ] )
            mAllUsers[ user.mIndex ]->copyFromUser( &user );
    }

    for( auto& cell : diffMap )
    {
        _CallCB( cell.first.x(), cell.first.y(), -2, kPlayer );
        CELLAT( cell.first ) = cell.second;
        _CallCB( cell.first.x(), cell.first.y(), cell.second.mPlayer, kPlayer );
    }
}


void
cPaperLogic::AddSnapShot( cSnapShot * iSnap )
{
    mSnapShots.Write( iSnap );

}


void
cPaperLogic::ApplySnapShotHistoryUpToTick( quint64 iTick, eRollBackType iRollbackType )
{
    int indexCurrentSnap = GetSnapShotIndexByTick( mTick );
    int indexAimedSnap = GetSnapShotIndexByTick( iTick );

    if( indexCurrentSnap < 0 || indexAimedSnap < 0 )
        return;

    // Then, go from current snap, to iTick snap
    if( indexCurrentSnap == indexAimedSnap )
    {
        return;
    }
    else if( indexCurrentSnap > indexAimedSnap )
    {
        for( int i = indexCurrentSnap; i >= indexAimedSnap; --i )
        {
             if( iRollbackType == kSetTickToSnap )
                 mTick = mSnapShots[ i ]->Tick();

             ApplySnapShot( mSnapShots[ i ] );
        }
    }
    else
    {
        for( int i = indexCurrentSnap; i <= indexAimedSnap; ++i )
        {
            if( iRollbackType == kSetTickToSnap )
                mTick = mSnapShots[ i ]->Tick();

            ApplySnapShot( mSnapShots[ i ] );
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
cPaperLogic::_AddTrailAtIndex( const QPoint& iPoint, cUser*  iUser )
{
    SetTrailValueAt( iPoint, iUser->mIndex );

    if( !iUser->mTrailPoints.contains( iPoint ) )
        iUser->mTrailPoints.append( iPoint );
}


bool
cPaperLogic::_IsAvailableSpaceAtPoint( const QPoint& iPoint ) const
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


QDebug&
operator<<( QDebug & oStream, const cPaperLogic::eDataCell & iCell )
{
    oStream << "Cell : (" << iCell.mGround << "," << iCell.mTrail << "," << iCell.mPlayer << "):";
    return  oStream;
}



QDataStream&
operator<<(QDataStream& oStream, const cPaperLogic& iPaperLogic )
{
    oStream << compute_hash( "cPaperGrid" );
    oStream << iPaperLogic.mPaperGrid
            << iPaperLogic.mAllUsers
            << iPaperLogic.mTick;

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
            >> oPaperLogic.mAllUsers
            >> oPaperLogic.mTick;


    return  iStream;
}



// ===========================
// ===========================
// ===========================
// ===========================



cSnapShot::~cSnapShot()
{
}


cSnapShot::cSnapShot( quint64 iTick ) :
    mTick( iTick)
{
}


void
cSnapShot::AddCellDiff( const QPoint & iPoint, cPaperLogic::eDataCell iData )
{
    //qDebug() << __FUNCTION__ << " at : " << iPoint << " :: " << iData;
    // If the cell is already there, we override the data, because we only want the last change to be in the SS
    // So that we have the final state in the SS
    for( auto& pair : mDiffMap )
    {
        if( pair.first == iPoint )
        {
            pair.second = iData;
            //qDebug() << "OVERRIDE";
            return;
        }
    }

    QPair< QPoint, cPaperLogic::eDataCell > newPair( iPoint, iData );
    mDiffMap.push_back( newPair );
}


void
cSnapShot::AddUserDiff( cUser * iUser )
{
    cUser userCopy( iUser->mIndex, Qt::transparent );
    userCopy.copyFromUser( iUser );

    mDiffUsers.push_back( userCopy );
}


QVector< QPair< QPoint, cPaperLogic::eDataCell > >&
cSnapShot::DiffMap()
{
    return  mDiffMap;
}


QVector< cUser >&
cSnapShot::DiffUsers()
{
    return  mDiffUsers;
}


quint64
cSnapShot::Tick() const
{
    return  mTick;
}


QDebug&
operator<<( QDebug & oStream, const cSnapShot & iSnapShot )
{
    oStream << "START cSnapShot : =================";
    for( auto& cell : iSnapShot.mDiffMap )
        oStream << cell;
    oStream << "END cSnapShot ==================";
    return  oStream;
}


QDataStream&
operator<<(QDataStream& oStream, const cSnapShot& iSnapShot )
{
    oStream << compute_hash( "cSnapShot" );

    oStream << iSnapShot.mTick
            << iSnapShot.mDiffMap
            << iSnapShot.mDiffUsers;

    return  oStream;
}


QDataStream&
operator>>(QDataStream& iStream, cSnapShot& oSnapShot )
{
    uint32_t  id;
    iStream >> id;
    if( id != compute_hash( "cSnapShot" ) )
    {
        //qDebug() << "Invalid cPaperLogic object!";
        return  iStream;
    }

    iStream >> oSnapShot.mTick
            >> oSnapShot.mDiffMap
            >> oSnapShot.mDiffUsers;


    return  iStream;
}

