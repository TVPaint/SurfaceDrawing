#include "cCustomGraphicsView.h"

#include "cItem.h"


cCustomGraphicsView::cCustomGraphicsView( QWidget *parent )
    : QGraphicsView( parent )
{
    QGraphicsScene* scene = new QGraphicsScene( this );
    setScene( scene );
    setAlignment( Qt::AlignVCenter | Qt::AlignLeft );

    for( int i = 0; i < 5; ++i )
    {
        auto item = new  cGraphicItem( this );
        item->setPos( i*5 + i*32, 50 );
        item->setIndex( i );
        item->setZValue( -1 );
        scene->addItem( item );
    }
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
    for( int i = 0; i < iSortedItems.size(); ++i )
    {
        iSortedItems[ i ]->setPos( i*5 + i*32, 50 );
    }
}
