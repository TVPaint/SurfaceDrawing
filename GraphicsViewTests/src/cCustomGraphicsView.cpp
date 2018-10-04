#include "cCustomGraphicsView.h"

#include "cItem.h"
#include "cAddItem.h"


cCustomGraphicsView::cCustomGraphicsView( QWidget *parent )
    : QGraphicsView( parent )
{
    QGraphicsScene* scene = new QGraphicsScene( this );
    setScene( scene );
    setAlignment( Qt::AlignVCenter | Qt::AlignLeft );

    int i = 0;
    for( i = 0; i < 5; ++i )
    {
        auto item = new  cGraphicItem( this );
        item->setPos( i*5 + i*32, 50 );
        item->setIndex( i );
        item->setZValue( -1 );
        scene->addItem( item );
    }

    mAddItem = new  cAddItem( this );
    mAddItem->setPos( i*5 + i*32, 50 );
    mAddItem->setZValue( -1 );
    scene->addItem( mAddItem );
}


void
cCustomGraphicsView::AddItem()
{
    int index = scene()->items().size() - 1; // Cuz addbutton is within
    mAddItem->setPos( (index+1)*5 + (index+1)*32, 50 );

    auto item = new  cGraphicItem( this );
    item->setPos( index*5 + index*32, 50 );
    item->setIndex( index );
    item->setZValue( -1 );
    scene()->addItem( item );
}


void
cCustomGraphicsView::itemMoving( cGraphicItem* iItem, const QPointF& iNewPosition )
{
}


void
cCustomGraphicsView::itemMoved()
{
    QVector< QGraphicsItem* > sortedItems;
    _SortItems( &sortedItems );
    _UpdateItemsPosition( sortedItems );
}


void cCustomGraphicsView::_SortItems( QVector<QGraphicsItem*>* oSortedItems )
{
    oSortedItems->clear();
    for( auto item : scene()->items() )
    {
        if( item == mAddItem )
            continue;

        QPointF position = item->pos();

        int i;
        for( i = 0; i < oSortedItems->size(); ++i )
        {
            if( oSortedItems->at( i )->pos().x() > position.x() )
                break;
        }

        oSortedItems->insert( i, item );
    }
}


void
cCustomGraphicsView::_UpdateItemsPosition( QVector< QGraphicsItem* >& iSortedItems )
{
    int i = 0;
    for( i = 0; i < iSortedItems.size(); ++i )
    {
        iSortedItems[ i ]->setPos( i*5 + i*32, 50 );
    }

    mAddItem->setPos( i*5 + i*32, 50 );
}
