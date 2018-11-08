#include "SurfaceDrawing.h"

#include <QApplication>
#include <time.h>



#include "cServer.h"
#include "CircularBuffer.h"

#include <QObject>
#include <QTimer>


void
TestCircularBuffer()
{
    cCircularBuffer< int >  b( 100 );

    assert( b.Size() == 100 );

    for( int i = 0; i < 50; ++i )
        b.Write( i );

    assert( b[5] == 5 );
    assert( b.Back() == 49 );
    assert( b.Back() == b[49] );
    assert( b.Count() == 50 );

    for( int i = 50; i < 200; ++i )
        b.Write( i );

    assert( b[5] == 105 );
    assert( b.Back() == 199 );
    assert( b.Back() == b[99] );
    assert( b.Count() == 100 );
}




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
