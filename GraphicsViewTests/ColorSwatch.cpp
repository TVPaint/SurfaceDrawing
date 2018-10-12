#include "ColorSwatch.h"



ColorSwatch::~ColorSwatch()
{
}


ColorSwatch::ColorSwatch( QWidget* iParent ) :
    QLabel( iParent )
{
    setFixedHeight( 16 );
}


void
ColorSwatch::mouseReleaseEvent( QMouseEvent * iEvent )
{
    emit  swatchClicked();
    QLabel::mouseReleaseEvent( iEvent );
}


void
ColorSwatch::SetColor( const QColor & iColor )
{
    QPixmap px( size() );
    px.fill( iColor );
    setPixmap( px );
}

