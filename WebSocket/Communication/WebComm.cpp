#include "WebComm.h"

#include <QtWebSockets/QWebSocket>
#include <QTimerEvent>
#include <QStringList>

struct WebCommData
{
    WebCommData()
        : address("192.168.1.109:20001"),
          socket(0)
    {}

    QString address;
    QWebSocket *socket;
};

WebComm::WebComm(QObject *parent)
    : QObject(parent)
{
    d = new WebCommData;
}

WebComm::~WebComm()
{
    delete d;
    d = 0;
}

QObject *WebComm::containerObject()
{
    return this;
}

void WebComm::setAddress(const QString &address)
{
    if(d->address == address)
        return;

    d->address = address;
    emit addressChanged(address);
}

QString WebComm::address() const
{
    return d->address;
}

void WebComm::start()
{
    if(d->socket)
        this->stop();

    d->socket = new QWebSocket(QString(), QWebSocketProtocol::VersionLatest, this);
    connect(d->socket,
            &QWebSocket::stateChanged,
            this,
            &WebComm::stateChanged);
    connect(d->socket,
            &QWebSocket::binaryMessageReceived,
            this,
            &WebComm::dataRecieved);
    connect(d->socket,
            &QWebSocket::textMessageReceived,
            this,
            &WebComm::textReceived);
    connect(d->socket,
            SIGNAL(error(QAbstractSocket::SocketError)),
            this,
            SLOT(onError(QAbstractSocket::SocketError)));

    d->socket->open(QUrl(this->address()));
}

void WebComm::stop()
{
    if(!d->socket)
        return;

    d->socket->close();
    d->socket->disconnect(this, 0);
    d->socket->deleteLater();
    d->socket = 0;
}

QAbstractSocket::SocketState WebComm::state()
{
    if(!d->socket)
        return QAbstractSocket::UnconnectedState;

    return d->socket->state() ;
}

int WebComm::writeData(const QByteArray &data)
{
    if(!d->socket)
        return 0;

    return d->socket->sendBinaryMessage(data);
}

int WebComm::writeText(const QString &text)
{
    if(!d->socket)
        return 0;

    return d->socket->sendTextMessage(text);
}

void WebComm::onError(QAbstractSocket::SocketError error)
{
    qDebug() << "WebComm::Error:"<<d->address<<" : " << error;
}
