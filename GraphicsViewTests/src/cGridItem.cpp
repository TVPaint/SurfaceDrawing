#include "cGridItem.h"

#include "cCustomGraphicsView.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>


cGridItem::cGridItem( QGraphicsItem *parent ) :
    QGraphicsItem( parent )
{
}


QRectF
cGridItem::boundingRect() const
{

    return  QRectF( 0, 0, size.width(), size.height() );
}


void
cGridItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    if( psize > 10 )
    {
        painter->setBrush( Qt::white );

        int numbOfLines = size.width() / psize;
        for( int i = 0; i < numbOfLines; ++i )
        {
            int position = std::round( i*psize );
            painter->drawLine( position, 0, position, size.height() );
        }

        int numbOfColu = size.height() / psize;
        for( int i = 0; i < numbOfColu; ++i )
        {
            int position = std::round( i*psize );
            painter->drawLine( 0, position, size.width(), position );
        }
    }
}

