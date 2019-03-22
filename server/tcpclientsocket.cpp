#include "tcpclientsocket.h"
#include <QTextCodec>

QTextCodec *tc = QTextCodec::codecForLocale();

TcpClientSocket::TcpClientSocket(QObject *parent)
    : QTcpSocket(parent)
{    
    connect(this, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    connect(this, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
}

TcpClientSocket::~TcpClientSocket()
{

}

void TcpClientSocket::dataReceived()
{
    while(bytesAvailable() > 0) {
        QByteArray buf = readAll();

        QString msg = tc->toUnicode(buf);
        emit updateClients(msg);
    }
}

void TcpClientSocket::slotDisconnected()
{
    emit disconnected(socketDescriptor());
}
