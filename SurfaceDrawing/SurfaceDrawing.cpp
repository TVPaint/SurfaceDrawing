#include "SurfaceDrawing.h"


#include "TickerTimer.h"


#include <QDebug>

SurfaceDrawing::~SurfaceDrawing()
{
    delete  mPaperLogic;
    delete  mTimer;
}


SurfaceDrawing::SurfaceDrawing(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi( this );
    Init();
}


void
SurfaceDrawing::Init()
{
    mPaperLogic = new cPaperLogic();
    mPaperLogic->Init();


    mCanvas = new cCanvas( mPaperLogic, this );
    mMainLayout = new QVBoxLayout();
    mMainLayout->addWidget( mCanvas );
    ui.centralWidget->setLayout( mMainLayout );

    mClientSocket = new cClient();
    mClientSocket->AskConnection();

    connect( mCanvas, &cCanvas::directionChanged, this, &SurfaceDrawing::DirectionChanged );
    connect( mCanvas, &cCanvas::respawnRequest, this, &SurfaceDrawing::RespawnRequest );


    connect( mClientSocket, &cClient::newUserArrived, this, &SurfaceDrawing::NewUserArrived );
    connect( mClientSocket, &cClient::myUserAssigned, this, &SurfaceDrawing::MyUserAssigned );
    connect( mClientSocket, &cClient::paperLogicArrived, this, &SurfaceDrawing::PaperLogicArrived );
    connect( mClientSocket, &cClient::userChangedDirection, this, &SurfaceDrawing::UserDirectionChanged );
    connect( mClientSocket, &cClient::userRequestedRespawn, this, &SurfaceDrawing::UserRequestedRespawn );
    connect( mClientSocket, &cClient::userDisconnected, this, &SurfaceDrawing::UserDisconnected );
}


void
SurfaceDrawing::Update()
{
    mPaperLogic->Update( mClientSocket->mApplicationClock->remainingTimeAsDuration().count() );
    mCanvas->Update();
}


void
SurfaceDrawing::Start()
{
    mTimer = new  QTimer();
    mTimer->start( SPEED/2 );

    connect( mTimer, &QTimer::timeout, this, &SurfaceDrawing::Update );
}


void
SurfaceDrawing::RespawnRequest()
{
    mClientSocket->SendRespawnRequest();
}


void
SurfaceDrawing::DirectionChanged( int iDirection )
{
    mClientSocket->SendNewDirection( iDirection );
}


void
SurfaceDrawing::PaperLogicArrived( cPaperLogic & iPaper, quint64 iTimestamp )
{
    // As we send newUser and otherUsers info, this should never be out of sync
    Q_ASSERT( iPaper.mAllUsers.size() == mPaperLogic->mAllUsers.size() );

    //if( iPaper.mAllUsers.size() != mPaperLogic->mAllUsers.size() )
    //{
    //    for( auto user : iPaper.mAllUsers )
    //    {
    //        int  index = iPaper.mAllUsers.key( user );
    //        if( mPaperLogic->mAllUsers.find( index ) != mPaperLogic->mAllUsers.end() )
    //            continue;

    //        auto newUser = new cUser( index );
    //        mCanvas->AddUser( newUser );
    //        mPaperLogic->AddUser( newUser );
    //    }
    //}

    qDebug() << "DELTATIME " + QString::number( iTimestamp - mClientSocket->mApplicationClock->remainingTimeAsDuration().count() );

    mPaperLogic->CopyFromPaper( iPaper, 0 );

    quint16 missingUpdates = 0;
    quint64  currentTime = mClientSocket->mApplicationClock->remainingTimeAsDuration().count();


    if( currentTime < iTimestamp )
    {
        quint64 deltaTime = iTimestamp - currentTime;
        missingUpdates = deltaTime / (1000/60); // 60 fps
        qDebug() << "DELTATIME " + QString::number( deltaTime );
    }

    qDebug() << "OFF BY -------------> " + QString::number( missingUpdates );

}


void
SurfaceDrawing::NewUserArrived( cUser* iUser )
{
    mCanvas->AddUser( iUser, cCanvas::kOther );
    mPaperLogic->AddUser( iUser );
}


void
SurfaceDrawing::MyUserAssigned( cUser * iUser )
{
    mCanvas->AddUser( iUser, cCanvas::kMyself );
    mPaperLogic->AddUser( iUser );
    Start();
}


void
SurfaceDrawing::UserDirectionChanged( cUser * iUser )
{
    mPaperLogic->mAllUsers[ iUser->mIndex ]->setMovementVector( iUser->mGUIMovementVector );
}


void
SurfaceDrawing::UserRequestedRespawn( cUser * iUser )
{
    mPaperLogic->TryRespawningPlayer( mPaperLogic->mAllUsers[ iUser->mIndex ] );
}


void
SurfaceDrawing::UserDisconnected( int iIndex )
{
    mPaperLogic->RemoveUser( mPaperLogic->mAllUsers[ iIndex ] );
}



