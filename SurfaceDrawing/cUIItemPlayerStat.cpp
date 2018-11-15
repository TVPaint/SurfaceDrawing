#include "cUIItemPlayerStat.h"


#include "cPaperLogic.h"
#include "cUser.h"

#include <QBrush>
#include <QPainter>
#include <QPen>
#include <QGraphicsColorizeEffect>



cUIItemPlayerStat::~cUIItemPlayerStat()
{
}


cUIItemPlayerStat::cUIItemPlayerStat( cPaperLogic*  iPaperLogic, cUser* iUser, QGraphicsItem* iParent ) :
    QGraphicsItem( iParent ),
    mPaperLogic( iPaperLogic ),
    mUser( iUser )
{
}


QRectF
cUIItemPlayerStat::boundingRect() const
{
    return  QRectF( 0, 0, UI_STAT_ITEM_WIDTH, UI_STAT_ITEM_HEIGHT );
}


void
cUIItemPlayerStat::paint( QPainter* iPainter, const QStyleOptionGraphicsItem* iOptions, QWidget* iWidget )
{
    QColor color = mUser->mColor;

    iPainter->setBrush( color );
    iPainter->setPen( Qt::black );

    QString  text = QString::number( mPaperLogic->ComputeLandUsage( mUser ) * 100, 'f', 2 );

    iPainter->drawRect( iconBoundingRect() );
    iPainter->drawText( textBoundingRect(), text );
}


QRectF
cUIItemPlayerStat::iconBoundingRect() const
{
    return   QRectF( QPoint( 0, 0 ), QSize( UI_STAT_ICON_SIZE, UI_STAT_ICON_SIZE ) );
}


QRectF
cUIItemPlayerStat::textBoundingRect() const
{
    return   QRect( QPoint( UI_STAT_ICON_SIZE + UI_STAT_TEXT_OFFSET, 0 ), QPoint( UI_STAT_TEXT_WIDTH, UI_STAT_ITEM_HEIGHT ) );
}

