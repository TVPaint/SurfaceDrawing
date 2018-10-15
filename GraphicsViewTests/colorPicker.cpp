#include "colorPicker.h"


#include <QPaintEvent>
#include <QPainter>

colorPicker::~colorPicker()
{
}


colorPicker::colorPicker(QWidget *parent) :
    QWidget( parent )
{
}


void
colorPicker::paintEvent( QPaintEvent * event )
{
    QPainter painter( this );
    painter.setBrush( Qt::black );


    painter.drawRect( 0, 0, width(), height() );
}
