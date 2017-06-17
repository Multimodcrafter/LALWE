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

    void cancelTermination();

private:
    Controller* controller;

    bool doAnimations;
    bool continueAnim;
    bool idle;
    bool waitForInput;
    bool sigTerm;
    bool animationsPlaying;
    sint inputValue;

    ALU* alu;

    void setCycleState(int state);
    void setDecodedOpc(QString opc);
    void setEffectiveAddress(QString addr);
public slots:
    void stepAnimation();
    void togglePlayState(bool newState);
};

#endif // PROCESSOR_H
