#pragma once

#include <QGraphicsItem>

class  cCustomGraphicsView;

class cGridItem :
    public QGraphicsItem
{
public:
    cGridItem( QGraphicsItem *parent = Q_NULLPTR );

public:
    QRectF  boundingRect() const override;
    void    paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) override;

public:
    float  psize;
    QSizeF size;
};

