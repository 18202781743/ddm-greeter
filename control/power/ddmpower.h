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

#pragma once

#include <QObject>
#include <QPointer>
#include "powerinterface.h"

class DDMPower : public PowerInterface
{
    Q_OBJECT
public:
    explicit DDMPower(QObject *parent = nullptr);
    void setImpl(QObject *impl);

    void setCapabilities(int capabilities);

public slots:
    virtual void powerOff() override;
    virtual void reboot() override;
    virtual void suspend() override;
    virtual void hibernate() override;
    virtual void hybridSleep() override;

private slots:
    void onCapabilitiesChanged(int capabilities);

private:
    void init();
private:
    QPointer<QObject> m_impl;
};
