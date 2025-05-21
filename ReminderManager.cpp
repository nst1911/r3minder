#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

#include "ReminderManager.h"
#include "Helper.h"

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
    m_db.setDatabaseName(Helper::appDataLocation + "/" + c_dbFileName);

    if (!m_db.open())
    {
        qWarning() << "Failed when open DB";
        return;
    }

    m_db.exec(
        "CREATE TABLE reminders("
        "uuid TEXT UNIQUE,"
        "description TEXT,"
        "dateTime TEXT)"
    );
}

QList<r3minder::Reminder*> r3minder::ReminderManager::getReminders()
{
    RETURN_IF_DB_IS_NOT_VALID({});

    QSqlQuery q(m_db);
    q.exec("SELECT * FROM reminders");

    RETURN_IF_SQL_QUERY_FAILED({}, "Error while getting reminders from DB")

    QList<Reminder*> reminders;
    while (q.next())
    {
        QUuid uuid = q.value(0).toUuid();
        QString description = q.value(1).toString();
        QDateTime dateTime = q.value(2).toDateTime();
        reminders.append(new Reminder(uuid, description, dateTime, this));
    }

    return reminders;
}

bool r3minder::ReminderManager::addReminder(const Reminder *reminder)
{
    RETURN_IF_DB_IS_NOT_VALID(false);

    QSqlQuery q(m_db);
    q.prepare(
        "INSERT OR REPLACE INTO reminders"
        "(uuid, description, dateTime)"
        "VALUES"
        "(:uuid, :description, :dateTime)"
    );
    q.bindValue(":uuid", reminder->uuid());
    q.bindValue(":description", reminder->description());
    q.bindValue(":dateTime", reminder->dateTime());
    q.exec();

    RETURN_IF_SQL_QUERY_FAILED(false, "Error while adding reminder to DB")

    return true;
}

bool r3minder::ReminderManager::removeReminder(const Reminder *reminder)
{
    RETURN_IF_DB_IS_NOT_VALID(false);

    QSqlQuery q(m_db);
    q.prepare("DELETE FROM reminders WHERE uuid=(:uuid)");
    q.bindValue(":uuid", reminder->uuid());
    q.exec();

    RETURN_IF_SQL_QUERY_FAILED(false, "Error while removing reminder from DB")

    return true;
}
