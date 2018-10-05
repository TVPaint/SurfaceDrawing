#include "cItem.h"

#include "cCustomGraphicsView.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>


#define SIZE 32
#define YPOS 0


cGraphicItem::cGraphicItem( cCustomGraphicsView* iParentView, QGraphicsItem *parent ) :
    QGraphicsPixmapItem ( parent ),
    mParentView( iParentView )
{
    setFlags( ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges );
    setAcceptHoverEvents( true );
    mColor = Qt::lightGray;
    setPixmap( QPixmap::fromImage( QImage( "Resources/Empty.png" ) ) );
    setShapeMode( BoundingRectShape );

    mPlaybackHighlight = false;
}


void
cGraphicItem::setIndex( int iIndex )
{
    mIndex = iIndex;
    mText = QString::number( iIndex );
}

void
cGraphicItem::setFile( const QString & iFile )
{
    setPixmap( QPixmap::fromImage( QImage( iFile ) ) );
}


void
cGraphicItem::setHighlighted( bool iHighlighted )
{
    mPlaybackHighlight = iHighlighted;
}


void
cGraphicItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    QColor fillColor = ( option->state & QStyle::State_Selected ) ? mColor.darker( 150 ) : mColor;
    if (option->state & QStyle::State_MouseOver)
        fillColor = fillColor.lighter(125);

    painter->setBrush(fillColor);
    painter->drawRect( 0, 0, SIZE, SIZE );

    QGraphicsPixmapItem::paint( painter, option, widget );
}


void cGraphicItem::mousePressEvent( QGraphicsSceneMouseEvent* iEvent )
{
    update();
    setZValue( 1 );
    QGraphicsItem::mousePressEvent( iEvent );
}


void cGraphicItem::mouseMoveEvent( QGraphicsSceneMouseEvent* iEvent )
{
    QGraphicsItem::mouseMoveEvent( iEvent );
}


void cGraphicItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *iEvent )
{
    update();
    mParentView->itemMoved();
    setZValue( -1 );
    QGraphicsItem::mouseReleaseEvent( iEvent );
}


QVariant
cGraphicItem::itemChange( GraphicsItemChange change, const QVariant & value )
{
    QPointF newPoint = value.toPointF();

    if( change == ItemPositionChange && scene() )
    {
        newPoint.setY( YPOS );
        if( newPoint.x() < 0 )
            newPoint.setX( 0 );

        return newPoint;
    }

    mParentView->itemMoving( this, newPoint );

    return QGraphicsItem::itemChange( change, value );
}

