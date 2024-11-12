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

#include "authinterface.h"

#include <QObject>

class AuthManagerPrivate;
class AuthManager : public QObject
{
    Q_OBJECT
public:
    static AuthManager *instance();
    void setAuthFrame(AuthInterface::AuthFrameType type);

    void setDDMAuthImpl(QObject *impl);
    AuthInterface *create(AuthInterface::AuthType type);

    AuthInterface *get(AuthInterface::AuthType type) const;

    AuthInterface::AuthType supportedAuthType() const;
    void setUser(const QString &user);
    void setSession(const QVariantMap &session);
    QVariantMap session() const;

    void requestCreate();
    void requestStart();
    void requestDestroy();

signals:
    void authSuccessed();
    void authFailed();

private slots:
    void handleAuthStateChanged(AuthInterface::AuthType type);
private:
    explicit AuthManager();

private:
    AuthManagerPrivate *d{ nullptr };
};
