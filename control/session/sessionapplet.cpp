// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "sessionapplet.h"

#include <QLoggingCategory>

#include <appletbridge.h>
#include <pluginfactory.h>

SessionApplet::SessionApplet(QObject *parent)
    : DApplet(parent)
{
}

bool SessionApplet::init()
{
    m_sessionModel = new SessionModel(this);
    emit sessionModelChanged();

    return DApplet::init();
}

SessionApplet::~SessionApplet()
{
    if (m_sessionModel)
        m_sessionModel->deleteLater();
}

SessionModel *SessionApplet::sessionModel() const
{
    return m_sessionModel;
}

D_APPLET_CLASS(SessionApplet)

#include "sessionapplet.moc"
