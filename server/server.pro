TEMPLATE = app
QT += widgets network
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += ServerWindow.h \
    user.h
SOURCES += ServerWindow.cpp main.cpp \
    user.cpp
