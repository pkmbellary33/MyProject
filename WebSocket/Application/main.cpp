#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "WSClient.h"
#include <qtwebengineglobal.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QtWebEngine::initialize();
    QGuiApplication app(argc, argv);
    WSClient wsclient;
    wsclient.connectWebSocketServer();

    qmlRegisterType<WSClient>("WSClient", 1, 0, "WSClient");

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("wsClient", &wsclient);

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}
