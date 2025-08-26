#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

#include "ReminderManager.h"
#include "Common.h"

#define RETURN_IF_DB_IS_NOT_VALID(r) \
if (!m_db.isValid()) \
{ \
    qCritical() << "DB is not valid"; \
    return r; \
}

#define RETURN_IF_SQL_QUERY_FAILED(r, msg) \
if (q.lastError().type() != QSqlError::NoError) \
{ \
    qCritical() << msg << ":" << q.lastError(); \
    return r; \
}

namespace
{
const QString c_dbFileName = "r3minder.db";
}

r3minder::ReminderManager::ReminderManager(QObject *parent)
    : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(Common::getAppDataLocation() + "/" + c_dbFileName);

    if (!m_db.open())
    {
        qWarning() << "Failed when open DB";
        return;
    }

    m_db.exec(
        "CREATE TABLE reminders("
        "uuid TEXT UNIQUE,"
        "dateTime TEXT,"
        "description TEXT,"
        "repeatEverySecs INT)"
    );

    connect(this, &ReminderManager::reminderFired, this, &ReminderManager::onReminderFired);
}

QList<r3minder::Reminder> r3minder::ReminderManager::getReminders()
{
    RETURN_IF_DB_IS_NOT_VALID({});

    QSqlQuery q(m_db);
    q.exec("SELECT * FROM reminders");

    RETURN_IF_SQL_QUERY_FAILED({}, "Error while getting reminders from DB")

    QList<Reminder> reminders;
    while (q.next())
    {
        reminders.append(Reminder(
            q.value(1).toDateTime(),
            q.value(3).toInt(),
            q.value(2).toString(),
            q.value(0).toString()
        ));
    }

    return reminders;
}

bool r3minder::ReminderManager::addReminder(const Reminder &reminder)
{
    RETURN_IF_DB_IS_NOT_VALID(false);

    QSqlQuery q(m_db);
    q.prepare(
        "INSERT OR REPLACE INTO reminders"
        "(uuid, dateTime, repeatEverySecs, description)"
        "VALUES"
        "(:uuid, :dateTime, :repeatEverySecs, :description)"
    );
    q.bindValue(":uuid", reminder.uuid);
    q.bindValue(":dateTime", reminder.dateTime);
    q.bindValue(":repeatEverySecs", reminder.repeatEverySecs);
    q.bindValue(":description", reminder.description);
    q.exec();

    RETURN_IF_SQL_QUERY_FAILED(false, "Error while adding reminder to DB")

    return addReminderToSchedule(reminder);
}

bool r3minder::ReminderManager::removeReminder(const QUuid &reminderUuid)
{
    RETURN_IF_DB_IS_NOT_VALID(false);

    QSqlQuery q(m_db);
    q.prepare("DELETE FROM reminders WHERE uuid=(:uuid)");
    q.bindValue(":uuid", reminderUuid);
    q.exec();

    RETURN_IF_SQL_QUERY_FAILED(false, "Error while removing reminder from DB")

    return removeReminderFromSchedule(reminderUuid);
}

bool r3minder::ReminderManager::scheduleReminders()
{
    auto reminders = getReminders();
    if (reminders.isEmpty())
    {
        qWarning() << "No reminders found";
        return true;
    }

    for (auto *timer : m_timers.values())
    {
        timer->deleteLater();
    }
    m_timers.clear();

    bool result = true;

    for (const auto &reminder : reminders)
    {
        if (!addReminderToSchedule(reminder))
        {
            qWarning() << "Can't add reminder" << reminder << "to schedule";
            result = false;
        }
    }

    return result;
}

bool r3minder::ReminderManager::addReminderToSchedule(const Reminder &reminder)
{
    // timer is already activated
    if (m_timers.contains(reminder.uuid))
    {
        qDebug() << "timer for" << reminder << "is already activated";
        return true;
    }

    qint64 delayMs = QDateTime::currentDateTime().msecsTo(reminder.dateTime);
    // target time is in past. fire a reminder right now.
    if (delayMs <= 0)
    {
        emit reminderFired(reminder);
        return true;
    }

    auto *timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(delayMs);

    connect(timer, &QTimer::timeout, this, [reminder, this]() {
        qDebug() << "Reminder fired:" << reminder;
        emit reminderFired(reminder);
    });

    timer->start();

    m_timers.insert(reminder.uuid, timer);

    return true;

}

bool r3minder::ReminderManager::removeReminderFromSchedule(const QUuid &reminderUuid)
{
    if (!m_timers.contains(reminderUuid))
    {
        qWarning() << "There's no" << reminderUuid << "in schedule";
        return false;
    }

    auto *timer = m_timers.value(reminderUuid);
    timer->stop();
    timer->deleteLater();

    m_timers.remove(reminderUuid);

    return true;
}

void r3minder::ReminderManager::onReminderFired(const Reminder &reminder)
{
    removeReminder(reminder.uuid);

    if (reminder.repeatEverySecs > 0)
    {
        addReminder(Reminder(
            QDateTime::currentDateTime().addSecs(reminder.repeatEverySecs),
            reminder.repeatEverySecs,
            reminder.description
        ));
    }
}
