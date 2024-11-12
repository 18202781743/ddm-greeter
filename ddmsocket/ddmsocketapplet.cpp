// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ddmsocketapplet.h"

#include <QCoreApplication>
#include <QLocalSocket>
#include <QLoggingCategory>

#include <Messages.h>
#include <SocketWriter.h>

#include <pluginfactory.h>

using namespace DDM;

bool DDMSocketApplet::isConnected() const
{
    return m_socket->state() == QLocalSocket::ConnectedState;
}

void DDMSocketApplet::connected()
{
    qDebug() << "Connected to the daemon.";

    SocketWriter(m_socket) << quint32(GreeterMessages::Connect);
}

void DDMSocketApplet::disconnected()
{
    qDebug() << "Disconnected from the daemon.";

    Q_EMIT socketDisconnected();
}

void DDMSocketApplet::error()
{
    qCritical() << "Socket error: " << m_socket->errorString();
}

void DDMSocketApplet::powerOff()
{
    SocketWriter(m_socket) << quint32(GreeterMessages::PowerOff);
}

void DDMSocketApplet::reboot()
{
    SocketWriter(m_socket) << quint32(GreeterMessages::Reboot);
}

void DDMSocketApplet::suspend()
{
    SocketWriter(m_socket) << quint32(GreeterMessages::Suspend);
}

void DDMSocketApplet::hibernate()
{
    SocketWriter(m_socket) << quint32(GreeterMessages::Hibernate);
}

void DDMSocketApplet::hybridSleep()
{
    SocketWriter(m_socket) << quint32(GreeterMessages::HybridSleep);
}

QString DDMSocketApplet::hostName() const
{
    return m_hostName;
}

int DDMSocketApplet::capabilities() const
{
    return m_capabilities;
}

QLocalSocket *DDMSocketApplet::socket()
{
    return m_socket;
}

void DDMSocketApplet::readyRead()
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

QLocalSocket *DDMSocketApplet::createSocket()
{
    const QStringList args = QCoreApplication::arguments();
     QString server;
     int pos;

     if ((pos = args.indexOf(QStringLiteral("--socket"))) >= 0) {
         if (pos >= args.length() - 1) {
             qWarning() << "The startup parameter is missing --socket.";
             return nullptr;
         }
         server = args[pos + 1];
     } else {
         return nullptr;
     }
     std::unique_ptr<QLocalSocket> socket(new QLocalSocket(this));
     // connect signals
     connect(socket.get(), &QLocalSocket::connected, this, &DDMSocketApplet::connected);
     connect(socket.get(), &QLocalSocket::disconnected, this, &DDMSocketApplet::disconnected);
     connect(socket.get(), &QLocalSocket::readyRead, this, &DDMSocketApplet::readyRead);
     connect(socket.get(), &QLocalSocket::errorOccurred, this, &DDMSocketApplet::error);

     // connect to server
     socket->connectToServer(server);
     if (!socket->isValid()) {
         qWarning() << "Can't connect to service." << server << ", error:" << m_socket->error();
         return nullptr;
     }
     return socket.release();
}

DDMSocketApplet::DDMSocketApplet(QObject *parent)
    : DApplet(parent)
{
    m_socket = createSocket();
}

bool DDMSocketApplet::load()
{
    if (!m_socket)
        return false;

    return DApplet::load();
}

DDMSocketApplet::~DDMSocketApplet()
{
}

D_APPLET_CLASS(DDMSocketApplet)

#include "ddmsocketapplet.moc"
