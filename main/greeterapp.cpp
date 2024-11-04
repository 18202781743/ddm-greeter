// Copyright (C) 2023 justforlxz <justforlxz@gmail.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "greeterapp.h"
#include "greeterauthinterface.h"
#include "usermodel.h"

#include <QLoggingCategory>

#include <pluginfactory.h>
#include <applet.h>
#include <appletbridge.h>

DS_USE_NAMESPACE

GreeterPanel::GreeterPanel(QObject *parent)
    : DPanel(parent)
    , m_proxy(new GreeterProxy(this))
{
    QObject::connect(m_proxy, &GreeterProxy::localeChanged, this, &GreeterPanel::localeChanged);
}

bool GreeterPanel::load()
{
    return DPanel::load();
}

bool GreeterPanel::init()
{
    DAppletBridge bridge("org.deepin.ds.greeter.auth");
    if (auto applet = bridge.applet()) {
        auto containment = reinterpret_cast<DContainment *>(applet);
        for (auto item : containment->applets()) {
            if (auto auth = item->property("auth").value<GreeterAuthInterface *>()) {
                m_proxy->setAuth(auth);
                qDebug() << "Using authentication applet:" << item->pluginId();
                break;
            }
        }
    }

    m_proxy->init();

    return DPanel::init();
}

GreeterProxy *GreeterPanel::proxy() const
{
    return m_proxy;
}

void GreeterPanel::show()
{
    m_visible = true;
    emit visibleChanged(m_visible);
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

D_APPLET_CLASS(GreeterPanel)

#include "greeterapp.moc"
