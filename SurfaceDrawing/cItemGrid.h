#pragma once

#include <QGraphicsItem>

class cItemGrid : public QGraphicsItem
{
public:
    cItemGrid(QGraphicsItem *parent);
    ~cItemGrid();

public:
    QRectF boundingRect() const override;

    void   paint( QPainter* iPainter, const QStyleOptionGraphicsItem*  iOptions, QWidget* iWidget = 0 )  override;
};
