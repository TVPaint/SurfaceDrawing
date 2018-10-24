#pragma once

#include <QGraphicsItem>

class cBasicTile :
    public QGraphicsItem
{
public:
    ~cBasicTile();
    cBasicTile( QGraphicsItem* iParent=0);

public:
    enum eDirection
    {
        kNorth,
        kSouth,
        kEast,
        kWest
    };

public:
    QRectF boundingRect() const override;
    void   paint( QPainter* iPainter, const QStyleOptionGraphicsItem*  iOptions, QWidget* iWidget = 0 )  override;

public:
    QColor      mColor;
    bool        mHalf;
    eDirection  mDirection;
};