/*
 * This file was generated by qdbusxml2cpp-fix version 0.8
 * Command line was: qdbusxml2cpp-fix -c Authenticate -p /home/repo/dde-session-shell/src/global_util/dbus/authenticate_interface /home/repo/dde-session-shell/xml/org.deepin.dde.Authenticate1.xml
 *
 * qdbusxml2cpp-fix is Copyright (C) 2016 Deepin Technology Co., Ltd.
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef AUTHENTICATE_INTERFACE_H
#define AUTHENTICATE_INTERFACE_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

#include <DDBusExtendedAbstractInterface>
#include <QtDBus/QtDBus>


/*
 * Proxy class for interface org.deepin.dde.Authenticate1
 */
class __AuthenticatePrivate;
class __Authenticate : public DTK_CORE_NAMESPACE::DDBusExtendedAbstractInterface
{
    Q_OBJECT

public:
    static inline const char *staticInterfaceName()
    { return "org.deepin.dde.Authenticate1"; }

public:
    explicit __Authenticate(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~__Authenticate();

    Q_PROPERTY(int FrameworkState READ frameworkState NOTIFY FrameworkStateChanged)
    int frameworkState();

    Q_PROPERTY(QString SupportEncrypts READ supportEncrypts NOTIFY SupportEncryptsChanged)
    QString supportEncrypts();

    Q_PROPERTY(int SupportedFlags READ supportedFlags NOTIFY SupportedFlagsChanged)
    int supportedFlags();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<QString> Authenticate(const QString &username, int authFlags, int appType)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(username) << QVariant::fromValue(authFlags) << QVariant::fromValue(appType);
        return asyncCallWithArgumentList(QStringLiteral("Authenticate"), argumentList);
    }



    inline QDBusPendingReply<QString> GetLimits(const QString &username)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(username);
        return asyncCallWithArgumentList(QStringLiteral("GetLimits"), argumentList);
    }



    inline QDBusPendingReply<QString> PreOneKeyLogin(int flag)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(flag);
        return asyncCallWithArgumentList(QStringLiteral("PreOneKeyLogin"), argumentList);
    }




Q_SIGNALS: // SIGNALS
    void LimitUpdated(const QString &in0);
    // begin property changed signals
    void FrameworkStateChanged(int  value) const;
    void SupportEncryptsChanged(const QString & value) const;
    void SupportedFlagsChanged(int  value) const;

public Q_SLOTS:
    void CallQueued(const QString &callName, const QList<QVariant> &args);

private Q_SLOTS:
    void onPendingCallFinished(QDBusPendingCallWatcher *w);
    void onPropertyChanged(const QString &propName, const QVariant &value);

private:
    __AuthenticatePrivate *d_ptr;
};

namespace org {
  namespace deepin {
    namespace dde {
      typedef ::__Authenticate Authenticate1;
    }
  }
}
#endif
