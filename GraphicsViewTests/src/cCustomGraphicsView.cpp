#include "cCustomGraphicsView.h"

#include "cItem.h"
#include "cAddItem.h"


#define YPOS 0

cCustomGraphicsView::cCustomGraphicsView( QWidget *parent )
    : QGraphicsView( parent )
{
    QGraphicsScene* scene = new QGraphicsScene( this );
    setScene( scene );
    setAlignment( Qt::AlignVCenter | Qt::AlignLeft );

    auto imgA = new  cGraphicItem( this );
    imgA->setPos( 0, YPOS );
    imgA->setZValue( -1 );
    imgA->setFile( "Resources/ImgA.png" );
    scene->addItem( imgA );

    int i = 1;
    auto imgB = new  cGraphicItem( this );
    imgB->setPos(  i*5 + i*32, YPOS );
    imgB->setZValue( -1 );
    imgB->setFile( "Resources/ImgB.png" );
    scene->addItem( imgB );

    ++i;
    auto imgC = new  cGraphicItem( this );
    imgC->setPos(  i*5 + i*32, YPOS );
    imgC->setZValue( -1 );
    imgC->setFile( "Resources/ImgC.png" );
    scene->addItem( imgC );

    ++i;
    auto imgD = new  cGraphicItem( this );
    imgD->setPos(  i*5 + i*32, YPOS );
    imgD->setZValue( -1 );
    imgD->setFile( "Resources/ImgD.png" );
    scene->addItem( imgD );

    ++i;
    mAddItem = new  cAddItem( this );
    mAddItem->setPos(  i*5 + i*32, YPOS );
    mAddItem->setZValue( -1 );
    scene->addItem( mAddItem );
}


void
cCustomGraphicsView::AddItem()
{
    int index = scene()->items().size() - 1; // Cuz addbutton is within
    mAddItem->setPos( (index+1)*5 + (index+1)*32, YPOS );

    auto item = new  cGraphicItem( this );
    item->setPos( index*5 + index*32, YPOS );
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
        iSortedItems[ i ]->setPos( i*5 + i*32, YPOS );
    }

    mAddItem->setPos( i*5 + i*32, YPOS );
}