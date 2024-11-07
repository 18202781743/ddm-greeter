/*
 * This file was generated by qdbusxml2cpp-fix version 0.8
 * Command line was: qdbusxml2cpp-fix -c ImageEffect -p /home/repo/dde-session-shell/src/global_util/dbus/imageeffect_interface /home/repo/dde-session-shell/xml/org.deepin.dde.ImageEffect1.xml
 *
 * qdbusxml2cpp-fix is Copyright (C) 2016 Deepin Technology Co., Ltd.
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#include "/home/repo/dde-session-shell/src/global_util/dbus/imageeffect_interface.h"

DCORE_USE_NAMESPACE
/*
 * Implementation of interface class __ImageEffect
 */

class __ImageEffectPrivate
{
public:
   __ImageEffectPrivate() = default;

    // begin member variables

public:
    QMap<QString, QDBusPendingCallWatcher *> m_processingCalls;
    QMap<QString, QList<QVariant>> m_waittingCalls;
};

__ImageEffect::__ImageEffect(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : DDBusExtendedAbstractInterface(service, path, staticInterfaceName(), connection, parent)
    , d_ptr(new __ImageEffectPrivate)
{
}

__ImageEffect::~__ImageEffect()
{
    qDeleteAll(d_ptr->m_processingCalls.values());
    delete d_ptr;
}

void __ImageEffect::CallQueued(const QString &callName, const QList<QVariant> &args)
{
    if (d_ptr->m_waittingCalls.contains(callName))
    {
        d_ptr->m_waittingCalls[callName] = args;
        return;
    }
    if (d_ptr->m_processingCalls.contains(callName))
    {
        d_ptr->m_waittingCalls.insert(callName, args);
    } else {
        QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(asyncCallWithArgumentList(callName, args));
        connect(watcher, &QDBusPendingCallWatcher::finished, this, &__ImageEffect::onPendingCallFinished);
        d_ptr->m_processingCalls.insert(callName, watcher);
    }
}

void __ImageEffect::onPendingCallFinished(QDBusPendingCallWatcher *w)
{
    w->deleteLater();
    const auto callName = d_ptr->m_processingCalls.key(w);
    Q_ASSERT(!callName.isEmpty());
    if (callName.isEmpty())
        return;
    d_ptr->m_processingCalls.remove(callName);
    if (!d_ptr->m_waittingCalls.contains(callName))
        return;
    const auto args = d_ptr->m_waittingCalls.take(callName);
    CallQueued(callName, args);
}
