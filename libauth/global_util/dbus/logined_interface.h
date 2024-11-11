/*
 * This file was generated by qdbusxml2cpp-fix version 0.8
 * Command line was: qdbusxml2cpp-fix -c Logined -p /home/repo/dde-session-shell/src/global_util/dbus/logined_interface /home/repo/dde-session-shell/xml/org.deepin.dde.Logined.xml
 *
 * qdbusxml2cpp-fix is Copyright (C) 2016 Deepin Technology Co., Ltd.
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef LOGINED_INTERFACE_H
#define LOGINED_INTERFACE_H

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
 * Proxy class for interface org.deepin.dde.Logined
 */
class __LoginedPrivate;
class __Logined : public DTK_CORE_NAMESPACE::DDBusExtendedAbstractInterface
{
    Q_OBJECT

public:
    static inline const char *staticInterfaceName()
    { return "org.deepin.dde.Logined"; }

public:
    explicit __Logined(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~__Logined();

    Q_PROPERTY(uint LastLogoutUser READ lastLogoutUser NOTIFY LastLogoutUserChanged)
    uint lastLogoutUser();

    Q_PROPERTY(QString UserList READ userList NOTIFY UserListChanged)
    QString userList();

public Q_SLOTS: // METHODS

Q_SIGNALS: // SIGNALS
    // begin property changed signals
    void LastLogoutUserChanged(uint  value) const;
    void UserListChanged(const QString & value) const;

public Q_SLOTS:
    void CallQueued(const QString &callName, const QList<QVariant> &args);

private Q_SLOTS:
    void onPendingCallFinished(QDBusPendingCallWatcher *w);
    void onPropertyChanged(const QString &propName, const QVariant &value);

private:
    __LoginedPrivate *d_ptr;
};

namespace org {
  namespace deepin {
    namespace dde {
      typedef ::__Logined Logined;
    }
  }
}
#endif
