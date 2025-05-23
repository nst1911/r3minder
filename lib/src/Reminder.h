#pragma once

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QUuid>

namespace r3minder
{

struct Reminder
{
    QUuid uuid;
    QString description;
    QDateTime dateTime;
    int repeatEverySecs = 0;

    bool isValid() const;
};

QDebug operator<<(QDebug dbg, const Reminder &reminder);

}
