#include "colorPicker.h"


#include <QPaintEvent>
#include <QPainter>

colorPicker::~colorPicker()
{
}


colorPicker::colorPicker( QWidget *parent ) :
    QWidget( parent )
{
    _pixmap = new QPixmap( width(), height() );
    _image = new QImage( width(), height(), QImage::Format_ARGB32_Premultiplied );

    _hudCircle = new QPixmap( 20, 20 );
    _hudPos = QPoint( 0, 0 );
    _light = 0.5F;

    drawWheel();
    drawHUDCircle();
    _timer.start( 200 );
    _timer.setSingleShot( true );
    _closeOnRelease = true;
}


void
colorPicker::paintEvent( QPaintEvent * event )
{
    QPainter painter( this );
    painter.drawPixmap( 0, 0, *_pixmap );
    painter.drawPixmap( _hudPos, *_hudCircle );
}


void
colorPicker::mousePressEvent( QMouseEvent * event )
{
    _hudPos = event->pos() - QPoint( 10, 10 );
    _selColor = _image->pixel( event->pos() );
    drawHUDCircle();
}


void
colorPicker::mouseMoveEvent( QMouseEvent * event )
{
    _hudPos = event->pos() - QPoint( 10, 10 );
    _selColor = _image->pixel( event->pos() );
    drawHUDCircle();
    update();
}


void
colorPicker::mouseReleaseEvent( QMouseEvent * event )
{
    if( _timer.isActive() )
    {
        _closeOnRelease = false;
        return;
    }

    _selColor = _image->pixel( event->pos() );
    emit  colorPicked( _selColor );
}


void
colorPicker::resizeEvent( QResizeEvent * event )
{
    delete  _pixmap;
    delete  _image;
    _pixmap = new QPixmap( width(), height() );
    _image = new QImage( width(), height(), QImage::Format_ARGB32_Premultiplied );
    drawWheel();
}


void
colorPicker::drawWheel()
{
    _pixmap->fill( Qt::transparent );

    QPainter painter( _pixmap );

    QConicalGradient outterWheel( width()/2, height()/2, 0 );
    outterWheel.setColorAt( 0, QColor::fromHslF( 0/360.0, 1.0, _light, 1.0 ) );
    outterWheel.setColorAt( 60.F/360.F, QColor::fromHslF( 60.F/360.0, 1.0, _light, 1.0 ) );
    outterWheel.setColorAt( 120.F/360.F, QColor::fromHslF( 120.F/360.0, 1.0, _light, 1.0 ) );
    outterWheel.setColorAt( 180.F/360.F, QColor::fromHslF( 180.F/360.0, 1.0, _light, 1.0 ) );
    outterWheel.setColorAt( 240.F/360.F, QColor::fromHslF( 240.F/360.0, 1.0, _light, 1.0 ) );
    outterWheel.setColorAt( 300.F/360.F, QColor::fromHslF( 300.F/360.0, 1.0, _light, 1.0 ) );
    outterWheel.setColorAt( 1, QColor::fromHslF( 360.F/360.0, 1.0, _light, 1.0 ) );
    painter.setBrush( outterWheel );

    painter.drawEllipse( 0, 0, width(), height() );

    *_image = _pixmap->toImage();
}


void
colorPicker::drawHUDCircle()
{
    _hudCircle->fill( Qt::transparent );

    QPainter painter( _hudCircle );
    painter.setPen( Qt::black );
    painter.setBrush( _selColor );

    painter.drawEllipse( 0, 0, 19, 19 );
}


void
colorPicker::setLight( float light )
{
    _light = light;

    drawWheel();
    qreal h;
    qreal s;
    qreal l;
    _selColor.getHslF( &h, &s, &l );

    _selColor.setHslF( h, s, _light );
    drawHUDCircle();
    update();

    emit  colorPicked( _selColor );
}


float
colorPicker::getLight() const
{
    return  _light;
}


void
colorPicker::setColor( const QColor & color )
{
    _selColor = color;

    qreal h;
    qreal s;
    qreal l;
    _selColor.getHslF( &h, &s, &l );
    _light = l;

    drawWheel();
    drawHUDCircle();
    update();
}


bool
colorPicker::closeOnRelease() const
{
    return  _closeOnRelease;
}


