// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "daauth.h"
#include "authcommon.h"

#include <QGuiApplication>
#include <QLocalSocket>
#include <QLoggingCategory>
#include <dbuslogin1manager.h>

Q_LOGGING_CATEGORY(dmgAuth, "ddm.greeter.auth", QtDebugMsg);

bool DAAuth::isConnected() const
{
    return true;
}

void DAAuth::powerOff()
{
    m_login1Inter->PowerOff(true);
}

void DAAuth::reboot()
{
    m_login1Inter->Reboot(true);
}

void DAAuth::suspend()
{
    m_login1Inter->Suspend(true);
}

void DAAuth::hibernate()
{
    m_login1Inter->Hibernate(true);
}

void DAAuth::hybridSleep()
{
    m_login1Inter->HybridSleep(true);
}

void DAAuth::login(const QString &user, const QString &password, const QVariantMap &session) const
{
    qCDebug(dmgAuth) << "login" << user;
    m_authFramework->SendTokenToAuth(user, AuthCommon::AT_Password, password);
}

void DAAuth::unlock(const QString &user, const QString &password)
{
}

void DAAuth::activateUser(const QString &user)
{
    qCDebug(dmgAuth) << "Activate user" << user;

    activeAuthentication(m_currentUserName, false);

    m_currentUserName = user;

    activeAuthentication(user, true);
}

void DAAuth::connected()
{
    qCDebug(dmgAuth) << "Connected to the daemon.";
}

void DAAuth::disconnected()
{
    qCDebug(dmgAuth) << "Disconnected from the daemon.";

    Q_EMIT socketDisconnected();
}

void DAAuth::error()
{
    qCCritical(dmgAuth) << "Socket error: ";
}

DAAuth::DAAuth(QObject *parent)
    : GreeterAuthInterface(parent)
    , m_login1Inter(new DBusLogin1Manager("org.freedesktop.login1", "/org/freedesktop/login1", QDBusConnection::systemBus(), this))
{
    if (m_login1Inter->isValid()) {
        QString sessionSelf;
            // v23上m_login1Inter->GetSessionByPID(0)接口已不可用，使用org.deepin.Session获取
            QDBusInterface inter("org.deepin.dde.Session1", "/org/deepin/dde/Session1",
                                 "org.deepin.dde.Session1", QDBusConnection::sessionBus());
            QDBusReply<QString> reply = inter.call("GetSessionPath");
            if (reply.isValid())
                sessionSelf = reply.value();
            else
                qWarning() << "org.deepin.dde.Session1 get session path has error!";

        if (!sessionSelf.isEmpty()) {
            m_login1SessionSelf = new Login1SessionSelf("org.freedesktop.login1", sessionSelf, QDBusConnection::systemBus(), this);
            m_login1SessionSelf->setSync(false);
            // m_login1SessionSelf->property("user");
            // m_currentUserName = "shanshany";
        }
    } else {
        qWarning() << "m_login1Inter:" << m_login1Inter->lastError().type();
    }

    connect(m_login1SessionSelf, &Login1SessionSelf::ActiveChanged, this, [ = ](bool active) {
        qInfo() << "DBusLockService::ActiveChanged:" << active;
        if (active) {
            createAuthentication();
        } else {
            endAuthentication();
            destroyAuthentication();
        }
    });
}

void DAAuth::onAuthStateChanged(const int type, const int state, const QString &message)
{
    qDebug() << "AuthStateChanged:" << type << state << message;

    switch (state) {
    case AuthCommon::AS_Success:
        endAuthentication();
        destroyAuthentication();
        emit loginSucceeded(m_currentUserName);
        break;
    case AuthCommon::AS_Failure:
        emit loginFailed(m_currentUserName);
        break;
    case AuthCommon::AS_Cancel:
        destroyAuthentication();
        emit loginFailed(m_currentUserName);
        break;
    case AuthCommon::AS_Recover:
        createAuthentication();
        break;
    default:
        break;
    }
}

void DAAuth::createAuthentication()
{
    qInfo() << "createAuthentication:";
    m_authFramework->CreateAuthController(m_currentUserName, m_authFramework->GetSupportedMixAuthFlags(), AuthCommon::Lock);
}

void DAAuth::startAuthentication()
{
    qInfo() << "startAuthentication:";
    m_authFramework->StartAuthentication(m_currentUserName, AuthCommon::AT_Password, 1000);
}

void DAAuth::activeAuthentication(bool active)
{
    if (active) {
        activeAuthentication(m_currentUserName, false);
    }
    activeAuthentication(m_currentUserName, active);
}

void DAAuth::activeAuthentication(const QString &user, bool active)
{
    const bool exist = m_authFramework->authSessionExist(user);
    if (active) {
        if (exist) {
            m_authFramework->EndAuthentication(user, AuthCommon::AT_All);
            m_authFramework->DestroyAuthController(user);
        }
        m_authFramework->CreateAuthenticate(user);
    } else {
        if (exist) {
            m_authFramework->EndAuthentication(user, AuthCommon::AT_All);
            m_authFramework->DestroyAuthController(user);
        }
    }
}

void DAAuth::endAuthentication()
{
    qDebug() << "endAuthentication:" << m_currentUserName;

    m_authFramework->EndAuthentication(m_currentUserName, AuthCommon::AT_All);
}

void DAAuth::destroyAuthentication()
{
    qInfo() << "destroyAuthentication:";
    m_authFramework->DestroyAuthController(m_currentUserName);
}

bool DAAuth::init()
{
    m_authFramework = new DeepinAuthFramework(this);
    connect(m_authFramework, &DeepinAuthFramework::AuthStateChanged, this, &DAAuth::onAuthStateChanged);
    return true;
}
