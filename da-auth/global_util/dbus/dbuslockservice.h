/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -c DBusLockService -p dbuslockservice org.deepin.dde.LockService1.xml
 *
 * qdbusxml2cpp is Copyright (C) 2015 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef DBUSLOCKSERVICE_H
#define DBUSLOCKSERVICE_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface org.deepin.dde.LockService1
 */
class DBusLockService: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "org.deepin.dde.LockService1"; }

    enum EventType {
        PromptQuestion = 1,
        PromptSecret = 2,
        ErrorMsg = 3,
        TextInfo = 4,
        Failure = 5,
        Success = 6
    };

public:
    DBusLockService(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~DBusLockService();

public Q_SLOTS: // METHODS

    inline QDBusPendingReply<QString> CurrentUser()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("CurrentUser"), argumentList);
    }

    inline QDBusPendingReply<bool> IsLiveCD(const QString &username)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(username);
        return asyncCallWithArgumentList(QStringLiteral("IsLiveCD"), argumentList);
    }

    inline QDBusPendingReply<> SwitchToUser(const QString &username)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(username);
        return asyncCallWithArgumentList(QStringLiteral("SwitchToUser"), argumentList);
    }

    inline QDBusPendingReply<> UnlockCheck(const QString &username, const QString &password)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(username) << QVariant::fromValue(password);
        return asyncCallWithArgumentList(QStringLiteral("UnlockCheck"), argumentList);
    }

    inline QDBusPendingReply<> AuthenticateUser(const QString &username)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(username);
        return asyncCallWithArgumentList(QStringLiteral("AuthenticateUser"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void UserChanged(const QString &user);
    void Event(quint32 eventType, quint32 pid, const QString &username, const QString &message);
};

namespace com {
  namespace deepin {
    namespace dde {
      typedef ::DBusLockService lock;
    }
  }
}
#endif
