#include "EditableItem.h"

#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QPainter>



cEditableItem::cEditableItem( QGraphicsItem *parent ) :
    QGraphicsPixmapItem ( parent ),
    mpixmap( 0 )
{
    setAcceptHoverEvents( true );
}


QRectF
cEditableItem::boundingRect() const
{
    if( mpixmap )
        return QRectF( 0, 0, mpixmap->width(), mpixmap->height() );

    return  QRectF( 0, 0, 32, 32 );
}


void
cEditableItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
    if( mpixmap )
    {
        painter->setRenderHint( QPainter::SmoothPixmapTransform, (transformationMode() == Qt::SmoothTransformation) );
        painter->drawPixmap( offset(), *mpixmap );
    }

    QPen pen( Qt::black );
    pen.setWidth( 1 );
    auto  rect = boundingRect();
    int off = 1;
    rect.setLeft( rect.left() + off );
    rect.setTop( rect.top() + off );
    rect.setWidth( rect.width() - off );
    rect.setHeight( rect.height() - off );
    painter->drawRect( rect );
}


void
cEditableItem::setFile( const QString & iFile )
{
    mFileToEdit = iFile.toStdString();
}


void cEditableItem::mousePressEvent( QGraphicsSceneMouseEvent* iEvent )
{
    QGraphicsItem::mousePressEvent( iEvent );
}


void cEditableItem::mouseMoveEvent( QGraphicsSceneMouseEvent* iEvent )
{
    QGraphicsItem::mouseMoveEvent( iEvent );
}


void cEditableItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *iEvent )
{
    QGraphicsItem::mouseReleaseEvent( iEvent );
}


void
cEditableItem::contextMenuEvent( QGraphicsSceneContextMenuEvent * iEvent )
{
    //QMenu menu;
    //menu.addAction("Remove Frame");
    //menu.addAction("Select File");
    //QAction *a = menu.exec( iEvent->screenPos() );
    //if( !a )
    //    return;

    //if( a->text() == "Remove Frame" )
    //{
    //    mParentView->ItemAskToBeRemoved( this ); // DON'T DO ANYTHING AFTER THIS
    //    return;
    //}
    //else if( a->text() == "Select File" )
    //{
    //    _SelectNewFile();
    //}
}

