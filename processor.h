#ifndef PROCESSOR_H
#define PROCESSOR_H
#include "controller.h"
#include "alu.h"

class Processor : public QObject
{
    Q_OBJECT
signals:
    void setGuiProperty(const QString &name, const QVariant &data);
public:
    Processor(QObject &appMgr);

    /** runs the programm stored in the ram*/
    void runProgram();

    RAM* getRam();

    void toggleAnimations(bool newState);

private:
    Controller* controller;

    bool doAnimations;

    ALU* alu;

    void setCycleState(int state);
    void setDecodedOpc(QString opc);
    void setEffectiveAddress(QString addr);
};

#endif // PROCESSOR_H
