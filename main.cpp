#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QObject>
#include "assembler.h"
#include "processor.h"
#include "stdint.h"
#include "logger.h"
#include "lalwe_appmanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    QObject* root = engine.rootObjects().at(0);

    LALWE_AppManager* appMngr = new LALWE_AppManager(engine.rootContext(), root);

    QObject::connect(root ,SIGNAL(assembleProgram(QString)),
                     appMngr, SLOT(assembleSlot(QString)));

    QObject::connect(root ,SIGNAL(saveProgram(QString,QString,int)),
                     appMngr, SLOT(saveProgramSlot(QString,QString,int)));

    QObject::connect(root ,SIGNAL(openProgram(QString)),
                     appMngr, SLOT(openProgramSlot(QString)));

    QObject::connect(root ,SIGNAL(playProgram()),
                     appMngr, SLOT(playProgramSlot()));

    QObject::connect(root ,SIGNAL(toggleAnimations(bool)),
                     appMngr, SLOT(toggleAnimationsSlot(bool)));

    QObject::connect(root ,SIGNAL(toggleRamHex(bool)),
                     appMngr, SLOT(toggleRamHex(bool)));

    QObject::connect(root ,SIGNAL(verifyProgram(QString)),
                     appMngr, SLOT(verifySlot(QString)));

    QObject::connect(root, SIGNAL(animStepForward()),
                     appMngr, SLOT(animStepForward()));

    QObject::connect(root, SIGNAL(windowClosing()),
                     appMngr, SLOT(windowClosing()));

    QObject::connect(root, SIGNAL(setRamViewAddr(int,QString)),
                     appMngr, SLOT(ramViewAddressChanged(int,QString)));

    /*assembler.assemble("function addthree p1 p2 p3 ra\n\
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
    processor->runProgram();*/

    return a.exec();
}
