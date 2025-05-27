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
    connect(m_mngr, &ReminderManager::reminderFired, this, [this](const Reminder &r) {
        emit reminderFired(QDBusVariant(QVariant::fromValue(r)));
    });
}

bool r3minder::ReminderManagerDBusAdaptor::addReminder(const QDBusVariant &reminder)
{
    return m_mngr->addReminder(reminder.variant().value<Reminder>());
}

QDBusVariant r3minder::ReminderManagerDBusAdaptor::getReminders()
{
    auto reminders = m_mngr->getReminders();
    return QDBusVariant(QVariant::fromValue(reminders));
}

bool r3minder::ReminderManagerDBusAdaptor::removeReminder(const QString &uuid)
{
    return m_mngr->removeReminder(QUuid(uuid));
}

bool r3minder::ReminderManagerDBusAdaptor::scheduleReminders()
{
    return m_mngr->scheduleReminders();
}

