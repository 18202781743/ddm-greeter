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

#include "daauth.h"

#include "deepinauthframework.h"

DAAuth::DAAuth(QObject *parent)
    : AuthInterface(parent)
{
}

void DAAuth::setImpl(DeepinAuthFramework *impl)
{
    m_impl = impl;

    connect(m_impl, &DeepinAuthFramework::AuthStateChanged, this, &DAAuth::onAuthStateChanged);
    connect(m_impl, &DeepinAuthFramework::LimitsInfoChanged, this, &DAAuth::onLimitedInfoChanged);
}

void DAAuth::create()
{
    qDebug() << "Auth create" << m_user;
    m_impl->CreateAuthController(m_user, m_impl->GetSupportedMixAuthFlags(), 2);
}

void DAAuth::start()
{
    qDebug() << "Auth start" << m_user;
    m_impl->StartAuthentication(m_user, m_type, -1);
}

void DAAuth::send(const QString &token)
{
    qDebug() << "Auth send";
    m_impl->SendTokenToAuth(m_user, m_type, token);
}

void DAAuth::end()
{
    m_impl->EndAuthentication(m_user, m_type);
}

void DAAuth::destroy()
{
    m_impl->DestroyAuthenticate();
}

void DAAuth::onAuthStateChanged(const int type, const int state, const QString &message)
{
    if (!(type & m_type))
        return;

    qDebug() << "AuthState changed, state: " << state << ", message:" << message;
    updateLimitedInfo();
    setState(static_cast<AuthInterface::AuthState>(state));
    switch (state) {
    case AS_Success: {
    } break;
    case AS_Failure: {
        // 单因失败会返回明确的失败类型，不关注type为-1的情况
        if (AT_All != type) {
            end();
            if (!m_limits.locked) {
                QTimer::singleShot(50, this, [this, type] {
                    start();
                });
            }
        }
    } break;
    case AS_Cancel: {
        destroy();
    } break;
    case AS_Recover: {
        create();
    } break;
    default:
        break;
    }
}

void DAAuth::onLimitedInfoChanged(const QString &user)
{
    if (user != m_user)
        return;
    updateLimitedInfo();
}

void DAAuth::updateLimitedInfo()
{
    qDebug() << "Update limits info";
    const auto info = m_impl->GetLimitedInfo(m_user);
    const QJsonDocument limitsInfoDoc = QJsonDocument::fromJson(info.toUtf8());
    const QJsonArray limitsInfoArr = limitsInfoDoc.array();
    for (const QJsonValue &limitsInfoStr : limitsInfoArr) {
        const QJsonObject limitsInfoObj = limitsInfoStr.toObject();
        const auto flags = limitsInfoObj["flag"].toInt();
        if (flags != m_type)
            continue;

        LimitsInfo info;
        info.unlockSecs = limitsInfoObj["unlockSecs"].toVariant().toUInt();
        info.maxTries = limitsInfoObj["maxTries"].toVariant().toUInt();
        info.numFailures = limitsInfoObj["numFailures"].toVariant().toUInt();
        info.locked = limitsInfoObj["locked"].toBool();
        info.unlockTime = limitsInfoObj["unlockTime"].toString();
        m_limits = info;
    }
}
