#pragma once


#include <QTcpSocket>

class cClient :
    public QTcpSocket
{
    Q_OBJECT

public:
    ~cClient();
    cClient();

public:
    void  AskConnection();

public slots:
    void  Connected();
    void  HostFound();
    void  ConnectionError( QAbstractSocket::SocketError );
};

