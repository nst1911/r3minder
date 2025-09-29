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
    static ReminderManager* instance()
    {
        static auto *instance = new ReminderManager;
        return instance;
    }

    Q_INVOKABLE Reminder getReminder(const QUuid &reminderUuid);
    Q_INVOKABLE QList<Reminder> getReminders();

    Q_INVOKABLE bool addReminder(const Reminder &reminder);

    Q_INVOKABLE bool removeReminder(const QUuid &reminderUuid);
    Q_INVOKABLE bool removeReminders();

    Q_INVOKABLE bool snoozeReminder(const QUuid &reminderUuid, const quint64 snoozeTimeSec);

    Q_INVOKABLE bool scheduleReminders();

protected:
    bool addReminderToSchedule(const Reminder &reminder);
    bool removeReminderFromSchedule(const QUuid &reminderUuid);
    bool clearSchedule();

protected slots:
    virtual void onReminderFired(const Reminder &reminder);

signals:
    void reminderFired(const Reminder &reminder);

private:
    ReminderManager(QObject *parent = nullptr);

    QSqlDatabase m_db;
    QMap<QUuid, QTimer*> m_timers;
};

}
