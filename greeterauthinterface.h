/***************************************************************************
 * Copyright (c) 2013 Abdurrahman AVCI <abdurrahmanavci@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 ***************************************************************************/

#pragma once

#include <QObject>

class GreeterAuthInterface : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(GreeterAuthInterface)

public:
    explicit GreeterAuthInterface(QObject *parent = nullptr);

    QString hostName() const { return m_hostName; }
    bool canPowerOff() const { return m_canPowerOff; }
    bool canReboot() const  { return m_canReboot; }
    bool canSuspend() const { return m_canSuspend; }
    bool canHibernate() const { return m_canHibernate; }
    bool canHybridSleep() const { return m_canHybridSleep; }

public Q_SLOTS:
    virtual void powerOff() {}
    virtual void reboot() {}
    virtual void suspend() {}
    virtual void hibernate() {}
    virtual void hybridSleep() {}

    virtual void login(const QString &user, const QString &password, const QVariantMap &session) const;
    virtual void activateUser(const QString &user);
    virtual void unlock(const QString &user, const QString &password);

Q_SIGNALS:
    void hostNameChanged(const QString &hostName);
    void canPowerOffChanged(bool canPowerOff);
    void canRebootChanged(bool canReboot);
    void canSuspendChanged(bool canSuspend);
    void canHibernateChanged(bool canHibernate);
    void canHybridSleepChanged(bool canHybridSleep);

    void loginFailed(const QString &user);
    void loginSucceeded(const QString &user);

protected:
    QString m_hostName;
    bool m_canPowerOff{ false };
    bool m_canReboot{ false };
    bool m_canSuspend{ false };
    bool m_canHibernate{ false };
    bool m_canHybridSleep{ false };
};
