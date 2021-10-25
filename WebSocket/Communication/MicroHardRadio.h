#ifndef MICROHARDRADIO_H
#define MICROHARDRADIO_H

#include "IRadio.h"

struct MicroHardRadioData;
class MicroHardRadio :
        public QObject,
        virtual public MyProject::IRadio
{
    Q_OBJECT
    Q_INTERFACES(MyProject::IRadio)
    Q_PROPERTY(int signalStrength READ signalStrength
               NOTIFY signalStrengthChanged)

public:
    MicroHardRadio(QObject *parent = 0);
    ~MicroHardRadio();

    // MyProject::IRadio Implementation
    QObject* containerObject();

    void setUserName(const QString& name);
    QString userName() const;

    void setPassword(const QString& password);
    QString password() const;

    void setAddress(const QString& address);
    QString address() const;

    void setRadioType(int type);
    int radioType();

    int signalStrength() const;

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();

signals:
    void userNameChanged(const QString& name);
    void passwordChanged(const QString& password);
    void addressChanged(const QString& ipAddress);
    void radioTypeChanged(int type);
    void signalStrengthChanged(int signalStrength);

private slots:
    void updateSignalStrength(const QByteArray &data);

private:
    MicroHardRadioData *d;
};

#endif // MICROHARDRADIO_H
