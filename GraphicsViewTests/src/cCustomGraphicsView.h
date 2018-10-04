#pragma once

#include <QGraphicsView>

class cGraphicItem;
class cAddItem;

class cCustomGraphicsView :
    public QGraphicsView
{
    Q_OBJECT

public:
    cCustomGraphicsView( QWidget *parent = nullptr );

public:
    void  AddItem();
    void  itemMoving( cGraphicItem* iItem, const QPointF& iNewPosition );
    void  itemMoved();

    QVector< cGraphicItem* >& GetAnimationImages();

public:
    void  CurrentFrameChanged( int iCurrent );

private:
    void  _UpdateItemsPosition( QVector< cGraphicItem* >& iSortedItems );
    void  _SortItems( QVector< cGraphicItem* >* oSortedItems );

private:
    cAddItem*  mAddItem;
    QVector< cGraphicItem* > mAnimationImages;

};

