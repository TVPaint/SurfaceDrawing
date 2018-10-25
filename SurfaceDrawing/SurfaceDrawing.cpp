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

    mTimer = new  QTimer();
    mTimer->start( 1000/60 );

    mCanvas = new cCanvas( mPaperLogic, this );
    mMainLayout = new QVBoxLayout();
    mMainLayout->addWidget( mCanvas );
    ui.centralWidget->setLayout( mMainLayout );

    auto userInvalidA = new cUser( -2 );
    auto userInvalidB = new cUser( -1 );
    mCanvas->AddUser( userInvalidA );
    mPaperLogic->AddUser( userInvalidA );
    mCanvas->AddUser( userInvalidB );
    mPaperLogic->AddUser( userInvalidB );


    mAllUsers.push_back( new cUser( 0, cPaperLogic::GetColorByIndex( 0 ) ) );
    mCanvas->AddUser( mAllUsers.back() );
    mPaperLogic->AddUser( mAllUsers.back() );

    mAllUsers.push_back( new cUser( 1, cPaperLogic::GetColorByIndex( 1 ) ) );
    mCanvas->AddUser( mAllUsers.back() );
    mPaperLogic->AddUser( mAllUsers.back() );

    connect( mTimer, &QTimer::timeout, this, &SurfaceDrawing::Update );

    mClientSocket = new cClient();
    mClientSocket->AskConnection();
}


void
SurfaceDrawing::Update()
{
    mPaperLogic->Update();
    mCanvas->Update();
}
