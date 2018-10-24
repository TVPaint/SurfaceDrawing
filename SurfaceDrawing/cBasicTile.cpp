#include "cBasicTile.h"


#include "cPaperLogic.h"


#include <QBrush>
#include <QPainter>
#include <QPen>


cBasicTile::~cBasicTile()
{
}


cBasicTile::cBasicTile( QGraphicsItem* iParent ) :
    QGraphicsItem( iParent ),
    mHalf( false ),
    mDirection( kNorth )
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
    QRectF bbox = boundingRect();
    if( mHalf )
    {
        switch( mDirection )
        {
            case cBasicTile::kNorth:
                bbox.setTop( bbox.height() / 2 );
                break;
            case cBasicTile::kSouth:
                bbox.setBottom( bbox.height() / 2 );
                break;
            case cBasicTile::kEast:
                bbox.setRight( bbox.width() / 2 );
                break;
            case cBasicTile::kWest:
                bbox.setLeft( bbox.width() / 2 );
                break;
            default:
                break;
        }
    }

    iPainter->drawRect( bbox );
}


