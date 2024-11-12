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

#include <QPointer>

class DeepinAuthFramework;
class DAAuth : public AuthInterface {
    Q_OBJECT
public:
    explicit DAAuth(QObject *parent = nullptr);

    void setImpl(DeepinAuthFramework *impl);

    virtual void create();
    virtual void start();
    virtual void send(const QString &token);
    virtual void end();
    virtual void destroy();

private slots:
    void onAuthStateChanged(const int type, const int state, const QString &message);
    void onLimitedInfoChanged(const QString &user);
    void updateLimitedInfo();

private:
    QPointer<DeepinAuthFramework> m_impl;
};
