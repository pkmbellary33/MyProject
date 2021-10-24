#include "Application.h"
#include <QFile>
#include <QStringList>
#include <QTimerEvent>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

#include "TelemetryServer.h"

struct ApplicationData
{
    ApplicationData() :
        lastIndex(0),
        timerId(-1),
        mode("NAV"),
        telemetryServer(0)
    {}

    QStringList entries;
    int lastIndex;
    int timerId;
    QString mode;
    TelemetryServer *telemetryServer;
};

Application::Application(QObject *parent)
{
    d = new ApplicationData;
    QFile telemetryFile(":/File/telemetry.txt");
    if(telemetryFile.open(QIODevice::ReadOnly))
    {
        QString data = telemetryFile.readAll();
        d->entries = data.split("\r\n", QString::SkipEmptyParts);
    }
    else
        qDebug() << "error: " << telemetryFile.errorString();

    d->telemetryServer = new TelemetryServer(this);
    connect(d->telemetryServer,
            SIGNAL(messageReceived(QString)),
            this,
            SLOT(onMessageReceived(QString)));
    d->telemetryServer->listen(QHostAddress::Any, 10001);
    qDebug() << "Listening to 0.0.0.0:10001";
    d->timerId = this->startTimer(100);
}

Application::~Application()
{
    delete d;
    d = 0;
}

void Application::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == d->timerId)
    {
        if(d->telemetryServer)
        {
            QJsonObject json = QJsonDocument::fromJson(d->entries.value(d->lastIndex++).toUtf8()).object();
            json["mode"] = d->mode;
            d->telemetryServer->writeToClients(QJsonDocument(json).toJson(QJsonDocument::Compact));
        }

        if(d->lastIndex > d->entries.count())
            d->lastIndex = 0;
    }
}

void Application::onMessageReceived(const QString &message)
{
    QJsonDocument msgDoc = QJsonDocument::fromJson(message.toUtf8());
    d->mode = msgDoc.object()["command"].toString();
    qDebug() << "command received: " << d->mode;
}
