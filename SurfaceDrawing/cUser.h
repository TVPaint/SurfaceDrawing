#pragma once


#include <QColor>
#include <QPoint>
#include <QSize>


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




