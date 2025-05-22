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

    Q_INVOKABLE QList<Reminder*> getReminders();
    Q_INVOKABLE bool addReminder(Reminder *reminder);
    Q_INVOKABLE bool removeReminder(const QUuid &reminderUuid);
    Q_INVOKABLE bool scheduleReminders();

protected:
    bool addReminderToSchedule(Reminder *reminder);
    bool removeReminderFromSchedule(const QUuid &reminderUuid);

protected slots:
    virtual void onReminderFired(Reminder *reminder);

signals:
    void reminderFired(Reminder *reminder);

private:
    QSqlDatabase m_db;
    QMap<QUuid, QTimer*> m_timers;
};

}
