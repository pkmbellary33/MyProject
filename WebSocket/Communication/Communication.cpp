#include "Communication.h"

#include "MicroHardRadio.h"
#include "TcpComm.h"
#include "WebComm.h"

#include <QHash>
#include <QPointer>

struct CommunicationData
{
    ~CommunicationData()
    {
        Q_FOREACH(QObject *key, tcpCommHash.keys())
        {
            QObject *value = tcpCommHash.take(key);
            if(value)
                value->deleteLater();
        }

        Q_FOREACH(QObject *key, microRadioHash.keys())
        {
            QObject *value = microRadioHash.take(key);
            if(value)
                value->deleteLater();
        }

        Q_FOREACH(QObject *key, webCommHash.keys())
        {
            QObject *value = webCommHash.take(key);
            if(value)
                value->deleteLater();
        }
    }

    QHash<QObject *, QPointer<TcpComm> >
    tcpCommHash;
    QHash<QObject *, QPointer<MicroHardRadio> >
    microRadioHash;
    QHash<QObject *, QPointer<WebComm> >
    webCommHash;
};

Communication &Communication::instance()
{
    static Communication theInstance;
    return theInstance;
}

Communication::Communication()
{
    d = new CommunicationData;
}

Communication::~Communication()
{
    delete d;
    d = 0;
}

MyProject::IComm *Communication::tcpComm(QObject *key)
{
    TcpComm *tcpComm = d->tcpCommHash.value(key, 0);
    if(!tcpComm)
    {
        tcpComm = new TcpComm(key);

        if(key)
            d->tcpCommHash.insert(key, tcpComm);
    }

    return tcpComm;
}

MyProject::IRadio *Communication::radio(QObject *key)
{
    MicroHardRadio *microRadio = d->microRadioHash.value(key, 0);
    if(!microRadio)
    {
        microRadio =  new MicroHardRadio(key);

        if(key)
            d->microRadioHash.insert(key, microRadio);
    }

    return microRadio;
}

MyProject::IComm *Communication::webComm(QObject *key)
{
    WebComm *webComm = d->webCommHash.value(key, 0);
    if(!webComm)
    {
        webComm = new WebComm(key);

        if(key)
            d->webCommHash.insert(key, webComm);
    }

    return webComm;
}
