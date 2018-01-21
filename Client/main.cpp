#include <QApplication>
#include "clientwindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    CLientWindow window;
    window.show();

    return app.exec();
}
