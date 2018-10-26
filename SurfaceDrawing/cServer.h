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

public slots:
    void  NewClientConnected();
    void  Update();

signals:
    void  quit();

private:
    cPaperLogic*            mPaperLogic;
    QVector< QTcpSocket* >  mClients;
    QTimer*                 mTimer;

    bool                    mQuit = false;
};

