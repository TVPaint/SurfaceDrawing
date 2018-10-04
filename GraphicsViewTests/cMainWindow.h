#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_cMainWindow.h"

class cMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    cMainWindow(QWidget *parent = Q_NULLPTR);

private:
    Ui::cMainWindowClass ui;
};
