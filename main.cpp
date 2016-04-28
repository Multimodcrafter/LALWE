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
    assembler.assemble("Line One\nfunction tEs_t8 hello\nLine Three",ram);
    return a.exec();
}
