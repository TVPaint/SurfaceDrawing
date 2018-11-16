#include "cBasicTile.h"


#include "cPaperLogic.h"


#include <QBrush>
#include <QPainter>
#include <QPen>


cBasicTile::~cBasicTile()
{
    delete  mDropCDReducPix;
}


cBasicTile::cBasicTile( QGraphicsItem* iParent ) :
    QGraphicsItem( iParent ),
    mHalf( false ),
    mDirection( kNorth ),
    mGroundColor( Qt::transparent ),
    mTrailColor( Qt::transparent ),
    mDrop( cPaperLogic::kNone )
{
    mDropCDReducPix = new QPixmap( ":/SurfaceDrawing/Resources/Time.png" );
}


QRectF
cBasicTile::boundingRect() const
{
    return   QRectF( QPoint( 0, 0 ), QSize( CELLSIZE, CELLSIZE ) );
}


void
cBasicTile::paint( QPainter * iPainter, const QStyleOptionGraphicsItem * iOptions, QWidget * iWidget )
{
    QRectF bbox = boundingRect();

    // Ground
    iPainter->setBrush( mGroundColor );
    iPainter->setPen( mGroundColor );
    iPainter->drawRect( bbox );

    // Trails
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

    iPainter->setBrush( mTrailColor );
    iPainter->setPen( mTrailColor );
    iPainter->setCompositionMode( QPainter::CompositionMode_Multiply );
    iPainter->drawRect( bbox );

    if( mDrop == cPaperLogic::kCooldownReduction )
        iPainter->drawPixmap( 0, 0, *mDropCDReducPix );
}


