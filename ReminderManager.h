#pragma once

#include "Reminder.h"
#include <QSqlDatabase>

namespace r3minder
{

class ReminderManager : public QObject
{
    Q_OBJECT

public:
    ReminderManager(QObject *parent = nullptr);

    Q_INVOKABLE QList<Reminder*> getReminders();
    Q_INVOKABLE bool addReminder(const Reminder *reminder);
    Q_INVOKABLE bool removeReminder(const Reminder *reminder);

private:
    QSqlDatabase m_db;
};

}
