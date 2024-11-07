// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "lockapplet.h"
#include "dbuslockfrontservice.h"

#include <QGuiApplication>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QLoggingCategory>

#include <pluginfactory.h>
#include <appletbridge.h>

static const QString DBUS_LOCK_PATH = "/org/deepin/dde/LockFront1";
static const QString DBUS_LOCK_NAME = "org.deepin.dde.LockFront1";

LockApplet::LockApplet(QObject *parent)
    : DApplet(parent)
{
}

bool LockApplet::load()
{
    auto service = new DBusLockFrontService(this);
    QDBusConnection conn = QDBusConnection::sessionBus();
    if (conn.interface()->registerService(DBUS_LOCK_NAME,
                             QDBusConnectionInterface::ReplaceExistingService,
                             QDBusConnectionInterface::AllowReplacement)) {
        if (conn.registerObject(DBUS_LOCK_PATH, this)) {
            return true;
        } else {
            qDebug() << "Failed to register object" << DBUS_LOCK_PATH;
        }
    } else {
        qDebug() << "Failed to register service" << DBUS_LOCK_NAME;
    }
    return false;
}

bool LockApplet::init()
{
    return DApplet::init();
}

bool LockApplet::Visible()
{
    bool ret = false;
    if (auto proxy = greeterProxy()) {
        QMetaObject::invokeMethod(proxy, "visible", Qt::DirectConnection, Q_RETURN_ARG(bool, ret));
    }
    return ret;
}

void LockApplet::Show()
{
    if (auto proxy = greeterProxy()) {
        QMetaObject::invokeMethod(proxy, "show", Qt::DirectConnection);
    }
}

void LockApplet::ShowUserList()
{
    if (auto proxy = greeterProxy()) {
        QMetaObject::invokeMethod(proxy, "ShowUserList", Qt::DirectConnection);
    }
}

void LockApplet::ShowAuth(bool active)
{
    if (auto proxy = greeterProxy()) {
        QMetaObject::invokeMethod(proxy, "showAuth", Qt::DirectConnection, Q_ARG(bool, active));
    }
}

void LockApplet::Suspend(bool enable)
{
    if (auto proxy = greeterProxy()) {
        QMetaObject::invokeMethod(proxy, "Suspend", Qt::DirectConnection, Q_ARG(bool, enable));
    }
}

void LockApplet::Hibernate(bool enable)
{
    if (auto proxy = greeterProxy()) {
        QMetaObject::invokeMethod(proxy, "Hibernate", Qt::DirectConnection, Q_ARG(bool, enable));
    }
}

DAppletProxy *LockApplet::greeterProxy() const
{
    DAppletBridge bridge("org.deepin.ds.greeter");
    return bridge.applet();
}

LockApplet::~LockApplet()
{
}

D_APPLET_CLASS(LockApplet)

#include "lockapplet.moc"
