// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "appletloader.h"
#include "pluginloader.h"
#include "applet.h"
#include "containment.h"
#include "appletdata.h"
#include "qmlengine.h"

#include <dobject_p.h>

#include <QMap>
#include <QLoggingCategory>
#include <QElapsedTimer>
#include <QTranslator>
#include <QCoreApplication>
#include <QFile>
#include <DWindowManagerHelper>

DS_BEGIN_NAMESPACE
DCORE_USE_NAMESPACE;
DGUI_USE_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(dsLog)
Q_LOGGING_CATEGORY(dsLoaderLog, "dde.shell.loader")

namespace {
    class Benchmark
    {
    public:
        explicit Benchmark(const QString &text)
            : m_text(text)
        {
            m_timer.start();
        }
        ~Benchmark()
        {
            const auto elasped = m_timer.elapsed();
            if (elasped >= m_timeout) {
                qCWarning(dsLoaderLog) << qPrintable(m_text) << ": elasped time [" << elasped << "].";
            }
        }
    private:
        QString m_text;
        int m_timeout = 100;
        QElapsedTimer m_timer;
    };
}

class DAppletLoaderPrivate : public DObjectPrivate
{
public:
    explicit DAppletLoaderPrivate(DAppletLoader *qq)
        : DTK_CORE_NAMESPACE::DObjectPrivate(qq)
    {
    }
    QList<DAppletData> groupList(DApplet *applet, const DAppletData &data) const
    {
        if (!data.groupList().isEmpty())
            return data.groupList();

        QList<DAppletData> groups;
        auto children = DPluginLoader::instance()->childrenPlugin(applet->pluginMetaData().pluginId());
        std::sort(children.begin(), children.end(), [] (const DPluginMetaData &item1, const DPluginMetaData &item2) {
            return item1.value("Order", 0).toInt() > item2.value("Order", 0).toInt();
        });
        for (const auto &item : children) {
            groups << DAppletData::fromPluginMetaData(item);
        }
        return groups;
    }

    ~DAppletLoaderPrivate()
    {
        for (const auto &tl : std::as_const(m_pluginTranslators)) {
            tl->deleteLater();
        }
    }

    bool doLoad(DApplet *applet);
    void doCreateRootObject(DApplet *applet);
    bool doInit(DApplet *applet);

    bool load(DApplet *applet);
    void createRootObject(DApplet *applet);
    bool init(DApplet *applet);

    void createChildren(DApplet *applet);

    void loadTranslation(const DPluginMetaData &pluginData);
    void removeTranslation(const QString &pluginId);

    QPointer<DApplet> m_applet = nullptr;
    QMap<QString, QTranslator *> m_pluginTranslators;
    QString m_locale;

    D_DECLARE_PUBLIC(DAppletLoader);
};

DAppletLoader::DAppletLoader(class DApplet *applet, QObject *parent)
    : QObject(parent)
    , DObject(*new DAppletLoaderPrivate(this))
{
    D_D(DAppletLoader);
    d->m_applet = applet;
    d->m_locale = QLocale::system().name();
}

DAppletLoader::~DAppletLoader()
{

}

void DAppletLoader::exec()
{
    D_D(DAppletLoader);
    d->loadTranslation(d->m_applet->pluginMetaData());

    if (!d->load(d->m_applet))
        return;

    d->createRootObject(d->m_applet);

    if (!d->init(d->m_applet))
        return;
}

DApplet *DAppletLoader::applet() const
{
    D_DC(DAppletLoader);
    return d->m_applet;
}

void DAppletLoader::setLocale(const QString &locale)
{
    D_D(DAppletLoader);
    d->m_locale = locale;
}

void DAppletLoader::reloadTranslations()
{
    D_D(DAppletLoader);
    if (!d->m_applet)
        return;

    d->removeTranslation(d->m_applet->pluginId());

    d->loadTranslation(d->m_applet->pluginMetaData());
}

void DAppletLoaderPrivate::doCreateRootObject(DApplet *applet)
{
    if (applet->pluginMetaData().url().isEmpty())
        return;

    DQmlEngine *engine = new DQmlEngine(applet, applet);
    QObject::connect(engine, &DQmlEngine::createFinished, applet, [this, applet, engine]() {
        auto rootObject = engine->rootObject();
        applet->setRootObject(rootObject);
        engine->completeCreate();
        if (!rootObject) {
            D_Q(DAppletLoader);
            qCWarning(dsLoaderLog) << "Create root failed:" << applet->pluginId();
            Q_EMIT q->failed(applet->pluginId());
        } else {
            qCDebug(dsLoaderLog) << "Created rootObject for the applet:" << applet->pluginId();
        }
    });

    qCDebug(dsLoaderLog) << "Begin to create rootObject the applet:" << applet->pluginId();
    if (!engine->create()) {
        engine->deleteLater();
    }
}

bool DAppletLoaderPrivate::doLoad(DApplet *applet)
{
    D_Q(DAppletLoader);
    Benchmark benchmark(QString("Load applet %1").arg(applet->pluginId()));
    Q_UNUSED(benchmark);
    if (!applet->load()) {
        qCWarning(dsLoaderLog) << "Plugin load failed:" << applet->pluginId();
        if (auto containment = qobject_cast<DContainment *>(applet->parentApplet())) {
            containment->removeApplet(applet);
        }
        Q_EMIT q->failed(applet->pluginId());
        return false;
    }
    return true;
}

bool DAppletLoaderPrivate::doInit(DApplet *applet)
{
    D_Q(DAppletLoader);
    Benchmark benchmark(QString("Init applet %1").arg(applet->pluginId()));
    Q_UNUSED(benchmark);
    if (!applet->init()) {
        qCWarning(dsLoaderLog) << "Plugin init failed:" << applet->pluginId();
        if (auto containment = qobject_cast<DContainment *>(applet->parentApplet())) {
            containment->removeApplet(applet);
        }
        Q_EMIT q->failed(applet->pluginId());
        return false;
    }
    return true;
}

void DAppletLoaderPrivate::createChildren(DApplet *applet)
{
    if (auto containment = qobject_cast<DContainment *>(applet)) {
        const auto data = applet->appletData();
        auto groups = groupList(applet, data);
        for (const auto &item : std::as_const(groups)) {

            auto child = containment->createApplet(item);
            if (!child) {
                continue;
            }
        }
    }
}

bool DAppletLoaderPrivate::load(DApplet *applet)
{
    if (!doLoad(applet)) {
        return false;
    }

    createChildren(applet);

    if (auto containment = qobject_cast<DContainment *>(applet)) {
        auto applets = containment->applets();
        for (const auto &child : std::as_const(applets)) {

            load(child);
        }
    }
    return true;
}

void DAppletLoaderPrivate::createRootObject(DApplet *applet)
{
    doCreateRootObject(applet);
    if (auto containment = qobject_cast<DContainment *>(applet)) {
        auto applets = containment->applets();
        for (const auto &child : std::as_const(applets)) {

            createRootObject(child);
        }
    }
}

bool DAppletLoaderPrivate::init(DApplet *applet)
{
    if (!doInit(applet))
        return false;

    if (auto containment = qobject_cast<DContainment *>(applet)) {
        QList<DApplet *> applets = containment->applets();
        for (const auto &child : std::as_const(applets)) {
            init(child);
        }
    }
    return true;
}

void DAppletLoaderPrivate::loadTranslation(const DPluginMetaData &pluginData)
{
    const QString baseDir = pluginData.pluginDir();
    const QString pluginId = pluginData.pluginId();

    QString pluginTransFilePath = QString(baseDir + "/translations/" + pluginId + "_%1.qm").arg(m_locale);
    if (QFile::exists(pluginTransFilePath)) {
        auto translator = new QTranslator(qApp);
        if (translator->load(pluginTransFilePath)) {
            m_pluginTranslators[pluginId] = translator;
            qApp->installTranslator(translator);
            qInfo(dsLoaderLog) << "Loaded translation:" << pluginTransFilePath;
        } else {
            qCWarning(dsLoaderLog) << "Failed to load translation:" << pluginTransFilePath;
            translator->deleteLater();
        }
    }

    const auto children = DPluginLoader::instance()->childrenPlugin(pluginId);
    for (const auto &childPluginData : children) {
        loadTranslation(childPluginData);
    }
}

void DAppletLoaderPrivate::removeTranslation(const QString &pluginId)
{
    if (m_pluginTranslators.contains(pluginId)) {
        qApp->removeTranslator(m_pluginTranslators.value(pluginId));
        m_pluginTranslators.value(pluginId)->deleteLater();
        m_pluginTranslators.remove(pluginId);
    }
}

AppletManager::AppletManager(const QStringList &pluginIds)
{
    qCDebug(dsLog) << "Preloading plugins:" << pluginIds;
    auto rootApplet = qobject_cast<DContainment *>(DPluginLoader::instance()->rootApplet());
    Q_ASSERT(rootApplet);

    for (const auto &pluginId : pluginIds) {
        auto applet = rootApplet->createApplet(DAppletData{pluginId});
        if (!applet) {
            qCWarning(dsLog) << "Loading plugin failed:" << pluginId;
            continue;
        }

        auto loader = new DAppletLoader(applet);
        m_loaders << loader;

        QObject::connect(loader, &DAppletLoader::failed, qApp, [this, loader, pluginIds](const QString &pluginId) {
            if (pluginIds.contains(pluginId)) {
                m_loaders.removeOne(loader);
                loader->deleteLater();
            }
        });
    }
}

void AppletManager::exec()
{
    for (auto loader : std::as_const(m_loaders)) {
        loader->exec();
    }

    for (auto loader : std::as_const(m_loaders)) {
        auto applet = loader->applet();
        if (!applet)
            continue;
        const auto localeSingle = QMetaObject::normalizedSignature("localeChanged(const QString &)");
        if (applet->metaObject()->indexOfSignal(localeSingle) >= 0) {
            qCDebug(dsLog) << "Regist the response of localeChanged for the applet:" << applet->pluginId();
            QObject::connect(applet, SIGNAL(localeChanged(const QString &)), this, SLOT(onLocaleChanged(const QString &)));
        }
    }
}

void AppletManager::quit()
{
    for (auto item : std::as_const(m_loaders)) {
        item->deleteLater();
    }
}

void AppletManager::onLocaleChanged(const QString &locale)
{
    qCDebug(dsLog) << "Retranslate to the locale:" << locale;
    for (auto loader : std::as_const(m_loaders)) {
        loader->setLocale(locale);
        loader->reloadTranslations();
    }
}

DS_END_NAMESPACE
