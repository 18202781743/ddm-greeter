/***************************************************************************
 * Copyright (c) 2015 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#include "greeterproxy.h"

#include "global.h"
#include "usermodel.h"
#include "greeterauthinterface.h"

#include <DisplayManager.h>
#include <DisplayManagerSession.h>

#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QGuiApplication>
#include <QLocalSocket>
#include <QVariantMap>

Q_LOGGING_CATEGORY(dmGreeter, "ddm.greeter", QtDebugMsg);

class GreeterProxyPrivate
{
public:
    UserModel *userModel{ nullptr };
    DisplayManager *displayManager{ nullptr };
    GreeterAuthInterface *m_auth = nullptr;
    GreeterProxy::GreeterState m_state = {GreeterProxy::NotReady};
};

GreeterProxy::GreeterProxy(QObject *parent)
    : QObject(parent)
    , d(new GreeterProxyPrivate())
{
    d->m_auth = new GreeterAuthInterface(this);
    d->userModel = new UserModel(this);
    d->displayManager = new DisplayManager("org.freedesktop.DisplayManager",
                                           "/org/freedesktop/DisplayManager",
                                           QDBusConnection::systemBus(),
                                           this);
}

GreeterProxy::~GreeterProxy()
{
    delete d;
}

QString GreeterProxy::hostName() const
{
    return d->m_auth->hostName();
}

void GreeterProxy::setAuth(GreeterAuthInterface *auth)
{
    if (d->m_auth) {
        d->m_auth->deleteLater();
    }
    d->m_auth = auth;
    QObject::connect(d->m_auth, &GreeterAuthInterface::hostNameChanged, this, &GreeterProxy::hostNameChanged);
    QObject::connect(d->m_auth, &GreeterAuthInterface::loginFailed, this, &GreeterProxy::onLoginFailed);
    QObject::connect(d->m_auth, &GreeterAuthInterface::loginSucceeded, this, &GreeterProxy::onLoginSuccessed);
}

UserModel *GreeterProxy::userModel() const
{
    return d->userModel;
}

void GreeterProxy::init()
{
    connect(d->displayManager, &DisplayManager::SessionAdded, this, &GreeterProxy::onSessionAdded);
    connect(d->displayManager,
            &DisplayManager::SessionRemoved,
            this,
            &GreeterProxy::onSessionRemoved);

    auto sessions = d->displayManager->sessions();
    for (auto session : sessions) {
        onSessionAdded(session);
    }
    connect(d->userModel, &UserModel::currentUserNameChanged, this, &GreeterProxy::currentUserChanged);
    connect(this, &GreeterProxy::currentUserChanged, this, &GreeterProxy::onCurrentUserChanged);
}

void GreeterProxy::unlock(const QString &user, const QString &password)
{
    auto userInfo = userModel()->get(user);
    if (userInfo.isValid()) {
        d->m_auth->unlock(user, password);
    }
}

void GreeterProxy::quit()
{
    setState(GreeterProxy::Quit);
}

void GreeterProxy::onLoginFailed(const QString &user)
{
    const auto currentUser = d->userModel->currentUserName();
    qDebug(dmGreeter) << "Last activate user:" << user << "current user:" << currentUser;
    if (user != currentUser) {
        return;
    }
    setState(GreeterProxy::AuthFailed);
}

void GreeterProxy::onLoginSuccessed(const QString &user)
{
    const auto currentUser = d->userModel->currentUserName();
    qDebug(dmGreeter) << "Last activate user:" << user << "current user:" << currentUser;
    if (user != currentUser) {
        return;
    }
    setState(GreeterProxy::AuthSucceeded);
}

void GreeterProxy::setState(GreeterState state)
{
    if (d->m_state == state)
        return;

    d->m_state = state;
    emit stateChanged();
}

void GreeterProxy::activateUser(const QString &user)
{
    qDebug(dmGreeter) << "Activate user:" << user;
    auto userInfo = userModel()->get(user);
    if (userInfo.isValid()) {
        userModel()->setCurrentUserName(user);
    } else {
        qWarning(dmGreeter) << "Invalid user" << user;
    }
}

void GreeterProxy::onSessionAdded(const QDBusObjectPath &session)
{
    DisplaySession s(d->displayManager->service(), session.path(), QDBusConnection::systemBus());

    userModel()->updateUserLoginState(s.userName(), true);
}

void GreeterProxy::onSessionRemoved(const QDBusObjectPath &session)
{
    // FIXME: Reset all user state, because we can't know which user was logout.
    userModel()->clearUserLoginState();

    auto sessions = d->displayManager->sessions();
    for (auto session : sessions) {
        onSessionAdded(session);
    }
}

void GreeterProxy::onCurrentUserChanged()
{
    auto user = userModel()->getUser(userModel()->currentUserName());
    if (!user) {
        qCWarning(dmGreeter) << "couldn't find user:";
        return;
    }

    auto locale = user->locale();
    qCInfo(dmGreeter) << "current locale:" << locale.language();
    emit localeChanged(locale.name());
}

GreeterProxy::GreeterState GreeterProxy::state() const
{
    return d->m_state;
}
