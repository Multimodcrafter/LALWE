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
    engine.rootContext()->setContextProperty("APP_VERSION",APP_VERSION);
    engine.rootContext()->setContextProperty("myModel1",QVariant::fromValue(QStringList()));
    engine.rootContext()->setContextProperty("myModel2",QVariant::fromValue(QStringList()));
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

    QObject::connect(root, SIGNAL(changeAnimSpeed(double)),
                     appMngr, SLOT(changeAnimSpeed(double)));

    QObject::connect(root, SIGNAL(resetSimulation()),
                     appMngr, SLOT(resetProcessor()));

    QObject::connect(root, SIGNAL(sendInput(QString)),
                     appMngr, SLOT(getInput(QString)));

    return a.exec();
}
