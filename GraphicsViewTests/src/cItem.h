#pragma once

#include <QGraphicsItem>

class  cCustomGraphicsView;

class cGraphicItem :
    public QGraphicsItem
{
public:
    cGraphicItem( cCustomGraphicsView* iParentView, QGraphicsItem *parent = Q_NULLPTR );

public:
    void setIndex( int iIndex );

public:
    QRectF  boundingRect() const override;
    void    paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) override;

protected:
    void        mousePressEvent( QGraphicsSceneMouseEvent* iEvent ) override;
    void        mouseMoveEvent( QGraphicsSceneMouseEvent* iEvent ) override;
    void        mouseReleaseEvent( QGraphicsSceneMouseEvent* iEvent ) override;

    QVariant    itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    QString                 mText;
    int                     mIndex;
    QColor                  mColor;
    cCustomGraphicsView*    mParentView;
};

