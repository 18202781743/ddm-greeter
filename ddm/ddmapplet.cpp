// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ddmapplet.h"

#include <QCoreApplication>
#include <QLocalSocket>
#include <QLoggingCategory>

#include <Messages.h>
#include <SocketWriter.h>

#include <pluginfactory.h>

using namespace DDM;

bool DDMApplet::isConnected() const
{
    return m_socket->state() == QLocalSocket::ConnectedState;
}

void DDMApplet::connected()
{
    qDebug() << "Connected to the daemon.";

    SocketWriter(m_socket) << quint32(GreeterMessages::Connect);
}

void DDMApplet::disconnected()
{
    qDebug() << "Disconnected from the daemon.";

    Q_EMIT socketDisconnected();
}

void DDMApplet::error()
{
    qCritical() << "Socket error: " << m_socket->errorString();
}

void DDMApplet::powerOff()
{
    SocketWriter(m_socket) << quint32(GreeterMessages::PowerOff);
}

void DDMApplet::reboot()
{
    SocketWriter(m_socket) << quint32(GreeterMessages::Reboot);
}

void DDMApplet::suspend()
{
    SocketWriter(m_socket) << quint32(GreeterMessages::Suspend);
}

void DDMApplet::hibernate()
{
    SocketWriter(m_socket) << quint32(GreeterMessages::Hibernate);
}

void DDMApplet::hybridSleep()
{
    SocketWriter(m_socket) << quint32(GreeterMessages::HybridSleep);
}

QString DDMApplet::hostName() const
{
    return m_hostName;
}

int DDMApplet::capabilities() const
{
    return m_capabilities;
}

void DDMApplet::login(const QString &user, const QString &password, const QVariantMap &session) const
{
    Session::Type type =
        static_cast<Session::Type>(session["type"].toInt());
    const QString name = session["name"].toString();
    Session sessionInfo(type, name);
    SocketWriter(m_socket) << quint32(GreeterMessages::Login) << user << password << sessionInfo;

}

void DDMApplet::activateUser(const QString &user)
{
    SocketWriter(m_socket) << quint32(GreeterMessages::ActivateUser) << user;
}

void DDMApplet::unlock(const QString &user, const QString &password)
{
    SocketWriter(m_socket) << quint32(GreeterMessages::Unlock) << user << password;
}

void DDMApplet::readyRead()
{
    // input stream
    QDataStream input(m_socket);

    while (input.device()->bytesAvailable()) {
        // read message
        quint32 message;
        input >> message;

        switch (DaemonMessages(message)) {
        case DaemonMessages::Capabilities: {
            qDebug() << "Message received from daemon: Capabilities";

            // read capabilities
            quint32 capabilities;
            input >> capabilities;

            Q_EMIT capabilitiesChanged(capabilities);
        } break;
        case DaemonMessages::HostName: {
            qDebug() << "Message received from daemon: HostName";

            // read host name
            input >> m_hostName;

            // emit signal
            Q_EMIT hostNameChanged(m_hostName);
        } break;
        case DaemonMessages::LoginSucceeded: {
            QString user;
            input >> user;

            qDebug() << "Message received from daemon: LoginSucceeded:" << user;

            Q_EMIT loginSucceeded(user);
        } break;
        case DaemonMessages::LoginFailed: {
            QString user;
            input >> user;

            qDebug() << "Message received from daemon: LoginFailed" << user;

            Q_EMIT loginFailed(user);
        } break;
        case DaemonMessages::InformationMessage: {
            QString message;
            input >> message;

            qDebug() << "Information Message received from daemon: " << message;
            Q_EMIT informationMessage(message);
        } break;
        case DaemonMessages::SwitchToGreeter: {
            qInfo() << "switch to greeter";
            Q_EMIT switchToGreeter();
        } break;
        case DaemonMessages::UserActivateMessage: {
            QString user;
            input >> user;

            Q_EMIT userActivateMessage(user);
            qInfo() << "activate successfully: " << user;
        } break;
        default: {
            qWarning() << "Unknown message received from daemon." << message;
        }
        }
    }
}

bool DDMApplet::initSocket()
{
    const QStringList args = QCoreApplication::arguments();
     QString server;
     int pos;

     if ((pos = args.indexOf(QStringLiteral("--socket"))) >= 0) {
         if (pos >= args.length() - 1) {
             qWarning() << "The startup parameter is missing --socket.";
             return false;
         }
         server = args[pos + 1];
     } else {
         return false;
     }
     m_socket = new QLocalSocket(this);

     // connect signals
     connect(m_socket, &QLocalSocket::connected, this, &DDMApplet::connected);
     connect(m_socket, &QLocalSocket::disconnected, this, &DDMApplet::disconnected);
     connect(m_socket, &QLocalSocket::readyRead, this, &DDMApplet::readyRead);
     connect(m_socket, &QLocalSocket::errorOccurred, this, &DDMApplet::error);

     // connect to server
     m_socket->connectToServer(server);
     if (!m_socket->isValid()) {
         qWarning() << "Can't connect to service." << server << ", error:" << m_socket->error();
         m_socket->deleteLater();
         m_socket = nullptr;
         return false;
     }
     return true;
}

DDMApplet::DDMApplet(QObject *parent)
    : DApplet(parent)
{
}

bool DDMApplet::load()
{
    initSocket();
    if (!m_socket)
        return false;

    return DApplet::load();
}

DDMApplet::~DDMApplet()
{
    if (m_socket)
        m_socket->deleteLater();
}

D_APPLET_CLASS(DDMApplet)

#include "ddmapplet.moc"
