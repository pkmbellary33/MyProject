TEMPLATE = subdirs

CONFIG += ordered
QT += location positioning network opengl concurrent sql sensors
SUBDIRS += \
    Communication \
    Application
