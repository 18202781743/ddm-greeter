/*
 * This file was generated by qdbusxml2cpp-fix version 0.8
 * Command line was: qdbusxml2cpp-fix -c Authenticate -p /home/repo/dde-session-shell/src/global_util/dbus/authenticate_interface /home/repo/dde-session-shell/xml/org.deepin.dde.Authenticate1.xml
 *
 * qdbusxml2cpp-fix is Copyright (C) 2016 Deepin Technology Co., Ltd.
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#include "/home/repo/dde-session-shell/src/global_util/dbus/authenticate_interface.h"

DCORE_USE_NAMESPACE
/*
 * Implementation of interface class __Authenticate
 */

class __AuthenticatePrivate
{
public:
   __AuthenticatePrivate() = default;

    // begin member variables
    int FrameworkState;
    QString SupportEncrypts;
    int SupportedFlags;

public:
    QMap<QString, QDBusPendingCallWatcher *> m_processingCalls;
    QMap<QString, QList<QVariant>> m_waittingCalls;
};

__Authenticate::__Authenticate(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : DDBusExtendedAbstractInterface(service, path, staticInterfaceName(), connection, parent)
    , d_ptr(new __AuthenticatePrivate)
{
    connect(this, &__Authenticate::propertyChanged, this, &__Authenticate::onPropertyChanged);

}

__Authenticate::~__Authenticate()
{
    qDeleteAll(d_ptr->m_processingCalls.values());
    delete d_ptr;
}

void __Authenticate::onPropertyChanged(const QString &propName, const QVariant &value)
{
    if (propName == QStringLiteral("FrameworkState"))
    {
        const int &FrameworkState = qvariant_cast<int>(value);
        if (d_ptr->FrameworkState != FrameworkState)
        {
            d_ptr->FrameworkState = FrameworkState;
            Q_EMIT FrameworkStateChanged(d_ptr->FrameworkState);
        }
        return;
    }

    if (propName == QStringLiteral("SupportEncrypts"))
    {
        const QString &SupportEncrypts = qvariant_cast<QString>(value);
        if (d_ptr->SupportEncrypts != SupportEncrypts)
        {
            d_ptr->SupportEncrypts = SupportEncrypts;
            Q_EMIT SupportEncryptsChanged(d_ptr->SupportEncrypts);
        }
        return;
    }

    if (propName == QStringLiteral("SupportedFlags"))
    {
        const int &SupportedFlags = qvariant_cast<int>(value);
        if (d_ptr->SupportedFlags != SupportedFlags)
        {
            d_ptr->SupportedFlags = SupportedFlags;
            Q_EMIT SupportedFlagsChanged(d_ptr->SupportedFlags);
        }
        return;
    }

    qWarning() << "property not handle: " << propName;
    return;
}

int __Authenticate::frameworkState()
{
    return qvariant_cast<int>(internalPropGet("FrameworkState", &d_ptr->FrameworkState));
}

QString __Authenticate::supportEncrypts()
{
    return qvariant_cast<QString>(internalPropGet("SupportEncrypts", &d_ptr->SupportEncrypts));
}

int __Authenticate::supportedFlags()
{
    return qvariant_cast<int>(internalPropGet("SupportedFlags", &d_ptr->SupportedFlags));
}

void __Authenticate::CallQueued(const QString &callName, const QList<QVariant> &args)
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
        connect(watcher, &QDBusPendingCallWatcher::finished, this, &__Authenticate::onPendingCallFinished);
        d_ptr->m_processingCalls.insert(callName, watcher);
    }
}

void __Authenticate::onPendingCallFinished(QDBusPendingCallWatcher *w)
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