#include "Canvas.h"


#include "EditableItem.h"

#include <QApplication>
#include <QColorDialog>
#include <QDragEnterEvent>
#include <QMimeData>


cCanvas::cCanvas( QWidget *parent ) :
    QGraphicsView( parent ),
    mPainter( 0 ),
    mPen( 0 ),
    mBrush( 0 )
{
    // Config
    setAcceptDrops( true );

    // Scene
    QGraphicsScene* scene = new QGraphicsScene( this );
    setScene( scene );
    setAlignment( Qt::AlignCenter );

    mEditableItem = new cEditableItem();
    scene->addItem( mEditableItem );
    QRectF sceneRect = geometry();
    setSceneRect( sceneRect );

    _SetupTools();
}


void
cCanvas::dragEnterEvent( QDragEnterEvent * iEvent )
{
    iEvent->acceptProposedAction();
}


void
cCanvas::dragMoveEvent( QDragMoveEvent * iEvent )
{
    iEvent->acceptProposedAction();
}


void
cCanvas::dragLeaveEvent( QDragLeaveEvent * iEvent )
{
    iEvent->accept();
}


void
cCanvas::dropEvent( QDropEvent * iEvent )
{
    const QMimeData* mimeData = iEvent->mimeData();
    if( mimeData->hasImage() )
    {
    }
    else if( mimeData->hasUrls() )
    {
    }

    iEvent->acceptProposedAction();
}


void
cCanvas::keyPressEvent( QKeyEvent * iEvent )
{
    if( iEvent->key() == Qt::Key_Alt && QApplication::mouseButtons() & Qt::MouseButton::LeftButton )
        mState = kPan;

    if( iEvent->key() == Qt::Key_Plus )
    {
        mEditableItem->setScale( mEditableItem->scale() * 1.5 );
    }
    else if( iEvent->key() == Qt::Key_Minus )
    {
        mEditableItem->setScale( mEditableItem->scale() * (1/1.5) );
    }
    else if( iEvent->key() == Qt::Key_C )
    {
        QColorDialog dialog( mAPen, this );
        if( dialog.exec() )
        {
            mAPen = dialog.selectedColor();
            mPen->setColor( mAPen );
            mBrush->setColor( mAPen );
        }
    }

    QGraphicsView::keyPressEvent( iEvent );
}


void
cCanvas::keyReleaseEvent( QKeyEvent * iEvent )
{
    if( iEvent->key() == Qt::Key_Alt )
    {
        mState = kIdle;
    }
    else if( iEvent->key() == Qt::Key_Delete )
    {
        mItemPixmap = mEditableItem->pixmap();
        mPainter = new QPainter( &mItemPixmap );
        mPainter->fillRect( QRect( QPoint( 0, 0), mItemPixmap.size() ), Qt::white );
        delete  mPainter;
        mEditableItem->setPixmap( mItemPixmap );
    }

    QGraphicsView::keyReleaseEvent( iEvent );
}



void
cCanvas::mousePressEvent( QMouseEvent * iEvent )
{
    mClickPos = iEvent->pos();

    if( QApplication::keyboardModifiers() & Qt::AltModifier )
    {
        mState = kPan;
    }
    else
    {
        mState = kDrawing;
        mItemPixmap = mEditableItem->pixmap();
        mPainter = new QPainter( &mItemPixmap );
        mPainter->setPen( *mPen );
    }

    QGraphicsView::mousePressEvent( iEvent );
}


void
cCanvas::mouseMoveEvent( QMouseEvent * iEvent )
{
    if( mState == kPan )
    {
        QPointF offset = iEvent->pos() - mClickPos;
        mEditableItem->setPos( mEditableItem->pos() + offset );
    }
    else if( mState == kDrawing )
    {
        QPointF originInItemCoordinate = mEditableItem->mapFromScene( mapToScene( mClickPos.x(), mClickPos.y() ) );
        QPointF newPointInItemCoordinate = mEditableItem->mapFromScene( mapToScene( iEvent->pos().x(), iEvent->pos().y() ) );
        mPainter->drawLine( originInItemCoordinate, newPointInItemCoordinate );
        mEditableItem->setPixmap( mItemPixmap );
    }


    mClickPos = iEvent->pos();
    QGraphicsView::mouseMoveEvent( iEvent );
}


void
cCanvas::mouseReleaseEvent( QMouseEvent * iEvent )
{
    if( mState == kDrawing )
    {
        delete  mPainter;
        mEditableItem->setPixmap( mItemPixmap );
        currentFrameGotPainted( mItemPixmap );
    }

    mState = kIdle;
    QGraphicsView::mouseReleaseEvent( iEvent );
}


void
cCanvas::wheelEvent( QWheelEvent * iEvent )
{
    int delta = iEvent->delta();
    if( QApplication::keyboardModifiers() & Qt::AltModifier )
    {
        if( delta > 0 )
            mEditableItem->setScale( mEditableItem->scale() * 1.5 );
        else
            mEditableItem->setScale( mEditableItem->scale() / 1.5 );
    }

    QGraphicsView::wheelEvent( iEvent );
}


void
cCanvas::SetPixmap( const QPixmap & iPixmap )
{
    mEditableItem->setPixmap( iPixmap );
}


void
cCanvas::_SetupTools()
{
    mPen        = new QPen();
    mBrush      = new QBrush();
    mAPen       = QColor( Qt::black );
    mToolSize   = 2;

    mBrush->setColor( mAPen );
    mBrush->setStyle( Qt::SolidPattern );
    mPen->setWidth( mToolSize );
    mPen->setBrush( *mBrush );
}


