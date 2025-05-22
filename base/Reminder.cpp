#include <QDebug>

#include "Reminder.h"

r3minder::Reminder::Reminder(const QString &description, const QDateTime &dateTime, int repeatEverySecs, QObject *parent)
    : Reminder(QUuid::createUuid(), description, dateTime, repeatEverySecs, parent)
{

}

r3minder::Reminder::Reminder(const QUuid &uuid, const QString &description, const QDateTime &dateTime, int repeatEverySecs, QObject *parent)
    : QObject(parent), m_uuid(uuid), m_description(description), m_dateTime(dateTime), m_repeatEverySecs(repeatEverySecs)
{

}

QUuid r3minder::Reminder::uuid() const
{
    return m_uuid;
}

void r3minder::Reminder::setUuid(const QUuid &uuid)
{
    m_uuid = uuid;
}

QString r3minder::Reminder::description() const
{
    return m_description;
}

void r3minder::Reminder::setDescription(const QString &description)
{
    if (description != m_description)
    {
        m_description = description;
        emit descriptionChanged();
    }
}

QDateTime r3minder::Reminder::dateTime() const
{
    return m_dateTime;
}

void r3minder::Reminder::setDateTime(const QDateTime &dateTime)
{
    if (dateTime != m_dateTime)
    {
        m_dateTime = dateTime;
        emit dateTimeChanged();
    }
}

int r3minder::Reminder::repeatEverySecs() const
{
    return m_repeatEverySecs;
}

void r3minder::Reminder::setRepeatEverySecs(int repeatEverySecs)
{
    if (repeatEverySecs != m_repeatEverySecs)
    {
        m_repeatEverySecs = repeatEverySecs;
        emit repeatEverySecsChanged();
    }
}

bool r3minder::Reminder::isValid() const
{
    return !m_uuid.isNull() && !m_description.isEmpty() && m_dateTime.isValid();
}

QDebug r3minder::operator<<(QDebug dbg, const Reminder *reminder)
{
    dbg << QString("[uuid=%1 description=%2 dateTime=%3]")
               .arg(reminder->uuid().toString())
               .arg(reminder->description())
               .arg(reminder->dateTime().toString(Qt::ISODate));
    return dbg;
}
