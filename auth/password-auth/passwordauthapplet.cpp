// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "passwordauthapplet.h"
#include "authmanager.h"

#include <QGuiApplication>
#include <QLocalSocket>
#include <QLoggingCategory>

#include <appletbridge.h>
#include <pluginfactory.h>

PasswordAuthApplet::PasswordAuthApplet(QObject *parent)
    : DApplet(parent)
{
}

bool PasswordAuthApplet::load()
{
    std::unique_ptr<AuthInterface> auth(AuthManager::instance()->create(AuthInterface::AT_Password));
    if (!auth->init()) {
        return false;
    }
    m_auth = auth.release();
    return DApplet::load();
}

bool PasswordAuthApplet::init()
{
    return DApplet::init();
}

PasswordAuthApplet::~PasswordAuthApplet()
{

}

AuthInterface *PasswordAuthApplet::auth()
{
    return m_auth;
}

DAppletProxy *PasswordAuthApplet::greeterProxy() const
{
    DAppletBridge bridge("org.deepin.ds.greeter");
    return bridge.applet();
}

D_APPLET_CLASS(PasswordAuthApplet)

#include "passwordauthapplet.moc"
