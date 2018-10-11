#include "cMainWindow.h"

#include "cGraphicItem.h"
#include <QGraphicsPixmapItem >

cMainWindow::cMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    mCurrentFrame = 0;
    emit  currentFrameChangeAsked( mCurrentFrame );

    mAnimationTimer = new QTimer();
    mAnimationTimer->start( 1000 / 24 );


    connect( ui.playButton, &QPushButton::clicked, this, &cMainWindow::PlayPressed );
    connect( ui.stopButton, &QPushButton::clicked, this, &cMainWindow::StopPressed );
    connect( this, &cMainWindow::currentFrameChangeAsked, ui.graphicsView, &cCustomGraphicsView::CurrentFrameChanged );
    connect( ui.graphicsView, &cCustomGraphicsView::currentFrameChanged, this, &cMainWindow::CurrentFrameChanged );
}


void
cMainWindow::TimerTick()
{
    int animationCount = ui.graphicsView->GetAnimationImages().size();
    emit  currentFrameChangeAsked( ++mCurrentFrame % animationCount );
}


void
cMainWindow::PlayPressed()
{
    if( ui.playButton->text() == "Play" )
    {
        connect( mAnimationTimer, &QTimer::timeout, this, &cMainWindow::TimerTick );
        ui.playButton->setText( "Pause" );
        mAnimationTimer->start( 1000/24 );
    }
    else
    {
        disconnect( mAnimationTimer, &QTimer::timeout, this, &cMainWindow::TimerTick );
        ui.playButton->setText( "Play" );
        mAnimationTimer->stop();
    }
}


void
cMainWindow::StopPressed()
{
    disconnect( mAnimationTimer, &QTimer::timeout, this, &cMainWindow::TimerTick );
    ui.playButton->setText( "Play" );
    mAnimationTimer->stop();
    emit  currentFrameChangeAsked( 0 );
}


void
cMainWindow::CurrentFrameChanged( int iNewIndex )
{
    if( iNewIndex == -1 )
        return;

    mCurrentFrame = iNewIndex;
    auto currentItem = ui.graphicsView->GetAnimationImages().at( mCurrentFrame );
    ui.canvas->SetPixmap( currentItem->pixmap() );
}
