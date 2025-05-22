#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QLocale>
#include <QTranslator>
#include <QDateTime>
#include <QUuid>
#include <QDir>

#include "Helper.h"
#include "ReminderManager.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "r3minder_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    // TODO: Create this dir while installing
    QDir appDataDir(Helper::appDataLocation);
    if (!appDataDir.exists())
    {
        appDataDir.mkpath(Helper::appDataLocation);
    }

    r3minder::ReminderManager rm;

    rm.addReminder(new r3minder::Reminder("test123", QDateTime::currentDateTime().addSecs(30)));

    rm.scheduleReminders();

    return app.exec();
}
