#include "cItemPlayfield.h"

#include "cPaperLogic.h"


#include <QBrush>
#include <QPainter>
#include <QPen>


cItemPlayfield::cItemPlayfield( QGraphicsItem *parent ) :
    QGraphicsItem( parent )
{
}

cItemPlayfield::~cItemPlayfield()
{
}


QRectF
cItemPlayfield::boundingRect() const
{
    return   QRectF( QPoint( 0, 0 ), QSize( 1, 1 ) * GRIDSIZE * CELLSIZE );
}


void
cItemPlayfield::paint( QPainter*  iPainter, const QStyleOptionGraphicsItem*  iOptions, QWidget*  iWidget )
{
    QColor color = QColor( 200, 200, 200 );
    iPainter->setBrush( QBrush( color ) );

    auto pen = QPen(  Qt::black );
    pen.setWidth( 5 );
    iPainter->setPen( pen );
    QRectF bbox = boundingRect();
    iPainter->drawRect( bbox );
}
