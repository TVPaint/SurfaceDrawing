#include "cCurrentFrameItem.h"

#include "cCustomGraphicsView.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>


#define WIDTH 36
#define FRAMEHEIGHT 32
#define HEIGHT 50
#define YPOS 0


cCurrentFrameItem::cCurrentFrameItem( cCustomGraphicsView* iParentView, QGraphicsItem *parent ) :
    QGraphicsItem ( parent ),
    mParentView( iParentView )
{
    setFlags( ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges );
    setAcceptHoverEvents( true );
    mColor = QColor( 120, 205, 233 );
    setZValue( -5 );

    mPlaying = false;
}


void
cCurrentFrameItem::setIndex( int iIndex )
{
    mIndex = iIndex;
}


void
cCurrentFrameItem::setPlaying( bool iPlaying )
{
    mPlaying = iPlaying;
}


QRectF
cCurrentFrameItem::boundingRect() const
{
    return  QRectF( 0, 0, WIDTH, HEIGHT );
}


void
cCurrentFrameItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    QColor fillColor = mColor;

    if( mPlaying )
        fillColor = QColor( 31, 237, 181 );

    if( option->state & QStyle::State_Selected )
        mColor.darker( 150 );

    if( option->state & QStyle::State_MouseOver )
        fillColor = fillColor.lighter(125);

    painter->setBrush(fillColor);
    painter->drawRect( 0, 0, WIDTH, HEIGHT );
}


void
cCurrentFrameItem::mousePressEvent( QGraphicsSceneMouseEvent* iEvent )
{
    update();
    QGraphicsItem::mousePressEvent( iEvent );
}


void
cCurrentFrameItem::mouseMoveEvent( QGraphicsSceneMouseEvent* iEvent )
{
    QGraphicsItem::mouseMoveEvent( iEvent );
}


void
cCurrentFrameItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *iEvent )
{
    update();
    QGraphicsItem::mouseReleaseEvent( iEvent );
}


QVariant
cCurrentFrameItem::itemChange( GraphicsItemChange change, const QVariant & value )
{
    QPointF newPoint = value.toPointF();
    int currentFrame = newPoint.x() / 37;

    if( change == ItemPositionChange && scene() )
    {
        if( currentFrame < 0 )
            currentFrame = 0;
        if( currentFrame >= mParentView->GetAnimationImages().size() )
            currentFrame = mParentView->GetAnimationImages().size() - 1;

        newPoint.setY( -(HEIGHT/2 - FRAMEHEIGHT/2) );
        newPoint.setX( currentFrame*5 + currentFrame*32 );

        return newPoint;
    }
    else if( change == ItemPositionHasChanged && scene() )
    {
        mParentView->ItemCurrentFrameMoved();
    }

    return QGraphicsItem::itemChange( change, value );
}

