#include "cPaperLogic.h"


#include "cUser.h"

#include <stack>
#include <random>

#include <QDebug>
#include <QDataStream>

#define  CELLAT( point ) mPaperGrid[ point.x()][point.y()]


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

            _SetCellData( cellPoint, externCell );
            mSnapShots.Back()->AddCellDiff( cellPoint, CELLAT( cellPoint ) );
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
    _TryRespawningPlayer( iUser );
}


void
cPaperLogic::RemoveUser( cUser * iUser )
{
    KillUser( iUser );
    mAllUsers.erase( mAllUsers.find( mAllUsers.key( iUser ) ) );
}


void
cPaperLogic::SetUserAskedRespawn( cUser * iUser )
{
    iUser->mAskedRespawn = true;
    mSnapShots.Back()->AddUserDiff( iUser ); // We might not care about this bool
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

    int tickCount   = mTimeBuffer / SPEED; // Because we want 1 pixel per SPEEDms
    mTimeBuffer     = mTimeBuffer % SPEED; // This is remaining ms that doesn't make a full tick, so we add them next round

    mTick += tickCount;
}


void
cPaperLogic::GoToTick( quint64 iTick )
{
    int deltaTick = int(iTick - mTick);

    if( deltaTick < 0 ) // Go back in time
        ApplySnapShotHistoryUpToTick( iTick, kSetTickToSnap );
    else
        ApplyDeltaTick( deltaTick );
}


void
cPaperLogic::ApplyDeltaTick( quint64 iDeltaTick )
{
    if( iDeltaTick == 0 )
    {
        //qDebug() << "*********************************Nothing";
        return;
    }
    else if( iDeltaTick > 0 ) // Advance in ticks
    {
        // This will override history if needed
        int indexSnap = FindSnapshotIndexByTick( mTick + iDeltaTick );
        auto newSnap = new cSnapShot( mTick + iDeltaTick );
        if( indexSnap == -1 )
            mSnapShots.Write( newSnap );
        else
            mSnapShots.WriteAt( newSnap, indexSnap );

        //qDebug() << "*********************************FWD : " << iDeltaTick;
        for( auto user : mAllUsers )
        {
            if( !user )
                continue;

            if( user->mIsDead )
            {
                if( user->mAskedRespawn )
                    _TryRespawningPlayer( user );

                continue;
            }


            _RunSpeedForUser( user, iDeltaTick );
            if( _RunRollbackForUser( user, iDeltaTick ) )
                continue;


            _RunStandardUpdateForUser( user, iDeltaTick );
        }
    }
    else if( iDeltaTick < 0 ) // Go back in time
    {
        // Dunno yet
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

    // add 2 row an 2 column to be sure to have an outside area
    const int MASKWIDTH = GRIDSIZE + 2;
    const int MASKHEIGHT = GRIDSIZE + 2;

    std::vector< bool >  mask( MASKWIDTH * MASKHEIGHT, false );

    #define POINT2MASK( point ) point.x() + 1 + (point.y() + 1) * MASKWIDTH
    #define IS_MASK_VALID( index ) index >= 0 && index < mask.size()

    for( int y = 0; y < GRIDSIZE; ++y )
    {
        for( int x = 0; x < GRIDSIZE; ++x )
        {
            QPoint point( x, y );
            mask[POINT2MASK( point )] = CELLAT( point ).mGround == iUser->mIndex;
        }
    }

    // we need to find the first point outside of player area : -1 -1 should be ok!
    std::stack< QPoint >  stack;
    QPoint empty(-1, -1);
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
    iUser->mAskedRespawn = false;
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


float
cPaperLogic::ComputeLandUsage( const cUser*  iUser )
{
    float count = 0;
    for( auto r: mPaperGrid )
    {
        for( auto c: r )
        {
            if( c.mGround == iUser->mIndex )
                ++count;
        }
    }
    return  count / ( GRIDSIZE * GRIDSIZE );
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
cPaperLogic::FindSnapshotIndexByTick( quint64 iTick )
{
    for( int i = 0; i < mSnapShots.Count(); ++i )
    {
        if( mSnapShots[i]->Tick() == iTick ) // The one exactly equal
            return  i;
    }

    return  -1;
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
        _SetCellData( cell.first, cell.second );
}


void
cPaperLogic::AddSnapShot( cSnapShot * iSnap )
{
    int indexSnap = FindSnapshotIndexByTick( iSnap->mTick );
    if( indexSnap == -1 )
        mSnapShots.Write( iSnap );
    else
        mSnapShots.WriteAt( iSnap, indexSnap );
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
cPaperLogic::_TryRespawningPlayer( cUser*  iUser )
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


void
cPaperLogic::_SetCellData( const QPoint & iPoint, const eDataCell & iCellData )
{
    _CallCB( iPoint.x(), iPoint.y(), -2, kPlayer ); // CB to erase old value
    CELLAT( iPoint ) = iCellData;
    _CallCB( iPoint.x(), iPoint.y(), iCellData.mPlayer, kPlayer ); // CB to set new value with player flag, as it's the only one that has a specific treatment
}


void
cPaperLogic::_RunStandardUpdateForUser( cUser* iUser, int iDTick )
{
    // USER
    QPoint oldPosition = iUser->mPosition;

    SetPlayerValueAt( oldPosition, -1 );

    // User movement
    iUser->Update( iDTick );
    mSnapShots.Back()->AddUserDiff( iUser );

    QPoint newPosition = iUser->mPosition;

    if( newPosition.x() < 0 || newPosition.x() >= GRIDSIZE
        || newPosition.y() < 0 || newPosition.y() >= GRIDSIZE )
    {
        KillUser( iUser );
        return;
    }

    SetPlayerValueAt( newPosition, iUser->mIndex );

    if( oldPosition != newPosition && CELLAT(oldPosition).mGround != iUser->mIndex )
    {
        _AddTrailAtIndex( oldPosition, iUser );
    }

    // TRAILS
    if( CELLAT(newPosition).mGround == iUser->mIndex && iUser->mIsOutOfGround == true ) // if we land back on our ground
    {
        iUser->mIsOutOfGround = false;
        FillZone( iUser );
    }
    else if( CELLAT(newPosition).mGround != iUser->mIndex && iUser->mIsOutOfGround == false ) // If we leave our land
    {
        iUser->mIsOutOfGround = true;
    }
    else if( CELLAT( newPosition).mTrail >= 0 ) // If user encounters a trail
    {
        KillUser( mAllUsers[ CELLAT(newPosition).mTrail ] );
    }
}


bool
cPaperLogic::_RunRollbackForUser( cUser* iUser, int iDTick )
{
    if( !iUser->mComps[ 0 ].mActive )
        return  false;

    if( !iUser->mIsOutOfGround || iUser->mIsDead )
        return  false;

    if( iUser->mTrailPoints.size() == 0 )
        return  false;


    iUser->mComps[ 0 ].mCooldown = iUser->mComps[ 0 ].mCooldownBase;

    auto trailCell = iUser->mTrailPoints.back();

    QPoint trailCellGUICenter( trailCell.x() * CELLSIZE + CELLSIZE/2, trailCell.y() * CELLSIZE + CELLSIZE/2 );
    QPoint userCellGUICenter = iUser->mGUIPosition + QPoint( CELLSIZE/2, CELLSIZE/2 );

    if( std::abs(trailCellGUICenter.x() - userCellGUICenter.x()) > 2
            || std::abs(trailCellGUICenter.y() - userCellGUICenter.y()) > 2 )
    {
        QPoint directionVector = trailCellGUICenter - userCellGUICenter;
        if( directionVector.x () > 0 )
            directionVector.setX( 1 );
        else if( directionVector.x () < 0 )
            directionVector.setX( -1 );

        if( directionVector.y () > 0 )
            directionVector.setY( 1 );
        else if( directionVector.y () < 0 )
            directionVector.setY( -1 );

        iUser->mGUICurrentMovementVector = -directionVector;

        SetPlayerValueAt( iUser->mPosition, -1 );
        iUser->setGUIPosition( iUser->mGUIPosition + directionVector * ROLLBACKSPEED * iDTick );
        SetPlayerValueAt( iUser->mPosition, iUser->mIndex );
    }
    else
    {
        SetPlayerValueAt( iUser->mPosition, -1 );
        iUser->mTrailPoints.pop_back();
        iUser->setPosition( trailCell ); // To snap properly on grid
        SetPlayerValueAt( iUser->mPosition, iUser->mIndex );
    }

    if( CELLAT( iUser->mPosition ).mTrail == iUser->mIndex )
        SetTrailValueAt( iUser->mPosition, -1 );

    iUser->UpdateComps( iDTick ); // Or not if we say rollback freezes other comps cooldown

    mSnapShots.Back()->AddUserDiff( iUser );

    return  true;
}


bool
cPaperLogic::_RunSpeedForUser( cUser * iUser, int iDTick )
{
    if( !iUser->mComps[ 1 ].mActive )
        return  false;

    if( iUser->mIsDead )
        return  false;

    if( iUser->mComps[ 1 ].mCompDuration <= 0 )
    {
        iUser->deactivateComp( 1 );
        iUser->mSpeedMultiplicator = 1;
        iUser->mComps[ 1 ].mCooldown = iUser->mComps[ 1 ].mCooldownBase;
        mSnapShots.Back()->AddUserDiff( iUser );
        return  true;
    }

    // If already set, don't set it again
    if( iUser->mSpeedMultiplicator == SPEEDBOOST )
        return  true;


    iUser->mSpeedMultiplicator = SPEEDBOOST;
    mSnapShots.Back()->AddUserDiff( iUser );

    return  true;
}


QDataStream&
operator<<(QDataStream& oStream, const cPaperLogic::eDataCell& iDataCell )
{
    oStream << compute_hash( "cPaperGrid::eDataCell" );
    oStream << iDataCell.mPlayer
            << iDataCell.mTrail
            << iDataCell.mGround;

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
    oStream << "Tick : " << iSnapShot.mTick;
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

