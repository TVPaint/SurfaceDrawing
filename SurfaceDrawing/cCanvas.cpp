#include "cCanvas.h"


#include "cUser.h"
#include "cItemUser.h"
#include "cBasicTile.h"
#include "cPaperLogic.h"


#include <QGraphicsScene>
#include <QKeyEvent>


cCanvas::~cCanvas()
{
}


cCanvas::cCanvas( cPaperLogic* iPaperLogic, QWidget* parent ) :
    QGraphicsView( parent ),
    mPaperLogic( iPaperLogic )
{
    QGraphicsScene* scene = new QGraphicsScene();
    scene->setSceneRect( 0, 0, GRIDSIZE * CELLSIZE, GRIDSIZE * CELLSIZE );
    setScene( scene );


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
            int singleIndex = x + y*GRIDSIZE;
            auto tile = mAllTiles[ singleIndex ];

            if( iType == cPaperLogic::kPlayer )
            {
                if( newValue == -1 )
                {
                    tile->mHalf = false;
                }
                else
                {
                    if( mPaperLogic->mPaperGrid[x][y].mGround == -1 )
                    {
                        tile->mHalf = true;
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
                return;
            }

            QColor color = cPaperLogic::GetColorByIndex( 0 );

            if( iType == cPaperLogic::kTrail )
                color = color.lighter( 170 );
            else if( iType == cPaperLogic::kGround )
                color = color.darker( 170 );

            tile->mColor = color;
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
    if( iEvent->key() == Qt::Key_Left )
    {
        mAllUserItems[ 0 ]->mUser->setMovementVector( QPoint( -1, 0 ) );
    }
    else if( iEvent->key() == Qt::Key_Right )
    {
        mAllUserItems[ 0 ]->mUser->setMovementVector( QPoint( 1, 0 ) );
    }
    else if( iEvent->key() == Qt::Key_Up )
    {
        mAllUserItems[ 0 ]->mUser->setMovementVector( QPoint( 0, -1 ) );
    }
    else if( iEvent->key() == Qt::Key_Down )
    {
        mAllUserItems[ 0 ]->mUser->setMovementVector( QPoint( 0, 1 ) );
    }
}


void
cCanvas::paintEvent( QPaintEvent * iEvent )
{
    QGraphicsView::paintEvent( iEvent );

    //auto painter = QPainter( scene() );

    //QPointF test = mapToScene( 0, 0 );

    //int first_col = test.x() / CELLSIZE;
    //int first_row = test.y() / CELLSIZE;
    //int last_col  = 1 + (test.x() + width() ) / CELLSIZE;
    //int last_row  = 1 + (test.y() + height() ) / CELLSIZE;

    //for( int x = first_col; x <= last_col; ++x )
    //{
    //    painter.drawLine( x, 0, x, height() );
    //}
    //for( int y = first_row; y <= last_row; ++y )
    //{
    //    painter.drawLine( 0, y, width(), y );
    //}
}


// ==================================================================================
// ==================================================================================
// ==================================================================================


void
cCanvas::AddUser( cUser* iUser )
{
    auto userItem = new cItemUser( iUser );
    mAllUserItems.insert( iUser->mIndex, userItem );
    scene()->addItem( userItem );
}


void
cCanvas::Update()
{
    for( auto item : mAllUserItems )
        item->setPos( item->mUser->mGUIPosition );

    ensureVisible( mAllUserItems[ 0 ], width()/2-CELLSIZE - 2, height()/2-CELLSIZE - 2 );
}
