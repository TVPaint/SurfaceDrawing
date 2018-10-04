#include "cAddItem.h"

#include "cCustomGraphicsView.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>


#define SIZE 32


cAddItem::cAddItem( cCustomGraphicsView* iParentView, QGraphicsItem *parent ) :
    QGraphicsItem( parent ),
    mParentView( iParentView )
{
    setFlags( ItemIsSelectable );
    setAcceptHoverEvents( true );
    mColor = Qt::red;
    mText = "+";
}


QRectF
cAddItem::boundingRect() const
{
    qreal size = SIZE;
    return QRectF( -size/2,   -size/2,
                   size,       size);
}


void
cAddItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    QColor fillColor = ( option->state & QStyle::State_Selected ) ? mColor.dark( 150 ) : mColor;
    if (option->state & QStyle::State_MouseOver)
        fillColor = fillColor.light(125);

    painter->setBrush(fillColor);

    painter->drawRect( -SIZE/2, -SIZE/2, SIZE, SIZE );
    painter->drawText( 0, 0, mText );
}


void cAddItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *iEvent )
{
    update();
    mParentView->AddItem();
    QGraphicsItem::mouseReleaseEvent( iEvent );
}
