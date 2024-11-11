// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "powerapplet.h"
#include "dapower.h"

#include <QLoggingCategory>

#include <pluginfactory.h>

PowerApplet::PowerApplet(QObject *parent)
    : DApplet(parent)
{
}

bool PowerApplet::init()
{
    m_proxy = new DAPower(this);

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
