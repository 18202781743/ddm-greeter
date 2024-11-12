/***************************************************************************
 * Copyright (c) 2013 Abdurrahman AVCI <abdurrahmanavci@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software FounDDMtion; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the
 * Free Software FounDDMtion, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 ***************************************************************************/

#include "ddmpower.h"

#include <Messages.h>

using namespace DDM;

DDMPower::DDMPower(QObject *parent)
    : PowerInterface(parent)
{
}

void DDMPower::setImpl(QObject *impl)
{
    m_impl = impl;

    connect(m_impl, SIGNAL(capabilitiesChanged(int)), this, SLOT(onCapabilitiesChanged(int)));

    int capabilities = 0;
    QMetaObject::invokeMethod(m_impl, "capabilities", Qt::DirectConnection, Q_RETURN_ARG(int, capabilities));
    setCapabilities(capabilities);
}

void DDMPower::setCapabilities(int capabilities)
{
    // parse capabilities
    m_canPowerOff = capabilities & Capability::PowerOff;
    m_canReboot = capabilities & Capability::Reboot;
    m_canSuspend = capabilities & Capability::Suspend;
    m_canHibernate = capabilities & Capability::Hibernate;
    m_canHybridSleep = capabilities & Capability::HybridSleep;

    // emit signals
    Q_EMIT canPowerOffChanged(m_canPowerOff);
    Q_EMIT canRebootChanged(m_canReboot);
    Q_EMIT canSuspendChanged(m_canSuspend);
    Q_EMIT canHibernateChanged(m_canHibernate);
    Q_EMIT canHybridSleepChanged(m_canHybridSleep);
}

void DDMPower::powerOff()
{
    QMetaObject::invokeMethod(m_impl, "powerOff", Qt::DirectConnection);
}

void DDMPower::reboot()
{
    QMetaObject::invokeMethod(m_impl, "reboot", Qt::DirectConnection);
}

void DDMPower::suspend()
{
    QMetaObject::invokeMethod(m_impl, "suspend", Qt::DirectConnection);
}

void DDMPower::hibernate()
{
    QMetaObject::invokeMethod(m_impl, "hibernate", Qt::DirectConnection);
}

void DDMPower::hybridSleep()
{
    QMetaObject::invokeMethod(m_impl, "hybridSleep", Qt::DirectConnection);
}

void DDMPower::onCapabilitiesChanged(int capabilities)
{
    setCapabilities(capabilities);
}
