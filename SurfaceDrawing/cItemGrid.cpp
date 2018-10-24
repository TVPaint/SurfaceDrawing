#include "cItemGrid.h"

#include "cPaperLogic.h"


#include <QBrush>
#include <QPainter>
#include <QPen>

cItemGrid::cItemGrid( QGraphicsItem *parent ) :
    QGraphicsItem( parent ),
    mSize( 1, 1 )
{
}

cItemGrid::~cItemGrid()
{
}


QRectF
cItemGrid::boundingRect() const
{
    return   QRectF( QPoint( 0, 0 ), mSize );
}


void
cItemGrid::paint( QPainter*  iPainter, const QStyleOptionGraphicsItem*  iOptions, QWidget*  iWidget )
{
    iPainter->setBrush( QBrush( Qt::black ) );
    iPainter->setPen( QPen(  Qt::black ) );
    QRectF bbox = boundingRect();

    for( int x = 0; x < mSize.width(); x += CELLSIZE )
    {
        iPainter->drawLine( x, 0, x, mSize.height() );
    }
    for( int y = 0; y < mSize.height(); y += CELLSIZE )
    {
        iPainter->drawLine( 0, y, mSize.width(), y );
    }
}
