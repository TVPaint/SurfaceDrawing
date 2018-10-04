#include "cItem.h"

#include "cCustomGraphicsView.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>


#define SIZE 32


cGraphicItem::cGraphicItem( cCustomGraphicsView* iParentView, QGraphicsItem *parent ) :
    QGraphicsItem( parent ),
    mParentView( iParentView )
{
    setFlags( ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges );
    setAcceptHoverEvents( true );
    mColor = Qt::blue;
}


void
cGraphicItem::setIndex( int iIndex )
{
    mIndex = iIndex;
    mText = QString::number( iIndex );
}


QRectF
cGraphicItem::boundingRect() const
{
    qreal size = SIZE;
    return QRectF( -size/2,   -size/2,
                   size,       size);
}


void
cGraphicItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    QColor fillColor = ( option->state & QStyle::State_Selected ) ? mColor.dark( 150 ) : mColor;
    if (option->state & QStyle::State_MouseOver)
        fillColor = fillColor.light(125);

    painter->setBrush(fillColor);

    painter->drawRect( -SIZE/2, -SIZE/2, SIZE, SIZE );
    painter->drawText( 0, 0, mText );
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
        newPoint.setY( 50 );
        if( newPoint.x() < 0 )
            newPoint.setX( 0 );

        return newPoint;
    }

    mParentView->itemMoving( this, newPoint );

    return QGraphicsItem::itemChange( change, value );
}

