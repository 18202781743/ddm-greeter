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

#include "authmanager.h"
#include "daauth.h"
#include "ddmauth.h"

#include "deepinauthframework.h"

class AuthManagerPrivate
{
public:
    AuthInterface::AuthFrameType m_authFrameType{ AuthInterface::AF_DA };
    DeepinAuthFramework *m_authFramework = nullptr;
    QMap<AuthInterface::AuthType, AuthInterface *> m_auths;
    QPointer<QObject> m_ddmAuthImpl;
    QVariantMap m_session;
};

AuthManager *AuthManager::instance()
{
    static AuthManager *g_instance = nullptr;
    if (!g_instance) {
        g_instance = new AuthManager();
    }
    return g_instance;
}

void AuthManager::setAuthFrame(AuthInterface::AuthFrameType type)
{
    qDebug() << "Auth frame type:" << type;
    d->m_authFrameType = type;
    if (type == AuthInterface::AF_DA) {
        d->m_authFramework = new DeepinAuthFramework();
    }
}

void AuthManager::setDDMAuthImpl(QObject *impl)
{
    d->m_ddmAuthImpl = impl;
}

AuthInterface *AuthManager::create(AuthInterface::AuthType type)
{
    AuthInterface *auth = nullptr;
    if (!(supportedAuthType() & type))
        return nullptr;
    if (d->m_authFrameType == AuthInterface::AF_DA) {
        auto da = new DAAuth();
        da->setImpl(d->m_authFramework);
        auth = da;
    } else if (d->m_authFrameType == AuthInterface::AF_DDM) {
        auto ddm = new DDMAuth();
        ddm->setImpl(d->m_ddmAuthImpl);
        auth = ddm;
    }
    auth->m_type = type;
    connect(auth, &AuthInterface::stateChanged, this, [this, type] () {
        handleAuthStateChanged(type);
    });
    d->m_auths[auth->m_type] = auth;

    return auth;
}

AuthInterface *AuthManager::get(AuthInterface::AuthType type) const
{
    if (d->m_auths.contains(type))
        return d->m_auths.value(type);

    return const_cast<AuthManager *>(this)->create(type);
}

AuthInterface::AuthType AuthManager::supportedAuthType() const
{
    if (d->m_authFrameType == AuthInterface::AF_DA) {
        return static_cast<AuthInterface::AuthType>(d->m_authFramework->GetSupportedMixAuthFlags());
    }
    return AuthInterface::AT_Password;
}

void AuthManager::setUser(const QString &user)
{
    qDebug() << "Set user" << user;
    for (auto item : d->m_auths) {
        item->setUser(user);
    }
}

void AuthManager::setSession(const QVariantMap &session)
{
    qDebug() << "Update current session" << session;
    d->m_session = session;
}

QVariantMap AuthManager::session() const
{
    return d->m_session;
}

void AuthManager::requestCreate()
{
    qDebug() << "Request create";
    for (auto item : d->m_auths) {
        item->create();
    }
}

void AuthManager::requestStart()
{
    qDebug() << "Request start";
    for (auto item : d->m_auths) {
        item->start();
    }
}

void AuthManager::requestDestroy()
{
    qDebug() << "Request destroy";
    for (auto item : d->m_auths) {
        item->end();
        item->destroy();
    }
}

void AuthManager::handleAuthStateChanged(AuthInterface::AuthType type)
{
    auto auth = get(type);
    const auto state = auth->state();
    switch (state) {
    case AuthInterface::AS_Success: {
        auth->end();
        auth->destroy();
        authSuccessed();
    } break;
    case AuthInterface::AS_Failure: {
        authFailed();
    } break;
    default: {
    } break;
    }
}

AuthManager::AuthManager()
    : d(new AuthManagerPrivate())
{

}