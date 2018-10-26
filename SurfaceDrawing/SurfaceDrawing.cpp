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
}


void
SurfaceDrawing::Update()
{
    mPaperLogic->Update();
    mCanvas->Update();
}


void
SurfaceDrawing::Start()
{
    mTimer = new  QTimer();
    mTimer->start( 1000/60 );

    connect( mTimer, &QTimer::timeout, this, &SurfaceDrawing::Update );
}
