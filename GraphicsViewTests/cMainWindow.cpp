#include "cMainWindow.h"

#include "cItem.h"
#include <QGraphicsPixmapItem >

cMainWindow::cMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    mCurrentFrame = 0;

    mAnimationTimer = new QTimer();
    connect( mAnimationTimer, &QTimer::timeout, this, &cMainWindow::TimerTick );
    mAnimationTimer->start( 1000 / 24 );
}


void
cMainWindow::TimerTick()
{
    int animationCount = ui.graphicsView->GetAnimationImages().size();
    mCurrentFrame = ++mCurrentFrame % animationCount;

    auto currentItem = ui.graphicsView->GetAnimationImages().at( mCurrentFrame );

    ui.previewLabel->setPixmap( currentItem->pixmap() );
}
