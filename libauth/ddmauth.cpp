// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ddmauth.h"

#include <QLocalSocket>
#include <QLoggingCategory>

#include <Messages.h>
#include <SocketWriter.h>

using namespace DDM;

void DDMAuth::setSocket(QLocalSocket *socket)
{
    m_socket = socket;
}

void DDMAuth::send(const QString &password)
{
    QVariantMap session;
    Session::Type type =
        static_cast<Session::Type>(session["type"].toInt());
    const QString name = session["name"].toString();
    Session sessionInfo(type, name);
    SocketWriter(m_socket) << quint32(GreeterMessages::Login) << m_user << password << sessionInfo;
}
