// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "authapplet.h"

#include "authmanager.h"

#include <QGuiApplication>
#include <QLocalSocket>
#include <QLoggingCategory>

#include <appletbridge.h>
#include <pluginfactory.h>

AuthApplet::AuthApplet(QObject *parent)
    : DContainment(parent)
{
}

bool AuthApplet::load()
{
    return DContainment::load();
}

bool AuthApplet::init()
{
    if (auto proxy = greeter()) {
        QObject::connect(proxy, SIGNAL(visibleChanged(bool)), this, SLOT(requestAuthentication(bool)));
    }
    if (rootObject()) {
        const auto user = currentUserName();
        AuthManager::instance()->setUser(user);

        connect(AuthManager::instance(), &AuthManager::authFailed, this, [this] () {
            const auto user = currentUserName();
            emit loginFailed(user);
        });
        connect(AuthManager::instance(), &AuthManager::authSuccessed, this, [this] () {
            const auto user = currentUserName();
            emit loginSuccessed(user);
        });
    } else {
        connect(this, &AuthApplet::rootObjectChanged, [this] () {
            const auto user = currentUserName();
            AuthManager::instance()->setUser(user);

            connect(AuthManager::instance(), &AuthManager::authFailed, this, [this] () {
                const auto user = currentUserName();
                emit loginFailed(user);
            });
            connect(AuthManager::instance(), &AuthManager::authSuccessed, this, [this] () {
                const auto user = currentUserName();
                emit loginSuccessed(user);
            });
        });
    }

    return DContainment::init();
}

void AuthApplet::requestAuthentication(bool active)
{
    if (active) {
        AuthManager::instance()->requestCreate();
        AuthManager::instance()->requestStart();
    } else {
        AuthManager::instance()->requestDestroy();
    }
}

AuthApplet::~AuthApplet()
{
}

QObject *AuthApplet::greeter() const
{
    DAppletBridge bridge("org.deepin.ds.greeter");
    return bridge.applet();
    return nullptr;
}

QObject *AuthApplet::greeterProxy() const
{
    if (auto applet = greeter()) {
        return applet->property("proxy").value<QObject *>();
    }
    return nullptr;
}

QObject *AuthApplet::userModel() const
{
    if (auto proxy = greeterProxy()) {
        return proxy->property("userModel").value<QObject *>();
    }
    return nullptr;
}

QString AuthApplet::currentUserName() const
{
    if (auto proxy = userModel()) {
        return proxy->property("currentUserName").toString();
    }
    return {};
}

D_APPLET_CLASS(AuthApplet)

#include "authapplet.moc"
