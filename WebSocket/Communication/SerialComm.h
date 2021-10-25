#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include "IComm.h"

#include <QSerialPort>

struct SerialCommData;
class SerialComm :
        public QObject,
        virtual public MyProject::IComm
{
    Q_OBJECT
    Q_INTERFACES(MyProject::IComm)
    Q_PROPERTY(QString address READ address
               WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(QAbstractSocket::SocketState state READ state
               NOTIFY stateChanged)

public:
    SerialComm(QObject *parent = 0);
    ~SerialComm();

    // MyProject::IComm Implementation
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

private:
    void stopDownlinkTimer();
    void startDownlinkTimer();

private slots:
    void onReadyRead();
    void onError(QSerialPort::SerialPortError error);

protected:
    void timerEvent(QTimerEvent *event);

private:
    SerialCommData *d;
};


#endif // SERIALCOMM_H
