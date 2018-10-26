#pragma once


#include <QColor>
#include <QDebug>
#include <QPoint>
#include <QSize>
#include <ostream>

#include "cHash.h"

class cUser
{
public:
    ~cUser();
    cUser( int iIndex );

public:
    void setPosition( QPoint iPosition );
    void Update();
    void setSize( QPoint iSize );
    void setMovementVector( QPoint iMovementVector );

public:
    int         mIndex;

    QColor      mColor;
    QPoint      mPosition;

    QPoint      mGUIPosition;
    QPoint      mGUISize;
    QPoint      mGUIMovementVector;

    QPoint      mGUICenter;

    QPoint      mGUICurrentMovementVector;

    bool        mAskDirectionChange;
    bool        mIsOutOfGround;
    bool        mIsDead;

    QList< QPoint >  mTrailPoints;
};

std::ostream& operator<<( std::ostream& oStream, const cUser& iUser );
QDebug& operator<<( QDebug& oStream, const cUser& iUser );


QDataStream& operator<<(QDataStream& oStream, const cUser& iUser );
QDataStream& operator>>(QDataStream& iStream, cUser& oUser );

QDataStream& operator<<(QDataStream& oStream, const cUser* iUser );
QDataStream& operator>>(QDataStream& iStream, cUser*& oUser );

