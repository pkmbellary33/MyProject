#include "WSClient.h"
#include "IComm.h"
#include "Communication.h"
#include <QJsonDocument>
#include <QWebSocket>
#include <QFile>

struct WSClientData
{
    WSClientData() :
        webSocketAddress("ws://localhost:1000"),
        webComm(0)
    {}

    QString webSocketAddress;
    MyProject::ICommAddendum1 *webComm;
};

WSClient::WSClient(const QString& address)
{
    d = new WSClientData;
    d->webSocketAddress = address;
    QString fileName = "./log_"+QDate::currentDate().toString("dd-MM-yyyy") +"_"+ QTime::currentTime().toString("hh-mm-ss")+".csv";
    file.setFileName(fileName);
    file.open(QIODevice::Append);
    QTextStream stream(&file);
    stream <<"Signal\tCharge\tLatitude\tLongitude\tAltitude\tHeading\tMode\n";
}

WSClient::~WSClient()
{
    file.close();
}

void WSClient::connectWebSocketServer()
{
    if(d->webComm)
        this->disconnectWebSocketServer();

    d->webComm = dynamic_cast<MyProject::ICommAddendum1*>(
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
    QJsonObject obj;
    obj["command"] = message;
    QJsonDocument doc(obj);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    if(d->webComm)
        d->webComm->writeText(strJson);
}

void WSClient::onStateChanged(QAbstractSocket::SocketState state)
{

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
    {
        if(jsonObject.value("mode").toString() == "NAV")
            setMode(MODE_NAV);
        else if(jsonObject.value("mode").toString() == "LTR")
            setMode(MODE_LTR);
        else if(jsonObject.value("mode").toString() == "LND")
            setMode(MODE_LND);
        else
            setMode(MODE_RTL);
    }
    if(jsonObject.contains("signalStrength"))
        setSigstr(jsonObject.value("signalStrength").toDouble());
    QTextStream stream(&file);
    stream <<sigstr()<< "\t"<< charge() <<"\t"<< latitude() <<"\t"<< longitude() <<"\t"<< altitude() <<"\t"<< heading() <<"\t"<< mode() <<"\n";
}


