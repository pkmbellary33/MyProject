#ifndef WSCLIENT_H
#define WSCLIENT_H

#include <QObject>
#include <QAbstractSocket>
#include <QFile>

struct WSClientData;
class WSClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float altitude READ altitude WRITE setAltitude NOTIFY mapDataChanged)
    Q_PROPERTY(float charge READ charge WRITE setCharge NOTIFY mapDataChanged)
    Q_PROPERTY(float latitude READ latitude WRITE setLatitude NOTIFY mapDataChanged)
    Q_PROPERTY(float longitude READ longitude WRITE setLongitude NOTIFY mapDataChanged)
    Q_PROPERTY(float heading READ heading WRITE setHeading NOTIFY mapDataChanged)
    Q_PROPERTY(float mode READ mode WRITE setMode NOTIFY mapDataChanged)
    Q_PROPERTY(float sigstr READ sigstr WRITE setSigstr NOTIFY mapDataChanged)

    Q_ENUMS(MODE)

public:
    WSClient(const QString& address = "ws://localhost:1000");
    ~WSClient();

    void connectWebSocketServer();
    void disconnectWebSocketServer();

    enum MODE
    {
        MODE_NAV,
        MODE_LTR,
        MODE_LND,
        MODE_RTL
    };

    // use writeMessage to send command to telemetryServer
    Q_INVOKABLE void writeMessage(const QString &message);

    void setAltitude(float data) { m_altitude = data; emit mapDataChanged();}
    float altitude() const { return m_altitude; }

    void setCharge(float data) { m_charge = data; emit mapDataChanged();}
    float charge() const { return m_charge; }

    void setLatitude(float data) { m_latitude = data; emit mapDataChanged();}
    float latitude() const { return m_latitude; }

    void setLongitude(float data) { m_longitude = data; emit mapDataChanged();}
    float longitude() const { return m_longitude; }

    void setHeading(float data) { m_heading = data; emit mapDataChanged();}
    float heading() const { return m_heading; }

    void setMode(float data) { m_mode = data; emit mapDataChanged();}
    float mode() const { return m_mode; }

    void setSigstr(float data) { m_sigstr = data; emit mapDataChanged();}
    float sigstr() const { return m_sigstr; }

private slots:
    void onStateChanged(QAbstractSocket::SocketState state);

    // receives the telemetry in JSON string format inside this slot
    void onMessageReceived(const QString &text);

signals:
    void mapDataChanged();
private:
    WSClientData *d;
    float m_altitude;
    float m_charge;
    float m_latitude;
    float m_longitude;
    float m_heading;
    float m_mode;
    float m_sigstr;
    QFile file;
};

#endif // WSCLIENT_H
