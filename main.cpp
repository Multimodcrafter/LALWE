#include "mainwindow.h"
#include <QApplication>
#include "assembler.h"
#include "processor.h"
#include "stdint.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Assembler assembler;
    Processor* processor = new Processor();
    RAM* ram = processor->getRam();
    assembler.assemble("function addthree p1 p2 p3 ra\n\
mul 0\n\
add par[p1]\n\
add par[p2]\n\
add par[p3]\n\
sto par[ra]\n\
ret\n\
endfunction\n\
function addfive p1 p2 p3 p4 p5 ra\n\
call addthree par[p1] par[p2] par[p3] par[ra]\n\
ldi par[ra]\n\
call addthree reg[in] par[p4] par[p5] par[ra]\n\
ret\n\
endfunction\n\
entrypoint\n\
ld 130\n\
mov &reg[in] &reg[acc]\n\
add 10\n\
define testresult\n\
sto &dpt[testresult]\n\
define funcresult\n\
call addfive 1 4 5 10 5 &dpt[funcresult]",ram);
    processor->runProgram();
    return a.exec();
}
