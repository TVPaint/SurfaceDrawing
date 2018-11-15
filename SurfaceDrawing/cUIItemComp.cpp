#include "cUIItemComp.h"


#include "cPaperLogic.h"


#include <QBrush>
#include <QPainter>
#include <QPen>
#include <QGraphicsColorizeEffect>


cUIItemComp::~cUIItemComp()
{
    delete  mPixmap;
    delete  mPixmapGrayOut;
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
    return   QRectF( QPoint( 0, 0 ), QSize( UI_COMP_SIZE, UI_COMP_SIZE ) );
}


void
cUIItemComp::paint( QPainter * iPainter, const QStyleOptionGraphicsItem * iOptions, QWidget * iWidget )
{

    QRectF bbox = boundingRect();
    QColor color = QColor( 50, 150, 50, 100 );

    if( mActive )
        color = QColor( 150, 50, 50, 100 );
    else if( mCoolingdown )
        color = QColor( 126, 126, 126, 100 );

    iPainter->setBrush( color );
    iPainter->setPen( Qt::black );


    if( mPixmap )
    {
        if( mActive && mUsagePercent > 0 )
        {
            float inverse = (1.0F-mUsagePercent);
            QRectF subRectToTop( 0, 0, UI_COMP_SIZE, UI_COMP_SIZE * inverse);
            QRectF subRectBottom( 0, UI_COMP_SIZE*inverse, UI_COMP_SIZE, UI_COMP_SIZE*mUsagePercent );

            iPainter->drawRect( subRectBottom );

            iPainter->drawPixmap( subRectToTop, *mPixmap, subRectToTop );
            if( mPixmapGrayOut )
                iPainter->drawPixmap( subRectBottom, *mPixmapGrayOut, subRectBottom );

        }
        else if( mCoolingdown && !mActive )
        {
            QRectF subRectoTop( 0, 0, UI_COMP_SIZE, UI_COMP_SIZE*mCDPercent );
            QRectF subRectBottom( 0, UI_COMP_SIZE*mCDPercent, UI_COMP_SIZE, UI_COMP_SIZE - UI_COMP_SIZE*mCDPercent );

            iPainter->drawRect( subRectBottom );

            iPainter->drawPixmap( subRectoTop, *mPixmap, subRectoTop );
            if( mPixmapGrayOut )
                iPainter->drawPixmap( subRectBottom, *mPixmapGrayOut, subRectBottom );
        }
        else
        {
            iPainter->drawRect( bbox );
            iPainter->drawPixmap( 0, 0, *mPixmap );
        }
    }
}


void
cUIItemComp::SetImage( const QString & iImageFileName )
{
    delete  mPixmap;
    mPixmap = new QPixmap( iImageFileName );

}


void
cUIItemComp::SetGrayImage( const QString & iImageFileName )
{
    delete  mPixmapGrayOut;
    mPixmapGrayOut = new QPixmap( iImageFileName );
}


void
cUIItemComp::Update()
{
    mActive         = mUser->mComps[ mCompIndex ].mActive;
    mCoolingdown    = mUser->mComps[ mCompIndex ].mCooldown > 0;
    mCDPercent      = float(mUser->mComps[ mCompIndex ].mCooldown) / float(mUser->mComps[ mCompIndex ].mCooldownBase);
    mUsagePercent   = float(mUser->mComps[ mCompIndex ].mCompDuration) / float(mUser->mComps[ mCompIndex ].mCompDurationBase);

    if( mCoolingdown || mActive )
        update();
}


void
cUIItemComp::_CreateGrayOutEffect()
{
}


