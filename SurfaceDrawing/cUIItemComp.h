#pragma once

#include <QGraphicsItem>
#include <QGraphicsColorizeEffect>

#include "cUser.h"

class cUIItemComp :
    public QGraphicsItem
{
public:
    ~cUIItemComp();
    cUIItemComp( cUser* iUser, int iCompIndex, QGraphicsItem* iParent = 0);

public:
    QRectF boundingRect() const override;
    void   paint( QPainter* iPainter, const QStyleOptionGraphicsItem*  iOptions, QWidget* iWidget = 0 )  override;

public:
    void  SetImage( const QString& iImageFileName );
    void  SetGrayImage( const QString& iImageFileName ); // Yeah i know, but QGraphicsEffects are kinda shit to use as they apply to the wole item and affects every paint operation
    void  Update();

private:
    void  _CreateGrayOutEffect();

public:
    QPixmap*                    mPixmap             = 0;
    QPixmap*                    mPixmapGrayOut      = 0;
    //QGraphicsColorizeEffect*    mGreyOut            = 0;
    cUser*                      mUser               = 0;
    int                         mCompIndex          = -1;

    bool                        mActive             = false;
    bool                        mCoolingdown        = false;
    float                       mCDPercent          = 1.0F;
    float                       mUsagePercent       = 1.0F;
};
