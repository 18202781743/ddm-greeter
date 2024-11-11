// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once


#include "authinterface.h"

#include <QPointer>

class QLocalSocket;
class DDMAuth : public AuthInterface
{
    Q_OBJECT
public:
    DDMAuth(QObject *parent = nullptr);

    void setSocket(QLocalSocket *socket);

    virtual void send(const QString &token);

private:
    QPointer<QLocalSocket> m_socket;
};
