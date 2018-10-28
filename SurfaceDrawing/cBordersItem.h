#pragma once

#include <QGraphicsItem>

class cBordersItem :
    public QGraphicsItem
{
public:
    ~cBordersItem();
    cBordersItem( QGraphicsItem * parent = 0 );

public:
    QRectF  boundingRect() const override;
    void  paint( QPainter* iPainter, const QStyleOptionGraphicsItem*  iOptions, QWidget* iWidget = 0 )  override;
};
