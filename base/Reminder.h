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

public:
    Reminder(const QString &description, const QDateTime &dateTime, QObject *parent = nullptr);
    Reminder(const QUuid &uuid, const QString &description, const QDateTime &dateTime, QObject *parent = nullptr);

    QUuid uuid() const;
    void setUuid(const QUuid &uuid);

    QString description() const;
    void setDescription(const QString &description);

    QDateTime dateTime() const;
    void setDateTime(const QDateTime &dateTime);

    bool isValid() const;

signals:
    void uuidChanged();
    void descriptionChanged();
    void dateTimeChanged();

private:
    QUuid m_uuid;
    QString m_description;
    QDateTime m_dateTime;
};

QDebug operator<<(QDebug dbg, const Reminder *reminder);

}
