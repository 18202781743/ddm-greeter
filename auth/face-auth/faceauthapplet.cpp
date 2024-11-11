// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "faceauthapplet.h"
#include "authmanager.h"

#include <QGuiApplication>
#include <QLoggingCategory>

#include <appletbridge.h>
#include <pluginfactory.h>

FaceAuthApplet::FaceAuthApplet(QObject *parent)
    : DApplet(parent)
{
}

bool FaceAuthApplet::load()
{
    std::unique_ptr<AuthInterface> auth(AuthManager::instance()->create(AuthInterface::AT_Face));
    if (!auth->init()) {
        return false;
    }
    m_auth = auth.release();
    return DApplet::load();
}

bool FaceAuthApplet::init()
{
    return DApplet::init();
}

FaceAuthApplet::~FaceAuthApplet()
{

}

AuthInterface *FaceAuthApplet::auth()
{
    return m_auth;
}

DAppletProxy *FaceAuthApplet::greeterProxy() const
{
    DAppletBridge bridge("org.deepin.ds.greeter");
    return bridge.applet();
}

D_APPLET_CLASS(FaceAuthApplet)

#include "faceauthapplet.moc"
