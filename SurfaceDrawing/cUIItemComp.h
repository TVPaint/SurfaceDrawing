#pragma once

#include <QGraphicsItem>

#include "cUser.h"

class cUIItemComp :
    public QGraphicsItem
{
public:
    ~cUIItemComp();
    cUIItemComp( cUser* iUser, int iCompIndex, QGraphicsItem* iParent = 0);

public:
    QRectF boundingRect() const override;
    void   paint( QPainter* iPainter, const QStyleOptionGraphicsItem*  iOptions, QWidget* iWidget = 0 )  override;

public:
    void  Update();

public:
    cUser*  mUser;
    int     mCompIndex;

    bool    mActive;
    bool    mCoolingdown;
};
