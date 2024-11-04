// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "greeterauthinterface.h"

#include <applet.h>

DS_USE_NAMESPACE

class QLocalSocket;
class DDMAuth : public GreeterAuthInterface
{
    Q_OBJECT
public:
    DDMAuth(QObject *parent = nullptr);

    bool init();

    bool isConnected() const;

    void powerOff() override;
    void reboot() override;
    void suspend() override;
    void hibernate() override;
    void hybridSleep() override;

    void login(const QString &user, const QString &password, const QVariantMap &session) const override;
    void activateUser(const QString &user)  override;
    void unlock(const QString &user, const QString &password) override;

private Q_SLOTS:
    void connected();
    void disconnected();
    void readyRead();
    void error();

Q_SIGNALS:
    void informationMessage(const QString &message);
    void socketDisconnected();

    void loginFailed(const QString &user);
    void loginSucceeded(const QString &user);
    void switchToGreeter();

private:
    QLocalSocket *socket{ nullptr };
};

class DDMAuthApplet : public DApplet
{
    Q_OBJECT
    Q_PROPERTY(GreeterAuthInterface* auth READ auth CONSTANT FINAL)
public:
    explicit DDMAuthApplet(QObject *parent = nullptr);
    ~DDMAuthApplet();

    GreeterAuthInterface *auth();

    virtual bool load() override;

private:
    GreeterAuthInterface *m_auth = nullptr;
};
