#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "communication_global.h"

class QObject;

namespace Asteria
{

class IComm;
class IRadio;

}

struct CommunicationData;
class COMMUNICATIONSHARED_EXPORT Communication
{
public:
    static Communication &instance();
    ~Communication();

    Asteria::IComm *tcpComm(QObject *key = 0);
    Asteria::IRadio *radio(QObject *key = 0);
    Asteria::IComm *webComm(QObject *key = 0);

protected:
    Communication();

private:
    CommunicationData *d;
};

#endif // COMMUNICATION_H
