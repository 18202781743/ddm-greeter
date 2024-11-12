// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <applet.h>

DS_USE_NAMESPACE

class QLocalSocket;
class DDMSocketApplet : public DApplet
{
    Q_OBJECT
public:
    explicit DDMSocketApplet(QObject *parent = nullptr);
    ~DDMSocketApplet();

    bool load() override;

    bool isConnected() const;
    void connected();
    void disconnected();
    void error();

    Q_INVOKABLE void powerOff();
    Q_INVOKABLE void reboot();
    Q_INVOKABLE void suspend();
    Q_INVOKABLE void hibernate();
    Q_INVOKABLE void hybridSleep();

    Q_INVOKABLE QString hostName() const;
    Q_INVOKABLE int capabilities() const;

    Q_INVOKABLE void login(const QString &user, const QString &password, const QVariantMap &session) const;
    Q_INVOKABLE void activateUser(const QString &user);
    Q_INVOKABLE void unlock(const QString &user, const QString &password);

signals:
    void capabilitiesChanged(int capabilities);
    void hostNameChanged(const QString &hostName);
    void loginSucceeded(const QString &user);
    void loginFailed(const QString &user);
    void informationMessage(const QString &message);
    void switchToGreeter();
    void userActivateMessage(const QString &user);
    void socketDisconnected();
private slots:
    void readyRead();

private:
    bool initSocket();
private:
    QLocalSocket *m_socket{ nullptr };
    QString m_hostName;
    int m_capabilities;
};
