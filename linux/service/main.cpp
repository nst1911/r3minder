#include <QCoreApplication>
#include <QDir>
#include <QDebug>

#include "Common.h"
#include "ReminderManagerDBusAdaptor.h"

using namespace r3minder;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // TODO: Create this dir while installing
    QDir appDataDir(Common::appDataLocation);
    if (!appDataDir.exists())
    {
        if (appDataDir.mkdir("."))
        {
            qCritical() << "Failed when creating directory" << Common::appDataLocation;
            return 1;
        }
    }

    qDebug() << QFile::remove(r3minder::Common::appDataLocation + "/r3minder.db");

    // DBus registration

    new ReminderManagerDBusAdaptor(ReminderManager::instance());

    QDBusConnection connection = QDBusConnection::sessionBus();

    static const QString serviceName = "com.github.r3minder";

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



//    r3minder::ReminderManager::instance()->scheduleReminders();

    return a.exec();
}
