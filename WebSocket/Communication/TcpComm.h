#ifndef TCPCOMM_H
#define TCPCOMM_H

#include "IComm.h"

struct TcpCommData;
class TcpComm :
        public QObject,
        virtual public Asteria::IComm
{
    Q_OBJECT
    Q_INTERFACES(Asteria::IComm)
    Q_PROPERTY(QString address READ address
               WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(QAbstractSocket::SocketState state READ state
               NOTIFY stateChanged)

public:
    TcpComm(QObject *parent = 0);
    ~TcpComm();

    // Asteria::IComm Implementation
    QObject* containerObject();

    void setAddress(const QString &address);
    QString address() const;

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();

    QAbstractSocket::SocketState state();

public slots:
    Q_INVOKABLE int writeData(const QByteArray &data);

signals:
    void addressChanged(const QString &address);
    void stateChanged(QAbstractSocket::SocketState state);
    void dataRecieved(const QByteArray &data);

private slots:
    void onReadyRead();
    void onError(QAbstractSocket::SocketError error);

private:
    TcpCommData *d;
};

#endif // TCPCOMM_H
