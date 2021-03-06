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
    void  UpCursor();
    void  DrawCursor();
    void  toolChanged( const QModelIndex& Left, const QModelIndex& Right, const QVector< int >& Roles );

signals:
    void  currentFrameGotPainted( const QPixmap& iPixmap );
    void  previousFrameGotPainted( const QPixmap& iPixmap );


private:
    enum  eState
    {
        kIdle,
        kPan,
        kZoom,
        kDrawing
    };

    enum  eTool
    {
        kBrush,
        kEraser
    };

    eState              mState;
    cEditableItem*      mEditableItem;
    cGridItem*          mGridItem;
    QPointF             mClickPos;
    QPixmap*            mItemPixmap;

    eTool               mTool;
    cToolModel*         mToolModel;
    QPainter*           mPainter;

    QPixmap*            cursorPixmap;
};

