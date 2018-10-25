#pragma once

#include "ui_ConnectionDialog.h"

#include <QDialog>


class cConnectionDialog :
    public  QDialog
{

public:
    ~cConnectionDialog();
    cConnectionDialog( const QString& ipAddress, QWidget* iParent = 0 );

public:
    QString  GetIP() const;
    quint16  GetPort() const;

private:
    Ui::ConnectionDialog     ui;
};

