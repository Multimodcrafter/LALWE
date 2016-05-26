#include "mainwindow.h"
#include <QApplication>
#include "assembler.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Assembler assembler;
    RAM ram;
    assembler.assemble("define varA\ndefine varB\nhlt varB\nfunction test par1\nhlt par1\ndefine someVar\nhlt someVar\nendfunction\nentrypoint\ncall test 0",ram);
    ram.debugRam();
    return a.exec();
}
