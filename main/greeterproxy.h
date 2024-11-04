/***************************************************************************
 * Copyright (c) 2013 Abdurrahman AVCI <abdurrahmanavci@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 ***************************************************************************/

#ifndef SDDM_GREETERPROXY_H
#define SDDM_GREETERPROXY_H

#include "greeterauthinterface.h"

#include <QDBusObjectPath>
#include <QObject>
#include <QtQml/qqml.h>
Q_MOC_INCLUDE(<sessionmodel.h>)
Q_MOC_INCLUDE(<usermodel.h>)

class QLocalSocket;

class SessionModel;
class UserModel;

class GreeterProxyPrivate;

class GreeterProxy : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(Proxy)
    Q_PROPERTY(GreeterState  state      READ state          NOTIFY stateChanged)
    Q_PROPERTY(QString  hostName        READ hostName       NOTIFY hostNameChanged)
    Q_PROPERTY(bool     canPowerOff     READ canPowerOff    NOTIFY canPowerOffChanged)
    Q_PROPERTY(bool     canReboot       READ canReboot      NOTIFY canRebootChanged)
    Q_PROPERTY(bool     canSuspend      READ canSuspend     NOTIFY canSuspendChanged)
    Q_PROPERTY(bool     canHibernate    READ canHibernate   NOTIFY canHibernateChanged)
    Q_PROPERTY(bool     canHybridSleep  READ canHybridSleep NOTIFY canHybridSleepChanged)
    Q_PROPERTY(SessionModel* sessionModel READ sessionModel NOTIFY sessionModelChanged)
    Q_PROPERTY(UserModel* userModel READ userModel NOTIFY userModelChanged)

public:
    enum GreeterState {
        NotReady = 0,
        AuthSucceeded = 1,
        AuthFailed = 2,
        Quit = 3
    };
    Q_ENUM(GreeterState)

    explicit GreeterProxy(QObject *parent = nullptr);
    ~GreeterProxy();

    void setAuth(GreeterAuthInterface *auth);

    QString hostName() const;
    bool canPowerOff() const;
    bool canReboot() const;
    bool canSuspend() const;
    bool canHibernate() const;
    bool canHybridSleep() const;

    bool isConnected() const;

    SessionModel *sessionModel() const;
    UserModel *userModel() const;

    GreeterState state() const;
    void setState(GreeterState state);

public Q_SLOTS:
    void powerOff();
    void reboot();
    void suspend();
    void hibernate();
    void hybridSleep();
    void init();

    void login(const QString &user, const QString &password) const;
    void activateUser(const QString &user);
    void unlock(const QString &user, const QString &password);

    void quit();
    void onLoginFailed(const QString &user);
    void onLoginSuccessed(const QString &user);

private Q_SLOTS:
    void connected();
    void disconnected();
    void readyRead();
    void error();
    void onSessionAdded(const QDBusObjectPath &session);
    void onSessionRemoved(const QDBusObjectPath &session);
    void onCurrentUserChanged();

Q_SIGNALS:
    void informationMessage(const QString &message);
    void hostNameChanged(const QString &hostName);
    void canPowerOffChanged(bool canPowerOff);
    void canRebootChanged(bool canReboot);
    void canSuspendChanged(bool canSuspend);
    void canHibernateChanged(bool canHibernate);
    void canHybridSleepChanged(bool canHybridSleep);
    void sessionModelChanged(SessionModel *model);
    void userModelChanged(UserModel *model);

    void socketDisconnected();
    void switchToGreeter();

    void stateChanged();
    void localeChanged(const QString &locale);
    void currentUserChanged();

private:
    GreeterProxyPrivate *d{ nullptr };
};

#endif // SDDM_GREETERPROXY_H
