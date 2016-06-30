#ifndef LALWE_APPMANAGER_H
#define LALWE_APPMANAGER_H
#include <QQmlContext>
#include "assembler.h"
#include "processor.h"
#include <QFuture>
#include <QtConcurrent>
#include <iostream>
#include <fstream>

class LALWE_AppManager : public QObject
{
    Q_OBJECT
signals:
    void stepAnimation();

private:
    QQmlContext* ctxt;
    QObject* guiObject;
    Assembler* assembler;
    QList<Processor*> processor;
    RAM* ram;
    bool programRunning;
    bool assemblyRunning;
    QFuture<void> programHandle;
    QFuture<void> assembleHandle;
    QTimer animationTicker;

public slots:
    void assembleSlot(const QString &code);
    void saveProgramSlot(const QString &code, const QString & path, const int &mode);
    void openProgramSlot(const QString &path);
    void playProgramSlot();
    void toggleAnimationsSlot(const bool &newState);
    void toggleRamHex(const bool &newState);
    void verifySlot(const QString &code);
    void setCtxProperty(const QString &name, const QVariant &data);
    void setGuiProperty(const QString &name, const QVariant &data);
    void assemblyDone();
    void animStepForward();
    void windowClosing();
    void ramViewAddressChanged(const int &index, const QString &action);

public:
    LALWE_AppManager(QQmlContext* ctxt, QObject* obj);
};
#endif // LALWE_APPMANAGER_H
