// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ddmauthapplet.h"

#include <QGuiApplication>
#include <QLocalSocket>
#include <QLoggingCategory>

#include <Messages.h>
#include <SocketWriter.h>

#include <pluginfactory.h>

Q_LOGGING_CATEGORY(dmgAuth, "ddm.greeter.auth", QtDebugMsg);

using namespace DDM;

bool DDMAuth::isConnected() const
{
    return socket->state() == QLocalSocket::ConnectedState;
}

void DDMAuth::powerOff()
{
    SocketWriter(socket) << quint32(GreeterMessages::PowerOff);
}

void DDMAuth::reboot()
{
    SocketWriter(socket) << quint32(GreeterMessages::Reboot);
}

void DDMAuth::suspend()
{
    SocketWriter(socket) << quint32(GreeterMessages::Suspend);
}

void DDMAuth::hibernate()
{
    SocketWriter(socket) << quint32(GreeterMessages::Hibernate);
}

void DDMAuth::hybridSleep()
{
    SocketWriter(socket) << quint32(GreeterMessages::HybridSleep);
}

void DDMAuth::login(const QString &user, const QString &password, const QVariantMap &session) const
{
    Session::Type type =
        static_cast<Session::Type>(session["type"].toInt());
    const QString name = session["name"].toString();
    Session sessionInfo(type, name);
    SocketWriter(socket) << quint32(GreeterMessages::Login) << user << password << sessionInfo;
}

void DDMAuth::unlock(const QString &user, const QString &password)
{
    SocketWriter(socket) << quint32(GreeterMessages::Unlock) << user << password;
}

void DDMAuth::activateUser(const QString &user)
{
    SocketWriter(socket) << quint32(GreeterMessages::ActivateUser) << user;
}

void DDMAuth::connected()
{
    qCDebug(dmgAuth) << "Connected to the daemon.";

    SocketWriter(socket) << quint32(GreeterMessages::Connect);
}

void DDMAuth::disconnected()
{
    qCDebug(dmgAuth) << "Disconnected from the daemon.";

    Q_EMIT socketDisconnected();
}

void DDMAuth::error()
{
    qCCritical(dmgAuth) << "Socket error: " << socket->errorString();
}

void DDMAuth::readyRead()
{
    // input stream
    QDataStream input(socket);

    while (input.device()->bytesAvailable()) {
        // read message
        quint32 message;
        input >> message;

        switch (DaemonMessages(message)) {
        case DaemonMessages::Capabilities: {
            // log message
            qCDebug(dmgAuth) << "Message received from daemon: Capabilities";

            // read capabilities
            quint32 capabilities;
            input >> capabilities;

            // parse capabilities
            m_canPowerOff = capabilities & Capability::PowerOff;
            m_canReboot = capabilities & Capability::Reboot;
            m_canSuspend = capabilities & Capability::Suspend;
            m_canHibernate = capabilities & Capability::Hibernate;
            m_canHybridSleep = capabilities & Capability::HybridSleep;

            // emit signals
            Q_EMIT canPowerOffChanged(m_canPowerOff);
            Q_EMIT canRebootChanged(m_canReboot);
            Q_EMIT canSuspendChanged(m_canSuspend);
            Q_EMIT canHibernateChanged(m_canHibernate);
            Q_EMIT canHybridSleepChanged(m_canHybridSleep);
        } break;
        case DaemonMessages::HostName: {
            qCDebug(dmgAuth) << "Message received from daemon: HostName";

            // read host name
            input >> m_hostName;

            // emit signal
            Q_EMIT hostNameChanged(m_hostName);
        } break;
        case DaemonMessages::LoginSucceeded: {
            QString user;
            input >> user;

            qCDebug(dmgAuth) << "Message received from daemon: LoginSucceeded:" << user;

            Q_EMIT loginSucceeded(user);
        } break;
        case DaemonMessages::LoginFailed: {
            QString user;
            input >> user;

            qCDebug(dmgAuth) << "Message received from daemon: LoginFailed" << user;

            Q_EMIT loginFailed(user);
        } break;
        case DaemonMessages::InformationMessage: {
            QString message;
            input >> message;

            qCDebug(dmgAuth) << "Information Message received from daemon: " << message;
            Q_EMIT informationMessage(message);
        } break;
        case DaemonMessages::SwitchToGreeter: {
            qCInfo(dmgAuth) << "switch to greeter";
            Q_EMIT switchToGreeter();
        } break;
        case DaemonMessages::UserActivateMessage: {
            QString user;
            input >> user;

            qCInfo(dmgAuth) << "activate successfully: " << user;
        } break;
        default: {
            qCWarning(dmgAuth) << "Unknown message received from daemon." << message;
        }
        }
    }
}

DDMAuth::DDMAuth(QObject *parent)
    : GreeterAuthInterface(parent)
{
}

bool DDMAuth::init()
{
    const QStringList args = QCoreApplication::arguments();
    QString server;
    int pos;

    if ((pos = args.indexOf(QStringLiteral("--socket"))) >= 0) {
        if (pos >= args.length() - 1) {
            qCWarning(dmgAuth) << "The startup parameter is missing --socket.";
            return false;
        }
        server = args[pos + 1];
    } else {
        return false;
    }
    socket = new QLocalSocket(this);
    // connect signals
    connect(socket, &QLocalSocket::connected, this, &DDMAuth::connected);
    connect(socket, &QLocalSocket::disconnected, this, &DDMAuth::disconnected);
    connect(socket, &QLocalSocket::readyRead, this, &DDMAuth::readyRead);
    connect(socket, &QLocalSocket::errorOccurred, this, &DDMAuth::error);

    // connect to server
    socket->connectToServer(server);
    if (!socket->isValid()) {
        qCWarning(dmgAuth) << "Can't connect to service." << server << ", error:" << socket->error();
        return false;
    }
    return true;
}

DDMAuthApplet::DDMAuthApplet(QObject *parent)
    : DApplet(parent)
{
}

bool DDMAuthApplet::load()
{
    std::unique_ptr<DDMAuth> auth(new DDMAuth());
    if (!auth->init()) {
        return false;
    }
    m_auth = auth.release();
    return DApplet::load();
}

DDMAuthApplet::~DDMAuthApplet()
{
}

GreeterAuthInterface *DDMAuthApplet::auth()
{
    return m_auth;
}

D_APPLET_CLASS(DDMAuthApplet)

#include "ddmauthapplet.moc"
