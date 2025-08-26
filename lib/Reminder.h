#pragma once

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QUuid>
#include <QDBusArgument>

namespace r3minder
{

struct Reminder
{
    Reminder() = default;
    Reminder(const QDateTime &dateTime, int repeatEverySecs = 0, const QString &description = QString(),  const QUuid &uuid = {});

    QUuid uuid;
    QDateTime dateTime;
    QString description;
    int repeatEverySecs = 0;

    bool isValid() const;

    static QString toJson(const Reminder &reminder);
    static Reminder fromJson(const QString &str);

    friend bool operator==(const Reminder& r1, const Reminder& r2);
};

bool operator==(const Reminder& r1, const Reminder& r2);
QDebug operator<<(QDebug dbg, const Reminder &reminder);

}

Q_DECLARE_METATYPE(r3minder::Reminder)
