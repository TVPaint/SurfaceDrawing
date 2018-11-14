#pragma once

#include <QGraphicsItem>

class cItemPlayfield :
    public QGraphicsItem
{
public:
    ~cItemPlayfield();
    cItemPlayfield( QGraphicsItem* iParent );

public:
    QRectF boundingRect() const override;
    void   paint( QPainter* iPainter, const QStyleOptionGraphicsItem*  iOptions, QWidget* iWidget = 0 )  override;
};

