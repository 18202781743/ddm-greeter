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
    DAppletBridge bridge("org.deepin.ds.greeter.ddm");
    if (auto applet = bridge.applet()) {
        AuthManager::instance()->setAuthFrame(AuthInterface::AF_DDM);
        AuthManager::instance()->setDDMAuthImpl(applet);
    } else {
        AuthManager::instance()->setAuthFrame(AuthInterface::AF_DA);
    }
    return DContainment::load();
}

bool AuthApplet::init()
{
    if (rootObject()) {
        initAuth();
    } else {
        connect(this, &AuthApplet::rootObjectChanged, [this] () {
            initAuth();
        });
    }

    return DContainment::init();
}

void AuthApplet::activateUser(const QString &user)
{
    qDebug() << "Activate user" << user;
    requestAuthentication(false);
    AuthManager::instance()->setUser(user);
    requestAuthentication(true);
}

void AuthApplet::requestAuthentication(bool active)
{
    qDebug() << "Request auth" << active;
    if (active) {
        AuthManager::instance()->requestCreate();
        AuthManager::instance()->requestStart();
    } else {
        AuthManager::instance()->requestDestroy();
    }
}

void AuthApplet::initAuth()
{
    const auto user = userName();
    AuthManager::instance()->setUser(user);

    connect(AuthManager::instance(), &AuthManager::authFailed, this, [this] () {
        const auto user = userName();
        emit loginFailed(user);
    });
    connect(AuthManager::instance(), &AuthManager::authSuccessed, this, [this] () {
        const auto user = userName();
        emit loginSuccessed(user);
    });

    if (auto proxy = greeter()) {
        QObject::connect(proxy, SIGNAL(authActiveChanged(bool)), this, SLOT(requestAuthentication(bool)));
        requestAuthentication(active());
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

QString AuthApplet::userName() const
{
    if (auto proxy = userModel()) {
        return proxy->property("currentUserName").toString();
    }
    return {};
}

bool AuthApplet::active() const
{
    if (auto proxy = greeter()) {
        return proxy->property("authActive").toBool();
    }
    return false;
}

D_APPLET_CLASS(AuthApplet)

#include "authapplet.moc"
