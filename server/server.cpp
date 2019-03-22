#include "server.h"
#include <QNetworkProxyFactory>
#include <QDebug>
Server::Server(QObject *parent, int port)
    : QTcpServer(parent)
{
    QNetworkProxyFactory::setUseSystemConfiguration(false);
    listen(QHostAddress::Any, port);
}

Server::~Server()
{

}

void Server::updateClients(QString msg)
{
    emit updateServer(msg);
    for(int i = 0; i < tcpSocketList.size(); ++i) {
        TcpClientSocket *item = tcpSocketList.at(i);
        if (item->write(msg.toLocal8Bit()) != msg.toLocal8Bit().size()) {
            continue;
        }
    }
}

void Server::slotDisConnected(qintptr descriptor)
{
    for(int i = 0; i < tcpSocketList.size(); ++i) {
        TcpClientSocket *item = tcpSocketList.at(i);
        if (item->socketDescriptor() == descriptor) {
            tcpSocketList.removeAt(i);
            return;
        }
    }
    return;
}

void Server::incomingConnection(qintptr handle)
{
    TcpClientSocket *tcpSocket = new TcpClientSocket();
    connect(tcpSocket, SIGNAL(updateClients(QString)), this, SLOT(updateClients(QString)));
    connect(tcpSocket, SIGNAL(disconnected(qintptr)), this, SLOT(slotDisConnected(qintptr)));
    tcpSocket->setSocketDescriptor(handle);
    tcpSocketList.append(tcpSocket);
}
