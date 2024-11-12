// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <applet.h>
#include "sessionmodel.h"

DS_USE_NAMESPACE

class SessionApplet : public DApplet
{
    Q_OBJECT
    Q_PROPERTY(SessionModel* sessionModel READ sessionModel NOTIFY sessionModelChanged FINAL)
public:
    explicit SessionApplet(QObject *parent = nullptr);
    ~SessionApplet();

    virtual bool load() override;

    SessionModel *sessionModel() const;

    Q_INVOKABLE QVariantMap currentSession() const;

signals:
    void sessionModelChanged();
    void currentSessionChanged();
private:
    SessionModel *m_sessionModel{ nullptr };
};
