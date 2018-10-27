#pragma once

#include  "cPaperLogic.h"

#include <QTcpSocket>
#include <QTcpServer>
#include <QTimer>

class cServer :
    public QTcpServer
{
    Q_OBJECT

public:
    ~cServer();
    cServer();

public:
    void  Run();


    void  SendDataToAllClients( const QString& iData );
    void  SendDataToSpecificClients( const QString& iData, QTcpSocket* iClient );

    void  SendGridToClient( QTcpSocket* iClient );
    void  SendSimpleUserPositionToClient( QTcpSocket* iClient, cUser* iUser );

public slots:
    void  NewClientConnected();
    void  Update();

signals:
    void  quit();

private:
    cPaperLogic*                mPaperLogic;
    QMap< int, QTcpSocket* >    mClients;
    QTimer*                     mTimer;

    bool                        mQuit = false;
};

