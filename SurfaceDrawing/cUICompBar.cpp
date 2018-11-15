#include "cUICompBar.h"

#include "Defines.h"

#include <QBrush>
#include <QPainter>
#include <QPen>



cUICompBar::~cUICompBar()
{
    delete  mPixmap;
}


cUICompBar::cUICompBar( QGraphicsItem* iParent ) :
    QGraphicsItem( iParent )
{
    mPixmap = new QPixmap( ":/SurfaceDrawing/Resources/Bar200.png" );
}


QRectF
cUICompBar::boundingRect() const
{
    return   QRectF( QPoint( 0, 0 ), QSize( UI_BAR_WIDTH, UI_BAR_HEIGHT ) );
}


void
cUICompBar::paint( QPainter * iPainter, const QStyleOptionGraphicsItem * iOptions, QWidget * iWidget )
{

    QRectF bbox = boundingRect();
    QColor color = QColor( 70, 65, 50, 180 );

    iPainter->setBrush( color );
    iPainter->setPen( Qt::black );

    if( mPixmap )
        iPainter->drawPixmap( 0, 0, *mPixmap );
    else
        iPainter->drawRect( bbox );
}

