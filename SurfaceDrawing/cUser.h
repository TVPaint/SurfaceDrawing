#pragma once


#include <QColor>
#include <QDebug>
#include <QPoint>
#include <QSize>
#include <ostream>
#include <QFile>

#include "cHash.h"

#include "cComp.h"

class cUser
{
public:
    ~cUser();
    cUser();
    cUser( int iIndex, const QColor& iColor );

public:
    void copyFromUser( const cUser* iUser );

public:
    void setPosition( QPoint iPosition );
    void setGUIPosition( QPoint iGUIPosition );
    void Update( int iTickCount );
    void UpdateComps( int iTickCount );
    void setSize( QPoint iSize );
    void setMovementVector( QPoint iMovementVector );

    void activateComp( int iCompNumber );
    void deactivateComp( int iCompNumber );

public:
    int         mIndex;

    QColor      mColor;
    QPoint      mPosition;

    QPoint      mGUIPosition;
    QPoint      mGUISize;
    QPoint      mGUIMovementVector;

    QPoint      mGUICurrentMovementVector;

    bool        mAskDirectionChange;
    bool        mIsOutOfGround;
    bool        mIsDead;
    bool        mAskedRespawn = false;

    int         mSpeedMultiplicator = 1;

    QList< QPoint >  mTrailPoints;

    QVector< cComp > mComps;
};

std::ostream& operator<<( std::ostream& oStream, const cUser& iUser );
QDebug& operator<<( QDebug& oStream, const cUser& iUser );


QDataStream& operator<<(QDataStream& oStream, const cUser& iUser );
QDataStream& operator>>(QDataStream& iStream, cUser& oUser );

QDataStream& operator<<(QDataStream& oStream, const cUser* iUser );
QDataStream& operator>>(QDataStream& iStream, cUser*& oUser );

