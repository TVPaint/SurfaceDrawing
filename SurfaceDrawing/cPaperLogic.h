#pragma once


#include <QVector>
#include <QColor>

#include <functional>

class cUser;

#define GRIDSIZE 100
#define CELLSIZE 16

class cPaperLogic
{
public:
    ~cPaperLogic();
    cPaperLogic();

public:
    struct  eDataCell
    {
        eDataCell() : mPlayer( 0 ), mTrail(0), mGround(0){};
        char  mPlayer;
        char  mTrail;
        char  mGround;
    };

    enum eDataType
    {
        kPlayer,
        kTrail,
        kGround
    };


public:
    void Init();


public:
    static  QPoint  MapToGrid( const QPoint& iPoint );
    static  QPoint  MapFromGrid( const QPoint& iPoint );
    static  QColor  GetColorByIndex( int iIndex );

public:
    void   AddUser( cUser* iUser );


public:
    void Update();

    void  AddGridChangedCB( std::function< void( int, int, int, eDataType ) > iCB );

    void  SetPlayerValueAt( int x, int y, int value );
    void  SetTrailValueAt( int x, int y, int value );
    void  SetGroundValueAt( int x, int y, int value );

    void  FillZone( int iIndex );




private:
    bool  SanityChecks() const;
    void  _CallCB( int, int, int, eDataType );
    void  _AddTrailAtIndex( int iX, int iY, int iIndex );


public:
    QVector< QVector< eDataCell > >     mPaperGrid;
    QVector< cUser* >                   mAllUsers;
    QList< QPoint >                     mTrailPoints;

    QList< std::function< void( int, int, int, eDataType ) > > mCBList;
};

