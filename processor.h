#ifndef PROCESSOR_H
#define PROCESSOR_H
#include "controller.h"
#include "alu.h"

class Processor : public QObject
{
    Q_OBJECT
signals:
    void setGuiProperty(const QString &name, const QVariant &data);
    void printLine(const QString &line);
public:
    Processor(QObject &appMgr);

    /** runs the programm stored in the ram*/
    void runProgram();

    RAM* getRam();

    void toggleAnimations(bool newState);

    void reset();

    void sendInput(sint value);

    void requestTermination();

private:
    Controller* controller;

    bool doAnimations;
    bool continueAnim;
    bool idle;
    bool waitForInput;
    bool sigTerm;
    sint inputValue;

    ALU* alu;

    void setCycleState(int state);
    void setDecodedOpc(QString opc);
    void setEffectiveAddress(QString addr);
public slots:
    void stepAnimation();
};

#endif // PROCESSOR_H
