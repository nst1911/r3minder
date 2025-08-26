#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QProcess>

#include "Common.h"
#include "ReminderManagerDBusAdaptor.h"

using namespace r3minder;

int sendDesktopNotification(const QString &description)
{
    return QProcess::execute("notify-send", { description });
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
