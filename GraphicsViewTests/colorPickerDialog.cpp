#include "colorPickerDialog.h"



colorPickerDialog::~colorPickerDialog()
{
    delete   _colorPicker;
    delete   _slider2;
    delete   _mainLayout;
    delete   _horlayout;
    delete   _container;
    delete   _slider;
}


colorPickerDialog::colorPickerDialog( cToolModel* model, QWidget* iParent ) :
    QWidget( iParent ),
    _ToolModel( model )
{
    setWindowFlags( Qt::Widget | Qt::FramelessWindowHint | Qt::Popup );
    setAttribute( Qt::WA_NoSystemBackground, true );
    setAttribute( Qt::WA_TranslucentBackground, true );
    setAttribute( Qt::WA_DeleteOnClose );

    _slider = new QSlider( this );
    _slider->setOrientation( Qt::Horizontal );
    _slider->setRange( 1, 5000 );
    _slider->setValue( _ToolModel->getSize() );

    _container = new QWidget( this );
    _colorPicker = new colorPicker( this );

    _slider2 = new QSlider( this );
    _slider2->setOrientation( Qt::Vertical );
    _slider2->setRange( 0, 100 );
    qreal h;
    qreal s;
    qreal l;
    _ToolModel->getColor().getHslF( &h, &s, &l );
    _slider2->setValue( l * 100 );

    _horlayout = new QHBoxLayout( this );
    _horlayout->setContentsMargins( 0, 0, 0, 0 );
    _horlayout->addWidget( _colorPicker );
    _horlayout->addWidget( _slider2 );
    _container->setLayout( _horlayout );


    _mainLayout = new QVBoxLayout( this );
    _mainLayout->setContentsMargins( 0, 0, 0, 0 );
    _mainLayout->addWidget( _slider );
    _mainLayout->addWidget( _container );

    setLayout( _mainLayout );

    connect( _slider, &QSlider::valueChanged, this, &colorPickerDialog::sliderChanged );
    connect( _slider2, &QSlider::valueChanged, this, &colorPickerDialog::slider2Changed );
    connect( _colorPicker, &colorPicker::colorPicked, this, &colorPickerDialog::colorPicked );
}


void
colorPickerDialog::openAtPosition( QPointF iPos )
{
    setGeometry( iPos.x(), iPos.y(), 200, 200 );
    _colorPicker->setColor( _ToolModel->getColor() );
    qreal h;
    qreal s;
    qreal l;
    _ToolModel->getColor().getHslF( &h, &s, &l );
    _slider2->setValue( l * 100 );
    show();
}


QSize
colorPickerDialog::size() const
{
    return QSize( 200, 200 );
}


void
colorPickerDialog::sliderChanged( double iValue )
{
    _ToolModel->setSize( int(iValue) );
}


void
colorPickerDialog::slider2Changed( double iValue )
{
    _colorPicker->setLight( iValue / 100 );
}


void
colorPickerDialog::colorPicked( const QColor & color )
{
    _ToolModel->setColor( color );

    if( _colorPicker->closeOnRelease() )
        close();
}
