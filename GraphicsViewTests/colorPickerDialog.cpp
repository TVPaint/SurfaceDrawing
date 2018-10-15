#include "colorPickerDialog.h"



colorPickerDialog::~colorPickerDialog()
{
}


colorPickerDialog::colorPickerDialog( QWidget* iParent ) :
    QWidget( iParent )
{
    setWindowFlags( Qt::Widget | Qt::FramelessWindowHint | Qt::Popup );
    setAttribute( Qt::WA_NoSystemBackground, true );
    setAttribute( Qt::WA_TranslucentBackground, true );
    setAttribute( Qt::WA_DeleteOnClose );

    _colorPicker = new colorPicker( this );
    _mainLayout = new QVBoxLayout( this );
    _mainLayout->setContentsMargins( 0, 0, 0, 0 );
    _mainLayout->addWidget( _colorPicker );

    setLayout( _mainLayout );
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
