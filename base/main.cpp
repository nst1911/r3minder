#include <QCoreApplication>
#include <QDir>

#include "Helper.h"
#include "ReminderManager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // TODO: Create this dir while installing
    QDir appDataDir(Helper::appDataLocation);
    if (!appDataDir.exists())
    {
        appDataDir.mkpath(Helper::appDataLocation);
    }

    r3minder::ReminderManager rm;

    rm.addReminder(new r3minder::Reminder("test123", QDateTime::currentDateTime().addSecs(30)));

    rm.scheduleReminders();

    return a.exec();
}
