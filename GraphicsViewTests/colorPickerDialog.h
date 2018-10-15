#pragma once

#include "colorPicker.h"

#include  <QWidget>
#include  <QVBoxLayout>

class colorPickerDialog :
    public  QWidget
{

public:
    ~colorPickerDialog();
    colorPickerDialog( QWidget* iParent = nullptr );

    void  openAtPosition( QPointF iPos );

    QSize  size() const;

private:
    colorPicker* _colorPicker;
    QVBoxLayout* _mainLayout;
};

