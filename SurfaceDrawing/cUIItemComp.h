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
    void  Update();

private:
    void  _CreateGrayOutEffect();

public:
    QPixmap*                    mPixmap             = 0;
    QGraphicsColorizeEffect*    mGreyOut            = 0;
    cUser*                      mUser               = 0;
    int                         mCompIndex          = -1;

    bool                        mActive             = false;
    bool                        mCoolingdown        = false;
};
