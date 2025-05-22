#pragma once

#include <QString>
#include <QStandardPaths>

namespace r3minder
{

namespace Helper
{

const QString appDataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/r3minder";

}

}
