#include <QDebug>

#include "Reminder.h"

bool r3minder::Reminder::isValid() const
{
    return !uuid.isNull() && !description.isEmpty() && dateTime.isValid();
}

QDebug r3minder::operator<<(QDebug dbg, const Reminder &reminder)
{
    dbg << QString("[uuid=%1 description=%2 dateTime=%3]")
               .arg(reminder.uuid.toString())
               .arg(reminder.description)
               .arg(reminder.dateTime.toString(Qt::ISODate));
    return dbg;
}
