#include "SerialComm.h"

#include <QTimerEvent>
#include <QSerialPortInfo>

#include <QDebug>

struct SerialCommData
{
    SerialCommData():
        serialPort(0),
        state(QAbstractSocket::UnconnectedState),
        isDownlink(false),
        downlinkTimerId(-1)
    {}

    QString address;
    QSerialPort *serialPort;
    QAbstractSocket::SocketState state;
    bool isDownlink;
    int downlinkTimerId;
};

SerialComm::SerialComm(QObject *parent)
    : QObject(parent)
{
    d = new SerialCommData;
}

SerialComm::~SerialComm()
{
    delete d;
    d = 0;
}

QObject *SerialComm::containerObject()
{
    return this;
}

void SerialComm::setAddress(const QString &address)
{
    if(d->address == address)
        return;

    d->address = address;
    emit addressChanged(address);
}

QString SerialComm::address() const
{
    return d->address;
}

void SerialComm::start()
{
    if(d->serialPort &&
            d->serialPort->isOpen())
        this->stop();

    if(d->address.isEmpty())
        return;

    d->serialPort = new QSerialPort(this);
    connect(d->serialPort,
            SIGNAL(readyRead()),
            this,
            SLOT(onReadyRead()));
    connect(d->serialPort,
            SIGNAL(error(QSerialPort::SerialPortError)),
            this,
            SLOT(onError(QSerialPort::SerialPortError)));

    QStringList addressList = d->address.split(":");
    d->serialPort->setPortName(addressList.first());
    d->serialPort->setBaudRate(addressList.value(1).toInt());

    if(d->serialPort->open(QIODevice::ReadWrite))
    {
        d->state = QAbstractSocket::ConnectedState;
        emit stateChanged(d->state);
    }
}

void SerialComm::stop()
{
    if(!d->serialPort)
        return;

    if(d->serialPort->isOpen())
        d->serialPort->close();

    d->state = QAbstractSocket::UnconnectedState;
    emit stateChanged(d->state);

    delete d->serialPort;
    d->serialPort = 0;
}

QAbstractSocket::SocketState SerialComm::state()
{
    if(!d->serialPort)
        return QAbstractSocket::UnconnectedState;

    if(d->isDownlink)
        return d->state;

    return MyProject::IComm::NoDownlink;
}

int SerialComm::writeData(const QByteArray &data)
{
    bool success = false;
    if(!d->serialPort)
        return success;

    success = d->serialPort->write(data) > 0 ? true : false;
    d->serialPort->flush();
    return success;
}

void SerialComm::stopDownlinkTimer()
{
    if(d->downlinkTimerId > -1)
    {
        this->killTimer(d->downlinkTimerId);
        d->downlinkTimerId = -1;
    }
}

void SerialComm::startDownlinkTimer()
{
    this->stopDownlinkTimer();

    d->downlinkTimerId = this->startTimer(2000);
}

void SerialComm::onReadyRead()
{
    if(!d->serialPort)
        return;

    this->startDownlinkTimer();
    QByteArray data = d->serialPort->readAll();

    emit dataRecieved(data);

    if(!d->isDownlink)
    {
        d->isDownlink = true;
        emit stateChanged(this->state());
    }
}


void SerialComm::onError(QSerialPort::SerialPortError error)
{
    qDebug() << "SerialComm::Error:"<<d->address<<" : " << error;
}

void SerialComm::timerEvent(QTimerEvent *event)
{
    if(!event)
        return;

    if(d->downlinkTimerId == event->timerId())
    {
        this->stopDownlinkTimer();
        d->isDownlink = false;
        emit stateChanged(this->state());
    }
}
