#pragma once

#include <QSqlDatabase>
#include <QMap>
#include <QTimer>

#include "Reminder.h"

namespace r3minder
{

class ReminderManager : public QObject
{
    Q_OBJECT

public:
    ReminderManager(QObject *parent = nullptr);
    virtual ~ReminderManager();

    Q_INVOKABLE QList<Reminder*> getReminders();
    Q_INVOKABLE bool addReminder(const Reminder *reminder);
    Q_INVOKABLE bool removeReminder(const QUuid &reminderUuid);
    Q_INVOKABLE bool scheduleReminders();

protected:
    bool addReminderToSchedule(const Reminder *reminder);
    bool removeReminderFromSchedule(const QUuid &reminderUuid);

signals:
    void reminderFired(const Reminder *reminder);

private:
    QSqlDatabase m_db;
    QMap<QUuid, QTimer*> m_timers;
};

}
