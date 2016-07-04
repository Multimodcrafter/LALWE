#ifndef RAM_H
#define RAM_H
#include "stdint.h"
#include <string>
#include <QObject>
#include <QEventLoop>
#define sint int32_t

class RAM : public QObject
{
    Q_OBJECT
signals:
    void setCtxtProperty(const QString &name, const QVariant &data);
    void setGuiProperty(const QString &name, const QVariant &data);
public:
    RAM(QObject &appMgr);

    /** returns the value inside the RAM at the specified location*/
    sint getValueAt(sint addr);

    /** stores the given value inside the RAM at the specified location*/
    void setValueAt(sint addr, sint value);

    /** returns the address range*/
    sint getMaxAddress();

    /** prints out a certain range of the RAM for deugging purposes*/
    void debugRam();

    void toggleHexDisplay(bool newState);

    void toggleAnimations(bool newState);

    void setRamViewAddress(int index, QString action);

    void reset();

private:
    sint* memory;
    sint addressView1;
    sint addressView2;
    bool formatHex;
    bool doAnimations;
    bool continueAnim;
    bool idle;
    bool followActive;

    void updateList();
    void highlightCell(sint addr);
    bool addressInSight(sint addr);

public slots:
    void stepAnimation();
    void toggleFollowActive(const bool &newState);
};

#endif // RAM_H
