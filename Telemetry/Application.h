#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>

struct ApplicationData;
class Application : public QObject
{
    Q_OBJECT

public:
    Application(QObject *parent = 0);
    ~Application();

protected:
    void timerEvent(QTimerEvent *event);

private slots:
    void onMessageReceived(const QString &message);

private:
    ApplicationData *d;
};

#endif // APPLICATION_H
