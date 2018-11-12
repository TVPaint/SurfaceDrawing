
#ifdef SERVERSIDE
#include "cServer.h"
#include <QObject>
#endif

#ifdef CLIENTSIDE
#include "SurfaceDrawing.h"
#endif

#include <QApplication>
#include <time.h>
#include <QTimer>



int main(int argc, char *argv[])
{
    srand( time(NULL) );

#ifdef CLIENTSIDE

    QApplication a(argc, argv);

    SurfaceDrawing w;
    w.show();

    return a.exec();

#endif // CLIENTSIDE


#ifdef  SERVERSIDE

    QCoreApplication a(argc, argv);

    cServer s;

    QObject::connect( &s, &cServer::quit, &a, &QCoreApplication::quit );
    s.Run();

    return  a.exec();

#endif
}
