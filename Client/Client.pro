TEMPLATE = app
QT += widgets network
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += ClientWindow.h \
    user.h
FORMS += ClientWindow.ui
SOURCES += ClientWindow.cpp main.cpp \
    user.cpp
