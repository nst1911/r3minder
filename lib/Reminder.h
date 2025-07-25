#pragma once

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QUuid>

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
};

QDebug operator<<(QDebug dbg, const Reminder &reminder);

}

Q_DECLARE_METATYPE(r3minder::Reminder)
