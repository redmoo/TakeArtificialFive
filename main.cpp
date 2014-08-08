#include "maingui.h"
//#include "core.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Core *core = new Core;
    MainGUI w(core);
    w.show();

    return a.exec();
}
