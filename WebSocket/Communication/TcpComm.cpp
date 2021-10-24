#include "TcpComm.h"

#include <QTcpSocket>
#include <QTimerEvent>
#include <QStringList>

struct TcpCommData
{
    TcpCommData()
        : address("192.168.1.109:20001"),
          socket(0)
    {}

    QString address;
    QTcpSocket *socket;
};

TcpComm::TcpComm(QObject *parent)
    : QObject(parent)
{
    d = new TcpCommData;
}

TcpComm::~TcpComm()
{
    delete d;
    d = 0;
}

QObject *TcpComm::containerObject()
{
    return this;
}

void TcpComm::setAddress(const QString &address)
{
    if(d->address == address)
        return;

    d->address = address;
    emit addressChanged(address);
}

QString TcpComm::address() const
{
    return d->address;
}

void TcpComm::start()
{
    if(d->socket)
        this->stop();

    QStringList addressList = d->address.split(":");

    d->socket = new QTcpSocket(this);
    connect(d->socket,
            &QTcpSocket::stateChanged,
            this,
            &TcpComm::stateChanged);
    connect(d->socket,
            &QTcpSocket::readyRead,
            this,
            &TcpComm::onReadyRead);
    connect(d->socket,
            SIGNAL(error(QAbstractSocket::SocketError)),
            this,
            SLOT(onError(QAbstractSocket::SocketError)));

    d->socket->connectToHost(addressList.first(), addressList.value(1).toInt());
}

void TcpComm::stop()
{
    if(!d->socket)
        return;

    d->socket->disconnectFromHost();
    d->socket->disconnect(this, 0);
    d->socket->deleteLater();
    d->socket = 0;
}

QAbstractSocket::SocketState TcpComm::state()
{
    if(!d->socket)
        return QAbstractSocket::UnconnectedState;

    return d->socket->state() ;
}

int TcpComm::writeData(const QByteArray &data)
{
    if(!d->socket)
        return 0;

    return d->socket->write(data);
}

void TcpComm::onReadyRead()
{
    if(!d->socket)
        return;

    QByteArray data = d->socket->readAll();
    emit dataRecieved(data);
}

void TcpComm::onError(QAbstractSocket::SocketError error)
{
    qDebug() << "TcpComm::Error:"<<d->address<<" : " << error;
}
