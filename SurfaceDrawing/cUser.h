#pragma once


#include <QColor>
#include <QPoint>
#include <QSize>


class cUser
{
public:
    ~cUser();
    cUser( const QColor& iColor );

public:
    void setPosition( QPoint iPosition );
    void setGUIPosition( QPoint iPosition );
    void setSize( QSize iSize );
    void setMovementVector( QPoint iMovementVector );

public:
    QColor    mColor;
    QPoint    mPosition;
    QSize     mSize;


    QPoint    mGUIPosition;
    QSize     mGUISize;
    QPoint    mGUIMovementVector;

    QPoint    mGUICurrentMovementVector;

    bool      mAskDirectionChange;
};




