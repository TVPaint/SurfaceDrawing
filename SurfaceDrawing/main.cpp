#include "SurfaceDrawing.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SurfaceDrawing w;
    w.show();
    return a.exec();
}
