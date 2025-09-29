#include <QDBusMessage>
#include <QDBusReply>
#include <QDBusConnection>
#include <QDebug>

#include "NotificationManager.h"
#include "ReminderManager.h"

QMap<quint32, r3minder::Reminder> r3minder::NotificationManager::sm_reminderMap = {};

r3minder::NotificationManager::NotificationManager(QObject *parent)
{
    QDBusConnection::sessionBus().connect(
        "org.freedesktop.Notifications",
        "/org/freedesktop/Notifications",
        "org.freedesktop.Notifications",
        "ActionInvoked",
        this,
        SLOT(onActionInvoked(quint32, QString))
    );
}

quint32 r3minder::NotificationManager::sendNotification(const Reminder &reminder)
{
    QDBusMessage dbusMsg = QDBusMessage::createMethodCall(
        "org.freedesktop.Notifications",
        "/org/freedesktop/Notifications",
        "org.freedesktop.Notifications",
        "Notify"
        );

    dbusMsg << "r3minder"               // app_name
            << quint32(0)               // replaces_id
            << ""                       // app_icon
            << "r3minder"               // summary
            << reminder.description     // body
            << QStringList{"0", "OK", "1", "+15min", "2", "+1h"} // actions
            << QVariantMap{}            // hints
            << 30 * 1000;               // expire_timeout

    QDBusReply<uint> reply = QDBusConnection::sessionBus().call(dbusMsg);
    if (reply.isValid())
    {
        sm_reminderMap[reply.value()] = reminder;
    }
    else
    {
        qDebug() << "Error:" << reply.error().message();
    }

    return reply.value();
}

void r3minder::NotificationManager::onActionInvoked(quint32 id, const QString &actionKey)
{
    Reminder reminder = sm_reminderMap[id];
    if (!reminder.isValid())
    {
        qDebug() << "Got invalid reminder from reminder map";
        return;
    }

    bool result = false;

    if (actionKey == "1")
    {
        result = ReminderManager::instance()->snoozeReminder(reminder.uuid, 15 * 60);
    }
    else if (actionKey == "2")
    {
        result = ReminderManager::instance()->snoozeReminder(reminder.uuid, 60 * 60);
    }
    else
    {
        qDebug() << "Invalid action key" << actionKey;
        return;
    }

    if (!result)
    {
        qDebug() << "Error while snoozing reminder";
        return;
    }

    sm_reminderMap.remove(id);
}
