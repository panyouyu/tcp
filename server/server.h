#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include "tcpclientsocket.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr, int port = 8010);
    ~Server();

signals:
    void updateServer(QString);
public slots:
    //¹ã²¥ÏûÏ¢
    void updateClients(QString);
    void slotDisConnected(qintptr);
protected:
    void incomingConnection(qintptr handle);
private:
    QList<TcpClientSocket*>tcpSocketList;
};

#endif // SERVER_H
