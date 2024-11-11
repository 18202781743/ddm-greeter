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

#include "authinterface.h"

AuthInterface::AuthInterface(QObject *parent)
    : QObject(parent)
{
}

AuthInterface::AuthStage AuthInterface::stage() const
{
    return m_stage;
}

void AuthInterface::setUser(const QString &user)
{
    m_user = user;
}

QString AuthInterface::prompt() const
{
    return m_prompt;
}

void AuthInterface::setPrompt(const QString &newPrompt)
{
    if (m_prompt == newPrompt)
        return;
    m_prompt = newPrompt;
    emit promptChanged();
}

void AuthInterface::setStage(AuthStage newStage)
{
    if (m_stage == newStage)
        return;
    m_stage = newStage;
    emit stageChanged();
}

AuthInterface::AuthState AuthInterface::state() const
{
    return m_state;
}

void AuthInterface::setState(AuthState newState)
{
    if (m_state == newState)
        return;
    m_state = newState;
    emit stateChanged();
}
