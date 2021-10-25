#include "MicroHardRadio.h"

#include "TcpComm.h"

#include <QTime>

struct MicroHardRadioData
{
    MicroHardRadioData() :
        address("192.168.1.100:23"),
        userName("admin"),
        password("MyProject"),
        signalStrength(-1),
        tcpComm(0),
        previousTime(0),
        radioType(MyProject::IRadio::PicoDDL)
    {}

    ~MicroHardRadioData()
    {
        if(tcpComm)
        {
            delete tcpComm;
            tcpComm = 0;
        }
    }

    QString address;
    QString userName;
    QString password;
    int signalStrength;
    TcpComm *tcpComm;
    QTime timer;
    int previousTime;
    MyProject::IRadio::RadioType radioType;
};

MicroHardRadio::MicroHardRadio(QObject *parent) :
    QObject(parent)
{
    d = new MicroHardRadioData;
    d->tcpComm = new TcpComm(this);
    d->tcpComm->setAddress(d->address);

    connect(d->tcpComm,SIGNAL(dataRecieved(QByteArray)),
            SLOT(updateSignalStrength(QByteArray)));
}

MicroHardRadio::~MicroHardRadio()
{
    delete d;
    d = 0;
}

QObject *MicroHardRadio::containerObject()
{
    return this;
}

void MicroHardRadio::setUserName(const QString &name)
{
    if(d->userName != name)
    {
        d->userName = name;
        emit userNameChanged(d->userName);
    }
}

QString MicroHardRadio::userName() const
{
    return d->userName;
}

void MicroHardRadio::setPassword(const QString &password)
{
    if(d->password != password)
    {
        d->password = password;
        emit passwordChanged(d->password);
    }
}

QString MicroHardRadio::password() const
{
    return d->password;
}

void MicroHardRadio::setAddress(const QString &address)
{
    if(d->address != address)
    {
        d->address = address;
        d->tcpComm->setAddress(d->address);
        emit addressChanged(d->address);
    }
}

QString MicroHardRadio::address() const
{
    return d->address;
}

void MicroHardRadio::setRadioType(int type)
{
    if(d->radioType != type)
    {
        d->radioType = MyProject::IRadio::RadioType(type);
        emit radioTypeChanged(type);
    }
}

int MicroHardRadio::radioType()
{
    return d->radioType;
}

int MicroHardRadio::signalStrength() const
{
    return d->signalStrength;
}

void MicroHardRadio::start()
{
    if(d->tcpComm){
        d->tcpComm->start();
        d->timer.start();
    }
}

void MicroHardRadio::stop()
{
    if(d->tcpComm)
        d->tcpComm->stop();
}

void MicroHardRadio::updateSignalStrength(const QByteArray &data)
{
    if(!d->tcpComm)
        return;

    QByteArray signalCommand = "AT+MWRSSI\r\n";
    QByteArray rssiKey = "-[0-9][0-9]";
    QByteArray inputReadyStr = "UserDevice>";
    int rssi = 0;

    if(this->radioType() == MyProject::IRadio::NanoDDL)
    {
        signalCommand = "AT+WRSSI?\r\n";
        rssiKey = "RSSI :";
        inputReadyStr = "IPnDDL>";

        if(data.contains("RSSI :"))
            rssi = QString(data).split("RSSI :").last().split("OK")
                    .first().remove("dBm").toInt();
    }
    else
    {
        if(QString(data).contains(QRegExp(rssiKey)))
            rssi = QString(data).split("OK").first().toInt();
    }

    if(data.contains("login"))
    {
        QByteArray byteArray = QString("%1\r\n")
                .arg(d->userName).toLatin1();
        d->tcpComm->writeData(byteArray);
    }
    else if(data.contains("Password"))
    {
        QByteArray byteArray = QString("%1\r\n")
                .arg(d->password).toLatin1();
        d->tcpComm->writeData(byteArray);
    }
    else if(data.contains(inputReadyStr))
    {
        QByteArray byteArray = signalCommand;
        d->tcpComm->writeData(byteArray);
    }

    if(rssi)
    {
        double timeDiff = (d->timer.elapsed() - d->previousTime) * 0.001;

        if(timeDiff >= 1)
        {
            int signalStrength = 0;
            if(rssi > -65)
                signalStrength = 100;
            else if(rssi < -95)
                signalStrength = 0;
            else
                signalStrength = 100/30*(rssi+95);

            d->signalStrength = signalStrength;
            d->previousTime = d->timer.elapsed();

            emit signalStrengthChanged(d->signalStrength);
        }
    }
}
