#ifndef ICOMM_H
#define ICOMM_H

#include <QtPlugin>
#include <QAbstractSocket>

namespace Asteria
{

class IComm
{
public:
    static const QAbstractSocket::SocketState NoDownlink =
            static_cast<QAbstractSocket::SocketState>(
                QAbstractSocket::ListeningState + 1000);

    virtual QObject *containerObject() = 0;

    virtual void setAddress(const QString &address) = 0;
    virtual QString address() const = 0;

    virtual void start() = 0;
    virtual void stop() = 0;

    virtual QAbstractSocket::SocketState state() = 0;

    // must be implemented as slots
    virtual int writeData(const QByteArray &data) = 0;

    // must be implemented as signals
    virtual void addressChanged(const QString &address) = 0;
    virtual void stateChanged(QAbstractSocket::SocketState state) = 0;
    virtual void dataRecieved(const QByteArray &data) = 0;
};

class ICommAddendum1 : virtual public Asteria::IComm
{
public:
    virtual int writeText(const QString &text) = 0;

    // must be implemented as signals
    virtual void textReceived(const QString &text) = 0;
};

}

Q_DECLARE_INTERFACE(Asteria::IComm, "com.asteria.IComm/1.0")
Q_DECLARE_INTERFACE(Asteria::ICommAddendum1, "com.asteria.ICommAddendum1/1.0")

#endif // ICOMM_H
