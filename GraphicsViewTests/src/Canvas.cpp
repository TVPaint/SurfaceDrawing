#include "Canvas.h"


#include "EditableItem.h"

#include <QApplication>
#include <QClipboard>
#include <QColorDialog>
#include <QDragEnterEvent>
#include <QMimeData>


cCanvas::cCanvas( QWidget *parent ) :
    QGraphicsView( parent )
{
    // Config
    setAcceptDrops( true );

    // Scene
    QGraphicsScene* scene = new QGraphicsScene( this );
    setScene( scene );
    setAlignment( Qt::AlignCenter );

    mEditableItem = new cEditableItem();
    scene->addItem( mEditableItem );

    mGridItem = new cGridItem();
    scene->addItem( mGridItem );

    QRectF sceneRect = geometry();
    setSceneRect( sceneRect );


    setStyleSheet( "background-color: #555555");
}


void
cCanvas::paintEvent( QPaintEvent * iEvent )
{
    QGraphicsView::paintEvent( iEvent );
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
        UpdateGridItem();
    }
    else if( iEvent->key() == Qt::Key_Minus )
    {
        mEditableItem->setScale( mEditableItem->scale() * (1/1.5) );
        UpdateGridItem();
    }
    else if( iEvent->key() == Qt::Key_C )
    {
        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setPixmap( *mItemPixmap );
    }
    else if( iEvent->key() == Qt::Key_V )
    {
        QPainter pp( mItemPixmap );
        QClipboard* clipboard = QApplication::clipboard();
        auto px = clipboard->pixmap();

        pp.drawPixmap( 0, 0, px );
        mEditableItem->update();
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
        mItemPixmap = mEditableItem->mpixmap;
        mItemPixmap->fill( Qt::transparent );
        currentFrameGotPainted( *mItemPixmap );
        mEditableItem->update();
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
        mItemPixmap = mEditableItem->mpixmap;
        mPainter = mToolModel->getNewPainter( mItemPixmap );
    }

    QGraphicsView::mousePressEvent( iEvent );
}


void
cCanvas::mouseMoveEvent( QMouseEvent * iEvent )
{
    if( mState == kPan )
    {
        QPointF offset = iEvent->pos() - mClickPos;
        QPointF pos = mEditableItem->pos() + offset;
        mEditableItem->setPos( pos );
        mGridItem->setPos( pos );
    }
    else if( mState == kDrawing )
    {
        QPointF originInItemCoordinate = mEditableItem->mapFromScene( mapToScene( mClickPos.x(), mClickPos.y() ) );
        QPointF newPointInItemCoordinate = mEditableItem->mapFromScene( mapToScene( iEvent->pos().x(), iEvent->pos().y() ) );
        mPainter->drawLine( originInItemCoordinate, newPointInItemCoordinate );
        mEditableItem->update();
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
        currentFrameGotPainted( *mItemPixmap );
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
            mEditableItem->setScale( mEditableItem->scale() * 2.5 );
        else
            mEditableItem->setScale( mEditableItem->scale() / 1.5 );

        UpdateGridItem();
    }

    QGraphicsView::wheelEvent( iEvent );
}


void
cCanvas::SetPixmap( const QPixmap & iPixmap )
{
    delete  mEditableItem->mpixmap;
    mEditableItem->mpixmap = new QPixmap( iPixmap );
    mEditableItem->update();
}


void
cCanvas::SetToolModel( cToolModel* iToolModel )
{
    mToolModel = iToolModel;
}


void
cCanvas::UpdateGridItem()
{
    float  pixelSize = mEditableItem->sceneBoundingRect().width() / mEditableItem->boundingRect().width();

    mGridItem->psize = pixelSize;
    mGridItem->size = mEditableItem->sceneBoundingRect().size();
}

