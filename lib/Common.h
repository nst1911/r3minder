#pragma once

#include <QString>
#include <QStandardPaths>

namespace r3minder
{

namespace Common
{

extern const char *appDataLocationEnvVar;
QString getAppDataLocation();

}

}
