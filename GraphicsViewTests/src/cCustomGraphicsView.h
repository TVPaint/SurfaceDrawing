#pragma once

#include <QGraphicsView>

class cGraphicItem;
class cCurrentFrameItem;
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
    void  _UpdateItemsPosition();
    void  _UpdateCurrentFrameItemPosition();
    void  _SortItems();

private:
    cAddItem*                   mAddItem;
    cCurrentFrameItem*          mCurrentFrameItem;
    QVector< cGraphicItem* >    mAnimationImages;
    int                         mCurrentFrame;

};

