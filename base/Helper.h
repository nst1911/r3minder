#pragma once

#include <QString>
#include <QStandardPaths>

namespace Helper
{

const QString appDataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/r3minder";

}
