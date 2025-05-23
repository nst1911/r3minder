#include <QCoreApplication>
#include <QDir>
#include <QDebug>

#include "Common.h"
#include "ReminderManager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // TODO: Create this dir while installing
    if (QDir(r3minder::Common::appDataLocation).mkpath("."))
    {
        qCritical() << "Failed when creating directory" << r3minder::Common::appDataLocation;
    }

    auto *reminderMngr = new r3minder::ReminderManager;
    reminderMngr->scheduleReminders();

    return a.exec();
}
