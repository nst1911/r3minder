#include <QCoreApplication>
#include <QDir>
#include <QDebug>

#include "Common.h"
#include "ReminderManager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // TODO: Create this dir while installing
    QDir appDataDir(r3minder::Common::appDataLocation);
    if (!appDataDir.exists())
    {
        appDataDir.mkpath(r3minder::Common::appDataLocation);
    }

    r3minder::ReminderManager rm;

//    rm.addReminder(new r3minder::Reminder("test1", QDateTime::currentDateTime().addSecs(10), 10));

    qDebug() << "test123";

    return a.exec();
}
