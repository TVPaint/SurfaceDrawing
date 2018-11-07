#pragma once


#include <QColor>
#include <QDebug>
#include <QPoint>
#include <QSize>
#include <ostream>
#include <QFile>

#include "cHash.h"

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
    void Update( int iTickCount );
    void setSize( QPoint iSize );
    void setMovementVector( QPoint iMovementVector );

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

    QList< QPoint >  mTrailPoints;


    QFile*          mDEBUGFile;
    QTextStream*    mDEBUGStream;
};

std::ostream& operator<<( std::ostream& oStream, const cUser& iUser );
QDebug& operator<<( QDebug& oStream, const cUser& iUser );


QDataStream& operator<<(QDataStream& oStream, const cUser& iUser );
QDataStream& operator>>(QDataStream& iStream, cUser& oUser );

QDataStream& operator<<(QDataStream& oStream, const cUser* iUser );
QDataStream& operator>>(QDataStream& iStream, cUser*& oUser );

