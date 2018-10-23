#include "cBasicTile.h"


#include "cPaperLogic.h"


#include <QBrush>
#include <QPainter>
#include <QPen>


cBasicTile::~cBasicTile()
{
}


cBasicTile::cBasicTile( QGraphicsItem* iParent ) :
    QGraphicsItem( iParent )
{
}


QRectF
cBasicTile::boundingRect() const
{
    return   QRectF( QPoint( 0, 0 ), QSize( CELLSIZE, CELLSIZE ) );
}


void
cBasicTile::paint( QPainter * iPainter, const QStyleOptionGraphicsItem * iOptions, QWidget * iWidget )
{
    iPainter->setBrush( QBrush( mColor ) );
    iPainter->setPen( QPen( mColor ) );
    iPainter->drawRect( boundingRect() );
}


