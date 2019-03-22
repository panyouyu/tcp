#include "tcpserver.h"

TcpServer::TcpServer(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("TCP Server"));

    ContentListWidget = new QListWidget(this);

    PortLabel = new QLabel(QStringLiteral("端口:"), this);
    PortLineEdit = new QLineEdit(this);

    CreateBtn = new QPushButton(QStringLiteral("创建聊天室"),this);

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(ContentListWidget, 0, 0, 1, 2);
    mainLayout->addWidget(PortLabel, 1, 0);
    mainLayout->addWidget(PortLineEdit, 1, 1);
    mainLayout->addWidget(CreateBtn, 2, 0, 1, 2);

    port = 8010;
    PortLineEdit->setText(QString::number(port));

    connect(CreateBtn, SIGNAL(released()), this, SLOT(slotCreateServer()));
}

TcpServer::~TcpServer()
{

}

void TcpServer::slotCreateServer()
{
    server = new Server(this, port);
    connect(server, SIGNAL(updateServer(QString)), this, SLOT(updateServer(QString)));
    CreateBtn->setEnabled(false);
}

void TcpServer::updateServer(QString msg)
{
    ContentListWidget->addItem(msg);
}
