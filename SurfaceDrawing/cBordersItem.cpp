#include "cBordersItem.h"


#include "cPaperLogic.h"

#include <QPainter>


cBordersItem::~cBordersItem()
{
}


cBordersItem::cBordersItem( QGraphicsItem * parent ) :
    QGraphicsItem( parent )
{
}


QRectF
cBordersItem::boundingRect() const
{
    int size = GRIDSIZE * CELLSIZE;
    return   QRectF( QPoint( 0, 0 ), QSize( size, size ) );
}


void
cBordersItem::paint( QPainter * iPainter, const QStyleOptionGraphicsItem * iOptions, QWidget * iWidget )
{
    iPainter->setPen( QPen( Qt::black ) );
    iPainter->drawRect( boundingRect() );
}

