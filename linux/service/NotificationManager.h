#pragma once

#include <QObject>
#include "Reminder.h"

namespace r3minder
{

class NotificationManager : public QObject
{
    Q_OBJECT

public:
    static NotificationManager* instance()
    {
        static auto *instance = new NotificationManager;
        return instance;
    }

    quint32 sendNotification(const Reminder &reminder);

private slots:
    void onActionInvoked(quint32 id, const QString &actionKey);

private:
    NotificationManager(QObject *parent = nullptr);

    static QMap<quint32, Reminder> sm_reminderMap; // key = notification ID
};

}
