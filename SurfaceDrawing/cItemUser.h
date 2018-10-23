#pragma once

#include "cUser.h"


#include <QGraphicsItem>

class cItemUser :
    public QGraphicsItem
{
public:
    ~cItemUser();
    cItemUser( cUser* iUser, QGraphicsItem * parent = 0 );

public:
    QRectF  boundingRect() const override;
    void  paint( QPainter* iPainter, const QStyleOptionGraphicsItem*  iOptions, QWidget* iWidget = 0 )  override;


public:
    cUser*  mUser;
};
