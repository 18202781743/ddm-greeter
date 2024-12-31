// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ddmauth.h"
#include "authmanager.h"

#include <QLocalSocket>
#include <QLoggingCategory>

#include <Messages.h>
#include <SocketWriter.h>

using namespace DDM;

DDMAuth::DDMAuth(QObject *parent)
    : AuthInterface(parent)
{
}

void DDMAuth::setImpl(QObject *impl)
{
    m_impl = impl;

    connect(m_impl, SIGNAL(loginFailed(const QString &)), this, SLOT(onLoginFailed(const QString &)));
    connect(m_impl, SIGNAL(loginSucceeded(const QString &)), this, SLOT(onLoginSuccessed(const QString &)));
}

void DDMAuth::send(const QString &password)
{
    const auto session = AuthManager::instance()->session();

    QMetaObject::invokeMethod(m_impl, "login", Qt::DirectConnection,
                              Q_ARG(const QString &, m_user),
                              Q_ARG(const QString &, password),
                              Q_ARG(const QVariantMap &, session));
}

void DDMAuth::onLoginFailed(const QString &user)
{
    setState(AuthInterface::AS_Failure);
}

void DDMAuth::onLoginSuccessed(const QString &user)
{
    setState(AuthInterface::AS_Success);
}