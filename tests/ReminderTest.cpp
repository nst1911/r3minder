#include "Common.h"
#include "Reminder.h"
#include "ReminderManager.h"
#include <QtTest>

// TODO: Divide it to ReminderTest and ReminderManagerTest classes
class ReminderTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Reminder class tests
    void isValid_data();
    void isValid();

    void toJson_data();
    void toJson();

    void fromJson_data();
    void fromJson();

    // ReminderManager tests
    void getReminders();

private:
    r3minder::ReminderManager *reminderMngr();
};

static const QString appDir = "/tmp/r3minder";

void ReminderTest::init()
{
    qDebug() << "init mkdir" << QDir().mkdir(appDir);
}

void ReminderTest::cleanup()
{
    QDir dir(appDir);
    qDebug() << "cleanup removeRecursively" << dir.removeRecursively();
}

void ReminderTest::initTestCase()
{
    qDebug() << "initTestCase qputenv" << qputenv(r3minder::Common::appDataLocationEnvVar, appDir.toLocal8Bit());
}

void ReminderTest::cleanupTestCase()
{
    qDebug() << "cleanupTestCase qputenv" << qputenv(r3minder::Common::appDataLocationEnvVar, QByteArray());
}

void ReminderTest::isValid_data()
{
    QTest::addColumn<QUuid>("uuid");
    QTest::addColumn<QDateTime>("dateTime");
    QTest::addColumn<bool>("expectedIsValid");

    QTest::newRow("Valid") << QUuid("{b517c2d4-65e1-475c-bdba-9aa28a72c955}") << QDateTime(QDate(1996,11,26), QTime(7, 40, 33)) << true;
    QTest::newRow("Invalid uuid") << QUuid() << QDateTime(QDate(1996,11,26), QTime(7, 40, 33)) << false;
    QTest::newRow("Invalid datetime") << QUuid("{b517c2d4-65e1-475c-bdba-9aa28a72c955}") << QDateTime() << false;
}

void ReminderTest::isValid()
{
    // Arrange
    QFETCH(QUuid, uuid);
    QFETCH(QDateTime, dateTime);
    QFETCH(bool, expectedIsValid);
    r3minder::Reminder reminder;
    reminder.uuid = uuid;
    reminder.dateTime = dateTime;

    // Act
    bool isValid = reminder.isValid();

    // Assert
    QCOMPARE(isValid, expectedIsValid);
}

void ReminderTest::toJson_data()
{
    QTest::addColumn<r3minder::Reminder>("reminder");
    QTest::addColumn<QJsonObject>("expectedJsonObj");

    {
        r3minder::Reminder reminder = {
            QDateTime(QDate(1996,11,26), QTime(7, 40, 33)),
            30,
            "test123",
            "b517c2d4-65e1-475c-bdba-9aa28a72c955"
        };

        QJsonObject expectedJsonObj = QJsonDocument::fromJson(QString(R"(
        {
            "dateTime": "1996-11-26T07:40:33",
            "repeatEverySecs": 30,
            "description": "test123",
            "uuid": "{b517c2d4-65e1-475c-bdba-9aa28a72c955}"
        }
        )").toUtf8()).object();

        QTest::newRow("Happy path") << reminder << expectedJsonObj;
    }
}

void ReminderTest::toJson()
{
    // Arrange
    QFETCH(r3minder::Reminder, reminder);
    QFETCH(QJsonObject, expectedJsonObj);

    // Act
    QString actual = r3minder::Reminder::toJson(reminder);

    // Assert
    QCOMPARE(QJsonDocument::fromJson(actual.toUtf8()).object(), expectedJsonObj);
}

void ReminderTest::fromJson_data()
{
    QTest::addColumn<QJsonObject>("jsonObj");
    QTest::addColumn<r3minder::Reminder>("expectedReminder");

    {
        QJsonObject jsonObj = QJsonDocument::fromJson(QString(R"(
        {
            "dateTime": "1996-11-26T07:40:33",
            "repeatEverySecs": 30,
            "description": "test123",
            "uuid": "{b517c2d4-65e1-475c-bdba-9aa28a72c955}"
        }
        )").toUtf8()).object();

        r3minder::Reminder expectedReminder = {
            QDateTime(QDate(1996,11,26), QTime(7, 40, 33)),
            30,
            "test123",
            "{b517c2d4-65e1-475c-bdba-9aa28a72c955}"
        };

        QTest::newRow("Happy path") << jsonObj << expectedReminder;
    }

}

void ReminderTest::fromJson()
{
    // Arrange
    QFETCH(QJsonObject, jsonObj);
    QFETCH(r3minder::Reminder, expectedReminder);
    QString jsonObjStr = QJsonDocument(jsonObj).toJson();

    // Act
    auto reminder = r3minder::Reminder::fromJson(jsonObjStr);

    // Assert
    QCOMPARE(reminder, expectedReminder);
}

void ReminderTest::getReminders()
{
    // Arrange
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QList<r3minder::Reminder> reminders = {
        {
            currentDateTime.addDays(1),
            30,
            "test1",
            "{aaaaaaaa-65e1-475c-bdba-9aa28a72c955}"
        },
        {
            currentDateTime.addDays(2),
            45,
            "test2",
            "{bbbbbbbb-65e1-475c-bdba-9aa28a72c955}"
        },
        {
            currentDateTime.addDays(3),
            60,
            "test3",
            "{cccccccc-65e1-475c-bdba-9aa28a72c955}"
        }
    };
    for (const auto &reminder : reminders)
    {
        reminderMngr()->addReminder(reminder);
    }

    // Act
    auto actualReminders = reminderMngr()->getReminders();

    // Assert
    QCOMPARE(actualReminders, reminders);
}

r3minder::ReminderManager* ReminderTest::reminderMngr()
{
    return r3minder::ReminderManager::instance();
}

QTEST_GUILESS_MAIN(ReminderTest)

#include "ReminderTest.moc"
