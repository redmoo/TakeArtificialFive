#include "main_gui.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Core *core = new Core;
    MainGUI gui(core);
    gui.show();

    return a.exec();
}
