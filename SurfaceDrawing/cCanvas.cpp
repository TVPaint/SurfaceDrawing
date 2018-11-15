#include "cCanvas.h"


#include "cUser.h"
#include "cItemUser.h"
#include "cBasicTile.h"
//#include "cItemGrid.h"
#include "cItemPlayfield.h"
#include "cPaperLogic.h"
#include "cBordersItem.h"
#include "cUIItemPlayerStat.h"
#include "cUIItemComp.h"
#include "cUICompBar.h"


#include <QGraphicsScene>
#include <QKeyEvent>


cCanvas::~cCanvas()
{
    // when the scene deletes it'll delete items inside
}


cCanvas::cCanvas( cPaperLogic* iPaperLogic, QWidget* parent ) :
    QGraphicsView( parent ),
    mPaperLogic( iPaperLogic )
{
    int offset = 600;
    int gridSize = GRIDSIZE * CELLSIZE + offset*2;

    QGraphicsScene* scene = new QGraphicsScene();
    scene->setSceneRect( -offset, -offset, gridSize, gridSize );
    setScene( scene );

    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );


    //setStyleSheet( "background: #404050" ); // Lags the game !


    mItemPlayfied = new cItemPlayfield( 0 );
    scene->addItem( mItemPlayfied );

    mCompBar = new cUICompBar();
    mCompBar->setZValue( 10 );
    scene->addItem( mCompBar );

    //mGrid = new cItemGrid( 0 );
    //mGrid->mSize = size();
    //scene->addItem( mGrid );

    mBordersItem = new cBordersItem();
    scene->addItem( mBordersItem );

    for( int i = 0; i < GRIDSIZE * GRIDSIZE; ++i )
    {
        auto tile = new cBasicTile();
        tile->mColor = QColor( Qt::transparent );
        tile->setPos( QPoint(       (i%GRIDSIZE) * CELLSIZE,
                                    (i/GRIDSIZE) * CELLSIZE )
                    );
        mAllTiles.push_back( tile );
        scene->addItem( tile );
    }

    mPaperLogic->AddGridChangedCB(

        [ this ]( int x, int y, int newValue, cPaperLogic::eDataType iType )
        {
            int singleIndex = x + y * GRIDSIZE;
            auto tile = mAllTiles[ singleIndex ];
            tile->mHalf = false;

            tile->mGroundColor  = mPaperLogic->GetColorByIndex( mPaperLogic->mPaperGrid[x][y].mGround ).darker( 170 );
            tile->mTrailColor   = mPaperLogic->GetColorByIndex( mPaperLogic->mPaperGrid[x][y].mTrail ).lighter( 170 );

            if( iType == cPaperLogic::kPlayer )
            {

                if( newValue >= 0 )
                {
                    if( mAllUserItems[ newValue ] == nullptr )
                        return;

                    if( mPaperLogic->mPaperGrid[x][y].mGround < 0 )
                    {
                        tile->mHalf = true;
                        tile->mTrailColor   = mPaperLogic->GetColorByIndex( mPaperLogic->mPaperGrid[x][y].mPlayer ).lighter( 170 );

                        if( mAllUserItems[ newValue ]->mUser->mGUICurrentMovementVector.x() > 0 )
                            tile->mDirection = cBasicTile::kEast;
                        else if( mAllUserItems[ newValue ]->mUser->mGUICurrentMovementVector.x() < 0 )
                            tile->mDirection = cBasicTile::kWest;
                        else if( mAllUserItems[ newValue ]->mUser->mGUICurrentMovementVector.y() > 0 )
                            tile->mDirection = cBasicTile::kSouth;
                        else if( mAllUserItems[ newValue ]->mUser->mGUICurrentMovementVector.y() < 0 )
                            tile->mDirection = cBasicTile::kNorth;
                    }
                }
            }

            tile->update();
        }
    );
}


// ==================================================================================
// ==================================================================================
// ==================================================================================


void
cCanvas::keyPressEvent( QKeyEvent * iEvent )
{
    if( iEvent->isAutoRepeat() )
        return;

    if( iEvent->key() == Qt::Key_Left )
    {
        mMyself->mUser->setMovementVector( QPoint( -1, 0 ) );
        emit directionChanged( mPaperLogic->mTick, 1 );
    }
    else if( iEvent->key() == Qt::Key_Right )
    {
        mMyself->mUser->setMovementVector( QPoint( 1, 0 ) );
        emit directionChanged( mPaperLogic->mTick, 2 );
    }
    else if( iEvent->key() == Qt::Key_Up )
    {
        mMyself->mUser->setMovementVector( QPoint( 0, -1 ) );
        emit directionChanged( mPaperLogic->mTick, 3 );
    }
    else if( iEvent->key() == Qt::Key_Down )
    {
        mMyself->mUser->setMovementVector( QPoint( 0, 1 ) );
        emit directionChanged( mPaperLogic->mTick, 4 );
    }
    else if( iEvent->key() == Qt::Key_Space )
    {
        mPaperLogic->SetUserAskedRespawn( mMyself->mUser );
        emit respawnRequest();
    }
    else if( iEvent->key() == Qt::Key_Ampersand )
    {
        mMyself->mUser->activateComp( 0 );
        emit compRequest( mPaperLogic->mTick, 0 );
    }
    else if( iEvent->key() == Qt::Key_Eacute )
    {
        mMyself->mUser->activateComp( 1 );
        emit compRequest( mPaperLogic->mTick, 1 );
    }
    else if( iEvent->key() == Qt::Key_P )
    {
        emit pingRequest();
    }
    else if( iEvent->key() == Qt::Key_R )
    {
        emit rollbackTest();
    }
    else if( iEvent->key() == Qt::Key_U )
    {
        emit askResync();
    }
}


void
cCanvas::keyReleaseEvent( QKeyEvent * iEvent )
{
    if( iEvent->key() == Qt::Key_Ampersand && !iEvent->isAutoRepeat() )
    {
        mMyself->mUser->deactivateComp( 0 );
        emit compStopRequest( mPaperLogic->mTick, 0 );
    }
}


void
cCanvas::resizeEvent( QResizeEvent* event )
{
    //mGrid->mSize = size() + QSize( CELLSIZE, CELLSIZE );
    //mGrid->update();
    for( auto item : items() )
        item->update();

    QGraphicsView::resizeEvent( event );
}



// ==================================================================================
// ==================================================================================
// ==================================================================================


void
cCanvas::AddUser( cUser* iUser, eUserType iUserType )
{
    if( iUser->mIndex < 0 )
        return;

    auto userItem = new cItemUser( iUser );
    mAllUserItems.insert( iUser->mIndex, userItem );
    scene()->addItem( userItem );

    auto uiStat = new cUIItemPlayerStat( mPaperLogic, iUser );
    mUIItemsStats.push_back( uiStat );
    scene()->addItem( uiStat );

    if( iUserType == kMyself )
    {
        mMyself = mAllUserItems[ iUser->mIndex ];

        auto uiRollback = new cUIItemComp( mMyself->mUser, 0 );
        uiRollback->SetImage( ":/SurfaceDrawing/Resources/Icone.png" );
        uiRollback->SetGrayImage( ":/SurfaceDrawing/Resources/IconeGray.png" );
        uiRollback->setZValue( 11 );
        mUIItemsComps.push_back( uiRollback );
        scene()->addItem( uiRollback );

        auto uiSpeed = new cUIItemComp( mMyself->mUser, 1 );
        uiSpeed->SetImage( ":/SurfaceDrawing/Resources/SpeedUp.png" );
        uiSpeed->SetGrayImage( ":/SurfaceDrawing/Resources/SpeedUpGray.png" );
        uiSpeed->setZValue( 11 );
        mUIItemsComps.push_back( uiSpeed );
        scene()->addItem( uiSpeed );
    }
}


void
cCanvas::RemoveUser( cUser* iUser )
{
    if( iUser->mIndex < 0 )
        return;

    auto userItem = mAllUserItems[iUser->mIndex];
    if( !userItem || userItem == mMyself )
        return;
    auto it = mAllUserItems.find( iUser->mIndex );
    mAllUserItems.erase( it );

    cUIItemPlayerStat* uiItem = 0;
    for( int i = 0; i < mUIItemsStats.size(); ++i )
    {
        if( mUIItemsStats[i]->mUser->mIndex == iUser->mIndex )
         {
             uiItem = mUIItemsStats[i];
             mUIItemsStats.erase( mUIItemsStats.begin() + i );
             break;
         }
    }
    if( uiItem )
    {
        scene()->removeItem( uiItem );
        delete uiItem;
    }
}


void
cCanvas::Update()
{
    //QPointF firstPos = mapToScene( 0, 0 );
    //firstPos.setX( firstPos.x() - (int(firstPos.x()) %  CELLSIZE) );
    //firstPos.setY( firstPos.y() - (int(firstPos.y()) %  CELLSIZE) );
    //mGrid->setPos( firstPos );

    // Users
    for( auto item : mAllUserItems )
    {
        if( item )
            item->setPos( item->mUser->mGUIPosition );
    }

    ensureVisible( mMyself, width()/2-CELLSIZE - 2, height()/2-CELLSIZE - 2 );

    // CompBar
    QPointF uiBarPos = mapToScene( size().width()/2 - UIBARWIDTH/2, size().height() - UIBARHEIGHT );
    mCompBar->setPos( uiBarPos );

    // Comps
    int uiElementCount = mUIItemsComps.size();
    int totalUIElementsSize = UICOMPCOUNT * UICOMPSIZE   +   (UICOMPCOUNT-1) * UICOMPSPACING;


    float xPos = size().width()/2 - totalUIElementsSize/2;
    QPointF uiPos = mapToScene( xPos, size().height() - UICOMPSIZE - 3 );

    for( auto uiItem : mUIItemsComps )
    {
        uiItem->setPos( uiPos );
        uiItem->Update();
        uiPos.setX( uiPos.x() + UICOMPSIZE + UICOMPSPACING );
    }


    // Stats
    QPointF uiStatPos = mapToScene( size().width() - cUIItemPlayerStat::UI_ITEM_RECT.width(), cUIItemPlayerStat::UI_ITEM_RECT.height() + 2 );
    for( auto uiItem : mUIItemsStats )
    {
        uiItem->setPos( uiStatPos );
        uiStatPos += QPointF( 0, cUIItemPlayerStat::UI_ITEM_RECT.height() + 2 );
    }

}








