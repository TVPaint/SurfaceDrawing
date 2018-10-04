#include "cMainWindow.h"

#include "cItem.h"
#include <QGraphicsPixmapItem >

cMainWindow::cMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    mCurrentFrame = 0;
    emit  CurrentFrameChanged( mCurrentFrame );

    mAnimationTimer = new QTimer();
    mAnimationTimer->start( 1000 / 24 );


    connect( ui.playButton, &QPushButton::clicked, this, &cMainWindow::PlayPressed );
    connect( ui.stopButton, &QPushButton::clicked, this, &cMainWindow::StopPressed );
    connect( this, &cMainWindow::CurrentFrameChanged, ui.graphicsView, &cCustomGraphicsView::CurrentFrameChanged );
}


void
cMainWindow::TimerTick()
{
    int animationCount = ui.graphicsView->GetAnimationImages().size();
    mCurrentFrame = ++mCurrentFrame % animationCount;
    emit  CurrentFrameChanged( mCurrentFrame );

    UpdatePreview();
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
    mCurrentFrame = 0;
    emit  CurrentFrameChanged( mCurrentFrame );
    UpdatePreview();
}


void
cMainWindow::UpdatePreview()
{
    auto currentItem = ui.graphicsView->GetAnimationImages().at( mCurrentFrame );
    ui.previewLabel->setPixmap( currentItem->pixmap() );
}
