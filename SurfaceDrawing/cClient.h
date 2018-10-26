#pragma once

#include "cUser.h"

#include <QDataStream>

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

    void  ReadNewUser( const QString& iUserSerialized );

public slots:
    void  Connected();
    void  ConnectionError( QAbstractSocket::SocketError );
    void  GetData();


signals:
    void  newUserArrived( cUser* iUser );

private:
    bool            mConnectedToServer = false;
    QDataStream     mDataStream;
};

