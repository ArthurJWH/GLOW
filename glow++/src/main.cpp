#include "glowwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GlowWindow w;

    w.show();
    return a.exec();
}
