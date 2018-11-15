#pragma once

#include <QGraphicsItem>

class cUICompBar :
    public QGraphicsItem
{
public:
    ~cUICompBar();
    cUICompBar( QGraphicsItem* iParent = 0);

public:
    QRectF boundingRect() const override;
    void   paint( QPainter* iPainter, const QStyleOptionGraphicsItem*  iOptions, QWidget* iWidget = 0 )  override;

public:
    QPixmap*    mPixmap  = 0;
};
