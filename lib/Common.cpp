#include "Common.h"
#include <QDebug>

const char * r3minder::Common::appDataLocationEnvVar = "R3MINDER_APP_DATA_LOCATION";

QString r3minder::Common::getAppDataLocation()
{
    auto appDataLocation = QString::fromLocal8Bit(qgetenv(appDataLocationEnvVar));

    if (appDataLocation.isEmpty())
    {
        appDataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/r3minder";
    }
    else
    {
        qDebug() << appDataLocationEnvVar << "is set to" << appDataLocation;
    }

    return appDataLocation;
}
