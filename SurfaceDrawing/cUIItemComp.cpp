#include "cUIItemComp.h"


#include "cPaperLogic.h"


#include <QBrush>
#include <QPainter>
#include <QPen>
#include <QGraphicsColorizeEffect>


cUIItemComp::~cUIItemComp()
{
    delete  mPixmap;
    delete  mGreyOut;
}


cUIItemComp::cUIItemComp( cUser* iUser, int iCompIndex, QGraphicsItem* iParent ) :
    QGraphicsItem( iParent ),
    mUser( iUser ),
    mCompIndex( iCompIndex )
{
}


QRectF
cUIItemComp::boundingRect() const
{
    return   QRectF( QPoint( 0, 0 ), QSize( UICOMPSIZE, UICOMPSIZE ) );
}


void
cUIItemComp::paint( QPainter * iPainter, const QStyleOptionGraphicsItem * iOptions, QWidget * iWidget )
{

    QRectF bbox = boundingRect();
    QColor color = QColor( 50, 150, 50, 100 );

    if( mActive )
        color = QColor( 150, 50, 50, 100 );

    iPainter->setBrush( color );
    iPainter->setPen( Qt::black );

    iPainter->drawRect( bbox );

    if( mPixmap )
        iPainter->drawPixmap( 0, 0, *mPixmap );
}


void
cUIItemComp::SetImage( const QString & iImageFileName )
{
    delete  mPixmap;
    mPixmap = new QPixmap( iImageFileName );

}


void
cUIItemComp::Update()
{
    mActive        = mUser->mComps[ mCompIndex ].mActive;
    mCoolingdown   = mUser->mComps[ mCompIndex ].mCooldown > 0;

    if( mCoolingdown && !mActive )
    {
        if( mGreyOut == 0 )
        {
            _CreateGrayOutEffect();
            setGraphicsEffect( mGreyOut );
        }
    }
    else
    {
        setGraphicsEffect( 0 );
        mGreyOut = 0;
    }
}


void
cUIItemComp::_CreateGrayOutEffect()
{
    mGreyOut = new QGraphicsColorizeEffect();
    mGreyOut->setColor( Qt::gray );
    mGreyOut->setStrength( 1.0F );
}


