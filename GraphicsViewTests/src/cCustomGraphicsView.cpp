#include "cCustomGraphicsView.h"

#include "cItem.h"
#include "cAddItem.h"
#include "cCurrentFrameItem.h"


#define YPOS 0
#define ITEMSIZE 32

cCustomGraphicsView::cCustomGraphicsView( QWidget *parent ) :
    QGraphicsView( parent ),
    mCurrentFrame( 0 )
{
    QGraphicsScene* scene = new QGraphicsScene( this );
    setScene( scene );
    setAlignment( Qt::AlignVCenter | Qt::AlignLeft );

    auto imgA = new  cGraphicItem( this );
    imgA->setZValue( -1 );
    imgA->setFile( "Resources/ImgA.png" );
    scene->addItem( imgA );
    mAnimationImages.push_back( imgA );

    auto imgB = new  cGraphicItem( this );
    imgB->setZValue( -1 );
    imgB->setFile( "Resources/ImgB.png" );
    scene->addItem( imgB );
    mAnimationImages.push_back( imgB );

    auto imgC = new  cGraphicItem( this );
    imgC->setZValue( -1 );
    imgC->setFile( "Resources/ImgC.png" );
    scene->addItem( imgC );
    mAnimationImages.push_back( imgC );

    auto imgD = new  cGraphicItem( this );
    imgD->setZValue( -1 );
    imgD->setFile( "Resources/ImgD.png" );
    scene->addItem( imgD );
    mAnimationImages.push_back( imgD );

    mAddItem = new  cAddItem( this );
    mAddItem->setZValue( -1 );
    scene->addItem( mAddItem );

    mCurrentFrameItem = new cCurrentFrameItem( this );
    scene->addItem( mCurrentFrameItem );

    _UpdateItemsPosition();
    _UpdateCurrentFrameItemPosition();
}


void
cCustomGraphicsView::AddItem()
{
    int index = scene()->items().size() - 2; // Cuz addbutton and currentFrame is within
    mAddItem->setPos( 2+ (index+1)*5 + (index+1)*32, YPOS );

    auto item = new  cGraphicItem( this );
    item->setPos( 2+ index*5 + index*ITEMSIZE, YPOS );
    item->setIndex( index );
    item->setZValue( -1 );
    scene()->addItem( item );
    mAnimationImages.push_back( item );
}


void
cCustomGraphicsView::itemMoving( cGraphicItem* iItem, const QPointF& iNewPosition )
{
}


void
cCustomGraphicsView::itemMoved()
{
    _SortItems();
    _UpdateItemsPosition();
}


void
cCustomGraphicsView::itemCurrentFrameMoved()
{
    auto index = mCurrentFrameItem->pos().x() / ITEMSIZE;
    CurrentFrameChanged( index );
    _UpdateCurrentFrameItemPosition();
}


QVector< cGraphicItem* >&
cCustomGraphicsView::GetAnimationImages()
{
    return  mAnimationImages;
}


void
cCustomGraphicsView::CurrentFrameChanged( int iCurrent )
{
    mCurrentFrame = iCurrent;
    _UpdateCurrentFrameItemPosition();
    emit  currentFrameChanged( mCurrentFrame );
}


void cCustomGraphicsView::_SortItems()
{
    mAnimationImages.clear();
    for( auto item : scene()->items() )
    {
        if( item == mAddItem || item == mCurrentFrameItem )
            continue;

        QPointF position = item->pos();

        int i;
        for( i = 0; i < mAnimationImages.size(); ++i )
        {
            if( mAnimationImages.at( i )->pos().x() > position.x() )
                break;
        }

        auto itemAsGI = dynamic_cast< cGraphicItem* >( item );
        if( itemAsGI )
            mAnimationImages.insert( i, itemAsGI );
        else
            itemAsGI->acceptDrops(); // CRASH so we are aware in this example if any mistake is done
    }
}


void
cCustomGraphicsView::_UpdateItemsPosition()
{
    int i = 0;
    for( i = 0; i < mAnimationImages.size(); ++i )
        mAnimationImages[ i ]->setPos( 2 + i*5 + i*ITEMSIZE, YPOS );

    mAddItem->setPos( 2 + i*5 + i*ITEMSIZE, YPOS );
}


void
cCustomGraphicsView::_UpdateCurrentFrameItemPosition()
{
    mCurrentFrameItem->setPos( mCurrentFrame*5 + mCurrentFrame*ITEMSIZE, 1 );
}
