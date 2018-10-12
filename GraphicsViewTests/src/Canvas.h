#pragma once

#include <QGraphicsView>
#include <QPaintEvent>

#include "ToolModel.h"
#include "cGridItem.h"

class cEditableItem;

class cCanvas :
    public QGraphicsView
{
    Q_OBJECT

public:
    cCanvas( QWidget *parent = nullptr );

public:
    void  paintEvent( QPaintEvent* iEvent ) override;

    void  dragEnterEvent( QDragEnterEvent* iEvent ) override;
    void  dragMoveEvent( QDragMoveEvent* iEvent ) override;
    void  dragLeaveEvent( QDragLeaveEvent* iEvent ) override;
    void  dropEvent( QDropEvent* iEvent ) override;

    void  keyPressEvent( QKeyEvent* iEvent ) override;
    void  keyReleaseEvent( QKeyEvent* iEvent ) override;

    void  mousePressEvent( QMouseEvent* iEvent )    override;
    void  mouseMoveEvent( QMouseEvent* iEvent )     override;
    void  mouseReleaseEvent( QMouseEvent* iEvent )  override;
    void  wheelEvent( QWheelEvent* iEvent )  override;

public:
    void  SetPixmap( const QPixmap& iPixmap );
    void  SetToolModel( cToolModel* iToolModel );

    void  UpdateGridItem();

signals:
    void  currentFrameGotPainted( const QPixmap& iPixmap );


private:
    enum  eState
    {
        kIdle,
        kPan,
        kZoom,
        kDrawing
    };

    eState              mState;
    cEditableItem*      mEditableItem;
    cGridItem*          mGridItem;
    QPointF             mClickPos;
    QPixmap             mItemPixmap;

    cToolModel*         mToolModel;
    QPainter*           mPainter;
};

