#ifndef REGISTER_H
#define REGISTER_H
#include "stdint.h"
#define sint int32_t
#include <QObject>
#include <QVariant>
#include <QEventLoop>

class Register : public QObject
{
    Q_OBJECT
signals:
    void setGuiProperty(const QString &name, const QVariant &data);
public:
    Register(QObject &appMgr, QString &name, int pos);

    sint getValue();

    void setValue(sint);

    void toggleAnimations(bool newState);

private:
    sint value;
    bool doAnimations;
    bool continueAnim;
    bool idle;

    QString name;
    int position;
    void highlightReg(bool active);

public slots:
    void stepAnimation();
};

#endif // REGISTER_H
