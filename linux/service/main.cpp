#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QProcess>

#include "Common.h"
#include "ReminderManagerDBusAdaptor.h"
#include "NotificationManager.h"

using namespace r3minder;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // TODO: Create this dir while installing
    QDir appDataDir(Common::getAppDataLocation());
    if (!appDataDir.exists())
    {
        if (!appDataDir.mkpath("."))
        {
            qCritical() << "Failed when creating directory" << appDataDir;
            return 1;
        }
    }

    auto adaptor = new ReminderManagerDBusAdaptor(ReminderManager::instance());
    QObject::connect(
        adaptor,
        &ReminderManagerDBusAdaptor::reminderFired,
        adaptor,
        [](const QString& str) {
            NotificationManager::instance()->sendNotification(Reminder::fromJson(str));
        }
    );

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
