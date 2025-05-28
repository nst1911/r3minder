#pragma once

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include "ReminderManager.h"

namespace r3minder
{

class ReminderManagerDBusAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.github.r3minder")
    Q_CLASSINFO("D-Bus Introspection",
    "  <interface name=\"com.github.r3minder\">\n"
    "    <method name=\"getReminders\">\n"
    "      <arg direction=\"out\" type=\"as\" name=\"reminders\"/>\n"
    "    </method>\n"
    "    <method name=\"addReminder\">\n"
    "      <arg direction=\"in\" type=\"s\" name=\"dateTime\"/>\n"
    "      <arg direction=\"in\" type=\"i\" name=\"repeatEverySecs\"/>\n"
    "      <arg direction=\"in\" type=\"s\" name=\"description\"/>\n"
    "      <arg direction=\"out\" type=\"b\" name=\"result\"/>\n"
    "    </method>\n"
    "    <method name=\"removeReminder\">\n"
    "      <arg direction=\"in\" type=\"s\" name=\"uuid\"/>\n"
    "      <arg direction=\"out\" type=\"b\" name=\"result\"/>\n"
    "    </method>\n"
    "    <method name=\"scheduleReminders\">\n"
    "      <arg direction=\"out\" type=\"b\" name=\"result\"/>\n"
    "    </method>\n"
    "    <signal name=\"reminderFired\">\n"
    "      <arg name=\"reminder\" type=\"s\" direction=\"out\"/>"
    "    </signal>\n"
    "  </interface>\n"
    )

public:
    ReminderManagerDBusAdaptor(r3minder::ReminderManager *mngr);

public slots:
    bool addReminder(const QString &dateTime, int repeatEverySecs, const QString &description);

    QStringList getReminders();
    bool removeReminder(const QString &uuid);
    bool scheduleReminders();

signals:
    void reminderFired(const QString &reminder);

private:
    r3minder::ReminderManager *m_mngr;
};

};
