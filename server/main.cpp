#include <QApplication>
#include "serverwindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    ServerWindow window;
    window.show();

    return app.exec();
}
