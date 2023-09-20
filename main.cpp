#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
//    w.setPixel(10, 20, true);
//    w.setPixel(50, 30, true);

    return a.exec();
}
