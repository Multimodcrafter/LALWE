#ifndef ALU_H
#define ALU_H
#include "controller.h"
#include <QObject>
#include <QVariant>
#include <QString>


class ALU : public QObject
{
    Q_OBJECT
signals:
    void setGuiProperty(const QString &name, const QVariant &data);
public:
    ALU(Controller* cont, QObject &appMgr);

    void add(sint value);

    void subtract(sint value);

    void multiply(sint value);

    void divide(sint value);

    void And(sint value);

    void Or(sint value);

    void Xor(sint value);

    void rotL(sint value);

    void rotR(sint value);

    void shiL(sint value);

    void shiR(sint value);

    void comp(sint value);

    void toggleAnimations(bool newState);
private:
    Controller* controller;

    bool doAnimations;

    void setOperator(std::string op);
    void setArg(sint);
    void setResult(sint);

    sint countBits(sint value);
};

#endif // ALU_H
