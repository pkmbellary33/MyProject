#ifndef TELEMETRYSERVER_H
#define TELEMETRYSERVER_H

#include <QObject>
#include <QHostAddress>

struct TelemetryServerData;
class TelemetryServer : public QObject
{
    Q_OBJECT

public:
    TelemetryServer(QObject *parent = 0);
    ~TelemetryServer();

    // listens on ws://localhost:port
    void listen(const QHostAddress &host = QHostAddress::LocalHost, int port=1000);
    void writeToClients(const QString &data);

    Q_SIGNAL void messageReceived(const QString &message);

private slots:
    void onNewConnection();
    void onClientDisconnected();

private:
    TelemetryServerData *d;
};

#endif // TELEMETRYSERVER_H
