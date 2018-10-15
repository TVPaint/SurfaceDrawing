#include "colorPickerDialog.h"



colorPickerDialog::~colorPickerDialog()
{
}


colorPickerDialog::colorPickerDialog( cToolModel* model, QWidget* iParent ) :
    QWidget( iParent ),
    _ToolModel( model )
{
    setWindowFlags( Qt::Widget | Qt::FramelessWindowHint | Qt::Popup );
    setAttribute( Qt::WA_NoSystemBackground, true );
    setAttribute( Qt::WA_TranslucentBackground, true );
    setAttribute( Qt::WA_DeleteOnClose );

    _colorPicker = new colorPicker( this );

    _slider = new QSlider( this );
    _slider->setOrientation( Qt::Horizontal );
    _slider->setRange( 1, 5000 );
    _slider->setValue( _ToolModel->getSize() );

    _mainLayout = new QVBoxLayout( this );
    _mainLayout->setContentsMargins( 0, 0, 0, 0 );
    _mainLayout->addWidget( _slider );
    _mainLayout->addWidget( _colorPicker );

    setLayout( _mainLayout );


    connect( _slider, &QSlider::valueChanged, this, &colorPickerDialog::sliderChanged );
}


void
colorPickerDialog::openAtPosition( QPointF iPos )
{
    setGeometry( iPos.x(), iPos.y(), 200, 200 );
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
