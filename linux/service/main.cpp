#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QProcess>
#include <QDBusMessage>

#include "Common.h"
#include "ReminderManagerDBusAdaptor.h"

using namespace r3minder;

int sendDesktopNotification(const QString &description)
{
    QDBusMessage message = QDBusMessage::createMethodCall(
        "org.freedesktop.Notifications",
        "/org/freedesktop/Notifications",
        "org.freedesktop.Notifications",
        "Notify"
    );

    message << "r3minder"    // app_name
            << uint(0)       // replaces_id
            << ""            // app_icon
            << "Reminder"    // summary
            << description   // body
            << QStringList{} // actions
            << QVariantMap{} // hints
            << 30 * 1000;    // expire_timeout

    QDBusReply<uint> reply = QDBusConnection::sessionBus().call(message);
    if (!reply.isValid())
    {
        qDebug() << "Error:" << reply.error().message();
    }

    return reply.value();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // TODO: Create this dir while installing
    QDir appDataDir(Common::getAppDataLocation());
    if (!appDataDir.exists())
    {
        if (appDataDir.mkdir("."))
        {
            qCritical() << "Failed when creating directory" << appDataDir;
            return 1;
        }
    }

    auto adaptor = new ReminderManagerDBusAdaptor(ReminderManager::instance());
    QObject::connect(adaptor, &ReminderManagerDBusAdaptor::reminderFired, adaptor, [](const QString& str) {
        auto description = Reminder::fromJson(str).description;
        sendDesktopNotification(description.isEmpty()
                                    ? "r3minder: Empty notification"
                                    : "r3minder: " + description);
    });

    static const QString serviceName = "com.github.r3minder";

    QDBusConnection connection = QDBusConnection::sessionBus();
    if (!connection.interface()->isServiceRegistered(serviceName))
    {
        if (!connection.registerService(serviceName))
        {
            qWarning() << "Failed to register DBus service:" << connection.lastError().message();
        }
        if (!connection.registerObject("/com/github/r3minder", ReminderManager::instance()))
        {
            qWarning() << "Failed to register DBus object:" << connection.lastError().message();
        }
    }
    else
    {
        qDebug() << "DBus service is already registered";
    }

    r3minder::ReminderManager::instance()->scheduleReminders();

    return a.exec();
}
