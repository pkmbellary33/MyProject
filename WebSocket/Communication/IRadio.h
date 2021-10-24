#ifndef IRADIO_H
#define IRADIO_H

#include <QtPlugin>

namespace Asteria
{

class IRadio
{
public:
    enum RadioType {
        PicoDDL,
        NanoDDL
    };

    virtual QObject* containerObject() = 0;

    virtual void setUserName(const QString& name) = 0;
    virtual QString userName() const = 0;

    virtual void setPassword(const QString& password) = 0;
    virtual QString password() const = 0;

    virtual void setAddress(const QString& address) = 0;
    virtual QString address() const = 0;

    virtual void setRadioType(int type) = 0;
    virtual int radioType() = 0;

    virtual int signalStrength() const = 0;

    virtual void start() = 0;
    virtual void stop() = 0;

    // must be implemented as signals
    virtual void userNameChanged(const QString& name) = 0;
    virtual void passwordChanged(const QString& password) = 0;
    virtual void addressChanged(const QString& ipAddress) = 0;
    virtual void radioTypeChanged(int type) = 0;
    virtual void signalStrengthChanged(int signalStrength) = 0;
};

}

Q_DECLARE_INTERFACE(Asteria::IRadio, "com.asteria.IRadio/1.0")

#endif // IRADIO_H
