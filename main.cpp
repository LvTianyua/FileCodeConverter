#include "MainWidget.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWidget w;
    w.show();
    return a.exec();
}
