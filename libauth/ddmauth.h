// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once


#include "authinterface.h"

#include <QPointer>

class DDMAuth : public AuthInterface
{
    Q_OBJECT
public:
    DDMAuth(QObject *parent = nullptr);

    void setImpl(QObject *impl);

    virtual void send(const QString &token);

private slots:
    void onLoginFailed(const QString &user);
    void onLoginSuccessed(const QString &user);

private:
    QPointer<QObject> m_impl;
};
