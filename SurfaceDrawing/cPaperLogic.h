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
    enum  ePaperEnum
    {
        kEmpty = 100,
        kWall,
        // Then its user based let's say :
        // 1-9 == user from 1 to 9
        // 11-19 is user's trace
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

    void  AddGridChangedCB( std::function< void( int, int, int ) > iCB );

    void  ChangeGridValueAt( int x, int y, int value );

    void  FillZone( int iIndex );




private:
    bool  SanityChecks() const;
    void  _CallCB( int, int, int );


public:
    QVector< QVector< int > >   mPaperGrid;
    QVector< cUser* >           mAllUsers;

    QList< std::function< void( int, int, int ) > > mCBList;
};

