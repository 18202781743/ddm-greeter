// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "greeterauthinterface.h"
#include "deepinauthframework.h"
#include "dbuslockservice.h"
#include "login1sessionself_interface.h"

using Login1SessionSelf = org::freedesktop::login1::Session;
class DBusLogin1Manager;
class DAAuth : public GreeterAuthInterface
{
    Q_OBJECT
public:
    DAAuth(QObject *parent = nullptr);

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

public:
    void destroyAuthentication();
    void endAuthentication();
    void createAuthentication();

    void startAuthentication();
    void activeAuthentication(bool active);
    void activeAuthentication(const QString &user, bool active);

private Q_SLOTS:
    void connected();
    void disconnected();
    void readyRead();
    void error();

    void onAuthStateChanged(const int type, const int state, const QString &message);
Q_SIGNALS:
    void informationMessage(const QString &message);
    void socketDisconnected();

    void loginFailed(const QString &user);
    void loginSucceeded(const QString &user);

private:
    DBusLogin1Manager *m_login1Inter{ nullptr };
    DeepinAuthFramework *m_authFramework{ nullptr };
    DBusLockService *m_lockInter{ nullptr };
    Login1SessionSelf *m_login1SessionSelf{ nullptr };
    QString m_currentUserName;
};
