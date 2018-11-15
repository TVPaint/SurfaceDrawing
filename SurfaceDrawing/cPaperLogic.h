#pragma once


#include <QVector>
#include <QMap>
#include <QHash>
#include <QPoint>
#include <QColor>

#include <functional>
#include <ostream>


#include "CircularBuffer.h"
#include "Defines.h"

class cUser;
class cSnapShot;

class cPaperLogic
{
public:
    ~cPaperLogic();
    cPaperLogic();

public:
    struct  eDataCell
    {
        eDataCell() : mPlayer( -1 ), mTrail( -1 ), mGround( -1 ) {};

        bool  operator==( const eDataCell& iRHS )
        {
            return  mPlayer == iRHS.mPlayer && mTrail == iRHS.mTrail && mGround == iRHS.mGround;
        }

        eDataCell&  operator=( const eDataCell& iRHS )
        {
            mPlayer = iRHS.mPlayer;
            mTrail = iRHS.mTrail;
            mGround = iRHS.mGround;

            return  *this;
        }

        bool  empty() const
        {
            return  mPlayer == -1 && mTrail == -1 && mGround == -1;
        }

        int8_t  mPlayer;
        int8_t  mTrail;
        int8_t  mGround;
    };

    enum eDataType
    {
        kPlayer,
        kTrail,
        kGround
    };

    enum eRollBackType
    {
        kSetTickToSnap,
        kKeepOwnTick
    };


public:
    void  CopyFromPaper( const cPaperLogic& iPaper, quint16 iMissingUpdates, eRollBackType iType );


public:
    void Init();


public:
    static  QPoint  MapToGrid( const QPoint& iPoint );
    static  QPoint  MapFromGrid( const QPoint& iPoint );
    QColor  GetColorByIndex( int iIndex );

public:
    void   AddUser( cUser* iUser );
    void   RemoveUser( cUser* iUser );
    void   SetUserAskedRespawn( cUser* iUser );

public:
    void Update( quint64 iCurrentTimeRemaining );
    void TickUpdate( quint64 iCurrentTimeRemaining );
    void GoToTick( quint64 iTick );
    void ApplyDeltaTick( int iTick );

    void  AddGridChangedCB( std::function< void( int, int, int, eDataType ) > iCB );

    void  SetPlayerValueAt( const QPoint& iPoint, int value );
    void  SetTrailValueAt( const QPoint& iPoint, int value );
    void  SetGroundValueAt( const QPoint& iPoint, int value );

    void  FillZone( cUser*  iUser );


    void  KillUser( cUser*  iUser );

    void  SpawnUserAtPoint( cUser*  iUser, const QPoint& iPoint );

    float  ComputeLandUsage( const cUser*  iUser );


public:
    cSnapShot*  FindSnapShotByTick( quint64 iTick );
    int         FindSnapshotIndexByTick( quint64 iTick );
    int         GetSnapShotIndexByTick( quint64 iTick );
    void        ApplySnapShot( cSnapShot* iSnap );
    void        AddSnapShot( cSnapShot* iSnap );
    void        ApplySnapShotHistoryUpToTick( quint64 iTick, eRollBackType iRollbackType );

private:
    bool  SanityChecks() const;
    void  _TryRespawningPlayer( cUser*  iUser );
    void  _CallCB( int, int, int, eDataType );
    void  _AddTrailAtIndex( const QPoint& iPoint, cUser*  iUser );

    void  _SetCellData( const QPoint& iPoint, const eDataCell& iCellData );


    void  _RunStandardUpdateForUser( cUser* iUser, int iDTick );
    bool  _RunRollbackForUser( cUser* iUser, int iDTick ); // Returns true if it handled the user, false otherwise, meaning main loop has to handle still the user
    bool  _RunSpeedForUser( cUser* iUser, int iDTick ); // Returns true if it handled the user, false otherwise, meaning main loop has to handle still the user


public:
    QVector< QVector< eDataCell > >     mPaperGrid;
    cCircularBufferP< cSnapShot >       mSnapShots;
    QMap< int, cUser* >                 mAllUsers;

    QList< std::function< void( int, int, int, eDataType ) > > mCBList;

    quint64                             mPreviousTime = 0;
    quint64                             mTimeBuffer = 0;

    quint64                             mTick;
};



std::ostream& operator<<( std::ostream& oStream, const cPaperLogic& iPaperLogic );
QDebug& operator<<( QDebug& oStream, const cPaperLogic& iPaperLogic );
QDebug& operator<<( QDebug& oStream, const cPaperLogic::eDataCell& iCell );

QDataStream& operator<<(QDataStream& oStream, const cPaperLogic::eDataCell& iDataCell );
QDataStream& operator>>(QDataStream& iStream, cPaperLogic::eDataCell& oDataCell );

QDataStream& operator<<(QDataStream& oStream, const cPaperLogic& iPaperLogic );
QDataStream& operator>>(QDataStream& iStream, cPaperLogic& oPaperLogic );






class cSnapShot
{
public:
    ~cSnapShot();
    cSnapShot( quint64 iTick );


public:
    void  AddCellDiff( const QPoint& iPoint, cPaperLogic::eDataCell iData );
    void  AddUserDiff( cUser* iUser);
    QVector< QPair< QPoint, cPaperLogic::eDataCell > >&     DiffMap();
    QVector< cUser >&                                       DiffUsers();
    quint64                                                 Tick() const;

public:
    quint64                                                 mTick;
    QVector< QPair< QPoint, cPaperLogic::eDataCell > >      mDiffMap;
    QVector< cUser >                                        mDiffUsers;
};


QDebug& operator<<( QDebug& oStream, const cSnapShot& iSnapShot );

QDataStream& operator<<(QDataStream& oStream, const cSnapShot& iSnapShot );
QDataStream& operator>>(QDataStream& iStream, cSnapShot& oSnapShot );



