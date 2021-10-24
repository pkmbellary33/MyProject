#include <QCoreApplication>
#include "Application.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Application app;

    return a.exec();
}
