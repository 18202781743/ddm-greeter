// Copyright (C) 2023 justforlxz <justforlxz@gmail.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "greeterapp.h"
#include "dbusfrontservice.h"

#include <QLoggingCategory>

#include <pluginfactory.h>
#include <applet.h>
#include <appletbridge.h>
#include <QDBusConnection>
#include <QDBusInterface>


static const QString DBUS_LOCK_PATH = "/org/deepin/dde/LockFront1";
static const QString DBUS_LOCK_NAME = "org.deepin.dde.LockFront2";

static const QString DBUS_SHUTDOWN_PATH = "/org/deepin/dde/ShutdownFront1";
static const QString DBUS_SHUTDOWN_NAME = "org.deepin.dde.ShutdownFront2";

DS_USE_NAMESPACE

GreeterPanel::GreeterPanel(QObject *parent)
    : DPanel(parent)
    , m_proxy(new GreeterProxy(this))
{
    QObject::connect(m_proxy, &GreeterProxy::localeChanged, this, &GreeterPanel::localeChanged);
}

bool GreeterPanel::load()
{
    new DBusFrontService(this);
    QDBusConnection conn = QDBusConnection::sessionBus();
    if (!conn.registerService(DBUS_LOCK_NAME) || !conn.registerObject(DBUS_LOCK_PATH, this)) {
        qDebug() << "register dbus failed"<< "maybe lockFront is running..." << conn.lastError();
        return false;
    }
    if (!conn.registerService(DBUS_SHUTDOWN_NAME) || !conn.registerObject(DBUS_SHUTDOWN_PATH, this)) {
        qDebug() << "register dbus failed"<< "maybe lockFront is running..." << conn.lastError();
        return false;
    }

    return DPanel::load();
}

bool GreeterPanel::init()
{
    DAppletBridge bridge("org.deepin.ds.greeter.auth");
    if (auto applet = bridge.applet()) {
        auto containment = reinterpret_cast<DContainment *>(applet);
        Q_ASSERT(containment);
        connect(containment, SIGNAL(loginFailed(const QString&)), this, SLOT(onLoginFailed(const QString &)));
        connect(containment, SIGNAL(loginSuccessed(const QString&)), this, SLOT(onLoginSuccessed(const QString &)));
    }

    m_proxy->init();

    show();

    return DPanel::init();
}

GreeterProxy *GreeterPanel::proxy() const
{
    return m_proxy;
}

void GreeterPanel::setAuthActive(bool active)
{
    if (active == m_authActive)
        return;
    m_authActive = active;
    emit authActiveChanged(m_authActive);
}

void GreeterPanel::show()
{
    m_visible = true;
    emit visibleChanged(m_visible);

    setAuthActive(true);
}

void GreeterPanel::showUserList()
{

}

void GreeterPanel::showAuth(bool active)
{

}

void GreeterPanel::suspend(bool enable)
{

}

void GreeterPanel::hibernate(bool enable)
{

}

bool GreeterPanel::visible() const
{
    return m_visible;
}

bool GreeterPanel::authActive() const
{
    return m_authActive;
}

void GreeterPanel::onLoginFailed(const QString &user)
{
    qDebug() << "Login failed:" << user;
    m_proxy->onLoginFailed(user);
}

void GreeterPanel::onLoginSuccessed(const QString &user)
{
    qDebug() << "Login successed:" << user;
    m_proxy->onLoginSuccessed(user);
}

D_APPLET_CLASS(GreeterPanel)

#include "greeterapp.moc"
