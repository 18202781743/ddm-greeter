// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QGuiApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QQmlApplicationEngine>
#include <DLog>
#include <QQuickWindow>
#include <QWindow>

#include <iostream>

#include <applet.h>
#include <containment.h>
#include <pluginloader.h>
#include <appletloader.h>
#include <qmlengine.h>
#include <QDir>
DS_USE_NAMESPACE

DS_BEGIN_NAMESPACE
Q_DECLARE_LOGGING_CATEGORY(dsLog)
DS_END_NAMESPACE

void outputPluginTreeStruct(const DPluginMetaData &plugin, int level)
{
    const QString separator(level * 4, ' ');
    std::cout << qPrintable(separator + plugin.pluginId()) << std::endl;
    for (const auto &item : DPluginLoader::instance()->childrenPlugin(plugin.pluginId())) {
        outputPluginTreeStruct(item, level + 1);
    }
}

// disable log's output in some case.
static void disableLogOutput()
{
    QLoggingCategory::setFilterRules("*.debug=false");
}

inline QString buildingDir(const QString &subdir)
{
    QDir dir(QCoreApplication::applicationDirPath());
    if (dir.exists() && dir.exists(subdir))
        return dir.absoluteFilePath(subdir);

    return QString();
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app.setOrganizationName("deepin");
    app.setApplicationName("org.deepin.ddm-greeter");
    app.setApplicationVersion(QT_STRINGIFY(DS_VERSION));

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption socketOpaction(QStringList{ "s", "socket" },
                                                        "set ddm socket",
                                                        "socket");
    parser.addOption(socketOpaction);
    QCommandLineOption panelOption("p", "collections of panel.", "panel", QString());
    parser.addOption(panelOption);
    QCommandLineOption categoryOption("C", "collections of root panels by category.", "category", QString());
    parser.addOption(categoryOption);
    QCommandLineOption testOption(QStringList() << "t" << "test", "application test.");
    parser.addOption(testOption);
    QCommandLineOption disableAppletOption("d", "disabled applet.", "disable-applet", QString());
    parser.addOption(disableAppletOption);
    QCommandLineOption listOption("list", "List all applets.", QString());
    parser.addOption(listOption);
    QCommandLineOption sceneviewOption("sceneview", "View applets in scene, it only works without Window.", QString());
    parser.addOption(sceneviewOption);
    QCommandLineOption dbusServiceNameOption("serviceName", "Registed DBus service for the serviceName, if it's not empty.", "serviceName", QString("org.deepin.dde.shell"));
    parser.addOption(dbusServiceNameOption);

    parser.process(app);

    DPluginLoader::instance()->addPackageDir(buildingDir("packages"));
    DPluginLoader::instance()->addPluginDir(buildingDir("plugins"));

    if (parser.isSet(listOption)) {
        disableLogOutput();
        for (const auto &item : DPluginLoader::instance()->rootPlugins()) {
            outputPluginTreeStruct(item, 0);
        }
        return 0;
    }

    Dtk::Core::DLogManager::registerConsoleAppender();
    Dtk::Core::DLogManager::registerJournalAppender();
    Dtk::Core::DLogManager::registerFileAppender();

    QList<QString> pluginIds {"org.deepin.ds.greeter"};
    if (parser.isSet(panelOption)) {
        pluginIds << parser.values(panelOption);
    } else if (parser.isSet(categoryOption)) {
        const QList<QString> &categories = parser.values(categoryOption);
        for (const auto &item : DPluginLoader::instance()->rootPlugins()) {
            const auto catetroy = item.value("Category").toString();
            if (catetroy.isEmpty())
                continue;
            if (categories.contains(catetroy)) {
                pluginIds << item.pluginId();
            }
        }
    }
    if (parser.isSet(disableAppletOption)) {
        const auto disabledApplets = parser.values(disableAppletOption);
        DPluginLoader::instance()->setDisabledApplets(disabledApplets);
        pluginIds.removeIf([disabledApplets] (const QString &item) {
            return disabledApplets.contains(item);
        });
    }

    DQmlEngine().engine()->addImportPath(buildingDir("plugins"));

    AppletManager manager(pluginIds);

    QMetaObject::invokeMethod(&app, [&manager](){
        manager.exec();
    }, Qt::QueuedConnection);

    QObject::connect(qApp, &QCoreApplication::aboutToQuit, qApp, [&manager]() {
        qCInfo(dsLog) << "Exit ddm-greeter.";
        DPluginLoader::instance()->destroy();
        manager.quit();
    });

    return app.exec();
}
