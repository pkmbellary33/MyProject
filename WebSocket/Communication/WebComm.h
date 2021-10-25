#ifndef WEBCOMM_H
#define WEBCOMM_H

#include "IComm.h"

struct WebCommData;
class WebComm :
        public QObject,
        virtual public MyProject::IComm,
        virtual public MyProject::ICommAddendum1
{
    Q_OBJECT
    Q_INTERFACES(MyProject::IComm)
    Q_INTERFACES(MyProject::ICommAddendum1)
    Q_PROPERTY(QString address READ address
               WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(QAbstractSocket::SocketState state READ state
               NOTIFY stateChanged)

public:
    WebComm(QObject *parent = 0);
    ~WebComm();

    // MyProject::IComm Implementation
    QObject* containerObject();

    void setAddress(const QString &address);
    QString address() const;

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();

    QAbstractSocket::SocketState state();

public slots:
    Q_INVOKABLE int writeData(const QByteArray &data);

    // MyProject::ICommAddendum1 implementation
    Q_INVOKABLE int writeText(const QString &text);

signals:
    void addressChanged(const QString &address);
    void stateChanged(QAbstractSocket::SocketState state);
    void dataRecieved(const QByteArray &data);

    // MyProject::ICommAddendum1 implementation
    void textReceived(const QString &text);

private slots:
    void onError(QAbstractSocket::SocketError error);

private:
    WebCommData *d;
};

#endif // WEBCOMM_H
