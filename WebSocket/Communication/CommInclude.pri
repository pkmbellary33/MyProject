QT -= gui
QT += network serialport

INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/Communication.cpp \
    $$PWD/TcpComm.cpp \
    $$PWD/MicroHardRadio.cpp \
    $$PWD/SerialComm.cpp

HEADERS += \
    $$PWD/Communication.h \
    $$PWD/communication_global.h \
    $$PWD/TcpComm.h \
    $$PWD/IComm.h \
    $$PWD/IRadio.h \
    $$PWD/MicroHardRadio.h \
    $$PWD/SerialComm.h
