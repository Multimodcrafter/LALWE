#include "mainwindow.h"
#include <QApplication>
#include "assembler.h"
#include "processor.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Assembler assembler;
    Processor processor;
    RAM* ram = processor.getRam();
    assembler.assemble("define varA\nlda 15\nmov in acc\nsto varA\ndefine varB\nsto varB\nadd varB\nsto varB",ram);
    processor.runProgram();
    return a.exec();
}
