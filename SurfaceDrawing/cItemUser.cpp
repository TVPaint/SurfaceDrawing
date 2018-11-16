#include "cItemUser.h"


#include <QPainter>


cItemUser::~cItemUser()
{
}


cItemUser::cItemUser( cUser* iUser, QGraphicsItem * parent ) :
    QGraphicsItem( parent ),
    mUser( iUser )
{
    qDebug() << "Create*********************************************************************************";
}


QRectF
cItemUser::boundingRect() const
{
    return   QRectF( QPoint( 0, 0 ), mUser->mGUISize );
}


void
cItemUser::paint( QPainter * iPainter, const QStyleOptionGraphicsItem * iOptions, QWidget * iWidget )
{
    if( mUser->mIsDead )
        return;

    auto size = mUser->mGUISize;
    QRadialGradient grad( mUser->mGUISize/2, mUser->mGUISize.x(), mUser->mGUISize/2 );
    grad.setColorAt( 0, mUser->mColor );
    grad.setColorAt( 1, mUser->mColor.darker( 120 ) );

    iPainter->setBrush( grad );
    iPainter->setPen( QPen( Qt::black ) );
    iPainter->drawRect( boundingRect() );

    iPainter->drawText( mUser->mGUISize/2 - QPoint( 2, -3 ) , QString::number( mUser->mCompPower ) );

    if( mUser->mCompPower > 1 )
    {
        QRectF bbox = boundingRect();
        QColor powerColor( 200, 200, 20, 100 );

        if( mUser->mCompPower > 3 )
        {
            powerColor.setRed( 255 );
            powerColor.setGreen( 120 );
        }

        if( mUser->mCompPower > 5 )
        {
            powerColor.setGreen( 20 );
        }


        if( mUser->mCompPower > 10 )
        {
            powerColor.setRed( 0 );
            powerColor.setGreen( 0 );
            powerColor.setBlue( 0 );
            powerColor.setAlpha( 255 );
        }


        iPainter->setBrush( powerColor );

        powerColor.setAlpha( 255 );
        iPainter->setPen( powerColor );
        iPainter->drawRect( bbox );
    }
}

