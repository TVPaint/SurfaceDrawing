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
    void  ConnectionError( QAbstractSocket::SocketError );
    void  GetData();

private:
    bool mConnectedToServer = false;
};

