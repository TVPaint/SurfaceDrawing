#pragma once

#include <QGraphicsView>

class cGraphicItem;

class cCustomGraphicsView :
    public QGraphicsView
{
    Q_OBJECT

public:
    cCustomGraphicsView( QWidget *parent = nullptr );

public:
    void  itemMoving( cGraphicItem* iItem, const QPointF& iNewPosition );
    void  itemMoved();

private:
    void  _UpdateItemsPosition( QVector< QGraphicsItem* >& iSortedItems );
    void  _SortItems( QVector< QGraphicsItem* >* oSortedItems );

};

