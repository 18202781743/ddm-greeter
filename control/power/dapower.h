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
#include "powerinterface.h"
#include "sessionmanager_interface.h"

using SessionManagerInter = org::deepin::dde::SessionManager1;
class DAPower : public PowerInterface
{
    Q_OBJECT
public:
    explicit DAPower(QObject *parent = nullptr);

public Q_SLOTS:
    virtual void powerOff() override;
    virtual void reboot() override;
    virtual void suspend() override;
    virtual void hibernate() override;
    virtual void hybridSleep() override;

private:
    void init();
private:
    SessionManagerInter *m_inter{ nullptr };
};
