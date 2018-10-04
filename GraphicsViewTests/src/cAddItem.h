#pragma once

#include <QGraphicsItem>

class  cCustomGraphicsView;

class cAddItem :
    public QGraphicsItem
{
public:
    cAddItem( cCustomGraphicsView* iParentView, QGraphicsItem *parent = Q_NULLPTR );

public:
    QRectF  boundingRect() const override;
    void    paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) override;

protected:
    void        mouseReleaseEvent( QGraphicsSceneMouseEvent* iEvent ) override;

private:
    QString                 mText;
    QColor                  mColor;
    cCustomGraphicsView*    mParentView;
};

