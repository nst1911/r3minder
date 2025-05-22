#pragma once

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QUuid>

namespace r3minder
{

class Reminder : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QUuid uuid READ uuid WRITE setUuid NOTIFY uuidChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QDateTime dateTime READ dateTime WRITE setDateTime NOTIFY dateTimeChanged)
    Q_PROPERTY(int repeatEverySecs READ repeatEverySecs WRITE setRepeatEverySecs NOTIFY repeatEverySecsChanged)

public:
    Reminder(const QString &description, const QDateTime &dateTime, int repeatEverySecs = 0, QObject *parent = nullptr);
    Reminder(const QUuid &uuid, const QString &description, const QDateTime &dateTime, int repeatEverySecs = 0, QObject *parent = nullptr);

    QUuid uuid() const;
    void setUuid(const QUuid &uuid);

    QString description() const;
    void setDescription(const QString &description);

    QDateTime dateTime() const;
    void setDateTime(const QDateTime &dateTime);

    // if repeatEverySecs <= 0, it considered as a single-shot reminder
    int repeatEverySecs() const;
    void setRepeatEverySecs(int repeatEverySecs);

    bool isValid() const;

signals:
    void uuidChanged();
    void descriptionChanged();
    void dateTimeChanged();
    void repeatEverySecsChanged();

private:
    QUuid m_uuid;
    QString m_description;
    QDateTime m_dateTime;
    int m_repeatEverySecs = 0;
};

QDebug operator<<(QDebug dbg, const Reminder *reminder);

}
