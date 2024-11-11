// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "lockapplet.h"
// #include "dbuslockfrontservice.h"

#include "dbuslockagent.h"
#include "dbuslockfrontservice.h"
#include "dbusshutdownagent.h"
#include "dbusshutdownfrontservice.h"
#include "lockframe.h"
#include "lockworker.h"
#include "sessionbasemodel.h"

#include <DApplication>
#include <DGuiApplicationHelper>
#include <DLog>
#include <DPlatformTheme>

#include <QDBusInterface>

#include <QGuiApplication>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QLoggingCategory>

#include <pluginfactory.h>
#include <appletbridge.h>

// static const QString DBUS_LOCK_PATH = "/org/deepin/dde/LockFront1";
// static const QString DBUS_LOCK_NAME = "org.deepin.dde.LockFront1";

LockApplet::LockApplet(QObject *parent)
    : DApplet(parent)
{
}

bool LockApplet::load()
{
    SessionBaseModel *m_sessionModel = new SessionBaseModel();
    m_sessionModel->setAppType(Lock);
    LockWorker *worker = new LockWorker(m_sessionModel);

    DBusLockAgent *m_lockAgent = new DBusLockAgent();
    m_lockAgent->setModel(m_sessionModel);
    new DBusLockFrontService(m_lockAgent);

    DBusShutdownAgent *m_shutdownAgent = new DBusShutdownAgent();
    m_shutdownAgent->setModel(m_sessionModel);
    new DBusShutdownFrontService(m_shutdownAgent);

    bool showUserList = false;
    bool showShutdown = false;
    bool showLockScreen = true;

    QDBusConnection conn = QDBusConnection::sessionBus();
    int ret = 0;
    if (!conn.registerService(DBUS_LOCK_NAME) ||
            !conn.registerObject(DBUS_LOCK_PATH, m_lockAgent) ||
            !conn.registerService(DBUS_SHUTDOWN_NAME) ||
            !conn.registerObject(DBUS_SHUTDOWN_PATH, m_shutdownAgent)) {
        qDebug() << "register dbus failed"<< "maybe lockFront is running..." << conn.lastError();

        const char *lockFrontInter = "org.deepin.dde.LockFront1";
        const char *shutdownFrontInter = "org.deepin.dde.ShutdownFront1";
        if (showUserList) {
            QDBusInterface ifc(DBUS_LOCK_NAME, DBUS_LOCK_PATH, lockFrontInter, QDBusConnection::sessionBus(), nullptr);
            ifc.asyncCall("ShowUserList");
        } else if (showShutdown) {
            QDBusInterface ifc(DBUS_SHUTDOWN_NAME, DBUS_SHUTDOWN_PATH, shutdownFrontInter, QDBusConnection::sessionBus(), nullptr);
            ifc.asyncCall("Show");
        } else if (showLockScreen) {
            QDBusInterface ifc(DBUS_LOCK_NAME, DBUS_LOCK_PATH, lockFrontInter, QDBusConnection::sessionBus(), nullptr);
            ifc.asyncCall("Show");
        }
    } else {
        if (showUserList) {
            emit m_sessionModel->showUserList();
        } else if (showShutdown) {
            emit m_sessionModel->showShutdown();
        } else if (showLockScreen) {
            emit m_sessionModel->showLockScreen();
        }
    }

    if (auto proxy = greeterProxy()) {
        QObject::connect(proxy, SIGNAL(rootObjectChanged()), this, SLOT(onFrameChanged()));
    }

    // auto service = new DBusLockFrontService(this);
    // QDBusConnection conn = QDBusConnection::sessionBus();
    // if (conn.interface()->registerService(DBUS_LOCK_NAME,
    //                          QDBusConnectionInterface::ReplaceExistingService,
    //                          QDBusConnectionInterface::AllowReplacement)) {
    //     if (conn.registerObject(DBUS_LOCK_PATH, this)) {
    //         return true;
    //     } else {
    //         qDebug() << "Failed to register object" << DBUS_LOCK_PATH;
    //     }
    // } else {
    //     qDebug() << "Failed to register service" << DBUS_LOCK_NAME;
    // }
    return true;
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

void LockApplet::onFrameChanged()
{
    // // lockFrame->setScreen(screen, count <= 0);
    // QObject::connect(lockFrame, &LockFrame::requestSwitchToUser, m_worker, &LockWorker::switchToUser);
    // // QObject::connect(model, &SessionBaseModel::visibleChanged, lockFrame, &LockFrame::setVisible);
    // QObject::connect(m_sessionModel, &SessionBaseModel::visibleChanged, lockFrame,[&](bool visible) {
    //     emit lockService.Visible(visible);
    // });
    // QObject::connect(lockFrame, &LockFrame::requestSetKeyboardLayout, worker, &LockWorker::setKeyboardLayout);
    // QObject::connect(lockFrame, &LockFrame::requestEnableHotzone, worker, &LockWorker::enableZoneDetected, Qt::UniqueConnection);
    // QObject::connect(lockFrame, &LockFrame::sendKeyValue, [&](QString key) {
    //     emit lockService.ChangKey(key);
    // });
    // QObject::connect(lockFrame, &LockFrame::requestStartAuthentication, m_worker, &LockWorker::startAuthentication);
    // QObject::connect(lockFrame, &LockFrame::sendTokenToAuth, m_worker, &LockWorker::sendTokenToAuth);
    // QObject::connect(lockFrame, &LockFrame::requestEndAuthentication, m_worker, &LockWorker::endAccountAuthentication);
    // QObject::connect(lockFrame, &LockFrame::authFinished, m_worker, &LockWorker::onAuthFinished);

    // QObject::connect(lockFrame, &LockFrame::requestCheckAccount, m_worker, &LockWorker::checkAccount);
    // // lockFrame->setVisible(model->visible());
    // emit lockService.Visible(true);
}

QObject *LockApplet::frame() const
{
    if (auto proxy = greeterProxy()) {
        return proxy->property("rootObject").value<QObject *>();
    }
    return nullptr;
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
