QT += network websockets

INCLUDEPATH += $$PWD

CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/../Binary/ -lCommunicationd
} else {
    LIBS += -L$$PWD/../Binary/ -lCommunication
}
