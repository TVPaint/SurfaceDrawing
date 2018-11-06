#pragma once


#include <QVector>
#include <QMap>
#include <QPoint>
#include <QColor>

#include <functional>
#include <ostream>

class cUser;

#define GRIDSIZE 75
#define CELLSIZE 16
#define SPEED 20 // ms for 1 pixel
#define CLOCKTIME 108000000 // 30 min

class cPaperLogic
{
public:
    ~cPaperLogic();
    cPaperLogic();

public:
    struct  eDataCell
    {
        eDataCell() : mPlayer( -1 ), mTrail( -1 ), mGround( -1 ), mSpawnIsImpossibleHere( false ){};

        bool  operator==( const eDataCell& iRHS )
        {
            return  mPlayer == iRHS.mPlayer && mTrail == iRHS.mTrail && mGround == iRHS.mGround;
        }

        int8_t  mPlayer;
        int8_t  mTrail;
        int8_t  mGround;

        bool  mSpawnIsImpossibleHere; // To check if spawn is available, just to avoid having a whole different array
    };

    enum eDataType
    {
        kPlayer,
        kTrail,
        kGround
    };


public:
    void  CopyFromPaper( const cPaperLogic& iPaper, quint16 iMissingUpdates );


public:
    void Init();


public:
    static  QPoint  MapToGrid( const QPoint& iPoint );
    static  QPoint  MapFromGrid( const QPoint& iPoint );
    QColor  GetColorByIndex( int iIndex );

public:
    void   AddUser( cUser* iUser );
    void   RemoveUser( cUser* iUser );


public:
    void Update( quint64 iCurrentTimeRemaining );
    void GoToTick( quint64 iTick );

    void  AddGridChangedCB( std::function< void( int, int, int, eDataType ) > iCB );

    void  SetPlayerValueAt( const QPoint& iPoint, int value );
    void  SetTrailValueAt( const QPoint& iPoint, int value );
    void  SetGroundValueAt( const QPoint& iPoint, int value );

    void  FillZone( cUser*  iUser );


    void  KillUser( cUser*  iUser );

    void  TryRespawningPlayer( cUser*  iUser );
    void  SpawnUserAtPoint( cUser*  iUser, const QPoint& iPoint );

private:
    bool  SanityChecks() const;
    void  _CallCB( int, int, int, eDataType );
    void  _AddTrailAtIndex( const QPoint& iPoint, cUser*  iUser );

    bool  _IsAvailableSpaceAtPoint( const QPoint& iPoint ) const;


public:
    QVector< QVector< eDataCell > >     mPaperGrid;
    QMap< int, cUser* >                 mAllUsers;

    QList< std::function< void( int, int, int, eDataType ) > > mCBList;

    quint64                             mPreviousTime = 0;
    quint64                             mTimeBuffer = 0;

    quint64                             mTick;
};



std::ostream& operator<<( std::ostream& oStream, const cPaperLogic& iPaperLogic );
QDebug& operator<<( QDebug& oStream, const cPaperLogic& iPaperLogic );

QDataStream& operator<<(QDataStream& oStream, const cPaperLogic::eDataCell& iDataCell );
QDataStream& operator>>(QDataStream& iStream, cPaperLogic::eDataCell& oDataCell );

QDataStream& operator<<(QDataStream& oStream, const cPaperLogic& iPaperLogic );
QDataStream& operator>>(QDataStream& iStream, cPaperLogic& oPaperLogic );


