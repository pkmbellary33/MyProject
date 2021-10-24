#include "WSClient.h"
#include "IComm.h"
#include "Communication.h"
#include <QJsonDocument>
#include <QWebSocket>
#include <QFile>

struct WSClientData
{
    WSClientData() :
        webSocketAddress("ws://localhost:10001"),
        webComm(0)
    {}

    QString webSocketAddress;
    Asteria::ICommAddendum1 *webComm;
};

WSClient::WSClient(const QString& address)
{
    d = new WSClientData;
    d->webSocketAddress = address;

    QFile file("./log.csv");
    if(!file.exists())
    {
        file.open(QIODevice::WriteOnly);
        QTextStream stream(&file);
        stream <<"Signal\tCharge\tLatitude\tLongitude\tAltitude\tHeading\tMode\n";
        file.close();
    }
}

WSClient::~WSClient()
{

}

void WSClient::connectWebSocketServer()
{
    if(d->webComm)
        this->disconnectWebSocketServer();

    d->webComm = dynamic_cast<Asteria::ICommAddendum1*>(
                Communication::instance().webComm(this));

    if(!d->webComm)
        return;

    connect(d->webComm->containerObject(),
            SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,
            SLOT(onStateChanged(QAbstractSocket::SocketState)));
    connect(d->webComm->containerObject(),
            SIGNAL(textReceived(QString)),
            this,
            SLOT(onMessageReceived(QString)));

    d->webComm->setAddress(d->webSocketAddress);
    d->webComm->start();
}

void WSClient::disconnectWebSocketServer()
{
    if(d->webComm)
    {
        d->webComm->stop();
        d->webComm->containerObject()->disconnect(0, 0, 0, 0);
        d->webComm->containerObject()->deleteLater();
        d->webComm = 0;
    }
}

void WSClient::writeMessage(const QString &message)
{
    qDebug() << "wsClient write: " << message;
    QJsonObject obj;
    obj["command"] = message;
    QJsonDocument doc(obj);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    if(d->webComm)
        qDebug() << d->webComm->writeText(strJson);
}

void WSClient::onStateChanged(QAbstractSocket::SocketState state)
{
    qDebug() << "\nWS state: " << state;
}

void WSClient::onMessageReceived(const QString &text)
{
    QJsonDocument jsonResponse = QJsonDocument::fromJson(text.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    if(jsonObject.contains("barometricAltitude"))
        setAltitude(jsonObject.value("barometricAltitude").toDouble());
    if(jsonObject.contains("charge"))
        setCharge(jsonObject.value("charge").toDouble());
    if(jsonObject.contains("heading"))
        setHeading(jsonObject.value("heading").toDouble());
    if(jsonObject.contains("latitude"))
        setLatitude(jsonObject.value("latitude").toDouble());
    if(jsonObject.contains("longitude"))
        setLongitude(jsonObject.value("longitude").toDouble());
    if(jsonObject.contains("mode"))
        setMode(jsonObject.value("mode").toDouble());
    if(jsonObject.contains("signalStrength"))
        setSigstr(jsonObject.value("signalStrength").toDouble());

    QFile file("./log.csv");
    file.open(QIODevice::Append);
    QTextStream stream(&file);
    stream <<sigstr()<< "\t"<< charge() <<"\t"<< latitude() <<"\t"<< longitude() <<"\t"<< altitude() <<"\t"<< heading() <<"\t"<< mode() <<"\n";
    file.close();
}


