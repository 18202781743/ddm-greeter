// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "powerapplet.h"
#include "dapower.h"
#include "ddmpower.h"

#include <QLoggingCategory>

#include <QLocalSocket>
#include <appletbridge.h>
#include <pluginfactory.h>

PowerApplet::PowerApplet(QObject *parent)
    : DApplet(parent)
{
}

bool PowerApplet::init()
{
    DAppletBridge bridge("org.deepin.ds.greeter.ddmsocket");
    if (auto applet = bridge.applet()) {
        auto ddm = new DDMPower(this);
        ddm->setImpl(applet);
        m_proxy = ddm;
    } else {
        m_proxy = new DAPower(this);
    }

    return DApplet::init();
}

PowerApplet::~PowerApplet()
{
    if (m_proxy)
        m_proxy->deleteLater();
}

PowerInterface *PowerApplet::proxy() const
{
    return m_proxy;
}

D_APPLET_CLASS(PowerApplet)

#include "powerapplet.moc"
