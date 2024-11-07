// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "daauthapplet.h"
#include "daauth.h"

#include <QGuiApplication>
#include <QLocalSocket>
#include <QLoggingCategory>

#include <appletbridge.h>
#include <pluginfactory.h>

DAAuthApplet::DAAuthApplet(QObject *parent)
    : DApplet(parent)
{
}

bool DAAuthApplet::load()
{
    std::unique_ptr<DAAuth> auth(new DAAuth());
    if (!auth->init()) {
        return false;
    }
    m_auth = auth.release();
    return DApplet::load();
}

bool DAAuthApplet::init()
{
    if (auto proxy = greeterProxy()) {
        QObject::connect(proxy, SIGNAL(visibleChanged(bool)), this, SLOT(requestAuthentication(bool)));
    }

    return DApplet::init();
}

void DAAuthApplet::requestAuthentication(bool enable)
{
    qDebug() << "Request authentication" << enable;
    m_auth->activeAuthentication(enable);
}

DAAuthApplet::~DAAuthApplet()
{
}

GreeterAuthInterface *DAAuthApplet::auth()
{
    return m_auth;
}

DAppletProxy *DAAuthApplet::greeterProxy() const
{
    DAppletBridge bridge("org.deepin.ds.greeter");
    return bridge.applet();
}

D_APPLET_CLASS(DAAuthApplet)

#include "daauthapplet.moc"
