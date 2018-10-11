#pragma once

#include <QGraphicsView>

class cEditableItem;

class cCanvas :
    public QGraphicsView
{
    Q_OBJECT

public:
    cCanvas( QWidget *parent = nullptr );

public:
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

signals:
        void  currentFrameChanged( int iNewIndex );


private:
    void  _SetupTools();

private:
    enum  eState
    {
        kIdle,
        kPan,
        kZoom,
        kDrawing
    };

    enum  eCurrentTool
    {
        kPen,
        kBrush
    };

    eState          mState;
    cEditableItem*  mEditableItem;
    QPointF         mClickPos;
    QPixmap         mItemPixmap;


    // Tools
    eCurrentTool    mCurrentTool;
    QPainter*       mPainter;
    QPen*           mPenTool;
    QBrush*         mBrushTool;
    QColor          mAPen;
    QSize           mToolSize;
};

