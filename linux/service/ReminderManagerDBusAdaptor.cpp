#include "ReminderManagerDBusAdaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

r3minder::ReminderManagerDBusAdaptor::ReminderManagerDBusAdaptor(ReminderManager *mngr)
    : QDBusAbstractAdaptor(mngr), m_mngr(mngr)
{
    connect(m_mngr, &ReminderManager::reminderFired, this, [this](const Reminder &reminder) {
        emit reminderFired(Reminder::toJson(reminder));
    });
}

bool r3minder::ReminderManagerDBusAdaptor::addReminder(const QString &dateTime, int repeatEverySecs, const QString &description)
{
    qDebug() << "addReminder with args" << dateTime << repeatEverySecs << description << "called via DBus";
    return m_mngr->addReminder(Reminder(QDateTime::fromString(dateTime, Qt::ISODate), repeatEverySecs, description));
}

QStringList r3minder::ReminderManagerDBusAdaptor::getReminders()
{
    qDebug() << "getReminders called via DBus";

    QStringList strList;
    for (const auto &reminder : m_mngr->getReminders())
    {
        strList.append(Reminder::toJson(reminder));
    }
    return strList;
}

bool r3minder::ReminderManagerDBusAdaptor::removeReminder(const QString &uuid)
{
    qDebug() << "removeReminder with arg" << uuid << "called via DBus";
    return m_mngr->removeReminder(QUuid(uuid));
}

bool r3minder::ReminderManagerDBusAdaptor::scheduleReminders()
{
    qDebug() << "scheduleReminders called via DBus";
    return m_mngr->scheduleReminders();
}
