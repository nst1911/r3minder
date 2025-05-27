#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QDBusMetaType>
#include <QDBusConnection>

#include "Common.h"
#include "ReminderManager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // TODO: Create this dir while installing
    QDir appDataDir(r3minder::Common::appDataLocation);
    if (!appDataDir.exists())
    {
        if (appDataDir.mkdir("."))
        {
            qCritical() << "Failed when creating directory" << r3minder::Common::appDataLocation;
            return 1;
        }
    }

    r3minder::ReminderManager::instance()->scheduleReminders();

    return a.exec();
}
