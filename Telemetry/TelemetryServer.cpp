#include "TelemetryServer.h"
#include "QWebSocketServer"
#include "QWebSocket"

struct TelemetryServerData
{
    TelemetryServerData() :
        server(0)
    {}

    QList<QWebSocket*> websockets;
    QWebSocketServer *server;
};

TelemetryServer::TelemetryServer(QObject *parent)
    : QObject(parent)
{
    d = new TelemetryServerData;
    d->server = new QWebSocketServer(QStringLiteral("Telemetry Server"),
                                     QWebSocketServer::NonSecureMode, this);
    connect(d->server,
            SIGNAL(newConnection()),
            this,
            SLOT(onNewConnection()));
}

TelemetryServer::~TelemetryServer()
{
    delete d;
    d = 0;
}

void TelemetryServer::listen(const QHostAddress &host, int port)
{
    if(d->server)
        d->server->listen(host, port);
}

void TelemetryServer::writeToClients(const QString &data)
{
    Q_FOREACH(QWebSocket *websocket, d->websockets)
       websocket->sendTextMessage(data);
}

void TelemetryServer::onNewConnection()
{
    if(!d->server)
        return;

    QWebSocket *websocket = d->server->nextPendingConnection();
    connect(websocket,
            SIGNAL(textMessageReceived(QString)),
            this,
            SIGNAL(messageReceived(QString)));
    connect(websocket,
            SIGNAL(disconnected()),
            this,
            SLOT(onClientDisconnected()));

    d->websockets << websocket;
}

void TelemetryServer::onClientDisconnected()
{
    QObject *sender = this->sender();
    if(!sender)
        return;

    QWebSocket *websocket = qobject_cast<QWebSocket*>(sender);

    if(!websocket)
    {
        return;
    }

    d->websockets.removeAll(websocket);
}

