#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "communication_global.h"

class QObject;

namespace MyProject
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

    MyProject::IComm *tcpComm(QObject *key = 0);
    MyProject::IComm *webComm(QObject *key = 0);

protected:
    Communication();

private:
    CommunicationData *d;
};

#endif // COMMUNICATION_H
