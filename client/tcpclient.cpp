#include "tcpclient.h"
#include <QMessageBox>
#include <QHostInfo>
#include <QTextCodec>

QTextCodec *tc = QTextCodec::codecForLocale();

TcpClient::TcpClient(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("TCP Client"));

    contentListWidget = new QListWidget(this);

    sendLineEdit = new QLineEdit(this);
    sendBtn = new QPushButton(QStringLiteral("发送"), this);

    userNameLabel = new QLabel(QStringLiteral("用户名："), this);
    userNameLineEdit = new QLineEdit(tr("ppan"), this);

    serverIPLabel = new QLabel(QStringLiteral("服务器地址："), this);
    serverIPLineEdit = new QLineEdit(tr("10.40.113.112"), this);

    portLabel = new QLabel(QStringLiteral("端口："), this);
    portLineEdit = new QLineEdit(this);

    enterBtn= new QPushButton(QStringLiteral("进入聊天室"), this);

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(contentListWidget,0,0,1,2);
    mainLayout->addWidget(sendLineEdit,1,0);
    mainLayout->addWidget(sendBtn,1,1);
    mainLayout->addWidget(userNameLabel,2,0);
    mainLayout->addWidget(userNameLineEdit,2,1);
    mainLayout->addWidget(serverIPLabel,3,0);
    mainLayout->addWidget(serverIPLineEdit,3,1);
    mainLayout->addWidget(portLabel,4,0);
    mainLayout->addWidget(portLineEdit,4,1);
    mainLayout->addWidget(enterBtn,5,0,1,2);

    status = false;

    port = 8010;
    portLineEdit->setText(QString::number(port));

    serverIP =new QHostAddress();

    connect(enterBtn,SIGNAL(clicked()),this,SLOT(slotEnter()));
    connect(sendBtn,SIGNAL(clicked()),this,SLOT(slotSend()));

    sendBtn->setEnabled(false);
}

TcpClient::~TcpClient()
{

}

void TcpClient::slotEnter()
{
    if(!status)
        {
            QString ip = serverIPLineEdit->text();
            if(!serverIP->setAddress(ip))
            {
                QMessageBox::information(this,tr("error"),tr("server ip address error!"));
                return;
            }

            if(userNameLineEdit->text()=="")
            {
                QMessageBox::information(this,tr("error"),tr("User name error!"));
                return;
            }

            userName=userNameLineEdit->text();

            tcpSocket = new QTcpSocket(this);
            connect(tcpSocket,SIGNAL(connected()),this,SLOT(slotConnected()));
            connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
            connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(dataReceived()));
            connect(tcpSocket, SIGNAL(hostFound()), this, SLOT(slotState()));
            connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotState()));

            tcpSocket->connectToHost(*serverIP, portLineEdit->text().toInt());

            status=true;
        }
        else
        {
            QString msg=userName+tr(":Leave Chat Room");
            if(msg.toLocal8Bit().size() != tcpSocket->write(msg.toLocal8Bit()))
            {
                return;
            }

            tcpSocket->disconnectFromHost();
            tcpSocket->deleteLater();
            tcpSocket = nullptr;

            status=false;
    }
}

void TcpClient::slotConnected()
{
    sendBtn->setEnabled(true);
    enterBtn->setText(QStringLiteral("离开"));

    QString msg=userName+QString(":Enter Chat Room");
    tcpSocket->write(msg.toLocal8Bit());
}

void TcpClient::slotDisconnected()
{
    sendBtn->setEnabled(false);
    enterBtn->setText(QStringLiteral("进入聊天室"));
}

void TcpClient::dataReceived()
{
    while(tcpSocket->bytesAvailable()>0)
    {
        QByteArray datagram = tcpSocket->readAll();

        QString msg=tc->toUnicode(datagram);

        contentListWidget->addItem(msg);
    }
}

void TcpClient::slotSend()
{
    if(sendLineEdit->text()=="")
        {
            return ;
        }

        QString msg=userName + ":" + sendLineEdit->text();

        QByteArray arr = msg.toLocal8Bit();

        tcpSocket->write(arr);
        sendLineEdit->clear();
}

void TcpClient::slotState()
{
    qDebug() << "state" << tcpSocket->state();
    qDebug() << "error" << tcpSocket->error();
}
