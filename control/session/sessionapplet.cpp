// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "sessionapplet.h"

#include <QLoggingCategory>

#include <appletbridge.h>
#include <pluginfactory.h>

using namespace DDM;

SessionApplet::SessionApplet(QObject *parent)
    : DApplet(parent)
{
}

bool SessionApplet::load()
{
    m_sessionModel = new SessionModel(this);
    emit sessionModelChanged();

    connect(m_sessionModel, &SessionModel::lastIndexChanged, this, [this] () {
        qDebug() << "Current session changed";
        emit currentSessionChanged();
    });

    return DApplet::load();
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

QVariantMap SessionApplet::currentSession() const
{
    if (!m_sessionModel) {
        qCritical() << "Session model is not set.";
        return {};
    }

    const auto sessionIndex = m_sessionModel->lastIndex();
    // get model index
    QModelIndex index = m_sessionModel->index(sessionIndex, 0);

    // send command to the daemon
    Session::Type type =
        static_cast<Session::Type>(m_sessionModel->data(index, SessionModel::TypeRole).toInt());
    QString name = m_sessionModel->data(index, SessionModel::FileRole).toString();

    QVariantMap session;
    session["type"] = type;
    session["name"] = name;
    return session;
}

D_APPLET_CLASS(SessionApplet)

#include "sessionapplet.moc"
