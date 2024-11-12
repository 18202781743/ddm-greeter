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

#include "dapower.h"

DAPower::DAPower(QObject *parent)
    : PowerInterface(parent)
    , m_inter(new SessionManagerInter("org.deepin.dde.SessionManager1", "/org/deepin/dde/SessionManager1", QDBusConnection::sessionBus(), this))
{
    init();
}

void DAPower::powerOff()
{
    m_inter->Shutdown();
}

void DAPower::reboot()
{
    m_inter->RequestReboot();
}

void DAPower::suspend()
{
    m_inter->RequestSuspend();
}

void DAPower::hibernate()
{
    m_inter->RequestHibernate();
}

void DAPower::hybridSleep()
{
    m_inter->RequestHibernateQueued();
}

void DAPower::init()
{
    m_canPowerOff = m_inter->CanShutdown();
    m_canReboot = m_inter->CanReboot();
    m_canSuspend = m_inter->CanSuspend();
    m_canHibernate = m_inter->CanHibernate();
}
