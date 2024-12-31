/*
 * This file was generated by qdbusxml2cpp-fix version 0.8
 * Command line was: qdbusxml2cpp-fix -c AuthenticateSession2 -p /home/repo/dde-session-shell/src/global_util/dbus/authenticatesession2_interface /home/repo/dde-session-shell/xml/org.deepin.dde.Authenticate1.Session2.xml
 *
 * qdbusxml2cpp-fix is Copyright (C) 2016 Deepin Technology Co., Ltd.
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#include "/home/repo/dde-session-shell/src/global_util/dbus/authenticatesession2_interface.h"

DCORE_USE_NAMESPACE
/*
 * Implementation of interface class __AuthenticateSession2
 */

class __AuthenticateSession2Private
{
public:
   __AuthenticateSession2Private() = default;

    // begin member variables
    MFAInfoList FactorsInfo;
    bool IsFuzzyMFA;
    bool IsMFA;
    int PINLen;
    QString Prompt;
    QString Username;

public:
    QMap<QString, QDBusPendingCallWatcher *> m_processingCalls;
    QMap<QString, QList<QVariant>> m_waittingCalls;
};

__AuthenticateSession2::__AuthenticateSession2(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : DDBusExtendedAbstractInterface(service, path, staticInterfaceName(), connection, parent)
    , d_ptr(new __AuthenticateSession2Private)
{
    connect(this, &__AuthenticateSession2::propertyChanged, this, &__AuthenticateSession2::onPropertyChanged);

    if (QMetaType::type("ArrayInt") == QMetaType::UnknownType)
        registerArrayIntMetaType();
    if (QMetaType::type("MFAInfoList") == QMetaType::UnknownType)
        registerMFAInfoListMetaType();
}

__AuthenticateSession2::~__AuthenticateSession2()
{
    qDeleteAll(d_ptr->m_processingCalls.values());
    delete d_ptr;
}

void __AuthenticateSession2::onPropertyChanged(const QString &propName, const QVariant &value)
{
    if (propName == QStringLiteral("FactorsInfo"))
    {
        const MFAInfoList &FactorsInfo = qvariant_cast<MFAInfoList>(value);
        if (d_ptr->FactorsInfo != FactorsInfo)
        {
            d_ptr->FactorsInfo = FactorsInfo;
            Q_EMIT FactorsInfoChanged(d_ptr->FactorsInfo);
        }
        return;
    }

    if (propName == QStringLiteral("IsFuzzyMFA"))
    {
        const bool &IsFuzzyMFA = qvariant_cast<bool>(value);
        if (d_ptr->IsFuzzyMFA != IsFuzzyMFA)
        {
            d_ptr->IsFuzzyMFA = IsFuzzyMFA;
            Q_EMIT IsFuzzyMFAChanged(d_ptr->IsFuzzyMFA);
        }
        return;
    }

    if (propName == QStringLiteral("IsMFA"))
    {
        const bool &IsMFA = qvariant_cast<bool>(value);
        if (d_ptr->IsMFA != IsMFA)
        {
            d_ptr->IsMFA = IsMFA;
            Q_EMIT IsMFAChanged(d_ptr->IsMFA);
        }
        return;
    }

    if (propName == QStringLiteral("PINLen"))
    {
        const int &PINLen = qvariant_cast<int>(value);
        if (d_ptr->PINLen != PINLen)
        {
            d_ptr->PINLen = PINLen;
            Q_EMIT PINLenChanged(d_ptr->PINLen);
        }
        return;
    }

    if (propName == QStringLiteral("Prompt"))
    {
        const QString &Prompt = qvariant_cast<QString>(value);
        if (d_ptr->Prompt != Prompt)
        {
            d_ptr->Prompt = Prompt;
            Q_EMIT PromptChanged(d_ptr->Prompt);
        }
        return;
    }

    if (propName == QStringLiteral("Username"))
    {
        const QString &Username = qvariant_cast<QString>(value);
        if (d_ptr->Username != Username)
        {
            d_ptr->Username = Username;
            Q_EMIT UsernameChanged(d_ptr->Username);
        }
        return;
    }

    qWarning() << "property not handle: " << propName;
    return;
}

MFAInfoList __AuthenticateSession2::factorsInfo()
{
    return qvariant_cast<MFAInfoList>(internalPropGet("FactorsInfo", &d_ptr->FactorsInfo));
}

bool __AuthenticateSession2::isFuzzyMFA()
{
    return qvariant_cast<bool>(internalPropGet("IsFuzzyMFA", &d_ptr->IsFuzzyMFA));
}

bool __AuthenticateSession2::isMFA()
{
    return qvariant_cast<bool>(internalPropGet("IsMFA", &d_ptr->IsMFA));
}

int __AuthenticateSession2::pINLen()
{
    return qvariant_cast<int>(internalPropGet("PINLen", &d_ptr->PINLen));
}

QString __AuthenticateSession2::prompt()
{
    return qvariant_cast<QString>(internalPropGet("Prompt", &d_ptr->Prompt));
}

QString __AuthenticateSession2::username()
{
    return qvariant_cast<QString>(internalPropGet("Username", &d_ptr->Username));
}

void __AuthenticateSession2::CallQueued(const QString &callName, const QList<QVariant> &args)
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
        connect(watcher, &QDBusPendingCallWatcher::finished, this, &__AuthenticateSession2::onPendingCallFinished);
        d_ptr->m_processingCalls.insert(callName, watcher);
    }
}

void __AuthenticateSession2::onPendingCallFinished(QDBusPendingCallWatcher *w)
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