#pragma once

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include "ReminderManager.h"

namespace r3minder
{

class ReminderManagerDBusAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.github.r3minder.api")
    Q_CLASSINFO("D-Bus Introspection",
    "  <interface name=\"com.github.r3minder.api\">\n"
    "    <method name=\"getReminders\">\n"
    "      <arg direction=\"out\" type=\"v\" name=\"reminders\"/>\n"
    "    </method>\n"
    "    <method name=\"addReminder\">\n"
    "      <arg direction=\"in\" type=\"v\" name=\"reminder\"/>\n"
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
    "      <arg type=\"v\" name=\"reminder\"/>\n"
    "    </signal>\n"
    "  </interface>\n"
    )

public:
    ReminderManagerDBusAdaptor(r3minder::ReminderManager *mngr);

public slots:
    bool addReminder(const QDBusVariant &reminder);
    QDBusVariant getReminders();
    bool removeReminder(const QString &uuid);
    bool scheduleReminders();

signals:
    void reminderFired(const QDBusVariant &reminder);

private:
    r3minder::ReminderManager *m_mngr;
};

};
