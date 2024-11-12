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

    virtual bool init() override;

    SessionModel *sessionModel() const;

signals:
    void sessionModelChanged();
private:
    SessionModel *m_sessionModel{ nullptr };
};
