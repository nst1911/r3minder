#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

#include "Reminder.h"

r3minder::Reminder::Reminder(const QDateTime &dateTime, int repeatEverySecs, const QString &description, const QUuid &uuid)
{
    this->dateTime = dateTime;
    this->repeatEverySecs = repeatEverySecs;
    this->description = description;
    this->uuid = uuid.isNull() ? QUuid::createUuid() : uuid;
}

bool r3minder::Reminder::isValid() const
{
    return !uuid.isNull() && dateTime.isValid();
}

QString r3minder::Reminder::toJson(const Reminder &reminder)
{
    const QJsonObject jsonObj = {
        {"repeatEverySecs", reminder.repeatEverySecs},
        {"dateTime", reminder.dateTime.toString(Qt::ISODate)},
        {"description", reminder.description},
        {"uuid", reminder.uuid.toString()},
    };
    return QJsonDocument(jsonObj).toJson(QJsonDocument::Compact);
}

r3minder::Reminder r3minder::Reminder::fromJson(const QString &str)
{
    QJsonParseError parseErr;
    QJsonObject jsonObj = QJsonDocument::fromJson(str.toUtf8(), &parseErr).object();
    if (parseErr.error != QJsonParseError::NoError)
    {
        qWarning() << "Parsing JSON error:" << parseErr.errorString();
    }

    return Reminder(
        QDateTime::fromString(jsonObj["dateTime"].toString(), Qt::ISODate),
        jsonObj["repeatEverySecs"].toInt(),
        jsonObj["description"].toString(),
        QUuid::fromString(jsonObj["uuid"].toString())
    );
}

bool r3minder::operator==(const r3minder::Reminder& r1, const r3minder::Reminder& r2)
{
    return r1.dateTime == r2.dateTime &&
           r1.description == r2.description &&
           r1.repeatEverySecs == r2.repeatEverySecs &&
           r1.uuid == r2.uuid;
}

QDebug r3minder::operator<<(QDebug dbg, const Reminder &reminder)
{
    dbg << "{" << reminder.uuid << reminder.dateTime << reminder.repeatEverySecs << reminder.description << "}";
    return dbg;
}
