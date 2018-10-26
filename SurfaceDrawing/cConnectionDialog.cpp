#include "cConnectionDialog.h"


cConnectionDialog::~cConnectionDialog()
{
}


cConnectionDialog::cConnectionDialog( const QString& ipAddress, QWidget* iParent ) :
    QDialog( iParent )
{
    ui.setupUi( this );
    ui.iPLineEdit->setText( ipAddress );
    ui.portSpinBox->setValue( 55666 );
}


QString
cConnectionDialog::GetIP() const
{
    return  ui.iPLineEdit->text();
}


quint16
cConnectionDialog::GetPort() const
{
    return  ui.portSpinBox->value();
}


