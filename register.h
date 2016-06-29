#ifndef REGISTER_H
#define REGISTER_H
#include "stdint.h"
#define sint int32_t
#include <QObject>
#include <QVariant>

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
    QString name;
    int position;
    void highlightReg(bool active);
};

#endif // REGISTER_H
