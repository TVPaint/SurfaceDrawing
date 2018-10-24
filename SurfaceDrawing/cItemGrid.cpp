#include "cItemGrid.h"

#include "cPaperLogic.h"


#include <QBrush>
#include <QPainter>
#include <QPen>

cItemGrid::cItemGrid( QGraphicsItem *parent ) :
    QGraphicsItem( parent )
{
}

cItemGrid::~cItemGrid()
{
}


QRectF
cItemGrid::boundingRect() const
{
    return   QRectF( QPoint( 0, 0 ), QSize( CELLSIZE, CELLSIZE ) );
}


void
cItemGrid::paint( QPainter*  iPainter, const QStyleOptionGraphicsItem*  iOptions, QWidget*  iWidget )
{
    iPainter->setBrush( QBrush( Qt::black ) );
    iPainter->setPen( QPen(  Qt::black ) );
    QRectF bbox = boundingRect();
    //switch( mDirection )
    //{
    //    case cBasicTile::kNorth:
    //        bbox.setTop( bbox.height() / 2 );
    //        break;
    //    case cBasicTile::kSouth:
    //        bbox.setBottom( bbox.height() / 2 );
    //        break;
    //    case cBasicTile::kEast:
    //        bbox.setRight( bbox.width() / 2 );
    //        break;
    //    case cBasicTile::kWest:
    //        bbox.setLeft( bbox.width() / 2 );
    //        break;
    //    default:
    //        break;
    //}

    //QPointF test = mapToScene( 0, 0 );

    //int first_col = test.x() / CELLSIZE;
    //int first_row = test.y() / CELLSIZE;
    //int last_col  = 1 + (test.x() + width() ) / CELLSIZE;
    //int last_row  = 1 + (test.y() + height() ) / CELLSIZE;

    //for( int x = first_col; x <= last_col; ++x )
    //{
    //    painter.drawLine( x, 0, x, height() );
    //}
    //for( int y = first_row; y <= last_row; ++y )
    //{
    //    painter.drawLine( 0, y, width(), y );
    //}

    iPainter->drawRect( bbox );
}
