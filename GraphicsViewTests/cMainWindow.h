#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_cMainWindow.h"

#include <QGraphicsScene>

class cCustomGraphicsView;

class cMainWindow :
    public QMainWindow
{
    Q_OBJECT

public:
    cMainWindow(QWidget *parent = Q_NULLPTR);

private:
    Ui::cMainWindow ui;

    QGraphicsScene* mScene;
};
