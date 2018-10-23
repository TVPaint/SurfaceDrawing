#pragma once

#include <QGraphicsItem>

class cBasicTile :
    public QGraphicsItem
{
public:
    ~cBasicTile();
    cBasicTile( QGraphicsItem* iParent=0);

public:
    QRectF boundingRect() const override;
    void   paint( QPainter* iPainter, const QStyleOptionGraphicsItem*  iOptions, QWidget* iWidget = 0 )  override;

public:
    QColor  mColor;
};
