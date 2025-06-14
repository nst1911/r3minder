#pragma once

#include <QString>
#include <QStandardPaths>

namespace r3minder
{

namespace Common
{

const QString appDataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/r3minder";

}

}
